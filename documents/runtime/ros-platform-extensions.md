# ROSv2 platform extensions

## Management hierarchy

WASIp1, deprecated ROSv1 and ROSv2 are peer host-policy families. ROS1 and
ROS2 modules belong to ROSv1. Common robotics modules and platform-extension
modules belong to ROSv2. The extension family is not a fourth policy owner.
Neither WASIp1 preopens nor ROSv1 communication grants confer ROSv2 authority.

A platform ABI has the exact module identity
`uwvm_ros_v2.platform_extension.<platform>.v<major>`. The platform uses its
lowercase SDK identifier, including existing underscores or hyphens. The
positive protocol major is an explicit update boundary. It does not introduce
version suffixes into internal operation names, inheritance between versions,
wildcard imports or permission aliases. A newer ABI requires its own reviewed
registration and explicit child selection.

The parent ROSv2 environment owns the resource. A platform adapter shares the
common driver resource identity, authority generation, controller arbitration,
source witnesses, command sequence, limits, replay receipt, audit trail and
reserved stop path. Enabling a platform removes an additional restriction;
it does not grant, open, duplicate or replenish a capability.

## Implemented child-policy boundary

`platform_extension_policy.h` implements exact compiled declarations, default
disabled children, explicit ownership of already granted device aliases and
immutable startup configuration. The registration store uses recoverable
fast_io-backed containers, without a fixed 64-module array. Device ownership
lookups use a separate index, so common admission does not scan every registered
platform. A failed insertion does not partially install an ownership record.

`device_dispatch` applies child ownership to common calls and extension routes.
Selecting the common `device.*` or `control.*` entry cannot bypass a disabled
platform. Disabled devices retain inert bookkeeping only: their descriptors are
not inspected or opened. An environment with no enabled native device binding
does not start the device supervisor. Controllers resolve their existing common
device rather than creating a parallel platform resource.

The trusted extension route resolves a closed common carrier, checks its live
parent capability and actual device, then enters the existing driver dispatcher.
That dispatcher rechecks authority, cancellation, input schema, bounds,
controller epoch and source evidence. Both entry paths share the same request
ledger and exact-retry receipt. Ordinary revocation retires normal work while
preserving reserved stop delivery. A transport acknowledgement is not evidence
that a physical mechanism has stopped.

Configuration is a child command under the existing ROSv2 environment:

```text
--ros-scene ENV platform_extension MODULE enable
--ros-scene ENV platform_extension MODULE disable
--ros-scene ENV platform_extension.bind MODULE DEVICE_ALIAS
```

Selection and binding are distinct, single-assignment settings. The parent
environment and common device grant must already exist when binding. Missing
parents, unknown compiled modules, duplicate ownership, wildcard names,
post-freeze changes and attempts to register a module through the CLI fail
closed. There is no peer `--platform-extension` permission system. Startup
configuration cannot re-enable a running revoked resource; deployment must
follow the common retirement and fresh-generation procedure.

**Current implementation boundary:** the child policy and common dispatcher
integration are implemented. The `fixture` module exists only in tests. There
is no production vendor-module registration or general original-binary driver
launcher. SDK catalogue presence, a downloaded repository or a valid-looking
module name cannot load a driver. A production extension requires only reviewed
semantic restrictions and common-carrier mapping; its native provider must be
qualified through the external process manager separately.

## Stable grants and live attenuation

The authority lifecycle is zero authority, explicit stable grant, then optional
traffic attenuation. Stable authority permits the declared operations within
their resource scope and fixed reservations; it does not remove deadlines,
physical bounds, provenance checks, controller arbitration or safety stops.

The trusted `deployment::attenuate(environment, alias, limits)` operation reduces
the existing live parent account. It is not a guest import, debugger command or
live CLI mutation. The first reduction starts prospective call/byte accounting
and requires explicit `uncertain=true`, a positive window and a selected meter.
Previously admitted work keeps its reservation. Later reductions may lower a
limit or add a meter; they cannot widen/remove a meter, change its window, reset
usage, re-enable a child or renew a controller. Repeating an identical update is
idempotent. All handles and both common/platform paths share that account.

Zero traffic allowance refuses new normal work with `again`; it is distinct
from absent or revoked authority. Exact reserved stop and resource cleanup
paths remain available under their existing grants and ownership checks.
`resource.close` does not consume normal call/byte rates, including local
guards, timers, sealed buffers and artifact readers. It cannot create resources,
free another alias's state, restore authority or reset usage. A failed result
copy cannot close the resource. Inspection reports live `attenuated` and
`revoked` flags separately from cached child enablement without changing any
account or invoking a provider.

## Retained historical driver ports

`platform_agilex_motion.h` and `platform_agilex_receiver.h` are retained under
literal `#if 0`. They cannot be enabled as platform extensions. Their old
[protocol experiments and performance receipts](ros-platform-protocol-history.md)
are historical and do not qualify external vendor binaries.

## Trust and isolation

