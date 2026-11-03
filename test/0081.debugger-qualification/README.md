# Current debugger qualification

This is an executable candidate, authored on macOS without running a compiler,
Python import or test there. Copy this entire directory and the updated
`tools/debug/dap_adapter.py` to the admitted SSH Linux tree before any test.
The keeper owns the exclusive Linux lock, verified CPU topology, at most 64 GiB,
zero swap, process-tree wall deadline and cleanup. The runner serializes stages;
it does not start another resource supervisor or acquire a competing lock.

The actual bug fixed here is the DAP memory formatter contract. The current
console emits `Wasm memory module=... index=... offset=... bytes=...` followed by
16-byte hex/ASCII rows. The adapter previously accepted only `memory: XX...`.
The parser now checks the selected module/memory/range, contiguous row offsets,
exact byte count, padding and ASCII, while preserving the earlier bounded wire
spelling. Existing complete-cohort checks still surround every real copy.
A malformed later page or a changed stop withholds the entire response. The new
protocol regression covers every byte value, partial/full/empty pages, large
guest offsets, 64 KiB DAP pagination, wrong selectors, malformed dumps and
mid-copy stop retirement. These formatter tests do not establish native access.

`test_dap_ros_trace_schema.py` additionally covers the actual ROS audit trace
schema emitted by the frozen r13 console. It preserves public resource bytes,
explicit redaction, stable errno reasons and `dropped_sequence` loss counters.
The internal C++ `resource_redacted` flag is represented on the wire by
`resource=<redacted>` and in DAP by `resourceRedacted`; it is not a wire column.
Complete legacy packets remain accepted, while partial/mixed schemas, duplicate
or unknown fields, private epoch candidates and inconsistent reason/result pairs
fail closed. An empty trace retains the requested `after` cursor. The paired
`ros_trace_packets.cc` fixture executes the actual C++ formatter and bounded
trace store for ring overwrite, nonconsuming reads and cursor continuation.
Its unsigned-limit packet is detached boundary rendering, not sequence-exhaustion
execution. The separate frozen Linux packet must be run before these changes
are qualified; no macOS import, compile or test was performed.

`run_endpoint_lifecycle.py` adds genuine late management and detach tests using
the maintained original-Popen/pidfd owner. It launches a real looping guest
with an inherited opt-in endpoint, observes a real recurring breakpoint after
the guest counter/result advanced, then arms a finite exit gate. `quit` and EOF
must release both a cooperative stop and an actual later native trap. A separate
case sends from a forked process with the same UID and same endpoint; the wrong
kernel PID credentials must close management and release the guest. Success
requires waiting the original process and observing kernel retirement after the
guest's own result checks. This is launch-authorized late management. It does
not add arbitrary PID attachment or reuse a retired inherited endpoint.

`run_qualification.py` invokes maintained actual console/DAP drivers and checks
their complete selected case tuples. It distinguishes formatter-only evidence,
real Wasm/native sessions and missing capabilities. The smoke profile requests
DWARF5/O1 for C, C++ and Rust; the full profile adds DWARF4 and the existing full
source-step/value cases. Each stage fails on an unavailable required capability.
`scope.json` is the current gap ledger. Passing selected stages does not set
`full_requested_objective_qualified` to true.

For ROS, the public unwind-selection option has been removed. Every actual ROS
command omits `-Rllvm-call-stack`; checked native unwind and crash stacks are
mandatory. `-Rllvm-exception-dispatch native-unwind` remains valid. Shared
upstream drivers retain their original sources. On Linux the suite copies their
test directories into its output, records every original/derived file hash and
exact textual substitution, removes the obsolete argv pairs, and runs one
`mandatory-native` matrix label. The source-step copy also preserves the
supplied `wasm-ld` driver spelling: resolving its symlink to `lld` changes LLVM's
argv[0] flavor. Production source/dependencies and tools in that derived harness
are symlinks to the current admitted tree. An unexpected source spelling fails
derivation before execution. Current CI-migrated language drivers retain their
already mandatory ROS route and independent ordinary-upstream branch; only the
remaining wasm-ld spelling correction is derived and recorded. No runtime
source or prior evidence is rewritten.

