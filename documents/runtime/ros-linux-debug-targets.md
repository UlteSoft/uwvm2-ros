# Linux ROS debugger and update targets

The ROS product admits the following compiler ABIs. Selecting an architecture
does not grant guest interfaces, operations or management authority.

| Product architecture | Accepted aliases | Compiler ABI |
| --- | --- | --- |
| x86_64 | x64, amd64 | x86_64 Linux GNU, LP64 |
| i686 | i386, x86 | i686 Linux GNU, ILP32 |
| aarch64 | arm64 | Little-endian AArch64 Linux GNU, LP64 |
| arm | armsf | Little-endian ARMv7 Linux GNU EABI, base/soft-float PCS |
| armhf | armv7, armv7a, armv7-a | Little-endian ARMv7 Linux GNU EABI, VFP/hard-float PCS |

`xmake f -p linux -a <architecture>` validates explicit `target` and
`llvm-target` triples against this selection. The product rule also defines a
compiler-checked ABI marker: a host compiler accidentally used for a cross build,
wrong pointer width, big-endian ARM or mismatched ARM floating-point PCS fails
compilation. Cross builds still need the target's libc headers/libraries, compiler
builtins, and the product's paired libc++/libc++abi/libunwind and bundled LLVM.
A system LLVM or another target's archive is not a substitute. The current
provider contract is `23.1.1-uwvm-ros.18`, including the ARM epilogue-CFI capability.

The VM communicates with a separately deployed native ROS Gateway over the
existing typed protocol. Admission of a VM ABI does not assert availability of
every ROS distribution's native binary packages for that ABI.

## Stack walking and native execution

The full ROS runtime uses native Unwind for Wasm stack capture, including the
debugger's cooperative stack view and trap reports. Instruction-maintained call
stacks are disabled; the removed call-stack selector cannot re-enable them.
Activation identities and code-generation owners authenticate recovered frames;
they do not replace Unwind with an instruction-maintained backtrace.

On x86_64, i686 and AArch64 the paired unwinder consumes registered native unwind
information. ARM uses the paired EHABI unwinder and the owned dynamic
`.ARM.exidx` registry. The ARM lookup hook must really be exported and available.
Compiler-generated EHABI cleanup continuations resolve the exact paired
`__cxa_end_cleanup` helper privately; it is not a guest import or a global symbol
override. A second, bounded `.debug_frame` observer provides instruction-precise CFI for
native caller/return operations; EHABI call-site tables alone are insufficient
for a stop inside an epilogue.

The bundled ARM backend emits precise epilogue rows only for attributed VM
debug functions in A32 ELF code. Rows track each actual integer/VFP pop and
constant SP adjustment. A restored r11 is never treated as the old frame base.
TailCC may leave CFA below SP after popping incoming arguments; this state
allows restored-register evidence with an empty stack-read window. It never
authorizes reads from retired stack slots. Old stale-r11 rows, contradictory
saved slots, unqualified frame writes, Thumb/interworking, literal regions,
overflow, missing owners and retired generations continue to refuse. A32 NEON
vector destinations do not alias core r11. VFP reads through r11 without
writeback preserve the frame; VFP core-register transfers retain integer
destination checks. Structure-load/store base writeback
still invalidates r11 evidence when it targets r11; DMB/DSB/ISB do not change a
frame. These metadata facts do not grant instruction execution authority.

The built-in debugger has no unrestricted native-debugging launch option or
session command. Debugging VM, libc or dynamic-library code requires a separately
attached host debugger; a guest cannot request that attachment.

Native `si`, `ni` and `finish` operate inside the authenticated, stopped guest
JIT domain. Return continuations retain and recheck the actual worker, stop
revision, code owner, module generation and execution epoch. A root frame has
no guest caller to finish into. Timeout does not manufacture a returned frame
or release a still-running owner. Raw host stack pointers, VM memory and arbitrary
native code remain outside the public debugger. An admitted opaque host call
runs normally without instruction traps in its body and returns to an
authenticated guest continuation. A backend without that continuation proof
refuses before execution; explicit Wasm stepping can execute the authorized
import normally. Returning from a native stop to a cooperative Wasm stop
always retires the previous stop identifier and its derived references.

