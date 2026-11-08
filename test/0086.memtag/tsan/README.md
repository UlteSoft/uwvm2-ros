# ThreadSanitizer qualification for Wasm threads and software memory tags

This directory contains test-only instrumentation and reproducible controls. It does
not add a release build option. Keep all generated Wasm, snapshots, objects, IR and
logs in the repository `.tmp` directory. Use the existing Linux cgroup with exactly
64 GiB of memory and zero swap, and exclude CPU 0. Reuse installed compilers and
providers; no installation is required by these scripts.

## What is instrumented

`build_snapshot.py` copies a frozen, previously built native x86_64 source snapshot,
applies only the test hook, and rebuilds all three project translation units with
`-fsanitize=thread`: main, host API, and runtime. The private threaded JIT runs LLVM's
function TSan pass after its ordinary optimization and before the existing IR,
machine-frame, call-graph, object, symbol, memory and stack-budget checks. Those
checks remain in place. Exact sanitizer callback signatures bind to the host's
existing compiler-rt. The native unwind personality is accepted only as a Function
personality operand; direct calls and pointer escapes are rejected. The matching
paired unwinder handles cleanup; no second unwinder is introduced.

The host executable initializes TSan. No unexecuted JIT constructor is used. Each
compiled module records callback counts and writes its actual instrumented LLVM IR.
The runner rejects persistent JIT caching, so an old uninstrumented object cannot
satisfy the test. Generated code receives the pass exactly once.

Coverage is deliberately limited:

- This hook instruments the threaded JIT only. Ordinary non-threaded JIT code is not
  covered by this hook.
- Existing LLVM, C/C++ runtime, unwind, libc and OpenSSL providers are not fully
  TSan-instrumented. Interceptors do not make all their internals observable.
- No broad suppression or `ignore_noninstrumented_modules` setting is enabled.
- The VM lowers ordinary shared Guest accesses to host byte atomics to avoid C++
  undefined behavior. TSan sees those atomics. It can therefore miss a Guest-level
  ordinary data race even while correctly observing the actual host operations.
- A race-free sequence of atomics does not imply correct allocation lifetime.
  `logical_retag_window.cc` deliberately reproduces the known check/retag/access
  protection failure: the check accepts tag 6, another thread changes it to 7, and
  the already checked write changes byte 42 to 99. The next check rejects. Exit zero
  from this control means the failure was reproduced, not that protection passed.
- Native x86_64 is the executed profile. Availability of AArch64 runtime archives
  does not establish working QEMU TSan; the local QEMU probe failed in sanitizer
  virtual-address/shadow initialization. 32-bit targets are not covered. QEMU is
  never used for performance conclusions.

## Reproduce a private VM build

Run every command through the existing cgroup launcher, for example:

```sh
R=/absolute/path/to/uwvm2-ros
G="$R/.tmp/platform-parity-20261105/in-renewed-cgroup"
"$G" env TMPDIR="$R/.tmp/sdk-repair-20261104/tmp" PYTHONDONTWRITEBYTECODE=1 \
  taskset -c 20-23 python3 "$R/test/0086.memtag/tsan/build_snapshot.py" \
  --baseline-vm "$R/.tmp/wasmmte/integration-vm5/x86_64" \
  --output "$R/.tmp/wasmmte/tsan-reproduction"
```

The baseline directory must contain the matching frozen `source_root`, receipt,
three compile argument files and link argument file. The test patch is anchored to
that source layout: a failed apply is a review requirement, not permission to omit
instrumentation or security checks. `--prepare-only` validates and prepares the
snapshot without compiling. Additional reviewed production patches can be applied
with repeatable `--patch PATH`; their hashes appear in the receipt. In particular,
the first source5 baseline needs the subsequently discovered registry/maintenance
lock-order fix before a clean qualification can be expected. Keep the failing
baseline and its reports.

All project objects must be instrumented. For iterative qualification, reuse is
valid only for already instrumented objects whose exact dependency files exclude
every changed header; record that proof and the original object hashes. Reusing
ordinary release main/host-API objects would establish only partial coverage.

## Actual Wasm-origin controls

`build_guests.py WASM_TOOLS OUTPUT_DIRECTORY` parses real WAT using the installed
`wasm-tools`, then writes separate extension variants using the pinned research ISA.
It records the binary hashes. `run_guests.py VM OUTPUT_DIRECTORY MANIFEST` runs all
24 standard/MTE-enabled/MTE-ignored interpreter/JIT cases with explicit interface
exposure, call permission, capabilities, resource grants and cache disablement.

Both the main activation and an actual `wasi.thread-spawn` child execute 20,000
operations. Cases cover atomic RMW, an intentional ordinary Guest data race,
concurrent memory growth, wait/notify, tagged host output, and stale access after
explicit retirement. Expected denial exits are distinguished from sanitizer exit
66. A sanitizer warning always fails the runner. The ordinary-race cases record
lost updates separately; their expected execution does not establish Guest race
freedom. Require `TSAN_TEST_IR` receipts for every tested threaded-JIT module.

The existing `test/0086.memtag/threaded_matrix.py` supplies additional scalar, SIMD,
atomic, bulk, wait/notify, grow and tag-negative cases, including real child threads.
Use the built private `vm/run-vm` wrapper and keep its persistent-cache enforcement.

`run_renumber_exit.py VM OUTPUT_DIRECTORY WASM_TOOLS` reuses the existing actual
child `proc_exit` versus main renamed-FD write tests: quota wait and a genuinely
blocked native pipe, each with explicit cancel/wait exit policy. It preserves TSan
diagnostics before recognizing the child's NUL readiness marker. The timing checks
are functional wait/cancel distinctions, not sanitized performance benchmarks.

