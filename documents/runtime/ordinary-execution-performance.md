# Ordinary execution, ROS and Wasm performance

## What changed

Ordinary LLVM CLI execution previously prepared a cooperative debugger domain
for specified guest-fault containment. As a result, a run with no debugger
recorded activations, emitted safe points and used debugger host wrappers.
Hardware Hotspots attributed substantial GC-loop time to debug_safe_point,
debug_host_scope destruction and activation-ledger suspend_host.

Ordinary owned-source preparation now uses a private runtime-generation marker.
Specified guest faults terminate through the existing process-exit coordinator
on the cold failure edge. This preserves the first exit code and explicitly
selected pending-operation waits, with exit 126 for a newly selected guest trap.
Explicit bounds checks stay enabled in the ROS product, so the coordinator is
never entered from a memory-fault signal handler. Real native frames remain
available for the selected Unwind trace. Uncaught typed guest exceptions still
unwind and retire GC roots before coordinated exit.

DEV control, explicit RELEASE diagnostics and release-update preparation retain
their existing cooperative domains, frame accounting and authenticated
controllers. No guest import can enable the ordinary private marker or obtain
a management channel. Existing namespace, capability, module ownership and
native-address restrictions are unchanged.

A normal checked 32-bit GC struct read no longer publishes a ROS continuation
token. This exact helper cannot call a guest callback, a provider or a collector;
its original reference checks, field checks, ownership leases and locking remain.
Debug wrappers, actual imports, providers and the experimental managed numeric
page retain their existing publication.

Linux synchronous waiting reuses one idle eventfd/timerfd pair per thread.
The cache stores kernel resources only: every operation still registers a new
exit owner and obtains its own permission and quota decisions. Nested live
waiters allocate independent pairs, with no new live-operation limit.
Destruction unregisters the exit owner before disarming/caching descriptors;
reuse drains old notifications before registering the next owner. Timers use
the existing fast_io monotonic-clock helper and are rearmed before polling.
Failed resources are closed. TLS destruction and fork inheritance are checked:
a child neither drains an inherited idle pair nor disarms a parent's live timer.

## Measurement protocol

Use the already installed native Intel VTune 2026.4; no toolchain or ROS
environment is reinstalled. The CPU is an Intel Core i9-14900HX. Worker processes
run on x86_64 CPU 8 (P-core, sibling CPU 9), with the orchestration process on CPU 10 where
possible. All builds, tests and profiles run inside the existing 64 GiB,
zero-swap cgroup. Performance cohorts start after the build and regression
queues finish. An external campaign pins workers to CPUs 16–31; the first
CPU-21 timing cohort overlaps that campaign and is excluded in full. The final
P-core cohort avoids those pinned workers. Other package load may still affect
frequency, so comparisons remain interleaved and specific to this machine.
QEMU is used only to check the other four target ABIs.

The before-optimization executable is retained from revision
812ac5200d886e349cea119b7ae1fae38de5c7de. The WASI/ROS ABI microbenchmarks also
retain the executable preceding full host-call recording. Candidate and
reference ELF hashes, guest modules, exact commands and raw samples are saved
with the local receipts.

Core workloads derive from test/0017.runtime/fixtures/debug_core3_performance.
Every guest also executes the SDK WAT static_math checksum_bias example before
the timed loop. Guest monotonic clocks isolate kernel time from compilation and
process startup. JIT disk caching is disabled, uwvm uses one compile worker,
and the installed Wasmtime 50 uses opt-level=2 with memory64 enabled.
One warmup precedes seven interleaved candidate/Wasmtime samples and three
baseline samples. Checksums and exits must agree.

The ordinary NaN/relaxed-SIMD case accepts the standard's permitted results;
it does not claim to test an opt-in deterministic host profile. Constant-folded
loops are identified separately and are not evidence of realistic application
throughput. Startup wall time is retained separately.

ROS1 Noetic and ROS2 Jazzy use genuine native std_srvs Trigger request/reply
traffic in private network namespaces. The native peer and gateway use -O3
with source information. Native direct calls, authenticated IPC, the Wasm
path and the old Wasm/gateway pair execute the same exchanges; exact reply
bytes and total service callbacks are checked. The guest executes the SDK C
static_pointer_library algorithm before timing. Each launch warms up twenty
calls; two of nine interleaved rounds are excluded, leaving seven timed rounds
of 2,000 requests per mode. The call deadline is explicitly ten seconds on all
service paths to accommodate initial middleware discovery. Discovery, capability binding and JIT compilation
are outside the measured request window.

WASI and ROS namespaces are created/enabled explicitly for main. Guest rights
are granted individually; normal service quotas are unlimited by default.
Authentication, current process/module identity, response validation and
allow-and-deny auditing remain enabled. Native direct ROS has no VM/IPC
security boundary, which is part of the interpretation of the comparison.

