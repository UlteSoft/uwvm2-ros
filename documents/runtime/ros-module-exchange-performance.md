# Registered Wasm messages: qualification and profiling

`uwvm_ros_v2.module.call.*` carries bounded messages between explicitly registered
Wasm endpoints. The route fixes caller, recipient, operation, opaque request and
response schema identities, byte capacities, outstanding slots and lifetime.
Import exposure, operation authority, endpoint role and runtime incarnation are
independent checks. Route registration never authorizes arbitrary export calls.
See the [operator contract](../command-line/ros-scene.md#module-observation-and-registered-messages)
and [operation reference](ros-operation-reference.md#modulecallsubmit).

## Memory and observation boundaries

Each endpoint owns an unexported shared memory. The broker rejects imported
memory, memory exports and nonshared heaps. No memory or native pointer is
given to the peer. An accepted input carrier is copied with byte-atomic accesses
and overwritten before dispatch by default. A frozen, per-route
`clear_after_copy` Boolean lets the trusted host deliberately retain the sender's
input bytes. It defaults to `true`; `false` also skips input failure cleanup but
does not change authority, memory validation or broker/delivery erasure. The
guest cannot select this policy. See `--ros-scene-call-clear-input` in the
[operator contract](../command-line/ros-scene.md#module-observation-and-registered-messages).
Registered delivery extents remain readable until
their corresponding response or acknowledgement. Cancellation, observed expiry,
revocation and retirement also erase retained delivery regions and all written host payload bytes. Each reserved slot starts fully zeroed. Writes begin at offset zero, and consumption clears the complete written prefix before reuse. The unused suffix stays zero. Final buffer destruction clears its entire allocation.
Failed erasure quarantines the slot instead of making it reusable. Concurrent
guest stores require byte-atomic access; replacing these stores with an ordinary
vectorized `memset` would violate the memory-access contract.

Byte-atomic transport does not promise an atomic snapshot of a whole message.
The host validates its owned copy before dispatch; concurrently changed input
is still untrusted data, never a borrowed record interpreted in place.

Erasure covers the broker's owned storage and registered guest extents. It
cannot remove independent copies deliberately made by a guest, an authorized
debugger or the operating system. Expiry is evaluated when the router observes
it, not by a hard real-time timer. Shared memory declaration does not enable
WASI-thread creation. Active threaded host calls and resumable/replay compilation
profiles remain unsupported for these carriers.

A live debugger `observe_values` profile is distinct from resumable execution.
The runtime permits that profile without enabling capture or restoration of
message transactions. `scene resources` and `scene authority`, including the
DAP `uwvm/sceneState` view, return detached counters and labels only. They do not
read payloads, consume queues or change erasure state. Full/read-only debugger
memory reads are separate, explicitly authorized inspection operations; limited
debugger permission cannot perform them.

## Payload size and reserved capacity

The broker accepts a route-specific upper bound on each message. An 8-byte
reply on a route admitting 16 KiB uses the same checked copy and erasure
path as an 8-byte reply on a smaller route. Reserving a larger buffer costs
memory at registration. Completed transfers erase the bytes written to the
slot. Guest delivery regions retain their independently registered extent
and erasure rules.

The following Linux x86-64 measurements used the VM built
with `-O3 -g` and its AOT guest execution engine. The native program used the
same broker, roles, authorization and input-clearing policy. Each observation
covers submit, take, respond, result and acknowledge. Each of three interleaved
runs measured 101 batches of 10,000 complete round trips pinned to the same
P-core. The figures are medians of the per-run median ns per round trip.

| Payload | Reservation per direction | Native | Wasm AOT | Wasm / native |
| --- | --- | ---: | ---: | ---: |
| 8 B | 8 B | 655 ns | 970 ns | 1.48 |
| 256 B | 256 B | 900 ns | 1,196 ns | 1.33 |
| 4 KiB | 4 KiB | 3,883 ns | 4,263 ns | 1.10 |
| 8 B | 16 KiB | 655 ns | 966 ns | 1.48 |

A separate interleaved comparison against the previous VM measured the
8 B / 16 KiB case at 1,154 ns before and 967 ns after the erasure change.
The VM binary used here has SHA-256
`35d90a136f979b98a24defa5e83a40f0af15289931ab6c8ba341675f02f6262c`.
The retained-byte unit test inspects the entire slot directly. Guest
regressions cover input clearing, delivered frames, permission failure,
retirement, payload auditing and debugger visibility.

Intel VTune hardware Hotspots and detailed Microarchitecture Exploration
resolved the broker's functions, source lines and assembly at the period
accepted by the collector. VTune rejected an explicit 0.01 ms interval on
this host. A separate user-mode hardware recording with Linux perf used
24,000 reference cycles per sample. Median wall spacing was 10.394 µs
for native and 10.421 µs for Wasm. Both recordings reported zero lost
samples or throttling. These samples describe repeated work on one host.
Physical I/O latency, distributed communication and worst-case execution
time require separate measurements.

## Configurable caller input erasure

A route plan carries `bool clear_after_copy{true}`. The trusted deployment may
set it before activation through `module_call_input_clear(environment, route,
value)` or `--ros-scene-call-clear-input ENV ROUTE true|false`. It affects only
the invoking module's input carrier; peer delivery regions, broker buffers,
revocation and retirement cleanup remain mandatory. Retained input may disclose
previous request bytes to code with access to that sender's memory. This is a
host-selected tradeoff, not a new Wasm authority or a guest-controlled bypass.

Exact admission precedes reading this setting. Environments with a uniform
input policy use a constant summary; mixed environments resolve the admitted
route. Configuration updates recompute this summary before activation, and
frozen environments refuse subsequent changes. No live permission, incarnation,
revocation or budget decision is cached. The debug resource row reports the
Boolean without reading a payload; the IDE warns on explicit retention.

The first input-copy experiment interleaved each atomic load with a zero store.
Its exact-VM comparison (`module-exchange-input-policy-qualified-performance`)
measured 4 KiB round trips at 5751.71 ns versus 4301.05 ns for the previous
implementation, a 33.73% regression. That experiment is rejected. Hardware
Hotspots and detailed microarchitecture receipts in
`module-exchange-input-policy-interleaved-hardware` retain native/VM source and
assembly reports. The input loop accounts for approximately 3.16 s of native
and 3.22 s of VM sampled CPU time in their respective Hotspots runs. The native
instruction report identifies the mixed byte-copy/zero loop; it is not a
permission-check regression inferred from elapsed time alone.

The corrected implementation keeps one complete-range/tag proof and backend
pin, finishes the byte-atomic copy, and conditionally executes a separate
byte-atomic zero pass. A standalone actual-memory helper experiment
(`module-exchange-input-loop-experiment`) measured 1496.60 to 797.85 ns for the
4 KiB clearing path. These helper numbers exclude complete API routing and
must not be substituted for end-to-end results. Both policies retain explicit
bounds/tag checks, pinned growth lifetime and atomic byte accesses.

### Exact-VM qualification of the input policy

The final separate-pass VM is `vm-module-exchange-input-separated/uwvm2-ros`,
SHA-256 `eb2bfd0bbae8fd21348b09b8474c4a1747b402fb1f12fbd52ce5115c69272a4d`.
Its frozen source manifest is
`d8c8207c808af6780aacf3418d2378de78ae0e7a545da437f5cf90aaeefe0e99`;
all three compilation units retain `-O3 -g`. The
`module-exchange-input-separated-serial-qualification.json` sequence completes
22 functional suites, followed by assembly and timing comparisons. Its 1,421
actual-VM cases comprise:

- 528 frame cases: 176 each for the default, explicit `true` and explicit
  `false` policies, including Wasm32/64, memory indices zero/one, software tags,
  interpreter/AOT, complete bytes and mandatory delivery erasure.
- 24 CLI/warning cases, 340 import-denial cases, 164 module-security cases,
  eight live DAP cases and 58 observation/FP cases.
- 99 native-component fault cases, including 44 unchanged original RealMan
  library cases, 31 model cases, 41 real two-worker ROS cases and 128 bounded
  official-MuJoCo simulation cases.

The five separate `module-exchange-input-separated-{preflight,memory,fp,
linux-five,module-check}` receipts cover ASan/UBSan, TSan, GCC, hostile native
floating-point state, recoverable allocation failure and named-module helper
inclusion. IA32 executes with its native loader; AArch64 and both ARM float
ABIs use QEMU. These are supporting functional checks, not additional executions
of the selected VM or native-performance qualification on four other machines.

The rejected interleaved candidate's first ROS process run observed error 125
at the first node's `communication.open.take`. Its cause remains unresolved.
The exact command subsequently passed twice on that candidate and twice on the
previous baseline; the complete process suite also passed on rerun. Those
failed and diagnostic receipts remain under `module-exchange-input-policy-*`.
The final separate-pass candidate passes all 41 process cases in its own fresh
suite. This does not establish a root cause or justify deleting the earlier
failure evidence.

The final A/B/B/A comparison against the preceding frame-spans VM is retained
in `module-exchange-input-separated-qualified-performance/results.json`:

- 8 bytes: 990.79 to 996.72 ns, a measured 0.60% increase.
- 256 bytes: 1204.36 to 1172.61 ns, a measured 2.64% decrease.
- 4096 bytes: 4302.10 to 4304.63 ns, a measured 0.06% increase.

The large interleaved-loop regression is removed; these observations do not
prove a default-policy speedup for every payload. Measurements are medians of
batch means for the complete submit/take/respond/result/acknowledge lifecycle,
with 10,000 round trips per batch. They are not per-call or tail latencies.
The separate same-source native comparison in
`module-exchange-input-separated-default-native/results.json` measures
639.01/982.09 ns for native/Wasm at 8 bytes, 857.10/1185.23 ns at 256 bytes,
and 3905.20/4288.67 ns at 4096 bytes: ratios of 1.537, 1.383 and 1.098.
Native arrays replace VM memory ownership machinery but retain the production
carrier's authorization, routing, FP controls, copying and cleanup. This is
not an arbitrary vendor IPC comparison or a claim of universal native parity.

With input retention selected, the separate
`module-exchange-input-separated-false-native/results.json` cohort measures
636.55/983.30 ns for native/Wasm at 8 bytes, 810.48/1128.02 ns at 256 bytes,
and 3117.72/3558.62 ns at 4096 bytes. The corresponding ratios are 1.545,
1.392 and 1.141. Against the preceding default-policy cohort, the observed
Wasm differences are approximately +0.12%, -4.83% and -17.02%. These are
sequential cohorts with the same CPU and workload parameters, not an interleaved
policy A/B experiment or a guaranteed percentage gain. The small input does
not demonstrate a speedup. Retention remains an explicit data-remanence tradeoff;
no host, response or delivery cleanup was removed to obtain these timings.

### Retained hardware and instruction evidence

`module-exchange-input-separated-{default,false}-hardware/results.json`
completes 16 serial collections: both policies, native/Wasm, 8/4096-byte inputs,
and hardware Hotspots/detailed Microarchitecture Exploration. Each full
analysis requests 0.1 ms and retains its actual programmed event periods.
The separate `*-hardware-10us/results.json` receipts complete eight explicit
user-PMU traces imported into VTune. All verify the 24,000-reference-tick
period, a nominal 9920.63 ns, with no lost or throttle records. Observed median
reference-event spacing ranges from 10912 to 10959 ns. This verifies the
nominal hardware target, not a fixed wall-clock interval or per-call latency;
the limited explicit event set does not replace detailed microarchitecture.

The final join in `module-exchange-input-separated-final-evidence/results.json`
pins the 1,421-case exact-VM qualification, five separate core receipts, both
native timing cohorts and all hardware receipts. It audits 36 assembly reports:
24 carrier reports and 12 reports selecting the VM's actual checked input
helper. The native input function has 119 decoded instructions, including
separate unrolled byte-copy and conditional byte-zero loops with sampled
instructions. The VM helper has 252 decoded instructions. Its loop body is
present, but VTune's inline attribution leaves some source-line and event cells
empty. A retained `module-exchange-input-separated-inline-off` report confirms
that disabling inline display does not resolve that limitation; no complete
per-instruction VM-loop attribution is claimed.

The 4096-byte Hotspots profiles attribute approximately 1.633/1.660 s of sampled
CPU time to native/VM input copying with default clearing, and 0.874/0.880 s
with input retention. These are whole-profile function totals supporting the
loop diagnosis, not individual API timings. The frozen source, full optimized
instructions, source/function reports and raw explicit-period traces remain
available for independent inspection. No gate or sampling permission was
widened by these runs.

The full VM build and functional/timing sequence preserve the original 64 GiB,
zero-swap gate. Their memory-event counters remain unchanged, including OOM
and OOM-kill. These tests qualify the selected carriers and fixtures; they do
not establish complete ROS-ecosystem coverage or physical safety certification.

## Scoped access and exact operation admission

The optimized carrier authenticates the immediate Wasm caller once per host
invocation. A runtime-constructed, noncopyable access object proves the selected
memory's owned/shared/unexported declaration once. Its closed synchronous
callback neither runs a guest/provider nor retains the object. The execution
lease excludes replacement or owner destruction; each copy still pins the
current memory backend and checks current bounds and tags. No identity,
permission decision or raw data pointer is cached across calls.

Current-owner input copying and default erasure share one scoped owner proof,
complete bounds/tag check and backend pin. A byte-atomic copy pass precedes
the optional byte-atomic zero pass. Opting out keeps the same checked
copy without zero stores. Failure cleanup uses the existing scoped
erase operation only when clearing is enabled. Delayed/remote erasure still resolves the sealed
incarnation under the publication guard. Both use byte-atomic stores and erase
the complete registered extent, even if tags changed after capture. Failed
erasure still quarantines the retained route slot. Preflight and final admission
remain separate; neither a successful preflight nor the scoped memory object
can authorize work after revocation.

The six exact module-call action names have frozen internal indices, sharing
the existing policy admission implementation. This eliminates repeated
string-tree searches while keeping live membership, exact exposure, handle,
revocation, rate accounting and authority locking. Unknown enum values fail
before any bit shift. These indices are not a guest ABI or a new permission
surface. Differential tests compare the indexed and string paths across all
grant/exposure/action combinations, unauthorized identities, exhaustion,
zero-budget limits and revocation.

An `erased_buffer` now clears its live extent once at destruction. Shrinking or
changing storage already erases the complete previous extent; unused inline
bytes never contain an unerased payload owned by that buffer. Removing a second
unconditional 512-byte wipe does not shorten erasure of reserved route capacity.
Optimized allocator-interposition and inline-storage tests verify clearing
before free, after shrink/destruction, and across recoverable allocation failure.

## November 17 hardware collection and rejected experiment

`worker-born-domain-hardware-100us-driver-access/results.json` pins runtime
`d865f87e3e759a7710f6e878450f9bdbf388c20f623a8ae96278901ad09fa039`
and completes eight serial hardware collections: native and Wasm, 8-byte and
4096-byte payloads, hardware Hotspots and detailed microarchitecture analysis.
Every variant retains function, source-line and assembly reports. Native
reference code uses the same frozen carrier, authority, router and erasure
implementation. It substitutes stable arrays for VM memory ownership checks;
this comparison does not establish native process IPC or robot-loop parity.

The successful request is **0.1 ms**, with actual perf event periods retained
in the receipt. It must not be reported as 0.01 ms. With the kernel maximum
set to 100000 and automatic CPU-time throttling disabled, the separate
`worker-born-domain-hardware-10us-driver-access/results.json` attempt still
fails before collection. Its retained `sep.events` reports minimum-period
adjustments; `sep-pmu.strace` records `EPERM` opening `/dev/sep5/c` inside
the required cgroup even after the host device group/mode was corrected. The
attempt grants no sampling evidence. No test leaves the memory-limited group.

The native comparison runner also supports explicit user-only perf event
periods followed by VTune trace import. That opt-in mode validates event
attributes, every recorded period, reference frequency provenance, sample
loss and throttling. Native and Wasm samples are audited separately. A
successful import is not sufficient to qualify the 10-microsecond target,
and this limited event set does not replace detailed microarchitecture
collection. Sampling intervals are not individual request latency.

The retained-region-count candidate passed its core and 484 real-VM cases,
but `module-exchange-region-count-comparison/results.json` measured no
convincing improvement in serial A/B/B/A: 1124.96 to 1130.53 ns for 8 bytes,
1317.00 to 1312.68 ns for 256 bytes, and 4423.08 to 4400.48 ns for 4096 bytes.
It is **not integrated**. These are five-operation round-trip batch means,
not a one-copy duration or individual-request tail distribution.

## Prepared admission optimization

The accepted follow-up uses runtime
`d5f4e39ac1278bd0c5fd46281f1b9008a5fecfa6c61df23d9ff26c3084178e25`
from `vm-module-exchange-enrollment/full-build-results.json`. It changes only
the prepared deployment preflight and its message carrier. An already enrolled
caller reaches exact indexed authority admission without repeating the string
exposure lookup and enrollment. The denied path retains exact configured
exposure and recoverable first-use enrollment for group capability sharing.
Deployment and authority lock ordering, generation fences, operation grants,
attenuation and final post-copy revalidation remain unchanged.

`module-exchange-enrollment-core/results.json` passes optimized GCC,
ASan/UBSan and TSan, including 36 exposure/action combinations, no-allocation
warm admission, recoverable cold enrollment, stale/retired callers, cross-group
handles, zero quota, cleanup and revocation. The preceding admission candidate
incorrectly refused a first-use receiver holding an authorized group capability.
The real VM message fixture detects that regression; the candidate was rejected
and never integrated. This negative control remains in
`module-exchange-admission-vm-module-security/results.json`.

The accepted candidate passes 612 real-VM cases: 340 import denials, 82 module
security cases, four DAP cases, 58 observation/FP cases and 128 bounded MuJoCo
cases. Its own receipts are separate from the preceding runtime's 2,914-case
aggregate. Serial A/B/B/A at 10,000 round trips per batch gives:

- 8 bytes: 1114.62 to 1046.39 ns, a 6.12% reduction.
- 256 bytes: 1311.64 to 1248.52 ns, a 4.81% reduction.
- 4096 bytes: 4397.33 to 4343.30 ns, a 1.23% reduction.

These are batch means for submit/take/respond/result/acknowledge, including
authorization and erasure. They are not direct-call latency or request tails.
`module-exchange-enrollment-comparison/results.json` retains all A/B observations
and identical guest hashes.

The separate unprofiled `module-exchange-enrollment-native-baseline/results.json`
uses the identical frozen production carrier on both sides, with stable native
arrays replacing VM identity and memory checks. Median batch means are
687.57/1035.18 ns for native/Wasm at 8 bytes (1.506x), 907.42/1237.97 ns at
256 bytes (1.364x), and 3885.38/4317.16 ns at 4096 bytes (1.111x). This is
measurable overhead relative to that reference. It does not satisfy a blanket
near-native goal, especially for the small-message case, and cannot be used as
a native driver IPC or full robot-loop comparison.

`module-exchange-enrollment-explicit-hardware-10us/results.json` successfully
collects and imports four native/Wasm traces for 8 and 4096 bytes. Reference
events use a verified period of 24,000 ticks and the recorded reference
frequency, giving a nominal 9,920.63 ns interval. The per-event attributes and
sample records agree, with no lost or throttle records. Observed median
reference-event spacing is approximately 10.9 microseconds. This qualifies the
requested nominal hardware period, never a fixed wall-clock interval. Both
carriers retain source and assembly reports for `preflight_module_call`. The
trace-import event set supplements the full 0.1 ms microarchitecture analyses;
it is not a replacement for their additional counters. No sudo or resource-limit
change was used, and all cgroup memory-event counters remain unchanged.

The full-analysis follow-up is retained in
`module-exchange-enrollment-hardware-direct-target/results.json`: six completed
Hotspots/microarchitecture collections plus the two successful native 8-byte
collections explicitly reused from the earlier attempt. All eight analyzed
combinations retain source and assembly evidence. The interrupted attempt is
kept incomplete: one `taskset`-launched Wasm trace lost the target's module
mapping and could not resolve assembly. Recollection sets affinity in the
runner and profiles the native/VM executable directly. The profiling tool now
uses that arrangement; explicit `--assembly-function` selectors replace the
default selectors. A missing symbol never silently counts as assembly evidence.

The final installed runner is independently exercised by
`module-exchange-enrollment-profiler-runner-final/results.json`: both native and
Wasm 8-byte runs complete with verified 24,000-reference-tick periods, source
and assembly reports, unchanged input/source hashes, and zero lost/throttled
records. Observed median reference-event spacing is 10,961 ns for native and
10,915 ns for Wasm. The nominal 9,920.63 ns period is qualified; neither those
spacings nor profiled batch timings are substituted for unprofiled latency.

## Qualified binary and correctness evidence

### Complete-frame experiment and instruction-level correction

The frame experiment uses runtime SHA-256
`a4d00066fee4f52450b7815cd45d66d30c49491237386c667fe020ed56e6f405`
from `vm-module-exchange-frame-spans/full-build-results.json`. The runtime
checks the entire header-plus-payload destination once before writing either
part. Bounds, tags, owner, memory declaration, lifetime, byte atomicity and
quarantine remain enforced. No temporary payload allocation is introduced.

The first implementation is rejected: its payload loop repeatedly reloads
captured span offsets, and its isolated 4096-byte write takes 1256.08 ns versus
422.30 ns for separate writes. The corrected implementation derives separate
header and payload spans after the complete-range proof. Optimized assembly
then has fixed loop bounds and four-byte unrolling, with individual atomic
byte stores. The corrected isolated write takes 381.69 ns. These observations
come from `module-exchange-frame-spans-isolated/results.json`; an isolated
write does not establish complete-call acceleration.

In fact, `module-exchange-frame-spans-qualified-performance/results.json`
does **not** demonstrate a complete-round-trip improvement. Serial A/B/B/A
medians are 984.27 to 991.21 ns (8 bytes), 1179.33 to 1196.89 ns (256 bytes),
and 4301.91 to 4307.64 ns (4096 bytes). The corrected complete-frame path is
retained for its single full-range refusal boundary, with no throughput claim.
The independent native comparison in `module-exchange-frame-spans-native/results.json`
records native/Wasm means of 665.47/994.55, 900.37/1198.96 and 3915.52/4303.93 ns,
respectively. Both sides use the same production broker; stable native arrays
replace VM memory ownership and incarnation checks. These ratios do not measure
native process IPC, physical device latency or a complete control loop.

`module-exchange-frame-spans-qualification.json` joins 959 cases executed by
this exact VM and three separately identified core receipts. The cases cover
denied imports, registered messages, module observation, DAP, bounded model
and MuJoCo workflows, native constructor/call/completion failures, controlled
stop, resource readback and the pinned original RealMan library's metadata
adapter. Injected adapter failures are not vendor defects. Core evidence covers
ASan/UBSan, TSan, GCC 16, named-module helper inclusion and four additional
Linux ABIs; emulated ABI runs do not qualify native timing or hardware MTE.

`module-exchange-frame-spans-hardware/results.json` completes eight serial
native/Wasm hardware Hotspots and microarchitecture collections at a requested
0.1 ms interval. `module-exchange-frame-spans-hot-loop-instructions/results.json`
retains sampled instructions for the actual copy and erase loops. The final
`module-exchange-frame-spans-hardware-10us/results.json` is also complete: four
explicit user-mode perf traces were imported into VTune, each verifying a
24,000-reference-tick period at the recorded reference frequency, nominally
9920.63 ns, with no lost or throttle records. This qualifies that reference
event period, not a uniform wall-clock interval or the full microarchitecture
counter set at 0.01 ms. The driver-device denial inside the existing cgroup
remains recorded separately; no run bypassed that group. All these runs use
`-O3 -g`, retain source/assembly reports, and show unchanged memory-event counts.

The 2026-11-16 Linux x86-64 qualification uses the binary SHA-256
`e83bb9f8aacd98edc64ffb01789ea8697944447d27c26e6112c545053f4c422a`.
Every production translation unit was compiled with `-O3 -g`. The retained
build manifest pins the original sources and verifies the linked debug sections.
All builds, tests, profilers and descendants used the existing 64 GiB cgroup
with swap disabled. These runs did not increase its memory-limit or OOM counters.

Receipts are retained under `uwvm2-ros-sdk/.tmp/ros-scene-security/`:

- `vm-module-exchange-observer/full-build-results.json`: full optimized build,
  source manifest, compiler arguments, binary hash and debug-section checks.
- `module-exchange-observer-vm/results.json`: 62 real guest cases across the
  interpreter and LLVM AOT, wasm32/wasm64 and memory indices zero/one. They
  exercise self/foreign reports, denial without output mutation, message
  transfer and erasure, wrong endpoint roles, overlap, cancellation, rejected
  nonshared/exported memory, hidden imports and function attenuation.
- `module-exchange-observer-dap-final/results.json`: actual VM, authenticated
  broker and DAP with full/read-only/limited permissions. Repeated metadata
  queries preserve a retained delivery's bytes and route counters. A separate
  live breakpoint verifies that attempting to start recording is refused and
  leaves execution, journal state, payload and route counters unchanged.
- `module-exchange-fp-disjoint/results.json`: retained native and ASan/UBSan
  tests of the unchanged closed-wrapper FP boundary, including 48 control
  profiles, 4,080 hostile callbacks and 96 recoverable allocation checks per
  variant. This component evidence is distinct from the new whole-VM receipt.
- `module-exchange-schema-qualified/results.json`: regenerated canonical
  catalogue shapes, the exhaustive 160-operation design ledger and existing
  robot-contract decoder regressions. Schema-table metadata is used by the
  standalone validators; it does not add guest authority or a new provider.
- `module-exchange-wire-qualified/results.json`: independent canonical record
  fixtures checked under ASan/UBSan and GCC, including truncation, trailing data
  and structural resource limits for the added module operations.

These checks qualify the stated paths on this host. They do not prove complete
ROS ecosystem portability, safety certification, native-driver fault tolerance
or worst-case control-loop latency.

## Unprofiled timing

The 2026-11-17 scoped-access/indexed-admission binary is
`536c3bac802977125c72d2a9104729c910f18a2cbda3f7c9634c9344ef2911fc`.
`module-exchange-indexed-paired/results.json` compares it with the preceding
robot-fault binary in a serial A/B/B/A sequence on CPU 2. Each of the three
observations per run uses 51 timed batches of **10,000 complete round trips**,
after 1,000 warm-up trips. The measured workloads use identical guest hashes;
compilation and simulation finished before timing began.

Medians of six run-level batch medians, before and after, were:

- 8 bytes: 1,705.35 ns to **1,111.61 ns**, a 34.8% reduction.
- 256 bytes: 1,867.53 ns to **1,302.49 ns**, a 30.3% reduction.
- 4,096 bytes: 4,944.11 ns to **4,383.31 ns**, an 11.3% reduction.

The intermediate scoped-access-only cohort measured 1,274.43 ns at 8 bytes
against its paired 1,673.75 ns baseline. These are controlled cohort comparisons,
not attribution of every difference between runs to a single source edit.
The final 8-byte lifecycle remains above one microsecond. Reporting it in
nanoseconds does not establish a sub-microsecond result or native equivalence.
The direct i64 reference measured 6.45/6.90 ns (before/after) and has different
security semantics. None of these batch means is an individual-request tail
latency, NUMA/concurrency result or hard deadline guarantee.

The final cohort passed 62 real Wasm cases (interpreter/AOT, wasm32/64, memory
zero/one), including fresh access after growth and rejection of changed memory
indices. Four DAP cases verified retained delivery and nonmutating full/read-only
views, limited-policy refusal and replay-recording refusal. Optimized Clang
ASan/UBSan and GCC tests verified inline/heap erasure and recoverable allocation
failure; indexed/string authority comparisons also passed TSan. Host wrapper
fault/FP-control tests passed sanitized and native builds. Named model/storage
modules and the isolated host export compiled and linked; that fixture is not
a qualification of the entire production C++ modules graph.

`module-exchange-indexed-mujoco/results.json` additionally retains 128 simulation
cases, 1,088 actual physics steps and 56 refusal cases with zero physics effects.
`module-exchange-indexed-component-crash/results.json` retains eight Wasm crash
observations and three DAP permission modes. These remain scalar simulation and
controlled native-library fixtures, not original vendor or hardware acceptance.
All cited cohorts used the existing 64 GiB, zero-swap cgroup with zero recorded
memory-limit/OOM events in this run. The actual host qualification is Linux
x86-64; wasm32/64 coverage is not coverage of other native CPU architectures.

`module-exchange-observer-baseline/results.json` records three runs per payload,
each with 101 batches of 1,000 complete round trips after a 1,000-round-trip
warmup. CPU affinity selects P-core logical CPU 2 on an Intel Core i9-14900HX.
Guest timestamps exclude loading and JIT compilation. Each round trip includes
submit, take, respond, result and acknowledge, with the required copies, checks
and erasure. The test uses WASIp1 only for its measurement clock and output;
message transfer itself uses ROSv2.

The median of the three batch-mean p50 values is 1.563 microseconds for 8 bytes,
1.788 microseconds for 256 bytes and 4.857 microseconds for 4,096 bytes. The
retained pre-optimization receipt, `module-exchange-clean-before/results.json`,
records 2.263, 2.536 and 5.615 microseconds respectively. The observations are
consistent with the closed FP-control guard and reduced repeated declaration
owner pinning improving this workload. They are separate runs, not a controlled
claim that the debugger fix itself caused the timing difference.

The direct i64 Wasm-call reference is about 6.7 nanoseconds and provides none of
the message isolation, admission, buffering or erasure contract. It cannot be
used to claim equivalent native performance. Batch percentiles are percentiles
of batch means, not individual-request p95/p99 or a real-time bound. Concurrent
routes, NUMA placement, scheduler contention and native workers need their own
deployment measurements.

The subsequent device/component fault fixes were checked separately with
`robot-fault-exchange-paired/results.json`. A serial before/after/after/before
sequence used the same CPU, guest workload and `-O3 -g` policy, after the native
simulation workloads had finished. The before binary is the qualification
binary above; the after binary is
`7486320ed5408b6c5b3d3275ce6bc4e8156c9a55d779f9b52c94537cf335e4e6`.
The medians of six run-level batch medians were 1.736/1.723 microseconds for
8 bytes, 1.956/1.909 microseconds for 256 bytes, and 4.995/4.999 microseconds for
4,096 bytes (before/after). This comparison found no material slowdown in this
workload. It does not equate differences between earlier measurements taken at
different times with a code regression, nor establish a worst-case bound or
native equivalence. The paired receipt retains all per-run results and hashes.

## Native production-carrier reference

`module_exchange_native.cpp` executes the production host wrappers, deployment
policy, authority, route state machine, FP guard, byte-atomic copying and full
owned-region erasure. Stable native arrays substitute for VM identity,
declarations, memory growth and memory tagging. This gives a closer security
reference than a direct i64 call, while deliberately retaining those differences.
It is not native interprocess IPC or native robot-application parity.

`module-exchange-native-checked/results.json` uses the FP-control VM
`09dad11ebf764980f3527d7f71b9e27cd21dbe1223c87f9e1bec22e8a766798b`.
Both references use `-O3 -g`, identical payload/checksum semantics and three
alternating native/Wasm timing rounds. Sanitized correctness and actual erased
storage assertions run before timing. Native/Wasm batch medians are
759.14/1,138.16 ns for 8 bytes (1.50 times), 989.65/1,313.60 ns for 256 bytes
(1.33 times), and 3,986.36/4,365.50 ns for 4,096 bytes (1.10 times). Each sample
includes all five operations: submit, take, respond, result and acknowledge.
The eight-byte result remains above one microsecond. This is one controlled
cohort, not a guaranteed tail latency or an industrial deadline bound.

`module-exchange-native-vtune/results.json` retains eight hardware profiles:
native/Wasm, 8/256 bytes, Hotspots/detailed Microarchitecture, each with source
and instruction reports. Actual reference periods are 240,000 cycles for the
requested 0.1 ms setting. The leading costs include router invocation,
retained-region overlap, authority admission, deployment lookup/locking,
byte-safe transport, secure erasure and the steady clock. A normal SIMD
`memcpy`/`memset` is not a valid substitute for byte-atomic guest accesses.

Reproduce with `tools/ros_scene/module_exchange_native_compare.py baseline` or
`profile`, supplying the selected full VM, matching frozen `--compile-argv`
and `--link-argv`, and a fresh `--output` directory. Always use
`in_existing_cgroup.py`, and complete compilation/simulation before timing.
The retained native fixture is a benchmark only; it adds no guest interface,
permission or native driver to the VM.

## Hardware periods, source lines and assembly

The 2026-11-17 before/after traces are
`module-exchange-carrier-before-hardware` and
`module-exchange-indexed-hardware`. Both use 101 batches of 10,000 complete
8-byte trips, a 24,000 reference-cycle period and the same explicit event set.
The optimized trace contains 117,532 reference samples at exactly that period
(about 9.921 microseconds of reference-counter progress), with no lost/throttle
records. Observed wall spacing is 10.966 microseconds at p50 and 11.874 at p95.
The baseline trace contains 188,507 reference samples. Both pass the period and
trace-frequency provenance audit; this evidence is independent of unprofiled
latency measurements.

For these equal-workload traces, attributed reference-event counts fell from
556.032 million to 106.344 million in caller identity lookup and from
568.512 million to 129.624 million in `memcmp`. These are statistical attribution
counts, not exact function-cycle instrumentation. The remaining leading entries
include router invocation, retained-region overlap checks, the once-per-call
declaration proof and checked byte transport. VTune retains function, source-line
and instruction reports tied to each cohort's frozen `-O3 -g` source.

`module-exchange-indexed-uarch` is a separate full Microarchitecture run at a
requested **0.1 ms**. It also retains source and assembly reports. Its multiplexed
metrics include startup/JIT and must not be treated as per-message hardware
counters or as a 0.01-ms full-microarchitecture result. The explicit 24,000-period
capture above supplies the audited approximately 10-microsecond evidence.

`module-exchange-observer-hardware/results.json` records explicit per-process
Linux perf events and imports the trace into the installed official VTune.
Intel documents both [command-line result import](https://www.intel.com/content/www/us/en/docs/vtune-profiler/user-guide/2026-0/importing-results-from-the-command-line.html)
and [Linux perf trace import](https://www.intel.com/content/www/us/en/docs/vtune-profiler/user-guide/2026-0/import-linux-perf-trace.html).
The named `cpu_core` events and actual periods are:

- `CPU_CLK_UNHALTED.REF_TSC`: 24,000 reference cycles.
- `CPU_CLK_UNHALTED.THREAD`, `INST_RETIRED.ANY` and
  `BR_INST_RETIRED.ALL_BRANCHES`: 240,000 events each.
- `BR_MISP_RETIRED.ALL_BRANCHES`: 10,000 events.

The same-host measured reference frequency is 2,419,200,230 Hz; the kernel's
trace time conversion independently agrees. All 190,778 reference-event samples
report period 24,000, corresponding to approximately 9.921 microseconds of
reference-counter progress. The observed wall-time spacing is 10.915 microseconds
at p50 and 11.829 microseconds at p95. No lost-sample or throttle records were
found. Interrupt latency, scheduling and event availability prevent a guarantee
of a fixed 10-microsecond wall interval. This is sampling evidence, not the
latency of a message.

The runner retains `perf evlist -v`, per-sample event periods/timestamps, raw
record diagnostics and the `.perf` trace. `hardware_sampling.py` rejects
clamped/frequency-mode events, missing evidence, losses and throttling; it also
checks reference-frequency provenance and agreement with trace time conversion.
The imported VTune result contains function, source-file/line and instruction
reports. The largest contributors include immediate-caller identity checks,
string comparisons/hashing, shared-memory erasure and checked memory copies.
Assembly reports identify the byte-store erase loop and owner-pin operations.
The remaining costs are not justification for removing identity or erasure.

The prior `module-security-profile-10us-fixed/explicit-perf/result` is **not** a
10-microsecond success: its `sep.events` and `perfcmd` show that a requested
24,000 period was raised to 100,000, approximately 41.3 microseconds on this
host. A successful profiler exit or a requested interval in context metadata
is insufficient evidence.

The full VTune `uarch-exploration` analysis failed at the requested 0.01 ms
configuration. The separate `module-exchange-observer-uarch-100us` run completed
at a requested 0.1 ms; its actual perf configuration contains a 240,000
reference-cycle period. Its multiplexed metrics and assembly localization are
separate evidence, not a 10-microsecond microarchitecture qualification.
Do not interpret its unusually low aggregate CPI as a definitive IPC result.

## Paired extent erasure

The `module-exchange-paired-erase-comparison/results.json` experiment compares
VM `d62905bcd7d580897dbf7b846421920d9737c93fac3faedf9d530d46da3e49ed`
with `fc3284ca60cdd28e95105d49787d1c1e4c6d5df6c926fc4220b38230238e03a5`.
Both are full `-O3 -g` builds. Two serial A/B/B/A cycles use identical guests,
CPU 2 and 51 batches of 10,000 five-operation lifecycles per observation.
The paired native references use matching source snapshots; their trusted
arrays still omit the VM lifetime/declaration/MTE machinery.

At 8, 256 and 4096 bytes, actual Wasm medians change from 1112.52 to
1057.81 ns, 1278.69 to 1259.88 ns and 4372.66 to 4344.75 ns respectively.
The reductions are 4.92%, 1.47% and 0.64%. Native medians remain approximately
749.32/750.83 ns, 981.37/979.85 ns and 3967.12/3974.21 ns (before/after).
The eight-byte gain repeats in each A/B/B/A cycle. These are batch-mean
medians of complete submit/take/respond/result/acknowledge lifecycles, not
single-call tails. The result still exceeds one microsecond and does not
establish native driver IPC or whole-robot parity.

The accepted change resolves the owner and pins the current memory backend
once for both the retained payload and its length slot. Each extent retains
an independent subtraction-based bound check. Invalid payload bounds do not
skip a valid length wipe, or vice versa; any failure quarantines the route.
Stores remain byte-atomic. Neither a guest pointer nor an identity/authority
cache survives the call. No guest import, permission or wire record changes.
The helper is shared by runtime and its direct bounds/concurrency fixture.

`module-exchange-paired-erase-core` and `module-exchange-paired-erase-bounds`
pass Clang ASan/UBSan, GCC and TSan. The latter checks overflow, independent
partial failure, an absent length extent, denied backend access, overlapping
extents and concurrent atomic writes. `module-exchange-paired-erase-fp-checked`
passes hostile-callback FP and allocation checks. Actual-VM receipts under
`module-exchange-paired-erase-vm-*` cover 82 security/memory cases, four DAP
cases and 58 observation/FP cases. The old source and candidate are retained;
`module-exchange-paired-erase-decision.json` records acceptance.

`module-exchange-paired-erase-qualification.json` joins **779 actual-VM cases**
against this exact binary and three separately identified core receipts. Its
integration checks include 340 exposure/refusal cases, 128 scalar MuJoCo cases,
37 CPU-model cases, 62 DDS cases and 24 DDS launch-failure cases. Four component
fault variants contribute 44 cases; module security, DAP and observation/FP
contribute 82, 4 and 58 respectively. These counts overlap interface families
and include refusal cases; they are not 779 independently qualified providers.
The DDS fixture uses the reservation-aware external backend with SHA-256
`0822ab3828459fc41d0459c6e86e3e3367930962f6bcc9649c20dd2f4baeafda`.

`module-exchange-paired-erase-hardware/results.json` completes eight hardware
profiles: native/Wasm, 8/4096 bytes, Hotspots/detailed Microarchitecture
Exploration. Each retains function, source-line and instruction reports.
The requested interval is **0.1 ms**, with a 240,000-reference-cycle period;
this is not a successful 0.01 ms collection. The earlier failed configuration
is retained. Leading short-message Hotspots include the router, region-overlap
scan, exact exposure/admission, memory identity and mutex paths. Individual
microarchitecture counters are multiplexed and include initialization/JIT;
their function-table order is not an exclusive per-message cost ranking.

## Finite-worker-ceiling safety candidate

The complete VM with SHA-256
`5b2ad6662f1412c87fd6ca319233fcd8f02148649cdc8443046462ad0c745e67`
adds checked finite worker limits and exact pre-exec readback. The safety
qualification is recorded separately in the
[worker lifecycle contract](ros-native-driver-lifecycle.md#exact-finite-resource-ceilings).
It does not deliberately change the steady-state registered-message path.
That does not make its performance equivalent automatically: binary layout
and observed timings must still be checked.

`worker-finite-limits-256-confirmation/results.json` retains a serial A/B/B/A
run, 101 batches of 10,000 round trips per observation. The prior enrollment
binary measures 1235.87 ns and this candidate 1303.96 ns at 256 bytes, a
**5.51% regression**. Earlier 8/256/4096-byte comparison medians are
1034.02/1237.02/4343.15 ns before and 1033.37/1279.40/4321.04 ns after. The
256-byte regression is therefore retained as unresolved performance evidence;
it is not dismissed as measurement noise.

The same-source native reference in
`worker-finite-limits-native-baseline/results.json` measures 708.93/934.09/3914.16 ns
at 8/256/4096 bytes; actual Wasm measures 1053.64/1308.96/4316.98 ns, or
1.486/1.401/1.103 times that reference. These are complete five-operation
registered-message round trips and distributions of batch means. The native
reference retains the same authority, routing and erasure semantics with
native storage; it is neither a direct function call nor external driver IPC.

`worker-finite-limits-microarchitecture/results.json` completes eight hardware
profiles: native/Wasm at 8/4096 bytes, each with Hotspots and detailed
Microarchitecture Exploration. These full analyses request 0.1 ms. Separate
explicit-event traces in `worker-finite-limits-hardware-10us/results.json`
verify a 24,000-reference-tick period, nominally 9920.634 ns on this host,
without recorded lost or throttled samples. Neither interval represents
individual message latency or a guaranteed fixed wall-clock sampling interval.
The focused before/after 256-byte Hotspots collection in
`worker-finite-limits-256-profiles/results.json` shows additional sampled time
in mutex unlock and several unchanged carrier functions. Those profiled timings
include profiler overhead and do not replace the unprofiled comparison.

The original `worker-finite-limits-assembly-comparison/results.json` was
invalidated: the symbol-name extraction returned zero instructions. Its runner
exit code is not accepted evidence. Address-bounded extraction subsequently
retained real instructions, but the intermediate
`worker-finite-limits-assembly-addressed/results.json` also had its equivalence
claims invalidated because its normalization masked constants. The replacement
`worker-finite-limits-strict-assembly-final/results.json` preserves constants
and symbolic targets: 19 of 20 selected functions match, with one unresolved
read-only-data label displacement retained as a difference. Function-address
changes alone are not a causal proof of the 256-byte regression.

## One traversal for carrier overlap checks

The candidate with SHA-256
`c1847c2503ab19d422ee0afbdeaad05423e34f7b38e9477a1bd7bd5c90ecafb3`
retains the finite-worker-limit fix and checks both input and output carriers
in one traversal of retained message regions. The same router mutex and
authority lease cover all four payload/length intersection predicates. The
second delivery check after charged admission remains in place. This changes
neither route registration nor grants, byte-atomic transport, full-extent
erasure, failed-erasure quarantine or incarnation ownership.

`module-exchange-paired-scan-core/results.json` passes the existing route,
incarnation, cross-route overlap, quarantine, cancellation and authority tests
with GCC, Clang ASan/UBSan and TSan. The actual VM passes 745 cases across
12 receipts: 82 module security, four DAP, 58 observation/FP, 340 import
refusal, 128 bounded MuJoCo, 55 component fault/stop, 37 model and 41
two-worker ROS/component cases. These are exact-binary regressions, not a
claim that every registered operation or real robot platform is qualified.

The serial A/B/B/A comparison against the finite-limit baseline uses 101
batches of 10,000 complete round trips. At 8/256/4096 bytes, medians change
from 1033.46/1287.24/4333.16 ns to 1006.19/1234.94/4280.09 ns, improvements
of 2.64%/4.06%/1.22%. A second direct comparison against the earlier enrollment
binary measures 1018.89/1219.04/4337.96 ns before and
994.06/1218.98/4306.62 ns after. Thus that comparison recovers the earlier
256-byte regression while retaining the new startup checks; it does not prove
an improvement for 256 bytes over the original baseline. Both are batch-mean
distributions, not per-request tail-latency guarantees.

The completed same-source native comparison in
`module-exchange-paired-scan-native-baseline-corrected/results.json` measures
687.33/916.80/3889.71 ns for native and 1031.08/1243.78/4296.20 ns for Wasm at
8/256/4096 bytes. The respective ratios are 1.500/1.357/1.105. These runs
use the same router and carrier security contract, with native arrays replacing
VM memory ownership machinery. They do not establish universal native parity.
The earlier `module-exchange-paired-scan-native-baseline` directory retains a
failed compile invocation and contributes no measurements.

`module-exchange-paired-scan-hardware/results.json` completes eight
Hotspots/detailed Microarchitecture Exploration profiles for native and Wasm
at 8 and 256 bytes. Every retained assembly report contains 417 decoded
instruction rows for the combined overlap function, 271 with source lines and
at least 229 with sampled events. Full analysis requests 0.1 ms; the separate
four traces in `module-exchange-paired-scan-hardware-10us/results.json` verify
the nominal 9920.634 ns reference-event period with no lost or throttle records.
Their observed reference-event median spacing is 10962/10931 ns for native/Wasm
at 8 bytes and 10916/10934 ns at 256 bytes. This is not a fixed wall-clock
interval guarantee. All profile receipts verify unchanged source and inputs.

Address-bounded comparison in `module-exchange-paired-scan-assembly/results.json`
retains the full optimized instructions: the combined `available` body grows
from 82 to 1642 bytes, replacing two calls into the retained 818-byte
`available_locked` helper. That helper remains necessary for final delivery.
The 1560-byte function-size increase is an explicit speed/size tradeoff. It
introduces no driver implementation, allocation, cached permission or payload
copy. The strict comparison tool preserves constants and resolved symbolic
targets; empty disassembly and unknown instructions cannot qualify equality.

The build preserves `-O3 -g` and the existing 64 GiB zero-swap gate. Its
memory.max counter rises from 5 to 550, with no OOM or OOM-kill event. The
compiler and unrelated processes share that gate. The resource pressure is
recorded and no memory limit was increased.

## Complete-frame writes under one memory proof

`ros_exchange_memory_access::write_frame` combines the native header and payload
parts into one contiguous checked destination. It uses the existing authenticated
exchange-memory owner and `with_host_access`: the entire destination, including
every software tag, is validated before either part is stored. The memory backend
remains pinned through both relaxed byte-atomic loops. There is no intermediate
payload copy, allocation, persistent guest pointer or cached authority. This is
not whole-frame atomic publication; the existing router owns delivery state and
full-capacity erasure. The separate length word retains its own access check.

The earlier `module-exchange-short-frame-experiment` used a 512-byte temporary
frame. Its native-only measurements were inconsistent at 256 bytes, so that
implementation was not adopted. The new two-part write does not use that buffer
or add a payload-size threshold. Internal runtime declarations and named-module
inclusion are updated; public imports, CLI policy and debugger fields are
unchanged.

`module-exchange-frame-memory-qualified/results.json` verifies the real memory
backends using Clang ASan/UBSan, GCC 16 and TSan. A tag error in only the final
payload granule leaves the header and all payload bytes untouched. Null storage,
size overflow, an out-of-bounds final byte, zero-length parts and concurrent
allocator growth are checked. GCC 16 is needed for the memory backend's C++26
pack indexing. Its existing best-effort allocator diagnostic emits an
`unused-result` warning, retained without promoting that unrelated warning to
an error. `module-exchange-frame-linux-five/results.json` adds IA32 native-loader
and AArch64/ARM soft-float/ARM hard-float QEMU checks. Emulation establishes
functional assertions only.

`module-exchange-frame-vm/results.json` contains 176 actual VM cases: eleven
payload lengths from zero through 4096 bytes, memory32/64, memory indices zero/one,
tagged/untagged storage and interpreter/AOT. Both endpoints use only ROSv2 host
imports. Each case completes two exchanges, checks every request and response
byte against distinct patterns, and verifies erasure of the full reserved
delivery extent, unused suffix and length word. These tests cover carrier
semantics, not arbitrary vendor protocols or physical control qualification.

The frozen VM is `vm-module-exchange-frame/uwvm2-ros`, SHA-256
`55b39735cc4201c6f87242ff85d2268a54a0dde288fd33c1890b63cc6a33e6cc`.
Its three compilation units use `-O3 -g`. The source snapshot isolates this
change from concurrent workspace edits. The module-only include guard and
implementation include are separately checked by
`module-exchange-frame-module-check/results.json`; that receipt is not a full
VM named-module graph build. The build's shared cgroup `memory.max` event count
increased from 831 to 1160, with no OOM or OOM-kill. The 64 GiB and zero-swap
limits were not changed.

## Reproduction

### Rejected operation specialization

The `module-exchange-specialized-vm-comparison/results.json` experiment compares
the communication-cleanup VM (`13ee3655695e8ec6af98d4cad83ecc76c99ec18bef089546eb2dcda1537c65f5`)
with a frozen candidate (`f72fde60f5a41f7fe4889aac5a2f0038e6ef210ce6d6a2a0edba2482c54c9602`).
The candidate replaces dynamic router selection with operation-specific template
instantiations while retaining every check and atomic erase. Both use `-O3 -g`.
The isolated CPU-2 experiment runs two A/B/B/A cycles, 51 batches of 10,000
complete lifecycles per observation, with no overlapping compilation or tests.

At 8, 256 and 4096 bytes respectively, native carrier medians change from
762.50 to 747.93 ns, 991.48 to 980.63 ns, and 3995.54 to 3973.17 ns. Actual
Wasm medians change from 1080.99 to 1090.76 ns, 1288.88 to 1279.94 ns, and
4396.28 to 4342.44 ns. Thus the important 8-byte path regresses by 0.90% and
the other measured improvements are only 0.69% and 1.22%. These are batch-mean
medians, not per-request percentiles or native robot-application comparisons.

Instruction inspection records fewer branches per specialization, but the five
emitted router bodies occupy 7049 bytes versus the original 3385-byte router;
the native fixture gains 3696 bytes of `.text`. The experiment is rejected:
it does not establish a consistent VM benefit. Live router/host code remains
unchanged. The frozen candidate, assertions, timing, assembly and rejection
decision remain available under the SDK evidence directory.

Use a fresh output directory for each run. The launcher joins the existing
resource gate before starting any compiler, guest, profiler or reporting tool;
it never increases the memory or swap limits. From the VM repository root:

For the paired complete-lifecycle comparison, after other test workloads exit:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 2 \
  /usr/bin/python3 tools/ros_scene/module_exchange_compare.py \
  --before ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-robot-fault-simulation/uwvm2-ros \
  --after ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-module-exchange-indexed/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/module-exchange-indexed-repeat \
  --iterations 10000 --batches 51 --direct
```

The profiling runner accepts repeated `--assembly-function` selectors for the
current compiled symbols in both explicit hardware-import and full analysis
modes. For the original observer cohort, the commands below remain historical
reproduction examples.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/module_exchange_performance.py baseline \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-module-exchange-observer/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/module-exchange-baseline-repeat \
  --cpu 2 --iterations 1000 --batches 101 --payloads 8 256 4096 --direct

/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/python3 tools/ros_scene/module_exchange_performance.py perf-import \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-module-exchange-observer/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/module-exchange-hardware-repeat \
  --cpu 2 --iterations 10000 --batches 101 --payloads 256 \
  --reference-context ../uwvm2-ros-sdk/.tmp/ros-scene-security/module-security-profile-10us-fixed/explicit-perf/result/config/context_values.cfg
```

The PMU and CPU choice are host-specific. The runner verifies that the selected
CPU belongs to the requested PMU and that the reference context names the same
host. The user configured perf sample-rate limits and loaded Intel's sampling
drivers; the working collection path requires no further privilege change.
