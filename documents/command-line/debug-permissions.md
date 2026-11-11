# Debugger permissions

`--debug-permissions full|read-only|limited|none` belongs to the `debug` command-line category. The host selects it at process launch. It is immutable through debugger commands, module replacement, runtime reset and reconnect. The former `--management-profile`, `dev` and `release` spellings are removed.

| Selection | Inspect sensitive Guest state | Modify Guest state or code | Manual dump |
| --- | --- | --- | --- |
| `full` | Yes, within the owned Wasm/JIT boundary | Yes, subject to each operation's safety and signature checks | All available captured state |
| `read-only` | Yes, including memory, variables and authorized WASI/ROS diagnostic views | No pause/resume, writes, stepping, execution relocation, breakpoint installation, restart, checkpoint restore or replacement | All available captured state |
| `limited` | No; only execution phase, stop identifier, numeric Wasm participants/backtraces and classified terminal status | No | Explicitly redacted, non-sensitive diagnostic metadata |
| `none` | No debugger or diagnostic endpoint | No | No |

Without an explicit selection, `-Rdbg` / `--runtime-debug` and `-m debug` / `--mode debug` select `full`; other launches select `limited`. An explicit selection wins regardless of argument order. For example, `-Rdbg --debug-permissions limited` does not authorize interactive inspection. Build optimization mode and debugger permission are independent: an optimized deployment may explicitly select `full` or `read-only`.

These permissions do **not** grant or revoke WASIp1, ROS1, ROS2, sockets, trusted native drivers, or signed production update authority. Those facilities retain their independent launch policy. An independently authenticated debugger and signed update coordinator can coexist through distinct sealed endpoints. Update credentials, signed scope and real execution-lease drainage remain mandatory in every debugger permission level. While the production coordinator is configured, raw debugger replacement is refused even with `full`.

The removed `rebuild` and `rebuild-module` debugger commands are rejected even
with `full`. Whole-module deployment requires the independent signed update
coordinator. A terminal fault freezes state/code mutation, execution relocation,
scripted mutation and replacement before dispatch; continuing a terminal stop
only completes the original termination. It never preserves or migrates the
faulted epoch into a resumed continuation.

## Observation and fault handling

A live debugger still requires its authenticated, inherited host control FD/HANDLE. Guest stdio, network sockets and Wasm imports cannot obtain this authority. `none` refuses both debugger and diagnostic endpoints before they are adopted. Ordinary launches without a selected endpoint do not install debugger probes merely because their default permission is `limited`.

`read-only` and `limited` reject pause, resume/continue (including a terminal-fault continuation), stepping, restart, state/code mutation and trace configuration changes. Only `full` controls execution. An observer may inspect a stop created independently by the launcher or explicit fault policy and request an authorized dump. Closing an observer connection or its local input does not resume, cancel or terminate the Guest; existing execution owners and stops remain for the supervisor. A full-permission terminal continuation completes the original fault termination and never retries the faulting instruction.

`--debug-fault-action pause` is a separate crash policy. It never creates a dump automatically or upgrades debugger permissions. With a debugger endpoint selected at launch, a terminal Wasm fault retains a real cooperative Guest stop while the host management channel remains usable. Without that endpoint, the existing process-wide stop requires the deployer to manage the stopped process externally.

On an authenticated debugger channel:

```text
permissions
status
dump wasm STOP_ID /private/path/fault.wasm
```

The dump command requires the exact current, complete cooperative stop and leaves it paused. Output is created exclusively with mode `0600`; existing files and symlinks are refused. Full/read-only dumps preserve capture-completeness metadata: unavailable owner state is not fabricated. A `limited` dump omits Guest memory, locals, globals, source, paths, tag values, payloads and native addresses, and records its redaction in `uwvm.core.diagnostic`. It is a diagnostic artifact, not an executable restore image. A read-only or limited dump never grants restore authority.

The separate `--diagnostic-control-fd` service remains a bounded non-sensitive metadata service; provision the debugger endpoint when a cooperative fault dump is required. Changing permissions after launch is deliberately unsupported.

## IDEs and remote use

The shared `tools/debug/import_ide.py` profile accepts `debugPermissions` with the same four values. It propagates the value to VS Code/Cursor, Neovim, JetBrains/LSP4IJ, NetBeans, generic DAP and local/SSH launch arguments. Conflicting profile/CLI selections are rejected. Without an explicit permission selection, `-Rdbg` and `-m debug` use full debugger access; other modes use limited access. The importer preserves this VM default instead of injecting an inferred permission grant. Counted WASI `force-args` payloads and arguments after the actual run delimiter are guest data, including strings such as `-Rdbg` or `--debug-permissions`. A `none` profile generates no debugger attach configuration or debugger descriptor. If an independent `updateSocketDir` is configured, it launches an update-only broker; that endpoint grants no debugging authority.

The adapter queries `permissions` from the authenticated VM before attachment. A configured expected level must match; the IDE cannot elevate it. DAP initially advertises conservative capabilities, then sends a `capabilities` event for the actual level. VM command admission remains the security boundary, including REPL and status-wrapped requests.

Custom DAP requests are `uwvm/permissions`, `uwvm/diagnostics`, and `uwvm/dump` (`stopId` plus an absolute `path` on the VM host). Source/Wasm/assembly views and sensitive WASI/ROS views are available only with full or read-only inspection permission and the corresponding genuine stopped-state evidence. Limited stacks contain actual numeric Wasm module/function/offset coordinates; unavailable offsets are labeled explicitly. They contain no source names, paths, memory references, values or native addresses. Remote configurations use the same permissions and authenticated transport; paths for dump output refer to the VM host.

The Code/Cursor extension provides separate Memory, Registers, Memory Tags,
WASIp1 and ROS windows. The memory window pins writes to its displayed stop;
retired pages cannot write a later stop. Read-only windows omit write controls;
limited/none windows explain the denied operation without requesting sensitive
state. See [GUI integration and client coverage](../../tools/debug/README.gui.md).

`--debug-fault-action exit` selects stable process exit for an unhandled Guest fault; omission retains crash termination. This launch policy is independent of all four debugger permission levels and never suppresses internal VM invariant failures.
