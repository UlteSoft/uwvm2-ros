"use strict";
// Wasm instruction listing. Rows are absolutely positioned inside a rolling
// segment so long functions stay virtual, while the address rail stays fixed.
// Data never becomes a native pointer or an execution capability.
(() => {
  const api = acquireVsCodeApi();
  const code = document.getElementById("code"), rail = document.getElementById("rail-rows"), body = document.getElementById("code-rows"),
    space = document.getElementById("space"), tip = document.getElementById("hover");
  let rows = [], state = {valid: false}, revision = 0, rowHeight = 20, charWidth = 8, base = 0, segment = 1;
  let points = new Map(), drawn = new Map(), selection = 0, anchor = 0, lastFollow, hoverSerial = 0, hoverTimer, pointer;
  let mnemonicWidth = 12, operandWidth = 8, maxColumns = 1, followExtent = 0, manualSaved = api.getState(), editorStyle = {};
  const post = message => api.postMessage({...message, revision});
  const element = (name, className, text) => { const node = document.createElement(name); if (className) node.className = className; if (text !== undefined) node.textContent = text; return node; };
  const canWrite = () => state.valid && state.permission === "full";
  const canMove = () => canWrite() && !!state.movable && state.movable.wasm;
  function notice(text, error = false) { const node = document.getElementById("notice"); node.textContent = text || ""; node.classList.toggle("error", error); node.hidden = !text; }
  function hideHover() { clearTimeout(hoverTimer); ++hoverSerial; tip.hidden = true; pointer = undefined; }
  function save() { const position={top:base+code.scrollTop/rowHeight,left:code.scrollLeft,selection,anchor};api.setState(position);post({action:"viewport",position:{...position,line:selection+2}}); }
  function measure() {
    const probe = element("span", "", "0000000000");
    probe.style.cssText = "position:absolute;visibility:hidden;white-space:pre";
    code.append(probe); charWidth = probe.getBoundingClientRect().width / 10 || 8; probe.remove();
    const font = parseFloat(getComputedStyle(code).fontSize) || 13, line = editorStyle.lineHeight || 0;
    rowHeight = Math.round(line === 0 ? font * 1.5 : line < 8 ? line * font : line);
    document.documentElement.style.setProperty("--row-height", rowHeight + "px");
    segment = Math.max(1, Math.floor(8_000_000 / rowHeight));
    space.style.height = Math.min(segment, Math.max(0, rows.length - base)) * rowHeight + "px";
    space.style.width = Math.max(code.clientWidth, (maxColumns + 3.2) * charWidth, followExtent) + "px";
  }
  function setBase(first) {
    base = Math.max(0, Math.min(first, Math.max(0, rows.length - segment)));
    drawn.clear(); body.replaceChildren(); rail.replaceChildren();
    space.style.height = Math.min(segment, rows.length - base) * rowHeight + "px";
  }
  function go(index, follow = false) {
    if (!rows.length) return;
    index = Math.max(0, Math.min(index, rows.length - 1));
    if (index < base || index >= base + segment) setBase(Math.max(0, index - Math.floor(segment / 2)));
    const top = (index - base) * rowHeight, bottom = top + rowHeight;
    if (top < code.scrollTop || bottom > code.scrollTop + code.clientHeight) code.scrollTop = Math.max(0, top - code.clientHeight * 0.35);
    if (follow) {
      // Move horizontally only when the instruction start is outside the
      // visible instruction pane. A visible start keeps the user's position.
      const row = rows[index], indent = (row.depth * 4 + 1.2) * charWidth;
      const visible = indent - code.scrollLeft, width = code.clientWidth;
      if (visible < 0) code.scrollLeft = Math.max(0, indent - width * 0.25);
      else if (indent > width - 8 * charWidth) {
        const target = Math.max(0, indent - width * 0.75);
        followExtent = Math.max(followExtent, Math.ceil(target + width));
        measure(); code.scrollLeft = target;
      }
    }
    selection = anchor = index; draw(); save();
  }
  // Guides are drawn only for scopes that are actually open at each depth, the
  // way an editor draws indent guides: the ancestor chain of this row.
  function openScopes(row) {
    const chain = [], seen = new Set();
    let id = row.scope, guard = 0;
    while (id !== undefined && id >= 0 && !seen.has(id) && guard++ < 512) { seen.add(id); chain.push(id); id = scopeParent(id); }
    return new Set(chain);
  }
  const parents = new Map();
  function scopeParent(id) { return parents.has(id) ? parents.get(id) : undefined; }
  function styleRow(index, pair) {
    const row = rows[index], point = points.get(row.line), isCurrent = (state.valid && state.line === row.line) || (!!state.next && state.next.exact === true && state.next.line === row.line);
    const isNext = !isCurrent && !!state.next && state.next.line === row.line, marked = isCurrent || isNext;
    for (const node of [pair.text, pair.gutter]) {
      node.classList.toggle("current", isCurrent); node.classList.toggle("next", isNext);
      node.classList.toggle("selected", index >= Math.min(anchor, selection) && index <= Math.max(anchor, selection));
    }
    pair.pc.hidden = !marked;
    pair.pc.classList.toggle("next", isNext);
    pair.pc.classList.toggle("movable", isCurrent && canMove());
    pair.pc.title = isCurrent ? "Exact position: the stop is at this Wasm instruction · thread " + (state.valid ? state.threadId : state.next.threadId) + " · stop " + (state.valid ? state.stopId : state.next.stopId) +
        (canMove() ? "\nDrag the arrow to another instruction to move execution there." : state.movable && state.movable.reason ? "\n" + state.movable.reason : "")
      : isNext ? "Inside this Wasm instruction: execution is inside it (assembly stop" + (state.next.nativePc ? " at " + state.next.nativePc : "") + ")." +
        (state.next.provenance === "exact" ? "" : "\nMapped from the last Wasm safepoint.") + "\nThe next pointer cannot be dragged." : "";
    const b = pair.breakpoint;
    b.className = "breakpoint " + (!point ? "empty" : point.pending ? "pending" : point.enabled ? "enabled" : "disabled") + (marked ? " at-pc" : "") + (point && point.condition ? " conditional" : "");
    b.disabled = !canWrite() || !row.safe;
    b.title = point ? (point.pending ? "Waiting for the runtime to confirm this breakpoint" : (point.enabled ? "Breakpoint " : "Disabled breakpoint ") + point.ids.map(id => "#" + id).join(", ")) + (point.condition ? "\nCondition: " + point.condition : "") + "\nShift-click to " + (point.enabled ? "disable" : "enable") + " · right-click for options"
      : !row.safe ? "Display-only opcode: this optimized generation has no stop point here" : !canWrite() ? "Changing breakpoints needs full permission at a current stop" : "Click to add a breakpoint · right-click for options";
    b.setAttribute("aria-label", b.title + " at " + row.address);
    b.setAttribute("aria-pressed", point ? "true" : "false");
    pair.text.setAttribute("aria-selected", String(index === selection));
  }
  function createRow(index) {
    const row = rows[index];
    const text = element("div", "row"), gutter = element("div", "rail-row"), bp = element("button", "breakpoint"), pc = element("span", "pc"),
      address = element("span", "address", row.address);
    text.dataset.index = gutter.dataset.index = String(index);
    text.setAttribute("role", "row"); text.id = "instruction-" + index;
    bp.type = "button"; bp.dataset.breakpoint = String(row.line);
    address.dataset.hover = "address"; address.dataset.line = String(row.line);
    const glyph = document.createElementNS("http://www.w3.org/2000/svg", "svg"), path = document.createElementNS("http://www.w3.org/2000/svg", "path");
    glyph.setAttribute("viewBox", "0 0 16 16"); glyph.setAttribute("aria-hidden", "true"); glyph.setAttribute("focusable", "false");
    path.setAttribute("d", "M8.879 14H5c-1.103 0-2-.897-2-2V4c0-1.103.897-2 2-2h3.879a2.01 2.01 0 0 1 1.414.586l4.212 4.212c.321.321.498.748.498 1.202 0 .454-.177.881-.498 1.202l-4.212 4.212A2.01 2.01 0 0 1 8.879 14ZM5 3c-.551 0-1 .449-1 1v8c0 .551.449 1 1 1h3.879c.263 0 .521-.107.707-.293l4.212-4.212a.696.696 0 0 0 0-.99L9.586 3.293A1.008 1.008 0 0 0 8.879 3H5Z");
    glyph.append(path); pc.append(glyph);
    // Match the source editor's supported two-lane decoration layout.
    const dot = element("span", "dot"); dot.setAttribute("aria-hidden", "true");
    bp.append(dot, pc);
    gutter.append(bp, address);
    const scopeSet = openScopes(row), indent = element("span", "indent");
    indent.style.width = row.depth * 4 + "ch";
    for (let level = 0; level < row.depth; level++) {
      if (scopeSet.has(row.ancestors[level])) indent.append(element("span", "guide"));
      else indent.append(element("span", "guide blank"));
    }
    const name = element("span", "mnemonic token " + category(row), row.name);
    name.dataset.hover = "instruction"; name.dataset.line = String(row.line);
    const operands = element("span", "operands");
    row.tokens.forEach((token, n) => {
      if (n) operands.append(document.createTextNode(" "));
      const key = token.meaning && token.meaning.kind === "object" ? token.meaning.key : undefined;
      const kind = token.navigation ? "label" : key === "function" ? "function" : key === "memory" || key === "table" ? "type" : key === "label" ? "label" : key ? "index"
        : /^(?:i32|i64|f32|f64|v128|ref|func|extern|struct|array|any|eq|i31|externref|funcref)$/.test(token.text) ? "type" : "number";
      const span = element("span", "token " + kind, token.text);
      span.dataset.token = String(n); span.dataset.line = String(row.line);
      if (token.navigation) span.dataset.targetLine = String(token.navigation.line);
      operands.append(span);
    });
    text.append(indent, name, operands);
    if (row.comment) text.append(element("span", "comment", "; " + row.comment));
    return {text, gutter, breakpoint: bp, pc};
  }
  function category(row) {
    return {Control: "control", Variables: "variable", Numeric: "numeric", Memory: "memory", GC: "gc", SIMD: "vector",
      References: "reference", Tables: "table", Threads: "atomic", MTE: "memtag"}[row.family] || "numeric";
  }
  function draw() {
    if (!rows.length) return;
    const first = Math.max(base, base + Math.floor(code.scrollTop / rowHeight) - 8),
      last = Math.min(rows.length, base + segment, base + Math.ceil((code.scrollTop + code.clientHeight) / rowHeight) + 8);
    for (const [index, pair] of drawn) if (index < first || index >= last) { pair.text.remove(); pair.gutter.remove(); drawn.delete(index); }
    for (let i = first; i < last; ++i) {
      let pair = drawn.get(i);
      if (!pair) { pair = createRow(i); drawn.set(i, pair); body.append(pair.text); rail.append(pair.gutter); }
      pair.text.style.top = pair.gutter.style.top = (i - base) * rowHeight + "px";
      styleRow(i, pair);
    }
    rail.style.transform = "translateY(" + (-code.scrollTop) + "px)";
    code.setAttribute("aria-activedescendant", "instruction-" + selection);
  }
  code.addEventListener("scroll", () => {
    const absolute = base + code.scrollTop / rowHeight;
    if (rows.length > segment && (base > 0 && code.scrollTop < code.clientHeight * 2 || base + segment < rows.length && code.scrollTop > segment * rowHeight - code.clientHeight * 3)) {
      setBase(Math.floor(absolute) - Math.floor(segment / 2)); code.scrollTop = (absolute - base) * rowHeight;
    }
    hideHover(); draw(); save();
  });
  document.getElementById("rail").addEventListener("wheel", event => { code.scrollTop += event.deltaY; code.scrollLeft += event.deltaX; event.preventDefault(); }, {passive: false});
  code.addEventListener("wheel", event => { if (event.shiftKey && event.deltaY && !event.deltaX) { code.scrollLeft += event.deltaY; event.preventDefault(); } }, {passive: false});
  // Drag the exact (yellow) pointer onto another instruction to move
  // execution. A press without movement is an ordinary gutter click
  // (toggle the breakpoint). The next (green) pointer never moves.
  let drag, suppressClick = false;
  document.getElementById("rail").addEventListener("pointerdown", event => {
    const pc = event.target.closest(".pc"); if (!pc || event.button !== 0 || pc.hidden) return;
    const cell = pc.closest("[data-breakpoint]");
    event.preventDefault(); suppressClick = true;
    drag = {pointerId: event.pointerId, x: event.clientX, y: event.clientY, moved: false, next: pc.classList.contains("next"),
      movable: canMove() && !pc.classList.contains("next"), ghost: undefined, target: undefined,
      line: cell ? Number(cell.dataset.breakpoint) : undefined, shift: event.shiftKey, cellDisabled: !cell || cell.disabled};
    try { pc.setPointerCapture(event.pointerId); } catch (_) { }
  });
  function moveDrag(event) {
    const box = code.getBoundingClientRect(), y = Math.max(box.top, Math.min(box.bottom - 1, event.clientY));
    const index = Math.max(0, Math.min(rows.length - 1, base + Math.floor((y - box.top + code.scrollTop) / rowHeight)));
    drag.target = index;
    drag.ghost.style.top = (box.top + (index - base) * rowHeight - code.scrollTop) + "px";
    drag.ghost.style.left = document.getElementById("rail").getBoundingClientRect().left + "px";
    drag.ghost.classList.toggle("invalid", !rows[index].safe);
    if (event.clientY < box.top + rowHeight) code.scrollTop -= rowHeight; else if (event.clientY > box.bottom - rowHeight) code.scrollTop += rowHeight;
  }
  document.addEventListener("pointermove", event => {
    if (!drag) return;
    if (!(event.buttons & 1)) { endPointerDrag(false); return; }
    if (!drag.moved && Math.hypot(event.clientX - drag.x, event.clientY - drag.y) > 4) {
      drag.moved = true;
      if (!drag.movable) {
        notice(drag.next ? "The green arrow marks the next Wasm instruction inside the current assembly stop; it cannot be moved." :
          state.movable && state.movable.reason ? state.movable.reason : "Moving the execution pointer needs full permission at a current stop.", true);
      } else { drag.ghost = element("div", "drag-ghost"); document.body.append(drag.ghost); }
    }
    if (drag.moved && drag.ghost) moveDrag(event);
  });
  function endPointerDrag(commit) {
    if (!drag) return;
    const current = drag; drag = undefined;
    if (current.ghost) current.ghost.remove();
    if (!commit) return;
    if (!current.moved) {
      // Plain click on the arrow: same as clicking the breakpoint cell.
      if (!current.cellDisabled && current.line !== undefined) post({action: "breakpoint", operation: current.shift ? "enabled" : "toggle", line: current.line});
      return;
    }
    const target = current.target;
    if (!current.ghost || target === undefined || !state.valid || target === state.line - 2) return;
    if (!rows[target].safe) { notice("That opcode has no stop point in this generation; execution can move only to an instruction with a stop point.", true); return; }
    notice("Moving execution to " + rows[target].address + "…");
    post({action: "jump", line: rows[target].line});
  }
  document.addEventListener("pointerup", () => { if (drag) endPointerDrag(true); });
  document.addEventListener("keydown", event => { if (drag && event.key === "Escape") { event.preventDefault(); endPointerDrag(false); notice("Move cancelled."); } }, true);
  document.addEventListener("click", event => {
    if (suppressClick) { suppressClick = false; if (event.target.closest(".breakpoint")) return; }
    const button = event.target.closest("button"), action = button && button.dataset.action;
    if (action) { if (action === "follow") go((state.line || 2) - 2, true); else post({action}); return; }
    const point = event.target.closest("[data-breakpoint]");
    if (point && !point.disabled) { post({action: "breakpoint", operation: event.shiftKey ? "enabled" : "toggle", line: Number(point.dataset.breakpoint)}); return; }
    const destination = event.target.closest(".token[data-target-line]");
    if (destination && (event.ctrlKey || event.metaKey)) {
      const index = Number(destination.dataset.targetLine) - 2;
      if (Number.isSafeInteger(index) && index >= 0 && index < rows.length) {
        event.preventDefault(); hideHover(); post({action:"navigate-line",line:index+2}); code.focus({preventScroll: true});
        notice("Branch destination: " + rows[index].address + " · " + rows[index].name);
      }
      return;
    }
    const call = event.target.closest(".token.function[data-token]");
    if(call&&(event.ctrlKey||event.metaKey)){event.preventDefault();post({action:"navigate-call",line:Number(call.dataset.line),token:Number(call.dataset.token)});return;}
    const row = event.target.closest("[data-index]");
    if (row) { selection = Number(row.dataset.index); if (!event.shiftKey) anchor = selection; draw(); code.focus({preventScroll: true}); save(); }
  });
  document.addEventListener("contextmenu", event => { const point = event.target.closest("[data-breakpoint]"); if (point && !point.disabled) { event.preventDefault(); post({action: "breakpoint", operation: "menu", line: Number(point.dataset.breakpoint)}); } });
  document.getElementById("jump").addEventListener("submit", event => {
    event.preventDefault(); const input = document.getElementById("address"), raw = input.value.trim();
    if (!/^(?:0x[0-9a-f]+|[0-9]+)$/i.test(raw)) { notice("Enter a hexadecimal or decimal byte offset.", true); return; }
    const target = BigInt(raw); let low = 0, high = rows.length;
    while (low < high) { const mid = Math.floor((low + high) / 2); if (BigInt(rows[mid].offset) < target) low = mid + 1; else high = mid; }
    if (low === rows.length || BigInt(rows[low].offset) !== target) { notice("That offset is not an instruction boundary in this function.", true); return; }
    notice(""); post({action:"navigate-line",line:low+2}); code.focus({preventScroll: true});
  });
  document.addEventListener("keydown", event => {
    if (event.target instanceof HTMLInputElement) return;
    if(event.altKey&&!event.ctrlKey&&!event.metaKey&&["ArrowLeft","ArrowRight"].includes(event.key)){
      event.preventDefault();event.stopPropagation();post({action:event.key==="ArrowLeft"?"navigate-back":"navigate-forward"});return;
    }
    if (event.key === "Escape") { hideHover(); return; }
    if (event.key === "F9") { event.preventDefault(); if (canWrite()) post({action: "breakpoint", operation: event.shiftKey ? "enabled" : "toggle", line: rows[selection] && rows[selection].line}); return; }
    if ((event.ctrlKey || event.metaKey) && event.key.toLowerCase() === "c" && !window.getSelection().toString()) { event.preventDefault(); post({action: "copy", first: anchor, last: selection}); return; }
    if (event.key === "ArrowDown" || event.key === "ArrowUp") { event.preventDefault(); go(selection + (event.key === "ArrowDown" ? 1 : -1)); }
    if (event.key === "ArrowRight" || event.key === "ArrowLeft") { event.preventDefault(); code.scrollLeft += (event.key === "ArrowRight" ? 1 : -1) * 4 * charWidth; }
    if (event.key === "Home") { event.preventDefault(); go(0, true); }
    if (event.key === "End") { event.preventDefault(); go(rows.length - 1, true); }
  });
  document.addEventListener("pointerover", event => {
    const token = event.target.closest(".token"); if (!token) return;
    hideHover(); pointer = {x: event.clientX, y: event.clientY, target: token};
    const request = ++hoverSerial;
    hoverTimer = setTimeout(() => { if ((state.valid || state.next) && pointer && pointer.target === token) post({action: "hover", request, line: Number(token.dataset.line), part: token.dataset.hover, token: token.dataset.token === undefined ? undefined : Number(token.dataset.token)}); }, 350);
  });
  document.addEventListener("pointerout", event => { if (event.target.closest(".token")) hideHover(); });
  window.addEventListener("message", event => {
    const message = event.data;
    if (!message || !Number.isSafeInteger(message.revision) || message.revision < revision) return;
    revision = message.revision;
    if (message.action === "editorStyle") {
      const top = base + code.scrollTop / rowHeight, left = code.scrollLeft;
      editorStyle = message;
      const root = document.documentElement.style;
      root.setProperty("--vscode-editor-font-size", message.fontSize + "px");
      root.setProperty("--vscode-editor-font-family", message.fontFamily);
      root.setProperty("--vscode-editor-font-weight", message.fontWeight);
      code.style.letterSpacing = rail.style.letterSpacing = message.letterSpacing + "px";
      measure(); code.scrollTop = (top - base) * rowHeight; code.scrollLeft = left; draw(); return;
    }
    if (message.action === "load") {
      rows = message.rows; followExtent = 0; mnemonicWidth = message.mnemonicWidth + 2; operandWidth = message.operandWidth;
      parents.clear();
      for (const scope of message.scopes || []) parents.set(scope.id, scope.parent);
      for (const row of rows) {
        const chain = [], seen = new Set();
        let id = row.scope, guard = 0;
        while (id !== undefined && id >= 0 && !seen.has(id) && guard++ < 512) { seen.add(id); chain.unshift(id); id = parents.get(id); }
        row.ancestors = chain;
      }
      document.documentElement.style.setProperty("--mnemonic-width", mnemonicWidth + "ch");
      document.documentElement.style.setProperty("--operand-width", operandWidth + "ch");
      const addressColumns = rows.reduce((n, row) => Math.max(n, row.address.length), message.addressBits / 4 + 2);
      document.documentElement.style.setProperty("--address-width", addressColumns + "ch");
      code.setAttribute("aria-rowcount", String(rows.length));
      maxColumns = rows.reduce((n, row) => Math.max(n, row.depth * 4 + mnemonicWidth + operandWidth + row.comment.length + 2), 0);
      drawn.clear(); body.replaceChildren(); rail.replaceChildren(); measure();
      if (manualSaved) { setBase(Math.floor(manualSaved.top / segment) * segment); code.scrollTop = (manualSaved.top - base) * rowHeight; code.scrollLeft = manualSaved.left; selection = manualSaved.selection || 0; anchor = manualSaved.anchor || selection; manualSaved = undefined; }
      draw(); return;
    }
    if (message.action === "state") {
      clearTimeout(pendingState);
      if (!message.valid && state.valid) { const held = message; pendingState = setTimeout(() => applyState(held), 220); return; }
      applyState(message); return;
    }
    if(message.action==="navigation"){for(const [name,key]of[["navigate-back","back"],["navigate-forward","forward"]])document.querySelector('[data-action="'+name+'"]').disabled=!message[key];return;}
    if(message.action==="restore-viewport"){if(rows.length){setBase(Math.floor(message.top/segment)*segment);code.scrollTop=(message.top-base)*rowHeight;code.scrollLeft=message.left;selection=anchor=message.line-2;draw();save();}return;}
    if (message.action === "reveal") { if(Number.isSafeInteger(message.line)&&message.line>=2&&message.line<=rows.length+1)go(message.line-2,true);return; }
    if(message.action==="safe-lines"){const safe=new Set(message.lines);for(const row of rows)row.safe=safe.has(row.line);draw();return;}
    if (message.action === "breakpoints") { points = new Map(message.rows.map(row => [row.line, row])); draw(); return; }
    if (message.action === "notice") { notice(message.message, message.error); return; }
    if (message.action === "hover" && message.request === hoverSerial && pointer && (state.valid || state.next)) showHover(message);
  });
  let pendingState;
  function applyState(message) {
    {
      const old = state; state = message;
      if (old.stopId !== state.stopId || old.threadId !== state.threadId || old.valid !== state.valid) hideHover();
      document.getElementById("status").textContent = state.valid ? "Paused · thread " + state.threadId + " · stop " + state.stopId + " · " + state.permission :
        state.next ? "Assembly stop" + (state.next.nativePc ? " at " + state.next.nativePc : "") + (state.next.exact ? " · yellow arrow = before this Wasm instruction" : " · green arrow = inside this Wasm instruction") : state.message || "Historical view; execution controls are disabled.";
      document.getElementById("identity").textContent = (state.sessionName ? state.sessionName + " · " : "") + "module " + (state.module === undefined ? "?" : state.module) + " · func " + (state.function === undefined ? "?" : state.function) + (state.name ? " · " + state.name : "");
      document.body.dataset.canWrite = String(canWrite());
      const key = state.valid ? state.stopId + ":" + state.threadId + ":" + state.line : undefined;
      const nextKey = !state.valid && state.next ? "next:" + state.next.stopId + ":" + state.next.line : undefined;
      if (key && key !== lastFollow) { lastFollow = key; if(message.follow!==false)post({action:"navigate-line",line:state.line});else draw(); }
      else if (nextKey && nextKey !== lastFollow) { lastFollow = nextKey; if(message.follow!==false)go(state.next.line - 2, true);else draw(); }
      else draw();
      if (old.valid && !state.valid) notice("");
    }
  }
  function showHover(message) {
    {
      const heading = element("strong", "", message.title || "");
      // Use the actual hovered token's semantic colour, including theme overrides.
      heading.style.color = getComputedStyle(pointer.target).color;
      tip.replaceChildren(heading);
      if (message.type !== undefined) tip.append(element("span", "type", message.type + " "));
      if (message.value !== undefined) tip.append(element("span", "value", message.value + "\n"));
      if (message.description) tip.append(element("div", "", message.description));
      tip.hidden = false;
      const width = tip.offsetWidth, height = tip.offsetHeight;
      tip.style.left = Math.max(4, Math.min(pointer.x, window.innerWidth - width - 8)) + "px";
      tip.style.top = Math.max(4, Math.min(pointer.y + 18, window.innerHeight - height - 8)) + "px";
    }
  }
  new ResizeObserver(() => { measure(); draw(); }).observe(code);
  post({action: "ready"});
})();
