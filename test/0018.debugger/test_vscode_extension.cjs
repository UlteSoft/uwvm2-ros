"use strict";

const assert = require("assert");
const fs = require("fs");
const path = require("path");
const vm = require("vm");

const extensionPath = path.resolve(__dirname, "../../tools/debug");
let factory, provider, terminated, memoryTags;
class DebugAdapterExecutable {
  constructor(command, args, options) { Object.assign(this, {command, args, options}); }
}
const vscode = {
  DebugAdapterExecutable,
  commands: {registerCommand(name, callback) { assert.ok(["uwvm.showMemoryTags","uwvm.showDiagnostics","uwvm.dump"].includes(name));if(name==="uwvm.showMemoryTags")memoryTags=callback;return {dispose(){}}; }},
  workspace: {registerTextDocumentContentProvider(scheme, value) {
    assert.strictEqual(scheme, "uwvm-wasm"); // Never override debug:/file:/other providers.
    provider = value; return {dispose() {}};
  }},
  debug: {
    onDidTerminateDebugSession(callback) { terminated = callback; return {dispose() {}}; },
    registerDebugAdapterDescriptorFactory(type, value) {
      assert.strictEqual(type, "uwvm-llvm-full");
      factory = value;
      return {dispose() {}};
    }
  }
};
const sandbox = {
  module: {exports: {}}, process,
  require(name) { return name === "vscode" ? vscode : require(name); }
};
vm.runInNewContext(fs.readFileSync(path.join(extensionPath, "extension.js"), "utf8"), sandbox);
const context = {extensionPath, subscriptions: []};
sandbox.module.exports.activate(context);
assert.strictEqual(context.subscriptions.length, 8);
const id = "11223344-5566-7788-99aa-bbccddeeff00";
const requests = [];
const session = {id, type: "uwvm-llvm-full",
  configuration: {pythonPath: "/trusted/python3", capability: "private-host-token"},
  async customRequest(command, args) { requests.push({command, args}); return {content: ";; 4 local.get 0\n", mimeType: "text/x-wat"}; }
};
const descriptor = factory.createDebugAdapterDescriptor(session);
assert.strictEqual(descriptor.command, "/trusted/python3");
assert.deepStrictEqual(Array.from(descriptor.args), [path.join(extensionPath, "dap_adapter.py")]);
assert.strictEqual(descriptor.options.env.UWVM_DAP_SOURCE_SESSION, id);
const remoteDescriptor = factory.createDebugAdapterDescriptor({...session, configuration: {transportProfile: "/private/project.json"}});
assert.deepStrictEqual(Array.from(remoteDescriptor.args), [path.join(extensionPath, "import_ide.py"), "adapter", "--profile", "/private/project.json"]);
for (const transportProfile of ["relative.json", 1, {}, "\\n"]) assert.throws(() => factory.createDebugAdapterDescriptor({...session, configuration: {transportProfile}}), /transportProfile/);
assert.ok(!JSON.stringify(descriptor).includes("private-host-token"));
for (const stepLevel of ["source", "wasm", "native"]) {
  const selected = factory.createDebugAdapterDescriptor({...session, configuration: {stepLevel}});
  assert.deepStrictEqual(Array.from(selected.args), [path.join(extensionPath, "dap_adapter.py")]);
}
for (const stepLevel of [null, true, 1, [], {}, "unknown"]) {
  assert.throws(() => factory.createDebugAdapterDescriptor({...session, configuration: {stepLevel}}), /stepLevel/);
}
for (const bad of [undefined, "file:///host", "../session", "11223344-5566-7788-99aa-bbccddeeff00\n"]) {
  assert.throws(() => factory.createDebugAdapterDescriptor({...session, id: bad}), /session identity/);
}
const config = JSON.parse(fs.readFileSync(path.join(extensionPath, "package.json"), "utf8"));
const registered = config.contributes.debuggers[0];
assert.deepStrictEqual(registered.configurationAttributes.attach.properties.stepLevel.enum, ["source", "wasm", "native"]);
assert.strictEqual(registered.configurationAttributes.attach.properties.stepLevel.default, "source");
assert.ok(registered.configurationSnippets.every(item => item.body.stepLevel === "source"));
const uri = (changes={}) => ({
  scheme: "uwvm-wasm", authority: "", fragment: "",
  path: "/module-0/function-6/stop-13.wat", query: "session="+id+"&ref=5",
  toString() { return this.scheme+":"+this.path+"?"+this.query; }, ...changes
});
(async () => {
  factory.createDebugAdapterDescriptor(session);
  assert.strictEqual(await provider.provideTextDocumentContent(uri()), ";; 4 local.get 0\n");
  assert.strictEqual(requests.length, 1);
  assert.strictEqual(requests[0].command, "source");
  assert.strictEqual(requests[0].args.sourceReference, 5);
  assert.strictEqual(requests[0].args.source.path, uri().toString());
  const invalid = [
    {scheme: "debug"}, {scheme: "file"}, {authority: "another-host"}, {fragment: "suffix"},
    {path: "/etc/passwd"}, {path: "/module-0/../function-6/stop-13.wat"},
    {query: "session="+id+"&ref=0"}, {query: "session="+id+"&ref=-1"},
    {query: "session="+id+"&ref=9007199254740992"},
    {query: "session="+id+"&ref=5&ref=6"}, {query: "ref=5&session="+id},
    {query: "session=ffffffff-ffff-ffff-ffff-ffffffffffff&ref=5"}
  ];
  for (const value of invalid) await assert.rejects(provider.provideTextDocumentContent(uri(value)));
  await assert.rejects(provider.provideTextDocumentContent(uri(), {isCancellationRequested: true}));
  assert.strictEqual(requests.length, 1, "invalid paths/sessions cannot issue DAP requests");
  terminated(session);
  await assert.rejects(provider.provideTextDocumentContent(uri()));
  assert.strictEqual(requests.length, 1);
  const pending = {...session, async customRequest() { terminated(session); return {content: "old", mimeType: "text/x-wat"}; }};
  factory.createDebugAdapterDescriptor(pending);
  await assert.rejects(provider.provideTextDocumentContent(uri()), /retired during query/);
  let preview, shown=0, decorated=[];
  vscode.Range=class {constructor(...args) {this.args=args;}};
  vscode.workspace.openTextDocument=async options => {preview=options;return {uri:"untitled:memory-tags"};};
  vscode.window={async showTextDocument() {++shown;return {setDecorations(type,ranges) {decorated.push({type,ranges});}};},
    createTextEditorDecorationType(options) {return {...options,dispose(){}};}};
  const memorySession={...session,async customRequest(command,args) {
    assert.strictEqual(command,"uwvm/memoryTags");assert.strictEqual(args.memoryReference,"wasm-memory:2:1:15");
    return {memoryReference:args.memoryReference,offset:"15",count:2,stopId:"41",status:"available",data:"QUI=",
      tags:[{offset:"0",tag:0,hex:"0x00",color:"#ff4040"},{offset:"16",tag:1,hex:"0x01",color:"#40d7ff"}]};}};
  vscode.debug.activeDebugSession=memorySession;
  await assert.rejects(memoryTags("2 1 15 2"),/live UWVM/);
  factory.createDebugAdapterDescriptor(memorySession);
  for (const input of ["0 0 0 257","0 0 18446744073709551615 1","0 4294967296 0 1","$rsp","0 0 -1 1",{}]) {
    await assert.rejects(memoryTags(input));
  }
  assert.strictEqual(shown,0);
  assert.strictEqual(await memoryTags("2 1 15 2"),"untitled:memory-tags");
  assert.ok(preview.content.includes("stop 41"));assert.ok(preview.content.includes("tag=0x00  41"));
  assert.ok(preview.content.includes("tag=0x01  42"));assert.strictEqual(decorated.length,2);
  terminated(memorySession);await assert.rejects(memoryTags("2 1 15 2"),/live UWVM/);
  console.log("PASS VS Code levels, private URI provider, session/reference boundaries, cancellation and retirement");
})().catch(error => { console.error(error); process.exitCode = 1; });

for (const debugPermissions of ["full", "read-only", "limited"]) factory.createDebugAdapterDescriptor({...session, id:"abcdefab-1234-1234-1234-123456789abc", configuration:{debugPermissions}});
assert.throws(() => factory.createDebugAdapterDescriptor({...session, id:"abcdefab-1234-1234-1234-123456789abc", configuration:{debugPermissions:"none"}}), /Debugger disabled/);
for (const debugPermissions of ["dev", "release", true, 1]) assert.throws(() => factory.createDebugAdapterDescriptor({...session, id:"abcdefab-1234-1234-1234-123456789abc", configuration:{debugPermissions}}), /debugPermissions/);
