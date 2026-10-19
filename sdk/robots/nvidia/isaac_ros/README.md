# Isaac ROS full AprilTag values

The selected fixed NVIDIA source publishes the genuine
`isaac_ros_apriltag_interfaces/msg/AprilTagDetectionArray` at tag_detections.
The full catalog preserves array Header; every detection's family/id/center/four
corners; its independently stamped complete PoseWithCovariance; all36 covariance
values; nested Point/Quaternion/Time and full source files/licenses. A maximum16
detections,64-byte family/frame text and finite floating values are explicit
canonical quotas. They do not reduce native detector capacities or turn64
native detections into16 successful detections; the whole oversized value is90.

The actual current detector uses rosidl::Buffer and CUDA backend input promotion,
while exposing genuine CPU-visible typed AprilTag result messages. The NITROS
repository's fixed README announces deprecation in favor of the new native
buffer path. This package does not cast a GPU buffer into a CPU vector, expose
CUDA pointers through Wasm, or claim the old NITROS ABI still works. Image input
bridging needs its own actual CPU-copy/format/stride/lifetime proof; only the
complete result profile is selected here. Sensor/camera/calibration/TF hardware
are trusted native dependencies, not Guest privileges.

First prepare actual records from the whole catalog with the real complete-v1
vendor producer. Build the modern metadata exporter against the actual
interfaces and all real standard targets, freeze its full actual source/Event/
constant/default graph, then bind the output. `native/CMakeLists.txt` has a real
strong exporter. No Native hash is supplied by this source package.

For a genuine native detector process, explicitly select
UWVM_NVIDIA_BUILD_DETECTOR and the concrete exported/build target from the actual
NVIDIA SDK (the original Source creates apriltag_node). Do not invent a target
name or suppress unresolved symbols. UWVM_NVIDIA_ENABLE_NATIVE_DETECTOR defaults
OFF and returns13. The positive executable takes DOMAIN LIFETIME_MS IMAGE_TOPIC
CAMERA_INFO_TOPIC DETECTIONS_TOPIC; lifetime1..10000ms. It constructs the actual
original AprilTagNode and SingleThreadedExecutor, keeps full default NodeOptions
and native backend semantics, uses finite positive spin slices, then retires
executor/node/context separately. A blocking native callback still requires the
outer process owner. It is perception only, not a robot driver.

Build examples with the real wasm32 WASI toolchain and actual bound bundle.
telemetry.cpp opens only nvidia-apriltags and exposes every typed value, including
all corners/covariance entries; it never counts opaque blobs as SDK support.
The default build expects denial13. A separate positive Source needs only its
explicit topic.subscribe alias, full actual native/root/canonical identity,
message and byte quotas and peer-bound Gateway configuration. Each stdin R asks
one actual poll, up to64 observations; it is not a fake clock. Receive error,
primary output failure and independent close/rollback are retained. Follow the
root tutorial for actual CLI and C++ DBG steps; zero effects need an independent
native observation, not only a process return code.