Native register pages expose the complete finite ABI layout: 48 entries on
x86_64, 18 on i686, 66 on AArch64 and 49 on each ARM ABI. Omitted/zero DAP page
count returns all remaining entries, including AArch64 V30/V31. This does not
expose native memory, stack/frame pointers or register writes.

On 32-bit glibc builds with `_FILE_OFFSET_BITS=64`, the private POSIX aliases
bind `mmap64` and `getrlimit64`, matching the system headers and argument/result
widths. Alternate signal-stack setup and stack-exhaustion handling retain their
guard pages and bounds checks.

## Interpreter and LLVM behavior

The product can explicitly include both engines with
`--execution-int=uwvm-int --execution-jit=llvm`; the default LLVM product selection
is unchanged. Runtime interpreter selection does not grant imports or operations
and rejects debugger activation.

Specified guest traps and uncaught guest exceptions terminate through the
trusted process-exit coordinator with status 126 in both engines. Internal VM
invariant failures remain distinct from guest faults. The ROS interpreter keeps
stable mmap memory while checking the complete committed access extent before
forming a pointer, including SIMD, Memory64 and atomic accesses. Guest memory
faults therefore do not enter a signal handler that locks or waits in the exit
coordinator. Stack exhaustion retains controlled status 127.

## Management separation

DEV requires explicit launch authorization and an authenticated control channel.
Its guest mutation, scripts and source replacement retain the approved guest
contract and quota/resource owners. They do not edit host permission tables.

RELEASE seals full debugging for that process lifetime. Its diagnostic endpoint
exposes bounded, redacted facts; it cannot pause, step, inspect live memory or
perform arbitrary mutation. Production updates use the independent `update`
command category and nonce/scope/measurement-bound Ed25519 permits. Both `debug`
and `update` are listed by `uwvm2-ros --help`.

These management channels are host capabilities, never Wasm imports. Enabling
WASIp1 or ROS does not expose them. Guest stdout and ROS payloads remain data.
Kernel peer credentials, per-message sender authentication and pinned process
lifetimes prevent a passed endpoint or foreign VM from becoming its supervisor.
A guest does not gain authority by knowing another process's PID, socket path,
nonce or a signed update frame.

Update support has the same transaction restrictions on all five ABIs: one owned
module, verified selected changes, exclusive drain before publication and no
rollback after authenticated COMMIT. Shared/imported state, multi-module
publication and ROS/native resource handoff remain explicitly unsupported.
An architecture gate does not relax these restrictions.

See [management authority](management-authority.md),
[DEV reconstruction](dev-world-rebuild.md),
[RELEASE diagnostics](release-diagnostics.md) and
[production updates](production-update.md) for the concrete interfaces and limits.

## Validation coverage

The revision-18 product was built for all five ABIs with the matching bundled
LLVM and paired libc++/libc++abi/libunwind. x86_64 tests run natively; i686,
AArch64, ARM and ARMhf tests execute their actual target ELF binaries under
QEMU user mode. Product and bundled-provider builds use the native x86_64 Clang
cross toolchain and the retained target sysroots; QEMU is not used to run the
product C++ compiler. Wasm JIT compilation inside an emulated VM remains part
of testing that actual target implementation. QEMU results are functional evidence, not physical hardware
or real-time latency qualification.

Every target exercises SDK-derived Wasm modules through the real VM, broker and
DAP: Wasm break/step and memory bounds, native `si`/`ni`/`finish`, recursive
64-argument calls, mixed i32/f32/f64/i64/v128 calls, stale stop/frame references,
root-return refusal and timeout containment. Trap and stack-exhaustion cases
check actual native Unwind and guarded termination. No instruction-stack
fallback is used.

Each target also executes 911 update protocol assertions, plus immutable-global
and module transitions, bounded migrations, signed policy transitions, restart
and managed-trap cases. Post-update traps retain the two exact guest frames
and explicitly report truncation before the host boundary. These results do
not claim an untruncated host backtrace.

Boundary tests supply a real attacking Wasm process with an inherited endpoint
and send bytes through WASI `fd_write`, testing both self-process and foreign-VM
debug/update routes. RELEASE write/step/script refusals and diagnostic redaction
are exercised independently. Valid update permits do not bypass sender identity.

