# Retained direct-RCL service registry: source-only component

This implements a Native registry over the existing actual direct-RCL
`client<Service>` and `server<Service>`. It does not register a Runtime import,
route a Gateway request, grant any capability, run a compiler/probe/model, or
qualify a ROS distribution. All proposed files are new. Existing RCL8, CPP10,
ServiceWire7, the original81 ABI and Root Host/Gateway implementation are kept
as separate exact source inputs. This r2 adds one separately reviewed presence
refusal at the original RCL `selected()` entrance; every original nonexplicit
function body/token path is preserved.

## Explicit construction and authority

The caller supplies a real `rclcpp::Node::SharedPtr`, a real immutable shared
`uwvm_rmw_cpp::Registry const`, and finite `limits{maximum_entries,
history_per_endpoint}`. No Registry or eleven expected hashes are synthesized.
Actual original native identities must have been separately admitted and the
complete C++ Registry must already be warmed before a stopped native mapping
checkpoint. Its nonmoving implementation and actual native libraries outlive
all borrowing entities. All B/C/codec/application consumers must rebuild against
the actual current trailing `codec.h::validation_state` layout.

The default factory creates genuine private direct-RCL Empty or AddTwoInts
objects using their complete actual generated type-support and Registry graph.
Both actual request/response QoS and the full actual client GID come from the
same retained object. No rclcpp Client/Service competing executor consumer is
installed. Original NodeOptions/default endpoints are preserved.

The exact current spellings are `std_srvs/srv/Empty` and
`example_interfaces/srv/AddTwoInts`. Trigger, SetBool, ROS1 spellings or matching
resource names are not converted into those types. Root's separate actual
policy/schema admission is required; constructing a capability in the Native
fixture does not prove that Host/Gateway profiles accept it. The fixed CPP10
vtable proves Native graph and fixed canonical0/16/8-byte conversion, but has no
canonical-identity string member. Therefore the fixed factory refuses95 on
**every explicit `adapter-codec-identity`**, rather than ignoring/removing it or
pretending Native RIHS is that proof. The registry's early `create_fixed` path
returns95. The direct RCL `selected` predicate now returns false on that same
field's presence, so its original create/respond callers refuse13 before the
Native effect. Direct users cannot bypass the constraint by skipping this
registry. The Native witness checks both that real rejection and the unchanged
nonexplicit predicate over its actual Registry descriptor. Its final fast_io
status uses an explicit bounded four-byte `strvw`, never a raw character pointer. A real compiled canonical-proof/profile
successor is still required. It must preserve default denial and all original
schema/native/canonical identity checks. This limitation is not complete vendor
or standard-service support.

`factory` is an explicit trusted compile-time extension point. Supplying a new
function pointer is not itself support: this finite registry's bounded-storage
preflight also recognizes only the two complete original families. A future
family must extend that actual preflight and provide a real typed endpoint,
complete Native graph/canonical proof and bounded allocation before any effect.
There is no empty provider, fake expected hash, legacy route fallback or generic
Native type cast.

For each `execute`, `admitted` contains the actual primary/paired capabilities
and full paired owner. The Host must retain both real current leases through the
entire Native call/packet transaction. A client pairs `service.call` with its
own `event.observe`; the observer has `builtin/event-v1` envelope identity,
not the service Native type/QoS. It must explicitly include service-response;
resource-error/closed are optional explicit bits, never silently granted. A
server pairs `service.advertise` with independent `service.respond` and full
Native type/identity/all8QoS/canonical constraint equality. The shared
`rmw_service_bindings` helper is structural only. It confers no rights, takes no
quota, proves no Native identity and replaces no live handle/policy validation.

`admitted` for availability additionally contains its own independently granted
`service.available` capability and full owner. It still includes the original
primary+observer pair. UMRC.peer names availability only for that operation;
all other operations name the saved observer/responder. `available` queries this
same retained direct-RCL client; it creates no probe client and sends no request.
wait_ns0 is one actual observation. Positive waits return95: no graph-event finite
wait/probe has been implemented here. Sleep/poll counts, graph names or a fresh
client are not substituted. The Native witness's externally bounded actual-take
driver is not a positive availability API implementation.

Gateway routing must use primary.alias for control/submit/peek/ACK/abandon,
real responder.alias for respond, and real availability.alias for a query's
separate quota. It must recover the original saved primary/pair from the trusted
configuration/history, never derive authority from UMRC numbers. Every Host
current-source/caller/independent quota admission remains outside this header.

## Lifetime, allocation and transactions

The shared pair history never evicts/reuses failed or retired source keys. The
registry also retains exact original capability snapshots, original paired full
owner, actual Native receipt and independent Native report. It reserves complete
reply storage, pair/map rows, capability copies, report and type-erased owner
before Native creation. The actual RCL endpoint preallocates complete generated
Request/Response objects, record history and body storage before native init.
A process-lifetime synchronized nonreusing allocator mints Native incarnations;
source/session associations and Native signed request IDs remain distinct.

Local maximum_entries is explicit1..65536; per-endpoint immutable history is
explicit1..256. Native history count must cover actual configured inflight and
QoS depth and leave a separate observer/control slot in the original queue
bound. Original UWRE policy byte accounting remains64+actual canonical body;
finite native URMI/generated-object/tombstone overhead is independent physical
memory that Root must admit. A larger declared max-request/response does not
turn a fixed Empty/Add codec into a larger Native message. Whole-registry
physical memory is not magically bounded by a per-message byte limit.

