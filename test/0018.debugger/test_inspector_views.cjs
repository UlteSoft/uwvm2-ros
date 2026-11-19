"use strict";

// Unit checks for the inspector view model, its webview contract and the
// Wasm instruction layout. Pure Node: no VS Code, broker or VM is started.
const assert = require("assert");
const fs = require("fs");
const path = require("path");
const vm = require("vm");

const DEBUG = path.resolve(__dirname, "../../tools/debug");
const model = require(path.join(DEBUG, "inspection_model.js"));
let checks = 0;
const check = (value, message) => { assert.ok(value, message); checks++; };

// --- selector validation stays exact (no silent clamping)
check(model.memorySelection({offset: "18446744073709551614", count: 1}).offset === "18446744073709551614", "64-bit offset kept exact");
assert.throws(() => model.memorySelection({offset: "18446744073709551615", count: 1}), /64-bit/); checks++;
assert.throws(() => model.memorySelection({offset: "-1"}), /unsigned/); checks++;
assert.throws(() => model.editBytes("4"), /complete/); checks++;
check(model.editBytes("de ad be ef").equals(Buffer.from([0xde, 0xad, 0xbe, 0xef])), "hex pairs with spaces");

// --- memory view: the webview owns the virtual grid; the model describes the memory
const tagged = {
  memoryReference: "wasm-memory:0:0:504", offset: "504", count: 16, stopId: "7", status: "available",
  memoryBytes: "65536", tagBits: 4, granuleBytes: 16, data: Buffer.from([...Array(16).keys()]).toString("base64"),
  tags: [{offset: "496", tag: 0, hex: "0x00", color: "#ff4040"}, {offset: "512", tag: 3, hex: "0x03", color: "#4086ff"}]
};
const decoded = model.decodeMemory(tagged, {module: "0", memory: "0", offset: "504", count: 16});
check(decoded.bytes.length === 16 && decoded.tags[1].hex === "0x03", "pages decode bytes and per-granule tags exactly");
const info = {memoryBytes: "65536", status: "available", tagBits: 4, granuleBytes: 16, addressBits: 32};
const memoryModel = {id: 1, kind: "memory", sessionId: "s", sessionName: "Robot", permission: "full", stopId: "7", epoch: 3,
  selector: {module: "0", memory: "0", offset: "504", count: 16}, columns: 8, showTags: true, data: info};
let view = model.view(memoryModel);
check(view.memory.info.memoryBytes === "65536" && view.memory.info.addressBits === 32, "the view describes the whole memory, not one page");
check(view.memory.selector.columns === 8 && view.memory.showTags === true, "row width and tag switch reach the grid");
check(view.memory.canWrite === true, "full permission at a current stop can edit in place");
check(JSON.parse(JSON.stringify(view)).memory.info.memoryBytes === "65536", "view survives postMessage serialization");
check(view.footer.some(item => /64 KiB · 32-bit/.test(item.text)), "status line shows the memory size and address width");
view = model.view({...memoryModel, permission: "read-only"});
check(view.memory.canWrite === false && view.permission.canInspect === true, "read-only inspects but never edits");
view = model.view({...memoryModel, running: true, stopId: undefined});
check(view.memory.canWrite === false && view.status.state === "running", "a running guest is shown as running and cannot be edited");
check(!view.banners.some(b => /earlier stop/.test(b.text)), "stepping does not flash a stale-values banner");
view = model.view({...memoryModel, running: true, showStale: true, stopId: undefined});
check(view.banners.some(b => /earlier stop/.test(b.text)), "a long run marks the retained values as not live");
view = model.view({...memoryModel, data: {...info, status: "disabled"}});
check(view.memory.showTags === false, "tags stay off when runtime MTE is disabled");

