# Robot scene qualification

These tools exercise the independent `uwvm_ros_v2` implementation. The existing
WASIp1 and ROS-v1 implementations and SDK bindings are outside their mutation
scope. Compilation, testing and profiling must enter the existing shared
64 GiB, zero-swap resource gate before running any workload:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/runtime_checks.py --mode all
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/runtime_checks.py --mode thread
```

The runner enrolls the child into the existing container cgroup, verifies its
identity and limits, then executes the requested command. It creates no second
workload budget and never increases limits. The check tool independently
verifies membership and records before/after source hashes and memory events.
Default output is `../uwvm2-ros-sdk/.tmp/ros-scene-security/runtime-current`.

`runtime_checks.py --mode all` runs Clang ASan/UBSan and GCC portability tests.
`--mode thread` runs the concurrent resource and broker tests with TSan. These
cover authority isolation, exact action indices, revocation, allocation failure
during retirement, quota accounting, transactional delivery, deadlines and
nonblocking wait ownership. They are core C++ tests; real Wasm memory and import
checks are in `test/0084.ros-runtime/run_scene_vm.py`.

## Actual VM, debugger and portability

The complete-frame exchange checks exercise full-range tag/bounds validation,
byte-atomic writes and concurrent memory growth. The actual VM matrix checks
all payload bytes and full-capacity erasure with zero through 4096-byte messages:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/module_security_checks.py \
  --suite module_exchange_frame --thread --source-root SOURCE --output OUTPUT
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 test/0084.ros-runtime/run_module_exchange_frames.py \
  --runtime VM --output OUTPUT
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/module_exchange_frame_portability.py \
  --source-root SOURCE --recipes PLACEMENT_RECEIPT --output OUTPUT
```

Use a frozen source tree with local copied vendor headers for the dependency
snapshot check, and a fresh output directory per invocation. The portability
receipt adds four target ABIs to the x86-64 sanitizer checks; QEMU results are
functional evidence, not performance measurements. The complete-frame runtime
helper has no public import or independent permission.

The scheduling import suite exercises its 19 scene entries on interpreter
and LLVM, including memory64 and nonzero memory indices. The threaded suite
uses two real concurrent child threads with a shared capability and separately
checks missing grants, denied imports and explicit zero budgets:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 test/0084.ros-runtime/run_scene_vm.py --runtime VM --output OUTPUT
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 test/0084.ros-runtime/run_scene_threads.py --runtime VM --output OUTPUT
```

[Scene debugger qualification](../debug/README.scene.md) includes formatter,
DAP and editor-controller checks plus a real Linux VM/broker/DAP process chain.
The graphical editor and native macOS/Windows execution require separate
qualification. Debugger permission is independent of robot scene authority.

The tagged-memory host-boundary suite exercises actual software MTE pointers,
memory32/64, memory indexes zero/one and both interpreter/LLVM backends:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 test/0084.ros-runtime/run_scene_memtag.py \
  --runtime VM --output OUTPUT
```

Matching tags must copy correctly. Separately, the trusted `silent-log` mode
allows different tag encodings of the same physical span, proving scene request
and output length-slot disjointness still rejects aliases without consuming a
queued guard event or overwriting canaries. A disjoint diagnostic-mode control
must succeed and emit the exact tag evidence. Request, output and length tag
mismatches under default enforcement must terminate with the real crash signal
and exact pointer/memory/tag diagnostics. Diagnostic code 122 is not asserted
as the process exit code: this build's default crash action delivers SIGILL.
The driver sets `RLIMIT_CORE=0` before intentional fault children. This suite
qualifies software tag checks; it makes no ARM hardware-MTE claim.

`portability.py --recipes BUILD_RECIPES --output OUTPUT` reuses existing
cross-toolchain recipes for AArch64, ARM32 and i686 core contracts. ARM runs
under QEMU; i686 runs with its matching sysroot loader. Neither is a hardware
performance measurement. `storage_portability.py` independently checks the
filesystem backend with native compilers and 32/64-bit file-offset modes.
Run both through the same cgroup launcher. Their artifacts belong in the SDK
`.tmp`; they do not change SDK bindings.

The [semantic catalogue](interface-catalogue.json) deliberately distinguishes
registered local imports, a tested storage backend and designed provider
operations. `check_catalogue.py --corpus CORPUS --output RECEIPT` validates names,
wire shapes and pinned source evidence. A successful inventory check does not
qualify native ROS providers, devices, models or whole upstream packages.

The local scheduling callbacks have an explicit sixteen-entry floating-point
control policy in LLVM-enabled builds. Each selected native wrapper constructs
the runtime's native control-restoration guard before caller identification or
environment enrollment, retaining it through result delivery. Allocation can
invoke a process `std::new_handler` that changes FP controls; inspecting only
the normal C++ arithmetic is insufficient. Unsupported architectures and
interpreter-only builds retain the full outer guard. New callbacks remain
conservative by default; ABI discovery and capability open/close retain the
full host FP guard. Qualify this policy with the core and actual-Wasm regression:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 test/0084.ros-runtime/run_scene_fp.py \
  --mode all --runtime RELEASE_VM --output OUTPUT
```

The core test varies rounding, FTZ/DAZ, exception masks and x87 precision across
48 profiles, covering all sixteen operations, refused authority, short outputs,
failed transactional sinks, timeouts, cancellation and injected allocation
failure. Clang ASan/UBSan and GCC are checked. Sticky exception flags are
intentionally outside the preservation contract. The Wasm fixtures load FP
operands from guest memory and check positive/negative halfway rounding,
subnormal input/output, infinity and NaN immediately after each host call in
the same function. Interpreter and optimized LLVM execute success, denial and
missing-grant cases for memory32/64 and memory index zero/one. These tests
qualify local scheduling, not arbitrary native ROS/model/provider callbacks.
The raw broker core test does not establish wrapper behavior during real
allocator failure. `wrapper-allocator-fp/results.json` separately checks the
production libc++ allocation path with a control-changing handler that throws,
and one that frees reserved memory and returns for a successful retry. The
wrapper guard is present for all sixteen scheduling and seven output entries;
artifact reads and lifecycle entries retain their conservative outer guard.

The checked host lends its synchronous output callback with `std::ref`; it is
never retained after the call, including a blocking wait. Qualify allocation
and lifetime behavior separately:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 test/0084.ros-runtime/run_borrowed_sink.py \
  --runtime-directory RELEASE_BUILD_DIRECTORY --output OUTPUT
```

This checks the default and production libc++ builds with ASan/UBSan and stack
lifetime detection, GCC portability, and blocking wake/cancel/revoke under
TSan. Fault injection proves the original four-reference owning callback
allocates, while borrowed conversion and fixed-output operations complete with
all new allocations refused. Failed or throwing sinks must preserve guard and
timer events. Wait result vectors still allocate bounded storage; this is not
an allocation-free claim for the entire wait path. TSan's lifetime-only build
uses TSan's allocator instead of the fault-injection overrides.

## Official native comparison and VTune

The [optional Intel JIT registration design](README.intel-jit-profile.md)
contains independently qualified lifecycle helpers and a controlled generated
code probe. They are not yet connected to the VM guest source-mapping pipeline.

Choose a fresh output directory for each implementation being measured. Prepare
builds both implementations with the same container GCC using `-O3 -g
-fno-omit-frame-pointer`, preserves their exact source snapshots and hashes the
native ROS library closure. Native uses the installed official ROS Jazzy `rcl`
and Fast DDS implementation. The scene executable calls the C++ broker.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/performance.py prepare --output OUTPUT
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/performance.py baseline --output OUTPUT
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/performance.py profile --output OUTPUT
```

Baseline uses three rounds with alternating provider order, the same selected
CPU, workloads and iteration counts. Each iteration completes a clock sample,
a trigger/observe/consume guard cycle, or both. Native `rcl_wait` consumes its
guard notification; scene `wait.poll` observes readiness and `guard.take`
consumes it. Scene additionally enforces its resource authority. Batch-mean
percentiles measure throughput variation, not individual request tail latency.

The profiler defaults to the official installation at
`/opt/intel/oneapi/vtune/latest/bin64/vtune`. It records hardware Hotspots and
detailed Microarchitecture Exploration, plus summary, function and source-line
reports. `-no-analyze-system` restricts analysis to the target process tree;
VTune can still report a driverless system-wide PMU collector for detailed
microarchitecture sampling. Preserve the reported collector type. No sysctl or
PMU security setting is changed. Existing VTune result directories are never
overwritten.

Before/after PMU metrics are comparable only with the same workload and CPU.
Source paths refer to the saved snapshot, not a later edited checkout. Inlined
code and native libraries without debug source can retain unknown source rows;
do not invent their source attribution.

These small local-resource workloads do not execute Wasm, exercise native topic
transport or qualify complete robot performance. Their ratios identify broker
costs. Native parity still requires optimized VM builds and equivalent real ROS
workflows, including message sizes, QoS, executor behavior and CPU contention.

## Actual Wasm imports

`wasm_performance.py` and `performance_guest.c` perform the same clock and guard
cycles through real Wasm32 imports. Preparation compiles nine freestanding
guests and executes no VM. The execution modes require a runtime directory with
the saved `main.default.cpp.o.argv.json`, `uwvm_runtime.default.cpp.o.argv.json`
and `host_api.default.cpp.o.argv.json`; each must select `-O3`. The executable,
guest and native-library hashes are recorded and checked.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/wasm_performance.py prepare
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/wasm_performance.py smoke --runtime RELEASE_VM
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/wasm_performance.py baseline --runtime RELEASE_VM
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/wasm_performance.py profile --runtime RELEASE_VM
```

By default, output lives in
`../uwvm2-ros-sdk/.tmp/ros-scene-security/performance-wasm`. Native is taken from
the recorded `performance-typed` preparation; `--native-directory` can select
another complete native preparation. The native program uses exactly the
guest's iteration and batch counts. Baseline alternates the provider order
across three rounds; it never compares a batched scene call with individual
native operations.

The guest has an independent explicit WASIp1 policy allowing only monotonic
timestamps and stdout reporting. Scene rights remain in their separate exact
resource environment. Timestamp calls occur twice per batch, and stdout occurs
once after the timing region. Timer-pair calibration is reported without
subtracting it. VM memory checks and default security behavior remain enabled.

