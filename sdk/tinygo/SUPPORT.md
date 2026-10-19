# TinyGo API and complete example matrix

| Concept | Actual public API | Runnable lesson |
| --- | --- | --- |
| Exact ABI / wire | all38 RawROS1/all43 RawROS2 functions, fixed-buffer codecs and Message values | raw, typed; coverage is compile-only |
| Ownership / context selection | Open, Capability Close/Revoke/RawHandle, NewSession/NewNode and checked group Close | node, resources |
| Publisher/subscription/executor | typed Publish, Poll, NewCheckedExecutor, TryAdd/RemoveSubscription, SpinOnce/Close | node |
| Client/service server | Trigger/SetBool, Server SpinOnce/RespondBool | resources, servers |
| Action client/server | send, own/any/all cancel, feedback/result, SpinGoalOnce/RespondFibonacci | action_parameters, servers |
| Parameter arrays/events/metadata | Get/Set/DeclareValue, raw operation-specific methods, SubscribeEvents, typed event codecs | resources, action_parameters, ROS2 events_tf_lifecycle |
| Graph / TF / clock / logger | filtered snapshots, Lookup/BroadcastTransform, exact Read domains, Write | resources, tf_graph_clock |
| ROS2 lifecycle | State/Change, fixed host node/transition | events_tf_lifecycle |
| Local queued work/results | NewContext, package Defer, Future Take/Cancel, caller SpinOnce/Shutdown | scheduling |
| Local Timer/CallbackGroup | explicit deadline/period/now, checked overflow/backwards time, group Run | scheduling |

The selected build tag fixes backend/import module. Node/Session labels create
no native context/identity or rights. Every typed resource exposes checked close,
revoke and full-u64 handle borrowing; servers expose two independent IDs. Copies
share retirement state. Group Close marks closed first, attempts every live
state once and returns all errors; closed resources/factories are9. An active
server callback shares its lease with both capability states. Group cleanup
preflights every live state before closing anything;17 leaves the Node/Session
and all owners open for checked cleanup after dispatch. Factories
roll back successful prior acquisitions and preserve the original failure.
Executor registrations transfer ownership only on success; ignored compatibility
registration leaves ownership with the caller. Its existing nested spin/mutation
refusal stays22; Server/ActionServer recursive spin/poll is17 before dequeue.

Context is local state, not native ROS or an asynchronous RPC implementation.
Defer does not call inline or start a goroutine. A synchronous Client still
blocks that later step under its configured host deadline. Future cancellation
is local125 for not-started work; it does not revoke a grant or undo native work.
Timer time is caller-supplied, with no implicit clock or catch-up replay.
Borrowed callback scratch must be decoded/copied before retention. No finalizer,
automatic retry after90, timeout rollback or Wasm trap-time cleanup is promised.

ROS1 has no ROS2 declare/undeclare/describe/events, Action graph inspection,
system/steady clock or lifecycle. ROS2 has no ROS1 delete/exists/search or wall
clock. Their exact absent dispatches remain95. Existing bounded builtin adapters
and trusted generated fixed-record custom topics do not imply arbitrary native
Service/Action/IDL/CDR loading or native rclcpp/rclc ABI compatibility.

[Example instructions](../examples/tinygo/README.md) cover actual aliases,
complete operation limits, native peers/discovery, CLI identity bindings and
default13. Native Go local/codec tests, TinyGo Wasm compilation/import validation
and native ROS delivery are different acceptance scopes; all actual new tests
are owned by the root's serial `/tmp` lane.

`server_callbacks.go` is the private actual dispatch core used by Wasm Server
methods. Its three native fake-source tests cover nested poll/close guards,
owned IDs, wrong response/no-side-effect, malformed/idle events and buffer
overlap before dequeue. Go Server copies share this guard as well as capability
retirement state. The two `owner_state_test.go` tests exercise the same actual
retirement core used by Capability.Close/Revoke: busy17 sends no notification,
later cleanup retires once even after a timeout, and a busy later owner prevents
partial grouped retirement. Subscription.Poll only returns bytes; it invokes
no user callback, so its semantics are unchanged. These five tests add to the13
codec/local-state functions, making the complete native Go inventory18 tests.
They prove dispatch mechanics with
local bytes/counters, not ROS authentication, middleware or native delivery.

The caller-driven compiler profile explicitly uses `-scheduler=none`; these
lessons use no goroutine or sleep. Inspect actual binary imports rather than
assuming that selecting this scheduler removes every clock/poll/entropy import
from arbitrary application code. The selected TinyGo0.43 WASIp1 runtime's
startup `hardwareRand` calls libc `arc4random`, so entropy can remain independent
of scheduler choice. Denying `random_get` can abort initialization before main;
the SDK does not rely on a noncryptographic fallback or rewrite that runtime.
Only an explicit reviewed WASIp1 `random` grant can supply this dependency, or
a separately qualified freestanding target can avoid it. Neither compilation,
ROS grants nor these local scheduling types automatically authorize randomness.
Default WASI/ROS authority remains unchanged. The old19-row compiler PASS and
later scheduler/startup acceptance describe distinct frozen recipes and scopes.

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
