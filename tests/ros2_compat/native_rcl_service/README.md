# Direct RCL service endpoints with same-take metadata

This is an independent **source-only** provider candidate. It has not been
imported, generated, configured, compiled, probed, tested, installed, run or sent
by SSH. It does not change the frozen CPP10/B/C/D/subscription packages or the
repository. Current Host/IPC service routing is still REQUIRED; this source is
not an installed route or permission and has no native/Guest execution credit.

## Why the native ownership differs

The actual selected RCL API `rcl_take_request_with_info` and
`rcl_take_response_with_info` returns the initialized generated C++ object and
`rmw_service_info_t` from the **same** native take. Its request ID, signed source
timestamp and signed received timestamp stay with that exact object's canonical
body. The older `rcl_take_request`/`rcl_take_response` wrappers explicitly discard
the timestamp fields and return only `rmw_request_id_t`. A callback carrying only
that older ID cannot satisfy this provider's metadata contract.

`typed_endpoint<Service,Client>` retains the existing real `rclcpp::Node` and its
original `get_shared_rcl_node_handle()`. It owns a separate zero-initialized
`rcl_client_t` or `rcl_service_t`, initializes it with the **actual generated C++
Service TypeSupport**, and never adds that direct entity to any rclcpp executor.
An existing executor therefore has no registration from which to steal these
requests/responses. Poll explicitly performs at most one native take. It does
not spin the Node or manufacture an upper callback/waitset.

The shared native Node prevents node finalization while these owners live. It
is not a fictional global Context lease: explicit Context shutdown still has
its real upstream semantics. The caller must order provider retirement before
its own Node/Context teardown and must not race explicit context destruction
against admitted operations. Every business entry also checks the actual
native node validity. Finalization uses the genuine RCL API, which permits its
original cleanup path after context shutdown when the node itself still exists.
A per-endpoint nonwaiting admission mutex returns17 for recursive/concurrent
operations on that one endpoint. It does not impose a whole-Context17 rule on
unrelated original rclcpp callbacks.

## Concrete real types and codec composition

The first two complete Service families are `std_srvs::srv::Empty` and
`example_interfaces::srv::AddTwoInts`. Their real Request/Response objects are
preconstructed and fixed-size. No hand-written generated header or C-object
reinterpretation is supplied. String remains the separate real topic codec and
D4 publisher/subscription path; there is no invented String service.

The provider consumes the independently held C++ Registry component: seven
business descriptors backed by all eleven selected native identities and the
full nine Message/two Service graphs, including Event/ServiceEventInfo/Time.
The selected actual generic TypeSupport pointer and the Registry's complete
modern introspection/TypeDescription/source/hash validator must match before
native initialization. Providing identity bytes is not installed-SDK
qualification. Root must independently admit the real complete native SDK export
and compile every B/C/adapter/provider/application from the same new descriptor
header. The trailing validation_state seam is source-recompile-only, not old
binary ABI compatibility.

For Empty, the actual original `.srv` has no fields and the fixed original
rosidl adapter inserts its native byte only for an otherwise empty struct.
Its complete native field/TypeDescription/hash is retained. Logical wire extent0
admits only the default placeholder0; a caller-written nonzero placeholder is
refused22 without being silently dropped. Native requests/responses of that
unrepresentable value fail; this is not a claim that native C++ cannot hold it.
AddTwoInts retains both signed64 request values and the signed64 response value,
with canonical16/8 bytes; no addition/overflow application policy is hidden here.

## Before effects, after effects and once-only cleanup

The caller creates and retains a `report` before calling create, reserves its
own endpoint registry slot, and supplies the exact current authenticated source
binding plus a fresh nonreusing native incarnation. The factory allocates its
shared owner/control block, capability copy, fixed history, every canonical
payload buffer, per-history native C++ Request/Response objects and report
storage **before** native init. Each send's native object and response-ID copy
remain owned until native fini/history retirement; a later request does not
overwrite a buffer the native middleware may still borrow. Only the receive
scratch is reused after complete canonical encoding and an independent fixed
native value copy into that request's retained history.
The report is single-use and survives failed creation; primary errors and the
actual fini result are separate fixed facts. Business output/receipt commits
only after initialization, exact actual resolved service name and all actual
QoS/GID checks. If initialization succeeds but any later check fails, RAII
attempts the matching native fini once before releasing that native owner.

