# Owned original-native subscription and same-record metadata

This is a source-only candidate, not an executed native SDK result. It adds a
new real `rclcpp::Subscription<Native>` owner and one bounded receive queue. The
Host namespace, authenticated IPC dispatch, public RMW C adapter and Guest SDK
are deliberately not connected by this package. Existing 81 Guest imports,
legacy event bytes, existing listen/activation paths and NodeOptions are not
changed. These headers confer no ROS, network, debugging or management rights.

## Source and actual API prerequisites

Use the exact separately held Root D4 endpoint/metadata/builtin sources named
in this package's manifest. D4 maps actual queried QoS through the linked
original `rmw_time_normalize`, and uses real `rmw_time_equal` in its configured
QoS checks. Do not replace it with the older D2 arithmetic-only duration mapper.
The actual selected SDK must independently compile and link the D4 feature
probe; unconfigured `UWVM_ROS2_HAVE_COMPLETE_RMW_QOS` is false. A retained native
subscription without a real actual-QoS getter refuses 95 before entity creation.

The callback signatures are original SDK interfaces: a shared const native
message and `const rmw_message_info_t&` on the selected older SDK, or a shared
const native message and `const rclcpp::MessageInfo&` when its original header
exists. In the latter case the callback immediately reads that exact wrapper's
`get_rmw_message_info()`. The pinned original Dashing/Jazzy callback definitions
and the original subscription/MessageInfo sources are recorded as dependencies.
Header presence chooses a real signature; it is not a native compatibility
test. No fake ROS headers, callback entity, native GID, timestamp or type hash
is supplied by a model.

The builtin subscription contract is distinct from D4's publish-only contract.
It requires `topic.subscribe`, the exact builtin type, and the existing complete
`builtin_message<Native>` identity proof. It reuses D4 canonical validation,
including Twist bounds. Custom instantiations use the unchanged complete
generated `custom_contract<Native, Codec>` and the existing trusted table. A
missing table/type/schema/default proof refuses. Fixed generated codecs use
their actual fixed `wire_bytes`; bounded generated codecs and the four builtins
use their actual `encoded_size`. No generator or native converter ran here.

## Public lifetime and authority contract

The future transport must authenticate the Host session, lease the exact current
source capability and reserve its registry/quota/control slot *before* calling
`typed_endpoint::create`. The complete binding is backend 2 plus full 64-bit
source handle, instance, actual source generation and session. The caller also
assigns a nonzero never-reused native incarnation. A numeric match is only an
anti-mixup check; this is not a bearer-token API or permission to query a graph.
Every inspect/read/commit/cancel/retire operation requires that exact binding
and incarnation. A stale component returns 13; a retired owner returns 125.

Creation copies capability/control state, initializes the rate bucket, allocates
queue controls and installs an unpublished empty endpoint owner before creating
the native subscription. The callback captures a weak pointer to its separate
owned state, not an endpoint raw pointer. The factory retains the actual created
subscription, checks its actual topic name and actual QoS against the configured
profile, then activates this queue. Output owner and observed packet commit only
after those checks. Failure destroys this exact created object and attempts the
same original executor notification as D4; it never adopts a graph peer or falls
back to legacy listen. Local reset/notification alone is not proof that a native
executor has retired all shared references: the real external owner still owes
its independently bounded native cleanup and process/namespace teardown.

`inspect` queries the same retained object again. A changed actual QoS does not
silently update its admitted association. `retire` closes the queue ingress and
consumer under its mutex before resetting the native subscription and notifying
the executor. It also discards queued business with an explicit retired-record
counter. A native callback already encoding may finish local work, but enqueue
after this linearization point returns 125. A callback's retained shared state
keeps its immutable capability/queue alive until it returns, so no callback uses
a freed endpoint. No native reset waits while holding the queue mutex.

The template is a trusted Gateway component, not a sandbox for arbitrary codecs.
The original SDK owns the callback native object's immutable lifetime. These
checks do not protect a native application that illegally mutates the same const
message concurrently. No Guest memory reference is retained or sampled twice.

## Atomic record, counters and consumer transaction

One accepted record owns its encoded 192-byte URMI header, canonical body and
native-capture error. Metadata is read in the same actual callback as that body.
There is no second queue or last-message metadata cache. Callback serialization
bounds encoding to one temporary per subscription and serializes its rate bucket.
Cheap dynamic String/Image size checks precede full native validation/copy;
complete generated/native and owned canonical validation are both retained.

