# Mono WASI P1 AOT research SDK: C#, F# and VB.NET

This is a separate, explicitly selected Source implementation using official runtime commit `573461dc897731c11d6166574b77a8c3aabba220` (`v9.0.0-preview.6.24327.7`). It preserves the existing Mono 8.0.15 interpreter SDK and its artifacts. This historic preview is a research target, not a maintained .NET 9 support statement. Official primary documentation calls WASI support a prototype; the actual selected targets contain the AOT pipeline described below. Actual offline tool packs, compilation and runtime compatibility still have to be admitted and tested.

**Current status: unexecuted development Source.** No candidate syntax parser, import, generator, compiler, Native/Guest, debugger, QEMU or VTune command has run. Source review is not runtime approval. Missing packs or unsupported LLVM/runtime behavior must fail qualification, without downloading replacements, editing SDK targets or silently running the Mono8 interpreter. All future commands belong to the existing admitted 64GiB/no-swap Linux cgroup, with one build at a time.

## What the producer actually does

`producer.py` is an actual future build driver, not a manifest that declares AOT. It captures the externally admitted Source closure; stages those bytes into a fresh disjoint directory; selects an exact already installed SDK without roll-forward; uses only an already admitted offline NuGet directory; and runs real restore, bridge compilation and `dotnet publish` under the existing pidfd owner. It never runs `workload install`, fetches a package, changes a tool tree or substitutes host CLR/ROS transport.

The actual selected common target invokes `MonoAOTCompiler`, emits assembly LLVM bitcode and `driver-gen.c`, compiles those bitcode files with WASI clang, then links real objects with the runtime and class libraries. The new hooks snapshot actual MSBuild item metadata before it is cleared. They retain real compiler paths, runtime pack, mode, assembly/bitcode/object association, generated registration/PInvoke tables, compile/link response files, PDB, binlog and native link map. The inspector executes actual LLVM bitcode decoding/object-symbol inspection and a separate whole-core-Wasm validator.

The generated registration calls must refer to module-info definitions in the actual objects and final link map. Actual compiled runtime Source must match the selected `main.c`, `driver.c` and shared HOST_WASI `runtime.c`; real compile flags must include `ENABLE_AOT`, `DRIVER_GEN` and `WASM_SINGLE_FILE`. An empty instruction inspection is rejected for the application and SDK. A genuine forwarding assembly may be metadata-only, which gets **no method credit**. Missing bitcode is listed explicitly.

`LLVMOnlyInterp` is hybrid AOT with interpreter fallback. `LLVMOnly` requests and registers Mono's full-AOT runtime mode. Neither mode, a module symbol, `RunAOTCompilation=true`, nor a successful link proves every managed method, generic instantiation or dynamically reached method compiled or executed. Reports retain actual per-object symbol coverage and selected real LLVM function definitions. They never label those facts complete managed-method coverage. Real fallback/refusal/generic/exception tests are separate.

Stable 9.0.0 targets use P2; the fixed Mono8 native chain does not wire its existing AOT target into its core build. This implementation does not relabel either route as P1 AOT. `ROUTE.md` and retained fixed primary data explain those source-level distinctions.

## Prepare the real offline admission

An independent operator must construct and SHA-pin the admission JSON after inspecting actual tool files. A producer-generated report cannot authenticate its own compiler or runtime. Do not supply placeholder paths, self-signed package attestations or a guessed complete-file list.

The exact format is `uwvm-mono-wasi-aot-offline-admission-v1`. Required fields are:

