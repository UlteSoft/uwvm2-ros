"use strict";
const assert = require("assert");
const {register} = require("../../tools/debug/runtime_launch");
(async () => {
  let factory, provider, finish;
  const configuration = {transportProfile: "/private/profile.json", preLaunchTask: "uwvm: host-owned broker"};
  const run = {task: {name: configuration.preLaunchTask, execution: {args: ["/extension/import_ide.py", "broker", "--profile", configuration.transportProfile]}}};
  const vscode = {tasks: {taskExecutions: [run], onDidEndTask: fn => (finish = fn, {dispose() {}})}, debug: {
    registerDebugAdapterTrackerFactory: (_, value) => (factory = value, {dispose() {}}),
    registerDebugConfigurationProvider: (_, value) => (provider = value, {dispose() {}})
  }};
  register(vscode, {subscriptions: []});
  const tracker = factory.createDebugAdapterTracker({configuration});
  tracker.onDidSendMessage({type: "event", event: "terminated"});
  assert.strictEqual(await provider.resolveDebugConfiguration(null, configuration), configuration, "detach does not restart the runtime");
  tracker.onDidSendMessage({type: "event", event: "exited"});
  let resolved = false;
  const next = provider.resolveDebugConfiguration(null, configuration).then(() => resolved = true);
  await new Promise(resolve => setImmediate(resolve));
  assert.strictEqual(resolved, false, "F5 must wait until the old task is gone");
  finish({execution: {task: run.task}});
  assert.strictEqual(resolved, false, "another task cannot release this wait");
  vscode.tasks.taskExecutions = []; finish({execution: run}); await next;
  assert.strictEqual(resolved, true);
  console.log("PASS explicit restart after observed exit waits for the owned task; detach/unrelated tasks unchanged");
})().catch(error => {console.error(error); process.exitCode = 1;});
