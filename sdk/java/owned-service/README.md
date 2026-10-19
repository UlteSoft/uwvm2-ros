# Java retained ROS2 services (optional owned RMW v1)

This is a source-only Java SDK increment for TeaVM 0.13.1
`WEBASSEMBLY_WASI`, with the existing linear heap and memory0. It has not been
parsed, generated, compiled, instantiated or executed. No native ROS, debugger,
QEMU, cache/CPUID or VTune result follows from this source packet. The Linux
owner must independently qualify those stages in the existing 64GiB cgroup.

The optional namespace is `uwvm_ros2_rmw_srv_v1`. Its complete twelve numeric
signatures and output extents come from `sdk/abi/rmw-service-v1.json` and
`sdk/include/roswasm/rmw_service_v1.h`. The original business81 and optional
service-client3 catalogs/imports keep their existing implementations. There is
no ROS1 fallback, JVM/JNI provider, graph probe or hidden capability grant.

## Explicit selection and trust

A normal application continues to use its existing transport. New code selects
`new Ros2OwnedServiceTransport()` and passes that transport to its real
`Session`. Constructing a transport, session or group opens nothing. Production
`OwnedService.Group` refuses a custom/fake business transport. A package-private
constructor exists solely for the separately labelled local source models.

The producer defaults `--ros-rmw-service-enabled` to `0`. Enabling it requires
backend2, the exact optional catalog SHA, explicit source root, checker and
binary-inspector source SHAs, and a reviewed full import multiset with its SHA.
No `--allow-module` bypass exists on this path. All twelve optional imports must
be present with their exact actual function signatures. Business and client
imports additionally match their existing catalogs; WASI is a caller-reviewed
exact module/name/kind/parameter/result policy, with no namespace allowance.
That policy is not automatically derived from the output being checked. It is
not a proof that a WASI provider supports a requested function: independent full
Wasm validation, linking and the actual Runtime configuration remain required.

The Host obtains the genuine Runtime caller. The caller opens independently
granted aliases for one exact pair: `service.call` and `event.observe`, or
`service.advertise` and `service.respond`. A client may additionally open its
separate `service.available` alias. Source/type/QoS/resource matching and all
quota debits remain Host/Gateway decisions. Receipt numbers, tickets, schema
strings and canonical codec identities are values, never bearer authority.
An empty or denied deployment stays denied. Do not substitute a fake
`HostTransport`, an invented native identity, or a blanket permission policy.

## Local owner and storage APIs

```java
Session session = new Session(new Ros2OwnedServiceTransport(), 3);
OwnedService.Options limits = new OwnedService.Options(
    1, 8, definition.requestMaximum, definition.responseMaximum,
    0, 1024 * 1024);
OwnedService.Group group = new OwnedService.Group(session, limits);
OwnedService.Opening<OwnedService.Client<Request, Response>> opening =
    group.client(definition, callAlias, observerAlias, availabilityAliasOrNull,
                 expectedRequestQos, expectedResponseQos);
```

`Request`/`Response` and `definition` above denote actual generated complete
record types and one generated service factory, not classes shipped as names
`Request` and `Response`. `examples/TypeQueryApplication.java` and
`ParameterApplication.java` contain concrete types and calls.

The constructor preallocates every endpoint's input/packet buffers, receipts,
history entries, GUID storage and diagnostics before the first capability open.
Options allow 1..256 endpoints and 1..256 transaction attempts per endpoint.
Request and response maxima cannot exceed the actual new Host input bound
4193626 (`4194304 - 518 - 160`); business81 retains its old bound. Peek capacity
is `352 + selected component maximum`, not the shared scratch's larger extent.
The storage allowance covers an explicit logical payload/state allowance;
it is not exact Java object overhead, GC/RSS, stack or native DDS memory billing.
The original32 record profile independently limits canonical wire to32768. Application-owned decoded values, outstanding Delivery objects and copies retained by callers are additional memory; the group allowance is not a process memory limit.

