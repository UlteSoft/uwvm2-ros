# Current ARM custom-topic Wasm loopbacks

`run_custom_guest.py` prepares and executes two bounded raw-import WebAssembly
fixtures through the current dual Worker, its authenticated Gateway IPC path and
the actual typed ROS1/ROS2 custom topic adapters. It does not build native
products. It translates only `test/0073.ros-tests/support/source/ros_custom_types/loopback_ros1.wat` and
`loopback_ros2.wat`, using the explicitly supplied current `wat2wasm` executable
inside the verified Linux cgroup. The existing host WABT tool may be ELF62;
the report records its actual executable architecture and does not call it an
ARM compiler.

Each fixture has one Guest holding separately granted `pub` and `sub` aliases on
one absolute custom topic. The data passes through real Gateway IPC and the
native topic implementation before returning to the Guest. This is a native
loopback in one Gateway, not an independent native publisher/subscriber peer.
The separate custom native fixtures exercise independent native ROS peers.
Neither witness alone completes the original G01 custom interoperability task.
This controller does not qualify generic custom Service/Action/dynamic types,
SROS2 enforcement, revocation/reconnect, performance or the full regression
matrix.

## Required immutable inputs

Run the controller from the exact complete frozen Source tree after building:

1. The current dual ARM Worker with the reviewed custom manifest explicitly
   selected through `UWVM_ROS_CUSTOM_TOPIC_MANIFEST` in all three compile commands.
2. Both current custom-enabled C++17 ARM Gateways. Their fresh targeted reports
   must pass the shared system-inclusive compiler dependency, actual recipe,
   source/SDK preservation, product identity, architecture and cleanup qualifier.
3. Official native custom packages, installed in the explicitly recorded fresh
   `/opt/uwvm-custom-types` SDK prefix. Old SDK input inventories cannot be reused
   after this addition.

The generated deployment is
`test/0073.ros-tests/support/source/ros_custom_types/deployment-r1/generated` beneath that frozen Source. Its
`generation.json` must describe the exact 23-byte ROS1
`uwvm_custom_ros1_fixture/Packet` and 45-byte ROS2
`uwvm_ros_custom_test_msgs/msg/FixedSample`, with official native identities and
the separately generated `adapter_codec_identity`. The controller derives these
values from the pinned generation report; it does not invent native hashes.
Both the native headers and all Worker descriptor dependencies belong to the
same reviewed frozen source snapshot.

Worker qualification uses the existing fresh compilation validator. It requires
the current authority/CLI source dependency closure, exact original ARM object
identities, the three successful compiles/link/ARM version command and a matching
configuration/report binding. One dual profile is sufficient for this scoped
witness; it does not replace the original four-profile matrix.

## Explicit fixture rights and process binding

The Worker receives only the selected backend's single target named `main`.
The controller records its complete raw argv, exact grants and mirrored Gateway
configuration. Both topic aliases specify resource/type/native identity/codec
identity and every required quota, transport/QoS and audit field. There is no
global ROS enable/grant or implicit rights for the other backend.

The fixture explicitly creates/enables `main`'s WASI target, disables host
environment inheritance and grants only `monotonic-clock`. Its one-byte stdin
gate uses the existing baseline fd0 read right. Neither the gate nor native
registry selection enlarges filesystem, socket, randomness, other clock,
signal or scheduling rights. `proc_exit` is a completion mechanism; the WAT
executes `proc_exit(0)` only after all its checked assertions and both closes.
Production default-deny policy is unchanged.

The Worker launch shell is paused to reserve its exact future PID. The Gateway
configuration binds to that owned PID and UID. A ready native Gateway socket
must exist before the shell is resumed. The Guest then blocks on ordinary stdin
before its tested ROS body. The controller discovers the actual mapped Worker
image, stops it through its birth-pinned pidfd, checks the current ARM file map,
host QEMU executable, live cgroup membership, PID ancestry and incarnation, then
resumes it and releases precisely byte `G`. This is a gate before the tested
Guest ROS calls; it is not Worker pre-main instrumentation. Native Gateway and
Master mappings are stopped and verified after their images are observed;
their evidence likewise makes no pre-main claim.

