# Robot scene provider contracts

This document specifies provider contracts and qualification boundaries for
`uwvm_ros_v2`. The accompanying [semantic catalogue](../../tools/ros_scene/interface-catalogue.json)
distinguishes registered imports, a tested backend and proposed operations.
It is a design input, not a generated import table or a portability certificate.
Current native driver, model and ROS communication providers attach to processes
owned by external deployment. The VM checks provisioned identities and channels,
enforces its own grants and disconnects its sessions. Creation, cgroup/namespace
policy, CPU/NUMA placement, termination, restart and update belong to deployment.
Only test scripts simulate those responsibilities here; no full deployer is
implemented. See [communication attachment](ros-communication-attachment.md)
and [model attachment](ros-model-attachment.md). Historical launcher-based
qualification below applies only to its recorded source cohorts, not to the
current attachment implementation.
The [complete operation reference](ros-operation-reference.md) specifies every
catalogue request/result and its authority, commit, delivery, retry and retirement
contract. The [robot provider procedures](ros-robot-provider-procedures.md) add
typed profiles, independent stop sequences, execution evidence and platform
activation requirements for device, control, media, bridge and component providers.
Their 22 imports now have concrete dispatchers, CLI bindings and cached debugger
records; [robot bindings](ros-robot-bindings.md) specifies their supported schemas
and platform limits. A codec test alone does not qualify a provider.
The [platform extension contract](ros-platform-extensions.md) places vendor
adapters below ROSv2 and requires them to share common driver authority,
arbitration and retirement. Child-policy support does not register vendor ABIs.
The current catalogue has **162 registered imports and no unregistered catalogue
operations**. The registered set includes `abi`, `capability.open`,
`capability.close`, `capability.revoke`, and the 16 guard/clock/timer/wait/resource operations in
[`runtime/contract.h`](../../src/uwvm2/imported/ros/runtime/contract.h).
Eight self/foreign permission and module/VM queries and six registered cross-Wasm
message operations are specified in the [module policy section](../command-line/ros-scene.md#module-observation-and-registered-messages).
Their [qualification record](ros-module-exchange-performance.md) separates
live debugger observation, erasure, unsupported replay and hardware profiling.
Three additional imports, `artifact.open/read/describe`, provide Linux sealed
immutable inputs with exact generation/SHA256 and explicit retained capacity.
Their concrete wire is specified in [scene CLI](../command-line/ros-scene.md);
raw digests/data do not use the proposed length-prefixed encoding below.
Eight volatile cache transaction/status/receipt imports are also registered;
their exact sequence, receipt and no-durability contract is in the same CLI
document. Six local lifecycle imports (`open/state/transitions/submit/take/respond`)
also have a concrete fixed-word contract in
[`managed_lifecycle_contract.h`](../../src/uwvm2/imported/ros/scene/managed_lifecycle_contract.h).
They implement a local logical state machine with deferred guest callbacks,
not a native ROS node or physical safe-stop controller. Additional native transports,
ROS 1/ROS 2 translation, device schemas, compressed media, native parameter
replication and durable output bindings require separate qualification. Five
canonical buffer operations are registered with finite data/workspace/lease
reservations and immutable SHA256 sealing; complete `artifact.publish` adds
one operation beyond the original 132-entry catalogue. Registration counts
now also include five model and two request carriers for the explicit local-model binding.
The carriers implement bounded submission/replay/cancellation and compound input
authority. The approved CPU model worker has real isolated inference and Wasm
qualification, with its precise scope in [worker deployment](ros-scene-model-worker.md).
Nine local-transform imports now implement bounded frame history, interpolation,
velocity, owned query waits and explicit history clearing. Their CLI/host core
checks have passed, together with 320 actual Wasm/CLI cases and three live DAP sessions.
Native TF subscription/broadcast is not selected implicitly by that binding.
Two private log/diagnostic carriers now have complete CLI and debugger wiring.
The corrected local-retirement cohort passed 111 Wasm/CLI cases and three live
DAP sessions, with both module modes and real imported-memory callbacks. The
same frozen VM passed 959 actual VM/DAP cases across its 59 registered provider
regressions. See [log qualification](ros-log-diagnostic-provider.md).
Seven private recording/replay, three receive-only sensor and four model-proposal
imports now have central host, CLI and cached debugger wiring. Their combined
VM qualification is in progress; the older 959-case receipt does not cover them.
Twenty-eight communication carriers now have VM and CLI routing for sealed
Linux worker pairs. Node, Topic and Service creation is asynchronous; native
receives use an explicit reservation followed by acknowledgement or release.
Their actual-VM qualification is recorded separately from component tests.
The current codecs are closed and do not establish generic ROSIDL support.
Twelve parameter carriers now route to bounded local stores attached to an exact
configured native node alias. Their CLI and cached debugger paths are connected;
actual isolated-node origin tests pass under Clang ASan/UBSan and GCC. Actual
Wasm carrier qualification is recorded separately. This binding does not
replicate into native parameter services; see [parameter contracts](ros-parameter-provider.md).
Seven action carriers now route to owned isolated worker providers. Actual native
deployment tests pass under Clang ASan/UBSan and GCC, including three schema
origins, goals, feedback, explicit cancellation sets, results, failed delivery and
independent cleanup. Six original carriers passed 41 actual Wasm/CLI cases;
the separate cancellation-selection cohort passed another 41 cases on its
own frozen VM, including all four memory layouts in both engines. This is
Linux worker evidence; other platform qualification remains outstanding.
Counts describe source registration, not whole-package portability or industrial
safety qualification. Device/control use finite scalar profiles and an independent
stop lane; media supports sealed raw buffers; bridge supports an explicitly directed
topic relay; components use isolated, role-bound message IPC. These bindings do not
establish arbitrary native ABI, codec, hardware or operating-system coverage.

Permanent self-revocation has now completed actual VM qualification: 60 dedicated
Wasm/CLI cases, three live DAP permission sessions, and 121 existing output and
publication regression cases. The same source closure passed native sanitizer,
GCC, thread-sanitizer and AArch64/ARM32/i686 core checks. The two isolated module
export modes use an explicitly recorded global-module-fragment fix. These checks
prove local fencing, exact retry and notification semantics; they do not prove
worker exit or physical stopping. The pinned cohort is documented in the
[qualification runner notes](../../tools/ros_scene/README.md).

The [legacy coverage contract](ros-legacy-coverage.md) inventories all 124
deprecated imports across eight modules. Its review added explicit permanent
revocation, receive reservation/acknowledgement/release, service endpoint
description, parameter existence/search and bounded cancellation-selection
contracts. Permanent revocation, six receive operations and service description
and parameter existence/search are now registered. Explicit owned-goal cancellation
sets are registered with per-candidate results; foreign-principal and native
time/wildcard selection remain unimplemented and prevent full legacy equivalence. The expanded
native feature and endpoint metadata records also require provider updates and
native differential qualification before migration can claim equivalence.

Legacy ROS interfaces are deprecated. Legacy ROS and WASIp1 receive bug fixes,
which may break faulty unreleased semantics; new robot capabilities belong in
ROSv2. No ROSv2 operation aliases an old function or inherits its authority.
The [26-domain capability contract](ros-robot-capabilities.md) adds ecosystem-wide
coverage obligations; the operation count below does not establish that those
obligations are implemented or qualified.
The major belongs to the module name. Functions use semantic names such as
`parameter.set`, `transform.lookup`, `model.submit` and `event.stream.poll`.
No operation suffix encodes an internal implementation revision.

## Evidence and the unit of coverage

The [ecosystem review](ros-ecosystem-review.md) records limited reviews of
sensor UDP input, SocketCAN commands, vehicle links, ROS transport negotiation,
rosbag storage, maps, models, media, bridges, plugins and control loops.
The catalogue also records the exact locally reviewed Jazzy parameter,
tf2 and lifecycle declarations, with file and archive hashes. These sources
motivate requirements; they are not execution tests of those packages.

The corpus currently contains package names from different distributions and
release snapshots. A package-name count is neither a public API count nor the
number of portable programs. Native ABI assumptions, plugins, build tools,
runtime dependencies and physical devices must be qualified separately.
Unreviewed interfaces remain gaps even if a similarly named catalogue entry
exists. All proposed operations require implementation, denial tests and native
semantic comparison before being advertised as supported.

## Admission and execution guarantees

Binding exposes ABI discovery and the caller's permission report by default.
The operator controls those queries individually. All other imports start hidden.
Every effect requires
the exact function exposure, caller generation, resource grant, operation,
purpose, provider binding and any secondary capability described below.
`capability.open` resolves an existing alias; it never grants or upgrades it.
Different environments remain separate even when aliases have the same text.

Stable authority has no implicit request-rate throttle. Finite resource
capacity is a distinct admission condition: a stream, recording, inference
or control cycle reserves the capacity required by its accepted contract.
Optional calls/bytes/sampling limits must explicitly state
the resulting rejection, loss or timing semantics. A value of zero is zero;
an omitted optional rate is unlimited within the granted scope. Provider and
hardware failure remain possible under stable authority.

A trusted policy change chooses `drain`, `cancel` or a provider-defined
`safe_stop` transition for already admitted work. It cannot silently change an
accepted operation's QoS, partial-delivery rules or reserved result size.
Revocation prevents new effects, wakes waiters and prevents unpublished results
from becoming newly readable. It does not assert reversal of a packet, actuator
command or remote operation that already happened. Final accounting and a
sanitized terminal receipt can remain available under a separately retained
inspection grant; revocation never implies access to private payloads.

Authority management is trusted configuration, not a guest import. A proposed
provider descriptor has immutable identity/digest, supported purposes and
schemas, direction, deployment-specific endpoint identities, maximum capacities,
failure modes, cancellation support and the set of externally visible effects.
Unknown fields or unsupported guarantees reject deployment. A purpose string
alone cannot activate a provider. Startup must validate the complete dependency
and authority closure before opening native resources.

## Wire records and ownership

Existing local imports keep their implemented signatures and word layouts.
Proposed effect imports use the same checked seven-argument carrier:

```text
(capability:u64, memory:u32, request:u64, request_bytes:u64,
 output:u64, output_capacity:u64, output_length:u64) -> result:u32
```

The catalogue specifies ordered typed request/result fields. The proposed
record encoding uses little-endian `u64`, two's-complement `i64`, IEEE binary64
`f64`, `bool` encoded as `u64` 0/1 and union tags encoded as `u64` using explicit
catalogue tag numbers. JSON key order has no wire meaning. Closed selectors
typed as text remain text and are validated against their declared enum or
immutable profile schema. A `bytes` or
UTF-8 `text` value is a `u64` byte length followed by exactly those bytes. An
array is a `u64` element count followed by encoded elements; an optional value
is a `u64` presence flag followed by the value only when present. Records are
ordered concatenation without padding, native pointers or embedded guest
addresses. Trailing bytes, unknown tags, noncanonical booleans, invalid UTF-8,
overflow and excessive nesting are invalid. Every variable schema has trusted
element/depth/byte bounds; a length field never authorizes allocation by itself.
Each registered canonical family has its own decoder. The fixed-word families
retain their concrete layouts. Defining the remaining variable records does not
make the fixed-word dispatcher or another family accept them.

The host copies and validates the request, validates every output span, admits
the complete effect and reserves result capacity before starting. Native
callbacks cannot retain guest addresses or execute guest callbacks. Read/take
methods reserve one immutable result; an invalid or undersized output must not
consume it. Metadata and content commit together. Unknown metadata has an
explicit absent value; providers cannot invent timestamps, publisher identity,
delivery acknowledgements or causal ordering.

The implemented cache transfer optimization uses a private per-call
`byte_buffer`: up to 512 bytes use stack storage; larger allowed transfers use
owned heap storage without redundant zero initialization. Host append validates
the spans, exact authority, transfer bound, optional rate allowance and receipt
capacity before allocating or copying. Parsing occurs only after a complete
checked read succeeds. Published-cache reads validate ownership, publication,
original requested transfer and offset under the registry lock, then allocate
and initialize exactly the available extent before exposing a view. These
buffers are temporary workspace, not persistent objects or extra permissions;
their memory cost is separate from retained content and replay reservations.

Scheduling, immutable-artifact and cache actions can use indexed authorization.
Each bit is compiled from the deployed exact spelling and each operation enum
is bounds-checked before shifting. The indexed path performs the same caller,
exposure, action, revocation and quota checks as string admission. It does not
interpret a guest bitmask, grant prefix access or bypass provider purpose and
binding validation. Lifecycle operations continue to use exact string admission.
Optional quota preflight observes available allowance without refreshing the
window or changing counters; final admission repeats checks and charges atomically.

The LLVM host wrapper uses a native control-register RAII guard for 26 selected
callbacks: 16 scheduling/resource entries, seven cache output entries and the
three artifact open/read/describe entries. The separate complete-object
`artifact.publish` entry keeps its full outer FP guard. The selected
guard starts before identity lookup or caller enrollment and ends after the
result is stored, covering success, allocation failure and application-installed
`std::new_handler` callbacks. Only architectures supported by that register
guard declare these wrappers as preserving Wasm floating-point controls;
unsupported architectures and interpreter-only builds retain the complete
outer guard. Lifecycle, model, transform, log and diagnostic calls retain the
complete outer guard as well. Arbitrary imported-memory callbacks still use their nested
runtime guard. This optimization restores controls; it does not infer safety
merely from integer code, ordinary byte-copy tests or an allocator that throws
without running a failure hook. Changes to supported controls, native callbacks
or backend dispatch require renewed qualification. CPU timing or successful
profiler collection alone is not evidence of this safety property.

Handles are monotonic registry identities tagged internally with environment,
grant incarnation, type and provider generation. Passing a handle confers no
authority. Resource sharing requires an explicit same-environment grant or a
trusted transfer operation with a new recipient-bound handle; there is no raw
descriptor or pointer export. `resource.close` releases a reference, while a
family's shutdown/cancel/abort operations determine external effects.
Its present implementation accepts local scheduling resources, sealed-input
handles and local lifecycle handles. Closing a sealed-input handle retains the
approved snapshot; closing a lifecycle handle retains its state, pending work
and retry history. Volatile output objects require `artifact.abort/remove`.
Extending close to other providers requires a qualified cleanup policy: active irreversible work
returns busy unless an explicit drain/stop/cancel contract permits closing.
Dropping a handle must never hide an unresolved effect or release cleanup debt.
Observation through `resource.observe` is a permission used by wait attachment,
not a function that broadens source access.

## Submission, retry and cancellation

A side-effecting submission carries an endpoint-scoped `request_key` and the
expected resource generation. The key contains the immutable endpoint/session
identity and a monotonic sequence. A bounded ledger retains the canonical
request digest, admitted capacities, operation, provider generation and receipt.
Repeating a live key with the same request returns the same receipt; changing
the request produces a conflict. A sequence below the ledger's accepted
watermark with no retained entry is stale and must never execute again.
Ledger exhaustion rejects before effects. Closing/reopening a capability must
not reset endpoint sequence or quota state.

Admission publishes an owned request handle successfully before making the
job runnable. Failed delivery rolls back the unpublished resource. The job
state machine is `admitted → queued → running → succeeded|failed|cancelled|
outcome_unknown`, with `cancel_requested` as an orthogonal flag. A cancellation
acknowledgement only confirms local receipt unless the provider reports actual
termination. Terminal results distinguish committed effect, observed remote
effect, partial effect and unknown outcome. A timeout is not a proof of failure
and never authorizes automatic non-idempotent resubmission.

The ledger does not make a remote provider exactly-once. Native ROS services
usually do not expose remote cancellation or durable deduplication. A lost
response may therefore yield `outcome_unknown`. Native restart changes provider
generation; replay is allowed only with a provider's separately qualified
durable deduplication protocol. The generic adapter must not fabricate it.
Result retention, unresolved receipts and cleanup debt consume explicit capacity.

## Contexts, nodes, names and types

`context.open` selects a deployment binding for ROS domain/master, middleware,
security identity and transport/discovery policy. `node.open` selects a fixed
node profile with namespace, basename, remappings, parameter overrides and
logging identity. The guest can choose only constrained profile values; neither
operation accepts arbitrary endpoint URLs, environment inheritance or shell
arguments. `context.shutdown` cancels that owned context and does not exit the
process. Closing one shared handle cannot destroy another environment's node.

Name expansion and remapping precede endpoint and type authorization. The
resolved name is recorded in metadata. Graph filters apply after resolution.
`type.describe` exposes only approved, bounded, immutable schema descriptors and
their dependency identities. A type name cannot select a native library.
ROS1 wire serialization, ROS2 CDR variants and portable canonical records are
distinct negotiated encodings. Unsupported encodings fail explicitly.

Guest-side templates, message containers, callback groups, executor selection,
allocators and pure algorithms remain guest library responsibilities. The VM
implements safe readiness and ownership, not one host import per C++ method.
An optional deployment execution profile may reserve worker capacity, scheduling
class and watchdog behavior. It cannot enable arbitrary native threads or
callbacks. Hard realtime remains a separately measured claim.

## Topics, QoS and shared buffers

`topic.open` binds a node, exact resolved topic/type and one direction. Publishing
and receiving are independent grants. `topic.publish` reports local acceptance
and an owned receipt; native acknowledgement, when supported, is a separate
receipt state. `topic.take` atomically delivers one body plus message metadata.
Loaned/native zero-copy support cannot be advertised by a copy implementation.

`qos.describe` reports requested, effective and supported settings with explicit
unknown values. `qos.liveliness.assert` and `qos.acknowledgements.wait` apply only to
the granted endpoint and require corresponding native support. Reliability,
durability, history/depth, deadline, lifespan, liveliness/lease, ordering and
namespace conventions are profile fields. A provider cannot silently replace an
unsupported reliable contract with best effort. Incompatible QoS, deadline,
liveliness, message loss and match changes enter an authorized event stream.

`buffer.create/write/seal/read` provide bounded owned data for large images,
point clouds and tensors. Sealing makes the input immutable; submission pins it
until completion. Output publication validates shape, encoding and byte extent
before exposing a readable handle. Tensor products and strides use checked
arithmetic; no CUDA/native address crosses the ABI. A consumer needs both its
operation grant and permission for every referenced buffer. Trusted buffers
carry provenance; guest-written buffers are explicitly untrusted-origin data.

## Services and actions

`service.open` fixes client/server role, node, service/type and request/response
bounds. `service.submit` returns an owned request. `service.take` distinguishes
server requests from client responses and preserves native correlation.
`service.respond` may answer an owned received request once. `service.abandon`
stops local waiting without claiming remote cancellation. Availability is an
observation, not a guarantee that the next call will succeed. Server response
capacity is reserved before accepting a request that cannot otherwise be served.

An action endpoint has separate authority for goal submission, goal acceptance,
execution, feedback, result, cancellation proposal and cancellation response.
`action.respond` uses a tagged transition (`accept`, `reject`, `execute`,
`feedback`, `succeed`, `abort`, `cancelled`, `cancel_response`) whose exact grant
is checked independently. A broad endpoint-open grant enables none of these.
Goal UUID, native stamp, owner and state are preserved; UUID alone is not a
capability. Duplicate UUID handling follows the bound native protocol.
Cancel acceptance and terminal cancellation are different transitions.
Preemption must use explicit authorized transitions and cannot silently cancel
another owner's goal. Status/feedback/result queue and retention are bounded.

## Parameters and lifecycle

Parameters require their own purpose and scope; a service grant must not
implicitly bypass a parameter grant. `parameter.open` fixes the node, approved
key set/prefix policy, value algebra, descriptor constraints and remote/local
role. ROS2 has not-set, boolean, signed integer, floating value, string and typed
arrays. ROS1 XML-RPC values additionally have bounded nested arrays/structs and
different scalar rules. Conversion must be declared and loss-checked; the ROS2
not-set state is not silently treated as a ROS1 null value.

`parameter.declare/undeclare`, `get`, `describe`, `list`, `set` and
`set.atomic` are separate actions. `set` returns a per-item decision; `set.atomic`
commits every item or none. Atomic requests include an expected local revision
when the provider can enforce it. A remote ROS provider without revision
compare-and-swap advertises that absence; it must not claim global atomicity or
serializability across unrelated nodes. Read-only, dynamic-type and numeric
range/step constraints are checked before callbacks.

Guest validation is a bounded queue delivered by `parameter.take`, answered
through `parameter.respond` before a steady deadline. Pre-set transformations
are validated again against the same key/value grants. Validation cannot mutate
the pending transaction recursively. All validators must accept before commit;
post-set notifications are observational. Denial, timeout, cancellation or
revocation commits nothing. Events contain only committed changes and have
explicit source/revision/presence metadata. ROS1 cache invalidation must be
observed; a stale cache is never labelled a fresh remote read.

`lifecycle.open` selects an approved transition graph and client/server role.
`lifecycle.transitions` exposes only authorized transitions. `lifecycle.submit`
requests one transition with expected state generation; `lifecycle.take` and
`respond` deliver the bounded guest transition callback. A response names the
same request and expected state. Wrong, late, repeated or revoked responses do
not commit. Configure/activate/deactivate/cleanup/shutdown/error processing
follow the declared graph, including failures. Lifecycle publishers check the
active state at admission. Deactivation drains/cancels according to profile;
it cannot imply an arbitrary process or device privilege.

The implemented `local-lifecycle` binding is a narrower, concrete profile. One
deployment alias owns one logical state machine initially unconfigured, with
explicit finite handle/deadline bounds, transition bits and client/server roles.
The role is an additional restriction: every import still needs its own action
grant. There is one pending callback and bounded last-submission/last-response
recovery storage. Close/reopen does not reset state or history. Exact retries
recover delivered effects; callback responses require the same request and
generation and a successfully delivered callback. Delivery reserves under the
provider lock, copies without that lock, then revalidates before consumption.
Reentrant closure, revocation, cancellation or deadline expiry cannot make a
stale callback response commit.

Public transitions and callback outcome numbers retain the ROS lifecycle
graph, including error-processing callbacks. Deadline zero is invalid; finite
expiry is enforced when provider methods execute, without a background timer.
Expiration finalizes pending logical work and prevents late responses.
Revocation cancels pending work and denies access; idle state may remain in
retired diagnostic snapshots. Debug inspection copies cached state and never
advances an expired transition. This profile does not create native nodes,
drain native publishers, stop a process, command hardware or integrate device
active-state admission. Those effects, independent watchdog deadlines and
physical safety guarantees remain separate implementation obligations.

## TF, clocks, graph and events

`transform.open` admits a bounded transform buffer or approved native provider
with frame-set and cache bounds. `transform.publish` accepts static/dynamic
batches with finite values, normalized rotation, authoritative source and
clock epoch. `lookup`/`available` support source/target time and optional fixed
frame; `velocity` additionally binds reference frame/point and averaging window.
Every traversed frame must satisfy the grant, including the fixed frame and
intermediate path. A provider unable to enforce hidden-frame isolation must
use an isolated permitted subgraph or reject the query. It cannot merely filter
the final answer after reading forbidden graph state.

Asynchronous `transform.submit/take/cancel` replaces native callbacks. Requests
bind clock epoch and bounded deadline. Cache clear, ROS-time activation or
backward jump invalidates affected requests and dynamic history according to
the approved policy. Static transforms have separate replacement authority.
The existing `clock.*` implementation supplies local time primitives; connecting
a live `/clock` source is a new provider binding with its own subscription
grant, not an effect of creating a ROS clock.

`graph.snapshot` returns a bounded, revisioned snapshot and continuation cursor
bound to the exact filter and environment. `graph.watch` creates an event source.
Counts, node/endpoint names, identities and type information obey the same
scope; suppressed entries cannot leak through total counts. Pagination expiry
is explicit. Discovery grants do not grant communication or transport access.

`event.stream.open/poll/close` defines the shared readiness envelope. The
currently connected production sources are subscriptions, actions and graph
watches. Provider-status, service, parameter, sensor, model, lifecycle and
control adapters require separate integration. Opening requires observation authority
on each source and a field-visibility policy. Poll commits complete event
records, with monotonic stream sequence, source generation, payload schema,
loss markers and timestamps. `wait.add` for provider sources must revalidate
that source grant just as local resources do. A readiness bit never grants
permission to take source data. Provider queues share one bounded lifetime
model with cancellation, retirement and wakeup; no new unbounded callback bus.

## Sensor, device and media workflows

`sensor.open/take/describe` selects an ingress-only binding. UDP/TCP/serial/CAN
configuration, peer validation, timestamp origin, frame validation, buffering
and reconnect remain provider-owned. The guest cannot send through an ingress
handle or replace a profile with a listener/broadcast URL. Negotiated ROS ports
are allowed only under the trusted ROS transport domain/peer policy; purpose
restriction does not mean every ROS connection has one compile-time port.
The current `bounded-sensor` binding accepts only a trusted startup descriptor
for a read-only nonblocking pipe or raw terminal and the compiled `octets`
schema. It does not implement those network/CAN transports. The definition
binds exact source and clock incarnations, units/frame, sealed-buffer profile
and overflow behavior. Static checking does not probe the descriptor; activation
checks and duplicates it. `sensor.take` checks independent `data.export` and
capacity for the complete maximum reply before native reads. Its local capture
sequence and optional native sequence are separate wire fields.

`device.open/submit/result/stop/describe` admits an exact actuator/control
protocol. Check bus/interface identity, CAN ID/mask/frame type, MAVLink
system/component/message/command identifiers, schema, units, frame, freshness,
position/velocity/effort limits and command expiry as applicable. Scheduling a
trajectory reserves the complete execution envelope. Partial command admission
is forbidden unless the named device protocol explicitly reports safe partial
effects. `device.stop` needs a stop grant; deployment's own watchdog may stop a
device independently of guest authority. An application cannot use revocation
to disable the trusted watchdog.

`control.open/sample/submit/result/stop` describes a continuous read-update-write
cycle with sequence/deadline, missed-cycle and watchdog semantics. The fixed
profile defines whether a missed or revoked phase holds, brakes or invokes a
certified stop. A rate limit cannot skip one write phase silently. A simulated
provider must label its observations and cannot establish physical safety.

The immutable control profile must identify the controller owner, priority,
lease expiry, authority epoch, controller-lease epoch and provider/device boot
generation separately. Ownership changes fence already queued commands. The
final driver/receiver checks these values at execution admission, not only at
VM enqueue time. `control.open` can acquire a finite lease and `resource.close`
can request its defined safe release; reopening or reconnecting never renews
an old lease implicitly. Guest-controlled lease renewal is not currently
designed as a registered operation and would need its own explicit authority.

Stop/watchdog configuration reserves its own queue, execution capacity and
deadline, including a documented duplicate-stop coalescing rule. Ordinary
rate limiting, full inference queues and unknown-outcome ledger entries
cannot exhaust that reservation. Deployment must provide an independent
watchdog where stopping must survive guest stalls, VM failure or link loss.
A stop request, a driver acknowledgement and independent evidence of stopped
equipment are distinct observations. Safety latch rearming is trusted
deployment management; `lifecycle.activate`, capability reopening and bridge
reconnection cannot clear a latch. Rearming never itself commands motion.

An unknown result cannot be converted into failure and automatically retried.
Outstanding effects and cleanup debt survive logical close/restart in a
qualified persistent provider. Recovery requires authenticated durable records
or trusted reconstruction from device state, and a new provider generation
rejects old fencing. Stop and recovery capacity remain independent of the
ordinary request ledger. The current local registries do not implement these
physical or persistent guarantees.

`media.open/submit/take/close` binds an approved codec and stream profile. Ingress,
encoded egress, listener provisioning and viewer session are distinct grants.
Frame count, resolution, decoded expansion, queue duration, codec workspace and
viewer concurrency are reserved. Camera read permission alone cannot create an
HTTP server. Stopping one viewer requires ownership of that viewer session.

## Artifacts, recording and replay

`artifact.open/read/describe` now selects an explicitly granted sealed input
by exact deployment generation and digest. It provides opaque bytes, without
claiming map/model bundle validation. The following durable/schema-aware output requirements remain
proposed. The implemented volatile cache subset is documented separately: `begin/append/commit/abort/remove` operate on purpose-bound output
stores. An append carries the exact expected offset and request key; it cannot
seek, extend sparsely or duplicate bytes after a lost response. Commit requires
a trusted purpose-specific validator, exact content digest and expected prior
generation. Publication and crash durability are distinct receipt fields.
Map/calibration publication is a transaction over the entire dependency bundle,
including YAML/image relationships; a generic opaque-file commit does not
qualify that workflow.

The existing backend establishes local logical accounting and sealed input
behavior but currently refuses nonempty-store restart. Provider integration
still needs an authenticated recovery manifest, reconciliation of open/staged/
published objects, exact principal identity and bounded replayable append
receipts. Failed deletion retains charge. Backing-store identity shares quotas
across aliases. Physical hard disk bounds require an enforced project/volume
quota and journal reserve; free-space checks alone cannot provide them.

`recording.open/append/checkpoint/stop` adds a bounded approved container/storage
profile and schema catalogue. Total quota covers segments, metadata, indexes,
journals/WAL, staging and compression workspace. Rotation is not a total bound.
Checkpoint returns the committed segment/digest set and durability state.
`replay.open/read/seek` consumes immutable recordings with separately granted
clock authority if it drives simulation time. Reading a record does not publish
it to a topic or command device; replay forwarding needs destination grants.

The allocation-free closed MCAP segment codec now has native ROS1/ROS2
reader/writer interoperability and bounded malformed-input checks, described in
[recording portability](ros-recording-portability.md). The bounded store now
owns schemas, reserves segments/indexes/retry storage, retains immutable replay
pins and advances the cursor only after successful delivery. External import
requires whole-object SHA256 and bounded indexing before publication. These
checks cover the private store and container components. The seven recording/replay
imports are now registered with explicit authority, finite storage reservations
and guest bindings; qualification of their actual VM boundary remains in progress.
This does not add disk durability or arbitrary native bag import.

## Models and proposals

`model.open/submit/take/cancel/describe` covers language, vision and world models
through registered task schemas: generation, embeddings, structured extraction,
image/video inference, state encoding and bounded rollouts. It selects only a
deployment-bound model/provider identity and admitted parameters. Remote
credentials and accelerator/device identity remain host-owned. Input buffers
are sealed and held for the job. Output bounds cover tokens, encoded/decoded
bytes, tensor elements, frames, rollout horizon/branches, time, concurrency and
provider spending units. A reservation must cover the maximum accepted request;
missing settlement data cannot authorize an unlimited continuation.

Data egress is independently checked against destination, purpose and trusted
input provenance. Guest-created bytes cannot claim sensor provenance. Once a
guest can read sensitive content and submit arbitrary bytes to a remote model,
the VM cannot infer that copied/reencoded bytes are unrelated to that content.
Deployments requiring stronger information-flow separation must isolate the
data-reading and remote-submitting modules or restrict egress to host-mediated
approved transforms. Object provenance is not general-purpose taint tracking.

Model tool calls are immutable proposals. `proposal.capture` obtains actual
ordered source observations and sealed buffer references under independent
source/buffer rights. After the ordinary model submission and complete output
delivery, `proposal.publish` authenticates the full terminal envelope against
that original capture and model request. Neither action implies the other or
`proposal.read`. Exact keys and finite retained capture/workspace capacities
make lost output recoverable without repeating publication.
`proposal.read` exposes a validated
tool schema and arguments; `proposal.accept` requires a separate arbiter grant
and the current target operation/resource grant. Acceptance binds request key,
expiry, source observations, frame and physical limits. Model text cannot set
policy or approve its own proposal. A model session has no capability to call
the arbiter automatically. World-state and rollout outputs remain predictions,
with schema-defined uncertainty, never authoritative sensor updates.
Training, adapter installation and persistent shared-model changes are excluded
from ordinary inference and require trusted deployment management.

The [proposal guest facade](ros-scene-proposal-storage.md) has a qualified
capture → actual model completion → publication → read → inert acceptance
flow. Eight new guest facade groups and fourteen original dispatcher groups
passed Clang ASan/UBSan, GCC and TSan in frozen source cohorts. The actual target
adapter currently covers only a nonphysical local lifecycle operation; it does
not execute that operation. The four proposal imports now have VM/CLI/sensor/debug
wiring, with combined execution qualification still pending. Device execution and
industrial safety qualification remain separate obligations.

Target classification follows effects and physical resource scope, including
commands routed through topics, services, actions, parameters and lifecycle
transitions. Their immutable profiles must bind the same physical constraints
and controller fencing as direct device/control operations. Proposal approval
is an inert record. Before any effect, the target provider atomically rechecks
current arbiter/target authority, target generation, owner fencing, argument
and physical limits, observations and expiry. A receipt is not transferable
authority and an accepted proposal is not a completed physical command.

Observation contracts distinguish source incarnation from sample sequence and
calibration/transform generations. Schema policy specifies units, invalid or
unknown estimates, uncertainty, timestamp origin and frame relationships.
Each tool explicitly chooses latest-required or bounded-age snapshots; a new
sample is not necessarily a new source incarnation. The isolated proposal
core checks both policies using host-only metadata. Bounded-age reuse still
requires unchanged source authority, clock identity/epoch, frame and calibration/
transform generations. A frame change advances transform generation and a
clock-identity change advances source incarnation; changing back cannot revive
an old proposal. Actual buffer leases, source authority and canonical wire
provenance have core qualification for the closed receive-only source; this
does not qualify arbitrary sensor/ROS adapters or physical target execution.

## Bridges and externally provisioned drivers

`bridge.create/start/stop/describe` binds source and destination capabilities,
directions, type identities, approved conversion, QoS contract, loop identity
and failure policy. Both endpoint checks occur after remapping. Reverse flow
requires a separate grant. Service/action correlation and cancel propagation
must state exactly what the other protocol supports. ROS1/ROS2 conversion loss,
QoS degradation, clock mismatch, source disappearance, loop suppression and
restart are observable. The bridge cannot forward a model proposal into a
device action without the separate arbiter and device checks.

The former `component.open/start/stop/describe` imports are removed. The current
[driver attachment contract](ros-driver-attachment.md) exposes only
`driver.connect/submit/result/describe/disconnect`. The external deployment owns
native image selection, process creation, confinement, replacement and recovery.
The VM accepts an inherited authenticated channel for one exact module, resource
and compiled semantic schema, subject to independent explicit authorization.
Disconnect releases only that channel. It never terminates the shared driver.
No environment, package search path or configuration rewrite grants recursive
filesystem access. Failed or uncertain physical operations are never replayed
automatically; a replacement requires a fresh deployment-approved incarnation.

## Required verification before provider registration

Each family must first pass its own native provider qualification. Common
acceptance covers cross-environment/kind/generation misuse, undeclared imports,
denial before effects, malformed/oversized records, output-copy failure,
concurrent consumers, cancellation/revocation races, bounded metadata, cleanup
debt and request replay. Every claimed native semantic must have a differential
fixture against the official bound ROS distribution/provider. Drivers need
device or simulator evidence explicitly labelled by scope.

All builds, tests and Intel VTune collection use the existing shared 64 GiB,
zero-swap cgroup through `tools/ros_scene/in_existing_cgroup.py`; no alternative
resource gate is a valid substitute. Performance qualification separately
records native provider, C++ broker and actual Wasm paths under equivalent
loads, compiler settings, payloads, QoS, batching and concurrency. Hardware
counters, hotspot/microarchitecture analysis and source mapping identify costs;
successful collection is not evidence of near-native performance. Report
steady throughput, latency distribution, admission failures, queue loss,
memory/storage and cancellation latency. Do not conceal cost by omitting
security checks, batching only one side or comparing different semantics.

Every public provider declaration requires Doxygen `@brief` and `@details`
covering ordered fields, required primary/secondary grants, reservation,
commit point, cancellation, revocation, unknown outcome, retry key, ownership,
resource closure, nonconsuming copy failure and sanitized inspection. The
catalogue supplies review obligations; generated help/debug labels must only
advertise the operations actually registered in that build.