Additional target tests cover O0/O3 ordinary and TailCC 64/1030-argument returns,
actual ARM EHABI cleanup/resume, same-object malformed CFI/relocation mutations,
SIMD/VFP frame-register writes, large-file POSIX ABI variants and wrong-product
compiler ABI rejection. Temporary drivers, cross sysroots, build products,
failed intermediate runs and machine-readable evidence remain under `.tmp`
and are excluded from version control.

The revision-18 combined-engine matrix also covers 65 Wasm/WASI programs per
engine and target, with the same 65 programs checked independently by the
installed Wasmtime 42 and 50 binaries. Separate checks cover 14 result/exit
parity cases per target, uncaught exceptions, interpreter debugger rejection,
all 92 registered WASIp1 signatures and 124 ROS signatures, module permission
isolation, positive operations, quota rejection/waiting and realtime-clock jumps.
Native ROS1/ROS2 subscribers verify actual payload delivery after gateway calls;
language examples execute as Wasm through the VM.

Current DAP regression coverage comprises 535 passing tests without skips.
Actual target sessions additionally check every ABI register page, native
breakpoint lifecycle, source stepping, guest-only instruction stepping across
WASI calls, opaque native next, and rejection of stale references after returning
to cooperative Wasm execution. The stop-identity fix is followed by a fresh
five-target debugger, Unwind, update and cross-process security regression.

All five targets pass the six extended Wasm debugger state suites: memory pages,
memory mutation, local pages, seven-scope pages, GC-member pages and frame/scope
lifetime. The seven-scope case covers more than one page of locals, operands,
saved parameters, controls, handlers, globals and table entries. On ARM/ARMhf,
its approximately 253,000 generated IR instructions take substantially longer
to compile under QEMU. The final test waits 840 seconds before attaching; the
VM and broker timeouts, authorization and scope bounds are unchanged. Earlier
startup timeouts remain recorded as failed attempts, not accepted passes.

## Native call and return parity

The five Linux ROS ABIs also run the SDK WAT math function through direct,
indirect, function-reference, tail and recursive calls using consecutive native
SI commands. A separate case for each into/over/out policy switches from a
native stop to Wasm stepping and back. These 40 sessions require real callee
entry and return, with no manual Wasm-step fallback for a refused native call.
Every successful step retires the previous native reference.

Guest SI authenticates the actual typed JIT target on both the hardware and
software trap backends. Opaque helper/import calls use the same sealed guest
fallthrough as NI; their bodies never become debugger stops. i686 near-call
register and memory operands use its actual register layout and 32-bit
effective-address arithmetic. Only equality with a registered typed slot
allows reading that slot.

Retirement checks use the actual platform ABI: i686 cdecl arguments remain
inside the current worker's CFI-proved frame, ARM joins r0/r1 for the 64-bit
activation identity and uses r2 for the exit kind, and AArch64 uses x0/x1.
The same execution, publication, worker, code-owner and stop checks surround
these reads. ARM/AArch64 debug retirement calls load a relocatable registered
symbol through private JIT data, so the debugger need not follow a loader
veneer. The cache policy distinguishes this emission.

A32 single stepping admits the exact LLVM DMB ordering form used by acquire
loads of typed entries. Other barrier encodings, DSB/ISB and other system
instructions do not gain this exception. A separate actual-LLVM MC test checks
52 positive and negative instruction/operand cases.

The call-parity change passes a further 12 live regression groups on every
target: SDK DAP, native finish/next, timeout retention, Unwind traps, stack
exhaustion, signed production updates, cross-process management rejection,
native register/breakpoint controls, source stepping/replacement, and SI/NI
across WASI imports. QEMU runs establish target correctness only; native
performance measurements remain restricted to x86_64.

The same final binaries pass 9,300 WASI/ROS namespace and permission assertions
across both engines, including module authority and wasm64 argument bounds.
Positive execution adds 80 WASI quota/path/clock cases and 20 ROS1/ROS2 gateway
sessions with actual native message witnesses. Realtime clock jumps do not
shorten monotonic quota waits. Sixty-five core Wasm/SDK programs run on both
engines on all five targets (650 runs), with 130 reference runs on the retained
Wasmtime versions. Fourteen exit/trap cases per target additionally compare
both engines' process outcomes. The DAP suite passes 535 checks, and all five
binaries expose the same 266 long command-line options.

