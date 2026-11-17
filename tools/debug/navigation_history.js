"use strict";

// Navigation stores view coordinates, never copied values, stop capabilities,
// or executable commands. Each live DAP session has a separate bounded trail.
const controllers = new WeakMap();
const sessionControllers = new WeakMap();
class Trail {
  constructor(limit = 256) { this.limit = limit; this.rows = []; this.cursor = -1; }
  visit(entry, replace = false) {
    if (this.rows[this.cursor]?.key === entry.key || replace && this.rows[this.cursor]?.target === entry.target) this.rows[this.cursor] = entry;
    else {
      this.rows.splice(this.cursor + 1); this.rows.push(entry);
      if (this.rows.length > this.limit) this.rows.shift();
      this.cursor = this.rows.length - 1;
    }
  }
  get state() { return {back: this.cursor > 0, forward: this.cursor + 1 < this.rows.length}; }
}
function create(vscode, context, sessions) {
  const trails = new Map(), endpoints = new Map(), listeners = new Set();
  let serial = 0, traveling = false;
  const active = () => { const s = vscode.debug.activeDebugSession; return s && sessions.get(s.id) === s ? s : undefined; };
  const trail = s => { if (!trails.has(s)) trails.set(s, new Trail()); return trails.get(s); };
  const state = s => s ? trail(s).state : {back: false, forward: false};
  let published;
  function publish() {
    const current = state(active());
    const signature = JSON.stringify([active()?.id,...[...trails].map(([s,t])=>[s.id,t.state])]);
    if(signature===published)return;published=signature;
    void vscode.commands.executeCommand("setContext", "uwvm.canNavigateBack", current.back);
    void vscode.commands.executeCommand("setContext", "uwvm.canNavigateForward", current.forward);
    for (const fn of listeners) fn();
  }
  function visit(s, target, position, replace = false) {
    if (traveling || !s || sessions.get(s.id) !== s || !endpoints.has(target)) return;
    const key = target + ":" + JSON.stringify(position);
    trail(s).visit({key, target, position}, replace); publish();
  }
  function register(s, show, capture) {
    sessionControllers.set(s, api);
    const id = ++serial;
    endpoints.set(id, {s, show, capture});
    return {visit: (position, replace) => visit(s, id, position, replace),
      dispose: () => {
        endpoints.delete(id);
        const t = trails.get(s);
        if (t) { const before = t.rows.slice(0, t.cursor + 1).filter(e => e.target !== id).length;
          t.rows = t.rows.filter(e => e.target !== id); t.cursor = before - 1; }
        publish();
      }};
  }
  async function move(direction, s = active()) {
    if (!s || s !== active()) throw new Error("Select this navigation history's live debug session.");
    if (traveling) return;
    const t = trail(s), wanted = t.cursor + direction;
    if (wanted < 0 || wanted >= t.rows.length) return;
    const current = t.rows[t.cursor], origin = current && endpoints.get(current.target);
    if (origin?.capture) { const position = origin.capture(); if (position !== undefined) t.visit({...current, position, key: current.target + ":" + JSON.stringify(position)}, true); }
    const entry = t.rows[wanted], endpoint = endpoints.get(entry.target);
    if (!endpoint || endpoint.s !== s) throw new Error("That debug view has been closed. Open it again to start a new navigation trail.");
    traveling = true;
    const valid = require("./navigation_state").begin(s);
    try {
      await endpoint.show(entry.position, () => valid() && active() === s && sessions.get(s.id) === s);
      if (valid() && active() === s && sessions.get(s.id) === s) t.cursor = wanted;
    } catch (error) {
      // A retired native location must not permanently block older views.
      // Drop only the failed destination; no fallback reads or execution.
      t.rows.splice(wanted, 1); if (wanted <= t.cursor) t.cursor--;
      throw error;
    } finally { traveling = false; publish(); }
  }
  const api = {register, state, move,
    trackTab(s, entry, match, resolve) {
      let checking = false;
      const changed = async () => {
        if (checking || traveling || active() !== s || !match(vscode.window.tabGroups?.activeTabGroup?.activeTab)) return;
        checking = true;
        const valid = require("./navigation_state").current(s);
        try { const position = await resolve();
          if (valid() && active() === s && match(vscode.window.tabGroups?.activeTabGroup?.activeTab)) entry.visit(position);
        } catch (_) { /* A tab selection grants no unavailable code authority. */ }
        finally { checking = false; }
      };
      add(vscode.window.tabGroups?.onDidChangeTabs?.(changed));
      add(vscode.window.tabGroups?.onDidChangeTabGroups?.(changed));
    },
    subscribe(fn) { listeners.add(fn); return {dispose: () => listeners.delete(fn)}; }};
  controllers.set(sessions, api);
  const add = x => x && context.subscriptions.push(x);
  for (const [command, direction] of [["uwvm.navigateBack", -1], ["uwvm.navigateForward", 1]])
    add(vscode.commands.registerCommand(command, () => move(direction)));
  add(vscode.debug.onDidChangeActiveDebugSession?.(publish));
  add(vscode.debug.onDidStartDebugSession?.(s => { if (s.type === "uwvm-llvm-full") sessionControllers.set(s, api); }));
  add(vscode.debug.onDidTerminateDebugSession(s => { trails.delete(s); sources.delete(s); for (const [id, e] of endpoints) if (e.s === s) endpoints.delete(id); publish(); }));
  // Native source files participate in the same trail. Only already-open local
  // text editors are registered; browsing does not turn a file into guest state.
  const sources = new Map();
  function source(editor, selection = false) {
    const s = active(); if (!s || !editor || editor.document.uri.scheme !== "file") return;
    let byUri = sources.get(s); if (!byUri) sources.set(s, byUri = new Map());
    const key = editor.document.uri.toString(); let entry = byUri.get(key);
    const capture = () => { const e = vscode.window.visibleTextEditors?.find(e => e.document.uri.toString() === key); if (!e) return;
      return {line: e.selection.active.line, column: e.selection.active.character, viewColumn: e.viewColumn,
        top: e.visibleRanges?.[0]?.start.line}; };
    if (!entry) {
      entry = register(s, async (p, valid) => {
        const doc = await vscode.workspace.openTextDocument(editor.document.uri); if (!valid()) return;
        const line = Math.min(p.line, doc.lineCount - 1), col = Math.min(p.column, doc.lineAt(line).text.length), at = new vscode.Position(line, col);
        const shown = await vscode.window.showTextDocument(doc, {preview: false, viewColumn: p.viewColumn, selection: new vscode.Range(at, at)});
        if (valid() && Number.isInteger(p.top)) { const top = new vscode.Position(Math.min(p.top, doc.lineCount - 1), 0);
          shown.revealRange(new vscode.Range(top, top), vscode.TextEditorRevealType.AtTop); }
      }, capture); byUri.set(key, entry);
    }
    const p = capture(); if (p) entry.visit(p, selection);
  }
  add(vscode.window.onDidChangeActiveTextEditor?.(e => source(e)));
  add(vscode.window.onDidChangeTextEditorSelection?.(e => { if (e.textEditor === vscode.window.activeTextEditor)
    source(e.textEditor, e.kind !== vscode.TextEditorSelectionChangeKind?.Command); }));
  add({dispose() { trails.clear(); endpoints.clear(); sources.clear(); listeners.clear(); controllers.delete(sessions); }});
  publish();
  return api;
}
module.exports = {Trail, create, get: sessions => controllers.get(sessions), forSession: s => sessionControllers.get(s)};
