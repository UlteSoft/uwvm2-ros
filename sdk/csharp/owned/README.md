# Optional C# owned ROS2 services

This is an unqualified source candidate. It has not been generated, compiled,
linked or executed. No native ROS, Mono/WASI, debugger, cache, QEMU or performance
result is claimed. The ordinary C# SDK remains the default. This source variant
requires explicit ROS2 and owned-service selection; ROS1 is refused.

The optional transport uses the actual `uwvm_ros2_rmw_srv_v1` twelve-import ABI.
Mono calls a C object linked into the **same Wasm module**. Managed arrays are
pinned for one synchronous call; the bridge receives numeric offsets into that
module's memory0. It does not marshal a CLR object, retain a managed pointer,
open a socket, load a desktop ROS shared library or select another principal.
The required include directory contains the complete bridge declarations and
includes the existing complete `roswasm/rmw_service_v1.h` ABI contract.

## Source variants and prerequisites

Use this assembly **instead of** `Uwvm.RosWasm.csproj` for an application using
owned services. It compiles the same ordinary core sources plus the optional
files. Do not reference both assemblies: they deliberately contain the same
core types. The existing-file proposal changes `NativeTransport` and `Session` to partial
classes. Both layers share the **actual** admission gate and reentry state,
and the optional Session method reserves a real Capability in the actual core
owner ledger before the numeric open. It does not make ordinary SDK APIs read-only or add imports to them.

The proposed recipe is pinned to .NET/Mono8.0.15, `wasi-wasm`, and wasm32. It
uses the existing `_WasmNativeFileForLinking`/`ScanForPInvokes` seam. The existing
generic interop evidence is not evidence that this new ROS bridge links or runs.
Use a fresh external build root, the real installed WASI sysroot/toolchain and
the actual native compiler required by that SDK version. All following commands
are **future Linux instructions**, not operations performed for this packet.
Run them in the user's existing 64GiB cgroup, and record the actual cgroup,
compiler/runtime/stdlib identities and complete command/status evidence.

## Generate all 32 records and seven service roots

The authoritative parser is `sdk/tools/custom_types.py` together with the exact
current `sdk/custom-types/bounded_profile.py`. The new operator captures four
reviewed source modules from ordinary no-follow FDs, verifies exact SHA256,
executes only captured bytes in fixed private namespaces, and keeps the FDs for
terminal identity/byte checks. It captures the exact original definitions and
gives the real parser only those captured bytes. An ambient `.pyc`, catalog
plugin, alternative parser or `-B` flag cannot substitute the executed helpers.
The actual interpreter and Python standard library remain external execution
TCB and need their own evidence.

First produce the separate authoritative C/C++ CONTROL source revision using
the exact admitted parent/bounded pair and the existing documented source-only
command. Supply its **fresh actual** directory as `CANONICAL`. The C# operator
checks all seven source products plus `generation.json`, exact parser hashes
and the same definition pins. It does not accept an old revision's ledger and
does not manufacture Native identity or registration.

```sh
python3 "$SDK/csharp/owned/generate_control.py" \
  --source-root "$SOURCE_ROOT" --canonical "$CANONICAL" \
  --out "$FRESH_CS_RECORDS" --operator-sha256 "$REVIEWED_OPERATOR_SHA" \
  --emitter-sha256 "$REVIEWED_CS_EMITTER_SHA" --explicit-owned-ros2
```

`SOURCE_ROOT`, `CANONICAL` and `FRESH_CS_RECORDS` must be absolute. The output
must not exist and must be outside both source and canonical roots. A failed
write can leave a partial fresh directory; the proof is written last. Preserve
the failure evidence, remove only that owned partial directory, and start a new
explicit source revision. Do not treat a missing proof as generation success.

The outputs are `BoundedRecords.cs`, `ControlRoots.cs` and
`generation.control-csharp.json`. The operator reuses the complete existing C#
field emitter through counted, exact-base template deltas. It adds the actual
CONTROL raw-byte string domain, original constants, aggregate/cartesian metadata
and local nested-record extent checks. It never edits shared generator files.
Generated value types live under `Uwvm.RosWasm.Generated.Control.BoundedRecords` and
start with `_Ros2...`; native fields keep their complete spelling using `@`.
Every scalar, fixed array, sequence, nested value and declared default is emitted.

The original `TypeSource.raw_file_contents` is a **byte[]**, not a C# string.
NUL and invalid UTF8 remain valid data in that one explicit byte domain. Other
strings are strict bounded UTF8 without NUL. Full unsigned64 uses `ulong`;
native signed64 retains signed bits. Float values must be finite. Sequence
counts are bounded and checked against remaining bytes before allocation.
Every nested record and root must fit its resolved local maximum; a 32768-byte
aggregate wire cap is not the larger cartesian storage maximum, RSS or a native
DDS allocation limit. Original constants consume no wire bytes. CONTROL uses
the actual shared IR semantics; no language-specific extra Time normalization
or invented variant projection is added under an unchanged identity.