| Field | Actual required content |
|---|---|
| `runtime_commit`, `runtime_version` | The exact commit and Preview6 version above |
| `sdk_version` | Exact installed prerelease9 SDK version; no major/latest/roll-forward selector |
| `sdk_root`, `wasi_sdk_root`, `runtime_pack_root`, `offline_nuget` | Existing canonical absolute directories, all separately admitted |
| `tools` | Exact paths for `dotnet`, WASI `clang`, real Mono `crosscompiler`, `llvm-dis`, `llvm-nm`, `llvm-objdump`, `validator` (`wasm-tools` CLI) |
| `tool_files` | Complete externally admitted compiler, loader, task DLL, runtime/native/classlib and Python startup/stdlib file closure, each `path`, `bytes`, `sha256`; actual Python executable also included |
| `sources` | Complete Source rows `{source,target,bytes,sha256}`; target is its staged repository-relative path |
| `source_roots` | Canonical directories containing every admitted Source input; stage must be disjoint from them |
| `primary_map` | Actual Source paths for the six fixed primary roles checked by `inspect_aot.py`; these paths must also be admitted files |
| `existing_cgroup` | Exact `path`, boot ID, directory `dev` and `ino` from the existing64GiB/no-swap group; never create a new one |
| `p1_startup_signatures` | Exact complete final non-ROS function rows `{module,name,kind,params,results}`; module must be `wasi_snapshot_preview1`, with no wildcard |
| `canonical_source_revision` | For ROS, actual complete same-source C/C++ canonical generation revision, including all seven products and proof |
| `raw12_catalog_sha256` | For ROS, exactly `9e743ad0077cfab6d96bd9ca37d4d23a8804d9c2593af70678b6406222b9ac18` |

This Source authority cohort is a new input identity. Frozen SDK packets and
their original pins remain historical; prior model/build/runtime/ROS/DBG results
are not retagged or extended by this metadata update. Fresh composition and
separate qualification are required.

The source closure contains this entire directory, actual `test/0050.high_level_languages/owned_process.py`, the full bounded Wasm reader, the complete selected C# SDK and native bridge/header dependencies. For ROS it also contains the current exact parent/bounded/managed/C# generator sources, every original control catalog definition, actual raw catalog/authority/implementation/header pair, and the complete shared C# SDK. No fabricated native identity is inserted for a preparation identity that is absent. The current operator/emitter copies are exact existing complete bodies; their ordinary public Mono8 projects are not imported by these projects.

The task's actual files need external authenticity and complete transitive admission. A checksum does not establish source-build provenance, safe package origin or Host permissions. Windows/macOS host .NET and browser/component packs are not substitutes for the admitted Linux/WASI route.

## Produce ROS and non-ROS binaries

The outer reviewed keeper supplies the exact literal `entry-bootstrap.py.txt` as Python's `-c` argument, with the producer path and expected producer SHA before its arguments. That bootstrap reads the entry once through an authenticated regular FD and executes the same captured bytes. Candidate helpers similarly use captured bytes in fresh private modules. `-B` alone would not prohibit ambient PYC reads. The actual Python executable and standard library remain an external admitted TCB.

Producer arguments are explicit:

```text
--admission /absolute/admitted.json --admission-sha256 EXACT_SHA
--stage /absolute/fresh-disjoint-stage
--language csharp|fsharp|visualbasic --ros 0|1
--mode LLVMOnlyInterp|LLVMOnly --case application|guest-faults
```

Run one combination at a time. `guest-faults` is the separate actual C# SDK fault build and requires `--ros 1 --language csharp`; it is not a production application or Native positive result. The parent keeper must hold exclusive execution authorization. Each command has a start barrier before authenticated pidfd registration, finite deadline,8GiB address/RSS limits, observed group membership/pressure, disk reserve/growth limits and bounded physical retirement. Helper ownership is sampled; a helper that detaches and loses all observable ancestry before discovery may be missed. Do not compensate by signalling numeric unrelated PID/PGID values. All incomplete/failed command receipts remain failures.

### Parent PIPE collector and natural command completion

The r2 Source successor fixes the two independent r1 keeper findings; r1 and its original report stay immutable. The earlier Node scheduling runner is only an ownership/resource-observation reference: its regular-file logs do not establish this PIPE contract or confer any Mono execution credit.

