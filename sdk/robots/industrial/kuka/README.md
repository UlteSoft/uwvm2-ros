# KUKA: complete typed Source SDK

Read the [shared preparation/build/permission tutorial](../README.md) first.
Default authority is empty. The separately scoped native interface and Wasm
example validations below have actually executed; positive Guest-to-Gateway
and physical controller qualification remain separate.
Primary driver: [kuka-ros/kuka_drivers](https://github.com/kuka-ros/kuka_drivers/tree/15fbb6941356390c38bbd135a0676dfd96a050c0).
The per-definition `source-map.json` and `source-lock.json` are authoritative for
all transitive repositories; the driver commit alone does not identify them.

|Catalog|Whole definitions|Registered roots|
|---|---:|---:|
|ros2|26|8|

## Qualified native interface lane

The [r31 original evidence](../../../../documents/ros/continuation-20261018-evidence/kuka-complete-production-gateway-r31/README.md)
builds the unmodified kuka_driver_interfaces 2.0.0 package with genuine Humble
amd64, verifies complete installed source/native metadata and executes all eight
registered interfaces through the production Gateway with independent native
DDS peers. It covers 21 bidirectional Topic value pairs, three bidirectional
FollowJointTrajectory success cases, owned cancellation, foreign-session
refusals, zero-capability endpoint absence and denied debugger/update operations.
Thirty independent complete native/converter/CDR cases and 330 decoder refusals
also pass. All execution stays inside the hard 64 GiB, swap 0 cgroup and an
isolated loopback-only network namespace. No Guest, full driver, RSI/FRI
controller, hardware, simulator or performance result is claimed by that lane.

The [r32 actual Wasm example lane](../../../../documents/ros/continuation-20261018-evidence/kuka-actual-wasm-examples-r32/README.md)
builds the C11 status and C++20 trajectory reactors against that exact bound
generation. Four actual modules pass full Wasm validation and fixed ROS import
signature checks. Two executable wrappers pass without Runtime grants on the
explicitly pinned existing R110 Runtime. The C wrapper checks all 64 Boolean
status patterns, 1,024 malformed-payload refusals preserving sample/counter,
and counter saturation; the C++ wrapper checks complete trajectory fixture
codecs and capacity refusals. This does not qualify a current whole-Runtime
build or positive Guest delivery.

## Read-only C status embedding

`examples/status.c` is a no-entry reactor. Initialize constructors through the
embedding, then call `initialize_case(0)` for the default-denial check or
`initialize_case(1)` to open the separately authorized `vendor-status` alias
and subscribe. Drive `poll_once()` from the application executor. Error 11
means no new value; zero commits one complete decoded sample. Inspect
`sample_ready()` before reading the exported `control_mode`, `cycle_time`,
`drives_powered`, `emergency_stop`, `guard_stop`, `in_motion`, `motion_possible`,
`operation_mode` and `robot_stopped` getters. `sample_count()` saturates at
UINT32_MAX. `primary_status()` and `cleanup_status()` preserve operation and
close errors independently; `close_case()` explicitly retires the capability.
The source includes complete lifecycle comments. These getters expose saved
application data, with no timestamp or physical freshness guarantee, and grant
no authority to change controller, Runtime, update or debugger settings.

The captured master is a Jazzy-era driver family with separate Humble branches.
It contains distinct iiQKA.OS2 RSI variants (`rsi_only`, `eki_rsi`, `mxa_rsi`), FRI
and lifecycle/control packages. The recorded tested RSI combinations include
KR C5 OPS/C5 iiQKA9.1.0 with RSI6.1.2 and C5 micro-2/LBR iisy3 R760 iiQKA9.3.0
with RSI6.3.0. This is not a claim that all KSS, Sunrise or KUKA generations share
one interface or a compatible firmware.

The26-type catalog retains all six vendor messages: FRIState/FRIStateArray,
FriConfiguration, HardwareEvent and the complete nine-field KssStatus plus
KssStatusArray, together with full JointState/FJT dependencies. `status.c` decodes
all actual KssStatus fields through the generated C codec. It never infers motor
safety from a single flag. Trigger, ChangeState and GetState remain complete
dependency-only builtin records; lifecycle commands use their existing genuine
builtin transport/explicit aliases, not new custom type registrations.

The actual selected controller example uses joint_1..joint_6, position interface,
50Hz state publication,20Hz action monitor and accepts nonzero final velocity.
Use the actual variant's URDF, controller YAML, robot configuration and connection
parameters. The Guest receives portable canonical values; the trusted native
driver owns RSI/FRI sockets and native controller handles. `robot_manager`
lifecycle deactivate is non-real-time and does not promise immediate physical
stop. Pendant emergency stop remains an independently supervised native/hardware
control. Never add it as an implicit ROS permission. A real active trajectory
uses the owned-goal cancel protocol and actual terminal result; transition or
recover only after actual native state and installation procedures confirm it is
appropriate. The upstream mock hardware path exists in source but has not been
executed here and does not qualify a physical controller.

## Concrete application and independent witness

1. Prepare the selected catalog in a fresh owned directory through the actual
   shared adapter, then build/capture the real original native exporter and bind
   only its complete metadata. Every generated component and C/C++ alias in
   `binding_ros*.hpp` must resolve against that same generation revision.
2. Build the real Wasm C/C++ reactors and the separately admitted native synthetic
   fixture using the common CMake entry. Use `trajectory.cpp` for full ROS2
   JointState/FJT and the vendor query/status source for its actual family.
   The synthetic fixture's two joint names and MultiDOF values are test values,
   not a hardware trajectory.
3. Start a denied instance using the exact `denied_argv` in
   `permissions.fixture.json`: no grants, actual socket/UID placeholders. Require
   local13/no owner and independent zero native goal/query/publication effects.
   Source markers or an SDK return code do not establish that negative witness.
4. For a separately owned positive instance, replace every source/identity/socket
   placeholder with actual admitted facts and apply only the listed explicit
   operation aliases. All message/body/queue/clock budgets are genuine policy
   constraints, not Native or hardware permissions inferred from a catalog.
   Drive the exported reactor through the real Runtime embedding, retain full64
   goal, checked values, controller result, uncertainty and every cleanup status.
5. Compare the independent real native request/feedback/result values and counts.
   Qualify success and owned cancel separately. Force a deadline/disconnect and
   confirm that no automatic resend/reconnect/unsafe recovery occurs. Keep the
   original Native process/map/SDK, Wasm imports/memory and actual source epoch.
6. Use only a genuine admitted all-stop DBG observation. Current ROS-provider
   typed-read admission is a separately tracked Source gap, so this tutorial
   claims no ROS debug snapshot, checkpoint, rollback or replay success. The
   Guest counter exports are application data, not authority to inspect memory.

For a real controller, separately select the actual model/joint configuration,
load complete current JointState, review all typed request fields and quotas,
and use its actual measured middleware QoS. Physical stop/recovery and observable
native effects require an explicitly controlled hardware/simulation lane. No
such run, qualification or performance measurement occurred in this cohort.
