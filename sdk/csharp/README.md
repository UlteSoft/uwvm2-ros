# C# ROS Wasm SDK

This is a concrete capability SDK for the actual Mono WASI native-object bridge.
It contains every current ABI v1 import: 38 ROS1 plus 43 ROS2 declarations and C
forwarders. Exactly ONE backend is selected per assembly/native-object pair.
There is no host socket relay, native SDK fallback, dynamic ROS discovery, or
implicit alias grant. The [complete C# qualification](../../test/0050.high_level_languages/results/native-budget/language-csharp-records-jointstate-20261019/README.md)
now builds twelve real fixture/ROS1/ROS2 JointState applications, verifies the
actual selected static bridge, P/Invoke table, original-object link response
and final binary import signatures, and executes sixteen default-disabled
JointState outcomes on the frozen ROS Release/Debug products. The unchanged
record fixture also passes 24 UWVM runs across all three repositories, completing
360 methods and 1368 checks, with 24 expected corrupt-golden assertion failures.
Positive native JointState delivery and the remaining SDK operation semantics
are not qualified by these results. Full official rcl/rclcpp/rclpy/rcljava
compatibility remains separate work.

## Native/managed contract

`SelectedRaw.cs` uses actual `DllImport("roswasm_bridge")`, exact C entry names,
Cdecl and numeric uint32/ulong64 parameters (Wasm i32/i64). `roswasm_bridge.c` calls the existing
selected SDK C ABI declarations without changing their signatures/operations.
The bridge refuses a non-Wasm compile. Native Linux generic interop is a
separate proof, not a way to execute these ROS imports on the host.

`NativeTransport` supplies actual copied canonical request bytes, pinned managed
byte arrays for request/output/eight-byte slot, and explicit memory index 0.
All allocations and all three pins precede address extraction. The pinned
handles remain live until the complete synchronous import returns. wasm32
IntPtr offsets explicitly zero-extend the 32-bit pattern; no object header,
managed reference, arbitrary host pointer or truncated handle enters an import.
The arrays also remain managed references in scope. The backend invokes the
host once and does not resize/move a pinned array during the import.

Only output LENGTH uses `ulong.MaxValue` as an untouched sentinel. Open and
submit initialize their VALUE slot to 0, reject successful 0 and accept every
other opaque ulong, including ulong.MaxValue. ABI version is a value, not errno.
Successful lengths must fit the admitted output capacity; error 90 exposes only
an actually written required size, never partial output. Error 110 may have
admitted external effects. There is NO retry, resize-and-replay or backend
fallback on 90/110/107. Submit's ID is admission, not completion or a capability.

A successfully opened raw handle stays in the bridge's cleanup obligation until
the managed Result has been constructed. If that construction fails, exactly
one direct close is attempted while the real numeric handle is retained.
`LastOpenCleanupError`/`LastOpenCleanupException` record that failed-call cleanup
without allocating or replacing the primary exception. These are latest-call
local diagnostics, not a persistent audit log; capture them before another
native invocation. An admitted service ID is never passed to capability_close.
A process kill, unrecoverable runtime failure or stack overflow still requires
the independent Runtime/Instance owner; a managed finally cannot promise to run
on those paths.

## Ownership, callbacks and errors

Create a Session with a finite owner limit. It preallocates its owner ledger.
Open allocates/registers a pending Capability before the native open and commits
only its actual nonzero handle. Capabilities remain scoped to that Session and
selected backend. Revoke invalidates every local sibling for the same alias
before the one native revoke. It never grants/reopens a revoked alias.

All shutdown/cleanup is explicit. Session.CloseChecked(primary) allocates the
entire cleanup report BEFORE any retirement, refuses a busy group before any
effect, retires live owners once in reverse order, attempts remaining owners
after ordinary managed exceptions and retains every result/exception separately.
The original application error is preserved in `Primary`; cleanup errors are
not relabeled as its cause. Call `CloseChecked` in your own try/finally and
inspect all Items. `Dispose` is convenience for a normal success path; C# using
can replace an application exception with a disposal exception, so it is not
the primary-preserving exceptional-path API. Fatal process/runtime termination
is an outer-owner obligation. No finalizer performs ROS I/O.

Node factories implement topics, synchronous/async Service clients, Service
servers, typed Action clients/servers and versioned observers. All multi-role
factories roll back only handles they actually acquired; `LastSetupCleanup`
retains up to five reverse-order rollback outcomes without allocating after
cleanup. Each new factory resets that local diagnostic. Advertise/respond and
send/cancel-own/feedback/result aliases are distinct. Failure 90/110 is returned
without reissuing the operation; ambiguous effects remain the caller's problem.

Subscription/Service/Action callbacks and codec decoding hold their owner group
busy, so close/revoke cannot partially retire a callback's resources. Callbacks
run on the guest's caller thread, never from a native ROS thread. The Executor
performs one bounded round-robin pass and refuses source mutation/reentry while
spinning. Poll 11 means idle. There is no implicit clock permission or background
wait. This first SDK's Node/group APIs are designed for ONE guest execution
thread; they do not advertise a multithreaded ROS executor. The native buffer
frame independently serializes and refuses reentrant imports.

ActionClient decodes the full matching ID/outcome/typed component before retiring
a local goal. Feedback does not retire it. Cancel-own acknowledgment does not
retire it; no cancel-any role is inferred. ActionServer owns each successfully
polled ticket, emits complete feedback/result envelopes and retires only after
actual successful terminal response. Its codecs are supplied explicitly, not
selected by untrusted native type strings. Resource closure does not by itself
prove physical stop/hold behavior for a robot.

Observer owns one fresh queue and one global sequence across all watched sources.
It checks the exact 64-byte header layout, reserved bytes, kind/backend/flags,
full body length, nonzero identities, source/correlation binding, whole queue
sequence and stable Instance/session before publishing an Event. A resource
error/close is metadata, not a successful native payload. Sharing/externally
consuming this queue invalidates its fresh/exclusive sequence contract.

ICodec<T> supports complete explicit custom values. Builtin codecs cover strict
counted UTF8 String, Bool, all six Twist doubles, exact signed int64, finite
float64, empty Trigger, full Service response and bounded scalar arrays.
Embedded NUL/BOM and all Unicode scalars are preserved by builtin String; this
is distinct from the custom bounded-profile string policy. Decoders reject
truncation/trailing bytes/nonfinite values. The optional [bounded-record generator](../custom-types/MANAGED_CSHARP.md)
provides complete typed C# values/codecs and has actual complete-record WASI
coverage. Native registry/identity export and installed native qualification
remain separate requirements. Preparation catalogs retain null native IDs;
the SDK does not fabricate RIHS, MD5 or vendor native identities.

## Real pinned native hook and build

The actual generic integration source under
`test/0050.high_level_languages/csharp-native-interop` establishes the Mono 8.0.15
hook shape with .NET SDK 8.0.408 and WASI SDK 20.0. This SDK does not modify that
project or its receipts. `NativeBridge.targets` uses the same private
AfterTargets `_GetNativeFilesForLinking` / BeforeTargets `_GenerateManagedToNative`
hook and adds the SAME original object to `_WasmNativeFileForLinking` with
`ScanForPInvokes=true`. Its filename MUST be `roswasm_bridge.o`: the filename
supplies the DllImport module, and it must also be in the final link response.
Private targets are version-specific. Requalify actual source/installed target
bytes and fail on drift; never patch installed SDKs as an automatic workaround.

The [actual WASI build tutorial](../../tests/ros_custom_types/managed-csharp/README_WASM.md)
provides the qualified complete-record and JointState project commands,
private SDK/metadata isolation, baseline versus retained native DWARF outputs
and exact result boundaries. The generic `RosExamples.csproj` invocation below
remains a recipe; the new matrix executes the separate JointState lesson.
The build owner must admit actual tools, caches, complete source and native
object dependencies, full MD/link/PInvoke/Wasm results and process cleanup:

```sh
"$WASI_CC" --target=wasm32-wasi --sysroot="$WASI_SYSROOT" -O1 -g -MD \
  -DROSWASM_BACKEND=2 -I "$SOURCE/sdk" \
  -MF "$OUT/roswasm_bridge.d" -c "$SOURCE/sdk/csharp/roswasm_bridge.c" \
  -o "$OUT/roswasm_bridge.o"
"$DOTNET" restore "$SOURCE/sdk/examples/csharp/RosExamples.csproj" \
  -p:RosBackend=2 -p:RosBuildRoot="$OUT" \
  --source "$EXISTING_EMPTY_LOCAL_FEED" --packages "$EXISTING_PINNED_PACKAGES"
"$DOTNET" build "$SOURCE/sdk/examples/csharp/RosExamples.csproj" --no-restore -c Release \
  -p:RosBackend=2 -p:RosNativeBridgeObject="$OUT/roswasm_bridge.o" \
  -p:RosNativeEvidenceDir="$OUT/link-evidence" \
  -p:RosBuildRoot="$OUT" -m:1
```

Use backend 1 consistently in both compilation steps for ROS1. Do not combine
objects or both compile symbols. Both local Directory.Build.props files use the actual project name under the
explicit RosBuildRoot for distinct intermediate/output directories. Keep those
files when staging; do not replace them with one shared global obj path. Resolve all
SDK/workload/reference packs from actual existing admitted installation. No
network/cache/SDK replacement is authorized by these recipes.

Before any ROS command, inspect actual generated P/Invoke tables for the one
`roswasm_bridge` module and every selected exact symbol/prototype; inspect the
full native link response for the exact original .o. Validate the full Wasm and
its actual imports (only selected ROS backend plus the reviewed Mono WASIp1
vocabulary), memory/entry/exports/names/debug data. Numeric prototype presence
alone does not prove execution, permissions, native SDK identity or delivery.
Suggested future bounds: low-job serial build, <=8 GiB per process within Root's
explicit admitted 16 GiB aggregate/current 64 GiB leaf, whole-source/SDK admission,
backing 8 GiB reserve plus declared growth and independent final cleanup. These
are source guidance; no future boot/cgroup/source/tool pin is invented.

The managed pin API semantics were checked against the actual .NET 8.0.15
[GCHandle implementation](https://raw.githubusercontent.com/dotnet/runtime/v8.0.15/src/libraries/System.Private.CoreLib/src/System/Runtime/InteropServices/GCHandle.cs)
and [Mono internal calls](https://raw.githubusercontent.com/dotnet/runtime/v8.0.15/src/mono/System.Private.CoreLib/src/System/Runtime/InteropServices/GCHandle.Mono.cs).
The former returns the array data reference for pinned arrays; the latter uses
the Mono handle allocator/free/get calls. This is primary-source API research,
not a claim that this candidate's GC behavior has run in the installed SDK.
