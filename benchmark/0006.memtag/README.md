# Native WasmMTE performance qualification

This workload separates ordinary Wasm, an instrumented binary with tag execution
disabled, and the same instrumented binary with checks enabled. Each timed run
checks its result. The loop uses volatile accesses, so a constant-folded result
cannot replace the measured memory traffic.

Use the existing compiler, runtime builds and Intel VTune installation. Run
compilation, fixture generation, VM execution and profiling through the
configured 64 GiB / zero-swap resource-control launcher. Select a non-system
native x86_64 core; do not use QEMU timing as a performance result. The current
development campaign reserves CPU 2 and leaves its sibling CPU 3 unused.

The fixture manifest records the linked module hash, pointer width,
instrumentation mode and iteration count. Pass native ELF executables to
run_linear.py, so its executable hashes identify the measured programs.
Retain exact commands, exit codes, checksum results, raw timing samples and
machine/compiler versions under .tmp/. The runner alternates engine order,
uses CLOCK_MONOTONIC, disables JIT disk caching, and reports whole-process
elapsed time. It does not subtract a separate startup measurement.

## Bounds-proof optimization

The runtime can use the actual local owner's immutable effective minimum.
It must not substitute another memory's minimum, an imported provider's
assumed size, a profile observation, or a previously grown size that can become
invalid across lifecycle transitions.

A versioned private compiler witness marks only a stable mmap owner's internal
bounds length read. After LLVM optimization, a second check removes that exact
acquire only when its result is unused and its type/range/witness agree.
Explicit memory.size, guest atomics, tag reads, used length results and
unmarked reads remain. A grown-suffix access retains the acquire needed to
observe the publication of its pages. The cleanup introduces no per-access
runtime policy branch.

The accompanying minimum_proof_ir.cc and minimum_runtime.py regressions cover
the compiler witness, actual WAT compilation, observable memory growth,
different owners and negative exits. The runtime tests can execute identical
standard modules in Wasmtime. Extension-specific positive and negative tests
remain in vm_matrix.py.

## Development measurements and limits

On the recorded native x86_64 campaign, five interleaved samples of the
initial-memory linear workload gave enabled-MTE medians of 3.759 to 2.294
seconds for Wasm32 and 4.457 to 2.753 seconds for Wasm64, before and after the
integrated lowering changes. Checksums and exits agreed in all samples.
Other-core compilation was active, so these are workload-specific development
measurements rather than a controlled publication benchmark.

In the updated build, the same instrumented binaries with tag execution
disabled took 1.662 and 1.744 seconds. Enabled checking therefore still cost
about 38% and 58% in this workload. These results do not establish negligible
software cost, full sanitizer coverage, or ROS application performance.

A separate workload places its working array in a newly grown page. Enabled
medians changed from 3.787 to 3.547 seconds for Wasm32 and from 4.835 to 4.035
seconds for Wasm64. An earlier candidate with an added runtime prefix branch
regressed this control and was rejected; it is not the implemented lowering.

Hardware Hotspots, Microarchitecture Exploration and native PMU counters
corroborate the removal of repeated instructions. For the isolated late-cleanup
comparison, Wasm32 retired instructions decreased from approximately 92.493
billion to 71.010 billion. This comparison is between two intermediate
compiler variants, distinct from the complete before/after executable timing
above. The same private cold JIT-registration observer was used for both.

VTune resolves the generated function and its native assembly in this campaign.
Guest source-line attribution is still unavailable for this ordinary JIT
profile; host source information must not be represented as guest source
coverage. The tool's invalid aggregate frequency estimate is excluded from
conclusions. No AArch64 hardware performance claim is derived from simulation.

These measurements do not close the concurrent software check/retag/access
window. A reader/retirement protocol and its compiled-region optimizations need
separate correctness and performance qualification before mandatory SDK use.


The subsequent merged debugger/shared-runtime/replacement-policy build was
compared with the earlier integrated build on the same CPU2 and identical
guest binaries. Five alternating whole-process runs gave ordinary32 medians
1.948174→1.866959s, disabled32 1.874078→1.864852s, ordinary64
1.895393→1.887210s and disabled64 1.849337→1.858173s. Enabled timings were
2.559617→2.609821s (32) and 2.816940→2.885205s (64), about2% slower in
this wall-clock batch; other-core builds were active.

A separate three-pair native PMU check examined that timing difference.
Median enabled32 retired instructions were71,001,135,060→71,001,604,238
and cycles11,790,673,964→11,846,886,992. Enabled64 instructions were
83,890,555,272→83,890,400,086 and cycles13,068,535,253→13,032,430,936.
Thus these whole-process counts show no material instruction increase in the
tested workload; they do not justify calling the wall-clock changes zero or
claiming all SDK workloads are unaffected. Frequency/load and code/data
placement remain possible contributors. The records are under
.tmp/wasmmte/integration-vm5/{timing,pmu}, with exact VM and guest hashes.
This follow-up does not reduce the separately measured cost of enabling
software MTE compared with disabled checking.
