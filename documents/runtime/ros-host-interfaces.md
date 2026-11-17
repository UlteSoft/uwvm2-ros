# ROS host interface design

This is the implementation contract for the breaking ROS host redesign. A
designed operation is not a claim that its native provider or an upstream ROS
package has passed qualification. The machine-readable operation inventory
records implementation and verification separately.

## Names and compatibility

The new scene-oriented namespace is **`uwvm_ros_v2`**. It combines portable
robot workflows, explicit ROS1/ROS2 provider bindings and bridge operations.
It does not re-export or alias WASIp1 or the existing ROS namespaces.

The entire legacy ROS VM ABI family is **deprecated**: `uwvm_ros1_v1`,
`uwvm_ros1_client_v1`, `uwvm_ros2_v1`, `uwvm_ros2_client_v1`,
`uwvm_ros2_rmw_v1`, `uwvm_ros2_rmw_sub_v1`, `uwvm_ros2_rmw_srv_v1` and
`uwvm_portable_robot_v1`. All their imports and `--ros1-*` / `--ros2-*`
deployment options carry this status. Unreleased legacy ROS and WASIp1 receive
bug fixes, including corrections that break faulty semantics; they are not
compatibility targets for new ROSv2 features. Enabling legacy ROS uses the
conditional `deprecated-ros` warning policy. New ROSv2 contracts may change
without compatibility aliases while the redesign remains unreleased.

ROSv2 builds independently with `--import-u2rosv2=y` (the default).
`--import-u2rosv1-ros1`, `--import-u2rosv1-ros2` and `--enable-preload-dl`
default to `n`; retained legacy code is restored only by an explicit rebuild.
WASIp1 remains compiled, with no default guest imports or resource authority.

New robot deployments target `uwvm_ros_v2` with explicit `--ros-scene-*`
configuration. Consult the [provider implementation inventory](ros-scene-provider-contracts.md)
before migrating an operation: a planned provider is not an implemented
replacement. Import names cannot be mechanically translated, and legacy grants
confer no ROSv2 authority. This is deprecation of the VM ABI, not of the ROS1
ecosystem or ROSv2's ROS1 bridge design. WASIp1 retains its existing status and
contract for separately authorized general-purpose libraries.

## Implementation ownership

The legacy implementation spans both repositories. This VM repository owns
`src/uwvm2/uwvm/imported/ros/host.h`, capability policy and the
`src/uwvm2/imported/ros/ipc/` protocol/client machinery. The SDK repository owns
Wasm language/platform bindings **and** the native gateways under
`src/gateway/ros1`, `src/gateway/ros2` and `src/gateway/common`. Those gateways
translate the VM IPC operations into ROS APIs; the SDK is not exclusively a
collection of guest headers. ROS/DDS transport implementations remain upstream
dependencies of the selected native provider.

ROSv2's host boundary is `src/uwvm2/uwvm/imported/scene/host.h`. Its owned wire
contracts, authority, process supervision and native worker adapters are in
`src/uwvm2/imported/ros/scene/`, including `native/communication_ros2.h` and the
worker/transport protocol headers. Keeping those sources in the VM repository
does not put untrusted native code in the VM process: deployment selects the
approved isolated worker closure. SDK production ROSv2 migration remains a
separate task; an existing ROSv1 SDK binding does not automatically call ROSv2.

The [legacy coverage contract](ros-legacy-coverage.md) inventories the actual
eight-module, 124-import legacy boundary and separates source coverage,
semantic obligations, registration and native differential qualification.
ROSv2 is the complete intended robot capability boundary: drivers and native
components are deployment-managed typed message providers, not guest-selected
shared libraries or generic socket/filesystem adapters.
The [native driver boundary](ros-native-driver-isolation.md) distinguishes
executable, shared-library/plugin and static-SDK hosting, and records the
current implementation limits. The [deployment boundary](ros-deployment-boundaries.md)
separates ROSv2 workflows, POSIX compatibility, multi-ROS security and evidence
required before a deployment can be qualified.
The [robot capability domains](ros-robot-capabilities.md) define six groups and
26 responsibility domains, including control ownership, physical execution,
safety supervision, deployment recovery and the costs of process isolation.

