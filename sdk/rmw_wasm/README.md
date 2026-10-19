# Actual upstream RMW capability bridge: lifecycle/publisher cohort B

This source-only cohort implements real RMW symbols against the original public
rmw/rcutils/rosidl headers. It does **not** install replacement `rcl/`, `rmw/`,
`std_msgs/` or `rosidl/` headers and must never see the finite
`sdk/compat/rclc/include` facade on its include path. It is not a claim that full
rcl, rclc, rclcpp or the official examples have compiled or run. No candidate was
imported, generated, compiled, tested, executed or sent by SSH here.

The composed source tree now also contains the independently reviewed
[concrete five-import publisher transport](publisher_transport/include/uwvm_rmw/publisher_transport.h)
and [original C++ generated codecs](cpp/README.md). Both are source integrations,
with zero new build or execution results. The matching publisher Host/Gateway
route and [complete standalone original-RMW publisher example](examples/publisher/README.md)
are also independently reviewed source integrations. Actual original-SDK/Wasm/native
qualification remains required. The complete selected subscription source chain
now includes [real C subscription delivery](subscription_transport/README.md),
Host/raw7 and authenticated Gateway routing. [Real C service transport and full
examples](service_transport/README.md), paired service history, actual policy and
the retained direct-RCL registry are also source-integrated. Service Host/raw12
and authenticated Gateway routing are now integrated after independent full and
narrow corrections. [Original-generated-C codecs and complete C examples](c_codecs/README.md)
and [same-client independent availability](client_availability/README.md) are
integrated. Query authority is an explicitly selected third owner; absent alias
or provider cannot create another native client or grant readiness. Genuine
[Same-retained-client positive graph waits](../../tests/ros2_compat/native_graph_availability/README.md)
and the [actual finite WASIP1 wait provider](wasi_wait/README.md) are now also
source-integrated. The [scalar-default successor](../custom-types/scalar-defaults/README.md)
adds complete original declarations/initializers and modern Service/Action graph
proof. Full custom service factories/Gateway composition and genuine threaded
original-rclcpp support now have separate source integrations. The
[complete original default-control source](default_control/README.md) adds real
mixed C/C++ Guest codecs, the unconditional TimeSource ParameterEvent
subscription and independently typed Native control factories. Its Host/build
selection is explicit and its exact integration state is in `required-work.json`.
Actual cross-package builds, full original APIs, unchanged default-Node delivery,
physical retirement and official suites remain required. No source integration
is a Native/upstream suite pass.
The codec
descriptor's trailing read-only validation state extends its source layout;
recompile every RMW/provider/codec consumer together. This is not binary ABI
compatibility with an object built against the earlier descriptor layout.

## Source-level Guest migration

C/C++ compatibility rebuilds the original rcl/rclc/rclcpp application to Wasm;
it does not expose the host ROS binary ABI. Python runs its compatibility layer
inside the actual CPython.wasm bridge. Java/JAR uses a real bytecode/IR-to-Wasm
producer with explicit classpath and actual ROS import/linear-buffer lowering.
Every application path ends at the same uwvm2 ROS permission boundary, with no
default ROS rights or Guest update/debug/policy-management imports. Native ROS
libraries at the Gateway and original Native peers provide controlled network
transport and independent behavioral evidence, not Guest native ABI access.
Finite migration headers and the original-upstream static RMW build remain
separate source compatibility scopes; complete API/test coverage is required.

## Real implementation and explicit composition

The implementation defines actual init/options/context, node, publisher,
canonical publication through a required native provider, actual queried QoS and provider-GID functions. It uses
original `rmw_get_zero_initialized_*`, validation, allocator, security/discovery
copy/fini and error utilities from the real rmw/rcutils libraries. B does not
replace those utilities. It has no blanket fake-success functions for unbuilt
APIs. Every remaining original declaration is a concrete implementation/build
obligation in `required-work.json`; linking the full stack requires cohorts A/C.

Cohort A owns `platform/` and `cmake/`, including the actual `rmw_uwvm` static
WASI build entry and platform contracts. Use the original
`RMW_IMPLEMENTATION_DISABLE_RUNTIME_SELECTION=ON` branch and exactly one real
generated typesupport implementation. The official typesupport template then
statically forwards to that actual provider. There is no fake dynamic loader,
native pointer, generated schema, RIHS or header. A must preserve actual upstream
API/default behavior and make all platform clock/log/environment choices explicit.
B itself does not read a clock/environment, log through ROS, open a file, sleep,
start a thread or acquire a graph projection in rcl initialization.

Cohort C owns real local guard/wait/subscription/service/metadata sources. The
version1 revision7 headers in `include/uwvm_rmw` are the common contract. A missing
genuine graph provider makes node creation fail95 before resource effects. C's
provider must allocate/track a real trigger state and implement its wait/close
semantics. A Guest-local guard does not grant native graph/discovery access.
Native graph projections still require separate exact aliases.

