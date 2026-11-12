// node pull.mjs [out.sqlite]   (or: deno run -A pull.mjs)
//
// Copies the reports out of D1 into a local SQLite file built from schema.sql,
// for reading in whatever SQL tool you like. Start with the crashes view.
//
// Needs CLOUDFLARE_API_TOKEN (D1 Read is enough) and CLOUDFLARE_ACCOUNT_ID.
// The copy holds the HANDLEs players chose to send: keep it to yourself.
import { readFileSync, renameSync, rmSync } from "node:fs";
import { DatabaseSync } from "node:sqlite";
import process from "node:process";

const DATABASE = "ja2-crashes";
const PAGE = 500;

const token = process.env.CLOUDFLARE_API_TOKEN;
const account = process.env.CLOUDFLARE_ACCOUNT_ID;
if (!token || !account) {
  console.error("set CLOUDFLARE_API_TOKEN and CLOUDFLARE_ACCOUNT_ID");
  process.exit(2);
}
const out = process.argv[2] ?? "crashes.sqlite";

const api = async (path, body) => {
  const res = await fetch(`https://api.cloudflare.com/client/v4/accounts/${account}/d1/database${path}`, {
    method: body ? "POST" : "GET",
    headers: { Authorization: `Bearer ${token}`, "Content-Type": "application/json" },
    body: body && JSON.stringify(body),
  });
  const json = await res.json();
  if (!json.success) throw new Error(`${path}: ${JSON.stringify(json.errors)}`);
  return json.result;
};
const query = async (uuid, sql, params = []) => (await api(`/${uuid}/query`, { sql, params }))[0].results;

const found = (await api(`?name=${DATABASE}`)).find((d) => d.name === DATABASE);
if (!found) throw new Error(`no D1 database named ${DATABASE} in this account`);
const uuid = found.uuid;

// Built next to the target and moved over it only when complete, so a failed
// pull leaves the previous copy intact.
const tmp = `${out}.partial`;
rmSync(tmp, { force: true });
const db = new DatabaseSync(tmp);
db.exec(readFileSync(new URL("schema.sql", import.meta.url), "utf8"));

const copy = async (table, key) => {
  let n = 0, last = "";
  for (;;) {
    const rows = await query(uuid, `SELECT * FROM ${table} WHERE ${key} > ? ORDER BY ${key} LIMIT ${PAGE}`, [last]);
    if (rows.length === 0) return n;
    const cols = Object.keys(rows[0]);
    const insert = db.prepare(`INSERT INTO ${table} (${cols.join(",")}) VALUES (${cols.map(() => "?").join(",")})`);
    for (const row of rows) insert.run(...cols.map((c) => row[c]));
    n += rows.length;
    last = rows.at(-1)[key];
  }
};

const reports = await copy("reports", "id");
const releases = await copy("releases", "tag");
db.close();

renameSync(tmp, out);
console.log(`${out}: ${reports} reports, ${releases} releases`);