## Real embedding stop/reset/retirement controls

`build_embedding.py` freezes the existing B1 adaptation, its real after-join
Source-owned memory reader and WAT inputs. It replaces only the native CLI main,
compiles the embedding with the already instrumented VM arguments and frozen
project headers, and links the hash-verified TSan host-API/runtime objects. It
records the linker map and cgroup/boot identity. The original B1 directory retains
its unexecuted-source annotation; this is a separately identified private test.

```sh
"$G" env TMPDIR="$R/.tmp/sdk-repair-20261104/tmp" PYTHONDONTWRITEBYTECODE=1 \
  taskset -c 20-23 python3 "$R/test/0086.memtag/tsan/build_embedding.py" \
  --instrumented-vm "$R/.tmp/wasmmte/tsan/vm2" \
  --output "$R/.tmp/wasmmte/tsan-embedding-reproduction"
"$G" env TMPDIR="$R/.tmp/sdk-repair-20261104/tmp" PYTHONDONTWRITEBYTECODE=1 \
  taskset -c 20-23 python3 "$R/test/0086.memtag/tsan/run_embedding.py" \
  --embedding "$R/.tmp/wasmmte/tsan-embedding-reproduction" \
  --wasm-tools "$R/.tmp/debug-update-full-20261104/wasm-tools" \
  --output "$R/.tmp/wasmmte/tsan-embedding-results"
```

The runner supplies real canonical imported shared memory and an actual
`wasi.thread-spawn` child. `control` and `race` require two native worker identities,
one queued atomic wait plus a progressing main, 100 stop/reset episodes, actual
terminal cancellation, physical join and public drain, a zero post-wait sentinel,
and stale/empty observer behavior. No test constructs an Admission, ChildLease,
Group or pause participant. The `race` mode repeats a real independent native
stopper against guard reset, but cannot claim the precise narrow token-publication
interleaving occurred. Each process has an owned external deadline, pidfd and
process identity and a 2 GiB observed RSS bound; timeout, sanitizer initialization
failure and remaining native
descendants all fail. Enabled LLVM control/race cases also require actual main/child JIT IR receipts.
The `none` case can retire before Guest code begins, so it does not imply any
child executed. The adaptation explicitly selects the real trusted Host `dev`
launch policy before opting into debug metadata/control; the production zero
default and debug admission gates remain in force.
These controls use standard shared atomic accesses and add no claim that the
separate logical check/retag/access failure has been repaired.

## Native components and positive controls

`compile_component.py SOURCE BASELINE_VM FROZEN_SOURCE OUTPUT [--build-only]` reuses
the installed paired compiler/provider arguments and instruments a component. Run:

- `owner_parallel.cc`: 32 real workers across mmap/moving owners, memory32/64,
  shared atomic tags, aliases, independent owners, pinned copies and concurrent
  growth. Destruction occurs only after physical joins; it does not invent support
  for concurrent owner destruction.
- `../storage_lifecycle.cc` and `../runtime_copy_wait.cc`: production lifecycle,
  growth, wait/notify and alias-range components.
- `logical_retag_window.cc`: the separately classified known logical failure.
- `personality_representation.cc`: an actual LLVM-generated cleanup personality
  positive control, plus `call`, `data`, and `prefix` negative modes. Set
  `UWVM_TSAN_IR_DIRECTORY` to an existing artifact directory when running it.

Compile `detector.c` with the installed C compiler, `-O1 -g -pthread
-fsanitize=thread -fno-omit-frame-pointer --rtlib=compiler-rt`. The default atomic
case must exit 0 with count 20000. Its `race` mode must emit an actual data-race report
with both worker creation stacks and exit 66 under
`TSAN_OPTIONS=halt_on_error=1:exitcode=66:second_deadlock_stack=1`.
A sanitizer initialization failure with exit 66 is not a successful positive control.

## Recorded local qualification scope

The frozen native `source2` artifacts under `.tmp/wasmmte/tsan` record 24 actual
Wasm-origin concurrent controls, 68 shared-memory matrix cases and eight exit/I/O
races without a TSan warning after the registry/maintenance lock-order correction.
The separately built `embedding4` records 24 B1 process controls: two disabled
controls and 22 enabled real two-worker cohorts, each with 100 stopped episodes.
All 24 retirement receipts have physical join true, native error zero and every
pending counter zero. The 22 enabled after-join reads have ready=1, positive
progress, and post-wait continuation=0. Twenty independent native stopper attempts
complete; they do not prove the precise narrow scheduling window occurred.
The 11 enabled LLVM processes each retain two actual instrumented module IR files.
No timeout, RSS-bound failure, sanitizer warning or unreclaimed process group was
observed. These sanitized elapsed values are not performance measurements.

`embedding-results3` preserves the initial failed adaptation: the real zero-default
Host debug gate refused its missing trusted development launch. The subsequent
adaptation selects that real Host boot policy and leaves the gate intact. The
`none` LLVM case correctly permits retirement before any child JIT code is emitted.
The original B1 directory and the current production source6 receive no implied
qualification from these private frozen tests. `qualification.json` and the B1
`report.json`/`summary.json` retain source, binary, object and report identities.
The reset episodes exercise `threaded_debug_all_stop::reset`; complete
Runtime/source reset is not covered by this B1 follow-up. The separately
reproduced logical protection failure remains unresolved.

## Result discipline

Keep full commands, compiler/provider identity, source and binary hashes, dependency
files, actual instrumented IR, stderr, counters, exits, real worker evidence, and
failed controls. Separate functional effects, detector positive controls, clean
observed host runs and unresolved logical protection failures. Do not aggregate
these categories into a claim of complete memory or concurrency safety.
