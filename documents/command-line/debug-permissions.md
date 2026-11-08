# Debugger permissions

`--debug-permissions full|read-only|limited|none` belongs to the `debug` command-line category. The host selects it at process launch. It is immutable through debugger commands, module replacement, runtime reset and reconnect. The former `--management-profile`, `dev` and `release` spellings are removed.

| Selection | Inspect sensitive Guest state | Modify Guest state or code | Manual dump |
| --- | --- | --- | --- |
| `full` | Yes, within the owned Wasm/JIT boundary | Yes, subject to each operation's safety and signature checks | All available captured state |
| `read-only` | Yes, including memory, variables and authorized WASI/ROS diagnostic views | No writes, stepping, execution relocation, breakpoint installation, restart, checkpoint restore or replacement | All available captured state |
| `limited` | No; only execution phase, stop identifier, participant count and classified terminal status | No | Explicitly redacted, non-sensitive diagnostic metadata |
| `none` | No debugger or diagnostic endpoint | No | No |

Without an explicit selection, `-Rdbg` (alias `--runtime-debug`) selects `full`; other launches select `limited`. An explicit selection wins regardless of argument order. For example, `-Rdbg --debug-permissions limited` does not authorize interactive inspection. Build optimization mode and debugger permission are independent: an optimized deployment may explicitly select `full` or `read-only`.

These permissions do **not** grant or revoke WASIp1, ROS1, ROS2, sockets, trusted native drivers, or signed production update authority. Those facilities retain their independent launch policy. Actual debugger and update coordinators currently cannot simultaneously own the same runtime control domain; choosing `full` without starting a debugger does not prevent a separately authorized update coordinator.

## Observation and fault handling

A live debugger still requires its authenticated, inherited host control FD/HANDLE. Guest stdio, network sockets and Wasm imports cannot obtain this authority. `none` refuses both debugger and diagnostic endpoints before they are adopted. Ordinary launches without a selected endpoint do not install debugger probes merely because their default permission is `limited`.

`read-only` allows cooperative pause/resume for observation; these do not edit Guest values or instructions. Stepping and all state/code mutation requests are rejected in the VM as well as the DAP adapter. A terminal-fault continuation completes the original fault termination; it never retries the faulting instruction. `limited` only permits continuation of that terminal stop.

`--debug pause` is a separate crash policy. It never creates a dump automatically or upgrades debugger permissions. With a debugger endpoint selected at launch, a terminal Wasm fault retains a real cooperative Guest stop while the host management channel remains usable. Without that endpoint, the existing process-wide stop requires the deployer to manage the stopped process externally.

On an authenticated debugger channel:

```text
permissions
status
dump wasm STOP_ID /private/path/fault.wasm
```

The dump command requires the exact current, complete cooperative stop and leaves it paused. Output is created exclusively with mode `0600`; existing files and symlinks are refused. Full/read-only dumps preserve capture-completeness metadata: unavailable owner state is not fabricated. A `limited` dump omits Guest memory, locals, globals, source, paths, tag values, payloads and native addresses, and records its redaction in `uwvm.core.diagnostic`. It is a diagnostic artifact, not an executable restore image. A read-only or limited dump never grants restore authority.

The separate `--diagnostic-control-fd` service remains a bounded non-sensitive metadata service; provision the debugger endpoint when a cooperative fault dump is required. Changing permissions after launch is deliberately unsupported.

## IDEs and remote use

The shared `tools/debug/import_ide.py` profile accepts `debugPermissions` with the same four values. It propagates the value to VS Code/Cursor, Neovim, JetBrains/LSP4IJ, NetBeans, generic DAP and local/SSH launch arguments. Conflicting profile/CLI selections are rejected. If only `-Rdbg` was specified, the result remains full debugger access. A `none` profile generates no attach configurations and launches without a broker or control descriptor.

The adapter queries `permissions` from the authenticated VM before attachment. A configured expected level must match; the IDE cannot elevate it. DAP initially advertises conservative capabilities, then sends a `capabilities` event for the actual level. VM command admission remains the security boundary, including REPL and status-wrapped requests.

Custom DAP requests are `uwvm/permissions`, `uwvm/diagnostics`, and `uwvm/dump` (`stopId` plus an absolute `path` on the VM host). Source/Wasm/assembly views and sensitive WASI/ROS views are available only with full or read-only inspection permission and the corresponding genuine stopped-state evidence. Limited diagnostics contain no synthetic stack frames or memory references. Remote configurations use the same permissions and authenticated transport; paths for dump output refer to the VM host.
