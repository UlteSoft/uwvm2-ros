# Upstream XML output portability slice

The test uses the unmodified TinyXML2 implementation shipped in the ROS release
archive `ros2-gbp/behaviortree_cpp_v4-release`, ref `upstream/4.11.1`. The acquired
archive is pinned by SHA256
`c9fabad593809e9179f4ca5a21ebf08a73ec4ec97997f240d13795f145592a47`.
Source, acquisition receipt, compiler/library hashes, commands and generated
fixtures are retained in the SDK `.tmp` qualification directory.

Five real BehaviorTree XML files are embedded as data. Two explicit boundary
fixtures add escaping/UTF-8 and an empty XML document. The same upstream
`XMLDocument::Print` / `XMLPrinter` implementation serializes each document in
pretty and compact modes. A native reference emits the exact fourteen byte
sequences, and the runner generates their expected lengths, hashes and SHA256
records. The terminating C-string NUL is excluded from artifact content.

The execution fixture transports every result with chunk sizes 1, 7, 64 and
4096, for 56 complete transactions. Native uses bounded staged memory and an
explicit readback copy. Wasm uses one exact `cache` grant with binding
`volatile-output`; it begins, appends, commits, reads back and removes one
object at a time. The full byte vectors must match before native/Wasm summary
output is compared. Every mutation replay must return the same retained
receipt, and a receipt query must not acknowledge it. Metadata checks include
operation/sequence, object ownership, accepted/total/retained bytes and
publication state. `durable` must remain false. Empty output still exercises
begin, commit, status and remove and consumes an object slot until removed.

The test grants one object, 65536 retained payload bytes and a 4096-byte maximum
transfer. The independent receipt reservation is 4192 bytes: the maximum
request header plus payload (24 + 4096), and the fixed 72-byte recovery receipt.
The current receipt is bounded and replaced only by an accepted next sequence;
56 transactions do not require 56 permanent receipt slots. Denial builds keep
capability opening available but deny `artifact.begin` exposure, verifying
that configuration alone cannot bypass the entry's explicit authority check.

Input XML paths and `include` attributes never become guest file operations.
The guest imports no `path_open`, `fd_read` or sockets. WASIp1 is used only for
explicit stdout reporting, with its other linked libc helper imports denied.
The benchmark additionally grants the monotonic clock for batch timing.

This qualifies serialization and a volatile cache workflow. It does not run a
BehaviorTree, load its plugins/includes, publish ROS messages, validate a robot
configuration schema, grant actuator authority, persist a file, or establish
crash durability. These are separate providers and separate qualifications.

## Reproduction

Every command must run inside the existing shared 64 GiB, zero-swap cgroup:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 8-15 \
  /usr/bin/python3 tools/ros_scene/upstream_artifact_output.py --mode prepare \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-final-prepared

/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 8-15 \
  /usr/bin/python3 tools/ros_scene/upstream_artifact_output.py --mode execute \
  --runtime /absolute/path/to/qualified/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-final-prepared
```

Preparation alone records `preparation_complete=true`, while `complete` remains
false until both actual VM engines and both deny cases pass. Native and
Clang ASan/UBSan outputs must match first. Execution checks the frozen fixture
and toolchain hashes before and after running; it records the actual VM hash.

`tools/ros_scene/upstream_artifact_output_benchmark.py` adds a separate O3
benchmark with `prepare`, `baseline` and `profile` modes. It requires the exact
VM that passed correctness and a complete O3 build receipt. Each cycle
serializes all fourteen variants and performs the complete volatile
begin/append/commit/read/remove workflow. Intentional retry, receipt-query,
status and EOF stress is excluded from the timed path. Publication remains
volatile; this is an in-memory native comparison, not a disk-I/O baseline.

Use `--chunk 4096` for the recommended bounded batch path and `--chunk 64` for
boundary pressure. Baseline mode alternates native/Wasm for three rounds and
reports the median of per-round batch medians. Time excludes VM compilation,
initialization and output reporting. Official Intel VTune hardware Hotspots
and detailed microarchitecture collection cover the complete process and
produce separate native/Wasm source reports. Unknown guest JIT samples remain
unattributed; successful collection alone does not establish guest source
mapping or near-native performance. Keep the benchmark CPU and its SMT sibling
free of other assigned workloads, and retain the exact source manifest used by
the VM build for source-line attribution.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 8-15 \
  /usr/bin/python3 tools/ros_scene/upstream_artifact_output_benchmark.py prepare \
  --prepared ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-final-prepared \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-batched --chunk 4096

/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 8-15 \
  /usr/bin/python3 tools/ros_scene/upstream_artifact_output_benchmark.py baseline \
  --prepared ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-final-prepared \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-batched \
  --runtime /absolute/path/to/qualified/uwvm2-ros

/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 8-15 \
  /usr/bin/python3 tools/ros_scene/upstream_artifact_output_benchmark.py profile \
  --prepared ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-final-prepared \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-batched \
  --runtime /absolute/path/to/qualified/uwvm2-ros

/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/profile_artifact_sources.py \
  --profile ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-batched \
  --runtime /absolute/path/to/qualified/uwvm2-ros
```

Prepare and baseline the 64-byte boundary case in a separate
`output-xml-boundary` directory, passing `--chunk 64` to preparation. Preparation
directories are immutable; use a fresh directory when source or toolchain
inputs change. Performance execution requires both the correctness receipt and
the complete three-translation-unit O3 build receipt to match the same binary.

