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

// ---- the Worker ------------------------------------------------------------

const REPORT = `
*** CRASH  code=C0000005  eip=0071D5A0  esp=202BF99C  ebp=202BFA18 ***
  time 2026-07-26 10:41:02 UTC
  build 6a941c06
  handle @marco*evil
  access violation: read from 00000002 [x](https://evil.test) \`
  [0] 0071D5A0
  [1] 006BE7DE
`;

let sent = null;               // what we handed Discord on the last call
let upstream = () => new Response(null, { status: 204 });
globalThis.fetch = async (url, init) => { sent = init; return upstream(); };

let throttled = false;
const ENV = {
  DISCORD_WEBHOOK: "https://discord.test/hook",
  UPLOAD_LIMITER: { limit: async () => ({ success: !throttled }) },
};

const post = (body, env = ENV) =>
  worker.fetch(new Request("https://x/", { method: "POST", body }), env);

// happy path
let r = await post(REPORT);
assert.equal(r.status, 204);
const content = JSON.parse(sent.body.get("payload_json")).content;
assert.match(content, /C0000005/);
assert.match(content, /read from 00000002/);
assert.match(content, /build `6a941c06`/);
assert.match(content, /marcoevil/);            // markdown and @ stripped from the handle
// every field is attacker-chosen: no field may carry markup or a link into the
// channel, and none may close the backticks or bold the summary wraps it in.
assert.ok(!content.includes("]("), content);   // no link syntax out of the report
assert.ok(!content.includes("://"), content);  // and no bare URL either
assert.equal(content.match(/`/g).length, 4);   // only the two pairs summarize() opens
assert.deepEqual(JSON.parse(sent.body.get("payload_json")).allowed_mentions, { parse: [] });
assert.equal(await sent.body.get("files[0]").text(), REPORT);

// junk: client should delete these, so they must be 400
assert.equal((await post("hello")).status, 400);
assert.equal((await post("x".repeat(32 * 1024 + 1))).status, 400);

// throttled: 429, and nothing reaches Discord. reportIsSettled() leaves 429
// unsettled, so the client keeps the report for next launch.
throttled = true;
sent = null;
assert.equal((await post(REPORT)).status, 429);
assert.equal(sent, null);
throttled = false;

// our failures: client must keep the report, so these must be 5xx
upstream = () => new Response(null, { status: 429 });          // Discord rate limit
assert.equal((await post(REPORT)).status, 503);
upstream = () => { throw new Error("network"); };
assert.equal((await post(REPORT)).status, 503);
upstream = () => new Response(null, { status: 204 });
assert.equal((await post(REPORT, {})).status, 503);            // secret not set

// wrong method
assert.equal((await worker.fetch(new Request("https://x/"), {})).status, 405);

console.log("ok");