Guest timestamps exclude startup and LLVM compilation. Process wall time minus
the measured interval also includes initialization, warmup, calibration, output
and teardown; it is not an isolated compilation measurement. Whole-process
VTune captures include those phases and are labeled accordingly. Hotspot source
attribution for the compiled VM does not establish source attribution for
dynamically generated guest code. The runner keeps guest JIT source mapping
unqualified until actual profiler evidence demonstrates it.

Use `compare_wasm_builds.py --before OLD_VM --after NEW_VM
--prepared-directory FIXTURES --output OUTPUT` through the same cgroup launcher
for a paired optimization comparison. It checks both release build receipts and
alternates the two VMs with native `rcl`, using identical guest bytes and loop
counts. Reserve the selected CPU and its reported SMT sibling during collection;
affinity on one logical CPU alone does not exclude competition on its physical
core. Keep noisy historical runs and label their limits instead of overwriting
them or selecting only the fastest round. VTune function/file/line reports can
be joined to saved source snapshots if its source-text renderer cannot resolve
relative DWARF paths; unknown or ambiguous source rows remain unattributed.

The 2026-11-14 paired qualification is saved under SDK
`.tmp/ros-scene-security/performance-wasm-paired-fp/results.json`. Both recorded
VMs use `-O3`; old `vm-release` has SHA-256
`e13f96503a413fe10390b8c3f83ab078181bdfcd9de46301651865bdab3a333f`, and new
`vm-local-fp` has SHA-256
`06456f97494f893748bea0828e404fab6f1846f14eb39e5aaff6864a9c9872a8`.
Three alternating rounds used the same guest bytes, 20,000 iterations per batch,
101 batches, CPU 2 with SMT sibling 3 reserved, LLVM `max`, one compile thread,
disabled JIT disk cache and the same default memory checks and explicit scene
grants. The median of each round's batch p50 was: clock 309.048 → 211.057 ns
(native 12.142 ns); guard 920.356 → 587.966 ns (native 38.994 ns); mixed
1,225.272 → 821.684 ns (native 47.190 ns). This reduces these local Wasm costs
by 32–36%, while leaving a 15–17× native gap. It does not establish native parity.
The separate C++ broker benchmark's approximately 3× mixed ratio omits the Wasm
boundary and must not be substituted for the real-Wasm comparison.

Reproduce the paired comparison with a fresh output directory:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  taskset -c 2 /usr/bin/python3 tools/ros_scene/compare_wasm_builds.py \
  --before ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-release/uwvm2-ros \
  --after ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-local-fp/uwvm2-ros \
  --prepared-directory ../uwvm2-ros-sdk/.tmp/ros-scene-security/performance-wasm-local-fp \
  --output FRESH_OUTPUT
```

The earlier unpaired new-build baseline remains in `performance-wasm-local-fp`
and shows substantial shared-core interference: native mixed rounds changed
from approximately 84 ns to 48 ns. Its raw medians are not the before/after
optimization result. That directory also preserves four successful official
VTune collections (VM/native × hardware Hotspots/detailed Microarchitecture
Exploration). Profiler overhead and contention differ between analyses; use
their samples to locate costs, not as interchangeable wall-time measurements.
The new Hotspots report has no samples in `fegetenv`/`fesetenv`; absence of
samples does not prove zero calls. Remaining sampled costs include memory
descriptor construction/resolution, mutexes, caller identity and the common
host dispatcher. Stack gaps and cycles prevent exact inclusive cost attribution.

Before/after `source-line-reports.json`, `lines-*.stdout` and `lines-*.json` are
in `performance-wasm-profile` and `performance-wasm-local-fp`. The JSON joins
VTune's function/file/line samples to preserved source text with SHA-256 hashes;
the original source-text renderer failure remains recorded separately. This
qualifies host source locations only: `guest_jit_source_mapping_qualified`
remains false. The paired, FP and profiler receipts record the same existing
cgroup (inode 12495), `memory.max=68719476736`, `memory.swap.max=0`, and unchanged
zero memory-limit/OOM events. FP receipts are `fp-local/all.json` for host
Clang/GCC and `fp-local/wasm.json` for the final 24 real-Wasm cases; runtime and
source hashes are retained with each run.

The subsequent borrowed-callback build `vm-local-callback` has SHA-256
`4c1769f0b71073e9a7b291f2756cb73822a91ddce3ab9de85d8e2dff1c591434`.
`performance-wasm-paired-callback/results.json` repeats the same three-round
comparison against original `vm-release`, using the identical prepared guests
and CPU reservation: clock 309.805 → 204.636 ns (native 11.654 ns), guard
890.577 → 576.478 ns (native 39.685 ns), mixed 1,213.968 → 790.873 ns (native
45.815 ns). Combined improvement is 34–35%; the remaining native ratios are
17.56×, 14.53× and 17.26×. Reproduce it with the preceding paired command,
changing `--after` to `vm-local-callback/uwvm2-ros` and choosing a fresh output.
The callback-specific increment is not isolated by comparing these medians
with the earlier run at a different time.

Final callback allocation/lifetime checks are recorded in
`borrowed-sink-final/results.json`; final 24-case Wasm FP execution is in
`fp-local-callback/wasm.json`. All passed with stable recorded source/runtime
hashes and zero memory-limit/OOM events in the same gate. The four VTune
collections above belong to the earlier `vm-local-fp` binary, not this final
hash; their exact source snapshots and sample interpretation remain separate.

Final-hash official VTune qualification is separately complete in
`performance-wasm-callback-profile/profile.json`: VM/native hardware Hotspots
and detailed Microarchitecture Exploration each execute 30,300,000 mixed
cycles. Its `source-line-reports.json` and `lines-*.json` provide host
function/file/line attribution against saved snapshots for this exact final
binary. Scene source hashes match the final build receipt. Additional runtime
snapshots are contemporaneously hashed; DWARF identifies their files but does
not contain a content checksum, as recorded in
`runtime-source-snapshot-notes.json`. Hotspots uses per-process hardware
sampling; detailed microarchitecture collection reports system-wide hardware
sampling with target-process analysis. Guest JIT source mapping remains
unqualified. The final VM's principal sampled costs remain memory descriptor
construction/resolution and mutexes; all four collections completed without
memory-limit/OOM events in the existing 64 GiB gate.

## Immutable artifact guest and upstream parser qualification

`test/0084.ros-runtime/run_scene_artifacts.py` exercises the independent
`artifact.open`, `artifact.read` and `artifact.describe` guest records. Its
matrix runs interpreter and LLVM AOT with memory32/memory64 and memory index
zero/one, covering exact generation/digest identity, cross-alias and cross-kind
handles, transfer/offset bounds, malformed output ranges, empty/partial reads,
resource capacity and persistent call attenuation. The CLI cases use an
inotify open/access witness: policy checking and insufficient retained input
budgets must cause no source I/O; an authorized launch provides the positive
control. Digest/length mismatches, symlinks and nonregular inputs must fail.
The `validation` and `section-details` modes additionally check valid inputs,
wrong content digests and absent native sources without input activation;
invalid retained budgets still fail before I/O. `--static-only` records this
entire twelve-case matrix before returning failure, preserving both modes'
regression evidence when testing an older binary.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  taskset -c 8-15 /usr/bin/python3 test/0084.ros-runtime/run_scene_artifacts.py \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-artifacts-safe/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/artifact-guest-FRESH
```

Use a fresh output directory. `--prepare-only` builds Wasm fixtures without
executing the VM; its receipt is explicitly marked `mode: prepare` and is not
a guest execution result. Execute only against a successful build receipt for
the artifact-enabled VM, rather than an executable copied from an earlier
build. Result receipts bind guest, source and runtime hashes to the existing
64 GiB, zero-swap cgroup and retain before/after memory events.

`upstream_artifact_portability.py` extracts unmodified TinyXML2 from the pinned
BehaviorTree.CPP 4.11.1 release acquisition. It parses five actual upstream XML
files and two explicit malformed/empty inputs through four bounded read sizes
(1, 7, 31 and 64 bytes), for 28 parser cases. Native and ASan/UBSan native output
must match; interpreter and LLVM AOT must reproduce the same parse errors,
element/attribute/text/include counts and content summary. A separate denied
guest verifies that withholding `artifact.open` authority prevents all 28
loads. XML include attributes remain strings: this slice does not resolve
includes, load plugins or execute BehaviorTree nodes.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  taskset -c 8-15 /usr/bin/python3 tools/ros_scene/upstream_artifact_portability.py \
  --mode all \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-artifacts-safe/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/upstream-artifact-xml-FRESH
```

The guest uses exact scene configuration capabilities and a finite retained
snapshot budget. It imports no `path_open`, `fd_read` or socket function and
receives no generic filesystem rights. WASIp1 is independently configured for
stdout reporting; other static libc helper imports are explicitly denied.
The runner hashes the upstream receipt, original parser sources, fixtures,
adapter, binaries and toolchain libraries. `--mode prepare` records native
and compilation evidence only; `--mode execute` revalidates these inputs
before running the prepared guests. This qualifies an immutable-input parser
portability slice, not the full BehaviorTree package or ROS ecosystem.

`upstream_artifact_benchmark.py` measures the same original parser using
preopened artifact handles. Native preloads the exact source files once into
immutable byte snapshots; its measured loop performs the same bounded copies,
parser calls and checksum checks as Wasm. This intentionally provides an
in-memory native baseline. Each cycle parses all seven inputs; native file
initialization, guest compilation, handle opening and output reporting occur
outside batch timing. It is not a comparison against native filesystem reads
or the complete ROS executor.

Prepare separate output directories with `--chunk 64` and `--chunk 4096`.
The former stresses the host boundary; the latter is the recommended batching
case and reads each of these small XML inputs in one call. Requests use the
known remaining input length, with the same chunk choice on both sides.
`baseline` alternates native and Wasm for three rounds on one logical CPU;
reserve its reported SMT sibling as well. The default CPU is 8 (sibling 9 on
the qualification host). All modes must use the cgroup launcher above.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  taskset -c 8-15 /usr/bin/python3 tools/ros_scene/upstream_artifact_benchmark.py prepare \
  --chunk 4096 --prepared COMPLETED_XML_DIRECTORY --output FRESH_BENCHMARK_DIRECTORY
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  taskset -c 8 /usr/bin/python3 tools/ros_scene/upstream_artifact_benchmark.py baseline \
  --prepared COMPLETED_XML_DIRECTORY --output FRESH_BENCHMARK_DIRECTORY --runtime ARTIFACT_VM
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  taskset -c 8 /usr/bin/python3 tools/ros_scene/upstream_artifact_benchmark.py profile \
  --prepared COMPLETED_XML_DIRECTORY --output FRESH_BENCHMARK_DIRECTORY --runtime ARTIFACT_VM
```

