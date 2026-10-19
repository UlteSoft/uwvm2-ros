# Port existing ROS client source into a Wasm Guest

The compatibility target is old application source rebuilt for a Wasm Guest and
executed by uwvm2 under its actual per-module capability policy. It is not native
ROS binary ABI compatibility inside the Guest. A native ROS pointer, a JNI
handle, a CPython object header and a Java GC reference are not Guest linear
memory offsets or permissions. Native ROS counterpart programs run outside the
Guest, use actual installed native SDKs, and provide independent delivery and
identity evidence; their successful execution cannot qualify a Guest library.

`guest-porting-v1.json` records the actual source paths and open implementation
work. `guest-build-recipes-v1.json` records argv-level build stages and required
physical input roles. These files do not execute tools, choose Host grants or
predict any product hash. `sdk/tools/check_guest_porting.py` checks this source
contract and its exact reference hashes without importing a candidate. The
caller must separately own each compiler/process tree, cgroup, disk/memory
budget, deadlines, inputs, outputs and cleanup. No build was run by this cohort.

## Choose the actual source route

There are two C/C++ routes. They must not be mixed on one include/link path.

* The finite `sdk/compat/rclc` and `sdk/compat/rclcpp` profiles preserve supported
  official application source spellings while using the capability C/C++ SDK.
  Their replacement structures are rebuilt Guest objects, not a native RCL ABI
  shim. They cover the explicitly recorded topic/timer/service subsets. Missing
  APIs, callback behavior, messages and official tests are still required work.
* `sdk/rmw_wasm` builds real upstream rcutils/rcl/rclc/rclcpp and real generated
  type-support sources for the Wasm target, with an explicit Guest RMW provider.
  Its generated C/C++ pointers and introspection layout belong to that rebuilt
  Guest module. They are never forwarded as host addresses. Native metadata and
  canonical bodies cross only the separately admitted owned transport. A
  configure result or source-reviewed provider is not full upstream acceptance.

For Python, existing source imports the in-Guest `rclpy` profile. The real
CPython.wasm interpreter runs that Python code. Its built-in `_roswasm_native`
extension retains owned byte buffers and calls numeric ROS Wasm imports. A
native `_rclpy` wheel, host Python, ctypes, a subprocess or a socket relay is not
an alternate success path. The generic CPython producer remains owned by the
high-level-language task; the ROS extension, frozen package selection and
compatibility implementation live under `sdk/python`.

For Java or an application JAR, the intended route is application Java bytecode
through the HLL producer's Java IR and Wasm lowering, with the source-compatible
`org.ros2.rcljava` implementation and ROS raw imports in the same Guest. The
current concrete SDK/POM backend is TeaVM0.13.1 `WEBASSEMBLY_WASI`: its `@Import`
methods become numeric Wasm imports, and `Address.ofData(...).toLong()` supplies
zero-extended linear memory offsets. The current code is not a generic
producer-independent JavaIR bridge. An alternate HLL JavaIR frontend must
explicitly provide and verify those import and owned linear-buffer semantics;
it must not reinterpret WasmGC objects or call a native JNI rcljava library.
The example POM demonstrates selected source examples. The separate implemented
[existing-JAR entry](../../compat/rcljava/tools/JAR_INPUT.md) binds original
application/dependency JARs to the actual linear-WASI HLL producer. It prepares
an explicit ordered build plan and post-link gates; preparation is not a compiler
or Guest execution result. Unsupported JNI/native dependencies still require
source adaptation and must fail their actual producer/API admission.

## C and C++: preserve business source, select authority separately

Use the exact application TU as an ordinary main in each separate Guest. Only
local model links rename mains; those model symbols are not the production
recipe. The existing actual source recipes are:

* `sdk/compat/rclcpp/README.md`: original String publisher/subscriber with the
  actual `selected_stdio_binding.hpp` and imported ROS2 backend;
* `sdk/compat/rclcpp/services/recipe.json` and its README: original service server
  and separate explicit-ready asynchronous client lesson;
