# Booster: complete ROS values and an explicit native SDK bridge

This package supplies source for two distinct ROS2 families, complete typed
Guest examples, real installed-TypeSupport exporter entry points, and a finite
trusted native SDK bridge. No generator, compiler, model, Runtime, native bridge
or robot was run while authoring it. Every identity in a preparation catalog is
unbound until a separately owned actual exporter supplies it. Default authority
is empty; both native networking switches and Guest positive switches are OFF.

## 1. Select the exact source and robot

The official ROS family is retained from BoosterRobotics/robocup_demo commit
`fa4f31ccb4428a9e60e90b4f54eb80499caeb0ca`, tree
`049d793782c96b86559107fd5e7c5a78040e4581`. Its sixteen original messages and
both original services are copied whole under `source-definitions/ros2`.
`std_msgs/Header` and `builtin_interfaces/Time` complete the selected closure.
The original LowCmd and ButtonEvent/FallDown constants are retained, not stripped
before parsing. RawBytes `char[]` retains ROS2's char/uint8 metadata semantics;
it is not silently changed to the distinct byte/octet declaration.

The native SDK selection is BoosterRobotics/booster_robotics_sdk commit
`87a9a26d06a94ddd50968e7576f1170f6fa5a80f`, tree
`79583bf6e97b44e92907566f758903a243089016`. Its README names Ubuntu22.04,
aarch64/x86_64 and GCC11.4.0. Those source statements do not prove the installed
binary, firmware or machine ABI. SDK CMake's release-project version0.1.0 is not
a firmware version. The selected SDK defines K1's22, T1's23 and T1_7DofArm's29
joint indices. The concrete bridge main in this package admits **T1/23** only;
its29-element canonical quota is not permission to command another model.

Record the actual SDK headers/library SHA and build/link closure, actual model,
firmware/version, serial and operating mode in the native owner receipt. The
read-only `booster-sdk-observe` executable uses real GetRobotInfo2022 and
GetMode2017 RPCs, checks exact caller-selected model/version/mode and retains
both full native JSON responses. It opens its own explicitly selected native
RPC transport, has a finite2s observation deadline and calls Stop once. It never
changes mode. Its observations are not signed firmware attestation.

## 2. Keep ROS and native SDK types separate

The official ROS MotorCmd.mode and MotorState.mode/temperature are **int8**.
The selected native SDK's corresponding members are **uint8_t**. Its LowCmd
cmd_type is a native CmdType enum. Equal width does not make these the same
schema, and a value255 must not turn into a claimed valid signed ROS value.

`sdk_bridge_interfaces` is a NEW authored ROSIDL package named
`uwvm_booster_sdk_bridge`. Its five complete messages preserve SDK unsigned
fields and all remaining native fields. LowCmd's explicit int32 cmd_type is
accepted only for the actual parallel0/serial1 enum values. This package has its
own genuine ROS2 generated types/TypeSupport/hash after an actual build; it is
not an official Booster package and cannot reuse an official ROS RIHS identity.
The two source domains and profiles remain separate.

The native SDK and official ROS package even define classes in the same
`booster_interface::msg` namespace. The native bridge must not include both in
one translation unit. `native/CMakeLists.txt` gives the original exporter only
original ROS targets, and gives SDK bridge/contract targets only the NEW bridge
package plus the selected native SDK. `sdk_conversion.hpp` copies actual typed
objects field by field into a private destination and commits only after full
validation; it never casts between TypeSupport objects or native layouts.

LowState includes every IMU rpy/gyro/acc value and both motor-state sequences.
Every motor preserves mode, q, dq, ddq, tau_est, temperature, lost and both reserve
words. LowCmd preserves every mode, q, dq, tau, kp, kd and weight. Both sequences
are bounded at29; overbound input returns90 without changing the destination.
Nonfinite floats, invalid enum values and incomplete shapes return22. There is
no native value truncation or projection. `native/sdk_value_contract.cpp`
contains whole-value round trips and late-field/255/quota/sentinel cases over
the actual SDK and real generated bridge objects; it has not been executed.

## 3. Prepare the real complete codecs and identities

Use the explicit `uwvm-ros-vendor-complete-bounded-v1` mode of
`sdk/robots/industrial/vendor_profile.py`, through Root's reviewed vendor
preparation lane. The older vendor19 Git-only preparer does not understand this
new domain. Do not relabel authored bridge definitions as files in the official
SDK Git tree to make that preparer accept them.

