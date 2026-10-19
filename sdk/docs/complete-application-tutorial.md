# Build a complete WasmROS application

An application consists of a real Wasm module, its complete language runtime
and libraries, explicit resource/type bindings, a trusted Runtime policy, a
native ROS Gateway, and an independent ROS peer. A successful compile is the
first part of this chain. This tutorial connects the language examples to the
deployment and acceptance steps; it does not mark an unexecuted route as passed.

For an existing ROS application, first read
[source migration](source-migration.md). For an original native library, read
[native library porting](native-library-porting.md). Keep each route's finite
API contract visible when choosing an implementation.

## Install all headers, examples and language packages

Use the complete `sdk/` tree. The public C forwarding headers depend on sibling
`common/`, `ros1/` and `ros2/` directories; copying `include/` alone breaks that
layout. The SDK's [CMake package](../CMakeLists.txt) installs the complete source
tree, including compatibility headers, language packages and examples. Generated
build trees, dependency directories and local verification reports are excluded.

Run configuration and installation with the actual Linux task controller:

```sh
cmake -S "$SOURCE/sdk" -B "$OUT/sdk-package" \
  -DCMAKE_INSTALL_PREFIX="$OUT/install"
cmake --install "$OUT/sdk-package"
```

`SOURCE` is the complete checkout. `OUT` is a fresh owned output directory.
Set `SDK="$OUT/install/share/roswasm/sdk"` for the installed tree. A consuming
CMake application can use `find_package(roswasm CONFIG REQUIRED)` and link
`roswasm::c`, `roswasm::cpp`, or the optional header-only `roswasm::unitree`.
Select the consuming application's real Wasm toolchain separately. These
interface targets do not locate native ROS, start a Gateway or grant authority.

## Define one application contract

Start with one bounded String publisher/subscriber pair. Pick an explicit
backend, node name, namespace, full topic, native type identity, QoS, maximum
message size, queue depth and finite execution budget. Use two independent
aliases, for example `publish` and `subscribe`. Bind the example's exact alias
spelling; the names `pub`, `publish` and a ROS topic are not interchangeable.

For a service, add separate call or advertise/respond roles. For an Action,
declare send, feedback and result roles and select cancel-own separately from
cancel-any. Graph, parameters, TF, clocks, logging and lifecycle transitions
have their own contracts. The String grant does not supply these operations.

The raw ABI uses an i32 memory index and i64 offsets, lengths and opaque
handles, including on wasm32. A successful open writes an eight-byte
little-endian handle. Retain all 64 bits. Codecs encode canonical values;
native pointers, object headers, JNI references and native ROS structs are
never payloads. Read the [ABI reference](abi-v1.md) and the selected language's
ownership/error documentation before adding custom messages.

## Choose the real language producer

The following table identifies complete starting applications and the required
producer. It is a routing table, not a statement that every producer/artifact
combination has completed native ROS acceptance.

| Language | Application and API | Actual producer and important requirement |
| --- | --- | --- |
| C | [C lessons](../examples/c/README.md), `include/roswasm/roswasmc.h` | WASI C compiler and libc; explicit ROS1/ROS2 selection |
| C++ | [C++ lessons](../examples/cpp/README.md), `include/roswasm/roswasmcpp.hpp` | WASI C++20 compiler and target C++ archives; selected exception/RTTI profile |
| Rust | [Rust applications](../examples/rust/README.md), `rust/` | Real `wasm32-wasip1` target, selected `ros1`/`ros2` feature and actual target libc |
| TinyGo | [TinyGo applications](../examples/tinygo/README.md), `tinygo/` | Pinned TinyGo WASI target and `ros1`/`ros2` build tag |
| AssemblyScript | [AssemblyScript applications](../examples/assemblyscript/README.md), `assemblyscript/` | Real `asc` output; `_start` plus checked application status |
| JavaScript | [in-Guest applications](../examples/javascript_guest/README.md), `javascript/guest.mjs` | QuickJS compiled into the same Wasm with a closed embedded module table |
| Java | [typed topic application](../examples/java/TypedTopics.java), `java/` | TeaVM linear WASI; real ordered bytecode/classpath and rooted linear buffers |
| C# | [C# applications](../examples/csharp/README.md), `csharp/` | Mono/WASI plus the matching statically linked `roswasm_bridge.o` and P/Invoke scan |
| MoonBit | [MoonBit applications](../examples/moonbit/README.md), `moonbit/` | Real Wasm producer with the reserved ordinary-memory arena and verified final layout |
| Python | [Python applications](../examples/python/01_typed_topics.py), `python/` | Real CPython.wasm, built-in `_roswasm_native`, frozen SDK, stdlib and application |

### C and C++

Compile a real selected source file, keeping the SDK layout intact:

