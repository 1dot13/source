// node test.mjs — exercises the status-code contract the client depends on.
import assert from "node:assert";
import worker from "./worker.js";

const REPORT = `
*** CRASH  code=C0000005  eip=0071D5A0  esp=202BF99C  ebp=202BFA18 ***
  time 2026-07-26 10:41:02 UTC
  build 6a941c06
  handle @marco*evil
  access violation: read from 00000002
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
assert.deepEqual(JSON.parse(sent.body.get("payload_json")).allowed_mentions, { parse: [] });
assert.equal(await sent.body.get("files[0]").text(), REPORT);

// junk: client should delete these, so they must be 400
assert.equal((await post("hello")).status, 400);
assert.equal((await post("x".repeat(64 * 1024 + 1))).status, 400);

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
