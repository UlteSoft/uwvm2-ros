# Bounded rcljava Services and deferred Futures

This is an opt-in source successor over the existing Java copied-buffer SDK and
rcljava topic/timer profile. Independent source review and actual compilation
with `javac --release 17` passed. Twenty-eight local service groups performed
seventy checks, and a separate local model executed the entire unchanged
original server main. The prior sixty-one policy and eleven original-source
checks also passed against this successor. All five serial compiler/JVM
commands retired; observed process-group RSS stayed below 134 MB.
No source review, JVM model, compiler exit or console marker substitutes for
actual target-Wasm, Host/Gateway, native identity or ROS delivery qualification.
Old topic/timer receipts retain their original source and artifact identities.

## Fixed original API and business sources

Primary original interfaces: [Client](https://raw.githubusercontent.com/ros2-java/ros2_java/d0e4e952bad1977ce9818d00b520d3ad0aeafd8d/rcljava/src/main/java/org/ros2/rcljava/client/Client.java), [RCLFuture](https://raw.githubusercontent.com/ros2-java/ros2_java/d0e4e952bad1977ce9818d00b520d3ad0aeafd8d/rcljava/src/main/java/org/ros2/rcljava/concurrent/RCLFuture.java), and [NodeImpl](https://raw.githubusercontent.com/ros2-java/ros2_java/d0e4e952bad1977ce9818d00b520d3ad0aeafd8d/rcljava/src/main/java/org/ros2/rcljava/node/NodeImpl.java).

The selected API is the actual `ros2-java/ros2_java` commit
`d0e4e952bad1977ce9818d00b520d3ad0aeafd8d` and its exact Git tree. Complete
Client/ClientImpl, Service/ServiceImpl, RMWRequestId, ServiceDefinition,
TriConsumer, RCLFuture, ClientTest, NodeImpl and Java generator templates are
preserved with original LICENSE/NOTICE and commit/tree evidence under
`upstream/`. Those JNI/native implementations are evidence, not Guest source
roots. Their static JNI initialization and native pointer methods are not copied
into the executable migration profile.

The existing exact original `ros2_java_examples` commit
`3c1fa01f95fb96110953c0f7f196424aa0557b97` supplies the unchanged
`AddTwoIntsService.main` and `AddTwoIntsClient.main`. The former is called by
`examples/OfficialServiceBootstrap.java`; no rewritten handler, replacement main
or class-only bootstrap bypass is used. The client has its real Client/Future/
value API; **its full main execution/qualification remains required**. In the
early service/Future increment all availability entry points refused95. The
current separately opt-in [availability source contract](../availability/README.md)
implements `isServiceAvailable` and both `waitForService` overloads with a
separate `AvailabilityBinding`, actual optional imports, independent query
owner and a separately granted steady Clock for positive/infinite waits. The
old `ServiceBinding` constructor keeps no query selection and still refuses95.
The business source is never edited to skip the wait. Graph listings, empty
calls, a model Boolean and submit admission are not native availability proof.
The optional Gateway owns an extra retained native query client matched against
the call configuration; it does not attest the business path's same retained
RMW client. Source implementation or older local models do not qualify the
current optional-import artifact, complete original main or native delivery.

The public Service/ServiceDefinition/TriConsumer/RMWRequestId API copies are
byte-exact. Client removes only its unused `RCLFuture` import; every method
signature is unchanged. The actual returned value is `java.util.concurrent.Future`
with a private completion path. Writable standalone `RCLFuture.set`, native
constructors and direct injected `handleResponse`/`executeCallback` are not
pretended as installed native dispatch. The latter two refuse95.

## Complete selected values and codecs

The profile implements all fields, original Java names, fluent setters and full
canonical request/response bytes for:

| Native type | Request | Response |
| --- | --- | --- |
| example_interfaces/srv/AddTwoInts | signed int64 a, b | signed int64 sum |
| std_srvs/srv/Trigger | empty | bool success, complete UTF-8 string message |
| std_srvs/srv/SetBool | bool data | bool success, complete UTF-8 string message |

The exact official `.srv` source and acquisition evidence are preserved. These
owning Guest values do not derive an MD5, RIHS, native converter, serializer
pointer or native type-support object. `getServiceTypeSupport` and the generated
native conversion/destructor methods throw. The RequestType/ResponseType class
roles preserve the real generator naming. Encoding uses the actual SDK's LE
integer, strict Boolean and strict UTF-8 codecs, with exact lengths and no
trailing bytes. NUL/BOM are preserved; an invalid/null string is refused before
submit/respond. No IDL field is renamed, dropped, or projected.

For custom AddTwoInts native binding, `catalogs/` and the byte-exact reviewed
modern native exporter/codec contract are copied from the existing complete
Service family. Root must use genuine installed SDK root and Request/Response
metadata, full protocol closure, exact native bind and fresh generation before
building the optional contract. All hash-capable provider failures refuse;
there is no implicit legacy downgrade or Python-computed native hash.
Trigger and SetBool already occupy Runtime builtin adapter slots: do not
register them as shadow custom adapters. Their actual installed native identity,
QoS, canonical builtin codecs and Host/Gateway contracts remain independently
required; the native peer uses real installed `std_srvs` headers.

## Explicit independent selection, empty authority

`Deployment.Profile` retains the original constructor with an empty service
table. An additive constructor accepts a cloned finite `ServiceBinding[]`.
Each immutable row binds exact node FQN, resolved resource, complete native type,
QoS token, client/server role, two distinct nonempty aliases, byte capacity and
pending bound. Server aliases are advertise/respond; client aliases are
service.call/event.observe. Every role, type, alias, bounds and registration
slot is checked/allocated before the first open. A local selection is not a
Host grant. Default authority remains `[]`; a missing binding returns13 before
opening anything.

The actual official NodeImpl uses `QoSProfile.SERVICES_DEFAULT` for implicit
service/client overloads. The new constant and servicesDefault method select
`official-default-service-profile`; the prior topic `official-default-profile`
is unchanged. These are explicit deployment tokens, not guessed native QoS
values. Root must prove their actual granted alias/native correspondence.
Wrong resource/type/QoS cannot silently choose another row or backend.

Use `OfficialServiceBootstrap` only with the exact advertise/respond aliases
qualified for `/minimal_service`, `/add_two_ints`, complete AddTwoInts type and
the independently established service default. The original handler's output
is ordinary stdout, not a hidden ROS logger grant. All entry paths use the same
whole-Context callback lease. A separately granted logger or clock is never
opened automatically.

## Actual asynchronous completion and finite progress

Client construction opens both real aliases and calls actual `event_watch` once
with the owned call handle. `asyncSendRequest` preallocates its Future/diagnostic
record and encodes the complete request before actual `service_submit`. Its
nonzero u64 ID is retained by bit pattern, monotonically nonreused per actual
source. No `service_call` fallback, native availability probe, timeout replay or
buffer-resize replay exists.

A later independently polled `event_poll_v1` envelope must pass complete magic,
version, backend, reserved field, flags, length, source, nonzero instance/session,
exact next u64 sequence and admitted correlation checks. Only a complete matching
SERVICE_RESPONSE sets its value; RESOURCE_ERROR/CLOSED preserve error/flags and
complete affected Futures. Malformed consumed events fail the source closed.
Cancellation locally terminalizes the Java Future and retains its inflight
tombstone until the real completion drains. Even a cancelled result is fully
decoded before discard. It does not cancel native work or interrupt a ROS
executor. Source disposal/revocation/shutdown terminalizes pending local Futures
with125 and a distinct local-retirement origin, not an invented native event.

The real original RCLFuture.get drives spinOnce. This profile preserves that
Guest-driven behavior only through the explicit finite deployment driver and
existing Node resource rotation: no thread, sleeping, implicit clock, native
waitset or synchronous request is created. The shared credit bound also holds
if a custom driver keeps returning true. The driver callback is inside the
Context lease before it can reenter lifecycle or progress code. No driver is95
before polling; exhaustion reports interruption with an admitted request still
pending and never resubmitted. Explicit close still preserves it as local125.

`get(timeout, unit)` does not reinterpret credits as nanoseconds. Ready values
need no wait; zero/nonpositive polls can time out immediately. Positive native
wait deadlines require a real explicitly granted Clock or reviewed bounded
Host-wait extension and currently refuse95 before a poll. A terminal Future
may be read inside its completion callback: both get overloads return the
already recorded value or failure without polling, credits or a native call.
The timed overload still rejects a null TimeUnit. Pending get and nested spin,
resource mutation and Context shutdown during a callback continue to refuse17.
Arbitrary native threading/RCLFuture/Task compatibility remains required
follow-up; this read-only fast path does not implement concurrent execution.

## Retention, rollback, cleanup and recoverability

The pair wrapper, WeakReference, actual endpoint registrations, two cleanup
records and fixed pending/callback slots exist before an effect. Each returned
Capability is assigned to a retained field immediately. Failed second open,
watch or advertise retires only this newly acquired pair in reverse order,
retaining the original primary and every real cleanup result/exception through
`lastFactoryCleanup().next`. Suppressed diagnostics are best effort; reserved
records remain authoritative if diagnostic allocation fails. Native close is
never retried merely because it returned110/107 or threw.

The complete Context retains every Node/resource strongly during callbacks.
Factories, close, alias revoke, Node destroy and shutdown refuse17 before any
lifecycle mutation. Callbacks may perform separately authorized business calls
on live resources; they cannot convert a request ID or Java object into native
authority. Successful/failed cleanup outcomes are preserved in fixed chained
attempts; an earlier failure or callback exception cannot skip another owner
or pending Future. Unchanged topic/timer registration, authorized clock reads,
phase skipping and existing cleanup remain in the same Context.

This is recoverability while Java still executes. An unrecoverable VM trap,
process death or exhausted language runtime still needs actual Host Instance
retirement and whole process-tree cleanup. It is not made immune to OOM or
terminal traps by preallocation. Raw imports/alternate Transports are outside
this local facade lease; only genuine current producer/Host receipts qualify
real effects.

## Source-local and actual acceptance recipe

Use copied source and fresh private output, an actual fixed JDK/TeaVM0.13.1
classpath and independent Root process/memory/disk/deadline ownership. Do not
execute downloaded code, install dependencies, use network or write the SDK
source tree during qualification. The actual copied-source run compiled
seventy-two selected classes, including twelve unchanged original application
classes, with the real TeaVM 0.13.1 interop jar. Its report is retained with the
continuation evidence. Target-Wasm and native service lanes below remain
required; compiling the original client does not execute its unavailable wait.

Compile only the actual `sdk/java/src/main/java` and
`compat/rcljava/src/main/java` package roots, the four `services/examples`
entries, the new ServicePolicyTest and the two explicitly selected original
business classes. Do not recursively compile `upstream` JNI implementations or
audits. Preserve existing old source-local tests as separate cases. The actual
TeaVM interop/classlib producer is required, not a fake annotation/import jar.
The existing POM explicitly includes the original server/client and the new
bootstrap entries; all native/upstream evidence stays outside executable roots.

Use the real linear-WASI producer and numeric ROS2 imports. Inspect the complete
module's actual import signatures and memory addresses against current ABI1;
GC object/reference addresses are never linear offsets. Each runtime case is a
fresh Guest. Native permission is selected only in the actual Host policy.

1. Run local models from copied sources, retaining genuine compiler stdout,
   stderr, exit, selected classes and method results; native ROS count stays0.
2. Build DefaultDeniedServiceBootstrap with a present local row and EMPTY Host
   grants. Require real ABI1 and actual first open13, no nonzero handle, no
   import/JNI/unsupported-type failure masquerading as default denial.
3. Compile/build the unmodified official server main via its independent
   bootstrap. Actual native `service_peer client add` uses I/M/P/F credits;
   submit only after its real native readiness and exact output policy/socket
   ownership admission. Require complete41,1/42 and actual no-replay closure.
4. For each of add/trigger/setbool, the new explicit-ready client lesson uses
   actual service_submit plus observer. A separately owned native server must
   already be genuinely ready. Require full ID/event scope, complete values,
   exactly one submit, both actual handles and terminal cleanup.
5. Repeat reverse-direction server lessons against native clients. Check all
   empty/Bool/String/i64 fields and native endpoint provenance. A server-created
   marker alone is not client/server match or delivery proof.
6. Keep the early original client main's availability95 result attached to its
   actual old binding/artifact. The separately opt-in availability source and
   exact producer hooks now exist, but require a new admitted artifact and real
   execution qualification before a whole original-client-main PASS is claimed.

The native CMake peer requires real installed `rclcpp`, `example_interfaces` and
`std_srvs` and modern Jazzy future declarations. Root owns full MD/compiler/link
trace/archive stats/native ELF/full7/maps, held original pidfds, namespace/domain
isolation, endpoint admission, resource limits and every terminal path. The peer
creates an endpoint at I; capture its genuine loaded type-support closure after
I and before M/P. P submits at most once and performs one bounded progress pass;
F requires complete data and actual close. An external independent wall timer
must also bound stdin/native waits and all descendants. No source recipe itself
is an execution or whole-lane PASS.

## Required remaining implementation

`coverage.json` in this directory is an implementation ledger, not permanent
exclusions. The separately opt-in service availability/wait and Java import
bridge are source-implemented; actual artifact/import/Runtime/native and whole
original client qualification remain required. Implementation work includes
physical timed Future waits with actual authorized Clock/Host deadlines; native
RMW header propagation if ever exposed by a reviewed protocol; standalone
writable RCLFuture/Task APIs; remaining service/generated-value/equality/native
reflection APIs and all Actions; additional versions/vendors/QoS graphs; all
remaining official tests and actual target/native lanes. Default authority stays
empty throughout those changes.


## Current native source selection and paired headers

The standalone Native codec contract requires an actual fresh native-bound
catalog AND all generated outputs from the selected current parent/bounded
source. A previous delivered header or a copied RIHS literal is not enough:
current Service admission also needs the complete emitted root/Event/Time
`custom_native_interface_schema` trait. Keep all historical outputs unchanged.
Follow `sdk/custom-types/native_current/recipes.json` in an admitted private tree.
Generation is an explicit implementation prerequisite, never run by CMake.

For the contract configure command supply both
`-DUWVM_BOUND_CATALOG=/absolute/fresh-bound/catalog.json` and
`-DUWVM_BOUND_GENERATED=/absolute/fresh-generated`, plus the actual source root
and selected Python interpreter. The real CMake helper checks source/product
pairing, selects `manifest.hpp` and `native_ros2.hpp` together, supplies actual
project include roots and concrete installed ROS imported targets, then links
an EXECUTABLE modern Service/Request/Response probe. An outer STATIC_LIBRARY
try-compile setting cannot replace that link. A probe link is not an executed
Native contract. The actual bounded SIGSTOP/I owner protocol remains required.

`codec_contract.cc` intentionally does not include `native_ros2.hpp` first.
The Gateway loads the common manifest before its selected provider; predefining
the type table is still a hard error. Every root/schema/component/value assertion
remains, with additional comparisons against the exact paired common Service
and Request/Response descriptors. Existing unsupported/modern refusal paths
are preserved; no header, Native identity, dependency target or success is faked.
All source selection/build/probe/native commands in this successor are unrun.