Select ONE complete catalog and output Source revision: original20 in
`source-definitions/ros2/catalog.preparation.json`, or SDK bridge5 in
`source-definitions/sdk_bridge/catalog.preparation.json`. Pass the captured
current parent/bounded generator bytes, all four real managed interface
sources, the adapter's own bytes and exact source hashes to its actual API.
`API.products/generate(..., guest_only=True, helper_source=...)` emits records,
complete constants and the genuine `native_identity.cc` source. No Node or
permission is created. Do not hand-create a missing `bounded_records.hpp`.

The `native/export_identities.cpp` and `native/export_sdk_bridge_identities.cpp`
entries strongly include that exact actual generated exporter. Their CMake
requires the actual bundle's native_identity.cc, native_constants.hpp and
generation.vendor.json. It instantiates genuine installed types and constant
proofs; modern TypeSupport warmup precedes the real SIGSTOP/I input gate.
The raw export format is `uwvm-ros-industrial-native-v1`, with exact
source_profile, actual constant_schema_sha256, complete roots/components and
full modern referenced descriptions. A similarly shaped old native-v2 JSON is
not this domain's input. Feed the actual export into API.bind, retain the
independent stopped process/source/MD/link/SDK/ELF/maps evidence, and emit the
new bound revision. Native execution and later robot acceptance stay separate.

Root's Gateway/native provider must explicitly select this complete source
profile and actual canonical/native identities. Constants or modern metadata
must not be admitted by bypassing an older matcher's refusal. The package adds
no hidden global registry, automatic grant or native socket import to Wasm.

## 4. Start with the raw import boundary, then the typed SDK

`examples/raw_command.cpp` calls the real uwvm_ros2_v1 capability_open,
topic_publish and capability_close signatures. It passes i64 handle/offsets,
a selected linear-memory index0 and an eight-byte LE handle sentinel. Default
mode expects error13 with an untouched sentinel. Positive mode first decodes
one whole typed canonical LowCmd, checks T1's23 motors and enum, displays every
command field, then opens one alias and attempts one publish. An error after
that attempt is never replayed. Close has its own result.

`examples/complete_command.cpp` uses the same full record with the real
roswasmcpp::Publisher<Command> and its actual Codec specialization. The stdin
protocol is exactly one LE32 body length followed by that complete canonical
body. It does not invent a safe posture, zero-torque hold, control mode or gain.
The application supplies a real simulator/controller-approved command whose
q/dq/tau/kp/kd/weight/mode values have been independently checked.

`examples/telemetry.cpp` owns a real typed Subscription, polls only when given
an actual R byte, and stops after one complete record or64 caller credits.
Idle11 is not delivery and credits are not elapsed time. It re-encodes the
whole decoded LowState, displays all IMU and motor fields through `values.hpp`,
then records primary/close/rollback outcomes. No timer, graph query, raw socket,
mode change or management capability is inferred from an alias.

`examples/original_ros2` is the separate official int8 route. Its generated
Booster_interface record must come from the original20 catalog, not the SDK
bridge5 bundle. It is not interchangeable with the unsigned SDK bridge.

## 5. Separate denied and positive instances

`recipes.json` lists the actual source/CMake interfaces and a complete finite
capability template. Replace every external input with an actually admitted
absolute artifact/identity before constructing argv; an unresolved token is an
error. These are future instructions, not commands executed by this package.

First build the examples with UWVM_HUMANOID_POSITIVE=OFF. Create a fresh Runtime
Source with WASI basic stdin/stdout only and **no ROS enable or grant options**.
The attempt must return13, keep no live owner and produce zero independently
observed native commands. A process exit0 alone does not establish zero effects.

Use a distinct positive Source/artifact with UWVM_HUMANOID_POSITIVE=ON and an
explicit single-target policy. The actual Runtime syntax is
`--ros2-single-create main`, `--ros2-single-enable main`, connection options,
`--ros2-single-grant main ALIAS OP RESOURCE TYPE ACTUAL_NATIVE_ID`, followed by
`--ros2-single-set main ALIAS KEY VALUE` for every limit, including the actual
adapter-codec-identity. Only booster-sdk-lowcmd/topic.publish and
booster-sdk-lowstate/topic.subscribe are needed for the SDK example; they are
independent owners. A matching trusted Gateway config mirrors the complete
policy and actual worker PID/UID. Use reliable/volatile/keep-last depth1 and
explicit remaining DDS duration/liveliness fields as shown. No ROS rights,
clock, network, filesystem or SDK administration follow from enabling WASI.

