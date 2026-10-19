# Executable TinyGo lessons

Select exactly one backend with the wasi target and ros1/ros2 tag. Lessons
`raw`, `typed`, `node`, `resources`, `servers`, `events_tf_lifecycle` and
`scheduling` contain real main entries. Failure is an application panic/trap,
not a successful silent return. Configure the intended TinyGo panic strategy
explicitly and inspect its WASI imports. All resource cleanup is checked on
normal success/error return; a trap/panic cannot promise native cleanup.

| Entry | Actual lesson and separately required aliases |
| --- | --- |
| raw / typed | `pub`: String topic.publish, raw8-byte slot / checked canonical codec |
| node | `pub` plus `sub`: String publish/subscribe, checked Executor registration and one poll |
| resources | trigger-call, fibonacci-send, bool-get/set, topics-list, ros-clock and log-write, each exact operation |
| servers | trigger-events/replies and fibonacci-events/replies; distinct advertise/respond authorities for the same native resource/type |
| events_tf_lifecycle | ROS2-only parameter-events, transform-lookup, nodes-list, steady-clock and lifecycle-state/change |
| scheduling | Local Future, Timer, CallbackGroup, Context; no ROS alias or implicit clock |
| action_parameters | fibonacci-send/cancel-own/feedback/result, array-get/set and parameter-list; no implicit cancel-any |
| tf_graph_clock | transform-lookup/broadcast, nodes-list and ros-clock; real ROS1/ROS2 TF, never a lifecycle/events substitute |

The Action/parameter lesson reads feedback, requests own-goal cancellation, and
then reads the result, each once. A successfully consumed terminal result
retires its ID in the Runtime, so cancellation afterwards would return stale9.
False is a valid cancellation Bool when computation has finished but the result
has not been consumed. Again11 is idle; no automatic cancel/read replay occurs.

The `coverage` directory is a compiler/import-retention fixture. Its main is
intentionally inert and its zero-argument exported probe must not run with real
grants. It is not counted as a complete application or a native behavior test.
Every application lesson above is executable.

Use a compiler already installed, a separate `/tmp` output and one job:

```sh
cd sdk/examples/tinygo
tinygo build -p 1 -target=wasi -scheduler=none -tags=ros2 -o /tmp/roswasm-tinygo-servers.wasm ./servers
```

Replace ros2 with ros1 except for the ROS2-only lesson. Verify the selected
TinyGo release supports the explicit `go:wasmimport`/generic-code features and
inspect all emitted imports/signatures; native Go tests do not prove Wasm code
generation. A compile failure must not trigger an automatic broad WASI grant.

`-scheduler=none` selects caller-driven execution for these lessons; do not add
goroutines or `time.Sleep`. The default wasi asyncify scheduler can introduce
clock/poll dependencies. None is a compiler profile, not a permission grant,
and application/library code may still retain such imports. Inspect the actual
binary. TinyGo0.43 WASIp1 startup can independently require entropy via libc
`arc4random`; a denied `random_get` may abort before this lesson's main. No
noncryptographic fallback or automatic random grant is assumed. If required by
the actual binary/startup, explicitly review and grant WASIp1 `random` for the
intended scope, or qualify a freestanding target without that dependency.
Keep default permissions empty and leave the official runtime unchanged.

The [Rust lesson host instructions](../rust/README.md#build-and-host-prerequisites)
apply to these identical aliases/types/operations and explain native peers,
actual Worker PID/UID binding, complete CLI grants/limits and default13. Use
[host-setup](../../docs/host-setup.md) and the authoritative
[capability limit schema](../../../documents/ros/capabilities.md): every alias
requires all its fields, including native identity, bounds and QoS. The String
tutorial grants only explicitly selected pub/sub and cannot authorize servers,
parameters, Action, graph, TF, lifecycle or clock/logger operations.

Session/Node labels are local; construction creates no native identity or right.
Node and Session close return every checked error. Capability copies share
retirement state; repeated close9 never repeats a native notification. Executor
receives borrowed scratch bytes: decode/copy before retention. One poll may
deliver nothing during asynchronous native discovery. For an application loop,
call SpinOnce once per explicitly bounded step and treat Again11 as idle.
Caller clocks/counters own the deadline; no goroutine, implicit sleep/clock,
retry after90, or timeout rollback is supplied.

## Explicit unified events

The next runnable lesson uses an independently opened Source and `subEvents`
observer, checks a single poll, closes the Source, verifies its explicitly
permitted closed event and checks observer cleanup. See
[unified event v1](../../docs/unified-events.md) for exact aliases, header,
limits, backend entries, native peer prerequisites and policy preparation.
Metadata-only observation grants no payload permission. New38/43 inventories
and event source fixtures await the unified serial test stage; historical
receipts keep their original revision and narrower scope.