* `sdk/compat/rclc/recipes.json` and `services/README.md`: compile `compat.c` once,
  compile the explicit selection once, and link each unchanged original main as
  a separate Wasm command;
* `sdk/rmw_wasm/cmake/BUILD.md`: the real upstream static cross-package route.

The original-rclcpp hook now delegates to the explicit
`sdk/rmw_wasm/original_build/CompositeOriginal.cmake` source composition. Its
`UWVM_BUILD_ORIGINAL_RCLCPP_WASM` option defaults OFF. Enabling it requires the
genuine threaded static WASI toolchain, coherent core/platform/dispatch targets,
real executable-link admission for the signal and genuine-C lifetime helpers,
and the exact fixed original source ports. Those source requirements do not
prove an actual build, unchanged default Node startup or Native delivery. The
full default-control codecs, TimeSource dependencies and explicit endpoint
grants must also be complete; do not suppress defaults to pass a partial test.

The accompanying argv recipes add explicit target/sysroot roles to the finite C
recipe and keep all build products outside the source tree. Use a real selected
WASI C11 or C++20 compiler and its matching sysroot/libc/libc++ archives. Include
`third-parties/fast_io/include` for the actual C++ profile. Never place a native
ROS installation or the finite facade include root into the real upstream RMW
cross-build. The latter has actual static typesupport/OS-dependency requirements
and must fail on missing packages/providers rather than linking host objects.

A local selection is not policy. Each endpoint role binds the exact resolved
node/resource, complete native type/canonical codec and requested QoS to an
explicit alias. Logging, clocks, observation and availability are independent
roles. Default grants remain `[]`. Original upstream NodeOptions defaults are
not quietly made "quiet": if real upstream source creates rosout, parameter or
type-description endpoints, it needs the complete actual grants/codecs. Only an
application's own real quiet options may suppress those effects.

Stdin credits in the finite examples are a bounded driver, not a DDS waitset or
a nanosecond clock. True wait/sleep/availability require their independent real
capability contracts. The currently documented unsupported waits must not be
removed from original clients just to obtain a passing main. An edited
explicit-ready lesson is a distinct source/artifact and earns only its own
credit. No graph listing or empty request proves availability.

## Python: freeze the application and link the actual interpreter

The current producer contract is CPython3.14.8/WASI SDK24, as recorded in the
actual HLL `python/versions.json`. The matching Linux native build-Python runs
`sdk/python/tools/freeze.py`; a different host bytecode version is refused.
Supply the real CPython source `Lib`, a private SDK staging tree, one explicit
entry and a fresh frozen output. The five compatibility package roots are
`rclpy`, `std_msgs`, `example_interfaces`, `std_srvs` and
`examples_rclpy_minimal_service`. Preserved upstream/audit/developer tests are
not automatically frozen. The application is only `--entry` as `roswasm_app`.
The official service bootstrap retains the unchanged original package main.

Compile `sdk/python/roswasm/_native.c` for Wasm with an explicit
`ROSWASM_BACKEND=1` or `2`, the real target `pyconfig.h`, CPython `Include`, and
`sdk` include root. Compile `embedding/main.c` with the generated frozen header.
Link both objects with the actual target `libpython.a` and the target producer's
recorded ordered link flags/libraries. The HLL producer already derives
`LINKCC`, `PY_CORE_LDFLAGS`, `LIBRARY`, `LIBS`, `MODLIBS`, `SYSLIBS` and
`LINKFORSHARED` from its actual generated Makefile. Reuse the admitted actual
values; do not invent a short generic library list or execute a target
interpreter on the host to discover them. This document leaves that toolchain
response file as an explicit actual input, not a fake generated product.

