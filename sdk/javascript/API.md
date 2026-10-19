# Complete supported JavaScript client surface

The synchronous and copied asynchronous APIs wrap every current selected ABI
entry. Importing a class or creating a local organizer grants no authority.
Capability aliases independently select operation, exact native resource/type,
identity, QoS and numeric quotas in trusted CLI policy. The default configuration
has no ROS rights. Local Node labels cannot change the Runtime principal.

## Transport and setup

Use `Session` with the exact real `WebAssembly.Memory` and admitted Instance
exports supplied by an authority-aware embedder. The reserved scratch is owned
against the guest allocator and views are reacquired after memory growth.
`fromGuestBridge` validates actual exports; it never installs native ROS mocks.

Use `AsyncSession.attach({version,readable,writable,inputCapacity,outputCapacity,
timeoutMs})` for exclusive supervisor-owned binary pipes. The retained command
calls real ROS imports inside its original Wasm Instance. This route uses copied
results and supplies no fake Memory/Instance. The supervisor owns launch,
authenticated Worker PID/Gateway session, policy, images/maps, deadlines and
cleanup. Node never connects the Gateway socket. The command's only additional
imports are basic fd_read/fd_write/proc_exit; those stdio rights are explicitly
selected without environment/file/network/process/thread/clock inheritance.
Read [the closed protocol](RELAY.md) before using it. Actual new relay compilation
and delivery remain pending; source/pure tests are not native acceptance.

## Resource methods and factories

`Async` indicates the copied counterpart: resource methods return Promises.
That is a host transport contract, not a new nonblocking native ABI.

| Object / counterpart | Node / AsyncNode factory and public methods |
| --- | --- |
| Session / AsyncSession | `open(alias)`, `request(operation,handle,input)`, `poll(handle)`, `write(operation,handle,input)`; async also `raw`, `handle`, `shutdown` |
| Capability / AsyncCapability | `check`, `request`, `poll`, `close`, `revoke`; opaque handle remains BigInt |
| Node / AsyncNode | Explicit factories below, `open`, `close`, `pendingCleanup`; pending rows retain full BigInt handles |
| Publisher / AsyncPublisher | `createPublisher(alias,codec)`, `publish(value)`, `close` |
| Subscription / AsyncSubscription | `createSubscription(alias,codec,callback)`, `spinOnce`, `close`, explicit `revoke` |
| Client / AsyncClient | `createClient(alias,requestCodec,responseCodec)`, `call(value)`, `callDeferred(executor,value)`, `close` |
| Service / AsyncService | `createService(advertiseAlias,respondAlias,handler,requestCodec)`, `spinOnce`, `close`, explicit pair `revoke` |
| ActionClient / AsyncActionClient | `createActionClient({send,cancelOwn,cancelAny,feedback,result})`, `sendGoal`, `cancelOwn`, `cancelAny`, `cancelAll`, `feedback`, `result`, `close` |
| ActionServer / AsyncActionServer | `createActionServer(advertiseAlias,respondAlias,handler)`, `spinOnce`, `close`, explicit pair `revoke` |
| Parameters / AsyncParameters | `createParameters(operationAliases,codec)`, `get`, `set`, `list`, `describe`, `declare`, `undeclare`, `delete`, `exists`, `search`, `subscribeEvents`, `pollEvent`, `onEvent`, `close` |
| Graph / AsyncGraph | `createGraph(alias,operation)`, `call`, `read`, `close` |
| Clock / AsyncClock | `createClock(alias,operation)`, `call`, `now`, `close` |
| Tf / AsyncTf | `createTf(lookupAlias,broadcastAlias)`, `lookup(nanoseconds)`, `broadcast(value)`, `close` |
| Lifecycle / AsyncLifecycle | `createLifecycle(getAlias,changeAlias)`, `getState`, `changeState`, `close` |
| Logger / AsyncLogger | `createLogger(alias)`, `write(text)`, `close` |
| Resource / AsyncResource | `createResource(alias,exactSevenArgumentOperation)`, `call(bytes)`, `close` |

