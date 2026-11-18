# Noetic and Humble native String bridges

The qualification compiles the unmodified upstream String bridge examples from
[ros2/ros1_bridge at the pinned revision](https://github.com/ros2/ros1_bridge/tree/611755fd917285316051cbea80507e8b2f6b7ec1/src).
Each binary forwards one direction on `/chatter`. The runner checks both source
digests before compilation using Clang with `-O3 -g`. Noetic supplies roscpp,
rospy and rosmaster. Humble supplies rclcpp, the Cyclone RMW and the DDS runtime.

Each direction gets two independent loopback-only networks. In each one the
runner starts a real ROS 1 master, a rospy endpoint and an rclcpp endpoint.
A witness sends a fresh 32-character challenge. With the bridge disabled the
receiver must time out after six seconds. With the bridge present it must
receive that exact string and the bridge must log its conversion. The ROS 2
endpoint verifies its middleware identifier at startup. Each publisher and
subscriber exits at its deadline. The runner terminates its private master and
bridge after recording the endpoints' results.

Supply the Noetic filesystem image and the extracted Humble prefix and its
matching Jammy shared-library directory. The Noetic Python processes use the image's ELF loader, Python interpreter,
standard library and ROS packages. The shared loopback-only namespace exposes
read-only host files and a writable output directory.
An example invocation from the VM checkout is:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/taskset -c 4-7 /usr/bin/flock "$SDK_ROOT/.tmp/sdk-qualification.lock" \
  /usr/bin/python3 -B test/0084.ros-runtime/ros1_native_bridge/run.py \
  --noetic-root "$NOETIC_ROOT" --humble-prefix "$HUMBLE_PREFIX" \
  --humble-system-libs "$JAMMY_RUNTIME_LIBS" --bridge-source "$ROS1_BRIDGE" \
  --output "$PWD/.tmp/noetic-humble-string"
```

The receipt identifies the ROS package versions, both bridge binaries, the
witness code, selected ROS libraries, payloads and case logs. This native
fixture covers the two static String examples with plaintext TCPROS and DDS
on one host. A ROSv2 Wasm caller, per-capability admission, a dynamic bridge,
other message types, DDS Security and physical deployment require separate
qualification.

## Wasm application using a registered bridge adapter

After running the native qualification, supply its output directory to the VM
test. This fixture uses the VM's existing `uwvm_ros_v2.driver.connect`,
`driver.submit`, `driver.result` and `driver.disconnect` imports. The grant
binds one fixed resource, direction and operation schema. VM and adapter
authenticate over a TLS 1.3 channel with fresh sealed identity descriptors.
Only the external ROS client participates in DDS discovery.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/taskset -c 4-7 /usr/bin/flock "$SDK_ROOT/.tmp/sdk-qualification.lock" \
  /usr/bin/python3 -B test/0084.ros-runtime/ros1_native_bridge/wasm_bridge.py \
  --vm-source "$VM_SOURCE" --runtime "$VM_BINARY" --runtime-receipt "$VM_BUILD_JSON" \
  --native-dir "$PWD/.tmp/noetic-humble-string" --noetic-root "$NOETIC_ROOT" \
  --humble-prefix "$HUMBLE_PREFIX" --humble-system-libs "$JAMMY_RUNTIME_LIBS" \
  --openssl-root "$BUILT_OPENSSL_PREFIX" --allow-plaintext-loopback \
  --output "$PWD/.tmp/noetic-humble-wasm"
```

The interpreter and AOT cases carry a fresh 32-character String between a Wasm
module and a Noetic node in each direction. For publication the deployment
waits for a real Noetic TCPROS connection, and the Wasm application submits a
single String to the adapter. The Noetic subscriber checks the exact value.
For subscription the application checks every byte returned by
`driver.result`. Each denial case omits the submission grant while leaving
connect and disconnect available. The application must receive access denied,
the adapter must receive zero requests, and its dormant native client must exit
without initializing ROS. The explicit CLI acknowledgement allows plaintext
for that test's private loopback-only ROS network. The caller has no WASI
imports.

The report binds the VM digest, native bridge receipt, two adapter binaries,
compile arguments, guest modules and logs. This exercises an external test
adapter. Direct `bridge.*` attachment to a native ROS 1 endpoint and dynamic
message conversion remain separate qualification targets.

Supply `--sdk /absolute/path/to/site-configured-c-development` to
`wasm_bridge.py` to compile the application from `driver_guest.c`. The
harness compiles each challenge at `-O3 -g` using the installed SDK before
starting the ROS participants. It checks that the strict build receipt binds
the selected VM and retains MTE. The resulting core Wasm imports just the
capability and driver operations exercised by that case. Both peers compare
the payload to the same independently generated token. The provisioned
legacy link still requires its explicit plaintext-loopback option.