Use a small number of semantic categories: `topic.publish`, `parameter.get`,
`event.stream.poll`, `clock.read`, `wait.poll`, `recording.append`,
`model.submit`, `bridge.create`. A direct name is appropriate if unambiguous;
extra nesting must distinguish actual semantics. Native ROS generation is a
provider binding, never a version suffix on individual operations. Internal C++
symbols use descriptive names. All new public declarations require Doxygen
`@brief` and `@details` documenting admission, effects, cancellation, quotas,
ownership, failure/partial-completion and inspection. Design inventory entries
must not be advertised as registered implementations.

## Scope and semantic mapping

The target is portable expression of ROS semantics, not loading arbitrary
native machine code into a Wasm process. The coverage unit is a public semantic
operation and its observable behavior. Package discovery, dependency closure,
public API compilation, execution, and hardware acceptance are separate units.
No package coverage percentage is inferred from host-function counts.

The reference inventory covers rcl, rmw, rcl_action, rcl_lifecycle, rclcpp,
rclpy, rosidl typesupport, tf2 and ROS1 roscpp/rospy/actionlib. Container helpers,
C++ templates, allocators and pure algorithms execute in Guest libraries.

VM-owned scene allocations use `fast_io` allocator adapters, including owned
strings/containers, smart-pointer objects and shared control blocks. The normal
terminate-on-failure allocator and the recoverable silent/strict allocator are
different contracts. Industrial operation paths use
`uwvm2::utils::allocator::fast_io_strict` and its `utils::container` adapters:
allocation failure is reported to the host boundary and unfinished work rolls
back, rather than terminating the VM. Default standard-library allocators,
allocating `new`, `std::make_shared`/`std::make_unique` and owning
`std::function` are not used for new VM-owned scene state. Borrowed synchronous
callbacks use the existing `function_ref` adapter. Native ROS/LLVM factory and
destructor pairs mandated by external ABIs remain paired; their source is not
rewritten. This policy does not introduce a global allocation-mode switch.

Qualification injects failure at the actual malloc-family backing used by the
recoverable adapter, checks retained capacity and transaction rollback, and
does not rely solely on overriding `operator new`. Performance qualification
uses both `-O3` and full `-g` in the effective compiler arguments for the VM,
native reference and Wasm guest. Line-tables-only builds remain historical
evidence. Hotspots must resolve to frozen C++ file/line data, and
Microarchitecture measurements must retain hardware events joined to actual
instructions and their DWARF source frames. Unknown JIT/module samples remain
explicitly unattributed; sampling estimates and skid are not exact instruction
latencies.
Native pointers, function addresses, arbitrary dlopen, unrestricted discovery,
environment inheritance and device access are replaced by explicit handles,
reviewed deployment bindings and separately granted operations.

## Authority and ownership

The authority progression is **zero authority -> explicit elevation -> stable
granted authority -> explicit attenuation -> potentially uncertain execution
guarantees**. Elevation grants an exact resource/operation scope. Attenuation
never adds scope; it applies budgets or restrictions to an already granted
capability. Unspecified optional quotas add no throttling after authorization,
while a finite zero is an actual zero allowance. Stable granted authority is
not a promise that physical providers can never fail.

Authority and execution guarantees are separate state. Admission must report
when a configured restriction can affect delivery completeness, timing or
continuity. Stream drop/sampling, reject-before-start, wait-before-start and
stop-during-execution are distinct policies. Operations requiring atomic or
continuous execution reserve their admitted budget before starting. Runtime
attenuation cannot silently rewrite an already accepted execution contract;
the deployment selects drain, cancel or a defined safe-stop transition.
Model predictions never upgrade either authority or execution guarantees.

Every interface starts hidden. Enabling a namespace grants no function, resource,
node, clock, model, tensor, filesystem, network, environment or device authority.
Interface `allow` and a matching resource capability are independent checks.
Deployment creates named environments and binds exact modules. Several modules
may explicitly share one environment; there is no ambient Global fallback.
Existing ROS1, ROS2 and WASI policy do not grant scene authority. Model, device,
bridge and storage grants are independent within a scene policy environment.

Every resource reference is an opaque nonzero integer. Its trusted registry
records kind, environment, backend, generation, creating grant and lifetime.
A reference is never a host address or a transferable authorization token.
Cross-environment use, wrong kind, stale generation and exhausted identifier
space fail before effects. A Group may share only its own explicitly selected
environment. Cross-module ancestry is currently rejected by the scene entry.
A future delegation contract requires separate review; passing a resource
integer does not delegate permissions.