Native init failures retain their original RCL return code. The original RCL
implementation performs its own internal rollback; some secondary internal
RMW destruction errors are printed by original RCL rather than returned.
The future owned runner must preserve its raw stderr; this provider does not
claim to recover a return value the native API does not expose. Ordinary
pre-effect allocation failures return12. Terminal VM/allocator traps and foreign
native code violating the C API exception contract require actual process drain;
there is no claim of cleanup immunity.

Every later send/take/response/fini addresses the same retained native handle.
Every native send is marked attempted before entering RCL. No timeout or
uncertain failure permits replay. A response ticket becomes attempted before
`rcl_send_response`, and a second response returns125. `retire` attempts native
fini at most once, records its actual returned status (or explicit nonreturning
exception fact), retires local delivery and preserves fixed diagnostic/history
storage. Two stream positions are reserved for a terminal endpoint error and
its later actual retirement; consuming the error cannot silently consume the
closure reserve. Destruction does not retry a failed native fini. A nonzero cleanup
result remains a real failed cleanup requiring Root's external process ownership.

## Five independent identities, bounded without reuse

The actual `rcl_send_request` returns only a **signed int64 sequence**. It does
not return a writer GUID. This code never reinterprets a client GID as that GUID.
Within the exclusively owned direct client/incarnation, the full returned signed
sequence selects a reserved row; the later same native take supplies and retains
the actual complete request ID. Server rows receive that complete ID directly
from their one native request take. The genuinely queried client GID remains a
separate native receipt fact. Zero GID/writer value bytes are not a missing-data
sentinel; API presence, extent and actual association are explicit.

Gateway tickets come from the existing process-lifetime nonreusing full uint64
Service allocator. They are never cast to a signed native sequence. URMI stream
sequences and peek-reservation nonces are separate monotonic domains. Neither a
numeric match nor a copied binding confers Host authority. A history row is
retained after completion/expiry/abandon; no old native sequence or full request
ID is admitted again. This first bounded profile supports up to256 history rows
per native incarnation and refuses90 before a later send/take when exhausted.
It does not evict tombstones or pretend an unbounded production history. The
caller must retire that actual endpoint and obtain a new native incarnation.

All canonical bodies are encoded into a separately preallocated scratch buffer
before an old accepted row can be changed. Even a locally abandoned request's
late response must pass the complete codec before discard. Duplicate/unknown
native responses or duplicate native requests are failures, not new requests or
replayed Futures. Bounded first/latest unexpected native-info facts are retained
in the report; they are diagnostic data, never accepted mapping evidence.

## Same-record queue and deadlines

This is an explicit Service-only queue, separate from Memory's topic-only queue.
Each accepted record owns one192-byte URMI header and its exact canonical body.
It preserves actual same-take signed timestamps, full writer bytes and signed
native sequence. Client records use service_response, server records
service_request, and real error/closure records carry only their actual control
flags/error; no synthetic timestamp/GUID is attached to a control record. Queue
commit independently compares the full stored request identity and signed times
against the header, rejecting even a well-formed header from another take.

`peek(owner,incarnation,expected_writer_extent,snapshot,capacity)` checks the
exact source association, complete native metadata, actual ABI extent and wire
capacity before allocating a snapshot/token. A failed95/90/12 does not consume
or alter the queued record. Successful reservations are positive and never
reused. `commit` and `cancel` require exact owner/incarnation/stream/token.
Retirement or request expiry invalidates an older reservation. The future Host
transport must make its Guest output publication and queue commit transactional
under its actual operation lease; a copied snapshot alone is not acceptance.

