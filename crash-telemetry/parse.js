// The crash report grammar -- what sgp::writeExceptionBacktrace writes, plus the
// names sgp/crash_telemetry.cpp adds to frames before uploading -- turned into the
// columns of the `reports` table in schema.sql.
//
// Every field is kept as the report wrote it. A line this grammar does not know
// keeps the whole text in `raw`: that is the case where the upload is the only
// copy and the columns alone would not give it back.

const HEADER = /^\*\*\* CRASH  code=([0-9A-F]{8})  eip=[0-9A-F]{8}  esp=([0-9A-F]{8})  ebp=([0-9A-F]{8}) \*\*\*$/;
const TIME = /^ {2}time (\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}) UTC$/;
const BUILD = /^ {2}build (.*)$/;
const IMAGE = /^ {2}image ([0-9A-F]{8})$/;
const HANDLE = /^ {2}handle (.*)$/;
const AV = /^ {2}access violation: (write to|execute at|read from) +([0-9A-F]{8})$/;
const MODULES = /^ {2}modules \(base size name\):$/;
const MODULE = /^ {4}([0-9A-F]{8}) ([0-9A-F]{8}) (.*)$/;
const FRAME = /^ {2}\[(\d+)\] ([0-9A-F]{8})(?: (.*))?$/;
const INLINED_INTO = /^ {6}inlined into (.*)$/;

const AV_KIND = { "write to": "write", "execute at": "execute", "read from": "read" };

export function parseReport(text) {
  const out = {
    reported_at: null, build: null, image: null, handle: null,
    code: null, esp: null, ebp: null,
    av_kind: null, av_addr: null,
    modules: [], frames: [], unparsed: [],
  };
  for (const line of text.split(/\r?\n/)) {
    if (line === "") continue;
    let m;
    if ((m = HEADER.exec(line))) {
      out.code = m[1];
      out.esp = parseInt(m[2], 16);
      out.ebp = parseInt(m[3], 16);
    } else if ((m = TIME.exec(line))) {
      out.reported_at = m[1];
    } else if ((m = BUILD.exec(line))) {
      out.build = m[1];
    } else if ((m = IMAGE.exec(line))) {
      out.image = parseInt(m[1], 16);
    } else if ((m = HANDLE.exec(line))) {
      out.handle = m[1];
    } else if ((m = AV.exec(line))) {
      out.av_kind = AV_KIND[m[1]];
      out.av_addr = parseInt(m[2], 16);
    } else if (MODULES.test(line)) {
      // the table's header line; nothing to keep
    } else if ((m = MODULE.exec(line))) {
      out.modules.push({ n: m[3], b: parseInt(m[1], 16), s: parseInt(m[2], 16) });
    } else if ((m = FRAME.exec(line)) && Number(m[1]) === out.frames.length) {
      out.frames.push(m[3] === undefined ? { a: parseInt(m[2], 16) }
        : { a: parseInt(m[2], 16), s: m[3] });
    } else if ((m = INLINED_INTO.exec(line)) && out.frames.length > 0) {
      const frame = out.frames[out.frames.length - 1];
      (frame.i = frame.i || []).push(m[1]);
    } else {
      out.unparsed.push(line);
    }
  }
  return out;
}

// sha-256 of the uploaded text: the row's primary key, so a report the client
// sends twice -- it cannot tell a timeout from a failure -- is one row.
export async function reportId(text) {
  const digest = await crypto.subtle.digest("SHA-256", new TextEncoder().encode(text));
  return [...new Uint8Array(digest)].map((b) => b.toString(16).padStart(2, "0")).join("");
}

export const COLUMNS = [
  "id", "received_at", "reported_at", "build", "image", "code", "esp", "ebp",
  "av_kind", "av_addr", "handle",
  "modules", "frames", "raw",
];

export const INSERT =
  `INSERT OR IGNORE INTO reports (${COLUMNS.join(", ")}) VALUES (${COLUMNS.map(() => "?").join(", ")})`;

export function toRow(text, id, receivedAt) {
  const p = parseReport(text);
  return [
    id, receivedAt, p.reported_at, p.build, p.image, p.code, p.esp, p.ebp,
    p.av_kind, p.av_addr, p.handle,
    JSON.stringify(p.modules), JSON.stringify(p.frames),
    p.unparsed.length ? text : null,
  ];
}
