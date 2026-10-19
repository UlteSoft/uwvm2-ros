# Current interface limits and evidence boundaries

This inventory supplies original delivery output22, the remaining technical
limitations. It describes the current source contract; it is not a compilation,
execution or acceptance receipt. The accompanying [capability guide](current-capabilities.md),
[client-library coverage](../../sdk/docs/client-library-coverage.md),
[bounded custom profile](../../sdk/custom-types/BOUNDED_V2.md) and
[unified event contract](../../sdk/docs/unified-events.md) define the supported
interfaces in more detail.

`UNSUPPORTED` below means that the named operation or input is outside an
explicit source-supported interface. It does not claim that a rejection test
has run. `SCOPE_BOUNDARY` describes the extent of an implemented contract and
does not declare that contract broken. Actual `PASS`, `FAIL`, `BLOCKED` and
observed unsupported refusals must come from the current frozen source's
terminal reports, including their native effects, architecture, permissions,
input preservation and cleanup evidence. Preparation or a historical receipt
cannot supply a missing current execution.

No entry here waives an original required acceptance item. A required item that
is unimplemented, unexecuted, failed or blocked must keep final completion
false. An explicitly permitted bounded source profile still needs its actual
positive and refusal experiments; documenting its bounds is insufficient.

## Interface inventory

| ID | Source classification | Exact boundary and implementation evidence |
| --- | --- | --- |
| L01 | SCOPE_BOUNDARY | The six libraries implement this Runtime's WasmROS imports and local client concepts. Native rclcpp/rclc/rclpy/roscpp source, binary, ABI and plugin compatibility are not promised. See the [coverage contract](../../sdk/docs/client-library-coverage.md), especially its public-layer and backend boundaries. This is not a claim that the implemented Topic, Service or Action wrappers are absent. |
| L02 | UNSUPPORTED | Unregistered native types, arbitrary runtime-discovered IDL/CDR and dynamic middleware/plugin loading are outside the custom interface. Native adapters and canonical identities come from trusted frozen build outputs. The compiled registry lookups are in [custom_type.h](../../src/uwvm2/imported/ros/ipc/custom_type.h) (`custom_topic_entries`, `find_custom_topic`) and [custom_interface.h](../../src/uwvm2/imported/ros/ipc/custom_interface.h) (`find_custom_service`, `find_custom_action`). An unknown type is never permission to load an adapter. |
| L03 | UNSUPPORTED | Bounded-v2 accepts complete trusted `.msg`, `.srv` and `.action` definitions. It refuses unknown/recursive dependencies, constants/defaults, ambiguous byte/char aliases, implicit Header/time/duration, wide strings and unreviewed IDL preprocessing. Its [parser and ceilings](../../sdk/custom-types/bounded_profile.py) (`parse_fields`, `load_catalog`, `MAX_TYPES`, `MAX_NODES`, `MAX_TOTAL_NODES`, `MAX_WIRE`, `MAX_DEPTH`) enforce the finite profile. The older fixed-v1 profile retains its separate closed subset; this entry does not remove it. |
| L04 | SCOPE_BOUNDARY | Canonical custom values have finite string/sequence/nesting bounds, at most128 interfaces in the historical bounded profile (512 in the explicitly paired complete-vendor profile),4096 expanded nodes per component,16384 aggregate nodes,16 nested levels and32768 canonical bytes per component. Bool is0/1, floats are finite and UTF-8 is strict/NUL-free. These are profile maxima, not permission or allocation defaults; a grant may be smaller. Empty Service/Action components are supported with their normal ticket/Goal envelope, while empty native Topics are refused. See [bounded_profile.py](../../sdk/custom-types/bounded_profile.py) and the [complete wire profile](../../sdk/custom-types/BOUNDED_V2.md). |
| L05 | UNSUPPORTED | The deadline-bound ROS1 Service transport does not perform arbitrary DNS lookup. Its Master and returned `rosrpc://` endpoint must use a numeric IPv4/IPv6 address or the explicitly recognized `localhost` spelling; an unsupported endpoint returns95 before sending the Service request. This restriction is specific to [bounded_service.h](../../gateways/ros1/bounded_service.h) (`numeric_address`, `endpoint`, `call`); it is not a statement that all ROS1 transports reject hostnames. The finite response, MD5/type, timeout, close and malformed-body paths remain supported and require their own actual cases. |
| L06 | UNSUPPORTED | ROS2 parameter declare/undeclare operates on the explicitly selected local Gateway node. The remote branch returns95 instead of inventing a remote declaration service or an atomic conditional deletion operation. See [resources.h](../../gateways/ros2/resources.h), `ros2_resources::call`'s `param.declare`/`param.undeclare` remote branch, and the [ABI reference](../../sdk/docs/abi-v1.md). Supported remote get/set/list/describe and parameter events are not classified as unsupported by this entry. |
| L07 | SCOPE_BOUNDARY | Queue limits and backpressure bound this Runtime's Source/observer queues, reservations and one explicitly budgeted pending record. They do not bound every DDS/TCPROS/RMW allocation, automatically throttle a remote publisher or guarantee that reliable QoS prevents application-level refusal. See [event_queue.h](../../src/uwvm2/imported/ros/ipc/event_queue.h) (`event_stream_settings`, `enqueue`, reservations/statistics) and [unified events](../../sdk/docs/unified-events.md). Native transport buffers can exist before canonical callback validation, as documented in the [bounded profile](../../sdk/custom-types/BOUNDED_V2.md). Deployment memory/network budgets remain separate requirements. |
| L08 | SCOPE_BOUNDARY | Local Context/Future/Executor/Timer scheduling is caller-driven. It creates no implicit native RPC, thread, clock, retry or stronger cancellation permission. Separately granted `service_submit` is a real bounded asynchronous admission/completion path, distinct from merely deferring the synchronous import. Neither local cancellation nor native timeout promises rollback of an already admitted remote effect. See [scheduling.mjs](../../sdk/javascript/scheduling.mjs), [async_service.h](../../gateways/common/async_service.h) and the [service submission contract](../../sdk/docs/unified-events.md). Queued jobs, admitted jobs, discarded completions and owned-thread teardown have separate observable obligations. |
| L09 | SCOPE_BOUNDARY | Typed SDK busy guards protect the owned facade while its callback or await is active. Raw imports/numeric-handle escape paths and independent sibling owners remain governed by the Runtime, rather than those local guards. Caller-owned decoded storage and registered callback owners must outlive their use. Guest destructors/finally blocks cannot be assumed to run after a fatal Wasm trap; the Runtime/Gateway's independent trap cleanup is still a mandatory actual test. See the [ownership contract](../../sdk/docs/client-library-coverage.md), [JavaScript ownership leases](../../sdk/javascript/ownership.mjs) and [C++ owners](../../sdk/include/roswasm/roswasmcpp.hpp). |
| L10 | SCOPE_BOUNDARY | JavaScript host code runs outside the Wasm engine. The persistent relay forwards real imports from one retained Runtime Instance; it does not turn an arbitrary Node object, Memory or native ROS connection into an authenticated Instance. Its compatibility digest is not process authentication. The trusted supervisor must bind the actual Worker birth, Gateway peer credentials, policy and exclusive pipes. See [relay.mjs](../../sdk/javascript/relay.mjs), [relay.c](../../sdk/javascript/relay.c) and the [relay guide](../../sdk/javascript/RELAY.md). No default ROS authority is added by attaching a Session. |
| L11 | SCOPE_BOUNDARY | A language compiler/runtime may introduce WASIp1 imports beyond a handwritten SDK call. Each actual binary's imports and startup rights must be inspected and explicitly selected. In particular the supplied TinyGo recipes use `-scheduler=none`; any startup entropy requirement needs separate explicit authority. The SDK does not grant random, clock, file, environment or network rights to make a tutorial start. See [client-library coverage](../../sdk/docs/client-library-coverage.md) and [build_delivery.py](../../tests/ros_acceptance/build_delivery.py)'s explicit startup-right selection. A compiled artifact alone proves no native ROS delivery. |
| L12 | SCOPE_BOUNDARY | Qualification covers the explicitly selected SDK, native type metadata, products, input pins and observed process mappings. It is not an exhaustive instruction trace or a transitive proof of every possible dynamically loaded dependency. The [native loader helper](../../tests/ros_aarch64/native_runtime_environment.py) checks its closed selected custom-library directory; it does not claim a complete DT_NEEDED graph. A different SDK/RMW/toolchain requires fresh compatible qualification and actual execution evidence. |

