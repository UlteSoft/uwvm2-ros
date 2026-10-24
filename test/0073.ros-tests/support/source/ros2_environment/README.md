# Explicit ROS2 startup environment

The Gateway selects its RMW, domain, security mode, keystore and enclave from
the validated trusted configuration. All native environment edits are checked:
an edit failure stops startup before a context, node, executor or IPC server is
created. The Gateway clears `ROS_SECURITY_ENCLAVE_OVERRIDE` before initialization.
Jazzy's [rcl security implementation](https://github.com/ros2/rcl/blob/jazzy/rcl/src/rcl/security.c)
selects a nonempty override before the enclave passed to the context. Clearing
it preserves the exact configured signed identity. This startup adjustment
adds no Guest import, capability, preopen or automatic permission.

The standalone C++17 unit uses 13 assertions on synthetic environment values,
idempotent override removal, explicit replacement and failure of invalid names.
It reads no inherited credential value and creates no ROS context. Its actual
compilation and execution are pending in the original Linux 64 GiB, zero-swap
cgroup. The AArch64 driver exposes a separate opt-in suite; the six original
units and 17 WASIp1 rows retain their existing membership.

```sh
PYTHONDONTWRITEBYTECODE=1 python3 -B test/0073.ros-tests/support/source/ros2_environment/source_models.py \
  --report /absolute/new-output/ros2-environment-source.json
```

The Python check examines source call ordering and models the official lookup
precedence with synthetic values. It does not execute the C++ helper, a native
environment edit, rcl, DDS, a Guest or QEMU. Actual Enforce acceptance must still
prove both signed endpoints, exact payloads, refusal outside signed permissions,
explicit internal Guest grants and the final current-source product bindings.
The earlier native-only Enforce result is preserved at its own source version;
it is not a Guest result or validation of this new helper.
