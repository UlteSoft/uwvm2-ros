# Robot provider admission and execution procedures

This is the normative design for the device, control, media, bridge and component
families of `uwvm_ros_v2`. Their 22 operations have concrete request and result
schemas, allocation-free executable decoders and registered guest carriers.
The [concrete binding guide](ros-robot-bindings.md) specifies the available
directional scalar devices, control phases, raw media, topic bridges and isolated
message-role components. These procedures also govern additional adapters;
registration does not establish every driver, codec or platform. The complete
[160-operation reference](ros-operation-reference.md) covers the other families
and their distinct existing encodings and availability.

The machine-readable sources are
[operation contracts](../../tools/ros_scene/operation-contracts.json) and
[robot provider contracts](../../tools/ros_scene/robot-provider-contracts.json).
The latter defines 62 typed profile fields, finite reservations, deadline fields,
closed states, per-operation secondary admission and independent sequence domains.
Both sources are checked against the actual catalogue. A new operation must add
its design row; a deleted or renamed operation must update its references.

## Common carrier and ownership

Each designed operation uses the checked carrier
`(capability, memory, request, request_bytes, output, output_capacity, output_length)`.
Its result code reports admission/delivery status. An operation receipt reports
the state of work that has already been admitted. These two channels must not be
conflated after a native effect or a lost result copy.

Fields are ordered, little-endian, and defined by the catalogue. No request
contains a native pointer, loader symbol, file descriptor, process ID, arbitrary
socket address or credential. A `profile` is a semantic identity and a 32-byte
digest of a trusted immutable definition. It grants nothing by itself.

The caller supplies the decode byte, text, blob and aggregate element budgets.
All default to zero. The decoder borrows a complete owned immutable carrier and
publishes a result only after full validation. It rejects trailing bytes,
noncanonical booleans/text, impossible plane extents, zero identities, unknown
selectors and duplicate or descending role/reference lists. There is no fixed
64-entry source table in these contracts. Implementation support limits must be
reported explicitly and must never truncate an accepted list.

The provider performs the following ordered procedure:

1. Resolve the actual caller generation, exposed import and exact primary grant.
   Validate every input/output span and the admitted carrier extent.
2. Own the entire request; decode it without allocation. Resolve each independently
   supplied capability and all ancestors from the host registry. Match the frozen
   profile, resource generation, role and semantic value validator.
3. Resolve original input provenance, current controller ownership and active
   configuration dependencies. Reserve complete reply, replay record, work item,
   pins, native workspace and bounded failure/cleanup accounting.
4. Commit the accepted request identity before scheduling an external effect.
   Recheck the complete origin set at the actual effect boundary. The receiver
   independently checks its boot generation, ownership epoch and expiry.
5. Deliver only complete validated results. A failed output copy keeps a queued
   item or effect receipt recoverable. Roll back an undelivered inert new handle.
6. Retire capacity only after actual borrowers/native work exit and effect
   uncertainty has been resolved under the declared reconciliation policy.

Authority/provider locks are not held across IPC waits or external sink calls.
If state changes while a copy is in progress, revalidation fences consumption.
Lock ordering and atomic compound-origin checks are part of each provider's
implementation qualification; a syntactically valid request does not establish
them. Owned state uses the project's recoverable fast_io allocator. The ordinary
terminating allocator is not permitted on untrusted admission paths.

## Authority, capacity and attenuation

The progression is **zero authority → explicit elevation → stable authority →
explicit attenuation → potentially uncertain service guarantees**. No implicit
rate applies inside a stable grant. An absent optional rate is unlimited inside
that grant; an explicit zero is zero. Finite storage, process, thread, queue and
workspace reservations are separate admission requirements.

A permission names the operation, concrete resource, direction and immutable
profile. `control.submit` on one controller does not authorize another controller,
`media.submit` does not grant data export, and `component.start` cannot manufacture
the roles that its child process needs. Full origin closure is checked again on
delivery, forwarding and execution.

Each writable route declares a nonempty set of its downstream effect classes.
Physical execution, configuration, maintenance and data export can coexist;
selecting one supposedly strongest label must not discard the other predicates.
Unknown effect classification refuses activation.

Rate attenuation of a continuous control phase must select a qualified missed-
phase policy. It cannot silently skip a write or consume the stop reservation.
Deployment may fence and stop a controller independently of guest grants.
Guest-requested stop still requires its own grant. Normal throughput, logging,
model inference and unknown-outcome records cannot exhaust stop/fault capacity.

## Sequences, receipts and uncertainty

Each keyed request uses its owned resource as `key.session`. The operation selects
an independent sequence domain. Device command and device stop are separate;
control write and control stop are separate; media frames and close are separate;
bridge/component start and stop are separate. Each starts at one. Zero and wrap
are invalid. A lost normal-work receipt therefore cannot block a stop request.

