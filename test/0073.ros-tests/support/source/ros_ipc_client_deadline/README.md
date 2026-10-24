# IPC client entry deadlines

`unit.cc` uses real Linux Unix sockets and an authenticated client session. It
observes the queued caller's actual futex wait, and separately delays the exact
final request allocation. Expired calls must return110 without transmitting a
request. Unexpired calls must send the remaining budget, and a later request
must still use the same session. These are IPC tests, not ROS network evidence.

`run_unit.py` requires the existing Linux64GiB cgroup, a frozen source tree and a
fresh output directory. It records six C++17/20/26 × libc++/libstdc++ ASan/UBSan
profiles, raw MMD inputs, commands, logs and actual live ELF identity captured
before hashing the pinned FD. It also runs the existing request-view encoding
test to check exact wire bytes, skipped IDs and allocations. `--before` keeps
the four semantic counterexamples as failures; it never counts them as PASS.
The existing `test/0073.ros-tests/support/source/ros/run_unit.py --suite ipc` runs both the framing tests and
this regression. Its `--suite all` also includes the new test, so the existing
CI unit step selects it without a separate workflow. Hosted CI execution is a
separate claim.

```
python3 test/0073.ros-tests/support/source/ros_ipc_client_deadline/run_unit.py \
  --source /absolute/frozen/source --out /absolute/fresh/unit-output \
  --compiler /absolute/clang++ --llvm-libs /absolute/llvm/lib
```

`native_ros2.cc` and `run_native.py` exercise the same client through a freshly
compiled Gateway and official Jazzy Trigger services. A native callback gates
the first call while a second caller waits on the shared client. The expired
call must have zero native requests; a later call on that session must return
the exact canonical response. A longer budget must still execute normally.

```
python3 test/0073.ros-tests/support/source/ros_ipc_client_deadline/run_native.py \
  --workspace /absolute/fresh/native-output --baseline /absolute/frozen/contracts-integrated \
  --sdk /absolute/frozen/ros-sdk --inputs /absolute/current/source --phase after
```

The native runner needs root and the previously prepared Jazzy rootfs, including
real proc/sys/dev mounts. It uses a private mount tree and DDS domain196, keeps
all failed attempts, checks the original source/binaries/report unchanged, and
cleans its own processes and mounts. Neither runner changes the cgroup limit.
Native ROS middleware is not fully sanitizer-instrumented. Waiting for the
serialization mutex has no hard interruption guarantee. A request that has
started transport can already have an external effect before a timeout; only
an unwritten expired request is proved to have no peer operation.

`run_product.py` builds all four interpreter profiles from the frozen source and
runs the existing nine WASI/Memory64/tail-call/TrustedDL checks per profile. It
records12 MMD files and exact compiler inputs. This is a focused build/regression
qualification, separate from the previously delivered A15 network matrix.
