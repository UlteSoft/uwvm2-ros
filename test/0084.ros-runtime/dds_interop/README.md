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

Some installations put Cyclone's public headers under `include/CycloneDDS`.
The fixture locates the installed header layout and reads the vendor version
header to select the IDL compiler's output option and the loopback-interface
XML syntax. For a pre-C++20 rclcpp installation, pass
`--peer-cxx-standard 17` explicitly. Foxy omits the idlc program in its binary
package. For an rclcpp exchange, `--cyclone-prefix` may point at a separate
installation for the additional C compile. The echo process resolves its DDS
library from `--peer-ros-prefix`. A direct Cyclone C run may use
`--cyclone-system-libs` for its system runtime libraries.

Dashing and Eloquent link against their installed rclcpp dependencies. The
fixture includes statistics and tracing libraries when the release supplies
them. Endpoint home and ROS log paths live under the run's output directory.
This also contains the default logfile created by Eloquent's spdlog backend.

## Native GetParameters service

Supply both native rclcpp installations and add `--service` to the command
above. The first process creates a Jazzy Fast DDS service client. The second
creates an independently compiled service server. Select the second middleware
with `--service-peer-rmw rmw_cyclonedds_cpp` or
`--service-peer-rmw rmw_fastrtps_cpp`. The fixture issues two successive
requests containing independently identifiable names. Each request must return
one string parameter with the requested name in its value. Both processes must
finish before their deadlines.

Use `--service-control request` to run both roles using the first installation,
or `--service-control reply` to run both using the second. Compare those controls
with the mixed installation before attributing a failure to package installation,
service discovery or delivery. Inspect `ros.log`, `cyclone.log` and
`exchange.json` for the actual roles and results. Pass the resulting
`results.json` to `tools/ros_scene/rmw_inventory.py --native-service` alongside
the release catalog and package index. The verifier checks compiler inputs and
flags, executable and library digests, process exits, both endpoint logs and
the order and contents of both replies.

### Crossing unlike service mappings

Run `parameter_relay.py` with `--fast-prefix`, `--fast-libs`,
`--cyclone-prefix`, `--cyclone-libs` and a new `--output` directory under
`.tmp`. Run under the same cgroup as the direct test. The Jazzy client uses
domain 61, the Humble server domain 62, and the two native relay processes use
two reliable String topics on domain 63. Each end uses its local RMW for the
original GetParameters service. The test forwards exactly two named reads and
checks the server's typed values at the original client. With the egress
process absent, the client must fail discovery and the ingress must time out.
All four native processes are outside the VM. This fixture exercises the
read-only operation on private plaintext loopback. Production use needs
separately approved endpoints and a qualified security profile on every leg.