The next successful sequence acknowledges only the preceding replay receipt in
that same domain. Exact last retry returns the same accepted request; changed
canonical fields conflict. Historical exact retries may remain decodable after
their deadline, but recovering their payload still needs current authority.
Acknowledging a receipt never releases an unresolved physical effect or cleanup
debt. Unkeyed open/read/take calls do not promise exactly-once replay.

The five families use this closed receipt relation:

- `admitted`: `none` or `locally_accepted`.
- `queued` or `running`: `locally_accepted`.
- `succeeded`: `committed` or `observed_remote`.
- `failed` or `cancelled`: a known `none`, `locally_accepted`, `committed`,
  `observed_remote` or `partial` effect.
- `outcome_unknown`: `unknown` or `partial`.

Known partial cancellation is distinct from an unknown outcome. Neither proves
that an actuator is stopped. A valid transport acknowledgement is not a qualified
state observation. Device evidence has an exact provider generation, controller
epoch, clock incarnation and original source references. A claimed observed state
must include an observation; syntax validation cannot authenticate that source.

No automatic non-idempotent retry follows timeout, partial transmission, process
death or lost acknowledgement. Such work retains identity, pins and accounting.
Recovery across restart requires an authenticated durable ledger or qualified
state reconstruction. Loss of both prevents rearming. New worker generations
invalidate prior fencing, commands and leases; reconnect is not lease renewal.

## Device procedure

`device.open` resolves an already prepared qualified driver profile and independent
watchdog, reserves an inert handle and publishes it only after complete copying.
It performs no bus operation. The profile fixes the physical target, protocol,
command validator, permitted modes, controller scope, stop procedure, source and
dependency rules, qualified evidence source and four directional channels.

The normal command and stop channels send; their acknowledgement channels receive.
The stop path has independently reserved queue and execution capacity. A target
must not claim that a separate descriptor alone guarantees timely stopping if
both descriptors depend on the same saturated executor or failed link.

`device.submit` independently checks the controller capability and epoch, all
observation buffers and their actual trusted origins, and every active semantic
revision. Validate the whole command: schema, units, coordinate frames, finite
values, permitted mode, coupled limits and freshness. A scalar bound is not a
collision or full-trajectory validator. The selected machine profile must supply
the complete validator needed by that operation; unavailable validators deny it.

Command acceptance reserves its original inputs and recovery record. The final
driver rechecks receiver fencing and deadline at execution admission, including
time spent in IPC and native queues. A partially written or unacknowledged command
is retained as uncertain and triggers the independent declared fault response.

`device.result` reads the retained receipt and optional qualified evidence. It
does not resend, reconcile, retire unknown debt or rearm. `device.stop` uses its
own sequence and stop grant, latches admission and coalesces the same target stop
without consuming normal capacity. `device.describe` returns only cached support,
state, watchdog and finite reservations. Describing never polls hardware or renews
ownership. `ready` requires available, armed watchdog evidence.

The current scalar/device/arbitration/channel cores provide useful implementation
building blocks. They do not by themselves qualify a physical driver, an
autonomous watchdog or these guest imports.

## Continuous control procedure

`control.open` independently requires the exact device submission and stop
authority and complete source/dependency closure. It prepares exclusive ownership
under immutable priority, preemption, mode and finite lease policy. Only complete
result delivery commits the cycle/epoch. Failed publication rolls back prepared
ownership. Receiver fencing still must be confirmed before actual commands.

Preemption is either denied or admitted after the declared observed-stop handover.
A higher priority does not bypass safety predicates. Lease expiry, authority epoch,
controller epoch, source incarnation and device boot generation are different
values and cannot substitute for one another.

`control.sample` reserves one complete input phase with its original pins, active
map/calibration/tool/energy/safety dependencies, sequence, epoch and write deadline.
A failed copy cannot advance that sequence. `control.submit` must refer to that
exact phase; changing the schema, epoch, input identity or active dependency is
rejected. The sample and receiver-side deadline cover queuing and execution
admission, not just guest call duration.

`control.result` reads actual device completion for the submitted phase. The next
sample acknowledges only a qualified terminal phase. Unknown completion retains
the phase and latches the fault path. `control.stop` has an independent sequence
and reservation; it neither renews control nor rearms the latch.

High-frequency servo loops may run within a qualified isolated controller using
a bounded admitted program and local watchdog. The host manages ownership,
configuration, constraints, telemetry and stop. This avoids requiring one generic
VM RPC for every servo tick while preserving the execution authority boundary.

## Media procedure

`media.open` fixes decode, encode or transform mode, approved codec, input layouts,
output layout, source provenance, metadata/loss policy and optional exact egress.
It independently checks source observation and output buffer creation/write/seal
authority. Optional egress also requires the exact destination action and original
source export authority. Opening a stream cannot implicitly create a listener.