There are13 typed/generic resource factories plus explicit `open`. A factory
validates codec/handler/version before opening, acquires all requested aliases
before advertising and commits ownership only after construction/activation.
A requested optional alias failure rolls back the whole factory. It attempts
cleanup of every earlier acquisition, preserves the first refusal and retains
known unentered owners in a private Node ledger. Secondary cleanup failures are
reported separately. Missing optional authority is a local denial13 and opens no
fallback alias. Services separately require advertise/respond; every Action
operation separately requires a grant. Cancel-all passes zero only through the
explicit cancel-any handle. Other Action/request IDs remain nonzero BigInt.

Direct async polling/server constructors validate capability/session/codec
binding but do not await activation inside a JS constructor. Use Node factories
for the complete open/activate/rollback contract. Graph/Clock check their exact
read family on construction and before every call; mutation cannot turn a
read-only wrapper into an unrelated write. Generic Resource intentionally accepts
any selected seven-argument operation with its exact handle authority.

## Ownership, Context and cleanup

`Context(session)` and `AsyncContext(session)` are local ownership scopes. They
create Nodes with `createNode(name)` and prohibit fresh opens after `shutdown`.
They create no native ROS context/node/thread. Each Node remembers handles
returned by explicit opens/factories. Local scalar/form/capacity validation,
busy checks and delegate lookup complete before checked close/revoke attempts
the captured raw bridge, copied native `invoke`, or relay `write`. A known
refusal before that boundary preserves the owner. At the delegate attempt it
becomes permanently retired, including a throw or returned107/110, because
Runtime retirement can precede backend failure. Exception classes and status
codes never establish that no effect occurred. GC never executes imports.

`Capability.closed` and `AsyncCapability.closed` are enumerable, nonconfigurable
read-only getters. They already report true during synchronous delegate reentry;
assigning `closed=false` throws in ESM strict code. `session` and `handle` remain
ordinary writable properties. A checked close requires a registered SDK callable;
replacing it with an unregistered wrapper/bound function refuses before entry.
The internal preparation helpers are exported between SDK modules, so this is a
local lifetime contract for ordinary SDK paths, not authentication or an
unforgeable membrane against hostile code in the same JavaScript realm. The
actual Runtime still enforces identity, memory bounds and every permission.

Synchronous close returns the existing array of Runtime statuses. If a bridge
throws, every remaining close is attempted first, then AggregateError exposes
`results` and thrown errors. Async close returns per-handle `{handle,status}` or
`{handle,error}` records and continues all distinct closes. An error property may
contain `null`, `undefined` or another primitive; test property presence rather
than truthiness. Inspect every item.

Batch results and a synchronous `AggregateError` expose `attempts`. Each row has
`{handle,status,error,threw,attempted,retained}`. Handles remain exact BigInt;
`threw` distinguishes a thrown `null` from no exception. `attempted` means the
JavaScript delegate was attempted; it does not mean a Host import ran or cleanup
succeeded. A retained owner can be cleaned up explicitly after the local refusal
has been resolved. An attempted owner must never be replayed automatically.

`node.pendingCleanup()` returns `{handle,attempted:false}` rows for remaining
live owners without performing I/O. Its nonenumerable `factory` property is
either null or `{primary,attempts}` for the latest failed factory, including a
frozen or primitive primary. Those attempt rows are snapshots of factory
rollback; later explicit cleanup does not rewrite them. The original primary is
re-thrown unchanged; extensible object primaries also receive `cleanupAttempts`
and, when present, `cleanupFailures`. Retain the Node while cleanup is pending.

AsyncNode prevents overlapping factory/close transactions before opening more
authority. AsyncContext refuses shutdown while a Node acquisition transaction
is active, then closes all Nodes. A Node/Context remains open when known
unentered owners remain. With such owners, `shutdown({transport:true})` returns
`transport: {status:17,cleanupPending:true}` without shutting down the channel.
After explicit cleanup resolves them, transport shutdown can be requested.
Otherwise `shutdown({transport:true})` separately requests
relay acknowledgement; the supervisor still waits for actual Workerexit0 and
records native observation and cleanup. Default transport shutdown is false.