`Session.preflightOwners` checks the real owner ledger before opens. A failed
factory remains a retained diagnostic row; attempts and handles are never
recycled behind the caller's back. The wrapper and opening result exist before
activation. Failed partial construction once-retires any structurally proved
native binding and independently closes all successfully opened raw owners.

Aliases are nonempty strict UTF8 without NUL and at most255 bytes. The aliases
within this group are distinct, including historical rows. The optional
availability alias may be null; this grants no availability operation and causes
`available` to return95. The factories check actual generation/index handles and
refuse duplicate actual handles. A receipt must match both explicitly expected
native QoS profiles and the group's actual instance/generation/native session.

Use the twelve-field `Qos` constructor, or `Qos.fromBytes` on exactly80 checked
canonical bytes. Both profiles are required. The group does not change native
QoS or invent defaults from a service name. Examples accept each complete80-byte
profile as160 hex digits from the caller. A receipt retains two profiles plus
an actual client GID of extent0..32. A present all-zero GID is valid data and is
not a request writer GUID.

## Transactions and complete decoding

`client.submit(request)` encodes the full request under the group lease, reserves
one preallocated history slot before sending, and returns `Submission` with a
full-width `long` Gateway ticket. A Java `long` holds u64 bits: use
`Long.toUnsignedString`/`parseUnsignedLong` for display/input. No ticket, handle,
nonce, stream sequence, native sequence or offset travels through `double`.
Native request sequence and timestamps remain independent signed64 bit patterns.
A submission is not a completed response. Even Again11 consumes this finite
local attempt slot; the SDK does not replay an uncertain send or recycle history.

`takeResponse` and `takeRequest` return owning `Delivery<T>` values. They first
inspect the same actual native endpoint; then validate the full UMRC160,
URMI192, source/pair/incarnation, exact capacity/payload extents, accepted stream
sequence and fresh independent reservation nonce. Reserved/padding bytes,
control flags, actual GUID16 extent and both source timestamps are checked.
A native GID32 is never substituted for the same-take request writer GUID16.
All-zero actual GUID/timestamp values remain data. Control records preserve
actual kind/error/flags/correlation and contain no fabricated service metadata.

A complete body copy and complete generated record decode occur before ACK.
The result, metadata and server history storage already exist. Successful ACK
is followed only by scalar writes and fixed bounded copies into those reserved
objects. Decoder/copy/OOM failure before ACK cancels exactly one proved borrow;
a successful cancel preserves the record and a later explicit take needs a
fresh nonce. No malformed/unassociated prefix is used to fabricate an ACK.
There is no retry after unknown commit/cancel/submit/respond/retire. Those cases
poison the endpoint and once-retire its native association, retaining115 and the
original throwable separately from cleanup results.

`client.abandon(ticket)` marks only local abandonment and is attempted once.
It does not claim native/DDS cancellation. A later actual abandoned response
still passes the complete metadata/body decoder and successful exact ACK; its
result has `abandoned=true`, no published value and code125.
`server.respond(ticket,response)` requires a previously delivered request, fully
encodes before the send, and tombstones its response attempt before importing.
Server history also refuses duplicate actual GUID16/native-sequence pairs,
independent of the opaque Gateway ticket.

`available(waitNs)` uses only the same retained client and its separate live
availability owner. Wait is nonnegative, finite and at most the explicitly
selected group bound (never above60e9ns); the Host/native original deadlines
still apply. A nonzero errno is not reported as readiness=false. No graph lists,
new probe client, implicit sleep or synthetic response is used.

## Callbacks, exceptions, GC and cleanup

This version is for one Guest thread. A group lease covers encoding, complete
decoding, factory work and effects. Recursive operations, factory calls,
close/revoke and callback reentry on that group return17 before importing.
All existing group owners are also leased through their real `Capability`
objects, so Session retirement is refused during a live group operation.
Concurrency/Guest-thread support and original rcljava executor callback semantics
are separate unqualified work, not implied by this finite API.

