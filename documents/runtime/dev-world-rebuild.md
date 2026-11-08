# DEV guest-world rebuild

The Linux authenticated debugger channel accepts `rebuild <path>` in a trusted
`--debug-permissions full` LLVM full launch. RELEASE cannot open that channel.
`rebuild-module <source-index> <path>` replaces either the main module (index 0)
or one preload (indices 1..N in trusted startup order). These source indices are
not debugger runtime module ordinals. An out-of-range index is refused before
retirement.

This operation reconstructs the guest world from a complete, fixed input image;
it is separate from the signed production-update protocol.

The path is the remainder of one UTF-8 SOCK_SEQPACKET command, without NUL.
The existing owned-file reader accepts a regular file and copies its complete
bytes before stopping the old world. The combined main/preload image budget is
1 GiB. Existing preload module names, feature policies and immutable bytes are
retained, except for the one selected source. `rebuild <path>` selects the main
module. A preload replacement retains the main module's already owned bytes,
not a second read of its startup path. The normal loader, import
visibility checks, full validator and compiler process the new world. Function,
type, global, memory, table, element and data layouts may change when accepted by
that ordinary pipeline. There is no reuse of an old immutable global slot as a
mutable configuration value.

The original authenticated socket, peer credentials and pidfd remain owned by
the launcher. Every command still authenticates the original peer. The old
controller, registered execution worker, native borrowers and runtime producers
must complete actual retirement and OS joins before source replacement. A
successful replacement creates a fresh controller and registered worker and
arms the initial pause. Breakpoints, stopped-reference handles, checkpoints and
compiled-source tokens from the old world do not authorize the new one.

Replies are one packet per command:

- `rebuild complete\n`: the old world has retired and the new world has been
  loaded and compiled, with its new worker armed to pause before execution.
- `rebuild pending\n`: cleanup has not finished within the current two-second
  slice. Send exactly `rebuild` to continue that same captured request. Other
  commands cannot resume the retiring world or replace its captured bytes.
- `error: rebuild input unavailable\n`: input capture failed before retirement.
- `error: rebuild unsupported for this launch\n`: the launch is outside the
  admitted profile described below.
- `rebuild failed; execution stopped\n`: failure after retirement; the launcher
  exits without restoring the old source or its authority. Disconnect while
  cleanup is pending exits 75 and does not claim that cleanup completed.

The host WASIp1 environments are retained in place. This includes the exact
module/group bindings, exposed imports, granted operations, descriptor table,
closed-descriptor tombstones, reduced rights, open-file offsets, argv/env, and
shared quota ledger. Only old guest-memory bindings are cleared and later
rebound to the new guest memory. Re-reading startup configuration to restore
rights, reopen descriptors or refill quotas is forbidden. New guest imports
still need the already approved exact namespace/function exposure and grants.

The first implementation admits non-threaded LLVM full worlds with owned input
and no configured or registered ROS provider, native DL/weak provider, or WSEA
secure execution. These combinations are explicitly refused; this command does
not claim ROS physical-authority migration or general checkpoint restoration.
A candidate that newly references a ROS provider is also refused before provider
registration. Existing WASIp1-only preloads are retained under the same names.

No production-update epoch, launch permit or durable supervisor decision is
created by this DEV command. It provides a fresh guest world under the existing
DEV management authorization and the same host contract.
