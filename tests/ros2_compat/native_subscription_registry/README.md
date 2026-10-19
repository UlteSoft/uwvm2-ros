# Native subscription registry and transactional record transport

This is an independent source-only candidate. No generator, candidate, compiler,
contract/model, native process, ROS case, SSH command or repository change was
run while authoring it. The real same-callback Subscription/queue/D4 code was
already independently source reviewed; source review is not native execution.
The new registry and wire are not yet connected to Host, server or Runtime.
They add no Guest import, numeric transport action or capability by themselves.

## Exact native object and authority boundary

`native_rmw_subscription::registry` retains the actual endpoint returned by
`builtin<Codec>::create` or an explicit trusted compile-selected factory. The
four concrete builtins are original `std_msgs::msg::String`, Bool, Twist and
Image. A custom factory must instantiate the real selected generated Native,
complete Codec and existing custom binding contract. Missing custom support is
95. This source supplies no legacy listen fallback or fake empty endpoint and
no custom shard implementation. A future actual custom shard must be separately
reviewed; its source and complete link/MD closure remain required.

Before **each** public operation, the trusted Host and authenticated Gateway
must authorize and lease the exact current source grant, actual consumer right
where its API requires one, runtime instance, true source generation, full
64-bit source handle and live transport session. The Gateway capability must be
the matching `topic.subscribe` alias/resource/type/native identity/codec/QoS
policy. These numeric wire associations are anti-mixup values, not credentials.
No graph name or native receipt confers authority. Default denial is unchanged.
The actual Node and all default NodeOptions endpoints must already be admitted;
this candidate does not suppress logging/parameters or install any quiet mode.
Guest traffic has no debugger/update/management entry through these functions.

The existing finite endpoint history is used without editing its header. Slots,
reply storage and ownership are reserved before native creation. Failed attempts
remain tombstones, so retrying one full source handle cannot create another
native object. Fresh native incarnations never wrap or reset. All public registry
methods serialize, including complete peek/ACK/cancel operations. Native callbacks
only use the endpoint's own same-record queue and cannot reenter this registry.

UMRE-v1 stays **exactly 256 bytes** for activate/inspect/retire, subscription role2.
Its complete owner/incarnation, concrete actual native QoS, reserved bits and
response rules remain the original D4 contract. Creation, inspection, consumption
and retirement refer to the same retained native Subscription. No publisher or
legacy endpoint is temporarily created to fabricate subscription metadata.

Root must also admit the actual sum of per-source queue byte/depth controls and
the one serialized snapshot/response temporary against the owned process budget;
finite per-source maxima alone are not a global RSS qualification.

The registry validates this transport's stricter maximum canonical body
`max_payload_bytes - 96 - 192` before actual creation. The underlying standalone
queue's original larger limit is not changed. Native API calls/destructors may
block: a steady-clock postcheck detects a late result but is not an interruptible
RMW return guarantee. Root's actual pidfd/worker/IPC whole deadline and verified
teardown are required. No in-process mutex or native destructor is claimed to
provide an independent hard deadline.

## UMRS-v1: one closed 96-byte control header

All integers are unsigned little endian unless the unchanged URMI header says
otherwise. `rmw_subscription.h` is the public raw value header, with no guessed
Wasm import attributes or future ABI tuple. Root must separately register any
new transport operation and Guest numeric namespace/catalog. Neither the original
81 tuples nor existing optional catalogs are changed here.

| Bytes | Meaning |
| --- | --- |
| 0..3 | `UMRS` magic |
| 4..5 | version1 |
| 6 | command: peek1, commit2, cancel3 |
| 7 | backend2 |
| 8..11 | exact header extent96 |
| 12 | expected actual native publisher GID extent1..32 for peek, otherwise0 |
| 13..15 | zero reserved |
| 16..23 | full source handle |
| 24..31 | actual runtime instance |
| 32..39 | true source generation |
| 40..47 | authenticated transport session |
| 48..55 | actual retained native incarnation |
| 56..63 | accepted URMI record sequence, zero in a peek request |
| 64..71 | independent borrowing-attempt reservation nonce, zero in peek request |
| 72..79 | complete admitted output capacity for peek, otherwise0 |
| 80..87 | complete URMI192+canonical record extent in peek reply, otherwise0 |
| 88..95 | zero reserved |