```sh
"$WASI_CC" --target=wasm32-wasi --sysroot="$WASI_SYSROOT" \
  -std=c11 -O2 -g -I "$SDK/include" \
  -DROSWASM_EXAMPLE_ROS_VERSION=2 \
  "$SDK/examples/c/01_raw_imports.c" -o "$OUT/raw-ros2.wasm"
```

The raw lesson attempts an actual open, publication and close. Without its
grant it reports permission denial; exit13 is not a positive delivery result.
Move to the node/executor and session-ownership lessons for bounded callbacks
and cleanup. For C++, use the selected example's C++20, target archive,
exception and RTTI recipe. Do not link native `librclcpp.so` into a Wasm build.

### Rust and TinyGo

The [Rust/TinyGo guide](rust-tinygo.md) supplies the exact local package layout,
feature/tag selection and linker requirements. Rust uses `no_std` with optional
allocation and a real target libc for compiler-emitted primitives. Keep its
documented encoded Rust flags, one codegen unit and bounded linker threads.
Do not replace required libc symbols with handwritten stubs to obtain a link.

From a prepared copy of `examples/rust`, a ROS2 node build selects
`--target wasm32-wasip1 --no-default-features --features ros2,alloc --bin node`,
offline dependencies and `-j1`. From `examples/tinygo`, a node build uses:

```sh
"$TINYGO" build -target=wasi -tags=ros2 -o "$OUT/tinygo-node.wasm" ./node
```

The local `go.mod` must still resolve `uwvm2.local/roswasm` to the selected SDK.
Ordinary host Go execution is not evidence for TinyGo's Guest ABI.

### AssemblyScript and actual JavaScript

The AssemblyScript node lesson can be lowered with its real abort handler:

```sh
"$ASC" "$SDK/examples/assemblyscript/03-node.ts" \
  --outFile "$OUT/as-node.wasm" --runtime incremental \
  --use "abort=$SDK/assemblyscript/api/abortGuest"
```

Use the ROS1 subdirectory for that backend. Its void `_start` cannot return the
application status: the acceptance driver must also call the documented
`lastStatus` export and require zero. Process exit0 by itself is insufficient.

AssemblyScript is a typed language compiled to Wasm. It does not execute
arbitrary JavaScript source, and it is different from asm.js. To run JS logic
inside the sandbox, use [QuickJS-in-Wasm](../javascript/guest/README.md).
Preparation selects one real recipe and explicit source/tool inputs, emits
ordered compile/link jobs, and labels the result `PREPARED_NOT_COMPILED`.
Execute those jobs, fully validate the final Wasm, then run native acceptance.
Keep engine heap, C stack, job and interruption budgets plus an external
deadline. Promise callbacks remain in that same Guest principal.

The [external Node relay](../javascript/RELAY.md) and supplied-memory bridge
remain separate embedding choices. Node and an external JS driver have their
hosting process authority. Their successful execution cannot qualify a JS
application as executing inside Wasm.

### Java and JAR input

The [SDK Java project](../examples/java/pom.xml) compiles selected SDK/example
classes and lowers `fixture.main` with TeaVM0.13.1's WASI backend. Select the
typed topic class explicitly and preserve Unicode input/output and exact
class-path evidence. Keep `Address.ofData(...).toLong()` zero-extension,
strong roots and the synchronous frame lifetime; Java object addresses are
not Host addresses.

The HLL [JAR input producer](../../test/0050.high_level_languages/java/jar-input/README.md)
adds a real application/dependency JAR route. It requires the pinned patched
producer, toolchain receipt and complete ordered classpath. Supplying
`--ros-backend 2` selects an import ceiling; it does not grant ROS resources.
The SDK [existing-JAR entry](../compat/rcljava/tools/JAR_INPUT.md) supplies the
source-bound packaging plan and independent artifact checker. The optional
service-client extension requires its own explicit ABI catalog and SHA256 in
addition to the selected backend; its three function signatures are checked
separately from the original business ABI. The planner grants no authority and
does not run the compiler. Actual translation, native acceptance and DBG remain
required tests. JVM execution, JNI, `.class` parsing or a producer manifest alone
cannot qualify the final Guest application.

### C#

