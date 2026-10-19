# Command relay and copied asynchronous SDK

`relay.c` is a freestanding Wasm32 command which calls the existing ROS imports
inside one retained Runtime Instance. `relay.mjs` attaches exclusive binary
streams, and `async.mjs` supplies copied-result client-library objects. Node runs
the JavaScript host driver. The Runtime runs the Wasm command; this design does
not embed a JavaScript engine in Wasm or grant Node a POSIX sandbox.

The source is prepared. The previous small-memory pure stream test passed 824
checks; this is mathematical framing/ownership evidence. The new command has
not yet been compiled, validated or executed against a Runtime/Gateway/native
peer. Future tests are run serially by the task owner in fresh `/tmp` snapshots.

## Authority and launch contract

The default configuration has no ROS authority. The command's selected ROS
imports and an ABI-version query open no resource. A permitted alias must be
explicitly configured with operation, exact resource, type identity, transport,
QoS and all required numeric quotas. Permission to read/write basic stdio is a
separate explicit WASIp1 launch choice. Do not inherit environment, file, socket,
thread, clock, process or other rights for this relay.

A supervisor launches an admitted Worker with the exact selected ROS backend
and the command artifact. It reserves/stops the eventual Worker PID before
configuring Gateway authentication, then resumes the same PID through `exec`.
Gateway verifies the actual Worker `SO_PEERCRED` PID and pidfd lifetime; Runtime
`session.bind` and the active Wasm call frame enforce the principal. Node never
connects the Gateway socket and never sends an Instance ID, PID, principal or
policy in a frame. A JavaScript implementation of native ROS imports is outside
this execution route. HELLO is a compatibility/layout message, not identity
attestation. The supervisor records image/source/product pins, birth/starttime,
live maps, exact policy, cgroup and cleanup independently.

Use dedicated Worker stdin/stdout binary pipes and a separate audit stderr.
Never combine stderr with stdout. The SDK attaches existing streams and starts
no Worker, Gateway or ROS process itself. One owner writes requests and one
owner reads replies. The Node driver's own control stdin and stdout markers are
separate from Worker binary streams.

## Closed framing version 1

Every integer below is little-endian, without packing or alignment padding.

| Header offset | Width | Field |
| --- | --- | --- |
| 0 | 4 | Magic `0x524a5755`, bytes `UWJR` |
| 4 | 2 | Protocol version, exactly 1 |
| 6 | 1 | Explicit ROS backend, exactly 1 or 2 |
| 7 | 1 | HELLO=1, CALL=2, RESULT=3, SHUTDOWN=4, FATAL=5 |
| 8 | 8 | Sequence; HELLO=0, subsequent requests strictly increase by one |
| 16 | 2 | Exact ABI-table index; control records use 65535 |
| 18 | 1 | Exact original ABI arity, at most 7 |
| 19 | 1 | Reserved, exactly zero |
| 20 | 4 | Complete following body byte count, bounded before allocation |

HELLO has exactly 80 body bytes: ABI version u32, selected function count u32,
32-byte SHA256 of the canonical selected ABI table, memory byte size u64, input,
slot and output offsets u64 each, input/output capacities u32 each. The selected
table is exactly 38 ROS1 or 43 ROS2 functions. The digest covers names, order,
parameter types and i32 result; the C table and JS table are source-bound to
`sdk/abi/abi-v1.json`.

A CALL body contains eight bytes per typed argument, the eight-byte output-slot
seed, u32 snapshot count, then the complete copied input payload. An i32 cell's
upper 32 bits are zero. An i64 remains an unsigned BigInt with all 64 bits; no
Number conversion is permitted. Only memory0 and the negotiated reserved
input/slot/output offsets are accepted. Input lengths equal actual transferred
bytes. Poll/RPC capacities are bounded by output storage; scalar operations
accept no payload. Frames cannot select arbitrary guest memory or function
pointers.

A RESULT body is status u32, raw slot u64, snapshot count u32, guard result u32
(exactly 1), followed by that many copied output bytes. The raw snapshot can be
used to check refusal sentinels. High-level APIs decode only status0, and expose
`required` on status90 only if the UINT64_MAX seed was overwritten. Legacy receive90 may
consume an event; event_poll_v1 retains it on capacity90. The SDK does not resize/replay. Error11 can represent empty
queues or quota refusal and never proves delivery. Open uses zero as its slot
seed, accepts a nonzero returned handle and preserves every handle bit.

SHUTDOWN is exactly index65535, arity0 and body0. RESULT acknowledgement has
status0, slot0, snapshot0 and guards1. The command then exits0. EOF alone is
failure, and an acknowledgement alone does not prove actual process cleanup.
The supervisor waits for Worker exit0 and releases the independent native
peer's final observation gate afterwards.

FATAL has index65535, arity0 and one u32 fixed public code: header1, body2,
input-I/O3, output-I/O4, guard5, ABI6 or sequence7. It is a transport failure,
never a Runtime errno. No payload, credential, alias or exception is printed.

## Bounds and ownership