The concrete bridge uses TeaVM's actual `Address.ofData(...).toLong()` payload
intrinsic and a static strong root for the entire synchronous call. `Address.pin`
is a no-op on this backend and is not used. All scratch is reserved before opens;
no managed allocation lies between numeric address extraction and the import.
This is not a WasmGC object-pointer bridge. The original business transport keeps
its original allocation/retirement code. A trap/process abort is not recoverable
Java exception cleanup; the Runtime/Gateway lifetime owner must reclaim it.

`closeChecked` retires each admitted native incarnation at most once, then
independently attempts each raw owner in reverse order even after earlier errors
or exceptions. `revokeChecked` additionally attempts explicit original alias
revocation before independent remaining closes. Repeated close has no imports.
Every primary, cancel, commit, native-retire, raw-close/revoke code/throwable is
retained separately. A known preparation failure remains a real live owner in
the original Session ledger; this facade neither discards nor automatically
retries it. Recovery uses the Session's explicit known-not-attempted retirement
API under the caller's supervision. Attempted/unknown raw effects never replay.

`group.diagnostic(row)` is a detached deep snapshot of fixed report fields.
Opening results also contain detached preallocated snapshots. Mutating those
reports cannot alter the live once-only flags, source identity or history.
`operations` indices are0 ABI discovery (returns the actual version),1 activate,
2 inspect,3 submit,4 abandon,5 respond,6 peek,7 commit,8 cancel,9 available,
10 retire. Outcome separates attempted, returned, code and failure. Other codes
are u32 errno bits held in Java `int`; this SDK does not turn remote errors into
an invented native success.

The two application entries reserve `OwnedApplicationFailure(session, group)`
before their first open. It is a stackless failure envelope, not the original
primary Throwable: `primaryFailure()` and `getCause()` preserve that exact
original object. `finish()` observes group cleanup, Session shutdown and fixed
snapshots independently, without printing, suppression or a new exception.
It retains every existing native/raw Outcome, complete receipt/prefix evidence,
original/remaining owner IDs and the actual raw retirement attempt facts.
`copyDiagnostic(row, target)` copies into a caller's already reserved Diagnostic;
mutating that target cannot change the saved envelope or live ownership.
`owner(row, slot)` is a read-only snapshot, including preparation failure and
the full original handle. Unknown/fatal execution still needs Runtime teardown.

After the finally block, `throwIfFailed()` throws the same preallocated envelope
for a caught body Throwable, a nonzero cleanup return or cleanup throw, remaining raw owner or
snapshot/shutdown/reporting failure. Successful business work followed by failed
cleanup therefore cannot return normally. No null Throwable is thrown, and no
`addSuppressed` or diagnostic printing is needed to retain parallel failures.
The source fixture `ApplicationFailureContract` checks this envelope over actual
Group/Session model paths; `ApplicationFailureGuest` calls both real application
main methods under externally controlled real cleanup faults. Neither has run.

## Complete original control records and service roots

Run `generate_control_java.py` only in the Linux owner's admitted interpreter,
with exact captured source SHAs. It loads the actual parent/bounded/managed
modules from the same captured bytes, under fresh private module names and
cleans its own registrations. It never uses their legacy `_trusted_module`
SourceFileLoader entry. The parser consumes only the previously captured exact
catalog/definition bytes. Python/stdlib/interpreter startup remains a keeper
trust boundary; this loader does not make Python hermetic or retrospectively
authenticate its own entry's executed bytes.

The adapter uses actual `bounded.load_catalog` and the existing complete
`managed_profile.emit_java` with a CONTROL-only extension. It binds the separate
same-source canonical C/C++ seven products and eighth `generation.json`. It
requires all32 actual records and seven Service/two Topic roots, not a partial
handwritten codec or JSON projection. The resulting `BoundedRecords.java`
retains every declared field, bounded arrays, raw-byte versus strict UTF8
strings, aliases/defaults/constants, full u64 values, finite floats and
per-nested aggregate limits. `generation.control-java.json` records full actual
IR, original source pins, native identities if supplied, canonical identities,
aggregate/cartesian bounds and emitted product hashes. Preparation identity
null remains null; no native RIHS is computed or registered by this operator.

