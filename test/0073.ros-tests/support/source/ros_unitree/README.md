# Unitree simulation, modular execution, and native performance fixtures

This directory contains **test implementations, not recorded passing results**.
Every executable, SDK, official source snapshot, model, and build artifact is an
explicit immutable input. No script downloads software, discovers a compiler,
starts a physical robot, launches an LLM, or gives a Guest implicit ROS rights.
Run actual compilation and acceptance only after the complete source freeze.

## What each result measures

| Entry point | Actual workload | Qualification boundary |
| --- | --- | --- |
| `prepare.py` | Serial direct compiler/linker commands and full dependency reopening | Compilation only; never runtime acceptance |
| `run_typed_transport.py` | Fourteen same-main C/C++/Rust/embedded-JS/AssemblyScript/TinyGo command products in28 fresh denied/positive Runtime processes, with separate real generated GO/HG/API DDS witnesses | Actual selected interpreter/Gateway/native typed transport only; requires complete producer receipts and live mappings; see [the exact contract](TYPED_TRANSPORT.md) |
| `run_simulation.py --scope replay-relay` | Matched native/Wasm Go2 command generation, real synchronous MuJoCo, actual separate native DDS/ROS peer | Commands are replayed through a trusted native peer; this scope alone does not prove Guest motor imports |
| `run_simulation.py --scope direct-runtime` | Actual typed Guest LowState subscription, shared control calculation, CRC and LowCmd publication; matched compiled native C++ controller; separate real simulator and native state witness | Real ROS2/SDK2 protocol and selected model/slot map; no physical firmware or locomotion certification |
| `run_simulation.py --scope native-model` | Actual native GO20/HG35 sampled-position feedback, official message CRC and a caller-selected real model | Native protocol/mapping/finite physics only; no Guest equivalence claim |
| `benchmark.py` | Identical deterministic scalar math in a fused native C executable and actual modular Wasm instances/callbacks | Math/dispatch and declared transport overhead; excludes ROS, DDS, MuJoCo and perception |
| `profile_vtune.py` | Actual official installer/self-check/host hardware collections in separate actions | Observed host PMU data only; installation or metadata cannot prove PMU support |

`--scope all` runs the Go2 replay/relay and selected direct controller scopes.
For a humanoid or other explicit model map, select `direct-runtime` with
`controller_mode: "sampled-hold"`; the Go2 stand trajectory is rejected for
other robots. A sampled hold recomputes its target from each delivered native
position. It demonstrates typed protocol and control composition, **not balance,
walking, an autonomous policy, or a persistent desired-pose controller**.

`model-support.json` lists the nine actual scene paths retained in the official
source acquisition. Candidate message families are not compatibility results.
The admitted model's exact actuator names, sensor names, carrier slots, native
message family, operating modes, and model include/mesh closure must agree with
the selected official revision. No test result for Go2 qualifies G1, a wheeled
model, another firmware release, or a legacy ROS1 driver.

## Admission, resource limits, and process ownership

Invoke the actual frozen source entrypoint from the Root-owned Linux execution
lane. The coordinator, Python actors, native peers, compiler/linker children,
installer, VTune collector, finalizer, and reporting children must already
inherit the **same exact 64 GiB cgroup with swap disabled**. The harness never
creates a cgroup, migrates a process, increases limits, invokes systemd, uses a
container/socket daemon, changes a sysctl, or installs a sampling driver.

The closed configuration records current boot ID, cgroup leaf, device/inode,
CPU set, `memory.max = 68719476736`, and `memory.swap.max = 0`. A reboot or
recreated leaf invalidates those identities. The common probe maintains a
four-GiB memory reserve and requires every admitted ancestor's OOM counters to
start and remain zero. The local process budget is at most eight GiB aggregate
owned RSS, one compilation/case/collection at a time, finite deadlines, bounded
logs, and at most four GiB total owned output. Cooperating simulator, witness,
controller and collector processes form one case; they are not parallel test
jobs. All output ancestors must retain eight GiB filesystem free space, which
also checks the backing filesystem of a sparse Btrfs artifact image.