Benchmark execution requires a successful real-VM XML qualification and an
unchanged successful three-TU `-O3` build receipt. It independently permits
only monotonic timestamps and stdout through WASIp1; all input authority stays
in exact scene grants. Official Intel VTune profiling runs hardware Hotspots
and detailed Microarchitecture Exploration for both native and Wasm, preserving
summary/function/file/line reports. These whole-process profiles also include
startup and compilation. Host source snapshots are checked against the build
receipt; unknown or guest JIT source rows remain unqualified until actual
source mapping evidence exists. The historical guard/clock profiles above
belong to their recorded VM hashes and cannot qualify this artifact path.

The 2026-11-14 artifact qualification uses VM SHA-256
`16a6d888e5d81d01aebd15c20f3a7937a33aa04caa65394a5ebe2a59406a9926`.
`artifact-guest-safe/results.json` records 24 successful guest executions and
31 successful CLI/input rejection or activation checks. Its no-I/O witnesses
remained empty for inert checks and insufficient budgets, while the authorized
positive control observed input access. Both static modes pass with valid,
wrong-digest and missing native inputs while rejecting invalid budgets without
I/O. `artifact-static-before/results.json` preserves six reproduced failures
against the earlier binary; it is intentionally incomplete, not a successful
qualification. `upstream-artifact-xml-safe/results.json`
records identical native/sanitized-native/interpreter/AOT parser summaries and
both successful denial runs. Each parser run checks 28 cases. These receipts
are under SDK `.tmp/ros-scene-security` and preserve zero memory-limit/OOM
events in cgroup inode 12495.

Three alternating performance rounds on CPU 8 (SMT sibling 9 reserved for this
task) used 31 batches of 2,000 cycles, seven XML inputs totaling 1,601 bytes per
cycle. `upstream-artifact-batched-qualified/baseline.json` records the recommended
4 KiB case: median of round p50 values is 6,433.984 ns native and 13,736.778 ns
Wasm, 2.135×. `upstream-artifact-boundary-qualified/baseline.json` records the
64-byte boundary stress case: 6,546.240 ns native and 27,061.176 ns Wasm,
4.134×. Checksums match
across all rounds. These are distinct read-size workloads, and neither proves
native performance parity or whole-package portability.

`upstream-artifact-batched-qualified/profile.json` completes all four official VTune
collections for this exact VM and the recommended 4 KiB path: native/Wasm ×
hardware Hotspots/detailed Microarchitecture Exploration, each with 310,000
timed cycles. Hotspots is per-process hardware sampling; detailed
microarchitecture sampling is system-wide with target-process analysis.
`source-line-attribution.json` and `source-lines-*.json` join 457/471 VM and
555/590 native file/line rows to exact preserved sources for the two analyses.
The VM source manifest covers all 2,636 frozen source files. Large sample groups
are labeled `[ld.so.cache]` or `[Outside any known module]`; those labels do
not establish guest functions or source lines. Guest JIT source mapping is
explicitly unqualified, and these unresolved samples prevent a complete cost
breakdown. The profiles preserve all raw reports and unchanged zero
memory-limit/OOM events.
The later output qualification found and corrected duplicate `Source File`
columns in the attribution parser. The historical input-workflow line joins
above have not been revalidated with that correction and must not be used as
verified source attribution. Their raw measurements and reports remain intact;
the output workflow below uses the corrected parser and its regression test.

Earlier artifact and performance receipts remain preserved under their
original directory names; final qualification uses only the safe-build hash
and the paths named here.

Reproduce the conservative source join after profiling with
`profile_artifact_sources.py --profile BENCHMARK_DIRECTORY --runtime ARTIFACT_VM`
through the same cgroup launcher. It validates the VM hash, build manifest and
every matched source hash; ambiguous or absent source rows stay unattributed.


Final immutable-input acceptance is indexed by
`../uwvm2-ros-sdk/.tmp/ros-scene-security/artifact-acceptance.json`.
It pins 21 completed component, VM, debugger, portability and profiling receipts,
the final executable SHA256 and the frozen source manifest. All scene model,
CLI, loader and debug serialization sources match that frozen build. It also
records the remaining Linux-only provider, transactional-output, whole-ROS,
production-module-graph and guest-JIT-source-mapping limitations. The dedicated
`--mode validation` / `section-details` regression demonstrates that these
inspection paths cannot provision input snapshots, independently of execution
mode's explicit retained capacity and content identity checks.

## Volatile output qualification

The cache output profile adds seven imports, bringing the registered inventory
to 29; 103 provider operations remain planned. Its [contract](../../documents/runtime/ros-scene-volatile-output.md)
defines finite payload/object/recovery reservations, exact last-request replay,
independent read authority and publication without disk durability. Neither a
cache receipt nor an XML serializer result authorizes a model or actuator.

Use the existing cgroup launcher for each command. All outputs below must be
under `../uwvm2-ros-sdk/.tmp`; SDK production headers remain outside this work.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 0-3 \
  /usr/bin/python3 tools/ros_scene/output_security_checks.py \
  --thread --recipes BUILD_RECIPES --output CORE_OUTPUT
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 0-3 \
  /usr/bin/python3 test/0084.ros-runtime/run_scene_outputs.py \
  --runtime OUTPUT_VM --output VM_OUTPUT
```

The core matrix checks allocation failure, exact replay/conflict/ordering,
object ownership/publication, physical allocation charges, lost result delivery,
retirement, fork refusal, concurrency and optional attenuation. Clang ASan/UBSan,
GCC, AArch64/ARM32 under QEMU and native i686 retain allocation-fault injection.
TSan uses its own allocator and tests concurrency without replacing global
new/delete. The separate `run_scene_output_host.py` fixture exercises the real
host wrapper and broker with controlled runtime memory APIs: insufficient
receipt capacity, exhausted or zero allowance, excessive transfers and denied
authority must fail before a large request allocation or guest read. It includes
a positive allocation-failure control, so an inactive injector cannot pass.

The actual-VM suite checks interpreter/AOT, memory32/64 and memory indexes zero
and one. It covers empty objects, staged-read refusal, published EOF reads,
short responses, bad spans, strict keys, independent aliases, immutable
publication, retained capacity after commit, closing without resetting quotas,
inert static modes and invalid CLI plans. It also tests exact policy refusal
and missing grants. The [debugger suite](../debug/README.scene.md) covers all
output FP wrappers, tagged and normalized memory spans, cached transaction
metadata, pagination, stale stop tokens and independent inspection permissions.

[Upstream output portability](../../documents/runtime/ros-upstream-output-portability.md)
uses unchanged TinyXML2 XMLPrinter from the acquired BehaviorTree.CPP source,
real XML fixtures, UTF-8/escaping and empty content. Correctness includes exact
retry receipts and byte-for-byte native/Wasm readback. Timing excludes those
intentional retry probes but retains begin, append, commit, read and remove.
The native baseline is bounded memory serialization/copying, not a complete
native ROS middleware workload. The benchmark requires a matching completed
VM correctness receipt and a full O3 build receipt before collecting timings
or the four official native/Wasm VTune analyses. Host file/line attribution
requires exact frozen source hashes; unresolved guest JIT rows are not source
mapping evidence.

Final output acceptance is indexed by
`../uwvm2-ros-sdk/.tmp/ros-scene-security/output-acceptance.json`. It pins 27
completed component checks, execution suites, dependency audits, corpus
consistency checks and performance/source-attribution records to VM SHA256
`eb758b5d24c4e755f67f35204248b97d89562a0991fcc2d1549b0cf26ea2d64d`.
The frozen manifest covers 2,638 VM source files. The output change differs
from the preceding immutable-input build in eleven scene/CLI/debug files;
none are legacy WASIp1 or ROS implementation files.

The final run includes 46 output VM/CLI cases, 120 output FP/tag cases,
55 input VM/CLI regressions, 48 input boundary regressions, two live DAP suites
with full/read-only/limited permission, 28 scheduling cases, 8 threaded cases,
24 scheduling FP executions and 40 scheduling tag cases. The isolated module
configurations and real-host preflight fixture have their explicit dependency
scope documented in the debugger README. All memory-limit/OOM counters remain
zero in cgroup inode 12495. The upstream serializer's 56-case correctness
matrix passes, but its measured 4 KiB and 64-byte timing ratios are 3.859x and
6.091x native respectively. Four official VTune analyses and the corrected
source-column regression complete; unresolved JIT rows, near-native performance
and the 103 planned provider operations remain unqualified.

## Managed lifecycle and optimized output qualification

The subsequent frozen safe build registers six local lifecycle imports, for
35 total registered imports and 97 planned operations. Its executable SHA256
is `92354c74047cc872e6b4c6161beafcf0c49f65e2168cbd454a9f8a4743ae0121`.
`../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-acceptance.json` indexes
36 completed build, core, actual VM, debugger, module, upstream XML and
performance receipts against a 2,641-file source manifest. The qualified source
is `lifecycle-source-safe`; concurrent allocator/runtime changes in the live
checkout are preserved and explicitly listed as outside this binary's scope.

[Lifecycle qualification](../../documents/runtime/ros-lifecycle-portability.md)
includes 44 VM/CLI cases, 184 FP/tag/alias boundary cases and three real DAP
permission cases. Deadline expiry is lazy on interface access. Logical
finalization is neither an autonomous watchdog nor a physical safe stop.
The immutable-input, output and scheduling regressions remain passing. The
host preflight test now also checks partial failed reads at 32, 512, 513 and
4,120 bytes: uninitialized scratch cannot be parsed or published, canaries
remain intact and the request key is not consumed. Scheduling FP qualification
uses a fresh GCC scratch directory with explicit transitive headers, so old
container files cannot silently supply missing dependencies.

Output changes use owned stack scratch for small requests, exact indexed
admission and a single registry snapshot/copy for published reads. The selected
native wrappers retain FP control restoration even when a real allocator
handler changes controls. The official VTune and emitted assembly evidence
checks those paths; it does not establish guest JIT source mapping.

Three alternating same-fixture rounds measured 4 KiB output cycles at
69.012 to 61.217 microseconds (11.30% less time), with native at 18.554
microseconds. For 64-byte chunks, 111.696 became 94.736 microseconds (15.18%
less time), with native at 20.160 microseconds. The remaining ratios are
3.299x and 4.699x native. The historical 3.859x baseline is retained; only the
paired rounds support the quoted improvement. Four official VTune collections,
conservative frozen source joins and 31 assembly artifacts identify remaining
lock, authority-selection and checked-memory costs. Near-native performance
and industrial safety qualification remain unmet.

The same configured 64 GiB, zero-swap cgroup path has historical inode 12495
and current inode 12409 after an external recreation. Receipts retain their
own memory counters; the earlier 683 limit-hit count and current zero count
are not a counter decrease within one cgroup. No workload bypasses the gate
and no cross-incarnation event delta is used. Historical incomplete receipts
are retained separately, including the source-drift scheduling-tag run that
was rerun successfully against the frozen source.

## Isolated model proposal core

[Proposal admission](../../documents/runtime/ros-proposal-admission.md) now has
an executable fixed-capacity core, closed tool actions, separate arbiter and
target authority, effect classification, observation/target fencing and exact
retry receipts. Optional rates can be introduced and tightened after stable
authority, without refunding previously consumed allowance. Model producers
cannot approve their own proposals. Admission records remain inert; this core
has no target execution entry and adds no registered imports.

`proposal-admission-observation-final/results.json` records 12 successful compile/
run commands across Clang ASan/UBSan, GCC, TSan, AArch64, ARM32 and i686 in the
same 64 GiB gate, compiled from a preserved source snapshot. These are ten
native core test groups, not VM or physical robot qualification. Explicit
latest-required and bounded-age policies separate new samples from source,
permission, clock and calibration changes. Frame/clock ABA tests ensure that
switching back cannot revive old proposals. Canonical wire codecs, trusted
schema/buffer leases and atomic target-provider admission still need
implementation before `proposal.read/accept` can be registered. The earlier
hash-only and intermediate observation-policy receipts are retained as history;
they are superseded by this snapshot-based final core qualification.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 12-15 \
  /usr/bin/python3 tools/ros_scene/proposal_admission_checks.py \
  --recipes BUILD_RECIPES --output CORE_OUTPUT
```

