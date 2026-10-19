# Owned ROS2 service imports

The optional `uwvm_ros2_rmw_srv_v1` namespace has twelve numeric functions. It is
separate from the original 81 business imports, the three-function independent
availability namespaces, the five publisher functions and the seven subscription
functions. The original signatures and modules stay unchanged. Memory index is
i32; every handle, incarnation, ticket, offset, length, capacity and wait is i64;
results are errno i32. This is a source-only implementation. It has not been
compiled, linked, executed or tested with a Runtime, Gateway or native ROS SDK.

The fixed raw declarations are in `sdk/include/roswasm/rmw_service_v1.h`. The
machine inventory is `sdk/abi/rmw-service-v1.json`. The strict neighboring tool
`sdk/tools/check_rmw_service_imports.py` checks actual binary import signatures;
it neither runs Wasm nor proves full validation, authority, metadata or delivery.
An independent full Wasm validator and owned Runtime/native execution remain
required. Select each other optional ROS namespace explicitly. Non-ROS imports
also require an explicit allowance. ROS1 service module spellings, unknown ROS
imports, duplicate functions, wrong memory widths and wrong results are refused.

## Real authority and one native endpoint

Default rights are empty. A client needs an already opened `service.call` owner
and a distinct `event.observe` owner whose original `event-source-alias` selects
that call alias. The observer keeps its own builtin event identity. It does not
pretend to have the native service's type or QoS. Its explicit event mask must
include `service-response` and can additionally include only `resource-error`
and `resource-closed`. Optional control kinds are not silently granted.

A server needs independently opened `service.advertise` and `service.respond`
owners with distinct full handles/aliases and matching original resource, native
type, type identity, all eight QoS fields and optional adapter codec identity.
Native code must separately prove the selected canonical codec against the
complete actual native graph. A configured custom codec identity is not proof
that a fixed builtin converter implements it. The actual integrated RCL selected
presence guard now rejects such a configuration (13), and NativeRegistry fixed
factory also refuses it (95). That omission is closed by explicit rejection.
Implementing a genuine complete custom canonical factory remains required; this
refusal is not support for arbitrary configured custom identities.

Every Host call identifies the true Runtime caller and retains the actual
primary and paired capability leases from that same instance/generation/target.
UMRC numbers and alias strings do not replace those leases. Activation alone may
connect. Later inspect, data, ACK, abandonment and retirement never reconnect.
The genuine UMRE256 receipt belongs to the same native object used for later
business calls and destruction; it contains actual QoS and any actual client GID.
Client GID, full Gateway ticket and signed native request sequence are distinct
values. No Guest counter is substituted for any native identity.

The immutable local pair and endpoint histories each have a finite lifetime
ceiling of 65,536 rows. Tombstones remain; there is no eviction/reset/regrant.
Actual server `max-services` controls its active alias count. The existing real
capability manager controls all raw source opens. There is no invented
`max-clients` property and `max-inflight` is not treated as request history space.
Native request/response tables impose their own independently bounded storage.

## Atomic data and independent quota

Submit/respond allocate one UMRC160 plus canonical body before their native
operation and read Guest input once. Submit returns one complete little-endian
eight-byte ticket. Respond uses the distinct responder alias and grant. The
fixed legacy ticket prefix is temporarily placed only in private reserved bytes
and restored before any RPC or Guest copy. The trusted manager validates the
complete positive-ticket/body envelope but charges only canonical body bytes.
Logical Empty body length zero remains zero; private allocations still have
valid non-null envelope storage.

Peek returns one whole UMRC160 + URMI192 + canonical body. The entire Guest output
capacity is checked before borrowing; the final actual Runtime writer checks
again and copies the complete result once. There is no separate length-slot
write and no fallible operation after a successful output copy. Every nonzero
return preserves all Guest output bytes. The minimum capacity is 352 bytes and
the maximum is the actual IPC ceiling, 4 MiB. Canonical capacity/native metadata
are verified from the actual complete associated response, not a valid prefix.

A client spends one `event.observe` poll operation even for native idle11,
matching the original event poll contract. Its response/control delivery then
uses the actual existing `authorize_event_delivery`: explicit observer mask,
64-byte logical event header plus body credit, correct source alias, genuine
response codec and independent canonical response source credit. This logical
UWRE64 is an admission view of the same URMI, not a second delivered record or
another metadata queue. Server requests use the real advertise ticket/body
validator and canonical body credit. Server control records have no invented
canonical body or advertise request operation. UMRC, UMRE, URMI and ACK overhead
are transport metadata and do not consume service canonical-body credit twice.