Each command creates `stdout=PIPE` and `stderr=PIPE`. Only the parent writes `stdout.log` and `stderr.log`. A synchronous nonblocking selector reads each ready stream in bounded batches (at most four64KiB reads), then returns to the actual ownership/resource/deadline observation. Each stream independently retains at most64MiB. Any byte beyond its cap is recorded as overflow and refuses success, even when the tool subsequently exits0. This is not a promise to preserve unlimited rejected output: the capped prefix, actual bytes observed, overflow and read/write errors are retained. The512MiB child file-size limit remains a separate defence for tool-created files; it does not limit PIPE output or replace this collector.

After the actual owner retirement attempt, the parent gives inherited writer FDs at most3seconds to reach real EOF. Leader exit is not EOF. Missing EOF, forced pipe closure, selector/read/write/close failure or incomplete output refuses success. The synchronous collector has no reader thread to detach or join; its descriptors/selector close explicitly, while the actual pidfd helper performs the bounded leader wait/join. Evidence records these two lifecycles separately. Parent storage/kernel operations and Python startup/standard-library remain the admitted external TCB, not a hard real-time or hermetic guarantee.

A command may pass only with natural leader exit0, real leader join, a non-null closed ownership record containing the actual leader identity, all retained pidfds closed, **an empty cleanup-signals list**, and both complete uncapped streams. A successfully SIGTERM/SIGKILL-cleaned helper is a failed command. Missing/invalid ownership, a cleanup exception or later Source/tool/cgroup recheck failure cannot inherit success from the leader. The primary failure and independent cleanup errors remain in the command ledger allocated before release; no cleanup operation retries the tool.

Future admission must test, without waiving any evidence, a normal empty/output command, an exact-cap stream, a one-byte overflow/burst on each stream, interleaved stdout/stderr, a tool that fails/times out, a retained helper still alive after leader0, a helper retaining a writer FD, a log-write failure and incomplete retirement. Require capped parent files, real EOF/forced-close status and all signal/identity/closed witnesses. These tests have **not run** for r2; Source revision does not prove either finding fixed on Linux or qualify a real AOT toolchain.

For `--ros 0`, no ROS C object or SDK assembly is linked. Each language has a real independent non-ROS project. Its final import set must equal the admitted complete P1 startup signatures. For `--ros 1`, the full shared SDK and complete32/seven-root sources are compiled into `Uwvm.RosWasm.Aot`; the assembly is a trimmer root so an example selecting one service is not intentionally a projected partial SDK. All43 original ROS2 imports and raw12 signatures must match the exact catalogs. Final inspection rejects duplicates, missing/extra signatures, nonfunction imports, component containers, JS/env/browser imports and every unselected ROS namespace. Source-stage bindings must match actual current Host/header bodies. An old catalog mismatch is a refusal, not a passed negative test.

The output `producer.json` can say only `ACTUAL_AOT_ARTIFACT_EVIDENCE_ONLY_RUNTIME_AND_DEBUG_UNQUALIFIED` after real future tools succeed. It is not a Native/Guest/performance/debug certificate. Whole-Wasm validation is separate from the final import parser. Preserve the unstripped `.wasm`, PDB/IR/source maps and actual tool/assembly/object/native/classlib provenance.

## Actual caller applications in all three languages

The C#, F# and VB.NET programs directly use the same real `OwnedGroup`, `Session`, `Capability`, generated classes and numeric same-Wasm bridge. There is no local fake transport or implicit native endpoint. They reserve `ApplicationLifetime` before the first open. `ApplicationLifetime.Last` retains the exact primary, full group/owner diagnostics and separate cleanup/printing errors. Business or cleanup failure produces process1. Every raw close/native retire is owned by the actual SDK ledger; an unknown entered operation is not retried.

Each ROS entry accepts the same modes:

```text
type-query CALL_ALIAS OBSERVER_ALIAS AVAILABILITY_ALIAS_OR_- ACTUAL_NATIVE_TYPE ACTUAL_RIHS FINITE_STEPS
get CALL_ALIAS OBSERVER_ALIAS PARAMETER_NAME FINITE_STEPS
set CALL_ALIAS OBSERVER_ALIAS PARAMETER_NAME SIGNED_INT64 FINITE_STEPS
```