The same prepared serializer can be checked across the reviewed AArch64,
ARM32 and i686 native ABIs with `upstream_artifact_output_cross.py`. Its output
must exactly match the x86_64 native reference for all 56 cases. The AArch64
and ARM32 runs use QEMU and the reviewed sysroots; i686 uses the sysroot's native
IA32 loader. All input bytes are embedded, so no host-file access or synthetic
source-root mapping is introduced. These runs qualify ABI-level correctness,
not native hardware timing or complete package portability.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 10-15 \
  /usr/bin/python3 tools/ros_scene/upstream_artifact_output_cross.py \
  --prepared ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-final-prepared \
  --recipes .tmp/gc-allocation-fix-20261113/build \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-native-cross
```

## Qualification receipts

The retained preparation is
`../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-final-prepared/results.json`.
It records fourteen reference outputs and matching native and ASan/UBSan runs
covering all 56 transactions. Preparation is distinct from actual VM
qualification; `complete` is set only after both VM engines and deny cases
pass against the recorded VM binary hash.

`output-xml-native-cross/results.json` records successful AArch64, ARM32 and
i686 builds and executions with all 56 cases exactly matching the prepared
x86_64 native output. The reviewed cross compilers, static libraries, emulator
or loader, input files and commands are hashed in that receipt. The cgroup's
`memory.events` counters, including `max`, `oom` and `oom_kill`, were zero before
and after these cross-ABI runs.

The final output-enabled VM was qualified on 2026-11-14. Its SHA256 is
`eb758b5d24c4e755f67f35204248b97d89562a0991fcc2d1549b0cf26ea2d64d`;
`vm-output-final/full-build-results.json` records all three O3 translation
units, successful linking and unchanged frozen sources. Against that exact
binary, both interpreter and LLVM AOT passed all 56 output transactions, and
both denied-entry executions passed their 56 checks.

For each benchmark cycle, fourteen XML outputs are serialized and transported
through the full begin/append/commit/read/remove sequence. With a 4096-byte
chunk, the median of three round medians was **18.754 microseconds native** and
**72.370 microseconds Wasm**, a **3.859x** elapsed-time ratio. With a 64-byte
chunk, the corresponding measurements were **19.115 microseconds** and
**116.437 microseconds**, a **6.091x** ratio. Each round contains 31 batches of
1000 cycles. Both providers were pinned to CPU 8; the host is shared, not an
exclusive benchmark machine. Compilation and initialization are outside these
batch timings. These measurements do **not** meet the near-native performance
goal, and they compare this bounded native memory adapter, not a ROS executor
or filesystem implementation. The larger bounded transfer reduces the cost
in this workload, but does not justify removing authorization or quota checks.

Official Intel VTune Profiler 2026.4.0 completed four collections: hardware
Hotspots and detailed microarchitecture exploration, each for native and Wasm.
The profile workload uses 31 batches of 5000 cycles and includes process
startup and compilation. Wasm's whole-process P-core top-down percentages were
41.4 retiring, 33.8 front-end bound, 9.7 bad speculation and 15.2 back-end bound;
native's were 42.9, 38.0, 9.4 and 9.7 respectively. They are separate sampled
process runs, not a decomposition of the unprofiled timing difference.

The corrected source join resolved 634 of 3721 Wasm Hotspots rows to preserved
source lines, accounting for 1.386 of 12.538 sampled CPU seconds. Native
resolved 731 of 1108 rows, accounting for 1.719 of 3.093 sampled CPU seconds.
Resolved host sites include preload-memory descriptor resolution and checked
memory-copy bridges; resolved native sites include TinyXML2 character handling
and printer copies. Wasm's `Outside any known module` bucket accounts for
5.827 sampled CPU seconds. It remains unattributed: it cannot be presented as
TinyXML2 guest source, an interface's exact cost, or evidence of complete JIT
source mapping. The detailed microarchitecture source report additionally has
sparse CPU-time samples; its line counts must not be treated as Hotspots time
coverage. Kernel symbols are restricted by the existing host configuration;
the qualification did not alter it.

VTune exports two identically named `Source File` columns. The first identifies
the grouped sampled source line; the second identifies the containing function.
`profile_artifact_sources.py` now preserves the first and renames the second to
`Function Source File`. This avoids attributing an inlined header's line number
to an unrelated line in the function's file. A regression uses unchanged rows
from this real report and deliberately different source contents to verify
the correct column/line pairing, ambiguous-basename rejection and unknown-JIT
rejection. Earlier input-workflow attribution artifacts were not rewritten.

Final receipts beneath `../uwvm2-ros-sdk/.tmp/ros-scene-security/` are:

- `vm-output-final/full-build-results.json`: exact O3 VM identity and frozen
  build-source manifest.
- `output-xml-final-prepared/results.json`: reference/native/sanitized and
  interpreter/AOT/denied correctness, `complete=true`.
- `output-xml-native-cross/results.json`: all three additional native ABIs.
- `output-xml-batched/prepare.json` and `baseline.json`: 4096-byte O3 fixture
  identities and three alternating native/Wasm rounds.
- `output-xml-boundary/prepare.json` and `baseline.json`: 64-byte boundary case.
- `output-xml-batched/profile.json`: four successful official VTune collections
  and their function/file/line report commands.
- `output-xml-batched/source-line-attribution.json` and
  `source-lines-{native,wasm}-{hotspots,uarch-exploration}.json`: exact preserved
  source hashes, corrected source-column semantics and unresolved rows.
- `output-source-column-regression-final/regression-results.json`: four parser
  regression reports using the real duplicate-column example.

All these test/profiler workloads ran in the existing 64 GiB, zero-swap cgroup;
its memory-event counters remained zero. `output-xml-preliminary` is explicitly
marked as an older ABI precheck and is excluded from final qualification and
performance. Complete source mapping, native performance parity and complete
BehaviorTree/ROS package portability remain unqualified.

## Follow-up phase and assembly experiment

The unchanged full-workflow baseline above remains the acceptance comparison.
`upstream_artifact_output_phases.py` adds separate diagnostic loops using the
same frozen compiler flags, VM binary, grants and CPU. It checks the same
fourteen outputs and expected checksums over three alternating rounds, each
with 31 batches of 1000 cycles. Serialization mode measures upstream parsing,
printing and hashing with no artifact calls. Transport mode constructs those
exact bytes before timing, then measures begin/append/commit/read/remove and
readback verification. These components are not additive accounting: allocation,
cache state and lifetime differ from the combined loop.

For the recorded 4096-byte case, serialization measured **18.378 microseconds
native / 26.848 microseconds Wasm (1.461x)**. Transport measured **2.256
microseconds native / 40.897 microseconds Wasm (18.128x)**. The fourteen outputs
total 3005 bytes; twelve are nonempty. Their complete workflow makes 66 artifact
calls at 4096 bytes and 148 at 64 bytes, excluding the once-only capability open.
This identifies call/transaction overhead as the primary actionable gap in this
small-output workload. The native adapter deliberately lacks the VM's authority,
receipt and retirement machinery, so its transport ratio is not an equal-policy
implementation comparison.

`inspect_output_assembly.py` verifies the completed profile's binary identity,
aggregates its samples and disassembles exact host symbols from that executable.
It separately hashes the installed libc/libm used for instruction inspection.
The recorded host facts are:

- `invoke_output<artifact.append>` contains three consecutive
  `preload_memory_normalize_range_host_api` calls, then checked read/write calls
  that resolve descriptors again. `try_build_preload_memory_descriptor` and
  `resolve_preload_memory` account for 0.462303 sampled CPU seconds (3.69%). A
  future per-call checked memory lease could reuse resolution while preserving
  ownership, physical-alias checks and current bounds; cross-call raw-pointer
  caching would not satisfy that contract.
- The imported-call FP boundary calls `fegetenv` and `fesetenv`; together these
  symbols account for 0.822420 seconds (6.56%). The actual libm instructions
  include `fnstenv`, `fldenv`, `stmxcsr` and `ldmxcsr`. The output callbacks are
  absent from the audited FP-preserving whitelist. A narrowly audited local
  cache path could use that existing contract only after validating all return,
  failure and imported-memory-provider paths; arbitrary providers still require
  their complete FP guard.
- `getpid` accounts for 0.439484 seconds (3.51%), and output registry operations
  perform process-identity checks before their locks. Consolidating duplicate
  checks inside one admitted operation is a candidate; caching a PID across
  calls without fork invalidation would incorrectly admit inherited state.
  `deployment::select_exposed` also contains an actual global mutex acquisition
  and three string-comparison call sites. Binding immutable selection to a
  generation-scoped owner would still need current revocation and retirement
  checks before every effect.

These are measured candidates, not claims that those costs have been removed.
Self-sampled percentages are whole-process observations and do not sum to a
predicted speedup. The unresolved JIT bucket remains unresolved.

The optional Intel listener is currently an independent tested helper; no VM
materialization path instantiates it. Setting an Intel collector environment
variable alone therefore cannot name the current guest code. The helper can
report actual relocated text-symbol extents, sufficient for honest guest-symbol
and assembly views, but has no original guest C/C++ line mapping. A future
ROS-v2-only profiling integration must be explicitly host-enabled before
materialization, attach before finalization, and retain the observer with the
actual executable publication until execution drain. It must leave other
imports, normal code generation, authority and default-off behavior unchanged.
The existing debugger's synthetic Wasm-offset lines must not be presented as
original TinyXML2 source lines. See `tools/ros_scene/README.intel-jit-profile.md`
for the code-lifetime and source-mapping requirements.

Reproduce the diagnostics without modifying the original receipts:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 8-15 \
  /usr/bin/python3 tools/ros_scene/upstream_artifact_output_phases.py \
  --baseline ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-batched \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-phases

/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 8-15 \
  /usr/bin/python3 tools/ros_scene/inspect_output_assembly.py \
  --profile ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-xml-batched \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-output-final/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-asm-qualified
```

