# Purpose-bound native ROS communication

The independent `scene::communication` provider implements typed context, node,
message-description, topic, effective-QoS, service, filtered graph and readiness
event operations through the communication host and canonical dispatchers.
Its plans are trusted deployment objects, not guest
JSON or native ROS arguments. The old WASIp1 and ROS v1 providers are untouched.
The current native binding includes complete `std_msgs/msg/String`,
`std_msgs/msg/Bool`, and bidirectional `std_srvs/srv/SetBool` codecs. Explicit
worker builds can also select [bounded generated topic adapters](ros-bounded-native-topics.md)
with native shape and type-hash checks. Generic service/action adapters and
authenticated remote transport remain separate qualification obligations.
The [external attachment](ros-communication-attachment.md) connects provisioned
providers using authenticated control and directional pipes. Deployment owns
processes and sandboxing; each actual VM/provider combination needs its own evidence.
The separate [action provider](ros-action-provider.md) now has a real
`LookupTransform` binding and its own qualification scope and outstanding limits.

## Authority and configuration

`registry` starts with zero capacities and no profiles. Trusted deployment must
install explicit context/node/type/topic/service plans before any resource opens;
the first open freezes configuration. An owner is the broker-supplied environment,
grant and grant generation triple. It is not guest authentication. A caller must
hold the scene authority lease through every native effect. Profiles have exact
identities and deployment-supplied SHA256 provenance; this core does not attest a
foreign schema merely because its caller supplied a digest.

Context plans bind a domain and transport. Node plans bind a context profile and
an exact name/namespace. Endpoints bind their node, resolved name, direction,
types, effective QoS, finite byte/correlation capacities and maximum steady-clock
operation deadline. A node/context from another environment is refused. Unknown
native type bindings are refused before endpoint creation. A message type's
revocation fences endpoints dependent on it, in addition to direct endpoint,
node or context revocation. No grant implies ambient ROS discovery permissions.

Receive queues and service correlation slots are reserved before native creation.
Payload buffers, current/attempt replay buffers and native object counts have
separate finite reservations. Optional traffic attenuation belongs to broker
admission; it is not inferred from queue depth. A full queue never silently gains
unbounded storage. Topic overflow is an explicit reject/drop-oldest plan; a
borrowed front record cannot be overwritten. Service slots never drop an older
correlation to admit a newer request.

## Native transport and implicit interfaces

The current Linux profile requires an explicit permit for **unauthenticated
loopback peers**. Domain numbers do not authenticate participants. Installation
must precede initialized ROS contexts and DDS participants in a dedicated worker.
Conflicting security, XML, peer, discovery and middleware environment settings
are refused, not downgraded. The worker checks its process identity on use.

The provider parses a compile-time FastDDS XML profile directly and checks its
result. It disables builtin transports, configures only UDPv4 on `127.0.0.1`,
removes multicast discovery locators and uses loopback initial peers. It sets
`ROS_AUTOMATIC_DISCOVERY_RANGE=SYSTEM_DEFAULT` so RMW preserves that explicit
configuration. Jazzy's `LOCALHOST` branch otherwise adds an unrestricted UDP
transport and shared-memory transport after parsing XML. Linux memfd XML paths
are not used: FastDDS calls `realpath`, which rejects the deleted memfd path.
Neither a profile nor these environment checks constitute a syscall sandbox.
Actual traces still show dynamic-library startup probes for local LTTng/nscd
sockets and network-interface enumeration. The isolated worker must account for
these paths before this profile is suitable for deployment.

The backend constructs only NodeBase, NodeGraph, NodeTimers, NodeTopics and
NodeServices interfaces. It does not construct full `rclcpp::Node`: its mandatory
TimeSource subscribes to `/parameter_events` even when parameter services and the
parameter-event publisher are disabled. The minimal node exposes no implicit
parameter, type-description, logger, rosout or ROS-time endpoint. Actual graph
inspection tests this property, rather than inferring it from NodeOptions flags.

## Topic and QoS operations

Publish validates the complete approved body, identity, encoding, owner,
deadline and output-recovery capacity before the native call. Each endpoint uses
an exact last-request key: the endpoint session and a sequence beginning at one.
Replaying identical input returns the retained receipt without another native
call. Reusing a key with different input conflicts. A possibly effective native
exception remains `unknown`; a new key or close cannot erase it.

A subscription copies the complete body and actual RMW metadata into a bounded
queue. Unavailable source/receive timestamps or publisher identity remain absent;
queue sequence/loss values describe the local queue only. Take reserves a record,
releases the queue mutex while copying, and consumes only after successful copy.
A thrown/failed sink preserves it. Revocation during the copy fails closed.

Effective QoS is read back from the actual native endpoint and checked against
the approved profile. Publisher GID is actual middleware data. Liveliness uses
its own exact-key operation tag in the publication ledger. Acknowledgement wait
requires reliable QoS and reports an observed transport acknowledgement, never
application completion or a physical safe state.

## Services and recovery

`service_plan` binds separate request and response schemas and exactly one client
or server role. `service_mutate` submits a request or answers a delivered server
correlation; wrong direction, schema, owner and stale keys fail before effects.
Each native client request has a bounded slot reserved before `async_send_request`.
A reply retains that slot until a complete checked take. Native request IDs and
server request headers stay private. A server uses deferred native responses and
cannot answer a request whose guest delivery failed.

Client and server slots preserve data across failed output delivery. Respond is
once per delivered correlation; its exact replay returns a receipt. A rejected
pre-effect submit does not consume its key or overwrite the previous recovery
body. A native exception after a possible effect becomes a latched unknown
outcome. Context shutdown has a keyed receipt too; an exact retry remains valid
after admission has been fenced.

Client `abandon` removes local interest and pending native response bookkeeping.
It does **not** cancel remote execution. Its unknown remote outcome latches
recovery debt, which blocks new mutations and close/reopen. Revocation similarly
preserves debt when pending correlations exist. The current provider has no
trusted reconciliation or durable restart protocol that can clear this latch;
manual restarting is not a supported safety recovery mechanism. A production
binding needs qualified durable receipts or trusted remote-state reconstruction.
Closing a context, cancelling an executor or revoking a grant is not a physical
stop or an independently powered watchdog action.

Native middleware may allocate before a callback, and native calls can block.
Finite callback pools and post-deadline checks do not bound those allocations or
provide hard realtime deadlines. A resource-limited worker, forced authenticated
DDS policy for nonlocal deployment and independent actuator safety mechanisms
remain required integration work. No industrial certification or full ROS
portability is asserted.

## Reproducible qualification

All compiler, test and native runs use the existing 64 GiB, zero-swap cgroup, on
CPUs 4–7:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 4-7 \
  /usr/bin/python3 tools/ros_scene/communication_checks.py --mode all \
  --ros-root ../uwvm2-ros-sdk/.tmp/ros-scene-security/communication-ros-sysroot \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/NEW-OUTPUT-DIRECTORY