These boundaries do not weaken default denial: constructing a Node, enabling a
backend, compiling a codec, observing metadata or acquiring one operation role
does not create another grant. Rights, resource names, native identities,
operation-specific quotas and QoS remain explicit caller selections.

## Reading the eventual final report

The final package must pair this inventory with the actual current serial
runner and outer-supervisor receipts. Report each required experiment's
terminal result, and retain failed attempts even if a later frozen revision
succeeds. An unsupported source form is a build-time refusal; do not invent a
Runtime errno for it. Numeric95 claims in L05/L06 require actual matching
refusal cases before they can be reported as observed behavior. Missing tools,
a preparation that has not run, or an unfinished test are not themselves proof
that the underlying feature is unsupported.

The current authorized Linux budget is 64 GiB with swap disabled. The current
boot3 admission and actual results are recorded in the
[validation checkpoint](validation-checkpoint-20261016-boot3.md). Re-measure the
actual boot, leaf, device, inode, all readable ancestor constraints and OOM
state before execution; earlier 48 GiB and previous-boot observations remain
historical evidence.
QEMU aarch64 results are execution under simulation and do not establish
physical ARM performance. An SSH CI-equivalent serial lane can exercise the
required profiles/regressions without claiming execution on hosted GitHub
Actions. See the [acceptance lane](../../tests/ros_acceptance/README.md).
