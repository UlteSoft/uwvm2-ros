# L01 Native Tests in the ARM Entry

`test/0073.ros-tests/support/source/ros_aarch64/run_unit.py --suite wasip1-default-deny` is the concrete
compile/run entry for 17 native rows. `--suite all` retains the original five
ROS units and C++17 sorted-row regression while adding this group. Existing
wrappers that replace `SUITES`, including the Action QoS policy wrapper, remain
compatible.

| Rows | Purpose |
| --- | --- |
| `wasip1-configuration` | Real policy helpers/public callbacks and all mask-selection combinations. |
| `wasip1-authority32`, `wasip1-authority64` | Actual direct native API denial and independently granted positive cases in both WASI pointer widths. |
| Twelve `wasip1-positive-*` function rows | Existing clock resolution/time, random, signal callback, yield callback and polling regressions in both widths. |
| Two `wasip1-positive-hybrid-*` rows | Existing multithreaded and mixed WASI stress tests with explicit monotonic grants. |

These programs test the real implementation as ARM native executables. They do
not execute a Wasm Guest or establish ROS network interoperability. The separate
Worker CLI matrix must retain its own Guest, Wasm, process and result evidence.

## Inputs and Closure

Use the current frozen source manifest and exact current Worker compiler/QEMU
configuration. Every selected source and actual project dependency must appear
in that manifest, including `test/0008.imported/wasi/wasip1/func/fp_control_probe.h`.
Old manifests omitting the new tests cannot qualify this matrix.

The entry inspects the adjacent frozen `build_worker.py` through Python AST,
without importing/executing its builder. It extracts the current literal base
flags and all four include directories, preserving the Worker C++26, wasm64,
socket and native memory configuration. The builder SHA is recorded and checked
again. The compiler JSON supplies actual compile/link flags; `-UNDEBUG` keeps
checks active, and each authority row adds its pointer-width macro.

`--sdk-manifest` supplies the frozen cross SDK inventory (`regular_files` and
`symlinks`); its default is `sysroot.parent/sdk-files.json`. SHA/size and link
spelling are checked before and after. `--compiler-resource-root` supplies the
Clang builtin header tree; its default is `compiler.parent.parent/lib/clang`.
Files and links are snapshotted before compilation and rechecked afterward.
Compiler resource links may not escape that root.

Actual original/witness compilations use `-MD` and distinct `-MF` records. Every
dependency must belong to the frozen project, qualified SDK, prequalified
compiler resource tree or recorded generated barrier/witness. Unknown or
mutated dependencies fail. This records actual compilation, not preprocessing
replay. These inputs are mandatory for the new L01 rows.

From inside the already verified Linux cgroup, use new output paths:

```bash
python3 -B /path/to/frozen-drivers/run_unit.py \
  --source /path/to/current-frozen-source \
  --manifest /path/to/current-source-manifest.json \
  --config /path/to/current-worker-config.json \
  --sdk-manifest /path/to/cross-deps/sdk-files.json \
  --compiler-resource-root /path/to/current-toolchain/lib/clang \
  --suite wasip1-default-deny \
  --out /path/to/new-l01-native-results
```

These placeholders identify required inputs, not existing qualified artifacts.
Do not compile or execute the programs locally as a substitute for Linux.

## Execution and Results

Compile/link commands are serial and bounded to 180 seconds with a sampled
2 GiB aggregate compiler process-tree RSS budget. Tests are bounded to
60 seconds and use private per-row working directories for created resources.
The driver requires the provided 64 GiB cgroup and zero swap, records leaf
device/inode, and rechecks identity. `Evidence` pins leaf device/inode and
kernel boot before accepting the OOM baseline, captures baseline/final probes
between identity checks, and never subtracts counters across changed or
unproven incarnations. Missing required OOM counters fail. Caller metadata
cannot replace this mandatory verdict.

Existing architecture evidence retains live child membership, PID birth, host
executable identity and actual mapped ARM device/inode. A SIGSTOP constructor
in the original translation unit stops before main; the driver captures the
live ARM map before SIGCONT. Original/witness objects and final test binaries
must be ELF64 little-endian AArch64. QEMU's host `/proc/exe` is recorded separately.

The 14 existing positives are often silent and some contain empty wasm64
fallback mains. Their source/main names remain unchanged. A separate generated
object uses linker `--wrap=main` to call `__real_main`, then emits one exact
selected-row marker after its zero return. Compile-time guards reject missing
WASI/wasm64 features. Renaming main would remove C++'s implicit return-zero
semantics, so it is avoided. Original and witness have separate dependency
files; only the original receives the SIGSTOP constructor.

Configuration success requires its own marker with at least 16,777,216 checks.
Authority success requires an exact width and nonzero check count. Other,
duplicate or zero-count markers and explicit failures do not satisfy a row.
Raw logs, commands, artifact hashes, dependency closures and architecture
evidence remain available. The main report is `results.json`; architecture
evidence is nested at `architecture/architecture-evidence.json`.

LLVM clock floating-point control and current-policy checkpoint restore remain
mandatory separate regressions in their existing LLVM/runtime harness. This
native matrix does not satisfy those separate requirements.

## Prepared Source Verification

`check_arm_entry_sources.py` inspects source and runs pure Python helper and
synthetic filesystem/proc models, including device/inode/boot replacement,
missing baseline/counters, metadata override and the existing six fake-proc
rejection models. It invokes no compiler, QEMU, Guest, ROS or real cgroup:

```bash
python3 -B test/0073.ros-tests/support/source/wasip1_default_deny/check_arm_entry_sources.py \
  --output /tmp/new-l01-arm-entry-source-report.json
```

The report binds exact source hashes and refuses to overwrite evidence. Actual
native compilation/execution remains `PENDING_LINUX_CGROUP` until this entry
passes with real qualified inputs. Source-model success is never native or
Guest execution evidence.