Do not quietly change upstream NodeOptions to obtain a passing demonstration.
Original rcl/rclcpp defaults may construct rosout, parameter/event/type-description
endpoints and time infrastructure in addition to an application's publisher.
Every endpoint that actual original code constructs needs its full real codec,
actual native identity/QoS and independent explicit alias. Missing grants fail
through genuine RCL/RMW errors. Quiet options are legitimate only where the
application itself explicitly selects those actual upstream options. The first
String witness is not acceptance of an unchanged default rclcpp Node.

## Default-denied resource and type selection

An external explicit bootstrap calls `uwvm_rmw_install_profile` before creating
any options or context. The default profile is NULL and refuses setup with a
clear raw13 diagnostic. Installation performs no capability open. The entire
profile, endpoint names, alias strings, codec records and provider state must
remain immutable/alive through all borrowed options/contexts. No profile swap is
allowed while either remains live. Caller corruption/copying of these C lifetime
objects is not supported authority transfer.

Each endpoint row matches the exact selected node name/namespace, resolved full
name, original native type, endpoint role and **every** requested QoS field.
Aliases are bounded UTF8, nonempty and distinct across the complete selection.
Requested QoS and the independently selected expected effective QoS are separate
complete records. The expected record is only an assertion checked against an
actual same-incarnation provider reply. It is never returned as actual native
QoS. Every getter calls the real provider's inspect callback and checks the full
source/instance/runtime-generation/session/native-incarnation association plus
concrete queried QoS and the unchanged actual native GID before changing the caller's output. Host policy remains independently
empty by default, and the real raw open can still return13 despite a matching
Guest row. Source labels cannot grant a native endpoint.

Duration fields use the actual linked original `rmw_time_equal` from the original
`rmw/src/time.c` library for deadline, lifespan and liveliness lease. Original
`rmw_time_t` deliberately accepts nonnormalized nanoseconds: `{0,1000000000}` and
`{1,0}` are equivalent. Its public utility also owns saturation/infinite-duration
semantics. B does not reimplement or normalize those values and no longer rejects
an otherwise valid profile merely because `nsec >= 1000000000`. Unknown enums,
SYSTEM_DEFAULT/concrete policy distinctions and every nonduration field still
receive the same strict checks. This follows the original public comparison
contract; it neither relaxes Host grants/transport limits nor substitutes the
expected row for actual native QoS. Actual getters preserve the provider's full
queried representation even when its duration spelling differs equivalently.

This middleware performs no DDS discovery. Its own default init discovery option
is explicitly OFF and it rejects static peers, foreign domain choices and
unsupported security configuration. It never treats enclave/domain/network text
as endpoint permission. This does not patch any upstream NodeOptions behavior.
Actual upstream environment, logger, parser and clock code remains A's explicit
platform/build responsibility; an upstream call cannot silently obtain those
rights merely because `rcl_init` is being reused.

## Extensible real-generated canonical codec registry

The registry accepts source-selected conversion vtables over actual generated
objects/typesupport accessors. A MESSAGE entry must validate its complete original
native graph and actual selected full RIHS01, provide complete canonical encode
and transactional native decode, and choose its independent canonical maximum.
SERVICE roots refer to exact registered MESSAGE Request/Response entries under
the original native names; they do not guess a direction from `void *`. The root
validator still owes the complete native service/protocol/event graph and real
root/component identities. Payload-only hashes are insufficient. Service clients
will encode Request/decode Response and servers the reverse in cohort C.

String is the first implemented entry, using actual generated
`std_msgs__msg__String`, real C introspection, original runtime string functions
and type hash/description/source callbacks. The validator checks the actual
namespace/name, struct size, complete one-field layout, original unbounded native
string capacity semantics, offset, key/default/array metadata and full selected
hash. It creates no fake native metadata. `maximum_wire_bytes` is a separate
canonical wire bound. Existing bounded IR/generated native adapters can produce
additional entries of this same contract; they are required implementations,
not silently accepted by a namespace/type-string match.

Publication validates the real initialized String invariant, UTF8, full u32-length
canonical representation and bound before the provider publishes through the
SAME native incarnation that was activated at creation. There is no fallback to
the old lazy `roswasmc_publish` path.
Decode validates the whole canonical input first and uses the real
`rosidl_runtime_c__String__assignn`; failed allocation preserves its prior owned
storage. Native object bytes, CDR and canonical capability bytes remain distinct.
No RIHS is computed or guessed in this bridge. The selected native identity must
come from genuine Root-qualified exporter/native evidence before a real route.

## Required same-object native publisher provider

