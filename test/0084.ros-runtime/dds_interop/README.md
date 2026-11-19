# ROS 2 and native DDS round trip

## Bounded native image exchange

`run_image.py` compiles one rclcpp witness against the generated
`sensor_msgs/msg/Image` headers and type support of each installation. Select
Jazzy with Fast DDS and Humble with Cyclone DDS. The peers use private IPv4
loopback, unicast discovery and the dedicated image transport profiles.
The output records the compiler and binary hashes, loaded middleware and
type-support libraries, endpoint logs and individual round-trip measurements.

Each permitted run transfers 36 images of each size through the same
request/reply topics. Every endpoint checks the dimensions, row stride, RGB8
encoding, sequence, fresh trial identity and every pixel byte. The 921,600-byte
frame exceeds the largest UDP datagram. The 5,760-byte frame provides a smaller
comparison. The four permitted runs exercise both same-installation controls
and both orderings of the mixed installations.

The Best Effort writer versus Reliable reader control requires graph
visibility, zero request endpoint matches, an actual publication attempt
and a reliability incompatibility event. The wrong-stride control uses
matched endpoints and requires the echo application to observe and reject
the invalid frame. The requester receives no reply in either control.

The test transport profiles set a 32 KiB maximum message size, explicit
Cyclone fragment sizing and a paced Fast DDS writer. An initial run with the
ordinary loopback profiles exhausted the fixture's time bound during the
mixed large-frame exchange. The image profiles are scoped to this local test.

Run through the shared qualification launcher and lock from a fresh output
directory under the VM project's `.tmp`. Each prefix needs its own rclcpp,
generated Image package and middleware libraries.

```sh
"$GATE" /usr/bin/python3 -B test/0084.ros-runtime/dds_interop/run_image.py \
  --jazzy-prefix "$JAZZY_PREFIX" --jazzy-system "$JAZZY_SYSTEM_LIBS" \
  --humble-prefix "$HUMBLE_PREFIX" --humble-system "$HUMBLE_SYSTEM_LIBS" \
  --output "$NEW_OUTPUT"
```

Use `--sanitized` in a separate run for ASan/UBSan checks of the witness.
Use the optimized build without instrumentation for timing comparisons.
The latency includes publish, asynchronous network delivery, the peer's
content checks and reply, and the requester's final content check. The
summary excludes four warm-up observations per frame size.

Both builds passed all six cases in the x86-64 qualification cgroup.
The 921,600-byte observations from one optimized run were as follows.

| Requester → echo | Median round trip | 95th percentile |
| --- | ---: | ---: |
| Jazzy Fast DDS → Jazzy Fast DDS | 20.45 ms | 20.61 ms |
| Humble Cyclone DDS → Humble Cyclone DDS | 1.99 ms | 199.89 ms |
| Jazzy Fast DDS → Humble Cyclone DDS | 32.16 ms | 32.78 ms |
| Humble Cyclone DDS → Jazzy Fast DDS | 31.21 ms | 32.33 ms |

The mixed run with a Humble echo retained four rcutils error-state diagnostics
alongside the verified image responses. This evidence covers the exact image
exchange and local transport profiles. General ROS graph interoperability,
Wasm, DDS Security, endpoint permissions and physical-device latency have
separate qualification boundaries.