Both output directories contain `results.json`, exact commands, output and
hashes. Existing directories are not overwritten. All workloads stay in the
same 64 GiB cgroup, and its memory-event counters remained zero.

## Qualified local-lifecycle build and safe output optimization

The follow-up binary is `vm-lifecycle-safe/uwvm2-ros`, SHA256
`92354c74047cc872e6b4c6161beafcf0c49f65e2168cbd454a9f8a4743ae0121`.
Its three O3 translation units use the 2641-file manifest in
`lifecycle-source-safe`; `lifecycle-xml-prepared/results.json` repeats the
actual interpreter/AOT correctness and denied-entry checks against this binary.
Concurrent workspace allocator changes are preserved and are outside this
frozen qualification. The earlier `vm-lifecycle-optimized` experiment is not
an accepted performance result: its initial FP-preserving audit omitted
installed allocation-failure handlers.

The accepted implementation keeps small owned requests and cache readbacks in
a 512-byte per-call scratch buffer, allocates larger buffers without redundant
zero-fill, and validates that the entire checked guest read succeeded before
parsing or delivering any byte. Cache readback resolves the registry once and
copies initialized bytes under its lock into detached storage before invoking
the sink. Exact artifact/output action admission uses indexed bits, with
invalid indices rejected before shifting. Preflight remains uncharged;
effect-time admission still checks current authority and optional limits.

