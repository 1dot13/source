// node test.mjs — the report grammar and the views that read it, then the
// status-code contract the client depends on.
import assert from "node:assert";
import { createHash } from "node:crypto";
import { readFileSync } from "node:fs";
import { DatabaseSync } from "node:sqlite";
import { INSERT, parseReport, reportId, toRow } from "./parse.js";
import worker from "./worker.js";

// ---- the grammar, against what the client really sends --------------------

// Captured, not written by hand: reports the crash handler wrote in a console
// harness under Wine, as sgp/crash_telemetry.cpp prepared them for upload.
const fixture = (name) => readFileSync(new URL(`fixtures/${name}`, import.meta.url), "utf8");
const FAULT = fixture("fault.txt");                        // named, one frame inlined
// As a writer from before the image line wrote it.
const OLDER = FAULT.replace(/  image [0-9A-F]{8}\r\n/, "");

for (const text of [FAULT, OLDER])
  assert.deepEqual(parseReport(text).unparsed, []);

const fault = parseReport(FAULT);
assert.equal(fault.code, "C0000005");
assert.equal(fault.image, 0x6AB2DD5A);
assert.equal(fault.av_kind, "read");
assert.deepEqual(fault.frames[0], { a: 0x004027E7, s: "`anonymous namespace'::walk  harness.cpp:16" });
assert.deepEqual(fault.frames[3], {
  a: 0x00409941,
  s: "invoke_main  exe_common.inl:78",
  i: ["__scrt_common_main_seh  exe_common.inl:288"],
});
assert.deepEqual(fault.frames[4], { a: 0x7BABFC44 });      // outside the image: never named

assert.equal(parseReport(OLDER).image, null);

// A frame out of sequence would shift every frame after it, and a folded level
// with no frame above it belongs nowhere: both are kept as text, not guessed at.
assert.deepEqual(parseReport("  [1] 00401000\r\n").unparsed, ["  [1] 00401000"]);
assert.deepEqual(parseReport("      inlined into f  x.cpp:1\r\n").unparsed,
  ["      inlined into f  x.cpp:1"]);

// ---- the schema ------------------------------------------------------------

const db = new DatabaseSync(":memory:");
db.exec(readFileSync(new URL("schema.sql", import.meta.url), "utf8"));
const one = (sql, ...v) => db.prepare(sql).get(...v);
const store = async (text) => db.prepare(INSERT).run(...toRow(text, await reportId(text), 0));

const UNKNOWN = FAULT.replace("  modules (", "  something new\r\n  modules (");
for (const text of [FAULT, OLDER, UNKNOWN]) await store(text);
await store(FAULT);
assert.equal(one("SELECT count(*) n FROM reports").n, 3);

// A row renders back into the exact bytes that were uploaded -- the id is their
// hash, so that is checked twice over. A report with a line the grammar does
// not know comes back from `raw`, whole.
for (const text of [FAULT, OLDER, UNKNOWN]) {
  const id = await reportId(text);
  assert.equal(one("SELECT text FROM report_text WHERE id = ?", id).text, text);
  assert.equal(createHash("sha256").update(text).digest("hex"), id);
}
assert.equal(one("SELECT count(*) n FROM reports WHERE raw IS NOT NULL").n, 1);

const frames = db.prepare("SELECT idx, func, source, name, rva FROM report_frames WHERE id = ? ORDER BY idx")
  .all(await reportId(FAULT));
assert.deepEqual(frames[0], { idx: 0, func: "`anonymous namespace'::walk", source: "harness.cpp:16",
  name: "harness.exe", rva: 0x27E7 });
assert.deepEqual(frames[3], { idx: 3, func: "invoke_main", source: "exe_common.inl:78",
  name: "harness.exe", rva: 0x9941 });
assert.deepEqual(frames[4], { idx: 4, func: null, source: null, name: "kernel32.dll", rva: 0xFC44 });
assert.equal(frames[7].name, null);                          // 00000000: in no image at all

// A report's site is the first function of ours on its stack, and its fault is
// said in words; none of the fixtures is a CRASH_TEST.
const id = await reportId(FAULT);
assert.equal(one("SELECT site FROM report_site WHERE id = ?", id).site,
  "`anonymous namespace'::walk (harness.cpp:16)");
assert.match(one("SELECT fault FROM report_fault WHERE id = ?", id).fault,
  /^access violation, reading 0x[0-9A-F]{8}.* in `anonymous namespace'::walk \(harness\.cpp:16\)$/);
assert.deepEqual(db.prepare("SELECT func FROM report_calls WHERE id = ? ORDER BY pos").all(id)
  .slice(3, 5).map((c) => c.func), ["invoke_main", "__scrt_common_main_seh"]);
assert.equal(one("SELECT count(*) n FROM test_reports").n, 0);

// The views for people: the stack one call per line with the inlined level on a
// line of its own, and a release found by SHA prefix.
db.exec("INSERT INTO releases VALUES ('v0', 'abcdef1230000000000000000000000000000000', '2026-01-01')");
db.exec(`UPDATE reports SET build = 'abcdef123' WHERE id = '${id}'`);
const human = one("SELECT stack, release, latest_release FROM crash_reports WHERE id = ?", id);
assert.deepEqual(human.stack.split("\n").slice(3, 5),
  ["invoke_main  exe_common.inl:78", "__scrt_common_main_seh  exe_common.inl:288"]);
assert.equal(human.release, "v0");
assert.equal(human.latest_release, 1);
assert.equal(one("SELECT count(*) n FROM crashes").n, 1);   // three reports, one crash
db.exec("DELETE FROM releases");

// ---- the Worker and the status-code contract ------------------------------

// The Worker's view of D1 -- prepare, bind, run -- over the same in-memory
// database, so a statement D1 would reject is rejected here too.
let throttled = false;
const ENV = {
  DB: { prepare: (sql) => ({ bind: (...v) => ({ run: async () => db.prepare(sql).run(...v) }) }) },
  UPLOAD_LIMITER: { limit: async () => ({ success: !throttled }) },
};
const post = (body, env = ENV) =>
  worker.fetch(new Request("https://x/", { method: "POST", body }), env);
const rows = () => one("SELECT count(*) n FROM reports").n;

db.exec("DELETE FROM reports");
assert.equal((await post(FAULT)).status, 204);
assert.equal(one("SELECT text FROM report_text").text, FAULT);
// Sent again, because the client reports a timeout as unsettled even when we
// committed: still one row.
assert.equal((await post(FAULT)).status, 204);
assert.equal(rows(), 1);

// Junk: the client deletes these, so they must be 400.
assert.equal((await post("hello")).status, 400);
assert.equal((await post("*** CRASH " + "x".repeat(32 * 1024))).status, 400);
assert.equal((await worker.fetch(new Request("https://x/"), ENV)).status, 405);
// An assertion from an older client: 400, so it is deleted rather than resent.
assert.equal((await post(FAULT.replace("code=C0000005", "code=E1A55E27"))).status, 400);
assert.equal(rows(), 1);

// Throttled: 429, and nothing stored. reportIsSettled() leaves 429 unsettled, so
// the client keeps the report for next launch.
throttled = true;
assert.equal((await post(FAULT)).status, 429);
throttled = false;

// Our failures must be 5xx, or the client throws away a report we never stored.
const broken = { ...ENV, DB: { prepare: () => { throw new Error("D1_ERROR"); } } };
assert.equal((await post(OLDER, broken)).status, 503);
assert.equal((await post(OLDER, { ...ENV, DB: undefined })).status, 503);
assert.equal(rows(), 1);

console.log("ok");