Every native vendor driver runs outside the VM, including reviewed source,
official binaries and open-source builds. Source availability and signatures
do not permit in-process execution. Platform extensions may contain only small
permission, constraint and audit declarations routed through common managers.
They contain no vendor codec, SDK runtime, native function pointer, socket,
plugin loader or device-I/O implementation. Portable algorithms run in Wasm.

The common manager owns the external process lifecycle and device authority;
a platform switch cannot bypass it. Shared libraries require a matching external
plugin host, executables require a confined process tree, and static SDKs require
an external linked host. See the [native driver contract](ros-native-driver-isolation.md)
for real vendor examples and the current launcher implementation limits.

Each adapter specification must state the supported firmware/protocol range,
units, frames, clock domain, stale-input rule, controller ownership, cancellation
point, command-acceptance boundary, observable completion and unknown-outcome
behavior. Safe-stop capacity is reserved independently of normal command rates.
An uncertain remote result is retained; it must not trigger an automatic motion
retry. Motor enable, calibration, firmware changes and safety configuration
require separate semantic operations and grants.

Process isolation must be qualified with the actual transport. Measure native
in-process, native isolated and VM-isolated paths with the same workload and
budgets. Record IPC copies, queue occupancy, backpressure, median and tail
latency, jitter, CPU affinity, NUMA placement, memory pressure and worker failure.
Test crash, hang, malformed frames, partial writes, lost replies, revocation,
controller handover and reserved stop under load. Pin every binary and build
with `-O3 -g`; VTune Hotspots must join frozen C++ sources and Microarchitecture
Exploration must identify actual sampled assembly. A process boundary or an
affinity setting does not establish a real-time guarantee.

The current target set is Linux x86_64, i686, AArch64, ARM base/soft PCS and ARM
hard/VFP PCS. ELF closure packaging distinguishes all five and rejects mixed
class, machine, endianness, interpreter and ARM floating-point ABI. Packaging
qualification is separate from native ROS execution, isolation qualification,
timing qualification and physical device validation.

## Debugger contract

Device/control authority and resource rows carry `platform` and
`platform_enabled`. The module name is bounded, canonical and never truncated.
The boolean is the cached startup selection, not effective authority. For
example, an enabled child can have a revoked parent grant. Unowned resources
carry an empty module and false. Inspection does not register a driver, activate
a provider, access descriptors, advance a deadline or send a command.

The console formatter, strict DAP decoder and IDE inspector share these fields.
The inspector explicitly distinguishes child selection from resource authority.
Neither debugger surface provides a platform enablement or command operation.

## Source inventory and remaining common semantics

The retained `linux-five-capability-inventory` cohort identifies 9,432 distinct
observed package names across 30,712 source instances, and 45,063 interface files
in 1,623 packages. These are distribution/community inventories, not 9,432
officially authored libraries or completed portability reviews. Of those files,
42,898 have extracted field shapes; 110 require syntax review, 1,989 require an
IDL parser and 66 require encoding review. There are also 607 unowned interface
definitions. None of these counts is a semantic acceptance result.

The shapes include nested records, arrays, sequences and bounded strings.
The current native communication bindings cover a small closed set of message,
service and action types. General bounded type descriptions, validation and
structural ROS1/XCDR1 conversion now exist in the common core. Explicit native
builds can register bounded generated topic adapters after native shape and
type-hash checks; generic service/action adapters remain required. Copying vendor type names into platform
modules would not close that gap. Motion control, controller
arbitration, safety supervision, time/frames/state and deployment recovery also
remain explicit common obligations for every adapter.

The expanded `platform-protocol-complete` cohort identifies 130 pinned snapshots
and provenance for all 32 SDK platform entries. Of these, 128 were acquired
normally; two Leju archives were refused because they contained unsafe links.
The `platform-protocol-quarantined-canonical` cohort preserves their regular
files with links recorded as inert metadata, never materialized. Those are not
complete upstream working trees. Canonical path checks, empty destination
requirements and hostile-archive tests guard extraction; no acquired code is
executed by acquisition. Several entries are related NVIDIA products rather
than distinct robot vendors. Source acquisition is neither a completed semantic
audit nor implementation or hardware qualification. Bounded ROS type descriptions
and wire validation are described in
[Bounded message schemas](ros-bounded-messages.md) and
[bounded native topics](ros-bounded-native-topics.md). Additional native type
qualification, generic service/action adapters, the remaining platform contracts
and native transports are still required.

## Exercising every SDK platform restriction

Both tests take the selected SDK commit's `share/targets.json` and the VM
executable. The physics test provisions a separate MuJoCo process and a fresh
mutually authenticated TLS channel for each case. The `--platform-targets`
option enables the named child and binds it to the common driver alias. The
guest runs 128 steps against the official humanoid model and checks the final
time and joint position against direct native execution. Further cases check
a rejected out-of-range request and loss of the receiver after one step.
Each child uses the same model and common driver contract. Vendor protocol,
vendor model and physical-device acceptance require their own fixtures.

