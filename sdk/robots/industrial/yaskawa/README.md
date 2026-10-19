# Yaskawa MotoROS2: complete typed Source SDK

Read the [shared preparation/build/permission tutorial](../README.md) first.
Default authority is empty. The native interface and Wasm example lanes below
have actually executed; full controller and positive Guest delivery remain
separate qualification work.
Primary driver: [Yaskawa-Global/motoros2](https://github.com/Yaskawa-Global/motoros2/tree/41ac074a879bb9f75fd1f96618ce54b4899cfd1a).
The per-definition `source-map.json` and `source-lock.json` are authoritative for
all transitive repositories; the driver commit alone does not identify them.

|Catalog|Whole definitions|Registered roots|
|---|---:|---:|
|ros2|52|35|

## Qualified interface and example lanes

The [r33 original evidence](../../../../documents/ros/continuation-20261018-evidence/yaskawa-complete-production-gateway-r33/README.md)
builds the original MotoROS2 interface package with genuine Humble amd64 and
separately tests the industrial_msgs original, default-OFF and explicit-ON
compatibility builds. All 35 registered interfaces pass the actual production
Gateway with independent native DDS peers: 45 bidirectional Topic value pairs,
57 Service cycles per direction, malformed/duplicate Service refusals and
complete Action feedback/results, owned cancel and foreign-session refusals.
The 56 public payloads pass 168 independent native/converter/CDR scenarios and
869 destination-preserving decoder refusals. These are synthetic transport
values, with no driver, Agent, controller or firmware qualification.

The [r34 actual Wasm example lane](../../../../documents/ros/continuation-20261018-evidence/yaskawa-actual-wasm-examples-r34/README.md)
builds the C query and C++ trajectory reactors against that exact bound
generation. Four actual modules pass full Wasm validation and fixed ROS import
signature checks; two wrappers execute without ROS grants on the pinned existing
R110 Runtime. Complete response getter/diagnostic-copy tests and C++ trajectory
fixture/capacity checks pass. The trajectory reactor is byte-identical to the
selected KUKA example, demonstrating reuse of this standard-interface business
module. Positive Guest-to-Gateway, simultaneous multi-robot communication,
current whole-Runtime, hardware, official-suite and performance qualification
are not inferred from that identity.

The actual driver source selects `motoros2_interfaces` release0.2.0 through its
client dependency manifest. That is the annotated tag peeled to commit
24a63cf8a7a582c9fa6744507d605415462c9c17; the separately captured newer HEAD is
historical research, not substituted interface proof. The industrial core source
is pinned to d547cdcfdaf3bc0d46325215b8219b0a190c8e6c. All22 actual interface
files, industrial state records and full standard dependencies are retained in
the52-type catalog. Trigger is builtin dependency-only.

## Native package export boundary

At the captured industrial_core commit
`d547cdcfdaf3bc0d46325215b8219b0a190c8e6c`, the original
`industrial_msgs/CMakeLists.txt` leaves `srv_files` empty. Its original ROS2
package build therefore exports its six Messages, but does not generate native
ROS2 headers, type support or installed IDL for the six historical Service files
still present in that source tree: CmdJointTrajectory, GetRobotInfo,
SetDrivePower, SetRemoteLoggerLevel, StartMotion and StopMotion.

Those complete source definitions remain in this portable catalog. Their
presence does not establish an upstream native ROS2 Service implementation or
an installed SDK binding. A native qualification owner must retain the actual
package exports and refuse a missing installed interface. If a deployment
deliberately builds an additional compatibility overlay, record that overlay's
complete source and build changes separately, verify its native peer and keep
each Service alias explicitly authorized. Do not silently edit the original
package, substitute another release, or count generated portable headers as a
successful original SDK build. The MotoROS2 0.2.0 Services have a separate,
explicit original ROSIDL generation list and must be qualified on their own.

### Explicit historical-Service compatibility build

`native_overlay.py` prepares a separate source copy of the exact captured
industrial_msgs package. It requires a physical original package directory,
the reviewed `catalogs/ros2/source-lock.json` and its explicitly selected SHA256.
Every original Message/Service byte and Git blob association must agree; the
upstream CMake and package.xml must match the fixed reviewed revision. The tool
refuses unexpected sources, changed declarations, symlink roots, duplicate lock
keys and existing output. It neither downloads nor modifies an upstream tree.

The copy adds `UWVM_INDUSTRIAL_LEGACY_SERVICES`, default **OFF**, to CMake.
With the default setting, only the original six Messages are generated. An
explicit `-DUWVM_INDUSTRIAL_LEGACY_SERVICES=ON` adds all six complete historical
Services to ROSIDL generation, with the required trajectory_msgs dependency.
The copied package metadata records that extra dependency. `upstream-build.diff`
and `overlay-receipt.json` retain both build-metadata changes, the exact original
and output file hashes, default option state and empty authority. No server or
controller command implementation is manufactured by generating type support.

Use fresh physical directories under an admitted build owner; the parent of
the output directory must already exist. The following uppercase paths are
operator-supplied placeholders, not identities or an executable deployment:

```sh
python3 native_overlay.py \
  --original-package /ORIGINAL_SOURCE/industrial_msgs \
  --source-lock /REVIEWED_SDK/yaskawa/catalogs/ros2/source-lock.json \
  --source-lock-sha256 REVIEWED_SOURCE_LOCK_SHA256 \
  --out /OWNED_WORK/compat/industrial_msgs

# Configure a fresh default build first, then inspect the installed exports.
cmake -S /OWNED_WORK/compat/industrial_msgs -B /OWNED_WORK/build-off \
  -DCMAKE_INSTALL_PREFIX=/OWNED_WORK/install-off -DBUILD_TESTING=OFF
cmake --build /OWNED_WORK/build-off --target install --parallel 1

# Historical Services require a distinct, explicitly enabled build.
cmake -S /OWNED_WORK/compat/industrial_msgs -B /OWNED_WORK/build-on \
  -DCMAKE_INSTALL_PREFIX=/OWNED_WORK/install-on -DBUILD_TESTING=OFF \
  -DUWVM_INDUSTRIAL_LEGACY_SERVICES=ON
cmake --build /OWNED_WORK/build-on --target install --parallel 1
```

Source the selected real ROS SDK and set its actual prefix dependencies before
these commands. Keep original, OFF and ON build/install trees distinct. Verify
complete installed IDL, native typed fields and independent peer values before
binding the portable catalog. These are native SDK build choices. The separate
`ros-vendor-yaskawa` xmake option remains default-disabled and selects only the
explicit matching Runtime registry. Neither option grants any Guest alias,
starts a driver, or enables a Service endpoint. Actual traffic requires its own
explicit operation/resource/type/identity/codec/quota/QoS policy.

The upstream controller families are DX200, YRC1000 and YRC1000micro with recorded
minimum firmware DN2.44, YAS2.80 and YBS2.45 respectively. The selected upstream
Agent path is FastDDS only and documents Foxy/Galactic/Humble/Jazzy rather than
Iron/Rolling. These statements identify a source support boundary; installed
SDK/Agent/controller compatibility still needs actual qualification.

The actual driver limits trajectories to200 points, requires every configured
joint/group, disallows asynchronous independent groups and partial goals,
requires the first point to match current state, and requires zero final
velocity/acceleration. JointState/RobotStatus use sensor-data best-effort QoS;
the reliable synthetic fixture's QoS must not be copied blindly to that physical
subscription. The catalog's finite capacities and physical driver restrictions
must both hold. `examples/query.c` performs a complete ReadSingleIO request for
address10011 and retains result_code, message, success and value. This is not a
permission to energize outputs; write methods require separate explicit aliases.

The C reactor exposes `response_ready`, `result_code`, `response_success`,
`io_value` and `message_size`. Call `copy_message(destination, capacity)` with
a valid caller-owned Guest buffer to copy the complete diagnostic bytes; it
adds no terminator. Insufficient capacity returns 90 without a partial write.
Inspect readiness before using the saved fields. A complete controller-reported
failure keeps all four response fields available after capability closure;
transport/decode failure does not publish an incomplete response. The example
records its first operation error and independent close error and never retries
the IO request. The source documents constructor initialization, one-shot query,
diagnostic access and explicit closure in detail.

`start_traj_mode` changes drives/job/mode and therefore is not a readiness query.
`stop_traj_mode` is an idle-mode Trigger that can fail during motion and leaves
motors on; it is not the active-goal stop protocol. Cancel the owned FJT goal and
observe the actual terminal result/physical state. `reset_error` is a separately
authorized effect after the cause has been resolved. Do not combine an unknown
submit/timeout with a reset/resend loop. Hardware emergency stop and recovery
supervision remain outside the Guest. The original controller job/configuration
and complete group/joint model must be retained in the actual deployment.

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

## Complete values within the aggregate response budget

The `GetRobotInfo` and `GetActiveAlarmInfo` responses use an explicit canonical
`wire_limits.response` of32768 bytes. All original fields, constants and
per-field string/sequence limits remain present. The aggregate budget applies
to the **encoded complete value**, rather than requiring every independent
string and sequence to reach its maximum simultaneously. A valid sparse value
may use all16 alarm slots; a single large alarm may use its full declared text
and help capacities if the complete response still fits. Exceeding the total
returns a whole-value refusal, without a prefix response, field omission,
count reduction or truncated diagnostic text. The generated codec identity
includes this aggregate constraint. It is a canonical transport quota, not an
assertion about the native controller's maximum alarm or robot count.

The original Source-only catalog omitted these two aggregate budgets, so the
actual Source producer correctly refused its Cartesian maximum before any
output was written. That failed preparation remains separately retained. This
revision uses the existing complete-profile aggregate facility; it changes no
shared generator or global Runtime quota. Native binding, compiler execution,
full-value/overbudget contracts and controller acceptance remain separate
unified validation steps.