Default compiled scratch has 4096-byte input and output regions. Sixteen-byte
guards leave 4080 usable bytes on each side, plus the independent eight-byte
slot. HELLO exposes only those usable ranges. Each call resets input/output,
preserves the seed, validates both guards, and makes exactly one original ABI
call. Input and responses are copied; application callbacks run after transport
ownership is released. Scratch capacities are storage choices, not grant quotas.

WASI reads/writes use exact wasm32 vectors and checked returned counts. Short
successful transfers advance by the returned count. At most sixteen consecutive
WASI EINTRs are retried without progress. EOF, zero-progress write, EAGAIN,
oversized count or other errors terminate the command. Retry of a partial I/O
transfer never replays a ROS call. Only `fd_read`, `fd_write` and `proc_exit` are
extra imports; no Guest clock is used.

`RelayChannel` keeps one outstanding call through both write completion and
reply copy. Reentry fails locally, with no queued/replayed command. Host deadlines
retire the channel; cancellation/timeout cannot prove that an already admitted
native side effect was rolled back. A supervisor cleans up its owned actors.
Checked handle close/revoke first prepares the full frame and captures the real
writable delegate. A local busy/form/capacity/lookup refusal before a write
attempt preserves the owner for later explicit cleanup. At that captured write
attempt the owner stays retired, including a throw or returned107/110; Runtime
retirement may precede a backend failure. A write attempt does not prove actual
Host import entry. No error class permits automatic replay. See
[the cleanup result contract](API.md#ownership-context-and-cleanup).

## SDK layers and local scheduling

Use `AsyncSession.attach` for the copied pipe route. Use synchronous `Session`
only with the real `WebAssembly.Memory` and Instance exports supplied by an
authority-aware embedder. A Promise or fake memory cannot replace that bridge.
Both surfaces expose Node, Publisher, Subscription, Client, Service, Fibonacci
Action client/server, Parameters, Graph, TF, Clock, Logger and ROS2 Lifecycle.
Each factory acquires every explicitly requested alias before activating a
server. Failure attempts cleanup of every earlier acquisition and retains the
first refusal. Known unentered owners stay in the Node's private cleanup ledger;
secondary cleanup failures and full handles are reported separately. An async
Context declines transport shutdown while those owners remain.

`Context`/`AsyncContext` are local ownership scopes, not native ROS contexts.
`Executor`/`AsyncExecutor` poll a registration snapshot once and reject nested
spin. `defer` returns a local Future and queues bounded caller-driven work.
The next explicit pass executes at most the configured `maxTasksPerPass`.
`Client.callDeferred` invokes the existing bounded service RPC in that pass;
it does not turn native `service_call` into a nonblocking operation. Unstarted
Future cancellation is local only. CallbackGroup supplies local mutual exclusion.
Timer requires explicit `ready(clock)` and an optional already-granted Clock;
it opens no timer/clock and schedules no automatic wakeup. Parameter callbacks
run after copied event decoding. No scheduling object creates threads, sleeps,
reads a clock or opens authority implicitly.

## Prepared compilation and acceptance

An owner may compile each backend serially in fresh output storage:

```sh
clang --target=wasm32 -std=c11 -O2 -ffreestanding -fno-builtin -nostdlib \
  -DROSWASM_BRIDGE_ROS_VERSION=2 \
  -Wl,--entry=_start -Wl,--export-memory \
  -Wl,--initial-memory=131072 -Wl,--max-memory=131072 \
  sdk/javascript/relay.c -o /tmp/fresh-output/ros2-relay.wasm
```

Choose backend1 explicitly for ROS1. The owner must inspect actual signatures,
require all selected ROS imports and exactly the three permitted WASI imports,
reject unresolved libc/helper imports, validate the module and pin its bytes.
No compilation recipe alone establishes executable compatibility or delivery.

Required delivery rows are ROS1-positive, ROS1-default-denied, ROS2-positive and
ROS2-default-denied. Positive rows independently observe one exact final String
publication, one exact reply and an ungranted alias denial13 with protected
slot/output, then explicit closes/stale9/shutdown/Workerexit0. Default rows have
no ROS grants and must independently observe no native effect. Native discovery,
writer identity and the quiet observation window are external proof, not inferred
from `topic_publish` success. New relay/native fixture acceptance is pending.

See [the checked application](../examples/javascript/05-runtime-relay.mjs) for
the fixed SDK delivery control protocol, and [the full resource lesson](../examples/javascript/06-async-application.mjs)
for caller-driven service, Action, parameter and local scheduling patterns.

## Additive unified event source contract

[Unified event v1](../docs/unified-events.md) specifies the separately granted
observer, complete64-byte envelope, seven kinds, Source/observer lease checks,
capacity-retaining poll, finite readiness and real bounded `service_submit`.
The new APIs are additive; local deferred tasks still advance synchronously
when explicitly scheduled, and do not gain native asynchronous authority.
No implicit alias, clock, thread, retry or stronger cancellation grant is added.
The six runnable unified-event lessons and source fixtures are prepared for the
unified serial test stage; dated historical34/39 receipts certify their original
revisions only.
