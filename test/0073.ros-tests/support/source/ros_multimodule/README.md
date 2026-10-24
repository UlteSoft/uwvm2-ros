# Required five-module functional scenario

This test runs five actual WebAssembly instances in one UWVM process against
native ROS1 and ROS2 nodes. Four preload modules perform their own ROS operations
from their start sections; `hybrid` runs as the main module. Native observers
check the exact bytes of two messages on each required resource.

| Module | Explicit ROS1 policy | Explicit ROS2 policy | Required native message |
|---|---|---|---|
| `camera_a` | Group `legacy_sensors` | Unconfigured, denied | `/five/camera_a`: `camera-a` |
| `camera_b` | Group `legacy_sensors` | Unconfigured, denied | `/five/camera_b`: `camera-b` |
| `controller` | Single `controller` | Unconfigured, denied | `/five/controller`: `controller` |
| `planner` | Unconfigured, denied | Group `modern_planners` | `/five/planner`: `planner` |
| `hybrid` | Single `hybrid` | Single `hybrid` | `/five/hybrid1`: `hybrid-ros1`; `/five/hybrid2`: `hybrid-ros2` |

The two Group camera instances share one trusted policy and one Gateway process.
They receive separate authenticated sessions and separate runtime capability
tables. Both cameras also open the same `quota` alias for
`/five/shared_cameras`. Its explicit rate is `0.001 Hz`, burst is `2`, and each
camera must independently publish twice before its third attempt returns `11`
and preserves the memory sentinel. The native observer must receive exactly
two `camera-a` and two `camera-b` messages on that shared resource. The Gateway
audit must record exactly two accepted quota requests in each camera session.

`camera_b` reads `camera_a`'s still-live exported handle through an imported
WebAssembly global and attempts to publish with it. The expected error is `9`.
Each instance also checks an unauthorized alias in the other backend, and
`camera_a` checks the controller's Single alias. `hybrid` checks that the camera
and planner Group aliases are unavailable to its Single policies. All denied
opens must return `13` and preserve the output sentinel. A failing assertion
terminates the actual Guest with a nonzero code.

The ROS policies use explicit Group and Single settings; no ROS Global policy
is created. The command uses
`--wasip1-global-noinherit-system-environment` to isolate WASI for every module.
ROS Group names do not bind or grant WASI permissions.

Generate the fixtures locally, then compile them using the existing Linux WABT
toolchain:

```sh
python3 test/0073.ros-tests/support/source/ros_multimodule/generate.py
for module in camera_a camera_b controller planner hybrid; do
    wat2wasm --enable-all "examples/ros_multimodule/$module.wat" \
        -o "examples/ros_multimodule/$module.wasm"
done
```

Build `uwvm-ros1-five-module-probe` and `uwvm-ros2-five-module-probe` using their
respective existing Noetic and Jazzy CMake directories. Run the driver inside
the existing Linux worker with its effective 64 GiB cgroup limit:

```sh
python3 test/0073.ros-tests/support/source/ros_multimodule/run.py \
    --uwvm /absolute/path/to/final/dual/uwvm2-ros \
    --expected-product-sha256 FINAL_PRODUCT_SHA256
```

The driver owns ROS1 Master port `11379`, DDS domain `79`, and
`/uwvm-five-test` bind mounts in the two existing ROS rootfs. It snapshots the
product, both Gateways, native observers, Wasm modules, WAT sources, and test
source files before starting. Every Gateway binds the same actual worker UID
and PID. `report.json` retains the full policy, exact command, binary hashes,
five host-observed instance IDs, independent camera sessions, native message
counts, cgroup measurements, and owned-process cleanup result. Product SHA
mismatch, missing native observations, unexpected content, extra quota
messages, or increased OOM-kill count fails the suite.