`media.submit` owns a sealed input reference and a concrete `media_frame`. Raw image
and audio planes have checked positive dimensions, row extents and strides;
their bounding intervals cannot overlap. The approved layout validates actual
channel/sample sizes, color/chroma, endianness and audio sample format/rate. No
SIMD implementation may read beyond the actual pinned extent.

Encoded packets contain no raw-plane assertion. A codec-specific parser checks
the header and declared expansion; the complete maximum output and workspace are
reserved before processing. Untrusted codecs run in a qualified isolated worker.
An invalid expansion cannot be handled by allocating beyond the admitted budget.

`media.take` delivers one complete sealed output with original source, processing
generation, distinct capture/receive times and explicit losses. It requires the
original output read/export grants. A bad copy preserves both item and buffer
identity. `media.close` fences input, then drains or cancels owned work under a
finite deadline. It has independent sequence/capacity, retains unknown egress
effects and never closes its input source or unrelated viewers.

## Bridge procedure

`bridge.create` validates the complete graph before enabling any edge. Every
nonzero slot appears once, in increasing order, and binds independently admitted
source and destination capabilities with their actual ancestor chains. Every
declared mandatory role is supplied; extra roles are rejected. A reverse direction
is an explicit separate edge. Conversion/type/QoS profiles are immutable.

ROS1 and ROS2 protocol generations remain explicit because they are properties
of the upstream protocols. The bridge specifies conversion loss, declaration and
parameter atomicity, action/service correlation, clock domains, disconnect and
cancel propagation. Unsupported native semantics return unsupported; a sequence
of non-atomic operations must not impersonate an atomic operation.

`bridge.start` commits its receipt before enabling the flow. Each forward rechecks
original origins, source export, destination action and destination semantic
predicates. Bridge authority does not turn a model proposal into a device command.
Loop identity, hop/lifetime bounds, seen-set capacity and retained correlations
are bounded. Reconnection cannot erase unresolved forwarded effects.

`bridge.stop` fences new forwarding and drains/cancels only owned work with its
independent sequence and reserved stop path. It preserves unknown remote outcomes
and source acknowledgement debt. `bridge.describe` reads cached graph state,
counts and the last receipt; it does not discover additional endpoints or forward
traffic. ROS1 integration remains a planned provider, not a legacy-loader fallback.

## Isolated component procedure

`component.open` selects an immutable executable/library/data closure, configuration
schema, dependency graph, health/stop/restart rules, message protocol and role
bindings. It separately authorizes the exact configuration artifact and every
role's target action. The guest supplies no executable path, argv, environment,
symbol, loader flag or unrestricted filesystem root. Opening publishes only an
inert handle and retained immutable configuration.

`component.start` records its keyed request before launching. A trusted platform
launcher applies the frozen isolation, CPU/NUMA, native process/thread, memory,
scratch, handle, IPC and stderr limits before untrusted code executes. Readiness
requires an authenticated generation and actual verified required guarantees;
process existence alone is insufficient.

Child messages contain only manifest role slots and bounded canonical data. The
supervisor authenticates the channel/generation and resolves that role's original
capabilities. It rechecks current authority and destination predicates on every
effect. Messages cannot carry new guest capability integers or call arbitrary host
methods. The VM never loads the child's native library.

Approved co-trusted components may share one explicitly selected worker process.
They then share address space, budget and failure domain. Distinct trust domains
require distinct qualified isolation. A shared worker is not a zero-cost or
independently isolated deployment of its individual libraries.

`component.stop` fences child requests before shutdown, uses independent stop
capacity and applies only the configured termination escalation. Positive
process-tree exit and IPC retirement are required before releasing resources.
The cached `component.describe` record cannot assert `exited` without exit evidence
or report live processes after observed complete exit. Exit does not establish
physical safe state. Unknown downstream effects can outlive the entire process.

## Deployment, resource placement and platforms

The family profile, definition and limit patterns in the machine-readable design
are implemented by the concrete CLI options in [robot bindings](ros-robot-bindings.md)
and the [generated CLI reference](../command-line/ros-scene-cli-catalog.md).
Profiles and limits are validated and frozen before activation. Static checking
starts no child, device or listener.
Unknown/duplicate fields, unsupported guarantees, unresolved profile references,
arithmetic overflow and dependency cycles fail. All capacities are explicit and
finite; zero enables no dependent workflow. Profile identity and the actual file
content are authenticated separately, eliminating mutable-path substitution.

Account for every process's image/runtime state, page tables, stack, native threads,
IPC buffers, pinned shared pages, codec workspace, retained requests, recovery debt
and shutdown traffic. Shared pages are not counted as freed while a borrower keeps
them. A cumulative CPU-time limit is not periodic bandwidth or a deadline guarantee.
Guest I1T permission is separate from native worker threads and remains managed by
its own CLI/policy implementation.

