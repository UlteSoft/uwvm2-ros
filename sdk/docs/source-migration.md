# Migrate existing ROS client applications

Choose a source compatibility route before changing an application. Keep the
original business source, select its target headers/packages separately, and
bind requested endpoints to explicit Host aliases. Source compatibility,
native binary compatibility and real ROS interoperability are different
properties. Each requires its own evidence.

## Available routes and their current boundaries

| Existing application | Source/build selection | Actual Guest implementation | Boundary to preserve |
| --- | --- | --- | --- |
| C using the finite rcl/rclc profile | `compat/rclc/include`, `src/compat.c`, one selected binding | C capability SDK with checked allocators, owners, codecs and bounded dispatch | Exact supported original calls and explicit type/QoS/role mappings |
| C++ using the finite rclcpp profile | `compat/rclcpp/include` before normal Guest headers | Checked C++ capability SDK | Supported original Node/publisher/subscriber/timer/service source profiles; missing APIs fail explicitly |
| Original upstream rcl/rclc/rclcpp libraries | Complete pinned upstream trees and `rmw_wasm/cmake` cross build | Static Guest RMW, platform and typesupport adapters | Original headers and generated Guest layouts; complete original default endpoints and OS/thread semantics |
| Python using rclpy | `python/compat/rclpy` in the frozen Guest package closure | Real CPython.wasm plus built-in `_roswasm_native` | Python package names, supported source semantics, exact request/Future/callback lifecycle |
| Java/rcljava source or JAR | `compat/rcljava` plus `java`, lowered through the real Java producer | TeaVM linear WASI and numeric ROS imports | Bytecode reachability/classpath, owned linear buffers and supported original package API |

These routes implement the finite profiles in their own ledgers. They do not
establish every upstream API, distro, message family or executor behavior.
[The shared porting contract](../compatibility/rcl/GUEST_PORTING.md) and each
profile's coverage/gap ledger list required remaining implementation work.
Do not treat an unsupported API as a successful empty operation.

## C and C++ source-level migration

Start by inventorying includes, generated messages, linked libraries, native
plugins, allocator assumptions, threads, clocks, signals, waits and callbacks.
Resolve each dependency with [the native library guide](native-library-porting.md).
Choose exactly one of the two C/C++ routes above.

For a finite facade build, keep the application TU unchanged and select the
compatibility include root only for its Wasm build. Compile the actual adapter
implementation exactly once where required. A separate immutable selection
maps each original node/resource/type/operation/QoS request to one alias.
The default selection is empty. A present selection does not grant that alias
in the Runtime.

The [rclc guide](../compat/rclc/README.md) supplies exact compiler flags and the
original complete executor/context examples. The
[rclcpp guide](../compat/rclcpp/README.md) supplies the original complete String
publisher/subscriber TUs, actual headers and a native peer. Their service
profiles have separate recipes and acceptance obligations. Do not mix facade
structs or objects with native rcl/rmw libraries.

For an original-library build, acquire complete upstream source trees and all
real generator inputs, not just research excerpts. Build the original package
graph for Wasm with real target libc/C++ archives and generated introspection
providers. Select the static RMW implementation explicitly. Preserve original
rosout, parameter and type-description behavior unless the application's own
options change it. Every endpoint created by those defaults needs an actual
codec, native identity and grant. A failed node initialization does not qualify
a publisher. The explicit original-rclcpp composite requires the actual static
thread/platform/signal and genuine C lifecycle source closures; its build option
defaults OFF. The [default-control source](../rmw_wasm/default_control/README.md)
retains all original endpoint roles, including the TimeSource subscription.
These implementations require actual SDK/Guest/Native qualification; the
separate facade must not be substituted silently.

## rclpy inside CPython.wasm

The application remains Python. The migration package supplies the supported
`rclpy`, message and service modules inside the Guest interpreter. The actual
bridge is a statically linked C extension that calls numeric Wasm imports;
it is not the native `_rclpy` extension, host Python, ctypes or a socket relay.

1. Select the actual matching CPython source, native build-Python, target
   `pyconfig.h`, target `libpython.a` and the HLL producer's ordered link flags.
2. Freeze the SDK, supported compatibility packages, pure Python stdlib and one
   explicit application entry with [freeze.py](../python/tools/freeze.py).
   Preserve package names; do not freeze upstream audit/test files as apps.
3. Compile `_native.c` with one `ROSWASM_BACKEND`; compile the real embedding
   launcher with the frozen header; link against the actual target interpreter.