The audited local output and scheduler wrappers now restore native FP control
with an existing architecture-specific guard surrounding the entire wrapper,
including caller enrollment and allocation. An installed `std::new_handler`
can change FP controls whether it throws or frees memory and returns, so
integer-only callback bodies do not justify an unguarded preserving annotation.
Nested imported-memory providers keep their existing protection. Artifact read,
open and describe and the new lifecycle entries retain their complete outer FP
boundary. Unsupported architectures and interpreter-only builds retain the
conservative outer policy. `wrapper-allocator-fp/results.json` records real
allocation-pressure and injected-path checks; this optimization does not alter
the old WASIp1 or ROS-v1 interfaces.

`compare_artifact_output_builds.py` runs the previous qualified VM, the safe VM
and the same native fixture in alternating order on CPU 8. Both VM build and
correctness receipts are checked before timing. The three-round, 31-batch,
1000-cycle shape and full XML workflow are unchanged. The paired 4096-byte run
measured **69.012 microseconds before, 61.217 microseconds after and 18.554
microseconds native**: **11.30% less VM elapsed time**, with an after/native
ratio of **3.299x**. The paired 64-byte run measured **111.696, 94.736 and
20.160 microseconds** respectively: **15.18% less VM elapsed time**, with an
after/native ratio of **4.699x**. These comparisons use one fixture within each
run; subtracting the earlier historical 72.370-microsecond result from a later
run would mix different shared-host conditions. Near-native performance remains
unachieved.

The safe-build diagnostic phases measured serialization at **18.329
microseconds native / 27.410 microseconds Wasm (1.495x)**, and pre-generated
transaction transport at **2.244 / 33.343 microseconds (14.859x)**. The phases
remain nonadditive diagnostics. A separate native-policy executable compiles
the actual frozen scene broker and reuses the guest's transaction sequence,
grants, bounds, receipts and readback checks. It measured **30.330 microseconds**
against **18.790 microseconds** for the paired original native reference
(**1.614x**). It excludes VM caller enrollment, host wrappers and guest-memory
validation. This additional result exposes some policy cost; it neither replaces
the original native reference nor isolates the complete hostcall cost.

The retained receipts under `../uwvm2-ros-sdk/.tmp/ros-scene-security/` are:

- `lifecycle-xml-safe-paired-final/results.json`: complete 4096-byte paired run.
- `lifecycle-xml-safe-paired-boundary/results.json`: complete 64-byte paired run.
- `lifecycle-xml-safe-phases/results.json`: separately compiled diagnostic loops.
- `native-scene-safe-diagnostic/prepare.json` and `run.json`: frozen native
  scene-policy build and paired reference execution.
- `lifecycle-performance-environment.json`: subsequent environment identity
  observation and hashes of those four execution receipts.

The interrupted `lifecycle-xml-safe-paired/results.json` remains incomplete and
is excluded. All new workloads passed the existing cgroup membership and
64 GiB/zero-swap checks. The cgroup path is unchanged, but its observed inode
is now 12409 rather than the earlier 12495; memory counters from these two
incarnations must not be subtracted. The new timing receipts record zero
memory-event counters at both ends. Older build-time `max` events remain in
their own receipts and are not erased by this later observation.

Official VTune again completed hardware Hotspots and detailed microarchitecture
collection for both providers; the new receipt is
`lifecycle-xml-safe-batched/profile.json`. Whole-process P-core top-down shares
were 43.2% retiring, 34.9% front-end bound, 9.3% bad speculation and 12.6%
back-end bound for Wasm; native was 43.5%, 37.5%, 9.2% and 9.7%. These sampled
process profiles include compilation and are distinct from the timed batches.
The corrected source join resolves 639 of 3757 Wasm Hotspots rows, covering
1.236 of 10.647 sampled CPU seconds; native resolves 735 of 1125 rows, covering
1.659 of 3.094 seconds. The Wasm unknown-module bucket remains 5.527 seconds
(51.91%). It is not assigned to a guest function or source file.

`lifecycle-output-asm-safe/results.json` records 31 disassembly artifacts from
the exact profiled binary and separately hashed system libraries. The emitted
append dispatch thunk contains `fnstcw`, `stmxcsr`, `fnstsw`, `fldcw` and
`ldmxcsr`, and the tool verifies its call to the actual append implementation.
This checks the compiler's emitted guard instructions; it is not a claim of
new handwritten assembly. Full `fegetenv`/`fesetenv` remains in the runtime
boundary used by unaudited imports. Its combined Hotspots share is now 1.43%,
compared with 6.56% in the historical profile. Different whole-process runs
cannot turn that percentage change into an exact attribution of speedup.

The remaining measured costs and concrete call sites are:

- Descriptor construction and memory resolution account for 3.78% of sampled
  CPU time. Both `scene_append` and `scene_read` disassemblies retain three
  normalize calls followed by a checked request read; receipt/data delivery
  still uses checked writes. A per-call memory lease would need to preserve
  ownership, physical alias detection, bounds, tags and provider reentrancy.
- `getpid` accounts for 3.42%. `output_bound`, `output_mutate` and
  `output_copy_read` each check process identity before acquiring the registry
  mutex. `output_preflight` and `output_broker` each call `request_size`, and
  mutation subsequently checks its own bound. Removing a check mechanically
  would change independent broker entry validation, short-output behavior or
  cancellation/error ordering. Any consolidation needs a new proof of those
  orderings and must still reject inherited state after fork.
