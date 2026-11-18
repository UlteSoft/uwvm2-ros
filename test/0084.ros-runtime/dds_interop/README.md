# ROS 2 and native DDS round trip

This fixture runs two processes in a fresh Linux network namespace containing only loopback. A native ROS node publishes a fresh positive integer on `/wros_dds_request`. The independent peer reads it and publishes its successor on `/wros_dds_reply`. Both directions use reliable DDS topics. The client rejects unexpected values and the test requires both processes to exit successfully.

The client uses the selected installation's `rclcpp` and checks the RMW implementation identifier. With only `--cyclone-prefix`, the peer uses Cyclone's C API and compiles the ROS DDS type with that installation's `idlc`. With the two optional peer arguments, the second process compiles against its own installation's `rclcpp` and checks for `rmw_cyclonedds_cpp`. Each process receives its own library and resource search paths. Native code runs entirely in the test processes.

On x86-64 Linux, select absolute paths to extracted native SDKs, their system libraries and a fresh output directory. Run the test in the configured 64 GiB test cgroup.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py /usr/bin/taskset -c 4-7 \
  /usr/bin/python3 -B test/0084.ros-runtime/dds_interop/run.py \
  --ros-prefix "$JAZZY_ROOT/opt/ros/jazzy" \
  --ros-system-libs "$JAZZY_ROOT/system-libs" \
  --cyclone-prefix "$HUMBLE_ROOT/opt/ros/humble" \
  --peer-ros-prefix "$HUMBLE_ROOT/opt/ros/humble" \
  --peer-system-libs "$JAMMY_LIBS" \
  --output "$PWD/.tmp/jazzy-humble-dds"
```

The receipt records the compilers' arguments, selected libraries, type definition, executables, challenge, expected response, network boundary and process exits. Its scope is unencrypted native UDPv4 and `std_msgs/msg/Int32`. Secure DDS, services, actions, discovery of unrelated hosts, Wasm execution, source migration and live actuator control each have separate acceptance criteria.
