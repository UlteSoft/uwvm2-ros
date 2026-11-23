"use strict";
const assert = require("assert");
const navigation = require("../../tools/debug/navigation_state");
const {reveal} = require("../../tools/debug/code_navigation");
(async () => {
  let release;
  const commands = [], session = {customRequest: () => new Promise(resolve => release = resolve)};
  const vscode = {debug: {activeDebugSession: session}, commands: {executeCommand: async command => commands.push(command)}};
  const isCurrent = navigation.begin(session);
  const pending = reveal(vscode, session, {level: "native", current: true, instructionReference: "uwvm-native-stop:2"}, {isCurrent});
  navigation.begin(session); // User selects W before the native query replies.
  release(); await pending;
  assert.deepStrictEqual(commands, [], "retired native reveals must not open Disassembly or focus the stack");
  session.customRequest = async () => {};
  await reveal(vscode, session, {level: "native", current: true, instructionReference: "uwvm-native-stop:2"});
  assert.deepStrictEqual(commands, ["debug.action.openDisassemblyView", "workbench.action.debug.callStackTop"]);
  commands.length = 0;
  await assert.rejects(reveal(vscode, session, {level: "native", current: true, instructionReference: "0x1234"}), /owned JIT/);
  assert.deepStrictEqual(commands, []);
  console.log("PASS delayed ASM reveal canceled; current opaque JIT navigation retained");
})().catch(error => {console.error(error); process.exitCode = 1;});