CPU affinity, SMT sharing, NUMA placement, real-time scheduling and exclusive-core
reservation are separate claims. The platform reports actual effective settings,
not merely requested values. First-touch and existing resident pages are accounted
for; shared mappings may still cause remote memory traffic. A topology/cpuset change
fences any claim that is no longer verified. Required-but-unavailable guarantees
refuse activation. Explicit inheritance reports inherited policy without claiming
exclusive or deterministic placement.

Qualification is per ROS distribution, OS release, architecture and RMW, rather
than a single timeless list of Linux/Windows/macOS targets. This follows the
platform model in [REP-2000](https://raw.githubusercontent.com/ros-infrastructure/rep/master/rep-2000.rst).

Linux launchers must qualify the actual sandbox, process-tree accounting,
directional handles, immutable image closure, affinity/memory policy and shutdown
evidence in the target environment. An inherited cgroup boundary may not be
widened to satisfy a failed request. Existing Linux-specific workers remain
separately scoped evidence, not a general platform implementation.

A Windows launcher needs distinct resource and security mechanisms. Job objects
can manage process groups, limits and termination, but security restrictions are
separate; child breakaway and incomplete notifications must be explicitly handled.
See Microsoft's [job object contract](https://learn.microsoft.com/en-us/windows/win32/procthread/job-objects).
The selected restricted identity/AppContainer and exact handle ACL policy require
independent qualification; a job alone is not the sandbox. Microsoft documents
[AppContainer isolation](https://learn.microsoft.com/en-us/windows/win32/secauthz/appcontainer-isolation)
as a resource/identity boundary.

A macOS launcher needs a qualified helper identity, entitlements, explicit brokered
resource access and actual exit/placement evidence. Apple's
[App Sandbox](https://developer.apple.com/documentation/security/app-sandbox)
restricts resources through entitlements; this does not establish this project's
entire process, timing or device contract. Linux translation/emulation can test
portable protocol logic only. It is not macOS sandbox or real-time qualification.

Existing `preloaddl` and deprecated ROSv1 warnings retain the established condition
and warning-to-error path. ROSv2 never silently opts into unisolated loading or
broad WASI access. WASIp1/ROSv1 bug fixes remain separate from this new design.

## Domain-specific semantics and qualification

The [26-domain map](ros-robot-capabilities.md) is a boundary inventory. Specific
robot skills use immutable typed profiles on these operations. Separate aliases,
targets and validators distinguish a planning service from navigation execution,
map storage from active-map replacement, camera reads from exposure control, or
power telemetry from charging/firmware maintenance. They do not inherit a broad
permission because they share a ROS service or action carrier.

Changing an active map, tool, calibration, estimator, safety envelope or power
state requires a domain-specific activation procedure with expected active
revision, allowed mode, qualified content checks and fenced dependent execution.
Atomicity is scoped to the actual provider; cross-device/distributed activation
must declare partial failure and compensation. A task's compensation itself needs
an explicit grant and cannot be assumed safe or reversible.

Human teaching/teleoperation additionally authenticates operator, enable state,
mode and lease. Fleet/door/elevator operations additionally validate exact peer,
reservation and handover. A model output remains inert until independent admission.
Simulation/replay never restores historical physical permission. Maintenance
cannot replace firmware or weaken safety limits through a generic artifact write.

For each proposed binding, acceptance must cover denial before effects, wrong
environment/kind/generation, malformed and maximal records, complete output
preflight, allocation failure, lost copies, exact replay, concurrent consumers,
revocation/cancellation races, queue saturation, stale sources/dependencies,
deadline expiry, partial transport, worker crash, restart and retained debt.
Continuous control additionally needs delayed old-owner commands, saturated stop
traffic, clock pause/rewind, missed phases, topology changes and qualified rearm
tests. Device claims need actual target or explicitly labelled simulator evidence.

Performance tests use the same payload, security, QoS, batching, CPU topology,
thread/process counts and NUMA conditions for native and VM. Build every measured
component with **`-O3 -g`**. Record cold/warm and tail latency, queue residence,
context switches, CPU, memory, loss and stop latency under competing model/log/
recording workloads. Use official VTune hardware Hotspots to frozen C++ source
and Microarchitecture events to actual instructions; retain unresolved samples.
All tests/profilers stay in the existing 64 GiB zero-swap cgroup.

Current request/result qualification covers all 44 designed shapes, truncation,
aggregate budgets, 128-slot bindings, plane overflow, contradictory result states,
allocator refusal and concurrent borrowing. The concrete providers add their own
execution and recovery tests. Windows/macOS launchers, complete package audits,
functional-safety certification and near-native performance each remain a
separate evidence gate.
