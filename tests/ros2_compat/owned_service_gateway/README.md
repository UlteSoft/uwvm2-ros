# Explicit original-RCL service route in the ROS2 Gateway

This candidate is source only. None of its configuration, source, probes,
models, compilers, Native routes or examples has run. It adds a private
`rmw.service-v1`/`rmw.service-retire-v1` Gateway route over the exact existing
UMRC160/UMRE256/URMI192 wire. It installs no Guest import, changes no original81
ABI tuple, and changes no Host/Runtime/loader/SDK source. Those independently
reviewed producers/consumers must be composed and tested later. Ordinary ROS1
and ROS2 Gateway targets must source-recompile against the added adapter seam;
no old C++ binary/vtable compatibility is claimed.

The concrete Native providers are the real original
`native_rcl_service::client<Service>` and `server<Service>` retained through the
actual source-integrated Registry9. Their initial complete families are
`std_srvs/srv/Empty` and `example_interfaces/srv/AddTwoInts`. Empty has canonical
request0/response0. AddTwoInts has request16/response8, all signed-i64 bit patterns
and little-endian canonical encoding. No handwritten ROS type, RMW function,
expected-hash array, generated placeholder header or legacy listen/temporary
client is substituted. An arbitrary same-width type is not a provider.
Explicit `adapter-codec-identity` remains refused: these finite CPP10 descriptors
prove their actual Native graph and canonical conversion but do not carry a
separate complete compiled canonical-identity string. Presence is never dropped.

## Two explicit selections, with no new grants

The existing trusted Gateway policy config still explicitly supplies every
alias/resource/type/Native identity/QoS/quota. This profile does not alter it.
The independent build option `UWVM_ROS2_BUILD_OWNED_RCL_SERVICE=ON` enables the
real provider. OFF preserves the existing C++17 route and original target set;
passing a runtime profile flag to an OFF binary is refused. ON requires an
explicit selected C++20 or23 SDK profile and actual CMake3.20 or newer. It links
the actual CPP10 source as a separate static component, using every original
installed generator/TypeSupport target required by its attachment helper. No
`rmw_uwvm` replacement library or fake dynamic loader is involved. Historical
plain ament link signatures remain plain on the Gateway target.

ON still creates no Registry or service endpoint when no runtime option is
passed. The historical invocation remains:

```sh
uwvm-ros2-gateway /absolute/trusted-gateway.cfg
```

An explicitly selected run instead adds:

```sh
uwvm-ros2-gateway /absolute/trusted-gateway.cfg \
  --owned-rmw-service-profile /absolute/actual-native-service.profile
```

The extra ordinary file is read with the actual `native_io::config_lines`:
NOFOLLOW/NONBLOCK open, owned regular-file fstat and safe permissions before
blocking reads, fixed4KiB input/line storage. The profile adds at most64 lines,
256 characters per line,8192 text characters, no unknown/duplicate keys, and
exactly14 mandatory key/value fields. Empty/comment lines count toward the
finite limits. No fixture identities are included as usable defaults:

```text
string <independently exported actual lowercase64hex>
time <independently exported actual lowercase64hex>
event_info <independently exported actual lowercase64hex>
empty_request <independently exported actual lowercase64hex>
empty_response <independently exported actual lowercase64hex>
empty_event <independently exported actual lowercase64hex>
add_request <independently exported actual lowercase64hex>
add_response <independently exported actual lowercase64hex>
add_event <independently exported actual lowercase64hex>
empty_service <independently exported actual lowercase64hex>
add_service <independently exported actual lowercase64hex>
maximum-entries <explicit1..65536>
history-per-endpoint <explicit1..256>
maximum-string-wire-bytes <explicit4..4193786>
```

The angle-bracket descriptions above are intentionally invalid profile values.
Root must export all11 genuine identities from the admitted actual original
SDK; copying a test fixture or these descriptions is not Native qualification.
The real immutable nonmoving CPP10 Registry warms and validates the complete
selected original graphs/identities before this Gateway creates its Node.
The profile and actual native libraries outlive all endpoint borrows. Each
session owns a distinct real Registry9 endpoint table sharing that same
immutable CPP10 Registry. Source history and storage are finite lifetime limits,
not capabilities. Client endpoint storage uses the profile `maximum-entries`,
not a fabricated max-clients grant or the pending-request `max-inflight` field.
Each actual client independently enforces its real pending-request limit.
Server activation additionally checks the actual `max-services` live count.