Use the pinned .NET/Mono/WASI closure in the [C# guide](../csharp/README.md).
Compile `csharp/roswasm_bridge.c` with exactly one `ROSWASM_BACKEND`, retaining
the exact filename `roswasm_bridge.o`. Pass it as `RosNativeBridgeObject` and
provide `RosNativeEvidenceDir` when building `examples/csharp/RosExamples.csproj`.
Retain the actual native-item scan, P/Invoke table, linker inputs and final
ROS imports. The bridge lives in Guest Wasm; it does not load a host DLL.

The application supports bounded topic/service/Action roles. Pin copied input,
output and slot arrays for each call. Preserve the primary error together with
all cleanup errors using the documented checked retirement method; a throwing
`Dispose` must not replace the original failure. Finalizer I/O is not cleanup.

### MoonBit

Copy the complete [MoonBit example workspace](../examples/moonbit/README.md)
with its selected local `uwvm/ros` package. Keep the actual package link
settings and frozen producer closure. The bridge explicitly copies bytes into
reserved ordinary-memory input/output/slot regions; a GC object representation
is never reinterpreted as a raw address.

Prove final active data, heap and stack do not overlap the reserved arena.
A minimum-memory check is not that proof. Use the supplied raw/default-denied,
topic/service/Action/executor applications for each backend. The dedicated ABI
coverage probe contains placeholders and is an import-coverage artifact, not
a program to execute as a successful application.

For complete bounded custom values, use the separate explicit
[`moonbit_profile.py`](../custom-types/moonbit_profile.py) producer and the
[typed JointState application](../robots/vendors/examples/joint_state/README_MOONBIT.md).
This additive route emits every admitted record field and actual scalar
defaults, including both Header layouts and optional JointState arrays. It
refuses a whole schema whose original names cannot be represented; it does not
rename or omit fields. The recipe stages one backend, mode and target at a
time. Its new stdin scratch320..353 needs fresh artifact-layout validation;
the older producer's arena evidence does not qualify that reservation. Source,
generated records, compiled Wasm and real native delivery remain separate
qualification stages.

### Python and rclpy

Use [the Python producer contract](../python/README.md). Freeze the selected
SDK, compatibility package closure, pure-Python stdlib and one explicit entry:

```sh
"$BUILD_PYTHON" -B "$SDK/python/tools/freeze.py" \
  --stdlib "$CPYTHON_STDLIB" --sdk-python "$SDK/python" \
  --entry "$SDK/examples/python/01_typed_topics.py" \
  --out "$OUT/python-frozen"
```

The actual matching Linux build-Python version is required. Compile the real
embedding launcher and built-in extension against the target interpreter;
link its actual target `libpython.a` using the producer's recorded ordered
Makefile link closure. Do not invent a short replacement link line or execute
the app under host Python. Strict ROS startup forbids directory preopens:
freeze required packages instead of opening the host stdlib directory.

For rclpy source, include the selected `python/compat/rclpy` package set and
read its [migration contract](source-migration.md#rclpy-inside-cpythonwasm).
Native `_rclpy` wheels are not Guest extensions.

## Validate and deploy the same artifact

1. Record application/source/schema/producer SHA256 and exact build arguments.
   Fully validate the final Wasm's types, memory, imports and enabled proposals.
   The SDK import inspector is an extra policy check, not a full validator.
2. Run default denial with no ROS grants. Require the expected error and no
   native peer effect. Then prepare exact grants with
   [host setup](host-setup.md), binding the real held Worker PID/UID and private
   Gateway socket. Tutorial placeholder PIDs are not launch identities.
3. Use the interpreter first for a simple debug trace. LLVM execution with ROS
   requires the authenticated instruction call stack; select the real
   `-Rllvm-call-stack instruction` mode as well as the chosen AOT mode.
4. Have an independent native peer verify actual payloads, types, resources,
   QoS, correlations and counts. A loopback SDK fake or a local codec roundtrip
   does not establish native ROS interoperability.
5. Exercise denied aliases/roles, wrong type/QoS, malformed/bounded data,
   callback lifetime, close/revocation, stale generations and complete cleanup.
   Retain raw diagnostics and primary plus cleanup failures.

WASI permissions are independent. A language runtime needing a monotonic clock
must receive that finite WASI permission deliberately. ROS grants do not imply
host environment inheritance, clocks, randomness, threads, preopens or sockets.

## Combine migrated and newly developed components

Keep a migrated publisher/service source and a newly written subscriber/client
as separate modules with separate grants. Reverse the roles as a second case.
Repeat with the actual ROS1 and ROS2 Gateways and an explicitly authorized
cross-backend application when mixing systems. Grant the conversion component
only its selected input and output routes; there is no automatic universal
ROS1/ROS2 bridge. Check schema conversion, bounded queues, correlations,
timeouts and errors with peers on both sides.

Use [the modular robotics application](../examples/robotics_modular/README.md)
for separately compiled perception, control and callback components. Vendor
starting points include [Unitree](../robots/unitree/README.md),
[the vendor inventory](../robots/vendors/README.md) and
[xArm](../robots/xarm/README.md). A standard JointState/Twist example does not
prove a proprietary service, Action or robot command path. Retain each actual
upstream schema/version, native type identity and independently observed
result. Simulator checks and physical-device checks are distinct outcomes.

Complete the [security/audit](security-and-audit.md) and
[platform/performance qualification](platform-qualification.md) steps before
calling a composed application qualified. The documentation and examples are
shipped sources; the evidence identifies which exact combinations really ran.
