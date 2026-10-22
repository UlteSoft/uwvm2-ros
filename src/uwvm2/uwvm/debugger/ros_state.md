# Read-only ROS Host diagnostics

These commands use the existing authenticated debugger controller. They do not
add a Guest import, ROS permission, native client, remote debugger listener or
network operation. The controller must own its **current complete cooperative
stop**. A running/stopping process, stale stop, exited owner or native
instruction stop cannot authorize the observation callback. The controller
checks the cooperative capture before and after the callback.

```
info ros capabilities
info ros sessions backend=ros1
info ros queues backend=ros2
info ros services module=client.wasm
info ros actions alias=feedback
info ros parameters
info ros tf
info ros graph
info ros stats first=0 count=8
trace ros backend=ros2 after=0 count=16 error=13
trace ros function=service_submit op=service.call
info ros stats if-stop=19
```

The existing debugger launch/authentication setup is still required. These are
console commands, not shell commands or a way to attach to an arbitrary process.
Help is available through `help ros`, `help info ros` and `help trace ros`.
Completion lists static selectors/options; it does not query native ROS.

## Identity and scope

Each response states `scope=process-owned`. The authenticated process owner can
inspect the process's registered ROS Host metadata, including other registered
module names. `instance` is the existing public capability-instance label;
`generation` is the actual registry lifetime generation. Neither is a numeric
Wasm module ID. No registry ordinal is presented as a module ID. `module=` is
an exact name filter and the response prints the actual registered name.

Module and alias display text is bounded to 255 bytes, operation/function text
to 64 bytes, resource text to 256 bytes and type text to 192 bytes. Truncation is
explicit. A truncated stored module name cannot satisfy an exact trace/session
name filter by matching its prefix. Capability filtering compares the complete
registered name before bounding the detached output. Names longer than the
filter limit can be inspected through unfiltered pages, with truncation shown.

`capabilities` shows configured aliases, operation/type/name metadata, enabled,
alive/revoked state, current open/active-handle counts, **stored** quota tokens
and retained audit counts. It does not open handles, refresh/debit quotas,
append audit events or drain audit bodies. `services`, `actions`, `parameters`,
`tf` and `graph` filter this same registry by operation family. They describe
Host policy/state, not a native service result, action result, parameter value,
TF value or remotely refreshed ROS graph.

`sessions` describes local Host connection state, timeout and observer count.
It does not display IPC descriptors, session credentials, connection strings or
security roots. `queues` describes existing local unified-event observers:
source operation/type, observer alias, retired state, tracked request/delivery
counts and allocation-free queue statistics. Queue statistics include depth,
bytes, pending/reserved depth/bytes, high-water marks, enqueue/dequeue/drop,
rejection/backpressure/admission-refusal, completion and close counters.
`stats` includes capability, session and local observer-queue rows. A nonempty
`alias=` filter selects alias-bearing rows, so it excludes session-only rows.
These counters are local Host/stream counters; they do not measure total DDS,
ROS client-library, kernel or network memory. Legacy native Source queues are
not remotely queried by this command.

Only ROS-name-shaped resource metadata is displayed. Other resource forms are
redacted. Payloads, parameter values, audit bodies, cookies/tokens, native
pointers, raw handles, descriptors, security URIs and native object addresses
are absent from the diagnostic DTO. Existing debugger authority remains
powerful process-owner authority; this display is not a new tenant boundary.

## Bounds and incomplete observation

`info ros` accepts `backend=all|ros1|ros2`, `instance=U64`, `module=NAME`,
`alias=NAME`, `first=0..4096` and `count=1..8`. Both commands accept an optional
nonzero `if-stop=U64` assertion; an actual previous stop label is refused
without invoking the observation provider. `trace ros` accepts the same
backend/instance/module/alias filters plus `after=U64`, `count=1..16`,
`error=U32`, `function=NAME` and `op=NAME`. Trace has no `first`; state queries
have no trace-only filters. Duplicate/unknown options, empty values, overflow,
mutation options and overlong text refuse. The existing 512-byte command cap
also applies, so all individually bounded filters need not fit simultaneously.