A peek request is exactly96 bytes. Its capacity includes **all** UMRS96, URMI192
and canonical body bytes: minimum288, maximum the original IPC payload ceiling.
A successful peek response is exactly its header96 followed by the one complete
URMI192+canonical record. No padding, hidden second metadata record or trailing
bytes are admitted. Only topic-message metadata is accepted, with actual source
and receive timestamps and the exact publisher GID extent; all-zero native GID
value bytes are legal when actual extent/presence are established. Publication
and reception sequence fields remain native optional facts; they are never
invented from the queue ordinal. Canonical type validation remains mandatory at
the trusted consumer after structural association validation.

Commit and cancel requests/responses are exactly96 bytes, echoing the full
binding/incarnation, positive URMI sequence and positive independent nonce.
Capacity, GID extent and record extent must be zero. The command, both identities,
version, backend, all reserved bytes and whole size are checked independently.
An ACK for commit cannot satisfy cancel, or the reverse.

The actual queue mints a new positive reservation nonce only on an eligible
successful borrow, after metadata/ABI extent/output-capacity preflight. Its
accepted stream sequence never changes when a borrow is cancelled. A new borrow
of that same record gets a different nonce. Old attempt A cannot commit or
cancel newer attempt B even if both refer to the same URMI sequence. Exhaustion
returns90, never reset/reuse/wrap. The registry independently compares its
pending sequence **and** nonce before entering the queue's matching checks.

## Complete record ownership and failure behavior

The actual queue owns URMI and canonical bytes together. A snapshot is a bounded
transaction copy of that one record, not another queue or a last-metadata cache.
The registry keeps the real queue reservation until explicit commit or cancel.
Dropping an encoded reply vector alone does not pop the record. One reservation
per source is admitted; another peek returns17. Native callbacks can keep adding
within existing policy, but cannot evict the reserved front record. The previous
queue's exact overflow/byte-count/rate rules remain unchanged.

A too-small output capacity returns90 **before borrowing**. Missing native times,
GID metadata or incompatible actual GID extent returns95 without borrowing or
consuming. The queue, output object and next nonce remain unchanged. Oversized
body/configuration is rejected before native activation. Source/instance/
generation/session/incarnation/alias mismatch does not touch the record.

After an eligible borrow the registry immediately retains its sequence/nonce,
then validates and serializes one complete bounded packet. Packing allocates one
owned response vector and only publishes it on success. Allocation/validation/
postcheck timeout failures explicitly cancel that attempt without popping.
Both the primary failure and real secondary cancel code are retained.
`last_cancel_error`, `last_retire_error`, and `last_commit_error` have separate attempt flags and
independent slots; successful or failed later retirement cannot replace the
original cancel result. `first_error`, `last_error` and `last_cleanup_error`
remain summary conveniences and are never used as the sole complete ledger.
Explicit cancel and commit returns, including translated native-call exceptions,
use these same recording helpers. `last_commit_completed` records the last actual
commit result and is not reset by another peek/cancel; a successful native commit
followed by a late/lost ACK stays successful in that native-result slot, while
the outer110/transport error is retained separately. The actual committed counter
also remains monotonic. Fixed slots are the most recent result of each operation,
not an unbounded attempt log; Root must retain the raw operation audit as well. If cancel
itself fails, the endpoint is locally tombstoned and actually retired; ambiguous
state cannot be borrowed again. There is no best-effort retry that hides errors.

The future transport MUST call `delivery_failed` if a peek response cannot be
sent, and must save the exact primary/secondary diagnostics. A timeout before
commit/cancel does not act on the queue. Successful cancel preserves the record;
successful commit pops exactly that record and records the actual effect. ACK
storage is allocated before either effect. A deadline crossed **after** a real
commit/cancel yields110 with the completed effect retained in diagnostics; it
is not described as rollback. A lost/late ACK is ambiguous, and cannot safely be
replayed. Root must retire/abandon that transport session and keep the real
commit count and packet/audit evidence. No exactly-once delivery guarantee across
process/network failure is asserted.