For the SDK route, an independently owned native process must be built with
UWVM_BOOSTER_ENABLE_NATIVE_SEND=ON and explicitly invoked with
`T1 DOMAIN_PLUS_ONE INTERFACE WINDOW_MS MAX_COMMANDS` (1..2000ms,1..32 attempts).
It uses actual SDK rt/joint_ctrl and rt/low_state channels and NEW ROS topics
`/uwvm_booster_sdk_bridge/T1/{command,state}`. It requires a complete23-motor
serial state received within100ms of the native host steady clock before each
send. The SDK callback queue is explicitly latest-only/capacity1. That is callback
receive age, not a forged native source timestamp or proof of hardware age;
the upstream DDS/network may have buffered a value before this callback. `Write` success
is transport submission, not actuator acknowledgement. The actual device mode,
firmware, DDS/domain/interface mapping and command interpretation must be
qualified independently before real hardware is selected.

## 6. Inspect real values and stop safely

Compile actual Guest sources with usable DWARF. In the real debugger, obtain
numeric module IDs with `info modules`, set `bs MODULE FILE:LINE` at the publish
line, and use `print /r command`, `print command.cmd_type`,
`print command.motor_cmd.size`, and `print command.motor_cmd.data[22].weight`.
For the telemetry callback use `print /r value` and inspect both sequence counts,
IMU elements and the last motor's reserve words. `info ros capabilities`,
`info ros sessions`, `info ros queues` and `info ros stats` are read-only state
views. Actual frame/type availability must be checked; an optimized-away value
must not be replaced by a guessed decoder or a synthetic passing screenshot.

Debugger pauses can outlast the native finite lease. Resume must then refuse
sending; do not extend the lease silently or replay a previously attempted
command. A new real attempt needs a fresh external owner/policy and separately
reviewed input. No Guest-side debugger inspection changes ROS authority.

On expiry/poison/shutdown the bridge marks local state inactive before trying
both native transport closes once outside callback locks. Callback backing
storage is strongly retained by real SDK callbacks; unknown cleanup terminates
the independently owned process rather than freeing possibly borrowed state.
Primary, reader-close, writer-close and ROS shutdown errors remain independent.
These actions do not establish a hardware stop or cancel a command already
accepted by a robot. Native RPC Stop closes RPC transport/wakes waits; it is not
ChangeMode, motor damping or operation cancellation. A real independently owned
controller watchdog/emergency-stop and actual final robot state are necessary
for hardware qualification. Higher-language lessons should reuse the actual
closed HLL SDK and this complete generated record family; they must not replace
it with an opaque blob or a native ABI shortcut.


## Actual generated SDK source delivery

The later Root implementation stage actually generated and independently
reviewed the following complete-value SDK source bundles. Earlier authored-only
statements above describe the original kit milestone; they do not erase this
separate actual Source-generation result. No compiler, Guest, Native, robot or
performance qualification follows from this delivery.