- Mutex/recursive-mutex lock and pthread unlock functions together account
  for 8.74%; `request_size` itself accounts for another 0.85% of self samples.
  The exact indexed authority function still takes its recursive mutex, and
  deployment selection still takes its global mutex. These are current live
  revocation and ownership boundaries, not optional diagnostics.

At that frozen revision, the fourteen-output workload makes 66 artifact calls. A bounded,
purpose-specific complete-object publication operation is a candidate for
reducing transition count without relaxing authority: it would need an exact
grant, a finite complete-payload reservation, full owned copying before any
effect, atomic publication, cancellation rules and an exact retry receipt.
The [complete-publication prototype](ros-complete-publication-proposal.md)
now exercises this algorithm against the same fourteen XML outputs. Its native
adapters make 66 existing-path calls versus 40 proposed-path calls, with exact
readback and separate safety tests. That historical prototype is not VM integration evidence; its call counts
are not a measured VM speedup. The production follow-up is qualified separately
below.
Generic unchecked batching, cross-call raw-pointer caching and skipping
fork/revocation checks are not justified by these measurements.

## Production complete-publication comparison

The measurements in this section are historical: this VM used `-O3` with
`-gline-tables-only`, while its native and Wasm fixtures used `-O3 -g`. They
do not satisfy the subsequently required full `-g` VM build. A fresh full-debug
cohort is measured separately; source-line evidence below remains evidence for
the identified historical binary, not proof of the new build requirement.

The production `artifact.publish` adapter now executes the same unmodified
TinyXML2 corpus through the actual VM, including exact retry and receipt probes
in the correctness suite. `publication-xml-prepared-first/results.json` and
`publication-staged-xml-prepared/results.json` each pass 56 checks under both
interpreter and LLVM, plus denied-action paths. Their native ASan/UBSan controls
also pass. Both bind VM SHA256
`606a8f2e1faf54c05af677e09fceecdcbab09d8335c0d08f1c1fe7830e7c15c7`.

`compare_artifact_publication.py` alternates native, old staged and publication
executables in three orders, pinned to CPU8 with its sibling CPU9 reserved.
Each round has 31 batches of 1000 fourteen-document cycles. The two separately
built native controls have identical `.text` and `.rodata` hashes despite their
different debug source paths. All exact XML expectations and runtime hashes
match; initialization, compilation and reporting remain outside batch timing.

- `publication-xml-paired-batched/results.json`: native 18.671099 microseconds,
  staged 60.630019, publication 52.892442. Publication is 12.76% faster than the
  staged path on this VM, and 2.832851 times the native reference. The exercised
  source-controlled topology is 40 artifact calls instead of 66 per cycle,
  excluding the one-time capability open.
- `publication-xml-paired-boundary/results.json`: native 20.043269 microseconds,
  staged 98.936722, publication 74.294044. Publication is 24.91% faster than the
  staged path, and 3.706683 times native. Readback uses 64-byte chunks; complete
  publication still requires the explicit 4096-byte transfer allowance and
  bounded scratch, whereas the staged control appends in 64-byte chunks.

Both comparisons use the same frozen production binary and preserve the old
API's independent permissions. They do not establish near-native performance
or full native ROS equivalence. The [VM security qualification](ros-buffer-portability.md)
covers the new carrier, CLI, debug, FP and memory-protection boundaries.

The new `publication-xml-benchmark-batched/profile.json` contains four completed
hardware Hotspots/detailed Microarchitecture Exploration collections for VM and
native. `source-line-attribution.json` joins 805 of 3912 VM Hotspots rows to
preserved source bytes (0.875965 of 9.888744 sampled CPU seconds). Attribution
retains the first source-file column paired with the sampled line. For example,
`broker.h:307` is the actual `output_store.publish(...)` call, `outputs.h:109`
is the process-identity check, and `byte_buffer.h:36` is the strict allocator
path. The native profile joins 713 of 1096 rows, or 1.754919 of 3.201333 CPU
seconds. Unknown VM locations account for 53.49% of sampled CPU; they are not
assigned to guest C++ source or inferred XML functions. Guest JIT source mapping
remains unqualified.

`publication-assembly-final/results.json` preserves 36 actual VM/system-library
machine-code artifacts, including the publication carrier, scratch reservation,
registry commit, checked read and concrete FP boundaries. Two earlier assembly
attempts remain incomplete because symbol selection first also selected a new
callback trampoline, then excluded the legitimate templated FP boundary. The
final selector distinguishes those cases; it does not substitute a guessed
address. New whole-process self samples include `fegetenv`/`fesetenv` at a
combined 3.09%, `getpid` at 2.80%, and mutex lock/unlock work. These percentages
are separate measurements, not a decomposition of the paired timing gain.

`publication-instruction-events-first/results.json` goes beyond a function
ranking: fifteen successful report/resolver commands associate five real
functions with VTune instruction addresses, assembly, sampled hardware events,
and independently resolved frozen inline DWARF frames. It retains full
Microarchitecture Exploration `hotspots` and `hw-events` CSVs. Across those functions, 1214 machine instructions include 459 sampled
locations and 1051 locations with at least one source frame verified against
the frozen manifest. These counts describe only the five selected functions. Representative
addresses in the frozen VM are:

- `0x158c3e0`, `cmpq %rcx, 0x8(%rax)`, maps to `outputs.h:376`, checking the
  complete request's declared payload size. VTune reports
  `CPU_CLK_UNHALTED.THREAD=12000000` and `INST_RETIRED.ANY=2400000` at that location.
