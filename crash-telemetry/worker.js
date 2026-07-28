// Crash-telemetry sink for JA2 1.13. Takes the POST that sgp::processCrashTelemetry
// makes, and forwards the report to a Discord webhook as a file attachment.
//
// It stores nothing. Reports are only useful next to a PDB, and the PDB lives on a
// developer's machine, so there is nothing for a bucket to do here that the channel
// we already read bug reports in does not do better.
//
// The status codes matter — the client acts on them (reportIsSettled()):
//   2xx  accepted; the client deletes its copy
//   400  junk, never going to be accepted; the client deletes its copy
//   429  throttled; the client keeps the file and retries next launch
//   5xx  our problem; the client keeps the file and retries next launch
// So never answer a settling 4xx for a failure on our side: that throws the report
// away. 429 is the one 4xx that is safe, because the client does not settle it.

// Reports run 2-8 KB, and the fixed bounds in writeExceptionBacktrace (128
// modules, 64 frames) cap them near 10 KB. Kept equal to kMaxReportBytes in the
// client: anything the client is willing to send must not meet a settling 400
// here, or the upload is what destroys the report.
const MAX_BYTES = 32 * 1024;

// Every field below is lifted out of the uploaded file, and the endpoint is public
// and unauthenticated: treat all of it as attacker-chosen, not just the handle.
// Keep printable ASCII minus everything Discord reads as markup or a link, and cap
// the length, so nothing in a report can shape the message around it.
const clean = (s) => (s || "").replace(/[^A-Za-z0-9 .,+-]/g, "").slice(0, 64).trim();

// Pull the header fields the client writes, for a one-line channel summary.
// See writeExceptionBacktrace(): "*** CRASH code=... ***", then "  <key> <value>".
function summarize(text) {
  const field = (name) => (text.match(new RegExp(`^\\s+${name} (.+)$`, "m")) || [])[1];
  const code = clean((text.match(/code=(\w+)/) || [])[1]) || "????????";
  const av = clean((text.match(/access violation: (.+)/) || [])[1]);
  const parts = [`\`${code}\`${av ? ` (${av})` : ""}`];
  const build = clean(field("build"));
  const handle = clean(field("handle"));
  if (build) parts.push(`build \`${build}\``);
  if (handle) parts.push(`from **${handle}**`);
  return parts.join("  ·  ");
}

export default {
  async fetch(request, env) {
    if (request.method !== "POST") return new Response("POST only\n", { status: 405 });
    if (!env.DISCORD_WEBHOOK) {
      // `wrangler secret put` targets the deployed Worker; `wrangler dev` reads
      // .dev.vars instead. Missing one of the two is the usual cause of a 503.
      console.error("DISCORD_WEBHOOK unset (deploy: wrangler secret put, dev: .dev.vars)");
      return new Response("not configured\n", { status: 503 });
    }

    // Per-IP cap, checked before the body is read so a flood costs nothing. 429 is
    // deliberate: reportIsSettled() does not settle it, so a throttled player keeps
    // the report and it goes out next launch. Answering 400 here would delete it.
    const ip = request.headers.get("cf-connecting-ip") || "unknown";
    const { success } = await env.UPLOAD_LIMITER.limit({ key: ip });
    if (!success) return new Response("slow down\n", { status: 429 });

    // Trust the declared length only as a cheap early out; the real cap is on the
    // bytes actually read, since Content-Length can lie or be absent.
    const declared = Number(request.headers.get("content-length") || 0);
    if (declared > MAX_BYTES) return new Response("too large\n", { status: 400 });

    const text = await request.text();
    if (text.length > MAX_BYTES) return new Response("too large\n", { status: 400 });
    // The endpoint is public and unauthenticated — the URL ships in every player's
    // Ja2.ini. This is not security, just a filter that keeps drive-by POSTs and
    // scanners out of the channel. Anything determined gets through; that is fine,
    // the blast radius is a message we delete.
    if (!text.includes("*** CRASH")) return new Response("not a crash report\n", { status: 400 });

    const stamp = new Date().toISOString().replace(/[-:]/g, "").slice(0, 15);
    const form = new FormData();
    form.append("payload_json", JSON.stringify({
      content: summarize(text),
      allowed_mentions: { parse: [] }, // a report is attacker-controlled text; never ping
    }));
    form.append("files[0]", new Blob([text], { type: "text/plain" }), `crash_${stamp}.txt`);

    let res;
    try {
      res = await fetch(env.DISCORD_WEBHOOK, { method: "POST", body: form });
    } catch (e) {
      console.error("discord unreachable:", e.message);
      return new Response("upstream unreachable\n", { status: 503 });
    }
    // Discord's own 429 included: not the player's fault, so keep the report alive.
    if (!res.ok) {
      console.error("discord rejected:", res.status, (await res.text()).slice(0, 200));
      return new Response("upstream rejected\n", { status: 503 });
    }
    return new Response(null, { status: 204 });
  },
};
