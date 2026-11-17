"use strict";

// A normal VM exit can precede its background preLaunchTask's end event.
// F5 must not mistake that dying task for an already-ready broker. Only an
// observed exit of this profile qualifies; detach and pause never restart it.
function register(vscode, context) {
  if (!vscode.tasks || !vscode.debug.registerDebugConfigurationProvider || !vscode.debug.registerDebugAdapterTrackerFactory) return;
  const ended = new Map();
  const owned = (task, profile) => task?.name === "uwvm: host-owned broker" &&
    Array.isArray(task.execution?.args) && task.execution.args.length === 4 &&
    /(?:^|[/\\])import_ide\.py$/.test(task.execution.args[0]) &&
    task.execution.args[1] === "broker" && task.execution.args[2] === "--profile" && task.execution.args[3] === profile;
  context.subscriptions.push(vscode.debug.registerDebugAdapterTrackerFactory("uwvm-llvm-full", {
    createDebugAdapterTracker(session) { return {onDidSendMessage(packet) {
      const profile = session.configuration.transportProfile;
      if (packet.type === "event" && packet.event === "exited" && typeof profile === "string" &&
          session.configuration.preLaunchTask === "uwvm: host-owned broker") {
        ended.set(profile, (vscode.tasks.taskExecutions || []).filter(run => owned(run.task, profile)));
      }
    }}; }
  }));
  context.subscriptions.push(vscode.debug.registerDebugConfigurationProvider("uwvm-llvm-full", {
    async resolveDebugConfiguration(folder, configuration) {
      const profile = configuration.transportProfile, runs = ended.get(profile);
      if (!runs || configuration.preLaunchTask !== "uwvm: host-owned broker") return configuration;
      const remaining = new Set(runs.filter(run => (vscode.tasks.taskExecutions || []).includes(run)));
      if (remaining.size) await new Promise((resolve, reject) => {
        const listener = vscode.tasks.onDidEndTask(event => {
          remaining.delete(event.execution);
          if (!remaining.size) { clearTimeout(timer); listener.dispose(); resolve(); }
        });
        const timer = setTimeout(() => {
          listener.dispose();
          reject(new Error("The previous UWVM broker task is still shutting down. Wait for its task to finish before starting again."));
        }, 15000);
        // It may have finished between the snapshot above and subscription.
        for (const run of remaining) if (!(vscode.tasks.taskExecutions || []).includes(run)) remaining.delete(run);
        if (!remaining.size) { clearTimeout(timer); listener.dispose(); resolve(); }
      });
      if (ended.get(profile) === runs) ended.delete(profile);
      return configuration;
    }
  }));
}
module.exports = {register};