A raw capability open succeeds before the historical Gateway lazily creates a
publisher. It therefore proves neither native endpoint creation nor actual QoS.
The original cohort requires all four explicit `profile.publisher` callbacks
(activate, inspect, publish, retire) before any open. The separate production
[publisher transport](publisher_transport/include/uwvm_rmw/publisher_transport.h) and Host/Gateway source
route now implement those callbacks and same-object receipts. Absent callbacks
still fail95. Actual selected-SDK/Guest/Native qualification of that complete
route remains required. An arbitrary function pointer or a nonzero binding is
not evidence that the real provider has been qualified.

Activation receives the actual source capability, selected endpoint, complete
codec/native identity and original publisher options. It must genuinely create
one session-owned native object, query that object's effective QoS and return an
associated receipt. The receipt retains backend2, the full source handle,
instance, runtime generation, session and never-reused native incarnation, plus
the actual complete domain-global native publisher GID. B
checks all association fields/nonzero extents and the actual concrete QoS against
the separate expected profile. Source names, expected QoS or a temporary probe
cannot manufacture that reply. The provider must enforce current actual native
type/codec/rights and same-source ownership at each operation.

All later publication and destruction use that exact receipt/provider. The
getter inspects that same incarnation and never substitutes the expected row.
No temporary native publisher can attest the old lazy SDK publisher; no dummy
publication forces activation. Binding/QoS failures preserve caller outputs and
trigger entered native/raw/local cleanup. A complete associated receipt received
even on an activation error still creates one native-retire obligation; an
unknown/malformed binding is not trusted to target a foreign endpoint. Its exact
source capability is still closed and its actual Host instance must drain any
unknown effects. Provider callbacks must preserve all entered state in their
ordinary error result and must not keep borrowed Guest pointers.

Native and raw retirement are independent preallocated obligations. Both callback
slots and all result slots exist before the first open. A genuine associated
receipt enables one native-retire callback before source close on rollback,
destroy or shutdown. Any native-retire error remains in its own slot while all
raw closes and local releases proceed once. Public objects stay allocated after
shutdown for normal original upstream destruction; no later destroy retries the
native close. The actual optional Host/Gateway wire/API source is a separate
Root-owned cohort, not a fabricated dependency pin in this package.

Generic MESSAGE registry entries may choose logical canonical maximum0, needed
by actual Empty and empty Service requests. Physical publisher scratch reserves
at least one private byte while passing the original logical capacity0 to the
codec/provider. No dummy payload byte is introduced. Each complete codec must
still validate its actual native type and graph. String's own concrete validator
requires exactly std_msgs/msg/String, MESSAGE kind and a minimum canonical extent4;
allowing Empty never weakens that check or any original native identity match.

## Allocator, lifetime, dispatch and cleanup

The caller's complete allocator callback/state shape is validated. The actual
RMW init-options copy is deep, using original enclave/security/discovery helpers.
Its source/destination remain distinct and failures leave the destination zero.
Context construction allocates its complete bounded ownership arena before any
capability can open: up to8 live contexts,32 lifetime node slots and256 lifetime
resource/owner slots per context. These are finite implementation limits, not
new upstream ABI struct definitions. Caller allocators and selection state must
outlive their actual objects. Admitted context/node/owner storage never moves.

A publisher allocates its object and maximum codec scratch, then reserves its
stable group release record **before** a raw open. The Session retains each real
owner in fixed storage. No post-open list append/allocation is needed to remember
that obligation. Any setup failure rolls back immediately once and keeps the
original raw primary independently from every owner cleanup and local release
code. No native timeout, failure107/110, close, publication or admitted request is
automatically replayed. The counters/records remain available through
`uwvm_rmw_get_diagnostics` until context finalization.

A shared context scope protects each actual RMW setup/take/wait/publish/cleanup
function while its data is borrowed. New factories, node/owner destruction and
shutdown reject busy17 while that scope is active. Publishing through an already
retained publisher may borrow an existing RMW function scope, with a separate
per-publisher scratch/receipt recursion guard. This is NOT a lease around an
arbitrary original upper-rclcpp callback after rmw_take/rmw_wait has returned.
Original upstream callback/shared-owner lifetime behavior remains intact; the
strict whole-Context17 finite facades are separate opt-in products.
Codec/provider callbacks must not invoke application code, start asynchronous
work or throw across the C boundary. C++ callback wrappers must catch exceptions
before returning. The original cohort is a single-threaded WASI baseline.
The separately selected [threaded RMW](threaded/README.md) and
[original platform](threaded_platform/README.md) now supply opt-in source
implementations; they default disabled and do not grant a thread or ROS role.
Actual concurrent RMW, original multithreaded executors, races and physical join
still require their own complete target/native qualification.