## Complete-object publication prototype

[The publication proposal](../../documents/runtime/ros-complete-publication-proposal.md)
tests an owned-copy, bounded, atomic publication path with one shared replay
stream. It preserves the existing maximum append framing reservation, checks
all publication/dependency rights atomically, reserves in-flight scratch and
checks a non-callback cancellation token at the commit decision. It does not
add an import, CLI flag or second production output registry.

`complete-publication-prototype-controlled-final/results.json` records six
successful Clang ASan/UBSan, GCC and TSan compile/run commands from saved
prototype snapshots against the frozen production headers. Its fourteen exact
XML payloads require 66 existing-path native adapter calls and 40 proposed-path
calls, with identical readback. This is an algorithm/topology result, not an
actual VM timing improvement. Small transfer limits still require the chunked
path. Runtime memory checks, one shared production mutation stream, compound
authority accounting, CLI/debugger integration and actual VM qualification
remain prerequisites for a supported `artifact.publish` interface.


### Full-debug model carrier and transform foundation

The new model source cohort is `model-source-carrier` (3,439 manifest files),
with seven model/request carriers and explicit local-model scope. Both
`model-modules-{llvm,interpreter}-first` receipts passed all 22 commands;
`model-host-second` passed the real host wrapper fixture with recoverable
fast_io allocation failures. `model-deployment-second` and
`model-debug-contract-first` cover aggregate configuration, native-owner
cleanup ordering and detached console/DAP/IDE state. These are not actual
Wasm model or isolated process worker qualification. The catalogue records
48 registered imports and 85 planned provider operations, and retains those
qualification limits explicitly.

`transform-math-upstream-first` separately qualifies allocation-free geometry
against 20,000 pinned upstream tf2 comparisons under Clang ASan/UBSan and GCC,
with effective `-O3 -g`. Its analytic velocity contract uses a complete explicit
window and a cross product for reference-point shifts. The nine transform
imports remain unregistered until their bounded cache, authority and carrier
integration is complete. See the model and transform provider documents.

### Qualified local transform cohort

The preceding carrier/prototype paragraphs describe their historical receipts.
`transform-acceptance.json` supersedes those implementation counts for the
frozen `transform-source-central` cohort: **57 registered imports, 76 planned**,
3,451 production source files, and 24 separately hashed qualification receipts.
Its VM SHA256 is
`9a816d916ce3589a98c4cda6c9ad04030ff288ae54bb2c8d7bceb97ef11ea8ad`.
All three VM translation units use effective `-O3 -g`, with real debug sections.
Nine transform imports passed 320 actual Wasm/CLI cases and three live DAP
sessions. Buffer, publication and model regressions passed 54, 75 and 31
cases respectively. Native tf2 cache comparison covers 6,001 cases per variant;
the exact frozen registry, policy and deployment tests include ASan/UBSan,
GCC and TSan. Inspection of pending queries never advances their deadlines.

`transform-xml-batched` records an XML cycle of fourteen serializations and
complete cache publication/readback/removal, using 40 hostcalls. The three-round
baseline was 19.045 microseconds native and 56.385 microseconds Wasm (2.961x).
It is neither native ROS parity nor persistent storage. Four official VTune
collections join known host Hotspots to frozen source lines and Microarchitecture
events to actual assembly; `transform-xml-instructions` qualifies five host
functions. About 53% of that Hotspots CPU time remains unregistered JIT PCs.
The Intel JIT listener helper has not been integrated, so those samples have
no qualified guest source attribution.

The existing 64 GiB, zero-swap cgroup remains mandatory. Its current boot
recorded 567 memory-limit pressure events and zero OOM/kill events; earlier
receipts retain their own boot/inode counters. Full source snapshots separate
these results from unrelated concurrent checkout changes. This cohort does
not qualify all ROS packages, physical safe-stop behavior or industrial safety
certification. See [transform provider](../../documents/runtime/ros-transform-provider.md).

### Artifact control-register candidate

`artifact-fp-source` changes only the three artifact wrapper whitelist entries
and their audit comment relative to the transform cohort. Its VM SHA256 is
`c0f1a858f76219595554f21e53802e7dfdd28204f8d0d93d440734abb7934860`.
Core hostile-callback and actual fast_io failure tests, both module modes,
the actual runtime imported-provider guard, 55 artifact VM cases, 48 FP/MTE
cases and the 320-case transform regression passed. XML exact readback also
passed. Publication retains the complete outer FP guard.

`artifact-fp-paired` alternates the prior VM, candidate and identical native
fixture on CPU 8, with its sibling excluded. The medians of round medians are
60.700, 60.217 and 22.041 microseconds. The candidate/prior ratio is 0.992,
within the observed round-to-round variation; this does **not** establish a
repeatable speedup. The candidate/native ratio is 2.732, still above the goal.
Both VM and fixture builds retain full `-O3 -g`. The candidate's separate
official VTune collections are in `artifact-fp-profile`; attribution and
assembly reports are qualified separately, never inferred from this timing.
See [artifact FP boundary](../../documents/runtime/ros-artifact-fp-boundary.md).

### Recording and replay provider core

`recording-store-final` contains nine successful dependency/compile/execution
commands across Clang ASan/UBSan, GCC and TSan. Its copied 399-file closure checks
nine security groups: owner/fork isolation, complete batches, exact retry,
immutable segment SHA256, finite capacity and pins, EOF/seek, failed/throwing
copies, concurrent close/retire and every one of nine actual fast_io allocation
failures, each returning `no_memory` through the silent fast_io path. No hot-path
allocation occurs. `recording-store-cross` separately compiles and executes all
nine groups on AArch64, ARM32 and i686 in nine more commands, with a copied
402-file cross-architecture closure. External MCAP import rejects incorrect
digests and truncation at every byte before publishing a local object. Both
schema definitions and imported payloads become owned immutable storage.

`recording-mcap-batch` adds twelve successful commands after whole-batch append
validation, retaining both ROS1/ROS2 upstream MCAP writer/reader directions.
Every compiled variant uses `-O3 -g`. All commands run in the existing 64 GiB,
zero-swap cgroup and retain the current inode/events independently of older
boots. These provider-core receipts register no guest import and make no
persistence, native ROS performance or industrial certification claim. See
[recording portability](../../documents/runtime/ros-recording-portability.md).

### Corrected log/diagnostic frozen VM

`log-retirement-qualification.json` fixes 26 receipt hashes for the corrected
3,457-file cohort. Its VM SHA256 is
`468e1bcf9eeea2ba57af01a40d78147a52506bc5b8afdc2cd7f6a99f22cb9254`.
The three full translation units use actual `-O3 -g`. The log/diagnostic suite
passed 111 Wasm/CLI cases and three DAP sessions; the broader registered-provider
regressions total 959 actual VM/DAP cases. Both module modes, hostile memory/FP
callbacks and native host sanitizer fixtures also passed. These results do not
qualify unrelated current checkout changes or the remaining planned providers.

The fresh three-round XML comparison measured native 26.643 and Wasm 79.661
microseconds (2.9899x) with 7–9 unrelated compilers active on other cores. System
observations and each round are preserved; this is not an idle-system comparison
or a demonstrated performance change from older boots. Four official VTune
collections produced 44 actual assembly artifacts and five instruction/event
maps. Unregistered JIT PCs account for 53.1489% of sampled Hotspots CPU time.
Implausible uarch CPU-time-derived frequency/utilization numbers are explicitly
excluded. Cgroup pressure events are reported per receipt and cannot all be
attributed to this VM; OOM and OOM-kill counts remained zero. Near-native ROS
performance and industrial safety certification remain unqualified.

## Recording, receive-only sensor and model-proposal integration

