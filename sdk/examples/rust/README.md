# Executable Rust lessons and separate compiler fixtures

Build one lesson with exactly one `ros1` or `ros2` feature. Enable `alloc` for
Node/Executor, Session and local scheduling lessons. All application entries
have a real main that opens the named aliases, invokes the demonstrated typed
operations and checks cleanup. Nonzero errors exit1. A single event poll may
legitimately deliver zero callbacks; none of these programs claims native
delivery merely because Publish returned0.

| Lesson | Complete entry | Independent aliases and actual operations |
| --- | --- | --- |
|1 | `raw.rs` | `pub`: topic.publish/String, exact raw offsets and8-byte slot |
|2 | `typed.rs` | `pub`: topic.publish/String, checked fixed scratch |
|3 | `node.rs` | `pub`/`sub`: String publish/subscribe, checked Executor registration, one poll and close |
|4 | `resources.rs` | trigger-call: service.call/Trigger; fibonacci-send: action.send-goal/Fibonacci; bool-get/set: param.get/set Bool; topics-list: graph.list-topics; ros-clock: time.read-ros; log-write: log.write |
|5 | `servers.rs` | trigger-events/replies: service.advertise/respond; fibonacci-events/replies: action.advertise/respond; same exact resource/type/QoS per pair |
|6 | `events_tf_lifecycle.rs` | ROS2-only parameter-events: param.subscribe-events; transform-lookup: tf.lookup; nodes-list: graph.list-nodes; steady-clock: time.read-steady; lifecycle-state/change: lifecycle.get-state/change-state |
|7 | `scheduling.rs` | No native alias or clock. Local Future, caller-time Timer, CallbackGroup and Context shutdown |
|8 | `action_parameters.rs` | fibonacci-send/cancel-own/feedback/result, array-get/set and parameter-list, all independent operation grants |
|9 | `tf_graph_clock.rs` | transform-lookup/broadcast, nodes-list and ros-clock, including real ROS1 TF |

Lesson8 receives feedback once, requests own-goal cancellation once, and only
then receives the terminal result once. Consuming a terminal result retires the
ID in the Runtime; a later cancellation of that ID returns stale9. A false
cancellation Bool is valid while the completed result is still unread. Again11
is an idle read, not a reason to retry cancellation or infer rollback.

`coverage.rs` is a **compiler/import-inspection fixture**, not an application
lesson. Its inert main is intentional; never execute its exported probe with
a valid index, because zero raw arguments are not buffers, handles or grants.
Keep it separate from behavioral acceptance. The complete entries above
provide the executable applications.

## Build and host prerequisites

With the selected compiler target already installed, use separate `/tmp`
outputs and one job, for example:

```sh
WASI_SDK=/absolute/already-installed/wasi-sdk
export CARGO_ENCODED_RUSTFLAGS
CARGO_ENCODED_RUSTFLAGS="$(printf '%s\037%s\037%s\037%s\037%s\037%s' \
  '-C' 'codegen-units=1' '-C' 'link-arg=--threads=1' \
  '-C' "link-arg=$WASI_SDK/share/wasi-sysroot/lib/wasm32-wasip1/libc.a")"
CARGO_BUILD_JOBS=1 cargo build --offline -j1 --manifest-path sdk/examples/rust/Cargo.toml \
  --target wasm32-wasip1 --target-dir /tmp/roswasm-rust-lessons \
  --no-default-features --features ros2,alloc --bin servers
```

Replace ros2 with ros1 except for the ROS2-only lesson. The library supports
no-alloc Capability/codec/resource ownership; a freestanding alloc binary
provides its allocator. These standard-runtime examples may emit ordinary
WASI startup/stdio imports: inspect them independently of the ROS import table.
The encoded flags above link the exact installed WASIp1 `libc.a`, including
compiler-generated memory intrinsics such as `memcmp`. This applies to `no_std`
guests too: omitting Rust `std` does not supply C intrinsic definitions. Do not
replace the archive with host `-lc`, add an `env.memcmp` import allowance or
grant a WASI permission to hide a link failure. The maintained compiler runners
bind this archive before/after and encode each flag as an independent token.
Unset `CARGO_ENCODED_RUSTFLAGS` before running unrelated native host Rust builds.

Use [host-setup](../../docs/host-setup.md),
[ROS1 CLI](../../../documents/command-line/ros1.md),
[ROS2 CLI](../../../documents/command-line/ros2.md) and the
[complete limit schema](../../../documents/ros/capabilities.md).
The String tutorial's generator accepts explicit actual Worker UID/PID,
Gateway UID, private socket, native node/namespace and concrete topic. Select
publish/subscribe explicitly; its JSON `Worker_arguments` contains every
connection/grant/limit argument and matching Gateway configuration. Do not
substitute an example PID for the actual held Worker identity.

String examples require max-message-bytes4096, explicit byte/rate/burst/queue
and audit bounds, and max-publishers/max-subscribers1 as applicable. ROS1 needs
TCPROS/queue-depth and publisher latch; ROS2 needs all eight endpoint QoS fields.
These are tutorial choices, not Runtime defaults. Other lessons need every
field from their operation schema: service/Action deadlines and inflight/event
limits, separate server respond grants, TF frames/finite bounds, parameter
kind/value/element bounds, graph filters and exact lifecycle transition.
ROS2 Action operations require their complete five-channel QoS profiles.

A real native peer must advertise the exact supported type and matching QoS,
serve the configured service/Action, provide the TF frame pair or lifecycle node,
and make discovery ready before a bounded positive exchange is expected.
ROS1 also needs its explicit Master/advertised transport; ROS2 selects explicit
domain/RMW/security. An SDK Node label changes none of these native identities.
No native libraries are linked into the Wasm example.

Default ROS authority is empty. A valid imported backend with an ungranted alias
returns13, and examples preserve it as failure. Enabling ROS without the exact
authenticated Gateway can instead fail startup preflight before main. A missing
import backend is a separate load failure. There is no fallback grant, backend,
wildcard, environment inheritance, directory/socket preopen or implicit clock.
Use explicit WASI environment isolation for ROS execution. Local scheduling's
literal caller times need no WASI clock permission; a real clock needs its own
explicit ROS or WASI grant. After timeout an admitted external effect may remain.

For a reusable polling loop, call spin_once at most once per application step,
treat11 as idle, cap the number of steps with a caller-owned counter/deadline,
and preserve other errors before checked shutdown. The SDK supplies no sleep,
thread, auto-resize/retry on90, or guarantee after a trap/abort.

## Explicit unified events

The next runnable lesson uses an independently opened Source and `subEvents`
observer, checks a single poll, closes the Source, verifies its explicitly
permitted closed event and checks observer cleanup. See
[unified event v1](../../docs/unified-events.md) for exact aliases, header,
limits, backend entries, native peer prerequisites and policy preparation.
Metadata-only observation grants no payload permission. New38/43 inventories
and event source fixtures await the unified serial test stage; historical
receipts keep their original revision and narrower scope.
