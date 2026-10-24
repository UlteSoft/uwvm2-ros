/** Run genuine StyLua2.6.0 Lua54 full-AST verification on exact Task08 inputs.
 * Native Node runs compiler/formatter tooling only; no SDK application is run.
 */
import fs from "node:fs";
import path from "node:path";
import crypto from "node:crypto";
import { pathToFileURL } from "node:url";

const [source, acquisitionFile, expectedAcquisition, output] = process.argv.slice(2);
if (process.argv.length !== 6 || process.platform !== "linux" ||
    process.env.UWVM_RESOURCE_HEAVY_LOCK_HELD !== "1") {
    throw new Error("exact inputs and original admitted Linux finite owner required");
}

function pin(file) {
    const data = fs.readFileSync(file);
    return { path: fs.realpathSync(file), bytes: data.length,
        sha256: crypto.createHash("sha256").update(data).digest("hex") };
}

function confined(root, relative) {
    if (typeof relative !== "string" || !relative || path.isAbsolute(relative) ||
        relative.split(/[\\/]/).some(part => part === ".." || !part)) {
        throw new Error("one confined ordinary source path required");
    }
    const file = path.join(root, relative);
    if (fs.lstatSync(file).isSymbolicLink() || !fs.statSync(file).isFile() ||
        !fs.realpathSync(file).startsWith(fs.realpathSync(root) + path.sep)) {
        throw new Error("source escaped its sealed ordinary-file root");
    }
    return file;
}

if (pin(acquisitionFile).sha256 !== expectedAcquisition) {
    throw new Error("pinned genuine formatter acquisition changed");
}
const acquisition = JSON.parse(fs.readFileSync(acquisitionFile, "utf8"));
if (acquisition.version !== "2.6.0" || acquisition.files.length !== 16) {
    throw new Error("exact retained official StyLua2.6.0 closure required");
}
const toolRoot = path.dirname(acquisitionFile);
const toolPins = acquisition.files.map(row => {
    const actual = pin(confined(toolRoot, row.path));
    if (actual.bytes !== row.bytes || actual.sha256 !== row.sha256) {
        throw new Error("complete genuine formatter package changed");
    }
    return actual;
});
const manifest = JSON.parse(fs.readFileSync(path.join(source, "manifest.json"), "utf8"));
if (manifest.format !== "task08-owned-lua-format-input-v1" ||
    !Array.isArray(manifest.files) || !manifest.files.length || manifest.files.length > 24 ||
    new Set(manifest.files).size !== manifest.files.length) {
    throw new Error("exact finite owned Lua source inventory required");
}
const inputs = manifest.files.map(relative => ({ relative, source: pin(confined(source, relative)) }));
if (inputs.reduce((total, row) => total + row.source.bytes, 0) > 384 * 1024) {
    throw new Error("honest tiny-job source extent exceeded");
}
const style = await import(pathToFileURL(path.join(toolRoot, "package/stylua_lib_node.mjs")).href);
const configuration = { syntax: "Lua54", indent_type: "Spaces", indent_width: 4,
    column_width: 100, line_endings: "Unix", quote_style: "AutoPreferDouble",
    sort_requires: false, verification: "Full" };
function config() {
    const result = style.Config.new();
    result.syntax = style.LuaVersion.Lua54;
    result.indent_type = style.IndentType.Spaces;
    result.indent_width = configuration.indent_width;
    result.column_width = configuration.column_width;
    result.line_endings = style.LineEndings.Unix;
    result.quote_style = style.QuoteStyle.AutoPreferDouble;
    return result;
}
fs.mkdirSync(output, { recursive: false });
const report = { format: "task08-owned-real-stylua-lua54-v1", status: "STARTED",
    acquisition: pin(acquisitionFile), node: pin(process.execPath), node_version: process.version,
    runner: pin(process.argv[1]), tool_pins: toolPins, configuration, files: inputs,
    compiler_frontends: 0, installed_xmake_builds: 0, Guest_runs: 0, native_ROS_runs: 0,
    application_executions: 0, normalized_languages: ["lua"], default_authority: [] };
function save() {
    fs.writeFileSync(path.join(output, "lua-format.json"), JSON.stringify(report, null, 2) + "\n");
}
save();
try {
    for (const row of inputs) {
        const raw = fs.readFileSync(row.source.path), text = raw.toString("utf8");
        if (!Buffer.from(text).equals(raw)) throw new Error("exact UTF-8 source required");
        const first = style.formatCode(text, config(), undefined, style.OutputVerification.Full);
        const second = style.formatCode(first, config(), undefined, style.OutputVerification.Full);
        const target = path.join(output, "formatted", row.relative);
        fs.mkdirSync(path.dirname(target), { recursive: true });
        fs.writeFileSync(target, first);
        row.product = pin(target);
        row.full_lua54_AST_verification = true;
        row.second_pass_byte_idempotence = first === second;
        save();
        if (!row.second_pass_byte_idempotence) throw new Error("actual format was not byte-idempotent");
    }
    report.status = "PASS_SCOPED_ACTUAL_LUA54_AST_FORMAT_IDEMPOTENCE_ONLY";
} catch (error) {
    report.status = "FAILED_RETAINED";
    report.error = String(error.stack || error);
    throw error;
} finally {
    report.source_unchanged = inputs.every(row => {
        const actual = pin(row.source.path);
        return actual.bytes === row.source.bytes && actual.sha256 === row.source.sha256;
    });
    report.tools_unchanged = [report.acquisition, report.node, report.runner, ...toolPins].every(row => {
        const actual = pin(row.path);
        return actual.bytes === row.bytes && actual.sha256 === row.sha256;
    });
    if (!report.source_unchanged || !report.tools_unchanged) {
        report.status = "FAILED_INPUT_IMMUTABILITY_RETAINED";
    }
    save();
    if (!report.source_unchanged || !report.tools_unchanged) throw new Error("sealed source/tool input changed");
}