Hardware Hotspots and Microarchitecture Exploration are separate from the
uninstrumented timing cohorts. Core profiles run about ten seconds of useful
kernel work. ROS profiles run 20,000 exchanges and include process startup so
that executable mappings are available. Source-line, symbol and summary
reports are retained. Unknown mappings and optimized libraries without full
line information are not presented as precise source attribution.

## Results

The native candidate contains the data-watchpoint, temporary-breakpoint,
callback and typed-local/operand edit implementation. Host tracing and
watchpoints are off in the ordinary execution cohorts. All twenty Core
workloads passed checksum and exit comparisons.

The interleaved kernel medians show the following changes relative to the
before-optimization executable:

- memory64: 134.199 ms to 7.197 ms (18.65 times faster); Wasmtime 7.231 ms.
- table64: 20.030 ms to 0.499 ms (40.16 times faster); Wasmtime 0.525 ms.
- GC struct: 119.393 ms to 35.525 ms (3.36 times faster); Wasmtime 1.189 ms.
- exception payload: 834.683 ms to 528.245 ms (1.58 times faster);
  Wasmtime 14.567 ms.

These are the complete timed-loop totals at the recorded repeat counts.
GC struct still takes 29.88 times the Wasmtime kernel time; exception payload
takes 36.26 times. Other non-folded cases range from 0.326 to 35.12 times
Wasmtime. The six loops folded to near-constant work (annotations,
extended_const, gc_i31, relaxed_swizzle, tail_direct, tail_ref) are excluded
from throughput claims. Compilation and startup remain separate costs.

For WASI, one million clock-resolution calls fall from 193.533 ms to
126.876 ms (34.4% less), and 100,000 random reads fall from 26.440 ms to
18.957 ms (28.3% less). The integer-only Linux query helpers preserve the
host FP-control state without the redundant save/restore wrapper. Their
public permission, bounds and quota checks remain. The 250,000 fd-write
cohort is 244.659 ms versus 241.599 ms before optimization, a 1.3% increase
in these samples; compared with the earlier pre-recording executable the
increase is 2.1%. This measurement does not establish zero disabled-trace
overhead for every import.

Two million ROS ABI-version calls take 266.202 ms (ROS1) and 267.623 ms
(ROS2), versus 298.690 ms and 299.880 ms before optimization. Real ROS
request/reply medians per exchange are:

- ROS1: native 39.624 microseconds, authenticated IPC 74.603 microseconds,
  full Wasm 91.872 microseconds, old full Wasm 93.263 microseconds.
- ROS2: native 24.800 microseconds, authenticated IPC 50.348 microseconds,
  full Wasm 59.671 microseconds, old full Wasm 63.522 microseconds.

The full Wasm path improves by 1.5% and 6.1% in these cohorts, but still
costs 2.32 and 2.41 times native. The ROS1 ranges overlap substantially.
The isolation, authentication and middleware costs have not disappeared.

### Hardware profile interpretation

Hardware Hotspots and Microarchitecture Exploration completed on the final
native ELF, with source-line reports. In the GC struct workload, the
remaining dominant symbols are the checked struct-get bridge (4.236 CPU
seconds) and gc_object_store::struct_set (4.196 CPU seconds). P-core
retiring is 66.6%, back-end bound 26.7%, CPI 0.248. The removed debugger
activation/safe-point work is no longer the dominant ordinary-run cost.
The remaining checked GC representation and helper work require separate
optimization; these measurements do not justify removing ownership checks.

Exception payload still spends substantial time in genuine libunwind
CFI/FDE parsing, DWARF stepping and module unwind lookup. P-core front-end
bound is 35.4%, retiring 54.1%, CPI 0.285. No instruction-stack substitute
was enabled. The memory64 profile is 92.5% retiring with CPI 0.138.

WASI clock-resolution Hotspots now identify mutex lock/unlock, the import
bridge and budget ledger as remaining costs. The ROS profiles include
native peers and gateways and retain significant unknown/kernel mappings;
they are useful for the process-level comparison, not precise attribution
of every sample to a source line. The stored source reports distinguish
resolved functions from unknown mappings.

### Debugger feature cost

A separate interleaved native test isolates this feature addition from the
earlier ordinary-runtime optimization: three rounds of 200 real broker
operations per case, using the retained pre-feature optimized executable.
The median of the three round medians changes as follows:

- Wasm single-step: 92.932 to 93.618 microseconds (+0.7%).
- Native single-step: 112.617 to 115.094 microseconds (+2.2%).
- Disassembly request: 1.317 to 1.327 milliseconds (+0.7%).
- Stack request: 254.360 to 256.674 microseconds (+0.9%).

No watches or callbacks were installed for this isolation test. Two further
12,000-step runs collected hardware Hotspots and Microarchitecture
Exploration. Resolved costs include observation-demand publication,
controller request/trace clearing, copying, synchronization and genuine
Unwind. Unknown mappings remain explicitly unknown. The instrumented
timings are not mixed into the uninstrumented latency results.

