# Language SDK, debugger, signing, and vendor integration

This document defines the shared integration contract for the language, ROS,
debugger, vendor, and WSEA implementation teams. It describes work in progress.
Current source reviews and individual regression results have the specific
scope recorded in their receipts. Complete language support requires the
application, debugger, permission, native ROS, and performance evidence below.

## Application capability

Each language needs an application API with the capabilities exposed by the C
SDK and the relevant `rcl`, `rclcpp`, `rcljava`, and `rclpy` interfaces. An import
declaration alone does not implement that application API. The implementation
must provide complete values, ownership, errors, scheduling, and cleanup for
each supported operation.

Keep an explicit implementation inventory covering contexts and nodes;
publishers and subscriptions; clients and services; Actions and cancellation;
parameters and parameter events; graph and discovery; clocks and timers;
executors and callback groups; QoS and metadata; lifecycle and logging; and
shutdown. Identify the actual original API signatures, selected source
versions, implemented behavior, and remaining differences. Unsupported
operations must fail explicitly before an effect.

For generated interfaces, retain every original field, constant, bound,
default, nested type, and complete request/response/event. A language-specific
identifier mapping must be explicit and preserve the original interface
identity and canonical bytes. Local aliases, codec identities, signature
capabilities, and object handles do not create a Host grant.

Existing concrete implementations and recipes are in:

| Route | Source and application documentation |
| --- | --- |
| C and C++ | [SDK](../../sdk/README.md), [original node CONTROL](../../sdk/rmw_wasm/default_control/README.md) |
| Java | [Owned Service SDK](../../sdk/java/owned-service/README.md), [producer recipe](../../sdk/java/owned-service/recipe.md), [rcljava migration](../../sdk/compat/rcljava/README.md) |
| C# | [Owned SDK](../../sdk/csharp/owned/README.md) |
| C#, F# and VB.NET / Mono WASI AOT research route | [Complete source, ROS and non-ROS applications, build admission, and debugger cases](../../sdk/dotnet/mono-wasi-aot/README.md); fixed historic Preview6 target, with actual build and runtime qualification pending |
| Python | [SDK](../../sdk/python/README.md), [Owned Service API and applications](../../sdk/python/OWNED_SERVICE.md); the GC-visible witness is source-reviewed, with target GC execution pending |
| MoonBit | [Owned Service SDK](../../sdk/moonbit/owned_service/README.md), [applications](../../sdk/examples/moonbit_owned_service/README.md) |
| JavaScript | [SDK](../../sdk/javascript/README.md), [owned services](../../sdk/javascript/owned_service/README.md), [generated RPC ownership, exact failures and complete application/build flow](../../sdk/custom-types/JAVASCRIPT_RPC_LIFETIME.md); 34 real-Session model cases are authored and await Linux generation/execution |

Source-only additions, private successors, and incomplete language ports remain
in that state until their own admission and execution results exist. In
particular, the integrated Python GC-visible witness, Mono AOT research sources
and F#/VB consumers, expanded Java JAR source closure, and additional language
ports each need their own actual target build and qualification evidence.

## Two application lessons per language

The non-ROS lesson must provide the exact supported compiler/runtime versions,
complete source, build commands, output selection, and execution commands. It
must explain argument and environment mapping, standard input/output, error
exits, memory limits, and required runtime imports. Use a self-contained
application that a user can build and run without ROS.

The ROS lesson must be a real application using the language API. Provide the
complete publisher/subscriber or client/server sides and a useful application
such as parameter configuration, a type-description query, sensor processing,
or robot command handling. Explain the full request and response values,
correlation, finite progress and deadlines, expected errors, and independent
cleanup outcomes. Retain the complete application main function in a
source-port lesson.

Each vendor tutorial must identify the actual robot/product, ROS version,
supported architecture, original vendor SDK version, message definitions,
transport, and native endpoint implementation. Include an architecture diagram,
installation prerequisites, complete source, build and launch steps, policy
selection, expected output, debugging procedure, and recovery procedure.
Explain the concrete replacement of original `.so`/DLL and `.a` dependencies:
which code is compiled into the Guest, which native libraries remain in the
Gateway, and how the reviewed protocol connects them. Vendor libraries and
native type-support objects stay subject to their actual platform and licensing
requirements.

Use professional English throughout the ROS tutorial. Describe ROS 1, ROS 2,
and mixed deployments separately, including exact resource names, type and QoS
bindings, bridge direction, and the distinct native principals. Permission
examples must include an empty-policy denial and specific allowed and refused
operations. A comparison with containers requires observed effects under the
actual competing policies.

## Debugger integration

The [owned language snapshot implementation and build handoff](../../sdk/debug/README.md)
contains the actual bounded wire format, strict Host parser, current Runtime
copy adapter, CPython trace producer, non-ROS probe and default-off ROS hook.
These are integrated development sources. Actual DWARF root evaluation,
controller logical-frame publication, Python source identity, target GC/ROS
observation and usable debugger commands remain unfinished. The Mono, Kotlin
and Go producers are separate development work; emitting the shared format
alone does not qualify a language debugger.