The original Gateway quiet-node configuration and existing security/context/
executor behavior stay unchanged. This package does not change upstream
NodeOptions defaults or claim to implement upstream WASI Threads.

## Authenticated session, actual grants and saved pairing

The ordinary Unix hello/peer-credential/pidfd/session.bind admission remains in
force. Only the exact configured trusted Host worker can reach this route. That
Host must hold both actual full current source/auxiliary handle leases across
each call; availability also requires its independent third real lease.
UMRC integers are associations within that authenticated Host session, never
bearer credentials or a reason to invent an alias/grant.

For activation the Gateway finds exactly one structurally valid ORIGINAL
configured peer using actual `service_endpoint_pair`. Client pairs use
`service.call` plus `event.observe`, the actual `event-source-alias`, same
resource, independent observer builtin identity and explicitly selected response
mask. Response is required; error/closed are optional and never added. Native
service QoS is not imposed on the observer. Server pairs use advertise/respond,
distinct aliases and the actual full type/identity/all8QoS/optional-codec fields.
Ambiguity or absence is refused. The Gateway mirror copies this full pair into
the same finite non-evicting helper before Native creation. Failed/closed keys
are never erased or reused. The real Native Registry separately verifies the
same full binding/instance/generation/session/incarnation and original grants.

Later ordinary control/submit/peek/ACK/abandon requests carry the primary alias.
Respond carries the stored independent responder alias. Query carries the
independent availability alias and its actual `service-source-alias`; the saved
original pair remains mandatory. The Gateway restores both original grants
from the saved full row and immutable config, checks current session/owner/peer
and revocation, then passes actual admission values into the real Registry.
Public retirement uses ordinary `rmw.service-v1` with the live original source
and paired leases, like the actual Host endpoint-control call. Private retirement
uses only `rmw.service-retire-v1`, requires the known saved full pair and primary
alias, and may use that saved row after auxiliary revocation. The private lane
rejects every non-retire command; it cannot become an opener. Unknown pairs cannot use cleanup as an opener.

Closing/revoking either paired alias tombstones all its saved rows before Native
cleanup, including rows with their old saved session. Closing the separately
independent availability grant does not manufacture call authority or a second
client. Disconnect clears all Native rows before unrelated async pool joins. The actual
cleanup guard is declared after the async pool and references the service route,
so exceptions from allocation, audit or response handling establish the same
ordering before the pool destructor joins; normal clear remains idempotent.
Once-only original RCL fini and all actual primary/commit/cancel/retire/fini
results remain in the real Registry's retained reports. Terminal bounded audit
lines expose each source's independent outcomes and actual per-operation Native
call counts. The outer owner must capture complete logs and treat suppressed/
missing diagnostic output as incomplete observation, not successful cleanup.

## Exclusive routes, quota and the reservation transaction

A configured alias claims either legacy or owned service routing until final
close. Legacy calls, advertise, respond and old availability check that claim;
owned activation claims both original aliases before Native create. Existing
legacy observations/queues refuse an owned activation before Native effects,
and an owned live row refuses a new legacy response watch. Legacy-to-owned
switching cannot be used to bypass simultaneous Native entity storage/count
limits. A refused activation may retain a finite route/history tombstone; it is
not permission to retry through another Native path.

Client submit and independent responder calls debit only canonical body bytes
under their actual aliases. Activation uses separate existing creation buckets.
Client completion delivery atomically previews actual response-body credit and
observer logical UWRE64+body credit, then debits both under the one request
owner. Server request delivery charges only its actual canonical request body.
UMRC,URMI,Native GUIDs,tickets and reservation nonces are not business payload.
Availability separately charges its actual64-byte envelope/rate grant. ACK,
retire and cleanup do not become extra requests or acquire quota-gated close.
There is no generic second debit of a service wire reply.

The real Native Registry allocates complete outputs before Native effects and
performs at most one original take per peek. The Gateway checks the whole
associated UMRC+URMI+body before use. A successful peek arms a delivery cleanup
obligation before any further quota, validation, allocation, audit or socket
write can fail. Failure cancels that exact borrow and preserves primary and
cancel outcomes independently. A failed/late socket reply never commits a
Native record. A successfully sent peek retains the actual borrow until the
separate exact sequence+nonreusing-nonce ACK; sequence is not substituted for
reservation identity. Any ambiguous/failed ACK terminates the session and
retires its Native objects rather than replaying it.

