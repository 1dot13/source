# Crash telemetry sink

Receives the crash reports `sgp::processCrashTelemetry` uploads and posts them to a
Discord channel. Stores nothing — a report is only useful next to the matching PDB,
which lives on a developer's machine.

## Deploy

```sh
npm i -g wrangler                     # once
wrangler login                        # once
wrangler deploy                       # creates the Worker
wrangler secret put DISCORD_WEBHOOK   # paste the channel's webhook URL
```

Deploy first: `secret put` needs the Worker to already exist. To point it at a
different channel later, run `secret put` again — it overwrites in place and
redeploys, no other step needed. Secrets are write-only; `wrangler secret list`
shows names, never values.

Then put the printed `https://ja2-crash-telemetry.<subdomain>.workers.dev` into
`gamedir/Ja2.ini`:

```ini
[Ja2 Settings]
CRASH_TELEMETRY_URL = https://ja2-crash-telemetry.<subdomain>.workers.dev
```

Empty or missing key = telemetry off, reports just accumulate locally.

## Test without the game

```sh
node test.mjs             # status-code contract, Discord payload shape. No network.
```

Against a real local instance:

```sh
cp .dev.vars.example .dev.vars   # put a webhook URL in it, see below
wrangler dev                     # local, http://localhost:8787

printf '*** CRASH  code=C0000005 ***\r\n  build test\r\n' \
  | curl -X POST --data-binary @- localhost:8787
```

Expect `204` for that, `400` for junk (`curl -X POST -d hello localhost:8787`). Any
real `crash_report_*.txt` out of a gamedir works as a body too.

**`wrangler dev` does not see `wrangler secret put`.** Secrets set that way go to the
deployed Worker; local dev reads `.dev.vars` and nothing else. Without it every
request answers `503 not configured`. Each 503 names its own cause in the response
body and in wrangler's console.

There is no stub webhook value that returns 2xx — a Discord webhook URL you do not
own answers 401 or 404, which the Worker correctly turns into a 503. So `.dev.vars`
needs a real webhook; point it at a throwaway channel rather than the live one.
`node test.mjs` is the path that needs no webhook at all.

## Status codes are a contract

The client (`reportIsSettled()` in `sgp/crash_telemetry.cpp`) deletes its copy of a
report on 2xx and on 400/413/415, and keeps it on everything else. So:

| Status | Meaning | Client does |
| --- | --- | --- |
| 2xx | delivered | deletes its copy |
| 400 | not a crash report, or too big | deletes its copy |
| 429 | throttled | keeps it, retries next launch |
| 5xx | our fault (Discord down, rate-limited, secret unset) | keeps it, retries next launch |

**Never answer a settling 4xx for a failure on our side** — that silently destroys
the report. 429 is the one 4xx that is safe here, precisely because the client does
not settle it.

## Rate limiting

A per-IP cap, via the `UPLOAD_LIMITER` binding in `wrangler.toml`, checked before
the body is read. The ceiling (50/minute) has to clear `kMaxUploadsPerRun` (20) in
the client, or a player draining a backlog throttles themselves.

This has to be a binding with a `.limit()` call in `worker.js`, not a dashboard
rule: WAF rate limiting rules need a zone, and a `workers.dev` subdomain is not
one. The counter is per-colo and best-effort, so treat the number as a rough
ceiling — it is there to keep scanners out of the channel, not to be exact.

Adding the binding from the Cloudflare dashboard instead would leave `wrangler.toml`
out of sync, and the next `wrangler deploy` would drop it. Edit the file.

## Free tier

100k requests/day and 10 ms CPU per invocation. Forwarding a few KB costs well under
a millisecond of CPU — the time spent waiting on Discord is not metered.

## Not done

No authentication. The endpoint is public and its URL ships in every player's
`Ja2.ini`, so assume it will eventually be found; the size and `*** CRASH` checks
only keep out drive-by scanners. Report contents are attacker-controlled text, which
is why the summary line strips markdown from the handle and sends
`allowed_mentions: {parse: []}`. Blast radius of abuse is a message we delete.
