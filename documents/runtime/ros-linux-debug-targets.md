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