- `0x158bb56`, `movupsx 0xf0(%rsp), %xmm0`, maps through inline frames to
  `broker.h:307`, the checked publication invocation. Its reported thread-cycle
  and retired-instruction estimates are 28800000 and 2400000.
- `0x19be230`, `movupsx 0x48(%rsp), %xmm0`, maps to
  `uwvm_runtime.default.cpp:6211`, copying the resolved memory descriptor to its
  caller. Its reported thread-cycle and retired-instruction estimates are
  148800000 and 40800000. The preceding actual call at `0x19be14a` maps to
  `uwvm_runtime.default.cpp:6186`, invoking `resolve_preload_memory`.

These are sampled/scaled event estimates, with possible instruction-pointer
skid and event multiplexing. They are neither exact per-instruction execution
counts nor instruction latency. Inline source text is included only when the
resolved file matches the frozen manifest; system headers and unknown/JIT
locations stay unqualified. This makes the C++ and assembly evidence reviewable
without weakening ownership, authority, revocation or fork checks to improve a
headline ratio.


## Full debug performance cohort

The later required build is now qualified separately. VM SHA256
`38f6a04afe0dfef932915540ed4789fa4107c2284390debe2abcaaef1b76eee6`
uses effective `-O3 -g` for all three translation units. All four native/Wasm
benchmark builds per readback size also use effective `-O3 -g`.
`performance_build_policy.py` audits the recorded actual compile arguments,
rejects later optimization/debug downgrades, line-only debug, opaque forwarded
options and response files, and records the VM argv file hashes. Sixteen
positive/negative policy cases pass in `performance-full-debug-policy-final`.
The 3434-file source manifest is the separately preserved module-fixed cohort;
no live workspace parameter/model additions enter these measurements.

Fresh correctness receipts `publication-full-debug-xml-prepared/results.json`
and `publication-full-debug-staged-prepared/results.json` again pass the actual
interpreter/LLVM XML checks and denied paths. The fresh paired measurements use
CPU 8, three alternating orders and 31 batches of 1000 cycles, in the existing
64 GiB, zero-swap cgroup:

- `publication-full-debug-paired-batched/results.json`: native 24.249866 us,
  staged 78.149981 us, publication 70.643638 us. Publication is 9.605% faster
  than staged and **2.913156 times native** with 4096-byte readback.
- `publication-full-debug-paired-boundary/results.json`: native 24.117207 us,
  staged 127.511364 us, publication 87.824490 us. Publication is 31.124% faster
  than staged and **3.641570 times native** with 64-byte readback. The approved
  complete-publication transfer remains 4096 bytes, as in the prior comparison.

The native machine text/constants and XML outputs match within each pair.
Absolute timings differ from the historical cohort and must not be interpreted
as an isolated effect of debug information. The algorithm removes actual
transaction boundary calls, but the near-native target remains unmet.

`publication-full-debug-guarded-batched/profile.json` contains all four official
hardware Hotspots/Microarchitecture collections for the new VM/native pair.
Its source-line join resolves 807 of 3958 VM Hotspots rows against exact frozen
source hashes. Actual sampled C++ examples include `broker.h:307`, the checked
`output_store.publish` invocation (0.007937 self CPU seconds),
`authority.h:227`, the caller membership check (0.010913), and `outputs.h:109`,
the process identity check (0.005952 in one containing function). These are
source-line observations, not a function-name-only attribution.

`publication-full-debug-assembly/results.json` retains 36 actual host/system
machine-code artifacts. `publication-full-debug-instruction-events/results.json`
contains 15 successful sequential VTune/addr2line reports over five host
functions: 1214 instructions, 463 sampled instruction locations and 1029
locations joined to exact frozen C++ inline source. Concrete instruction/counter
examples from that same binary are:

- `0x158ab20`, `cmpq %rcx, 0x8(%rax)`, joins `outputs.h:376`, the complete
  publication size check; sampled `CPU_CLK_UNHALTED.THREAD` estimate 9600000.
- `0x158a25c`, `movq 0x70(%rdi), %r9`, joins `broker.h:307`, the publication
  invocation; sampled `INST_RETIRED.ANY` estimate 4800000.
- `0x19be3f0`, VTune spelling `movupsx 0x48(%rsp), %xmm0`, joins
  `uwvm_runtime.default.cpp:6211`, the checked memory descriptor copy;
  sampled `CPU_CLK_UNHALTED.THREAD` 151200000 and `INST_RETIRED.ANY` 45600000.

These counter values are scaled sampling estimates, subject to event
multiplexing and instruction-pointer skid; they are not exact per-instruction
execution counts or latency measurements. Unresolved module/JIT PCs still
account for 54.39% of VM Hotspots self time. Guest JIT source mapping remains
unqualified. Identified residual host costs include PID checks (3.14%), mutex
unlock/lock (3.02%/2.64%), full FP environment save/restore (3.45% together),
and memory descriptor/resolve (1.73%/1.11%). Removing process, authority,
revocation or FP checks is not justified by these percentages.

## Output completion admission

The later placement cohort was profiled before changing the output broker.
`placement-xml-batched/profile.json` contains four official native/VM hardware
Hotspots and Microarchitecture collections. The source join preserves 763 of
3730 VM Hotspots rows; 48.50% of sampled time remains unresolved or in
unregistered JIT code. These samples cannot be attributed to guest XML by
guessing. Identified self-time includes mutex lock/unlock (4.38%/3.52%), process
identity (3.24%), memory descriptors (2.69%) and activation (2.66%).

