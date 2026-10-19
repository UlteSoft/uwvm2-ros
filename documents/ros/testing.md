# Reproduce real acceptance

All network tests require actual Noetic/Jazzy executables and native ROS witnesses. Unit tests validate policy, handles, memory layouts and framing; they do not establish ROS network success. Missing prerequisites yield BLOCKED. A failed assertion or deadline yields FAIL. No SKIP/BLOCKED contributes to PASS. Aggregate success also requires a nonempty evidence list with explicit PASS in every referenced report; exit zero alone is insufficient. `--llvm-libs` is applied to every child environment, including CLI and regression subprocesses. The native revoke bridge honors `CXX` (or explicit `--cxx`), including compiler wrappers.

## Current complete-source acceptance

The current user override is exactly48GiB (`51539607552` bytes), zero swap,
and serial build/test execution. Local checks must write only beneath `/tmp`
and use one compiler job with the selected512MiB process-group RSS bound.
Read [the complete acceptance workflow](../../tests/ros_acceptance/SELECTION.md)
for immutable SourceA, official custom package/identity generation, the complete
fixed-v1+bounded-v2 SourceB, all four fresh interpreter products, both C++17
Gateways, actual six-language clients/servers and the original requirement
mapping. Current receipt paths, boot, cgroup device/inode and container StartedAt
are explicit inputs. Signed and reconnect controllers also require a separately
opened `--expected-cgroup` JSON with exactly `path`, `device`, `inode`, `boot_id`.
Historical defaults are never used by their live preparation/execution.

The dated commands and test observations below describe retained earlier
workflows. Their previous source/budget receipts cannot satisfy current SourceB
acceptance. A new compilation or preparation remains distinct from actual
native ROS and real Wasm delivery; no successful status is inferred here.

## Independent build and unit entrypoints

```sh
CXX=/absolute/clang++ python3 tests/ros/build_product.py --profile all --out /absolute/fresh/products
CXX=/absolute/clang++ ROS_TEST_CXXFLAGS='-std=c++26 -stdlib=libc++ -O1 -g0' \
  tests/test_ros_security.sh
tests/test_ros_ipc.sh
python3 tests/ros_cli/run.py --uwvm /absolute/uwvm --output /absolute/cli.json
python3 tests/ros/regression.py --uwvm /absolute/uwvm --out /absolute/regressions
```

The regression runner executes normal WASI Preview 1 calls/output, memory64 WASI, an interpreter tail-call fixture and the repository's existing Trusted DL plugin source. Disabled-feature rejection is retained; the fixtures explicitly enable their required Wasm features.

## Provided SSH Linux and existing cgroup

The test host is `ssh linux`. The task reuses container `4dd58ad4560f` (`uwvm-debug-core3-v48-20261011`). Its current selected leaf must have `memory.max=51539607552`; earlier receipts recorded the original64GiB budget. Every restart requires new boot/StartedAt/device/inode qualification. The privileged container's existing `/host` mount supplies the remote host userspace/compiler while the tested process remains in that container's cgroup:

```sh
ssh linux 'docker exec 4dd58ad4560f chroot /host python3 /tmp/uwvm-ros-extension-20261012/source/tests/ros_cgroup/probe.py --output /tmp/uwvm-ros-extension-20261012/build/environment.json'
```

The probe verifies its own live PID appears in the resolved `cgroup.procs`, then walks visible ancestor limits. Host physical RAM alone never establishes cgroup membership. It records `memory.max/current/peak/events`, CPU quota, cpuset, PID limit, kernel and userspace. The existing cgroup is shared with earlier work; report cumulative peak separately from this task's bounded workloads, and compare OOM counters before/after rather than attributing the shared peak to this extension.

## Gateway userspaces and real network runner

Acceptance used official `ros:noetic-ros-base-focal` and `ros:jazzy-ros-base-noble` images exported into task-owned rootfs directories under `/tmp/uwvm-ros-extension-20261012/gateway/`. Necessary compiler/CMake/ROS interface development packages were installed inside those directories. Both executables were compiled independently using the actual Noetic GCC9 and Jazzy GCC13 environments. The rootfs `/work` bind exposes the task source. `/proc`, `/dev` and temporary test socket/log directories are mounted only inside the existing container mount namespace. Test-created mounts and processes are cleaned after their suites.