// Written ranges are carried separately from their text so Show bytes does not
// have to recover an address from a human-readable notification.
for (const bits of [32, 64]) {
  const offset = bits === 32 ? "19971" : "9007199254740993";
  const result = {status: "verified", text: "Written", module: "0", memory: "0", offset, count: 4};
  const m = {...memoryModel, data: {...info, addressBits: bits, memoryBytes: (BigInt(offset) + 100n).toString()}, writeResult: result};
  const shown = model.view(m);
  check(model.memoryAddress(offset, bits) === "0x" + BigInt(offset).toString(16).padStart(bits / 4, "0"), "write address width " + bits);
  check(shown.memory.write.offset === offset && shown.memory.write.count === 4, "exact written range " + bits);
  check(shown.banners.find(b => b.text === "Written").actions[0].id === "focus-written", "written range has navigation " + bits);
  const foreign = model.view({...m, selector: {...m.selector, memory: "1"}});
  check(foreign.memory.write.offset === undefined && !foreign.banners.find(b => b.text === "Written").actions, "old memory write cannot navigate another memory " + bits);
}
// --- characters: 'a' 'b' 'c' nul tab with separate quote/char/control parts
const values = require(path.join(DEBUG, "memory_values.js"));
const ascii = values.interpret([0x61, 0x62, 0x63, 0, 9], "little").find(row => row.name === "ASCII");
check(ascii.value === "'a' 'b' 'c' nul tab", "characters print as 'a' 'b' 'c' nul tab");
check(ascii.parts.map(part => part.kind).join(",") === "char,char,char,control,control", "quotes, characters and control names are separable for colouring");
check(values.interpret([0x27], "little").find(row => row.name.startsWith("Char")).value === "'\\''", "a quote character is escaped");
check(values.interpret([0xe5, 0xad, 0x97], "little").find(row => row.name === "UTF-8").value === "'字'", "valid multi-byte characters keep their text");

// Binary radix is explicit for all supported integer widths and byte orders.
for (const size of [1, 2, 4, 8, 16]) for (const endian of ["little", "big", "pdp"]) {
  if (endian === "pdp" && size === 1) continue;
  const bytes = Array.from({length: size}, (_, i) => (i * 31 + 5) & 255);
  const row = values.interpret(bytes, endian).find(row => row.name === "Bin" + (size > 1 && endian !== "little" ? "." + (endian === "big" ? "be" : "pdp") : ""));
  check(/^0b[01]+$/.test(row.value) && row.value.length === 2 + size * 8, "binary radix and width: " + size + " " + endian);
  check(BigInt(row.value) === values.integer(Uint8Array.from(bytes), endian), "binary value: " + size + " " + endian);
}

// Rounded text is plain numeric text; the exact value and rounding status
// remain available for the hover, including negative zero and nonfinite values.
const floatRows = values.interpret([0xcd, 0xcc, 0xcc, 0x3d], "little", {precision: 3});
const float = floatRows.find(row => row.name === "Float32");
check(float.value === "0.1" && float.rounded && /^0x/.test(float.exact), "rounded float keeps numeric text and exact metadata");
check(!floatRows.some(row => row.value.includes("≈")), "floating rows do not include an approximation prefix");
check(values.interpret([0, 0, 0, 0x80], "little").find(row => row.name === "Float32").value === "-0", "negative zero sign is preserved");
check(values.interpret([0, 0, 0x80, 0x7f], "little").find(row => row.name === "Float32").value === "Infinity", "infinity remains explicit");

// --- permission and session states produce explanations, not empty screens
view = model.view({id: 2, kind: "wasip1", epoch: 1, module: "0", wasiGroup: "env"});
check(view.session.connected === false && view.status.state === "detached", "no session is reported as detached");
view = model.view({id: 2, kind: "ros", epoch: 1, sessionId: "s", permission: "limited", backend: "all", rosGroup: "sessions"});
check(view.banners.some(b => /Limited/.test(b.text)), "limited permission explains what is withheld");
view = model.view({id: 3, kind: "diagnostics", sessionId: "s", permission: "full", threaded: true,
  stopId: "0", data: {State: "exited"}});