`placement-microarch-complete-columns/results.json` retains actual VTune
instruction reports for six selected functions, with explicit assembly and
source-line columns. The `__getpid` report includes the actual `syscall`
instruction at `0xffa09`, with 1,382,400,000 sampled/scaled clockticks. This is
an event estimate at a sampled instruction, not an exact latency measurement.

The full-output broker path now relies on the final store operation's existing
process, live-binding and request-size checks. It no longer performs a duplicate
size lookup, mutex acquisition and PID query immediately before that operation.
The host's allocation preflight remains, and short-output calls still validate
the binding and request extent before reporting the required receipt size,
without charging admission or changing state. Exact retry, cancellation and
revocation behavior remain covered by the output security fixture. For direct
full-capacity broker calls, cancellation can take precedence over an oversized
request; neither path commits an effect.

`output-portable-paired/results.json` compares the exact placement and completion
VMs with identical retained XML/native fixtures. Five alternating rounds, each
with 31 batches of 1000 cycles on CPU 8, produced median-of-round medians of
100.976270 us before, 92.413061 us after and 28.267202 us native. The completion
change reduced measured cycle time by 8.48%; the new VM remained **3.269268 times
native**. All VM translation units and both fixtures use effective `-O3 -g`.
The machine and 64 GiB cgroup were shared with other work. Absolute times cannot
be compared with older cohorts as an isolated optimization effect, and the
near-native target remains unmet.

This comparison uses VM SHA256
`7f3c804237982d023c56b5c79e78e38d1a2e2550a9bee00f001069e026835bf3`.
The subsequent ARM32 log-configuration correction has a separate frozen source
and build; its results must not be inferred from this earlier binary hash.

## Configuration-recovery build and instruction evidence

VM SHA256 `1c7a9888808e4a26e9b93126cb2e84e57fbdb423945be743ac3a4919df0eac00`
adds the ARM32-safe log configuration binding. Its 3537-file source manifest is
`e5ae8decf83e36a724829c2f143babeec6e88865f69eef689911c0e71d3560af`.
All three complete translation units use `-O3 -g`; source and external library
hashes remain unchanged through compilation and linking.

`output-unwind-paired/results.json` repeats the five-round staged-output
comparison on identical retained fixtures: before 61.553195 us, after 57.345416
us and native 17.669380 us. The measured reduction is 6.84%, with the new VM
**3.245468 times native**. Separately, `unwind-publication-batched/baseline.json`
measures the complete-publication workflow at 51.839532 us versus native
18.869144 us, or **2.747318 times native**. The latter is a separate three-round
baseline, not an alternating staged/publication comparison. All timings exclude
startup from the measured batches and concern volatile XML storage, not native
ROS end-to-end latency.

The four official hardware collections in `unwind-publication-batched/profile.json`
completed before an interruption. The source join resolves 828 of 3838 VM
Hotspots rows against the exact preserved source. Unknown module/JIT locations
account for 53.0663% of self CPU time and remain unattributed. The assembly
receipt retains 48 actual host/system artifacts. The completed offline export
in `unwind-publication-instructions-resumed` uses those same collected samples
and the same binary: five functions, 1261 instructions, 482 sampled instruction
locations and 1082 locations with a verified frozen inline source frame.

Examples from the Microarchitecture hardware-event export are:

- `0x165915c`, `cmp %eax, %r12d`, joins `outputs.h:109` and `outputs.h:373`,
  the retained process-identity comparison. Its sampled/scaled
  `CPU_CLK_UNHALTED.THREAD` estimate is 28,800,000.
- `0x165850f`, `mov $0x80000, %r9d`, joins `broker.h:442`, the output admission
  call and its exact charge. The sampled/scaled thread-cycle estimate is
  2,400,000.
- `0x1a9cbd5`, VTune spelling `movupsx %xmm0, 0x40(%rbx)`, joins
  `uwvm_runtime.default.cpp:6211`, the resolved memory descriptor copy. Its
  sampled/scaled thread-cycle estimate is 72,000,000.

These are observed machine instructions with independent DWARF/source joins.
Sampling skid and event multiplexing prevent interpreting the estimates as
exact instruction latency. The interrupted export is preserved separately;
resuming report generation did not recollect or combine timing samples across
machine boots. The near-native goal remains unmet.

## Bounded C++ SIMD entity-search experiment