The resource gate retains 64 GiB RAM and zero swap. Its inherited 64-task limit
initially prevented several QEMU workers from starting; the limit was raised
to 512 and those exact cases were rerun successfully. Original failures remain
in local evidence, separate from their successful reruns. No OOM kill occurred.

On native x86_64, five alternating baseline/fixed rounds of 100 requests give
median-of-round-medians of 0.193/0.194 ms for Wasm stepping, 3.673/3.680 ms for
disassembly and 0.830/0.830 ms for stack queries. These differences are below
1%; this parity fix makes no performance-improvement claim. The existing
official VTune installation also collected hardware Hotspots and
Microarchitecture Exploration profiles against the final binary. Function
hotspots resolve, but this release binary's source-line report is unknown;
it is not evidence of source-line attribution.

## Decoder reuse and boundary-scan costs

The debugger reuses one idle LLVM text decoder and one semantic decoder per
host thread. The complete triple, CPU, feature string, pointer width, byte order,
instruction maximum and alignment select the context. A lease removes the
context from the idle slot; nested requests therefore get distinct owners.
Only target data and LLVM machinery survive the request. Code bytes, PCs,
decoded results, captures, stop identities, generations and management
authority are never cached. Every operation still obtains and checks the
current owned code and stop.

Forward scans now ask the same target's LLVM MC decoder for instruction sizes
without formatting discarded prefix instructions. Only actual reply rows are
formatted, and their size must agree with that scan. The existing entry-to-stop
walk, literal masks, bounds, both branch successors, semantic vetoes and
current-owner checks remain in force. Negative windows still walk forward;
there is no reverse x86 resynchronization or remembered instruction boundary.

Native status labels qualified display-only source information as
"native-source", preserving the distinction from a cooperative "source" stop.
Previously a DWARF C function could emit both native and cooperative-looking
labels, causing the strict DAP parser to reject further assembly stepping.
The parser still rejects that mixed status and never uses native display
metadata to authorize source-value access.

On native x86_64, five alternating baseline/final rounds each measure 100
requests after five warmups. The VM and controller are pinned to separate
CPUs. Compilation, startup and profiling are outside these latency samples.
Median-of-round-medians for the SDK WAT loop are:

- Native single step: 0.650 to 0.245 ms, a 62% reduction.
- 128-instruction disassembly: 3.665 to 2.500 ms, a 32% reduction.
- 16-instruction disassembly: 0.860 to 0.590 ms, a 31% reduction.
- Native stack query: 0.830 to 0.563 ms, a 32% reduction.
- Wasm single step: 0.195 to 0.193 ms; no material change.

The retained Wasmtime 50.0.0 and GDB 16.3 execute the same DWARF SDK C Wasm
binary as the final VM. GDB/MI and the authenticated DAP client both measure
complete request/reply round trips. Final uwvm2-ros versus Wasmtime/GDB medians
are 0.513/0.289 ms for native single step, 0.886/0.345 ms for 16-instruction
disassembly and 0.360/0.117 ms for stack queries. The workloads differ in generated
machine code and authority: GDB permits host inspection and returns host frames;
uwvm2-ros proves and restricts guest ownership. The unmodified baseline cannot
complete this native C DAP scenario because of the status-label bug, so no
baseline speedup ratio is assigned to that scenario.

The existing WASI-SDK LLDB 22.1.0 also measures Wasmtime's remote Wasm debugger:
0.711 ms for Wasm single step and 0.151 ms for a stack query. Its SDK-derived WAT
loop additionally exports memory and contains debug names required by that
remote workflow. These are distinct debugger modes and frame representations,
not a general engine-throughput comparison.

The official VTune 2026.4 installation supplies hardware Hotspots and
Microarchitecture Exploration evidence. With the same 2,000 WAT disassembly
requests, sampled retired instructions decrease from 32.04 billion to
6.936 billion and VM CPU time from 3.393 to 1.081 seconds. A separate build
using the same optimized sources and release options plus line tables attributes
samples to debugger source lines; prebuilt LLVM code still has incomplete line
attribution. Native C stepping/disassembly are profiled separately. No QEMU
timing is presented as performance evidence.