check(view.status.state === "joined" && !view.status.text.includes("stop 0"), "joined cohort is not described as a paused instruction");

// An IDE preference cannot widen the VM's dump policy. Unknown/disabled launch
// policy never leaves an enabled Save button, even in a full debugger session.
for (const permission of ["full", "read-only", "limited", "none"]) {
  const base = {id: 3, kind: "diagnostics", epoch: 1, sessionId: "s", permission, stopId: "7", dumpDisclosure: "diagnostic"};
  check(!model.view(base).diagnostics.canDump, "unknown dump policy is closed: " + permission);
  check(!model.view({...base, dumpPolicy: {kind: "force", configured: "none", effective: "none", allowed: []}}).diagnostics.canDump,
    "disabled dump policy: " + permission);
  check(model.view({...base, dumpPolicy: {kind: "max", configured: "diagnostic", effective: "diagnostic", allowed: ["diagnostic"]}}).diagnostics.canDump === (permission !== "none"),
    "diagnostic disclosure remains permission scoped: " + permission);
  check(!model.view({...base, dumpDisclosure: "full", dumpPolicy: {kind: "force", configured: "diagnostic", effective: "diagnostic", allowed: ["diagnostic"]}}).diagnostics.canDump,
    "a stale selection cannot exceed the forced disclosure: " + permission);
}

// --- groups: structured rows, empty columns hidden, identifying columns first
const ros = {rows: [
  {module: {text: "", bytesBase64: "", truncated: false}, function: {text: "abi_version", bytesBase64: "YWJp", truncated: false}, calls: 1, errors: 0, generation: 0, alias: {text: "", bytesBase64: "", truncated: false}},
  {module: {text: "", bytesBase64: "", truncated: false}, function: {text: "log_write", bytesBase64: "bG9n", truncated: false}, calls: 3, errors: 1, generation: 0, alias: {text: "", bytesBase64: "", truncated: false}}
]};
const grouped = model.groupRows(ros);
check(grouped.columns[0] === "function", "the identifying column comes first");
check(!grouped.columns.includes("module") && grouped.hidden.includes("module"), "columns empty in every row are hidden");
check(grouped.columns.indexOf("generation") > grouped.columns.indexOf("errors"), "bookkeeping columns come last");
check(grouped.rows[0].function.text === "abi_version", "byte-exact text fragments are preserved");

// --- policy rows: key, value and flags; never a JSON string blob
const policy = model.policyRows([{key: {text: "enabled", bytesBase64: "", truncated: false}, value: {text: "false", bytesBase64: "", truncated: false},
  configured: true, enabled: false, redacted: false, value_offset: 0, value_total: 5, backend: "ros2", scope: "global"}]);
check(policy[0].key === "enabled" && policy[0].value.text === "false", "policy key/value stay separate");
check(policy[0].meta === "ros2 · global", "policy scope is summarised");
check(policy[0].fields.some(f => f.key === "configured"), "all original fields remain available as details");
check(!JSON.stringify(policy).includes("\\\"key\\\""), "policy rows are not stringified JSON");
check(policy[0].fields.some(f => f.key === "key") && policy[0].fields.some(f => f.key === "value"), "policy detail and copy retain both key and value");
const fragments = model.policyRows([0, 1024].map(offset => ({key: {text: "allowlist"}, backend: "ros2", scope: "group", target: "robots",
  kind: "limit", value_offset: offset, value_total: 1030,
  value: {text: "x".repeat(offset ? 6 : 1024), bytesBase64: Buffer.alloc(offset ? 6 : 1024, 120).toString("base64"), truncated: false}})));