The runner expects `gateway/noetic-rootfs/tmp/ros1-build/uwvm-ros1-gateway` and the corresponding Jazzy binary/native witness beneath the selected workspace. It starts a real ROS1 Master on the test port, distinct ROS1 and ROS2 Gateways, and a worker process with the exact PID recorded in each Gateway configuration. That waiting process executes the actual UWVM product after its CLI is fully prepared. Native nodes witness received values and denied network side effects.

```sh
ssh linux 'docker exec 4dd58ad4560f chroot /host python3 /tmp/uwvm-ros-extension-20261012/source/tests/ros/run_network.py --mode all --workspace /tmp/uwvm-ros-extension-20261012 --source /tmp/uwvm-ros-extension-20261012/source --uwvm /absolute/tested/uwvm'
```

Select `--mode ros1`, `ros2`, `dual`, or `multimodule` for the named entrypoints in `tests/test_ros*.sh`. Every case records the exact worker CLI, Gateway configuration, native witness logs, return codes and a machine-readable report. The fixed test values are explicit fixture data, never a runtime preset or permission level. The ordinary matrix uses explicitly marked `disabled-test`. A separate `run_native.py --suite secure` test verifies actual FastDDS/SROS2 Enforce and signed resource denial; those statuses remain separate.

## Aggregate entrypoint

```sh
tests/test_ros_all.sh --uwvm /absolute/uwvm --out /absolute/new/acceptance \
  --workspace /tmp/uwvm-ros-extension-20261012 \
  --llvm-libs /absolute/toolchain/lib:/absolute/toolchain/target-lib
```

Run it through the same `docker exec … chroot /host` path. Current guards require actual48GiB membership first, then run units, CLI, regressions, native Topic/Resource/TF/Lifecycle/Action/SROS2 witnesses, resource Wasm, real network/multimodule, operation/startup/Group denials, WASI/Plugin ingress, actual Trap cleanup, authorization/revoke concurrency, Service/Graph/Log quotas, malformed payloads and bounded performance suites with explicit time bounds. A final memory-events comparison rejects increased OOM/oom_kill counts. `report.json` preserves each suite status. CI's independent profile products and policy tests are in `.github/workflows/ros-extension.yml`; real ROS acceptance is separately evidenced because that job does not contain the provided SSH cgroup and prepared ROS userspaces.

The aggregate forwards `--wat2wasm` to CLI fixture generation as well as the other Wasm suites. Standalone network and five-module runners use the current `LD_LIBRARY_PATH` unless `--llvm-libs` overrides it. Argument combinations that select no applicable test are rejected before native setup: Lifecycle requires ROS2, and the dual revoke mode requires both backends. Resource reports bind Guest metadata to the immutable file actually executed, including its snapshot path and SHA, even if the original fixture later changes.

## Resource and secure native entrypoints

```sh
python3 tests/ros/run_native.py --workspace /tmp/uwvm-ros-extension-20261012 --suite all --out /absolute/new/native
python3 tests/ros/run_rpc.py --workspace /tmp/uwvm-ros-extension-20261012 --source /absolute/source --uwvm /absolute/uwvm --mode all
python3 tests/ros_perf/run.py --workspace /tmp/uwvm-ros-extension-20261012 --source /absolute/source --uwvm /absolute/uwvm
```

Native suites are bounded to45seconds each (expanded parameter resources90seconds; certificate generation60seconds), use separate exact network Domains/ROS Master ports, and record the actual descendant process tree with PID, command and cgroup membership. The host Ubuntu25.10 `chroot` utility supervises a child; recording only the wrapper PID is insufficient to identify the native executable. The performance runner samples wrapper plus descendants every10milliseconds and separately uses native `getrusage`. It compares separate native publisher/observer processes against actual authorized Wasm publishers with one excluded explicit discovery warm-up,100 measured messages per case,64/4096-byte payloads and a paced100Hz target. It also measures two independently authorized modules, simultaneous interleaved ROS1/ROS2 calls, and200 messages/backend paced at1000Hz, including both64/4096-byte dual workloads. Each process has a512MiB sampled RSS stop budget and bounded deadlines; no cgroup memory limit is changed. Latency uses the same host monotonic clock. Queue depth32 and overflow policy are explicit. These workloads measure paced one-way transport latency and receive rate, not maximum sustainable throughput.

