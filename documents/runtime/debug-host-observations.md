# DEV Host import observations

The authenticated debugger can record every genuine WASI Preview 1, ROS1 and
ROS2 builtin import invocation, including portable and RMW namespaces. The
record contains the ABI namespace/name, unsigned argument bits and widths,
observed normal return, elapsed monotonic time, module identity, and available
input/output byte segments. No import is added to the guest. These commands
use the existing DEV-only controller, private management channel, and current
complete cooperative stop; a native instruction stop or stale if-stop is
insufficient. RELEASE diagnostics cannot enable or read this full recorder.

## Commands

    trace host on [family=all|wasip1|ros1|ros2] [if-stop=N]
    trace host off [family=all|wasip1|ros1|ros2] [if-stop=N]
    trace host clear [if-stop=N]
    trace host read [family=all|wasip1|ros1|ros2] [after=N] [count=1..8]
    trace host bytes call=N [segment=N] [offset=N] [count=1..4096]

Full recording is off by default. The after cursor is an exclusive call ID;
next and more paginate records. Each call reports its segment count. Byte
replies identify the guest memory, offset, direction, total size, and next byte
offset. Bytes use hexadecimal encoding, including NUL, invalid UTF-8, and
terminal control characters. A page size is a transport limit, not a
payload/path limit. There is no fixed history or total payload capacity and no
silent ring overwrite. Clear explicitly releases history. IDs are not reused.
An in-flight scope retains its cleared record privately until it returns and
cannot republish it.

Module names longer than 255 bytes are explicitly marked as abbreviated in
metadata. Read the complete name with the special segment below; it returns an
escaped label page, not guest bytes. This preserves arbitrary name lengths
without exceeding the broker's per-message limit.

    trace host bytes call=N segment=18446744073709551615 offset=N count=N

The lightweight WASI and ROS metadata journals have independent switches:

    trace wasip1 on
    trace wasip1 off
    trace ros on
    trace ros off

The ROS switch is global; backend/module filters select displayed records,
not which backend that global switch enables. These existing lightweight
journals retain explicit retention/loss counters. To stop all Host diagnostic
collection, use all three off commands. User-configured security/audit sinks
are separate and retain their policy and error handling.

## Meaning and limits of observations

ROS segments are captured from the validated Host-owned buffers used by actual
guest-memory read/write operations. This includes capability alias input,
published/service/action/log payloads, successful result buffers, and published
length/handle fields. An input transfer does not by itself imply that a gateway
accepted or delivered that message: inspect the import's return and the existing
operation/resource audit. No gateway credential or native address is copied.

WASI segments are explicitly labelled **snapshots** of ABI inputs before the
call and successful output ranges after it. Iovec data is paginated independently;
read payloads use the returned transfer length. Paths, argv/environment buffers,
clock/random/stat/poll results and descriptor results follow their actual ABI
layouts, including the registered memory64 extension. Snapshotting validates
the complete range under the memory lease before allocating/copying. It never
dereferences an invalid guest address. A buffer modified concurrently by another
guest thread may differ from bytes consumed by the kernel; a snapshot is not a
kernel write receipt. A prestat name destination snapshot can include the
caller's unused destination capacity.

Scalar parameters/results remain visible when permissions deny an operation.
A rejected range increments invalid-ranges and marks the record incomplete.
An import with no observed normal return remains returned=0; it is not counted
as a successful return. Optional recorder allocation failures increment a
saturating loss counter, mark an existing record incomplete where possible, and
do not throw into, cancel, or change the original import result. Host memory
exhaustion and the containing cgroup still bound available resources. Builds
without recoverable C++ allocation exceptions refuse full recording.

## Execution cost

The full recorder's disabled path checks its channel flag before constructing
records, reading a clock, acquiring its mutex, or copying guest bytes. ROS
lightweight tracing now checks its own flag before timestamping, resolving
diagnostic capability labels, or appending diagnostic records. Ordinary
execution leaves that flag off. Debug-only WASI callbacks are initialized for
both direct debug launch and authenticated broker control, without redirecting
ROS guest stdio.

These changes do not add instruction-level tracing to ordinary interpreter or
JIT execution. Active debugging still incurs the existing breakpoint/safe-point
and controller costs, and enabled full recording necessarily costs time and
memory. Disabling a recorder avoids collection work; it does not promise a
literal zero-instruction flag check. Native benchmark evidence belongs alongside
the qualification results. QEMU results qualify functional behavior only.

## ARM unwind correction

The extended import stress test exposed an ARM soft-float crash while parking
for a debugger stop, before full recording was enabled. The native unwinder
resolved the generic personality through an out-of-range PREL31 relocation:
the intended Host address and the decoded address differed by bit 31. Ordinary
execution of the same module did not crash.

ARM ELF now retains a typed, object-local personality tail wrapper in every
MCJIT partition, including replacement generations. Its external declaration
binds only to the paired Host personality through the existing per-engine
binding checks. LLVM emits the executable full-address branch; the exception
table references nearby owned code. Finalization rejects a generic personality
outside authenticated owned code, and the ARM cache fingerprint excludes older
objects. Both soft-float and hard-float use this path. This adds no guest import,
debuggable Host-code range, or instruction-stack fallback.