Peek reserves the actual accepted stream sequence and a separate positive
per-attempt nonce. Cancel preserves the record; a later attempt at the same
sequence must have a fresh nonce. Commit consumes only that exact outstanding
pair. No unknown ACK, submit or response is replayed. Host diagnostics retain
first/last primary errors, operation, cancel, commit and native cleanup separately.
Host API return is explicitly separate from the main IPC client-call API
observation. A per-API flag resets immediately after actual known-source
authorization, before audit allocation, deadline and history checks; an older
returned RPC remains independently historical. Local preflight is not an RPC attempt; Host Guest-copy22 cannot
overwrite a returned RPC0. Pending RPC115 means the client call has no observed
return, not proof that Native ran. Even a returned RPC0 is not independently
proof of native execution, metadata validity or ROS completion. Cancellation and
cleanup have their own separately retained attempt/result fields.

A Host refusal or allocation/other exception after a completely known borrow
attempts exact cancel under the original entry deadline. The actual guarded
post-association region includes manager capability/map/audit work, and all
private prefix bytes are restored before compensation. Malformed association, unknown progress, lost/late ACK
or failed cancellation retires the native session. Only the real Registry
operation-specific empty peek11 (no visible queued item or reservation) retains
it. That poll may have disposed of a previously abandoned/timed-out late native
response; it is not a claim of zero native take. Submit, respond, availability
and ACK failures remain terminal absent a distinct future proven contract. Closing a session abandons
all of its native endpoints; a future connection cannot revive an old full source.

## Availability is a third authority

`client_available` uses the same retained native client and its actual owned RCL
availability API. It also requires a distinct independently opened
`service.available` owner and the original complete source/type/QoS/codec pairing.
It uses that grant's finite wait, history and timeout bounds, and its own actual
64-byte observation request/response credit. The call grant cannot authorize a
query. A zero wait means a real native observation, not false readiness. No graph
list, sleep, business request, probe client or guessed feature bit substitutes.
The output is exactly one byte 0 or 1. Missing native APIs return unsupported95.
The currently integrated NativeRegistry performs the genuine wait0 query only;
positive waits explicitly return95 until a real independently owned native wait
provider is implemented and qualified. This Host accepts only bounded requests
and does not turn that missing native wait into a successful observation.

This same-client query association has a separate local history. It does not
reuse the legacy availability native registry, create a second native client or
send legacy retirement to the wrong registry. Closing the third query owner
removes query authority while the independently authorized business pair can
remain. Source close/revoke retires the associated local query histories too.

## Retire both sides without reviving authority

Close/revoke receives the actual closing instance and generation. It matches
both stored full handles or either original alias against each row's SAVED old
native session. Connection session0 never hides a previously owned row. First it
marks every matching pair and UMRE locally retired, without allocation or quota.
Then it attempts native cleanup once per actually observed incarnation under one
shared entry deadline. The private action always uses the saved primary alias
and peer, including when the auxiliary has already lost authority. It creates no
new grant. Disconnection records independent cleanup107 and never reconnects.
An exception/error for one row cannot skip diagnostics for remaining rows.

Native cleanup, commit, cancellation and raw owner close remain independent
results. A successful native retirement does not close the Guest's capability
handles for it. A local tombstone does not prove actual native fini succeeded.
Unknown native cleanup may still leave effects until authenticated session
retirement drains them. This source cohort claims none of those native results.

## Required real qualification

Before any deployment, independently build the genuine current Runtime source
with complete dependency/MD evidence and ROS2 flags, then validate the exact
binary import inventory. Build the genuine native RCL/codec/registry/Gateway
closure against an actual installed SDK; missing APIs and metadata must fail
closed. Exercise Empty and AddTwoInts with full request/response values, positive
native identity/QoS/GID/metadata, wrong binding/revocation/credit rejection,
borrow/cancel/commit ambiguity, output sentinels and independent owner cleanup.
Keep original failure artifacts and separate source, model, Guest-default-deny,
actual native and complete integration evidence. None is implied by the authored
value tests or this import inventory.
