# Robot capability domains and isolated execution

ROSv2 is intended to mediate the robot application's external effects, including
driver calls, storage, motion, inference and supervision, with process ownership
remaining in the deployment layer. The six groups
and 26 domains below are an architectural coverage contract, not an official ROS
classification, a request for 26 processes, or a statement of implemented support.
The [operation catalogue](../../tools/ros_scene/interface-catalogue.json) and
[legacy coverage ledger](ros-legacy-coverage.md) remain the implementation and
migration inventories. Pure algorithms and message definitions do not require a
new host import for each package.
The [complete operation reference](ros-operation-reference.md) and
[provider procedures](ros-robot-provider-procedures.md) bind this inventory to
concrete request/result fields and failure/authority contracts. Machine checks
require a design row for every catalogue operation and retain each operation's
actual registration status. They do not convert a domain mapping into package
portability or physical assurance.

## Managed processes and external ROS peers

The target deployment manages multiple `uwvm2-ros` processes, each containing
one or more Wasm modules, and separately managed native driver processes.
Every VM process and every managed driver incarnation has its own resource
leaf. A separate recovery service retains its own admitted resources outside
the application's failure domain. `wros-deployer` is not implemented; these
are architecture and qualification requirements, not current deployment claims.

Within a VM, modules retain separate identities, private memory ownership,
capabilities and resource accounts. They are not separate OS processes or
independent memory cgroups. The VM's cgroup bounds their aggregate native and
guest consumption. A VM crash or process-level OOM can affect all modules in
that process; applications needing that additional failure separation must
use different VM processes. Sharing a permission group does not share private
memories or confer another module's authority.

The deployment, dependency and authority graphs are distinct:

- The deployment graph owns VM and driver process creation, termination,
  replacement, resource placement and recovery. The VM itself has no driver
  process-control interface, including an indirect privileged lifecycle RPC.
- The dependency graph relates many Wasm modules in many VM processes to many
  driver instances. One driver may serve several VMs. A dependency neither
  authorizes a call nor transfers process ownership; missing providers report
  unavailable rather than triggering a VM-owned launch.
- The authority graph grants exact principals permission to connect, inspect
  permitted state and perform typed operations. Each admitted connection is
  bound to deployment, VM incarnation, module incarnation, driver incarnation,
  protocol and resource scope. Guest-supplied module names are not credentials.

Shared-driver requests must retain their originating module identity across
the VM boundary. A trusted broker may multiplex an authenticated connection,
but it must enforce per-principal admission and stamp the origin from runtime
identity. Connection credentials cannot be exported into guest memory. The
external entry point must reject forged origins, stale generations and replayed
requests; a shared socket must not collapse all modules into one authority.
Buffers, outstanding requests, subscriptions, diagnostics, cancellation and
replies remain bounded and attributed to their respective principal.

Read-only sensor fan-out permits several explicitly authorized consumers.
Actuation needs common arbitration across every VM that can reach the same
resource. The trusted admission service or independently qualified receiver
must enforce exclusive control leases for each actuator or coordinated resource
set, including epoch fencing and handover. Independent checks inside each VM
cannot establish global exclusivity. An arbitrary native vendor library is
not sufficient evidence of a trusted arbiter. Concurrent writes require an
explicitly qualified composition policy; they are not the default consequence
of multiple clients having transport access.

One client's closure, revocation, exhausted queue or crash must not terminate
the shared driver or erase another client's pending results. Stop and lease
expiry follow the affected control resource and its declared dependency scope;
they are not an unrestricted broadcast kill. Updates and driver crashes affect
every dependent connection, so the deployer must drain or fence that dependency
closure, retain unknown outcomes and admit a new generation before reconnecting.
Automatic reconnection never renews a motion lease.

An external ROS network, Python application or separately operated service may
remain outside deployment ownership. ROSv2 connects through explicitly approved
protocol roles, peer identities, topics/services/actions, directions, schemas,
QoS, freshness and budgets. The external owner remains responsible for its
installation, runtime, cgroups, restart and updates. Unmanaged does not mean
trusted: external data and model proposals remain untrusted until validated and
independently admitted for an effect. No foreign network grants arbitrary host
sockets, device control, process enumeration or automatic discovery trust.
An external peer's asserted ROS name alone is not authentication.