Only an actually empty peek11 reply is the nonterminal idle exception. Error11
with any payload is inconsistent and terminates the session. Empty peek11 means no reservation or accepted visible item; one
actual take may nevertheless have fully validated/disposed an already
abandoned late response. No other numeric error is treated as a general
no-effect guarantee. After entering the Native backend, every error except
that specific idle peek case terminates the session. Expired/lost endpoint,
submit/respond/abandon/ACK replies are also terminal, never replayable sends.
Local preflight/quota failures before the Native call do not invent a Native
attempt. Middleware calls themselves are not preempted by post-return clock
checks: the owning Gateway process still requires Root's real finite external
deadline/pidfd termination/retirement envelope.

Zero-wait availability queries the exact original retained client, with its
independent grant. Positive waits return95 before a Native call. A true bounded
`get_graph_event`/`wait_for_graph_change` path and production link probe remain
future work. No sleep, iteration counter, graph name, replacement client or
existing ephemeral `service.call` client is presented as that implementation.
The witness's bounded polling driver is explicitly separate from this API.

## Authored contracts and remaining qualification

`value_contract.cc` contains10 pure actual production helper groups: strict hash
text grammar/atomic output; zero-byte Empty; full signed-i64 extents; unknown or
explicit unproven codec refusal; unique original pairing; wrong resource/mask;
exact private operation names; honest absent-provider95; the actual closed
public/private command-lane grammar; and operation-specific empty-idle classification,
including nonempty11 and non-peek11 negatives. An optional11th
reads an independently admitted real trusted profile with the actual no-follow
bounded reader. Hash grammar DATA in a pure parser test never warms a Registry
or becomes a published identity.

`native_wire_contract.cc` is a real future socket/Gateway/SDK witness, not a
mock endpoint. Root starts this process and a genuine Gateway separately. The
witness prints READY and waits for actual stdin G so Root can create a trusted
config with its real PID/UID and start the owned Gateway before release. It
binds that exact policy manifest, creates original Empty OR Add client/server,
queries the same client under independent authority, sends/takes complete
canonical bytes, verifies same-take request writer/signed sequence, cancels and
reborrows the same record with a fresh nonce, ACKs and responds through the
independent responder, then exercises genuine public client retirement, a retired inspect125, separately
explicit private server retirement and paired observer/responder alias cleanup.
Terminal Native reports must show each actual fini occurred exactly once.
Its full64 runtime-like handles are explicitly logical fixture DATA, not proof
of a real Runtime manager lease. Therefore even its future success would be a
Gateway/Native component result, not a Guest/Host/default-deny end-to-end result.
Root must separately check terminal actual once-fini/audit/MD/mapping evidence.
The existing Registry9 witness remains the actual stale-ACK ABA negative source.

Future owned build, never executed here:

```sh
cmake -S gateways/ros2 -B /tmp/uwvm-owned-service-gateway-build \
  -DUWVM_ROS2_CXX_STANDARD=20 \
  -DUWVM_ROS2_BUILD_OWNED_RCL_SERVICE=ON \
  -DUWVM_ROS2_BUILD_OWNED_RCL_SERVICE_CONTRACTS=ON
cmake --build /tmp/uwvm-owned-service-gateway-build -j1
```

The Gateway and witnesses must run inside the admitted original64GiB/swap-zero
scope with finite Root/leaf/process memory, whole clocks and physical floor,
exact selected SDK/compiler/TypeSupport mappings, source/input/output pins and
complete fullMD/link closure. No automatic run or CTest is installed. Native
metadata timestamps/GUID support must actually be qualified for each selected
RMW; a zero value is not absent data and an existing struct field is not a
runtime completeness proof. Every all-zero Native GID remains legal if the
actual complete API wrote it. No old Runtime60–63 product or retired ELF is
required. Shared codec.h source layout changes require complete source rebuild.

Host raw12 composition, real Guest full-handle lease tests, complete fault
injection (transport loss, quota cancellation, late ACK, Native fini failure),
positive finite wait, compiled custom service proof/families, all distributions,
security interoperability and hardware performance remain explicitly pending.
This source cohort claims none of those outcomes.