`ControlServices.java` supplies full typed `OwnedService.Definition` factories:

- `Ros2Rcl_interfacesSrvDescribeParameters`
- `Ros2Rcl_interfacesSrvGetParameterTypes`
- `Ros2Rcl_interfacesSrvGetParameters`
- `Ros2Rcl_interfacesSrvListParameters`
- `Ros2Rcl_interfacesSrvSetParameters`
- `Ros2Rcl_interfacesSrvSetParametersAtomically`
- `Ros2Type_description_interfacesSrvGetTypeDescription`

The complete32 dependency closure includes all ParameterValue alternatives and
the complete TypeDescription/TypeSource graph. `raw_file_contents` is owning
`byte[]` and may contain NUL/invalid UTF8. Native field spellings remain unchanged.
No native registry/permission table is emitted. Canonical byte capacities can
narrow dynamic native fields; actual installed native source/type/layout still
must be independently matched by the real Gateway before endpoint creation.

## Applications and migration

`TypeQueryApplication` calls the original GetTypeDescription Service with caller
supplied type name, actual type hash string and source-selection flag.
`ParameterApplication` calls original GetParameterTypes with caller supplied
parameter names. Both use real Session opens, actual retained client availability
when selected, full typed requests/responses, finite polling, exact metadata and
once-only cleanup. They do not pretend unavailable native ROS has run. The
recipe gives the concrete class names and input layout; actual deployment must
supply the matching grants/native registries and trusted Gateway instances.

These are new application facades. They are not a claim that unmodified
`org.ros2.rcljava` classes, futures, node options, executor/thread semantics or
all JNI operations were migrated. A true rcljava source port must retain the
pinned original sources, replace each JNI/native boundary with checked typed
imports/owner lifetime, audit callbacks and errors, and qualify real upstream
applications. The existing `sdk/compat/rcljava` tools document separate earlier
source-port and optional availability scopes; this increment does not relabel
those paths as raw12 migration.

A native `.so`/JNI DLL cannot be loaded inside this Wasm module. Keep genuine
native ROS/RMW/vendor libraries at the trusted Gateway side. A native `.a` is
also not a Wasm object archive: rebuild portable application code for this
Wasm ABI or replace its native boundary at source. Host/native shared-library
versions and runtime privileges require their own audited deployment identity.
The Guest sees only explicitly granted imports and memory copies. Permissions
can be narrower and more transparent than a broad process/container policy,
but this source packet supplies no benchmark or universal container-comparison
claim. Measure actual deployment costs/security and include the native provider.

## Qualification still required

The source fixtures cover complete decoding, unsigned high-bit identities,
late abandoned responses, recursion17, unknown send/ACK/retirement, independent
reverse cleanup, malformed metadata, raw source bytes/constants/defaults and
binary missing/duplicate/wrong signatures. They are not executed results.
`Raw12ImportGuest` is only an unprivileged numeric denial/import fixture; it opens
no owners and is never a native positive test. The two real caller-driven apps
are the separate positive route.

Before any support claim, the Linux owner must admit all source/tool binaries,
interpreter+stdlib/extensions and exact process/cgroup inode/membership,
retire descendants with bounded pipe drain, retain RSS/output limits and actual
termination evidence, run full Wasm validation and final exact binary inventory,
then run real denied and permitted Native/Gateway/ROS cases. Repeat stale,
foreign, swapped pairing, full-width u64, controls, disconnect/timeout/revoke,
OOM/trap and cleanup cases. Run actual Java/TeaVM DBG and ROS debug scenarios,
cache cold/store/warm/hit/refusal/CPU identity checks, VTune hardware timing,
hotspot and microarchitecture comparisons and required QEMU architectures.
None of these execution gates is waived or marked passed by Source review.
