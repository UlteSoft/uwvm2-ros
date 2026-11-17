"use strict";
const path = require("path");
const nativeHistory = new WeakMap();
async function reveal(vscode, session, location, {preserveFocus = false, isCurrent = require("./navigation_state").begin(session)} = {}) {
  if (vscode.debug.activeDebugSession !== session) throw new Error("Select this window's debug session before navigating.");
  const live = () => isCurrent() && vscode.debug.activeDebugSession === session;
  if (!live()) return;
  if (location.level === "native") {
    if (!location.current || !/^uwvm-native-(stop|code):[1-9][0-9]*$/.test(location.instructionReference || ""))
      throw new Error("No current owned JIT instruction is available for navigation.");
    await session.customRequest("uwvm/disassemblyView", {visible: true});
    if (!live()) return;
    await vscode.commands.executeCommand("debug.action.openDisassemblyView");
    // VS Code paints a focused (selected) disassembly frame green.  The
    // authenticated native stop is always the current LLVM-JIT instruction,
    // so move the workbench selection back to the top frame when the command
    // is available; the extension's own exact decoration remains the fallback
    // for hosts that do not expose this command.
    if (live()) try { await vscode.commands.executeCommand("workbench.action.debug.callStackTop"); } catch (_) { }
    const history = require("./navigation_history").forSession(session);
    if (live() && history) {
      let entry = nativeHistory.get(session);
      if (!entry) {
        entry = history.register(session, async (saved, valid) => {
          await session.customRequest("disassemble", {memoryReference:saved.instructionReference, instructionCount:1});
          if (valid()) await reveal(vscode, session, saved, {isCurrent:valid});
        }); nativeHistory.set(session, entry);
        // VS Code's built-in disassembly tab has no public TabInput type.
        // Identify the tab actually opened above, rather than matching a
        // translated hard-coded title or treating arbitrary editors as ASM.
        const tab = vscode.window?.tabGroups?.activeTabGroup?.activeTab;
        if (tab && tab.input === undefined) history.trackTab(session, entry,
          candidate => candidate?.input === undefined && candidate?.label === tab.label,
          () => session.customRequest("uwvm/codeLocation", {level:"native"}));
      }
      entry.visit({level:"native",current:true,instructionReference:location.instructionReference});
    }
    return;
  }
  if (!location.source || typeof location.source.path !== "string" || !Number.isSafeInteger(location.line) || location.line < 1)
    throw new Error("No exact code location is available.");
  if (location.level === "wasm") {
    await vscode.commands.executeCommand("uwvm.openWasmInstructions", {...location, sessionId: session.id, preserveFocus, isCurrent}); return;
  }
  if (location.level !== "source" || !path.isAbsolute(location.source.path)) throw new Error("A mapped source file is required.");
  const column = Number.isSafeInteger(location.column) && location.column > 0 ? location.column - 1 : 0;
  const document = await vscode.workspace.openTextDocument(vscode.Uri.file(location.source.path));
  if (!live()) return;
  if (location.line > document.lineCount) throw new Error("The local source is shorter than its debug information.");
  const position = new vscode.Position(location.line - 1, Math.min(column, document.lineAt(location.line - 1).text.length));
  const editor = await vscode.window.showTextDocument(document, {preview: true, preserveFocus, viewColumn: vscode.ViewColumn.One, selection: new vscode.Range(position, position)});
  if (live()) editor.revealRange(new vscode.Range(position, position), vscode.TextEditorRevealType.InCenterIfOutsideViewport);
}
module.exports = {reveal};
