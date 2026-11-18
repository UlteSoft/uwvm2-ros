"use strict";

// Validation and presentation model for the inspection windows. Everything
// here is pure: copied backend data in, JSON-safe view state out. The webview
// owns all DOM and styling (media/inspector.js); this file never builds HTML.
const {Buffer} = require("buffer");
const {ROUNDING} = require("./memory_values");

const PERMISSIONS = ["full", "read-only", "limited", "none"];
const KINDS = {memory: "Memory", registers: "Registers", wasip1: "WASIp1", wasithread: "WASI Threads", ros: "u2rosv1", scene: "u2rosv2 · Robot scene", preloaddl: "preloaddl", diagnostics: "Diagnostics", breakpoints: "Breakpoints", watch: "Watch"};
const KIND_ICONS = {memory: "database", registers: "symbol-numeric", wasip1: "terminal", wasithread: "list-tree", ros: "radio-tower", scene: "shield", preloaddl: "file-binary", diagnostics: "info", breakpoints: "debug-breakpoint", watch: "eye"};
const WASI_GROUPS = {args: "Arguments", env: "Environment", fds: "Descriptors", preopens: "Preopens", policy: "Policy", calls: "Recorded calls"};
const WASI_ICONS = {args: "list-flat", env: "symbol-field", fds: "files", preopens: "folder-opened", policy: "law", calls: "record"};
const ROS_GROUPS = {capabilities: "Capabilities", sessions: "Sessions", queues: "Queues", services: "Services", actions: "Actions",
  parameters: "Parameters", tf: "Transforms", graph: "Graph", stats: "Interface statistics", calls: "Call statistics",
  trace: "Call metadata", journal: "Recorded calls"};
const SCENE_GROUPS = {authority: "Authority", resources: "Resources", journal: "Recorded calls"};
const ROS_POLICY = "policy";
const COLUMN_WIDTHS = [1, 2, 4, 8, 16, 32, 64];