For a Guest-facing atomic-output API, Root must preflight the complete memory
range/canonical layout and retain its real memory/source leases, perform the
required protocol steps, then publish the prepared bytes with one bounded copy
at an explicitly established commit point. Failure outputs must stay untouched.
There is no automatic payload sent to a control/debug socket and no automatic
queue commit inside `associated_delivery`. Whether the future Guest API exposes
explicit peek/ACK or a fully prepared synchronous take is a separate reviewed
Host/SDK contract; this source does not silently decide or add import signatures.

Cleanup first tombstones local ownership, then attempts native retirement once
and releases retained pointers even after errors. It does not need quota or a
new allocation. Revocation, source-first close, instance-generation retirement,
session disconnect/reconnect and shutdown must all invoke the matching registry
cleanup. Queue retirement happens before native Subscription reset/notify, so a
previously retained callback cannot add business afterward. Whole native destroy
may still require the actual external process deadline owner. Cleanup diagnostics
remain bounded by the original finite history. Only value copies, not retained
native pointers, are exposed in the delivery history snapshot.

## Authored contracts, execution still pending

`wire_contract.cc` contains twelve SDK-free value groups: full64-bit association,
reserved bytes/header extents, closed request semantics, same-record delivery,
source/generation/session confusion, incomplete metadata refusal, capacity
atomicity, GID/sequence mismatch, trailing/short bodies, distinct nonce versus
record identity, ACK command/token association, and failure-atomic helpers.
These are wire cases, not fake ROS entities or native qualification.

`native_registry_contract.cc` uses actual original SDK Node/defaults,
SingleThreadedExecutor, real String publisher and real Registry Subscription.
It checks the actual256-byte activation/inspection receipt; true same-callback
publisher GID and all canonical bytes including NUL; short capacity90 and incompatible GID extent95 without
reservation; cancel/reborrow same record with new nonce; stale commit and cancel
both13; failed-send cleanup; new borrow; explicit expired-before-commit refusal;
one real consume/ACK; and retirement/no source-handle reuse. The graph counts
are only an independent native fixture observation after actual creation, never
an authority predicate. Fixture numeric bindings are clearly synthetic local
association inputs, never falsely claimed to be observed Runtime identities.
The actual SDK must expose required metadata and queried-QoS APIs; otherwise the
witness explicitly returns95 before native initialization.

The native witness also contains one value-only fault-result ledger contract
with six cases: primary110/cancel17/retire5, primary12/cancel13/retire0,
primary107/cancel0/no-retire, explicit-cancel17/retire5 after primary12,
commit13/retire5, and commit0/late110/retire5. It calls the exact production recording helpers,
without implementing a fake ROS endpoint or pretending an injected value was a
real observed native error. These cases are authored and have not run.

Both build options default OFF, and there is no `add_test`. The wire target can
use an admitted17/20/23/26 standard. The native witness explicitly uses the
actual Gateway C++17 profile and a real selected-SDK compile/link-probed D4 feature
header, original generated headers/libraries and OpenSSL. Root must materialize
a coherent private tree including this source, exact D4/subscription dependencies
and the actual selected endpoint-history header, then separately qualify all
compiler commands/fullMD/ELF mappings/process lifetime/namespace/cleanup. No
handwritten generated ROS headers, entity mocks, fake feature0-to-success shim
or incomplete macro table can substitute for that acceptance.

Pending: independent review of this whole candidate and its endpoint-history
source dependency; actual native/wire compilation and execution; custom subscription
shard factory; complete Host/server/Runtime/SDK routing/catalogs, real consumer
leases/memory transactions/audit rendering and transport failure injection;
fresh whole-source freeze/native Runtime/ROS tests. The current CPP10 integration
is only an explicit source dependency and source-recompile obligation, not a
claim that any of these new registry operations have executed.
