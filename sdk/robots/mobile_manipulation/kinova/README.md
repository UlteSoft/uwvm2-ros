# Kinova Gen3

The [shared complete Humble SDK](../../../custom-types/mobile-fjt-humble-complete/README.md)
now supplies all selected JointState/FJT records and RPC owners in C, C++, Rust,
JavaScript, AssemblyScript and TinyGo. The separately scoped current C/C++ Wasm
build/default-denial results are linked from the
[platform status](../../../../documents/ros/platform-sdk-status.md).
The original native-peer/physical recipes described below remain unexecuted.

The selected official ROS2 Kortex source is commit
`b2aa46b29ae8fadd5b293c57e3500925c61eb8e8` (tree
`bf29ce6eaf77e97d4d0a21370d41dd34747b6d74`). The controller YAMLs name
`joint_trajectory_controller/JointTrajectoryController`, position command
interfaces, position/velocity state interfaces and a complete joint list
`joint_1` through `joint_6` or `joint_7`. Partial joint goals are disabled.
The expected default action name is
`/joint_trajectory_controller/follow_joint_trajectory`; deployment namespaces
must be explicit and match the genuine native driver and policy resource.

Use the exact 6DOF/7DOF model selection in `Gen3Client`. It wraps full standard
FollowJointTrajectory codecs. Neither its name check nor the original canonical
encoder validates physical joint limits, torque, collisions, singularities,
gripper configuration or the active controller. Require real JointState first
and have the operator verify the complete native driver state.

The native driver maintains its own authenticated Kortex sessions and low-level
cyclic data connection. Runtime capability owners are not those sessions and
are not a fabricated vendor lease. No Guest connection to a robot IP, password,
Kortex UDP transport or native shared library is provided here. The pinned native
API recipe points at Kortex 2.8.0 Linux x86-64; the ZIP's upstream MD5 is only
quoted upstream evidence, not a locally verified binary SHA or firmware proof.

The SDK gives cancellation only for the exact owned FJT goal. Acceptance is not
physical stop. Wait for the real result and independent telemetry. If the reply
or result deadline is lost, keep unknown state and use the operator's separately
qualified hardware stop procedure. This cohort supplies no invented universal
Kinova Stop ROS service. It never force-deactivates a controller on failure.

`examples/native_peer.py` requires an explicit model, an operator positions JSON
file and `--execute`. The complete standard Goal is constructed using the real
generated type; a single two-second JointTrajectoryPoint is a convenience
example, not a reduction of the interface. All tolerances, multi-DOF trajectory
and feedback remain available to the complete SDK. The optional `--cancel`
issues one real own-goal cancel and still waits for the actual result.
Every executable recipe and physical test remains unrun.
