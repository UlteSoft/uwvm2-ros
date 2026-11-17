"use strict";
// Shared webview toolkit for UWVM inspector windows. It only builds DOM from
// already authorized, copied data; it never evaluates or dereferences anything.
(function(root) {
  const ICONS = root.UWVM_ICONS || {};
  const vscode = acquireVsCodeApi();

  function icon(name, extra) {
    const ns = "http://www.w3.org/2000/svg";
    const svg = document.createElementNS(ns, "svg");
    svg.setAttribute("viewBox", "0 0 16 16");
    svg.setAttribute("aria-hidden", "true");
    svg.setAttribute("class", "codicon codicon-" + name + (extra ? " " + extra : ""));
    // Paths are constant, generated codicon geometry (media/icons.js).
    svg.innerHTML = ICONS[name] || "";
    return svg;
  }

  // h("div.class#id", {attrs, on:{click}}, ...children)
  function h(tag, props, ...children) {
    let name = tag, id, classes = [];
    const parts = tag.split(/(?=[.#])/);
    name = parts[0] || "div";
    for (const part of parts.slice(1)) {
      if (part[0] === ".") classes.push(part.slice(1));
      else if (part[0] === "#") id = part.slice(1);
    }
    const node = document.createElement(name);
    if (id) node.id = id;
    if (classes.length) node.className = classes.join(" ");
    if (props !== undefined && props !== null && (typeof props !== "object" || props instanceof Node || Array.isArray(props))) {
      children.unshift(props); props = undefined;
    }
    for (const [key, value] of Object.entries(props || {})) {
      if (value === undefined || value === null || value === false) continue;
      if (key === "on") { for (const [event, fn] of Object.entries(value)) node.addEventListener(event, fn); }
      else if (key === "class") node.className = (node.className ? node.className + " " : "") + value;
      else if (key === "style" && typeof value === "object") {
        for (const [property, setting] of Object.entries(value)) {
          if (setting === undefined || setting === null) continue;
          if (property.startsWith("--")) node.style.setProperty(property, setting);
          else node.style[property] = setting;
        }
      }
      else if (key === "dataset") Object.assign(node.dataset, value);
      else if (key === "text") node.textContent = value;
      else if (key in node && typeof value !== "string" && key !== "list") node[key] = value;
      else node.setAttribute(key, value === true ? "" : String(value));
    }
    append(node, children);
    return node;
  }
  function append(node, children) {
    for (const child of children.flat(Infinity)) {
      if (child === undefined || child === null || child === false) continue;
      node.append(child instanceof Node ? child : document.createTextNode(String(child)));
    }
    return node;
  }

  function action(iconName, title, onClick, options = {}) {
    const node = h("button.action" + (options.label ? ".labeled" : "") + (options.danger ? ".danger" : ""), {
      type: "button", title: title + (options.key ? " (" + options.key + ")" : ""), "aria-label": title,
      disabled: options.disabled || undefined, "aria-pressed": options.pressed === undefined ? undefined : String(!!options.pressed),
      on: {click: event => { if (!node.disabled) onClick(event); }}
    }, icon(iconName, options.spin ? "spin" : ""), options.label);
    if (options.id) node.id = options.id;
    return node;
  }
  function button(text, onClick, options = {}) {
    return h("button.btn" + (options.secondary ? ".secondary" : "") + (options.small ? ".small" : ""), {
      type: options.submit ? "submit" : "button", disabled: options.disabled || undefined, title: options.title,
      on: options.submit ? undefined : {click: event => onClick && onClick(event)}
    }, options.icon ? icon(options.icon) : null, text);
  }

  // Typed value with Variables-view colouring.
  function value(raw, hint) {
    if (raw === null) return h("span.v-null", "null");
    if (raw === undefined) return h("span.v-undefined", "—");
    if (typeof raw === "object" && raw && typeof raw.text === "string" && "bytesBase64" in raw) {
      if (!raw.text && !raw.truncated) return h("span.v-empty", "empty");
      return h("span", h("span.v-string", raw.text), raw.truncated ? h("span.trunc", {title: "Value continues beyond this transport fragment"}, "truncated") : null);
    }
    if (typeof raw === "boolean") return h("span.v-boolean", String(raw));
    if (typeof raw === "number" || typeof raw === "bigint") return h("span.v-number", String(raw));
    const text = String(raw);
    if (hint === "policy") {
      if (/^(allow|allowed|enabled|true|yes|granted|configured)$/i.test(text)) return h("span.v-allow", text);
      if (/^(deny|denied|disabled|false|no|blocked)$/i.test(text)) return h("span.v-deny", text);
      if (/^(hidden|inherit|unset|none)$/i.test(text)) return h("span.v-hidden", text);
    }
    if (text === "") return h("span.v-empty", "empty");
    if (/^[+-]?(?:0x[0-9a-f]+(?:\.[0-9a-f]+)?p[+-]?[0-9]+|0x[0-9a-f]+|0b[01]+|[0-9]+(?:\.[0-9]+)?(?:e[+-]?[0-9]+)?|Infinity|[qs]?NaN)$/i.test(text)) return h("span.v-number", text);
    if (/^(true|false)$/.test(text)) return h("span.v-boolean", text);
    return h("span.v-string", text);
  }

  // Lazily expanding JSON tree. Paths are remembered across re-renders.
  const openPaths = new Set();
  function tree(data, rootPath = "$", options = {}) {
    const container = h("div.tree");
    const limit = options.limit || 200;
    function summary(v) {
      if (Array.isArray(v)) return "Array(" + v.length + ")";
      if (v && typeof v === "object") {
        if (typeof v.text === "string" && "bytesBase64" in v) return null;
        const keys = Object.keys(v);
        return "{" + keys.slice(0, 4).join(", ") + (keys.length > 4 ? ", …" : "") + "}";
      }
      return null;
    }
    function row(key, v, path, depth) {
      const complex = v !== null && typeof v === "object" && summary(v) !== null;
      const wrap = h("div");
      const open = complex && (openPaths.has(path) || (options.expandDepth || 0) > depth && !openPaths.has("!" + path));
      const twist = h("button.twistie" + (complex ? "" : ".leaf"), {type: "button", tabindex: complex ? 0 : -1, "aria-label": complex ? (open ? "Collapse" : "Expand") : undefined},
        complex ? icon(open ? "chevron-down" : "chevron-right") : null);
      const line = h("div.tree-row", {role: "treeitem", "aria-expanded": complex ? String(open) : undefined},
        twist, key !== undefined ? h("span.key", key) : null,
        complex ? h("span.summary", summary(v)) : h("span.val", value(v)));
      wrap.append(line);
      if (complex) {
        const toggle = () => {
          if (openPaths.has(path)) { openPaths.delete(path); openPaths.add("!" + path); }
          else { openPaths.add(path); openPaths.delete("!" + path); }
          wrap.replaceWith(row(key, v, path, depth));
          persist();
        };
        twist.addEventListener("click", toggle);
        line.addEventListener("dblclick", toggle);
        if (open) {
          const children = h("div.tree-children", {role: "group"});
          const entries = Array.isArray(v) ? v.map((item, index) => [String(index), item]) : Object.entries(v);
          for (const [childKey, child] of entries.slice(0, limit)) children.append(row(childKey, child, path + "/" + childKey, depth + 1));
          if (entries.length > limit) children.append(h("div.tree-more.inline-empty", (entries.length - limit) + " more entries are hidden; export JSON to see everything."));
          if (!entries.length) children.append(h("div.tree-more.inline-empty", Array.isArray(v) ? "Empty array" : "Empty object"));
          wrap.append(children);
        }
      }
      return wrap;
    }
    if (data !== null && typeof data === "object" && summary(data) !== null && options.hideRoot !== false) {
      const entries = Array.isArray(data) ? data.map((item, index) => [String(index), item]) : Object.entries(data);
      for (const [key, child] of entries.slice(0, limit)) container.append(row(key, child, rootPath + "/" + key, 0));
      if (!entries.length) container.append(h("div.inline-empty", "No fields."));
    } else container.append(row(undefined, data, rootPath, 0));
    return container;
  }

  // Context menu identical in structure to the workbench menu.
  let menuNode;
  function closeMenu() { if (menuNode) { menuNode.remove(); menuNode = undefined; } }
  function menu(event, items) {
    event.preventDefault(); closeMenu();
    const node = h("div.menu", {role: "menu"});
    for (const item of items) {
      if (item === "-") { node.append(h("hr")); continue; }
      if (!item) continue;
      const entry = h("button", {type: "button", role: "menuitem", disabled: item.disabled || undefined,
        on: {click: () => { closeMenu(); item.run(); }}},
        item.icon ? icon(item.icon) : h("span.blank"), h("span.label", item.label), item.key ? h("span.key", item.key) : null);
      node.append(entry);
    }
    document.body.append(node);
    const width = node.offsetWidth, height = node.offsetHeight;
    node.style.left = Math.max(2, Math.min(event.clientX, innerWidth - width - 4)) + "px";
    node.style.top = Math.max(2, Math.min(event.clientY, innerHeight - height - 4)) + "px";
    menuNode = node;
    const first = node.querySelector("button:not(:disabled)");
    if (first) first.focus({preventScroll: true});
    node.addEventListener("keydown", key => {
      const buttons = [...node.querySelectorAll("button:not(:disabled)")], at = buttons.indexOf(document.activeElement);
      if (key.key === "Escape") { key.preventDefault(); closeMenu(); }
      else if (key.key === "ArrowDown") { key.preventDefault(); buttons[(at + 1) % buttons.length]?.focus(); }
      else if (key.key === "ArrowUp") { key.preventDefault(); buttons[(at - 1 + buttons.length) % buttons.length]?.focus(); }
    });
  }
  addEventListener("mousedown", event => { if (menuNode && !menuNode.contains(event.target)) closeMenu(); }, true);
  addEventListener("blur", closeMenu);
  addEventListener("resize", closeMenu);

  // Resizable split with persisted size.
  function split(first, second, options = {}) {
    // A side pane needs room beside its main content. In a narrow editor it
    // moves below instead, so the main content never collapses to nothing.
    const vertical = options.vertical !== undefined ? !!options.vertical :
      options.stackBelow !== undefined && innerWidth < options.stackBelow;
    const key = "sash:" + (options.id || "main") + (vertical ? ":v" : "");
    const node = h("div.split" + (vertical ? ".vertical" : ""));
    const one = h("div.first", first), two = h("div.second", second), sash = h("div.sash", {role: "separator", "aria-orientation": vertical ? "horizontal" : "vertical"});
    const saved = state.get(key);
    const preferred = vertical ? (options.verticalSize || 240) : (options.size || 320);
    const size = Number.isFinite(saved) ? saved : preferred;
    two.style[vertical ? "height" : "width"] = size + "px";
    let finishDrag;
    sash.addEventListener("pointerdown", event => {
      if (event.button !== 0) return;
      if (finishDrag) finishDrag();
      event.preventDefault(); sash.setPointerCapture(event.pointerId); sash.classList.add("active");
      const start = vertical ? event.clientY : event.clientX, initial = vertical ? two.offsetHeight : two.offsetWidth;
      const move = e => {
        if (e.pointerId !== event.pointerId) return;
        if (!(e.buttons & 1)) { up(); return; }
        const total = vertical ? node.clientHeight : node.clientWidth;
        const next = Math.max(vertical ? 120 : (options.min || 160), Math.min(total - (options.minFirst || 200), initial - ((vertical ? e.clientY : e.clientX) - start)));
        two.style[vertical ? "height" : "width"] = next + "px";
      };
      const up = () => {
        if (finishDrag !== up) return;
        finishDrag = undefined;
        sash.removeEventListener("pointermove", move);
        for (const name of ["pointerup", "pointercancel", "lostpointercapture"]) sash.removeEventListener(name, up);
        window.removeEventListener("blur", up);
        if (sash.hasPointerCapture(event.pointerId)) sash.releasePointerCapture(event.pointerId);
        sash.classList.remove("active");
        state.set(key, vertical ? two.offsetHeight : two.offsetWidth);
      };
      finishDrag = up;
      sash.addEventListener("pointermove", move);
      for (const name of ["pointerup", "pointercancel", "lostpointercapture"]) sash.addEventListener(name, up);
      window.addEventListener("blur", up);
    });
    sash.addEventListener("dblclick", () => { two.style[vertical ? "height" : "width"] = preferred + "px"; state.set(key, preferred); });
    node.append(one, sash, two);
    if (options.hidden) { sash.hidden = true; two.hidden = true; }
    return node;
  }

  // Persisted UI state (survives tab hide/show and window reload).
  const memory = Object.assign({}, vscode.getState() || {});
  function persist() { memory.__open = [...openPaths].slice(-500); vscode.setState(memory); }
  for (const path of memory.__open || []) openPaths.add(path);
  const state = {
    get(key) { return memory[key]; },
    set(key, v) { memory[key] = v; persist(); }
  };

  // Keyboard navigation for a table body: Up/Down/Home/End/PageUp/PageDown.
  function listKeys(container, rowSelector, onSelect, onActivate) {
    container.addEventListener("keydown", event => {
      const rows = [...container.querySelectorAll(rowSelector)].filter(row => !row.hidden);
      if (!rows.length) return;
      const current = rows.findIndex(row => row === document.activeElement || row.contains(document.activeElement));
      let next = current;
      if (event.key === "ArrowDown") next = Math.min(rows.length - 1, current + 1);
      else if (event.key === "ArrowUp") next = Math.max(0, current - 1);
      else if (event.key === "Home") next = 0;
      else if (event.key === "End") next = rows.length - 1;
      else if (event.key === "PageDown") next = Math.min(rows.length - 1, current + 10);
      else if (event.key === "PageUp") next = Math.max(0, current - 10);
      else if (event.key === "Enter" && current >= 0 && onActivate) { event.preventDefault(); onActivate(rows[current], event); return; }
      else return;
      event.preventDefault();
      if (next < 0) next = 0;
      rows[next].focus({preventScroll: false});
      rows[next].scrollIntoView({block: "nearest"});
      if (onSelect) onSelect(rows[next], event);
    });
  }

  // Debounce helper
  function debounce(fn, ms) { let timer; return (...args) => { clearTimeout(timer); timer = setTimeout(() => fn(...args), ms); }; }

  // Editor font + line height follow the user's VS Code editor settings.
  function applyEditorStyle(message) {
    const style = document.documentElement.style;
    style.setProperty("--vscode-editor-font-size", message.fontSize + "px");
    style.setProperty("--vscode-editor-font-family", message.fontFamily);
    style.setProperty("--vscode-editor-font-weight", message.fontWeight);
    const line = message.lineHeight || 0;
    style.setProperty("--uwvm-code-line-height", (line === 0 ? Math.round(1.5 * message.fontSize) : line < 8 ? Math.round(line * message.fontSize) : line) + "px");
    style.setProperty("--uwvm-code-letter-spacing", (message.letterSpacing || 0) + "px");
  }

  function hex(n, width) { return "0x" + BigInt(n).toString(16).padStart(width, "0"); }
  function plural(n, word, many) { return n + " " + (Number(n) === 1 ? word : (many || word + "s")); }
  function copy(text) { vscode.postMessage({action: "copy", text: String(text)}); }

  root.uwvmUI = {vscode, h, append, icon, action, button, value, tree, menu, closeMenu, split, state, listKeys, debounce,
    applyEditorStyle, hex, plural, copy, openPaths, persist};
})(globalThis);