The queue assigns sequence 1, 2, ... only when acceptance commits. Refusals do
not reserve a number. It refuses at UINT64_MAX rather than wrapping or reusing.
Explicit drop-oldest can create observable consumption gaps, recorded by its
drop counter; accepted sequence order remains contiguous and never reused. A
local sequence placeholder used to call D4's pure metadata validator is replaced
inside the queue commit mutex and is never a stored receipt.

All-zero bytes in an actual native GID are legal. Presence is established by
D4's genuine native `implementation_identifier`/data extent, not by searching
for nonzero bytes. Original SDKs without native timestamps retain their absent
bits. A native capture returning 95 retains that error with this same body;
unknown/invalid metadata is never invented. A strict `peek` requires actual
source and receive times, publisher GID, and the exact admitted consumer GID
extent. A missing field, capture 95, or incompatible extent returns 95 before
allocation, reservation, output change or removal. That record remains queued;
there is no automatic lossy skip by that read. Later native arrivals still obey
the independently selected overflow policy; metadata refusal does not itself
force a drop. A caller may explicitly retire the whole owner.
Optional native publication/reception sequence fields retain their actual values
when present; UINT64_MAX remains the original unsupported sentinel.

`peek(owner, incarnation, expected_gid_bytes, snapshot, capacity)` produces one exact
owned contiguous header+body snapshot plus a separate positive reservation nonce,
and reserves the queue front. The optional last argument defaults to the protocol
maximum; an actual consumer passes its independently checked header-plus-body
destination capacity. An insufficient capacity returns 90 before borrowing a
nonce, allocation, output change or removal. It does not
consume. The authenticated caller must revalidate its source/observer grants,
check the complete destination extent and perform the atomic Guest-output copy
under the applicable leases. Only then call `commit(owner, incarnation,
snapshot.sequence, snapshot.reservation)`. If output validation/copy fails, call
`cancel` with those same four arguments; queue bytes and record sequence remain.
This package cannot make an external unleased raw Guest copy safe.

Only one reservation exists. A second peek returns 17. Drop-oldest refuses an
incoming record while the front is reserved, so callback activity cannot replace
a validated record before commit. Both record sequence and reservation nonce
must match; wrong/stale commit/cancel returns 13. Retire
invalidates the reservation and frees all business payloads. Snapshot copies
already owned by a caller remain ordinary memory; their continued existence is
not authority to write to a retired Guest resource.

## Quotas and failure policy

The source's `qos.depth` bounds queue records, with an additional implementation
ceiling 65536. `max-message-bytes` bounds canonical body size and must fit both
the URMI transport maximum and the source's `max-queue-bytes` after a header.
`max-queue-bytes` is at most 1 GiB and charges 192 bytes plus the actual retained
body vector capacity for each record. Fixed preallocated record control storage
is additionally bounded by record count. Pop/retire releases payload capacity;
it does not silently retain uncharged cleared vectors. The owned consumer
snapshot allocates exactly header+logical-body bytes; one snapshot and one codec
temporary are outside the retained-queue byte ledger. Codec allocation itself
remains a real selected C++ allocator operation: logical extents are checked
first, retained capacity is charged, allocation errors return 12, and the external
worker/cgroup deadline and memory limit remain required. This is not a claim of
an allocator-independent total-RSS bound.

The callback charges the existing real-time rate/byte token bucket before
canonical allocation. No arrival can bypass source type/body validation. A
refused/overflowed message may still cost a charged arrival token; it is not
automatically replayed. `drop-oldest`, `drop-newest`, and `reject` are explicit
local queue policies. Invalid headers are checked before any old record can be
evicted. A too-large incoming allocation is refused without evicting anything.
The package refuses `backpressure` with 95 before native creation: there is no
real native pause/ack protocol here, so no fake backpressure/pending queue exists.
Source subscriber/max-inflight limits are a future transport registry admission
obligation; this standalone endpoint does not globally count other owners.

The queue record sequence is encoded in URMI and remains the contiguous accepted
stream identity. The reservation nonce is never encoded as a queue sequence or
native metadata. A cancel followed by another peek of the same record retains
identical URMI/body bytes but receives a different nonce. An earlier attempt can
neither commit nor cancel the new one. A token is borrowed only after metadata
and ABI/capacity checks and successful snapshot allocation/copy. Every failed peek leaves
the output, token counter and reservation state unchanged. Positive u64 nonces
are never reused, reset or wrapped; after UINT64_MAX successful reservations,
an otherwise eligible peek refuses 90 without consuming. Unsupported metadata
still returns 95 before that exhaustion check. Retirement clears only the active
reservation, not its historical counter. Neither a nonce nor an owned snapshot
is a permission: the transport still owes all current independent leases.

