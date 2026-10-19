# Hello Robot Stretch

The [shared complete Humble SDK](../../../custom-types/mobile-fjt-humble-complete/README.md)
now supplies all selected JointState/FJT records and RPC owners in C, C++, Rust,
JavaScript, AssemblyScript and TinyGo. The separately scoped current C/C++ Wasm
build/default-denial results are linked from the
[platform status](../../../../documents/ros/platform-sdk-status.md).
Native mode/stop/navigation, driver and physical acceptance remain separate.

This profile is based on official `stretch_ros2` Humble commit
`751e701b87d6f34dd26df1c4abc39229d68e60ff` and independently fixed
`stretch_body` commit `92c9db0cc9ade6535f822e83bbf10cbd0c68136a`.
RE1V0/RE2V0/SE3 parameter source variants are copied without modification.
A model string is not proof of a particular installed tool, motor firmware,
calibration, homing state or driver configuration.

The actual action server is `/stretch_controller/follow_joint_trajectory` with
the complete standard FollowJointTrajectory type. The actual driver publishes
JointState on `joint_states` and `joint_limits`, plus battery, odometry, mode,
tool, IMU, magnetometer, gamepad and runstop telemetry. Their original standard
definitions are preserved; these additional endpoints do not receive implicit
permissions. The SDK convenience example selects only JointState and FJT.

Position, trajectory and navigation mode have different native validation and
joint naming. In the actual trajectory components, `joint_lift` is in meters,
`wrist_extension` is the arm component, `joint_head_pan/tilt` and wrist angles
are radians, gripper conversion is tool-dependent, and the base uses a genuine
MultiDOF trajectory. Do not turn a base target into joint positions or strip
MultiDOF fields. `StretchClient` records a selected mode but never changes it.

Native Trigger endpoints `/switch_to_trajectory_mode` and `/stop_the_robot`
are separate operations. Other real driver endpoints include home/stow and
mode changes; they remain ungranted unless independently selected. Runstop uses
SetBool and is not a success stub. A successful Trigger response is the actual
driver's success/message; it is not certified emergency braking or proof every
actuator is stationary. Cancel acceptance on FJT likewise requires actual
terminal feedback and telemetry. Timeouts retain unknown outcomes.

The native example's `lift` operation requires an already qualified, homed
trajectory-mode deployment. It does not automatically switch mode. Choose
`--operation trajectory-mode` or `--operation stop` as an independent explicit
native witness invocation. No operation runs without `--execute`.

Only core/helper/body sources relevant to this profile are copied. Their exact
Apache license notices are retained. Other packages in the upstream repository
have different licenses; this package does not relicense the entire repository.
SDK, generated product, actual ROS version, two-source concurrency, native
metadata, physical cancel/stop and Wasm execution all remain unqualified.