The output path is fresh, absolute, and cannot overlap frozen source or any
complete input root in either ancestry direction. `/` is never an admitted
input root. Whole source/SDK/upstream/tool/keystore inventories and physical
file pins are compared before and after. Commands are literal argv arrays with
an explicitly pinned executable; there is no shell interpolation or PATH
discovery. Process birth identities, pidfds, cgroup membership, owned descendants,
bounded terminal retirement records, logs, failures, and cleanup are retained.
No signal is sent using an unproved numeric PID or process group.

Use a separate Root-created **loopback-only network namespace** for simulation.
The harness verifies the exact selected namespace inode spelling, that `lo` is
the only network interface and that it is up. Direct controller fixtures use
isolated domain 1; other native-only cases may explicitly select domains 1–232.
Root creates/admits this namespace before invoking the harness and without
moving any process outside the cgroup. These entrypoints never configure a
physical network interface or route commands to a real robot.

## Immutable inputs and native environment

`config.schema.json` describes the closed configuration. Its top-level keys are
`format_version`, `cgroup`, `input_roots`, `inputs`, `environment`, `limits`,
`upstreams`, `simulation`, `benchmark`, and `profiler`; unused workload selections
are explicitly `null`. A named input has exactly `path`, `sha256`, and `bytes`.
At runtime the opened device/inode/mtime/ctime are also retained. Do not fill
unknown hashes with placeholders or manufacture successful producer receipts.

Each official upstream selection contains `root`, `repository`, exact Git
`commit`, and a `lock_input` label. The lock must be an actual
`OFFICIAL_SNAPSHOT_GIT_TREE_AND_ALL_BLOBS_VERIFIED` acquisition proof with the
complete regular-blob/symlink list and `code_executed: false`. The actual combined
six-repository proof is also supported when its object key equals the configured
upstream name. Every blob is reopened; the complete file set must agree.
Unmaterialized Git links remain absent and are never claimed as acquired.
Keep each selected upstream root explicitly in `input_roots`, along with the
complete native SDK/tool roots containing transitive executable dependencies.

Use actual compatible native Python packages and official SDK2 Python imports.
The pinned SDK2 Python setup requires **CycloneDDS Python 0.10.2**, NumPy and
OpenCV; a newer CycloneDDS wheel is not a substitute. A separately pinned
Python 3.10 native ROS2 SDK is a suitable explicit lane when supplied by Root;
availability of a host Python 3.14 executable does not establish compatibility.
The SDK's actual native CRC helper is required and currently has official
x86-64/AArch64 library inputs. This does not make other Runtime architectures
depend on SDK2: their pure ROS message gateway tests are separate.

The explicit environment allowlist is in `common.py`. Common selections are
`ROS_DOMAIN_ID=1`, `RMW_IMPLEMENTATION=rmw_cyclonedds_cpp`, a pinned
`CYCLONEDDS_URI` selecting loopback, and admitted `AMENT_PREFIX_PATH`,
`LD_LIBRARY_PATH`, `PYTHONPATH` and CPU-only `MUJOCO_GL=osmesa` when rendering.
The harness owns HOME, temporary/cache/config directories and sets numerical
thread pools to one thread. It imports actual MuJoCo, SDK2 and rclpy only after
child admission. No import starts an actor.

Selected ROS security settings are preserved. `ROS_SECURITY_ENABLE=true`
requires `ROS_SECURITY_STRATEGY=Enforce` and a completely inventoried explicit
keystore. A loopback fixture using an explicitly nonsecure native profile cannot
qualify secure ROS cells. Trusted rclpy witnesses disable rosout and parameter
services; their distro may still create its native parameter-event publisher.
That native fixture behavior never adds Guest rights.

## Real physics, topics, and motor mapping

The driver loads `MjModel.from_xml_path` from the selected actual official
Unitree asset closure and advances actual `mujoco.mj_step` once per explicit
barrier. This deterministic fixed-step adapter uses the official SDK2 message,
channel and CRC imports and the official actuator PD/torque formula. It does
not launch the original GUI/threaded simulator application or fabricate physics.
One thread owns `MjData`; DDS callbacks only fill a bounded single-command
handoff. Duplicate, stale, malformed, nonfinite, missing, or late records fail
the case and retain diagnostics.