`ControlRoots` exposes complete definitions for all seven original services:

| Method suffix after `Ros2` | Original root |
| --- | --- |
| `Rcl_interfacesSrvDescribeParameters()` | `rcl_interfaces/srv/DescribeParameters` |
| `Rcl_interfacesSrvGetParameterTypes()` | `rcl_interfaces/srv/GetParameterTypes` |
| `Rcl_interfacesSrvGetParameters()` | `rcl_interfaces/srv/GetParameters` |
| `Rcl_interfacesSrvListParameters()` | `rcl_interfaces/srv/ListParameters` |
| `Rcl_interfacesSrvSetParameters()` | `rcl_interfaces/srv/SetParameters` |
| `Rcl_interfacesSrvSetParametersAtomically()` | `rcl_interfaces/srv/SetParametersAtomically` |
| `Type_description_interfacesSrvGetTypeDescription()` | `type_description_interfaces/srv/GetTypeDescription` |

Each definition binds the genuine service ROOT codec identity separately from
the complete Request and Response component codecs. Generated type names,
RIHS/codec strings and the generation proof are source facts, never permissions
or proof that installed native type support matches.

## Build and inspect the final module

Compile both same-Wasm bridge objects using the real Mono/WASI compiler and
sysroot. Exact compiler selection is an explicit prerequisite; this recipe
does not guess a desktop compiler or silently fall back to it.

```sh
"$WASI_CC" --target=wasm32-wasi --sysroot="$WASI_SYSROOT" -O2 \
  -DROSWASM_BACKEND=2 -I"$SDK/include" \
  -c "$SDK/csharp/roswasm_bridge.c" -o "$FRESH_BUILD/roswasm_bridge.o"
"$WASI_CC" --target=wasm32-wasi --sysroot="$WASI_SYSROOT" -O2 \
  -DROSWASM_BACKEND=2 -DROSWASM_OWNED_SERVICE=1 \
  -I"$SDK/include" -I"$SDK/csharp/owned/include" \
  -c "$SDK/csharp/owned/roswasm_owned_bridge.c" -o "$FRESH_BUILD/roswasm_owned_bridge.o"
dotnet publish "$SDK/csharp/owned/examples/OwnedExample.csproj" \
  -p:RosBackend=2 -p:RosOwnedService=1 -p:RosOwnedExample=TypeQuery \
  -p:RosOwnedGeneratedDir="$FRESH_CS_RECORDS" \
  -p:RosNativeBridgeObject="$FRESH_BUILD/roswasm_bridge.o" \
  -p:RosOwnedBridgeObject="$FRESH_BUILD/roswasm_owned_bridge.o" \
  -p:RosNativeEvidenceDir="$FRESH_EVIDENCE" -p:RosBuildRoot="$FRESH_BUILD" \
  -o "$FRESH_BUILD/publish"
```

The native object basename must be exactly `roswasm_owned_bridge` for the
managed DllImport module. Inspect the actual linker/PInvoke/native item output;
an XML target merely requesting a file is not proof it reached the final Wasm.
Select `RosOwnedExample=Parameters` in another fresh build for the second app.
The existing original bridge is still required by the actual Session.

Inspect the **actual final** Wasm's complete import signatures:

```sh
python3 "$SDK/csharp/owned/check_final_imports.py" \
  --source-root "$SOURCE_ROOT" --wasm "$FINAL_WASM" \
  --reader-sha256 "$REVIEWED_WASM_READER_SHA" \
  --external-signatures "$EXPLICIT_EXTERNAL_SIGNATURES_JSON" \
  --report "$FRESH_EVIDENCE/final-import-signatures.json"
```

The external JSON is an explicit array of actual non-ROS function imports with
exact `module`, `name`, `kind:0`, `params` and `results`. Begin from an actual
inventory, review it, then check a fresh artifact. Do not use module-wide `env`
or WASI exceptions. A nonfunction external import currently refuses this narrow
recipe. The checker requires all twelve actual optional service functions,
permits only original ROS2 business functions with their exact old signatures,
and refuses wrong-backend or other optional ROS namespaces. Existing client3
contracts are unchanged; this particular variant does not select them.
Full Wasm3.0 validation is separate. Signature inspection proves neither valid
code bodies, actual execution, Native compatibility nor a Runtime grant.

## API, permissions and effect semantics

