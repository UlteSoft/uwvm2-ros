# LimX: complete native messages through an explicit ROS2 bridge

This is source for a real trusted native SDK bridge, a NEW complete ROSIDL
package, genuine installed-TypeSupport exporter entry, and full typed Wasm
examples. Default Guest authority is empty and native networking is compiled
OFF. No generator, compiler, contract, Runtime, native SDK or robot was executed
while authoring this package. There are no supplied native hashes or generated
headers that could stand in for an actual installed SDK/export.

## 1. Pin the model, SDK and controller separately

The SDK source is limxdynamics/limxsdk-lowlevel commit
`61624b58663b47928717069ff0eab2f1c85ec733`, tree
`824b4132698c805a69e92dd94b60b1366a3bd76d`. The model source is
limxdynamics/robot-description commit
`83dc60306e15936903e07f3de78cd144af583ac2`, tree
`5dfa658fe24a9cfb85c37d48ae3ff844dde738c0`. The selected PF_P441A description and
SDK PointFoot order give six joints: abad_L_Joint, hip_L_Joint, knee_L_Joint,
abad_R_Joint, hip_R_Joint, knee_R_Joint. This is not a CL1/other-humanoid mapping.
The bridge's PointFoot mode admits exactly PF_P441A/six. Its optional Humanoid
mode requires an explicit actual model/count and the SDK's complete actual
motor-name table; this source does not qualify a particular humanoid firmware.

Select the actual native SDK shared library with its exact SHA, actual complete
headers/MD/link/maps, controller firmware and native endpoint configuration.
The SDK's wheel filename/version and package.xml version0.0.0 are not controller
firmware proof. The reviewed public ApiBase has no firmware getter, public
shutdown, universal hardware stop or command-cancel API. External real controller
identity/watchdog/stop evidence remains necessary; no function is invented to
make that requirement disappear.

## 2. Preserve every native field in a distinct schema

The SDK contains real mros message mirrors and their complete definitions,
TYPE/MD5 constants, encoders and decoders. IMUData includes Header, uint64
imustamp, uint32 status, float64 euler[3], quat[4], acc[3] and gyro[3]. JointState
contains Header, all names, q/v/vd/tau and na. JointCmd contains all names,
q/v/tau/kp/kd/mode and na. JointCmdLimx also contains every
parallel_solver_mode byte, including values other than0/1. This package retains
those actual native declarations and never treats their ROS1 MD5 as a ROS2 RIHS.

`bridge_interfaces` defines the NEW package uwvm_limx_bridge, with five complete
messages. NativeHeader retains seq plus unsigned sec/nsec and frame_id. Replacing
it with standard ROS2 Header would lose seq and change the time domain, so that
projection is not used. This is our authored bridge schema, not an official
LimX ROS2 package or a renamed native TypeSupport. Its actual future generated
ROS2 hash and canonical codec identity are separate facts.

`native/conversion.hpp` converts genuine SDK objects to genuine generated
bridge objects and back, one field at a time into private storage. Only a full
valid destination is moved into the caller. Names may be empty because the SDK
supports nameless controllers; nonempty names must match na and each complete
name is retained. The native process additionally checks nonempty command names
against the actual SDK motor map. Every sequence is bounded at64, every name at
64 UTF-8 bytes and frame_id at128 bytes. Overbound values return90; invalid
UTF-8/NUL, nonfinite numbers, non-normalized nanoseconds or inconsistent array
counts return22. Both errors preserve the prior destination. A finite accepted
canonical subset does not shrink native capacities or authorize truncation.

`native/value_contract.cpp` uses the actual SDK/generated types for complete
round trips, signed values, max unsigned timestamps/status, parallel byte255,
late-field NaN, missing arrays, raw text and quota sentinels. It is authored
source with always-active checks, not a successful execution report.

## 3. Generate and export the actual complete family

Select `source-definitions/ros2/catalog.preparation.json` using the explicit
uwvm-ros-vendor-complete-bounded-v1 mode in
sdk/robots/industrial/vendor_profile.py. Root supplies captured current source
bytes/hashes, all four actual managed interface templates and the genuine
exporter helper source. The original vendor19 Git-only preparer cannot claim
these authored bridge files exist in the LimX SDK tree; a reviewed origin-aware
vendor selection is required.

The real API prepares all records/complete constants and native_identity.cc.
`native/export_identities.cpp` strongly includes that actual generated source,
not a fake TypeSupport or an empty main. Its modern metadata getters are warmed
before the true SIGSTOP/I gate. The genuine export includes source_profile,
constant_schema_sha256, each actual root/component hash and full referenced
TypeDescription. Root must retain installed SDK/source/full-MD/link/ELF/maps
and the independently owned exporter observation before API.bind and a new
bound output revision. Guest-only output, a nonzero hash and equal field widths
are insufficient proof. The emitted complete native provider must be explicitly
selected by the Gateway; an older matcher's refusal must not be bypassed.