`simulation.motor_sensors` is ordered by actual MuJoCo actuator ID. Each row
contains `actuator`, **`slot`**, `q`, `dq`, and `tau`. `slot` is a unique explicit
GO20/HG35 DDS carrier index, independent of actuator ordering. The three scalar
sensor names and the quaternion/gyro/accelerometer names in `imu` must exist
with actual expected dimensions. Do not infer a humanoid carrier map from Go2.
The complete DDS carrier is published, with inactive native state slots zero.
The independent peer checks actual full-state CRC and all motor float values.

SDK2 topics are `rt/lowcmd` and `rt/lowstate`; their ROS2 counterparts are
`/lowcmd` and `/lowstate`. The actual middleware must prove delivery between the
selected representations. Sport `/api/sport/{request,response}` is topic RPC,
not a ROS service, and the low-level MuJoCo fixture does not implement SportAPI.
Legacy `unitree_legged_msgs` camelCase ROS1 fields and native CRC differ from
SDK2 GO. A verified legacy bridge/SDK and a separate actual legacy run are
required; no ROS2 result is relabeled as ROS1 behavior.

Optional rendering is actual bounded CPU OSMesa RGB. Select an existing named
model camera or explicit camera `-1`, which is a derived MuJoCo free camera,
not a claimed native robot camera. Retained RGB bytes, dimensions, hashes and
explicit pixel tolerance are compared. Rendered physics and synthetic modular
distance arithmetic do not establish perception or a camera-driver adapter.
MuJoCo determinism here means the same admitted model/version/backend and step
sequence; this fixture makes no cross-version or cross-architecture bitwise
physics promise.

## Matched direct Guest/native contracts

The `direct_runtime` selection binds positive/empty-grant Guest jobs and build
receipts, actual Runtime ELF, matched compiled native controller ELF/receipt,
`steps` (1–32), `warmup_steps` (0–2), a declared slowdown trigger,
`controller_mode`, `hold`, `modular`, and `libraries`. Domain 1, ROS2 native
graph discovery and exact actual endpoint counts are required before publishing
the volatile initial state. Basic stdin/stdout is test instrumentation; no
Guest clock, random, socket, filesystem, or ambient ROS authority is required.

For `controller_mode: "go2-stand"`, `hold` is `null`, the robot is actual Go2,
and its twelve actuator slots are ordered 0–11. The initial twelve-byte stdin
frame is `UCK1`, little-endian count and little-endian timestep nanoseconds.
The single authoritative SDK `simulation_control.h` is shared by native C,
compiled native ROS2 C++, the typed Guest and the optional pure motion module.
`controller_kernel.h` is only a forwarding include.

For `controller_mode: "sampled-hold"`, `modular` is false and `libraries` empty.
`hold` supplies positive bounded binary32 `kp`, `kd`, and explicit `mode_pr`;
`mode_machine` comes from the simulation selection. GO modes must both be zero
because GO has no HG mode fields. The exact 59-byte setup is:

| Byte offset | Encoding |
| --- | --- |
| 0 | ASCII `UHL1` |
| 4, 8 | little-endian count and timestep nanoseconds |
| 12–15 | carrier capacity (20/35), active count, mode_pr, mode_machine |
| 16, 20 | little-endian IEEE binary32 kp and kd |
| 24–58 | 35 actuator-to-carrier slot bytes; unused entries are 255 |

The shared `simulation_hold.h` validates this frame and computes the same
sampled-position command on both controllers. Native GO/HG message types are
separate compiled selections. No Go2 trajectory is applied to a humanoid.

After actual endpoint construction the controller emits its policy-specific
READY line. A host `R` permits one poll. `UNITREE_AGAIN` means no command was
published. An actual callback requires input tick equal to the 1-based step,
computes and publishes one CRC-checked LowCmd, and emits:

```text
UNITREE_STEP <step> <input-tick> <command-crc-u32> <state-crc-u32>
UNITREE_STATE <step> <input-tick> <q/dq/tau IEEE-u32 words>
```