Construct `OwnedGroup(maximumEndpoints, transactionsPerEndpoint,
maximumRequest, maximumResponse)` with explicit ceilings. Storage is prepared
before any capability open. At most256 endpoints and65536 total nonreused history
rows are admitted; body maxima are at most32768. Choose small real application
ceilings rather than the maximum. `OpenClient(definition, callAlias,
observerAlias, optionalAvailabilityAlias)` opens each caller-selected actual
Session capability independently. `OpenServer(definition, advertiseAlias,
responderAlias)` does the same for two independent server roles. Construction
grants nothing. Distinct aliases/full owners are required; aliases are not
invented, looked up via graph APIs or inferred from a type name.

`Client.Submit(request)` returns an opaque full-width Gateway `Ticket`; it is
not native request completion or a signed native sequence. `Poll()` returns a
fully decoded and committed `Delivery<T>` or an idle result (`Present=false`).
Check `IsControl` before reading `Value`. `Information` retains the actual
same-take signed native sequence and timestamps and16-byte writer GUID. An
endpoint receipt separately retains both actual QoS80 values and a32-byte
native GID extent. Real zero-valued metadata stays data.

Decode, result storage and all potentially allocating body work happen before
ACK. The concrete raw call witness exists before entry and stores `Entered`,
`Returned`, actual uint code and exceptions before another allocation. `Entered`
means only the concrete interop call expression was attempted. It is not a
Guest claim that Host code actually executed. A returned error is retained; an
entered exception or unknown ACK makes the endpoint terminal. The SDK performs
no automatic resend, response retry, ACK retry, reopen or outcome inference.

`State(ticket)` reports local history. `Abandon(ticket)` is allowed once only
for `Admitted`; it does not promise DDS cancellation. An abandoned ticket can
receive one late full response: the SDK decodes and ACKs it, discards the value,
and records `Discarded`, without reporting request completion. Duplicate late
responses fail closed. No history row is recycled. `Available(waitNanoseconds)`
requires a separately granted availability owner, queries the actual retained
client and permits explicit bounded waits up to the admitted Host limit. It
does not create a probe client or use graph discovery as a substitute.

`Server.Poll()` returns an actual complete request ticket and metadata;
`Respond(ticket,response)` sends at most once for that delivered ticket.
`ServeOnce(handler)` runs the caller's synchronous handler and complete response
encode under the mutation lease. A handler/codec/close hook reentering the group
gets17 before a second dispatch. A handler failure does not send an empty or
successful response. Group mutation is synchronous; arbitrary concurrent or
asynchronous native retention is not supported by this API contract.

`Close()` once-retires a validated native incarnation, then independently closes
every raw owner in reverse order. Failed/dead owners never short circuit other
closes. No all-live preflight can hide remaining owners. The Group busy lease
remains held, while no Session/Capability lease spans raw cleanup. Retained
full-width IDs and every cleanup status/exception remain in `Diagnostic` and
`RawCloses`. A close hook runs at most once. A proven pre-entry raw-close failure
retains that actual owner for later explicit cleanup; an entered close is never
revived. There are at most24 recorded raw cleanup attempts per endpoint, after
which retained owners remain visible and cleanup fails. Unknown activation with
no valid incarnation cannot guess a native-retire argument; raw owner closure
and actual Runtime teardown are required and the original error remains.

Call `CloseAll()` in an application `finally` block and treat false as nonzero
failure. It preserves primary failures separately from cleanup; the examples
print both after cleanup. A committed terminal control is returned before
deferred cleanup, so cleanup errors cannot replace an already committed event.
Fatal process abort/uncatchable OOM, runtime marshalling failure, actual pointer
width and pinning behavior still require real Mono/WASI fault qualification.

## Actual caller-driven applications

`examples/TypeQuery.cs` submits the original `GetTypeDescription` request with
the caller's actual type name/hash and `include_type_sources=true`. Arguments:

```
CALL_ALIAS OBSERVER_ALIAS AVAILABILITY_ALIAS_OR_- TYPE_NAME ACTUAL_RIHS STEPS
```

It requires a genuinely successful full typed response, source records and
matching Gateway ticket. It never executes returned source bytes. A local step
count bounds work; it is not a wall-clock deadline. On exhaustion the client
explicitly abandons its admitted ticket and exits nonzero after cleanup.

`examples/Parameters.cs` has three modes:

```
get CALL OBSERVER PARAMETER_NAME STEPS -
set CALL OBSERVER PARAMETER_NAME SIGNED_INT64 STEPS -
serve GET_ADVERTISE GET_RESPOND SET_ADVERTISE SET_RESPOND OWNED_NAME INITIAL_INT64 STEPS
```

