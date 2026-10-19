# ROS2 incoming Action explicit QoS contract regressions

These new fixtures cover G04. They are **prepared, not compiled or executed**
while `ssh linux` is unavailable. They do not belong to the historical set of
18 native targets. Use new output directories and a new frozen source manifest
that includes the production fix and these tests; retain every prior failure.

The independent `action_matrix.json` was derived from the original G04 static
review. Its SHA is embedded in the file. `action_matrix.inc` contains the same
fixed expectations and is checked against the JSON by `generate.py`; neither
matrix is derived from the production helper's field array.

`policy.cc` tests all 40 fields with both enabled and disabled targets, both
directions of mismatch and equal mutations. The 35 individually legal fields
must conflict with errno 17; five `keep-all` histories must first return 95 when
enabled, while disabled unequal pairs still return 17. It also checks missing
fields on either side, all five `8`/`08` depth spellings, single grants, different
resources and independent targets. This source uses the existing fast_io Worker
headers and is compiled by the Worker unit driver as C++26.

`gateway_load.cc` runs the real trusted loader against private owned regular
files. It checks all 40 mismatches, exact diagnostics, positive matching
profiles, padded decimals and independent resources. Matcher-only fabricated
type/identity cases verify the exact pairing scope; they are explicitly not
valid Worker policies. This fixture does not create a ROS Context or IPC socket.
It proves the loader's rejection, not a dynamic startup test of `main.cc`.
Running generated bad configs against the fresh Gateway binary remains required
to prove its startup rejection before any native context/socket activity.

`native_ros2.cc` uses the real incoming Action adapter with an independent DDS
feedback subscription and native Action client/result future. It rejects 35
legal mutations across feedback/success/abort/canceled plus five `8`/`08`
mutations: 145 errno-13 responses. Every rejection preserves the actual goal's
flags, deadline, timer, queues, native handle, owner map, registry and cleanup
occupancy. It checks one live goal continues consuming the configured slot,
then exact `{0,1}` matching feedback/result and a replacement goal succeed.
Malformed wire, unknown/foreign/retired ownership and stored error priority
remain separate checks. The stored-110 check deliberately injects and restores
an internal fixture state; it is not actual deadline expiration evidence. This
fixture proves explicit contract rejection; it does not prove the RMW reports
or applies all 40 requested QoS fields.

## Frozen ARM/cgroup execution

All commands below run **inside the existing verified 64 GiB cgroup on linux**,
after SSH is restored. The parent owns a temporary exact ELF183/F binfmt lease;
the QoS driver validates it and never registers another entry. Use task-specific
variables for the newly frozen source, driver directory and unused output paths:

```sh
unshare -m /usr/bin/python3 -B "$QOS_DRIVERS/run_action_qos.py" \
  --mode prepare --source "$QOS_SOURCE" --rootfs "$QOS_JAZZY_ROOTFS" \
  --qemu "$QOS_QEMU" --binfmt-evidence "$QOS_LEASE_JSON" \
  --container-started-at "$QOS_CONTAINER_STARTED_AT" \
  --out "$QOS_PREPARE_OUT"

unshare -m /usr/bin/python3 -B "$QOS_DRIVERS/run_action_qos.py" \
  --mode execute --source "$QOS_SOURCE" --rootfs "$QOS_JAZZY_ROOTFS" \
  --qemu "$QOS_QEMU" --binfmt-evidence "$QOS_LEASE_JSON" \
  --container-started-at "$QOS_CONTAINER_STARTED_AT" \
  --prepared "$QOS_PREPARE_OUT/report.json" --domain 74 \
  --out "$QOS_EXECUTE_OUT"

/usr/bin/python3 -B "$QOS_DRIVERS/run_action_qos_policy.py" \
  --source "$QOS_SOURCE" --manifest "$QOS_SOURCE/../source-manifest.json" \
  --config "$QOS_WORKER_CROSS_CONFIG" --suite action-qos-policy \
  --out "$QOS_POLICY_OUT"
```

The Jazzy preparation compiles two new probes as C++17 with native GCC13, one
compiler at a time. It requires complete existing SDK contents including GCC,
CMake, `ament_cmake`, `rclcpp`, `rclcpp_action`, `action_tutorials_interfaces`,
Action messages, rcl/rmw/rosidl headers, typesupport and their linked libraries.
Actual GCC `-MD` files, project **and SDK** input hashes, compiler/frontend ELF
facts, `flags.make`, `link.txt`, `compile_commands.json` and generated CMake and
evidence barrier are recorded, with verbose CMake compiler/link recipes in the
build log. The compiler subtree has the same 2 GiB RSS guard and one build slot.
The full SDK byte/keyset/root-alias inventory is
checked before/after and again on reuse. Execute copies the qualified binaries
and verifies the exact source manifest plus every dependency and metadata SHA
before use. A SIGSTOP constructor
barrier captures actual ARM mapping, PID identity and inherited cgroup before
resuming each probe. The actual stopped ARM PID may be the launcher or one
strictly owned direct chroot child; PID/birth/PPid/ancestry, frozen device/inode
mapping, expected QEMU SHA and current live cgroup membership must all match.
No grandchildren or command-line-only matches pass. A pidfd pins the selected
stopped process for SIGCONT. The original host `/proc/exe` remains identified as QEMU.
The cgroup leaf device/inode, current membership and boot ID must remain identical
through the run; the parent's exact Docker `State.StartedAt` is also recorded.
The `UWVM_ROS_ARM_BINFMT_ENTRY` environment must match the provided parent lease.
SDK/source mounts are private, source and SDK views are read-only, and cleanup
and preservation are recorded. No ROS assertion or goal budget is relaxed.

The policy wrapper selects only the new test in the existing `run_unit.py` and
preserves its source manifest/MMD, cross SDK, compiler memory limit, stop barrier,
ARM mapping, cgroup and cleanup checks. Freeze the wrapper and all imported
driver helpers together and retain the outer invocation SHA in the parent
acceptance record. It requires the existing LLVM23 C++26 aarch64 cross config,
vendored fast_io headers and frozen cross SDK.

## CLI/config derivation

```sh
/usr/bin/python3 -B "$QOS_SOURCE/tests/ros_qos/generate.py" \
  --out "$QOS_CONFIG_OUT" --uid 0 --pid "$QOS_WORKER_PID" \
  --socket-dir "$QOS_PRIVATE_SOCKET_DIR" --domain 74
```

This produces 288 trusted configuration/Worker CLI cases in `manifest.json`,
including enabled/disabled and reversed pairs. It does not run a Worker,
Gateway or Guest. Generate actual trusted files on linux as the eventual
Gateway's effective UID. CLI options end at `--ros2-check-policy`, so a passing
CLI assertion establishes policy validation only. The five disabled history
mutations may pass the individually unsupported field phase and then conflict;
Gateway validation still rejects every `keep-all` field before pairing.

Future execution must preserve the raw case logs, actual product/native ELF and
live ARM evidence, source/compiler/SDK closures and prior failure reports. A
static fixture derivation report is not evidence that G04 is closed.
