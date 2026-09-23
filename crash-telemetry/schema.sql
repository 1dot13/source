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