Creation, observation and mutation use explicit operations. Parent destruction
retires descendants and wakes their waiters. Revocation prevents new admission,
invalidates queued deliveries as specified by their source policy, and cancels
owned pending work. Physical provider loss does not resurrect resources through
automatic reconnect. Retry of a side-effecting request requires an explicit
idempotency contract; a timeout is not proof that the side effect did not occur.

## Contexts, nodes and names

Contexts bind a deployment-approved ROS domain/master, middleware, security
identity and discovery configuration. Nodes bind a context, resolved basename,
namespace, parameter overrides, remapping rules and logging settings. One policy
environment can own multiple bounded nodes without merging their identities.

Name expansion and remapping finish before the exact resource/type/operation
authorization check. The effective resolved names are available in diagnostics.
Parameter-file content is parsed as data by the trusted launcher; a filename
does not grant Guest filesystem access. Package resources use explicit logical
resource handles rather than ambient search paths. Node/context shutdown is
observable and cancellation-safe. Context shutdown never invokes process exit.

## Communication and type identity

Publishers, subscriptions, service clients/servers and action clients/servers
are independent resources. Their type descriptors come from a trusted registry
with full dependency closure and exact upstream identity. Generic serialized
traffic is allowed only for a registered type under its byte, element, nesting,
encoding and content constraints. ROS1 serialization and ROS2 serialization are
distinct advertised encodings. No untrusted type name selects native code.

Messages carry source and reception timestamps with presence bits, publisher
identity, sequence information and request/goal correlation where applicable.
Metadata and body are reserved, copied and committed together. Unavailable
metadata remains absent; the VM never fabricates native timestamps or GIDs.
Actual QoS and middleware-supported capabilities are queryable independently of
requested QoS. Endpoint changes and DDS status events are observable.

All variable-output reads use one rule: an undersized or invalid output does
not consume the record. Required length is reported only through a validated,
nonoverlapping output slot. Success commits exactly the reserved record.
Peek does not permit another consumer to steal a reservation. Cancellation and
reservation expiry have explicit outcomes. No native callback retains a Guest
pointer or enters Guest code. Buffers are copied through the existing runtime
memory admission API, including memory64, retirement and memory-tag checks.

## Waiting, clocks and scheduling

A wait set contains authorized resource references and explicit interest masks.
Subscriptions, services, clients, actions, timers, guard conditions, graph
changes, QoS events, clock changes, model output and resource closure use the
same readiness contract. Membership changes and notifications increment an
epoch under the same synchronization as readiness checks. Registration and
sleep are atomic with respect to notification, avoiding lost wakeups.

Waiting observes readiness without consuming messages or timer occurrences.
Multiple waiters, detach, destruction, revocation, process exit and interruption
have defined results. A wake includes the source reference, interest mask and
generation; it grants no source access. Deadlines are monotonic and overflow
checked. Infinite waits require an explicit deployment allowance and remain
revocable. Callback groups and executor scheduling are Guest policy; the VM
provides cancellation-safe waits and does not run application callbacks on
native ROS threads.

Clocks distinguish steady, system and ROS time. ROS time can be unstarted,
paused, advanced or moved backward; clock jumps and source changes are events.
An authorized clock source updates a clock through its own capability. A timer
binds one clock and has explicit one-shot/periodic, reset, cancel and catch-up
semantics. Missed periods are reported, never replayed as an unbounded burst.
Signed time and duration arithmetic is checked. Host security deadlines and
quotas use steady time even when ROS time pauses or moves backward.

## Services, actions, parameters and lifecycle

Service submission returns an owned request identity. Availability, response
readiness, response delivery and local abandonment are separate operations.
Abandonment releases local waiting resources and does not claim remote
cancellation. Servers receive the exact request identity and may reply once.

Action goal proposal, accept/reject, deferred execution, status, feedback,
result, cancel proposal and cancel response are distinct transitions. UUID and
timestamp correlation are retained. Accepting cancellation is not terminal
completion. Result retention has a configured bound. Preemption policy belongs
to the application and uses explicit state transitions; the VM does not invent
a second action protocol or automatically cancel another owner's goals.

Parameters support typed values, descriptors, read-only and dynamic-type
constraints, declaration, batched reads, batched writes and atomic transactions.
Guest validation is delivered as a bounded request with a deadline. Commit
occurs only after all granted checks accept; events describe committed changes.
ROS1 nested XML-RPC values and cache invalidation are represented separately
from the ROS2 parameter value algebra. No arbitrary code is evaluated from YAML.