[Image definition](https://github.com/ros2/common_interfaces/blob/jazzy/sensor_msgs/msg/Image.msg)
and [Fast DDS pacing configuration](https://fast-dds.docs.eprosima.com/en/2.14.x/fastdds/use_cases/large_data/large_data.html).

## Native Int32 exchange

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

## Static TF with a late-joining rclpy reader

[简体中文](static-tf.zh.md)

`run_static_tf.py` builds a native rclcpp publisher against each selected ROS
installation. Each run creates a fresh two-edge camera frame tree on
`/tf_static`. The publisher writes its first sample before the Jazzy Python
process starts. The reader invokes the installed TF2 listener and verifies
both directions of the composed transform in its frame buffer at a nonzero
query time. It verifies each update's timestamp token, sequence, frame IDs,
finite translation and rotation before accepting it. See the
[Jazzy TF2 listener implementation](https://github.com/ros2/geometry2/blob/jazzy/tf2_ros_py/tf2_ros/transform_listener.py).

The Jazzy and Humble publishers each run against a Jazzy reader using the
listener's default transient-local QoS. The runner repeats both pairs with a
volatile reader and records whether the retained snapshot reaches it. Both
participants verify the durability reported by their RMW. The Humble and
Jazzy pair also runs with both ends volatile and must match without replaying
the earlier snapshot. Each live pair checks 24 fresh transforms. The final
control confirms that the publisher has exited before the listener starts.
That listener must observe neither a publisher nor a snapshot.

```sh
"$QUALIFICATION_GATE" /usr/bin/python3 -B test/0084.ros-runtime/dds_interop/run_static_tf.py \
  --jazzy-prefix "$JAZZY_PREFIX" --jazzy-system "$JAZZY_SYSTEM" \
  --humble-prefix "$HUMBLE_PREFIX" --humble-system "$HUMBLE_SYSTEM" \
  --python-home "$NOBLE_PYTHON_PREFIX" --python-site "$PYTHON_DEPENDENCIES" \
  --output "$NEW_DIR"
```

The Jazzy prefix must contain rclpy, geometry_msgs, tf2_msgs, tf2_py and
tf2_ros_py with their dependencies. The Python prefix must supply
`bin/python3.12`, its standard library and
`lib/x86_64-linux-gnu/libpython3.12.so.1.0` for the generated ROS extensions.
Supply extra Python dependency
directories with repeated `--python-site` options if needed. The Humble
prefix must supply the native generated tf2_msgs type support and its
dependencies. The reference run used Humble tf2_msgs 0.25.24 and Jazzy
tf2_ros_py 0.36.22. Use package sources from each installation's signed
archive.

The runner uses `-O3 -g` and writes the publisher binary identities, loaded
ROS libraries, Python TF2 modules, endpoint logs and monotonic observations
under a new project `.tmp` directory. `--sanitized` adds ASan and UBSan to the
compiled publisher witness while retaining the original RMW and Python
libraries. The late-join time runs from the start of listener construction to
the first validated composed transform. Each live time runs from the call
to publish until the Python listener has checked the sample and both frame
lookups, using the same Linux monotonic clock. Reported p50 and p95 omit four
warm-up samples. A completed receipt can contain a mismatch under
`documented_history_matches`. The field compares the observed number of
retained samples with the expectation for the pair in the
[Jazzy QoS guide](https://github.com/ros2/ros2_documentation/blob/jazzy/source/Concepts/Intermediate/About-Quality-of-Service-Settings.rst#qos-compatibilities).
These measurements cover plaintext loopback and the named native clients and
profiles. Wasm admission, target transforms on a physical robot, DDS Security
and real-time guarantees require separate qualification.

The optimized x86-64 run completed all six cases. Cyclone DDS 0.10.5
replayed the pre-join snapshot to the Jazzy Fast DDS 2.14.6 listener even
when its RMW graph reported volatile durability. The late reader validated
its original sequence before the fixture issued any live publish. The
Jazzy-to-Jazzy volatile-reader control and the Humble-to-Jazzy control with
both ends volatile received no historical samples. The mixed durability
pair differs from the Jazzy QoS guide. A site that requires fresh data must
verify freshness independently of its DDS durability configuration.

| Publisher → Jazzy TF2 listener | Retained | Live samples | Warmed p50 | Warmed p95 |
| --- | ---: | ---: | ---: | ---: |
| Jazzy transient → transient | 1 | 24 | 202.7 µs | 274.2 µs |
| Humble transient → transient | 1 | 24 | 166.4 µs | 204.8 µs |
| Jazzy transient → volatile | 0 | 24 | 196.7 µs | 287.6 µs |
| Humble transient → volatile | 1 | 24 | 148.3 µs | 197.3 µs |
| Humble volatile → volatile | 0 | 24 | 251.3 µs | 350.6 µs |
| Humble writer exited before join | 0 | 0 | — | — |

The first retained lookups took 106.8 ms in the same-installation transient
control, 8.35 ms in the mixed transient pair, and 8.76 ms in the mixed pair
with a volatile reader. The three active Humble publishers each logged two
rcutils error-state reports. The listener validated every expected update.

### Jazzy SROS2 Enforce

Run the same native TF witnesses against an extracted Jazzy x86-64 root
filesystem with rclcpp, rclpy, TF2, Fast DDS and the SROS2 CLI installed.

```sh
"$QUALIFICATION_GATE" /usr/bin/python3 -B test/0084.ros-runtime/dds_interop/run_static_tf_sros2.py \
  --rootfs "$JAZZY_ROOTFS" --output "$NEW_DIR"
```

The runner compiles the publisher with `-O3 -g` and creates three distinct
enclaves in the isolated namespace's temporary memory. It verifies the
generated signatures, domain admission, default-deny permissions and
encrypted topic metadata and data settings. The receipt identifies the
security directory actually selected by each participant. The permission
file also grants the exact `/parameter_events` endpoints required by the
native clients and `/tf` for the TF2 listeners.

The authorized Jazzy TF2 reader received the snapshot published before its
process started and validated 24 subsequent transforms. The other signed
identity successfully created an allowed `/tf` subscription. Fast DDS
denied its same-type `/tf_static` reader at creation and the participant
remained online with zero callbacks throughout authorized delivery. The
first retained lookup took 217.0 ms in the reference run. Its warmed
publish-to-validated-lookup p50 was 220.4 µs and p95 was 304.2 µs. The
output contains public artifact digests and process observations. The
keystore lives only in the temporary namespace. This test covers native
Jazzy Enforce on local loopback with the named topic grants. Packet-level
integrity verification, mixed releases and Wasm execution have separate
acceptance requirements.

Add `--wire` to the same command to trace completed UDP sends from the
publisher and receives by the authorized reader. The optional x86-64
fixture binds host strace and its unwind libraries read-only into the
namespace and records their digests. It uses the actual participant socket
inodes to identify each role's user unicast port. It matches complete RTPS
datagram digests across the traced send and receive calls and recognizes
DDS Security AES-GCM envelopes with the existing wire decoder. The traces
and credentials live in namespace memory. The output retains counts,
digests, participant ports and application observations. Timing summaries
come from the ordinary run with tracing disabled.

One traced run matched 58 distinct AES-256-GCM protected datagrams at the
authorized reader's UDP port. It found zero occurrences of either fixture
frame name in the observed datagrams. The TF2 listener validated one
retained transform batch and 24 live batches in that run. The wire count
can include middleware control traffic, with the TF2 lookups validating the
application data independently. The publisher sent five RTPS datagrams to
the denied participant's user port. The participant's `/tf_static` reader
creation was refused and its callback count remained zero.

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

For Dashing, Eloquent and Foxy, pass `--peer-cxx-standard 17 --server-domain-topics`.
These rclcpp versions read the domain from the process environment. Domain 61 holds
the client and ingress service. Domain 62 holds the server, egress client and both
transit topics. A real installation using this topology must authorize the two
additional topics on domain 62. Galactic accepts C++17 and the default three domains.
The runner reads the installed Cyclone version to select its loopback schema and
gives each process its own log directory. The [individual service
observations](service_observations.json) list nine successful relay pairings and
their missing-egress controls.

## Dust DDS

The isolated guest fixture compares the original Rust crate and Python wheel
with Jazzy Fast DDS. Its results distinguish participant discovery, endpoint
matching and a verified Int32 round trip. [Inputs, commands and observed outcomes](dust/README.md)