The `xml-simd-highway` experiment in the SDK temporary security workspace pins
[Google Highway 1.3.0](https://github.com/google/highway/tree/ac0d5d297b13ab1b89f48484fc7911082d76a93f),
commit `ac0d5d297b13ab1b89f48484fc7911082d76a93f`, under its Apache-2.0 license.
This is a portable SIMD implementation experiment, not a replacement XML parser
or a production VM dependency. Only a copied TinyXML2 `XMLPrinter::PrintString`
entity-search loop changes; parsing, entity expansion, XML bytes and the exact
ROS publication permissions remain the same. The new search helper allocates
no memory. The unmodified upstream fixture's allocation behavior is retained
for the comparison and is not introduced into production host code.

Full vector loads require sixteen known readable bytes. The remaining tail is
scalar; a NUL-terminated pointer alone never authorizes an overread. A protected
page plus Clang ASan/UBSan checks 2,384,066 combinations of length, position,
all 256 byte values and text/attribute escaping. Each complete benchmark also
checks the original fourteen expected output hashes and byte-for-byte readback.
All builds use `-O3 -g`, all execution stays in the existing 64 GiB zero-swap
cgroup, and timing is pinned to CPU 8. These receipts do not establish exclusive
CPU/NUMA placement, arbitrary XML conformance or production integration.

Three variants were measured in separate retained cohorts, each with matched
native/Wasm upstream controls, three alternating-order rounds and 31 batches
of 1,000 fourteen-document cycles:

- `bounded-second`: obtaining a bounded span with `strlen` before SIMD scanning
  measured 49.335 microseconds for upstream Wasm and 51.024 microseconds for the
  candidate. No end-to-end benefit was observed.
- `short-prefix`: retaining scalar scanning for the first sixteen bytes measured
  49.765 microseconds for upstream Wasm and 52.600 microseconds for the candidate.
  Native improved in this cohort; that does not establish a Wasm improvement.
- `short-prefix-bitmask`: an explicit Wasm SIMD adapter replaces Highway 1.3.0's
  two scalar multiplication mask reduction with `i8x16.bitmask`, guarding zero
  before `ctz`. Generated Wasm contains the actual vector loads, byte comparisons
  and bitmask instructions. Upstream Wasm measured 48.965 microseconds and the
  candidate 49.408 microseconds; native measured 17.646 and 17.495 microseconds.
  The small differences do not establish an end-to-end SIMD speedup.

The first `bounded-first` receipt retains a qualification-driver failure: it
incorrectly required the library to emit `i8x16.bitmask`. Inspection showed real
vector comparisons followed by Highway's scalar mask reduction. The following
cohort checks the operations actually needed rather than prescribing a compiler
lowering. No failed or slower result is removed. These short-document results
cannot predict large-text XML throughput, and cannot attribute unknown guest JIT
sample locations to XML functions. Host-call, authority and publication costs
remain independently relevant to the near-native target.

## Final robot binding build

The final receive-debt build has VM SHA256
`3d510640a4cd04fcb73d0f752c7bc537e8b078b7bd312c88412b257fe1cd9c6d`
and the 3582-file source manifest
`18b6e3435f44bd6aaf813e0d5464b6b58128f88ae76e28987296ddbf3dd77d3a`.
`robot-final-xml/results.json` qualifies the original upstream output bytes on
this exact binary before measurement. `robot-final-performance` retains the
fixture preparation, baseline and four official hardware VTune collections.
The VM's three translation units and both native/Wasm fixtures use `-O3 -g`.
The existing 64 GiB, zero-swap cgroup remains the execution boundary.

The complete-publication benchmark serializes fourteen TinyXML2 documents per
cycle, publishes each bounded immutable result, reads it back and removes it.
It uses three alternating native/Wasm rounds, each containing 31 measured
batches of 1,000 cycles, pinned to CPU 8. The median of round medians is
**18.657803 microseconds native** and **51.956371 microseconds Wasm**, or
**2.784699 times native**. Output hashes and readback bytes agree. Startup is
outside the batch timer; the separate hardware profiles cover the whole process.

This cohort verifies that the final component receive-debt correction did not
invalidate XML correctness or the performance measurement path. It does not
isolate that correction's timing effect. The machine and cgroup are shared;
CPU affinity does not establish exclusive cores or NUMA placement. This is a
process-local XML publication benchmark, not native ROS application latency,
isolated component IPC latency or a physical control loop. The near-native
target remains unmet. The earlier staged-output ratio and SIMD experiments
retain their own fixtures and binary identities and must not be merged into
this baseline as a controlled optimization comparison.

The official Hotspots source join preserves 831 of 3911 Wasm rows against the
exact frozen source. `[Outside any known module]` accounts for 53.2651% of
sampled self CPU time; it remains unattributed. Observed host costs include
mutex unlock/lock at 2.5485%/2.5181%, `getpid` at 2.4470%, the imported-memory
descriptor builder at 2.3251%, indexed authority admission at 1.6347%, and native
FP save/restore together at 1.9799%. These whole-process sample shares do not
decompose the timed batch difference or prove guest parser cost.

`robot-final-assembly/results.json` retains 73 actual host/system disassembly
artifacts, including the output and artifact dispatch thunks, complete
publication, indexed admission, memory resolution and native FP control. The
guard checks verify the emitted FP boundary instructions around the actual
calls; they are not inferred from a C++ function name or from compiler flags.

`robot-final-microarch/results.json` completes fifteen report/resolver commands
for five functions: publication, detached readback, output dispatch, indexed
authority and imported-memory descriptor construction. It retains 1261 actual
instructions, 478 sampled instruction locations and 1082 instruction locations
with a verified frozen inline source frame. Examples from the hardware-event
export are:

- `0x1513b63`, `cmpq 0x28(%rdx), %r15`, joins `authority.h:215`,
  `authority.h:336` and `authority.h:366`, the caller-generation membership
  check. The sampled/scaled thread-cycle estimate is 26,400,000.
- `0x17f436e`, `jnbe 0x17f43aa`, joins `broker.h:598`, the complete-publication
  result-capacity check. The sampled/scaled thread-cycle estimate is 4,800,000.
- `0x1c42820`, VTune spelling `movupsx 0x48(%rsp), %xmm0`, joins
  `uwvm_runtime.default.cpp:6211`, the resolved imported-memory descriptor copy.
  The sampled/scaled thread-cycle estimate is 170,400,000.

Each address is checked against the actual frozen function range. An independent
DWARF resolver supplies inline frames, and source bytes must match the build
manifest before source text is attached. Sampling skid and event multiplexing
mean these event estimates are not exact individual instruction latency.
Unresolved guest JIT locations remain unqualified; neither assembly inspection
nor source joins make the near-native target complete.