The ROS1 fixture uses a task-owned isolated Master with XMLRPC URI/PID readiness,
actual owned ARM Python mapping and a port lease. ROS2 explicitly selects a
leased test DDS domain, Fast DDS and disabled-test security. These environment
settings belong to this scoped native test and create no Guest authority.

## Example invocation

Launch only after the root task assigns the compiler/execution lane. All actual
WAT translation, native processes and Guest execution must remain in the existing
verified Linux 64 GiB cgroup with swap zero. Use the currently observed container
StartedAt and a fresh temporary fixed-QEMU binfmt lease, wrapping the controller
with `unshare -m`. Substitute only current qualified absolute input paths:

```text
python3 SOURCE/test/0073.ros-tests/support/source/ros_aarch64/binfmt_lease.py \
  --qemu PINNED_QEMU --name uwvm-ros-arm64-custom-guest-r1 \
  --report FRESH_LEASE_JSON --timeout 600 -- \
  unshare -m python3 SOURCE/test/0073.ros-tests/support/source/ros_aarch64/run_custom_guest.py \
  --source SOURCE --manifest SOURCE_MANIFEST \
  --worker CURRENT_DUAL_WORKER --worker-report FRESH_WORKER_REPORT \
  --worker-config FROZEN_WORKER_CONFIG \
  --ros1-gateway CURRENT_ROS1_GATEWAY --ros1-gateway-report STRICT_ROS1_REPORT \
  --ros2-gateway CURRENT_ROS2_GATEWAY --ros2-gateway-report STRICT_ROS2_REPORT \
  --sdk-workspace CURRENT_NATIVE_SDK_WORKSPACE --wat2wasm /usr/bin/wat2wasm \
  --qemu PINNED_QEMU --binfmt-evidence FRESH_LEASE_JSON \
  --container-started-at CURRENT_OBSERVED_STARTED_AT \
  --master-port 11473 --domain 213 --out FRESH_OUTPUT
```

Output must be a fresh directory. Native images are copied into owned immutable
snapshots, preserving their compiled origin facts and exact content. The SDK
static `usr`, `opt`, `etc`, `var/lib/dpkg` trees are mounted read-only, with owned
tmp/home/source/output views in the private mount namespace. The existing
trusted source-binding helper may create otherwise untracked mountpoint
directories outside those static input scopes. The controller records the
binding and does not describe that narrow inventory as the whole rootfs.

Each WAT translation is limited to 30 seconds. The actual Worker gets a total
60-second launch/body budget, including Gateway startup and stdin gate admission.
Native startup/readiness loops have their own bounded deadlines. Subprocess
logs and observed RSS are bounded, the cgroup retains a 4 GiB admission reserve,
and original source, metadata pins, snapshots and SDK static inputs/aliases are
rechecked. Cleanup uses observed process births and pidfds, not unowned process
groups. It removes owned mounts and the temporary short Unix socket directory.
Gateway configuration text and copies remain in the output report for review.

## What a successful result proves

Both cases must have actual Worker exit zero from the exact compiled frozen WAT
whose success path ends in `proc_exit(0)`. Inside that Guest, noncanonical Bool2
must return errno22, every returned record byte and its exact 23/45-byte length
must match, both aliases must close successfully, and using the stale publication
handle must return errno9. The controller independently requires actual current
ARM Worker/Gateway maps, active native readiness, genuine source/product
bindings, the same cgroup incarnation, zero OOM deltas and complete owned
process/mount cleanup. Any missing fact leaves the report failed.

The loopback does not independently count absence of native side effects for the
denied Bool, nor prove cross-instance handles or generalized attack coverage.
Independent native fixtures and broader Guest/native refusal witnesses remain
required. A source/AST/model check, a prepared recipe or a compiled Wasm artifact
must never be reported as this actual integration result.