Peek first checks the current owned queue. Only an idle11 triggers at most one
real direct-RCL take and then another queue inspection. This consumes middleware
only into its preowned actual same-take URMI/body record. Full output capacity is
allocated before that take. A snapshot records its positive original stream
sequence and independent positive reservation immediately. UMRC carries its own
correlation/ticket; all same-record owner/incarnation/metadata/kind/extent checks
precede packet exposure. Complete service metadata retains actual zero bytes,
signed native sequence and timestamps; original writer GUID is never clientGID.
RCL may zero-initialize timestamp fields: actual selected RMW completeness is
still a future qualification requirement, not inferred from structure members.

Only exact sequence+reservation ACK consumes/releases a borrow. Cancel/repeek
preserves the record sequence and gets a new token; old commit/cancel attempts
are rejected. Full actual server request commit is required before response.
Native responses and request submissions are one attempt. `client_abandon` is
local tombstone/late-response disposal, not Native cancellation. Unknown effects
are never replayed or called rolled back.

Packet formation/deadline/transport failure after a successful borrow calls
`delivery_failed`, which explicitly cancels that retained token. Failed cancel
causes local tombstone and once Native retirement. Primary, cancel, commit and
retire results are separate persistent slots; `last_commit_completed` never
turns a late ACK into rollback. Actual native init/send/take/respond/query/fini
return codes and calls remain separately in the real RCL report. Registry method
attempt flags refer to entry into the corresponding endpoint method; only that
Native report attests a middleware call actually occurred. No diagnostic success
is inferred from an unattempted Native cleanup.

`close_owner(domain,handle,alias,revoke,reason)` uses the real saved old session
and matches either pair member. It tombstones all matches before cleanup.
`retire_session` and `clear` do likewise. They require no fresh credit/heap
allocation or successful IPC and never reconnect. The registry drops each owned
endpoint after one actual native fini attempt; the report survives. Final native
fini failure remains a failure requiring external owned process/node/session
retirement. The registry mutex serializes calls, but does not make a blocking
foreign middleware call interruptible. The external owner must enforce a real
whole deadline and physical retirement. Clock checks here only detect late
returns, and never claim hard return-time preemption.

## Production feature probes and future actual recipes

`src/ros/gateway/ros2/compat/NativeRclServiceProbe.cmake` contains actual selected SDK
compile-and-link source checks and a production generated-header template.
It checks genuine direct-RCL init/fini/send/take-with-info/name/actual-QoS/client
GID APIs, full16-byte actual GUID extents, signed64 metadata and real linked
rmw_time_equal/normalize. Independent actual same-client availability is checked
separately. Every retained function address is volatile and referenced by main,
so optimized/section-garbage-collected probes cannot discard unused symbols.
No source probe runs a Native program. Old/header-only/distribution-name support
is not presumed. Unconfigured or unsupported paths return95. Current production
Gateway CMake must explicitly call this new probe and include its actual output;
the existing test-only feature header does not qualify production.

All real configure/compile/link must later run in Root's isolated bounded lane,
not during source review. With actual coherent reviewed source and SDK prefixes:

```sh
cmake -S test/0073.ros-tests/support/source/ros2_compat/native_rcl_service_registry \
  -B /tmp/uwvm-native-service-registry-build \
  -DUWVM_BUILD_NATIVE_SERVICE_REGISTRY_COMPONENT=ON \
  -DUWVM_SERVICE_REGISTRY_ROOT=/actual/reviewed/coherent/source
cmake --build /tmp/uwvm-native-service-registry-build --parallel 1
```

No `try_run`, CTest registration, automatic Native startup, Guest RMW replacement
or fake generated types are included. CMake explicitly requires actual original
SDK/TypeSupport/generator targets, exact new source and real vendored fast_io
include paths. The actual CPP10 source is linked; no `published_cpp_identity`
fixture is a production default.

`value_contract.cc` authors eight always-active production helper/ledger cases.
These are logical models only, with no fake endpoint or middleware symbol.
`native_contract.cc` authors real Empty/AddTwoInts routes through this registry,
actual generated full-value codecs, same RequestID/writer/time observations,
reservation ABA negatives and actual paired-owner once-fini observations.
Its fixture capabilities/handles are explicitly DATA, not Host authority proof.
It reads an independently pinned actual11-identity export (ordered lowercase
hex64 rows named string,time,event_info,empty_request,empty_response,empty_event,
add_request,add_response,add_event,empty_service,add_service; final LF each row).
The caller must prove those values against the actual selected original SDK.
The file is read through the actual vendored fast_io `native_file` and bounded
`operations::read_some` into 2049 local bytes, rejecting empty or >2048-byte
input. Short reads continue until EOF or the extra byte; parsing borrows only
that local array for its complete lifetime. The G gate reads exactly one byte
through `operations::read_some(posix_stdin(),...)`, with no whitespace/token
conversion, iostream buffering or additional native authority.
It warms the real Registry before printing READY, then requires stdin G before
creating a real default Node/native endpoints. Root owns that gate and all
Native mapping/domain/isolation/cleanup. The Native report is printed even on a
failed route after terminal cleanup. Actual feature0 returns95, never a PASS.

No source case has run. Host raw12 registration, Gateway routing, real shared
admission and complete production feature/build/full-MD
closure, actual SDK/RMW identities and timestamp support, correct default denial,
all source/session/alias races, Guest/provider correlation, complete native ROS
interoperability and hardware performance measurements remain required.