- [`sdk/custom-types/booster-ros2-complete`](../../../custom-types/booster-ros2-complete/README.md): [{'adapter_codec_identity': 'UWVMROS1_e2f9b66467bcb40a8194c744694f314e7fca56061f1cf3121eed565f724edd40', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_e2f9b66467bcb40a8194c744694f314e7fca56061f1cf3121eed565f724edd40', 'expanded_nodes': 3, 'maximum_wire_bytes': 4108, 'minimum_wire_bytes': 12, 'native_identity': None, 'type': 'booster_interface/msg/BoosterApiReqMsg'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/BoosterApiReqMsg'}, {'adapter_codec_identity': 'UWVMROS1_21ce208a51f60843eecb72edbc408e926353b1a6f178424c0b691ac237fed11f', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_21ce208a51f60843eecb72edbc408e926353b1a6f178424c0b691ac237fed11f', 'expanded_nodes': 3, 'maximum_wire_bytes': 4108, 'minimum_wire_bytes': 12, 'native_identity': None, 'type': 'booster_interface/msg/BoosterApiRespMsg'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/BoosterApiRespMsg'}, {'adapter_codec_identity': 'UWVMROS1_b61682fd9c35bd54c52000224411f4373b8620e16587b31d8a683076e0d14fe4', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_b61682fd9c35bd54c52000224411f4373b8620e16587b31d8a683076e0d14fe4', 'expanded_nodes': 3, 'maximum_wire_bytes': 5, 'minimum_wire_bytes': 5, 'native_identity': None, 'type': 'booster_interface/msg/ButtonEventMsg'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/ButtonEventMsg'}, {'adapter_codec_identity': 'UWVMROS1_8bf168ccefac72eda6070704c70e12e62776d29ad3db908095cc01220f4b533f', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_8bf168ccefac72eda6070704c70e12e62776d29ad3db908095cc01220f4b533f', 'expanded_nodes': 3, 'maximum_wire_bytes': 5, 'minimum_wire_bytes': 5, 'native_identity': None, 'type': 'booster_interface/msg/FallDownState'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/FallDownState'}, {'adapter_codec_identity': 'UWVMROS1_06f3d6c0e9bef1da409330038aadbfacfe60b02e099f2ae0c4dcb92a632b20a0', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_06f3d6c0e9bef1da409330038aadbfacfe60b02e099f2ae0c4dcb92a632b20a0', 'expanded_nodes': 165, 'maximum_wire_bytes': 528, 'minimum_wire_bytes': 16, 'native_identity': None, 'type': 'booster_interface/msg/HandCommand'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/HandCommand'}, {'adapter_codec_identity': 'UWVMROS1_55af9d6f272a72e75e86e1a6164d235aca5f2ce99c2529749c0d5e33d6091397', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_55af9d6f272a72e75e86e1a6164d235aca5f2ce99c2529749c0d5e33d6091397', 'expanded_nodes': 332, 'maximum_wire_bytes': 1060, 'minimum_wire_bytes': 4, 'native_identity': None, 'type': 'booster_interface/msg/HandDdsMsg'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/HandDdsMsg'}, {'adapter_codec_identity': 'UWVMROS1_3b91722e751e7c00c773335bb42af34850190ecc26a029e8792c49d912def158', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_3b91722e751e7c00c773335bb42af34850190ecc26a029e8792c49d912def158', 'expanded_nodes': 5, 'maximum_wire_bytes': 16, 'minimum_wire_bytes': 16, 'native_identity': None, 'type': 'booster_interface/msg/HandParam'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/HandParam'}, {'adapter_codec_identity': 'UWVMROS1_838ee8d4b93b0c3673e0b535fd3905913e94be24d6d21a01504dedaaaaa81092', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_838ee8d4b93b0c3673e0b535fd3905913e94be24d6d21a01504dedaaaaa81092', 'expanded_nodes': 13, 'maximum_wire_bytes': 36, 'minimum_wire_bytes': 36, 'native_identity': None, 'type': 'booster_interface/msg/ImuState'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/ImuState'}, {'adapter_codec_identity': 'UWVMROS1_845726a14e61d211b493041f4dda56e1556b76b9d30337a2ee5f6319a3d1e870', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_845726a14e61d211b493041f4dda56e1556b76b9d30337a2ee5f6319a3d1e870', 'expanded_nodes': 235, 'maximum_wire_bytes': 730, 'minimum_wire_bytes': 5, 'native_identity': None, 'type': 'booster_interface/msg/LowCmd'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/LowCmd'}, {'adapter_codec_identity': 'UWVMROS1_811db19cab7b790ed48506bca3e3ef512c1a661dc3ac99cbce6234efb6de631f', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_811db19cab7b790ed48506bca3e3ef512c1a661dc3ac99cbce6234efb6de631f', 'expanded_nodes': 654, 'maximum_wire_bytes': 1784, 'minimum_wire_bytes': 44, 'native_identity': None, 'type': 'booster_interface/msg/LowState'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/LowState'}, {'adapter_codec_identity': 'UWVMROS1_69d814e13595d53dcf44f2a5ccb19df0f8f2be0908c310c3b60cec33bcf27392', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_69d814e13595d53dcf44f2a5ccb19df0f8f2be0908c310c3b60cec33bcf27392', 'expanded_nodes': 8, 'maximum_wire_bytes': 25, 'minimum_wire_bytes': 25, 'native_identity': None, 'type': 'booster_interface/msg/MotorCmd'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/MotorCmd'}, {'adapter_codec_identity': 'UWVMROS1_6b19c0e8865e796fff5a498cff731ba268fa92ea2944e6c2c5869009b3aa066b', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_6b19c0e8865e796fff5a498cff731ba268fa92ea2944e6c2c5869009b3aa066b', 'expanded_nodes': 11, 'maximum_wire_bytes': 30, 'minimum_wire_bytes': 30, 'native_identity': None, 'type': 'booster_interface/msg/MotorState'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/MotorState'}, {'adapter_codec_identity': 'UWVMROS1_4fea33e3e2ae186dbeaf49c6068019b702e742266541087a5d280b9ce42c671a', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_4fea33e3e2ae186dbeaf49c6068019b702e742266541087a5d280b9ce42c671a', 'expanded_nodes': 4, 'maximum_wire_bytes': 12, 'minimum_wire_bytes': 12, 'native_identity': None, 'type': 'booster_interface/msg/Odometer'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/Odometer'}, {'adapter_codec_identity': 'UWVMROS1_557e8e79f206c9cc8b820ed7c2ad8cc01e9b01327197b745709993d958b4578c', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_557e8e79f206c9cc8b820ed7c2ad8cc01e9b01327197b745709993d958b4578c', 'expanded_nodes': 2050, 'maximum_wire_bytes': 2052, 'minimum_wire_bytes': 4, 'native_identity': None, 'type': 'booster_interface/msg/RawBytesMsg'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/RawBytesMsg'}, {'adapter_codec_identity': 'UWVMROS1_27cddd472b2d8c41c39effb87a8a53261aff309500dbb68df6ebd69a19c7720e', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_27cddd472b2d8c41c39effb87a8a53261aff309500dbb68df6ebd69a19c7720e', 'expanded_nodes': 2055, 'maximum_wire_bytes': 2192, 'minimum_wire_bytes': 16, 'native_identity': None, 'type': 'booster_interface/msg/RawBytesStamped'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/RawBytesStamped'}, {'adapter_codec_identity': 'UWVMROS1_f8a21705728862632c27b71d98c044a12bf1f9f9e0c1729fec27afe2c60c48a7', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_f8a21705728862632c27b71d98c044a12bf1f9f9e0c1729fec27afe2c60c48a7', 'expanded_nodes': 28, 'maximum_wire_bytes': 42, 'minimum_wire_bytes': 42, 'native_identity': None, 'type': 'booster_interface/msg/RemoteControllerState'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/msg/RemoteControllerState'}, {'adapter_codec_identity': 'UWVMROS1_3a612d975894437a954b8d197895c1dc07d1cab3a35b8912d63b0308a0fd9bd3', 'components': {'request': {'adapter_codec_identity': 'UWVMROS1_d8b972ee6c7f82ee948c03b22354d1073f120ad2616a9e833b578348775404e8', 'expanded_nodes': 2, 'maximum_wire_bytes': 4100, 'minimum_wire_bytes': 4, 'native_identity': None, 'type': 'booster_interface/srv/AgentService_Request'}, 'response': {'adapter_codec_identity': 'UWVMROS1_86aecf9ee8652016b5c74e30d2dd4100e8b547542a81a98e7795942b47b87520', 'expanded_nodes': 2, 'maximum_wire_bytes': 4100, 'minimum_wire_bytes': 4, 'native_identity': None, 'type': 'booster_interface/srv/AgentService_Response'}}, 'kind': 'service', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/srv/AgentService'}, {'adapter_codec_identity': 'UWVMROS1_08c19ac1f3b5280e9d1fc4662e19a56d3c2512e8b63127e92c4676cd7b32b04f', 'components': {'request': {'adapter_codec_identity': 'UWVMROS1_cc8eb67d80bdf131f1878a1be998d68560e235ffef6a539876410dfcd6c00150', 'expanded_nodes': 4, 'maximum_wire_bytes': 4108, 'minimum_wire_bytes': 12, 'native_identity': None, 'type': 'booster_interface/srv/RpcService_Request'}, 'response': {'adapter_codec_identity': 'UWVMROS1_7aeb7b45542f9f594e1f61be2f715abcde54b1e55efdde15100b0c553ae96322', 'expanded_nodes': 4, 'maximum_wire_bytes': 4108, 'minimum_wire_bytes': 12, 'native_identity': None, 'type': 'booster_interface/srv/RpcService_Response'}}, 'kind': 'service', 'native_identity': None, 'ros_version': 2, 'type': 'booster_interface/srv/RpcService'}, {'adapter_codec_identity': 'UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96', 'expanded_nodes': 5, 'maximum_wire_bytes': 140, 'minimum_wire_bytes': 12, 'native_identity': None, 'type': 'std_msgs/msg/Header'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'std_msgs/msg/Header'}, {'adapter_codec_identity': 'UWVMROS1_0fd520c23fd8cb06e2c27292fdc20694732fa9332d452fc3fa31da90f7605fb7', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_0fd520c23fd8cb06e2c27292fdc20694732fa9332d452fc3fa31da90f7605fb7', 'expanded_nodes': 3, 'maximum_wire_bytes': 8, 'minimum_wire_bytes': 8, 'native_identity': None, 'type': 'builtin_interfaces/msg/Time'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'builtin_interfaces/msg/Time'}] interfaces, 22 complete records, C/C++ and four managed record targets.
- [`sdk/custom-types/booster-sdk-complete`](../../../custom-types/booster-sdk-complete/README.md): [{'adapter_codec_identity': 'UWVMROS1_858ad4a47d2409da6b4e2513178d7d3b0d4568d6ce488729de0b9a220a100f04', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_858ad4a47d2409da6b4e2513178d7d3b0d4568d6ce488729de0b9a220a100f04', 'expanded_nodes': 13, 'maximum_wire_bytes': 36, 'minimum_wire_bytes': 36, 'native_identity': None, 'type': 'uwvm_booster_sdk_bridge/msg/ImuState'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'uwvm_booster_sdk_bridge/msg/ImuState'}, {'adapter_codec_identity': 'UWVMROS1_66fec912031dfbd29b8e4e747dcdc2dc465506bd003323bc929929bf6a9d31a4', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_66fec912031dfbd29b8e4e747dcdc2dc465506bd003323bc929929bf6a9d31a4', 'expanded_nodes': 8, 'maximum_wire_bytes': 25, 'minimum_wire_bytes': 25, 'native_identity': None, 'type': 'uwvm_booster_sdk_bridge/msg/MotorCmd'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'uwvm_booster_sdk_bridge/msg/MotorCmd'}, {'adapter_codec_identity': 'UWVMROS1_958168dc1e32d50827f255080117a82421d7fb6ae4ad6a05763ce2d3173a30af', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_958168dc1e32d50827f255080117a82421d7fb6ae4ad6a05763ce2d3173a30af', 'expanded_nodes': 11, 'maximum_wire_bytes': 30, 'minimum_wire_bytes': 30, 'native_identity': None, 'type': 'uwvm_booster_sdk_bridge/msg/MotorState'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'uwvm_booster_sdk_bridge/msg/MotorState'}, {'adapter_codec_identity': 'UWVMROS1_3324a161b056f9282ffa8dce3254a1456b75f6eae5f4a067e844e125e94cba3a', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_3324a161b056f9282ffa8dce3254a1456b75f6eae5f4a067e844e125e94cba3a', 'expanded_nodes': 235, 'maximum_wire_bytes': 733, 'minimum_wire_bytes': 8, 'native_identity': None, 'type': 'uwvm_booster_sdk_bridge/msg/LowCmd'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'uwvm_booster_sdk_bridge/msg/LowCmd'}, {'adapter_codec_identity': 'UWVMROS1_ca4bcb79077b0e026cf45a389981622ff9c80570ee813ff5633c605fe5021a58', 'components': {'message': {'adapter_codec_identity': 'UWVMROS1_ca4bcb79077b0e026cf45a389981622ff9c80570ee813ff5633c605fe5021a58', 'expanded_nodes': 654, 'maximum_wire_bytes': 1784, 'minimum_wire_bytes': 44, 'native_identity': None, 'type': 'uwvm_booster_sdk_bridge/msg/LowState'}}, 'kind': 'message', 'native_identity': None, 'ros_version': 2, 'type': 'uwvm_booster_sdk_bridge/msg/LowState'}] interfaces, 5 complete records, C/C++ and four managed record targets.

Use the directory layout and language instructions in each bundle README.
Keep the original selected catalog/definition bytes and one coherent build
domain. Native identities remain absent until a genuine separately owned native
export/binding and a new bound generation; source codec IDs are not grants.
