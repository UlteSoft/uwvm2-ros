# Debugger and hot-update performance

Native measurements use Intel VTune 2026.4 from /opt/intel/oneapi/vtune/2026.4, on an Intel Core i9-14900HX. The VM is pinned to x86_64 CPU 21 (E-core), with the test controller on CPU 20. All commands run inside the existing aggregate 64 GiB memory / zero-swap cgroup. Other target ELFs run under QEMU solely for correctness.

Two changes reduce work without caching authorization, stopped execution state, or executable bytes:

- LLVM revision 17 checks MC feature-list syntax in one allocation-free scan instead of a POSIX regular expression. The accepted grammar is unchanged, including empty lists, a single trailing comma and unrestricted feature-name bytes. Actual subtarget interpretation is unchanged.
- Private function replacement defers redundant LLVM function checks to the mandatory complete-module verifier. Fused Wasm validation still runs for every body. Complete-module verification precedes optimization and native emission, and optimization verifies its input and output. Checkpoint mode retains early function verification before sealing checkpoint metadata. DEV function replacement and RELEASE function-body updates use this private compilation path; their separate admission and publication checks remain in force.

The baseline is repository revision efdc1c869d7dd0edea5015a9e46462013e5e14f4 with LLVM revision 16. Baseline and final profiling executables use the same release -O3 flags plus line-table/profiling debug information. Each executable is retained with its SHA-256 and compile/link arguments. Disassembly runs explicitly disable the JIT disk cache; private replacement objects never enter a persistent or cross-process JIT cache. Performance windows exclude process startup, permit signing and initial debugger connection; they include real authenticated requests and replies.

The disassembly workload uses the SDK WAT static_math example inside a loop, a real broker/DAP connection and a stopped native function. Each query requests 128 rows, including 64 predecessor positions. It checks the exact unavailable-row representation, and subsequently proves an old frame handle is rejected after a real step. The update workload changes one or 32 functions in a 128-function SDK-derived module, with valid signed PREPARE/READY/COMMIT transactions. After each commit, the export calls every changed function, and two inputs are checked against an independent arithmetic oracle. No transaction-limit setting is relaxed.

Six alternating pairs of fresh VM sessions produced the following unprofiled results. Each binary supplied 3,600 disassembly queries, 1,200 stack queries, 1,200 Wasm steps and 144 transactions for each update workload.

| Operation | Baseline median (ms) | Revision 17 median (ms) | Latency reduction | Improved pairs |
| --- | ---: | ---: | ---: | ---: |
| 128-row native disassembly | 4.611080 | 3.698813 | 19.78% | 6/6 |
| Native stack query | 1.064715 | 0.840227 | 21.08% | 6/6 |
| Wasm single step | 0.202101 | 0.202228 | -0.06% | 3/6 |
| 32-function PREPARE | 434.321335 | 415.020127 | 4.44% | 6/6 |
| One-function PREPARE | 5.205322 | 5.083956 | 2.33% | 6/6 |
| 32-function READY | 0.097567 | 0.097882 | -0.32% | 3/6 |
| 32-function COMMIT | 0.193839 | 0.196806 | -1.53% | 1/6 |
| One-function READY | 0.092783 | 0.095175 | -2.58% | 2/6 |
| One-function COMMIT | 0.164707 | 0.166841 | -1.30% | 1/6 |

Wasm stepping has no consistent directional change. READY and COMMIT show no improvement in this sample set; COMMIT medians increase by about 2–3 microseconds. The reported hot-update improvement is preparation latency.

VTune Microarchitecture Exploration reported these totals for identical work (2,000 disassembly queries or 24 full 32-function transactions):

| Workload | Baseline retired instructions | Final retired instructions | Instruction reduction | CPU-cycle reduction |
| --- | ---: | ---: | ---: | ---: |
| Disassembly | 56,491,292,781 | 32,396,326,548 | 42.65% | 34.84% |
| 32-function updates | 78,252,223,627 | 73,863,502,212 | 5.61% | 4.60% |