Pending deadlines use the actual Gateway process steady clock and explicit
service timeout-ms, not native timestamps or synthetic polling steps. This does
not grant a Guest Clock. Client abandonment/expiry does not claim native cancel;
late full responses remain drained/validated and discarded. Server replies
check the original pending deadline and separately granted responder timeout.
Checks before/after a native send cannot interrupt a blocking middleware call:
Root's real finite process/watchdog and native QoS limits remain required.
There is no synchronous wait_for_service, graph-derived availability, fake
waitset or forced empty call. Native same-client readiness is the explicit
single-query seam below; finite waiting and Host routing remain separate work.

## Independent readiness on the exact retained client

`client<Service>::available(source_binding, current_incarnation,
availability_binding, original_availability_capability, bool_output)` performs
one original `rcl_service_server_is_available` call with this endpoint's retained
`rcl_node_t` and **the same `client_`** later used by send/take/fini. The pinned
original RCL implementation passes these exact two native handles to
`rmw_service_server_is_available`. It does not list graph names, construct a
second client, spin, sleep, invoke a business service or infer availability from
request success. There is no availability method on a server beyond refusal95.

A `service.call` grant is insufficient. The Host must independently authenticate
and hold current operation leases on the original source and a distinct explicit
`service.available` owner. The source binding and native incarnation must match
this retained endpoint; the observation owner must have a different full source
handle in the same backend/instance/Runtime generation/session. The common real
`service_availability_pair` contract compares the configured source alias,
resource, native type/identity, optional custom-codec presence/value and all eight
original QoS fields. This helper also checks the finite availability envelope
contract, including its own64-byte request/response bounds, max-inflight1 and
separate history/client/wait/time limits. Those are **not** service body bounds.
Availability has no `validator` key; the `validator=none` business preflight is
kept separate. It never expands the original call/respond authority.

Before the native query, the provider reopens the same retained endpoint's actual
QoS/GID receipt and rejects drift. It independently compares both actual native
QoS sides against the availability declaration. A local bool is passed to the
real API; the caller's output is assigned only after native success and elapsed
`timeout-ms` checks. False is a genuine successful native result. Error12/5,
unsupported95, denied13, busy17, retired125 or elapsed110 leaves output unchanged.
As with every synchronous native operation, a postcheck cannot preempt an RMW
that blocks; Root's process deadline/drain is required. Native status/count stays
in the preowned diagnostic slot, and the independent availability compile/link
feature never relies on an SDK version name. A missing query symbol returns95
without disabling otherwise available direct service business APIs. The first
build profile still requires the real baseline RCL/rmw headers/POD types and
modern generated Service Event/TypeDescription closure; absence of those build
inputs is an explicit build refusal, not a fabricated compatible SDK.

The native component first exercises denied pair/stale-session/same-owner cases
with unchanged output and no native availability call. It then makes genuine
queries on the same retained client until ready within its separately owned
finite component loop, before the first request send. Capability/binding records
inside that standalone component are explicit **test inputs**, not Host grants.
A future component result would not prove authenticated Host dispatch. Existing
legacy `service_availability.h` creates separate extra clients; those clients
are deliberately not reused as evidence for this new endpoint.

## Actual native query and authority boundaries

Client activation/inspection calls the original request-publisher and
response-subscription actual-QoS getters. Server activation/inspection separately
calls request-subscription and response-publisher getters. RCL's selected
implementation obtains and caches these native values during init. Its original
source specifically copies `avoid_ros_namespace_conventions` from the real init
options because the lower QoS getter does not return that flag; this is an
original-RCL creation fact, not a separate middleware attestation of that flag.
The remaining actual QoS fields are not substituted with the declaration. D4's actual duration normalization retains
original RMW saturation semantics. Requested concrete QoS is checked against
both observed profiles. Inspection refuses changed observations rather than
updating a receipt or substituting a new endpoint.

The first explicit profile accepts KEEP_LAST depth1..256 and `validator=none`,
with exact supported policy spellings and explicitly parsed durations. Unknown
spellings are refused before init rather than taking the older helper's default
branch. History capacity must cover both admitted inflight count and native
depth. KEEP_ALL and additional value policies require a separately implemented
profile; they are not silently ignored or claimed as full upstream QoS support.

