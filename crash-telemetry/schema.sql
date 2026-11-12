-- D1 schema for the crash reports sgp::processCrashTelemetry uploads.
--
--   wrangler d1 execute ja2-crashes --remote --file=schema.sql
--
-- One row per report, holding what the report said. Which module a frame lands
-- in is a view, so redefining it re-derives every report ever received.
--
-- The frame names are the one thing here that was derived before it arrived:
-- the client resolved them against the PDB beside its exe, and nothing
-- downstream can resolve them again.

-- Three layers, each built only from the ones before it:
--   1. tables: what was received, and the releases we tag by hand. The truth;
--      not meant to be read by people.
--   2. report_* views: building blocks. One fact each, joined by report id.
--   3. crashes and crash_reports: what a person opens first.

-- ==== 1. tables ==============================================================

CREATE TABLE IF NOT EXISTS reports (
  id          TEXT PRIMARY KEY,  -- sha-256 of the uploaded text
  received_at INTEGER NOT NULL,  -- our clock, epoch seconds
  reported_at TEXT,              -- the report's own "time" line, UTC; the player's clock
  build       TEXT,              -- names the commit
  image       INTEGER,           -- the exe's TimeDateStamp: names the binary
  code        TEXT,              -- C0000005, ...
  esp         INTEGER,
  ebp         INTEGER,           -- eip is frame 0
  av_kind     TEXT,              -- read | write | execute
  av_addr     INTEGER,
  handle      TEXT,              -- the player's optional self-chosen HANDLE
  modules     TEXT NOT NULL,     -- JSON [{n,b,s}] in load order; b is per run (ASLR)
  frames      TEXT NOT NULL,     -- JSON [{a,s?,i?}], index = frame number: a the
                                 -- address, s where the code is ("func  file:line"),
                                 -- i each function that was inlined into, outward
  raw         TEXT               -- the whole text, only when a line did not parse
);

-- No secondary indexes: D1 bills index entries as rows written, and a table
-- this size scans fast. Add one when a query is actually slow.

-- Releases, one row per tag, added by hand when a tag ships. A report's build
-- is the commit's short SHA, so the tag a report belongs to is a join.
CREATE TABLE IF NOT EXISTS releases (
  tag         TEXT PRIMARY KEY,  -- v6
  sha         TEXT NOT NULL,     -- the full commit SHA the tag points at
  released_at TEXT NOT NULL      -- YYYY-MM-DD
);

-- ==== 2. building blocks =====================================================

-- name is lowercased: the loader reports whatever case the path was written in,
-- and one binary arriving as ja2.exe and JA2.exe should not split in two.
CREATE VIEW IF NOT EXISTS report_modules AS
SELECT r.id,
       m.key                               AS idx,
       lower(json_extract(m.value, '$.n')) AS name,
       json_extract(m.value, '$.b')        AS base,
       json_extract(m.value, '$.s')        AS size
  FROM reports r, json_each(r.modules) m;

-- Every frame the stack walk printed, placed in the image it lands in. func and
-- source are NULL where the client named nothing; name and rva are NULL where
-- the address lands in no image, which on a wrecked stack is most of them.
CREATE VIEW IF NOT EXISTS report_frames AS
SELECT r.id,
       f.key                                    AS idx,
       json_extract(f.value, '$.a')             AS addr,
       CASE WHEN instr(json_extract(f.value, '$.s'), '  ') > 0
            THEN substr(json_extract(f.value, '$.s'), 1, instr(json_extract(f.value, '$.s'), '  ') - 1)
            ELSE json_extract(f.value, '$.s') END AS func,
       CASE WHEN instr(json_extract(f.value, '$.s'), '  ') > 0
            THEN substr(json_extract(f.value, '$.s'), instr(json_extract(f.value, '$.s'), '  ') + 2)
            END                                 AS source,
       m.name                                   AS name,
       json_extract(f.value, '$.a') - m.base    AS rva
  FROM reports r, json_each(r.frames) f
  LEFT JOIN report_modules m
    ON m.id = r.id AND json_extract(f.value, '$.a') >= m.base
   AND json_extract(f.value, '$.a') < m.base + m.size;

