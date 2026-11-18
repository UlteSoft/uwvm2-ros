# Micro XRCE-DDS with native ROS 2

The original eProsima XRCE Client connects to a separate upstream XRCE Agent
over loopback UDPv4. The Agent creates one DDS participant, two Int32 topics,
a reliable writer and a reliable reader. A distinct Jazzy rclcpp node running
Fast DDS reads the random input and sends back its successor. Before starting
the Agent, the fixture checks that the client refuses to create a session.

Use the source revisions and archive digests in [upstream.json](upstream.json).
Build Micro CDR, the XRCE Client and the Agent in that order. Set the respective
UCDR, UCLIENT and UAGENT superbuild switches to OFF. Build the Agent with
UAGENT_FAST_PROFILE enabled, the Jazzy Fast DDS and Fast CDR packages, and
their TinyXML2 and foonathan-memory development dependencies. Select the UDP
client transport. This test uses RelWithDebInfo with C and C++ flags
`-O3 -g`. Keep all compilers and native processes in the qualification cgroup.

First run the [Jazzy native Int32 fixture](../dds_interop/README.md). Its
completed receipt identifies the rclcpp executable, library roots and their
hashes. To reuse that executable as the independent echo node, run

```sh
python3 tools/ros_scene/in_existing_cgroup.py taskset -c 4-7 \
  /usr/bin/python3 -B test/0084.ros-runtime/micro_xrce_dds/run.py \
  --xrce-prefix "$XRCE_PREFIX" --microcdr-prefix "$MICROCDR_PREFIX" \
  --native-wire "$NATIVE_WIRE/results.json" --output "$PWD/.tmp/xrce-int32"
```

The receipt includes the identities of the static client libraries and the
vendor libraries mapped in the running Agent and rclcpp processes. This case
covers the named XRCE version, UDPv4 and two reliable topics. Serial and CAN
transports, firmware deadlines, XRCE services, authentication and vendor-specific
device operations require their own endpoint tests. Deployment must explicitly
approve a plaintext legacy network. All participants in this transport fixture are native processes.