Stand emits the first twelve motor triples; hold emits all twenty or thirty-five
triples. The host compares words with the independent native LowState witness,
then requires the real simulator to receive the same command CRC. The simulator
retains all actual command fields, real qpos/qvel, state CRC, and next state.
The native and Wasm command binary32 fields and subsequent physics are compared.
At most 4096 poll gates are allowed. `Q` is accepted only after the configured
publication count; checked endpoint closure and the exact final PASS count are
required. A Guest marker alone is never a native delivery oracle.

The empty-grant build must report its exact denied marker, while an independent
real SDK2 command subscriber observes at least 200 ms and 30 samples with zero
commands and zero physics steps. Default grants are never added by this harness.
This negative witness is evidence for the selected aliases/profile, not a claim
that every possible capability attack has been tested.

Set `modular: true` only for the stand root compiled with
`UWVM_UNITREE_MODULAR_MOTION=1`. Bind `libraries` in order `callbacks`, `motion`,
each with `guest_input` and actual `build_input`. Root argv must preload exactly
those artifacts as those names, disable global WASI/ROS1/ROS2 authority, and
explicitly disable all three interfaces for each pure provider. The motion
module calls the same real stand kernel and Wasm callback module through scalar
word getters; root performs CRC/ROS only after library calls unwind. No shared
linear-memory pointer is used. Actual native command and physics comparison
qualifies this composition independently from the synthetic String example.

The compiled native baseline is `native_ros2_controller.cpp` with the companion
explicit CMake input. It uses actual installed rclcpp/unitree_go or unitree_hg,
the same SDK helper/CRC, quiet explicit Context/Node options, single-thread
polling and the same bounded stdin gates. Its source cannot be replaced by a
Python controller for an equivalent C++ native/Wasm timing claim. Python peers
remain independent witnesses. Actual distro compilation is still required.

## Actual serial build receipts

`prepare.py --recipe-input <label>` consumes a pinned recipe with version 1 and
a bounded `targets` array. A target has `id`, relative `output`, `kind`
(`native`/`wasm`), integral `definitions`, direct command `steps`, `validation`, and `compiles`
associations `{step, source, object, dependency}`. There must be one or more
compile commands followed by a separate final direct link consuming every owned
object. Source paths are relative to the complete freeze; outputs/depfiles are
relative to the target's fresh owned directory. `steps` use normal pinned jobs
and additionally expand `{target_out}`. Other supported placeholders are
`{source}`, `{out}`, and `{input:<label>}`. No CMake/shell wrapper is interpreted
by this producer; Root can supply actual direct commands or a separately
reviewed equivalent physical producer.

For Wasm, `validation` is an explicit pinned wasm-tools job whose actual argv is
`validate <owned-output>`; its real execution/logs are retained in the receipt.
For native targets it is `null`; later consumers check the actual ELF and host
architecture. Magic bytes alone are never called Wasm validation. Pure-library
import/export/memory/start contracts additionally require the actual ABI
inspection evidence from Root's module acceptance lane.

All translation units require `-O3 -MD -MF <owned-depfile> -ffp-contract=off` and
their actual absolute source/object arguments. Use explicit `-fno-fast-math` for
floating controllers. Business definitions are explicit `-DNAME=<integer>` in
every compile. Assertions remain active; `-DNDEBUG`, `-MMD`, `-MP`, `-Ofast` and
fast math are rejected. The one-rule full dependency file includes system/SDK
headers and must target the real object. Each dependency belongs to the full
source or a completely inventoried SDK/tool root and is reopened/pinned.
Linker archive/library inputs must also be explicitly admitted and traced by
Root's actual link producer; a compiler executable pin alone is insufficient.

The receipt has exact fields `format_version`, `kind: "unitree-fixture-build"`,
`status: "PASS"`, `source_manifest_sha256`, opened seven-field `output`, complete
`source_inputs`, actual `definitions`, actual `commands` (argv, returncode,
stdout/stderr pins and full dependencies), and `full_dependencies_verified`.
It is emitted only after actual successful commands. Consumers reopen every
pin, verify required SDK math/header dependencies and reject source/definition
or output mismatches. Runtime LLVM/backend qualification is a separate Root
build/acceptance obligation; this receipt never claims to qualify an engine.