Counters saturate, and include accepted, consumed, both drop policies, rejected,
retired records, callback errors/first error, accepted metadata-unavailable records,
depth, charged bytes, high water, last reservation nonce and reservation state.
They are diagnostics, not a second source of payload metadata.
Error 11 means empty/not-yet-active/rate unavailable; 12 allocation; 13 foreign
association; 17 another consumer reservation; 22 malformed data/configuration;
90 local size/queue/exhaustion; 95 unsupported actual API/metadata/policy; 125
retired. Exceptions do not escape the callback: allocation maps to 12 and other
native/codec exceptions to 5. Successful outputs commit only at the end.

## Authored contracts and future execution

`queue_contract.cc` contains 19 explicitly called data groups. They use the real
URMI codec and owned queue but no ROS entity/header substitute. They cover complete
same-record bytes, signed times, actual all-zero GID data, missing metadata and
incompatible extent nonconsumption, every binding component, reservation/overflow
interleavings, cancel/one-shot commit, exact accepted sequences, drop accounting,
header/capacity charging, invalid-header non-eviction, retirement, empty body,
preactivation and a real two-thread queue enqueue/retire interleaving. Two new
ABA groups specifically refuse a cancelled attempt's commit and cancel after
another peek borrowed the same record with a new nonce; they also check missing
metadata and wrong ABI do not borrow nonces. The original 17 group bodies retain
their original checks with only necessary commit/cancel signature adaptation.
These are
authored, uncompiled, unexecuted; the thread contract is not a native entity model.

`native_subscription_contract.cc` is an unrun original-SDK witness. It requires
an explicit `--trusted-native-fixture /unused_absolute_prefix` and an external
owned deadline/cgroup/private network admission. That flag admits only this
trusted process's native fixture entities; it is not a Guest grant. Each of four
real builtin types creates a retained subscription and an independent D4 owned
publisher, inspects actual QoS, publishes exactly one original complete value,
checks canonical roundtrip and the publisher's actual GID in the same captured
record, tests a foreign binding/width refusal and cancel/commit, retires the
subscription, then publishes a second value and checks no new queue business.
String includes embedded NUL, Twist covers all six doubles and signed zero, and
Image covers the complete stamp/frame/dimensions/encoding/endianness/stride/data.
Original NodeOptions remain unchanged. A fresh absolute topic prefix is required.
No graph name/count is used as authority: bounded counts are only fixture
discovery/retirement observations. Unsupported actual SDK features fail nonzero,
never a passing skip. Real native publishers/subscribers and their default Node
endpoints must be admitted by the future external owner before this is run.

For later real execution, first assemble a coherent source snapshot containing
the exact six proposed files, all D4 candidate files and the selected current
dependency closure. Configure the genuine selected SDK's D4 feature directory.
With an admitted actual compiler, explicitly set C++17 or later and enable only
the intended CMake option: `UWVM_NATIVE_SUBSCRIPTION_QUEUE_CONTRACT` for the data
contract, or `UWVM_NATIVE_SUBSCRIPTION_REAL_SDK` plus the absolute actual
`UWVM_ROS2_GATEWAY_FEATURE_DIR` for the native witness. CMake adds no `add_test`;
Root must separately own/reap each executable, keep assertions active, retain raw
stdout/stderr/native source/MD/link/architecture receipts and prove teardown.
No configure, compiler, probe, model, generator, native call or SSH ran during
authoring. Full Host/IPC integration, custom-type native executions and actual
ROS delivery remain pending. This package establishes none of those outcomes.

## Preserved first hold and D4 preflight successor

The complete initial D3-dependent six-file hold remains unchanged and is copied
under `before-r1`. This successor adds the exact D4 complete-QoS feature guard
before native creation, in addition to the actual getter requirement. With a
real getter but missing compile/link-probed utility support, it now returns 95
without creating a native subscription. Callback/queue/retirement semantics and
all then-existing 17 authored queue groups/four native value families were
unchanged in that D4-only successor. The D4
input is the actual separately held source package, not a forecast final peer
or executed SDK result. Its complete files and dependency pins are preserved.

## Independent reservation attempt identity

The first and second holds identified a reservation only by the queued record
sequence. The independently reviewed ABA path was `peek A -> cancel A -> peek B`
on the same front record, followed by an old A commit/cancel. That old request
could act on B because its record sequence was unchanged. Both holds remain
immutable historical source; this third successor adds the independent nonce
described above. It changes neither accepted URMI sequence nor canonical/native
metadata bytes, and it does not rely on a future Host serialization assumption.
The native witness passes both snapshot identities through its existing
cancel/commit calls. No previous contract or native execution is claimed here.
