# Existing ROS client-library compatibility

Full compatibility with existing rclc/rclcpp/rclpy/rcljava products is a required
project objective. A complete38/43 import list or a Node-shaped class does not
establish that compatibility. Native ROS client libraries expose additional
message, parameter, scheduling, graph, introspection, allocator and lifecycle
contracts. Their exact behavior changes across releases.

The current C/C++/Rust/JS/AssemblyScript/TinyGo capability libraries remain
available. New Java and Python foundations provide real producer-specific
bridges. Opt-in source migration layers are being added separately, so existing
SDK users are not silently switched to an incomplete replacement library.

## Current implementation boundaries

| Family | Current source implementation | Still required for full compatibility |
| --- | --- | --- |
| rcl/rclc | C capability foundation and an [opt-in official-source String/timer/executor profile](../../compat/rclc/README.md), previously compiled with both unchanged upstream examples; the additive [three-family service profile](../../compat/rclc/services/README.md) is independently source-reviewed/integrated, with27 new authored groups still unexecuted | Combined successor regression, remaining public rcl/rclc structures/APIs, actual availability/native request headers, complete allocator/error/wait-set semantics, all generated message lifecycle and full original application/test coverage |
| rclcpp | Existing typed resources, original-source String/topic profile and [additive Service/Future profile](../../compat/rclcpp/services/README.md); the unchanged original service server compiled and 26 service models plus ten topic regressions passed | Complete API/message/options, native availability/wait sets, callback groups, context/executor/timer/parameter/graph semantics across releases and original application/native tests |
| rclpy | Explicit bindings, topic/QoS/lifecycle/timer/logger and an [additive typed service/bounded Future profile](../../python/compat/rclpy/SERVICES.md); 75 local models passed, including the unchanged original service main through an independent bootstrap | Complete Future/Task and Action APIs, native service availability, message introspection, callback groups, parameters/transactions, remapping, native identity and wait-set blocking, all corresponding original tests |
| rcljava | Numeric/typed foundations and opt-in topic/composition/timer plus [Service/Future profiles](../../compat/rcljava/services/README.md); twelve unchanged upstream classes compiled, twenty-eight service groups and the complete original server main passed in local models | Remaining org.ros2.rcljava APIs/generated messages, actual service availability and timed waits, parameter transactions, complete executors/timers/wait sets, native identity and original product/test coverage |
| ROS1 roscpp/rospy/rosjava | ROS1 capability backend and language foundations | Their own source compatibility surfaces and full historical API/native behavior qualification; ROS2 API equivalence is not assumed |

These rows are remaining implementation/acceptance work, not exclusions from
the user requirement. A source-reviewed profile, a compiled guest, local models,
Runtime execution, native ROS exchange and full upstream suites are recorded as
different evidence. Until the last stages pass, full compatibility remains
unqualified. Unsupported features must fail explicitly before creating effects;
they must not become successful empty operations.

The separate `sdk/rmw_wasm` implementation path uses original upstream public
RMW/rcutils/rcl headers and libraries rather than these finite facade structs.
Its static WASI clock/logging/build platform is source-reviewed and integrated;
it has not been configured or run. The real RMW entity, native metadata,
thread/signal, generated default-endpoint and complete original dependency
closures are still being implemented. A missing native provider is a refusal,
and this work does not promote the facade rows to full upstream compatibility.

## Migration without implicit authority

Deployment bindings map an original operation/resource/type/QoS to an explicit
alias. A bootstrap/header can select that map without modifying the original
application's publisher/subscriber API calls. The Host separately grants those
aliases to the actual module identity. A guest mapping cannot change grants,
native endpoint policy, ROS domain, update state or debugger configuration.

Default ROS authority is empty. Native defaults that normally discover a graph,
start parameter servers, access clocks or log through ROS must have separately
defined deployment contracts. Compatibility does not imply wildcard discovery,
socket preopens or a broad catch-all permission.

## Acceptance and performance

For each pinned upstream release, retain original public headers/sources,
license, application sources and complete official test inventory. Compile the
original application against the opt-in layer, record any actual source edits,
and exercise full values/callback order/shutdown/failure behavior with an
independent native peer. Missing APIs, skipped tests and environment failures
remain visible. A handful of minimal examples never substitutes for full suites.

The platform matrix still includes all26 ROS1/ROS2 distributions and all19
requested vendors. Multi-vendor qualification additionally needs simultaneous
independent native adapters, exact distinct aliases and real cross-vendor
application flows; separate single-vendor smoke tests do not prove coexistence.
Cross-architecture tests, including LoongArch, remain separate from a compiler
backend inventory or an emulated executable's successful launch.

Compare the same useful workload and payloads in native and Wasm deployments.
Preserve initialization/JIT time separately from steady execution, exact input
identities, result correctness, memory and latency distributions. Actual VTune
hardware Hotspots and Microarchitecture Exploration must have attributable
samples/counters for the selected workload. Empty collections, timer-only
profiles, concurrent runs and QEMU host costs cannot be labeled native guest
hardware performance. Slow workloads require evidence-backed optimization and
a repeated comparison; no measured ratio is currently claimed by this document.

Upstream interface references: [rclc](https://github.com/ros2/rclc),
[rclcpp Node](https://docs.ros.org/en/jazzy/p/rclcpp/generated/classrclcpp_1_1Node.html),
[rclpy Node](https://docs.ros.org/en/jazzy/p/rclpy/rclpy.node.html),
[rcljava Node](https://github.com/ros2-java/ros2_java/blob/main/rcljava/src/main/java/org/ros2/rcljava/node/Node.java).
Actual qualification must pin immutable source identities rather than treating
these moving documentation links as a version lock.