```

The runner freezes provider/test/allocator sources, hashes copied native ROS
headers and libraries, retains command output and cgroup events, and refuses an
existing output directory. Its native test exchanges real data with an independent
ROS2 context, inspects actual graph/QoS/GID, exercises real service requests in
both directions, checks replay does not duplicate native effects, and checks
failed take, saturation, deferred response and local-abandon semantics. Unit mock
backends only qualify the core contract; they are not native ROS evidence.

`communication-loopback-minimal-node-second` qualified minimal-node topics and
records a network trace. `communication-service-recovery` passed nine commands,
including real bidirectional services; later recovery-debt changes require their
own receipt. Historical failed attempts are retained and are not counted as
passing qualification.

## Event observations

`event_streams.h` and `event_broker.h` provide fixed-capacity owned streams over
trusted `source` adapters. Central admission must resolve every origin
capability/resource pair, retain its generation, and hold its independent
`resource.observe` authority throughout open and poll. `communication_events.h`
retains the communication registry and revalidates the real subscription owner,
generation, context health and queue before observing readiness. Merely possessing
a numeric resource ID or a stream grant cannot observe a different source.

The current event profile exposes only explicitly selected readiness/terminal
booleans, never source payload. Public source IDs are one-based positions in the
immutable requested source list, avoiding collisions between independent provider
handle spaces. A poll produces nonblocking `readiness.snapshot` records under a
bounded steady deadline. It samples state; it does not promise every intervening
edge, and its zero loss count means no staged snapshot was evicted. Time-critical
control and independent watchdogs must not reinterpret this as an edge-complete
transport. Failed or thrown output delivery retains the exact staged batch and
sequence. Newer state remains eligible for the next poll after successful retry.
An unselected terminal/readiness field is masked before encoding.
Readiness is level-triggered: while a source stays ready it can appear in each
poll. This also preserves observability when a queue drains and refills between
polls. It does not manufacture an unseen edge history.

The three `event.stream.*` canonical records are declared in `event_contract.h`.
Decoding has a hard 64-source bound and validates complete/trailing bytes without
allocation. The central broker still must validate requested byte/item/workspace
reservations against the configured plan before allocating a stream. The current
owned-handle wrapper accepts already resolved/admitted bindings, not raw guest
capability values. These declarations are not import registration.

`communication-event-native/results.json` passed 16 commands: core and event
ASan/UBSan, GCC and TSan runs, actual ROS2 compilation/link/execution, and a traced
native run. The actual subscription transitions empty -> queued -> consumed and
context-shutdown denial were checked through the trusted event adapter. All 38
IPv4 bind/connect/send operations in that run targeted `127.0.0.1`. The cgroup
inode was 12409; `memory.events` had `max=331` both before and after, with zero
OOM/kill counters. This does not qualify a worker syscall sandbox or ROS action,
graph-watch, authenticated transport, VM-carrier or SDK integration at that
historical source revision.

## Filtered graph snapshots and watches

`communication_graph.h` observes the actual native graph through a counted
`node_lease`. A node plan must reserve `max_observers` explicitly; the default is
zero. An ordinary node close is busy while attached observers exist. Context
shutdown or revocation fences the attachment before teardown. `node_lease::live`
is an atomic liveness check, not a substitute for the broker's authority lease or
an atomic transaction spanning another provider.

The graph profile fixes the exact observable node names, entity kinds, optional
type visibility, result bytes/rows, page size, cursor lifetime and scan deadline.
Native `NodeGraphInterface` data is filtered and bounded before publication.
Discovery announcements are untrusted: names and types are not identities or
capabilities. A scan racing with native graph changes returns an error/again
rather than inventing a successful empty graph.

Pages pin a particular revision. Failed delivery retains the exact page, request
limit and cursor; another request cannot replace it. A graph watch refreshes a
separate current revision without rewriting pinned pages. Completing a final
page acknowledges only its pinned revision, so a newer unread revision stays
ready. The event adapter exposes readiness only, never names or payload. The
canonical `graph.snapshot` and `graph.watch` carriers are declared in
`communication_graph_contract.h`; an entity body is kind, node, endpoint and
type text under `uwvm/graph/Entity` / `scene-canonical`. These declarations do not
register VM imports.

Real publisher-close testing found that the native executor could retain QoS
event waitables and their underlying publisher handles until another entity
collection rebuild. Endpoint teardown now explicitly removes its waitables
through `NodeWaitables`, releases the endpoint and notifies the node/callback
group guards. The actual graph then removes the closed publisher without an
unrelated ROS event. This is native resource cleanup, not simulated graph state.

`communication-graph-close-notify-second/results.json` passed native compile,
link, execution and network trace. The test verifies an actual 3-row graph
becomes 2 rows after publisher close, revision increases, and watch readiness
clears after full snapshot delivery. The binary SHA256 is
`107baf31b73f4771bff41c665bfd05b892f7e3b7859c96b6e52cf3cbc7e7872a`.
All 33 traced IPv4 bind/connect/send operations used `127.0.0.1`; local startup
socket probes remain recorded. Cgroup `memory.events max=2517` was unchanged,
and OOM/kill counters were zero. This safety/semantic fixture uses `-O1 -g`;
it is not a native performance result.

`communication_contract.h` declares the 19 context/node/type/topic/QoS/service
canonical carriers. Its decoder is allocation-free and checks complete records.
RMW source/receive timestamp numbers are omitted unless the trusted deployment
supplies their real clock identity and epoch. `service.available` requires the
central broker to provide a trusted observation clock stamp at its documented
sampling boundary; neither a fabricated zero clock nor an unrelated clock is
valid. Opaque publisher GIDs remain observations, not peer authentication.

`communication-graph-contract-fenced/results.json` subsequently passed all 22
commands, including canonical graph records, event source-generation checks after
delivery, deadline-preserving replay and fork-child cleanup fencing. Its binary
SHA256 is `ce05c07ee3d64d6b9ac5753ac85920ddf974efdebc30a738c73cda7f312ec0fb`.
The shared cgroup's `memory.events max` increased from 2517 to 2747, with zero
OOM/kill counters. Later communication/action canonical helpers have separate
receipts; a historical successful binary does not qualify changed source hashes.

## Explicit discovery observer origins

Graph snapshot/watch requests carry context capability/resource and observer-node capability/resource independently. The observer node must match the immutable graph node profile and belong to that exact context. Opening or inferring a capability from an alias is prohibited. Existing node/context grants are revalidated without consuming their creation rates again; the actual counted `node_lease` uses the node plan's finite observer capacity. Graph visibility remains limited by the separately admitted discovery profile.

### Portable authority dispatchers

`communication_dispatch.h` binds the 20 context, node, type, topic, service and
QoS operations to `native-communication`. The independent
`communication_action_dispatch.h` binds six action operations to `native-action`;
`communication_graph_dispatch.h` binds graph snapshot/watch to `native-graph`;
`communication_event_dispatch.h` binds three event-stream operations to
`event-stream`. These headers provide integration APIs; their presence does not
register VM imports, CLI plans or debugger views. Catalogue registration remains
separate from provider and dispatcher qualification.

Trusted deployment calls `bind` before `prepare`; each alias receives a dispatcher
minted environment/grant/generation owner. `prepare` injects trusted factories or
source resolvers and performs no implicit ROS context/node creation. Hosts call
`preflight` and reserve a bounded owned carrier before invoking the decoder.
Invocation rejects hidden or missing permissions, an unprepared provider, wrong
purpose/binding, malformed carriers and mismatched profiles. Pure origin checks
never spend context/node creation rates. Full authority groups stay held during
bounded effects and output; callbacks revalidate the complete chain afterwards.
Native middleware remains responsible for its documented bounded-call behavior;
these dispatchers are not a process sandbox or a physical safety controller.

Action open names an existing node capability/handle and three ordered schema
capabilities (goal, feedback, result). The immutable plan fixes all five native QoS
policies, role, exact endpoint name, node/schema profiles, critical/feedback
reservations, retention and permitted transitions. An `action.respond` tag also
requires its exact secondary action such as `action.accept` or `action.succeed`.
That transition permission contributes a separate work unit to an explicitly
attenuated call budget, with no duplicated byte charge. Node, context and all
schema roles are validation-only and do not consume creation budgets again.
The current cancellation profile admits only one exact owned goal; broad cancel
sets are refused. UUIDs and remote announcement names never establish authority.

Later action calls carry the current caller incarnation and capability, followed
by the original node, context and three ordered schema origins. A queued call
cannot substitute the first opener's capability. `revalidate` verifies all
capabilities and actual resources without charging again, and returns a group
for one bounded nonblocking send; the supervisor must release it before waiting.
Exact schema roles remain distinct even when two use the same capability. The
retained provenance contains no endpoint/provider ownership edge, preventing a
queue-token-endpoint reference cycle during retirement.

`topic.describe` returns one `endpoint_info` record, including its profile only
once. Provider incarnation and native identity are optional evidence. The
description copies cached data from the retained endpoint; it neither calls
middleware nor performs discovery. Unknown incarnation is absent, rather than
copied from the grant generation. The native ROS2 topic provider caches a
nonzero publisher GID obtained during creation; it does not invent a subscription
GID or substitute the identity of a publisher observed by that subscription.
Malformed provider evidence (zero incarnation, oversized identity or an all-zero
present GID) is rejected before output admission.

`service.describe` is independently authorized from availability, QoS probing,
request submission, response and take. It returns the retained endpoint and two
role-specific legs: request-transmit/response-receive for a client, or
request-receive/response-transmit for a server. Each leg separately reports its
requested profile, optional actual profile and supported fields. An unavailable
getter produces an absent actual profile and an empty supported-field list; it
does not copy the other leg. The native ROS2 factory obtains both actual QoS
profiles from their corresponding native getters during creation and rejects a
configuration that the endpoints cannot satisfy. Description later uses those
immutable cached observations without calling middleware. Any originating
request/response schema revocation prevents subsequent description.

Action mutation receipts bind the complete canonical owned request and operation.
An exact retry can recover its historical receipt after a source capability
closes; a changed request with the same key is a conflict. Fresh effects still
require every live source. A failed effect with a proven `none` outcome consumes
no mutation key. An unknown effect is retained and fences replacement. Factories
must return an endpoint that might already have been exposed even when startup
fails. The ROS2 factory now returns that ownership before `start`, allowing the
dispatcher to retire and retain a failed startup that already received a goal.
Destroying a deployment is not durable reconciliation; restart fencing still
requires a deployment journal.

Graph plans fix both the context and observer node profiles plus the exact
approved discovery filter. The first admitted request pins the supplied origins
until revocation. Watch handles retain that same snapshot and carry readiness
only. A closed watch cannot be observed through a previously retained source
binding; closing it does not grant another context or create an implicit node.
Short or failed page delivery retains its exact revision/cursor for recovery.
Capture deadlines detect overruns after capture, without promising preemption.

Every native graph capture now carries the actual call's primary capability,
the observer's `node.open` capability, that node's original `context.open`
capability, and the explicitly supplied `context.describe` capability. The two
context roles must identify the same owned resource; equal names or profiles
cannot substitute for either capability. A graph-readiness token additionally
binds the exact watch incarnation, independently of the stable graph binding.
Closing/reopening a watch invalidates its queued tokens. Retained provenance
owns parent identities without owning the graph, provider or job queue.

Tokenless watch inspection reads cached readiness only. The event dispatcher
supplies a per-call observer callback, carrying the current caller and the
stream's exact source capability, for actual native refresh. The callback is
never retained by the stream. Setup and polling release the stream mutex and
authority groups before provider observation; output is separately revalidated.
Readiness is level-triggered until the corresponding graph revision is
acknowledged by completing its snapshot pages.

`communication_worker::graph_source` performs captures through a single
explicitly reserved asynchronous query. Its factory receives the complete
immutable graph plan and declares queued accounting before publication. The
queue charges once before submission; copying or replaying an already captured
page does not charge another capture. Explicit zero capture budget sends no
request. Node/context creation budgets are validation-only dependencies.
The adapter validates the entire bounded worker record before creating strings,
rejects unapproved nodes, fields, noncanonical endpoint names and trailing bytes,
and sorts/deduplicates accepted metadata. Enumeration order alone therefore
does not manufacture a new revision. Unknown outcomes retain occupied capacity.

`graph-current-watch-identity/results.json` qualifies six dispatcher groups
with Clang ASan/UBSan, GCC 13 and TSan. `graph-worker-query-bounded/results.json`
qualifies the actual dispatcher-to-query path using real nonblocking pipes,
including exhausted-budget pagination, failed-copy replay, stable enumeration,
six malformed/out-of-scope reply classes and watch closure before/after send.
These are core integration receipts, not registered Wasm graph imports or a
qualification of every ROS graph backend.

Event open names capability/resource pairs. A trusted `source_access` resolver
returns an actual generation-fenced source and validates its ancestors on every
poll and after output. `source_access::from` adapts the communication, graph or
action dispatcher directly; a deployment combining providers must route by its
trusted grant kind, never by guessing a numeric handle. Each source independently
requires `resource.observe`; opening a stream grants no payload access.

The requested event capacity has three separate meanings: `items` bounds retained
event records, `bytes` bounds a canonical delivery, and `workspace` reserves
logical collection storage for the stream's lifetime. Zero or insufficient values
are refused. `stream::required_workspace` checks the exact element-size arithmetic
for source tracking, staged observations and pending events before allocation.
Fixed object/control-block metadata is bounded separately by the configured stream
slots, including closed records still borrowed by a poll or output callback.
Closing a public slot does not return this retained-record reservation until its
last borrower exits. Record destruction releases the stream and source backing
before refunding workspace, then releases the retained slot. Reentrant close/open
therefore cannot exceed `max_streams` or `grant.max_resources`. This reservation
is not total allocator pages or process RSS. Global
workspace also bounds concurrent detached output. A failed open rolls reservations
back. Closing releases only the stream, and retains one closed-handle receipt per
slot until the next successful open acknowledges it, so lost close output is
recoverable. Poll keeps staged sequence positions after a failed copy and cannot
reveal them after an origin is revoked. It reports current readiness snapshots,
not every intermediate native edge.

All dispatcher-owned containers and shared control blocks use the recoverable
fast_io adapters. Their nullable allocation result becomes `no_memory` at admitted
request boundaries. The `silent` allocator behavior is intentional for guest and
deployment induced exhaustion; ordinary terminating allocation is unsuitable for
these recoverable request paths. `communication_workspace` reserves capacity
before allocation and refuses inherited process use before locking after `fork`.

The native graph adapter uses public `rcl` C queries and a nullable fast_io query
allocator for per-node endpoint result arrays. It serializes those queries per
native node and finalizes every returned array. A zero query workspace denies
capture; exhaustion returns a bounded failure. The pinned Jazzy implementation
of `rcl_get_node_names` explicitly ignores its allocator because
`rmw_get_node_names` has no allocator argument. Its node-enumeration arrays and
middleware internal caches consequently remain under the isolated worker's
address-space/RSS controls, not this per-query allocator. The provider does not
claim that external ROS libraries use fast_io for all internal storage.

Owned definitions and their separate explicit capacity declarations are described
in [communication configuration](ros-communication-configuration.md). Accepting a
definition does not by itself activate or qualify a native worker.

Cancellation callbacks are reentrant. A callback that returns false may still
have revoked a primary grant, a schema grant, or a retained context/node origin.
Every communication, action, graph and event dispatcher therefore re-admits the
complete authority group after each cancellation checkpoint before native effects
or the first output copy. Keeping a recursive authority lease is not a substitute
for this revalidation. Exact retained context/node handles and provider-local
retirement are checked as well as the authority table; readiness sources use the
same checks. A failed output retains the existing retry/queue semantics.

The `*-dispatch-cancellation-second` receipts qualify 130 added reentrant cases
across those four dispatchers with Clang ASan/UBSan, GCC and TSan, all with `-O3 -g`
in the existing 64 GiB zero-swap cgroup. These are authority/core tests, not native
worker deployment or VM-carrier qualification.

The closed native-worker RPC uses a separate private control channel, fixed
manifest slot numbers and exact generation/sequence framing. It cannot encode a
native path, descriptor, socket address or guest-selected owner. The native
backend provides fixed handlers for context/node inspection, topic and service
operations, action state machines and graph capture. All ROS type-support ELF
libraries needed after confinement must be preloaded from the pinned image before
its final syscall filter; adding an endpoint must never reopen arbitrary paths
or relax the filter.

`communication-worker-channel-second` qualifies the host pipe channel's detached
reply storage reservations, partial frames, invalid generation/extent, bounded
diagnostics, timeout/unknown results, revocation, SIGPIPE handling and fork
rejection with Clang ASan/UBSan, GCC and TSan. A complete native deployment still
requires the real sealed backend plus controlled-peer interoperability tests.
Fixed loopback IP/port and RTPS GUID-prefix filters constrain routes; they do not
cryptographically authenticate a peer. No cross-host or arbitrary unauthenticated
peer profile is enabled by these tests.

## Explicit receive reservations

`communication_receive_contract.h` defines six canonical operations:
`topic.reserve/acknowledge/release` and `service.reserve/acknowledge/release`.
A reference binds the endpoint, actual provider generation, record sequence and
independent reservation nonce. Consumer identity comes from the current VM
caller and reserve capability; guest bytes cannot select that identity. These
carriers are registered VM imports. Their bounded worker binding preserves the
reservation and requires explicit acknowledgement or release; registration does
not qualify arbitrary ROSIDL or every platform.

The local topic queue pins the original preallocated slot across delivery and
processing. Failed or throwing output retains it for the same consumer. Ordinary
takes, another consumer, close/rebind and drop-oldest cannot steal or overwrite a
borrowed record. Acknowledgement removes it exactly once; release leaves it at
the head. Exact settlement replay returns the existing receipt. Changed decisions,
nonces, callers and skipped mutation keys are rejected. Expiry denies further
observation/acknowledgement while cleanup release remains possible. Retirement
does not implicitly acknowledge a record. These are access-time deadline checks,
not an autonomous watchdog or hard real-time deadline guarantee.

The native service implementation preserves its original correlation, request
header and payload. Server acknowledgement only marks a request delivered; a
separately admitted `service.respond` must send the actual response. Client
acknowledgement releases the retained response. Neither release nor acknowledgement
is a remote cancellation. The isolated protocol carries an opaque host-owned
consumer identity scoped to its worker and sealed binding. Full reply capacity is
checked before the native borrow. Lost cross-process acknowledgements still need
the host's owned completion ledger; local queue evidence alone cannot resolve them.

`communication-delivery-registry-complete/results.json` passes nine Clang
ASan/UBSan, GCC and TSan commands at `-O3 -g` in the existing 64 GiB, zero-swap
cgroup. It includes canonical framing, unchanged ordinary-take behavior, actual
registry ownership/parent-close checks, failed sinks, exact replay, expiry cleanup,
producer/consumer concurrency, reentrant retirement and fork rejection. Native
worker integration is qualified separately; these tests make no VM, throughput
or industrial certification claim.

`communication-native-receive-reservations-bounded/results.json` rebuilds the
actual native backend and runs two sealed Jazzy/FastDDS workers, with `-O3 -g`
and unchanged 64 GiB/zero-swap limits. Both directions exercise topic and service
reserve, release/reborrow, old-nonce rejection, competing-consumer rejection,
blocked ordinary take, exact acknowledgement replay and the subsequent service
response. All six original String/Bool, SetBool and LookupTransform directions
also pass. This test does not yet exercise the public VM adapters or lost-ACK
reconciliation in their future host receiver. The first attempt exhausted the
test's four-reply retention budget and remains an incomplete receipt; the fixed
fixture releases compared replies and passes with the original runtime budget.

## Native mutation result consistency

Native callbacks may not return an arbitrary status enum or substitute a foreign
request identity. `checked_mutation` retains contradictory mutation evidence as an
`io/unknown` receipt using the host's own request. In particular, success with no
effect, failure after acceptance, and an incomplete deferred-queue identity cannot
become a pre-effect rejection that permits blind retry. Action state is unchanged
and uncertainty remains latched. Malformed deferred completion enums are refused
without clearing pending work; creation failure with effect evidence retains
cleanup debt even when no native handle was returned.

The `communication-result-deferred`, `communication-result-creation` and
`communication-result-mutations` receipts each pass nine sanitizer/GCC commands.
They test malformed callbacks, exact recovery, no premature action transition,
duplicate settlement and unreleasable uncertain outcomes. Subsequent source
changes require their own qualification; historical receipts remain immutable.

## Owned receive transport and cleanup admission

`communication_receives.h` connects the six receive operations to the bounded
owned operation queue and the actual nonblocking worker transport. Configuration
reserves one request and three complete reply extents: the transport queue's
reply, detached reconciliation storage, and the retained original record. It
copies the approved schema using recoverable fast_io allocation. The native
endpoint and worker generation must come from verified creation evidence; neither
a profile nor a guest integer supplies them. There is no background thread or
implicit polling loop in the adapter.

A receive reply is decoded against that independently retained endpoint,
generation, exact deadline, schema, direction, sequence and nonce. Worker clock
claims are refused because the private protocol has no trusted clock mapping.
Native service correlations remain native evidence until a finite host mapping
resolves them. The adapter's trusted decoder may not perform guest I/O or retain
borrowed views. A VM delivery must independently revalidate authority.

Host settlement keys and native settlement keys have separate counters. A known
failed ACK retains a host receipt without advancing the native counter; a later
explicit release can use the next host key and the still-current native key.
Exact accepted retries neither enqueue nor charge again. Lost, malformed or
contradictory ACK replies retain an unknown outcome and occupied capacity. They
cannot authorize a new receive or imply physical completion.

`cleanup_only` is a closed authority claim for `topic.release` and
`service.release`. It still requires the exact live capability, action grant,
caller membership and import exposure. It accepts no byte charge and cannot be
combined with `grant_only` or `validation_only`. A successful cleanup admission
produces a one-use publication lease without spending traffic allowance. Stored
resource origins remain exact, but a closed parent node or schema capability does
not prevent returning an already retained record. Cleanup cannot disclose that
record or acknowledge its consumption. Revoking the cleanup capability itself
still denies the call; trusted provider retirement owns subsequent teardown.

`communication-owned-receives-cleanup-retry/results.json` records nine successful
Clang ASan/UBSan, GCC and TSan commands at `-O3 -g`. The preceding attempt retained
a Clang code-generation crash and its preprocessed source/run script; the same
frozen source passed on retry. The tests cover complete current origins, failed
decoding, exact receipts, independent counters, exhausted traffic allowance,
parent-capability closure, unknown ACKs, concurrent submission and fork fencing.
The later `communication-owned-receives-authority-final` cohort also checks
malformed cleanup claim roles, one-use leases and mandatory import exposure.

`communication-owned-receives-native-deadline/results.json` runs the new host
adapter against the previously frozen two-worker Jazzy/FastDDS image pair. Each
side completes 12 failed-decode recoveries, 12 releases/reborrows and 12 exact
acknowledgements, including String, Bool and both SetBool service roles. The
original action flows also pass. The fixture derives its reservation deadline
from the sealed endpoint policy; an earlier fixture incorrectly requested four
seconds against the worker's shorter bound and was correctly refused. All three
commands pass with unchanged source/image hashes and unchanged cgroup memory
events. These are native integration results using fixture publication handles,
not evidence that the public VM imports or native correlation mapping are complete.

`communication_dispatch::invoke_receive` supplies the canonical carrier boundary.
It reserves the maximum complete record and workspace before polling, rejects a
missing actual provider incarnation, retains one charged reserve reference across
failed copies, and stores exact settlement receipts before delivery. An exhausted
new-reserve budget is refused before pinning a record. The separate
`communication-receive-dispatch-checked` cohort passes nine Clang ASan/UBSan, GCC
and TSan commands for topic delivery, short output, unknown provider identity,
failed copies, ACK replay, cleanup after parent closure, cancellation and changed
callers. It does not replace service-adapter, real-VM or debugger qualification.

## Bounded service correlation translation

`communication_correlations.h` owns a finite, preallocated mapping for one exact
host endpoint, native endpoint and verified worker generation. Client entries
start with a host-issued submission request; a native reply resolves that original
request. Server entries receive a new host handle only after a validated native
reservation. Native request integers are never published as guest correlations.
The original caller, capability, sequence and nonce remain part of each entry.

The server cannot respond before acknowledgement. A release invalidates its
unacknowledged host correlation; reborrowing the same original native request
creates a fresh host correlation. A known response completion permits explicit
reclamation. Unknown submission, acknowledgement or response outcomes retain
their entries and cannot be reset into permission to repeat an effect. Client
response acknowledgement can reclaim the completed mapping while the finite
exact settlement receipt remains recoverable.

`communication-service-correlations-first/results.json` passes nine Clang
ASan/UBSan, GCC and TSan commands. The real two-worker
`communication-owned-receives-native-correlations/results.json` passes three
commands and exercises the mapping in both service roles, including failed
decoding, release/reborrow, stale server correlations and exact acknowledgement
replay. Both receipts retain source hashes, `-O3 -g` command lines and unchanged
64 GiB/zero-swap memory events. These handles are issued by the native fixture;
VM publication and full ROSIDL coverage remain separate obligations.

## Endpoint receive accounting and callback boundary

`communication_worker_receiver.h` provides the token-aware boundary between the
owned receiver and the communication registry. It reserves one extra complete
payload buffer, translates native source/correlation identifiers, and performs
the caller copy without a provider or transport mutex. Reentrant delivery and
settlement return `again`. A failed or throwing copy preserves the original
reservation; changing the caller cannot acquire it.

The registry freezes `queued_receives()` when it retains the actual endpoint.
For those providers, only the owned RPC queue charges the operation. Dispatcher
retry, completion retrieval and repeated copies validate current authority
without another debit. Local queues retain their original accounting path.
Known no-effect ACK refusals retain both the original reservation and a failed
receipt, allowing an independently authorized release with the next host key.
Unknown outcomes fence normal reads and close/reopen, with exact recovery of the
stored receipt still allowed. An absent retired provider returns a checked
cancellation rather than being dereferenced.

Configuration and occupancy checks must not wait for a receiver while holding
registry locks. Receive admission validates that same registry in the opposite
direction. Contention therefore reports `again` during configuration and
conservatively occupied during cleanup; it does not erase cleanup debt.
`communication-receiver-adapter-nonblocking/results.json` passes nine Clang
ASan/UBSan, GCC and TSan commands for the dispatcher, registry, detached-copy and
private-pipe path. Earlier integration runs exposed lock-order cycles; those
failed TSan receipts are retained. No sanitizer check was disabled. These tests
do not register a VM import or establish native-equivalent performance.

## Asynchronous creation and typed host adoption

The isolated creation supervisor owns the complete canonical open carrier before
charging it. Its creation-request identity, the native endpoint identity and the
reserved host resource identity are distinct. A creation request is never a
usable topic or service handle. Cancelling an unsent request can establish no
effect; cancelling after any transmitted byte preserves an unknown outcome and
cleanup debt. A live published endpoint prevents release of its creation receipt.

The communication registry mints a move-only `endpoint_slot` against an actual
counted node lease and the configured schemas. The pending slot prevents both a
competing open and parent close, but cannot be used by ordinary endpoint calls.
Typed adoption requires the exact registry token, live parent and schemas, and
verified worker generation. Neither dropping the token nor losing guest output
refunds capacity. Abandonment requires trusted positive evidence of no native
effect or completed native teardown. Revoked plans cannot be reactivated by
returning that capacity.

`communication-typed-adoption-checked/results.json` records nine passing Clang
ASan/UBSan, GCC and TSan commands for the registry adoption boundary. It covers
foreign tokens and nodes, invalid provider evidence, independent service QoS,
move semantics, concurrent reservation, parent/schema revocation and fork.
Its source snapshot precedes subsequent observation-accounting additions; the
receipt qualifies exactly its retained hashes, not later workspace edits.

## Typed isolated topic facade

`communication_worker_topic.h` composes the owned mutation, observation and
receive queues with an actual node lease. Publishers reserve a separate DDS
acknowledgement observation slot. Subscribers reserve a complete detached payload
and acknowledgement/release capacity. Aggregate capacity is explicit and
independent of optional traffic attenuation. All owning allocations use the
recoverable fast_io adapters. Configuration does not start a process, a thread,
or a native operation.

The sealed paired factory validates each topic and schema against its packaged
manifest. It checks aggregate topic capacity at configuration, before any worker
starts. It constructs a facade only from its own verified creation ledger and
retains it across failed host publication. Native identifiers never appear as
guest resource handles. Authority callbacks run outside the factory lock.

Each call carries the current caller, primary capability and complete original
node/context/schema cohort. The registry freezes mutation, receive and
observation accounting modes at adoption. An isolated queue is the sole charging
point for its operation. QoS output size is checked before starting a query;
local providers additionally perform budget preflight before their callback.
Polling and recovering an accepted query does not debit the operation again.

DDS acknowledgement polling preserves the first host request and exact deadline.
An identical poll can recover a terminal result after the deadline or after its
traffic allowance was consumed. A different observation cannot replace pending
work. `observed_remote` describes the middleware acknowledgement only; it does
not establish robot execution or physical safety. Malformed or contradictory
worker receipts become unknown outcomes and retain occupied capacity. Mutation
completion is handed to trusted host bookkeeping before its queue slot may be
released.

`communication-typed-topic-checked/results.json` records nine passing core
commands for publication, QoS, acknowledgement polling, zero allowance, original
authority, malformed receipts and fork. These are registry/adapter tests with
real nonblocking pipes. The later sealed-factory integration and the public VM
carrier each require their own source-pinned receipts. This addition alone does
not register communication imports or qualify legacy single-call take semantics.

## Typed isolated service facade

`communication_worker_service.h` owns separate request and response schemas, a
counted node lease and a finite correlation table. The paired factory checks the
complete open carrier, both schema capabilities, role, service type and sealed
profile before enqueueing creation. It obtains the native endpoint only from
its own creation ledger. The host adoption slot cannot be supplied as a native
request or used before verified publication.

Client submissions reserve a host correlation before charging or publishing an
RPC. The verified native submission receipt supplies its native counterpart.
Server requests obtain a fresh host correlation when a validated reservation is
copied; a failed copy repeats that same mapping. A response is permitted only
after an independently authorized successful acknowledgement. The response
queue translates the host correlation back to the exact native endpoint and
worker incarnation. Unknown submission, response or settlement outcomes retain
capacity and prohibit another effect being inferred from a timeout.

Submission/response, observations, receive settlement and cancellation of local
interest use independent reserved queues. A cancellation cannot borrow capacity
from an occupied response receipt. A known local cancellation can retire local
interest; it never proves remote cancellation, physical stopping or completion.
The current native adapter deliberately reports unknown remote outcome on
abandonment, and that outcome remains fenced. Failed mutations with an unknown
effect are returned as retained receipts carrying their actual error status,
rather than being misreported as pre-effect failures.

The reservation comprises two mutation slots with encoders, one observation slot
with detached reply, one receiver with detached payload, all correlation rows,
and an independent close slot. Native middleware reservations remain separate.
All plan copies and queue allocations occur inside the recoverable fast_io
allocation boundary. Partial preparation performs no native I/O, consumes no
traffic allowance and releases its counted node lease before a retry.

`communication-typed-services-native-carrier/results.json` records five passing
commands using the sealed Jazzy/FastDDS pair: Clang ASan/UBSan and GCC execute an
actual SetBool request/response through the factory and host registry. Checks
include separate schemas, distinct correlations, acknowledgement before
response, failed-copy recovery, exact mutation receipts and schema-capability
closure. This receipt precedes the subsequent explicit-close integration and
qualifies only its recorded source/runtime hashes.

`communication-typed-service-unknown-receipt/results.json` records nine passing
Clang ASan/UBSan, GCC and TSan commands for the service facade and registry.
`communication-typed-service-allocation/results.json` records six passing commands
and exhausts all 24 preparation allocation points for each service role. Both
receipts precede the additional close-slot allocation. These are distinct from
public VM, arbitrary ROSIDL, cross-platform and performance qualification.

## Explicit native endpoint destruction

`communication_worker_closure.h` excludes ordinary endpoint calls before
checking quiescence. A call pin remains held through detached delivery, so close
cannot race a successful payload copy. Occupied requests, reservations and
uncertain results are not implicitly discarded. A busy close leaves the endpoint
usable for the explicit operations needed to settle that work.

An admitted close owns its own pre-reserved RPC slot and fences new ordinary
operations. Only the verified native endpoint identifier in a complete close
reply establishes native cleanup. A different identifier, unexpected effect,
lost reply or interrupted transport retains debt. The registry refuses to
discard an isolated topic/service handle until that confirmation exists.
Synchronous local providers retain their explicit destruction path.

The `resource.close` action is independently exposed and granted. Its cleanup
admission does not consume optional traffic budgets or require live parent and
schema capabilities, but still checks the current caller, primary capability,
exact resource and retained origin cohort. This exception cannot create a
resource, read a payload, acknowledge a message or send a robot command. Closing
a native communication endpoint is not a physical safety stop. Releasing its
creation receipt and reusing factory capacity additionally require reconciliation
by the owning creation supervisor. `paired_factory::reconcile_close` checks the
exact retained creation request, host resource and facade's verified native-close
evidence before clearing creation debt. `release_open` then releases only a known
terminal receipt belonging to the original caller. Neither method is a guest
authority entry: the public dispatcher must independently admit its cleanup or
request-release action. Unsent cancellation releases preparation; an unknown
effect remains unreleasable even after positive lifetime cleanup.

Each immutable binding reserves one capacity slot before creation is admitted.
The sum of these fixed reservations is checked before starting any worker. A
successful facade takes ownership of its slot in a member destroyed after all
owned queues and correlation storage. Retained registry or observer references
therefore prevent another allocation even after native close and receipt release.
An expired `weak_ptr` is insufficient evidence of completed destruction and is
not used to return this capacity. Failed preparation retains its reservation for
an exact retry; there is no double charge and no hidden allocation of a second
endpoint. This fixed per-binding capacity does not implement traffic attenuation.

`communication-public-close-current/results.json` records nine passing commands
for the canonical public dispatcher, including exhausted traffic budgets,
closed parent/schema capabilities, a revoked primary capability and incorrect
native-close identity. `communication-typed-topics-lifetime-reuse/results.json`
and `communication-typed-services-lifetime-reuse/results.json` each record five
passing commands against the actual sealed Jazzy/FastDDS pair. They exercise
native close, exact creation reconciliation, retained-buffer capacity rejection,
recreation with fresh host/native identities, and repeated unsent cancellation.

`communication-topic-allocation-lifetime/results.json` and
`communication-service-allocation-lifetime/results.json` each record six passing
commands. The recoverable fast_io failure sweep reaches 18 publisher, 20
subscription, and 27 allocation sites in each service role. Failed preparation
keeps its capacity lease; successful destruction returns it. These counts refer
to the retained snapshots and must not be assumed for later source changes.

Mutation replies must also carry the exact worker generation in both their
envelope and canonical receipt body. The topic/service
`communication-*-receipt-generation` cohorts each pass nine commands and fence
foreign-generation bodies as unknown effects. The
`communication-native-receipt-generation` cohort records five passing commands
for actual SetBool operations and repeated service creation with this check.
All listed qualification remains `-O3 -g` in the existing 64 GiB, zero-swap cgroup;
none is a public-VM, cross-platform, timing or functional-safety certificate.

### Isolated action identity and lifecycle

`communication_worker_action.h` now owns a finite action correlation table,
mutation queue, retained event query and independent close reservation. A client
goal, server proposal and cancellation request have distinct host identities.
Native identities and UUIDs never confer authority. Complete event decoding checks
the worker incarnation, endpoint, sequence, UUID, role, schema and payload before
entering the host action ledger. Invalid or lost mutation evidence retains an
unknown outcome instead of permitting another effect.

Retained ingress does not invent loss when a local queue is full. State changes,
cancel-response settlement and terminal retention occur only after the complete
event is admitted to its reserved queue. Failed guest copies keep the same event
and do not issue another worker query. Native loss remains explicit and monotonic.
Delivery metadata uses the verified worker incarnation; the local grant generation
continues to identify authority and readiness subscriptions. These generations
are intentionally independent.

The paired factory validates all three schemas and the complete immutable action
plan before typed adoption. Its capacity lease transfers only after both facade
and host ledger are initialized. A verified close reconciles the exact creation
request and host resource. Reopening remains unavailable while any old facade or
host ledger reference still owns that reservation. Preparation, mutation and
delivery storage use recoverable fast_io allocation.

`communication-native-actions-incarnation/results.json` records five passing
commands against the sealed Jazzy/FastDDS pair: Clang ASan/UBSan and GCC execute
LookupTransform proposal, acceptance, execution, feedback, result, cancellation,
failed-copy recovery and close/reopen. The fixture permits two seconds of local
DDS discovery before submitting goals. It is not a discovery-readiness guarantee
or latency measurement. `communication-action-endpoint-incarnation/results.json`
adds nine sanitizer/GCC/TSan commands for the pipe-backed facade, including zero
allowance, mismatched receipt generation, unknown debt and fork fencing.

### Asynchronous creation controls and typed publication

`communication_open_dispatch.h` implements canonical creation take, cancel,
description and release controls. It checks the caller and immutable binding
against one atomic receipt/provenance snapshot. A take additionally validates
the original node, context and schema owners before preparing a typed resource.
The complete output is reserved before preparation or debit. Publication requires
fresh full origin authority and returns a separate 64-byte typed result. A lost
copy retains the same resource and does not repeat the take debit.

Cancellation and receipt release require independent current grants. Their
reserved cleanup admission is independent of traffic attenuation. Cancellation
cannot close a published resource; the caller must use `resource.close`. Release
cannot discard a live endpoint, an unknown outcome or unconfirmed cleanup debt.
Descriptions inspect cached state and do not pump IPC or advance a deadline.

`communication_worker_open_control.h` retains the sealed factory and the typed
adoption context. Factory publication checks remain mandatory; optional host
publication/release hooks connect the private resource to its owning dispatcher.
The adoption context must not strongly own its control dispatcher. Deployment
retires controls and drains the factory before destroying borrowed node callbacks.

`communication-open-controls-current/results.json` records nine passing
sanitizer/GCC/TSan commands covering short output, exhausted traffic allowance,
exact replay, parent closure, unknown outcomes, reentrant delivery, concurrency
and fork. `communication-native-actions-control-binding/results.json` records
five passing commands through the actual worker-backed control adapter. It proves
that a short take does not adopt a core, a failed copy retains the same handle,
and positive native close permits independently authorized receipt release.

The action dispatcher also has an asynchronous-only preparation mode. Its opaque
draft resolves actual node/context/schema owners before worker admission.
`action.open` produces a 48-byte creation receipt; a creation request is never an
endpoint handle. The worker ledger owns complete canonical retry bytes, performs
the sole creation debit and supplies verified creation evidence. The dispatcher
installs the core privately, then publishes it under the exact original lease.
Published retries inspect the retained receipt even after the counted node lease
has moved to the facade. Released creation keys cannot be reused for another draft.

`communication-action-adoption-carrier/results.json` and
`communication-action-dispatch-after-adoption/results.json` each record nine
passing sanitizer/GCC/TSan commands for the asynchronous carrier and existing
action behavior. These receipts qualify their retained source snapshots. The
earlier worker/control and action-dispatch suites qualified separate integration
boundaries. `communication-native-action-dispatch-deadline/results.json` joins
those boundaries against the actual sealed Jazzy/FastDDS pair: five passing
Clang ASan/UBSan and GCC commands cover dispatcher-minted origins, asynchronous
creation, typed publication, canonical goal/feedback/cancel/result, failed-copy
recovery and positive cleanup. The fixture explicitly allows two seconds for
discovery. None of these receipts establishes VM import, CLI, debugger, generic
ROSIDL, cross-platform, near-native-performance or functional-safety qualification.

### Canonical asynchronous topic and service drafts

`communication_dispatch::prepare_async` requires the receipt-based endpoint path
and disables its synchronous topic/service factory path. Context and node setup
remain trusted host operations at this integration boundary. Each opaque endpoint
draft resolves the actual node, original context and ordered schema grants before
reserving a private registry slot. The configured deadline bound, profile, resolved
name, role and schema identities must all match. Type descriptors and draft storage
use recoverable fast_io allocation. The draft owns a counted node lease; the
creation ledger separately reserves complete canonical retry bytes and performs
the sole creation debit.

`topic.open` and `service.open` return 48-byte creation receipts. A short output
cannot allocate a draft, debit a grant or invoke native work. An enqueue failure
or throwing cancellation callback releases only a proven uncommitted reservation.
After successful enqueue, failed output retains the actual request for exact
replay. Replays require the same caller, capability, key and canonical carrier;
they inspect the retained receipt after adoption has transferred the node lease.
Exhausting the final traffic allowance or closing a parent does not repeat an
already accepted native effect. The current primary capability remains mandatory.

The supervisor records the actual worker incarnation before adopting a typed
facade. A receipt request, host resource and native endpoint are distinct identities.
The facade must match the receipt generation and the reserved registry slot.
Adoption alone does not publish the resource. `communication.open.take` requires
fresh complete original authority and returns the same 64-byte published result
on a failed-copy retry. Independent endpoint close and verified native cleanup
precede receipt release. Unknown effects, active resources and unconfirmed cleanup
retain their capacity. A released key cannot create another resource; a new
creation uses exactly the next key.

The following immutable receipts cover this boundary:

- `communication-endpoint-adoption-zero/results.json`: nine Clang ASan/UBSan,
  GCC and TSan commands covering all four roles, exact origin composition, short
  output, failed-copy recovery, zero/final budgets, parent closure, revocation,
  explicit release, throwing/reentrant callbacks, concurrent retry and fork fencing.
- `communication-endpoint-draft-allocation-initial/results.json`: six commands
  covering every observed draft allocation failure, with three sites per Topic
  role and four per Service role. No injected failure debits a grant or creates
  a native endpoint; each permits a subsequent successful attempt.
- `communication-dispatch-after-endpoint-adoption/results.json`: nine commands
  retaining the existing eleven communication-dispatch security groups.
- `communication-native-endpoint-dispatch-capacity/results.json`: five commands
  against actual sealed Jazzy/FastDDS workers. Canonical dispatch drives String
  publication and SetBool request/response, both asynchronous creation paths,
  independent source authority, failed-copy recovery, explicit receive ACK, and
  verified close/release after a schema capability is closed.

The native endpoint fixture allows two seconds for DDS discovery and does not
measure latency. All listed compilers use `-O3 -g` within the existing 64 GiB,
zero-swap cgroup. These are host/provider receipts; the catalogue deliberately
keeps the corresponding VM imports unregistered until deployment, CLI, debugger
and Wasm boundary qualification are complete.

### Asynchronous node creation and actual node evidence

`communication_dispatch::prepare_all_async` additionally disables synchronous
node creation. An explicit positive `node_deadline_ns` bound is required; zero
disables this path. A node draft resolves the actual published context and its
original capability. Its canonical carrier includes the next creation key and
an absolute steady-clock deadline. Submission returns a 48-byte receipt without
starting a process. `communication_node_open_provider::pump` advances one bounded
startup step after fresh node/context admission. The ledger records possible
native effects before invoking startup, so a cancellation cannot erase an
in-flight process or manufacture a safe retry.

Successful startup supplies `communication::node_attachment`, an opaque proof
minted by the actual registry. It contains the exact node and context owners,
host resource identities, anchor identity and verified worker incarnation.
An integer native token cannot complete a node request. The proof keeps neither
the native node nor an extra observer alive, and reusing an anchor cannot revive
an old proof. `communication.open.take` privately prepares and publishes only
that exact node under the complete original authority. A request identity and a
published node handle remain distinct.

Cancelling started startup signals the retained provider failure domain. For
the paired worker provider, that domain includes both peers. A deployment
supervisor retires and reaps the factory outside authority locks, then calls
`reconcile_cleanup`. A timeout, process signal, logical node close or successful
launcher exit does not prove namespace cleanup. Unknown effects remain unknown
even after positive cleanup and cannot be released or retried as new work.
Published nodes require their independently admitted `resource.close` operation.

The source-pinned `communication-node-open-cancel-outcome` receipt contains nine
Clang ASan/UBSan, GCC and TSan commands covering inert submission, bounded startup,
failed-copy publication, exact replay, zero authority, parent closure, unsent and
started cancellation, reentrancy, fork and concurrent pump/cancel. The
`communication-node-allocation-current` receipt contains six commands and sweeps
the two observed allocation sites without charging or starting work on failure.
`communication-node-integration-node_evidence`, `-creation`, `-endpoint_adoption`,
`-action_adoption`, `-open_dispatch` and `-dispatch` each contain nine passing
commands; `-endpoint_allocation` contains six. The companion
`communication-node-integration-native` receipt contains five passing commands
against the sealed Jazzy/FastDDS pair. It explicitly verifies zero launches after
node receipt submission, then executes native String and SetBool traffic through
the published nodes and confirms process cleanup.

### Production endpoint coordination and callback ownership

`communication_worker_endpoint_open.h` composes canonical Topic/Service creation,
verified typed adoption, publication, queued mutation settlement and positive
close reconciliation. Deployment configures one coordinator per exact binding.
The coordinator retains its dispatcher and factory; the supervisor explicitly
pumps bounded work. A guest callback never drives a private polling loop or
starts an implicit thread. Ordinary receipt settlement requires the exact
operation token and observed result. Unknown effects retain both host and worker
receipt capacity.

`dispatch_admission` retains a weak dispatcher reference. Factory creation queues
and endpoint facades each retain the callback context through destruction. Its
stored pointer must equal the declared owner pointer. Dispatcher expiration and
forked use deny admission. These ownership edges prevent dangling callbacks
without making a factory/dispatcher reference cycle. A positive native-close
receipt and removal of the host resource are both required before creation
cleanup is reconciled. Request release does not drain queues while holding an
authority lease: a subsequent supervisor pump drops the closed facade references
and releases their actual buffer reservation. The small released draft remains
available for exact failed-copy recovery until a later creation replaces it.

These provider components do not, by themselves, register VM imports. Worker
discovery delay, scheduler placement and isolated IPC still require separate
deployment and performance qualification; successful semantic tests are not a
latency guarantee or an industrial safety certification.

The `communication-native-endpoint-coordinator-initial` receipt contains five
passing commands for production Topic/Service coordination with real String and
SetBool traffic, exact publication/release recovery and positive cleanup. The
three `communication-release-recovery-{node_open,endpoint_adoption,action_adoption}`
receipts each contain nine commands. They additionally qualify repeated release
of the same verified draft after its registry slot has been removed. A mismatched
request remains rejected; an already released key cannot create another resource.

### Owned activation, image preparation and registry reentry

`communication_worker_manifest.h` provides inert named settings for the exact
launcher, runtime images, startup profile, finite process limits and CPU/NUMA
placement. Missing fields and duplicate settings are rejected. `prepare` pins
regular files using `O_PATH | O_NOFOLLOW`, checks their exact sizes, and reopens
the same inodes for reading. It neither loads a library nor launches a worker.
The factory's subsequent sealing step verifies SHA-256 and executable image
requirements. Thus successful path preparation alone is not image authentication.
The `communication-image-configuration-elf` receipt contains six commands for
inert freeze, symlink/FIFO/size refusal, path replacement, immutable inode
selection, mandatory digest verification and closed placement syntax.

`activation_admission` retains weak references to the actual dispatchers. Before
enabling the private transport, it verifies that both dispatchers share the same
authority object and environment, then checks every node/context plan field and
the exact accepted startup request, including caller, capabilities and carrier
digest. Both original authority groups remain held across bounded activation.
A duplicated node, expired dispatcher or forked callback cannot authorize a pair.
The factory retains the callback owner through startup and retirement; its
declared owner pointer must match the callback context pointer.

Node preparation and creation call providers outside the registry mutex. Frozen
plan storage and retained context/provider objects remain valid for the call,
and the registry rechecks close/revoke state before accepting its result.
Creation marks the row in progress, excluding duplicate creation and context
close. Objects discarded after a failed check are destroyed after unlocking.
This permits a provider to perform exact bilateral authority checks by querying
the registry without lock reentry. Failed or cancelled startup retains the
coordinator's existing native cleanup obligation.

`communication-node-create-reentry` contains nine sanitizer/GCC/TSan commands
covering reentry during both startup stages and late context closure. The
`communication-native-activation-unlocked` receipt contains five passing commands
joining production image configuration, owned bilateral admission, both node
coordinators, Topic/Service creation, native traffic and cleanup. It also checks
that callback ownership does not keep the dispatchers alive after retirement.
Earlier `communication-native-canonical-activation` and
`communication-native-owned-deployment` attempts timed out on the two registry
lock-reentry defects; those incomplete receipts remain preserved and are not
qualification evidence.

### Broker composition and revocation

`communication_host` owns a finite immutable table of creation providers and
routes canonical requests by the already granted alias and purpose. It shares
one authority environment with its communication dispatcher and creation-control
dispatcher. All default capacities are zero. Preparation reserves host storage
without node startup; `pump` is a separate bounded supervisor step. Cached
inspection and request description do not perform supervision or native I/O.

The root broker routes communication `request.describe`, `request.release` and
`resource.close` to this owner before generic accounting. Failed zero-result
delivery is checked before close begins. A successful capability close notifies
pending startup, while published resources retain their independent cleanup path.
Worker lifetimes are separately retained for stop/reap outside authority locks.

Service revocation calls the facade's nonblocking `retire` fence. The supervisor
and final destructor use `drain` after authority and registry locks have been
released. Waiting for a pump inside the revocation callback would deadlock if
that pump is acquiring the same authority; the separate drain preserves the
receipt and native cleanup debt without that dependency. These composition
changes still require deployment, CLI, debugger and actual VM qualification.

### Explicit deployment and worker-local registries

`communication_deployment_configuration` joins authenticated startup definitions,
image manifests, capability aliases and process pairs. Every aggregate capacity
must be supplied before loading a worker definition. Loading checks the exact
file size and full-file SHA-256, decodes an owned definition, and records that
same file as the immutable `/worker/profile` image. The exact `profile-bytes`
limit is derived from the authenticated file; later settings cannot replace it.
Static freeze performs no path reads or process startup. Production preparation
pins and seals the complete executable closure again before publishing a broker.

Each worker has a private communication registry within the same authority
environment. Different workers may use identical ROS schema and profile names;
their actual context, node, capability, resource and creation identities remain
distinct. Each alias names one definition on one worker. The builder verifies
that its context, node and schema dependencies are also explicitly bound there.
Unused definitions in an approved image do not confer endpoint authority.

The deployment currently binds the implemented context, node, type, topic,
service and action provider families. Graph and event dispatchers require their
remaining deployment composition; accepting a generic binary or an unsupported
family is not a fallback. Linux isolation is the currently implemented native
backend. This is not a cross-platform qualification claim.

`communication_deployment_provider` routes exact host-minted owners to a sealed
pair. Preparation allocates and authenticates images and reserves queues, without
starting a ROS node. Only a separately admitted `node.open` starts that worker;
both original context/node authority groups must still be valid before private
transport activates. Creation take publishes a distinct typed host resource.

Each pair receives its own joinable supervisor and an explicit guarded stack and
poll period. The stack uses the recoverable fast_io allocation path. Supervisors
inherit the preparing thread's CPU/NUMA placement; worker processes apply their
separate approved placement. These settings do not provide exclusive cores,
CPU bandwidth reservations, real-time scheduling or a worst-case response bound.
The poll interval contributes to latency and must be included in native/VM
measurements. Work and cleanup on one pair do not run on another pair's
supervisor. Within a pair, one serialized RPC channel preserves framing and
causal ordering; throughput and tail latency must account for that serialization.

Retirement first fences every affected pair, then joins/drains outside authority
and registry locks. Actual namespace/process cleanup evidence is required before
node cleanup debt is reconciled. Unknown operation outcomes remain unknown.
The enclosing broker retains supervisors until joining completes, so closing a
guest capability cannot destroy a borrowed callback or recycle its worker.
Supervisor exit now fences each owned node creation ledger before reaping.
Unsent work becomes a releasable cancellation; work with unknown effects keeps
its uncertainty even after positive namespace cleanup. This also covers a peer
failure that prevents the other node from receiving another pump. The
`node-supervisor-retirement` receipt exercises queued, started, ready and
reentrant-stop states under ASan/UBSan, GCC and TSan. It does not explain every
possible native startup failure or substitute for actual VM stress evidence.

`communication_worker_action_open` retains one exact action draft, typed facade
and local action ledger through failed delivery and cleanup. Its admission
contexts use weak dispatcher ownership and outlive every callback-bearing queue.
The separate aggregate action workspaces and per-worker carrier/provider
reservations are mandatory. `action-deployment-native-owned/results.json`
records the production deployment path under Clang ASan/UBSan and GCC with real
sealed Jazzy workers. It covers owned-goal LookupTransform proposal, acceptance,
execution, feedback, cancellation response, terminal result and independent
resource/creation release. This is not generic ROSIDL, cross-platform or Wasm
qualification, and does not implement broad cancellation selection.

`service.available` uses a process-wide identity for the actual host steady
observation clock. Its epoch is fixed for that process, and forked use is denied.
This timestamp is an observation of availability; it is not a source timestamp
from DDS and conveys no ROS-time, clock-setting or device authority.

The VM now has checked carrier bindings for the communication, explicit receive
reservation and creation-control operation lists. Variable requests reserve the
selected worker's workspace before copying guest memory. Fixed receive/control
requests use bounded stack storage. The wrappers reject overlapping result and
length slots before effects and retain the normal full floating-point boundary.
Actual VM, debugger and performance qualification remain separate acceptance
steps; compilation or registration alone is not proof of complete ROS coverage.

## Independently authorized service readiness

The `service.available` request consists of two little-endian 64-bit words, the live
client endpoint and its source capability. A direct client grant supplies zero
in the second word. For independent observation, use the ordinary scene CLI to
register a service grant. Set its binding to `service-observer`, its resource to
the exact client grant alias, and its only action to `service.available`.
The source grant must own a configured native client with `service.submit`.
The source need not grant direct readiness to its holder.

For example, with an existing `worker.0.service-client` grant and native
client binding in the `robot` environment, add the observer before freeze.

```sh
--ros-scene-grant robot observe-client service worker.0.service-client service-observer service.available
```

The guest opens both grants. The observer capability is the first Wasm import
argument. The second request word must contain the live capability for its
registered client. Two clients with identical service names and profiles remain
distinct. The same environment must currently expose both actions. The host
binds the result to the exact endpoint and validates its context, node, request
schema, response schema and independent source. Only the observer incurs a
query debit. No service request is submitted during discovery.

The worker performs its own client's native readiness observation and retains
a correlated asynchronous result. A final authorization check precedes guest
delivery. An observer withdrawal leaves the actual client usable. The supervisor
can discard a settled, effect-free response for a retired observer. An unknown
effect remains fenced. An enabled host result journal adds the two operator
aliases and the observed authority loss only after a verified association.
Unauthenticated queries reveal neither alias. No payload is included.

Passing this query establishes readiness at the observed host timestamp. Legacy
ROS1 header/MD5 verification, a bounded wait with advertised native features
and event payload forwarding require separate providers and migration review.