Each Subscription, Service and ActionServer has an intrinsic direct-call guard,
independent of Executor registration. A shared local lease protects its actual
Capability owners from retirement through resource `close`/`revoke`, direct
Capability calls, Node cleanup or Context shutdown throughout poll, decode,
callback/await and response. Such refusal is17 before any Runtime call or owner
retirement. Synchronous pair cleanup returns `[17]`; asynchronous pair cleanup
returns `[{status:17}]`; synchronous Node cleanup returns `[17]`, while
AsyncNode and both Context shutdown paths throw RosError17. Refused owners stay open and a refused
Context stays active, so checked cleanup can run after the pass. Whole-group
cleanup leases also prevent a fresh poll during a pending asynchronous close.

Public Capability.poll is also refused with RosError17 during its resource's
polling lease. Only the private typed dispatcher may poll that leased owner;
ordinary responder request methods remain usable. Session.poll(handle) and raw
imports are deliberate lower-level lifetime escapes, without the typed guard.

The current callable and original owners are retained by reference, preserving
mutable captured state and the resource as callback `this`. Reassignment affects
the next pass. `finally` releases both leases after success, codec/import error,
handler exception or Promise rejection. Ordinary request operations remain
available on a polling owner: a server may issue exact-ID feedback/respond after
the copied poll result releases scratch/transport. Do not bypass local lifetime
rules by directly manipulating raw imports or fabricating duplicate owners.
These guards add no native synchronization, authority or cancellation semantics.

Action feedback/result require the exact requested nonzero BigInt ID, and result
requires terminal2/3/4. A canonical other-ID reply returns local status22 with
`runtimeStatus:0`, `value:null` and unchanged copied raw bytes/length metadata;
an earlier caller result is never mutated. The SDK does not replay or cancel it.
Successful terminal reception retires the native goal, so cancel-own afterward
can return9. Optional cancel-any remains independently supplied authority.

## Executor, Future, callback groups and timers

Executor/AsyncExecutor register objects exposing `spinOnce`. Every pass takes
the registration snapshot, polls each once and releases its busy guard in
`finally`. Additions from callbacks run next pass. Nested spin returns `[17]`
without another import. Callbacks run after copied output releases the scratch
or transport borrow, permitting an explicitly granted operation in a callback.
Callback exceptions propagate; unstarted deferred work in that pass receives the
same exception without being executed. No partial pass is replayed.

Executor close also cancels unstarted tasks already in the active entry snapshot
and stops subsequent entity polling. A running task may finish once; later
snapshot tasks remain cancelled. Closed executors refuse new registrations and
passes. Closing AsyncTimer while explicit readiness is awaited prevents the
callback from starting after that readiness resolves.

`defer(fn)` returns a local Future and creates no task thread. Default queue
bounds are64 pending tasks and1 task per pass; explicit limits must be1..4096
and the pass limit cannot exceed the queue bound. Tasks added during a pass wait
for the next entry snapshot. `future.done`, `future.result()` and
`future.promise` expose local completion; `cancel()` cancels only unstarted work.
`Client.callDeferred(executor,value)` executes the existing bounded synchronous
service RPC when the caller advances its executor. Awaiting a Future alone does
not drive that executor. Native service_call remains synchronous in the Worker;
this wrapper claims no remote rollback or native cancellation after admission.

Pending active-snapshot tasks remain charged to the pending queue limit while
entity callbacks run. Timer readiness and callback lifetime have their own busy
guard, rejecting direct reentry before readiness rather than invoking it twice.
CallbackGroup is local mutual exclusion with `run`/`runAsync`, refusing nested
group entry. Timer/AsyncTimer require explicit `ready(clock)` and callback.
The caller may supply an already-granted Clock; no implicit clock, sleep, host
wakeup or native timer is created. Readiness must return boolean. Timer close
only disables that local Pollable and never closes a caller-owned Clock.
Parameter `onEvent(callback)` registers a local post-decode hook and returns a
removal function. Callback snapshots permit safe registration changes; these
are not native parameter-set callbacks or mutation authority.

