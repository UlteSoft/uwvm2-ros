/** Task09 file selection around the genuine Task04 StyLua2.6.0 method. */
import fs from "node:fs";
import path from "node:path";
import crypto from "node:crypto";
import {pathToFileURL} from "node:url";

const [source, acquisitionFile, acquisitionSha, reference, referenceSha, output] = process.argv.slice(2);
if (process.argv.length !== 8 || process.platform !== "linux" ||
    process.env.UWVM_RESOURCE_HEAVY_LOCK_HELD !== "1") {
    throw new Error("explicit captured sources, real tools and finite Linux owner required");
}
function pin(file) {
    const raw = fs.readFileSync(file);
    return {path: fs.realpathSync(file), bytes: raw.length,
        sha256: crypto.createHash("sha256").update(raw).digest("hex")};
}
function same(actual, expected) {
    return actual.bytes === expected.bytes && actual.sha256 === expected.sha256;
}
if (pin(acquisitionFile).sha256 !== acquisitionSha || pin(reference).sha256 !== referenceSha) {
    throw new Error("reviewed real formatter acquisition/reference driver changed");
}
const acquisition = JSON.parse(fs.readFileSync(acquisitionFile, "utf8"));
if (acquisition.version !== "2.6.0" || acquisition.package_scripts_executed !== false ||
    acquisition.status !== "ACQUIRED_NOT_FORMAT_QUALIFIED" || acquisition.files.length !== 16) {
    throw new Error("exact genuine StyLua acquisition required");
}
const tools = path.dirname(acquisitionFile);
function verifyTools() {
    if (pin(acquisitionFile).sha256 !== acquisitionSha || pin(reference).sha256 !== referenceSha) {
        throw new Error("actual tool receipt/reference changed");
    }
    for (const row of acquisition.files) {
        if (!same(pin(path.join(tools, row.path)), row)) throw new Error("complete StyLua package changed");
    }
}
verifyTools();
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
const files = [
    "sdk/tools/platform/nvidia/examples/build_backend.lua",
    "sdk/example/platform/nvidia-groot/tinygo/xmake.lua",
    "sdk/example/platform/nvidia-groot/py/xmake.lua",
    "sdk/example/platform/nvidia-isaac-ros/c/xmake.lua",
    "sdk/example/platform/nvidia-isaac-ros/cpp/xmake.lua",
    "sdk/example/platform/nvidia-isaac-sim/cpp/xmake.lua",
    "sdk/example/platform/nvidia-isaac-sim/py/xmake.lua",
    "sdk/tools/platform/nvidia-isaac-sim/clock/clock_build.lua",
    "sdk/example/platform/nvidia-isaac-sim/rs/xmake.lua",
    "sdk/example/platform/nvidia-isaac-sim/as/xmake.lua",
    "sdk/example/platform/nvidia-isaac-sim/tinygo/xmake.lua"
];
fs.mkdirSync(output, {recursive: false});
const rows = [];
for (const relative of files) {
    const file = path.join(source, relative), raw = fs.readFileSync(file), text = raw.toString("utf8");
    if (!Buffer.from(text).equals(raw)) throw new Error("ordinary UTF-8 Lua source required");
    const first = style.formatCode(text, config(), undefined, style.OutputVerification.Full);
    const second = style.formatCode(first, config(), undefined, style.OutputVerification.Full);
    if (first !== second) throw new Error("real Lua54 formatter bytes are not idempotent");
    const target = path.join(output, "formatted", relative);
    fs.mkdirSync(path.dirname(target), {recursive: true});
    fs.writeFileSync(target, first);
    rows.push({relative, source: pin(file), product: pin(target),
        actual_full_stylua_ast_verification: true, second_pass_byte_idempotence: true});
}
verifyTools();
for (const row of rows) {
    if (!same(pin(row.source.path), row.source)) throw new Error("actual Lua source drift");
}
fs.writeFileSync(path.join(output, "results.json"), JSON.stringify({
    format: "task09-real-stylua-lua54-format-v1", status: "SCOPED_LUA54_FORMAT_AST_IDEMPOTENCE_PASS",
    formatter_version: acquisition.version, acquisition: pin(acquisitionFile),
    real_shared_method_reference: pin(reference),
    adaptation: "Task09 closed eleven-file selection/report; original real tool/config/Full verification method retained",
    tool_files_rechecked: acquisition.files.length, node: pin(process.execPath), node_version: process.version,
    configuration, files: rows, application_executions: 0, guest_credit: 0,
    public_api_documentation_qualification: false, default_authority: []
}, null, 2) + "\n");
