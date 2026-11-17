# Deprecated ROS ABI coverage and robot capability boundary

`uwvm_ros_v2` is the intended host boundary for robot applications, including
their storage, sensors, drivers, middleware, models and management workflows.
The eight legacy ROS VM modules are deprecated. WASIp1 remains a separately
authorized compatibility boundary for general libraries; a robot provider must
not fall back to guest filesystem/socket/native-library authority when a
scene operation is missing.

The [mapping](../../tools/ros_scene/legacy-coverage.json) inventories **124 actual
imports across eight legacy modules**. The
[checker](../../tools/ros_scene/check_legacy_coverage.py) compares both the actual
C++ tuples and the independent host-policy catalogue, rejects missing/unknown
rows, checks exact C++ targets and resolves every replacement operation against
the [scene catalogue](../../tools/ros_scene/interface-catalogue.json).
Each mapping has a semantic profile, authority constraints and differential
test obligations. This proves inventory coverage, not native observational
equivalence, package portability or industrial safety certification.

## Acceptance layers

Source inventory coverage requires exact set equality, including the client,
RMW publisher/subscription/service and portable-robot extensions. A count alone
does not pass. New or removed imports invalidate the mapping until reviewed.

Semantic coverage requires preservation of outcomes, ownership, ordering,
metadata, time/clock domains, failure/retry behavior and resource lifetimes.
A safer replacement may change the wire format and separate permissions, but
must describe that change. Matching names or a generic byte payload is
insufficient. A local lifecycle graph does not qualify native node transitions;
a private transform history does not qualify native TF; a queued device command
does not prove physical execution.

Implementation requires registered guest imports, complete admission and
bounded provider execution. Native equivalence requires differential tests of
the exact legacy scenario against ROSv2, including unsuccessful and concurrent
paths. The checker reports registration separately and currently reports
`native_equivalence_proven: false`. `--require-implemented` fails while any
mapping requires an unregistered operation.

Use `--source-root /absolute/path/to/frozen/source` to audit the exact legacy
host tuple and host-policy table compiled into a retained VM. The migration
catalogue and semantic obligations still come from this checkout. The receipt
records both roots and hashes every input, so concurrent legacy bug fixes do not
silently change the inventory attributed to an older integration binary.

## Operations added by the legacy review

The review identified missing authority, borrowing, parameter and cancellation
semantics. Their replacement operations are now registered; the complete
catalogue contains 162 registered operations and no unregistered planned rows.
The descriptions below specify the required semantics. Each concrete binding
has its own qualified subset and cannot inherit native equivalence from this
inventory result.

- `capability.revoke` permanently fences the exact admitted grant and all its
  descendants. Handle close remains distinct. Revocation has its own explicit
  permission, reserves an exact retry receipt, prevents reopening and preserves
  unresolved effects and cleanup debt. An authorized authority reduction does
  not compete with traffic rate budgets.
- `topic.reserve/acknowledge/release` and
  `service.reserve/acknowledge/release` preserve explicit native borrowing.
  Reserve retains the original record and copies detached bytes; only an exact
  consumer/endpoint/generation/sequence/nonce match can acknowledge it. Release
  returns the borrow without consuming the record. Failed guest copy must not
  acknowledge. Lost native acknowledgement is unknown, not automatically
  replayable. Reservation, receipt, retained bytes and cleanup capacity are
  separately bounded.
- `parameter.exists` permits existence inspection independently of value,
  descriptor and list access. `parameter.search` preserves ROS1 ancestor search
  within an approved namespace root, depth and work budget. Candidate authority
  precedes each lookup; denial is not absence. ROS1 atomic upsert/subtree
  replacement and ROS2 declaration semantics remain explicit provider contracts.
- `action.cancel.set` has a separate immutable scope for permitted goal origins,
  UUID/time selectors and maximum result set. Own-goal cancellation never grants
  broader cancellation. A provider unable to enforce the selected set rejects
  it; filtering a snapshot and issuing unrestricted cancel-all is invalid.
- `service.describe` reports the actual retained client/server incarnation and
  independently observed request/response QoS. Topic descriptions also need
  bounded optional native identity. `context.describe` reports closed native
  feature states, distinguishing configuration from actual native attestation.

The portable robot adapter maps state reads to sensor/control observation and
commands to device/control submission. Units, frame, freshness, sequence,
controller fencing, admission deadline, execution state and stop outcomes must
all survive that mapping. Those native driver tests remain outstanding.

## Driver and component boundary

The guest selects a logical approved profile and sends a typed message. It
never selects a native library path, symbol, address, descriptor, bus ioctl,
driver opcode or arbitrary socket destination. Native image selection, digest
verification, process isolation, startup, supervision and retirement belong to
trusted deployment. Guest-facing component management is a bounded lifecycle
request, not access to `preloaddl` or the native calling convention.

