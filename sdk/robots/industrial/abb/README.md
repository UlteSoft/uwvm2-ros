# ABB: complete typed Source SDK

Read the [shared preparation/build/permission tutorial](../README.md) first.
Default authority is empty and all examples/contracts are unexecuted Source.
Primary driver: [PickNikRobotics/abb_ros2](https://github.com/PickNikRobotics/abb_ros2/tree/e6486f7409073cdb2a10d6878bb50191dd50172b).
The per-definition `source-map.json` and `source-lock.json` are authoritative for
all transitive repositories; the driver commit alone does not identify them.

|Catalog|Whole definitions|Registered roots|
|---|---:|---:|
|ros1|53|48|
|ros2|63|48|

This kit preserves two distinct source domains: ROS1 abb_robot_driver commit
d1678957d3792e6aa54582ff894db62b6a51dac0 with abb_robot_driver_interfaces
 da809cd0bb6f270c0814fca07d7d4a5c0385fc39, and ROS2 rolling abb_ros2 commit
e6486f7409073cdb2a10d6878bb50191dd50172b. The latter's actual abb.repos selects
gbartyzel/abb_ros2_msgs071bb1fc2148a7cd850a1ff550c5987e67da6bdd. Full47 vendor
definitions are retained in each appropriate53/63-type catalog. ROS1 and ROS2
spelling, identities and wire schemas are never relabeled as one installed SDK.
The pinned libegm/librws/managers manifests are Source dependencies; no full
native installation or executed linkage is claimed.

The ROS2 source documents RWS1.0; RobotWare7/RWS2 is outside that support boundary.
Its IRB1200_5_90 example uses six joint_1..joint_6, position+velocity interfaces,
250Hz update,100Hz state and20Hz action monitor, no partial goals. Different
robot/controller installations require their actual official support/model and
configuration. RAPID/EGM ports and native sessions are held by trusted native
processes; a Wasm value contains no ABB native pointer/socket or implicit rights.

`examples/query.c` and `query_ros1.c` retain complete GetSpeedRatio request/
response fields. The actual ServiceResponses RC_SUCCESS is1, not a guessed zero
success code; ratio and full response message remain available. All RAPID,
EGM, state-machine and IO service families remain typed and separately granted.
The original `~/stop_rapid` and `~/stop_egm` are
abb_robot_msgs/TriggerWithResultCode, not std_srvs/Trigger. The former checks
RAPID-running then invokes the real prioritized stopRAPIDExecution; the latter
requires auto mode/RAPID running/manager readiness then issues the EGM-stop
signal. Neither is silently substituted for a Guest-owned Action cancel nor
asserted to be a hardware emergency stop. Stop/cancel ACK still requires actual
terminal and physical-state verification. Reset/restart/recovery is a distinct
native effect after independent operator/controller inspection. Never replay an
unknown command or borrow another source's cancel-any authority.

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