Native C `controller.c` and its Wasm build consume UCK1 and output `UCM1`, count,
timestep followed by records containing step and twenty six-word motors. All
frames use explicit little endian without native structure padding. The Wasm
entry uses a linker `export_name("_start")` on its actual function; use the
actual explicit no-entry/attribute export contract, not an invented native
symbol named `_start`. Select an actual pinned WASI libm and preserve its link
archive provenance. No guest clock is used.

## Performance scopes and hardware profiling

`benchmark.py` requires the same seed/count macros and exact shared
`robotics_math.h` in both actual builds. The native baseline intentionally fuses
the math; Wasm includes real separate motion/vision/callback dispatch. Their
exact eight-byte little-endian checksum must agree for every warmup and sample.
One-shot mode includes process/load/instantiate/batch/exit; separate process
warmups are never called a warmed instance. Session mode waits for `READY\n`
and sends bounded `W`/`P` batch commands and `Q` to the same live instance.
Every batch resets the seed and callback generation. Warmup+samples+quit is at
most 256 commands, with at least 30 actual retained measured samples. Session
roles run serially; one-shot adjacent repetitions alternate role order.

Reports retain actual wall samples, p50/p95/p99/min/max/mean, checksum, physical
ELF architecture, actual argv/backend selection, peak owned RSS, and declared
slowdown threshold. They do not present pipe/scheduler/supervisor overhead as
pure kernel CPU time. Direct ROS timings separately include callback polling,
supervisor barrier, real physics, DDS and independent state receipt. At least
30 post-warmup steps per role are required for direct performance qualification.
These are controlled fixture measurements, not a promise of native experience.

Only an actual retained slowdown report with preserved source/inputs can enable
collection. `profile_vtune.py --action install` requires an actual official Intel
installer acquisition receipt and runs an owned installer copy with explicit
nonroot UID/GID. It retains original help/options and uses official silent EULA,
custom install/log/cache/extraction paths. It changes no system package, driver,
global permissions or kernel setting. `--action verify` retains installed
version, per-analysis help and the actual self-checker. Self-checker process exit
alone does not label individual analyses supported.

`--action collect` launches the exact actual measured native/Runtime argv with
an explicit pinned stdin file when needed. `--action simulation-collect` creates
fresh real simulator/witness/controller cases for each analysis and attaches
to the pidfd/birth-verified actual controller after endpoints match. It waits
for actual collector readiness before releasing the initial state. Hardware
Hotspots and microarchitecture collections run serially. Both retain actual
raw hardware-event CSV, summary and collection files; Hotspots also retains
function attribution. Positive real cycles/instructions columns are required,
and uarch must expose actual top-down metrics. Unsupported PMU/permissions,
short samples, or absent counters fail and remain visible. Software CPU-time
sampling is never substituted for requested hardware counting.

The target ELF must match the physical host. QEMU user/system-mode functional
architecture tests belong to the broader runtime matrix; host Intel counters
under QEMU measure host translation/execution and cannot establish emulated
LoongArch/AArch64 microarchitecture. A collected profile is an optimization
input, not proof that a bottleneck was fixed. Root must implement any justified
optimization and repeat matching correctness, security and performance checks.

## Invocation after source freeze

All variables below are **actual Root-selected absolute paths/hashes**. Run the
selected command through the current Root guard in the admitted namespace and
cgroup. Each invocation gets its own never-existing output path. Do not execute
these commands on the macOS workspace or bypass the outer resource guard.