`rmw_shutdown` invalidates business operations, attempts every entered native
incarnation retire before raw source close, then attempts every retained raw
owner in reverse order. Every native-retire and raw-close result has independent
fixed diagnostics. A failure never skips another obligation or causes a retry. It **does not free public RMW
entity objects**. Real upstream code often destroys its publishers/nodes after
shutdown; those later destroy calls release local objects without a second raw
close. Node destruction refuses while any actual endpoint object remains. Context
finalization requires shutdown, every node retired and no C-owned guard/wait
object retaining that actual Context, then frees the complete arena/options.
`uwvm_rmw_context_child_retain/release` requires an already admitted leased
Context; C reserves the actual reference before exposing an object and releases
it only after removing/retiring the real registry object. Retain refuses shutdown;
release remains possible during shutdown. A live child prevents a dangling
Context from being freed. No upper callback is given a fictitious permanent lease. The graph provider's own detailed error evidence remains separate;
B records each actual graph-destroy RMW status, each owner-close raw ABI status
and each group-local release status in their explicitly named slots.

C allocator/VM terminal traps, longjmp, corrupted caller memory and unknown native
completion are not made safe by normal C cleanup. Genuine Runtime instance drain
and an external process/deadline owner remain mandatory. Source models do not
claim immunity to terminal failure or native cancellation of admitted work.

## Actual domain-global identity and native metadata

The original RMW public contract requires publisher/client GIDs to be globally
unique within a ROS domain. Process-local counters do not meet that requirement.
The first B draft made that incorrect global implication; its exact source and
review remain preserved separately. This successor requires the same native
publisher provider to return the actual native global GID at activation and
inspect. The complete byte extent must equal the actual original
RMW_GID_STORAGE_SIZE (16 for the pinned source). Presence comes from the
complete associated actual provider reply and exact extent; an actually written
all-zero GID is not rejected as a universal missing-value sentinel.
No truncation, synthetic prefix, process-local fallback or invented GUID is used.
`rmw_get_gid_for_publisher` genuinely inspects the same retained incarnation/GID
before copying the actual provider data; errors leave the caller output intact.
The local model's fixed GID values are explicitly test fixtures, not evidence of
global uniqueness, native identity or an installed provider. Context local counters
remain private lifetime/correlation bookkeeping only, never a public native GID.

Original `rmw_message_info_t` also defines UINT64_MAX for unsupported publication
or reception sequence numbers. That does not authorize relabeling opaque Runtime
tickets as native request headers or inventing timestamps. Cohort C must give
every field a truthful local/native provenance and implement a protocol extension
where the existing ABI cannot satisfy the original contract. In particular real
wait/guard/timer deadlines use actual authorized time and bounded Host waiting;
stdio/poll counts never stand for nanoseconds, and native availability cannot be
inferred from graph listings.

## Meaningful contracts and actual acceptance obligations

`tests/lifecycle_contract.c` contains22 authored local function groups using real
installed/generated std_msgs objects, introspection and rmw/rcutils utilities.
The raw capability, same-incarnation native publisher provider and local guard
are EXPLICIT LOCAL MODELS, not a real DDS/Host/Gateway transport. The provider
model has its own retained endpoint state and QoS; the legacy backend deliberately
has no publish callback so this regression cannot accidentally exercise fallback. They exercise default denial,
mandatory guard refusal, exact topic/type/QoS before-effects rejection, full
canonical String values, invalid native object rejection, denial with no owner
leak, actual provider-GID copies, requested/effective QoS separation, every cleanup107,
explicit RMW function scopes, local graph state/lifetime and deep options copies.
The seven new groups cover missing provider95-before-open, String name/kind/min
association, malformed activation/queried QoS and rollback, inspect binding/output
preservation, native-retire107 plus raw-close110 without retry, actual generated
Empty with logical0 canonical bytes/private allocation1, and retained C child
Context lifetime. The malformed activation group includes three parameter paths;
the22 group count is not an official/native test count. Three later regression
groups cover original finite/nonnormalized/infinite comparisons and exact
nonduration checks, equivalent requested-QoS selection, and activation/inspection
with equivalent effective durations while preserving actual reply fields. All19
prior function bodies are unchanged. No test
was compiled or executed here. A model hash read from real generated typesupport
is still not a qualified native counterpart/exporter/transport result.

After A/C source closure, Root must build the original dependency graph with a
real fixed WASI SDK, complete MD/import/ELF/Wasm/memory checks and bounded owned
processes. Native installed-SDK codec/exporter counterparts, default-empty grants,
all actual default endpoints, explicit alias routes, full real original business
source paths, native metadata, real clock/wait and teardown must then be tested.
Old finite-profile results stay attached to those old artifacts. This B package
adds no execution credit, business ABI import, debugger, update or policy grant.