check(fragments[0].id !== fragments[1].id, "policy value fragments have independent expansion identities");
check(fragments[0].part === "Bytes 0–1024 of 1030 (end exclusive)" && fragments[1].part === "Bytes 1024–1030 of 1030 (end exclusive)", "policy fragments show their complete byte ranges");
check(Buffer.from(fragments.map(row => row.exact).map(s => Buffer.from(s, "base64")).reduce((all, bytes) => Buffer.concat([all, bytes]), Buffer.alloc(0))).length === 1030, "policy exact bytes preserve every fragment");

// --- the webview scripts parse and only use their documented globals
for (const file of ["media/ui.js", "media/inspector.js", "media/update.js", "media/icons.js", "wasm_view_webview.js"]) {
  new vm.Script(fs.readFileSync(path.join(DEBUG, file), "utf8"), {filename: file}); checks++;
}
const icons = {}; vm.runInNewContext(fs.readFileSync(path.join(DEBUG, "media/icons.js"), "utf8"), {globalThis: icons});
for (const name of ["database", "refresh", "record", "debug-stackframe", "chevron-right", "law", "radio-tower"]) check(icons.UWVM_ICONS[name], "icon " + name);
check(Object.values(icons.UWVM_ICONS).every(svg => !/<script|on[a-z]+=|href/i.test(svg)), "icon geometry contains no script or links");
const css = fs.readFileSync(path.join(DEBUG, "media/ui.css"), "utf8") + fs.readFileSync(path.join(DEBUG, "media/inspector.css"), "utf8");
// Memory column headings are explicitly white; data colours remain theme-driven.
const whiteMemoryHeadings = ".hhead .haddr, .hhead .byte.colhead, .hhead .htext, .hhead .htags { color: #fff; }";
check(css.includes(whiteMemoryHeadings), "all memory column headings are white");
check(!/#[0-9a-f]{3,6}\b/i.test(css.replace(whiteMemoryHeadings, "").replace(/var\((?:[^()]|\([^()]*\))*\)/g, "")), "other inspector colours come from theme tokens");

// --- Wasm layout: real scope nesting for indent guides
const language = require(path.join(DEBUG, "wasm_language.js"));
const layout = require(path.join(DEBUG, "wasm_view_model.js")).layout;
const text = [";; module 0 func 1",
  ";; 0x00000000  block", ";; 0x00000002  loop", ";; 0x00000004  local.get 0", ";; 0x00000006  br_if 1", ";; 0x00000008  br 0",
  ";; 0x0000000a  end", ";; 0x0000000b  end", ";; 0x0000000c  end", ""];
const doc = {lineCount: text.length, lineAt: n => ({text: text[n]})};
check(language.instructionLine(text[1]), "instruction lines parse");
const result = layout(doc);
check(result.rows.map(r => r.depth).join(",") === "0,1,2,2,2,1,0,0", "depth follows block/loop nesting");
check(result.scopes.length === 3 && result.scopes[2].parent === 1, "scopes carry their parent for fixed-column guides");
check(result.rows[3].tokens[0].navigation.line === result.rows[6].line, "br_if 1 targets the block end");
check(result.rows[4].tokens[0].navigation.line === result.rows[1].line, "br 0 targets the loop header");

// --- package.json: every command used by the UI is contributed
const manifest = JSON.parse(fs.readFileSync(path.join(DEBUG, "package.json"), "utf8"));
const commands = new Set(manifest.contributes.commands.map(c => c.command));
for (const source of ["debug_tools.js", "inspection_ui.js", "update_ui.js", "wasm_view.js", "extension.js"])
  for (const match of fs.readFileSync(path.join(DEBUG, source), "utf8").matchAll(/registerCommand\(\s*"(uwvm\.[A-Za-z]+)"\s*,/g))
    check(commands.has(match[1]), "contributed command " + match[1]);
check(manifest.contributes.viewsWelcome.some(w => w.view === "uwvm.tools"), "the UWVM view explains what to do without a session");

console.log("PASS inspector view model, webview contract and Wasm layout (" + checks + " checks)");