The24 performance rows include1000 authenticated Gateway clock calls/backend and100 generated Trigger calls/backend across4 separate client sessions/threads. Clock timings include authorization, transport, native clock and serialization; they are not a pure IPC-only benchmark. Service/clock probes call the real trusted IPC worker interface and are labelled native, whereas the Topic comparisons execute actual Wasm. A separate100,000-operation Capability benchmark records the manager authorization cost without claiming a ROS network measurement.

The exact55-vector acceptance mapping is [acceptance-matrix.md](acceptance-matrix.md). Additional attack entrypoints are `run_additional_security.py --mode all`, `run_ingress.py --mode all`, `run_revoke_race.py --mode all`, `run_flood.py` and `run_payload_security.py`. The native admission/revoke test uses4 actual C++threads holding manager leases across live IPC publication and proves that the revoke barrier waits, then native receipt freezes. The same runner separately executes two actual dual-Wasm cases proving that revoking either backend leaves the other usable. This does not claim that the Wasm runtime currently exposes a thread-spawn import.

Resource witnesses expose bounded `--witness`/`--observe-*` modes used by independent Wasm invocations. Parameter mutation/deletion and Lifecycle configuration are checked on the actual native node; denied TF broadcasts are monitored for5seconds with no receipt. SROS2 test private keys are created only inside a fresh owned rootfs directory and removed in `finally`, including interrupted runs. Exported evidence contains logs, commands and policy files, not private keys.

The independent `tests/ros_tf/static_update_native_ros2.cc` probe publishes two real DDS `/tf_static` samples with the same timestamp and different translations. A separate native BufferCore witness receives those samples and supplies the reference value for the Gateway lookup. `tests/ros_tf/static_ingress_test.cc` checks the shared cache's single latest static sample, exact duplicate suppression, byte accounting, message/queue/rate admission, all three overflow policies and the unchanged dynamic duplicate-stamp behavior. Compiling this shared-header probe with Noetic C++17 verifies that userspace's code compatibility and cache behavior; it does not establish a ROS1 network static-update test. This cache fix has no implied native middleware sanitizer coverage.

The RPC runner compiles its selected scalar WAT fixtures with the explicit `--wat2wasm` compiler before creating ROS endpoints, checks input stability, and records each command and source/binary SHA. The 49 scalar cases exercise Bool, ROS1 int32, ROS2 int64 and float64 get/set plus Gateway-local ROS2 declare/undeclare. All compare exact canonical bytes, including int64 values above 2^53. The 41 remote cases use independent native parameter witnesses; the eight Gateway-local cases verify state through separately granted typed reads in the same Guest/Gateway. ROS1 denial witnesses sample exact native kind/value for five seconds; ROS2 generated SetParameters witnesses additionally count denied setter requests as zero. Malformed Bool, truncated/excess integer payloads, NaN/infinity, explicit-bound violations and native-kind mismatches preserve failed-read output sentinels and denied-write values. Remote ROS2 non-String scalar writes reject dynamically typed native parameters before SetParameters; internal GetParameters/DescribeParameters preconditions are distinguished from mutation callbacks. Metadata retains the String adapter. Explicit typed array adapters and their separate fixtures are documented below. Detailed observations and their limits are stated in the final report.

Typed ROS2 ParameterEvent scalar fixtures remain in `tests/ros/typed_parameter_events.py` and `generate_typed_parameter_event_fixtures.py`. The six additional projections are defined independently by `tests/ros_parameter_events/events.py` and `generate_fixtures.py`, with native `native_ros2.cc`/`witness_ros2.cc` probes and `sdk_codec_test.cc`/`ipc_contract_test.cc` checks. All use the existing subscription/poll ABI. Tags1..3 remain Bool/int64/Float64; tags4..9 select String, byte array, Bool array, int64 array, Float64 array and String array. The legacy String event type and bytes are unchanged. Expected wire includes version2, the selected tag and three u32-length fields, with fixed overhead23 plus node/name/raw payload bytes. String payloads have an inner byte length; arrays have an element count, and empty String/array values retain those four bytes. Deletion retains the selected tag but has an empty outer payload.