`FINITE_STEPS` is1..65536 explicit caller credits. This is not a DDS timeout promise. Empty polling does not automatically resend or create credits. TypeQuery requests the full source/description response, checks actual correlation and success, and leaves raw source file content as bytes. Get/Set use the complete ParameterValue/Parameter records, including all unused default fields serialized by the real generated codec. On credit exhaustion they explicitly abandon the admitted ticket once, then fail the application. Local abandon is not a native cancellation proof. The same SDK still fully decodes/validates a late abandoned reply before one disposal ACK.

Call and event.observe grants are distinct. Availability requires a third separate grant. The operator chooses the real Native type, codec identity, aliases, QoS and quotas through the established Host/Gateway configuration; command arguments cannot create them. A missing Native implementation or unsupported provider is a refusal, not success. Actual compatible Native provider/type registration and all seven real services remain required qualification.

## Full API use and server applications

All three .NET languages consume the same public shared SDK, not separate reduced codecs:

| Capability family | Real SDK API |
|---|---|
| Session and explicit aliases | `Session.Open`, `Capability.Invoke`, explicit retirement and cleanup reports |
| Topics | `Node.CreatePublisher`, `CreateSubscription`, `Publish`, caller-driven `PollOnce` |
| Original Services | `Node.CreateClient`, `CreateService`, explicit call/submit/observe and separately granted response |
| Owned RMW Services | `OwnedGroup.OpenClient/OpenServer`, `Submit`, `Poll`, `Respond`, `ServeOnce`, `State`, `Abandon`, `Available`, `Diagnostic`, `CloseAll` |
| Actions | `CreateActionClient/CreateActionServer`, full goal/result/feedback codecs, explicit owned goal IDs and separate cancellation rights |
| Events and executor | `CreateObserver`, readiness/watch and explicit `Executor.SpinOnce`; no autonomous background pump |
| Graph, TF, parameters, lifecycle, time and logs | `Node.OpenOperation` plus the complete original operation-specific canonical payload and explicit alias; no namespace expansion |

The seven complete owned definitions are `Ros2Rcl_interfacesSrvDescribeParameters`, `Ros2Rcl_interfacesSrvGetParameterTypes`, `Ros2Rcl_interfacesSrvGetParameters`, `Ros2Rcl_interfacesSrvListParameters`, `Ros2Rcl_interfacesSrvSetParameters`, `Ros2Rcl_interfacesSrvSetParametersAtomically`, and `Ros2Type_description_interfacesSrvGetTypeDescription` on `ControlRoots`. Every definition contains real request/response codecs and complete IR identities, field/default/constant/raw-byte/nested/aggregate bounds. Full32 generated metadata is preserved; no language-specific extra Time rule is added under the same identity.

For a server, open a separate advertise and responder pair with `group.OpenServer(definition, advertiseAlias, responderAlias)`. Call `Poll` under explicit caller credits. Reject controls, inspect the complete decoded request and saved `ServiceInformation`, then call `Respond(delivery.Ticket, completeGeneratedResponse)` once. `ServeOnce(handler)` offers this explicit one-request flow; the application handler runs after complete take/decode/ACK. F# supplies `System.Func<Request, ServiceInformation, Response>` and VB supplies the corresponding `Func(Of Request, ServiceInformation, Response)`; neither handler is a Native callback or scheduler. Keep the group in `ApplicationLifetime` before opening. For a true parameter store, allocate complete response/result objects before changing application state, decide each declared name/type explicitly and preserve SetAtomically application semantics; a sample client is not that store or a transplanted rcl node.

Use the original C# `sdk/csharp/owned/examples/Parameters.cs` server as an actual separate reference application, not a claim that its net8 project is already an AOT artifact. Port that Source into an explicit admitted net9 entry/project and apply the same full pipeline/inspection. The new three-language apps above are real clients; complete API availability does not imply every possible server/vendor application has been implemented or tested.