The [CPython debug build guide](../../sdk/python/debug/BUILD.md) includes the
matching-native freeze, three separate debug C objects, original ordered static
link, final-module checks and actual probe commands. Its preparation tools are
integrated Source; the complete current toolchain capture and real target
build, GC and debugger tests remain pending.

Preserve the source files, source maps or PDBs, generated sources, compiler and
linker options, DWARF, and final artifact identities needed by the debugger.
Record the actual source path and method correspondence through translation.
For interpreter-backed languages, implement the mapping from the language
frame and bytecode position to the actual stopped runtime frame. For AOT
languages, retain the correspondence between managed methods, emitted native
functions, variables, and object layouts.

Qualification must exercise a real source breakpoint, step, frame selection,
arguments, typed scalar and structured locals, and resumed completion. Include
optimized-out or unavailable values and invalid/stale frame selections. For
GC languages, test collection and object movement at supported stop boundaries.
Debugger inspection must respect the actual memory and stop/frame bounds and
must not invoke Guest methods, property getters, or arbitrary expressions.

ROS debugging also needs real application cases: two independently granted
owners, a complete request/response correlation, an event or callback, an
expected denial, and cleanup after interruption. Debugger output must distinguish
Guest values, Runtime capability facts, and independently observed native ROS
state. Section presence alone supplies no language-frame or typed-value result.

## Final artifact signing

The WSEA implementation team owns the shared verification and command-line
gates. Follow the current [WSEA contract](../security/wsea.md), including its
platform and protected-storage requirements. Module selection is explicit:

```text
--secure module <wasm-path> <protected-policy-path>
--secure named-module <wasm-path> <protected-policy-path> <expected-id>
```

Use `module` only with a policy containing exactly one module rule; the loader
binds the path to that rule's identity. A shared policy with several rules
requires `named-module` and the expected identity for that particular path.
A valid signature from a different authorized module must not satisfy that
selection. Repeat explicit selections for separate modules. The selected module
must fail closed on verification failure. Follow the WSEA team's admitted CLI
documentation for the exact `cosign wasm secure policy-init`, `policy-update`,
`sign`, `verify`, and `fetch` arguments. This language cohort has not run them.

Complete compilation, linking, stripping, embedded signing, and custom-section
processing before detached signing. WSEA covers the final complete Wasm bytes,
including custom and embedded-signature sections. Its detached materials use
`.sig.cose`, `.signer.pem`, and `.crl.pem` sidecars. Record the final Wasm digest
and the actual verification inputs with the application evidence.

The signed capability mask must fit the selected protected local rule; a wider
signed mask is refused, not silently reduced. Bits are basic WASI `1`, fd/path/
poll including standard I/O `2`, sockets `4`, ROS 1 `8`, ROS 2 `16`, and other
native imports `32`. Bits `2` and `4` also require bit `1`; ordinary WASI stdout
therefore needs mask `3`. Actual imports and import-reset destinations must fit
the signed ceiling. These bits do not grant ROS aliases or operations. Runtime,
WASI, and native ROS resource permissions remain independent checks.

Reverify changed Wasm artifacts and checkpoint reloads against their actual
final bytes. Verify every selected dependency module before entering cached
executable code. An earlier signature receipt cannot qualify a different
artifact or changed policy. Bind debugger source and sidecar metadata
independently; modifying a source file does not modify an already captured
executing Wasm snapshot.

The current WSEA source reports `unsupported_mode` for secure function
replacement and compiled-source replacement. Use a fresh admitted load for
that configuration. New CLI and owned instances recheck policy, signatures,
revocation material, and state before cache entry. An already running instance
has no automatic background revocation or stop mechanism in this increment;
its existing Runtime permission checks retain their separate behavior.

The WSEA team also reports protected persistent `revoked-<SPKI>` state: accepting
an older still-fresh good CRL must not erase a recorded revocation. Recovery
requires a new release key and an increased protected policy version. Fetching
evidence does not grant trust, overwrite an existing bundle, or remove that
state. These are coordinated source contracts; retain the verifier and actual
VM test receipts separately before claiming application acceptance.

## Execution evidence

Run all parsing, generation, compilation, model and Guest tests through the
coordinated SSH Linux entry into the existing 64 GiB, swap-disabled cgroup.
Use fresh directories under
`/home/macromodel/Documents/uwvm-test-261018`. Preserve exact source, tool,
binary, policy, module graph, raw output, exit, resource, and process-retirement
records. Coordinate compiler, QEMU, debugger, and profiler windows with the ROS
and debugger teams.

Keep source review, generation, compiler acceptance, local language contracts,
Wasm execution, native ROS delivery, security fault cases, debugger behavior,
cache behavior, architecture results, and performance measurements distinct.
The completed Node Future regression verifies its actual local scheduling
contract. It supplies no Wasm, ROS, debugger, or performance qualification.

Performance work must compare the same complete workload and actual outputs,
retain cold and warm behavior, and collect the requested hardware counters,
Hotspots, and Microarchitecture results in separately admitted quiet windows.
Measure signing and verification costs for their selected configuration.
Retain failures and unavailable measurements; report ratios and parity only
when the underlying comparable runs qualify.
