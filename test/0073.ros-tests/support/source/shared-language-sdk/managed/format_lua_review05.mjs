/** Actual upstream StyLua2.6.0 Lua54 AST verification and byte idempotence. */
import fs from "node:fs";
import path from "node:path";
import crypto from "node:crypto";
import {pathToFileURL} from "node:url";
const [source, acquisitionFile, expectedAcquisition, output, generated] = process.argv.slice(2);
if (!source || !acquisitionFile || !expectedAcquisition || !output || process.argv.length > 7 ||
    process.platform !== "linux" || process.env.UWVM_RESOURCE_HEAVY_LOCK_HELD !== "1") {
  throw new Error("explicit inputs and original admitted Linux finite owner required");
}
function pin(file) {
  const raw = fs.readFileSync(file);
  return {path: fs.realpathSync(file), bytes: raw.length, sha256: crypto.createHash("sha256").update(raw).digest("hex")};
}
if (pin(acquisitionFile).sha256 !== expectedAcquisition) throw new Error("pinned official formatter acquisition changed");
const acquisition = JSON.parse(fs.readFileSync(acquisitionFile, "utf8"));
const tools = path.dirname(acquisitionFile);
for (const row of acquisition.files) {
  const actual = pin(path.join(tools, row.path));
  if (actual.bytes !== row.bytes || actual.sha256 !== row.sha256) throw new Error("actual complete formatter package changed");
}
const style = await import(pathToFileURL(path.join(tools, "package/stylua_lib_node.mjs")).href);
const configuration = {syntax: "Lua54", indent_type: "Spaces", indent_width: 4, column_width: 100,
  line_endings: "Unix", sort_requires: false, quote_style: "AutoPreferDouble", verification: "Full"};
function config() {
  const value = style.Config.new();
  value.syntax = style.LuaVersion.Lua54;
  value.indent_type = style.IndentType.Spaces;
  value.indent_width = 4;
  value.column_width = 100;
  value.line_endings = style.LineEndings.Unix;
  value.quote_style = style.QuoteStyle.AutoPreferDouble;
  return value;
}
const files = generated ? ["bounded_records.lua", "roswasm/wire.lua"] : [
  "sdk/origin/lua/include/roswasm/init.lua", "sdk/origin/lua/include/roswasm/wire.lua",
  "sdk/origin/examples/lua/owned_session.lua", "sdk/origin/examples/lua/record_values.lua"];
fs.mkdirSync(output, {recursive: false});
const rows = [];
let generation;
if (generated) {
  if (generated !== "generated") throw new Error("one explicit generated-package mode required");
  generation = JSON.parse(fs.readFileSync(path.join(source, "generation.lua.json"), "utf8"));
  if (generation.kind !== "complete-lua-record-sources-v1" || !generation.records.length ||
      JSON.stringify(Object.keys(generation.outputs).sort()) !== JSON.stringify([...files].sort())) {
    throw new Error("actual complete generated Lua package required");
  }
  for (const relative of files) {
    const actual = pin(path.join(source, relative)), expected = generation.outputs[relative];
    if (actual.bytes !== expected.bytes || actual.sha256 !== expected.sha256) throw new Error("generated source pin changed");
  }
}
for (const relative of files) {
  const file = path.join(source, relative), raw = fs.readFileSync(file), text = raw.toString("utf8");
  if (!Buffer.from(text).equals(raw)) throw new Error("actual UTF-8 Lua source required");
  const first = style.formatCode(text, config(), undefined, style.OutputVerification.Full);
  const second = style.formatCode(first, config(), undefined, style.OutputVerification.Full);
  if (first !== second) throw new Error("actual Lua54 formatting is not byte-idempotent");
  const target = path.join(output, "formatted", relative);
  fs.mkdirSync(path.dirname(target), {recursive: true});
  fs.writeFileSync(target, first);
  rows.push({relative, source: pin(file), product: pin(target), actual_full_moon_ast_verification: true,
    second_pass_byte_idempotence: true});
}
const report = {format: "task04-real-stylua-lua54-format-v1", status: "SCOPED_LUA54_FORMAT_AST_IDEMPOTENCE_PASS",
  formatter_version: acquisition.version, acquisition: pin(acquisitionFile), tool_files_rechecked: acquisition.files.length,
  node: pin(process.execPath), node_version: process.version, configuration, files: rows,
  application_executions: 0, guest_credit: 0, public_api_documentation_qualification: false, default_authority: []};
fs.writeFileSync(path.join(output, "results.json"), JSON.stringify(report, null, 2) + "\n");
if (generation) {
  generation.unformatted_outputs = generation.outputs;
  generation.outputs = Object.fromEntries(rows.map(row => [row.relative,
    {bytes: row.product.bytes, sha256: row.product.sha256}]));
  generation.formatting = {source_generation_receipt: pin(path.join(source, "generation.lua.json")),
    actual_formatter_receipt: pin(path.join(output, "results.json")), configuration,
    actual_lua54_ast_verification: true, second_pass_byte_idempotence: true};
  fs.writeFileSync(path.join(output, "formatted/generation.lua.json"), JSON.stringify(generation, null, 2) + "\n");
}
