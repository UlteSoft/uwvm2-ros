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

The compiler and receiver each enforce a 64 MiB compiled-model limit by default. For a reviewed model that exceeds this limit, pass `--vendor-model-max-mib N` (1 to 512). The compiler also runs with that file-size limit, so an oversized asset cannot fill the test volume while being compiled. The source bundle has a separate 256 MiB bound.

The test exercises the simulation command through the Unitree child permission
and the common external driver session. It compares physics state after 128
steps, rejects an out-of-range command before a step and records loss of the
receiver after a step. The upstream DDS driver, the radio link and a physical
protective stop each require separate hardware or protocol tests.

### LimX TRON1 point-foot model

Use `test/0084.ros-runtime/mujoco_profiles/limx-tron1-pf.json` with both commands above. The fetcher retrieves the original `PF_TRON1A` MJCF and nine STL files. Add `--vendor-model-max-mib 128` to the simulation command. MuJoCo 3.4.0 produces an 84,003,794-byte model, which exceeds the default 64 MiB budget. [Original LimX model](https://github.com/limxdynamics/tron1-robot-description/tree/ac417d44656d3c7494a095cecd62928e777a0f3a/pointfoot/PF_TRON1A) · [Upstream license](https://github.com/limxdynamics/tron1-robot-description/blob/ac417d44656d3c7494a095cecd62928e777a0f3a/LICENSE)

The `limx` platform permission and isolated TLS receiver passed 24 cases for the motor driving `hip_L_Joint`. Both VM engines and all four memory layouts matched the direct native reference after 128 steps. Each refused command produced zero steps. Each receiver failure after a step left the VM running and the command outcome unknown. LimX SDK transport, IMU subscriptions, actuator firmware and an independent physical stop require separate tests.

### Additional vendor-published models

The following profiles fetch only the files needed for the selected simulation from the vendor’s pinned repository. Use the same qualification command as for Go2 and select the compiled size limit shown for each model.

| Vendor model | Joint and actuator | Compiled limit |
| --- | --- | --- |
| [AgiBot X2 Ultra](../../test/0084.ros-runtime/mujoco_profiles/agibot-x2-ultra.json) | `left_hip_pitch_joint` / `motor_left_hip_pitch_joint` | 512 MiB |
| [Deep Robotics Lite3](../../test/0084.ros-runtime/mujoco_profiles/deep-robotics-lite3.json) | `FL_HipX_joint` / `FL_HipX_joint_ctrl` | 64 MiB |
| [Doosan M0609](../../test/0084.ros-runtime/mujoco_profiles/doosan-m0609.json) | `joint_1` / unique joint actuator | 64 MiB |
| [UBTECH TienKung2 Lite](../../test/0084.ros-runtime/mujoco_profiles/ubtech-tienkung2-lite.json) | `hip_roll_l_joint` / `hip_roll_l_joint1` | 64 MiB |
| [Leju Kuavo seventh-generation waist](../../test/0084.ros-runtime/mujoco_profiles/leju-kuavo-waist.json) | `waist_yaw_joint` / `waist_yaw_motor` | 64 MiB |
| [NVIDIA Isaac Sim MJCF importer ant](../../test/0084.ros-runtime/mujoco_profiles/nvidia-ant.json) | `hip_4` / unique joint actuator | 64 MiB |
| [NVIDIA Isaac Sim MJCF importer humanoid](../../test/0084.ros-runtime/mujoco_profiles/nvidia-isaac-humanoid.json) | `left_elbow` / `left_elbow` | 64 MiB |

Each original model completed eight 128-step native comparisons, eight command-range rejections with zero steps and eight isolated receiver failures after one step. The VM survived every receiver failure and retained an unknown command outcome. These tests exercise the named simulated joint and the common permission-scoped local driver channel.

The Kuavo scene exercises the upstream waist mechanism with gravity and contact disabled in its MJCF. The two NVIDIA files are sample assets from the Isaac Sim MJCF importer. The ant uses the `nvidia` permission scope and the humanoid uses `nvidia-isaac-sim`. MuJoCo supplies the physics engine for both.

### Rotary joints from ROS package URDFs

The following models retain the upstream joint, collision meshes, masses and inertia tensors. The model compiler receives a private copy of the URDF with mesh URIs rewritten to digest-derived names. This lets a visual mesh and a different collision mesh use the same original filename. The intake receipt identifies the original and rewritten XML, the mesh digests and the selected link's mass.

| ROS package model | Selected joint | Source |
| --- | --- | --- |
| ABB IRB1200-5/0.9 | `joint_3` | [ROS-Industrial Xacro profile](../../test/0084.ros-runtime/mujoco_profiles/abb-irb1200-xacro.json) |
| Elephant Robotics myCobot Pro 450 | `joint1` | [Vendor Gazebo URDF profile](../../test/0084.ros-runtime/mujoco_profiles/elephant-mycobot-pro450.json) |
| Clearpath Husky A200 | `joint_front_left_wheel` | [Original Groovy vendor Xacro](../../test/0084.ros-runtime/mujoco_profiles/clearpath-husky-a200-xacro.json) |
| RealMan RM65 | `joint1` | [Vendor URDF profile](../../test/0084.ros-runtime/mujoco_profiles/realman-rm65.json) |
| Dobot CR16 | `joint1` | [Vendor URDF profile](../../test/0084.ros-runtime/mujoco_profiles/dobot-cr16.json) |
| Yaskawa Motoman SIA10D | `joint_s` | [ROS-Industrial support package profile](../../test/0084.ros-runtime/mujoco_profiles/yaskawa-sia10d.json) |

For these URDFs MuJoCo has a passive hinge. The test receiver supplies a simulated torque controller on that hinge, limited to one newton-metre. Intake checks the upstream effort rating, velocity rating, target angle range and the selected link's inertia. The native control first runs the same initial state with zero torque to establish a passive reference. Each driven reference must differ from its passive control. The TLS receiver then compares the simulated joint position and elapsed time with direct native execution over 128 steps. The qualification covers this controller and the published mechanical model. Factory servo firmware, brakes and emergency stopping require independent evidence.

The Pro 450 fixture pins the Gazebo-specific robot description and its 15 visual meshes in the vendor repository. Its collision cylinders and simulation mass parameters come from that URDF. The arm's first joint passed all 24 cases. The test operates the arm joint through external effort control. Gazebo's optional gripper-mimic plugin and the physical gripper have separate behavior.

The ABB package uses Xacro to share its robot description and materials between models. Set `ABB` to a new absolute directory under the project `.tmp`, and `STAGED` to another unused path there. With the ROS 2 `xacro` program and its `ament_index_python` package installed, the following commands make both pinned ROS packages visible for expansion.

```sh
PROFILES=test/0084.ros-runtime/mujoco_profiles
python3 -B "$PROFILES/fetch.py" --profile "$PROFILES/abb-irb1200-xacro.json" --output "$ABB/share"
mkdir -p "$ABB/share/ament_index/resource_index/packages"
touch "$ABB/share/ament_index/resource_index/packages/abb_irb1200_support" \
      "$ABB/share/ament_index/resource_index/packages/abb_resources"
AMENT_PREFIX_PATH="$ABB" xacro "$ABB/share/abb_irb1200_support/urdf/irb1200_5_90.xacro" > "$ABB/expanded.urdf"
python3 -B "$PROFILES/stage_xacro.py" --profile "$PROFILES/abb-irb1200-xacro.json" \
  --assets "$ABB/share" --expanded-urdf "$ABB/expanded.urdf" --output "$STAGED"
```

The stager verifies the original Xacro and mesh digests and checks the canonical expansion against its reviewed digest. Pass `--vendor-model "$STAGED/profile.json" --vendor-assets "$STAGED/assets"` to the qualification command above. The IRB1200 third joint and the TienKung2 Lite left hip each passed all 24 command, range and receiver-loss cases.

The Husky A200 profile pins the Clearpath Groovy description. Its continuous front-left wheel has a mass of 2.63657 kg and the URDF specifies limits of 100 Nm and 100 rad/s. MuJoCo normally fixes the root of a bare URDF to the world. For this mobile robot the intake adds a free joint above the original `base_footprint` and a stationary 4 m by 4 m floor, with its surface 10 mm below the initial footprint. These are test-environment additions. All original links, joints, collision shapes and inertia tensors remain in the description. The receiver checks that the selected wheel carries a positive normal load against that floor and compares wheel angle, chassis height and peak load against a native stepper. This tests the wheel and its load in MuJoCo. The 128-step run produced a peak normal load of 497.35 N on that wheel and a final wheel angle of 0.00120564 radians. The direct zero-torque control ended at −0.00005326 radians. Gazebo plugins, vehicle navigation and physical braking have separate behavior.

To reproduce the Husky expansion, follow the ABB procedure with `clearpath-husky-a200-xacro.json`, create the single ament package marker `husky_description` and run Xacro on `$HUSKY/share/husky_description/urdf/base.urdf.xacro`. The source uses the historical syntax for macros without a namespace prefix. The ROS 2 Dashing release of Xacro accepts it. Use that release with its supported Python version to generate the expansion, then pass the same source directory and expansion to `stage_xacro.py`. [Clearpath Groovy description](https://github.com/clearpathrobotics/clearpath_husky/tree/0e11aac561d1077266175dd38c4c3b23f2f3c376/husky_description)

### NVIDIA software scopes

The `nvidia-groot` scope needs a qualified inference provider running a specific checkpoint with an observation and action contract. The `nvidia-isaac-ros` scope needs a working accelerated ROS graph with a defined input and a checked output. Robot meshes alone cannot establish either result. [NVIDIA Isaac GR00T](https://github.com/NVIDIA/Isaac-GR00T) · [NVIDIA Isaac ROS](https://github.com/NVIDIA-ISAAC-ROS/isaac_ros_common)

The GR00T CPU preprocessing check uses the upstream LIBERO demonstration, including its recorded state and action vectors, modality layout and processor statistics. It runs the upstream `StateActionProcessor` and `PolicyHorizonSpec`. The check compares every state and action value against an independent min–max calculation, exercises saturation, and verifies rejection of missing state and invalid execution horizons. Episode 000000 supplied 214 consecutive frames, each with eight state and seven action values. The maximum absolute difference was below 6.0 × 10⁻⁸ after normalization and 2.2 × 10⁻⁷ after the inverse conversion. This covers the offline numerical stage before checkpoint inference.

Use Python 3.12 with the locked versions of NumPy (1.26.4), SciPy (1.15.3) and PyArrow (23.0.1). Check out the pinned upstream revision and fetch the Git LFS object for `demo_data/libero_demo/data/chunk-000/episode_000000.parquet`. From the existing test cgroup, run the following with a new receipt path under the project's `.tmp`. [Pinned NVIDIA GR00T source and demonstration](https://github.com/NVIDIA/Isaac-GR00T/tree/dac54b313160c201c450f97df0fd74ebd379c892)

```sh
python3.12 -B test/0084.ros-runtime/check_groot_preprocessing.py \
  --upstream "$GROOT" \
  --episode "$GROOT/demo_data/libero_demo/data/chunk-000/episode_000000.parquet" \
  --output "$RECEIPT"
```

The Isaac ROS CPU messaging check compiles the two upstream C++ test sources against the original `isaac_common` headers. The four buffer cases cover insertion, removal, timestamp order, capacity and range selection. The camera synchronizer case supplies two streams, delays and drops frames, and checks the matched identifiers after recovery. Its receipt records the source and binary digests and the individual GoogleTest results. Supply the root of a GoogleTest source release containing `src/gtest-all.cc`. [NVIDIA Isaac ROS common](https://github.com/NVIDIA-ISAAC-ROS/isaac_ros_common/tree/7bec6ae5dbb2faa11617f40226e8932a4a94e6e4/isaac_common)

```sh
python3 -B test/0084.ros-runtime/check_isaac_ros_message_sync.py \
  --upstream "$ISAAC_ROS_COMMON" --googletest "$GTEST_SOURCE" --output "$NEW_RESULT_DIR"
```

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
| `fanuc_crx5ia` | `J1` | [fanuc-crx5ia.json](../../test/0084.ros-runtime/mujoco_profiles/fanuc-crx5ia.json) |
| `franka_emika_panda` | `joint1` | [franka-emika-panda.json](../../test/0084.ros-runtime/mujoco_profiles/franka-emika-panda.json) |
| `universal_robots_ur5e` | `shoulder_pan_joint` | [universal-robots-ur5e.json](../../test/0084.ros-runtime/mujoco_profiles/universal-robots-ur5e.json) |
| `boston_dynamics_spot` | `fl_hx` | [boston-dynamics-spot.json](../../test/0084.ros-runtime/mujoco_profiles/boston-dynamics-spot.json) |
| `agilex_piper` (160 MiB compiled-model limit) | `joint1` | [agilex-piper.json](../../test/0084.ros-runtime/mujoco_profiles/agilex-piper.json) |
| `robotis_op3` (256 MiB compiled-model limit) | `head_pan` | [robotis-op3.json](../../test/0084.ros-runtime/mujoco_profiles/robotis-op3.json) |
| `hello_robot_stretch` (160 MiB compiled-model limit) | `joint_head_pan` | [hello-robot-stretch.json](../../test/0084.ros-runtime/mujoco_profiles/hello-robot-stretch.json) |
| `fourier_n1` (512 MiB compiled-model limit) | `left_hip_pitch_joint` | [fourier-n1.json](../../test/0084.ros-runtime/mujoco_profiles/fourier-n1.json) |

Separate Unitree Go2 runs compiled C, C++, Fortran, Lua, Nim, Objective-C and D BetterC controllers using their public SDK installations. A QuickJS controller also completed the six cases from a commit-pinned local development installation. Each controller passed the six wasm32 cases at memory index zero with the two engines, including rejection of an out-of-range command and loss of the receiver. These results exercise the simulated joint and the common external driver interface. Vendor controller firmware and physical stopping behavior have independent acceptance tests.
