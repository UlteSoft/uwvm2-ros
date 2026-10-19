# Retained native ROS2 client/service transport

This cohort implements the complete UMRC value protocol and its optional raw C
header. It is source-only. Host registration, retained native registry, Gateway
routing and concrete Guest C adapters are separate required implementations.
The declarations are not a claim that these imports already link in a Runtime.
No model, compiler, feature probe, Wasm, native ROS or performance run occurred.

## Permissions and native facts

A client binds one actual `service.call` source to its independently granted
`event.observe` observer. A server binds one `service.advertise` source to its
independently granted `service.respond` owner. Availability additionally needs a
distinct exact typed `service.available` source. Host and Gateway must hold the
real current leases for every participating owner. `event.observe` is the
authority operation; the original event-poll import/action name is not a grant. Alias, resource, native type,
complete schema/hash/codec and every QoS field must agree. Matching a service
name, knowing a numeric ticket, or a complete well-formed packet grants nothing.
No imports expose Host update, debugger, policy or management operations.

Activation eagerly owns one private direct-RCL client/service. The same object
must perform inspect, actual send/take/respond, availability and once-only fini.
It cannot be substituted with a temporary query client, graph-name lookup,
rclcpp executor consumer or a legacy business route. Original NodeOptions/default
endpoint behavior must remain intact. Concrete unsupported SDK features return
95 without fabricating native facts. A reduced feature profile is not complete
original rclcpp compatibility or qualification of all ROS distributions/vendors.

UMRE256 preserves both actual queried request/response QoS profiles. An actual
native client GID is separate from the complete request writer GUID supplied by
a real service take. Never cast, truncate, synthesize or substitute one for the
other. Signed actual native request sequence, actual writer GUID, Gateway ticket,
accepted record stream sequence and borrow reservation nonce are distinct values.
Native zero fields are data; metadata presence must be explicit. Actual selected
RMW timestamp completeness remains a real SDK qualification requirement even
when the RCL structure/API has timestamp members initialized to zero.

## Exact wire format

UMRC-v1 is a160-byte little-endian control header. Bytes14..15 and128..159 are
reserved zero. Every source and peer is a full generation-bearing handle. Every
operation except activation requires its original native incarnation. Peer
selects the retained observer/responder pairing; only availability substitutes
its independently admitted availability owner. Its full context is still the
primary instance/generation/authenticated native session, not Guest-supplied
principal fields. The metadata packet does not replace IPC authentication.

| Command | Effect and successful reply |
| --- | --- |
| activate1 | Allocate one actual endpoint; UMRC160 plus actual UMRE256 |
| inspect2 | Query that same endpoint; UMRC160 plus associated UMRE256 |
| retire3 | Once retire that incarnation; UMRC160 plus associated terminal UMRE256 |
| submit4 | One client send of complete canonical request; UMRC160 with positive Gateway ticket |
| peek5 | Borrow one whole owned service/control record; UMRC160 plus URMI192 plus entire body |
| commit6 | Consume exactly the accepted sequence and independent reservation nonce; exact UMRC160 ACK |
| cancel7 | Release only that borrow, retaining its record; exact UMRC160 ACK |
| respond8 | One actual response using separately admitted responder and retained request ticket; exact UMRC160 ACK |
| available9 | Observe the same client under separate availability authority and real finite bound; UMRC160 with boolean value |
| abandon10 | Local request abandonment and late-response disposal; exact UMRC160 ACK, no invented native cancellation |

Only submit/respond requests have a canonical body. The exact whole request
length must match `body_bytes`; a valid header prefix is insufficient. Input
uses the original IPC request encoder ceiling minus160 metadata bytes. A peek
capacity includes all160+192 header bytes and the entire body (minimum352,
maximum4MiB); it is not only canonical capacity. `record_bytes` is internal.
A zero-length Empty stays zero; the wire never pads it with a dummy business byte.

The peek reply echoes its original capacity/writer extent, and binds URMI owner,
incarnation, stream sequence and correlation to UMRC. A client may receive only
a complete service-response record, a server only a complete service-request
record. Both can receive real resource-error/closed controls with exact original
error/flags/correlation, zero business body and no invented service-info presence.
Commit/cancel ACKs echo both sequence and nonce. Cancel/repeek uses a fresh nonce.
No code may retry an uncertain send, response or ACK or claim cancellation rolled
back a native effect. Every Host rejection after a successful borrow needs one
bounded explicit cancel or terminal session retirement with separate outcomes.

## Raw C interface and transactional outputs

The twelve imports use only numeric Wasm values and actual memory indexes.
Offsets/lengths remain64-bit on both Wasm32 and Wasm64. Activation/inspection
write exactly256 UMRE bytes, submit writes one eight-byte little-endian ticket,
availability writes one byte0/1, and peek writes one complete UMRC/URMI/body
packet. Each has exactly one final Host write, with all preflight/validation and
deadline checks before it. Every nonzero error preserves output bytes. Other
operations have no Guest output. No native pointer/struct/callback/CDR crosses.

Availability wait_ns0 performs a single real query. Positive waits require an
actual independently admitted finite native wait mechanism and one shared
monotonic deadline. Poll counts, arbitrary sleeps and fresh client construction
are not elapsed-time/native-availability proofs. Until that mechanism exists,
positive waits must be unavailable rather than fake success.

After Native retire, close all original paired raw owners independently. Closing
or revoking either paired owner must tombstone all related incarnations before
cleanup. Session loss, timeout ambiguity, instance reset, update and debugger
retirement must drain every retained Native effect. Pending/failed cleanup is
kept independently from primary send/take/commit/cancel diagnostics. A retired
full source cannot automatically recreate an endpoint or reuse a native identity.

## Authored qualification inputs

`wire_contract.cc` contains18 always-active production-helper contracts for
complete extents/association, foreign peers, reserved bytes, output atomicity,
real zero/signed native values, explicit metadata presence, control records,
logical zero-body Empty, actual-receipt shapes and independent ticket/nonce
fields. Its records are logical fixtures and supply no native ownership or
complete Runtime/permission evidence. `raw_signature_contract.c` checks all
twelve real declaration types without fake Host definitions. CMake defaults OFF.

During the later unified test phase, build products only in an isolated `/tmp`
directory locally, or entirely within the exact64GiB/swap0 Linux cgroup. Actual
selected-SDK compilation/link/full MD, complete Wasm validation/import inventory,
current Runtime dispatch/default denial, same-object native full-value behavior,
terminal cleanup, all original upstream/distribution/vendor/QEMU suites and
native-versus-Wasm hardware VTune profiling remain required. No current-source
qualification is inherited from older successful binaries or local models.