Lifecycle servers expose legal transitions and deliver configure, activate,
deactivate, cleanup, shutdown and error-processing requests to the Guest.
Transition responses update owned state atomically. Lifecycle publishers obey
that state. Lifecycle clients, transition discovery and transition events are
independently authorized; a management transition is not a process privilege.

## TF, discovery and diagnostics

TF covers static/dynamic transforms, cache bounds, same-time and advanced
source-time/target-time/fixed-frame queries, asynchronous availability, cancel,
batch publication and explicit time-jump invalidation. Frame access is checked
against the resolved query, including the fixed frame and any permitted graph
disclosure. The Guest cannot use a broad TF query to read ungranted state.

Graph snapshots, endpoint inspection, counts, type descriptions and change
notifications use the same scope filter. Discovery does not grant communication.
Logging has explicit severity, rate and byte bounds. Telemetry distinguishes VM
queue loss, native middleware loss, quota rejection, timeout, cancellation and
provider failure. Arbitrary native exception text is not exposed to guests.

## Purpose-bound network and storage workflows

A guest selects a logical deployment binding, never an address, port, socket,
DNS name, URL, credential, host path or native plugin. A binding has a purpose,
direction, codec, peer identity, transport profile and lifecycle. Topic, service,
action, discovery, sensor ingestion, device control, video, bridge and model
provider traffic have distinct admission. Receive authority cannot transmit;
discovery cannot communicate; a model response cannot call a device. Multicast
and listening are provider-owned and separately provisioned. Arbitrary socket
fallback is forbidden when a purpose provider is unavailable.

Read-only package resources, configuration, calibration, maps and model weights
are immutable objects selected by binding and digest. Output logs, recordings,
map snapshots and caches are separate stores. A store grants workflow actions,
not recursive access to a directory. The implemented
[volatile cache profile](ros-scene-volatile-output.md) uses private heap memory,
exact-length publication and retained mutation receipts. It provides no disk
durability or schema validation, and aliases never share its backing store.
The following persistent-provider requirements remain design requirements;
their presence does not register a disk-output import.

Publishing a persistent object uses reserve -> begin
-> bounded write -> validate -> atomic commit. Cancellation aborts a staged
object. Failed cleanup retains its storage charge until trusted reconciliation.
There is no guest path traversal, symlink, hardlink, descriptor export or opaque
filesystem escape. Readers see committed generations only.

Capacity accounting includes staging, committed data, open-but-unlinked data,
metadata/file count and outstanding reservations. Logical length, allocated
blocks and device free-space headroom are different limits. Sparse offsets,
preallocation, overwrite, duplication, parallel sessions, restart and failed
commit cannot evade accounting. Store quotas are shared by backing-store
identity across aliases/environments; reboot reconstructs or verifies usage.
Physical hard bounds need a host-enforced volume/project quota and bounded
journal reserve. A directory preopen alone never establishes that guarantee.
Lifecycle limits are deployment capacity reservations, separate from optional
rate attenuation. Refuse admission before side effects if a complete operation
cannot be reserved. Recording drop, stop, block and rotation must be explicit;
never silently discard control/state data to meet a storage budget.

ROS1/ROS2 bridges bind two separately granted endpoint/type/QoS contracts and a
trusted conversion. Forward and reverse are distinct grants. Remapping happens
before both checks; loop suppression, correlation, QoS degradation, loss and
provider outages are observable. A bridge cannot broaden graph visibility or
forward models' tool proposals into robot commands.

## Portable native facilities

Plugin and component loading uses immutable, deployment-approved identifiers,
declared imports and resource budgets. Native shared objects are never selected
from Guest paths. Devices, accelerators, shared buffers and package resources
have separate capabilities. Shared buffers require lease, bounds, immutability
and retirement rules; zero-copy is advertised only when the actual provider
implements those rules. A pointer into native middleware memory is not exposed.

Hard realtime is a separately measured execution profile. CPU affinity,
priority, locked memory and scheduling budgets are trusted deployment controls.
VM support for a timer or ROS topic is not evidence of a deterministic control
loop. Hardware watchdog and safe-stop behavior remain separately provisioned.

## Model interfaces

Language, vision and world-model providers follow the independent contract in
[ros-model-integration.md](ros-model-integration.md). Model output never obtains
authority to call ROS, WASI, tools, package loaders or robot-control endpoints.
World-model rollouts are predictions, not authoritative sensor state.

## CLI, debugger and editor contract