Native fixtures publish exact selected values and filter adversaries: wrong node/name/native kind, typed NOT_SET deletion, malformed timestamps, excess total native entries, full-envelope byte overflow, explicit element-limit excess, numeric-bound violations, nonfinite floats and invalid UTF-8. Every array projection fixture supplies an explicit `max-parameter-elements`; scalar and legacy event grants reject that field. Canonical SDK/IPC scalar String permits embedded NUL. Native String interoperability fixtures use ordinary UTF-8 for the tested Jazzy `rmw_fastrtps_cpp`. Numeric arrays check every element against inclusive bounds, retaining exact int64 precision and finite Float64 bits. The controlled publisher sends rejected inputs before a valid marker from the same ordered writer; tests require the next delivery to be that marker and the queue to be empty afterward. Permission-denied/no-capability cases are separate from queue/filter cases.

The separate `native-nul-diagnostic-r2` runs the same native executable without a Gateway. It publishes official `rcl_interfaces/msg/ParameterEvent` messages whose selected `changed_parameters[0].value.string_value` is either ordinary UTF-8 or contains an embedded NUL, and subscribes independently to `/parameter_events` using reliable/volatile QoS. The plain case returns0 and receives the exact eight bytes; the NUL case returns255 with FastCDR `BadParamException` (`The string contains null characters`). Its original aggregate remains FAIL. This is evidence for the selected scalar String field under Jazzy `rmw_fastrtps_cpp`; it establishes no embedded-NUL network interoperability or behavior for other RMW implementations.

The follow-up `native-nul-diagnostic-r3` uses the same official message's StringArray value. Its NUL-free control is received exactly, but a sender's final element `a\0b` (three bytes) is taken as `a` (one byte), with return0. The Gateway has no original sending bytes after this truncation; neither native nor Wasm fixtures claim rejection of the original embedded NUL. That network negative case has been removed, leaving70 Wasm cases in `tests/ros_parameter_events/run_native.py`. SDK/IPC canonical StringArray NUL rejection remains covered separately. The diagnostic also preserves its scalar-NUL exception and original aggregate FAIL; these observations apply only to the tested Jazzy `rmw_fastrtps_cpp` and do not establish other RMW behavior.

The expanded native probe also uses the actual rclcpp Node parameter APIs to generate automatic new/changed events for each of the six added types. Its deleted-event coverage uses controlled native ParameterEvent publications carrying an explicit prior typed value. It does not claim automatic undeclaration/deletion coverage: typed NOT_SET deletion is intentionally ignored. Before emitting a controlled deletion, the Gateway checks borrowed prior type, element quota, canonical hard payload limit (4193786 bytes), UTF-8/finite validity and applicable bounds without copying old arrays. Delivery `max-parameter-bytes` bounds the actual empty-payload envelope; a prior value exceeding that delivery quota can still produce a deletion when all prior-value checks pass. Worker validation rechecks node/name/tag/structure, full-envelope bytes and nondeleted contents/bounds before Guest copy; empty wire cannot establish prior native contents independently.

SDK and IPC tests cover transactional `typed_parameter_event` encode/decode, 23-byte framing, nine closed tags, exact int64 extremes and values above2^53, bit-exact finite doubles, empty arrays/String, deletion tags, legacy wire compatibility, malformed lengths, full-envelope/element limits and output sentinels. The IPC encoder checks the complete envelope before its final buffer allocation; native projection checks count, bytes and contents before a bridge-owned array copy. `tests/ros_parameter_events/run_ipc_unit.py` records four ASan/UBSan combinations: C++20/C++26 with libc++/libstdc++. Shared codec/unit sanitizer coverage does not instrument native ROS libraries, Gateways or the complete Worker. Actual PASS status, compiler dependencies and executable/source SHA belong to recorded native/RPC/unit results; fixture presence alone establishes no pass result.

The v5 sanitizer run covers 46 policy/capability checks plus the IPC and SDK codec tests with ASan/UBSan and leak detection. Its compiler-recorded dependencies are frozen separately. The native Gateways and installed ROS middleware are not sanitizer instrumented; their bounded native stress tests provide a narrower observation.


The array parameter runner has a separate `--mode arrays` and is included in `--mode all`. It compiles selected WAT sources before starting any ROS endpoint, then uses independently generated native parameter services/actual ROS1 Master state to observe exact kinds/values, legitimate setters and denied mutation windows. Physical test directories use bounded hashed names, while result names retain the complete case labels. Each failed array read checks the entire4096-byte destination and output length sentinels. The aggregate resource deadline is1800seconds for the enlarged matrix; individual operation and native observation deadlines remain bounded separately.

## Additional contract and I/O regression entrypoints