The catalogue now records 73 concrete imports and 64 planned operations.
Registration of the new seven recording/replay, three sensor and four proposal
carriers is separate from whole-VM qualification. No native device execution,
generic ROS sensor codec, disk durability or industrial certification is claimed.

`recording-sensor-proposal-source` retains 3536 files from the earlier qualified
runtime, updating only the scene implementation and its host/CLI registration.
The full source manifest is
`8b97d7ff23c472aee145f6f21a29b7a20c3c562d3c25e0a35bda50974fa42445`.
Unrelated live runtime changes, WASIp1 and ROS v1 are excluded from this delta.
The VM build uses the recorded production recipes with effective `-O3 -g` and
verifies full DWARF sections and unchanged source hashes. New execution receipts
are pending until that binary completes its matrices.

Retained current evidence under SDK `.tmp/ros-scene-security`:

- `sensor-proposal-deployment-fifth/results.json`: nine commands, five security
  groups under Clang ASan/UBSan, GCC13 and TSan; 464 retained input files unchanged.
  Actual nonblocking pipes check static no-I/O validation, missing mandatory
  limits, receive direction, zero authority and explicit zero rate, one bounded
  native read per call, failed-copy recovery, independent export rights, cached
  debugger observation and revocation. Malloc-family failure injection checks
  that decoded hex and definition-receipt allocation fail recoverably and permit
  an exact retry. Earlier failed test-fixture attempts remain retained separately.
- `recording-cli-first/results.json` and `sensor-proposal-cli-first/results.json`:
  actual production CLI callbacks compile and execute with `-O3 -g` and ASan/UBSan,
  using the frozen source tree. They cover the six recording/replay and twelve
  sensor/proposal options, exact profiles, finite capacities, malformed/duplicate
  values, default-zero exposure and late-setting rejection. These are not VM
  execution or physical-provider qualification.
- `recording-sensor-proposal-allocators.json`: all 122 scene/host/CLI headers pass
  the direct default-allocation spelling check. This complements actual failure
  injection; it is not a proof about every external library or allocator template.
- `recording-sensor-proposal-catalogue-final.json`: concrete registration and
  canonical schemas agree with the operation headers; seven source evidence
  records retain their hashes. This check executes no providers.

`run_scene_recording.py` prepares 108 Wasm fixtures; `run_scene_sensors.py`
prepares 60. Preparation is not execution evidence. Their actual interpreter/AOT
runs cover memory32/64 and memory indices 0/1, full FP behavior, typed memory-tag
faults and range/alias failures. The sensor driver additionally retains the
native pipe byte counts, proving denied/export-disabled/rate-zero paths did not
consume data. Both drivers require the existing 64 GiB zero-swap gate and a
complete frozen VM build receipt.

All new recording/sensor/proposal wrappers retain the full outer FP guard.
The isolated named-module/header fixtures now explicitly check these fourteen
specializations in both LLVM and interpreter policy modes. No additional
wrapper has been admitted to the native-control fast-path whitelist.

Build-recipe correction: the first `vm-recording-sensor-proposal` compile was
stopped and remains incomplete because the inherited recipe still selected
workspace vendor headers. `build_frozen_vm.py` now remaps those paths to the
frozen vendor directory as well as remapping VM source paths. The replacement
cohort is `vm-recording-sensor-proposal-frozen-vendor`. Older full-source
manifests alone do not prove that old compiler invocations selected their
copied fast_io headers; consult their exact argv and dependency receipts. This
correction does not retroactively qualify those vendor selections.

## Placement, output completion and deprecated entry points

The current catalogue has 73 registered imports and 75 planned operations.
Earlier counts above describe their retained historical cohorts. The six-group,
26-domain [robot capability contract](../../documents/runtime/ros-robot-capabilities.md)
also defines the boundaries for physical effects, isolation, IPC latency,
CPU/SMT/NUMA placement and deployment recovery. Neither the catalogue nor the
downloaded package inventory is a completed ecosystem portability review.

`output-portable-regressions/results.json` pins eight passing actual-VM drivers
for the 3537-file output-completion cohort: 642 VM boundary/CLI/provider cases
and the staged/publication XML correctness suites. Its VM hash is
`7f3c804237982d023c56b5c79e78e38d1a2e2550a9bee00f001069e026835bf3`.
`legacy-warning-native-gcc-clean/results.json` separately covers 160 cases,
including actual native-library constructor probes. ROS v1 enablement uses the
`deprecated-ros` category. Unisolated native preload uses `untrusted-dl`;
promotion to fatal is checked on every attempted load, including after the
one-time warning was already emitted.

`output-completion-native-final` and `output-completion-cross-portable` qualify
the optimized output broker's seven safety groups, real fast_io allocation
failure, unsupported-platform branch and AArch64/ARMv7/i686 execution.
`output-portable-paired` reports an 8.48% reduction for the retained five-round
comparison, still 3.269268 times native. Source joins and actual instruction
sampling are described in the
[output portability report](../../documents/runtime/ros-upstream-output-portability.md).

`configuration-portable-owned` records twenty passing configuration commands
after the ARM32 by-value call-frame failure was reproduced and removed from
log binding. `configuration-arm-unwind-proof` retains the failing GDB stack
observations, before/after disassembly and the exact passing compiler arguments.
`log-diagnostic-owned-policy` verifies that binding does not retain caller plan
storage, including sanitizer runs. All compilers, executions and profilers use
the existing 64 GiB, zero-swap cgroup. Pressure-event counts are retained; zero
OOM kills must not be reported as zero memory-pressure events.

`output-unwind-qualification.json` pins the subsequent configuration-recovery
VM (`1c7a9888808e4a26e9b93126cb2e84e57fbdb423945be743ac3a4919df0eac00`)
and 25 completed receipts. The actual VM matrices total 642 cases, with separate
staged/publication XML checks, twenty configuration compile/execution commands,
native allocation/authority checks and four official VTune collections.
The final recording matrix and offline instruction reports completed in fresh
`*-resumed` directories after a machine interruption. Original incomplete
receipts remain incomplete. Each receipt retains its own cgroup identity; all
use the same required 64 GiB, zero-swap limits.

The completed instruction export covers five functions, 1261 instructions and
482 sampled instruction locations. Current staged and complete-publication
measurements are 3.245468 and 2.747318 times native respectively; their workloads
and comparison methods are documented separately in the output report. This
cohort does not finish the 75 planned operations, prove native equivalence for
the 124 deprecated imports, qualify every package, or establish industrial
safety certification. The aggregate records those limits explicitly.

`authority-reduction-qualification.json` pins the subsequent frozen VM
(`fc79b37e3423f2055c9a74f9a4717dfa60015d6c077078f3c1b89cb5a78fff9a`),
its 3538-file source manifest and ten completed receipts. It covers 60 dedicated
revocation cases, 46 output-transaction regressions, 75 complete-publication
regressions and three live DAP permission sessions. Native coverage includes
recoverable allocation failure, exact replay, zero traffic limits, callback
failure, reentrant revocation, concurrency, fork refusal and AArch64/ARM32/i686
core execution. The isolated LLVM and interpreter module exports use a separate
manifest whose only difference is global-fragment headers in `scene/impl.cppm`.
Local notification completion is not native shutdown or physical-stop evidence.
The current catalogue contains 74 registered imports and 74 planned operations.

## Current-call communication and isolated observations

`communication-managed-admission-complete`,
`communication-mutation-dispatch-locks`,
`communication-native-completion-identity`,
`communication-mutation-policy-core`,
`communication-creation-exact-groups`, and
`action-queued-transition-admission` each retain nine passing core commands with
Clang ASan/UBSan, GCC 13 and TSan under `-O3 -g`. These qualify reservation-first
charging, exact complete authority groups, typed deferred mutations, real numeric
completion status and single consumption of creation charges. Actual worker
request/goal identities remain separate from host identities. The failed
`communication-mutation-dispatch-first` TSan lock-order attempt is retained;
the corrected encoder configuration uses nonblocking acquisition.

`graph-current-watch-identity` and `graph-worker-query-bounded` each retain nine
passing core commands. The former adds exact current graph caller/capability,
original node/context and explicit context-description origins, with separate
watch-incarnation fencing. The latter routes the actual graph dispatcher through
the owned isolated query adapter over real nonblocking pipes. It checks single
capture charging, zero quota, exhausted-budget result pagination, failed-output
recovery, stable native enumeration order and six hostile graph reply classes.
`event-current-observation-first` retains the nine-command event dispatcher
regression after introducing per-call trusted observation callbacks.

`communication-owned-queries-first` retains nine passing core commands for the
closed nonblocking query class. `communication-query-allocation-first` exhausts
five actual recoverable fast_io allocation points in both ASan/UBSan and GCC
builds. The mutation allocation cohort is
`communication-mutation-allocation`. Each receipt pins its source closure;
subsequent watch-token refinements are qualified by the later graph cohorts,
not retroactively attributed to earlier binaries.

`communication-typed-queries-native` and `communication-typed-mutations-native`
each compile a fresh host fixture and execute the two sealed ROS 2 workers from
`communication-private-result-status`. They pass sixteen asynchronous opens and
bidirectional String/Bool topics, SetBool services and LookupTransform actions.
Typed queries add per-call complete admission and detached bounded result copy.
The publication handles are fixture handles; native endpoint adoption into the
VM registry, generic IDL and public deployment integration remain outstanding.
These additions do not change the 74 registered/74 planned catalogue count.
All listed receipts use the existing 64 GiB, zero-swap cgroup and record their
own before/after memory events; none is a near-native performance or industrial
safety certification claim.

Later core cohorts remain distinct from that frozen VM. The
`parameter-search-authority` and `parameter-search-policy-final` receipts cover
independent exists/search grants, bounded namespace traversal, negative results,
allocation failure, source revocation and definition-hash coverage. Definitions
include explicit `search_root` and `search_candidates`; ROS2 cannot enable ROS1
search. These core tests do not qualify parameter imports or a native bridge.

