/** Format the four owned xmake Lua modules with the actual pinned upstream Wasm tool. */
import fs from "node:fs";
import path from "node:path";
import crypto from "node:crypto";
import {pathToFileURL} from "node:url";
import {spawnSync} from "node:child_process";

if (process.platform !== "linux" || process.env.UWVM_RESOURCE_HEAVY_LOCK_HELD !== "1") {
    throw new Error("original admitted Linux finite owner required");
}
const run = process.env.UWVM_RESOURCE_RUN;
const inputs = path.join(run, "inputs");
const work = path.join(run, "work");
const acquisitionFile = "/home/macromodel/Documents/uwvm-test-261018/agents/04/round05-stylua-wasm-r1/work/formatter/acquisition.json";
const expectedAcquisition = "b3f3bee783667dbaa36d8c39c9f5f0932d90ee6dcbe7f763394c77f222f67f77";
const expectedNode = "893391109bf5d2b34dd50143bfab997b3fcd123dd59cb156b67f01b7a1db02bf";
const xmake = "/home/macromodel/.local/bin/xmake";
const files = ["tools/xmake/c.lua", "example/platform/ros/c/xmake.lua", "example/platform/ros/c/c/build.lua", "example/platform/ros/c/c/options.lua"];
function pin(file) {
    const raw = fs.readFileSync(file);
    return {path: fs.realpathSync(file), bytes: raw.length,
        sha256: crypto.createHash("sha256").update(raw).digest("hex")};
}
if (pin(process.execPath).sha256 !== expectedNode || process.version !== "v20.19.4") {
    throw new Error("published exact Node binary/version changed");
}
if (pin(acquisitionFile).sha256 !== expectedAcquisition) throw new Error("official acquisition pin changed");
const acquisition = JSON.parse(fs.readFileSync(acquisitionFile, "utf8"));
const tools = path.dirname(acquisitionFile);
if (acquisition.version !== "2.6.0" || acquisition.files.length !== 16 ||
    acquisition.unpacked_bytes !== 3871762 ||
    acquisition.archive_sha256 !== "d965fcbb5d129e5337ce1cae0d693fd7da08883afe5ac11616a7e3c3d88a7f4d") {
    throw new Error("exact upstream package closure required");
}
function checkTools() {
    for (const row of acquisition.files) {
        const file = path.join(tools, row.path);
        if (!fs.lstatSync(file).isFile()) throw new Error("ordinary read-only tool member required");
        const actual = pin(file);
        if (actual.bytes !== row.bytes || actual.sha256 !== row.sha256) throw new Error("formatter closure changed");
    }
    if (pin(acquisitionFile).sha256 !== expectedAcquisition || pin(process.execPath).sha256 !== expectedNode) {
        throw new Error("formatter acquisition or host changed");
    }
}
checkTools();
const commands = [];
function execute(label, argv) {
    const result = spawnSync(argv[0], argv.slice(1), {cwd: work, encoding: "utf8", timeout: 15000});
    fs.writeFileSync(path.join(work, label + ".stdout"), result.stdout || "");
    fs.writeFileSync(path.join(work, label + ".stderr"), result.stderr || "");
    commands.push({label, argv, exit_code: result.status});
    if (result.error || result.status !== 0) throw new Error("actual xmake failed " + label);
    return result.stdout;
}
function probe(text) {
    const found = text.match(/C_XMAKE_VM_JSON=(\{[^\r\n]+\})/);
    if (!found) throw new Error("actual VM observation missing");
    return JSON.parse(found[1]);
}
const vm = probe(execute("vm", [xmake, "lua", path.join(inputs, "xmake_vm_probe.lua")]));
const syntax = vm.actual_syntax;
if (!["Lua51", "Lua52", "Lua53", "Lua54", "LuaJIT"].includes(syntax)) {
    throw new Error("unsupported observed xmake VM; never fall back to All syntax");
}
const sourceCompilation = probe(execute("source-syntax", [xmake, "lua", path.join(inputs, "xmake_vm_probe.lua"),
    "compile", ...files.map(relative => path.join(inputs, "source", relative))]));