Resource-isolation and recovery guarantees stop at the managed boundary.
External overload, loss, compromised identities and stale data require explicit
refusal/fault behavior; remote resource isolation is not inferred from protocol
compatibility. See the
[attachment-only driver contract](ros-native-driver-lifecycle.md#attachment-only-driver-ownership)
for process ownership and the distinction between provisioned channels and
Linux cgroup/namespace containment. Model and communication providers now use
external attachments too; each test simulates deployment with its own script.
Current in-process module-exchange receipts do not qualify inter-process
multi-client driver attachment or arbitration across several VM processes.
The separate [driver attachment qualification](ros-driver-attachment-qualification.md)
now records two real VM clients sharing a MuJoCo receiver, with competing-owner,
stale-epoch and copied-epoch rejection, handover and independent connection
release. That bounded fixture does not complete the broader matrix below.
Qualification of that path must include multiple modules in each of two VMs
sharing a sensor and an actuator: forged module identity, concurrent control
acquisition, stale epochs, one client's queue exhaustion, client/VM/driver
failure, independent result cleanup, and delayed replies during replacement.
No test may infer safe shared actuation from successful concurrent reads.

These relationships use the same registered typed-call surface and per-resource
profiles; they do not require a new Wasm import for every client/driver pair.
Adding a dependency registers an intended route, not an automatic capability.
Platform extensions continue to describe narrowly scoped permissions and
constraints, with every vendor implementation remaining outside the VM.

## Capability domains

Each domain separates observation, computation, execution, configuration and
management where those actions exist. A grant selects an exact operation,
resource, direction and immutable provider profile. A domain name does not grant
all of its operations. Unsupported capabilities on a particular robot cannot be
granted merely because the domain exists in this catalogue.

Platform and authority:

1. **Identity, credentials and policy.** Authenticate runtime generations and
   exact external peers; grant, delegate within an explicit ceiling, inspect and
   revoke separately. Private credential use does not imply credential export.
2. **Compute, resources and scheduling.** Reserve memory, CPU/GPU work, queue
   capacity and deadline budgets; select approved placement and scheduling
   profiles. Starting inference does not imply real-time priority or exclusivity.
3. **Storage, data and configuration.** Read immutable objects; create, append,
   commit, abort, retain and remove bounded transactions. Storage publication is
   separate from activating a map, calibration, model or robot limit.
4. **Network, cloud and external services.** Bind protocol, peer identity,
   direction, schema and data-export scope. A model request, telemetry upload and
   driver channel are different capabilities, not arbitrary sockets.

ROS and hardware access:

5. **ROS communication, graph and types.** Topics, services, actions, parameters,
   discovery and schema inspection retain their complete origin chains and QoS
   obligations. Transport permission alone never authorizes physical execution.
6. **Node and component lifecycle.** Creation, configuration, activation,
   deactivation, shutdown, callback completion and isolated provider replacement
   have distinct effects and recovery receipts. Logical ROS node transitions
   are distinct from native driver process control. Native driver creation,
   termination and replacement belong exclusively to the deployment layer;
   a provider that cannot separate those effects must refuse that VM operation.
7. **Hardware and buses.** Bind a reviewed device, protocol, direction and command
   schema. Drivers run outside the VM; no guest-selected library, raw device file
   or universal bus command bypasses that boundary.
8. **Sensors.** Read streams with trusted source identity, acquisition time,
   calibration, frame and loss semantics. Exposure, rate, synchronization and
   power changes require separate configuration authority.
9. **Robot description, physical model and calibration.** Inspect and validate
   geometry, kinematics, dynamics, tools and calibration; activate an approved
   revision only in an allowed operating state.

Time, state and environment:

10. **Time, coordinates and synchronization.** Distinguish steady, wall and
    simulated clocks, epochs, frame histories and synchronization uncertainty.
    A simulated clock cannot extend a physical control lease or watchdog.
11. **Perception.** Consume explicitly authorized observations and publish typed
    detections or scene estimates with provenance and validity bounds.
12. **Estimation and localization.** Report pose, velocity, uncertainty and health;
    separately authorize resets, relocalization and trusted state injection.
13. **Mapping, maps and world state.** Query, propose changes, commit revisions and
    activate them separately. Plans reference immutable dependencies and fail or
    revalidate when relevant revisions change.

Motion and intelligence:

14. **Motion execution.** Admit reviewed trajectories, joint/base commands or
    bounded controller programs against current control ownership, mode, units,
    limits, observations, versions and deadlines. Feedback distinguishes accepted,
    started, stopping, observed stopped and terminal outcome.
15. **Navigation and route planning.** Read the required state/map and compute
    plans without automatically gaining execution or facility-access authority.
16. **Manipulation and whole-body planning.** Plan contact, tools, grasps, gait or
    coordinated body motion; explicitly acquire every execution resource.
17. **Tasks, skills and behavior.** Compose separately authorized capabilities;
    retain task/session identity, compensation obligations and recovery state.
18. **Models, inference and agents.** Isolate approved model runtimes, authorize
    each input and output destination, and treat proposed tool use as inert data
    until an independent authorized decision admits the exact operation.

Human interaction and safety:

19. **Human interfaces, teleoperation and teaching.** Authenticate the operator,
    bound device and mode. Teaching, manual execution and recording are separate
    capabilities; reconnecting an old session does not restore control.
20. **Control ownership, arbitration and modes.** Resolve conflicts under explicit
    policy, fence old owners with generations, and make handover observable.
    Acquiring control cannot add missing operation or resource permissions.
21. **Safety supervision, faults and recovery.** Reserve independent stop/fault
    capacity, supervise freshness and deadlines, latch uncertainty, and require
    trusted evidence before rearming. A software acknowledgement is not evidence
    of a physical safe state or a replacement for hardware protection.
22. **Energy, power and thermal state.** Read health and budgets; separately
    authorize power transitions, charging and operating-envelope changes.
23. **Fleet and facilities.** Coordinate robots, traffic zones, doors, elevators
    and shared infrastructure with exact peer/resource identity and explicit
    handover and communication-loss behavior.

Engineering and operations:

24. **Observability and audit.** Correlate grants, requests, model proposals,
    control decisions, effects and failures. Inspection is bounded and redacted;
    logging or debugger access does not confer execution authority.
25. **Simulation, recording, replay and validation.** Preserve provenance and
    clock domains. Replayed commands remain in an explicitly separate domain and
    cannot acquire physical actuation authority through a historical recording.
26. **Build, release, supply chain and maintenance.** Pin source/artifact identity,
    qualify platforms, stage updates, activate approved revisions and recover
    after failure through the independently authorized deployment workflow.
    VM observation or a proposal does not execute native driver management.
    Installing an artifact does not authorize its execution.

## Effects cannot be hidden behind ROS transport

A deployment must classify the complete destination behavior of every writable
topic, service, action, bridge and plugin route. A familiar message type, topic
name or an apparently harmless string payload cannot establish that a route is
nonphysical. The classification covers downstream consumers, bridges and active
configuration changes. An unknown effect is not classified as harmless.

Physical execution and safety-relevant configuration require all of: an exact
operation grant, current control ownership where applicable, an allowed mode,
validated command units/frame/schema, fresh trusted state, current dependencies,
and the deployment's safety predicates. Recheck these at the external effect
boundary. A communication queue acceptance does not satisfy them. The existing
communication receipt's `none`, `locally_accepted`, `observed_remote` and `unknown`
values describe an observed outcome, not this deployment classification.

Revoking a permission, revoking control, cancelling a task, requesting a safe
transition, observing that transition and asserting a hardware emergency stop
are different events. A partially written native command or lost reply remains
uncertain until independent evidence resolves it. Restarting a worker cannot
erase the old request or permit automatic replay of a non-idempotent effect.

These rules are prerequisites for exposing effectful communication and device
providers. They are not yet a claim that the registered local bindings implement
an integrated motion supervisor or that all planned transports enforce them.

## Process boundaries, IPC and latency

Process placement follows trust and failure boundaries. Approved components may
share one qualified worker when the deployment explicitly accepts their shared
address space, failure domain and resource budget. Different trust domains do
not share a native process. Guest code never selects `dlopen` paths in the VM.

The normal path reserves finite request, reply and recovery storage; owns or pins
all input before admission; publishes one exact operation; performs bounded
nonblocking transport work; and releases authority locks before waiting. Copying
and reply delivery reacquire the original authority. Native workers cannot turn
a response payload into a new host command or grant. Stop/fault traffic has
separate reserved capacity and cannot depend on a saturated inference/log queue.

Isolation introduces additional address spaces, private runtime state, page
tables, scheduler transitions, serialization, copies, cache/TLB pressure and
possibly remote memory access. Count these costs explicitly. Sealed shared
buffers can reduce copies only where immutable lifetime and reader authorization
are enforced; they do not remove coherence costs or make shared pages NUMA-local.
Do not describe the current pipe transport as zero-copy.

High-frequency control may stay inside a qualified isolated controller with a
bounded program and a local watchdog. Admission, configuration, ownership,
telemetry and supervisory stop cross the host boundary. This does not require a
synchronous VM-to-worker round trip for every servo cycle. The controller must
independently enforce session fencing, command validity and its robot-specific
failure behavior when the VM is late or absent.

## Placement and resource guarantees

The external deployer owns CPU affinity, NUMA policy, cgroup membership,
namespace construction and any real-time scheduling policy for every VM and
native provider. The VM attaches to preprovisioned descriptors; a cgroup path is
neither a transport address nor permission to move a process. The VM never
creates a child cgroup or enters a provider namespace to make a connection.

The repository's execution_placement.h and native sandbox launcher are retained
as test/deployment components. Their behavior must not be described as a VM
provider feature. Test scripts may apply explicit CPU affinity within the
existing 64 GiB gate. They do not constitute independent per-provider memory
reservations, core exclusivity or a production deployment service.

Deployment qualification must account for SMT siblings, IRQ/kernel work, other
processes, scheduler class/priority, first-touch placement and memory bandwidth.
Affinity is not core exclusivity. Cumulative CPU-time limits are not periodic
CPU-bandwidth reservations. Pipe IPC and additional processes add scheduling,
copy, cache and TLB costs; benchmark with the same placement on both sides.

CPU/node topology changes, changed cpuset constraints and modified NUMA policies
require external detection and requalification before continuing a timing or
locality claim. Privileged administrators remain in the trusted deployment
boundary. Linux fixtures do not qualify Windows/macOS scheduling or sandbox
behavior. The current attachment implementation and receipts explicitly state
which operating system and CPU architectures were actually exercised.

## Qualification obligations

Measure equivalent native and VM workloads with the same payloads, QoS, thread
counts, CPU placement, first-touch policy, dependency versions and deadline
semantics. Report first-call and warm-state results separately, including
throughput, median/tail latency, worst observed latency, queue residence, CPU
time, memory footprint, context switches and loss. Compare same-core,
different-core, SMT-sibling and cross-NUMA cases only where the actual machine
can provide those arrangements. A single-node machine cannot qualify remote
NUMA behavior. Include control work under model, sensor, recording and log load.

Use `-O3` and full `-g` for the VM, guest and native reference. VTune Hotspots
must resolve frozen C++ sources; Microarchitecture reports must retain hardware
events and actual assembly attribution. Keep unresolvable samples explicit.
No near-native or industrial safety claim follows from an isolated microbenchmark.

Acceptance also requires control-owner handover with delayed old commands,
driver crash/disconnection, lost acknowledgements, exhausted ordinary queues,
simulated-time pause/rewind, stale map/calibration/tool dependencies, replayed
motion data, restart during a transaction and failed deployment updates. Verify
robot-specific stop/hold/recovery behavior with physical evidence where required.

The ecosystem ledger must record package/repository/distribution identity,
source and artifact hashes, domain roles, dependencies, resources, effects,
platforms, recovery behavior and actual evidence. Indexed names and downloaded
sources are not completed semantic reviews or portability tests. Coverage is
complete only when every relevant external effect has an implemented, qualified
route or an explicit unsupported result, with unresolved gaps reported.