The hardware instruction-count reduction accounts for most CPU savings. E-core CPI changes from 0.378696 to 0.430261 for disassembly and 0.545799 to 0.551606 for updates, while total cycles decrease. Final disassembly hotspots include LLVM feature propagation; update preparation still spends substantial time in LLVM allocation and code generation.

The first attempted final measurement batch did not enumerate compiler children belonging to worker TIDs and is excluded in full. The accepted batch suspends all 23 owned build processes, verifies every retained TID is stopped, and resumes them automatically after profiling. It reports no sampling-time kernel alerts or cgroup OOM events.

VTune hardware Hotspots and Microarchitecture Exploration attach to the actual VM PID; the command-line launcher is not the sampled process. Collections are serialized, bounded to 30 seconds, with 10 ms samples and hardware stack collection disabled. The hybrid PMU companion is CPU 0; report E-core metrics for the VM pinned to CPU 21. Raw source-line reports, symbol hotspots and counters are retained. Existing optimized LLVM archives lack complete line tables, so their named hotspots are traced to the corresponding retained source; unknown source-line samples are not presented as precise source attribution. Profiled elapsed times are separate from unprofiled request latency.

The baseline disassembly hotspot was the regular-expression state machine used by TargetRegistry::isValidFeatureListFormat, together with repeated MC feature construction. The update preparation profile includes LLVM allocation, IR verification and code generation. The optimization reduces redundant verification; it does not change the guest optimization policy.

Final revision-17 provider builds pass on x86_64 natively and i686,
AArch64, ARM soft-float and ARM hard-float under QEMU. Qualification includes:

- 3,177,715 comparisons of the actual linked feature-list function against the
  previous regex, including exhaustive short inputs, NUL/newline bytes,
  randomized strings and long valid/malformed lists.
- 40 signed but invalid replacement bodies, independently rejected by WABT and
  rejected by the VM while retaining the installed image, policy and callable
  SDK behavior; a subsequent valid update still commits.
- 4,555 hot-update assertions covering permit tampering, ownership, stale
  generations, framing, cross-process credentials, replay, ledger exhaustion,
  and irreversible commit decisions.
- Actual broker/DAP Wasm and native debugging, failed/stale reference handling,
  DEV replacement, RELEASE diagnostics and mutation denial, 30 real native
  finish returns across large and mixed-argument functions, and post-update
  Unwind traces.
- CLI checks for the debug/update command groups and rejection of the removed
  instruction-stack policy, plus paired static C++/Unwind and ARM float ABI checks.
- The SDK C static_pointer_library algorithm compiled to Wasm, executed through
  all five VM targets and independently through installed Wasmtime 42 and 50.
  All VM test launches explicitly grant the base WASI namespace.

Pinned-source, exact-header, static-link and canonical-build-directory positive
and negative checks also pass. The LLVM source-manifest SHA-256 is
30ae9f2cacc115f3b9e6f3c805b4d8930e9362725ffe1314ddd565df6c3ecf2c.
No limit, admission check or publication check was relaxed for these measurements.

Reference checks reuse the existing Wasmtime 50.0.0 (ebe5e8df1), WASI SDK LLDB 22.1.0, WAMR 2.4.5 classic debugger and GDB 16.3. Actual guest LLDB sessions exercised Wasmtime/WAMR Wasm backtrace, disassembly and stepping. GDB hit transform_and_sum from the SDK C static_pointer_library example after compilation to Wasm, displayed its x86_64 JIT instructions, stepped and observed normal exit. These are capability checks, not speed rankings: interpreted Wasm steps, guest LLDB requests, native ptrace operations and authenticated owned native DAP queries perform different work. WAMR source analysis is separately pinned to commit 3a6fb9a8e7871f20471c48592390b9d2342287b1; that checkout was not substituted for the reused runtime binary.

Temporary fixtures, compiler outputs, native and QEMU logs, reference-client failures and corrections, and VTune datasets are retained under .tmp/debug-perf-20261105 and excluded from Git. The preliminary proposal executables are exploratory evidence only; final qualification identifies real revision-17 provider builds. An oversized 2048-function exploratory startup timed out and is excluded from the performance sample set. No claim is made for real-time robot deadlines or non-x86_64 throughput.