function decimal(value, label, bits = 64) {
  if (typeof value === "number" && Number.isSafeInteger(value)) value = String(value);
  if (typeof value !== "string" || !/^(0|[1-9][0-9]*)$/.test(value) || value.length > 20 || BigInt(value) >= (1n << BigInt(bits)))
    throw new Error(label + " must be an unsigned " + bits + "-bit decimal integer");
  return BigInt(value).toString();
}
function memoryAddress(value, bits) {
  if (bits !== 32 && bits !== 64) throw new Error("Memory address width must be 32 or 64 bits");
  return "0x" + BigInt(decimal(value, "Memory address", bits)).toString(16).padStart(bits / 4, "0");
}
function writtenRange(model) {
  const result = model.writeResult, info = model.data;
  if (!result || result.status !== "verified" || !info || result.module !== model.selector?.module || result.memory !== model.selector?.memory ||
      typeof result.offset !== "string" || !/^(0|[1-9][0-9]*)$/.test(result.offset) ||
      !Number.isSafeInteger(result.count) || result.count < 1 || BigInt(result.offset) + BigInt(result.count) > BigInt(info.memoryBytes)) return undefined;
  return {offset: result.offset, count: result.count};
}
function page(value, max = 256) {
  const text = decimal(value, "Page size", 32), count = Number(text);
  if (count < 1 || count > max) throw new Error("Page size must be 1.." + max);
  return count;
}
function extent(value, label) {
  const text = decimal(value, label), n = Number(text);
  if (!Number.isSafeInteger(n) || n < 1) throw new Error(label + " must be a positive exact integer");
  return n;
}
function columns(value = 16) { return extent(value, "Row bytes"); }
function memorySelection(input = {}) {
  if (typeof input === "string") {
    const parts = input.split(" ");
    if (parts.length !== 4) throw new Error("Use decimal MODULE MEMORY OFFSET BYTES");
    input = {module: parts[0], memory: parts[1], offset: parts[2], count: parts[3]};
  }
  if (!input || typeof input !== "object" || Array.isArray(input)) throw new Error("Memory selection must contain module, memory, offset and count");
  const selected = {
    module: decimal(input.module === undefined ? "0" : input.module, "Module", 32),
    memory: decimal(input.memory === undefined ? "0" : input.memory, "Memory", 32),
    offset: decimal(input.offset === undefined ? "0" : input.offset, "Offset"),
    count: extent(input.count === undefined ? 256 : input.count, "Range bytes")
  };
  if (BigInt(selected.offset) + BigInt(selected.count) >= (1n << 64n)) throw new Error("Memory range exceeds the unsigned 64-bit address space");
  return selected;
}
function decodeMemory(reply, selected) {
  const reference = `wasm-memory:${selected.module}:${selected.memory}:${selected.offset}`;
  if (!reply || reply.memoryReference !== reference || reply.offset !== selected.offset || reply.count !== selected.count ||
      !/^[1-9][0-9]{0,19}$/.test(reply.stopId) || BigInt(reply.stopId) >= (1n << 64n) ||
      !["available", "disabled", "unsupported-memory"].includes(reply.status) || !Array.isArray(reply.tags) || typeof reply.data !== "string")
    throw new Error("Invalid memory snapshot from the debugger");
  const bytes = Buffer.from(reply.data, "base64");
  if (bytes.length !== selected.count || bytes.toString("base64") !== reply.data) throw new Error("Incomplete memory bytes from the debugger");
  const start = BigInt(selected.offset), granules = selected.count ? Number((start + BigInt(selected.count) - 1n) / 16n - start / 16n + 1n) : 0;
  if (reply.tags.length !== (reply.status === "available" ? granules : 0)) throw new Error("Incomplete allocation tag coverage");
  const tags = reply.tags.map((row, index) => {
    const first = (start / 16n + BigInt(index)) * 16n;
    if (!row || row.offset !== first.toString() || !Number.isInteger(row.tag) || row.tag < 0 || row.tag > 255 ||
        row.hex !== "0x" + row.tag.toString(16).padStart(2, "0") || !/^#[0-9a-f]{6}$/.test(row.color))
      throw new Error("Invalid allocation tag row");
    return {offset: row.offset, tag: row.tag, hex: row.hex, color: row.color};
  });
  return {...reply, bytes: Array.from(bytes), tags};
}
function editBytes(value) {
  if (typeof value !== "string" || value.length > 2048) throw new Error("Enter 1..256 bytes as hexadecimal pairs");
  const compact = value.replace(/\s/g, "");
  if (!/^(?:[0-9a-fA-F]{2}){1,256}$/.test(compact)) throw new Error("Enter 1..256 complete hexadecimal byte pairs");
  return Buffer.from(compact, "hex");
}
function reason(permission, connected = true) {
  if (permission === "none") return "The runtime launched with debugger permission none; no inspection is authorized.";
  if (!connected) return "No UWVM debug session is attached. Start or attach one, then refresh this window.";
  if (permission === "limited") return "Limited permission exposes numeric crash diagnostics only.";
  if (!PERMISSIONS.includes(permission)) return "";
  return "";
}

// ---------------------------------------------------------------- formatting
function textValue(value) {
  if (value === null) return "null";
  if (value === undefined) return "—";
  if (typeof value === "object") {
    if (typeof value.text === "string") return value.text + (value.truncated ? " [truncated]" : "");
    return Array.isArray(value) ? "Array(" + value.length + ")" : "Object(" + Object.keys(value).length + ")";
  }
  return String(value);
}
const CONTROL_NAMES = {0: "NUL", 7: "BEL", 8: "BS", 9: "TAB", 10: "LF", 11: "VT", 12: "FF", 13: "CR", 27: "ESC", 127: "DEL"};
function fragment(row) {
  if (!row || typeof row !== "object") return undefined;
  if (typeof row.truncated === "boolean" || typeof row.bytesBase64 === "string") return {text: row.text, truncated: !!row.truncated, bytesBase64: row.bytesBase64};
  return undefined;
}
function policyRows(rows) {
  return (rows || []).map((row, index) => {
    if (!row || typeof row !== "object" || !Object.hasOwn(row, "key")) return {key: "", value: textValue(row), meta: "", fields: []};
    const context = [row.backend, row.scope, row.target && textValue(row.target), row.module && textValue(row.module), row.alias && textValue(row.alias)]
      .filter(v => v !== undefined && v !== null && v !== "").join(" · ");
    const flags = [];
    if (row.configured !== undefined) flags.push({label: "configured", value: String(row.configured)});
    if (row.enabled !== undefined) flags.push({label: "enabled", value: String(row.enabled)});
    if (row.redacted !== undefined) flags.push({label: "redacted", value: String(row.redacted)});
    if (row.value_offset !== undefined && row.value_total !== undefined) flags.push({label: "fragment", value: "bytes " + textValue(row.value_offset) + " of " + textValue(row.value_total)});
    // A long policy value occupies several wire rows, possibly on different
    // pages. Preserve each fragment's identity and complete original fields.
    // Key + scope alone would expand several different fragments together.
    const fields = Object.entries(row).map(([key, value]) => ({key, value: fragment(value) || value}));
    const exact = typeof row.value?.bytesBase64 === "string" ? row.value.bytesBase64 : row.valueBytesBase64;
    const offset = String(row.value_offset ?? ""), total = String(row.value_total ?? "");
    let part;
    if (!row.redacted && /^\d+$/.test(offset) && /^\d+$/.test(total) && typeof exact === "string") {
      const first = BigInt(offset), size = BigInt(Buffer.from(exact, "base64").length), length = BigInt(total);
      if (first > 0n || size < length) part = "Bytes " + first + "–" + (first + size) + " of " + length + " (end exclusive)";
    }
    return {key: textValue(row.key), value: fragment(row.value) || textValue(row.value), meta: context, flags,
      id: JSON.stringify([context, textValue(row.key), row.kind, offset, index]), part,
      exact: typeof exact === "string" ? exact : undefined, fields};
  });
}
// Identifying columns first, counters next, bookkeeping last.
const LEADING = ["id", "seq", "name", "function", "op", "topic", "service", "action", "parameter", "key", "type", "value", "module", "alias", "target", "backend", "instance", "calls", "errors"];
const TRAILING = ["generation", "incomplete", "unresolved", "saturated", "timed", "window_ns", "scope"];
function blank(cell) {
  if (cell === undefined || cell === null || cell === "") return true;
  if (typeof cell === "object" && typeof cell.text === "string" && "bytesBase64" in cell) return cell.text === "" && !cell.truncated;
  return false;
}
function groupRows(data) {
  const rows = (data && (data.rows || data.variables)) || [];
  if (!rows.length) return {rows: [], columns: [], hidden: []};
  const internal = new Set(["variablesReference", "presentationHint", "memoryReference", "evaluateName", "indexedVariables", "namedVariables"]);
  const all = Array.from(new Set(rows.flatMap(row => row && typeof row === "object" ? Object.keys(row) : ["value"]))).filter(key => !internal.has(key));
  // A column that is empty in every row carries no information here; it is
  // still listed in each row's expandable details.
  const hidden = all.filter(key => rows.every(row => blank(row && typeof row === "object" ? row[key] : row)));
  const rank = key => { const lead = LEADING.indexOf(key), tail = TRAILING.indexOf(key); return lead >= 0 ? lead : tail >= 0 ? 1000 + tail : 500; };
  const keys = all.filter(key => !hidden.includes(key)).sort((a, b) => rank(a) - rank(b));
  return {columns: keys, hidden, rows: rows.map(row => {
    const shown = Object.fromEntries(keys.map(key => {
      const cell = row && typeof row === "object" ? row[key] : row;
      return [key, fragment(cell) || (cell !== null && typeof cell === "object" ? cell : cell === undefined ? undefined : typeof cell === "boolean" || typeof cell === "number" ? cell : textValue(cell))];
    }));
    Object.defineProperty(shown, "__raw", {value: row, enumerable: false});
    return shown;
  }).map(row => ({...row, __raw: row.__raw}))};
}
function journalView(data) {
  if (!data || !data.journal) return undefined;
  const journal = data.journal;
  return {epoch: journal.epoch, enabled: journal.enabled, newest: journal.newest, cleared: journal["cleared-through"],
    failures: {allocation: journal["allocation-failures"], ranges: journal["invalid-ranges"], sequence: journal["sequence-failures"]}};
}

// ------------------------------------------------------------- view assembly
function statusOf(model) {
  if (model.error) return {state: "error", text: model.error};
  if (!model.sessionId) return {state: "detached", text: "No session"};
  if (model.running) return {state: "running", text: "Running"};
  if (model.busy && !model.data) return {state: "checking", text: "Reading…"};
  if (model.threaded && model.data?.State === "exited" && model.stopId === "0")
    return {state: "joined", text: "Cohort exited · replay available"};
  if (model.stopId) return {state: "paused", text: "Paused at stop " + model.stopId};
  if (model.retired) return {state: "retired", text: "Snapshot retired"};
  return {state: "idle", text: "Connected"};
}
function permissionView(model) {
  const level = model.permission;
  const labels = {full: "Full", "read-only": "Read-only", limited: "Limited", none: "None"};
  const detail = {full: "Inspect and edit guest state", "read-only": "Inspect only; changes are refused", limited: "Numeric diagnostics only", none: "Debugger disabled at launch"};
  return {level: level || "unknown", label: labels[level] || "Checking…", detail: detail[level] || "Authenticated permission pending",
    canInspect: ["full", "read-only"].includes(level), canWrite: level === "full" && !!model.data && !model.retired};
}
function bannerView(model, denied, connected) {
  const banners = [];
  if (denied && connected) banners.push({level: "warning", icon: "lock", text: denied});
  if (model.error) banners.push({level: "error", icon: "error", text: model.error, actions: [{id: "refresh", label: "Retry", icon: "refresh"}]});
  if (model.showStale && model.data && !model.error && !/^Assembly stop:/.test(model.notice || "")) banners.push({level: "info", icon: "history",
    text: "This snapshot belongs to an earlier stop. Values below are retained copies and are not live.", actions: [{id: "refresh", label: "Read current stop", icon: "refresh"}]});
  if (model.notice) banners.push({level: "info", icon: "info", text: model.notice});
  if (model.message) banners.push({level: "success", icon: "check", text: model.message});
  if (model.writeError) banners.push({level: "error", icon: "error", text: "Memory write: " + model.writeError});
  if (model.writeResult) banners.push({level: "success", icon: "verified", text: model.writeResult.text,
    actions: writtenRange(model) ? [{id: "focus-written", label: "Show bytes", icon: "target"}] : undefined});
  return banners;
}

function memoryView(model) {
  // The webview owns the virtual hex grid and its page cache. The extension
  // only describes the selected memory; pages travel as separate messages.
  const info = model.data && model.data.memoryBytes !== undefined ? model.data : undefined;
  const canWrite = model.permission === "full" && !!info && !!model.stopId && !model.running;
  return {
    selector: {module: model.selector.module, memory: model.selector.memory, offset: model.selector.offset, columns: columns(model.columns)},
    columns: COLUMN_WIDTHS, showTags: !!model.showTags && !!info && info.status === "available", canWrite,
    info: info ? {memoryBytes: info.memoryBytes, addressBits: info.addressBits || 32, status: info.status, tagBits: info.tagBits, granuleBytes: info.granuleBytes} : undefined,
    write: {selectionLimit: 256, ...writtenRange(model)}
  };
}
function view(model) {
  const connected = !!model.sessionId, permission = permissionView(model);
  const denied = reason(model.permission, connected);
  const base = {
    kind: model.kind, icon: KIND_ICONS[model.kind], title: KINDS[model.kind], instance: model.ordinal || 1,
    windowId: model.id, epoch: model.epoch, busy: !!model.busy, retired: !!model.retired,
    session: {id: model.sessionId, name: model.sessionName || "No active session", stopId: model.stopId,
      connected, everBound: !!model.sessionEverBound},
    permission, status: statusOf(model), banners: bannerView(model, denied, connected), footer: [],
    modules: model.modules, backend: model.backend
  };
  if (model.kind === "memory") {
    base.memory = memoryView(model);
    base.footer.push({icon: "symbol-numeric", text: "Module " + model.selector.module + " · Memory " + model.selector.memory});
    if (base.memory.info) {
      const size = BigInt(base.memory.info.memoryBytes);
      base.footer.push({icon: "database", text: (size >= 1048576n ? (size / 1048576n) + " MiB" : size >= 1024n ? (size / 1024n) + " KiB" : size + " bytes") + " · " + base.memory.info.addressBits + "-bit"});
      base.footer.push({icon: "tag", text: base.memory.info.status === "available" ? base.memory.info.tagBits + "-bit tags · " + base.memory.info.granuleBytes + "-byte granules" : "No allocation tags"});
    }
  } else if (model.kind === "breakpoints") {
    const snapshot = model.data;
    base.breakpoints = {
      available: snapshot ? snapshot.available !== false : undefined,
      state: snapshot ? snapshot.state : undefined,
      canWrite: permission.canWrite,
      rows: (snapshot && Array.isArray(snapshot.rows) ? snapshot.rows : []).map(row => ({
        id: row.id, kind: row.kind, level: row.level || (row.kind === "asm" ? "asm" : "wasm"), source: row.source,
        module: String(row.module), function: String(row.function), offset: String(row.offset),
        temporary: !!row.temporary, enabled: !!row.enabled, hits: String(row.hits), ignore: String(row.ignore), condition: row.condition || "",
        location: snapshot.locations && snapshot.locations[String(row.id)] || undefined,
        callbacks: model.callbackPages && model.callbackPages[row.id] || undefined
      }))
    };
  } else if (model.kind === "watch") {
    base.watch = {rows: model.watchRows || [], values: model.data || {}, children: model.watchChildren || {}};
  } else if (model.kind === "registers") {
    base.registers = {rows: model.data ? model.data.variables : [], frame: model.data ? model.data.frameName : undefined,
      available: !!model.data, canWrite: permission.canWrite};
  } else if (model.kind === "diagnostics") {
    base.diagnostics = {details: model.data || {}, text: model.diagnostics || (model.data && model.data.Status) || "",
      permission: model.permission, stopId: model.stopId, dumpPath: model.dumpPath || "", initialSection: model.focusDump ? "capture" : "overview",
      dumpReply: model.dumpReply, dumpPolicy: model.dumpPolicy, disclosure: model.dumpDisclosure,
      restorePath: model.restorePath || "", restoreEnvironment: model.restoreEnvironment || "current",
      restoreBindings: model.restoreBindings || "", restoreReply: model.restoreReply,
      replayCursor: model.replayCursor || "0",
      restorePending: !!model.restoreReply?.pending,
      recordingReply: model.recordingReply, historyReply: model.historyReply, threaded: !!model.threaded, hostInterfaces: model.hostInterfaces,
      canInspectHistory: ["full", "read-only"].includes(model.permission) && !!model.dumpPolicy?.allowed?.includes("full"),
      canRecord: model.permission === "full" && !!model.stopId && !model.retired && !!model.dumpPolicy?.allowed?.includes("full"),
      canRestore: model.permission === "full" && !!model.stopId && !model.retired,
      canDump: ["full", "read-only", "limited"].includes(model.permission) && !!model.stopId &&
        !!model.dumpPolicy?.allowed?.includes(model.dumpDisclosure), picking: !!model.picking};
  } else if (model.kind === "wasip1") {
    const data = model.data;
    base.wasip1 = {
      module: model.module, groups: WASI_GROUPS, icons: WASI_ICONS, active: model.wasiGroup,
      policy: data && data.policy ? {...groupRows(data.policy), total: data.policy.total, more: !!data.policy.more,
        status: data.policy.status, available: data.policy.available !== false, hasNext: data.policy.more, start: model.policyPage,
        back: !!(model.policyHistory && model.policyHistory.length)} : undefined,
      policyRows: data && data.policy ? policyRows(data.policy.rows) : [],
      group: data && data.environment ? {...groupRows(data.environment), total: data.environment.total, more: !!data.environment.more,
        status: data.environment.status, available: data.environment.available !== false,
        back: !!(model.groupHistory && model.groupHistory.length), journal: journalView(data.environment),
        calls: data.environment.calls || undefined,
        next: data.environment.next, start: data.environment.start} : undefined,
      payload: model.payload, exportPath: model.exportPath || "", exportResult: model.exportResult,
      payloadSelector: model.payloadSelector, picking: !!model.picking
    };
    base.footer.push({icon: "symbol-numeric", text: "Module " + model.module});
    if (data && data.policy) base.footer.push({icon: "law", text: data.policy.total + " policy rows"});
  } else if (model.kind === "wasithread") {
    const data = model.data && model.data.internal;
    base.wasithread = {active: model.threadGroup,
      view: data ? {...groupRows(data), total: data.total, next: data.next, more: !!data.more,
        start: data.start, available: data.available !== false, status: data.status,
        back: !!(model.groupHistory && model.groupHistory.length)} : undefined};
    base.footer.push({icon: "lock", text: "Launch authority is immutable; inspection never creates threads or replenishes counters"});
    base.footer.push({icon: "info", text: "Unassigned modules use Global. Explicit Global caps also cover Single and Group."});
  } else if (model.kind === "preloaddl") {
    base.preloaddl = model.data || model.hostInterfaces ? {...(model.data || model.hostInterfaces), back: !!model.groupHistory?.length} : undefined;
    base.footer.push({icon: "shield", text: "Read-only launch inventory · Native code and addresses are outside Wasm debugging"});
  } else if (model.kind === "scene") {
    const data = model.data && model.data.internal;
    base.scene = {groups: SCENE_GROUPS, active: model.sceneGroup, unavailable: model.data?.unavailable,
      exportPath: model.exportPath || "", exportResult: model.exportResult,
      view: data ? {...groupRows(data), total: data.total, next: data.next, more: !!data.more,
        journal: data.journal, calls: data.calls, start: data.start, available: data.available !== false, status: data.status,
        back: !!(model.groupHistory && model.groupHistory.length)} : undefined};
    base.footer.push({icon: "shield", text: "Read-only scene metadata"});
    base.footer.push({icon: "info", text: "Guest self reports are read-only after ROSv2 binding; other-module inspection and message routes require separate explicit authority"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && row.type.text === "module.call"))
      base.footer.push({icon: "info", text: "Registered message counts only; occupied slots retain cleanup responsibility. Quarantine means erasure could not be confirmed. Viewing never reads payloads or expires a call"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row?.type?.text === "module.call" && row.clear_after_copy === false))
      base.footer.push({icon: "warning", text: "Input clearing is disabled for a registered route. Copied requests remain in sender memory; response and broker cleanup remain enabled"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.platform && row.platform.text))
      base.footer.push({icon: "info", text: "Platform switches are u2rosv2 child configuration; enabling a module does not grant device authority"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && row.type.text === "transform"))
      base.footer.push({icon: "info", text: "Cached transform counts; inspection does not execute or expire queries"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && ["log", "diagnostic"].includes(row.type.text)))
      base.footer.push({icon: "info", text: "Cached log and diagnostic counts; messages and cursor contents are withheld"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && ["recording", "replay"].includes(row.type.text)))
      base.footer.push({icon: "info", text: "Private recording and replay counts; viewing does not read messages or advance replay cursors"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && ["device", "control"].includes(row.type.text)))
      base.footer.push({icon: "info", text: "Cached device/control state; a stop acknowledgement does not prove physical stopping. Viewing never pumps a command or renews a lease"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && row.type.text === "driver"))
      base.footer.push({icon: "info", text: "External driver session only. query_count=1 means an uncertain effect. Status is not physical safety evidence. VM disconnect never stops the shared driver; process lifecycle belongs to the external deployment"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && row.type.text === "component"))
      base.footer.push({icon: "info", text: "Cached isolated component state; status_code retains the first failure: 0 none, 1 admission, 2 launch, 3 startup protocol, 4 channel, 5 protocol, 6 deadline, 7 authority, 8 diagnostics, 9 backend. Stop and exit never clear it. Process exit does not establish physical safety"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && row.type.text === "bridge"))
      base.footer.push({icon: "info", text: "Cached bridge counts; viewing neither forwards messages nor settles source reservations"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && row.type.text === "media"))
      base.footer.push({icon: "info", text: "Cached raw media counts; viewing neither executes a codec nor consumes an output"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && row.type.text === "sensor"))
      base.footer.push({icon: "info", text: "Cached sensor ingress counts; viewing does not read a device or consume frames"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && row.type.text === "model.proposal"))
      base.footer.push({icon: "info", text: "Inert model proposal counts; viewing grants no approval and executes no proposed operation"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && row.type.text === "communication"))
      base.footer.push({icon: "info", text: "Cached worker-local registry counts; viewing performs no transport polling and reports no physical safety state"});
    if (data && Array.isArray(data.rows) && data.rows.some(row => row && row.type && row.type.text === "parameter"))
      base.footer.push({icon: "info", text: "Cached local parameter counts; viewing exposes no values, expires no validation and advances no cursor"});
  } else if (model.kind === "ros") {
    const data = model.data;
    base.ros = {
      backend: model.backend, groups: ROS_GROUPS, active: model.rosGroup, builds: model.hostInterfaces?.builds, unavailable: data?.unavailable,
      policy: data && data.policy ? {...groupRows(data.policy), total: data.policy.total, more: !!data.policy.more,
        back: !!(model.policyHistory && model.policyHistory.length), start: model.policyPage, status: data.policy.status, scope: data.policy.scope} : undefined,
      policyRows: data && data.policy ? policyRows(data.policy.rows) : [],
      view: data && data.internal ? {...groupRows(data.internal), total: data.internal.total, more: !!data.internal.more,
        back: !!(model.groupHistory && model.groupHistory.length), status: data.internal.status,
        available: data.internal.available !== false, journal: journalView(data.internal),
        calls: data.internal.calls || undefined, gap: data.internal.gap, scope: data.internal.scope,
        dropped: {busy: data.internal.dropped_busy, sequence: data.internal.dropped_sequence, full: data.internal.dropped_full}} : undefined,
      filters: {scope: model.policyScope || "all", target: model.policyTarget || "", alias: model.policyAlias || ""},
      payload: model.payload, exportPath: model.exportPath || "", exportResult: model.exportResult,
      payloadSelector: model.payloadSelector, picking: !!model.picking
    };
    base.footer.push({icon: "radio-tower", text: "u2rosv1 › " + (model.backend === "all" ? "ros1 + ros2" : model.backend)});
  }
  if (model.busy && !model.data) base.footer.push({icon: "sync", text: "Reading stopped runtime…", spin: true});
  return base;
}

module.exports = {PERMISSIONS, KINDS, KIND_ICONS, WASI_GROUPS, WASI_ICONS, ROS_GROUPS, SCENE_GROUPS, ROS_POLICY, COLUMN_WIDTHS,
  decimal, memoryAddress, page, extent, columns, memorySelection, decodeMemory, editBytes, reason, textValue, fragment, policyRows,
  groupRows, journalView, view, CONTROL_NAMES};