The native CMake requires the actual generated bridge targets and an explicit
absolute library/SHA, and uses the current finite link-input guard. No ambient
link directory, permissive unresolved-symbol mode or weak stub can fill a
missing SDK. The ordinary ROSIDL package CMake creates genuine native products
only when Root later builds it; no generated header is checked in here.

## 4. Use raw imports, then complete typed SDK operations

The raw Wasm lesson is `examples/raw_command.cpp`: exact uwvm_ros2_v1
capability_open/topic_publish/capability_close, full-width handles and linear
memory offsets, an eight-byte LE output sentinel, and exactly one send attempt.
It first decodes the actual complete generated JointCmd and checks na6 before
opening the positive alias. Default mode expects denied13 without modifying
the sentinel or reading a command body.

`examples/complete_command.cpp` performs the same operation with the real
roswasmcpp Publisher and generated Codec. Input is LE32 length plus one whole
canonical body, not a native struct image. Header, full names and every
q/v/tau/kp/kd/mode value remain explicit. `values.hpp` displays all command
fields before opening the positive owner. The input must come from the actual
selected simulator/controller's reviewed safe command; a zero vector is not
assumed to mean hold or stop.

`examples/telemetry.cpp` owns the complete generated JointState Subscription.
An R input byte releases one actual poll; at most64 caller credits are used.
Idle11 is not delivery, and a credit count is not elapsed nanoseconds. A full
record is decoded, re-encoded and displayed field by field, including all
header/name/q/v/vd/tau data. Primary, close and setup-rollback outcomes are
retained separately. The example imports no socket, ROS administration, SDK
singleton or automatic clock right into Wasm.

`recipes.json` records the real CMake/API interfaces and explicit policy data.
First use an independently built UWVM_HUMANOID_POSITIVE=OFF artifact in a fresh
WASI basic-I/O Source with no ROS enable/grants. Confirm actual open13 and zero
native command observations. Then use a separate positive artifact/Source and
explicit single-target Runtime connection, complete grant and limits. The real
syntax is --ros2-single-create main, --ros2-single-enable main,
--ros2-single-grant main ALIAS OP RESOURCE TYPE ACTUAL_NATIVE_ID and one
--ros2-single-set main ALIAS KEY VALUE for every field. Limx-command publish
and limx-state subscribe are distinct owners. Match their complete trusted
Gateway config, actual worker PID/UID, canonical identity and reliable/volatile/
keep-last depth1 QoS. Replace all recipe tokens with independently admitted
actual values; no placeholder is a runnable permission profile.

## 5. Bound the trusted native process and verify effects

The actual native bridge is built with UWVM_LIMX_ENABLE_NATIVE_SEND OFF by
default and returns13 before SDK init. A positive separately owned process takes
CLASS MODEL IP NATIVE_STATE NATIVE_IMU NATIVE_COMMAND ROS_PREFIX WINDOW_MS
MAX_COMMANDS EXPECTED_MOTORS. No IP, native endpoint, robot model, duration or
count is selected implicitly. Window is1..2000ms, command attempts1..32, motors
1..64. Its PointFoot example requires PF_P441A and6, plus the real SDK name map.

The process uses actual raw SDK openSub for JointState/IMUData and openPub for
JointCmd or JointCmdLimx, with the SDK's real TYPE/MD5/full definition. Each
incoming frame is bounded to32768 bytes. The actual SDK callback performs only
a try-lock and full copy into one preallocated slot per stream: contention or a
full slot is terminal105, with no overwrite. Genuine SDK decode, allocation,
complete conversion and ROS publish run outside that callback in the finite
executor loop. Decode must consume the whole frame.
The cap bounds one accepted frame, not the SDK's earlier internal network
buffers. The external process/cgroup owner must bound those native allocations.

The command path requires a complete recent state received within100ms of the
native host's steady clock, checks its finite lease again after private encode,
then increments attempts before one actual publishRaw. This age is local receive
age, not an invented hardware timestamp. SDK publishRaw success means it copied
submission bytes, not that an actuator acknowledged execution. Error, timeout,
trap, lost transport or unknown outcome does not cause command replay.

Root must separately supervise the whole process because an opaque SDK init,
callback or close is not guaranteed to finish within the local loop's window.
The SDK singleton is borrowed only for this process lifetime and is never
freed through a guessed API or reused between Runtime Source generations.

## 6. Inspect actual values without extending authority