## Typed codecs

| Profile | Public codecs |
| --- | --- |
| Topic values | StringMessage, BoolMessage, TwistMessage, ImageMessage / `imageMessage(version)`, TransformMessage / `transformMessage(version)` |
| Services | TriggerRequest, SetBoolRequest, ServiceResponse, TriggerResponse, SetBoolResponse; request/reply envelope helpers |
| Built-in Fibonacci Action | FibonacciGoal, GoalId, IncomingGoal, ActionFeedback, ActionResponse, ActionResult |
| Parameters | Parameter Bool/Int32/Int64/Float64/String, `parameterArray(kind)`, StringList, TypedParameterEvent v2, explicit legacy ParameterEvent |
| Resource responses | LifecycleState, TimeNanoseconds, encodeLookupTime |
| Custom topics | Caller-supplied generated fixed-record encode/decode codec with matching registered native/codec identities |

Strict codecs reject truncation/trailing bytes, invalid Bool/UTF-8/nonfinite
numbers and preserve all signed/unsigned64-bit values as BigInt. Strings preserve
literal leading U+FEFF. Image mirrors unconditional encoding/frame/timestamp/
stride/data rules. Transform mirrors strict distinct frames, finite fields and
quaternion tolerance without normalization. The versioned Header codecs enforce
ROS2 sequence0 and signed-int32 seconds. Policy can impose tighter bounds.
ActionResult permits terminal2/3/4, not feedback1. Typed parameter event payload
is explicitly selected by its tag; deletion carries no old value. Native ROS1
String-array XML restrictions and native RMW String behavior remain separate.

Decode only status0. `required` is non-null on success, or on90 only if the
UINT64_MAX length sentinel was overwritten. Error90 can consume a queued event
or leave the seed intact on admission refusal; no resize/replay occurs. Error11
can mean empty queue or rate refusal. Host deadlines retire the relay and do not
prove that admitted native effects were rolled back. Protocol errors are local
RelayError/SchedulingError and are never falsely reported as import errnos.

## Selected ABI and remaining native limits

Both versions support topics; Trigger/SetBool call/advertise/respond; bounded
Fibonacci Actions; fixed-frame TF lookup/broadcast; exact param get/set/list;
graph topic/node/service inspect/list; ROS time; log.write. ROS1 additionally
has param delete/exists/search and wall time. ROS2 has param describe/declare/
undeclare/events, graph Action inspection, Lifecycle get/change and system/
steady clocks. `abi.mjs`/`raw.mjs` expose the complete exact38/43 table.
Version-unsupported calls preserve95; no fallback native functionality appears.

The current source registry supports explicitly generated bounded Topic,
Service and Action profiles; see ../custom-types/BOUNDED_V2.md for their reviewed
codec/native adapter contract and pending acceptance scope. Arbitrary native
resource/node creation, dynamic type discovery, callback pointers, native
timers/executors and language interpreter management remain outside this numeric
ROS ABI. The separate static CPython and embedded QuickJS producers place their
application logic inside Wasm and still use ordinary admitted imports. Local
client-library concepts do not imply complete native rclcpp/rclpy
behavior. Unified event codecs and explicit service_submit are additive; see
../docs/unified-events.md for their independent Source/observer permissions.

See [the nine application lessons](../examples/javascript/README.md). Tests are
run serially by the owner with low memory in fresh `/tmp` snapshots. Existing
receipts stay immutable. New36-case client models and the earlier824 stream
checks are pure specification/ownership evidence. Current-source Node reruns,
relay compilation/signature validation and actual four-row native delivery are
pending; no source-ready checkpoint substitutes for those results.