-- A row back in the exact bytes that were uploaded:
--   wrangler d1 execute ja2-crashes --remote --json \
--     --command "SELECT text FROM report_text WHERE id = '<id>'" \
--     | jq -j '.[0].results[0].text' > /tmp/report.txt
CREATE VIEW IF NOT EXISTS report_text AS
WITH crlf(nl) AS (VALUES (char(13) || char(10)))
SELECT r.id,
       COALESCE(r.raw,
         nl
         || printf('*** CRASH  code=%s  eip=%08X  esp=%08X  ebp=%08X ***',
                   r.code, json_extract(r.frames, '$[0].a'), r.esp, r.ebp) || nl
         || CASE WHEN r.reported_at IS NULL THEN '' ELSE printf('  time %s UTC', r.reported_at) || nl END
         || CASE WHEN r.build IS NULL THEN '' ELSE printf('  build %s', r.build) || nl END
         || CASE WHEN r.image IS NULL THEN '' ELSE printf('  image %08X', r.image) || nl END
         || CASE WHEN r.handle IS NULL THEN '' ELSE printf('  handle %s', r.handle) || nl END
         -- "write to " carries the writer's own column alignment
         || CASE r.av_kind WHEN 'read'    THEN printf('  access violation: read from %08X', r.av_addr) || nl
                           WHEN 'write'   THEN printf('  access violation: write to  %08X', r.av_addr) || nl
                           WHEN 'execute' THEN printf('  access violation: execute at %08X', r.av_addr) || nl
                           ELSE '' END
         || '  modules (base size name):' || nl
         || COALESCE((SELECT group_concat(printf('    %08X %08X %s', json_extract(m.value, '$.b'),
                                                 json_extract(m.value, '$.s'), json_extract(m.value, '$.n')) || nl,
                                          '' ORDER BY m.key)
                        FROM json_each(r.modules) m), '')
         || COALESCE((SELECT group_concat(
                               printf('  [%d] %08X', f.key, json_extract(f.value, '$.a'))
                               || COALESCE(' ' || json_extract(f.value, '$.s'), '') || nl
                               || COALESCE((SELECT group_concat('      inlined into ' || i.value || nl, '' ORDER BY i.key)
                                              FROM json_each(f.value, '$.i') i), ''),
                               '' ORDER BY f.key)
                        FROM json_each(r.frames) f), '')) AS text
  FROM reports r, crlf;

-- Which release each report's build is, if any. A build is a SHA prefix.
CREATE VIEW IF NOT EXISTS report_release AS
SELECT r.id, rel.tag, rel.released_at,
       rel.tag = (SELECT tag FROM releases ORDER BY released_at DESC, tag DESC LIMIT 1) AS is_latest
  FROM reports r
  JOIN releases rel ON length(r.build) >= 7 AND substr(rel.sha, 1, length(r.build)) = r.build;

-- Every function on a report's stack, innermost first: a frame's own function,
-- then each one it was inlined into. Unnamed frames keep their module.
CREATE VIEW IF NOT EXISTS report_calls AS
SELECT r.id,
       f.key * 100                               AS pos,
       f.key                                     AS frame,
       rf.func, rf.source, rf.name               AS module
  FROM reports r, json_each(r.frames) f
  JOIN report_frames rf ON rf.id = r.id AND rf.idx = f.key
 WHERE rf.addr <> 0
UNION ALL
SELECT r.id, f.key * 100 + i.key + 1, f.key,
       substr(i.value, 1, instr(i.value, '  ') - 1),
       substr(i.value, instr(i.value, '  ') + 2),
       NULL
  FROM reports r, json_each(r.frames) f, json_each(f.value, '$.i') i;

-- Reports from CRASH_TEST in Ja2.ini: crashTest is on the stack, as a frame of
-- its own in Debug or inlined into its caller in an optimized build.
CREATE VIEW IF NOT EXISTS test_reports AS
SELECT DISTINCT id FROM report_calls WHERE func = 'crashTest';