Actual client GID uses `rcl_client_get_rmw_handle` and `rmw_get_gid_for_client`;
its missing/unsupported API returns95 and triggers real rollback after creation
if runtime support is absent. This first compiled ABI profile requires actual
16-byte GID/request-writer storage. No truncation or artificial native ID is used.
Different actual native storage is a future explicit profile, not silently made
compatible. Service receipts do not invent a GID.

No profile or provider is installed by default. Each factory requires a real
selected native Registry and exact original type/native identity, resource,
operation and explicit bounds. `service.advertise` never implies
`service.respond`: respond additionally requires a distinct live source/alias
in the same instance/generation/session, exact service/type/QoS and a pending
committed ticket. The owning Host must authenticate and lease those actual
separate grants. It must also enforce original quotas/validators/current policy
and observer pairing; this helper does not turn capability_spec data into proof.
The new D wire route is not connected here. Root still must implement and review
Host/IPC dispatch, grant/schema registration for these service families, source
lease/copy atomicity, observer/queue pairing and terminal drain. Original81 Guest
imports and existing UWRE payloads are unchanged.

## Build recipe and distinct unexecuted evidence

The standalone CMake entry is this directory. Supply an actual reviewed composed
source root with D4 helpers and the separately peer-closed CPP Registry applied.
It links only the reusable cpp_codecs.cpp component into the Host native SDK;
it does **not** link the Guest rmw_uwvm implementation over the Host's real RMW.
Separate selected-SDK compile **and link** checks reference actual RCL send/take-with-info,
init/fini, QoS/GID and duration symbols, plus the independently gated direct
readiness symbol. Neither probe is run by CMake. Missing
features leave the corresponding provider operation95 and do not emit a fake native implementation.
The complete modern C++ Registry still requires its independently admitted
original generated graphs/TypeSupport libraries. Root must qualify every actual
source/header/MD/library and the real selected SDK; header presence is no result.

The fifteen authored record groups use real rmw POD definitions with explicit
MODEL values; they are not native metadata. The native component source uses
actual generated Empty/AddTwoInts, actual native RCL and real retained objects,
a finite same-client readiness/poll loop, complete two-sided metadata, independent
availability/responder bindings and exactly-one fini checks.
It has no substitute RCL/RMW implementation. Its published identity fixture is
only the previously recorded official buildfarm JSON data, not a fresh native
export. Even a future native component PASS would not prove Host transport,
Guest control/default-denial/native peers or whole original rclcpp acceptance.
No tests, compilers, native probes or processes have been run by this author.

## Explicit future execution recipe

`recipe.json` is DATA, not an executor or an authorization. The caller supplies
physical paths for an actual reviewed composed source root, an actual complete
admitted native SDK and fresh build/output directories. Source all real SDK
setup/environment only under Root's owned process policy; this package does not
pick a distribution by label, install dependencies or invoke a shell for you.

The example configure/build commands in the recipe are unexecuted. Root must
first validate their complete actual input roots, selected compiler/Python/CMake,
full source manifest, same-FD target identities, generated-header/library closure
and original link/MD evidence. Run serially in a finite owned process tree with
an isolated **actual** DDS domain/network envelope and preserved raw stdout,
stderr, exit and cleanup. Do not run this component against an uncontrolled
robot. Its native Node preserves original NodeOptions defaults; original native
rosout/parameter endpoints are not disabled or claimed permission-free.

A future local records PASS is only the fifteen model groups. A future native
component PASS is only two concrete native Service families with real same-take
metadata/owning endpoints. Neither upgrades source-only SDK admission, proves
current Guest Runtime/Host routing, validates all vendors/old ROS distros, or
completes all original client libraries. Required follow-ups include current
Host/IPC service and availability routing, independent current grant/observer
pairing, quotas and revocation, actual event copy/commit atomicity, controlled
native-peer runs, post-drop closure, all concrete distribution/compiler/header
and callback/wait integration, dynamic Service families, KEEP_ALL and additional
validators. No missing native fact is substituted with a caller boolean, graph
list, synthetic GUID/time, temporary endpoint, or old execution receipt.