`action-current-call-origins-acyclic` and `action-current-call-allocation` cover
current caller/capability propagation, complete node/context/schema origins,
independent transition charges and non-cyclic retained provenance. Earlier
failed schema, fixture-policy and ownership-cycle attempts remain incomplete.
`communication-current-origin-metadata-final` and
`communication-native-endpoint-evidence` cover optional actual endpoint
incarnation/GID evidence, including real native publisher GID comparison and
loopback network tracing. `service-description-core` and
`service-description-native` add independent service description and separately
observed request/response QoS legs. These communication headers still require
the public asynchronous-creation, host, CLI and debugger integration before
catalogue registration. All compiler and native runs use `-O3 -g` in the same
64 GiB, zero-swap gate; no new performance claim is derived from correctness runs.

The subsequent `communication-factory-graph-checked` receipt qualifies explicit
aggregate graph capacity, real registry node leases, exact graph attachment,
nonblocking isolated queries and original-context capability fencing. Its five
commands include real-worker Clang ASan/UBSan and GCC runs. The
`communication-result-deferred`, `communication-result-creation` and
`communication-result-mutations` receipts each contain nine successful commands
for contradictory native outcomes, exact recovery and retained uncertainty.

`communication-delivery-registry-complete` contains nine successful commands for
the persistent original-record queue, registry ownership, canonical six-operation
receive carriers, failed copies, expiry cleanup, exact settlement and concurrency.
`communication-native-receive-reservations-bounded` contains seven successful
commands, including a rebuilt native backend and actual two-worker topic/service
reservation flows in both directions, followed by retained original action tests.
The source, runtime images and memory events are recorded per receipt. Failed
compiler/fixture attempts remain incomplete. Later source changes are not silently
covered by these historical results. VM registration remains 74 of 148; this
provider work does not promote an unintegrated interface or establish near-native
performance, package-wide portability or an industrial safety certification.

The owned receiver can be qualified with
`communication_deferred_checks.py --suite receives --thread`; canonical dispatch
has a separate `--suite receive_dispatch`. Both runners retain compiler-derived
source snapshots and use `-O3 -g`. `communication-owned-receives-authority-final`
and `communication-receive-dispatch-checked` each pass nine commands. The earlier
Clang code-generation crash and reproduction files remain recorded in the
incomplete `communication-owned-receives-cleanup` directory; its unchanged source
passed as `communication-owned-receives-cleanup-retry`.

`communication_jobs_ros2_checks.py --suite receives` uses a verified existing
two-worker pack receipt. `communication-owned-receives-native-deadline` passes
three commands with stable source/runtime hashes. Each worker exercises 12
decode recoveries, 12 releases and 12 acknowledgements with original String,
Bool and SetBool records. This host adapter has explicit bounded storage,
current origin checks and separate native/host settlement keys. These results
do not qualify VM handles, public asynchronous opens or native service
correlation mapping. All runs remain in the existing 64 GiB, zero-swap cgroup.

The subsequent `communication-service-correlations-first` cohort passes nine
commands for bounded native/host service correlation translation.
`communication-owned-receives-native-correlations` passes three actual two-worker
commands using that mapping. `communication-receive-dispatch-fenced` and
`communication-delivery-readiness-final` each pass nine commands for canonical
topic/service delivery, unknown-outcome fencing and pinned-record readiness.
These receipts supersede no historical source hashes and register no VM import.

Run `communication_deferred_checks.py --suite receiver_adapter --thread` for the
token-aware endpoint adapter, canonical dispatcher and private-pipe integration.
The adapter reserves a complete detached payload in addition to its RPC storage;
the isolated queue is the sole charging point. This suite includes failed-copy
recovery, exact known/unknown settlement receipts, service correlation mapping,
reentrant callbacks, parent cleanup, zero traffic allowance, fork and concurrency.
`communication-receiver-adapter-nonblocking` passes all nine commands. Its earlier
TSan integration attempts detected lock-order cycles and remain failed receipts;
the fix makes configuration and occupancy checks nonblocking while registry
locks may be held. The actual-native `--suite receives` harness now exercises the
production endpoint adapter and both service roles; VM qualification is separate.

`communication_creation_checks.py --suite adoption --thread` qualifies opaque
registry reservations and typed asynchronous topic/service adoption. The
`communication-typed-adoption-checked` receipt has nine passing commands.
`--suite worker_openings` qualifies the production creation supervisor, including
exact retry and positive-cleanup requirements. Its `communication-open-supervisor-first`
receipt also has nine passing commands. `communication-open-receive-supervisor-native`
records three passing actual-worker commands combining that supervisor with the
owned receiver and service correlation mapping. Native fixture publication in
that cohort is explicitly distinct from VM publication.

`communication_creation_checks.py --suite topic_endpoint --thread` composes the
registry with the production typed topic facade and real nonblocking pipes.
`communication-typed-topic-checked` has nine passing commands for bounded queued
publication, QoS observations, acknowledgement polling, exact completion,
zero quota, original-capability closure, malformed receipt retention and fork.
Receipts remain bound to their retained source hashes; subsequent factory changes
are qualified separately. Every compiler invocation uses `-O3 -g` in the existing
64 GiB, zero-swap cgroup.

`communication_worker_provider_checks.py --suite topics --pair-receipt PATH`
uses a verified existing sealed ROS runtime pack. It exercises factory-owned
native creation, actual host node leases, distinct host resource adoption,
native DDS String transport, QoS evidence, failed-copy recovery and ACK. It does
not build or qualify the public Wasm carrier. The default `--suite provider`
continues to cover bilateral activation, graph queries and worker teardown.

`communication_worker_provider_checks.py --suite services --pair-receipt PATH`
qualifies the typed service facade against the real sealed Jazzy/FastDDS pair.
It exchanges an actual SetBool request and response, verifies distinct host and
native correlations, requires explicit ACK before a server response and tests
failed guest-copy recovery and schema revocation. The source-pinned
`communication-typed-services-native-carrier` cohort passes five commands; it
precedes the later explicit-close changes and is not a VM execution receipt.

`communication_creation_checks.py --suite service_endpoint --thread` exercises
the actual registry, the production facade and nonblocking pipes. The
`communication-typed-service-unknown-receipt` cohort passes nine commands and
preserves error/unknown receipts rather than dropping them as pre-effect errors.
Use `--suite service_allocation` for exhaustive actual fast_io allocation failure
injection. The `communication-typed-service-allocation` cohort passes six commands
and covers 24 allocation points in each role before the additional close queue.
`communication_deferred_checks.py --suite correlations --thread` additionally
checks bounded local-interest cancellation without claiming remote cancellation.

Use `communication_creation_checks.py --suite closure --thread` for concurrent
call exclusion, exact native-close evidence, zero traffic allowance, fenced
parent cleanup, primary-capability revocation, malformed/unknown outcomes and
fork rejection. All compilation remains `-O3 -g`, and each runner verifies the
existing 64 GiB, zero-swap cgroup before compilation and execution. Native tests
serialize only their fixed private-port use; compiler work may proceed separately.

Use `communication_creation_checks.py --suite close_dispatch --thread` to test
the public dispatcher against the production close queue. The
`communication-public-close-current` receipt has nine passing commands for
independent cleanup authority, exhausted traffic, closed parents/schemas,
concurrent call pins, primary revocation and native identity mismatch.

The `communication-typed-topics-lifetime-reuse` and
`communication-typed-services-lifetime-reuse` cohorts each have five passing
actual-worker commands. They require positive native-close evidence before
creation reconciliation, retain capacity until host buffers are destroyed, and
reopen with fresh resource identities. Repeated unsent cancellation also returns
only its reserved preparation capacity. The allocation-lifetime cohorts cover
18 publisher, 20 subscription and 27 allocation sites per service role using
the actual recoverable fast_io allocator. Later source edits require new receipts.

`communication_deferred_checks.py --suite deferred --thread` additionally covers
retained Action ingress backpressure. The `communication-action-retained-ingress`
receipt has nine passing commands: retrying a retained event does not invent loss,
and malformed duplicate status bodies are rejected. Mutation receipt generation
must match the transport incarnation. `communication-topic-receipt-generation`
and `communication-service-receipt-generation` each have nine passing commands;
`communication-native-receipt-generation` has five actual SetBool/creation/close
commands. These remain provider/core receipts, not registered VM import evidence.

`communication_creation_checks.py --suite endpoint_adoption --thread` qualifies
the canonical asynchronous Topic/Service draft and publication path. The
`communication-endpoint-adoption-zero` receipt contains nine passing commands
for four roles, independent origins, failed-copy/reentrant retry, exhausted
traffic allowance, parent closure, explicit cleanup, concurrent calls and fork.
`--suite endpoint_allocation` injects every actual draft allocation failure:
three per Topic role and four per Service role. The
`communication-endpoint-draft-allocation-initial` receipt contains six passing
commands with no debit/native effect on allocation failure. Existing synchronous
host dispatch regression is retained in
`communication-dispatch-after-endpoint-adoption` (nine commands).

`communication_worker_provider_checks.py --suite endpoint_dispatch --pair-receipt PATH`
executes both canonical Topic and Service paths against the sealed Jazzy/FastDDS
pair. `communication-native-endpoint-dispatch-capacity` contains five passing
commands for native String/SetBool, asynchronous creation, typed publication,
retained receive output, ACK and confirmed cleanup after schema closure.
Use `--suite action_dispatch` for the equivalent canonical Action path;
`communication-native-action-dispatch-deadline` contains five passing commands
for goal, feedback, cancellation and result. Both fixtures permit two seconds of
DDS discovery and make no latency guarantee. Neither is a VM import receipt.

`communication_creation_checks.py --suite action_adoption --thread` qualifies
opaque Action drafts and exact creation replay; `--suite action_allocation`
injects the six observed allocation sites in each Action role. The
`communication-action-adoption-carrier` and
`communication-action-draft-allocation-current` receipts respectively contain
nine and six passing commands for their retained source snapshots. Use
`--suite open_dispatch --thread` for independently granted creation take,
cancel, cached request description and release; the
`communication-open-controls-current` receipt contains nine passing commands.
All these runners enforce the existing 64 GiB, zero-swap cgroup and `-O3 -g`.

