# Two-capability service association and permanent history

This is a source-only shared value contract for the trusted Host and Native
Registry. It grants nothing and executes no RPC or Native operation. No candidate,
import, generator, compiler, linker, probe, model, test, native ROS, SSH or repository
write was performed while authoring. These18 value groups are authored, unrun.
Root alone can integrate exact reviewed bytes and qualify an actual future build.

The common header is `src/uwvm2/imported/ros/common/rmw_service_bindings.h`.
`service_endpoint_pair(primary, auxiliary, role)` checks values from TWO original
already authorized capabilities. The role is explicitly client or service; all
other kinds refuse22. Aliases are nonempty valid distinct tokens. Their resource
is identical. This is not proof that either handle is live or that a packet is
an authorization token. Every caller still holds both real source leases and
validates actual instance/generation/session, revoke state, native type/identity,
codec, effective QoS, all source/observer/output limits and actual operation quotas.
A field equality never creates rights or native facts.

For a client, primary is `service.call` and auxiliary is **`event.observe`**.
`event.poll` is a business action, not a capability operation. The observer keeps
its own exact `builtin/event-v1` identity and original `event-source-alias`.
It is not required to carry the service's Native type or QoS. The explicit kinds
may contain only `service-response`, `resource-error`, `resource-closed` in the
existing strict ordered, duplicate-free grammar. Response is mandatory; error and
closed are optional. Nothing is automatically appended. Native/Host control
output still needs its actual bit to be granted. An exact original response-only
observer remains valid. A complete RMW example can explicitly request all3 bits.
`service_observer_kinds` exposes that same exact mask without allocating.

For a server, primary is `service.advertise`, auxiliary is `service.respond`.
The original resource, Native type and Native identity are identical, as are all8
original QoS fields. Optional `adapter-codec-identity` must be absent on both or
present, nonempty and identical on both. This ONLY associates the two declarations;
the actual selected converter must separately prove that precise canonical codec
identity. Comparing two unproved equal strings is not validation. In particular,
RCL8's fixed C++ Registry has no canonical wire-ID field: see the separate source
finding retained in this package; the original closed RCL8 body is not modified.

## Stable source API and lifecycle

`rmw_service_bindings` owns a finite `std::map<full_primary_handle, entry>`.
Its capacity is immutable and1..65536. Invalid capacity makes reserve22. Each entry
contains the complete original binding, client/service kind, distinct full peer
handle, both copied original aliases and a first-reason retirement tombstone.
The first successful reserve also fixes this object's actual instance/generation.
Different native sessions can coexist only for that same actual instance/generation;
the full stored session remains part of every lookup and cleanup match.

Reserve order is `(owner, kind, peer, primary, auxiliary, output)`. Validate both
real capabilities/domain first, reserve this entry and all Native row/buffers
before any native effect, then perform actual creation. Failed activation calls
`retire` without eviction. Alias copies and map allocation occur before reserve0;
exception-enabled allocation failure returns12 with unchanged output/history.
Without usable language exceptions, terminal allocator traps still require the
external owned process/session drain; no OOM immunity is claimed.

The same primary full handle can reserve ONLY ONCE for this object's lifetime.
An exact duplicate is17 while nonretired,125 when retired/sealed. A conflicting
owner/domain/session/role/peer/alias is13. Unknown lookup is9. There is no erase,
reset, counter wrap, expiration eviction, automatic reconnect or replay. Reaching
capacity11 includes every failed/retired row, not just live Native objects.
`lookup(owner,kind,out)` returns exact retired/sealed rows too. `find_live` refuses
those125; it proves only local nonretirement, never actual Native activation.
Outputs commit only on0. `retire` and `seal` preserve the first retirement reason,
including107 for a broken old session, rather than invent cleanup success.

`matches_peer(row, closing_domain, handle, alias, revoke)` checks the same actual
backend2/instance/generation/**saved session**. Closing either actual member full
handle with its stored alias matches. Alias revocation matches either stored alias
without requiring a still-live handle. `closing_domain.source` is intentionally
not used; handle is explicit. Derive the domain's actual instance/generation from
the truly authorized closing cap, and its session from each saved row. Never use
`connection.session_id()==0` after disconnect to skip previously owned rows.
This does not authorize an old Native call: real cleanup separately verifies the
old retained connection/object, and never reconnects or revives an old source.
Retired rows still match for independently pending cleanup. `retire_matching`
marks every newly matching row BEFORE any cleanup attempt; its count is only
new local tombstones. Iterate stable `rows()` to execute/record each once-only
Native retire and each raw close independently. The header performs none itself.
The caller serializes access; this does not add locks, threads or concurrent RMW.

## Authored source contracts and required actual verification

The18 groups use pure clearly synthetic capability VALUES, genuine current wire
binding/event grammar and a standard library. They implement no Native/Host model
provider or fake TypeSupport/hash. Groups cover response-only/full optional masks,
wrong/duplicate/absent kinds, event.observe versus event.poll, independent observer
identity and no invented native QoS, exact alias/resource, all8 server QoS fields,
optional codec presence, full64 handle/owner/peer mismatches, persistent retirement,
both sides' close, handle-free alias revoke, saved old sessions, multirow sweep,
capacity including retired rows and post-seal cleanup. Checks remain active in
Release. Marker counts report actual checks only after a future genuine execution.

An actual owner may build the separate `tests/ros_rmw_service_bindings` CMake target
with an exact `UWVM_SOURCE_ROOT`, an admitted C++17+ compiler, private fresh output,
serial finite process/cgroup/deadline/memory/backing budgets and complete MD/link
source closure. CMake runs no test automatically. Real Host/Registry integration,
allocation failure injection, all paired revoke/close/worker-loss/late ACK paths,
actual SDK/native metadata/Guest default denial and full service protocol remain
separately required. This value contract is not RMW/native/runtime acceptance.
