"use strict";

// "UWVM" view in Run and Debug. A grouped launcher with live session state:
// every inspector has a visible entry; unavailable ones explain why inline.
// No VM call is made until a real session exists; every action still goes
// through the authenticated adapter.
function register(vscode, context, sessions, inspection, wasmView) {
  if (!vscode.window || !vscode.window.registerTreeDataProvider || !vscode.EventEmitter) return;
  const changed = new vscode.EventEmitter(), permissions = new Map(), levels = new Map(), locks = new Map(), states = new Map(), hostBuilds = new Map();
  const active = () => { const s = vscode.debug.activeDebugSession; return s && sessions.get(s.id) === s ? s : undefined; };
  const navigation = require("./navigation_state");
  const LEVELS = {source: {letter: "S", label: "Source", icon: "file-code", detail: "Step source statements (needs DWARF)"},
    wasm: {letter: "W", label: "Wasm", icon: "symbol-method", detail: "Step single Wasm instructions"},
    native: {letter: "A", label: "Assembly", icon: "chip", detail: "Step owned JIT machine instructions"}};
  const statusItem = vscode.window.createStatusBarItem ? vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Left, 40) : undefined;
  if (statusItem) { statusItem.command = "uwvm.switchLevel"; context.subscriptions.push(statusItem); }
  const publish = () => {
    const session = active(), level = session ? levels.get(session.id) : undefined;
    void vscode.commands.executeCommand("setContext", "uwvm.stepLevel", level);
    void vscode.commands.executeCommand("setContext", "uwvm.stepLevelLocked", session ? locks.get(session.id) === true : false);
    void vscode.commands.executeCommand("setContext", "uwvm.active", !!session);
    void vscode.commands.executeCommand("setContext", "uwvm.permission", session ? permissions.get(session.id) : undefined);
    if (statusItem) {
      if (session && level) {
        const item = LEVELS[level], permission = permissions.get(session.id);
        statusItem.text = (locks.get(session.id) ? "$(lock) " : "") + "$(" + item.icon + ") " + item.letter + " · " + item.label;
        statusItem.tooltip = new vscode.MarkdownString("**UWVM step level: " + item.label + "**\n\n" + item.detail + "\n\nPermission: " + (permission || "checking") + "\n\nClick to change the level used by F10 / F11.");
        statusItem.show();
      } else statusItem.hide();
    }
    changed.fire();
  };
  const groups = [
    {id: "level", label: "Step level", icon: "debug-step-into", tooltip: "Granularity of Step Over / Step Into / Step Out"},
    {id: "code", label: "Code & breakpoints", icon: "code"},
    {id: "state", label: "Guest state", icon: "database"},
    {id: "host", label: "Host interfaces", icon: "plug"},
    {id: "session", label: "Session", icon: "debug-alt"}
  ];
  const items = {
    code: [
      {label: "Wasm instructions", command: "uwvm.openWasmInstructions", icon: "symbol-method", tooltip: "Instruction listing of the current Wasm function", access: "inspect", stop: true},
      {label: "Breakpoints", command: "uwvm.openBreakpoints", icon: "debug-breakpoint-conditional", tooltip: "Every breakpoint from the editor gutter and the console, with conditions, hit counts and callbacks", access: "inspect"},
      {label: "Watch", command: "uwvm.openWatch", icon: "eye", tooltip: "Independent Source, Wasm and Assembly watches; the native WATCH list is unchanged", access: "inspect"}
    ],
    state: [
      {label: "Memory", command: "uwvm.openMemory", icon: "database", tooltip: "Linear memory bytes, allocation tags and typed values; edit in place with full permission", access: "inspect", stop: true},
      {label: "Registers", command: "uwvm.openRegisters", icon: "symbol-numeric", tooltip: "Machine registers at an owned native instruction stop", access: "inspect", stop: true}
    ],
    host: [
      {label: "WASI", command: "uwvm.openWasip1", icon: "terminal", tooltip: "Arguments, environment, descriptors, preopens and the launch policy", access: "inspect", stop: true},
      {label: "WASI Threads", command: "uwvm.openWasithread", icon: "list-tree", tooltip: "Independent I1T authority, module membership and atomic thread accounting", access: "inspect", stop: true},
      {label: "u2rosv1", command: "uwvm.openRos", icon: "radio-tower", tooltip: "u2rosv1 › ros1 / ros2: policy, runtime state and recorded calls", access: "inspect", stop: true},
      {label: "u2rosv2 · Robot scene", command: "uwvm.openScene", icon: "shield", tooltip: "u2rosv2 (in development): read-only scene authority and resource metadata", access: "inspect", stop: true},
      {label: "preloaddl", command: "uwvm.openPreloadDl", icon: "file-binary", tooltip: "Actual build availability and loaded native libraries. Read-only; never enables native code debugging.", access: "inspect"}
    ],
    session: [
      {label: "Diagnostics", command: "uwvm.showDiagnostics", icon: "pulse", tooltip: "Runtime state, stop reason and participants", access: "diagnostics"},
      {label: "Save Wasm dump…", command: "uwvm.dump", icon: "save", tooltip: "Write the stopped state to a file on the VM host", access: "diagnostics", stop: true},
      {label: "Hot update…", command: "uwvm.submitSignedUpdate", icon: "cloud-upload", tooltip: "Submit an existing signed update frame. Independent of debugger permission.", access: "independent"}
    ]
  };
  function denial(row, session, permission) {
    if (row.access === "independent") return undefined;
    if (!session) return {short: "no session", long: "Start or attach a UWVM debug configuration first."};
    if (!permission) return undefined;
    if (permission === "none") return {short: "disabled", long: "This VM was launched with debugger permission none."};
    if (row.access === "inspect" && permission === "limited") return {short: "limited", long: "Limited permission only exposes numeric diagnostics. Relaunch with full or read-only."};
    return undefined;
  }
  // Element objects are stable: VS Code keeps expansion state per element and
  // reveal() needs identity. Live session data is read in getTreeItem.
  const headRow = {kind: "session"};
  const groupRows = groups.map(group => ({kind: "group", ...group}));
  const levelRows = [{kind: "level-lock", parent: groupRows[0]}, ...Object.entries(LEVELS).map(([level, item]) => ({kind: "level", level, ...item, parent: groupRows[0]}))];
  const itemRows = Object.fromEntries(Object.entries(items).map(([id, list]) => [id, list.map(row => ({kind: "item", ...row, parent: groupRows.find(g => g.id === id)}))]));
  function live(row) {
    const session = active(), permission = session ? permissions.get(session.id) : undefined;
    const current = session ? levels.get(session.id) : undefined;
    return {...row, session, permission, state: session ? states.get(session.id) : undefined, current: row.level !== undefined && current === row.level};
  }
  const provider = {
    onDidChangeTreeData: changed.event,
    getChildren(element) {
      if (!element) return active() ? [headRow, ...groupRows] : [];
      if (element.kind === "group" && element.id === "level") return levelRows;
      if (element.kind === "group") return itemRows[element.id] || [];
      return [];
    },
    getParent(element) { return element && element.parent ? element.parent : undefined; },
    getTreeItem(stable) {
      const row = live(stable);
      if (row.kind === "session") {
        const item = new vscode.TreeItem(row.session ? row.session.name : "No UWVM session", vscode.TreeItemCollapsibleState.None);
        const state = row.session ? states.get(row.session.id) : undefined;
        item.description = row.session ? [row.permission ? {full: "full", "read-only": "read-only", limited: "limited", none: "none"}[row.permission] : "checking…", state].filter(Boolean).join(" · ") : "Start a debug session";
        item.iconPath = new vscode.ThemeIcon(row.session ? (state === "paused" ? "debug-pause" : state === "running" ? "debug-start" : "debug-alt") : "debug-disconnect",
          row.session && state === "paused" ? new vscode.ThemeColor("debugIcon.pauseForeground") : undefined);
        item.tooltip = row.session ? "Permission: " + (row.permission || "checking") + "\nThe permission is fixed when the VM launches." : "Choose a UWVM configuration in Run and Debug, then press F5.";
        if (!row.session) item.command = {command: "workbench.view.debug", title: "Run and Debug"};
        item.contextValue = "uwvm.session";
        return item;
      }
      if (row.kind === "group") {
        // The step level is also on the status bar and the debug toolbar, so
        // its group starts collapsed and the inspectors stay in view.
        const item = new vscode.TreeItem(row.label, row.id === "level" ? vscode.TreeItemCollapsibleState.Collapsed : vscode.TreeItemCollapsibleState.Expanded);
        item.id = "uwvm.group." + row.id;
        item.iconPath = new vscode.ThemeIcon(row.icon);
        if (row.tooltip) item.tooltip = row.tooltip;
        item.contextValue = "uwvm.group";
        return item;
      }
      if (row.kind === "level-lock") {
        const locked = row.session && locks.get(row.session.id) === true;
        const item = new vscode.TreeItem(locked ? "Unlock step level" : "Lock step level", vscode.TreeItemCollapsibleState.None);
        item.id = "uwvm.level.lock"; item.iconPath = new vscode.ThemeIcon(locked ? "lock" : "unlock");
        item.description = locked ? "locked" : "unlocked";
        item.tooltip = "Keep the selected S / W / A stepping level. Breakpoint hits still reveal their own view. Unlock before changing levels.";
        item.command = {command: "uwvm.toggleStepLock", title: item.label};
        return item;
      }
      if (row.kind === "level") {
        const item = new vscode.TreeItem(row.letter + "  " + row.label, vscode.TreeItemCollapsibleState.None);
        item.description = row.current ? "active" : "";
        item.iconPath = new vscode.ThemeIcon(row.current ? "pass-filled" : "circle-large-outline", row.current ? new vscode.ThemeColor("debugIcon.startForeground") : undefined);
        item.tooltip = row.detail + (row.current ? " (active)" : "");
        const locked = row.session && locks.get(row.session.id) === true;
        const allowed = row.session && ["full", "read-only"].includes(row.permission) && !locked;
        item.command = allowed ? {command: {source: "uwvm.levelSource", wasm: "uwvm.levelWasm", native: "uwvm.levelAsm"}[row.level], title: row.label}
          : {command: "uwvm.explainUnavailable", title: row.label, arguments: [locked ? "Step level is locked. Use the first item to unlock it." : row.session ? "Changing the step level needs full or read-only permission." : "Start or attach a UWVM debug configuration first."]};
        if (!allowed) item.description = locked ? (row.current ? "active · locked" : "locked") : row.session ? "not permitted" : "";
        return item;
      }
      const item = new vscode.TreeItem(row.label, vscode.TreeItemCollapsibleState.None);
      item.iconPath = new vscode.ThemeIcon(row.icon);
      const denied = denial(row, row.session, row.permission);
      item.tooltip = new vscode.MarkdownString((denied ? "$(lock) " + denied.long + "\n\n" : "") + row.tooltip, true);
      if (denied) {
        item.description = denied.short;
        item.command = {command: "uwvm.explainUnavailable", title: row.label, arguments: [denied.long]};
      } else {
        if (row.stop && row.session && row.state === "running") item.description = "pause to read";
        const built = row.session && hostBuilds.get(row.session.id);
        const feature = {"uwvm.openRos": "u2rosv1", "uwvm.openScene": "u2rosv2", "uwvm.openPreloadDl": "preloaddl"}[row.command];
        if (built && feature && (feature === "u2rosv1" ? !built["u2rosv1-ros1"] && !built["u2rosv1-ros2"] : built[feature] === false)) item.description = "not built";
        item.command = {command: row.command, title: row.label};
      }
      item.contextValue = "uwvm.tool";
      return item;
    }
  };
  const tree = vscode.window.createTreeView ? vscode.window.createTreeView("uwvm.tools", {treeDataProvider: provider}) : undefined;
  context.subscriptions.push(changed, tree || vscode.window.registerTreeDataProvider("uwvm.tools", provider));
  // Extension views in Run and Debug start collapsed. Reveal the UWVM view
  // once per workspace when the first UWVM session starts, without taking
  // keyboard focus; a later collapse by the user is respected.
  async function revealOnce() {
    const memento = context.workspaceState;
    if (!tree || !memento || memento.get("uwvm.toolsRevealed")) return;
    try { await tree.reveal(headRow, {select: false, focus: false}); await memento.update("uwvm.toolsRevealed", true); } catch (_) { }
  }
  if (vscode.debug.onDidStartDebugSession) context.subscriptions.push(vscode.debug.onDidStartDebugSession(session => {
    if (session.type === "uwvm-llvm-full") { publish(); setTimeout(() => void revealOnce(), 300); }
  }));
  context.subscriptions.push(vscode.commands.registerCommand("uwvm.explainUnavailable", text => vscode.window.showInformationMessage(text)));
  for (const [command, level] of [["uwvm.levelSource", "source"], ["uwvm.levelWasm", "wasm"], ["uwvm.levelAsm", "native"]])
    context.subscriptions.push(vscode.commands.registerCommand(command, async () => {
      const session = active(); if (!session) throw new Error("Start or select a UWVM debug session first.");
      const isCurrent = navigation.begin(session);
      pendingNativeReveals.delete(session.id);
      const reply = await session.customRequest("uwvm/stepLevel", {level});
      if (!isCurrent() || active() !== session) return;
      if (reply?.level !== level) throw new Error("The adapter did not acknowledge the selected step level.");
      levels.set(session.id, level); publish();
      if (active() !== session) return;
      if (level === "source") {
        const location = await session.customRequest("uwvm/codeLocation", {level: "source"});
        await require("./code_navigation.js").reveal(vscode, session, location, {isCurrent});
      }
      if (level === "wasm" && wasmView) await wasmView.open({isCurrent});
      if (level === "native") {
        const location = await session.customRequest("uwvm/codeLocation", {level: "native"});
        await require("./code_navigation.js").reveal(vscode, session, location, {isCurrent});
      }
    }));
  context.subscriptions.push(vscode.commands.registerCommand("uwvm.toggleStepLock", async () => {
    const session = active(); if (!session) throw new Error("Start or select a UWVM debug session first.");
    const before = await session.customRequest("uwvm/stepLevel", {});
    const reply = await session.customRequest("uwvm/stepLevel", {locked: !before.locked});
    if (typeof reply?.locked !== "boolean" || !LEVELS[reply.level]) throw new Error("The adapter did not acknowledge the step lock.");
    locks.set(session.id, reply.locked); levels.set(session.id, reply.level); publish();
  }));
  const switchLevel = async () => {
    const session = active(); if (!session) throw new Error("Start or select a UWVM debug session first.");
    const current = await session.customRequest("uwvm/stepLevel", {});
    const choice = await vscode.window.showQuickPick([
      {label: current.locked ? "$(lock) Unlock step level" : "$(unlock) Lock step level", description: current.locked ? "locked" : "unlocked",
       detail: "Lock S / W / A stepping while breakpoint navigation stays automatic.", command: "uwvm.toggleStepLock"},
      ...Object.entries(LEVELS).map(([level, item]) => ({
      label: "$(" + item.icon + ") " + item.letter + " · " + item.label, description: [level === current.level ? "active" : "", current.locked ? "locked" : ""].filter(Boolean).join(" · "), detail: item.detail,
      level, command: {source: "uwvm.levelSource", wasm: "uwvm.levelWasm", native: "uwvm.levelAsm"}[level], picked: level === current.level}))],
      {title: "Step level · " + session.name, placeHolder: "Granularity used by Step Over (F10), Step Into (F11) and Step Out (Shift+F11)"});
    if (choice && active() === session) await vscode.commands.executeCommand(choice.command);
  };
  for (const suffix of ["Source", "Wasm", "Asm"]) context.subscriptions.push(vscode.commands.registerCommand("uwvm.switchFrom" + suffix, switchLevel));
  context.subscriptions.push(vscode.commands.registerCommand("uwvm.switchLevel", switchLevel));
  if (inspection) {
    context.subscriptions.push(vscode.commands.registerCommand("uwvm.openBreakpoints", () => inspection.open("breakpoints")));
  }
  const refresh = async session => {
    publish(); if (!session || sessions.get(session.id) !== session) return;
    try { const r = await session.customRequest("uwvm/permissions", {}); if (sessions.get(session.id) === session) permissions.set(session.id, r.permissions); } catch (_) { permissions.delete(session.id); }
    if (["full", "read-only"].includes(permissions.get(session.id))) try {
      const r = await session.customRequest("uwvm/stepLevel", {});
      if (sessions.get(session.id) === session && ["source", "wasm", "native"].includes(r?.level)) { levels.set(session.id, r.level); locks.set(session.id, r.locked === true); }
    } catch (_) { levels.delete(session.id); }
    if (["full", "read-only"].includes(permissions.get(session.id)) && !hostBuilds.has(session.id)) try {
      const r = await session.customRequest("uwvm/hostInterfaces", {});
      if (sessions.get(session.id) === session) hostBuilds.set(session.id, r.builds);
    } catch (_) { hostBuilds.delete(session.id); }
    publish();
  };
  if (vscode.debug.onDidChangeActiveDebugSession) context.subscriptions.push(vscode.debug.onDidChangeActiveDebugSession(refresh));
  context.subscriptions.push(vscode.debug.onDidTerminateDebugSession(s => { permissions.delete(s.id); levels.delete(s.id); locks.delete(s.id); states.delete(s.id); hostBuilds.delete(s.id); publish(); }));
  const pendingNativeReveals = new Map();
  async function revealBreakpoint(session) {
    const wanted = pendingNativeReveals.get(session.id);
    if (!wanted || !wanted.isCurrent() || active() !== session) return;
    try {
      const position = await session.customRequest("uwvm/positions", {});
      if (pendingNativeReveals.get(session.id) !== wanted || !wanted.isCurrent() || active() !== session || position.stopId !== wanted.stopId) return;
      const level = wanted.level || position.displayLevel || position.level;
      const location = await session.customRequest("uwvm/codeLocation", {level, ifStop: wanted.stopId, threadId: position.threadId});
      if (pendingNativeReveals.get(session.id) !== wanted || !wanted.isCurrent() || active() !== session) return;
      pendingNativeReveals.delete(session.id);
      await require("./code_navigation.js").reveal(vscode, session, location, {preserveFocus: true, isCurrent: wanted.isCurrent});
    } catch (_) { /* Keep the requested reveal pending until a frame arrives. */ }
  }
  if (vscode.debug.onDidChangeActiveStackItem) context.subscriptions.push(vscode.debug.onDidChangeActiveStackItem(() => {
    const session = active();
    // The workbench opens a frame's source after selecting it. Restore an
    // explicit S/W/A choice after that native operation, not before its late
    // stackTrace response. A newer user navigation invalidates this ticket.
    if (session) setTimeout(() => void revealBreakpoint(session), 80);
  }));
  if (vscode.debug.registerDebugAdapterTrackerFactory) context.subscriptions.push(vscode.debug.registerDebugAdapterTrackerFactory("uwvm-llvm-full", {
    createDebugAdapterTracker(session) {
      return {onDidSendMessage(packet) {
        if (packet.type === "response" && packet.command === "uwvm/hostInterfaces" && packet.success && sessions.get(session.id) === session) {
          hostBuilds.set(session.id, packet.body?.builds); publish();
        }
        if (packet.type !== "event") return;
        if (packet.event === "uwvm/permissions") { permissions.set(session.id, packet.body.permissions); publish(); }
        if (packet.event === "uwvm/stepLevel" && ["source", "wasm", "native"].includes(packet.body?.level)) { levels.set(session.id, packet.body.level); locks.set(session.id, packet.body.locked === true); publish(); }
        if (packet.event === "stopped" && states.get(session.id) !== "paused") { states.set(session.id, "paused"); publish(); }
        if (packet.event === "stopped" && packet.body?.uwvmStopId &&
            (packet.body.reason === "step" || packet.body.reason === "breakpoint")) {
          pendingNativeReveals.set(session.id, {stopId: packet.body.uwvmStopId, level: packet.body.uwvmBreakpointLevel, isCurrent: navigation.current(session)});
          setTimeout(() => void revealBreakpoint(session), 80);
        }
        if (packet.event === "stopped" && packet.body?.uwvmStopId && packet.body.uwvmPresentationChange) {
          pendingNativeReveals.set(session.id, {stopId: packet.body.uwvmStopId,
            level: levels.get(session.id), isCurrent: navigation.current(session)});
        }
        if (["continued", "terminated", "exited"].includes(packet.event)) { pendingNativeReveals.delete(session.id); navigation.begin(session); }
        if (packet.event === "continued" && states.get(session.id) !== "running") { states.set(session.id, "running"); publish(); }
      }};
    }
  }));
  return {refresh, provider};
}
module.exports = {register};
