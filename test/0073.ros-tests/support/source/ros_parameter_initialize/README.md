# Initialize a declared static ROS2 parameter

This focused Jazzy test checks a real remote parameter declared with an explicit
Bool type and no initial value. It uses the official `rclcpp::Node` parameter
services, an independent `AsyncParametersClient`, and authenticated Gateway IPC
in ROS domain 202.

The observed Jazzy GetParameters response for the uninitialized static parameter
has zero values. Its DescribeParameters response still has exactly one matching
name, `PARAMETER_BOOL`, `dynamic_typing=false` and `read_only=false`. The old
adapter rejects a typed setter with 95 before invoking the native setter.
The official SDK successfully initializes the same parameter, and the same
Gateway client then reads its value. The before attempt keeps its actual FAIL.

The fixed adapter lets an empty GetParameters result or a single NOT_SET value
continue to DescribeParameters. It requires the exact requested name and
selected static kind before sending SetParameters. These preliminary results
do not establish a previous concrete value. Missing or dynamic parameters and
wrong kinds remain refused; multiple values/descriptors or a mismatched name
remain malformed. All service stages share the original deadline and the
original input validation remains in force. This is initialization of an
already declared static parameter, not a remote declaration API or an atomic
conditional mutation.

The native test checks the uninitialized Bool case, an initialized static Bool
control, a dynamic NOT_SET parameter, a statically declared uninitialized
integer presented to the Bool adapter, and a missing parameter. It counts
actual native setter callbacks, checks same-client reads, and separately
initializes the selected parameter with the official SDK. Only this Bool
representative is exercised; it does not claim to run all scalar/array types.

Run on Linux with the existing Jazzy SDK rootfs and an effective 64 GiB cgroup
limit. Every output path must be new:

```sh
python3 -B test/0073.ros-tests/support/source/ros_parameter_initialize/run_native.py \
  --source /absolute/frozen/source \
  --rootfs-workspace /absolute/sdk/workspace \
  --out /absolute/new/attempt
```

Add `--gateway /absolute/gateway` to snapshot a prebuilt Gateway. Its original
build provenance must be verified separately. `--probe-source` can identify a
separately frozen complete probe revision. The runner mounts a private SDK
clone with its own temporary directory and ROS logs, binds source read-only,
records command logs, preexec and pinned actual-main identity, raw compiler
dependencies, and cleanup. The native probe is C++20 with fast_io; a self-built
Gateway remains C++17. It does not rebuild a Worker or run the historical A15
matrix, and it does not sanitizer-instrument the official ROS libraries.

The first fixture's incorrect assumption that official static-uninitialized
GetParameters returns tag0, and a subsequent fast_io raw-pointer print compile
failure, remain separate original failures. The corrected fixture directly
observes the generated service response. Earlier report and executed source
bytes must not be overwritten.