```sh
"$PYTHON" -B "$SOURCE/test/0073.ros-tests/support/source/ros_unitree/prepare.py" \
  --source "$SOURCE" --manifest "$MANIFEST" --manifest-sha256 "$MANIFEST_SHA256" \
  --config "$CONFIG" --config-sha256 "$CONFIG_SHA256" --out "$FRESH_BUILD_OUT" \
  --recipe-input "$RECIPE_LABEL"

"$PYTHON" -B "$SOURCE/test/0073.ros-tests/support/source/ros_unitree/run_simulation.py" \
  --source "$SOURCE" --manifest "$MANIFEST" --manifest-sha256 "$MANIFEST_SHA256" \
  --config "$CONFIG" --config-sha256 "$CONFIG_SHA256" --out "$FRESH_SIM_OUT" \
  --scope direct-runtime

"$PYTHON" -B "$SOURCE/test/0073.ros-tests/support/source/ros_unitree/benchmark.py" \
  --source "$SOURCE" --manifest "$MANIFEST" --manifest-sha256 "$MANIFEST_SHA256" \
  --config "$CONFIG" --config-sha256 "$CONFIG_SHA256" --out "$FRESH_BENCH_OUT"

"$PYTHON" -B "$SOURCE/test/0073.ros-tests/support/source/ros_unitree/profile_vtune.py" \
  --source "$SOURCE" --manifest "$MANIFEST" --manifest-sha256 "$MANIFEST_SHA256" \
  --config "$CONFIG" --config-sha256 "$CONFIG_SHA256" --out "$FRESH_PROFILE_OUT" \
  --action simulation-collect
```

Signing, WasmGit tracking, multi-program deployment orchestration and WasmHub are
outside these runtime fixtures. Per-module capability enforcement remains an
actual runtime obligation. An optional vLLM deployment is unnecessary for these
CPU simulations and is never launched implicitly.

## Primary sources

- [Official Unitree MuJoCo simulator and model assets](https://github.com/unitreerobotics/unitree_mujoco).
- [Official Unitree ROS2 integration](https://github.com/unitreerobotics/unitree_ros2).
- [Official SDK2 Python types, channels and CRC](https://github.com/unitreerobotics/unitree_sdk2_python).
- [Official legacy ROS-to-real integration](https://github.com/unitreerobotics/unitree_ros_to_real).
- [MuJoCo Python stepping and rendering](https://mujoco.readthedocs.io/en/stable/python.html).
- [Intel VTune official download](https://www.intel.com/content/www/us/en/developer/tools/oneapi/vtune-profiler-download.html).
- [Intel regular installation guide](https://www.intel.com/content/www/us/en/docs/vtune-profiler/installation-guide/2026-0/regular-install.html).
- [Intel hardware profiling without sampling drivers](https://www.intel.com/content/www/us/en/docs/vtune-profiler/cookbook/2025-4/profiling-hardware-without-sampling-drivers.html).
- [Intel microarchitecture command-line analysis](https://www.intel.com/content/www/us/en/docs/vtune-profiler/user-guide/2026-0/run-general-exploration-analysis-command-line.html).
- [Linux perf security and CAP_PERFMON](https://www.kernel.org/doc/html/latest/admin-guide/perf-security.html).

Repository links explain the upstream interfaces. Actual acceptance is bound
to exact caller-selected acquisition proofs, never the moving branch shown by
a documentation link.


## Direct Go2 discovery with a compiled ROS2-only Worker

The native controller creates a LowCmd publisher during initialization. The
typed Guest uses a lazy Gateway publisher: its first genuine LowState callback
creates and publishes LowCmd. The direct physics coordinator therefore requires
two actual LowState subscribers and zero LowCmd publishers before the Wasm
bootstrap; it sends that one initial LowState, consumes the actual Guest result,
then requires the real LowCmd publisher before advancing the first physics step.
Native readiness continues to require one publisher before bootstrap. No dummy
publication, native relay, extra grant or retry replaces the first Guest action.

For a modular direct job, optional `direct_runtime.runtime_build_input` names the
actual retained native-interpreter-four-profile build report for its selected
Worker. The harness checks the exact selected binary, all three genuine full-MD
compiler rows and ROS import macros, the complete original source/MF and unchanged
current counterparts of consumed project files. A compiled ROS2-only Worker has
no ROS1 CLI options. Only after that receipt and actual root/callback/motion
binary import inspection prove ROS1 is absent may ROS1 policy flags be omitted.
WASI and ROS2 global/single provider disables remain mandatory, as do actual
callbacks-then-motion preload order and empty provider authority. Configurations
without the optional receipt keep the original dual-backend policy requirements.

Compilation, default-denial, independent DDS delivery, actual MuJoCo stepping and
matched performance remain separate results. A timing result needs at least
thirty post-warmup samples per role and prior numeric/CRC/physics correctness;
this source change itself supplies no such result or physical-robot permission.
