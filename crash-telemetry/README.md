# Crash telemetry sink

Receives the crash reports `sgp::processCrashTelemetry` uploads and stores each as a
row in D1. `schema.sql` is the data model, `parse.js` the report grammar, and the
Worker the little that is left.

Reports arrive with their frames named. `sgp/crash_telemetry.cpp` resolves them
against the PDB shipped beside the exe before uploading, and holds back a fault it
cannot name — so a frame here carries where the code was and each function it was
inlined into, not a bare address. Nothing in this directory needs a PDB, and
nothing here can recover a name the client did not send.

## Deploy

```sh
npm i -g wrangler                                            # once
wrangler login                                               # once
wrangler d1 create ja2-crashes                               # once
wrangler d1 execute ja2-crashes --remote --file=schema.sql
wrangler deploy
```

`wrangler.toml` names the database and holds no id, so nothing in it is tied to
one Cloudflare account: wrangler finds `ja2-crashes` in whichever account it is
logged into. That needs a wrangler that resolves D1 bindings by name — 4.124 does.

Then put the printed `https://ja2-crash-reports.<subdomain>.workers.dev` into
`gamedir/Ja2.ini`:

```ini
[Ja2 Settings]
CRASH_TELEMETRY_URL = https://ja2-crash-reports.<subdomain>.workers.dev
```

Empty or missing key = telemetry off, reports just accumulate locally.

Re-running the schema is safe — every statement in it is `IF NOT EXISTS`. Changing
a view means dropping it first (`DROP VIEW report_frames;`), then re-running the
file; the view then re-derives every report ever received.

## Mining it

```sh
d1() { wrangler d1 execute ja2-crashes --remote --command "$1"; }

# where faults happen, by the code the client said the address was in
d1 "SELECT f.source, count(*) n, count(DISTINCT r.build) builds
      FROM reports r JOIN report_frames f ON f.id = r.id AND f.idx = 0
     WHERE f.source IS NOT NULL GROUP BY 1 ORDER BY n DESC LIMIT 20"

# reports per day
d1 "SELECT date(received_at, 'unixepoch') d, count(*) n FROM reports GROUP BY d ORDER BY d DESC LIMIT 30"

# what else was loaded — cnc-ddraw, an overlay, Wine
d1 "SELECT name, count(*) n FROM report_modules GROUP BY name ORDER BY n DESC"

# one report's backtrace, then what each of its frames was inlined into
d1 "SELECT idx, func, source, name FROM report_frames WHERE id = '<id>' ORDER BY idx"
d1 "SELECT idx, depth, func, source FROM report_inlines WHERE id = '<id>' ORDER BY idx, depth"

# reports the grammar did not fully understand — should be none
d1 "SELECT id, received_at, build FROM reports WHERE raw IS NOT NULL"
```

To read a row as the report the player's machine sent:

```sh
wrangler d1 execute ja2-crashes --remote --json \
  --command "SELECT text FROM report_text WHERE id = '<id>'" \
  | jq -j '.[0].results[0].text' > /tmp/report.txt
```

The names in it are the only ones there will be: nothing here has the PDB to
name a frame again.

## Test without the game

```sh
node test.mjs      # grammar, views, status-code contract. No network, no wrangler.
```

`test.mjs` runs `schema.sql` in node's SQLite and hands the Worker a binding backed
by it. Its fixtures are real uploads, captured from the client under Wine, not
reports written to match the grammar.

Against a local instance:

```sh
wrangler d1 execute ja2-crashes --local --file=schema.sql   # once
wrangler dev                                                # http://localhost:8787
curl -X POST --data-binary @fixtures/fault.txt localhost:8787
wrangler d1 execute ja2-crashes --local --command "SELECT id, code, build FROM reports"
```

Expect `204` for a report and `400` for junk (`curl -X POST -d hello localhost:8787`).
`--local` and `--remote` are different databases.

## Status codes are a contract

The client (`reportIsSettled()` in `sgp/crash_telemetry.cpp`) deletes its copy of a
report on 2xx and on 400/413/415, and keeps it on everything else. So:

| Status | Meaning | Client does |
| --- | --- | --- |
| 2xx | stored | deletes its copy |
| 400 | not a crash report, or too big | deletes its copy |
| 429 | throttled | keeps it, retries next launch |
| 5xx | our fault (D1 unreachable, binding missing) | keeps it, retries next launch |

**Never answer a settling 4xx for a failure on our side** — that silently destroys
the report. 429 is the one 4xx that is safe here, precisely because the client does
not settle it.

The same report sent twice is one row: the primary key is the hash of its text, and
a request that times out after we committed is one the client sends again.

## Rate limiting

A per-IP cap, via the `UPLOAD_LIMITER` binding in `wrangler.toml`, checked before
the body is read. The ceiling (50/minute) has to clear `kMaxUploadsPerRun` (20) in
the client, or a player draining a backlog throttles themselves.

This has to be a binding with a `.limit()` call in `worker.js`, not a dashboard
rule: WAF rate limiting rules need a zone, and a `workers.dev` subdomain is not
one. The counter is per-colo and best-effort, so treat the number as a rough
ceiling.

Adding the binding from the Cloudflare dashboard instead would leave `wrangler.toml`
out of sync, and the next `wrangler deploy` would drop it. Edit the file.

## Free tier

100k Worker requests/day; for D1, 5 GB stored, 5M rows read/day and 100k rows
written/day, index entries counted as rows written — which is why the schema has no
secondary index. A report is one row of 2-8 KB.

Nothing deletes anything. When that has to change:

```sh
wrangler d1 execute ja2-crashes --remote \
  --command "DELETE FROM reports WHERE received_at < unixepoch('now', '-1 year')"
```

## Not done

No authentication. The endpoint is public and its URL ships in every player's
`Ja2.ini`, so assume it will eventually be found; the size and `*** CRASH` checks
only keep out drive-by scanners. Every field in a row is therefore attacker-chosen:
bound, never interpolated, and never read back as anything but text. Blast radius
of abuse is rows we delete.

The per-IP limiter does nothing against a distributed flood — that would spend the
D1 write budget and the 100k/day request budget, not money.

Nothing pushes. A crash never seen before does not announce itself; you go and look.