The denial test independently runs two configurations for each child. With the
parent operation granted, disabling the child must deny the connection. With
the child enabled, withholding the parent connection grant must also deny it.
The guest checks the status and both output sentinels. The peer checks that
the VM sent zero bytes, including TLS handshake data.

Run both commands serially in the existing qualification cgroup. Choose a
fresh output directory for each run.

```sh
python3 test/0084.ros-runtime/run_driver_mujoco.py \
  --runtime "$VM" --source-root "$VM_SOURCE" --mujoco "$MUJOCO_ROOT" \
  --openssl-root "$OPENSSL_ROOT" --platform-targets "$SDK/share/targets.json" \
  --audit-calls --output "$PHYSICS_OUTPUT"
python3 test/0084.ros-runtime/run_platform_tls_denials.py \
  --runtime "$VM" --platform-targets "$SDK/share/targets.json" \
  --output "$DENIAL_OUTPUT"
```

## Checking a vendor's original MuJoCo model

Place the vendor assets in a local directory and select their reviewed model
profile. The profile names one platform, one simulated actuator and its hinge.
It pins the original scene, included MJCF and meshes by SHA-256. The test
copies and verifies those files before running the official MuJoCo compiler in
a private network and mount namespace. Both the direct native reference and
the external ROSv2 receiver load the resulting MJB. Wasm code has no path to
the vendor assets. The fixture verifies that the specified actuator drives
the selected joint.

For the Unitree Go2, supply the contents of
[the original model directory](https://github.com/unitreerobotics/unitree_mujoco/tree/1eb6642e3f3fdfb7fb13a9794fd6a2dd93ea0e7d/unitree_robots/go2)
to the following command within the qualification cgroup. To retrieve just
the public files referenced by this profile, set `GO2` to a new directory
under the project `.tmp` and run the pinned asset fetcher first.

```sh
python3 -B test/0084.ros-runtime/mujoco_profiles/fetch.py \
  --profile test/0084.ros-runtime/mujoco_profiles/unitree-go2.json --output "$GO2"
python3 -B test/0084.ros-runtime/run_driver_mujoco.py \
  --runtime "$VM" --source-root "$VM_SOURCE" --mujoco "$MUJOCO" \
  --openssl-root "$OPENSSL" --audit-calls --platform-targets "$SDK/share/targets.json" \
  --vendor-model test/0084.ros-runtime/mujoco_profiles/unitree-go2.json \
  --vendor-assets "$GO2" --output "$NEW_RESULTS"
```

The test exercises the simulation command through the Unitree child permission
and the common external driver session. It compares physics state after 128
steps, rejects an out-of-range command before a step and records loss of the
receiver after a step. The upstream DDS driver, the radio link and a physical
protective stop each require separate hardware or protocol tests.

## Models from MuJoCo Menagerie

The following profiles use original files from the [MuJoCo Menagerie repository](https://github.com/google-deepmind/mujoco_menagerie/tree/370c43965323c0944b433b202aac5058b5f907a9). Use the same fetch and test commands as for Go2, substituting the selected profile. Each row passed 24 cases covering wasm32 and wasm64, both carrier-memory positions, interpreter and AOT execution, the allowed command, range rejection, and receiver loss. Each allowed run compared 128 steps for the named joint against a native MuJoCo run.

| Simulation model | Selected hinge | Profile |
| --- | --- | --- |
| `trossen_wx250s` | `waist` | [trossen-wx250s.json](../../test/0084.ros-runtime/mujoco_profiles/trossen-wx250s.json) |
| `kinova_gen3` | `joint_1` | [kinova-gen3.json](../../test/0084.ros-runtime/mujoco_profiles/kinova-gen3.json) |
| `kuka_iiwa_14` | `joint1` | [kuka-iiwa-14.json](../../test/0084.ros-runtime/mujoco_profiles/kuka-iiwa-14.json) |
| `flexiv_rizon4s` | `joint1` | [flexiv-rizon4s.json](../../test/0084.ros-runtime/mujoco_profiles/flexiv-rizon4s.json) |
| `ufactory_lite6` | `joint1` | [ufactory-lite6.json](../../test/0084.ros-runtime/mujoco_profiles/ufactory-lite6.json) |
| `pal_tiago` | `arm_1_joint` | [pal-tiago.json](../../test/0084.ros-runtime/mujoco_profiles/pal-tiago.json) |
| `booster_t1` | `AAHead_yaw` | [booster-t1.json](../../test/0084.ros-runtime/mujoco_profiles/booster-t1.json) |

A separate Unitree Go2 run compiled C, C++, Fortran and Lua controllers using their public SDK installations. Each controller passed the six wasm32 cases at memory index zero with the two engines, including rejection of an out-of-range command and loss of the receiver. These results exercise the simulated joint and the common external driver interface. Vendor controller firmware and physical stopping behavior have independent acceptance tests.
