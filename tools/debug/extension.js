"use strict";

const path = require("path");
const vscode = require("vscode");
const {URLSearchParams} = require("url");
const {Buffer} = require("buffer");

function activate(context) {
  const sessions = new Map();
  const sessionPattern = /^[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}$/;
  // A separate scheme avoids replacing any IDE/other debugger source provider.
  // This provider never reads host files and retains no guest source contents.
  context.subscriptions.push(vscode.workspace.registerTextDocumentContentProvider(
    "uwvm-wasm", {async provideTextDocumentContent(uri, cancellation) {
      if (uri.scheme !== "uwvm-wasm" || uri.authority || uri.fragment ||
          !/^\/module-[0-9]+\/function-[0-9]+\/stop-[0-9]+\.wat$/.test(uri.path)) {
        throw new Error("Invalid UWVM Wasm document URI");
      }
      const query = new URLSearchParams(uri.query);
      const id = query.get("session"), rawReference = query.get("ref");
      if (!id || !sessionPattern.test(id) || !rawReference || !/^[1-9][0-9]*$/.test(rawReference) ||
          uri.query !== "session=" + id + "&ref=" + rawReference) {
        throw new Error("Invalid UWVM Wasm document session/reference");
      }
      const reference = Number(rawReference), session = sessions.get(id);
      if (!Number.isSafeInteger(reference) || !session || session.type !== "uwvm-llvm-full") {
        throw new Error("Unknown or retired UWVM Wasm document session");
      }
      if (cancellation && cancellation.isCancellationRequested) throw new Error("UWVM source request canceled");
      const reply = await session.customRequest("source", {
        sourceReference: reference, source: {path: uri.toString(true), sourceReference: reference}
      });
      if (sessions.get(id) !== session || (cancellation && cancellation.isCancellationRequested)) {
        throw new Error("UWVM source session retired during query");
      }
      if (!reply || typeof reply.content !== "string" || reply.mimeType !== "text/x-wat") {
        throw new Error("Invalid UWVM Wasm source response");
      }
      return reply.content;
    }}));
  const tagDecorations = new Set();
  context.subscriptions.push({dispose() { for (const item of tagDecorations) item.dispose(); tagDecorations.clear(); }});
  context.subscriptions.push(vscode.commands.registerCommand("uwvm.showMemoryTags", async selection => {
    const session = vscode.debug.activeDebugSession;
    if (!session || session.type !== "uwvm-llvm-full" || sessions.get(session.id) !== session) {
      throw new Error("Select a live UWVM debug session to inspect memory tags");
    }
    if (selection === undefined) selection = await vscode.window.showInputBox({
      prompt: "Wasm module, memory index, byte offset and page byte count (0..256)",
      placeHolder: "0 0 0 256", value: "0 0 0 256"
    });
    if (selection === undefined) return;
    const fields = typeof selection === "string" && /^([0-9]{1,20}) ([0-9]{1,10}) ([0-9]{1,20}) ([0-9]{1,3})$/.exec(selection);
    if (!fields || BigInt(fields[1]) >= 1n<<64n || BigInt(fields[2]) >= 1n<<32n ||
        BigInt(fields[3]) >= 1n<<64n || Number(fields[4]) > 256 || BigInt(fields[3])+BigInt(fields[4]) >= 1n<<64n) {
      throw new Error("Use decimal MODULE MEMORY OFFSET COUNT; COUNT is one 0..256-byte page");
    }
    const module = BigInt(fields[1]).toString(), memory = BigInt(fields[2]).toString();
    const offset = BigInt(fields[3]), count = Number(fields[4]);
    const reference = `wasm-memory:${module}:${memory}:${offset}`;
    const reply = await session.customRequest("uwvm/memoryTags", {memoryReference:reference,count,includeBytes:true});
    if (sessions.get(session.id) !== session) throw new Error("UWVM memory tag session retired during query");
    if (!reply || reply.memoryReference !== reference || reply.offset !== offset.toString() || reply.count !== count ||
        !/^[1-9][0-9]{0,19}$/.test(reply.stopId) || !["available","disabled","unsupported-memory"].includes(reply.status) ||
        !Array.isArray(reply.tags) || typeof reply.data !== "string") throw new Error("Invalid UWVM memory tag snapshot");
    const bytes=Buffer.from(reply.data,"base64");
    if (bytes.length !== count || bytes.toString("base64") !== reply.data) throw new Error("Invalid UWVM memory byte snapshot");
    const amount=count===0 ? 0 : Number((offset+BigInt(count)-1n)/16n-offset/16n+1n);
    if (reply.tags.length !== (reply.status === "available" ? amount : 0)) throw new Error("Incomplete UWVM allocation tag coverage");
    const lines=[`UWVM Wasm memory snapshot | stop ${reply.stopId} | module ${module} memory ${memory}`,
      `offset ${offset}, ${count} bytes | allocation tags: ${reply.status}`, "Run UWVM: Show Memory Tags again to refresh or choose another page.", ""];
    const decorations=[];
    for (let index=0;index<reply.tags.length;++index) {
      const row=reply.tags[index], first=(offset/16n+BigInt(index))*16n;
      if (!row || row.offset !== first.toString() || !Number.isInteger(row.tag) || row.tag<0 || row.tag>255 ||
          row.hex !== "0x"+row.tag.toString(16).padStart(2,"0") || !/^#[0-9a-f]{6}$/.test(row.color)) throw new Error("Invalid UWVM allocation tag row");
      const begin=Number(first<offset ? 0n : first-offset), end=Number(first+16n>offset+BigInt(count) ? BigInt(count) : first+16n-offset);
      const hex=bytes.subarray(begin,end).toString("hex").match(/../g) || [];
      lines.push(`0x${(offset+BigInt(begin)).toString(16)}  granule=0x${first.toString(16)}  tag=${row.hex}  ${hex.join(" ")}`);
      decorations.push({line:lines.length-1,color:row.color});
    }
    if (reply.status !== "available") lines.push("No allocation tags are available for this memory; zero tags have not been synthesized.");
    const document=await vscode.workspace.openTextDocument({content:lines.join("\n")+"\n",language:"plaintext"});
    const editor=await vscode.window.showTextDocument(document,{preview:true});
    for (const item of tagDecorations) item.dispose(); tagDecorations.clear();
    for (const row of decorations) {
      const type=vscode.window.createTextEditorDecorationType({color:row.color});tagDecorations.add(type);
      editor.setDecorations(type,[new vscode.Range(row.line,0,row.line,lines[row.line].length)]);
    }
    return document.uri;
  }));
  for (const command of ["uwvm.showDiagnostics", "uwvm.dump"]) {
    context.subscriptions.push(vscode.commands.registerCommand(command, async () => {
      const session = vscode.debug.activeDebugSession;
      if (!session || session.type !== "uwvm-llvm-full" || sessions.get(session.id) !== session)
        throw new Error("Select a live UWVM debug session");
      let reply;
      if (command === "uwvm.dump") {
        const raw = await vscode.window.showInputBox({prompt:"Current stop ID and absolute output path on the VM host", placeHolder:"1 /private/robot-fault.wasm"});
        if (raw === undefined) return;
        const match = /^([1-9][0-9]*) (\/[^\x00-\x1f]+)$/.exec(raw);
        if (!match || BigInt(match[1]) >= 1n<<64n) throw new Error("Enter a precise stop ID and absolute VM-host path");
        reply = await session.customRequest("uwvm/dump", {stopId:match[1],path:match[2]});
      } else reply = await session.customRequest("uwvm/diagnostics", {});
      if (sessions.get(session.id) !== session) throw new Error("UWVM session retired during diagnostics");
      const document = await vscode.workspace.openTextDocument({content:JSON.stringify(reply,null,2)+"\n",language:"json"});
      await vscode.window.showTextDocument(document,{preview:true});
    }));
  }
  context.subscriptions.push(vscode.debug.onDidTerminateDebugSession(session => sessions.delete(session.id)));
  context.subscriptions.push({dispose() { sessions.clear(); }});
  context.subscriptions.push(vscode.debug.registerDebugAdapterDescriptorFactory(
    "uwvm-llvm-full", {
      createDebugAdapterDescriptor(session) {
        const permissions = session.configuration.debugPermissions;
        if (permissions !== undefined && !["full", "read-only", "limited", "none"].includes(permissions)) {
          throw new Error("UWVM debugPermissions must be full, read-only, limited or none");
        }
        if (permissions === "none") throw new Error("Debugger disabled: run the generated runtime task without attaching");
        const level = session.configuration.stepLevel === undefined ? "source" : session.configuration.stepLevel;
        if (!["source", "wasm", "native"].includes(level)) {
          throw new Error("UWVM stepLevel must be source, wasm or native");
        }
        const python = session.configuration.pythonPath ||
          (process.platform === "win32" ? "python" : "python3");
        if (typeof python !== "string" || python.length === 0) {
          throw new Error("UWVM DAP requires a host Python 3 executable");
        }
        if (typeof session.id !== "string" || !sessionPattern.test(session.id)) {
          throw new Error("UWVM requires the host IDE session identity");
        }
        const profile = session.configuration.transportProfile;
        if (profile !== undefined && (typeof profile !== "string" || !path.isAbsolute(profile) || /[\x00-\x1f]/.test(profile))) {
          throw new Error("UWVM transportProfile must be an absolute host-owned profile path");
        }
        sessions.set(session.id, session);
        const adapterArguments = profile === undefined
          ? [path.join(context.extensionPath, "dap_adapter.py")]
          : [path.join(context.extensionPath, "import_ide.py"), "adapter", "--profile", profile];
        return new vscode.DebugAdapterExecutable(
          python, adapterArguments,
          {env: {UWVM_DAP_SOURCE_SESSION: session.id}});
      }
    }));
}

function deactivate() {}

module.exports = {activate, deactivate};
