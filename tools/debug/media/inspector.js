"use strict";
// Inspector windows: Memory, Registers, WASIp1, ROS, Breakpoints, Diagnostics.
// The extension pushes a JSON view; this script owns DOM, focus, selection,
// keyboard and scrolling. Re-rendering never resets the user's viewport.
(() => {
  const {vscode, h, icon, action, button, value, tree, menu, split, state, listKeys, debounce, applyEditorStyle, plural, copy} = uwvmUI;
  const app = document.getElementById("app");
  let view = JSON.parse(document.body.dataset.initial || "{}");
  const post = message => vscode.postMessage({...message, epoch: view.epoch});
  const local = {selection: state.get("selection") || null, anchor: undefined, editing: undefined,
    filter: state.get("filter") || "", openDetail: state.get("openDetail") || null, endian: state.get("endian") || "little",
    rounding: state.get("rounding") || "nearest_to_even", digits: state.get("digits") || "", valuesOpen: state.get("valuesOpen") !== false};

  let navigation = {back: false, forward: false};
  // ----------------------------------------------------------- shared chrome
  function titlebar(extraCrumbs, actions) {
    const status = view.status || {};
    const stateClass = status.state === "paused" ? "paused" : status.state === "error" ? "error" : ["detached", "joined"].includes(status.state) ? "ended" : "";
    const stateIcon = {paused: "debug-pause", error: "error", detached: "debug-disconnect", joined: "debug-stop", checking: "sync", retired: "history", idle: "circle-filled"}[status.state] || "circle-filled";
    const permIcon = {full: "unlock", "read-only": "eye", limited: "shield", none: "lock"}[view.permission.level] || "lock";
    return h("div.titlebar",
      h("div.crumbs",
        action("arrow-left", "Back (Alt+Left)", () => post({action:"navigate-back"}), {disabled:!navigation.back}),
        action("arrow-right", "Forward (Alt+Right)", () => post({action:"navigate-forward"}), {disabled:!navigation.forward}),
        icon(view.icon), h("span.title", view.title), view.instance > 1 ? h("span.crumb", "#" + view.instance) : null,
        h("span.sep.session", "›"), h("span.crumb.session", {title: view.session.name}, view.session.name),
        extraCrumbs,
        h("span.state." + stateClass, {title: status.text}, icon(stateIcon, status.state === "checking" ? "spin" : ""), status.text)),
      h("div.actions",
        actions,
        h("span.divider"),
        h("span.perm", {title: view.permission.detail}, icon(permIcon), view.permission.label),
        action("kebab-vertical", "More actions", event => menu(event, [
          {label: "Refresh", icon: "refresh", key: "Ctrl+R", run: () => post({action: "refresh"})},
          {label: "Use active debug session", icon: "debug-alt", run: () => post({action: "bind"})},
          {label: "Open another " + view.kind + " window", icon: "split-horizontal", run: () => post({action: "new-window"})},
          "-",
          {label: "Show diagnostics", icon: "info", run: () => post({action: "diagnostics"})}
        ]))));
  }
  const dismissedBanners = new Set();
  function banners() {
    const key = banner => JSON.stringify([view.session.id, banner.level, banner.text]);
    const current = new Set((view.banners || []).map(key));
    for (const dismissed of dismissedBanners) if (!current.has(dismissed)) dismissedBanners.delete(dismissed);
    const list = (view.banners || []).filter(banner => !dismissedBanners.has(key(banner))).map(banner => h("div.banner." + banner.level,
      {role: banner.level === "error" ? "alert" : "status"}, icon(banner.icon), h("div.text", banner.text),
      banner.actions ? h("div.banner-actions", banner.actions.map(item => button(item.label, () => bannerAction(item.id), {small: true, secondary: true, icon: item.icon}))) : null,
      action("close", "Dismiss notification", () => { dismissedBanners.add(key(banner)); draw(); })));
    return list.length ? h("div.banners", list) : null;
  }
  function bannerAction(id) {
    if (id === "refresh") post({action: "refresh"});
    if (id === "focus-written") {
      const written = view.memory && view.memory.write;
      if (!mem.scroller || !written || !/^(0|[1-9][0-9]*)$/.test(written.offset) ||
          !Number.isSafeInteger(written.count) || written.count < 1) return;
      const first = BigInt(written.offset), end = first + BigInt(written.count);
      if (end > mem.size) return;
      mem.anchor = first; mem.focus = end - 1n;
      scrollRowToTop(first / BigInt(mem.cols)); renderVisible(); paintSelection();
      mem.scroller.focus({preventScroll: true});
    }
  }
  function statusline(extra) {
    return h("div.statusline", (view.footer || []).map(item => h("span.item", icon(item.icon, item.spin ? "spin" : ""), item.text)), h("span.spacer"), extra);
  }
  function emptyState(iconName, title, text, actions) {
    return h("div.empty", icon(iconName), h("h3", title), text ? h("p", text) : null, actions);
  }
  function blocked() {
    if (!view.session.connected) return emptyState("debug-disconnect", "No debug session",
      view.session.everBound ? "The session this window was bound to has ended." : "Start a UWVM debug session, then this window connects automatically.",
      button("Use active session", () => post({action: "bind"}), {icon: "debug-alt"}));
    if (view.permission.level === "none") return emptyState("lock", "Debugger disabled", "This VM was launched with debugger permission none.");
    if (view.permission.level === "limited" && view.kind !== "diagnostics") return emptyState("shield", "Not available at limited permission",
      "Limited sessions expose numeric crash diagnostics only. Relaunch with full or read-only permission to inspect state.",
      button("Show diagnostics", () => post({action: "diagnostics"}), {icon: "info", secondary: true}));
    return null;
  }

  // ------------------------------------------------------------------ memory
  // A virtual hex grid over the WHOLE memory: a real scrollbar, native smooth
  // scrolling, and 256-byte pages fetched on demand. A new stop keeps the old
  // bytes on screen and replaces them in place, highlighting what changed.
  const PAGE = 256n, MAX_PAGES = 4096, HEX = Array.from({length: 256}, (_, n) => n.toString(16).padStart(2, "0"));
  const mem = {key: "", rowHeight: 20, base: 0n, segmentRows: 0, rows: 0n, cols: 16, size: 0n, digits: 8, showTags: false,
    pages: new Map(), pending: new Map(), changed: new Map(), written: new Map(), stopId: undefined, anchor: null, focus: null,
    drag: null, edit: null, drawn: new Map(), fetchTimer: 0, viewTimer: 0, dimTimer: 0, initialTop: null,
    scroller: null, space: null, rowsHost: null, legend: null, hosts: null};
  const bigMin = (a, b) => a < b ? a : b, bigMax = (a, b) => a > b ? a : b;
  const addressText = address => "0x" + BigInt(address).toString(16).padStart(mem.digits, "0");
  const memoryIdentity = () => [view.session.id, view.memory.selector.module, view.memory.selector.memory].join(":");
  function memoryKey() {
    const m = view.memory, i = m.info || {};
    return [view.session.id, m.selector.module, m.selector.memory, m.selector.columns, i.memoryBytes, i.addressBits, m.showTags].join(":");
  }
  function byteAt(address) {
    const page = mem.pages.get((address / PAGE).toString());
    if (!page) return null;
    const value = page.bytes[Number(address % PAGE)];
    if (value === undefined) return null;
    return {value, tag: page.tags.get((address / 16n * 16n).toString()), stale: page.stopId !== mem.stopId};
  }
  function memoryWindow() {
    const m = view.memory, sel = m.selector, info = m.info;
    const hosts = {title: h("div"), banners: h("div"), status: h("div")};
    mem.hosts = hosts;
    const toolbar = h("form.toolrow", {on: {submit: event => { event.preventDefault(); goToAddress(event.target.elements.offset.value, true); }}},
      h("label.field", "Module", h("input.num", {name: "module", value: sel.module, inputmode: "numeric", "aria-label": "Module index",
        on: {change: event => post({action: "refresh", fields: {module: event.target.value, offset: "0"}})}})),
      h("label.field", "Memory", h("input.num", {name: "memory", value: sel.memory, inputmode: "numeric", "aria-label": "Memory index",
        on: {change: event => post({action: "refresh", fields: {memory: event.target.value, offset: "0"}})}})),
      h("label.field", "Go to", h("input.mono", {name: "offset", value: "", size: 18, spellcheck: "false", placeholder: "0x… or decimal",
        title: "Hex (0x…) or decimal address. Enter jumps there.", "aria-label": "Go to address",
        // The toolbar has several text fields and no submit button, so the
        // browser does not submit on Enter; handle it explicitly.
        on: {keydown: event => { if (event.key === "Enter") { event.preventDefault(); goToAddress(event.target.value, true); } },
             input: event => event.target.classList.remove("invalid")}})),
      h("label.field", "Per row", h("select", {name: "columns", "aria-label": "Bytes per row", on: {change: event => { mem.initialTop = topAddress(); post({action: "columns", value: event.target.value}); }}},
        m.columns.map(n => h("option", {value: n, selected: n === sel.columns || undefined}, n)))),
      h("span.spacer"),
      h("label.field", {title: info && info.status === "available" ? "Colour each byte by its MTE allocation tag" : "Runtime MTE is not enabled for this memory"},
        h("input", {type: "checkbox", id: "memtag", checked: m.showTags || undefined, disabled: !info || undefined,
          on: {change: event => post({action: "toggle-tags", enabled: event.target.checked})}}), "Allocation tags"));
    let main;
    const denied = blocked();
    if (denied || !info) {
      mem.scroller = null;
      main = h("div.content", denied || emptyState("database", view.busy ? "Reading memory…" : view.status.state === "running" ? "The guest is running" : "No memory loaded",
        view.busy ? "" : "Pause the guest to read its memory.", view.busy ? null : button("Read memory", () => post({action: "refresh"}), {icon: "refresh"})));
    } else main = h("div.content", buildGrid());
    const body = mem.split = split(main, valuesPane(), {id: "memory-values", size: 330, min: 220, minFirst: 320, hidden: !local.valuesOpen,
      stackBelow: 330 + 16 * 26 + 120});
    updateMemoryChrome();
    return [hosts.title, toolbar, hosts.banners, h("div.body", body), hosts.status];
  }
  function buildGrid() {
    const m = view.memory, info = m.info;
    const sameMemory = mem.key && mem.key.split(":").slice(0, 3).join(":") === memoryKey().split(":").slice(0, 3).join(":");
    if (!sameMemory) {
      mem.pages.clear(); mem.pending.clear(); mem.changed.clear(); mem.written.clear(); mem.anchor = mem.focus = null;
      const saved = state.get("memorySelection");
      if (saved && saved.memory === memoryIdentity() && [saved.anchor, saved.focus].every(n =>
          typeof n === "string" && /^(0|[1-9][0-9]{0,19})$/.test(n) && BigInt(n) < BigInt(info.memoryBytes))) {
        mem.anchor = BigInt(saved.anchor); mem.focus = BigInt(saved.focus);
      }
    }
    mem.key = memoryKey(); mem.cols = m.selector.columns; mem.size = BigInt(info.memoryBytes); mem.showTags = m.showTags;
    if (mem.anchor !== null && (mem.anchor >= mem.size || mem.focus >= mem.size)) mem.anchor = mem.focus = null;
    mem.digits = info.addressBits / 4; mem.rows = (mem.size + BigInt(mem.cols) - 1n) / BigInt(mem.cols); mem.drawn.clear();
    if (mem.initialTop === null) mem.initialTop = BigInt(state.get("memTop:" + mem.key.split(":").slice(1, 3).join(":")) || m.selector.offset || "0");
    const head = h("div.hhead", {"aria-hidden": "true"}, h("span.haddr", "Address"),
      h("span.hbytes", Array.from({length: mem.cols}, (_, n) => [n && n % 8 === 0 && mem.cols > 8 ? h("span.byte-gap") : null, h("span.byte.colhead", n.toString(16).toUpperCase().padStart(2, "0"))])),
      h("span.htext", "Text"), mem.showTags ? h("span.htags", "Tag") : null);
    mem.rowsHost = h("div.hrows");
    mem.space = h("div.hspace", mem.rowsHost);
    mem.scroller = h("div.hexview", {tabindex: "0", role: "grid", "aria-label": "Memory bytes. Arrow keys move, Shift extends the selection, Enter or a hex digit edits."}, head, mem.space);
    // Both the header and virtual rows reserve the full address, including 0x.
    mem.scroller.style.setProperty("--addr-digits", String(mem.digits + 2));
    mem.legend = h("div.memory-legend", {hidden: !mem.showTags});
    hookGrid();
    requestAnimationFrame(() => {
      measureRows();
      const top = mem.initialTop !== null ? bigMin(mem.initialTop, mem.size ? mem.size - 1n : 0n) : 0n;
      mem.initialTop = null;
      scrollRowToTop(top / BigInt(mem.cols));
      renderVisible();
    });
    return [mem.scroller, mem.legend];
  }
  function measureRows() {
    const probe = h("div.hrow", h("span.haddr", "0x0"));
    mem.rowsHost.append(probe);
    mem.rowHeight = Math.max(14, Math.round(probe.getBoundingClientRect().height) || 20);
    probe.remove();
    mem.maxSegment = Math.max(1000, Math.floor(12000000 / mem.rowHeight));
    setBase(0n);
  }
  function setBase(row) {
    const seg = BigInt(mem.maxSegment);
    let base = row < 0n ? 0n : row;
    if (mem.rows <= seg) base = 0n; else if (base + seg > mem.rows) base = mem.rows - seg;
    mem.base = base; mem.segmentRows = Number(bigMin(seg, mem.rows - base));
    mem.space.style.height = mem.segmentRows * mem.rowHeight + "px";
    for (const node of mem.drawn.values()) node.remove();
    mem.drawn.clear();
  }
  function scrollRowToTop(row) {
    if (row < mem.base || row >= mem.base + BigInt(mem.segmentRows)) setBase(row - BigInt(Math.floor(mem.maxSegment / 2)));
    mem.scroller.scrollTop = Number(row - mem.base) * mem.rowHeight;
  }
  function topAddress() {
    if (!mem.scroller) return 0n;
    return (mem.base + BigInt(Math.floor(mem.scroller.scrollTop / mem.rowHeight))) * BigInt(mem.cols);
  }
  function headHeight() { const head = mem.scroller && mem.scroller.querySelector(".hhead"); return head ? head.offsetHeight : 0; }
  function visibleRange(overscan) {
    const top = Math.max(0, mem.scroller.scrollTop), height = Math.max(0, mem.scroller.clientHeight - headHeight());
    const first = Math.max(0, Math.floor(top / mem.rowHeight) - overscan);
    const last = Math.min(mem.segmentRows, Math.ceil((top + height) / mem.rowHeight) + overscan);
    return [first, last];
  }
  function renderVisible() {
    if (!mem.scroller) return;
    const [first, last] = visibleRange(6), keep = new Set();
    for (let i = first; i < last; i++) {
      const row = mem.base + BigInt(i), key = row.toString();
      keep.add(key);
      if (!mem.drawn.has(key)) { const node = h("div.hrow"); node.dataset.row = key; fillRow(node, row); mem.drawn.set(key, node); mem.rowsHost.append(node); }
    }
    for (const [key, node] of mem.drawn) if (!keep.has(key)) { if (mem.edit && node.contains(mem.edit.node)) continue; node.remove(); mem.drawn.delete(key); }
    updateLegend(); scheduleFetch(); scheduleViewReport();
  }
  function fillRow(node, row) {
    const cols = BigInt(mem.cols), start = row * cols, end = bigMin(start + cols, mem.size), now = performance.now();
    const bytesCell = h("span.hbytes");
    let text = "";
    for (let address = start, n = 0; address < end; address++, n++) {
      if (n && n % 8 === 0 && mem.cols > 8) bytesCell.append(h("span.byte-gap"));
      const span = document.createElement("span"), key = address.toString(), cell = byteAt(address);
      span.className = "byte"; span.dataset.address = key;
      if (!cell) { span.textContent = "··"; span.classList.add("missing"); text += " "; }
      else {
        span.textContent = HEX[cell.value];
        if (!cell.value) span.classList.add("zero");
        text += cell.value >= 32 && cell.value < 127 ? String.fromCharCode(cell.value) : "·";
        if (mem.showTags && cell.tag) { span.classList.add("tagged"); span.style.setProperty("--tag-colour", cell.tag.color); span.title = "Tag " + cell.tag.hex; }
      }
      const changed = mem.changed.get(key);
      if (changed !== undefined && now - changed < 1600) { span.classList.add("changed"); span.style.animationDelay = -(now - changed) + "ms"; }
      if (mem.written.has(key)) { span.classList.add("pending"); span.textContent = mem.written.get(key); }
      bytesCell.append(span);
    }
    const parts = [h("span.haddr", "0x" + start.toString(16).padStart(mem.digits, "0")), bytesCell, h("span.htext", text)];
    if (mem.showTags) {
      const chips = [];
      for (let granule = start / 16n * 16n; granule < end; granule += 16n) {
        const cell = byteAt(granule < start ? start : granule);
        if (cell && cell.tag) chips.push(h("span.tag-chip", {style: {"--tag-colour": cell.tag.color}, title: "Granule 0x" + granule.toString(16) + "–0x" + (granule + 15n).toString(16)}, cell.tag.hex));
      }
      parts.push(h("span.htags", chips));
    }
    node.replaceChildren(...parts);
    node.style.top = Number(row - mem.base) * mem.rowHeight + "px";
    paintRowSelection(node);
  }
  function selectionRange() {
    if (mem.anchor === null || mem.focus === null) return null;
    return [bigMin(mem.anchor, mem.focus), bigMax(mem.anchor, mem.focus)];
  }
  function paintRowSelection(node) {
    const range = selectionRange();
    for (const span of node.querySelectorAll(".byte[data-address]")) {
      const address = BigInt(span.dataset.address);
      span.classList.toggle("selected", !!range && address >= range[0] && address <= range[1]);
      span.classList.toggle("focus", mem.focus !== null && address === mem.focus);
    }
  }
  function paintSelection() {
    state.set("memorySelection", {memory: memoryIdentity(), anchor: mem.anchor === null ? null : mem.anchor.toString(), focus: mem.focus === null ? null : mem.focus.toString()});
    for (const node of mem.drawn.values()) paintRowSelection(node);
    updateValues(); updateSelectionStatus();
  }
  function repaintPages(touched) {
    const cols = BigInt(mem.cols);
    for (const [key, node] of mem.drawn) {
      if (mem.edit && node.contains(mem.edit.node)) continue;
      const start = BigInt(key) * cols, first = start / PAGE, last = (start + cols - 1n) / PAGE;
      if (!touched || touched.has(first.toString()) || touched.has(last.toString())) fillRow(node, BigInt(key));
    }
    updateLegend();
  }
  function updateLegend() {
    if (!mem.legend) return;
    if (!mem.showTags) { mem.legend.hidden = true; return; }
    const seen = new Map();
    for (const span of mem.rowsHost.querySelectorAll(".tag-chip")) seen.set(span.textContent, span.style.getPropertyValue("--tag-colour"));
    mem.legend.hidden = !seen.size;
    mem.legend.replaceChildren("Tags:", ...[...seen].sort().map(([hexTag, colour]) => h("span.swatch", {style: {"--tag-colour": colour}}, h("i"), hexTag)));
  }
  function scheduleFetch() { clearTimeout(mem.fetchTimer); mem.fetchTimer = setTimeout(fetchVisible, 12); }
  function fetchVisible() {
    if (!mem.scroller || !mem.stopId || !view.memory || !view.memory.info) return;
    const [first, last] = visibleRange(24), cols = BigInt(mem.cols);
    const start = (mem.base + BigInt(first)) * cols, end = bigMin((mem.base + BigInt(last)) * cols, mem.size), want = [];
    for (let page = start / PAGE; page * PAGE < end && want.length < 24; page++) {
      const key = page.toString(), entry = mem.pages.get(key);
      if (entry && entry.stopId === mem.stopId || mem.pending.get(key) === mem.stopId) continue;
      mem.pending.set(key, mem.stopId); want.push((page * PAGE).toString());
    }
    if (want.length) post({action: "mem-read", stopId: mem.stopId, pages: want});
  }
  function receivePages(message) {
    // Requests the controller could not serve come back as "dropped": forget
    // them so the next scroll or stop asks again instead of waiting forever.
    for (const raw of message.dropped || []) { const key = (BigInt(raw) / PAGE).toString(); if (mem.pending.get(key) === message.stopId) mem.pending.delete(key); }
    if (message.dropped && message.dropped.length && message.stopId === mem.stopId) setTimeout(scheduleFetch, 250);
    if (message.stopId !== mem.stopId || !mem.scroller) return;
    const now = performance.now(), touched = new Set();
    for (const page of message.pages) {
      const offset = BigInt(page.offset), key = (offset / PAGE).toString(), bytes = new Uint8Array(page.hex.length / 2);
      for (let i = 0; i < bytes.length; i++) bytes[i] = parseInt(page.hex.substr(i * 2, 2), 16);
      const old = mem.pages.get(key);
      if (old && old.stopId !== message.stopId)
        for (let i = 0; i < bytes.length; i++) if (old.bytes[i] !== undefined && old.bytes[i] !== bytes[i]) mem.changed.set((offset + BigInt(i)).toString(), now);
      for (let i = 0; i < bytes.length; i++) mem.written.delete((offset + BigInt(i)).toString());
      mem.pages.delete(key);
      mem.pages.set(key, {bytes, tags: new Map(page.tags.map(([granule, hexTag, colour]) => [granule, {hex: hexTag, color: colour}])), stopId: message.stopId});
      mem.pending.delete(key); touched.add(key);
    }
    while (mem.pages.size > MAX_PAGES) mem.pages.delete(mem.pages.keys().next().value);
    repaintPages(touched); updateValues();
    if (mem.changed.size) setTimeout(() => {
      const limit = performance.now() - 1600;
      for (const [key, time] of mem.changed) if (time < limit) mem.changed.delete(key);
    }, 1700);
  }
  function memoryStop(stopId) {
    if (stopId !== mem.stopId) { mem.stopId = stopId; mem.pending.clear(); }
    // Every view update re-checks the rows on screen; only pages that are not
    // yet current for this stop are requested.
    if (stopId && mem.scroller) { mem.scroller.classList.remove("stale"); scheduleFetch(); }
  }
  function scheduleViewReport() {
    clearTimeout(mem.viewTimer);
    mem.viewTimer = setTimeout(() => {
      if (!mem.scroller) return;
      const top = topAddress().toString();
      state.set("memTop:" + mem.key.split(":").slice(1, 3).join(":"), top);
      post({action: "mem-view", offset: top});
    }, 400);
  }
  function hookGrid() {
    const scroller = mem.scroller;
    scroller.addEventListener("scroll", () => {
      if (mem.rows > BigInt(mem.maxSegment || 0)) {
        const absolute = mem.base + BigInt(Math.floor(scroller.scrollTop / mem.rowHeight));
        const nearTop = mem.base > 0n && scroller.scrollTop < scroller.clientHeight * 2;
        const nearEnd = mem.base + BigInt(mem.segmentRows) < mem.rows && scroller.scrollTop > mem.segmentRows * mem.rowHeight - scroller.clientHeight * 3;
        if (nearTop || nearEnd) { setBase(absolute - BigInt(Math.floor(mem.maxSegment / 2))); scroller.scrollTop = Number(absolute - mem.base) * mem.rowHeight; }
      }
      renderVisible();
    }, {passive: true});
    scroller.addEventListener("pointerdown", event => {
      if (event.button !== 0) return;
      const node = event.target.closest(".byte[data-address]"); if (!node) return;
      event.preventDefault(); cancelEdit(); scroller.focus({preventScroll: true});
      const address = BigInt(node.dataset.address);
      if (!(event.shiftKey && mem.anchor !== null)) mem.anchor = address;
      mem.focus = address; mem.drag = {pointer: event.pointerId};
      try { scroller.setPointerCapture(event.pointerId); } catch (_) { }
      paintSelection();
    });
    scroller.addEventListener("pointermove", event => {
      if (!mem.drag) return;
      // Pressed state is checked on every move: a release outside the view
      // can never leave the selection following the mouse.
      if (!(event.buttons & 1)) { endDrag(); return; }
      const target = document.elementFromPoint(event.clientX, event.clientY), node = target && target.closest && target.closest(".byte[data-address]");
      if (node) { const address = BigInt(node.dataset.address); if (address !== mem.focus) { mem.focus = address; paintSelection(); } }
      const box = scroller.getBoundingClientRect();
      if (event.clientY < box.top + headHeight() + 8) scroller.scrollTop -= mem.rowHeight;
      else if (event.clientY > box.bottom - 8) scroller.scrollTop += mem.rowHeight;
    });
    for (const name of ["pointerup", "pointercancel", "lostpointercapture"]) scroller.addEventListener(name, endDrag);
    // Pointer capture (for drag-select) retargets click/dblclick to the grid
    // itself, so find the byte under the pointer from its coordinates.
    const byteAtPoint = event => { const hit = document.elementFromPoint(event.clientX, event.clientY); return hit && hit.closest ? hit.closest(".byte[data-address]") : null; };
    scroller.addEventListener("dblclick", event => { const node = byteAtPoint(event); if (node && !node.classList.contains("colhead")) beginEdit(BigInt(node.dataset.address)); });
    scroller.addEventListener("contextmenu", event => {
      const node = event.target.closest(".byte[data-address]");
      if (node) { const address = BigInt(node.dataset.address), range = selectionRange(); if (!range || address < range[0] || address > range[1]) { mem.anchor = mem.focus = address; paintSelection(); } }
      const range = selectionRange(); if (!range) return;
      const picked = selectedBytes();
      menu(event, [
        {label: "Edit byte", icon: "edit", key: "Enter", disabled: !view.memory.canWrite, run: () => beginEdit(range[0])},
        {label: "Edit selection…", icon: "edit", disabled: !view.memory.canWrite || !picked || !picked.bytes, run: () => openWriteBox()},
        "-",
        {label: "Copy hex", icon: "copy", key: "Ctrl+C", disabled: !picked || !picked.bytes, run: () => copy(picked.bytes.map(n => HEX[n]).join(" "))},
        {label: "Copy as C array", icon: "copy", disabled: !picked || !picked.bytes, run: () => copy("{ " + picked.bytes.map(n => "0x" + HEX[n]).join(", ") + " }")},
        {label: "Copy address", icon: "copy", run: () => copy(addressText(range[0]))},
        "-",
        {label: "Go to address…", icon: "go-to-file", key: "Ctrl+G", run: () => focusAddress()}
      ]);
    });
    scroller.addEventListener("keydown", memoryKeys);
  }
  function endDrag() {
    if (!mem.drag) return;
    try { mem.scroller.releasePointerCapture(mem.drag.pointer); } catch (_) { }
    mem.drag = null;
  }
  addEventListener("blur", () => endDrag());
  function ensureVisible(address) {
    const row = address / BigInt(mem.cols);
    if (row < mem.base || row >= mem.base + BigInt(mem.segmentRows)) setBase(row - BigInt(Math.floor(mem.maxSegment / 2)));
    const top = Number(row - mem.base) * mem.rowHeight, view = mem.scroller.clientHeight - headHeight();
    if (top < mem.scroller.scrollTop) mem.scroller.scrollTop = top;
    else if (top + mem.rowHeight > mem.scroller.scrollTop + view) mem.scroller.scrollTop = top + mem.rowHeight - view;
    renderVisible();
  }
  function memoryKeys(event) {
    if (mem.edit || !mem.scroller) return;
    const cols = BigInt(mem.cols), last = mem.size ? mem.size - 1n : 0n, focus = mem.focus !== null ? mem.focus : topAddress();
    const pageRows = BigInt(Math.max(1, Math.floor((mem.scroller.clientHeight - headHeight()) / mem.rowHeight) - 1));
    const ctrl = event.ctrlKey || event.metaKey;
    let next;
    if (event.key === "ArrowRight") next = focus + 1n;
    else if (event.key === "ArrowLeft") next = focus - 1n;
    else if (event.key === "ArrowDown") next = focus + cols;
    else if (event.key === "ArrowUp") next = focus - cols;
    else if (event.key === "PageDown") next = focus + pageRows * cols;
    else if (event.key === "PageUp") next = focus - pageRows * cols;
    else if (event.key === "Home") next = ctrl ? 0n : focus - focus % cols;
    else if (event.key === "End") next = ctrl ? last : bigMin(last, focus - focus % cols + cols - 1n);
    if (next !== undefined) {
      event.preventDefault();
      next = next < 0n ? 0n : next > last ? last : next;
      if (!event.shiftKey || mem.anchor === null) mem.anchor = next;
      mem.focus = next; ensureVisible(next); paintSelection(); return;
    }
    if ((event.key === "Enter" || event.key === "F2") && mem.focus !== null) { event.preventDefault(); beginEdit(mem.focus); return; }
    if (/^[0-9a-f]$/i.test(event.key) && !ctrl && !event.altKey && mem.focus !== null) { event.preventDefault(); beginEdit(mem.focus, event.key); return; }
    if (ctrl && event.key.toLowerCase() === "c") { const picked = selectedBytes(); if (picked && picked.bytes) { event.preventDefault(); copy(picked.bytes.map(n => HEX[n]).join(" ")); } return; }
    if (ctrl && event.key.toLowerCase() === "a") {
      event.preventDefault();
      const [first, lastRow] = visibleRange(0);
      mem.anchor = (mem.base + BigInt(first)) * cols; mem.focus = bigMin(last, (mem.base + BigInt(lastRow)) * cols - 1n); paintSelection(); return;
    }
    if (event.key === "Escape") { mem.anchor = mem.focus = null; paintSelection(); }
  }
  function beginEdit(address, first) {
    if (!view.memory.canWrite || view.busy) {
      flash(view.permission.level !== "full" ? "Editing needs full debugger permission; this session is " + view.permission.label.toLowerCase() + "." :
        view.status.state === "running" ? "Pause the guest to edit memory." : "Writing… the next edit opens when the VM has confirmed this one.");
      return;
    }
    ensureVisible(address);
    const node = mem.scroller.querySelector('.byte[data-address="' + address + '"]');
    if (!node || node.classList.contains("missing")) return;
    cancelEdit();
    const input = h("input", {value: first !== undefined ? first : node.textContent, maxlength: 2, spellcheck: "false", "aria-label": "New value for byte " + addressText(address)});
    node.classList.add("editing"); node.replaceChildren(input);
    mem.edit = {address, node, input};
    input.focus(); if (first === undefined) input.select(); else input.setSelectionRange(1, 1);
    const commit = () => {
      if (!/^[0-9a-f]{2}$/i.test(input.value)) { input.classList.add("invalid"); flash("Enter two hex digits (00–ff)."); return; }
      const value = input.value.toLowerCase(), cell = byteAt(address);
      cancelEdit(); mem.scroller.focus({preventScroll: true});
      if (cell && HEX[cell.value] === value) return;
      mem.written.set(address.toString(), value); repaintPages(new Set([(address / PAGE).toString()]));
      post({action: "write", fields: {writeOffset: address.toString(), hex: value}});
      if (address + 1n < mem.size) { mem.anchor = mem.focus = address + 1n; ensureVisible(address + 1n); paintSelection(); }
    };
    input.addEventListener("keydown", event => {
      event.stopPropagation();
      if (event.key === "Escape") { event.preventDefault(); cancelEdit(); mem.scroller.focus({preventScroll: true}); }
      else if (event.key === "Enter" || event.key === "Tab") { event.preventDefault(); commit(); }
    });
    input.addEventListener("input", () => { input.classList.toggle("invalid", !/^[0-9a-f]{0,2}$/i.test(input.value)); if (first !== undefined && /^[0-9a-f]{2}$/i.test(input.value)) commit(); });
    input.addEventListener("blur", () => setTimeout(() => { if (mem.edit && mem.edit.input === input) cancelEdit(); }, 0));
  }
  function cancelEdit() {
    if (!mem.edit) return;
    const {node, address} = mem.edit; mem.edit = undefined;
    node.classList.remove("editing");
    const row = node.closest(".hrow");
    if (row && row.isConnected) fillRow(row, address / BigInt(mem.cols)); else node.textContent = "··";
  }
  function flash(text) { const node = document.getElementById("selection-status"); if (node) { node.textContent = text; node.classList.add("pending"); } }
  function focusAddress() { const input = document.querySelector("input[name=offset]"); if (input) { input.focus(); input.select(); } }
  function goToAddress(raw, select) {
    const input = document.querySelector("input[name=offset]");
    let address;
    try {
      const text = String(raw || "").trim().replace(/_/g, "");
      if (!/^(0x[0-9a-f]+|[0-9]+)$/i.test(text)) throw new Error();
      address = BigInt(text);
      if (address >= mem.size) throw new Error();
    } catch (_) { if (input) { input.classList.add("invalid"); input.title = "Enter an address below 0x" + mem.size.toString(16); } return; }
    if (input) { input.classList.remove("invalid"); input.value = ""; }
    scrollRowToTop(address / BigInt(mem.cols)); renderVisible();
    if (select) { mem.anchor = mem.focus = address; paintSelection(); mem.scroller.focus({preventScroll: true}); }
  }
  function selectionStatus() { return h("span.item#selection-status", ""); }
  function selectedBytes() {
    const range = selectionRange(); if (!range) return null;
    const count = range[1] - range[0] + 1n;
    if (count > 256n) return {range, count, bytes: null, reason: "Select at most 256 bytes to interpret or edit them together."};
    const bytes = [];
    for (let address = range[0]; address <= range[1]; address++) { const cell = byteAt(address); if (!cell) return {range, count, bytes: null, reason: "Loading the selected bytes…"}; bytes.push(cell.value); }
    return {range, count, bytes};
  }
  function updateSelectionStatus() {
    const node = document.getElementById("selection-status"); if (!node) return;
    node.classList.remove("pending");
    const range = selectionRange();
    if (!range) { node.textContent = view.memory && view.memory.canWrite ? "Click a byte to select · Enter, double-click or a hex digit edits" : "Click a byte to select"; return; }
    const count = range[1] - range[0] + 1n;
    node.textContent = "Selection " + addressText(range[0]) + (count > 1n ? "–" + addressText(range[1]) : "") + " · " + plural(count.toString(), "byte");
  }
  function valuesPane() {
    const controls = h("div.values-controls",
      h("select#value-endian", {"aria-label": "Byte order", on: {change: event => { local.endian = event.target.value; state.set("endian", local.endian); updateValues(); }}},
        [["little", "Little-endian"], ["big", "Big-endian"], ["pdp", "PDP-endian"]].map(([v, t]) => h("option", {value: v, selected: local.endian === v || undefined}, t))),
      h("select#value-rounding", {"aria-label": "Decimal rounding", title: "Decimal display rounding (display only)", on: {change: event => { local.rounding = event.target.value; state.set("rounding", local.rounding); updateValues(); }}},
        uwvmMemoryValues.ROUNDING.map(mode => h("option", {value: mode, selected: local.rounding === mode || undefined}, mode.replaceAll("_", " ")))),
      h("input#value-digits", {type: "number", min: 1, max: 36, placeholder: "Auto", value: local.digits, title: "Significant digits (1–36, empty = automatic)", "aria-label": "Significant digits",
        on: {input: debounce(event => { local.digits = event.target.value; state.set("digits", local.digits); updateValues(); }, 150)}}));
    return [h("div.side-head", h("span.grow", "Selection"), action("close", "Hide value panel", () => { local.valuesOpen = false; state.set("valuesOpen", false); draw(); })),
      h("div.pane", h("div.values", h("div.range#value-range", "No selection"), controls, h("table#value-table"))),
      h("div.write-box#write-box", {hidden: true},
        h("label.field.stack", h("span", "Write selection"), h("textarea#write-hex", {rows: 2, spellcheck: "false", placeholder: "41 42 43 00", "aria-label": "Replacement bytes in hex"})),
        h("div.row", button("Write bytes", () => submitWrite(), {small: true, icon: "save"}), button("Cancel", () => { document.getElementById("write-box").hidden = true; }, {small: true, secondary: true})))];
  }
  // Characters render as 'a' 'b' nul tab: grey quotes, coloured characters,
  // control characters by name.
  function typedValue(row) {
    if (!row.parts) return value(row.value);
    const out = h("span.chars");
    row.parts.forEach((part, index) => {
      if (index) out.append(" ");
      if (part.kind === "char") out.append(h("span.q", "'"), h("span.v-string", part.text), h("span.q", "'"));
      else out.append(h("span.ctl", part.text));
    });
    return out;
  }
  function updateValues() {
    const range = document.getElementById("value-range"), tableNode = document.getElementById("value-table");
    if (!range || !tableNode) return;
    const picked = selectedBytes();
    if (!picked) { range.textContent = "Select bytes to interpret them."; tableNode.replaceChildren(); return; }
    range.textContent = addressText(picked.range[0]) + (picked.count > 1n ? " – " + addressText(picked.range[1]) : "") + " · " + plural(picked.count.toString(), "byte");
    if (!picked.bytes) { tableNode.replaceChildren(h("tr", h("td.val", {colspan: 2}, h("span.v-empty", picked.reason)))); return; }
    const digits = Number(local.digits);
    let rows = [];
    try { rows = uwvmMemoryValues.interpret(picked.bytes, local.endian, {rounding: local.rounding, ...(local.digits && digits >= 1 && digits <= 36 ? {precision: digits} : {})}); }
    catch (error) { rows = [{name: "Error", value: String(error.message || error)}]; }
    tableNode.replaceChildren(...rows.map(row => h("tr", {title: row.exact ? (row.rounded ? "Rounded display (" + local.rounding.replaceAll("_", " ") + "). " : "") + "Exact: " + row.exact : undefined, on: {dblclick: () => copy(row.value)}},
      h("td.name", row.name), h("td.val", typedValue(row)))));
    if (!rows.length) tableNode.append(h("tr", h("td.val", {colspan: 2}, h("span.v-empty", "No fixed-width interpretation for " + plural(picked.count.toString(), "byte") + "."))));
  }
  function openWriteBox() {
    const box = document.getElementById("write-box"), picked = selectedBytes(); if (!box || !picked || !picked.bytes) return;
    if (!local.valuesOpen) { local.valuesOpen = true; state.set("valuesOpen", true); draw(); return openWriteBox(); }
    box.hidden = false; const area = document.getElementById("write-hex"); area.value = picked.bytes.map(n => HEX[n]).join(" "); area.focus(); area.select();
  }
  function submitWrite() {
    const picked = selectedBytes(), area = document.getElementById("write-hex"); if (!picked || !area) return;
    post({action: "write", fields: {writeOffset: picked.range[0].toString(), hex: area.value}});
    document.getElementById("write-box").hidden = true;
  }
  // Chrome (title, banners, status) updates in place; the grid stays.
  function updateMemoryChrome() {
    if (!mem.hosts) return;
    if (mem.split) for (const pane of mem.split.children) if (pane.classList.contains("second") || pane.classList.contains("sash")) pane.hidden = !local.valuesOpen;
    mem.hosts.title.replaceChildren(titlebar([], [
      action("refresh", "Refresh", () => post({action: "refresh"}), {disabled: view.busy, key: "Ctrl+R"}),
      action("layout-sidebar-right", "Toggle value panel", () => { local.valuesOpen = !local.valuesOpen; state.set("valuesOpen", local.valuesOpen); draw(); }, {pressed: local.valuesOpen}),
      action("split-horizontal", "Open another memory window", () => post({action: "new-window"}))]));
    mem.hosts.banners.replaceChildren(banners() || "");
    mem.hosts.status.replaceChildren(statusline(selectionStatus()));
    updateSelectionStatus();
    if (mem.scroller) {
      clearTimeout(mem.dimTimer);
      if (view.status.state === "running") mem.dimTimer = setTimeout(() => mem.scroller && mem.scroller.classList.add("stale"), 1500);
      else mem.scroller.classList.remove("stale");
    }
    memoryStop(view.session.stopId);
  }

  // --------------------------------------------------------- generic tables
  function grid(columns, rows, options = {}) {
    const twist = !!options.detail;
    const table = h("table.grid", {role: "grid"},
      h("thead", h("tr", twist ? h("th") : null, columns.map(column => h("th", {style: column.width ? {width: column.width} : undefined}, column.title)), options.actions ? h("th") : null)));
    const body = h("tbody");
    rows.forEach((row, index) => {
      const rowKey = options.key ? options.key(row) : String(index), open = twist && local.openDetail === rowKey;
      const tr = h("tr", {tabindex: "-1", dataset: {index: String(index), key: rowKey}, class: options.rowClass ? options.rowClass(row) : undefined, "aria-expanded": twist ? String(open) : undefined},
        twist ? h("td.twist", h("button.twistie", {type: "button", "aria-label": open ? "Hide details" : "Show details", on: {click: event => { event.stopPropagation(); toggleDetail(rowKey); }}},
          icon(open ? "chevron-down" : "chevron-right"))) : null,
        columns.map(column => h("td" + (column.cls ? "." + column.cls : ""), column.render ? column.render(row) : value(row[column.key]))),
        options.actions ? h("td.actions-cell", h("span.row-actions", options.actions(row))) : null);
      tr.addEventListener("click", () => select(tr));
      if (options.onContext) tr.addEventListener("contextmenu", event => { select(tr); options.onContext(event, row); });
      if (options.onOpen) tr.addEventListener("dblclick", () => options.onOpen(row));
      body.append(tr);
      if (open) body.append(h("tr.detail-row", h("td", {colspan: columns.length + (options.actions ? 1 : 0) + 1}, options.detail(row))));
    });
    function select(tr) { body.querySelectorAll("tr[aria-selected]").forEach(r => r.removeAttribute("aria-selected")); tr.setAttribute("aria-selected", "true"); tr.focus({preventScroll: true}); }
    table.append(body);
    listKeys(body, "tr[data-index]", tr => select(tr), (tr, event) => { if (options.onOpen) options.onOpen(rows[Number(tr.dataset.index)], event); });
    if (twist) body.addEventListener("keydown", event => {
      const tr = event.target.closest && event.target.closest("tr[data-index]"); if (!tr) return;
      const isOpen = tr.getAttribute("aria-expanded") === "true";
      if (event.key === "ArrowRight" && !isOpen || event.key === "ArrowLeft" && isOpen) { event.preventDefault(); toggleDetail(tr.dataset.key); }
    });
    return table;
  }
  function toggleDetail(key) { local.openDetail = local.openDetail === key ? null : key; state.set("openDetail", local.openDetail); draw(); }
  function filterBox(placeholder, total, shown) {
    const input = h("input", {type: "search", placeholder, value: local.filter, "aria-label": placeholder, spellcheck: "false",
      on: {input: debounce(event => { local.filter = event.target.value; state.set("filter", local.filter); draw(true); }, 120)}});
    return h("div.filter", icon("filter"), input, h("span.count", local.filter ? shown + " of " + total : ""),
      local.filter ? action("close", "Clear filter", () => { local.filter = ""; state.set("filter", ""); draw(); }) : null);
  }
  function matches(row) {
    if (!local.filter) return true;
    const needle = local.filter.toLowerCase();
    return JSON.stringify(row).toLowerCase().includes(needle);
  }
  function pager(info, prevAction, nextAction, label) {
    if (!info) return null;
    return h("div.pager", h("span", label || (info.total !== undefined ? plural(info.total, "record") : "")), h("span.spacer"),
      button("Previous", () => post({action: prevAction}), {small: true, secondary: true, disabled: !info.back || view.busy, icon: "chevron-left"}),
      button("Next", () => post({action: nextAction}), {small: true, secondary: true, disabled: !info.more || view.busy, icon: "chevron-right"}));
  }
  function genericGroup(data, emptyTitle) {
    if (!data) return emptyState("sync", view.busy ? "Reading…" : "No data yet", view.busy ? "" : "Refresh at a stop to read this group.");
    if (data.error) return h("div.banner.error", icon("error"), h("div.text", data.error));
    if (data.available === false) return emptyState("circle-slash", "Unavailable", data.status || "The runtime does not provide this group at this stop.");
    if (!data.rows.length) return emptyState("inbox", emptyTitle || "Empty", "The runtime reports no records here at this stop.");
    const shown = data.rows.filter(matches);
    const keyOf = row => JSON.stringify(row).slice(0, 240);
    const columns = data.columns.map(key => ({key, title: humanize(key),
      cls: /(^|_)(name|key|value|text|path|topic|type|message)$/i.test(key) ? "wrap" : /ns$|count|calls|errors|bytes|seq|instance|generation|total|min|max/i.test(key) ? "num" : "nowrap",
      render: row => /_ns$|^ns$/.test(key) && /^[0-9]+$/.test(String(row[key])) ? h("span.v-number", {title: row[key] + " ns"}, formatNs(row[key])) : cellValue(row[key])}));
    return [data.hidden && data.hidden.length ? h("div.inline-empty", {title: data.hidden.join(", ")}, plural(data.hidden.length, "empty column") + " hidden · double-click a row for every field") : null,
      grid(columns, shown, {detail: row => tree(row.__raw || row, "$row", {expandDepth: 0}), key: keyOf, onOpen: row => toggleDetail(keyOf(row)),
        actions: row => [action("json", "Copy row as JSON", () => copy(JSON.stringify(row.__raw || row, null, 2)))]})];
  }
  function cellValue(cell) {
    if (cell !== null && typeof cell === "object" && !("bytesBase64" in cell)) {
      const summary = Array.isArray(cell) ? "Array(" + cell.length + ")" : "{" + Object.keys(cell).slice(0, 3).join(", ") + (Object.keys(cell).length > 3 ? ", …" : "") + "}";
      return h("span.summary.v-null", {title: JSON.stringify(cell, null, 1).slice(0, 2000)}, summary);
    }
    return value(cell);
  }
  function humanize(key) { return String(key).replace(/[_-]+/g, " ").replace(/([a-z])([A-Z])/g, "$1 $2").replace(/^./, c => c.toUpperCase()); }

  // ------------------------------------------------------- recorded calls
  function callsTable(group, family) {
    if (group.error) return h("div.banner.error", icon("error"), h("div.text", group.error));
    const calls = group.calls || [];
    const recording = group.journal && (Number(group.journal.enabled) & ({wasip1: 1, ros1: 2, ros2: 4, rosv2: 8}[family] || 0));
    if (!calls.length) return emptyState("record", "No recorded calls",
      recording ? "Recording is on. Run the guest and pause again to inspect new calls." :
      family === "wasip1" ? "Start recording, run the guest, and pause again. Calls made while recording appear here." :
        "Start recording for " + (family === "rosv2" ? "u2rosv2" : "u2rosv1 › " + family) + ", run the guest and pause again.");
    // A ROS result can be a version, handle or boolean. Only the WASI
    // Preview 1 errno convention gives a generic nonzero error predicate.
    const failed = row => family === "wasip1" && row["has-result"] === "1" && row.result !== "0";
    const shown = calls.filter(matches);
    if (family === "rosv2") return grid([
      {title: "#", width: "48px", cls: "num", render: row => row.id},
      {title: "Import module", cls: "wrap", render: row => row.namespace},
      {title: "Function", cls: "wrap", render: row => row.function},
      {title: "Raw result", cls: "num", render: row => row.result}
    ], shown, {key: row => "call:" + row.id});
    return grid([
      {title: "#", width: "48px", cls: "num", render: row => h("span.v-number", row.id)},
      {title: "Function", cls: "wrap", render: row => h("span", h("span.status-dot" + (row.returned === "1" ? (failed(row) ? ".bad" : "") : ".unknown")),
        h("span.v-name", row.function || (row.functionBytes && row.functionBytes.utf8IfValid) || "?"), " ", h("span.v-null", row.namespace || ""))},
      {title: "Arguments", cls: "wrap code", render: row => (row.args || []).map((arg, n) => [n ? ", " : "", h("span.v-number", {title: "u" + arg.widthBits + " " + arg.unsigned}, arg.hex)])},
      {title: "Result", cls: "nowrap", render: row => row["has-result"] === "1" ? h("span" + (failed(row) ? ".v-error" : ".v-number"), row.result) : h("span.v-null", "—")},
      {title: "Duration", cls: "num", render: row => row.timed === "1" ? h("span", formatNs(row["duration-ns"])) : h("span.v-null", "—")},
      {title: "Payload", cls: "num", render: row => h("span", {title: "Captured payload segments"}, row.segments)}
    ], shown, {key: row => "call:" + row.id, onOpen: row => toggleDetail("call:" + row.id), detail: row => callDetail(row),
      actions: row => [action("file-binary", "Read payload bytes", () => post({action: "payload", fields: {call: row.id, segment: "0", offset: "0", count: "256"}}), {disabled: row.segments === "0" || view.busy}),
        action("json", "Copy as JSON", () => copy(JSON.stringify(row, null, 2)))],
      onContext: (event, row) => menu(event, [
        {label: "Show details", icon: "eye", run: () => toggleDetail("call:" + row.id)},
        {label: "Read payload bytes", icon: "file-binary", disabled: row.segments === "0", run: () => post({action: "payload", fields: {call: row.id, segment: "0", offset: "0", count: "256"}})},
        "-", {label: "Copy as JSON", icon: "json", run: () => copy(JSON.stringify(row, null, 2))}])});
  }
  function formatNs(ns) { const n = Number(ns); if (!Number.isFinite(n)) return ns; if (n < 1e3) return n + " ns"; if (n < 1e6) return (n / 1e3).toFixed(1) + " µs"; if (n < 1e9) return (n / 1e6).toFixed(2) + " ms"; return (n / 1e9).toFixed(3) + " s"; }
  function callDetail(row) {
    const fields = {
      Call: row.id, Function: (row.namespace || "") + "." + (row.function || ""), Module: row.module + (row.moduleName ? " (" + row.moduleName.utf8IfValid + ")" : ""),
      Participant: row.participant, Generation: row.generation, Environment: row.environment,
      Returned: row.returned === "1" ? "yes" : "no", Result: row["has-result"] === "1" ? row.result : "none", Duration: row.timed === "1" ? formatNs(row["duration-ns"]) : "not timed",
      Segments: row.segments, Incomplete: row.incomplete === "1" ? "yes" : "no"};
    return h("div", h("dl.detail-grid", Object.entries(fields).map(([k, v]) => [h("dt", k), h("dd", value(v))])),
      row.args && row.args.length ? h("div", {style: {marginTop: "6px"}}, h("b", "Arguments"), tree(row.args, "$args" + row.id, {expandDepth: 1})) : null);
  }
  function payloadView(payload, selector) {
    if (!selector) return null;
    const request = fields => post({action: "payload", fields: {...selector, ...fields}});
    const controls = h("form.toolrow", {on: {submit: event => {
      event.preventDefault(); const fields = event.target.elements;
      request({call: fields.call.value, segment: fields.segment.value, offset: fields.offset.value, count: fields.count.value});
    }}}, h("b", "Payload"),
      ["call", "segment", "offset", "count"].map(key => h("label.field", humanize(key),
        h("input.num", {name: key, value: selector[key], "aria-label": "Payload " + key, inputmode: "numeric"}))),
      button("Read", null, {submit: true, small: true, disabled: view.busy}));
    if (!payload) return controls;
    const hexText = payload.hex || "", pairs = hexText.match(/../g) || [];
    return [controls, h("div.section-card", h("header", icon("file-binary"), h("span.grow", "Payload · call " + payload.call + " · segment " + payload.segment),
      h("span.chip.outline", payload.direction), h("span.chip.outline", "guest 0x" + BigInt(payload["guest-offset"] || 0).toString(16)),
      action("copy", "Copy hex", () => copy(pairs.join(" ")))),
      h("div.content", h("div.mono", {style: {whiteSpace: "pre-wrap", wordBreak: "break-all"}}, pairs.join(" ") || "(empty)"),
        payload.utf8IfValid !== undefined ? h("div", {style: {marginTop: "6px"}}, h("span.v-null", "UTF-8: "), value(JSON.stringify(payload.utf8IfValid))) : null,
        h("div.v-null", {style: {marginTop: "6px"}}, "Bytes " + payload.offset + "–" + payload.next + " of " + payload.size + (payload.more ? " · more available" : ""))),
      h("div.toolrow", button("Next bytes", () => request({offset: payload.next}),
        {small: true, secondary: true, disabled: view.busy || !payload.more})))];
  }
  function recordingBar(group, family, extra) {
    const journal = group && group.journal, canWrite = view.permission.level === "full";
    const on = journal && (Number(journal.enabled) & ({wasip1: 1, ros1: 2, ros2: 4, rosv2: 8}[family] || 0));
    return h("div.toolrow",
      h("span.kind-pill.recording-state" + (on ? ".on" : ".off"), icon("record"), on ? "Recording" : "Not recording"),
      button(on ? "Stop recording" : "Start recording", () => post({action: on ? "trace-off" : "trace-on"}), {small: true, secondary: true, icon: on ? "debug-stop" : "record", disabled: !canWrite || view.busy,
        title: canWrite ? "" : "Changing recording needs full permission"}),
      button("Clear", () => post({action: "trace-clear"}), {small: true, secondary: true, icon: "clear-all", disabled: !canWrite || view.busy, title: "Clear the retained journal for every family"}),
      h("span.spacer"), filterBox("Filter calls", group && group.calls ? group.calls.length : 0, group && group.calls ? group.calls.filter(matches).length : 0),
      extra);
  }
  function exportBar(exportPath, exportResult) {
    return h("form.toolrow", {on: {submit: event => { event.preventDefault(); post({action: "export", fields: {exportPath: event.target.elements.exportPath.value}}); }}},
      h("label.field", {style: {flex: "1"}}, "Export to", h("input.mono", {name: "exportPath", value: exportPath, placeholder: "/absolute/path/on/vm-host/calls.json", style: {flex: "1"}})),
      button("Browse…", () => post({action: "browse-export", fields: {exportPath: document.querySelector("input[name=exportPath]").value}}), {small: true, secondary: true}),
      button("Export JSON", null, {submit: true, small: true, icon: "export"}),
      exportResult ? h("span.chip.ok", icon("check"), "Saved") : null);
  }

  // ------------------------------------------------------------------- WASI
  function wasiWindow() {
    const w = view.wasip1, groups = Object.entries(w.groups);
    const nav = h("nav.nav", {"aria-label": "WASI sections"}, h("div.nav-head", "Environment"),
      groups.filter(([key]) => !["policy", "calls"].includes(key)).map(([key, label]) => navItem(key, label, w.icons[key], "wasi-group", w.active)),
      h("div.nav-head", "Policy & activity"),
      groups.filter(([key]) => ["policy", "calls"].includes(key)).map(([key, label]) => navItem(key, label, w.icons[key], "wasi-group", w.active)));
    let content;
    const denied = blocked();
    if (denied) content = denied;
    else if (w.active === "policy") content = policyPane(w.policyRows, w.policy, "Policy rows describe what the runtime admitted at launch.");
    else if (w.active === "calls") content = w.group ? [recordingBar(w.group, "wasip1"), h("div.pane", callsTable(w.group, "wasip1"), payloadView(w.payload, w.payloadSelector)), exportBar(w.exportPath, w.exportResult)]
      : genericGroup(undefined);
    else content = [h("div.toolrow", h("span", h("b", w.groups[w.active]), " ", h("span.v-null", w.group ? plural(w.group.total || 0, "entry", "entries") : "")), h("span.spacer"),
      filterBox("Filter " + w.groups[w.active].toLowerCase(), w.group ? w.group.rows.length : 0, w.group ? w.group.rows.filter(matches).length : 0)),
      h("div.pane", wasiGroup(w)), pager(w.group, "group-previous", "group-next")];
    return [titlebar([], [
      h("label.field", {title: "Wasm module index"}, "Module", h("input.num", {value: w.module, inputmode: "numeric", "aria-label": "Module index",
        on: {change: event => post({action: "refresh", fields: {module: event.target.value}})}})),
      action("refresh", "Refresh", () => post({action: "refresh"}), {disabled: view.busy, key: "Ctrl+R", spin: view.busy}),
      action("split-horizontal", "Open another WASI window", () => post({action: "new-window"}))]),
      banners(), h("div.body", nav, h("div.content", content)), statusline()];
  }
  function navItem(key, label, iconName, actionName, activeKey, count) {
    return h("button.nav-item", {type: "button", "aria-current": String(key === activeKey), on: {click: () => { local.filter = ""; state.set("filter", ""); local.openDetail = null; post({action: actionName, group: key}); }}},
      icon(iconName || "circle-small"), h("span.label", label), count !== undefined ? h("span.count-badge" + (count ? "" : ".zero"), count) : null);
  }
  function wasiGroup(w) {
    const group = w.group;
    if (!group) return genericGroup(undefined);
    if (group.error) return h("div.banner.error", icon("error"), h("div.text", group.error));
    const rows = (group.rows || []).filter(matches);
    if (!group.rows.length) return emptyState(w.icons[w.active], "No " + w.groups[w.active].toLowerCase(), "The guest has none at this stop.");
    if (["args", "env"].includes(w.active)) {
      return grid([
        {title: "#", width: "48px", cls: "num", render: row => h("span.v-number", String(row.name).replace(/[\[\]]/g, ""))},
        ...(w.active === "env" ? [{title: "Name", cls: "nowrap", render: row => h("span.v-name", envParts(row.value)[0])}, {title: "Value", cls: "wrap", render: row => value(envParts(row.value)[1])}]
          : [{title: "Argument", cls: "wrap", render: row => value(unquote(row.value))}])
      ], rows, {key: row => row.name, actions: row => [action("copy", "Copy", () => copy(unquote(row.value)))]});
    }
    if(["fds","preopens"].includes(w.active))return grid([
      {title:"Descriptor",render:row=>h("span.v-number",row.name)},
      {title:"Kind",render:row=>value(row.storageKind || row.type)},
      {title:"Guest mount",cls:"wrap",render:row=>value(row.guestName || "—")},
      {title:"Current host path",cls:"wrap",render:row=>h("span",{title:"Sampled from the live native handle. Display only; never used to reopen or delete a host file."},value(row.currentPath ?? "Unavailable"))},
      {title:"Rights",cls:"wrap",render:row=>h("span",{title:"Base rights / inheriting rights"},value("0x"+BigInt(row.rightsBase||0).toString(16)+" / 0x"+BigInt(row.rightsInheriting||0).toString(16)))}
    ],rows,{key:row=>row.name});
    return genericGroup(group);
  }
  function unquote(text) { const t = String(text); return /^".*"$/.test(t) ? t.slice(1, -1) : t; }
  function envParts(text) { const t = unquote(text), at = t.indexOf("="); return at < 0 ? [t, ""] : [t.slice(0, at), t.slice(at + 1)]; }

  function policyPane(rows, info, note) {
    if (!info) return genericGroup(undefined);
    const groups = new Map();
    for (const row of rows.filter(matches)) {
      const key = String(row.key), group = key.includes(".") ? key.slice(0, key.indexOf(".")) : "general";
      if (!groups.has(group)) groups.set(group, []);
      groups.get(group).push(row);
    }
    const body = h("tbody");
    for (const [group, list] of groups) {
      const interfaceLabels = {
        uwvm_ros1_v1: "u2rosv1 › ros1 › Interfaces", uwvm_ros1_client_v1: "u2rosv1 › ros1 › Client interfaces",
        uwvm_ros2_v1: "u2rosv1 › ros2 › Interfaces", uwvm_ros2_client_v1: "u2rosv1 › ros2 › Client interfaces",
        uwvm_ros2_rmw_v1: "u2rosv1 › ros2 › RMW publisher interfaces", uwvm_ros2_rmw_sub_v1: "u2rosv1 › ros2 › RMW subscription interfaces",
        uwvm_ros2_rmw_srv_v1: "u2rosv1 › ros2 › RMW service interfaces", uwvm_portable_robot_v1: "u2rosv1 › Portable robot interfaces",
        uwvm_ros_v2: "u2rosv2 › Interfaces"
      };
      const label = interfaceLabels[group] || (group === "general" ? "Availability" : humanize(group));
      body.append(h("tr.group-row", h("td", {colspan: 3}, label, " ", h("span.v-null", "(" + list.length + ")"))));
      for (const row of list) {
        const key = "policy:" + row.id;
        const tr = h("tr.policy-row", {tabindex: "-1", on: {dblclick: () => toggleDetail(key), contextmenu: event => menu(event, [
          {label: "Copy key", icon: "copy", run: () => copy(row.key)},
          {label: "Copy value", icon: "copy", run: () => copy(typeof row.value === "object" ? row.value.text : row.value)},
          {label: "Copy row as JSON", icon: "json", run: () => copy(JSON.stringify(Object.fromEntries(row.fields.map(f => [f.key, f.value])), null, 2))}])}},
          h("td", h("div.policy-key", h("button.twistie", {type: "button", style: {border: "none", background: "transparent", padding: 0, cursor: "pointer", color: "inherit"},
            "aria-label": "Details", on: {click: () => toggleDetail(key)}}, icon(local.openDetail === key ? "chevron-down" : "chevron-right")),
            h("span.k", row.key.includes(".") ? row.key.slice(row.key.indexOf(".") + 1) : row.key))),
          h("td.wrap", value(row.value, "policy"), row.part ? h("div.v-null", row.part) : null),
          h("td.nowrap", row.meta ? h("span.v-null", row.meta) : null, (row.flags || []).filter(f => f.label === "redacted" && f.value === "true").map(() => h("span.chip.outline", icon("lock"), "redacted"))));
        body.append(tr);
        if (local.openDetail === key) body.append(h("tr.detail-row", h("td", {colspan: 3}, h("dl.detail-grid", row.fields.map(f => [h("dt", humanize(f.key)), h("dd", cellValue(f.value))])),
          row.exact ? h("div.v-null", {style: {marginTop: "4px"}}, "Exact bytes (base64): ", h("span.mono", row.exact)) : null)));
      }
    }
    const scoped = rows.some(row => row.meta);
    if (!scoped) for (const tr of body.querySelectorAll("tr")) { const cells = tr.querySelectorAll(":scope > td"); if (cells.length === 3) cells[2].remove(); else if (cells.length === 1 && cells[0].colSpan === 3) cells[0].colSpan = 2; }
    const table = h("table.grid", h("thead", h("tr", h("th", "Setting"), h("th", "Value"), scoped ? h("th", "Scope") : null)), body);
    listKeys(body, "tr.policy-row", null, tr => tr.dispatchEvent(new MouseEvent("dblclick")));
    return [h("div.toolrow", h("span", h("b", "Policy"), " ", h("span.v-null", plural(info.total || 0, "row"))), note ? h("span.v-null.wide-only", note) : null, h("span.spacer"),
      filterBox("Filter settings on this page", rows.length, rows.filter(matches).length)),
      h("div.pane", rows.length ? table : emptyState("law", "No policy rows", "")), pager(info, "policy-previous", "policy-next", "Rows " + (rows.length ? (Number(info.start || 0) + 1) : 0) + "–" + (Number(info.start || 0) + rows.length) + " of " + info.total)];
  }

  function wasithreadWindow() {
    const threads = view.wasithread, page = threads.view;
    const groups = {policy: "Authority & counts", members: "Module membership"};
    const nav = h("nav.nav", {"aria-label": "WASI Threads sections"},
      Object.entries(groups).map(([key, label]) => navItem(key, label, "list-tree", "thread-group", threads.active)));
    const content = blocked() || [
      h("div.toolrow", h("b", groups[threads.active]), h("span.spacer"),
        filterBox("Filter this page", page ? page.rows.length : 0, page ? page.rows.filter(matches).length : 0)),
      h("div.pane", genericGroup(page, threads.active === "members" ? "No explicit membership; modules use Global" : "No policy data")),
      pager(page, "group-previous", "group-next")];
    return [titlebar([], [action("refresh", "Refresh", () => post({action: "refresh"}), {disabled: view.busy, spin: view.busy}),
      action("split-horizontal", "Open another WASI Threads window", () => post({action: "new-window"}))]),
      banners(), h("div.body", nav, h("div.content", content)), statusline()];
  }

  // ------------------------------------------------------------- Robot scene
  function sceneWindow() {
    const scene = view.scene, page = scene.view;
    const nav = h("nav.nav", {"aria-label": "u2rosv2 robot scene sections"},
      Object.entries(scene.groups).map(([key, label]) => navItem(key, label, key === "authority" ? "key" : "database", "scene-group", scene.active)));
    const content = blocked() || (scene.unavailable ? emptyState("shield", "u2rosv2 is not built", scene.unavailable) : scene.active === "journal" && page ? [
      recordingBar(page, "rosv2"), h("div.toolrow", "Dropped ROSv2 completions: " + (page.journal?.["rosv2-dropped"] ?? "unavailable")),
      h("div.pane", callsTable(page, "rosv2")), pager(page, "group-previous", "group-next"),
      exportBar(scene.exportPath, scene.exportResult)] : [
      h("div.toolrow", h("span.chip.outline", "u2rosv2 · In development")),
      h("div.toolrow", h("b", scene.groups[scene.active]), h("span.spacer"),
        filterBox("Filter this page", page ? page.rows.length : 0, page ? page.rows.filter(matches).length : 0)),
      h("div.pane", genericGroup(page, "No " + scene.groups[scene.active].toLowerCase())),
      pager(page, "group-previous", "group-next")]);
    return [titlebar([], [action("refresh", "Refresh", () => post({action: "refresh"}), {disabled: view.busy, spin: view.busy}),
      action("split-horizontal", "Open another scene window", () => post({action: "new-window"}))]),
      banners(), h("div.body", scene.unavailable ? null : nav, h("div.content", content)), statusline()];
  }

  // -------------------------------------------------------------------- ROS
  function rosWindow() {
    const r = view.ros, groups = Object.entries(r.groups);
    const backendSeg = h("div.ros-backends", h("span.ros-api-label", "u2rosv1"), h("span.path-separator", "›"),
      h("div.seg", {role: "group", "aria-label": "u2rosv1 ROS backend"}, ["all", "ros1", "ros2"].map(key =>
        h("button", {type: "button", "aria-pressed": String(r.backend === key), disabled: key !== "all" && r.builds?.["u2rosv1-" + key] === false,
          title: key !== "all" && r.builds?.["u2rosv1-" + key] === false ? "u2rosv1 › " + key + " is not built" : "",
          on: {click: () => post({action: "refresh", fields: {backend: key}})}}, key === "all" ? "All backends" : key))));
    const active = r.active === "policy" ? "policy" : r.active;
    const nav = h("nav.nav", {"aria-label": "u2rosv1 sections"},
      h("div.nav-head", "Configuration"), navItem("policy", "Policy", "law", "ros-section", active),
      h("div.nav-head", "Runtime state"), groups.filter(([key]) => !["calls", "trace", "journal", "stats"].includes(key)).map(([key, label]) => navItem(key, label, rosIcon(key), "ros-group", active)),
      h("div.nav-head", "Activity"), groups.filter(([key]) => ["calls", "trace", "journal", "stats"].includes(key)).map(([key, label]) => navItem(key, label, rosIcon(key), "ros-group", active)));
    nav.querySelector("[aria-current]") && nav.querySelectorAll(".nav-item").forEach(item => {
      if (item.textContent === "Policy") item.onclick = () => { local.rosPolicy = true; state.set("rosPolicy", true); draw(); };
      else item.addEventListener("click", () => { local.rosPolicy = false; state.set("rosPolicy", false); });
    });
    if (state.get("rosPolicy")) nav.querySelectorAll(".nav-item").forEach(item => item.setAttribute("aria-current", String(item.textContent === "Policy")));
    let content;
    const denied = blocked();
    if (denied) content = denied;
    else if (r.unavailable) content = emptyState("radio-tower", "u2rosv1 is not built", r.unavailable);
    else if (state.get("rosPolicy")) {
      content = [h("form.toolrow", {on: {submit: event => { event.preventDefault(); const f = event.target.elements;
          post({action: "refresh", fields: {policyScope: f.scope.value, policyTarget: f.target.value, policyAlias: f.alias.value}}); }}},
        h("label.field", "Scope", h("select", {name: "scope"}, ["all", "global", "single", "group"].map(s => h("option", {value: s, selected: r.filters.scope === s || undefined}, s)))),
        h("label.field", "Target", h("input", {name: "target", value: r.filters.target, size: 14, placeholder: "any"})),
        h("label.field", "Alias", h("input", {name: "alias", value: r.filters.alias, size: 12, placeholder: "any"})),
        button("Apply", null, {submit: true, small: true, secondary: true})),
        ...[].concat(policyPane(r.policyRows, r.policy))];
    } else if (r.active === "journal") {
      content = r.backend === "all" ? emptyState("radio-tower", "u2rosv1 › Choose ros1 or ros2", "Select a backend above to inspect its recorded calls.") :
        r.view && !r.view.error ? [recordingBar(r.view, r.backend), h("div.pane", callsTable(r.view, r.backend), payloadView(r.payload, r.payloadSelector)), exportBar(r.exportPath, r.exportResult)] : genericGroup(r.view);
    } else {
      content = [h("div.toolrow", h("span", h("b", r.groups[r.active]), " ", h("span.v-null", r.view ? plural(r.view.total || 0, "record") : "")),
        r.view && r.view.gap ? h("span.chip.bad", "gap in records") : null, h("span.spacer"),
        filterBox("Filter " + r.groups[r.active].toLowerCase(), r.view ? r.view.rows.length : 0, r.view ? r.view.rows.filter(matches).length : 0)),
        h("div.pane", genericGroup(r.view, "No " + r.groups[r.active].toLowerCase())), pager(r.view, "group-previous", "group-next")];
    }
    return [titlebar([h("span.sep", "›"), h("span.crumb", r.backend === "all" ? "ros1 + ros2" : r.backend)], [
      action("refresh", "Refresh", () => post({action: "refresh"}), {disabled: view.busy, key: "Ctrl+R", spin: view.busy}),
      action("split-horizontal", "Open another u2rosv1 window", () => post({action: "new-window"}))]),
      banners(), h("div.toolrow", backendSeg), h("div.body", r.unavailable ? null : nav, h("div.content", content)), statusline()];
  }
  function rosIcon(key) {
    return {capabilities: "key", sessions: "plug", queues: "list-flat", services: "server", actions: "run-all", parameters: "settings-gear",
      tf: "arrow-swap", graph: "graph", stats: "pulse", calls: "pulse", trace: "list-tree", journal: "record"}[key] || "circle-small";
  }

  function preloadDlWindow() {
    const data = view.preloaddl, denied = blocked();
    const built = data?.builds?.preloaddl, rows = data?.libraries || [];
    const content = denied || (built === false ? emptyState("file-binary", "preloaddl is not built",
      "Native library preloading is disabled in this VM build. Debugger inspection does not enable it.") : !data ?
      emptyState("file-binary", "No launch inventory", "The VM has not supplied authenticated library metadata.") :
      h("div.diagnostics-layout", diagnosticCard("preloaddl", "Native preload inventory", "file-binary", "Read-only metadata from this process. Native instructions and addresses are never exposed.",
        diagnosticFacts([["Build support", "Built"], ["Loaded libraries", data.total]]),
        !rows.length ? h("p.diagnostic-note", "No native libraries were loaded at launch. preloaddl is currently not enabled for this program.") :
          rows.map(row => h("div.preload-library", h("div.diagnostic-subhead", h("h3", value(row.name)), h("span.chip.outline", "#" + row.index)),
            diagnosticFacts([["Exported Wasm functions", row.functions], ["Signature", row.signatureVerified ? "Verified at load" : "Verification not requested"]]),
            diagnosticFold("preloaddl/" + row.index, "File identity", h("div.mono", value(row.path))))))));
    return [titlebar([], [action("refresh", "Refresh", () => post({action: "refresh"}), {disabled: view.busy, spin: view.busy})]), banners(),
      h("div.pane", content), data && built ? h("div.pager", h("span", "Libraries " + (rows.length ? BigInt(data.first) + 1n : 0) + "–" + data.next + " of " + data.total), h("span.spacer"),
        button("Previous page", () => post({action: "group-previous"}), {small: true, secondary: true, disabled: !data.back || view.busy}),
        button("Next page", () => post({action: "group-next"}), {small: true, secondary: true, disabled: !data.more || view.busy})) : null, statusline()];
  }

  // ------------------------------------------------------------ breakpoints
  // Breakpoints are grouped by the level they were set at. Every level is
  // armed whatever the S/W/A step level is: the step level only chooses what
  // F10/F11 step over, never which breakpoints can stop the guest.
  const LEVELS = [["source", "Source", "file-code"], ["wasm", "Wasm", "symbol-method"], ["asm", "Assembly", "chip"]];
  function breakpointLocation(row) {
    if (row.level === "source" && row.source) return h("span", h("span.mono", (String(row.source.path || "").split(/[\\/]/).pop()) + ":" + row.source.line),
      h("span.v-null", "  func " + row.function + " @0x" + BigInt(row.offset).toString(16)));
    if (row.level === "asm") return h("span.mono", "func " + row.function + " · JIT +0x" + BigInt(row.offset).toString(16));
    return h("span.mono", "module " + row.module + " · func " + row.function + " · @0x" + BigInt(row.offset).toString(16));
  }
  function breakpointsWindow() {
    const b = view.breakpoints, canWrite = b.canWrite && !view.busy;
    const denied = blocked();
    const add = h("form.toolrow", {on: {submit: event => { event.preventDefault(); const f = event.target.elements;
        post({action: "breakpoint-create", fields: {module: f.module.value, function: f.function.value, offset: f.offset.value, temporary: f.temporary.checked}}); }}},
      h("span", h("b", "Add Wasm breakpoint")),
      h("label.field", "Module", h("input.num", {name: "module", value: "0", disabled: !canWrite || undefined})),
      h("label.field", "Function", h("input.num", {name: "function", value: "0", disabled: !canWrite || undefined})),
      h("label.field", "Offset", h("input.mono", {name: "offset", value: "0x0", size: 8, disabled: !canWrite || undefined})),
      h("label.field", h("input", {type: "checkbox", name: "temporary", disabled: !canWrite || undefined}), "Remove after first hit"),
      button("Add", null, {submit: true, small: true, icon: "add", disabled: !canWrite}));
    const collapsed = state.get("bpCollapsed") || {};
    const body = h("tbody");
    const all = b.rows.filter(matches);
    for (const [level, title, iconName] of LEVELS) {
      const rows = all.filter(row => (row.level || (row.kind === "asm" ? "asm" : "wasm")) === level);
      const open = !collapsed[level];
      body.append(h("tr.group-row.level-row", {tabindex: "-1", "aria-expanded": String(open), on: {click: () => { collapsed[level] = open; state.set("bpCollapsed", collapsed); draw(); }}},
        h("td", {colspan: 8}, icon(open ? "chevron-down" : "chevron-right"), " ", icon(iconName), " ", title, " ", h("span.count-badge" + (rows.length ? "" : ".zero"), rows.length))));
      if (!open) continue;
      if (!rows.length) { body.append(h("tr", h("td.inline-empty", {colspan: 8}, level === "asm" ? "None. Set them in the Disassembly view gutter at an assembly stop." :
        level === "source" ? "None. Click the gutter of a source file, or use the console (bs)." : "None. Click the Wasm instruction view gutter, use the console (bw) or add one above."))); continue; }
      for (const row of rows) {
        const key = "bp:" + row.id, detailOpen = local.openDetail === key;
        const tr = h("tr" + (row.enabled ? "" : ".dim"), {tabindex: "-1", dataset: {index: String(row.id), key}, "aria-expanded": String(detailOpen),
          on: {dblclick: () => toggleDetail(key), contextmenu: event => menu(event, [
            {label: row.enabled ? "Disable" : "Enable", icon: row.enabled ? "circle-slash" : "pass", disabled: !canWrite, run: () => post({action: "breakpoint-toggle", id: row.id, nextState: row.enabled ? "disable" : "enable"})},
            {label: "Edit condition…", icon: "edit", disabled: !canWrite, run: () => toggleDetail(key)},
            {label: "Show callbacks", icon: "list-tree", run: () => { toggleDetail(key); post({action: "breakpoint-callbacks", id: row.id}); }},
            "-", {label: "Remove", icon: "trash", disabled: !canWrite, run: () => post({action: "breakpoint-delete", id: row.id})}])}},
          h("td.twist", h("button.twistie", {type: "button", "aria-label": detailOpen ? "Hide details" : "Show details", on: {click: event => { event.stopPropagation(); toggleDetail(key); }}}, icon(detailOpen ? "chevron-down" : "chevron-right"))),
          h("td", {style: {width: "24px"}}, h("input", {type: "checkbox", checked: row.enabled || undefined, disabled: !canWrite || undefined, "aria-label": (row.enabled ? "Disable" : "Enable") + " breakpoint " + row.id,
            on: {click: event => { event.stopPropagation(); post({action: "breakpoint-toggle", id: row.id, nextState: row.enabled ? "disable" : "enable"}); }}})),
          h("td.num", h("span.v-number", row.id)),
          h("td.nowrap", breakpointLocation(row), row.temporary ? h("span.chip.outline", {style: {marginLeft: "6px"}}, "once") : null),
          h("td.wrap.code", row.condition ? h("span.v-string", row.condition) : h("span.v-null", "always")),
          h("td.num", h("span.v-number", row.hits)),
          h("td.num", row.ignore === "0" ? h("span.v-null", "0") : h("span.v-number", row.ignore)),
          h("td.actions-cell", h("span.row-actions", action("go-to-file", "Go to breakpoint", () => post({action: "breakpoint-goto", id: row.id}), {disabled: view.retired || view.busy}), action("trash", "Remove breakpoint", () => post({action: "breakpoint-delete", id: row.id}), {disabled: !canWrite, danger: true}))));
        body.append(tr);
        if (detailOpen) body.append(h("tr.detail-row", h("td", {colspan: 8}, breakpointDetail(row, canWrite))));
      }
    }
    const table = h("table.grid", {role: "treegrid"}, h("thead", h("tr", h("th"), h("th"), h("th", "#"), h("th", "Location"), h("th", "Condition"), h("th", "Hits"), h("th", "Skip"), h("th"))), body);
    listKeys(body, "tr[tabindex]", null, tr => tr.dispatchEvent(new MouseEvent(tr.classList.contains("level-row") ? "click" : "dblclick")));
    return [titlebar([], [action("refresh", "Refresh", () => post({action: "refresh"}), {disabled: view.busy, key: "Ctrl+R"})]),
      banners(), denied ? denied : [add, h("div.toolrow", h("span", h("b", plural(b.rows.length, "breakpoint")), " ", h("span.v-null", "all levels stop the guest, whatever the S/W/A step level")), h("span.spacer"),
        filterBox("Filter breakpoints", b.rows.length, all.length)), h("div.pane", table)], statusline()];
  }
  function breakpointDetail(row, canWrite) {
    const condition = h("form.row", {style: {display: "flex", gap: "6px", marginBottom: "8px"}, on: {submit: event => { event.preventDefault(); post({action: "breakpoint-condition", id: row.id, fields: {value: event.target.elements.value.value}}); }}},
      h("label.field", {style: {flex: "1"}}, "Condition", h("input.mono", {name: "value", value: row.condition, placeholder: "always stop", style: {flex: "1"}, maxlength: 256, disabled: !canWrite || undefined})),
      button("Set", null, {submit: true, small: true, secondary: true, disabled: !canWrite}));
    const ignore = h("form.row", {style: {display: "flex", gap: "6px", marginBottom: "8px"}, on: {submit: event => { event.preventDefault(); post({action: "breakpoint-ignore", id: row.id, fields: {value: event.target.elements.value.value}}); }}},
      h("label.field", "Skip next", h("input", {name: "value", value: row.ignore, size: 6, inputmode: "numeric", disabled: !canWrite || undefined}), "hits"),
      button("Set", null, {submit: true, small: true, secondary: true, disabled: !canWrite}));
    const callbacks = row.callbacks;
    const cb = h("div", h("div", {style: {display: "flex", alignItems: "center", gap: "8px", margin: "6px 0"}}, h("b", "Callback actions"),
      button(callbacks ? "Reload" : "Show", () => post({action: "breakpoint-callbacks", id: row.id}), {small: true, secondary: true})),
      callbacks ? (callbacks.rows && callbacks.rows.length ? h("ol.mono", {start: Number(callbacks.start) + 1, style: {margin: "0 0 6px 18px", padding: 0}}, callbacks.rows.map(item => h("li", item.command))) : h("div.v-null", "No callback actions.")) : null,
      callbacks && callbacks.more ? button("Next page", () => post({action: "breakpoint-callback-next", id: row.id}), {small: true, secondary: true}) : null,
      h("form", {style: {display: "grid", gap: "6px", marginTop: "6px"}, on: {submit: event => { event.preventDefault(); const f = event.target.elements; post({action: "breakpoint-set-callbacks", id: row.id, fields: {value: f.value.value, mode: f.mode.value}}); }}},
        h("textarea", {name: "value", rows: 2, placeholder: "echo reached; continue", disabled: !canWrite || undefined, "aria-label": "Callback action list"}),
        h("div", {style: {display: "flex", gap: "6px", alignItems: "center"}},
          h("select", {name: "mode", disabled: !canWrite || undefined}, h("option", {value: "replace"}, "Replace actions"), h("option", {value: "append"}, "Append actions"), h("option", {value: "clear"}, "Clear actions")),
          button("Apply", null, {submit: true, small: true, secondary: true, disabled: !canWrite}),
          h("span.v-null", "Semicolon-separated debugger commands. No shell; the runtime validates every packet."))));
    return h("div", condition, ignore, cb);
  }

  // Independent workspace watch list: native VS Code WATCH is untouched.
  function watchWindow() {
    const w=view.watch, denied=blocked(), collapsed=state.get("watchCollapsed")||{}, body=h("tbody");
    const edit=local.watchEdit, existing=edit&&w.rows.find(row=>row.id===edit);
    const form=h("form.toolrow",{on:{submit:event=>{event.preventDefault();const f=event.target.elements;
      post({action:existing?"watch-edit":"watch-add",id:existing?.id,fields:{level:f.level.value,expression:f.expression.value}});local.watchEdit=undefined;}}},
      h("label.field","Level",h("select",{name:"level",value:existing?.level||"source"},[...LEVELS].map(([level,title])=>h("option",{value:level==="asm"?"native":level,selected:(existing?.level||"source")===(level==="asm"?"native":level)||undefined},title)))),
      h("label.field",{style:{flex:"1"}},"Expression",h("input.mono",{name:"expression",value:existing?.expression||"",placeholder:"Source: counter · Wasm: local[0] · Assembly: $pc",style:{flex:"1"},required:true})),
      button(existing?"Save":"Add",null,{submit:true,small:true,icon:existing?"save":"add"}),
      existing?button("Cancel",()=>{local.watchEdit=undefined;draw();},{small:true,secondary:true}):null);
    for(const [kind,title,glyph] of LEVELS){
      const level=kind==="asm"?"native":kind,rows=w.rows.filter(row=>row.level===level),open=!collapsed[level];
      body.append(h("tr.group-row",h("td",{colspan:5},h("button.twistie",{"aria-label":(open?"Collapse ":"Expand ")+title+" watches",on:{click:()=>{collapsed[level]=open;state.set("watchCollapsed",collapsed);draw();}}},icon(open?"chevron-down":"chevron-right")),icon(glyph)," ",title," ",h("span.count-badge",rows.length))));
      if(!open)continue;
      if(!rows.length)body.append(h("tr",h("td.inline-empty",{colspan:5},"No "+title.toLowerCase()+" watches.")));
      for(const row of rows){
        const result=w.values[row.id]||{},children=w.children[row.id],key="watch:"+row.id,expanded=local.openDetail===key;
        body.append(h("tr",{dataset:{watchId:row.id}},
          h("td.twist",result.variablesReference?action("chevron-"+(expanded?"down":"right"),"Expand watch value",()=>{toggleDetail(key);if(!expanded)post({action:"watch-children",id:row.id,start:0});},{disabled:view.retired||view.busy}):null),
          h("td.code",row.expression),h("td.v-type",result.type||""),
          h("td.code",result.error?h("span.v-null",{title:result.error},result.error):result.result===undefined?h("span.v-null","Not evaluated"):value(result.result)),
          h("td.actions-cell",h("span.row-actions",action("go-to-file","Go to evaluation location",()=>post({action:"watch-goto",id:row.id}),{disabled:view.retired||view.busy||!!result.error||result.result===undefined}),
            action("edit","Edit watch",()=>{local.watchEdit=row.id;draw();}),action("trash","Remove watch",()=>post({action:"watch-remove",id:row.id}),{danger:true})))));
        if(expanded)body.append(h("tr.detail-row",h("td",{colspan:5},children?
          [h("table.grid",h("tbody",children.rows.map(child=>h("tr",h("td.code",child.name),h("td.v-type",child.type||""),h("td.code",value(child.value)))))),
           button("Previous page",()=>post({action:"watch-children",id:row.id,start:Math.max(0,children.start-32)}),{small:true,secondary:true,disabled:children.start===0}),
           button("Next page",()=>post({action:"watch-children",id:row.id,start:children.start+32}),{small:true,secondary:true,disabled:children.rows.length<32})]:h("span.v-null","Loading members…"))));
      }
    }
    return [titlebar([],[action("refresh","Refresh watches",()=>post({action:"refresh"}),{disabled:view.busy,key:"Ctrl+R"})]),banners(),
      denied||[form,h("div.toolrow.v-null","Independent list · Native WATCH is unchanged · Go to navigates the evaluation frame; it does not change execution."),
      h("div.pane",h("table.grid",h("thead",h("tr",["","Expression","Type","Value",""].map(title=>h("th",title)))),body))],statusline()];
  }

  // -------------------------------------------------------------- registers
  function registersWindow() {
    const r = view.registers, denied = blocked();
    const rows = r.rows.filter(matches);
    const body = denied || (r.available ? grid([
      {title: "Register", width: "120px", cls: "nowrap code", render: row => h("span.v-name", row.name)},
      {title: "Value", cls: "nowrap code", render: row => value(row.value)},
      {title: "Type", cls: "nowrap", render: row => row.type ? h("span.v-type", row.type) : null},
      {title: "Availability", render: row => h("span", row.availability || "")}
    ], rows, {key: row => row.name, actions: row => [action("copy", "Copy value", () => copy(row.value))]}) :
      emptyState("symbol-numeric", "No native stop", "Registers require a captured JIT instruction stop. Full permission captures it when this window opens, without executing the instruction. Read-only mode can inspect an existing native capture."));
    return [titlebar(r.frame ? [h("span.sep", "›"), h("span.crumb", r.frame)] : [], [action("refresh", "Refresh", () => post({action: "refresh"}), {disabled: view.busy, key: "Ctrl+R", spin: view.busy})]),
      banners(), r.available ? h("div.toolrow", h("span", h("b", plural(r.rows.length, "register"))), h("span.spacer"), filterBox("Filter registers", r.rows.length, rows.length)) : null,
      h("div.pane", body), statusline()];
  }

  // ------------------------------------------------------------ diagnostics
  // Keep unfinished form edits and disclosure panels local to this inspector
  // and session. Refreshing runtime data must not erase a path being entered.
  const diagnosticUI = {session: undefined, fields: {}, remote: {}, open: new Set(), scrolls: {}, section: "overview"};
  const diagnosticSections = [["overview", "Overview", "pulse"], ["capture", "Save & record", "save"],
    ["history", "History", "inbox"], ["restore", "Restore & replay", "history"]];
  function diagnosticDraft(d) {
    if (diagnosticUI.session !== view.session.id) {
      diagnosticUI.session = view.session.id;
      diagnosticUI.fields = {historyPath: "", historyEvent: "0"};
      diagnosticUI.remote = {}; diagnosticUI.open.clear(); diagnosticUI.scrolls = {};
      diagnosticUI.section = d.initialSection || "overview";
    }
    for (const key of ["dumpPath", "restorePath", "restoreEnvironment", "restoreBindings", "replayCursor"]) {
      const next = d[key] ?? "";
      if (!Object.hasOwn(diagnosticUI.fields, key) || diagnosticUI.remote[key] !== next) diagnosticUI.fields[key] = next;
      diagnosticUI.remote[key] = next;
    }
    if (d.threaded) diagnosticUI.fields.restoreEnvironment = "replay";
    return diagnosticUI.fields;
  }
  function diagnosticTab(key, focus = true) {
    const pane = document.querySelector(".diagnostics-pane");
    if (pane) diagnosticUI.scrolls[diagnosticUI.section] = pane.scrollTop;
    diagnosticUI.section = key; draw();
    const next = document.querySelector(".diagnostics-pane");
    if (next) next.scrollTop = diagnosticUI.scrolls[key] || 0;
    if (focus) document.getElementById("diagnostic-tab-" + key)?.focus({preventScroll: true});
  }
  function diagnosticCard(key, title, iconName, description, ...children) {
    return h("section.diagnostic-card", {"aria-labelledby": "diagnostic-heading-" + key},
      h("header.diagnostic-card-head", icon(iconName), h("div", h("h2", {id: "diagnostic-heading-" + key}, title),
        description ? h("p", description) : null)), h("div.diagnostic-card-body", children));
  }
  function diagnosticFold(key, title, content, count) {
    const node = h("details.diagnostic-fold", {open: diagnosticUI.open.has(key) || undefined,
      on: {toggle: event => { if (event.target.open) diagnosticUI.open.add(key); else diagnosticUI.open.delete(key); }}},
      h("summary", h("span", title), count !== undefined ? h("span.count-badge", String(count)) : null),
      h("div.diagnostic-fold-body", content));
    return node;
  }
  function diagnosticFacts(entries, cls = "diagnostic-facts") {
    return h("dl." + cls, entries.filter(([, v]) => v !== undefined && v !== null).map(([key, raw]) =>
      h("div", h("dt", key), h("dd", value(raw)))));
  }
  function diagnosticInput(name, label, options = {}) {
    const fields = diagnosticUI.fields;
    return h("label.diagnostic-field", {class: options.wide ? "wide" : ""}, h("span", label),
      h(options.multiline ? "textarea.mono" : "input.mono", {name, value: options.multiline ? undefined : fields[name],
        rows: options.multiline ? 3 : undefined, placeholder: options.placeholder, inputmode: options.numeric ? "numeric" : undefined,
        "aria-label": options.ariaLabel || label, spellcheck: false, disabled: options.disabled || undefined,
        on: {input: event => { fields[name] = event.target.value; }}}, options.multiline ? fields[name] : null),
      options.hint ? h("small", options.hint) : null);
  }
  function historicalEnvironment(reply) {
    const env = reply.environment, selected = (reply.environments || []).find(row =>
      row.kind === (env.backend === 1 ? "ros1" : env.backend === 2 ? "ros2" : "wasip1") && String(row.id) === String(env.module ?? "0"));
    const identity = "$historical-environment/" + reply.recording + "/" + (env.backend || "wasip1") + "/" + (selected?.id ?? env.module ?? "0");
    if (Array.isArray(env.groups)) {
      const backend = env.backend === 1 ? "ros1" : env.backend === 2 ? "ros2" : "Unknown backend";
      return h("div.historical-environment", h("div.diagnostic-subhead", h("h3", "u2rosv1", h("span.path-separator", "›"), backend),
        h("span.diagnostic-muted", plural(env.groups.length, "policy group"))),
        h("p.diagnostic-note", "Recorded local state only. Viewing grants no restore authority and reconnects no external resources."),
        env.groups.map((group, index) => {
          const scope = {0: "Global", 1: "Single", 2: "Group"}[group.scope] || "Scope " + group.scope;
          const name = group.name || (scope === "Global" ? "Unassigned modules" : "Unnamed");
          const key = identity + "/group/" + index;
          return h("div.historical-group", diagnosticFold(key, scope + " · " + name,
            [diagnosticFacts([["Enabled", group.enabled], ["Registered", group.registered], ["Connected", group.connected],
              ["Session present", group.sessionPresent], ["Session label", group.sessionLabel], ["Timeout", group.timeout]]),
             ["members", "connection", "interfaces", "capabilities", "handles", "observations", "availabilityHistory"].filter(field => group[field] !== undefined).map(field =>
               diagnosticFold(key + "/" + field, humanize(field), tree(group[field], key + "/" + field, {expandDepth: 0}), Array.isArray(group[field]) ? group[field].length : undefined)),
             diagnosticFold(key + "/all", "All recorded group fields", tree(group, key + "/all", {expandDepth: 0}))],
            (group.members || []).length));
        }), diagnosticFold(identity + "/metadata", "Archive metadata", tree(Object.fromEntries(Object.entries(env).filter(([key]) => key !== "groups")), identity + "/metadata", {expandDepth: 0})));
    }
    return h("div.historical-environment", h("div.diagnostic-subhead", h("h3", "WASIp1", h("span.path-separator", "›"), "Module " + env.module)),
      diagnosticFacts([["Arguments", env.argumentCount], ["Environment variables", env.environmentCount], ["Descriptors", env.descriptorCount], ["Resources", env.resources?.length]]),
      ["members", "arguments", "variables", "resources", "bindings", "reserved", "closed"].filter(key => env[key] !== undefined).map(key =>
        diagnosticFold(identity + "/" + key, {members: "Module members", variables: "Environment variables", bindings: "Descriptor bindings", reserved: "Reserved descriptors", closed: "Closed descriptors"}[key] || humanize(key),
          tree(env[key], identity + "/" + key, {expandDepth: 0}), Array.isArray(env[key]) ? env[key].length : undefined)),
      diagnosticFold(identity + "/all", "All recorded environment fields", tree(env, identity + "/all", {expandDepth: 0})));
  }
  function diagnosticsWindow() {
    const d = view.diagnostics, details = d.details || {};
    const fields = diagnosticDraft(d), section = diagnosticUI.section;
    const facts = [["State", details.State], ["Stop", d.stopId && d.stopId !== "0" ? d.stopId : "—"], ["Reason", details["Stop reason"]], ["Permission", d.permission]];
    const participants = details.Participants || [];
    const extra = Object.fromEntries(Object.entries(details).filter(([key]) => !["State", "Permissions", "Stop reason", "Participants"].includes(key)));
    let content;
    if (section === "overview") content = h("div.diagnostic-columns",
      diagnosticCard("participants", "Participants", "list-tree", "Threads visible at the current stop.", participants.length ?
        h("div.diagnostic-table", h("table.grid", h("thead", h("tr", ["Thread", "Module", "Function", "Offset", "Generation", "Stop"].map(t => h("th", t)))),
          h("tbody", participants.map(p => h("tr", h("td.num", p.id), h("td.num", p.module), h("td.num", p.function), h("td.num.code", "0x" + BigInt(p.offset).toString(16)), h("td.num", p.generation), h("td.num", p.stop_id)))))) :
        h("p.diagnostic-note", "No Wasm instruction participants are reported at this point.")),
      diagnosticCard("runtime", "Runtime details", "output", "Status and additional fields reported by the VM.",
        d.text ? h("pre.diagnostic-output.mono", d.text) : h("p.diagnostic-note", "No status text reported."),
        Object.keys(extra).length ? diagnosticFold("runtime-fields", "Additional runtime fields", tree(extra, "$diag", {expandDepth: 0})) : null),
      d.hostInterfaces ? diagnosticCard("host-build", "Host interface build", "plug", "Build availability is separate from runtime authorization.",
        diagnosticFacts(Object.entries(d.hostInterfaces.builds).map(([key, built]) => [{"u2rosv1-ros1": "u2rosv1 › ros1", "u2rosv1-ros2": "u2rosv1 › ros2", u2rosv2: "u2rosv2 · In development", preloaddl: "preloaddl"}[key], built ? "Built · authorization required" : "Not built"]))) : null);
    if (section === "capture") content = h("div.diagnostic-columns",
      diagnosticCard("dump", "Save Wasm dump", "save", "Save the current stop to a file on the VM host.",
        d.dumpPolicy ? h("p.diagnostic-policy", icon("shield"), d.dumpPolicy.effective === "none" ? "Disabled by launch policy" :
          (d.dumpPolicy.kind === "force" ? "Forced disclosure: " : "Maximum disclosure: ") + d.dumpPolicy.configured) : null,
        h("form.diagnostic-form", {on: {submit: event => { event.preventDefault(); post({action: "dump", fields: {dumpPath: fields.dumpPath, disclosure: event.target.elements.disclosure.value}}); }}},
          h("label.diagnostic-field", h("span", "Disclosure"),
          h("select", {name: "disclosure", "aria-label": "Dump disclosure", disabled: !d.dumpPolicy?.allowed?.length || d.dumpPolicy.kind === "force",
            on: {change: event => post({action: "dump-disclosure", fields: {disclosure: event.target.value,
              dumpPath: fields.dumpPath}})}},
            (d.dumpPolicy?.allowed || []).map(level => h("option", {value: level, selected: level === d.disclosure}, level === "full" ? "Full" : "Diagnostic")))),
          diagnosticInput("dumpPath", "Output file on VM host", {wide: true, placeholder: "/absolute/path/fault.uwsnap", ariaLabel: "Dump file path on the VM host"}),
          h("div.diagnostic-actions", button("Browse…", () => post({action: "browse-dump", fields: {dumpPath: fields.dumpPath}}), {secondary: true, disabled: d.picking || view.busy}),
            button("Save dump", null, {submit: true, icon: "save", disabled: !d.canDump || view.busy}))),
        h("p.diagnostic-note", "Diagnostic: original Wasm and basic fault locations. Full: sensitive runtime state. Disclosure is fixed by the launch policy."),
        !d.stopId || d.stopId === "0" ? h("p.diagnostic-note", "Pause the guest before saving a dump.") : null,
        d.dumpReply ? diagnosticFold("dump-result", "Saved dump details", tree(d.dumpReply, "$dump", {expandDepth: 0})) : null),
      diagnosticCard("recording", "Execution recording", "record", d.threaded ? "Save and replay a dynamic WASI Threads cohort." : "Record between paused instruction boundaries.",
        h("p.diagnostic-note", "Replay consumes recorded WASI / u2rosv1 effects without repeating external operations."),
        h("div.diagnostic-actions", button("Save recording", () => post({action: "record-snapshot", fields: {action: "save", path: fields.dumpPath}}), {icon: "save", disabled: !d.canRecord || view.busy}),
          !d.threaded ? button("Start recording", () => post({action: "record-snapshot", fields: {action: "start"}}), {secondary: true, disabled: !d.canRecord || view.busy}) : null),
        h("div.diagnostic-actions.secondary-actions", button("Capture environment checkpoint", () => post({action: "record-snapshot", fields: {action: "mark"}}), {secondary: true, disabled: !d.canRecord || view.busy}),
          button(d.threaded ? "Launch recording status" : "Recording status", () => post({action: "record-snapshot", fields: {action: "status"}}), {secondary: true, disabled: d.permission !== "full" || view.busy})),
        h("p.diagnostic-note", "Uses the output file in Save Wasm dump. Requires full debugger permission and full dump disclosure."),
        d.recordingReply ? diagnosticFold("recording-result", "Recording details", tree(d.recordingReply, "$recording", {expandDepth: 0})) : null));
    if (section === "history") {
      const reply = d.canInspectHistory ? d.historyReply : undefined;
      const environments = reply?.environments || [], wasi = environments.filter(row => row.kind === "wasip1"), ros = environments.filter(row => ["ros1", "ros2"].includes(row.kind));
      const environmentButton = row => button((row.kind === "wasip1" ? "Module " : row.kind + " · Environment ") + row.id,
        () => post({action: "snapshot-history", fields: {kind: row.kind, id: row.id}}), {secondary: true, disabled: !d.canInspectHistory || view.busy});
      content = h("div.diagnostic-stack", diagnosticCard("history", "Historical environments", "inbox", "Inspect recorded WASI and u2rosv1 state without restoring or running the program.",
        h("form.diagnostic-form.history-form", {on: {submit: event => { event.preventDefault(); post({action: "snapshot-history", fields: {path: fields.historyPath, event: fields.historyEvent || "0"}}); }}},
          diagnosticInput("historyPath", "Saved history on VM host", {wide: true, placeholder: "/absolute/path/state.uwsnap", ariaLabel: "Historical snapshot file"}),
          diagnosticInput("historyEvent", "Environment sample / event", {numeric: true, hint: "0 selects the origin.", ariaLabel: "Environment event; zero selects origin"}),
          h("div.diagnostic-actions", button("Open saved history", null, {submit: true, icon: "folder-opened", disabled: !d.canInspectHistory || view.busy}),
            button("List historical environments", () => post({action: "snapshot-history"}), {secondary: true, disabled: !d.canInspectHistory || view.busy}))),
        !d.canInspectHistory ? h("p.diagnostic-note", "Historical environments require full or read-only inspection with full dump disclosure.") : null,
        reply ? [diagnosticFacts([["Position", reply.position], ["Event", reply.event], ["Instruction", reply.instruction], ["Journal cursor", reply.cursor], ["Environment samples", reply.sampleCount]]),
          h("div.history-families", wasi.length ? h("div.history-family", h("h3", icon("terminal"), "WASIp1"), h("div.diagnostic-actions", wasi.map(environmentButton))) : null,
            ros.length ? h("div.history-family", h("h3", icon("radio-tower"), "u2rosv1"), h("div.diagnostic-actions", ros.map(environmentButton))) : null),
          !environments.length ? h("p.diagnostic-note", "No historical environments at this selection.") : null,
          diagnosticFold("history-identity", "Recording identity & sample range", diagnosticFacts([["Recording", reply.recording],
            ["Sample range", reply.threaded && reply.sampleCount && BigInt(reply.sampleCount) > 0n ? "0 – " + (BigInt(reply.sampleCount) - 1n) : undefined]]))] : null),
        reply?.environment ? diagnosticCard("environment", "Recorded environment", "database", "Values at the selected observation. Later external changes are not inferred.", historicalEnvironment(reply)) : null);
    }
    if (section === "restore") content = h("div.diagnostic-columns",
      diagnosticCard("restore", "Restore unified snapshot", "history", "Restore a saved continuation, then pause before execution.",
        h("form.diagnostic-form", {on: {submit: event => { event.preventDefault(); post({action: "restore-snapshot", fields: {restorePath: fields.restorePath, restoreEnvironment: fields.restoreEnvironment, restoreBindings: fields.restoreBindings || "", replayCursor: fields.replayCursor || "0"}}); }}},
          diagnosticInput("restorePath", "Snapshot on VM host", {wide: true, placeholder: "/absolute/path/state.uwsnap", ariaLabel: "Snapshot to restore"}),
          h("div.diagnostic-actions", button("Browse…", () => post({action: "browse-restore", fields: {restorePath: fields.restorePath}}), {secondary: true, disabled: view.busy || d.picking})),
          h("label.diagnostic-field.wide", h("span", "WASIp1 / u2rosv1 environment"), h("select", {name: "restoreEnvironment", on: {change: event => { fields.restoreEnvironment = event.target.value; }}},
            !d.threaded ? h("option", {value: "current", selected: fields.restoreEnvironment === "current"}, "Retain current environment") : null,
            !d.threaded ? h("option", {value: "recorded", selected: fields.restoreEnvironment === "recorded"}, "Restore recorded local environment") : null,
            h("option", {value: "replay", selected: d.threaded || fields.restoreEnvironment === "replay"}, "Replay recorded execution (no external I/O)"))),
          d.threaded ? diagnosticInput("replayCursor", "Replay event cursor", {numeric: true, ariaLabel: "Replay event cursor; zero uses saved endpoint", hint: "0 uses the saved endpoint; a nonzero cursor reconstructs that prefix."}) :
            diagnosticInput("restoreBindings", "Recorded WASIp1 resource bindings", {wide: true, multiline: true, placeholder: "0:0:1", hint: "module:resource:descriptor · one per line. Current environment mode ignores these bindings."}),
          h("div.diagnostic-actions", button("Restore and pause", null, {submit: true, icon: "history", disabled: !d.canRestore || view.busy || d.restorePending}),
            d.restorePending ? button("Continue pending restore", () => post({action: "continue-restore"}), {disabled: view.busy || d.permission !== "full"}) : null)),
        d.permission !== "full" ? h("p.diagnostic-note", "Full debugger permission is required to change execution.") : null,
        d.restoreReply ? diagnosticFold("restore-result", "Restoration details", tree(d.restoreReply, "$restore", {expandDepth: 0})) : null),
      diagnosticCard("restore-boundary", "Recovery boundaries", "shield", "External effects are never rewound.",
        h("ul.diagnostic-boundaries", h("li", "Replay does not resend recorded file, socket or robot operations."),
          h("li", "Unknown native results can continue only in the original retained process."),
          h("li", "Recorded u2rosv1 state requires validated live session owners and permissions; numeric native handle rebinding is rejected."))));
    const tabs = h("div.diagnostic-tabs", {role: "tablist", "aria-label": "Diagnostics sections", on: {keydown: event => {
      const at = diagnosticSections.findIndex(([key]) => key === section);
      const next = event.key === "ArrowRight" ? (at + 1) % diagnosticSections.length : event.key === "ArrowLeft" ? (at + diagnosticSections.length - 1) % diagnosticSections.length : event.key === "Home" ? 0 : event.key === "End" ? diagnosticSections.length - 1 : -1;
      if (next >= 0) { event.preventDefault(); diagnosticTab(diagnosticSections[next][0]); }
    }}}, diagnosticSections.map(([key, label, iconName]) => h("button.diagnostic-tab", {id: "diagnostic-tab-" + key, type: "button", role: "tab",
      "aria-selected": String(section === key), "aria-controls": "diagnostic-panel", tabindex: section === key ? 0 : -1, on: {click: () => diagnosticTab(key)}}, icon(iconName), label)));
    return [titlebar([], [action("refresh", "Refresh", () => post({action: "refresh"}), {disabled: view.busy, key: "Ctrl+R", spin: view.busy})]), banners(),
      tabs, h("div.pane.diagnostics-pane", {id: "diagnostic-panel", role: "tabpanel", "aria-labelledby": "diagnostic-tab-" + section},
        h("div.diagnostics-layout", diagnosticFacts(facts, "diagnostic-overview"), content)), statusline()];
  }

  // ------------------------------------------------------------------ draw
  function draw(keepFocus) {
    if (view.kind === "memory" && mem.hosts && mem.scroller && mem.scroller.isConnected && view.memory && view.memory.info && memoryKey() === mem.key && !blocked()) {
      updateMemoryChrome();
      const tags = document.getElementById("memtag"); if (tags) tags.checked = !!view.memory.showTags;
      return;
    }
    const focused = document.activeElement, focusName = focused && (focused.name || focused.id), selStart = focused && focused.selectionStart, selEnd = focused && focused.selectionEnd;
    const scrolls = [...document.querySelectorAll(".pane,.table-wrap")].map(node => [node.scrollTop, node.scrollLeft]);
    if (view.kind === "memory" && mem.scroller && mem.scroller.isConnected) mem.initialTop = topAddress();
    cancelEdit();
    let parts;
    try {
      parts = view.kind === "memory" ? memoryWindow() : view.kind === "wasip1" ? wasiWindow() : view.kind === "ros" ? rosWindow() : view.kind === "scene" ? sceneWindow() : view.kind === "wasithread" ? wasithreadWindow() :
        view.kind === "preloaddl" ? preloadDlWindow() : view.kind === "breakpoints" ? breakpointsWindow() : view.kind === "watch" ? watchWindow() : view.kind === "registers" ? registersWindow() : diagnosticsWindow();
    } catch (error) {
      parts = [h("div.banner.error", icon("error"), h("div.text", "Display error: " + (error && error.message || error)))];
    }
    app.replaceChildren(...[].concat(parts).flat(Infinity).filter(Boolean));
    document.body.dataset.kind = view.kind; document.body.dataset.busy = String(!!view.busy);
    [...document.querySelectorAll(".pane,.table-wrap")].forEach((node, index) => { if (scrolls[index]) { node.scrollTop = scrolls[index][0]; node.scrollLeft = scrolls[index][1]; } });
    if (focusName && (keepFocus || document.hasFocus())) {
      const node = document.querySelector("[name='" + CSS.escape(focusName) + "']") || document.getElementById(focusName);
      if (node && node.tagName === "INPUT") { node.focus({preventScroll: true}); try { node.setSelectionRange(selStart, selEnd); } catch (_) { } }
    }
  }
  addEventListener("message", event => {
    const message = event.data; if (!message) return;
    if (message.action === "navigation") { navigation = message; draw(); return; }
    if (message.action === "editorStyle") { applyEditorStyle(message); return; }
    if (message.action === "mem-pages") { receivePages(message); return; }
    if (message.action === "view" && message.view && message.view.epoch >= (view.epoch || 0)) {
      const sameStop = view.session && message.view.session && view.session.stopId === message.view.session.stopId;
      view = message.view;
      if (!sameStop && view.kind !== "memory") local.openDetail = local.openDetail; // details survive identical keys
      draw();
    }
  });
  addEventListener("keydown", event => {
    if ((event.ctrlKey || event.metaKey) && !event.shiftKey && event.key.toLowerCase() === "r") { event.preventDefault(); event.stopPropagation(); post({action: "refresh"}); }
    if ((event.ctrlKey || event.metaKey) && event.key.toLowerCase() === "f") { const box = document.querySelector(".filter input"); if (box) { event.preventDefault(); box.focus(); box.select(); } }
    if ((event.ctrlKey || event.metaKey) && event.key.toLowerCase() === "g" && view.kind === "memory") { event.preventDefault(); focusAddress(); }
  }, true);
  let narrow = innerWidth < 330 + 16 * 26 + 120;
  addEventListener("resize", debounce(() => {
    const now = innerWidth < 330 + 16 * 26 + 120;
    if (now !== narrow) { narrow = now; draw(); }
  }, 120));
  draw();
  document.addEventListener("keydown", event => {
    if (event.altKey && !event.ctrlKey && !event.metaKey && ["ArrowLeft","ArrowRight"].includes(event.key) &&
        !event.target.closest("input,textarea,[contenteditable=true]")) {
      event.preventDefault(); event.stopPropagation(); post({action:event.key === "ArrowLeft" ? "navigate-back" : "navigate-forward"});
    }
  });
  vscode.postMessage({action: "ready"});
})();