-- What went wrong, in words.
CREATE VIEW IF NOT EXISTS report_fault AS
SELECT r.id,
       CASE r.code
         WHEN 'C0000005' THEN 'access violation, ' ||
              CASE r.av_kind WHEN 'read' THEN 'reading' WHEN 'write' THEN 'writing'
                             WHEN 'execute' THEN 'executing' ELSE 'at' END ||
              printf(' 0x%08X', r.av_addr) ||
              CASE WHEN r.av_addr < 65536 THEN ' (null pointer)' ELSE '' END
         WHEN 'C0000094' THEN 'integer divide by zero'
         WHEN 'C00000FD' THEN 'stack overflow'
         WHEN 'C0000409' THEN 'stack buffer overrun'
         WHEN 'C0000374' THEN 'heap corruption'
         WHEN 'C000001D' THEN 'illegal instruction'
         WHEN 'E06D7363' THEN 'uncaught C++ exception'
         WHEN 'E1A55E27' THEN 'assertion'
         ELSE 'exception ' || r.code END
       || ' in ' ||
       COALESCE((SELECT c.func || ' (' || c.source || ')' FROM report_calls c
                  WHERE c.id = r.id AND c.frame = 0 AND c.func IS NOT NULL ORDER BY c.pos LIMIT 1),
                (SELECT c.module FROM report_calls c WHERE c.id = r.id AND c.pos = 0),
                'unknown code') AS fault
  FROM reports r;

-- The first function from our own C++ on the stack: the vendored libraries in
-- ext/ are C, so a crash inside Lua or zlib lands on the code that called it.
CREATE VIEW IF NOT EXISTS report_site AS
SELECT r.id,
       (SELECT c.func || ' (' || c.source || ')' FROM report_calls c
         WHERE c.id = r.id AND c.source LIKE '%.cpp:%' ORDER BY c.pos LIMIT 1) AS site
  FROM reports r;

-- ==== 3. for people ===========================================================

-- One report, readable: the stack one call per line, runs of unnamed frames
-- in the same module folded into one line.
CREATE VIEW IF NOT EXISTS crash_reports AS
WITH runs AS (
  SELECT c.*,
         c.func IS NULL AND c.module IS lag(c.module) OVER w
                        AND lag(c.func) OVER w IS NULL AS cont
    FROM report_calls c
  WINDOW w AS (PARTITION BY c.id ORDER BY c.pos)),
grp AS (
  SELECT runs.*, sum(NOT cont) OVER (PARTITION BY id ORDER BY pos) AS g FROM runs),
lines AS (
  SELECT id, min(pos) AS pos,
         CASE WHEN max(func) IS NOT NULL THEN max(func) || '  ' || COALESCE(max(source), '')
              WHEN count(*) = 1 THEN COALESCE(max(module), '?')
              ELSE count(*) || ' frames in ' || COALESCE(max(module), '?') END AS line
    FROM grp GROUP BY id, g)
SELECT datetime(r.received_at, 'unixepoch')        AS received,
       r.build,
       COALESCE(rr.tag, '')                        AS release,
       COALESCE(rr.is_latest, 0)                   AS latest_release,
       COALESCE(r.handle, '')                      AS player,
       COALESCE(s.site, '')                        AS site,
       f.fault,
       (SELECT group_concat(l.line, char(10)) FROM (SELECT line FROM lines l WHERE l.id = r.id ORDER BY l.pos) l) AS stack,
       r.id IN (SELECT id FROM test_reports)       AS is_test,
       r.id
  FROM reports r
  JOIN report_fault f ON f.id = r.id
  JOIN report_site s ON s.id = r.id
  LEFT JOIN report_release rr ON rr.id = r.id;

-- Distinct crashes: those still happening on the latest release first, then
-- by how often. CRASH_TEST reports are left out.
CREATE VIEW IF NOT EXISTS crashes AS
SELECT site, fault,
       max(latest_release)                         AS in_latest_release,
       count(*)                                    AS reports,
       count(DISTINCT NULLIF(player, ''))          AS named_players,
       group_concat(DISTINCT NULLIF(release, ''))  AS releases,
       group_concat(DISTINCT build)                AS builds,
       min(received)                               AS first_seen,
       max(received)                               AS last_seen,
       max(id)                                     AS example
  FROM crash_reports
 WHERE NOT is_test
 GROUP BY site, fault
 ORDER BY in_latest_release DESC, reports DESC, last_seen DESC;
