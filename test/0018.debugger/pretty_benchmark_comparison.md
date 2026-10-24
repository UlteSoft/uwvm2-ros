# Native debugger comparison

These measurements were collected on 2026-10-19 in the existing Linux
64 GiB, no-swap cgroup. They compare complete stopped-console queries,
including transport, validation and display. They are not isolated
formatter timings or a claim of all-language feature parity.

| Fixture/profile | UWVM median | GDB median | LLDB median |
| --- | ---: | ---: | ---: |
| C++: vector of 16 integers and two 21-byte strings | 88 / 94 us | 44 / 46 us | 116 / 119 us |
| Rust 1.90: Vec<i32>, String, &[i32], &str, Vec<String> | 83 / 89 us | 76 / 70 us | 809 / 796 us |

The two entries in each cell are separate unprofiled rounds, not confidence
bounds. C++ used 3,000 queries per round; Rust used 10,000. Each measured
value query was paired with a read-only print-limit acknowledgement. The
original program was stopped at a verified source location; the same
values and source frame were checked before and after measurement, then
the original program and debugger exited with status zero. Measurement
affinity was applied only after actual stopped-value proof.

C++ compares LLVM/libc++ Wasm32 with GCC/libstdc++ native64. The mean of
UWVM round medians fell from about 135 us before the immutable interface
fingerprint cache to about 91 us afterward. An earlier closed-schema
optimization had reduced a separate baseline from about 208 us to 128 us;
these are different measured cohorts and must not be merged into one run.

Rust used the same original Rust 1.90 crate and standard-library revision
for Wasm32 and native64. GDB and LLDB used the Rust-distributed providers.
LLDB's String/str summaries had byte-child expansion disabled for the
table above while Vec element expansion stayed enabled. The default LLDB
provider was also measured separately (about 3.62 / 3.55 ms); its deeper
string output must not be presented as an algorithm-only comparison.
Rust Vec<()> is tested separately and excluded from common timing because
the selected native tools have different zero-sized-value behavior.

Output sizes remain different: about 213 bytes/query for the Rust UWVM
profile, 121 for GDB, and 227 for the LLDB profile in the table. The shared
host, target bitness, library ABI, compiler and output differences limit
what can be inferred from these numbers.

The table belongs to the pinned immutable-fingerprint performance cohorts.
Registered-worker exit and DAP startup fixes have separate functional
qualification; these earlier timings do not measure that later lifecycle cut.

## Actual hardware collection

Every engine was also sampled with VTune hardware Hotspots and detailed
microarchitecture collection, using 60,000 real value queries per profile.
The stored reports qualify actual target PID/TID, CPU and PMU samples.
Insufficient helper-thread samples are kept unqualified rather than
reported as fabricated zero counters.

For the C++ UWVM process's qualified main thread, VTune's scaled detailed
report changed from about 89.4 billion to 35.5 billion retired instructions
and from 7.46 to 4.68 seconds of self CPU time after the immutable
fingerprint cache. The corresponding GDB estimate was about 34.0 billion
instructions. These are sampler estimates for the measured main-thread
profile, not exact instruction counts or whole-process totals.

The cache contains only the static interface fingerprint. Live factory
membership, source identity, epoch, stopped frame and read authority are
still checked on every request.

The original receipts are retained in the task's Linux
/tmp/uwvm-pretty-20261018 workspace: console-latency-comparison-r721-*,
console comparison hardware cohorts r705/r731, rust-console-benchmark-r739-*,
and rust-console-benchmark-r769-lldb-summary-*. Compact pinned summaries
are performance-summary-r753.json, rust-performance-summary-r763.json
and rust-lldb-matched-summary-r823.json in the task's local /tmp review
workspace. Preserve the original receipts and tool/producer SHA pins
when reproducing or extending the comparison.