Software watches deliberately require comparisons at real stops; an
enabled large-range watch costs more. These results show small measured
changes for the disabled features on this machine, not a guarantee of
zero overhead or a new GDB/LLDB/Wasmtime-debugger comparison.

## Final functional qualification

The source-annotated native ELF used for final performance and new-feature
qualification has SHA-256
`cd61b25357c4de7e2d16c0bca8e0d5950d6945b5ad5051388685b1886a968cfd`.
Each other target has an immutable ELF/source manifest. All five targets
pass all four maintained data-control runners: 120 checks per ABI, 600 in
total. The DAP suite passes 553 tests with no skips; the C++ parser test
also passes. Long-range watches and paginated UTF-8 callback history are
tested without discarding their retained contents.

The wider five-ABI regression uses the immediately preceding candidate's
identical runtime and host-API objects. The final build reuses those objects
only after compiler dependency checks prove that the last controller-only
change is absent from their dependencies. That last change reports native
continue time-slice expiry as resumable progress at its actual stop.
The final per-ABI native data-control tests exercise that behavior.

The wider suite passes eleven groups on each target: interpreter/JIT Core
results and traps, source debugging, native register/control boundaries,
conditional breakpoints, Wasm/DAP state and replacement, host traces,
genuine Unwind, signed irreversible updates, cross-process refusal,
RELEASE diagnostics, and FP-control preservation on the optimized queries.
The shared checkout also received an independent WSEA crypto-only adapter
change while these private builds ran. Before integration, preprocessing
comparisons for all fifteen native translation units (three per ABI) prove
that change leaves native tokens identical apart from OpenSSL's diagnostic
source-line argument (493 to 509) and empty lines. The separate change is
preserved; it is not claimed as part of this implementation.

One AArch64 driver attempt started before its aggregate launcher receipt
was complete; the retained failed attempt is followed by a successful
actual rerun. It is not silently counted as a pass.

The Core corpus contains 79 cases per backend per architecture, including the
SDK C example, memory64, multi-memory, GC, SIMD, exceptions, atomics, malformed
calls, hidden/denied proc_exit, stack exhaustion and specified traps. Actual
five-frame native Unwind is checked; instruction-stack reconstruction is not
enabled. The update corpus passes 911 assertions per architecture (4,555 total),
including signature and binding corruption, foreign senders, replay, post-commit
abort refusal, ledger exhaustion and continued SDK behavior.

Every architecture records and exports the 214 returning WASI/ROS imports
(90 WASI, 124 ROS/portable/RMW) with checked scalar widths, order and complete
typed JSON. Turning recording off produces no additional records. The final
real-ROS quota suite passes all twenty combinations of five architectures,
two backends and ROS1/ROS2, checking explicit rejection and synchronous waits.

The optimized-wait native multi-threaded WASI exit tests pass 23 assertions covering default
cancellation, selected pending writes, late traps/exceptions, first-exit-code
preservation and second-module permissions. The wait-resource component passes
on all five targets, including concurrent/nested waits, stale notifications,
timer reuse, resource retirement, TLS destruction, fork isolation and exit
selection. Native ASan/UBSan passes; removing the destruction PID guard makes
the dedicated parent-timer regression fail as intended.

The maintained ROS driver passes a real ROS2 native/IPC/Wasm smoke test with an
explicit ten-second deadline. Its initial three-second discovery attempt timed
out, returned ROS errno 110 and produced the guest's specified trap/exit 126;
that failed attempt is retained and is not a performance sample.

## Reproducing and interpreting the measurements

The maintained ROS driver is
test/0073.ros-tests/support/source/runtime_repair/run_ros_service_benchmark.py.
Supply the existing native peer/gateway, matching Wasm fixture and VM, then
select --ros-version, --cpu, --socket-root and --cache-mode disabled.
The driver checks the actual cgroup membership, including a namespaced cgroup
root, before it starts workloads. Source the corresponding installed ROS
environment first. Benchmark sockets and all outputs belong under .tmp.

Local adapted fixtures, exact compiler/link commands, native baselines, QEMU
receipts and VTune datasets stay under `.tmp/runtime-performance` and are excluded
from Git. The indexes are `maintained-features6-final.json`,
`functional-features5-matrix.json`, `features6-builds.json`,
`features-dap-units.json`, `performance-features6.json` and
`debug-perf-features6.json`. Raw timing samples remain with each cohort.
The memory cgroup reports no OOM or OOM-kill events. A complete ROS application must additionally account for message
sizes, serializers, discovery, middleware/QoS, callback scheduling and traffic
mix. These microbenchmarks do not establish zero native overhead, hard
real-time deadlines, or performance on a foreign architecture.
