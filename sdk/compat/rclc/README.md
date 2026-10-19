# Opt-in rclc String, timer, executor and service source profiles

This candidate implements a finite **C source API adapter**, backed by the actual
existing roswasmc backend/session/owner/codec functions. It is not a native
rcl/rmw ABI replacement, full rclc, DDS waitset, full micro-ROS, rclcpp/rclpy/rcl
compatibility, or a native ROS acceptance result. It adds no import, Runtime
policy, debugger/update/control surface or grant. The earlier String foundation
compiled both complete official examples and passed ten local contract groups
with391 checks; its two earlier WASI artifacts validated with39 ROS2 imports.
Those results retain their earlier source identity. The newly integrated
[service profile](services/README.md) adds complete AddTwoInts/Trigger/SetBool
records, asynchronous client/server ownership and27 authored contract groups.
It has completed independent source review, but has not been compiled or run.
The combined successor and actual native delivery remain unqualified. Statements
about unexecuted assertions below describe their original source-only package;
they do not replace the separate actual receipts.

## Exact upstream business source and source lineage

The two complete business translation units are unchanged:

* `upstream/rclc_examples/src/example_executor.c` uses a String publisher,
  String subscription, autostart timer, executor and their original cleanup.
* `upstream/rclc_examples/src/example_sub_context.c` uses three String topics,
  independent publisher/subscription owners and context-bearing callbacks.

The official Jazzy rclc commit is
`45ef3de567588652c7f05017a91ea29921dd6862`. The retained CMake/package metadata,
LICENSE and NOTICE remain original. The external source-acquisition receipt
retains the actual immutable commit/tree/blob/source byte associations for all
related original rclc/rcl/rmw/rcutils/rosidl/common_interfaces research sources.
Those downloaded bytes were read as DATA only. The CMakeLists retained upstream
is historical source context, not the Guest build recipe.