The relocation range and expression are specified by
[Arm ELF ABI](https://github.com/ARM-software/abi-aa/blob/main/aaelf32/aaelf32.rst);
the table encoding is specified by
[Arm EHABI](https://github.com/ARM-software/abi-aa/blob/main/ehabi32/ehabi32.rst).

## Product qualification

The qualified Linux configurations are x86_64, i686, AArch64, ARM soft-float and
ARM hard-float. Foreign architectures execute under QEMU for correctness only.
After the ARM repair, all affected tests were repeated against the final ARM
ELFs. The other three architectures' three product translation units were
compared against the already-qualified source snapshot after preprocessing;
they are identical apart from source-root spelling and blank/edge whitespace.

The actual VM, authenticated broker and DAP adapter passed, on every target:

- Eight direct, indirect, reference, tail and recursive call/step variants.
- Thirteen debugger/update groups: source/DWARF, Wasm and native instruction
  stepping, next/finish, timeout, native controls, Unwind traps and stack
  exhaustion, host-boundary refusal, cross-VM refusal and hot-update protocol.
  Hot-update qualification contains 911 assertions per target.
- Rooted GC function-reference and tag/payload replacement, invalid replacement
  refusal, stale generation refusal and immediate invalidation of old DAP IDs.
- Full observations on/off, stale-stop refusal, binary bytes including NUL and
  invalid UTF-8, and complete pagination of a 70,000-byte random result.
- Real ROS1 and ROS2 gateways and native subscribers: published bytes were
  compared with what the native subscriber received, together with alias input
  and issued-handle output.
- One invocation/return record for each of 90 returning WASI ABI entries
  (wasm32 and the registered memory64 extension) and all 124 registered
  ROS/client/RMW/portable entries. The two proc_exit entries are excluded from
  this returning-call loop; separate exit/denial tests cover termination.

The 65-case Wasm feature/arithmetic/trap matrix passed in both interpreter and
LLVM modes on all five targets and in the retained Wasmtime 42 and 50 binaries.
It covers SIMD, GC, function references, tail calls, exceptions, memory64,
multiple memories, atomics and bounds/trap cases. Fourteen additional
interpreter/LLVM parity cases cover typed/null indirect calls, casts, arithmetic,
effective-address overflow, proc_exit and stack exhaustion. This is a concrete
regression matrix, not a claim that it exhausts the Core 3 specification.

Native ASan/UBSan component tests cover 12,000 concurrent records, 100,000-byte
payload/name pagination, nested scopes, clear during an active call, injected
allocation failures and 148 real WASI IO checks. All 535 DAP unit tests pass.
ARM/ARMHF component checks verify the real provider ABI, typed local
personality, must-tail forwarding and repeatable declaration identity.

An existing safety restriction remains: debugger writeMemory is refused when
the execution closure includes foreign/native imports that cannot safely lend
their state. The real ROS tests verify that refusal; ordinary guest writes,
payload observations and message delivery continue to work.

## Native benchmark evidence

Baseline: commit 426328d7a, with its exact ELF retained before these changes.
Measurements use the existing Intel VTune 2026.4 installation and retained
Wasmtime/GDB, native x86_64 only. VM threads are pinned to logical CPU 21
(an E-core), controllers to CPU 20. Compilation and correctness workloads
finish before timing. Generated workloads derive from SDK WAT static_math and
the SDK C static_pointer_library example.

Interleaved normal-run tests discard one warm-up and retain seven samples per
binary. For 3,000,000 ABI calls, ROS1/ROS2 LLVM-mode median elapsed time falls
from about 1.09/1.10 seconds to 0.721/0.722 seconds (34% lower). Interpreter
ROS call medians also improve. WASI clock-resolution calls differ by -0.35%
in LLVM mode and +0.35% in interpreter mode. These are specific ABI workloads,
not a throughput claim for all ROS message types.

Pure-interpreter timing is noisy: the short run showed +21.6%; increasing the
loop to 300,000,000 iterations reduced the ratio of medians to +4.4%.
Paired ratios have median +2.8% and a descriptive bootstrap interval spanning
roughly -4.0% to +16.8%. These results do not establish a stable regression or
literal zero overhead. The original and extended samples are both retained.

Three rounds of 6,000 native steps produce median-of-round-medians:

- Previous VM: 0.341821 ms/step.
- Current VM, default diagnostic switches: 0.340392 ms/step.
- Current VM, all three Host diagnostic switches off: 0.339503 ms/step.
- Wasmtime 50 with GDB MI: 0.298729 ms/step.

The current disabled-recording path is about 13.7% slower than this GDB
round-trip baseline. The comparison does not equate security models: uwvm
authenticates guest-only provenance and stop/generation ownership. Wasm-step,
stack and 16/128-instruction disassembly medians remain within about 2% of the
previous binary in these runs. No boundary or ownership check was removed.

Authenticated hot updates use 128 SDK-derived functions, 24 alternating
commits, and execution checks for all changed functions after every commit.
Changing one function gives prepare/commit medians of 5.084/0.164 ms versus
5.057/0.163 ms before; changing 32 gives 414.845/0.193 ms versus
415.304/0.192 ms. Signing and startup are outside these intervals.

VTune hardware hotspots and detailed uarch collections each complete 30,000
native steps on an O3 build with source line tables. The main sampled costs are
LLVM instruction decoding (24.8%), provenance byte classification (9.8%),
memset (8.0%) and LLVM instruction translation (7.0%). About 94% of the sampled
getInstruction caller attribution leads through instruction_size, consistent
with scanning earlier instructions to locate a window boundary. The E-core
breakdown is 47.9% retiring, 25.0% frontend bound, 8.9% bad speculation and
18.2% backend bound, including 3.9% memory-subsystem bound. No P-core execution
was sampled; its derived zero-denominator percentages are not used.

VM source lines and hardware-event reports are available. Prebuilt LLVM lacks
some source line information; kernel symbols are restricted and some sampled
stacks have skipped frames. VM thread affinity was verified independently
because driverless attach does not honor VTune's CPU-mask option. No kernel
settings were changed.

All logs, exact ELF hashes, raw timing samples, debugger transcripts and VTune
results remain under .tmp/debug-observation/final. They are excluded from Git.
The existing 64 GiB/zero-swap cgroup reports no OOM or OOM-kill event.