The required build receipt is the original completed link record used by the
maintained source-step/DAP tests: `binary_path`, `binary_sha256`, `link_argv`,
`link_cwd`, `link_returncode`, `source_before_file`, and `source_after_file`.
The source files are the genuine identical before/after canonical fingerprints
from `tools/ci/wasm3_source_fingerprint.py`. The new suite checks the full current
`src` membership and every recorded bundled dependency byte. A hand-written
receipt is not fresh compilation proof; original object/main/common-layout,
link closure and crash/unwind qualification remain keeper prerequisites.

The shortest Linux formatter check requires no VM build:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 "$SOURCE/test/0081.debugger-qualification/test_dap_memory_dump.py"
PYTHONDONTWRITEBYTECODE=1 python3 "$SOURCE/test/0018.debugger/test_dap_wasm_memory_pages.py"
PYTHONDONTWRITEBYTECODE=1 python3 "$SOURCE/test/0018.debugger/test_dap_wasm_memory_writes.py"
```

The first genuine ROS lifecycle slice, with keeper-supplied absolute paths:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 "$SOURCE/test/0081.debugger-qualification/run_endpoint_lifecycle.py" \
  --source-root "$SOURCE" --uwvm "$FRESH_PRODUCT" --build-receipt "$ORIGINAL_BUILD_RECEIPT" \
  --wasm-tools "$WASM_TOOLS" --ros --out "$NEW_LIFECYCLE_OUT"
```

The serial smoke suite runs all seven default stages:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 "$SOURCE/test/0081.debugger-qualification/run_qualification.py" \
  --source-root "$SOURCE" --uwvm "$FRESH_PRODUCT" --build-receipt "$ORIGINAL_BUILD_RECEIPT" \
  --wasm-clang "$WASM_CLANG" --wasm-ld "$WASM_LD" --rustc "$RUSTC" \
  --wasm-tools "$WASM_TOOLS" --llvm-dwarfdump "$LLVM_DWARFDUMP" \
  --ros --profile smoke --out "$NEW_QUALIFICATION_OUT"
```

Use repeated `--stage` selectors for a bounded slice, for example `--stage
protocol --stage endpoint --stage broker`. `--profile full` expands the source
matrix. `--stage blocked-wait` separately runs the actual in-flight atomic-wait
matrix. C/C++/Rust tools and installed Wasm Rust core must already exist; nothing
is downloaded. The cross-module stage qualifies exact Wasm leaf/caller locals
in three actual modules, independently of DWARF source paths. Broker disconnect
retires the adapter's handles while retaining the launcher's paused VM for a
new adapter; true endpoint detach is tested separately above.

Whole-module live replacement, multiworker full-VM update races, committed
rollback, all fifteen producer routes and actual IDE/platform acceptance are
still unresolved. Invalid-body nonpublication and stale-generation refusal
prove narrower function-update behavior. The existing two-worker controller
test uses a diagnostic provider and must not be relabelled concurrent full-VM
hot-update acceptance. A fresh runtime-linked threaded embedding is needed for
that proof, including active-target refusal on another worker, exclusive
publication and physical joins after concurrent stop/resume attempts.

Disabled-debug overhead must be measured on the final ROS product with its
mandatory checked unwind intact. Use the same original long-running workload,
exact result oracle, binary/build closure and pinned CPU for three cases:
ordinary execution without debug authorization; authorized idle observation;
and explicit active pause/step/breakpoint observation. Keep JIT startup separate
from steady guest time, randomize at least ten serial pairs, retain raw times,
RSS, context switches and confidence intervals, and preserve exact process and
cgroup identities. Collect VTune Hotspots/Microarchitecture Exploration or
equivalent hardware counters in separately labelled runs. Compare profiles of
ordinary execution for executed observer calls, allocations and synchronization;
code/source exclusions alone are not a throughput result. Do not report zero
overhead or a regression bound until that current native measurement exists.