4. Install the exact immutable deployment binding before `rclpy.init`. Missing
   resource/type/QoS bindings refuse; the Host independently checks its policy.
5. Test real default denial, positive native delivery, service/Future completion,
   callback-time lifetime refusal, errors and complete shutdown.

The supported profile includes selected topic and service source patterns.
Native wait sets, arbitrary generated message introspection, every Action and
parameter API, dynamic extensions and all concurrent executor semantics remain
separate implementation work. A frozen Python import or local CPython model
does not prove target-Wasm startup or ROS delivery. See
[the Python guide](../python/README.md) for exact buffer/error contracts.

## rcljava and existing Java application JARs

Use the Wasm-compatible `org.ros2.rcljava` classes in a separate migration
classpath. Never combine them with a native JNI rcljava JAR bearing the same
class names. Keep the application classes and their dependency inventory.
The current concrete backend is TeaVM 0.13.1 `WEBASSEMBLY_WASI`, with linear
memory and exact numeric `@Import` declarations. WasmGC references cannot be
reinterpreted as linear offsets.

For source examples, [the rcljava project](../compat/rcljava/README.md) supplies
the POM, original application classes and a separately selected deployment
bootstrap. For an existing JAR, the HLL
[`produce_jar.py`](../../test/0050.high_level_languages/java/jar-input/produce_jar.py)
entry accepts the application JAR, explicit dependency JARs and a main class
from its manifest or `--main-class`. It uses the real TeaVM Maven compile goal
and the selected private compiler repository. Select only one set of SDK and
compatibility classes; duplicate/shadowed native classes are refused.

Use the [SDK existing-JAR plan and artifact gate](../compat/rcljava/tools/JAR_INPUT.md)
to bind the selected compatibility classes, deployment bootstrap, explicit
application/dependency JARs and actual producer. For the optional
`isServiceAvailable`/`waitForService` migration, follow the separate
[availability profile](../compat/rcljava/availability/README.md): it binds an
explicit availability/source pair, a finite wait policy and the required real
clock rather than inferring permission from a client name. Its optional ABI
catalog and import signatures are separate from the unchanged original
business ABI. Existing constructors without that explicit profile retain their
documented unsupported-query behavior. All of these new source routes still
need actual Linux JAR, Wasm, native ROS and DBG qualification.

The JAR producer is newly integrated source, with real future positive and
negative tests; it still requires actual Linux translation, Guest, DBG and
performance qualification. Supplying a ROS import ceiling to that producer
does not create an SDK bridge or grant ROS authority. An alternate Java IR
frontend must implement the same numeric import and rooted-buffer semantics;
it cannot substitute JNI execution on the Host.

The original class-file line/local tables and supplied source roots constrain
debug information. Missing bytecode debug attributes cannot be recovered by
changing the Wasm filename. A JVM source/model check is not a target-Wasm result.

## Configure permissions after migration

The deployment administrator chooses exact module names and an explicit
Instance/Single/Group policy. Group members receive independent instance
capability tables; they do not share transferable handles. A Single/Group
binding is authoritative even if disabled or empty; it does not fall back to
Global permissions.

The application-local selection maps an original API call to an alias. The
Runtime then authenticates the actual calling module/incarnation and checks
that alias on every operation. Native resource/type/QoS, byte/rate/queue/timeout
limits and ROS connection/security settings remain Host controlled. Separate
roles require separate aliases: publish/subscribe, service advertise/respond,
Action send/feedback/result/cancel-own/cancel-any, clocks, logs and observers.

Validate and retain the effective policy before execution. First run the same
artifact with empty Host grants and verify a real operation returns denial
without a native side effect. Then grant only the required roles and establish
independent complete native observations. See [host setup](host-setup.md) and
[security and audit](security-and-audit.md).

## Preserve behavior, then measure it

Run the original native and migrated application against equivalent native
peers, data and QoS. Compare complete messages, service responses, Action IDs,
parameter/TF semantics, timer phase, callback order, error propagation and
resource cleanup. Identify every changed source or unsupported semantic.

WasmDBG must inspect the actual producer's source/value capabilities. ROS2
debugging additionally checks the correct module/instance, denied operations,
owned resources/events and post-update retired state. Benchmark startup/cold
compile/warm cache/execution separately; use the
[platform qualification protocol](platform-qualification.md). Passing a minimal
publisher does not qualify the rest of an application dependency graph.
