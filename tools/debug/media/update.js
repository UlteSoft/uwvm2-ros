"use strict";
// Independent hot-update window. Submits an existing signed frame exactly once;
// it never signs, builds a transaction or retries.
(() => {
  const {vscode, h, icon, action, button, tree, applyEditorStyle} = uwvmUI;
  const app = document.getElementById("app");
  let view = JSON.parse(document.body.dataset.initial || "{}");
  const draft = {};
  let navigation={back:false,forward:false};
  const post = message => vscode.postMessage({...message, epoch: view.epoch});
  const MODES = {
    local: {title: "Local host", text: "A Linux update relay on this machine.", icon: "vm", fields: ["pythonPath", "socketDir", "framePath"]},
    profile: {title: "Imported profile / SSH", text: "A host-owned profile; SSH keeps strict host-key checks.", icon: "server", fields: ["pythonPath", "profilePath", "framePath"]},
    session: {title: "Active debug session", text: "The session's own independent update socket.", icon: "debug-alt", fields: ["framePath"]}
  };
  const LABELS = {
    pythonPath: ["Python executable", "Absolute path on this machine", "/usr/bin/python3"],
    socketDir: ["Update socket directory", "Owner-only relay directory on this machine", "/home/operator/private/robot-update"],
    profilePath: ["Transport profile", "Absolute host-owned profile JSON", "/home/operator/private/robot-debug.json"],
    framePath: ["Signed frame", "Absolute path on the VM host", "/home/operator/updates/frame.uup1"]
  };
  function fields() {
    const data = {mode: draft.mode || view.mode};
    for (const key of Object.keys(LABELS)) data[key] = draft[key] !== undefined ? draft[key] : view.fields[key];
    return data;
  }
  function draw() {
    const mode = draft.mode || view.mode, disabled = view.busy || view.picking;
    const cards = h("div.radio-cards", {role: "radiogroup", "aria-label": "Transport"}, Object.entries(MODES).map(([key, item]) => {
      const unavailable = key === "session" && !view.sessionHasUpdate || key === "local" && view.platform !== "linux";
      return h("div.radio-card", {role: "radio", tabindex: "0", "aria-checked": String(mode === key), "aria-disabled": unavailable ? "true" : undefined,
        title: unavailable ? (key === "session" ? "The active debug session has no updateSocketDir." : "The local relay requires Linux; use a profile.") : "",
        style: unavailable ? {opacity: ".55"} : undefined,
        on: {click: () => { if (!disabled) { draft.mode = key; draw(); } }, keydown: event => { if (event.key === " " || event.key === "Enter") { event.preventDefault(); draft.mode = key; draw(); } }}},
        icon(item.icon || "circle-small"), h("div", h("div.t", item.title), h("div.d", item.text)));
    }));
    const inputs = MODES[mode].fields.map(key => {
      const [label, hint, placeholder] = LABELS[key];
      const value = draft[key] !== undefined ? draft[key] : view.fields[key];
      return h("label.field.stack", h("span", label),
        h("div.row", h("input.mono", {name: key, value, placeholder, disabled: disabled || undefined, spellcheck: "false",
          on: {input: event => { draft[key] = event.target.value; }}}),
          button("Browse…", () => post({action: "browse", field: key, fields: fields()}), {small: true, secondary: true, disabled})),
        h("span.hint", hint));
    });
    const reply = view.reply;
    app.replaceChildren(...[
      h("div.titlebar", h("div.crumbs", action("arrow-left","Back (Alt+Left)",()=>post({action:"navigate-back"}),{disabled:!navigation.back}), action("arrow-right","Forward (Alt+Right)",()=>post({action:"navigate-forward"}),{disabled:!navigation.forward}), icon("cloud-upload"), h("span.title", "Hot update"), view.windowId > 1 ? h("span.crumb", "#" + view.windowId) : null,
        h("span.sep", "›"), h("span.crumb", "independent of debugger permission")),
        h("div.actions", action("split-horizontal", "Open another update window", () => post({action: "new-window"})))),
      view.error ? h("div.banners", h("div.banner.error", {role: "alert"}, icon("error"), h("div.text", view.error))) : null,
      view.busy ? h("div.banners", h("div.banner.running", {role: "status"}, icon("sync", "spin"), h("div.text", "Waiting for the coordinator… The request is not retried."))) : null,
      h("div.pane.padded",
        h("form.form", {on: {submit: event => { event.preventDefault(); if (!disabled) post({action: "submit", fields: fields()}); }}},
          h("div.section", h("h2", "Transport"), cards),
          h("div.section", h("h2", "Paths"), inputs),
          h("div", button("Submit once", null, {submit: true, icon: "cloud-upload", disabled}),
            h("p.hint", {style: {color: "var(--muted)", marginTop: "8px"}}, "Only an existing signed frame is sent. The coordinator decides; this window never signs, commits or retries."))),
        reply ? h("div.section", {style: {marginTop: "20px"}},
          h("h2", icon(reply.status === "ok" || reply.status === "accepted" ? "pass" : "info"), "Coordinator reply", h("span.sub", reply.status)),
          h("p", "A reply is not a commit. Check the status and phase before the next deployment step."),
          tree(reply, "$reply", {expandDepth: 1})) : null)].filter(Boolean));
  }
  addEventListener("message", event => {
    const message = event.data; if (!message) return;
    if (message.action === "editorStyle") { applyEditorStyle(message); return; }
    if (message.action === "navigation") {navigation=message;draw();return;}
    if (message.action === "view" && message.view && message.view.epoch >= view.epoch) {
      view = message.view;
      for (const key of Object.keys(draft)) if (key !== "mode" && view.fields[key] === draft[key]) delete draft[key];
      if (draft.mode === view.mode) delete draft.mode;
      draw();
    }
  });
  addEventListener("keydown", event => {
    if (event.altKey && !event.ctrlKey && !event.metaKey && ["ArrowLeft", "ArrowRight"].includes(event.key) &&
        !event.target.closest("input,textarea,[contenteditable=true]")) {
      event.preventDefault(); event.stopPropagation(); post({action:event.key === "ArrowLeft" ? "navigate-back" : "navigate-forward"});
    }
  });
  draw();
  vscode.postMessage({action: "ready"});
})();
