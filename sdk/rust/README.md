# roswasm Rust guest package

Start with the exact feature-gated raw imports, then Capability, fixed-buffer
codecs and Node/resources. Enable `alloc` for owned builtin values and the
registering polling Executor. There is no default backend or implicit resource
right. All public APIs have English documentation; `#![deny(missing_docs)]`
requires documentation when the crate is compiled.

Read [the full Rust/TinyGo guide](../docs/rust-tinygo.md) for build selection,
raw memory contracts, ownership/error semantics and service/Action/parameter/
TF/lifecycle recipes. The nine progressive examples are in `../examples/rust`.
The library has no external crate dependency and is `no_std`; callers using
`alloc` in a freestanding binary supply an allocator explicitly.

Sources and 57 positive/11 negative codec tests are prepared. Source checking
and formatter parsing do not establish Wasm compilation or ROS execution.
Actual compilation/import inspection and native interoperability remain pending.

## Explicit ownership and shutdown

With `alloc`, `Session::new(backend, maximum_resources)` checks ABI version1
and an application budget of1..1024 live capabilities. `session.node(label)`
creates a local organizer. Neither constructor opens a resource, creates a
native ROS context, obtains a grant, or changes host entity/byte/rate quotas.
An alias still needs an explicit operation/type/QoS grant from the Runtime.
The selected Wasm Instance remains the native caller identity.

Inspect every result returned by `Session::close`. It marks the group closed
before closing every live handle once; resources and Nodes retained by the
application then report BadHandle9 locally. A second close also reports9 and
performs no import. Ordinary `Node::new` remains allocation-free and independent
of a Session. Every typed owner has explicit `close`, `revoke` and checked
`raw_handle`; servers use `raw_handles` for their separate event/respond grants.
Close/revoke retire local ownership before importing, including on transport
failure. Preserve the first operation error while still inspecting all cleanup
results. `Drop` is only best-effort fallback and cannot guarantee cleanup after
a Wasm trap or abort. Reopening never restores revoked authority.

## Callbacks, executor and resource families

`Executor::try_new(capacity)` checks the scratch limit before allocation.
`add_subscription` transfers ownership and returns a checked result;
`add_typed` decodes an owned canonical value before calling the handler.
`remove` returns the unique subscription without closing it. `close` attempts
all registered queues, preserves the first failure and disables future spins
and registrations with9. Reentrant mutation/spin is17. Each outer pass polls
each queue at most once; Again11 is idle. No worker thread, implicit clock,
automatic busy loop, resize/replay on90, or native callback enters Rust.

`Server::spin_once` dispatches one Trigger/SetBool request and verifies that the
handler's reply retains its Gateway-issued request ID before responding.
`ActionServer::spin_goal_once` dispatches a bounded Fibonacci goal; feedback,
result and cancellation remain separately granted operations. Typed parameter
arrays/events, filtered graph snapshots, TF lookup/broadcast, exact nanosecond
clock domains, logger and ROS2 lifecycle methods reuse the existing imports.
Unsupported backend operations return95. There is no arbitrary custom service
or Action registration, dynamic CDR, ungranted timer/context, or rclcpp ABI.

`local::Context` is a bounded local deferred queue plus caller-time timers.
`defer` produces a one-shot `Future`: pending11, consumed9, and cancellation of
not-started work produces a local125 result without revoking a host alias.
Callbacks run after releasing result-slot borrows. `CallbackGroup` prevents
same-group reentry with17. Timers accept explicit u64 timestamps/periods, refuse
backwards time and overflow, and run at most once per pass without catch-up.
`shutdown` cancels pending work and timers; it does not close a Session or
interrupt a started synchronous native RPC. `scheduling.rs` shows these local
concepts. This layer is not a native asynchronous ROS implementation.

Lessons1/2 introduce raw imports and fixed-buffer codecs; lessons3/4 introduce
Node/Executor and operation-specific resources; `servers.rs` and
`events_tf_lifecycle.rs` show checked group shutdown, servers and ROS2 events.
Examples require only the aliases named in their comments; configure each
alias explicitly and run only the lesson whose required operations are granted.

For local verification use a separate `/tmp` target directory and one compiler
job (`CARGO_BUILD_JOBS=1`). Run the source checker first, then the unit/codec
tests, and compile each example for exactly one backend and wasm32. Source
checks establish source contracts only; successful compilation establishes
neither native delivery nor permissions. Historical cgroup receipts retain
their original budgets and do not qualify later source revisions.

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