## Debugger development and actual capability checks

`DebugType=portable`, unstripped native DWARF, real Mono `--debug`/`dwarfdebug`, Source pins and real LLVM method/object evidence are retained. These are prerequisites. They do **not** implement UWVM managed source-line stepping, typed managed locals or an F#/VB expression evaluator.

Each non-ROS entry takes two arguments: signed-int64 and a non-ASCII label. With49 and the chosen label, its typed local at `DBG_STOP_*` is42. It has a signed struct/record field-7, full unsigned ID `18446744073709551615`, double1.25 and real recursive/nested frames. It writes a ready marker, blocks for one explicit `G` byte, then prints its real result. Post-call writes prevent deliberately writing a tail-call-only demonstration. The compiler may still optimize locations; the debugger test must demonstrate actual usable values or fail, not waive missing locals.

`debug-cases.json` records exact actual Source files/lines/hashes, argv, marker/release protocol and required typed locals/frames. Test source breakpoints, next/step/finish, caller frames, recursive depth locals, signed/unsigned values, aggregate fields, Unicode/empty argv and stable GC roots. Require a genuine source-line hit at the specified Source, not a breakpoint in a generated driver or an arbitrary DWARF function. Verify the managed PDB-to-document mapping and LLVM/native location map against the exact captured Source bytes. Reject stale source files or binary/PDB mismatches.

ROS cases additionally break before real Submit and after a real complete delivery/ACK. Inspect the full ticket, native signed sequence, raw writer GUID16 versus client GID32, `ApplicationLifetime.Last` and preallocated attempt/cleanup facts. A real independently observed provider must confirm the corresponding effect. Do not replay an entered Submit/ACK to make a breakpoint easier. Unknown commit/cleanup must remain unknown and retired. Test debugger pause/stop/reload and Host-thread ownership against the actual B1 control and ROS lifecycle; current Source does not establish those target capabilities.

The required actual Linux result for source lines, typed locals, argv and frames is currently **PENDING, not PASS** for every mode/language. If UWVM cannot consume a needed Mono PDB/DWARF/managed-type mapping, record that concrete development gap and implement it before claiming usable debugging. QEMU and ROS debug do not inherit support from a compiler emitting `.debug_line`.

## Final artifact signing and per-module policy

The current interface and its exact CLI, platform, policy and evidence rules are documented in [Explicit per-module secure execution (WSEA v1)](../../../documents/security/wsea.md). Use the admitted actual tool/help and that current contract. `--secure module <wasm-path> <protected-policy-path>` requires exactly one policy rule. Multiple rules require `--secure named-module <wasm-path> <protected-policy-path> <expected-id>`. Select every actual dependency separately and bind the expected signed identity; neither a path nor a valid signature alone selects an identity. The tooling family is `cosign wasm secure policy-init/policy-update/sign/verify/fetch`. No AOT producer or example performs those operations or authenticates a compiler by signing its own report.

Complete compilation, linking, validation, intended stripping, DWARF/source embedding and any existing embedded signing first. WSEA authenticates **all final Wasm bytes**, including custom sections. Then use the operator's already independently trusted keys, CA and protected policy to make the genuine detached `.sig.cose`, `.signer.pem` and `.crl.pem`. Never generate a key/CA on the user's behalf, accept a fetched trust root automatically, or treat this package's self-manifest as a trust anchor. A signature on an earlier assembly/object or pre-embedding Wasm does not authenticate the final artifact. The current protected-storage/OpenSSL/platform gates must also pass; a tool-side verify receipt is not VM authorization.