`PyInit__roswasm_native` is registered before interpreter initialization. The
isolated launcher disables environment/site search and imports the frozen app;
it does not need a native ROS module. The extension pins input with `Py_buffer`
and owns output `bytes` until the import returns. Every memory argument is a
Guest linear offset. Pure Python freezing does not supply missing WASI native
extensions, processes, sockets, arbitrary threading or asyncio transports.
Compile/link/stdlib startup and the full ROS cases remain separate obligations.

## Java: preserve class/source semantics through the actual producer

Stage the SDK layout under a private directory, including `sdk/java` and
`sdk/compat/rcljava`. The current POM selects the exact original example classes
and the compatible package, not the preserved JNI implementation evidence.
Choose `OfficialBootstrap`, `DefaultDeniedBootstrap`, or the exact service
bootstrap with `fixture.main`; build each in a fresh project output. The current
POM compiles Java17 and lowers through pinned TeaVM0.13.1 WASI. Admit the actual
native Java/Maven/TeaVM tools and complete offline Maven dependency cache.

For an existing application JAR, use
[`sdk/compat/rcljava/tools/plan_jar.py`](../../compat/rcljava/tools/plan_jar.py)
with the actual separate HLL `java/jar-input/produce_jar.py` and its complete
tool/helper closure. Retain the original JAR/classes and explicit dependencies;
the entry rejects conflicts with supplied SDK/facade classes instead of choosing
a duplicate or stripping a fat JAR. The actual producer resolves original class
bytecode and debug attributes through TeaVM0.13.1 WASI, with no JVM application
execution, JNI substitution or invented bytecode/source reconstruction.

The source contract binds56 actual SDK/facade Java files. An explicitly selected
deployment bootstrap adds one separate compile input (57 total); it calls the
reviewed original main and installs local mappings, not Host permissions. The
ordered plan compiles/packages the SDK and optional bootstrap, invokes the real
producer, admits its actual artifact/retirement receipt, validates that same
Wasm and checks its exact numeric ROS imports and linear-memory contract. Read
[JAR_INPUT.md](../../compat/rcljava/tools/JAR_INPUT.md) for every required input,
refusal and stage gate. All current ROS JAR build/Guest/Native qualifications
remain pending; a prepared JSON plan cannot supply those results.

Actual `Ros1Raw`/`Ros2Raw` declarations use Java int=i32 and long=i64. The active
`LinearMemory.Frame` roots complete arrays through one synchronous import.
The producer must preserve all unsigned handle bits, zero-extend addresses, and
keep GC references distinct from linear offsets. WasmGC language benchmarks do
not establish this ROS bridge. JVM contract tests and javac source checks test
local values/lifetimes only. A `.class` or `.jar` is not a Wasm execution result.

## Post-link admission and evidence levels

Validate the entire Wasm with an actual validator. Run the current bounded
`sdk/tools/check_wasm_imports.py` against the selected backend and actual allowed
WASI imports. Use `--require-all` only for a deliberate raw inventory fixture:
a typed application may dead-strip unreachable imports. The source catalog
contains38 ROS1 and43 ROS2 functions. Memory index is i32; offsets, lengths,
handles and correlations are i64; output slots are8-byte little endian. On
wasm32 addresses are zero-extended. Optional original-RMW transport namespaces
are separately versioned/admitted and do not silently replace or renumber81.

No Guest imports configure debugging, updates or Host policy. A default-denied
artifact must link and start, invoke a real checked ROS operation and observe13;
a loader error is not denial. Positive evidence needs actual current Runtime,
real grants, the same retained native endpoints, complete codecs/type identity/
QoS, full canonical values and independent Native observations. Preserve
submit/event correlation, one-close cleanup, unknown effects after timeout,
no replay and exact source/artifact identity. Do not label local model or Native
counterpart success as Guest compatibility. Do not overwrite historical receipts
when source changes.

The machine ledger lists remaining full legacy API, message, callback, parameter,
action, scheduling, packaging and version coverage as implementation obligations.
They are not permanent exclusions. This cohort adds a checked strategy/build
contract only; it neither implements missing APIs nor claims compatibility or
performance qualification.