Build with real DWARF and choose a numeric Wasm module using info modules.
Set `bs MODULE FILE:LINE` at the command publish line. At the actual stopped
frame use `print /r command`, `print command.header.seq`,
`print command.names.size`, `print command.q.data[5]` and
`print command.mode.data[5]`. In the telemetry callback inspect `value.header`,
all sequence lengths and the last vd/tau values. `set print elements 64` adjusts
only debug display; it changes no native/canonical bound. Unsupported layouts
or missing debug values remain unavailable. Use info ros capabilities/sessions/
queues/stats for actual local ownership; no display is a fabricated native
acceptance result.

A debug pause can outlast the finite native lease. Resuming then refuses a new
send, instead of secretly extending the lease. A new command needs a new real
external owner and reviewed value. Higher languages should reuse their actual
HLL SDK and these complete emitted records, not opaque blob templates or native
ABI shortcuts.

On close the process first makes callbacks inactive, then calls each actual
closeSub outside its callback/mutex. The selected SDK explicitly documents that
closeSub returns only after that callback stops and cannot be called from its
callback. IMU, state and command close attempts/errors and ROS shutdown remain
independent. A throwing close is unknown and not retried; process exit prevents
unsafe userdata reclamation. Closing a publisher/lease is not a hardware stop,
mode reset or native operation cancellation. Those outcomes and final robot
state require an independent real controller/firmware safety protocol.


## Actual generated SDK source delivery

The later Root implementation stage actually generated and independently
reviewed the following complete-value SDK source bundles. Earlier authored-only
statements above describe the original kit milestone; they do not erase this
separate actual Source-generation result. No compiler, Guest, Native, robot or
performance qualification follows from this delivery.

- [`sdk/custom-types/limx-bridge-complete`](../../../custom-types/limx-bridge-complete/README.md): [{'adapter_codec_identity': 'UWVMROS1_d86b96b26db6c2a28dfa86e2255cb3cccd6cd24648e877746d8d75394d1635b2', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_d86b96b26db6c2a28dfa86e2255cb3cccd6cd24648e877746d8d75394d1635b2', 'expanded_nodes': 5, 'maximum_wire_bytes': 144, 'minimum_wire_bytes': 16, 'native_identity': None, 'type': 'uwvm_limx_bridge/msg/NativeHeader'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'uwvm_limx_bridge/msg/NativeHeader'}, {'adapter_codec_identity': 'UWVMROS1_53ba9eb23b36de77e1793bdd19a31f708aca557b81f4ae72927b2a44f5ce097b', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_53ba9eb23b36de77e1793bdd19a31f708aca557b81f4ae72927b2a44f5ce097b', 'expanded_nodes': 25, 'maximum_wire_bytes': 260, 'minimum_wire_bytes': 132, 'native_identity': None, 'type': 'uwvm_limx_bridge/msg/ImuData'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'uwvm_limx_bridge/msg/ImuData'}, {'adapter_codec_identity': 'UWVMROS1_d99e5855c0dd55d4d1b5a724ad5719734a668a512ea09e553c8f4a560b97a3a4', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_d99e5855c0dd55d4d1b5a724ad5719734a668a512ea09e553c8f4a560b97a3a4', 'expanded_nodes': 332, 'maximum_wire_bytes': 5544, 'minimum_wire_bytes': 40, 'native_identity': None, 'type': 'uwvm_limx_bridge/msg/JointState'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'uwvm_limx_bridge/msg/JointState'}, {'adapter_codec_identity': 'UWVMROS1_8c477a93cc192cd3039b285f111a56eb1305707d798587ef7ab9504f714ce6f0', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_8c477a93cc192cd3039b285f111a56eb1305707d798587ef7ab9504f714ce6f0', 'expanded_nodes': 462, 'maximum_wire_bytes': 5872, 'minimum_wire_bytes': 48, 'native_identity': None, 'type': 'uwvm_limx_bridge/msg/JointCmd'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'uwvm_limx_bridge/msg/JointCmd'}, {'adapter_codec_identity': 'UWVMROS1_d469dd32f446fb4013144b7e176cdb831c146b6a9279ea776ec4a0c053060be9', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_d469dd32f446fb4013144b7e176cdb831c146b6a9279ea776ec4a0c053060be9', 'expanded_nodes': 527, 'maximum_wire_bytes': 5940, 'minimum_wire_bytes': 52, 'native_identity': None, 'type': 'uwvm_limx_bridge/msg/JointCmdLimx'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'uwvm_limx_bridge/msg/JointCmdLimx'}] interfaces, 5 complete records, C/C++ and four managed record targets.

Use the directory layout and language instructions in each bundle README.
Keep the original selected catalog/definition bytes and one coherent build
domain. Native identities remain absent until a genuine separately owned native
export/binding and a new bound generation; source codec IDs are not grants.