The signed capability mask must fit the selected local policy rule. Actual imports and import-reset destinations must fit that signed ceiling. Bit1 is basic P1; bit2 is fd/path/poll including standard I/O and additionally requires bit1; bit4 is sockets and additionally requires bit1; bit8 is ROS1; bit16 is ROS2; bit32 is other native imports. A non-ROS stdio example normally needs mask3 plus only its genuine additional imports. A ROS2 example also needs its explicit ROS2 ceiling. Reject unneeded socket/ROS1/other-native permissions. A signed manifest exceeding a narrower local rule is refused, not silently attenuated into success. These ceilings do not authorize ROS aliases/resources/operations: all genuine Runtime/WASI/ROS permissions remain independent. Require real denials for wrong expected module ID, ambiguous multi-rule selection, narrower local ceiling, missing resource grants and unsigned/unselected dependencies under the chosen whole-program policy.

New loads and initialization recheck current evidence, time, policy and exact source identity. Warm compiled caches cannot substitute for that source authorization. Re-sign after any changed final byte, and require current verification for every selected module before a new load. Current WSEA rejects function hot replacement and replacement of already compiled source while secure selection is active; do not promise an in-place debugger reload under that policy. An initial debugger-owned source load or checkpoint source restoration still uses the signed loader; an old receipt cannot authorize changed bytes. Restart through a genuine new verified load when required, preserve current revocation/policy/version-state and denial receipts, and keep actual cache/self-binary SHA/ISA checks and debugger read authority separate. Ordinary debugger memory inspection does not authenticate or re-sign the module.

## Source migration and original libraries

The shared managed SDK is a Wasm facade over explicit numeric Host operations. It is not the original rcl/rclcpp/rcljava/rclpy implementation. To port those implementations at Source level, rebuild rcl/rcutils/RMW dependencies, C/C++ wrappers or managed bindings for wasm32 and replace native process/network/platform callbacks with the reviewed bounded ABI. Audit every operation/ownership/thread/allocator/callback edge and register actual native codec/type identity. A new .NET language can consume the facade without claiming that source transplant complete.

Existing Linux `.so`/Windows DLL/native `.a` cannot be loaded into a Wasm linear heap. Recompile permitted implementation Source as Wasm objects/static archives and link it into the same final module, or deliberately retain a trusted Host provider behind a separately granted import. A .NET IL DLL may be AOT input only with its complete compatible dependencies; arbitrary desktop PInvoke/dlopen/CLR-native escape fails the closed final imports. Original ABI/layout/type-support compatibility and signed method/platform assumptions must be established, not inferred from a matching type name.

Before porting, two native plugins in one process typically share its address space even inside a container. Separate Wasm instances provide separate linear memories and explicit imports; an outer container can remain useful. This is an architectural property, not universal proof of superior security or unchanged performance. Demonstrate actual denied cross-instance/source/operation access, transparent grants/audit and real equivalent applications. Same-instance language classes still share that instance's authority. Vendor/robotics tutorials and ROS1/ROS2 mixed examples must bind their actual independently reviewed native providers and capabilities; these ROS2-only owned examples do not silently enable ROS1 or hardware.

## Complete qualification still required

Run all six ROS/non-ROS language variants in both actual registered modes, actual full32/seven-service values and Native peers, complete missing/wrong/stale/revoked/quota/role/pair/availability grants, after-effect wrapping/GC/OOM, unknown ACK/cancel, late abandonment/full decode, reverse independent close/reentry and real nonzero process failures. The shared `tests/GuestFaults.csproj` uses actual existing SDK guest hooks for pre-submit, after-submit, after-commit, decode-OOM, cleanup, reentry and late-abandon cases; an expected nonzero alone is never a passed fault. Require the exact intended operation, saved actual witness/status, Native count and physical retirement.

Qualify real source-line/typed-local/argv/frame debugging and ROS debugging, current signed/cache/update/ISA behavior, x86_64/aarch64 and admitted extra ROS/QEMU targets. Compare real equivalent Native/Mono8/AOT apps and compatible Wasmtime/WAVM/WasmEdge using hardware timing, VTune hardware counters, hotspot and microarchitecture. Never replace missing counters or failed target support with a Source-only assertion of parity. Keep all work in the existing64GiB group with finite budgets, preserve evidence and remove only owned disposable outputs after retention.
