# WasmROS guest SDK

Develop ROS1/ROS2 Wasm guests in C, C++, Rust, AssemblyScript and TinyGo, with
additive producer-specific Java, Python, C# and MoonBit SDK sources. The
JavaScript BigInt SDK supports an explicitly supplied authority-aware bridge
and an additive pinned-engine [JavaScript-in-Wasm path](javascript/guest/README.md).
The latter places actual user logic/callbacks inside its Wasm principal; its
new artifacts require separate actual build/Runtime qualification. Start with exact imports, learn the canonical
codecs and owned resources, then use the `roswasmc`/`roswasmcpp` and corresponding
language Node/resource/executor APIs. These direct numeric SDKs do not need
native ROS headers or libraries. The separate original-upstream migration
route rebuilds its selected ROS sources and static dependencies for Wasm;
preexisting native shared objects are never Guest libraries.

This SDK extends the existing `ros1/`, `ros2/` and `common/` headers. Those include
paths remain available. The new public C/C++ entry point is `include/roswasm/`.
Keep the SDK directory layout intact: compatibility forwarding headers depend
on its relative paths. All application code must still check errors and supply
explicit bounded buffers and trusted grants.

For a complete application, start with the
[all-language build and deployment tutorial](docs/complete-application-tutorial.md).
The [source migration guide](docs/source-migration.md) distinguishes finite
rmw/rcl/rclcpp/rclpy/rcljava profiles from original-upstream rebuilds.
[Native library porting](docs/native-library-porting.md) explains `.so`, `.dll`,
`.a`, `.lib`, managed extensions and vendor SDK dependencies.
[Security and audit](docs/security-and-audit.md) specifies permission boundaries
and a concrete container comparison; [platform qualification](docs/platform-qualification.md)
covers actual ROS architectures, mixed applications, debugger/cache tests and
matched hardware-counter performance. These guides state acceptance work and
current limitations; their presence does not mark unexecuted combinations as
qualified.

Trusted native embedders can read the [threaded debug-control contract](docs/threaded-debug-control.md). Its cooperative all-stop API is opt-in source implementation; actual Linux/ROS2 debugging and performance qualification remain pending.

## Choose a language and a learning stage

| Language | Library | Progressive examples | Guide |
| --- | --- | --- | --- |
| C11 | [include/roswasm](include/roswasm) | [examples/c](examples/c) | [C/C++ guide](docs/c-cpp.md) |
| C++20 | [roswasmcpp.hpp](include/roswasm/roswasmcpp.hpp) | [examples/cpp](examples/cpp) | [C/C++ guide](docs/c-cpp.md) |
| Rust | [rust](rust) | [examples/rust](examples/rust) | [Rust/TinyGo guide](docs/rust-tinygo.md) |
| TinyGo | [tinygo](tinygo) | [examples/tinygo](examples/tinygo) | [Rust/TinyGo guide](docs/rust-tinygo.md) |
| AssemblyScript | [assemblyscript](assemblyscript) | [examples/assemblyscript](examples/assemblyscript) | [JS/AssemblyScript guide](docs/javascript-assemblyscript.md) |
| JavaScript bridge client | [javascript](javascript) | [examples/javascript](examples/javascript) | [JS/AssemblyScript guide](docs/javascript-assemblyscript.md) |
| JavaScript embedded Wasm guest | [javascript/guest](javascript/guest) | [examples/javascript_guest](examples/javascript_guest) | [Closed engine/build/lifecycle](javascript/guest/README.md) |
| Java / TeaVM WASI | [java](java) | [examples/java](examples/java) | [Java bridge and qualification](java/README.md) |
| Python / CPython WASI | [python](python) | [examples/python](examples/python) | [Python bridge and migration](python/README.md) |
| C# / Mono WASI | [csharp](csharp) | [examples/csharp](examples/csharp) | [Static native bridge and qualification](csharp/README.md) |
| MoonBit | [moonbit](moonbit) | [examples/moonbit](examples/moonbit) | [Numeric memory arena and qualification](moonbit/README.md) |

