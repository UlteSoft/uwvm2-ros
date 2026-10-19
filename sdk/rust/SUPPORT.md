# Rust API and runnable-example coverage

This is a source/API matrix. Native integration receipts qualify their own
frozen revisions; this document does not report a new native ROS execution.

| Layer / concept | Actual public entry | Runnable lesson / verification boundary |
| --- | --- | --- |
| Exact ABI | raw::ros1 all38, raw::ros2 all43 imports | raw.rs; coverage.rs is a separate compile-only probe |
| Canonical values | codec fixed-buffer functions; alloc typed::Canonical builtin values | typed.rs; independent positive/negative wire tests |
| Backend / ownership | Backend, Capability, Session::new/node/close, Node::close | Node and resources lessons; all grants remain explicit |
| Publish / receive | Publisher typed methods, Subscription::poll/value, Executor checked add/remove/spin/close | node.rs; callbacks run only in the caller step |
| Service client/server | Client::trigger/set_bool, Server::spin_once/respond_bool | resources.rs, servers.rs; separate advertise/respond, exact request ID |
| Action client/server | ActionClient send/own-any-all cancel/feedback/result; ActionServer::spin_goal_once/respond_fibonacci | action_parameters.rs, servers.rs; optional permissions never inferred |
| Parameters / events | Parameters get/set/delete/exists/list/search/describe/declare/undeclare, typed arrays and subscribe_events | resources.rs, action_parameters.rs, events_tf_lifecycle.rs |
| Graph | Graph filtered inspect/list/snapshot | resources.rs, tf_graph_clock.rs, ROS2 events lesson |
| TF | Tf lookup/broadcast and typed Transform | tf_graph_clock.rs includes real ROS1 and ROS2 methods |
| Clock / logger | Clock::read exact u64 domain; Logger::write | resources.rs; no scheduling clock/logger opens implicitly |
| Lifecycle | Lifecycle::state/change | ROS2 events_tf_lifecycle.rs; exact host node/transition |
| Local asynchronous concepts | local::Context::defer/spin_once/shutdown, Future::try_take/cancel | scheduling.rs; a deferred native Client remains synchronous in that step |
| Timer / CallbackGroup | explicit first/period/now, checked time/overflow, mutually exclusive run | scheduling.rs and local_schedule.rs; no clock, sleep or thread import |

With alloc, Node.close attempts only that Node's registered live handles;
Session.close attempts all its live groups. Every status is returned, the first
operation failure can be retained while inspecting cleanup, and later factories
or resources report9. Without alloc, Node.close disables its factories only;
the unique typed owners must each be closed. Capability's existing best-effort
Drop remains fallback, not checked cleanup or a trap-time guarantee. Session
and Executor local budgets do not raise host limits. Server recursive spin/poll
is17 before dequeue. Both server role capabilities share the callback lease
with their alloc Node/Session owners; direct or grouped cleanup returns17 before
any retirement/import and keeps the group open for cleanup after dispatch.
No-alloc Node.close only disables factories and owns no native handle. Rust
Subscription.poll returns bytes/values without directly invoking a handler;
its existing Executor guard remains separate. Borrowed event data
expires after the caller's scratch is reused; retain decoded owned values instead.

ROS1 lacks ROS2 declare/undeclare/describe/events, Action graph inspection,
system/steady clock and lifecycle imports. ROS2 lacks ROS1 delete/exists/search
and wall clock. These operations stay95, not aliases for another function.
Trusted generated bounded Topic/Service/Action profiles use reviewed codecs and
pinned adapters; see ../custom-types/BOUNDED_V2.md. Arbitrary dynamic native
Service/Action/CDR/IDL loading remains outside this SDK. A local Session,
Node, Context or Future does not create a native ROS context, executor, connection
or asynchronous service. There is no native rclcpp/rclc binary/source ABI promise.
Every supported operation uses the real registered Runtime import; unsupported
behavior is documented instead of emulated or granted implicitly.

Use [example instructions](../examples/rust/README.md) for exact aliases,
operation limits, native peer readiness, CLI connection fields and default13.
Root runs all actual local compiler/model/Wasm tests serially in separate `/tmp`
outputs; source preparation is distinct from an executed receipt.

Private `callback.rs` is the actual guard/owned-ID dispatch core called by the
public Server methods. Its seven native fake-source tests exercise nested poll
and close refusal, exact request/goal IDs, malformed body/wrong reply refusal,
idle/failure behavior, shared retirement leases, whole-group preflight and guard
reset without importing or mocking native ROS. A handler may capture a Session
or Node through shared references; Rust borrowing alone does not protect that
route, so the same handle state is checked before grouped cleanup.
These tests add to the10 codec/local tests; the complete native Rust inventory
is17 test functions. Actual native delivery remains a separate acceptance lane.