`communication_creation_checks.py --suite node_evidence --thread` tests opaque
registry-node evidence, exact context/worker incarnation, stale-anchor rejection,
weak native lifetime, cleanup debt and concurrent observation. `--suite node_open
--thread` tests the production asynchronous node coordinator, including no launch
at submission and cancellation racing with startup advancement. `--suite
node_allocation` sweeps actual recoverable fast_io allocation failures before
node submission. The `communication-node-integration-*` cohort records the joined
core/dispatcher/real-worker regression before the later release-recovery change.
The native endpoint fixture now uses production Node and Topic/Service
coordinators; it no longer relies on fixture-owned endpoint adoption callbacks.
Native receipts continue to qualify only their exact retained source snapshots,
not VM imports, generic ROSIDL, cross-platform coverage or latency targets.

The graph/event and cancellation-selection cohorts are recorded in
[`ros-graph-event-portability.md`](../../documents/runtime/ros-graph-event-portability.md).
`observation-vm-budget-order` contains 43 actual VM/CLI cases;
`selection-vm-prepared` contains 41 cases against its own frozen binary.
Both selection module modes pass 22 commands. The cancellation set has explicit
owned-goal semantics and per-candidate outcomes; it does not implement native
UUID/time/wildcard cancellation. At that cohort, registration was 126 of 148
catalogue operations, with 22 provider operations still planned. The later robot
binding cohort below supersedes that registration count.

`observation-publication-performance` records a separate complete-publication
XML baseline: native 17.591 microseconds and Wasm 49.029 microseconds per fourteen
documents (median of three round medians), or 2.787 times native. These are
process-local bounded publication/readback measurements, not persistent storage
or a full native ROS application. Both fixtures and the frozen VM use `-O3 -g`.
Four official VTune collections are retained, together with corrected source
line joins, actual host disassembly and fifteen instruction/DWARF report commands
in `observation-publication-microarch`. Sampled addresses are not exact
instruction latency; guest JIT source attribution remains unqualified.

## Complete operation design and robot result contracts

`operation-contracts.json` assigns every one of the 148 catalogue operations an
explicit domain, authority reference and commit/copy/retry/retirement behavior.
`check_operation_design.py --generate` validates the closed 26-domain mapping and
regenerates the professional English
[`ros-operation-reference.md`](../../documents/runtime/ros-operation-reference.md)
and Doxygen `operation_design.h`. Without `--generate`, drift is an error.
`check_catalogue.py` also runs this audit and checks both generated artifacts.
The metadata registers no imports and must not populate callable IDE menus.

`robot-provider-contracts.json` specifies all 22 device/control/media/
bridge/component operations, 62 typed profile fields and 37 closed deployment
types. Normal work and stop/close use independent replay domains and reservations.
Route effects form a complete set, so physical execution, maintenance and export
predicates cannot be discarded by choosing a single supposedly stronger label.
The operational procedures, platform requirements and implementation limits are
in [`ros-robot-provider-procedures.md`](../../documents/runtime/ros-robot-provider-procedures.md).

Run `robot_provider_contract_checks.py --thread --cross --output PATH` through the
existing gate. `robot-design-complete/results.json` contains 16 passing commands:
schema/design generation checks, adversarial design mutations, macro-only header
compilation, Clang ASan/UBSan, GCC, TSan, AArch64, ARM32 and i686. All C++ builds use
`-O3 -g`. The request/result suite covers all 44 shapes, every truncated prefix,
aggregate zero/finite budgets, 128 role slots, plane extent overflow, receipt
state/effect combinations, contradictory evidence, allocator refusal and malformed
input/reply bytes. Cross execution uses the pinned existing sysroots/runtimes.

`robot-design-wire-final` separately passes all canonical structural shapes with
Clang ASan/UBSan and GCC. `robot-design-catalogue-complete.json` joins the current
148-operation design with concrete registration and pinned source evidence.
`robot-design-legacy-complete.json` checks all 124 deprecated imports across eight
modules; at that snapshot, six mappings depended on planned providers. That
snapshot recorded **126 registered / 22 planned**. These results qualify design and codecs, not new
native providers, VM guest carriers, physical assurance, all packages or native
performance parity. Earlier receipts remain immutable snapshots of earlier inputs.

## Qualified robot bindings and receive-debt recovery

The remaining 22 operations now have concrete registered bindings: five device,
five control, four media, four bridge and four component operations. The current
catalogue reports **148 registered / 0 planned**. Their deployment profiles,
independent authority, bounded resources, recovery rules, CPU/NUMA placement and
supported native semantics are specified in
[`ros-robot-bindings.md`](../../documents/runtime/ros-robot-bindings.md).

The final cohort uses `vm-robot-component-debt/uwvm2-ros`, SHA256
`3d510640a4cd04fcb73d0f752c7bc537e8b078b7bd312c88412b257fe1cd9c6d`,
with 3582 frozen source files. It retains `-O3 -g` for all three VM translation
units and for both performance fixtures. All compilers, guests, native workers,
sanitizers and profilers remain inside the existing 64 GiB, zero-swap cgroup.
Concurrent workspace changes outside the retained source closure are not
implicitly qualified.

The completed final VM cohort comprises:

- `robot-final-local`: 626 cases, including all 22 operations' tag boundaries,
  four memory layouts, two execution engines, zero authority and positive local
  robot workflows.
- `robot-final-process`: 41 cases for isolated component lifecycle and real typed
  ROS 2 topic forwarding, including static reservation refusals.
- `robot-final-component-authority`: 23 cases for actual
  VM/component/native-worker topic exchange, retained receive acknowledgement,
  confirmed cleanup, FP preservation and three independent denied grants.
- `robot-final-dap`: three real permission modes with closed provider DTOs,
  privacy, pagination and nonmutating inspection.
- `robot-final-cli`: 33 static configuration cases, including all mandatory
  component worker reservations, profile hashes and placement rejection.

The total is 726 VM/CLI/DAP checks: 654 guest executions, three debugger sessions
and 69 static checks. `robot-final-modules-llvm` and
`robot-final-modules-interpreter` each pass 22 commands against the isolated
scene graph. Provider core receipts cover Clang ASan/UBSan, GCC, TSan and
recoverable allocation refusal; codec checks also execute AArch64, ARM32 and
i686 binaries. `robot-final-catalogue.json`, `robot-final-design.json` and
`robot-final-legacy-frozen.json` audit the exact registration, 26-domain design
and 124 deprecated imports across eight frozen modules. All replacement targets
are registered; native semantic equivalence is a separate claim.

A timeout is not evidence of an empty asynchronous receive. Both bridge and
component dispatch require the separately authorized, side-effect-free debt
inspection before settling a timed-out receive as known empty. Otherwise they
retain the original uncertainty and cleanup responsibility. Component handle
closure retires the handle without erasing that binding state.
`robot-component-timeout-retirement` passes eight compiler/run commands across
five real-child safety groups. `robot-component-timeout-regression` substitutes
only the earlier dispatcher header and requires the new test to fail, preserving
the actual regression proof. The older `robot-22-acceptance.json` remains intact
and is explicitly superseded by `robot-22-acceptance-superseded.json`.

The implemented bindings qualify framed scalar PTY/pipe devices, one controller
incarnation per device, bounded raw mono/RGB/BGR conversion, dedicated typed
ROS 2 topics and sealed Linux message-role components. They do not establish
generic drivers or ROSIDL, ROS 1 conversion, Windows/macOS isolation, physical
robot stopping, all-package portability or industrial certification.

`robot-final-acceptance.json` joins 25 verification receipts to the exact final
binary and manifest, verifies unchanged source intersections and the matching
94-command CLI reference, and checks that the shared cgroup limits remained
64 GiB with zero swap and no increase in OOM counters. It includes the controlled
old-header regression as negative evidence, not as a successful qualification
of that older implementation.

`robot-final-xml` passes the upstream output correctness matrix before timing.
`robot-final-performance/baseline.json` measures 18.657803 microseconds native
and 51.956371 microseconds Wasm per fourteen-document complete-publication cycle,
or **2.784699 times native**, using three alternating rounds and 31 batches of
1,000 cycles. Four official VTune hardware collections retain both native and
Wasm Hotspots/Microarchitecture results. The exact source join preserves 831
of 3911 Wasm Hotspots rows; 53.2651% of sampled self CPU time remains in unknown
module locations. `robot-final-assembly` retains 73 actual disassembly artifacts.
`robot-final-microarch` completes fifteen report/resolver commands for five
functions, joining 1261 instructions, 478 sampled instruction locations and
1082 source-qualified locations. See the
[output portability report](../../documents/runtime/ros-upstream-output-portability.md)
for addresses, source frames and measurement limits. This XML result does not
qualify robot IPC latency or meet the near-native target.

## Observation and launch-cleanup qualification (2026-11-17)

[Module observation](../../documents/runtime/ros-module-observation.md) records
compile-time canonical VM labels, differential bounds/erasure tests and paired
native/Wasm evidence. `vm.describe` improves from 671.50 to 376.95 ns in the
retained A/B/B/A cohort; this is not a performance qualification of every import.
The four final hardware Hotspots/Microarchitecture profiles include source and
assembly evidence at a requested 0.1 ms, with 240,000 reference-cycle periods.

`module_exchange_native_compare.py` supplies a production-code native carrier
reference, including admission, routing, FP controls, byte-atomic copies and
full erasure. The native fixture omits actual VM identity/declaration/MTE
machinery. See [message qualification](../../documents/runtime/ros-module-exchange-performance.md)
for exact hashes, timings and eight hardware profiles. Run its `baseline` and
`profile` modes separately from compilation, simulation and each other.

`component_launch_failure_checks.py` uses real seccomp refusals for pidfd
acquisition, launch-gate send, both failures and pre-fork descriptor setup. It
accepts a corrected `--source-root`, immutable `--reference-root`, successful
`--component-receipt` and fresh `--output`. Its reference must fail all four
checks, while corrected Clang ASan/UBSan and GCC runs must pass. All invocations
use `in_existing_cgroup.py` and the unchanged 64 GiB, zero-swap gate.
`run_scene_component_crash.py` also offers `launch-pidfd`, `launch-gate` and
`launch-both` for actual Wasm/DAP validation. Its test-only wrapper preserves
the debugger parent's pidfd and authenticated nonblocking channel, while
refusing the new component's acquisition/send. No injection is in the product.

