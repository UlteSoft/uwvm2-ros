"use strict";

const path = require("path");
const crypto = require("crypto");
const modelAPI = require("./inspection_model");
const filePicker = require("./file_picker");
const editorStyle = require("./editor_style");

// One controller owns every inspector tab. Each tab keeps its own selection,
// session binding and copied stopped snapshot; the backend still authorizes
// every request independently.
function createController(vscode, context, sessions) {
  const panels = new Map();
  const history = require("./navigation_history").get(sessions);
  const coordinates = m => ({selector: {...m.selector}, columns: m.columns, showTags: m.showTags, module: m.module, backend: m.backend, wasiGroup: m.wasiGroup, rosGroup: m.rosGroup, sceneGroup: m.sceneGroup, sceneTraceFilters: {...m.sceneTraceFilters}, threadGroup: m.threadGroup, policyScope: m.policyScope, policyTarget: m.policyTarget, policyAlias: m.policyAlias, page: m.page, policyPage: m.policyPage, groupHistory: [...m.groupHistory||[]], policyHistory: [...m.policyHistory||[]]});
  let nextPanelId = 0;
  const stopStates = new Map();
  const mutationViews = new Map();
  let disabledPermission;
  const watchKey = "uwvm.groupedWatch.v1";
  let watchRows = (context.workspaceState?.get(watchKey, []) || []).filter(row => row && typeof row.id === "string" &&
    ["source","wasm","native"].includes(row.level) && typeof row.expression === "string" && row.expression.length);
  async function saveWatches() {
    await context.workspaceState?.update(watchKey, watchRows.map(({id,level,expression})=>({id,level,expression})));
    for(const model of panels.values())if(model.kind==="watch"){model.watchRows=watchRows;void refresh(model);}
  }

  const dispose = value => context.subscriptions.push(value);

  function active() {
    const session = vscode.debug.activeDebugSession;
    return session && session.type === "uwvm-llvm-full" && sessions.get(session.id) === session ? session : undefined;
  }
  function sessionFor(model) {
    const session = sessions.get(model.sessionId);
    if (!session || session.type !== "uwvm-llvm-full")
      throw new Error("This window's debug session has ended. Use “Use active session” or start a new session.");
    return session;
  }
  function post(model, message) {
    if (!model.disposed && model.ready) void model.panel.webview.postMessage(message);
  }
  function render(model) {
    if (model.disposed) return;
    model.lastView = modelAPI.view(model);
    if (!model.initialized) {
      model.initialized = true;
      const asset = (name, folder = "media") => model.panel.webview.asWebviewUri(vscode.Uri.file(path.join(context.extensionPath, folder, name))).toString();
      const nonce = crypto.randomBytes(18).toString("hex");
      model.panel.webview.html = `<!doctype html><html lang="en"><head><meta charset="utf-8">
<meta http-equiv="Content-Security-Policy" content="default-src 'none'; img-src ${model.panel.webview.cspSource} data:; style-src ${model.panel.webview.cspSource}; script-src 'nonce-${nonce}';">
<meta name="viewport" content="width=device-width,initial-scale=1">
<link rel="stylesheet" href="${asset("ui.css")}"><link rel="stylesheet" href="${asset("inspector.css")}">
</head><body class="inspector" data-initial="${escapeAttr(JSON.stringify(model.lastView))}">
<div id="app"></div>
<script nonce="${nonce}" src="${asset("icons.js")}"></script>
<script nonce="${nonce}" src="${asset("ui.js")}"></script>
<script nonce="${nonce}" src="${asset("memory_values.js", ".")}"></script>
<script nonce="${nonce}" src="${asset("inspector.js")}"></script>
</body></html>`;
    } else post(model, {action: "view", view: model.lastView});
  }
  function escapeAttr(value) {
    return String(value).replace(/&/g, "&amp;").replace(/"/g, "&quot;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
  }
  // Execution retires copied values, but the window keeps showing them until
  // the next stop replaces them, so stepping never flashes. Only a long run
  // (or an explicit failure) shows the "earlier stop" banner.
  function retire(model, message) {
    model.epoch++; model.payload = undefined; model.exportResult = undefined; model.stopId = undefined;
    model.retired = true; model.running = true; model.message = message || ""; model.notice = ""; model.error = "";
    model.policyHistory = []; model.groupHistory = []; model.callbackPages = {};
    if (model.kind === "wasip1" || model.kind === "ros" || model.kind === "scene" || model.kind === "wasithread") { model.policyPage = 0; model.page = 0; }
    clearTimeout(model.staleTimer);
    model.staleTimer = setTimeout(() => {
      if (!model.disposed && model.running) { model.showStale = true; render(model); }
    }, message ? 0 : 1500);
    if (message) render(model);
  }
  function navigationBinding(model, session, inherited) {
    model.navigation?.dispose();
    model.navigation = undefined;
    if (!history || !session) return;
    const binding = inherited || {target: model};
    binding.target = model;
    if (!binding.entry) binding.entry = history.register(session, async (place, valid) => {
      if (!valid()) return;
      const target = binding.target;
      if (target.disposed) { await open(target.kind, {...place, ...place.selector}, session, binding); return; }
      Object.assign(target, place, {selector: {...place.selector}, payload: undefined, exportResult: undefined});
      target.panel.reveal(target.panel.viewColumn, false);
      if (target.busy) target.refreshOnIdle = true;
      await refresh(target, target.kind === "wasip1" ? {module: place.module} : target.kind === "ros" ? {backend: place.backend} : undefined, place.showTags, true);
    }, () => coordinates(binding.target));
    model.navigation = binding.entry;
  }
  function bind(model, session, inherited) {
    // Reset without an intermediate render: the first frame must already show
    // the bound session, never a transient "no session" state.
    model.epoch++; model.payload = undefined; model.exportResult = undefined; model.stopId = undefined; model.message = ""; model.error = "";
    model.callbackPages = {}; model.data = undefined; model.dataSelection = undefined; model.running = false; model.showStale = false; clearTimeout(model.staleTimer);
    model.sessionId = session && session.id; model.sessionName = session && session.name;
    model.sessionEverBound = !!session || !!model.sessionEverBound;
    model.permission = session ? undefined : disabledPermission;
    model.showTags = undefined; model.notice = ""; model.diagnostics = undefined; model.retired = false;
    model.page = 0; model.policyPage = 0; model.policyHistory = []; model.groupHistory = [];
    model.writeError = ""; model.writeResult = undefined; model.payloadSelector = undefined; model.exportPath = "";
    if (model.dumpPending) { model.dumpPending = undefined; model.busy = false; }
    model.dumpReply = undefined; model.dumpPolicy = undefined; model.dumpDisclosure = undefined;
    if (model.restoreRequest) { model.restoreRequest = undefined; model.busy = false; }
    if (model.historyPending || model.recordingPending) {
      model.historyPending = undefined; model.recordingPending = undefined; model.busy = false;
    }
    model.refreshOnIdle = false;
    model.restoreReply = undefined;
    model.threaded = false; model.restoreEnvironment = "current"; model.replayCursor = "0";
    model.historyReply = undefined; model.recordingReply = undefined;
    model.hostInterfaces = undefined;
    navigationBinding(model, session, inherited);
  }
  function current(model, session, epoch) {
    if (model.disposed || model.epoch !== epoch || sessions.get(session.id) !== session || model.sessionId !== session.id)
      throw new Error("The stopped snapshot retired while this request was pending.");
  }
  function stopFrom(reply) {
    if (!reply || typeof reply.text !== "string") throw new Error("Invalid debugger diagnostics response");
    const match = /\bstop-id ([0-9]+)\b/.exec(reply.text);
    return match && match[1] !== "0" ? modelAPI.decimal(match[1], "Stop identifier") : undefined;
  }
  function snapshotSelection(model) {
    return JSON.stringify([model.kind, model.selector?.module, model.selector?.memory, model.module, model.backend,
      model.wasiGroup, model.rosGroup, model.sceneGroup, model.threadGroup,
      model.page, model.policyPage, model.policyScope, model.policyTarget, model.policyAlias]);
  }
  function stopState(session) {
    if (!stopStates.has(session.id)) stopStates.set(session.id, {checking: false, failed: false, recheck: false, changing: new Set()});
    return stopStates.get(session.id);
  }
  async function checkStop(session) {
    const state = stopState(session);
    // A new stopped event can arrive while diagnostics for the previous stop
    // are in flight. Drain it instead of dropping the newer stop notification.
    state.recheck = true;
    if (state.checking) return;
    state.checking = true;
    try {
      do {
        state.recheck = false;
        try {
          const observed = await session.customRequest("uwvm/diagnostics", {});
          const stop = stopFrom(observed) || (observed.threaded === true && observed.details?.State === "exited" ? "0" : undefined);
          if (sessions.get(session.id) !== session) return;
          if (state.recheck || !stop) continue;
          state.failed = false; state.lastStop = stop;
          for (const model of panels.values()) if (model.sessionId === session.id &&
              (stop !== "0" || model.kind === "diagnostics") && (model.stopId !== stop || model.retired)) {
            if (!model.running && !model.busy) retire(model);
            if (model.busy) model.refreshOnIdle = true; else setTimeout(() => refresh(model, undefined, undefined, true), 10);
          }
        } catch (_) {
          if (state.recheck || sessions.get(session.id) !== session) continue;
          state.failed = true;
          for (const model of panels.values()) if (model.sessionId === session.id) {
            retire(model, "The current stop could not be authenticated. Refresh explicitly before inspecting again."); model.showStale = true; render(model);
          }
        }
      } while (state.recheck);
    } finally { state.checking = false; }
  }
  async function groupRequest(session, command, args) {
    try {
      const reply = await session.customRequest(command, args);
      if (!reply || typeof reply !== "object" || !Array.isArray(reply.rows || reply.variables || reply.calls))
        throw new Error("The debugger returned an incomplete group page.");
      if (reply.calls) return {...reply, rows: reply.calls, status: "Journal epoch " + reply.journal.epoch + " · families " + reply.journal.enabled};
      return reply;
    } catch (error) { return {error: String(error.message || error)}; }
  }
  async function rosPolicyPage(model, session, stop, epoch) {
    // The wire packet remains bounded to four rows. Assemble a useful GUI
    // page without dropping rows or weakening the authenticated stop checks.
    let page, cursor = model.policyPage;
    const rows = [];
    do {
      const part = await session.customRequest("uwvm/rosState", {selection: "policy", backend: model.backend,
        start: cursor, count: 4, ifStop: stop,
        ...(model.policyScope ? {scope: model.policyScope} : {}), ...(model.policyTarget ? {target: model.policyTarget} : {}),
        ...(model.policyAlias ? {alias: model.policyAlias} : {})});
      current(model, session, epoch);
      if (!Array.isArray(part.rows) || page && (page.total !== part.total || page.stopId !== part.stopId))
        throw new Error("ROS policy changed while reading its page.");
      rows.push(...part.rows); page = part;
      if (part.more && (!Number.isSafeInteger(part.next) || part.next <= cursor)) throw new Error("ROS policy pagination did not advance.");
      cursor = part.next;
    } while (page.more && rows.length < 64);
    return {...page, start: model.policyPage, rows};
  }
  async function registers(session) {
    const threads = await session.customRequest("threads", {});
    if (!threads || !Array.isArray(threads.threads) || !threads.threads.length)
      throw new Error("No stopped guest thread. Native registers need an owned native instruction stop.");
    const stack = await session.customRequest("stackTrace", {threadId: threads.threads[0].id, startFrame: 0, levels: 32});
    for (const frame of stack.stackFrames || []) {
      const result = await session.customRequest("scopes", {frameId: frame.id});
      const scope = (result.scopes || []).find(scope => scope.presentationHint === "registers" || scope.name === "Registers");
      if (scope) {
        const values = await session.customRequest("variables", {variablesReference: scope.variablesReference, start: 0, count: 0});
        if (!values || !Array.isArray(values.variables)) throw new Error("Invalid register values from the debugger");
        return {variables: values.variables.map(row => ({name: row.name, value: row.value, availability: row.uwvmAvailability || "", ...(row.type ? {type: row.type} : {})})), frameName: frame.name};
      }
    }
    throw new Error("Registers require a native instruction stop. Full permission can capture the current JIT instruction without executing it; read-only sessions can inspect an already captured native stop.");
  }
  // Memory windows read the memory's size and tag format here; byte pages are
  // requested by the webview's virtual grid (see "mem-read").
  async function memoryInfo(model, session, stop, epoch) {
    const selected = model.selector, probeSelection = {...selected, offset: "0", count: 0};
    const probe = modelAPI.decodeMemory(await session.customRequest("uwvm/memoryTags",
      {memoryReference: `wasm-memory:${selected.module}:${selected.memory}:0`, count: 0, includeBytes: true}), probeSelection);
    current(model, session, epoch);
    if (probe.stopId !== stop) throw new Error("Memory belongs to a different stop; refresh the window.");
    const inventory = await session.customRequest("uwvm/wasmInventory", {moduleId: Number(selected.module), kind: "memories", first: Number(selected.memory), count: 1});
    current(model, session, epoch);
    if (inventory.stopId !== stop || !Array.isArray(inventory.rows) || inventory.rows.length !== 1 || ![32, 64].includes(inventory.rows[0].addressBits))
      throw new Error("The actual memory declaration could not be authenticated.");
    return {memoryBytes: probe.memoryBytes, status: probe.status, tagBits: probe.tagBits, granuleBytes: probe.granuleBytes, addressBits: inventory.rows[0].addressBits};
  }
  async function readPages(model, value) {
    if (model.kind !== "memory" || model.disposed || !Array.isArray(value.pages)) return;
    const requested = [...new Set(value.pages)].slice(0, 32).filter(raw => typeof raw === "string" && /^(0|[1-9][0-9]{0,19})$/.test(raw));
    const stop = value.stopId, out = [], dropped = [];
    const reply = () => post(model, {action: "mem-pages", stopId: stop, pages: out, dropped});
    const session = sessions.get(model.sessionId);
    // A page request names the stop the grid is showing. It is served for the
    // current stop and for the last authenticated stop while a refresh of the
    // same stop is in flight; the runtime still checks every page's stop.
    if (!session || !model.data || !stop || (stop !== model.stopId && stop !== model.lastStopId) ||
        !["full", "read-only"].includes(model.permission)) { dropped.push(...requested); reply(); return; }
    const selected = model.selector, size = BigInt(model.data.memoryBytes);
    for (let index = 0; index < requested.length; index++) {
      const offset = BigInt(requested[index]);
      if (offset % 256n || offset >= size) continue;
      const count = Number(size - offset < 256n ? size - offset : 256n);
      try {
        const page = modelAPI.decodeMemory(await session.customRequest("uwvm/memoryTags",
          {memoryReference: `wasm-memory:${selected.module}:${selected.memory}:${offset}`, count, includeBytes: true}), {...selected, offset: offset.toString(), count});
        if (model.disposed || sessions.get(model.sessionId) !== session || page.stopId !== stop) { dropped.push(...requested.slice(index)); break; }
        out.push({offset: page.offset, hex: Buffer.from(page.bytes).toString("hex"), tags: page.tags.map(tag => [tag.offset, tag.hex, tag.color])});
      } catch (_) { dropped.push(...requested.slice(index)); break; }
    }
    reply();
  }
  async function inventory(session, stop) {
    const summary = await session.customRequest("uwvm/wasmInventory", {moduleId: 0, kind: "summary", first: 0, count: 1});
    if (!summary || summary.stopId !== stop) return undefined;
    const functions = [];
    for (let first = 0; first < Math.min(summary.functions, 256); first += 32) {
      const page = await session.customRequest("uwvm/wasmInventory", {moduleId: 0, kind: "functions", first, count: 32});
      if (!page || !Array.isArray(page.rows)) break;
      functions.push(...page.rows.map(row => ({index: row.index, name: row.name, imported: row.imported})));
    }
    return {bits: summary.addressBits, functions, counts: {functions: summary.functions, globals: summary.globals, tables: summary.tables, memories: summary.memories}};
  }
  async function refresh(model, fields, requestedTags = model.showTags, quiet = false) {
    if (model.busy || model.disposed) return;
    if (quiet && model.panel.visible === false) {
      model.refreshWhenVisible = true;
      return;
    }
    model.refreshWhenVisible = false;
    model.refreshOnIdle = false;
    if (fields) {
      if (model.kind === "memory") {
        const selected = modelAPI.memorySelection({...model.selector, ...fields});
        if (selected.module !== model.selector.module || selected.memory !== model.selector.memory) model.writeResult = undefined;
        if (Object.keys(selected).some(key => model.selector[key] !== selected[key])) model.data = undefined;
        model.selector = selected;
        if (fields.columns !== undefined) model.columns = modelAPI.columns(fields.columns);
      }
      if (model.kind === "wasip1" && fields.module !== undefined) {
        const module = modelAPI.decimal(fields.module, "Module", 32);
        if (model.module !== module) { model.page = 0; model.policyPage = 0; model.policyHistory = []; model.payloadSelector = undefined; model.payload = undefined; model.exportResult = undefined; }
        model.module = module;
      }
      if (model.kind === "ros" && fields.backend !== undefined) {
        if (!["all", "ros1", "ros2"].includes(fields.backend)) throw new Error("ROS backend must be all, ros1 or ros2");
        if (model.backend !== fields.backend) { model.page = 0; model.policyPage = 0; model.policyHistory = []; model.payloadSelector = undefined; }
        model.backend = fields.backend;
      }
      if (model.kind === "ros") {
        for (const key of ["policyScope", "policyTarget", "policyAlias"]) if (fields[key] !== undefined) {
          const value = fields[key];
          if (typeof value !== "string" || /[\x00-\x1f]/.test(value) || Buffer.byteLength(value) > 1024 ||
              key === "policyScope" && !["all", "global", "single", "group"].includes(value) ||
              key === "policyAlias" && (value.length > 255 || /[^\x21-\x7e]/.test(value)))
            throw new Error("Invalid ROS policy filter: " + key);
          if (model[key] !== value) { model.policyPage = 0; model.policyHistory = []; }
          model[key] = value;
        }
      }
    }
    // A retained snapshot may only be displayed under its original selectors.
    // In particular, never relabel one module/backend/page as another at an
    // assembly stop where a new safe host snapshot cannot be taken.
    if (["memory", "wasip1", "wasithread", "ros", "scene"].includes(model.kind) &&
        model.dataSelection !== snapshotSelection(model)) model.data = undefined;
    model.busy = true; model.retired = true; model.error = ""; model.payload = undefined; model.exportResult = undefined; model.stopId = undefined;
    const epoch = ++model.epoch, pending = {}; model.pending = pending;
    // A quiet refresh (after a step) keeps the previous values on screen until
    // the new ones arrive; only an explicit refresh shows the reading state.
    if (!quiet || !model.data) render(model);
    try {
      if (!model.sessionId) { model.permission = disabledPermission; model.data = undefined; return; }
      const session = sessionFor(model);
      stopState(session).failed = false;
      const permission = await session.customRequest("uwvm/permissions", {});
      if (!permission || !modelAPI.PERMISSIONS.includes(permission.permissions)) throw new Error("Invalid authenticated permission reply.");
      const before = await session.customRequest("uwvm/diagnostics", {});
      current(model, session, epoch);
      // A permission reply alone is not admission: diagnostics must also
      // complete on the live authenticated broker.
      model.permission = permission.permissions; model.diagnostics = before.text;
      if (["diagnostics", "preloaddl", "ros", "scene"].includes(model.kind) && ["full", "read-only"].includes(model.permission)) {
        model.hostInterfaces = await session.customRequest("uwvm/hostInterfaces", {first: model.kind === "preloaddl" ? String(model.page) : "0"});
        current(model, session, epoch);
      }
      if (model.kind === "diagnostics") {
        const policy = await session.customRequest("uwvm/dumpPolicy", {}); current(model, session, epoch);
        if (!policy || !Array.isArray(policy.allowed) || !["max", "force"].includes(policy.kind) || policy.immutable !== true)
          throw new Error("The VM did not report an immutable dump policy.");
        model.dumpPolicy = policy;
        if (!policy.allowed.includes(model.dumpDisclosure)) model.dumpDisclosure = policy.effective;
        // The adapter retains an uncertain restore after a transport timeout.
        // Rebinding this panel must recover its continuation action without
        // resurrecting any values or stop IDs from the retired world.
        if (before.restorePending === true) model.restoreReply = {pending: true, text: before.text};
        else if (model.restoreReply?.pending) model.restoreReply = undefined;
        model.threaded = before.threaded === true;
        if(model.threaded)model.restoreEnvironment="replay";
        model.data = before.details || {Permissions: model.permission, Status: before.text};
        model.stopId = stopFrom(before) || (model.threaded && before.details?.State === "exited" ? "0" : undefined);
        model.running = !model.stopId && before.details?.State === "running";
        model.showStale = false; clearTimeout(model.staleTimer);
        model.retired = false; return;
      }
      if (!["full", "read-only"].includes(model.permission)) { model.data = undefined; model.retired = false; return; }
      // Launch metadata is immutable and has no dependency on a guest stop.
      // Keep it available before execution and after a joined thread cohort.
      if (model.kind === "preloaddl") {
        model.data = model.hostInterfaces; model.stopId = stopFrom(before);
        model.retired = false; model.running = false; model.showStale = false;
        clearTimeout(model.staleTimer); return;
      }
      const builds = model.hostInterfaces?.builds;
      const built = model.kind === "scene" ? builds?.u2rosv2 : model.kind === "ros" ?
        (model.backend === "all" ? builds?.["u2rosv1-ros1"] || builds?.["u2rosv1-ros2"] : builds?.["u2rosv1-" + model.backend]) : undefined;
      if (built === false) {
        const name = model.kind === "scene" ? "u2rosv2" : "u2rosv1" + (model.backend === "all" ? "" : " › " + model.backend);
        model.data = {unavailable: name + " is not included in this VM build. Runtime authorization cannot enable a compiled-out interface."};
        model.retired = false; model.running = false; model.showStale = false;
        clearTimeout(model.staleTimer); return;
      }
      const stop = stopFrom(before);
      if (!stop) throw new Error("Pause at a current guest stop before inspecting this window.");
      if (before.cooperative === false && ["memory", "wasip1", "wasithread", "ros", "scene"].includes(model.kind)) {
        // Native stops can hold runtime locks. Browsing an inspector must not
        // execute the guest, attempt an unsafe copy, erase unrelated native
        // capabilities or make the window disappear.
        model.running = false; model.showStale = !!model.data;
        clearTimeout(model.staleTimer);
        model.notice = "Assembly stop: this view needs a cooperative Wasm or source stop to read current state. " +
          (model.data ? "Showing a read-only copy from stop " + model.lastStopId + "; it is not live. " : "No snapshot is available for this selection. ") +
          "Selecting S/W alone does not execute. Step to a Wasm or source stop when ready.";
        return;
      }
      let data;
      if (model.kind === "memory") data = await memoryInfo(model, session, stop, epoch);
      else if (model.kind === "registers") data = await registers(session);
      else if (model.kind === "watch") {
        data = {}; model.watchRows = watchRows; model.watchChildren = {};
        for (const row of watchRows) {
          try { data[row.id] = await session.customRequest("uwvm/watch", {level: row.level, expression: row.expression, ifStop: stop}); }
          catch (error) { data[row.id] = {error: String(error.message || error)}; }
          current(model, session, epoch);
        }
      }
      else if (model.kind === "breakpoints") {
        data = await session.customRequest("uwvm/breakpoints", {});
        if (data?.available === false) throw new Error("The breakpoint inventory is unavailable while the VM is " + data.state + "; existing points were not deleted.");
      } else if (model.kind === "wasip1") {
        const module = Number(model.module);
        data = {policy: await groupRequest(session, "uwvm/wasip1Policy", {moduleId: module, start: model.policyPage, count: 64}),
          environment: model.wasiGroup === "calls" ? await groupRequest(session, "uwvm/hostTrace", {family: "wasip1", after: String(model.page), count: 8}) :
            model.wasiGroup === "policy" ? undefined :
            await groupRequest(session, "uwvm/wasip1State", {selection: model.wasiGroup, moduleId: module, start: model.page, count: 32})};
      } else if (model.kind === "wasithread") {
        data = {internal: await session.customRequest("uwvm/wasithreadState", {
          selection: model.threadGroup, start: model.page, count: 8, ifStop: stop})};
      } else if (model.kind === "scene") {
        const journal = model.sceneGroup === "journal";
        data = {internal: journal ? await session.customRequest("uwvm/hostTrace", {family: "rosv2", ...model.sceneTraceFilters, ifStop: stop, after: String(model.page), count: 8}) :
          await session.customRequest("uwvm/sceneState", {
          selection: model.sceneGroup, start: model.page, count: 8, ifStop: stop})};
        if (data.internal.stopId !== stop || (!journal && data.internal.available !== true))
          throw new Error("u2rosv2 scene metadata is unavailable or belongs to a retired stop.");
      } else if (model.kind === "ros") {
        data = {policy: await rosPolicyPage(model, session, stop, epoch),
          internal: model.rosGroup === "journal" ? (model.backend === "all" ? {error: "u2rosv1: select ros1 or ros2 to view recorded calls."} :
            await groupRequest(session, "uwvm/hostTrace", {family: model.backend, after: String(model.page), count: 8})) :
            await groupRequest(session, "uwvm/rosState", model.rosGroup === "trace" ?
              {selection: "trace", backend: model.backend, after: model.page, count: 8} : {selection: model.rosGroup, backend: model.backend, start: model.page, count: 8})};
      }
      const after = await session.customRequest("uwvm/diagnostics", {});
      current(model, session, epoch);
      if (stopFrom(after) !== stop) throw new Error("The guest stop changed while copying this window; refresh for a coherent snapshot.");
      stopState(session).lastStop = stop;
      model.data = data; model.stopId = stop; model.lastStopId = stop; model.retired = false; model.error = "";
      model.dataSelection = snapshotSelection(model);
      model.running = false; model.showStale = false; clearTimeout(model.staleTimer);
      if (/^Assembly stop:/.test(model.notice || "")) model.notice = "";
      if (!model.modules && model.kind === "wasip1") { try { model.modules = await inventory(session, stop); } catch (_) { model.modules = undefined; } }
      if (model.kind === "memory") {
        model.showTags = requestedTags !== false && data.status === "available";
        model.notice = requestedTags === true && !model.showTags ?
          (data.status === "disabled" ? "Runtime MTE is not enabled, so allocation tags stay off." : "Allocation tags are unavailable for this memory.") : "";
      }
      const journal = model.kind === "wasip1" ? data.environment : model.kind === "ros" ? data.internal : undefined;
      if (journal && journal.calls && journal.calls.length && !model.payloadSelector)
        model.payloadSelector = {call: journal.calls[0].id, segment: "0", offset: "0", count: 256};
    } catch (error) {
      if (!model.disposed && model.epoch === epoch) {
        const text = String(error.message || error);
        model.stopId = undefined;
        if (/requires a current (?:identified cooperative stop|cooperative Wasm stop)/.test(text)) {
          // Assembly stop: the runtime only reads guest state at Wasm stops.
          // Keep the last values with a quiet note instead of an error.
          model.error = ""; model.showStale = !!model.data; model.running = false;
          model.notice = "Assembly stop: this view needs a cooperative Wasm or source stop. " +
            (model.data ? "Showing a read-only copy from stop " + model.lastStopId + "; it is not live." : "No snapshot is available for this selection.");
        } else {
          model.error = text;
          if (!quiet) model.data = undefined; else model.showStale = true;
        }
      }
    } finally {
      if (model.pending === pending) {
        model.pending = undefined; model.busy = false;
        if (model.refreshOnIdle) {
          // Replace an obsolete in-flight read directly with the newest one;
          // do not flash its retirement/error state between two quick steps.
          model.refreshOnIdle = false; void refresh(model, undefined, undefined, true);
        } else if (!quiet || !model.running || model.showStale || model.error) render(model);
      }
    }
  }
  async function write(model, fields) {
    if (!model || model.disposed || model.busy || model.retired || !model.data || !model.stopId)
      throw new Error("Refresh a stopped memory window before editing.");
    if (model.permission !== "full") throw new Error("Writing guest memory requires full debugger permission; this session is " + model.permission + ".");
    const session = sessionFor(model), epoch = model.epoch, snapshot = model.data, selected = Object.freeze({...model.selector}), expected = model.stopId;
    const address = modelAPI.decimal(fields.writeOffset === undefined ? selected.offset : fields.writeOffset, "Write offset");
    const bytes = modelAPI.editBytes(fields.hex);
    const target = BigInt(address);
    if (target + BigInt(bytes.length) > BigInt(snapshot.memoryBytes)) throw new Error("The write must fit inside the memory.");
    const reference = `wasm-memory:${selected.module}:${selected.memory}:${address}`;
    const sameSelection = () => model.selector.module === selected.module && model.selector.memory === selected.memory;
    function displayed() {
      current(model, session, epoch);
      if (model.data !== snapshot || model.stopId !== expected || model.retired || model.permission !== "full" || !sameSelection())
        throw new Error("The displayed memory selection or session changed; refresh before editing.");
    }
    model.busy = true; model.error = ""; model.writeError = ""; model.writeResult = undefined;
    model.hex = fields.hex; model.writeOffset = address;
    let reply, committed = false;
    try {
      const permission = await session.customRequest("uwvm/permissions", {}); displayed();
      if (permission.permissions !== "full") throw new Error("The authenticated runtime does not authorize memory writes.");
      const diagnostic = await session.customRequest("uwvm/diagnostics", {}); displayed();
      if (stopFrom(diagnostic) !== expected) throw new Error("The displayed memory stop retired; refresh before editing.");
      reply = await session.customRequest("writeMemory", {memoryReference: reference, offset: 0, data: bytes.toString("base64"), allowPartial: false, ifStop: expected});
      if (!reply || reply.bytesWritten !== bytes.length) throw new Error("The memory write result is incomplete; inspect memory before retrying.");
      committed = true;
      if (sessions.get(session.id) !== session || model.sessionId !== session.id || !sameSelection())
        throw new Error("The write was acknowledged after this window changed. Inspect the original session and selection.");
      model.message = "";
      const checked = modelAPI.decodeMemory(await session.customRequest("uwvm/memoryTags", {memoryReference: reference, count: bytes.length, includeBytes: true}),
        {...selected, offset: address, count: bytes.length});
      if (checked.stopId !== reply.stopId || !Buffer.from(checked.bytes).equals(bytes))
        throw new Error("The commit was acknowledged, but read-back does not match the stopped snapshot.");
      model.message = "";
      model.writeResult = {status: "verified", text: "Wrote " + bytes.length + " byte(s) at " + modelAPI.memoryAddress(address, snapshot.addressBits) + "; read-back verified.",
        offset: address, count: bytes.length, stopId: reply.stopId, module: selected.module, memory: selected.memory};
      model.hex = "";
    } catch (error) {
      model.writeError = (committed ? "Commit acknowledged; do not retry automatically. " : "") + String(error.message || error);
      model.busy = false; render(model);
      if (!committed) throw error;
    } finally { model.busy = false; }
    await refresh(model, undefined, undefined, true);
    return reply;
  }
  async function message(model, value) {
    if (!value || typeof value !== "object" || Array.isArray(value) || typeof value.action !== "string") return;
    try {
      if (["navigate-back", "navigate-forward"].includes(value.action)) {
        await history?.move(value.action === "navigate-back" ? -1 : 1, sessionFor(model)); return;
      }
      if (value.action === "ready") {
        post(model, {action: "navigation", ...history?.state(sessions.get(model.sessionId))});
        model.ready = true;
        post(model, {action: "editorStyle", ...editorStyle.read(vscode)});
        if (model.lastView) post(model, {action: "view", view: model.lastView});
        post(model, {action: "navigation", ...history?.state(sessions.get(model.sessionId))});
        return;
      }
      if (value.action === "mem-read") { await readPages(model, value); return; }
      if (value.action === "mem-view" && model.kind === "memory") {
        if (typeof value.offset === "string" && /^(0|[1-9][0-9]{0,19})$/.test(value.offset) && BigInt(value.offset) < (1n << 64n)) model.selector = {...model.selector, offset: value.offset};
        return;
      }
      if (value.action === "copy") {
        if (typeof value.text !== "string" || value.text.length > 1048576) return;
        await vscode.env.clipboard.writeText(value.text);
        return;
      }
      if (!Number.isSafeInteger(value.epoch) || value.epoch !== model.epoch) {
        throw new Error("This window has changed. Use the current view and retry.");
      }
      if (value.action === "new-window") {
        await open(model.kind, model.kind === "memory" ? {...model.selector, showTags: model.showTags, columns: model.columns} :
          model.kind === "wasip1" ? {module: model.module} : model.kind === "ros" ? {backend: model.backend} : model.kind === "scene" ? {sceneGroup: model.sceneGroup} : model.kind === "wasithread" ? {threadGroup: model.threadGroup} : undefined,
          sessions.get(model.sessionId) || null); return;
      }
      if (value.action === "columns" && model.kind === "memory") { model.columns = modelAPI.columns(value.value); render(model); return; }
      if (value.action === "toggle-tags" && model.kind === "memory" && value.enabled === false) { model.showTags = false; model.notice = ""; render(model); return; }
      if (value.action === "toggle-tags" && model.kind === "memory") {
        if (model.busy) return;
        if (typeof value.enabled !== "boolean") throw new Error("Invalid Memtag switch value");
        if (!value.enabled) { model.showTags = false; model.notice = ""; render(model); return; }
        if (!["full", "read-only"].includes(model.permission) || !model.data)
          throw new Error("Allocation tags require a current full or read-only memory snapshot.");
        model.showTags = model.data.status === "available";
        model.notice = model.showTags ? "" : model.data.status === "disabled" ? "Runtime MTE is not enabled, so allocation tags stay off." : "Allocation tags are unavailable for this memory.";
        render(model); return;
      }
      if (value.action === "bind") { bind(model, active()); render(model); await refresh(model); return; }
      if (model.kind === "watch" && ["watch-add", "watch-edit", "watch-remove"].includes(value.action)) {
        if (value.action === "watch-remove") watchRows = watchRows.filter(row => row.id !== value.id);
        else {
          const fields = value.fields || {};
          if (!["source","wasm","native"].includes(fields.level) || typeof fields.expression !== "string" || !fields.expression.trim()) throw new Error("Select a level and enter a watch expression.");
          const row = {id: value.action === "watch-edit" ? value.id : crypto.randomUUID(), level: fields.level, expression: fields.expression.trim()};
          if (value.action === "watch-edit") { if(!watchRows.some(x=>x.id===row.id))throw new Error("Watch was removed.");watchRows=watchRows.map(x=>x.id===row.id?row:x); }
          else watchRows = [...watchRows, row];
        }
        await saveWatches(); return;
      }
      if ((model.kind === "watch" && value.action === "watch-goto") || (model.kind === "breakpoints" && value.action === "breakpoint-goto")) {
        if(model.retired || !model.stopId || !["full","read-only"].includes(model.permission)) throw new Error("Navigation requires a current inspectable stop.");
        const session=sessionFor(model), epoch=model.epoch;
        let location;
        if(model.kind==="watch") {
          const row=watchRows.find(row=>row.id===value.id);if(!row)throw new Error("Watch was removed.");
          const result=model.data?.[row.id];if(!result || result.error)throw new Error("This expression has no current evaluation location.");
          location=await session.customRequest("uwvm/codeLocation",{level:row.level,threadId:result.threadId,ifStop:model.stopId});
        } else {
          if(!model.data?.rows?.some(row=>row.id===value.id))throw new Error("Breakpoint was removed.");
          location=await session.customRequest("uwvm/breakpointLocation",{id:value.id,ifStop:model.stopId});
        }
        current(model,session,epoch);await require("./code_navigation").reveal(vscode,session,location);return;
      }
      if(model.kind==="watch" && value.action==="watch-children") {
        const row=watchRows.find(row=>row.id===value.id), result=row&&model.data?.[row.id];
        if(model.retired||!model.stopId||!result?.variablesReference)throw new Error("No current expandable watch value.");
        const start=value.start??0;if(!Number.isSafeInteger(start)||start<0)throw new Error("Invalid member page.");
        const session=sessionFor(model),epoch=model.epoch;
        const children=await session.customRequest("variables",{variablesReference:result.variablesReference,start,count:32});current(model,session,epoch);
        model.watchChildren={...model.watchChildren,[row.id]:{start,rows:children.variables||[]}};render(model);return;
      }
      if (model.kind === "breakpoints" && ["breakpoint-create", "breakpoint-condition", "breakpoint-ignore", "breakpoint-set-callbacks", "breakpoint-callbacks", "breakpoint-callback-next"].includes(value.action)) {
        if (model.retired || model.busy || !model.stopId) throw new Error("Refresh the current stopped breakpoint view first.");
        const session = sessionFor(model), epoch = model.epoch;
        const reading = ["breakpoint-callbacks", "breakpoint-callback-next"].includes(value.action);
        if (!reading && model.permission !== "full") throw new Error("Changing breakpoints requires full permission.");
        if (!["full", "read-only"].includes(model.permission)) throw new Error("Inspecting breakpoints requires full or read-only permission.");
        const id = Number(value.id);
        if (value.action !== "breakpoint-create" && (!Number.isSafeInteger(id) || !model.data?.rows?.some(row => row.id === id)))
          throw new Error("Select a breakpoint in the current inventory.");
        model.busy = true; render(model);
        try {
          if (reading) {
            const start = value.action === "breakpoint-callback-next" ? model.callbackPages?.[id]?.next : 0;
            if (!Number.isSafeInteger(start)) throw new Error("Read the first callback page before continuing.");
            const result = await session.customRequest("uwvm/breakpointCallbacks", {id, start}); current(model, session, epoch);
            model.callbackPages ??= {}; model.callbackPages[id] = result;
          } else if (value.action === "breakpoint-create") {
            const fields = value.fields || {};
            await session.customRequest("uwvm/breakpointCreate", {module: fields.module, function: fields.function, offset: fields.offset,
              temporary: fields.temporary === "on" || fields.temporary === true});
          } else {
            const action = {"breakpoint-condition": "condition", "breakpoint-ignore": "ignore", "breakpoint-set-callbacks": "callbacks"}[value.action];
            await session.customRequest("uwvm/breakpointControl", {id, action, value: value.fields?.value || "", mode: value.fields?.mode || "replace"});
          }
          model.message = reading ? "Callback page read." : "Breakpoint change acknowledged by the runtime.";
        } finally { model.busy = false; render(model); }
        if (!reading) await refresh(model); return;
      }
      if (model.kind === "breakpoints" && ["breakpoint-toggle", "breakpoint-delete"].includes(value.action)) {
        if (model.permission !== "full" || model.retired || model.busy) throw new Error("A current full-permission stop is required to change breakpoints.");
        const id = Number(value.id);
        if (!Number.isSafeInteger(id) || !model.data?.rows?.some(row => row.id === id)) throw new Error("The breakpoint is no longer in this view.");
        const action = value.action === "breakpoint-delete" ? "delete" : value.nextState;
        if (!["enable", "disable", "delete"].includes(action)) throw new Error("Unknown breakpoint action");
        await sessionFor(model).customRequest("uwvm/breakpointControl", {id, action}); await refresh(model); return;
      }
      if (value.action === "diagnostics") { await vscode.commands.executeCommand("uwvm.showDiagnostics"); return; }
      if (model.kind === "diagnostics" && value.action === "dump-disclosure") {
        if (!model.dumpPolicy?.allowed?.includes(value.fields?.disclosure))
          throw new Error("This dump disclosure is not allowed by the VM launch policy.");
        model.dumpDisclosure = value.fields.disclosure;
        if (typeof value.fields.dumpPath === "string") model.dumpPath = value.fields.dumpPath;
        render(model); return;
      }
      if (model.kind === "diagnostics" && value.action === "browse-dump") {
        const session = sessionFor(model); filePicker.requireLocal(await filePicker.localSession(session));
        model.picking = true; render(model);
        try {
          const picked = await filePicker.pick(vscode, {title: "Wasm dump on VM host", value: value.fields?.dumpPath || model.dumpPath, save: true});
          if (picked !== undefined && !model.disposed) model.dumpPath = picked;
        } finally { model.picking = false; render(model); }
        return;
      }
      if (model.kind === "diagnostics" && value.action === "browse-restore") {
        const session = sessionFor(model); filePicker.requireLocal(await filePicker.localSession(session));
        model.picking = true; render(model);
        try {
          const picked = await filePicker.pick(vscode, {title: "Unified snapshot on VM host", value: value.fields?.restorePath || model.restorePath, save: false});
          if (picked !== undefined && !model.disposed && model.sessionId === session.id) model.restorePath = picked;
        } finally { model.picking = false; render(model); }
        return;
      }
      if (model.kind === "diagnostics" && ["restore-snapshot", "continue-restore"].includes(value.action)) {
        const session = sessionFor(model);
        if (model.permission !== "full" || model.busy) throw new Error("Restoring execution requires full debugger permission.");
        const continuing = value.action === "continue-restore";
        if (!continuing && (!model.stopId || model.retired)) throw new Error("Refresh the current stop before restoring.");
        let args = {continue: true};
        if (!continuing) {
          const fields = value.fields || {};
          if (typeof fields.restorePath !== "string" || !fields.restorePath || !["current", "recorded", "replay"].includes(fields.restoreEnvironment))
            throw new Error("Select a snapshot and an environment policy.");
          model.restorePath = fields.restorePath; model.restoreEnvironment = fields.restoreEnvironment;
          model.restoreBindings = fields.restoreBindings || "";
          const bindings = [];
          if (model.restoreEnvironment === "recorded" && model.restoreBindings.trim()) {
            for (const line of model.restoreBindings.trim().split(/[,\n]/)) {
              const match = /^\s*(0|[1-9][0-9]*):(0|[1-9][0-9]*):(0|[1-9][0-9]*)\s*$/.exec(line);
              if (!match) throw new Error("Bindings use module:resource:descriptor, one per line or separated by commas.");
              bindings.push({module: match[1], resource: match[2], descriptor: match[3]});
            }
          }
          args = {path: model.restorePath, environment: model.restoreEnvironment, stopId: model.stopId, bindings};
          if (model.threaded) {
            if (model.restoreEnvironment !== "replay") throw new Error("Dynamic threads reconstruct recorded execution; live rebinding is not available.");
            model.replayCursor = modelAPI.decimal(fields.replayCursor || "0", "Replay event cursor");
            args.cursor = model.replayCursor;
          }
        } else if (!model.restoreReply?.pending) throw new Error("This session has no pending restoration in this window.");
        const request = {}; model.restoreRequest = request; model.busy = true; model.error = ""; render(model);
        const owns = () => !model.disposed && model.restoreRequest === request && model.sessionId === session.id && sessions.get(session.id) === session;
        try {
          const reply = await session.customRequest("uwvm/restoreSnapshot", args);
          if (owns()) model.restoreReply = reply;
        } catch (error) { if (owns()) throw error; }
        finally {
          if (model.restoreRequest === request) {
            model.restoreRequest = undefined; model.busy = false;
            if (owns() || (!model.disposed && model.sessionId === session.id && sessions.get(session.id) === session)) {
              if (model.restoreReply?.pending) render(model); else await refresh(model);
            }
          }
        }
        return;
      }
      if (model.kind === "diagnostics" && value.action === "snapshot-history") {
        const session = sessionFor(model), epoch = model.epoch;
        if (!["full", "read-only"].includes(model.permission) || model.busy || !model.dumpPolicy?.allowed?.includes("full"))
          throw new Error("Historical environments require full inspection and dump disclosure.");
        const request = {}; model.historyPending = request; model.busy = true; model.error = ""; render(model);
        const owns = () => !model.disposed && model.historyPending === request && model.epoch === epoch &&
          model.sessionId === session.id && sessions.get(session.id) === session;
        try {
          const reply = await session.customRequest("uwvm/snapshotHistory", value.fields || {});
          if (owns()) model.historyReply = reply;
        } catch (error) { if (owns()) throw error; }
        finally { if (model.historyPending === request) {
          model.historyPending = undefined; model.busy = false; render(model);
          if (model.refreshOnIdle) { model.refreshOnIdle = false; void refresh(model, undefined, undefined, true); }
        } }
        return;
      }
      if (model.kind === "diagnostics" && value.action === "record-snapshot") {
        const session = sessionFor(model), epoch = model.epoch;
        if (model.permission !== "full" || model.busy) throw new Error("Recording requires full debugger permission.");
        const action = value.fields?.action;
        if (!["start", "save", "status", "mark"].includes(action)) throw new Error("Unknown recording action.");
        if (action !== "status" && (!model.stopId || model.retired || !model.dumpPolicy?.allowed?.includes("full")))
          throw new Error("Recording requires a current stop and full dump disclosure.");
        const args = {action};
        if (action !== "status") args.stopId = model.stopId;
        if (action === "save") {
          if (!value.fields?.path) throw new Error("Choose the recording output path in Save Wasm dump first.");
          args.path = value.fields.path; model.dumpPath = args.path;
        }
        const pending = {}; model.recordingPending = pending; model.busy = true; model.error = ""; render(model);
        const owns = () => !model.disposed && model.recordingPending === pending && model.epoch === epoch &&
          model.sessionId === session.id && sessions.get(session.id) === session;
        try { const reply = await session.customRequest("uwvm/recordSnapshot", args); if (owns()) model.recordingReply = reply; }
        catch (error) { if (owns()) throw error; }
        finally { if (model.recordingPending === pending) {
          model.recordingPending = undefined; model.busy = false; render(model);
          if (model.refreshOnIdle) { model.refreshOnIdle = false; void refresh(model, undefined, undefined, true); }
        } }
        return;
      }
      if (model.kind === "diagnostics" && value.action === "dump") {
        const session = sessionFor(model), epoch = model.epoch, stop = model.stopId;
        if (!stop || model.retired || model.busy || !["full", "read-only", "limited"].includes(model.permission))
          throw new Error("Refresh a current permitted stop before saving a dump.");
        if (typeof value.fields?.dumpPath !== "string" || !value.fields.dumpPath) throw new Error("Choose an output file on the VM host.");
        const disclosure = value.fields.disclosure || model.dumpDisclosure;
        if (!model.dumpPolicy || !model.dumpPolicy.allowed.includes(disclosure))
          throw new Error("The requested dump disclosure is not allowed by the VM launch policy.");
        model.dumpDisclosure = disclosure;
        model.dumpPath = value.fields.dumpPath; model.busy = true; model.error = "";
        const pending = {}; model.dumpPending = pending; render(model);
        const ownsRequest = () => !model.disposed && model.dumpPending === pending && model.epoch === epoch &&
          sessions.get(session.id) === session && model.sessionId === session.id;
        try {
          const reply = await session.customRequest("uwvm/dump", {path: model.dumpPath, stopId: stop, disclosure});
          if (ownsRequest()) model.dumpReply = reply;
        } catch (error) { if (ownsRequest()) throw error; }
        finally {
          // An old export may finish after this panel binds another session.
          // It cannot publish its acknowledgement or clear the new request.
          if (model.dumpPending === pending) {
            model.dumpPending = undefined; model.busy = false;
            if (model.refreshOnIdle) { model.refreshOnIdle = false; void refresh(model, undefined, undefined, true); }
            else render(model);
          }
        }
        return;
      }
      if (value.action === "refresh") { model.message = ""; model.writeError = ""; await refresh(model, value.fields); return; }
      if (value.action === "write" && model.kind === "memory") { await write(model, value.fields || {}); return; }
      if (model.busy) return;
      if (["browse-export", "call-payload", "payload", "export", "trace-on", "trace-off", "trace-clear"].includes(value.action)) {
        if (!["wasip1", "ros", "scene"].includes(model.kind) || !["full", "read-only"].includes(model.permission) || !model.data || !model.stopId || model.retired)
          throw new Error("Refresh a current full or read-only stopped group before accessing recorded calls.");
        const family = model.kind === "wasip1" ? "wasip1" : model.kind === "scene" ? "rosv2" : model.backend;
        if (family === "rosv2" && (model.sceneGroup !== "journal" || ["payload", "call-payload"].includes(value.action)))
          throw new Error("ROSv2 records contain completion metadata only.");
        if (!["wasip1", "ros1", "ros2", "rosv2"].includes(family)) throw new Error("Choose one explicit ROS backend for host-call controls.");
        const session = sessionFor(model), expected = model.stopId, epoch = model.epoch;
        const diagnostic = await session.customRequest("uwvm/diagnostics", {}); current(model, session, epoch);
        if (model.stopId !== expected || model.retired || stopFrom(diagnostic) !== expected)
          throw new Error("The displayed stop retired; refresh before accessing retained recordings.");
        model.error = "";
        if (value.action === "browse-export") {
          if (model.picking) return;
          model.picking = true;
          try {
            filePicker.requireLocal(await filePicker.localSession(session)); current(model, session, epoch);
            const target = await filePicker.pick(vscode, {title: "Recorded calls JSON on VM host", value: value.fields?.exportPath || model.exportPath, save: true, filters: {JSON: ["json"]}});
            current(model, session, epoch);
            if (target !== undefined) model.exportPath = target;
          } finally { model.picking = false; render(model); }
          return;
        }
        if (value.action === "call-payload") {
          if (typeof value.call !== "string" || !/^[1-9][0-9]{0,19}$/.test(value.call)) throw new Error("Invalid recorded call ID");
          model.payloadSelector = {call: value.call, segment: "0", offset: "0", count: 256}; render(model); return;
        }
        if (value.action === "payload") {
          const f = value.fields || {};
          const selector = {call: modelAPI.decimal(f.call, "Call ID"), segment: modelAPI.decimal(f.segment, "Segment"),
            offset: modelAPI.decimal(f.offset, "Payload offset"), count: modelAPI.page(f.count, 4096)};
          if (selector.call === "0") throw new Error("Choose a nonzero retained call ID.");
          model.payloadSelector = selector;
          const copied = await session.customRequest("uwvm/hostTraceBytes", selector);
          current(model, session, epoch);
          if (copied.stopId !== expected || stopFrom(await session.customRequest("uwvm/diagnostics", {})) !== expected)
            throw new Error("Retained payload belongs to a different stop; refresh.");
          model.payload = copied; render(model); return;
        }
        if (value.action === "export") {
          const target = value.fields && value.fields.exportPath;
          if (typeof target !== "string" || !target.startsWith("/") || /[\x00-\x1f]/.test(target))
            throw new Error("Enter an absolute new JSON file path on the VM host.");
          model.exportPath = target;
          model.exportResult = await session.customRequest("uwvm/exportHostTrace", family === "rosv2" ?
            {family, path: target, ...model.sceneTraceFilters, ifStop: expected} : {family, path: target});
          current(model, session, epoch); render(model); return;
        }
        if (model.permission !== "full") throw new Error("Changing recording requires full debugger permission.");
        const permission = await session.customRequest("uwvm/permissions", {});
        current(model, session, epoch);
        if (permission.permissions !== "full") throw new Error("The runtime does not authorize recording changes.");
        const verb = value.action.slice(6), expression = "trace host " + verb + (verb === "clear" ? "" : " family=" + family) + " if-stop=" + expected;
        await session.customRequest("evaluate", {context: "repl", expression});
        model.message = "Recording " + verb + "."; await refresh(model); return;
      }
      if (value.action === "wasi-group" && Object.hasOwn(modelAPI.WASI_GROUPS, value.group)) {
        model.wasiGroup = value.group; model.page = 0; model.groupHistory = []; model.payloadSelector = undefined; await refresh(model); return;
      }
      if (value.action === "thread-group" && model.kind === "wasithread" && ["policy", "members"].includes(value.group)) {
        model.threadGroup = value.group; model.page = 0; model.groupHistory = []; await refresh(model); return;
      }
      if (value.action === "scene-trace-filter" && model.kind === "scene" && model.sceneGroup === "journal") {
        const filters = {};
        for (const [key, text] of Object.entries(value.fields || {})) {
          if (!["function", "namespace", "module", "result"].includes(key) || typeof text !== "string")
            throw new Error("Unknown ROSv2 completion selector.");
          if (!text) continue;
          if (key === "result" ? !/^(0|[1-9][0-9]{0,9})$/.test(text) || Number(text) > 4294967295 : !/^[A-Za-z0-9_.-]{1,255}$/.test(text))
            throw new Error("Invalid exact ROSv2 completion selector.");
          filters[key] = text;
        }
        model.sceneTraceFilters = filters; model.page = 0; model.groupHistory = [];
        model.exportResult = undefined; await refresh(model); return;
      }
      if (value.action === "scene-group" && model.kind === "scene" && Object.hasOwn(modelAPI.SCENE_GROUPS, value.group)) {
        model.sceneGroup = value.group; model.page = 0; model.groupHistory = []; await refresh(model); return;
      }
      if (value.action === "ros-group" && Object.hasOwn(modelAPI.ROS_GROUPS, value.group)) {
        model.rosGroup = value.group; model.page = 0; model.groupHistory = []; model.payloadSelector = undefined; await refresh(model); return;
      }
      if (value.action === "policy-next" && model.data && model.data.policy && model.data.policy.more && Number.isSafeInteger(model.data.policy.next)) {
        if (model.data.policy.next <= model.policyPage) throw new Error("The backend policy page did not advance.");
        (model.policyHistory ||= []).push(model.policyPage);
        model.policyPage = model.data.policy.next; await refresh(model); return;
      }
      if (value.action === "policy-previous" && model.policyHistory && model.policyHistory.length) { model.policyPage = model.policyHistory.pop(); await refresh(model); return; }
      if (value.action === "group-previous" && model.groupHistory?.length) { model.page = model.groupHistory.pop(); await refresh(model); return; }
      if (value.action === "group-next" && model.data) {
        const data = model.kind === "preloaddl" ? model.data : model.kind === "wasip1" ? model.data.environment : model.data.internal;
        if (data && data.more && (Number.isSafeInteger(data.next) && data.next >= 0 || typeof data.next === "string" && /^(0|[1-9][0-9]{0,19})$/.test(data.next))) {
          (model.groupHistory ||= []).push(model.page); model.page = data.next; await refresh(model);
        }
      }
    } catch (error) { model.error = String(error.message || error); render(model); }
    finally { if (model.panel.active && !["ready", "mem-read", "navigate-back", "navigate-forward", "copy"].includes(value.action)) model.navigation?.visit(coordinates(model), value.action === "mem-view"); }
  }
  // Memory opens in the active editor group. Other inspectors reuse an
  // existing inspector group; an explicit new window remains independent.
  function codeColumn() {
    const active = vscode.window.tabGroups?.activeTabGroup;
    if(active?.viewColumn !== undefined) return active.viewColumn;
    const groups = vscode.window.tabGroups?.all || [];
    const inspector = tab => typeof tab.input?.viewType === "string" &&
      /^uwvm\.(memory|registers|wasip1|wasithread|ros|scene|preloaddl|diagnostics|breakpoints|watch|signedUpdate)\./.test(tab.input.viewType);
    return groups.find(group => group.tabs.some(tab => !inspector(tab)))?.viewColumn || vscode.ViewColumn.One;
  }
  function inspectorColumn() {
    const existing = [...panels.values()].find(item => !item.disposed && item.panel.viewColumn !== undefined);
    if (existing) return existing.panel.viewColumn;
    const group = vscode.window.tabGroups && vscode.window.tabGroups.activeTabGroup;
    return group && group.tabs.length === 0 ? vscode.ViewColumn.Active : vscode.ViewColumn.Beside;
  }
  async function open(kind, initial, session = active(), inherited) {
    if (!Object.hasOwn(modelAPI.KINDS, kind)) throw new Error("Unknown UWVM inspection window");
    if (session) require("./navigation_state").begin(session);
    const selector = kind === "memory" ? modelAPI.memorySelection(initial || {}) : modelAPI.memorySelection();
    const columns = modelAPI.columns(initial && typeof initial === "object" ? initial.columns : undefined);
    const id = ++nextPanelId;
    const sameKind = [...panels.values()].filter(item => item.kind === kind && !item.disposed).length;
    const panel = vscode.window.createWebviewPanel("uwvm." + kind + "." + id, modelAPI.KINDS[kind] + (sameKind ? " (" + (sameKind + 1) + ")" : ""), kind === "memory" ? codeColumn() : inspectorColumn(),
      {enableScripts: true, retainContextWhenHidden: true, localResourceRoots: [vscode.Uri.file(path.join(context.extensionPath, "media")), vscode.Uri.file(context.extensionPath)]});
    const model = {watchRows, id, ordinal: sameKind + 1, kind, panel, selector, columns, module: "0", backend: "all", wasiGroup: "args", rosGroup: "sessions", sceneGroup: "authority", threadGroup: "policy", page: 0, policyPage: 0, epoch: 0, busy: false, showTags: undefined, focusDump: initial?.focusDump === true};
    panels.set(id, model); bind(model, session, inherited);
    if (initial && typeof initial === "object") {
      if (Object.hasOwn(modelAPI.WASI_GROUPS, initial.wasiGroup)) model.wasiGroup = initial.wasiGroup;
      if (["policy", "members"].includes(initial.threadGroup)) model.threadGroup = initial.threadGroup;
      if (Object.hasOwn(modelAPI.SCENE_GROUPS, initial.sceneGroup)) model.sceneGroup = initial.sceneGroup;
      if (Object.hasOwn(modelAPI.ROS_GROUPS, initial.rosGroup)) model.rosGroup = initial.rosGroup;
    }
    if (inherited) Object.assign(model, initial, {selector: {...initial.selector}});
    model.busy = !!model.sessionId; render(model); model.busy = false;
    model.navigation?.visit(coordinates(model));
    if (history) dispose(history.subscribe(() => post(model, {action: "navigation", ...history.state(sessions.get(model.sessionId))})));
    panel.iconPath = new vscode.ThemeIcon(modelAPI.KIND_ICONS[kind]);
    dispose(panel); dispose(panel.webview.onDidReceiveMessage(value => void message(model, value)));
    dispose(panel.onDidDispose(() => { model.disposed = true; model.epoch++; panels.delete(id); }));
    dispose(panel.onDidChangeViewState(() => {
      if (panel.active && !model.disposed) model.navigation?.visit(coordinates(model));
      if (panel.visible && model.refreshWhenVisible && !model.busy && !model.disposed)
        void refresh(model, undefined, undefined, true);
    }));
    if (kind === "registers" && session && !inherited) {
      try {
        const permission = await session.customRequest("uwvm/permissions", {});
        if (permission.permissions === "full") {
          const position = await session.customRequest("uwvm/positions", {});
          if (position.state === "stopped" && position.stopId && !position.native)
            await session.customRequest("uwvm/nativeEntry", {ifStop: position.stopId});
        }
      } catch (error) {
        model.error = String(error.message || error); render(model); return panel;
      }
    }
    await refresh(model, kind === "ros" || kind === "wasip1" ? initial : undefined, kind === "memory" && initial && typeof initial.showTags === "boolean" ? initial.showTags : undefined);
    return panel;
  }
  for (const [command, kind] of Object.entries({"uwvm.openMemory": "memory", "uwvm.openRegisters": "registers", "uwvm.openWasip1": "wasip1", "uwvm.openRos": "ros", "uwvm.openScene": "scene", "uwvm.openPreloadDl": "preloaddl", "uwvm.openWasithread": "wasithread", "uwvm.openWatch": "watch"}))
    dispose(vscode.commands.registerCommand(command, initial => open(kind, initial)));
  dispose(vscode.commands.registerCommand("uwvm.writeMemory", async fields => {
    const model = Array.from(panels.values()).find(item => item.kind === "memory" && item.panel.active && !item.disposed);
    if (!model) throw new Error("Focus a memory window before editing bytes.");
    if (fields === undefined) {
      const epoch = model.epoch, sessionId = model.sessionId;
      const value = await vscode.window.showInputBox({prompt: "Hex bytes to write at " + modelAPI.memoryAddress(model.writeOffset || model.selector.offset, model.data.addressBits), placeHolder: "41 42 43 00"});
      if (value === undefined) return;
      if (model.disposed || model.epoch !== epoch || model.sessionId !== sessionId) throw new Error("The memory window changed while entering bytes. Refresh before editing.");
      fields = {hex: value, writeOffset: model.writeOffset || model.selector.offset};
    }
    return write(model, fields);
  }));
  if (vscode.debug.onDidChangeActiveStackItem) dispose(vscode.debug.onDidChangeActiveStackItem(item => {
    const session = item?.session, pending = session && mutationViews.get(session.id);
    if (!pending?.stopped || !Number.isSafeInteger(item.frameId) || item.frameId === pending.frameId) return;
    mutationViews.delete(session.id);
    const model = pending.model;
    // Restore once, only after the client's fresh frame selection. A timer
    // following later stack selections would steal a user's next navigation.
    if (!model.disposed && model.sessionId === session.id && active() === session)
      model.panel.reveal(model.panel.viewColumn, true);
  }));
  if (vscode.debug.registerDebugAdapterTrackerFactory) dispose(vscode.debug.registerDebugAdapterTrackerFactory("uwvm-llvm-full", {
    createDebugAdapterTracker(session) {
      return {
        onWillReceiveMessage(packet) {
          if (packet.type !== "request") return;
          if (["writeMemory", "setVariable", "setExpression", "uwvm/nativeEntry"].includes(packet.command)) {
            const model = [...panels.values()].find(m => m.sessionId === session.id && m.panel.active && !m.disposed);
            if (model) mutationViews.set(session.id, {model, request: packet.seq, frameId: vscode.debug.activeStackItem?.frameId});
            else mutationViews.delete(session.id);
          } else if (["continue", "next", "stepIn", "stepOut", "restart", "goto", "uwvm/signedUpdate", "uwvm/restoreSnapshot", "uwvm/threadReplay"].includes(packet.command)) {
            mutationViews.delete(session.id);
          }
          const changes = ["continue", "next", "stepIn", "stepOut", "pause", "restart", "restartFrame", "goto", "writeMemory", "setVariable", "setExpression", "uwvm/nativeEntry", "uwvm/signedUpdate", "uwvm/restoreSnapshot"];
          const threadReplay = packet.command === "uwvm/threadReplay" && packet.arguments?.action === "replay";
          if (changes.includes(packet.command) || threadReplay || packet.command === "evaluate" && packet.arguments && packet.arguments.context === "repl") {
            const state = stopState(session); state.failed = false; state.changing.add(packet.seq);
            for (const model of panels.values()) if (model.sessionId === session.id &&
                (packet.command === "uwvm/restoreSnapshot" || threadReplay || !model.busy || model.pending)) {
              retire(model);
              if (threadReplay) render(model);
            }
          }
        },
        onDidSendMessage(packet) {
          const pending = mutationViews.get(session.id);
          if (packet.type === "response" && pending?.request === packet.request_seq && !packet.success)
            mutationViews.delete(session.id);
          if (packet.type === "event" && packet.event === "stopped") {
            if (pending && (packet.body?.uwvmStateMutation || packet.body?.uwvmInspectionStop)) pending.stopped = true;
            else mutationViews.delete(session.id);
          }
          if (packet.type === "response" && stopState(session).changing.delete(packet.request_seq)) {
            // REPL reads and rejected edits may leave the same stop intact.
            // Reauthenticate it after the reply instead of leaving the panel
            // permanently marked as running until a different stop appears.
            void checkStop(session);
          }
          if (packet.type !== "event") return;
          if (packet.event === "uwvm/threadReplay" && packet.body?.state === "joined") {
            for (const model of panels.values()) if (model.sessionId === session.id && model.kind === "diagnostics") {
              if (model.busy) model.refreshOnIdle = true; else void refresh(model);
            }
          }
          for (const model of panels.values()) if (model.sessionId === session.id) {
            if (["continued", "terminated", "exited"].includes(packet.event) && (!model.busy || model.pending)) {
              retire(model);
              // A step stops again within milliseconds: only a real run shows
              // "Running", so stepping never makes the window flicker.
              if (packet.event === "continued") setTimeout(() => { if (model.running && !model.disposed) render(model); }, 400);
              else render(model);
            }
          }
          // A memory write or variable edit re-publishes the same position
          // under a new stop: refresh quietly instead of retiring the view.
          if (["invalidated", "memory"].includes(packet.event)) { stopState(session).lastStop = undefined; void checkStop(session); }
          // Compare authenticated stop identifiers, never reason/thread labels.
          if (packet.event === "stopped") void checkStop(session);
          if (packet.event === "uwvm/breakpoints") for (const model of panels.values()) if (model.sessionId === session.id && model.kind === "breakpoints") {
            if (model.busy) model.refreshOnIdle = true; else void refresh(model);
          }
        }
      };
    }
  }));
  if (vscode.workspace.onDidChangeConfiguration) dispose(vscode.workspace.onDidChangeConfiguration(event => {
    if (event.affectsConfiguration("editor")) for (const model of panels.values()) if (model.ready && !model.disposed)
      void model.panel.webview.postMessage({action: "editorStyle", ...editorStyle.read(vscode)});
  }));
  dispose(vscode.debug.onDidTerminateDebugSession(session => {
    stopStates.delete(session.id);
    mutationViews.delete(session.id);
    for (const model of panels.values()) if (model.sessionId === session.id) {
      retire(model); model.data = undefined; model.sessionId = undefined; model.sessionName = "Session ended"; render(model);
    }
  }));
  if (vscode.debug.onDidStartDebugSession) dispose(vscode.debug.onDidStartDebugSession(session => {
    if (session.type !== "uwvm-llvm-full" || sessions.get(session.id) !== session) return;
    disabledPermission = undefined;
    for (const model of panels.values()) if (!model.sessionId && !model.sessionEverBound) { bind(model, session); setTimeout(() => refresh(model), 50); }
  }));
  return {
    open,
    noteDisabled(permission) {
      disabledPermission = permission;
      for (const model of panels.values()) if (!model.sessionId) { model.permission = permission; render(model); }
    },
    dispose() { for (const model of panels.values()) model.panel.dispose(); panels.clear(); }
  };
}
module.exports = {createController};
