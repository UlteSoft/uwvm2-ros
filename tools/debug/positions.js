"use strict";

// Execution position at every level. The step level (S/W/A) only chooses
// what F10/F11 step over; the stop itself has a position in source, Wasm and
// assembly at the same time. A position is EXACT when the stop is precisely
// at that level's instruction/statement start, and NEXT when execution is
// inside it (for example a native stop inside a Wasm instruction, or a Wasm
// stop inside a source statement).
//
// Native source arrows, breakpoint composition and line/column highlights
// belong exclusively to VS Code. Its yellow/green colors identify the top
// frame/selected caller, not statement exactness. Keep exactness in the
// authenticated position hint; never fabricate callers to obtain a color.
const path = require("path");

function register(vscode, context, sessions, wasmView) {
  if (!vscode.window || typeof vscode.window.createTextEditorDecorationType !== "function" || !vscode.debug.registerDebugAdapterTrackerFactory) return undefined;
  const sourceHintType = vscode.window.createTextEditorDecorationType({});
  context.subscriptions.push(sourceHintType);
  const latest = new Map(), serial = new Map();
  const disassemblySessions = new Map();
  const add = value => context.subscriptions.push(value);
  const same = (a, b) => {
    const left = path.resolve(a), right = path.resolve(b);
    return process.platform === "win32" ? left.toLowerCase() === right.toLowerCase() : left === right;
  };

  // DAP source paths are absolute host paths.  A few IDEs return a file URI
  // for the same editor, however, and comparing the two strings would make
  // the source marker silently disappear.  Normalize both forms before the
  // marker decision; this does not grant any file access.
  function sourcePath(value) {
    if (typeof value !== "string" || !value) return undefined;
    if (value.startsWith("file://")) {
      try {
        if (vscode.Uri && typeof vscode.Uri.parse === "function") {
          const parsed = vscode.Uri.parse(value);
          if (parsed.fsPath) return parsed.fsPath;
        }
        const pathname = decodeURIComponent(new URL(value).pathname);
        return /^\/[A-Za-z]:\//.test(pathname) ? pathname.slice(1) : pathname;
      } catch (_) { return undefined; }
    }
    return value;
  }

  function sourceTarget(pos) {
    // Only authenticated source projections qualify for position hints.
    if (!pos || pos.state !== "stopped" || !pos.source) return undefined;
    return pos.source;
  }
  async function stackProjection(session, pos) {
    // Some older/live runtimes publish the authenticated source location only
    // in the top DAP frame.  Keep the position endpoint authoritative when it
    // has the data, but recover that same stop-bound source projection from
    // stackTrace when it does not.  This is a debugger query only; no runtime
    // execution path is involved.
    if (!session || !pos || pos.state !== "stopped" || pos.source ||
        !Number.isSafeInteger(pos.threadId) || pos.threadId < 1) return pos;
    try {
      const trace = await session.customRequest("stackTrace", {threadId: pos.threadId, startFrame: 0, levels: 1});
      const frames = trace?.body?.stackFrames || trace?.stackFrames;
      const frame = Array.isArray(frames) ? frames[0] : undefined;
      const source = frame?.uwvmSourceLocation || frame?.source;
      const line = frame?.uwvmSourceLocation?.line || frame?.line;
      if (!source || typeof source.path !== "string" ||
          !Number.isSafeInteger(line) || line < 1) return pos;
      const exact = typeof frame.uwvmSourceExact === "boolean" ? frame.uwvmSourceExact :
        typeof frame.uwvmPosition?.source?.exact === "boolean" ? frame.uwvmPosition.source.exact :
        // A cooperative Wasm stop is still a valid source projection if its
        // statement-start bit is absent. Keep the hint conservative; never
        // infer exactness from the native arrow color or selected step level.
        false;
      return {...pos, source: {path: source.path, line, column: source.column || frame.column || 1, exact}};
    } catch (_) { return pos; }
  }
  function paint() {
    const active = vscode.debug.activeDebugSession, pos = active ? latest.get(active.id) : undefined;
    const target = sourceTarget(pos), targetPath = sourcePath(target && target.path);
    for (const editor of vscode.window.visibleTextEditors || []) {
      const hints = [];
      if (targetPath && editor.document.uri.scheme === "file" &&
          same(editor.document.uri.fsPath, targetPath) && Number.isSafeInteger(target.line) &&
          target.line >= 1 && target.line <= editor.document.lineCount) {
        hints.push({range: new vscode.Range(target.line - 1, 0, target.line - 1, editor.document.lineAt(target.line - 1).text.length),
          hoverMessage: (target.exact ? "At the start of this source statement" : "Execution is inside this source statement") +
            " · source " + target.line + ":" + (target.column || 1) +
            ". Changing step level does not execute the program or change this position's precision."});
      }
      editor.setDecorations(sourceHintType, hints);
    }
  }
  async function update(session) {
    if (sessions.get(session.id) !== session) return;
    const ticket = (serial.get(session.id) || 0) + 1; serial.set(session.id, ticket);
    let pos;
    try { pos = await session.customRequest("uwvm/positions", {}); } catch (_) { pos = undefined; }
    if (sessions.get(session.id) !== session || serial.get(session.id) !== ticket) return;
    // Cooperative Wasm/source stops do not expose a live native PC in the
    // runtime snapshot. Resolve one instruction through its opaque,
    // owner-authenticated DAP reference even before Disassembly is visible.
    // This guarantees that every source/Wasm stop has a precise ASM landing;
    // no user supplied address reaches this path.
    if (pos && pos.state === "stopped" && pos.asm?.instructionReference && !pos.asmPc) {
      try {
        let ref = pos.asm?.instructionReference;
        if (!ref) {
          const trace = await session.customRequest("stackTrace", {threadId: pos.threadId, startFrame: 0, levels: 1});
          ref = trace?.stackFrames?.[0]?.instructionPointerReference;
        }
        if (typeof ref === "string" && /^uwvm-native-code:[1-9][0-9]*$/.test(ref)) {
          const page = await session.customRequest("disassemble", {memoryReference: ref, instructionCount: 1,
            offset: 0, instructionOffset: 0, resolveSymbols: false});
          const address = page?.instructions?.[0]?.address;
          if (typeof address === "string" && /^0x[0-9a-f]+$/i.test(address)) pos = {...pos, asmPc: address.toLowerCase()};
        }
      } catch (_) { /* unavailable code view: do not invent an assembly point */ }
    }
    if (sessions.get(session.id) !== session || serial.get(session.id) !== ticket) return;
    pos = await stackProjection(session, pos);
    if (sessions.get(session.id) !== session || serial.get(session.id) !== ticket) return;
    latest.set(session.id, pos);
    paint();
    if (wasmView && typeof wasmView.positions === "function") wasmView.positions(session, pos);
  }
  function clear(session) {
    serial.set(session.id, (serial.get(session.id) || 0) + 1);
    latest.delete(session.id); paint();
    if (wasmView && typeof wasmView.positions === "function") wasmView.positions(session, undefined);
  }
  add(vscode.debug.registerDebugAdapterTrackerFactory("uwvm-llvm-full", {createDebugAdapterTracker(session) {
    let timer;
    const later = () => { clearTimeout(timer); timer = setTimeout(() => void update(session), 60); };
    return {onDidSendMessage(packet) {
      if (packet.type !== "event") return;
      if (packet.event === "stopped" || packet.event === "uwvm/stepLevel" || packet.event === "invalidated") later();
      else if (["continued", "terminated", "exited"].includes(packet.event)) { clearTimeout(timer); clear(session); }
    }};
  }}));
  if (vscode.window.onDidChangeVisibleTextEditors) add(vscode.window.onDidChangeVisibleTextEditors(() => paint()));
  if (vscode.debug.onDidChangeBreakpoints) add(vscode.debug.onDidChangeBreakpoints(() => paint()));
  // While VS Code's Disassembly view is open, the adapter gives the top frame
  // its owned JIT code reference at every step level, so the view shows the
  // exact assembly position of a Wasm or source stop too.
  const disassemblyVisible = () => (vscode.window.tabGroups ? vscode.window.tabGroups.all : [])
    .some(group => group.tabs.some(tab => tab.isActive && tab.input === undefined && /^(Disassembly|反汇编)$/i.test(tab.label)));
  async function syncDisassembly() {
    const open = disassemblyVisible();
    const active = vscode.debug.activeDebugSession;
    for (const session of sessions.values()) {
      const visible = open && session === active;
      if (disassemblySessions.get(session.id) === visible) continue;
      disassemblySessions.set(session.id, visible);
      try { await session.customRequest("uwvm/disassemblyView", {visible}); }
      catch (_) { if (disassemblySessions.get(session.id) === visible) disassemblySessions.delete(session.id); }
    }
  }
  if (vscode.window.tabGroups && vscode.window.tabGroups.onDidChangeTabs) add(vscode.window.tabGroups.onDidChangeTabs(() => void syncDisassembly()));
  if (vscode.debug.onDidStartDebugSession) add(vscode.debug.onDidStartDebugSession(session => {
    if (session.type === "uwvm-llvm-full") void syncDisassembly();
  }));
  if (vscode.debug.onDidChangeActiveDebugSession) add(vscode.debug.onDidChangeActiveDebugSession(() => { paint(); void syncDisassembly(); }));
  add(vscode.debug.onDidTerminateDebugSession(session => { latest.delete(session.id); disassemblySessions.delete(session.id); paint(); }));
  return {latest, update};
}

module.exports = {register};