The [driver lifecycle contract](../../documents/runtime/ros-native-driver-lifecycle.md)
distinguishes pre-spawn refusal from forked-child cleanup. These tests do not
qualify arbitrary original vendor ABIs, durable restart/update or a physical
safety stop. The [source review ledger](../ros_ecosystem/review-cases.json)
adds concrete UR effectful-configuration and Franka automatic-recovery cases;
those limited source reviews remain unexecuted vendor acceptance requirements.

### Communication launch authorization and cleanup

`communication_worker_launch_failure_checks.py` installs real per-process kernel
refusals after sealing. The reference snapshot must fail pidfd, authorization,
combined and nonblocking failure checks; Clang ASan/UBSan and GCC must pass the
corrected implementation. Pre-launch cancellation is a positive control. Retain
failed test attempts separately. Never edit a frozen hardlinked source in place.

`test/0084.ros-runtime/run_scene_communication_launch_failure.py` executes the
actual VM with an inherited test-only seccomp refusal. It covers interpreter/AOT,
wasm32/64 and memory indices zero/one. The runner adopts orphaned descendants and
requires none after each VM exits. Creation take cannot publish a node; cached
request descriptions conservatively retain unknown outcomes. Do not confuse
confirmed process cleanup with proof of physical outcome.

Use the existing cgroup launcher for both runners. Communication VM suites use
CPUs 4–7; the basic sandbox suite uses CPUs 12–15. Normal communication fixtures
must explicitly supply every aggregate and peer budget, including zero graph,
event and action reservations when those capabilities are unused. Missing
configuration is a refusal, not an implicit unlimited/default grant.


### Historical namespace-identity qualification

`component_identity_failure_checks.py` separately tests missing namespace identity
and cancellation before fork / between fork and pidfd publication. It takes the
same source, reference, component-image and fresh-output arguments as the launch
failure runner. The test owns independent pidfds solely to clean its fixtures;
those descriptors never become product cleanup evidence. The actual-VM
`run_scene_component_crash.py --fault namespace-identity` variant uses an external
seccomp notification supervisor rather than a library loaded into the VM.

In the earlier launcher, an authorized launcher being reaped does not prove its
namespace child exited. That supervisor retains cleanup debt, the first failure, retained
bytes and `outcome_unknown` when namespace identity is unavailable. It refuses
replacement. The installed bubblewrap 0.11.0 reads `--block-fd` before installing
the final parent-death signal; killing only its launcher cannot prove cleanup.
The birth-domain implementation below supersedes this startup ownership gap.
General vendor execution additionally requires its own image, resource,
device/network and recovery qualification.

`component-identity-fenced-qualification.json` binds 779 actual-VM cases to
`d62905bcd7d580897dbf7b846421920d9737c93fac3faedf9d530d46da3e49ed`.
The aggregate includes 340 exposure/refusal cases, 128 bounded scalar MuJoCo
cases and four representative component fault modes. It separately references
four source-core receipts, including the newer communication parent-setup fault
suite. These counts do not establish complete provider semantics or vendor
qualification. `assemble_runtime_qualification.py` checks each receipt's VM
hash, individual case outcomes, 64 GiB/zero-swap membership and recorded memory
events; it rejects unbound/mixed VM evidence and missing event fields.

The current DDS success fixture uses the backend pinned by
`communication-native-receive-reservations-bounded/results.json` (backend SHA256
`0822ab3828459fc41d0459c6e86e3e3367930962f6bcc9649c20dd2f4baeafda`).
The older `communication-private-ros2-fifth` image predates receive reservations
and is not a valid provider for those tests. Preserve failures from mismatched
inputs; do not relabel them as a completed semantic check.

### Atomically owned worker process domains

`worker_born_domain_checks.py` tests real model/component and DDS launchers with
launcher death, denial of subsequent pidfd acquisition, cancellation between
creation and ownership publication, and refused clone3. Negative controls use
the release-only implementation. Corrected Clang, GCC and ASan/UBSan receipts
record all expected outcomes and unchanged caller signal masks.

`worker_domain_exec_checks.py` additionally tests immediate exec, nested and
daemonized descendants, owner death, inherited SIGCHLD-ignore, denied gates and
stale mapping identities. The mapping negative control fails against the
initial birth-domain helper lacking its pidfd liveness check. Independent
observer descriptors never enter the production launcher. The 1 GiB inherited
reservation check verifies that the executing worker does not retain VM memory.

`run_scene_component_crash.py --fault domain-pin`, `domain-launcher` and
`domain-release` inject actual kernel refusals or process death from an external
seccomp notification supervisor. Successful reports require the whole observed
tree dead before fixture rescue. Use `--expect-no-created-process` with the new
launcher's `launch-pidfd` and `launch-both` cases: failed owner pinning happens
before creation, so zero members and no observed exit are the correct states.
The `launch-gate` case still requires real child cleanup and an observed exit.

`worker-born-domain-pinned-qualification.json` binds 823 VM cases and three
separate native-core receipts to
`d865f87e3e759a7710f6e878450f9bdbf388c20f623a8ae96278901ad09fa039`.
It includes 88 component fault/launch cases, 340 import exposure/refusal cases,
128 bounded scalar MuJoCo cases, 37 models, 62 DDS, 24 DDS launch failures,
82 module security, four module DAP and 58 observation/FP cases. All recorded
memory-event counters remain unchanged. Native-core receipts are not added to
the VM-case count. Historical receipts retain their own source/runtime identity.

`worker_owner_death_checks.py` qualifies the actual model/component and DDS
launchers at release-report, ready and attached boundaries. It compiles with
Clang, GCC and ASan/UBSan using `-O3 -g -UNDEBUG -Werror`, then kills the real
owner. Independent root/worker pidfds must show exit before fixture rescue.
`worker-born-domain-owner-death-outlined/results.json` retains 18 passing cases,
six compilation commands, pinned sources/images and unchanged cgroup events.
This is additional native-core lifecycle evidence, not physical-stop proof.

The follow-up `worker-born-domain-pinned-extended-qualification.json` validates
2,914 actual VM cases from 30 exact-runtime receipts and four separately
identified native-core receipts. It adds 2,091 closed-provider, permission and
carrier cases to the earlier 823-case cohort. No receipt with a failed command
or mismatched runtime digest enters that aggregate. This is neither an
ecosystem-wide semantic review nor an industrial safety certification.

`deployment_exchange_preflight.cpp` and
`module_security_checks.py --suite deployment_exchange_preflight --thread`
qualify the prepared admission fast path. Warm calls use exact indexed
authority checks; cold calls preserve fallible enrollment for explicitly
configured group members. The suite checks allocation rollback, cross-group
handles, retired epochs, exposure/action combinations, attenuation, cleanup
and post-preflight revocation under GCC, ASan/UBSan and TSan.

`module-exchange-enrollment-runner-results.json` retains 612 actual-VM cases
for runtime `d5f4e39ac1278bd0c5fd46281f1b9008a5fecfa6c61df23d9ff26c3084178e25`.
Its A/B/B/A and native-reference timings remain separate from profiled runs.
`module_exchange_native_compare.py perf-import` requires a same-host
`--reference-context`, validates explicit PMU sample periods independently
for native and Wasm, and imports retained traces into VTune. Repeated
`--assembly-function` options replace default source/assembly selectors in both
hardware-import and full analysis modes. The accepted 24,000-reference-tick
receipts qualify the nominal 10 us target without modifying cgroup limits.
The runner pins its own CPU affinity and passes the executable directly to
the profiler, avoiding an intermediate target launcher. Interrupted traces
with missing module mappings remain failed evidence and must be recollected.

`assembly_compare.py` resolves each selected symbol's address and size before
disassembling it. Run it through `in_existing_cgroup.py` with `--before`,
`--after`, `--toolchain`, one or more `--function` substrings and a fresh
`--output` directory. Every selector must match both executables, and every
function must contain ordered instructions beginning at its exact entry. The
receipt pins both binaries, tools and complete original disassembly. An empty
successful objdump output cannot establish equality. Normalization preserves
constants, field offsets and unresolved targets; only symbol-resolved branches
and RIP-relative relocations are normalized. Matching text does not establish
whole-program equivalence or explain a timing difference.
`test/0084.ros-runtime/test_assembly_compare.py` checks these rejection rules.

## Transactional component preparation and original library checks

`component_prepare_checks.py` injects actual supervisor preparation failures
against frozen reference/candidate sources. It verifies both stack guards and
pthread attribute/setstack/create boundaries across three bindings, immediate
joins without state locks, terminal refusal of reuse, and concurrent retirement.
All builds use `-O3 -g`; the scoped run covers Clang, GCC, ASan/UBSan and TSan.
Run it through `in_existing_cgroup.py` with `--source-root`, `--reference-root`,
`--component-receipt` and a new SDK `.tmp` output directory.

`test/0084.ros-runtime/run_scene_component_crash.py` additionally accepts
`--original-realman-receipt` for its completion, controlled-stop, call and
constructor cases. The helper verifies an unchanged vendor ELF and exact ABI
headers against the retained upstream archive, inspects dependencies without
executing them, and links only an external metadata test adapter. The adapter
calls `rm_api_version`; intentional faults belong to that adapter. The VM never
loads the library. This closed fixture is not a general vendor resolver or
robot-motion qualification. The [lifecycle evidence](../../documents/runtime/ros-native-driver-lifecycle.md#transactional-component-supervisor-preparation)
records exact hashes, case scopes and failed fixture attempts.

## Finite worker ceilings

`worker_resource_limit_checks.py` validates startup resource-limit conversion
and exact kernel readback using frozen source roots. Run it through
`in_existing_cgroup.py` with `--source-root FROZEN_CANDIDATE`,
`--reference-root FROZEN_REFERENCE`, `--model-images MODEL_IMAGES`,
`--communication-images DDS_IMAGES` and a fresh `--output OUTPUT`. It runs
serial optimized Clang/GCC and ASan/UBSan cases, actual model inference and
directional DDS pipe regressions. Six old-source negative controls must reject
the new assertions only after their children have been cleaned up.

The actual VM component runner also accepts `--fault resource-readback`.
Its external seccomp supervisor refuses only the born child's RLIMIT_NOFILE
getter, leaving the VM and the setter intact. Wasm and DAP must retain startup
failure and actual exit; a fixture rescue is never accepted as product cleanup.
This is a bounded Linux fixture, not arbitrary vendor or physical-stop evidence.
