# Build the ROS extension

The supported host is Linux, C++26, header build and the uwvm interpreter. UWVM does not include or link ROS headers/libraries. Noetic and Jazzy Gateways are independent C++17 executables built against their own distribution; neither needs the other distribution installed.

## Host

For the complete vendor/platform option table, an executable configuration
printer for **only A+B enabled**, coherent catalog preparation, cross-target ABI
selection and actual ELF size budgets, see
[vendor and platform build configuration](vendor-build-options.md). All vendor
and ROS import switches default off; choosing schemas never supplies authority.

The existing xmake build adds two disabled-by-default options:

```sh
xmake f --use-cxx-module=n --use-llvm-compiler=y --import-ros1=y --import-ros2=y
xmake b uwvm
```

Set either import option to `n` to omit that backend. Enabling an import makes its ABI and CLI category available; it does not grant any Guest permission. Execute enabled ROS with `-Rint`. Native LLVM execution with enabled ROS is rejected before initialization because native tail-call ancestry has not been authenticated. C++ named-module builds with ROS imports currently report UNSUPPORTED.

When WASI Preview1 is enabled for a ROS module, explicitly select `--wasip1-global-noinherit-system-environment` or its per-module/group equivalent. Strict ROS execution rejects inherited Host environment, directory/socket preopens, socket standard descriptors and additional native plugins before Guest initialization. A policy-only diagnostic does not execute these ingress checks. Tests record the intended rejection diagnostic as well as the startup exit code.

For independent, reproducible interpreter builds without downloading/linking LLVM backend libraries:

```sh
CXX=clang++ ROS_PRODUCT_CXXFLAGS='' \
  python3 tests/ros/build_product.py --profile all --out /absolute/new/build-directory
```

This copies the required source/header trees into an immutable input snapshot, records each input SHA-256, builds distinct runtime/main/host API objects for each of `none`, `ros1`, `ros2`, `dual`, and records commands, logs and product SHA-256. It uses one compiler process at a time and bounds compilation to 300 seconds per translation unit. `--opt 0` is available for a debug acceptance build. Set the compiler's dynamic library path when a standalone toolchain needs it; this path is a host build setting and is never Guest authorization. Actual build/test admission in this project requires the existing hard 64 GiB/swap0 Linux cgroup; a job count alone is not a memory limit.

## ROS1 Noetic Gateway

Use a Noetic environment with roscpp, std_msgs, geometry_msgs, sensor_msgs, std_srvs, actionlib_tutorials, tf2, tf2_ros and tf2_msgs development files:

```sh
. /opt/ros/noetic/setup.sh
cmake -S gateways/ros1 -B /absolute/ros1-build -DCMAKE_BUILD_TYPE=Release
cmake --build /absolute/ros1-build --parallel 2
```

The resulting `uwvm-ros1-gateway` and `uwvm-ros1-native-probe` link the actual generated message traits and serializers from that environment.

## ROS2 Jazzy Gateway

Use a separate Jazzy environment with rclcpp, rclcpp_action, rcl_interfaces, rcl_action, action_tutorials_interfaces, lifecycle_msgs, rclcpp_lifecycle, tf2, tf2_ros, tf2_msgs and the generated std_msgs/geometry_msgs/sensor_msgs/std_srvs interfaces:

```sh
. /opt/ros/jazzy/setup.sh
cmake -S gateways/ros2 -B /absolute/ros2-build -DCMAKE_BUILD_TYPE=Release
cmake --build /absolute/ros2-build --parallel 2
```

`uwvm-ros2-gateway` and `uwvm-ros2-native-probe` use generated Jazzy type-support hashes. No public type name substitutes for verification of the actual generated RIHS01 hash.

For the prepared official rootfs and existing64GiB acceptance cgroup, a fresh18-target native build and exact input archive can be reproduced with:

```sh
python3 tests/ros/build_gateways.py --workspace /tmp/uwvm-ros-extension-20261012 --tag fresh-scalar
```

The entrypoint requires the prepared `/work` source mounts and preexisting canonical build symlinks. It compiles one target at a time, bounds each distribution build to600seconds, checks source stability and OOM counters, and changes the canonical symlinks only after both builds pass. It preserves previous builds, copies the project dependencies recorded by each native compiler plus CMake inputs, and records exact commands, package versions, flags, links and18 binary hashes. Run this inside the same `docker exec … chroot /host` environment described in [testing.md](testing.md).

## Trusted launcher

A Gateway accepts a trusted configuration file owned by its effective UID, which must be regular and not writable by group/other. It requires explicit `backend`, `worker-uid`, exact `worker-pid`, `socket`, `timeout-ms`, `max-sessions`, `audit-max-records-per-second`, `audit-max-records` and `audit-max-bytes`. Every network connection field and complete resource grant is also supplied. Grant lines use `grant alias operation resource type identity`; limit lines use `set alias key value`; network fields use `connection key value`.

The worker CLI is the only authority granted to Guest. The Gateway configuration is a separate supervisor restriction, not a Guest JSON policy interface. At startup the worker compares the complete canonical binding manifest against the Gateway policy before any Wasm start code. Socket path, expected Gateway UID and connection timeout remain worker-local. The Gateway additionally validates the peer's UID and exact live PID.

A launcher may prepare the worker process in a waiting state, derive its real PID, write owned Gateway configurations, start each Gateway, and then let that same process `exec` UWVM. Never substitute a guessed PID or authorize every process with the worker's UID. Keep socket parents private and permit the worker's network access only through authorized Gateway IPC. See [security.md](security.md) and [testing.md](testing.md).

## SROS2 enforcement

Production mode uses external, owned enclave credentials and signed DDS permissions. The Gateway requires `security-mode=enforce`, exact `keystore`, `enclave`, `domain-id`, and qualified `rmw-implementation`; it verifies required files and initializes an enforced context. Missing/invalid credentials never fall back to disabled security. The reusable acceptance runner creates disposable test credentials with official SROS2 tools, selects the exact Domain74/FastDDS settings during generation, tests signed bidirectional communications plus a native DDS-denied publisher, and destroys private keys after the suite. Those test certificates are never a runtime permission preset or production credential service.

Jazzy's [TimeSource implementation](https://github.com/ros2/rclcpp/blob/jazzy/rclcpp/src/rclcpp/time_source.cpp) subscribes to `/parameter_events` for the node's `use_sim_time` tracking, even when its parameter publisher/services are disabled. The signed test policy explicitly lists that framework subscription alongside application topics; Guest delivery still requires a distinct exact-node/name ParameterEvent capability. SROS2 also explicitly generates the RMW discovery-topic permission for the selected RMW. The Gateway disables the default type-description service using the official [read-only `start_type_description_service` parameter](https://github.com/ros2/rclcpp/blob/jazzy/rclcpp/src/rclcpp/node_interfaces/node_type_descriptions.cpp), as well as rosout and parameter service/event publication.