The operation inventory is the source of truth for import registration, function
signatures, policy validation, help/completion and diagnostic labels. Structured
deployment configuration and individual CLI settings feed the same validator;
unknown keys, conflicting assignments and unsupported semantic settings fail.
`check-policy` and `dump-effective-policy` remain execution-free and report
resolved bindings, required providers, quotas and granted interfaces.

New resources have bounded metadata-only debugger snapshots. Console dbg, DAP,
idedbg and editor adapters show the same public names, source ownership,
readiness, lifetime and denial reasons. Inspection cannot consume a message,
trigger a guard, advance a clock, poll a native provider, run inference or grant
permissions. Credentials, native addresses, private model input, image/tensor
contents and IPC authentication material are excluded from ordinary metadata.

## Acceptance

Each implemented operation needs successful-path, wrong-owner, wrong-kind,
zero-authority, hidden/deny-interface, revoked/stale generation, bounds/overlap,
quota exhaustion, cancellation and teardown checks as applicable. Concurrent
waits include notification during registration, close while blocked, concurrent
detach and replacement. Model checks include prompt/tool-output authority
separation, tensor mutability, quota exhaustion and provider loss.

Compile and run actual Wasm guests on the supported interpreter and LLVM paths,
with explicit memory32/memory64 and cross-module policy cases. Exercise console,
DAP and editor rendering against actual runtime snapshots. Native differential
tests compare pinned ROS behavior and separately record distribution, RMW,
architecture, feature absence and hardware requirements. Mock-provider tests
verify VM contracts only and are never counted as native ROS qualification.

## Safety qualification beyond the operation inventory

The operation inventory is not an industrial safety certificate. Each concrete
provider must trace its effects, refusal modes, fault containment and recovery
to the deployment's hazards and safety functions. The current local scheduling,
immutable input, volatile cache and managed lifecycle profiles do not qualify
physical actuator safety or a complete ROS system. IEC 61508-3 requires specified
software safety functions, systematic capability and lifecycle activities;
ISO 10218 separates robot requirements from integration/application and cell
requirements. See the [IEC software requirements](https://webstore.iec.ch/en/publication/5517)
and [ISO robot safety scope](https://www.iso.org/sectors/engineering/robotics).

Device/control implementations must enforce command sequence, freshness,
expiry and units/frame identity at submission and again where the physical
effect occurs. A deployment needs validated joint/workspace/velocity/force
limits, mutually exclusive controller leases, explicit priority arbitration
and defined loss-of-link behavior. A safety-stop/watchdog path must have
independently reserved capacity and authority, so ordinary queue pressure,
model inference or optional traffic attenuation cannot obstruct it. A stop
request and confirmed stopped equipment are different states; unknown physical
outcomes must remain unknown until independent evidence resolves them.

Sensor and model providers must distinguish timestamp provenance, stale data,
coordinate frames, confidence and approved output constraints. A model proposal
cannot acquire device authority; proposal validation and actuator admission
must be separate, bounded steps. ROS1 bridges require constrained direction,
peer identity, minimal disclosure, reconnection generations and trust-boundary
validation. The ROS2 provider must apply application permission policy at the
transport boundary as well as the scene capability boundary; see the
[ROS2 access-control design](https://design.ros2.org/articles/ros2_access_control_policies.html).

Physical effects are classified by the immutable target profile and resource
scope, not by the import spelling. A permitted topic, service, action, parameter
update or lifecycle transition can affect the same equipment as a device/control
command. Such routes must converge on the same physical-resource authority,
controller lease/fencing, command limits and expiry checks. A bridge cannot
downgrade that effect classification. If the provider cannot establish this
mapping, the route cannot qualify as a safely constrained physical-effect target.
An accepted model proposal is only an inert approval record. Execution must
recheck the approval, current grants, target generation, controller fencing,
expiry and observation epochs at the same admission point as the effect;
acceptance itself is neither execution nor evidence of a confirmed safe state.

The remaining provider work must carry evidence for malformed input, concurrent
revocation, caller retirement, allocation/transport/device failure, delayed and
duplicate replies, cancellation races, restart recovery and exhausted capacity.
Performance acceptance must report representative end-to-end workloads plus
tail latency and deadline misses under stated load and failure conditions.
Throughput medians from the XML cache slice do not establish deterministic
execution, native ROS equivalence, a SIL/PL rating or completeness over the
acquired package corpus. The local lifecycle provider checks deadlines on
access and is expressly not an autonomous safety watchdog.