Official primary source:
[complete executor example](https://github.com/ros2/rclc/blob/45ef3de567588652c7f05017a91ea29921dd6862/rclc_examples/src/example_executor.c),
[complete context example](https://github.com/ros2/rclc/blob/45ef3de567588652c7f05017a91ea29921dd6862/rclc_examples/src/example_sub_context.c),
[original executor implementation](https://github.com/ros2/rclc/blob/45ef3de567588652c7f05017a91ea29921dd6862/rclc/src/rclc/executor.c),
[original timer phase](https://github.com/ros2/rcl/blob/18d96c9c2b98e616324ae9a2c7701dd0b0886ff0/rcl/src/rcl/timer.c).

## Intentional opt-in and exact endpoint binding

Compile `src/compat.c` exactly once and put this `include` directory before the
normal SDK include directory for the selected Guest build only. Never combine
these private structs/objects with installed native rcl/rmw libraries. The
forwarding headers expose only the finite implemented declarations. An API
outside that set fails compilation/linking; it does not receive a success stub.

By default `uwvm_rclc_selected_configuration()` returns NULL: support_init refuses
before opening anything. A build may define
`UWVM_RCLC_COMPAT_SELECTION_HEADER` to one exact current, source-locked selected
header. `examples/selected_stdio_binding.h` uses the real ROS2 raw import backend.
Obtaining that backend or matching its source selection grants nothing. With
Host policy `[]`, the actual alias open must still fail. There is no ROS1
emulation or fallback backend.

Every node name, original namespace spelling, topic spelling, canonical type,
role and **all QoS profile fields** must match an explicit selection row. The
original default profile is KeepLast10/reliable/volatile, unspecified
zero deadline/lifespan, system-default liveliness/zero lease duration and normal
ROS naming conventions. Sensor-data calls use the real KeepLast5/best-effort
profile and only work if an explicit matching row is selected. Only default and sensor-data profiles are admitted in this first family; other
QoS profiles and argc>1 options are refused before effects. No remapping,
substitution, discovery or arbitrary native topic opening is inferred. Aliases
are nonempty, bounded, distinct across rows and distinct from the clock alias.
The type token is a Guest-local String codec token with NULL native metadata;
it is **not** an installed typesupport, native schema, RIHS or SDK proof.
Actual current Runtime/Gateway native type identity remains mandatory.

`policy.template.json` records the concrete four topics and their resolved
`/executor_examples/...` native endpoints plus exact roles/limits. It is DATA,
not an executable Host policy. The actual current CLI/policy association must be
constructed and reviewed separately; native ROS authority remains default empty.

## Lifetime, allocator, raw errors and cleanup

`rcl_ret_t` has the actual signed32-bit rmw type and original numeric values
(OK0, ERROR1, TIMEOUT2, UNSUPPORTED3, BAD_ALLOC10, INVALID_ARGUMENT11 and the
retained rcl-specific constants). ROS ABI errno13 is not rebranded as a new rcl
success or invalid-argument value: the API returns RCL_RET_ERROR and records
raw_primary13 separately in the preallocated context diagnostics. Poll idle11
is idle/time-out; native/codec errors are not silently retried. Error state is
thread-local, bounded, explicit-set/reset and returned with message/file/line;
no diagnostic formatting allocation participates in cleanup. Native rcutils
warning output and every advanced error-handling helper are not implemented.

The allocator callback/state shape, malloc/free/realloc/calloc default and
reallocf failure behavior follow actual rcutils. String owns a genuinely
allocated NUL-terminated buffer with size/capacity; scalar init/fini/assign/copy
use the actual default allocator lifecycle. Failed realloc keeps the old
storage. String wire encoding/decoding is the real canonical SDK codec, not a
native struct memcpy. The selected bound is a complete wire bound. The original
String may be longer locally; oversize/invalid UTF8/NUL is rejected before a
publish. The arbitrary unchecked native String mutation seen in the official
example remains unchanged business code; malformed ownership invariants are a
caller error, not a new allocator or granted capability.

A support/context, node, owner, message and executor must remain at its original
address until the matching fini. Copying does not clone authority. Init uses a
finite static address registry to avoid reading indeterminate publisher bytes
from the official malloc-created arrays and to reject live double init. At most
16 contexts/4096 local objects are registered. Each context preallocates256
stable SDK owner slots and per-acquisition close codes/counts before effects.
Closed owner slots are not reused in that context; capacity exhaustion refuses
before another host open. The selected backend/state must outlive all owners.
The caller allocator/state must also outlive every corresponding allocation.

Subscription activation failure immediately closes the acquired owner once,
retaining both primary and cleanup result. No allocation or list append is
needed after a successful raw open to remember that owner. Each explicit fini
attempt retires the owner, even on107/110. support_fini attempts all retained
owners in reverse order, skips already retired slots, and retains every code
and attempt count independently from the primary. A later fini failure cannot
replace an already recorded application/setup primary. Normal fini returns **its own
cleanup status**, not a previously handled application operation failure.
`uwvm_rclc_close_context` additionally accepts an explicit primary for a driver
that wants combined teardown. Native C callbacks/backend functions must return
normally; allocator/VM terminal traps, longjmp and unknown-completion host calls
need genuine Host instance draining. There is no terminal-trap immunity claim.

The official examples contain early error returns that omit their own fini.
Those source bytes are not rewritten. A normal source-adapter caller should
follow a common cleanup path with a still-live context; a terminal/early Guest
exit must be drained by the actual Runtime owner. No automatic code dereferences
an expired support stack frame after main returns.

## Bounded executor and independent time

The original rclc implementation uses a native DDS/rcl waitset. This adapter
instead calls an **explicit selected driver** once per spin_some, passes its
requested timeout as DATA, then reads real ROS event queues once and invokes
ready callbacks in registration order, first-read then-process. It allocates
only bounded executor handle/scratch storage before registration. It never
creates a native waitset, thread, clock, busy-loop sleep or timeout claim.

The example driver reads exactly one stdio byte: R permits one pass, Q requests
local shutdown, EOF or any other byte fails. Do not send newline-separated
bytes to this driver. Stdio pacing is not a ROS management channel. Its lifetime
must have an independent actual owner deadline. A finite maximum step count
bounds every spin loop; budget exhaustion returns TIMEOUT with raw110, not
fabricated successful shutdown. Per-source subscriber timeouts are not replayed.
ALWAYS callbacks receive NULL for idle; ON_NEW_DATA callbacks run only after a
complete validated message copy. Application callback errors/longjmp cannot be
translated into a silent successful delivery.

A whole-context dispatch lease is acquired before the selected driver runs and
held through all polls/callbacks. Every supported open/register/fini/shutdown
operation checks that lease before mutation. Recursive spins refuse. A callback
can publish using an existing retained publisher; it cannot destroy the same
context or the retained source group mid-pass. Only the driver's explicit125
response performs local shutdown from inside a pass.

Timer init reads a separately granted actual steady-clock alias. A successful
reply must have exactly8 bytes and fit signed nanoseconds. Missing or failed
clock grants are errors; WASI time is never a substitute. A fresh failed clock
setup immediately closes its owner. One actual clock sample is used per pass,
an explicit finite-profile scheduling choice. Timer phase advances from the
original due time by whole periods, coalesces missed periods, handles zero
period and rejects backwards time/overflow. It does not use `now+period` drift.
Cancel/reset during a borrowed dispatch are refused by this finite profile;
full native concurrent/reentrant timer behavior remains a required extension.

## Source contracts and actual qualification plan

`tests/contract.c` contains10 authored local model groups. It links this actual
adapter implementation and the two complete unchanged original TUs. Only the
model recipe renames main/global/callback symbols at compilation to avoid TU
collisions; the source bytes themselves stay exact. It covers full original
flows, no-selection/wrong-ABI zero effects, exact QoS matching, activation13 plus
cleanup107, whole-context lease refusal, malformed event handling, allocator
validation, timer phase/clock failure, owned String lifecycle and native-style
uninitialized publisher storage. The marker has an **actual** positive check
count, not a hardcoded authored-site acceptance count. These local bridge tests
are not native/Guest/official tests. They have not been executed here.

`recipes.json` names the exact source/include/selection boundaries. Root must
supply an actual qualified C11 compiler/WASI SDK, compile full-MD closures, inspect
actual memory/import signatures and link both original programs separately.
The actual products then need default-deny and explicit-alias Guest tests using
a current completed ROS2 Runtime/Gateway and native SDK. No current packet or
future artifact identity is invented. Native `native/peer.c` intentionally uses
**installed** rclc/rcl/generated String headers and libraries, never this adapter.
It materializes middleware/typesupport before SIGSTOP, then has finite I/M/P/F
input gates and a bounded subscriber poll count. A future real owner must prove
native ELF/SDK/full-MD/maps/namespace/birth/pidfd/deadline, retain raw logs, close
all native resources and verify the actual canonical value. The peer has no
native ABI/typesupport stubs or fake IDs. It has not been built/run.

All broader rcl/rclc/rmw/rcutils/rosidl APIs are required future compatibility
work: native waitsets and actual timeout semantics, trigger/LET configurations,
executor removal/guard conditions, clients/services/actions and their full native
graphs, lifecycle/parameters, all message families and sequences, native loaning,
init options/remapping, full error macros, contexts across threads, callback
mutation/concurrency, richer QoS and source-level logger APIs. `GAPS.json`
records them as implementation obligations. Nothing here declares full rclc,
rcl/rmw/native ABI or robot-vendor support complete.


## Separate r2 source-review correction

The first held package remains unchanged. Independent source review identified
three exact public declaration/dependency issues before any compilation: the
copied RMW deprecation annotation macros were not part of this finite include
closure; `rclc_executor_init` must accept the original `const rcl_allocator_t *`;
and `rcutils_error_state_t.line_number` is the original `uint64_t`, including on
wasm32 where `size_t` is narrower. This successor keeps the deprecated enumerator
and value2, makes the pointer declaration and implementation const-correct, and
uses a full uint64 decimal formatter. `rcutils_set_error_state` still takes its
original `size_t` parameter. No native RMW dependency or authority is added.

The existing ten-group contract source now contains C11 width/value assertions,
an exact function-pointer signature and an actual const-allocator initialization
inside the existing allocation/capacity group. Its existing error group checks
normal formatting and, on a size64 target, the complete20-digit value. These are
unexecuted source assertions, not additional groups or a native/Wasm result.
The original77 official source blobs, six Git commit/tree associations, both
complete original examples, recipes and all original lifetime/scheduling code
remain unchanged. The header and implementation now retain explicit original
OSRF copyright, Apache License2.0, immutable source-path/commit and changed-source
notices for adapted declarations, enum comments and allocator/String lifecycle.
The original rclc LICENSE and NOTICE remain byte-exact under `upstream/`.