The final rebuilt x86_64, i686, AArch64, ARM soft-float and ARM hard-float
binaries all pass the same regression matrix: eight call/mode variants and
13 live VM groups per ABI. These cover direct/indirect/reference/tail/recursive
calls, native/Wasm transitions, next/finish/deadlines, register and breakpoint
controls, source stepping, unwind traps, stack exhaustion, replacement,
foreign-process rejection and WASI host-call boundaries. The hot-update groups
contain 4,555 assertions in total; the new C/DWARF test performs 600 native
steps across the five ABIs and rejects stale frames after each step. x86_64
executes natively; the other four cross-built ELFs execute under QEMU for
correctness only.

Additional checks pass for 535 DAP tests, 52 actual LLVM MC instruction-form
assertions, 5,000 fresh-versus-reused decoder comparisons and 6,000 old-versus-new
window comparisons across the five targets. Cache key validation, reentrancy,
eight-thread separation and thread-local cleanup pass ASan/UBSan; builds without
an LLVM provider remain unavailable. The metadata/cache module partitions
compile independently; this is not a full product module-build qualification.
The entire run uses the existing 64 GiB, zero-swap cgroup with no OOM events.

## Continuation lifetime and long-session stepping

Native stepping no longer accumulates completed continuation owners in fixed
256-entry controller arrays. The controller retains the current call/return
operation. After authenticating the real stop, it consumes the worker's final
hardware-descriptor access acknowledgement, closes that exact descriptor and
releases the completed issuer proof. Software completion has already restored
its patches before publishing the stop. Backend plans/providers independently
own every callback context that remains live. Cancellation still requires
disable, actual worker acknowledgement and backend clear before retirement.

The four private weak registries for activation captures, native cursors, call
proofs and return proofs also no longer impose 256-entry limits. Registration
reuses expired slots and grows only when all existing slots name live objects.
Canonicalization still compares both the object pointer and its ownership
control block before dereference. Parent-return publication obtains both
registry slots before publishing either owner; allocation failure is caught on
the host path and grants no partial authority. No registry allocation or mutex
operation is added to signal callbacks. There is no cumulative call, step or
finish quota and no requirement to switch to Wasm mode to clear history.

