# Local parameter exception boundary

This focused Jazzy probe checks the official SDK exception classes and the
Gateway's authenticated IPC session after a local parameter cannot be removed.
It uses ROS domain 201 and an independent reliable `std_msgs/msg/String`
subscription as a real DDS witness.

- `use_sim_time` is a statically typed, non-read-only SDK default. The SDK throws
  `InvalidParameterTypeException`; the existing Gateway result is 95. A later
  read on the same client and the sibling subscription must still work.
- `start_type_description_service` is read-only. The SDK throws
  `ParameterImmutableException`; the Gateway must return 13 with an empty
  payload, retain the value, and keep the same client and sibling subscription
  usable.
- In each case, a dynamically declared control parameter must be removed
  normally and a later read must return 2. The sibling subscription must also
  close normally and its native reader count must become zero.

The old Gateway's read-only exception escaped to the session boundary: the
request and a later read both returned 107, and the native DDS reader disappeared.
The static parameter is an existing-behavior control, not a second defect.
The DDS witness publishes continuously while checking teardown; it does not
claim to test an idle wait-set.

Run on a Linux host with the existing Jazzy SDK rootfs and an effective 64 GiB
cgroup limit. Every output path must be new. The runner mounts a private clone,
binds its own temporary directory, keeps the source bind read-only, records raw
compiler dependencies and actual process identities, and removes its mounts.
It builds the native probe as C++20. A self-built Gateway uses its C++17 target;
the official ROS middleware is not sanitizer-instrumented by this runner.

```sh
python3 -B tests/ros_parameter_exception/run_native.py \
  --source /absolute/frozen/source \
  --rootfs-workspace /absolute/sdk/workspace \
  --out /absolute/new/attempt
```

To test an already built Gateway, add `--gateway /absolute/uwvm-ros2-gateway`.
Use `--probe-source /absolute/frozen/probe-source` only when the probe revision
is recorded separately from the production snapshot. The executable is copied
to the private temporary directory. A reused executable's build provenance
must be checked separately; the runner's raw dependencies cover only what it
actually compiles.

The first attempt's incorrect assumption that every removal refusal throws
`ParameterImmutableException` remains a fixture failure. The corrected before
attempt retains its actual read-only session-loss failure. Subsequent after
attempts use the same corrected fixture; reports and executed driver bytes
from earlier attempts must not be overwritten.
