"use strict";

// Position projection is deliberately tested without a VS Code process.  The
// adapter endpoint is the only source of truth: a source/Wasm stop may acquire
// a source row from stackTrace, while an assembly row is always the yellow
// authenticated LLVM-JIT projection and never the green next marker.
const assert = require("assert");
const path = require("path");
const {register} = require(path.join(__dirname, "../../tools/debug/positions.js"));

const trackers = [], editors = [], decorations = [];
let tabsChanged, sessionChanged;
class ThemeColor { constructor(id) { this.id = id; } }
class Range { constructor(...args) { this.args = args; } }
const vscode = {
  Uri: {file: p => ({scheme: "file", fsPath: p, toString: () => "file://" + p})},
  ThemeColor, OverviewRulerLane: {Full: 1}, DecorationRangeBehavior: {ClosedClosed: 1}, Range,
  TextEditorRevealType: {InCenterIfOutsideViewport: 1},
  window: {visibleTextEditors: editors,
    createTextEditorDecorationType: options => {const value = {options}; decorations.push(value); return value;},
    onDidChangeVisibleTextEditors: () => ({dispose() {}}), tabGroups: {all: [], onDidChangeTabs: fn => (tabsChanged = fn, {dispose() {}})}},
  commands: {executeCommand: async () => {}},
  debug: {activeDebugSession: null, breakpoints: [],
    registerDebugAdapterTrackerFactory: (type, factory) => {assert.strictEqual(type, "uwvm-llvm-full"); trackers.push(factory); return {dispose() {}};},
    onDidStartDebugSession: () => ({dispose() {}}), onDidChangeActiveDebugSession: fn => (sessionChanged = fn, {dispose() {}}),
    onDidTerminateDebugSession: () => ({dispose() {}}), onDidChangeBreakpoints: () => ({dispose() {}})}
};
const context = {extensionPath: path.resolve(__dirname, "../../tools/debug"), subscriptions: []};
const sessions = new Map();
const positions = register(vscode, context, sessions, {positions() {}});

const sourceEditor = {document: {uri: {scheme: "file", fsPath: "/tmp/probe.cc"}, lineCount: 6,
  lineAt: line => ({text: "line " + (line + 1)})}, revealRange() {}, setDecorations(type, rows) {type.sourceRows = rows;}};
const asmEditor = {document: {uri: {scheme: "debug", fsPath: "/tmp/stop.asm"}, lineCount: 2,
  lineAt: line => ({text: line === 0 ? "0x1234  mov rax, rbx" : "0x1240  ret"})}, revealRange() {},
  setDecorations(type, rows) {type.asmRows = rows;}};
editors.push(sourceEditor, asmEditor);

const session = {id: "s", type: "uwvm-llvm-full", position: {state: "stopped", threadId: 1,
  wasm: {module: "0", function: "1", offset: "4", exact: true},
  asm: {instructionReference: "uwvm-native-code:1"}, asmPc: "0x1234"},
  async customRequest(command) {
    if (command === "uwvm/positions") return this.position;
    if (command === "stackTrace") return {stackFrames: [{source: {path: "/tmp/probe.cc"}, line: 3, column: 1, uwvmSourceExact: false}]};
    return {instructions: [{address: "0x1234"}]};
  }};
sessions.set(session.id, session); vscode.debug.activeDebugSession = session;
const tracker = trackers[0].createDebugAdapterTracker(session);

tracker.onDidSendMessage({type: "event", event: "stopped"});
setTimeout(async () => {
  assert.strictEqual(decorations.length, 1, "source has only a nonvisual hint decoration");
  const hint = decorations[0];
  assert.deepStrictEqual(hint.options, {}, "no second gutter, line background, column marker or color");
  assert.strictEqual(hint.sourceRows.length, 1);
  assert(hint.sourceRows[0].hoverMessage.includes("inside this source statement"));
  assert.strictEqual(hint.asmRows.length, 0);
  for (const exact of [true, false]) {
    session.position = {...session.position, source: {path: "/tmp/probe.cc", line: 3, column: 5, exact}};
    await positions.update(session);
    assert.strictEqual(hint.sourceRows.length, 1);
    assert(hint.sourceRows[0].hoverMessage.includes(exact ? "At the start" : "inside this"));
    assert.deepStrictEqual(hint.options, {});
  }
  session.position = {...session.position, source: {path: "/tmp/probe.cc", line: 9, column: 5, exact: false}};
  await positions.update(session);
  assert.strictEqual(hint.sourceRows.length, 0, "invalid source row gets no position hint");
  session.position = {...session.position, source: {path: "/tmp/probe.cc", line: 3, column: 5, exact: false}};
  await positions.update(session);
  tracker.onDidSendMessage({type: "event", event: "continued"});
  assert.strictEqual(hint.sourceRows.length, 0, "resuming retires the source hint");
  await positions.update(session);
  const calls = [];
  session.customRequest = async (command, args) => calls.push([session.id, command, args]);
  const other = {id: "other", type: session.type, customRequest: async (command, args) => calls.push(["other", command, args])};
  sessions.set(other.id, other);
  vscode.window.tabGroups.all = [{tabs: [{label: "Disassembly", isActive: true}]}];
  tabsChanged(); await new Promise(resolve => setImmediate(resolve));
  assert.deepStrictEqual(calls, [["s", "uwvm/disassemblyView", {visible:true}], ["other", "uwvm/disassemblyView", {visible:false}]]);
  calls.length = 0; vscode.debug.activeDebugSession = other;
  sessionChanged(); await new Promise(resolve => setImmediate(resolve));
  assert.deepStrictEqual(calls, [["s", "uwvm/disassemblyView", {visible:false}], ["other", "uwvm/disassemblyView", {visible:true}]]);
  calls.length = 0; tabsChanged(); await new Promise(resolve => setImmediate(resolve));
  assert.strictEqual(calls.length, 0, "unchanged tabs do not re-invalidate any session");
  vscode.window.tabGroups.all[0].tabs[0].isActive = false;
  tabsChanged(); await new Promise(resolve => setImmediate(resolve));
  assert.deepStrictEqual(calls, [["other", "uwvm/disassemblyView", {visible:false}]], "a retained inactive ASM tab cannot own navigation");
  assert.strictEqual(decorations.length, 1, "session/ASM changes cannot allocate another source marker");
  console.log("PASS native-only source marker ownership, exactness hints and ASM/session isolation");
}, 100);