This follows the operation-lifetime principle used by
[GDB's step-resume breakpoint cleanup](https://gnu.googlesource.com/binutils-gdb/+/d01e823438c7dc264d6885fbbfeace4d8955dcb7/gdb/infrun.c)
and [LLDB's completed step-plan breakpoint cleanup](https://github.com/llvm/llvm-project/blob/main/lldb/source/Target/ThreadPlanStepOverRange.cpp).
Their unrestricted host debugging policy is not adopted: all current Wasm
owner, generation, publication, trap and stale-reference checks remain.

The old controller limit was reproduced during a 12,000-request native C
profile: request 8,914 failed with continuation-owner capacity exhausted.
That collection is a failed run, not a successful latency sample. An initial
controller-only repair passed long call stepping but the 300-level recursion
test then exposed the separate registry limit after 255 native finishes.
Both failures motivated the complete fix above.

## Native C single-step cost attribution

The first-100-request measurements above describe that initial instruction
window. Three longer, unprofiled 6,000-step runs of the same SDK C Wasm on the
decoder-optimized baseline give uwvm2-ros medians of 0.339, 0.342 and 0.340 ms,
against Wasmtime/GDB medians of 0.299, 0.299 and 0.301 ms. Their median-of-medians
difference is about 14%, rather than the initial window's 77%. Both products
use the same CPU placement: the VM on logical CPU 21 and the client on CPU 20,
which are E-cores on this machine. Different generated instruction streams and
debugger authority still prevent interpreting this as equal guest work or
general engine throughput.

Hardware Hotspots sampling with stacks attributes 26.8% of sampled VM CPU
time to LLVM's X86GenericDisassembler::getInstruction. About 91% of that
function's samples arrive through instruction_size and decode_window_with:
forward decoding of the prefix needed to prove the current instruction boundary.
Other exclusive hotspots include current code-permission/provenance checks
(9.45%), LLVM instruction translation (8.66%), memset (6.3%) and memmove (3.94%).
The 6,000-step capture retires approximately 11.34 billion host instructions,
about 1.89 million per requested guest step, and samples 1.260 seconds of VM
CPU time. Inclusive call-tree rows must not be added to exclusive totals.

Microarchitecture Exploration's E-core aggregate reports roughly 50.3%
retiring, 26.8% front-end bound, 6.7% bad speculation and 16.2% back-end bound;
the memory-subsystem component is 3.6%. Together with the call stacks, this
points to instruction volume and decoder/front-end work as the main cost,
rather than primarily DRAM stalls. P-core rows are not meaningful for a
target pinned to CPU 21. This aggregate collection uses summary mode; it
does not provide per-source microarchitecture attribution. The separate
hardware Hotspots build includes release optimization plus source line tables.
Prebuilt LLVM and restricted kernel symbols still limit their line attribution.

An independently instrumented 6,000-step run observes exactly one broker
"step asm" command per DAP step. Mean adapter-to-broker time is about 264 us,
of which broker-to-VM exchange is about 219 us. Adapter request handling uses
about 71 us of process CPU, including about 15 us parsing status. These are
nested intervals from an instrumented run, not additive independent costs or
unperturbed latency results. They establish that repeated broker round trips
are not the cause; VM boundary/provenance work and Python protocol processing
both contribute.

The complete capacity repair passes the same 40 call/mode variants and 65 live
regression groups on rebuilt x86_64, i686, AArch64, ARM soft-float and ARM
hard-float products, including 4,555 update assertions and 535 DAP tests without
skips. A single native session additionally performs 60,000 assembly steps on
x86_64 and 30,000 on each other ABI: 180,000 stress steps in total. A separate
300-level recursive SDK-derived WAT performs 301 consecutive native finishes
per ABI, 1,505 in total, rejecting old frame/disassembly references after every
finish. Every stop must remain native; cooperative fallback is a failure.
The five products' hashes are checked before and after the runs.

All sampled stopped states retain seven process file descriptors. Native
x86_64 RSS varies by only 64 KiB over its 60,000 steps. QEMU process RSS includes
the emulator and is recorded separately; it is not a VM-only allocation
measurement. All work runs in the existing aggregate 64 GiB, zero-swap cgroup,
with no OOM events. Temporary fixtures, cross products and profiler data stay
under .tmp and are excluded from Git.

After all compilation and correctness work finishes, three fresh 6,000-step
rounds on the final native product produce median-of-round-medians of
0.342 ms versus GDB's 0.300 ms, about 14% slower. The corresponding means are
0.377/0.303 ms, P95 values 0.513/0.324 ms and P99 values 1.175/0.347 ms.
The tail is therefore materially farther from parity than the median.
The capacity repair does not claim a latency speedup.

Final-source VTune hardware Hotspots and detailed Microarchitecture Exploration
each complete 30,000 native requests. Source/function and hardware-event line
reports resolve the owned prefix walk in native_disassembly_window.h and the
provenance checks in uwvm_runtime_native_provenance_rows.h. Hotspots samples
6.022 seconds of VM CPU and 53.148 billion retired instructions, about 1.77
million per request. The x86 decoder accounts for 23.8% of VM CPU samples;
87.5% of its samples have the instruction_size prefix-walk caller. Code
permissions and LLVM translation each account for 7.5%, and memset for 8.0%.
Detailed E-core metrics are 46.8% retiring, 25.8% front-end bound, 9.0% bad
speculation and 18.4% back-end bound, including 4.0% memory-subsystem bound.
The default raw-stack capture skips some older frames; kernel symbols and
prebuilt LLVM source lines remain incomplete. Those gaps are not counted as
resolved source attribution.

A separate response-recording probe associates all 59 broker-to-VM exchanges
longer than 1 ms with callq continuation commands, among 174 calls in 6,000
steps. Their median exchange is 834 us, against 182 us for other instructions.
A further read-only /proc task sampler observes the VM control thread in
__wait_rcu_gp during x86_64 syscall 298 (perf_event_open), with three observed
RCU-wait spans of approximately 15--20 ms. This identifies a concrete off-CPU
source of the rare tens-of-milliseconds spikes, separate from CPU decoder work
and ordinary call-proof/event-setup overhead. Diagnostic instrumentation is
excluded from the unprofiled comparison. No global ptrace or kernel-symbol
restriction is changed. Reusing or modifying hardware events would require
preserving the exact cookie, queued-delivery and worker-retirement proof;
this repair keeps those checks intact.