[`tests/ros_parameter_deletion`](../../tests/ros_parameter_deletion/README.md)
tests the independent deletion-only projection: tag 0, kind 3 and an empty
payload. Six sanitizer configurations check IPC/Gateway contracts and, with
C++20/26, Worker schema and SDK codecs. The CLI runner exercises 37 explicit
global/single/group/ROS1 policy cases. Native SDK deletion and 19 Wasm cases use
domain 192 and a private Jazzy rootfs. Prior parameters are declared before the
subscription, so these notifications do not rely on cached previous values.
The existing 70 typed-event cases remain a separate regression in domain 191.

[`tests/ros_service_deadline`](../../tests/ros_service_deadline/README.md)
checks response admission against both the call and pending-request deadlines,
including wire/native preparation, mutex waits, retry, close and duplicate
responses. The native ROS2 matrix observes whether a response was delivered.
The Noetic shared-header checks and build establish C++17 compatibility, not an
additional ROS1 network result. These are admission checks; mutex acquisition,
regular-file I/O and middleware sends have no hard interruption guarantee.

[`tests/ros_parameter_borrowed`](../../tests/ros_parameter_borrowed/README.md)
measures scalar String validation and generated/native result construction.
Allocation intervals exclude policy/input setup and logging. Validation without
an owning decoded value allocates zero times; successful native helper results
allocate one final wire buffer. These helpers do not cover the whole Node RPC or
upstream deserialization. Historical sampler versions and failed attempts remain
recorded with their actual evidence boundaries.

[`tests/ros_ipc_client_deadline`](../../tests/ros_ipc_client_deadline/README.md)
checks the client's method-entry budget across its connection mutex and exact
request encoding. Six sanitizer profiles use real Unix sockets, observe the
actual futex waiter, and verify that unwritten expired requests leave the session
usable. The separate Jazzy test gates a real Trigger callback and observes native
requests on an independently granted service. The existing unit runner includes
the new test under `--suite ipc` and `--suite all`; that selection does not claim
hosted CI execution. Its product runner builds four fresh interpreter profiles
and checks the existing regression subset, separately from the earlier A15 run.

[`tests/ros_native_audit_io`](../../tests/ros_native_audit_io/README.md) checks
the C++17 Gateway audit formatter and sink, including real pipes, sockets, TTYs,
regular-file offsets, unchanged shared stderr flags and caller SIGPIPE/errno
state. Its three profiles exercise the helper and the exact budgeted audit
function; full Gateway integration compilation is a separate check. This scope
does not cover other `std::cerr` diagnostics or guarantee bounded regular-file
I/O. The native adapter uses C++17 `to_chars` and owned descriptors because the
vendored fast_io API requires C++20 concepts.

[`tests/ros_parameter_exception`](../../tests/ros_parameter_exception/README.md)
uses the actual Jazzy SDK to classify static and read-only local undeclaration
refusals, then checks the Gateway's result and the same session's native DDS
sibling subscription. The static95 path is an existing-behavior control;
read-only13 must preserve the session and subscription. A dynamically declared
parameter still permits normal deletion. The probe uses C++20 and fast_io, while
the Gateway remains C++17; this is a focused native test, not a Wasm/A15 rerun.

[`tests/ros_audit_io`](../../tests/ros_audit_io/README.md) checks bounded Linux
pipe/socket/TTY writes, unchanged stderr flags and SIGPIPE state, regular-file
offset/append behavior, and saturating delivered/dropped diagnostics. Standalone
tests compile the helper; full Worker compilation is a separate provenance
claim. [`tests/ros_sdk_event_encode`](../../tests/ros_sdk_event_encode/README.md)
checks exact typed-event wire and one final allocation per accepted encoding,
with zero allocations and an unchanged output on refusal. Its `.inc` reference
encoder is an explicit test oracle.

`tests/ros/run_ros2_contracts.py` builds four Worker profiles and 18 native targets
from a frozen previous run plus an explicit overlay, then runs all 15 A15 suites.
Use fresh output/mount paths and the prepared Noetic/Jazzy rootfs workspace.
`verify_ros2_contracts.py` independently checks its original commands, compiler
dependencies, exact artifacts, live cgroup proof and cleanup. The separate
`verify_ros2_contracts_current.py` binds current files to that frozen run and to
explicitly tested post-freeze test revisions; it never reclassifies a failed
attempt or claims a revised fixture was a main compilation input.