For a complete shared telemetry case, use the typed ROS1/ROS2 JointState
applications for [Java, C# and Python](robots/vendors/examples/joint_state/README_MANAGED.md)
and [MoonBit](robots/vendors/examples/joint_state/README_MOONBIT.md). They retain
the full Header, names and all three numeric arrays, with explicit default-denied
and positive application sources. MoonBit now has actual complete-record codec
tests, all sixteen selected application builds/layout checks and sixteen
default-disabled Runtime outcomes, as recorded in its linked guide. A separate
[standalone codec matrix](../test/0050.high_level_languages/results/native-budget/language-moonbit-codec-uwvm-20261019/README.md)
now completes 168 methods in 72 real UWVM executions across the three
repositories and both Runtime build modes, with 24 deliberately failing
golden-byte controls. The [Java matrix](../test/0050.high_level_languages/results/native-budget/language-java-records-jointstate-20261019/README.md)
also now compiles twelve fixture/JointState applications, completes 168 codec
methods and 564 checks in UWVM, checks twelve deliberately failing golden-byte
controls and passes eight default-disabled ROS1/ROS2 JointState outcomes.
The [C#/Mono matrix](../test/0050.high_level_languages/results/native-budget/language-csharp-records-jointstate-20261019/README.md)
now adds twelve actual producer builds, 24 final Wasms, 360 completed codec methods
and 1368 checks, 24 deliberate managed assertion failures and 16 default-disabled
ROS1/ROS2 JointState outcomes. It checks actual P/Invoke prototypes and binary
imports, and identical noncustom sections across baseline/retained-DWARF pairs.
The [Python/CPython matrix](../test/0050.high_level_languages/results/native-budget/language-python-records-jointstate-20261019/README.md)
adds twelve actual builds and 24 final Wasms, 408 completed codec methods and
1944 checks, 24 deliberate assertion failures and sixteen default-disabled
ROS1/ROS2 JointState outcomes. It verifies frozen sources/bytecode, actual static
bridge/target-library links and final imports. Positive native JointState
delivery, managed debugger and performance qualification remain separate work.

The additive languages have producer-specific foundations and explicit example
sources; their actual build/Runtime/native ROS evidence is tracked separately
from the original six libraries. Existing rclc/rclcpp/rclpy/rcljava product
compatibility is an ongoing required task; consult the
[versioned compatibility boundaries and acceptance requirements](compatibility/rcl/README.md).
An import inventory or a familiar Node name is not full client-library compatibility.

For a completed native ROS2 String delivery lesson, see
[MoonBit telemetry](examples/moonbit_native_telemetry/README.md). Its four
retained producer profiles now pass sixteen deliveries on the frozen
default-on Release/Debug interpreter products, including independent native
writer/GID/value checks, error-13 denial and checked teardown. This acceptance
is separate from the JointState/vendor and broader client-library work above.

Opt-in migration include/package roots are available for
[rclc](compat/rclc/README.md), [rclcpp](compat/rclcpp/README.md),
[rclpy](python/README.md) and [rcljava](compat/rcljava/README.md).
Their documented finite API profiles preserve the selected original application
sources; the remaining native APIs and upstream suites are required work.
See the [dated implementation and actual-test checkpoint](../documents/ros/sdk-compatibility-checkpoint-20261018.md)
for evidence beyond the immutable source-authoring manifests.

Use the examples in order. Stage 1 declares/calls raw imports and checks the ABI
version. Stage 2 constructs canonical values and owns a separately granted
resource. Stage 3 groups resources under a Node and delivers callbacks through a
bounded polling executor. Resource examples extend that pattern to services,
Actions, parameters, TF, graph queries, lifecycle, clocks and logging. The
English source comments explain arguments, memory lifetime, error handling,
required grants and the point where a callback may run.

## Robot vendor adapters

The [vendor SDK preparation pipeline and six-language telemetry examples](robots/vendors/README.md)
cover the shared integration path for all nineteen requested vendors. Select an
exact model and native SDK revision, admit the complete official interface
definitions, bind their actual native identities, then generate the six guest
codecs. The inventory records missing model-specific implementations and tests;
it does not grant robot access or mark an adapter as qualified. Existing
[Unitree adapters](robots/unitree/README.md) retain their separate model and test
requirements. The concrete [xArm RobotMsg family](robots/xarm/README.md) and
[CIOState family](robots/xarm/examples/cio_state/README.md) provide complete
six-language telemetry lessons, original schemas and independent native fixture
publisher sources. Their source reviews are closed; generation, native SDK
qualification, real ROS delivery and the remaining xArm interfaces are pending.

The [optional managed-record generator](custom-types/MANAGED_RECORDS.md) also
selects Python/Java records from the same admitted bounded schema model. Its
additive [C# emitter](custom-types/MANAGED_CSHARP.md) preserves exact native
field spellings with typed value/codec classes and no reflection. All optional
emitters preserve complete fields, bounds and supported defaults. Their actual
host codec tests do not supply native type identity or vendor delivery
qualification.

## What the Runtime actually exports

`uwvm_ros1_v1` has 38 functions and `uwvm_ros2_v1` has 43, including the four
ABI/capability functions. Both sets are complete in each raw language binding.
See the [exact import reference](docs/abi-v1.md) and
[machine-readable schema](abi/abi-v1.json). These inventories come from
`src/uwvm2/uwvm/imported/ros/host.h`, not a guessed list of native ROS APIs.

Every import returns `i32`; `abi_version` returns 1, and the others return an
error code. Memory index is `i32`. Offsets, byte lengths, capacities and handles
are **i64 in wasm32 as well as memory64**. Returned handles and lengths occupy
eight little-endian bytes. Raw multi-memory callers choose a memory explicitly;
pointer conveniences use memory zero. See [memory and errors](docs/porting-and-errors.md).

## The layers

[Architecture](docs/architecture.md) explains how the four layers fit:

1. **Exact imports:** declarations match module/name/type without native ROS
   dependencies or implicit permissions.
2. **Wire and ownership:** canonical typed codecs, bounded buffers and owned
   capability handles; arrays, Image/Transform, service/Action envelopes and
   ROS2 parameter events use the real Runtime format.
3. **Application resources:** Node factories take capability aliases;
   Publisher/Subscription, Client/Service and Action resources use separate
   grants. Parameters, graph, TF, clocks, logging and ROS2 lifecycle expose the
   implemented backend operations.
4. **Bounded executor:** `spin_once` polls on the guest thread and calls the
   registered application callback there. Native threads never call into Wasm.

The client libraries also provide explicit Context/Session shutdown, one-shot
local Futures, CallbackGroups and caller-driven Timers. Deferred service calls
execute the existing bounded synchronous RPC during a later explicit pass;
they do not create a native asynchronous RPC or an implicit clock/thread.
Examples continue through typed service/Action servers, parameters and events,
TF/graph/clock/lifecycle queries, scheduling and checked ownership cleanup.
The [complete client-library coverage](docs/client-library-coverage.md) lists
all six public layers and their application families.
See [serial local verification](docs/local-testing.md) to compile and test a
frozen SDK entirely below `/tmp` with one build job.

These APIs support familiar ROS programming patterns. The opt-in
[rclcpp String profile](compat/rclcpp/README.md) preserves its pinned official
example sources, and [rclpy migration](python/README.md) supplies a bounded
source interface. Full rclc/rclcpp/rclpy/rcljava and ROS1 client-library
compatibility remains required and incomplete. The libraries do not claim an arbitrary custom-IDL adapter, Wasm
threads, an asynchronous native callback, or an operation missing from the
Runtime. ROS1 parameter search includes fixed-key parent namespaces only when
the trusted grant explicitly enables them within its allowed prefix. ROS2
declaration retains its local Gateway-node boundary; see the
[operation contract](docs/abi-v1.md).

For fixed-layout custom Topic records, see the
[fixed-v1 generator](custom-types/README.md). This profile produces annotated
C, C++, Rust, JavaScript, AssemblyScript and TinyGo codecs for fixed primitives,
fixed arrays and acyclic nested records. Dynamic fields and custom Service/Action
adapters are outside this fixed-v1 profile.

The separate [bounded-v2 profile](custom-types/BOUNDED_V2.md) supports finite
custom `.msg`, `.srv` and `.action` definitions, including bounded UTF-8 strings,
bounded sequences, fixed arrays and declared nested message dependencies, for
Topics, Services and Actions. Start with the
[six-language client/server applications](custom-types/bounded-examples/README.md)
and the [delivered bounded codec fixture](custom-types/generated-bounded-v2).
ROS1 string/sequence bounds come from the trusted catalog; ROS2 bounds must also
respect the official native declarations. Unbounded canonical layouts, recursive
or unknown dependencies, arbitrary constants/defaults, wide strings, implicit
Header/time/duration, unions/inheritance and unreviewed IDL preprocessing,
annotations or aliases are refused. Fixed-v1 retains its documented closed IDL
subset; neither profile claims arbitrary runtime-discovered IDL support.

The additive [official-standard profile](custom-types/OFFICIAL_STANDARD.md)
preserves complete Header/time, JointState and admitted geometry/navigation values in all six languages, with
explicit dependency registration and the same native identity checks. Its
[concrete six-language applications](robots/vendors/examples/joint_state/README.md)
include fresh default-denied and explicitly authorized subscription modes. This
profile includes the exact official ROS2 Quaternion default (w=1); modern native
metadata must independently prove that default, which a type hash alone does
not establish. This profile and its examples are integrated source awaiting unified generation,
compilation and actual native-delivery qualification.

All generated native profiles require reviewed Worker/Gateway descriptors, official
native identities from generated ROS1 traits or ROS2 type support, and complete
native definition/schema correspondence before endpoint creation. Every custom
grant supplies an explicit `adapter-codec-identity`, resource and limits; each
Service/Action role needs its own explicit authority. The default registry is
empty, and constructing a codec or registering a type grants nothing. Generated
source, successful compilation and codec/mock models do not establish real ROS
delivery; that requires the actual Worker/Gateway/Guest exchange and independent
native observations from the selected acceptance run.

## Explicit authority comes first

An alias such as `pub` is a lookup key into a trusted host grant. It is not a
topic name and cannot authorize a different resource. The host policy fixes
backend, Instance/Single/Group binding, canonical resource name, type identity,
connection settings, QoS and all rate/byte/queue/deadline limits. The SDK starts
without ROS authority. Constructing a Node or codec grants nothing.

Do not copy a grant from another application without reviewing its limits.
The [example host policy generator](tools/example_policy.py) emits an explicit
bounded, explicitly selected String publisher/subscriber tutorial policy; it never starts a
Gateway, invokes a Worker, discovers credentials or adds permissions itself.
Read [the host setup guide](docs/host-setup.md) before running a guest.

Output error 90 does not always write a required length. Wrappers use an
eight-byte sentinel and expose only a length actually written by the host.
An undersized receive buffer may already have consumed an event; timeouts can
follow an admitted external effect. There is no automatic replay or authority
escalation. Call close explicitly when its cleanup status matters; close/revoke
can retire ownership before a later Gateway notification fails.

## JavaScript and AssemblyScript are different targets

AssemblyScript declarations use `@external` and produce actual Wasm imports.
The original JavaScript host path consumes a supplied Wasm bridge, shared
`WebAssembly.Memory` and caller-reserved scratch. That embedding must preserve
authenticated guest identity and Runtime policy. The package does not implement
a permissive ROS network host. The additive embedded path compiles a pinned
QuickJS interpreter, exact closed JS sources and real C ROS imports into one
Wasm, reusing the same SDK through bounded copied storage. Its pure reactor
libraries compile without ROS/WASI imports. No arbitrary filesystem JS loader
or default ROS authority is introduced. See the
[exact integration contracts](docs/javascript-assemblyscript.md).

## Check source and compiled imports separately

From this source checkout, the following are pure source/specification checks:

```sh
python3 -B sdk/tools/abi_contract.py
python3 -B sdk/tools/check_bindings.py
python3 -B sdk/tools/wire_vectors.py
python3 -B sdk/tests/test_import_inspector.py
node sdk/javascript/tests/test.mjs
node sdk/javascript/tests/factory_transactions.mjs
node sdk/javascript/tests/executor_contracts.mjs
```

To run the complete bounded source/model set and save the checked source hashes,
use `python3 -B sdk/tools/verify_sources.py --report /absolute/new-output/sdk-checks.json`.
The checker requires an existing Python with `tomllib` and an existing Node with
ES modules, BigInt and `WebAssembly.Memory`; it installs no dependencies.

The original binding checker compares 405 backend-qualified declarations across C/C++,
Rust, TinyGo, AssemblyScript and JS metadata, and checks all 88 C backend dispatch
slots against the actual Runtime operation inventory. The
[golden wire vectors](abi/wire-vectors-v1.json) contain 57 positive and 11 negative
specification samples. These counts do not include the additive managed-language
bindings; their separate source and final-binary checks remain required.
A codec/mock check is not evidence of ROS delivery.

After compiling a guest with the selected language's documented toolchain,
perform full Wasm validation and inspect its **binary** import signatures:

```sh
wasm-tools validate /absolute/path/to/guest.wasm
python3 -B sdk/tools/check_wasm_imports.py /absolute/path/to/guest.wasm \
  --backend 2 --allow-module wasi_snapshot_preview1 \
  --report /absolute/path/to/new-output/imports.json
```

The inspector never executes the file. Use `--backend 1` or `dual` when
appropriate, and list every permitted non-ROS module explicitly. `--require-all`
is for a smoke fixture intentionally retaining every import. The small inspector
rejects unsupported GC type encodings; it does not replace a full validator.

Native ROS and ARM acceptance for this task requires
the existing verified 64 GiB Linux cgroup with zero swap. SSH and toolchain availability,
actual product identity, live ARM/QEMU maps and cleanup evidence remain required.
The [serial build matrix](docs/build-matrix.md) prepares exact commands without
running them by default and requires pinned Linux cgroup identity for execution.
See [verification status](docs/verification.md) for what has run and what is
still pending; prepared source is never labeled as a compiled language PASS.
Local SDK verification uses the separate serial `/tmp` runner described above;
it does not qualify native ROS delivery or QEMU behavior. Resumed SSH work uses
the same explicitly verified Linux cgroup, with resource windows coordinated
with the separate language-runtime task.

The [native and pure model matrix](docs/model-matrix.md) prepares and guards
actual CMake relocation, installed C/C++ fixtures, native Rust/Go tests and
import-free AssemblyScript models. Its default mode starts no tools. Successful
model execution does not establish real ROS or ARM behavior.

## Distribution and compatibility

The SDK includes source-only CMake interface targets; see
[C/C++ integration](docs/c-cpp.md). Rust has a local Cargo crate, TinyGo a local
Go module, and JS/AssemblyScript have their own package/entry modules. No package
manager download is needed to inspect the sources. Compilers and their standard
libraries remain separate developer dependencies.

The [legacy C++ guide](docs/legacy-cpp.md) retains the previous detailed array
and parameter-event explanation. This SDK and its examples use the repository's
[repository license](LICENSE). New source comments and guides are in English.

## Additive unified event source contract

[Unified event v1](docs/unified-events.md) specifies the separately granted
observer, complete64-byte envelope, seven kinds, Source/observer lease checks,
capacity-retaining poll, finite readiness and real bounded `service_submit`.
The new APIs are additive; local deferred tasks still advance synchronously
when explicitly scheduled, and do not gain native asynchronous authority.
No implicit alias, clock, thread, retry or stronger cancellation grant is added.
The six runnable unified-event lessons and source fixtures are prepared for the
unified serial test stage; dated historical34/39 receipts certify their original
revisions only.

The additive [unified-event resource guide](docs/event-dispatch.md) explains
real callback/executor integration for all six languages, including native
`service_submit` completion, separate source retirement, fixed storage, callback
leases and checked cleanup. Updated lessons and new regression fixtures are
source implementations pending their own guarded compilation/execution; they
are not covered by older SDK or middleware receipts.

## Native MoonBit telemetry witness

The [MoonBit telemetry tutorial](examples/moonbit_native_telemetry/README.md)
records four Humble x86_64 producer profiles repeated twice, with complete
native rclcpp receipt, exact publisher GID matching, error-13 ungranted-alias
denial and checked teardown. It uses explicit disabled-test DDS security.
This scoped functional evidence does not establish full client-library parity,
vendor compatibility, managed debugging or performance equivalence.