Registry/backend/queue/trace locks are acquired with `try_lock`. `BUSY` means
the snapshot is unavailable and **does not mean empty**. Partial rows are
discarded on a registry/queue lock failure or bounded scan limit. Registry and
Host scans are each bounded to 4096 examined items; a larger registry yields an
explicit scan-limit result instead of unbounded work. A successful snapshot is
detached; separate registry/Host views are not a globally atomic native ROS
transaction. The stopped cooperative cohort is checked again before delivery.
Concurrent native callbacks can make local locks busy.

The fixed page sizes keep even maximally escaped metadata within the existing
65536-byte authenticated broker response cap. Nonprintable bytes, quotes and
backslashes are hex escaped before console output; metadata cannot inject a
terminal line. Output text is formatted with the existing fast_io console path.

## Trace semantics

Host calls append bounded function/operation/backend/errno and public identity
metadata to a 256-entry process-owned ring. This does not enable ROS rights or
change the original call's result/audit behavior. No native operation is issued
to enrich a trace. Failed identity lookup is marked `unresolved`; identity is
never guessed from a pointer or numeric module index. `abi_version` and denials
may have unresolved identities. The trace contains no request/response payload.

Reads do not consume, clear, enable or disable tracing. `after` is an exclusive
sequence cursor; `next` continues the page. `oldest`, `newest`, `overwritten`,
`gap` and `dropped_busy` distinguish overwritten history and nonblocking append
loss from a complete history. A busy ring drops only diagnostic metadata; it
does not block the original Host call. Sequence exhaustion also drops metadata.
`error=0` selects successful calls. Function and operation filters are exact.

ROS-disabled builds keep the independent DTO/parser/renderer and a null atomic
observation hook, returning `ROS Host not selected`. No native ROS headers or
libraries are needed for this path. The Host installs its read-only callback
only when a ROS backend is compiled. Existing ROS-enabled Host named-module
restrictions remain unchanged; the new standalone debugger module has a
separate ROS-off consumer probe.

## Verification status

The source increment includes the focused components documented in
[`test/0073.ros-tests/support/source/ros_debugger/README.md`](../../../../test/0073.ros-tests/support/source/ros_debugger/README.md).
Their presence is not a compilation, execution, Runtime authentication or
native ROS acceptance result. Root must run current-source components and
platform/backend integration serially and retain the actual reports before
claiming those results. Historical 48 GiB evidence remains historical; the new
execution plan uses the user's current 64 GiB/swap-zero requirement.

## Optional service-client observation

`service.available` is an independent explicit grant paired with one live
`service.call` source. `info ros services` already includes it by the exact
`service.` operation prefix; configured quota tokens have the same stored-only
meaning as other capabilities. No debugger query calls a ROS readiness API.

```
trace ros function=client.features op=service.available
trace ros function=client.service_available op=service.available alias=available
info ros sessions backend=ros2
```

Optional-client trace rows add the configured `source_alias` and a local
successful-connection ordinal `client_session_label`. This ordinal is **not**
the random native IPC session credential, a handle, a module ID or authorization.
It becomes0 if its diagnostic counter exhausts; diagnostics never reject an
otherwise permitted ROS call. A successful features return additionally shows
its actual validated `client_features` bitmask. Failed feature calls have no
feature-result claim. Session rows add the total lifetime `client_bindings`
(including retired tombstones), and that local session label, only after this
optional layer has actual bound rows. No hidden native query refreshes them.

Existing `op=`, `function=` and `alias=` filters remain exact and bounded; there
is no new management/Guest entry or implicit native session access. The new
source column uses the same escaped bounded text rendering; full trace pages
remain within the original65536-byte response bound. Default denial and raw
business-v1 imports remain unchanged. These additions are source-only pending
Root's current-source native/Wasm/debugger tests.
