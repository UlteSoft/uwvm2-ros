# Host interface policy regression

Run all commands inside the admitted Linux cgroup (memory.max = 68719476736,
memory.swap.max = 0). Keep binaries, generated WAT/Wasm and results under
uwvm2-ros/.tmp. Reuse the installed compiler, wat2wasm, SDK and cross launchers.

`run_host_interfaces.py --runtime PATH --sdk SDK_ROOT --output TMP_DIRECTORY`
runs the public CLI against real assembled Wasm. It uses the SDK's
`example/language/wat/library_kinds/static_math.wat` arithmetic export to check
that denied calls return to the guest without changing memory or corrupting
execution. It checks every registered host signature, disabled namespaces,
hidden imports, exposed denied calls, operation denial, quota rejection/wait,
warning selectors, error promotion, immediate module identity, Group policies,
proc_exit and closed/full output pipes.

The normal engines default to interpreter and AOT. An architecture launcher can
execute the cross-built product under QEMU for correctness. Use
`--threaded-engines interpreter` when the separately implemented WASI Threads
LLVM backend is unavailable on that architecture. The result records both engine
sets; omitting that threaded backend is not evidence that it passed.

Compile the source-level tests with an installed C++26 compiler and `-Isrc`:

- `host_diagnostics.cc`: selector precedence, default severities, nested calls,
  deferred quota reports, allocation failure, control-character escaping and
  long names. No diagnostic input-length cap is imposed.
- `policy_encoding.cc`: current wire roundtrip, rejected obsolete wire version,
  invalid states, truncated input and cross-family rejection. Also include
  `third-parties/fast_io/include`, `third-parties/bizwen/include` and
  `third-parties/boost_unordered/include`; define
  `UWVM_ROS_ZERO_HOST_DEFAULTS=1`.
- `ros_quota_diagnostics.cc`: the real ROS capability manager with unlimited,
  rejected and synchronously waited operations for both ROS families, plus
  default-silent and selected quota diagnostics. Include fast_io and use
  `-pthread`. This unit does not claim native ROS transport coverage.

`run_ros_quota.py` additionally exercises both interpreters and LLVM AOT with
real installed ROS1/ROS2 gateways. It requires an isolated loopback network and
explicit pinned local deployment records under `--config-root`:
`required-native/vendors-ros1/deployment.json` and
`native-ros/deployment.json`. Supply `--runtime` and a fresh `--root` directory.
The records select existing binaries, libraries, ROS1 master and native inputs;
the test installs nothing. It checks unlimited calls, rejection, warning family
and API selectors, promotion to error and monotonic synchronous waits. Its
clock-capability probes qualify host admission and quota handling, not topic,
service or action transport.

The companion SDK tests are
`tools/shared/python/tests/test_host_interfaces.py`. They exercise the actual
Xmake Lua and Python serializers, exact imports, explicit hidden/deny overrides,
module/Group/Global binding, guest argument separation and rejection of old
policy contracts.

Use `--thread-start-repetitions 100` to stress real worker startup while the
management thread polls completion. Every start must reach the denied guest
import and preserve its ABI result; transient internal mutex contention must
not become an EINVAL startup failure. This is a correctness test under QEMU.
