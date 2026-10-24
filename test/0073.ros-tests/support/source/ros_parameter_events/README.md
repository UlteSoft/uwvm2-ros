The expanded ROS 2 ParameterEvent tests use tags 4–9 for String, ByteArray,
BoolArray, Int64Array, Float64Array and StringArray. Existing scalar tags and
the version-2 envelope remain unchanged.

`sdk_codec_test.cc` checks independent golden bytes, transactional failures and
allocation preflight without ROS. `native_ros2.cc` checks generated DDS events,
official Node parameter mutations, exact envelopes, ordered rejection barriers,
18 queue overflow contracts and actual reader GIDs. `witness_ros2.cc` supplies
the real DDS witness for each Wasm fixture. `events.py` defines 70 cases and
`generate_fixtures.py` writes their WAT, Wasm and profiles into a separate output
directory; it never rewrites the old ROS fixtures.

Run the Linux entry in the existing 64 GiB cgroup, with the existing Jazzy SDK
workspace and a fresh output path:

```sh
python3 test/0073.ros-tests/support/source/ros_parameter_events/run_native.py \
  --source "$FROZEN_SOURCE" \
  --rootfs-workspace "$SDK_WORKSPACE" \
  --out "$FRESH_OUTPUT"
```

This builds the Gateway with its C++17 profile and the two probes with C++20,
compiles all fixtures, then runs the native matrix. Add `--gateway "$GATEWAY"`
and `--uwvm "$DUAL_WORKER"` to snapshot the final integrated executables and
actually execute all 70 Wasm cases. `--llvm-libs` supplies the Worker library
path; `--short-bind` must name a fresh short host path. Domain 191 is reserved
for this fixture. The runner creates a private rootfs mount, binds its own
temporary directory and read-only source, sets `ROS_LOG_DIR=/tmp/ros-logs`,
records commands, executable SHA/PID/cgroup proofs and raw compiler dependencies,
then removes its mounts and processes. The SDK workspace remains the mount
source. When probe source differs from the production snapshot, pass the full
frozen revision with `--probe-source`; both source inventories are recorded.

Canonical scalar String permits NUL and canonical StringArray rejects it. These
are unit-tested wire contracts. The actual Jazzy `rmw_fastrtps_cpp`/FastCDR
transport has a separate boundary: generated ParameterEvent scalar String with
NUL raises `BadParamException`, while a StringArray element `a\0b` is transmitted
as `a`. Independent native publish/take diagnostics preserve those observations
and the original failures. The 70 DDS/Wasm cases therefore use exact normal UTF-8
String values and do not claim rejection of a sender's StringArray NUL after
the transport has already removed it.