if (sourceCompilation.actual_syntax !== syntax || sourceCompilation.compiled_files.length !== files.length) {
    throw new Error("actual VM source syntax denominator changed");
}
const configurationFile = path.join(inputs, "lua-format.json");
const configuration = JSON.parse(fs.readFileSync(configurationFile, "utf8"));
if (configuration.version !== acquisition.version || configuration.syntax_policy !== "observed-xmake-vm" ||
    configuration.sort_requires !== false || configuration.verification !== "Full") {
    throw new Error("explicit formatter/syntax/verification configuration required");
}
const style = await import(pathToFileURL(path.join(tools, "package/stylua_lib_node.mjs")).href);
function config() {
    const value = style.Config.new();
    value.syntax = style.LuaVersion[syntax];
    value.indent_type = style.IndentType[configuration.indent_type];
    value.indent_width = configuration.indent_width;
    value.column_width = configuration.column_width;
    value.line_endings = style.LineEndings[configuration.line_endings];
    value.quote_style = style.QuoteStyle[configuration.quote_style];
    value.call_parentheses = style.CallParenType[configuration.call_parentheses];
    const sort = style.SortRequiresConfig.new();
    sort.enabled = false;
    value.sort_requires = sort;
    return value;
}
const rows = [];
for (const relative of files) {
    const source = path.join(inputs, "source", relative);
    const raw = fs.readFileSync(source), text = raw.toString("utf8");
    if (!Buffer.from(text).equals(raw)) throw new Error("exact UTF-8 source required");
    const first = style.formatCode(text, config(), undefined, style.OutputVerification.Full);
    const second = style.formatCode(first, config(), undefined, style.OutputVerification.Full);
    if (first !== second) throw new Error("actual formatter byte idempotence failed");
    const product = path.join(work, "files", relative);
    fs.mkdirSync(path.dirname(product), {recursive: true});
    fs.writeFileSync(product, first);
    rows.push({relative, source: pin(source), product: pin(product), full_moon_ast_verification: true,
        second_pass_byte_idempotence: true});
}
const contract = execute("contract", [xmake, "lua", path.join(inputs, "xmake_contract.lua"), path.join(work, "files/tools/xmake/c.lua")]);
const found = contract.match(/C_XMAKE_CONTRACT_CHECKS=(\d+)/);
if (!found || Number(found[1]) !== 23) throw new Error("actual current23-check VM contract required");
const catalog = probe(execute("catalog", [xmake, "lua", path.join(inputs, "xmake_vm_probe.lua"), path.join(work, "files/example/platform/ros/c/xmake.lua")]));
const productCompilation = probe(execute("formatted-syntax", [xmake, "lua", path.join(inputs, "xmake_vm_probe.lua"),
    "compile", ...files.map(relative => path.join(work, "files", relative))]));
if (productCompilation.actual_syntax !== syntax || productCompilation.compiled_files.length !== files.length) {
    throw new Error("actual VM formatted syntax denominator changed");
}
checkTools();
const report = {format: "task02-real-xmake-lua-format-v1", status: "PASS_OWNED_FOUR_FILE_FORMAT_AST_IDEMPOTENCE_AND_VM",
    formatter_version: acquisition.version, acquisition: pin(acquisitionFile), node: pin(process.execPath), node_version: process.version,
    complete_tool_files_rechecked_before_and_after: acquisition.files.length, tool_closure_read_only: true,
    configuration: {...configuration, actual_syntax: syntax}, configuration_pin: pin(configurationFile),
    xmake: pin(xmake), actual_vm: vm, source_syntax: sourceCompilation, formatted_syntax: productCompilation,
    catalog, actual_adapter_checks: 23, files: rows, commands,
    C_source_changes: 0, compiler_runs: 0, guest_runs: 0, native_ros_runs: 0, public_sdk_published: false};
fs.writeFileSync(path.join(work, "lua-format.json"), JSON.stringify(report, null, 2) + "\n");
console.log(JSON.stringify({status: report.status, files: rows.length, actual_syntax: syntax, actual_adapter_checks: 23}));