Clients submit actual original Get/SetParameters values and require the actual
correlated response. The server retains one caller-selected application int64
parameter, exposes read and write via distinct actual service pairs, and sends
complete per-parameter denial results for another name/type. It does not pretend
to transplant a native Node or native parameter store. Response storage is
allocated before local application state mutation. A subsequent unknown native
response still cannot be undone or retried; application state and transport
outcome are distinct. Applications requiring durable exactly-once semantics
need their own actual idempotence/persistence design.

## Source porting versus a new facade

A newly developed C# application may use these complete generated values and
the explicit owned client/server APIs. It gets only the aliases granted to its
actual Runtime instance/generation and authenticated native session. Native
type support, QoS, service target and pairing are verified again by the Host
and Gateway. Guest declarations, receipt numerals and generated type hashes
cannot elevate that authority. Original default policy denies ungranted
hardware; a simulation label does not make a hardware grant.

Porting existing rcl/rclcpp/rclpy/rcljava is a separate **source-level** task:
replace platform dependencies, executors/waits, allocator/thread assumptions and
native middleware linkage, then bind their actual operations to reviewed Wasm
interfaces. Calling this C# facade is not proof those libraries have been ported.
Native `.so`/`.dll` files cannot be loaded as Wasm guest code; retained native
libraries execute in the trusted Gateway/provider and remain native TCB. A
static `.a` must contain compatible Wasm objects for guest linking, or must be
rebuilt from source; a host-ISA archive cannot be made portable by renaming it.
Every original library moved into the guest must be rebuilt and audited with its
actual imports. Device/native provider authority stays independently explicit.

Containers can complement deployment isolation, but this source does not prove
it is universally safer or equally fast. The narrower per-instance capability
and full audit association are concrete contracts to test. Multiple ROS1/ROS2,
vendor bridges and mixed systems need actual separately authorized gateways;
this ROS2-only optional SDK does not silently select a cross-version adapter.

## Required tests and honest qualification

`tests/OwnedSourceFixtures.cs` uses a compile-time-only **local** model and the
actual group/wire implementation. It covers pre-entry busy refusal, after-effect
exception, full unsigned tickets, decode OOM before ACK, unknown ACK no replay,
abandoned late fulltake, duplicate late refusal, reverse failing raw closes,
close-hook reentry and committed terminal control before cleanup. The injection
factory is absent from ordinary builds. Model-entered is only model evidence.
These files were not compiled or executed here and cannot qualify native ROS.

`tests/GuestFaultFixture.cs` and its separate project are actual Guest fixture
source using the real raw bridge and full generated GetTypeDescription values.
Its explicit compile flag adds a before-entry hook and an after-returned-status
hook; no callback exists between the entered store and actual P/Invoke. Normal
builds contain neither hook. Modes include `pre-submit`, `after-submit`,
`after-commit`, `decode-oom`, `cleanup`, `reentry` and `abandon-late`, followed by
the caller's call/observer aliases, actual type/hash and finite poll steps.
An after-return fault preserves the actual returned uint before recording the
injected exception. Decode OOM follows a real complete decode and precedes ACK.
These fault applications deliberately exit nonzero and print all witnesses;
nonzero alone is never a test PASS. A controlled real provider must establish
the late-response schedule; idle polling alone cannot prove a late fulltake.
Both the source and build commands remain unexecuted.

Native retirement preparation failures are retained in up to eight separately
preallocated witnesses; an entered retirement is never repeated. Cleanup
success stays false after an observed earlier failure, even if a later explicit
pre-entry retry physically closes the retained owner. Inspect the individual
history and actual remaining owner state rather than treating a later0 as
erasing an earlier error. Each original numeric open has a separate preallocated witness and returned
full handle field. A returned-success handle is adopted into its already
reserved actual Capability before any fallible validation/reporting. The old
generic open Result allocation is avoided; ordinary Session.Open stays intact.
An entered interop throw without an actual returned status remains unknown,
even if output bytes look plausible. It requires actual Runtime teardown, not
a guessed handle or retry. No general concurrent attribution is claimed.

Future actual Guest/native fixtures must additionally verify pinning/memory0,
header corruption and all enum/reserved fields, output preservation on every
nonzero status, queue controls, stale generations/incarnations, partial factory
rollback, failed inspection, busy close, high-bit handles/sequence/nonce,
actual native availability, real request/response metadata and unknown ACK
teardown. Exercise both x86_64 and aarch64 Gateway hosts in the existing64GiB
cgroup, and supported additional platforms only when actually available. Test
DBG including ROS2 behavior, SHA-bound code-cache invalidation and CPUID/ISA
compatibility through the actual runtime path. Run baseline and candidate
VTune hardware-counting/hotspots/microarchitecture comparisons with real
workload/compiler/build pins; source complexity is not a performance result.
Keep wasmtime/WAVM/WasmEdge comparisons scoped to genuinely comparable features.
Preserve raw statuses and clean only owned build/results when space is needed.