A driver manifest fixes the image closure, protocol/schema digest, exact device
identity, ingress/egress direction, permitted operations, physical quantities,
units, frames, limits and state transitions. Read, command, configuration,
calibration, firmware maintenance and stop are separate operations or separate
immutable capability profiles. A broad `device.submit` grant cannot select a
different operation/schema or turn into a raw byte tunnel. New device families
need reviewed schemas and validators; an unknown operation fails closed.

The message envelope binds caller environment, resource and provider generation,
request/sequence, source evidence, controller lease/fence and deadlines. The
receiver checks them again before the actual effect. Checksums detect damaged
frames; peer authentication and isolation are independently required. An
acknowledgement distinguishes received, accepted, executed, stopped and unknown.
An uncertain outcome latches the affected scope until qualified reconciliation.

Stop/watchdog capacity and channels are independent of command traffic.
Watchdogs must survive guest stalls and applicable VM/provider failures. A
process receiving a stop message is not proof of physical safe stop. Hardware,
controller and deployment qualification must establish that separately.
Zero-copy is optional only with complete bounds, immutability and retirement
proof; a portable copied-message provider remains a valid baseline.

## Robot workflow decomposition

Every workflow starts with zero authority. Explicit elevation grants a stable
resource/operation scope; optional attenuation may then weaken timing or
completeness guarantees. Omitted rates add no throttling, explicit zero permits
zero traffic, and finite resource reservations remain a separate requirement.
Already accepted work follows its reserved contract or an explicitly defined
cancel/drain/safe-stop transition.

- Package/configuration/calibration/map/model-weight reads select immutable
  logical objects and exact digests. No directory traversal or ambient search
  path is inherited. Runtime configuration changes need their own operation.
- Transactional output, recordings, logs, map snapshots and caches have
  separate stores. Reserve, stage, validate and commit are bounded together
  with metadata, object count, journal, pending deletion, crash recovery and
  workspace. Disk durability requires a qualified provider and enforced storage
  quotas; the current volatile cache does not provide it.
- Sensor acquisition, state observation and media processing have typed source,
  calibration/frame/time provenance and explicit loss policy. Receive does not
  authorize network transmission, device configuration or model egress.
- Device commands and control cycles require independent controller arbitration,
  state/freshness checks, physical limits, sequence/deadline fencing and reserved
  stop capacity. Read-only telemetry does not grant actuation.
- ROS context/node/type, topic, service, action, graph, parameter, lifecycle and
  transform capabilities have separate operation and metadata scopes. ROS1/ROS2
  bridging binds exact source/destination schema routes, directions and loop
  prevention; it cannot merge their authority domains.
- Language, vision and world models consume independently authorized immutable
  inputs and bounded tasks. Model identity, output schema, compute/provider
  budgets and destination egress are explicit. A prediction is not sensor truth.
  Proposals remain inert until a separate arbiter and target authority admit the
  exact command. Model/provider failure cannot self-authorize a fallback.
- Components and native workers use approved images and message protocols.
  Accelerators, cloud/fleet peers and maintenance facilities require purpose
  profiles with exact credentials/peers/directions owned by deployment; their
  presence never creates generic guest socket, process or dynamic-load rights.
- Clocks, timers, wait sets and event streams expose explicit domains and
  cancellation/loss behavior. Diagnostics and audit records retain admitted
  source identities, policy revisions and effect outcomes with bounded,
  redacted fields. Debugging does not create operating authority.

This decomposition sets review obligations for the robot ecosystem. It does
not assert that every hardware profile, native ROS package or deployment has
already been implemented or qualified.

## Compatibility and migration

Existing wasm32-wasip1 binaries retain their established WASI import contracts
when explicitly authorized. The VM does not silently translate a generic
filesystem/socket request into a robot capability, inherit WASI rights into
ROSv2, or reinterpret old ROS imports as new ones. Pure algorithms and library
adaptation can remain in the guest; robot external effects use explicit ROSv2
providers. Legacy grants are rewritten and reviewed per workflow.

Legacy ROS and WASIp1 receive bug fixes rather than new robot-domain features.
Fixing faulty unreleased semantics may break compatibility. Enabling legacy ROS
also participates in the conditional deprecation warning policy. Deprecation
alone neither removes the old implementations nor proves migration equivalence.
New ROSv2 loader provenance is sized from the trusted compiled import tuple
using recoverable `fast_io` allocation; guest input cannot select the size.
Adding a trusted operation does not require raising a numeric import ceiling.

## Reproduce the inventory check

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/check_legacy_coverage.py \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/legacy-coverage.json
```

The receipt pins the mapping, actual tuples, policy catalogue and this contract.
Compiler, VM, native, allocator-failure and profiler tests remain in the same
existing 64 GiB zero-swap cgroup. Performance qualification requires effective
`-O3 -g`, Hotspots mapped to the frozen C++ and Microarchitecture hardware events
mapped to actual assembly. Inventory checking is not a performance result.
