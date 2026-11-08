# WasmMTE extension integration

## Qualification status

This is an implementation and qualification record, not a release-readiness
claim. The reference instruction contract, parser, memory-owner shadow storage,
startup activation and ordinary interpreter/JIT paths are connected. Executable
VM tests have exercised scalar, SIMD, atomic, bulk, marker and selected WASI
buffer cases on x86_64, i686, AArch64, ARM soft-float and ARM hard-float.
Cross-platform execution under QEMU establishes functional evidence, not native
performance evidence.

Those results do not qualify the entire SDK, every language runtime, every
shared-profile API, concurrent allocation retirement, or every hot-update and
debugger path. Complete-VM integration runs have exercised shared interpreter/
LLVM operations, actual WASI thread spawning, crash policies and selected host
buffers on all five targets. Newly added host adapters require their own merged
regression; a passing earlier binary is not evidence for later source changes.
Hardware AArch64 activation and physical fault reporting also have separate
acceptance boundaries. In particular, signal-safe termination is not proof of
safe debugger attachment at an arbitrary hardware fault. Mandatory SDK
instrumentation remains gated on complete coverage and performance qualification.

The extension is not part of WebAssembly Core 3.0. The pinned references are:

- [LLVM mt-2](https://github.com/trcrsired/llvm-project/tree/3b5342c56f38dea910d9ffe1264add1aaa3bcd36).
- [WAVM mt-2](https://github.com/trcrsired/WAVM/tree/cd7107792bf69ffe62d245ab9479ec5421f13c75).
- [Author presentation to the WebAssembly Community Group, 2 June 2026](https://github.com/WebAssembly/meetings/blob/main/main/2026/presentations/2026-06-02-WebAssembly-Memory-Tagging.pdf).

## Acceptance and execution policy

The parser policy accepts the experimental instruction family by default.
`--wasm-feature-disable-memtag-extension` rejects it during validation.
`--runtime-disable-memtag` is the separate startup execution-policy switch:
its lowering removes tag operations and checks while retaining
ordinary address computation and required zeroing. Parser rejection also
implies no tag execution. A guest cannot change either startup policy.

The existence of these switches does not establish the outstanding coverage
listed above. Official Core-version presets must not silently change the
independent extension policy.
Cache keys, hot-update admission, imported-memory ownership and backend selection
must preserve the effective policy.

## Fault timing and reporting

Production enforcement must reject a mismatched access before it performs a
load, store or host-visible effect. Delaying the fault until a later system call
does not provide this guarantee: a bad store may already have changed data.
The current AArch64 activation requests the strict synchronous mode, not a set
of acceptable synchronous and asynchronous modes.

Apple describes MIE as using synchronous EMTE checks together with secure
allocators and tag-confidentiality protections. It does not describe a policy
that classifies individual accesses as critical/synchronous or
potential/asynchronous. Linux's asymmetric mode has a different, specific
meaning: reads are synchronous and writes are asynchronous. These properties
must not be conflated. See [Apple's MIE description](https://security.apple.com/blog/memory-integrity-enforcement/)
and [Linux's MTE fault modes](https://www.kernel.org/doc/html/latest/arch/arm64/memory-tagging-extension.html#tag-check-faults).

Expensive diagnostic rendering, symbol lookup or export can be deferred after
execution has been stopped. That is asynchronous *reporting*, not asynchronous
permission to execute a mismatched access. Such a reporting implementation must
preserve the first failure and its owning memory, must not resume the guest on
export failure, and must not retain raw live guest pointers for a background
reader. The present fatal diagnostic path is synchronous; this paragraph is a
design constraint, not a claim that an asynchronous reporter is implemented.

Software checks have another timing boundary: an atomic shadow read followed
by a data access is still two operations. A concurrent retag can fall between
them. Migrated shared-memory accesses now hold an owner-local tag lease across
both the range/tag proof and the actual byte operation; tag changes acquire that
owner's exclusive gate. Relocation pins precede tag leases, and operations
involving two owners acquire them in canonical owner order. Imported aliases
refer to the same gate.

A tag lease must not span a native I/O operation, a quota wait, debugger
notification, trap reporting or stack unwinding. Host input uses owned staging
storage and a fresh checked copy into Guest memory after input completes.
Failing that copy does not repeat the native input. This migration is not
complete for every ordinary WASI/ROS provider or shared-profile API.

These leases close the check/retag/access interval on migrated paths. They do
not supply allocation identity: reusing tag 6 after a 6-to-7 transition can
still admit an old tag-6 pointer. Finite-tag reuse and same-granule overruns
remain separate limitations. The successor design must address them before
mandatory SDK instrumentation can be qualified.

## Concurrency qualification

ThreadSanitizer is used as a separate native qualification configuration.
The installed x86_64 toolchain first reports a deliberately racy pthread control
with both writer stacks and accepts a correctly synchronized control. Relevant
VM translation units must all be rebuilt with matching TSan instrumentation.
The private shared-profile LLVM factory also instruments generated functions
after optimization and before the existing frame, call-graph and object proofs.
The test helper binds only validated sanitizer signatures. It does not expose
a new guest import or reuse persistent uninstrumented code.

Existing external LLVM, C++ ABI, unwind and cryptographic libraries are not
automatically instrumented by rebuilding the VM. Their coverage remains an
explicit limitation. A generic TSan pass sees the actual lowered operations:
ordinary Wasm shared-memory accesses lowered as host atomics do not become
guest-language data-race reports. Instrumentation must not replace those
operations with stronger atomics or invent synchronization merely to make
a test pass.

The actual VM campaign detected a lock-order inversion between full-source
reset and threaded retirement. The retirement paths now acquire the registry
before execution-domain maintenance, matching configuration and source reset.
Native joining remains outside both locks. Qualification must retain the old
failure, exercise the corrected actual paths and keep deadlock detection
enabled; the source edit alone is not a passing result. The corrected frozen
native build completed 100 actual CLI cases and 24 separate embedding cases
without a sanitizer warning. The embedding cases include 2,200 all-stop episodes
and physical worker retirement. Resetting those stop guards is not a complete
Runtime/source reset. Later production changes need their own qualification;
these observations do not transfer to a different source snapshot automatically.

A retained deterministic counterexample demonstrates the original
check/retag/access weakness: a thread can pass a tag check, another can retag,
and the first can then perform its stale data operation. Atomic shadow access
can leave that sequence free of C++ data races, so a clean TSan report cannot
establish lifetime protection. The owner lease and complete-operation helpers
address this interval on migrated paths; raw provider spans and tag reuse
require separate qualification.

The merged lease implementation has passed bounded interpreter/JIT, host-buffer,
metadata and DEV replacement regressions on all five targets. A separate native
embedding matrix exercises atomic wait32/wait64, both engines, and subsequent
scalar/atomic stale accesses. Each case observes an actual blocked native
futex wait, permits another Guest worker to retag, grow and notify the shared
memory, verifies the wait result, then observes the stale access fail and the
workers physically retire. These are eight actual VM cases, not a complete
threaded-memory64 or all-provider acceptance matrix.

Ordinary random_get and random_get_wasm64 also have real-Guest batch-boundary
checks, with ordinary and shared memory declarations, enabled/disabled tag
execution, denied interface/authority and explicit request quotas. The 1 KiB
and 64 KiB native staging sizes select transfer batches; they do not cap an
authorized Guest request. A separate native component schedules real entropy
input across a retag/grow transition and verifies that stale copyback is refused
without reading the entropy again.

Ordinary fd_fdstat_get and fd_fdstat_get_wasm64 now encode the complete
24-byte record in owned host storage and commit it with one checked copy.
This includes the socket metadata branch and deterministic padding bytes.
The final copy reports failure instead of returning success after a failed
tag check. Native concurrency tests cover file, stream and datagram resources;
actual Guest tests on all five hosts cover both address widths, both engines,
checking enabled/disabled, interior tag mismatches, denied calls and invalid
descriptors. These tests do not extend the claim to native socket payload I/O.

The [official Clang TSan platform list](https://clang.llvm.org/docs/ThreadSanitizer.html)
includes Linux x86_64 and AArch64, not i686 or ARM32. Executable tests on those
32-bit targets remain required, but cannot be presented as equivalent TSan
runs. Sanitizer timings are not release-performance evidence.

## Recompilation policy

Function replacement and checkpoint engine reconstruction must pass the
immutable startup execution policy to the fused validator/translator. Setting
the compile options object alone is insufficient when a path calls the lower
level emitter directly. The ordinary, replacement and staged-engine calls use
the same policy.

The maintained `test/0086.memtag/hot_replacement.py` suite drives a real host
debugging session, pauses an actual JIT caller, replaces an inactive callee
under that stop, and executes generation two. It covers memory32/memory64,
enabled/disabled checking, rejected mismatches and zeroing data effects. It
also rejects stale replacement generations. These tests do not by themselves
qualify a complete checkpoint world restore or signed release deployment.

The private threaded LLVM factory also keeps its actual owner-emission scope
active while inserting control calls. Those calls can introduce a late ROS
provenance bridge even when the original Guest body has no host call. Binding
the bridge after that insertion preserves the existing helper allowlist and
lets empty and numeric-only threaded bodies execute consistently under the
interpreter and LLVM backend. The five-host regression covers both tag
execution policies; it does not establish support for active data segments in
the threaded profile.

## Native layout and cached code

Adding the shared tag gate changes native owner layout. Native main, host API
and runtime objects must therefore be rebuilt together. Generated ordinary
memory code binds current data, length and shadow addresses as separate
relocatable symbols, or passes an opaque current owner to rebuilt helpers; it
does not embed native owner field offsets. Shared tagged code additionally
carries the lifetime code-generation identity. The private threaded LLVM
factory validates its helper signatures and call-site witnesses and does not
reuse the ordinary persistent cache.

Persistent cache origin also includes the actual executable digest. Different
VM executable bytes therefore use different authenticated cache contexts
independently of the narrower code-generation identity. The layout review is a
source audit; it must not be described as successful cross-version cached-code
execution.

## Binary contract

All instructions use the `0xfc` prefix and an unsigned LEB32 memory index.
Every stack operand and result uses the selected memory's address width, even
for status, tag values and lengths. Memory64 does not widen the memory-index
immediate. Unknown opcodes, malformed LEBs, missing memories and wrong stack
types are rejected.

The instruction inventory is maintained in
`src/uwvm2/extension/memtag/instruction.h`, outside the official Core opcode
tables. The family has 20 LLVM encodings: `0x20` through `0x2f`, `0x33`, and
`0x36` through `0x38`. The reserved holes remain invalid. The local WAVM reference
implements 19 of them.

The author's presentation, page 30, explicitly treats `memtag.add` and related
add-store forms as placeholders. The checked LLVM branch provides an encoding
and signature for `memtag.add` but does not establish an executable address/tag
arithmetic formula. No byte-versus-granule formula may be invented and described
as reference compatibility.

The presentation's historical `b16` convention rounds a byte count down to a
multiple of 16. The pinned WAVM branch instead tags every granule intersecting a
byte range, including its partial tail. These differ for non-granule ranges.
The low-level shadow checker deliberately checks **every intersecting granule**;
this access-safety primitive does not resolve historical opcode-store semantics
by itself.

The reference software geometry uses 16-byte granules, four tag bits at bit 28
for memory32, and eight tag bits at bit 56 for memory64. The memory32 encoding
therefore has a 256 MiB untagged address domain. This is a representation limit
of that experimental ABI, not an added quota on ordinary untagged Wasm memory.
Finite tags can collide, and same-granule overflows can retain a matching tag;
memory tagging must not be described as deterministic detection of every memory
bug or as equivalent to full AddressSanitizer coverage.

## Whole-range software checks

The standalone checker proves the full-width effective address and byte length
against the owner's current memory length before forming a shadow address.
It checks each touched granule, including interior granules in bulk operations.
A matching first and last tag does not authorize an unchecked middle range.
A failed tag-store bounds proof leaves all tags unchanged.

Ordinary shadows compare eight granules per unaligned-safe word read and
resolve mismatches to the exact first granule. Shared shadows use atomic tag
accesses and owner leases on migrated complete-operation paths. Atomic tag
reads alone do not make a concurrent free/access race safe, and the current
finite tag representation does not establish allocation-generation identity.

Tag state must belong to the backing memory owner. Imported aliases share that
state; separate memories and VM processes do not. All WASI and ROS descriptors,
nested pointers and buffers need a tag-aware range proof before normalization
or a host effect. Removing tags at host API ingress without checking them is
not acceptable.

## Qualified component boundaries

The owner lifecycle and bridge suites cover x86_64, i686, AArch64, ARM
soft-float and ARM hard-float. They exercise empty initialization, relocation,
growth, shared atomic tags, allocation/commit failures, error-path pin release,
entropy failures and fork handling. Native sanitizer checks supplement those
target runs. Tag entropy comes from the host OS independently of guest WASI
permissions; failure does not publish a partially tagged allocation.

The scalar LLVM proof has executed 47,776 cases on each target: widths 1–16,
every alignment, first/last granule failures and unchanged data after rejection.
Non-x86 target execution uses QEMU. This is qualification of generated checking
code, not a claim that all VM entry paths have been connected. Separate disabled
IR tests cover all 20 encodings at both address widths; the undefined placeholder
is rejected, and required zeroing remains.

## Global initialization

LLD emits a `memtag.globals` custom section: ULEB64 version `1`, ULEB64 entry
count, followed by ULEB64 address/byte-length pairs for memory zero. The decoder
validates the complete immutable payload before invoking a visitor. It rejects
integer overflow, truncation, impossible counts, out-of-bounds ranges and
trailing bytes without a fixed section-size ceiling.

Runtime initialization must additionally prove shadow capacity and settle
overlapping-granule ownership before changing any tag. The decoder's complete
input validation does not make arbitrary visitor callbacks transactional.
PIC, relocatable output, constant data, TLS and globals referenced by static
pointer initializers require separate coverage checks; the reference compiler
does not automatically instrument every category.

## Stack lifetime and disabled execution

The downstream compiler patch emits explicit stack-tag restoration using the
original alloca pointer after every relevant replacement of its uses. This
preserves a nonzero thread stack tag without making `memtag.untagstore` depend
on JIT constant propagation. Returns-twice, multiple lifetimes, early returns,
exceptions and nonlocal exits need separate execution tests.

The pinned compiler does not provide a complete Wasm tagged heap allocator.
The native compiler-rt HWASan/Scudo implementations are not a Wasm malloc/free
integration. Instrumenting stack allocations and globals alone therefore does
not establish heap use-after-free coverage. Each SDK allocator and language
runtime needs a real allocation, reallocation and deallocation tagging path.

All four zeroing forms must preserve their data-memory zeroing when tag
execution is disabled. The pinned WAVM implementation is inconsistent for
`randomstorez` and `untagstorez`; copying that inconsistency would change program
behavior. Disabled JIT qualification must compare generated IR and machine
code, not merely elapsed time.

## Hardware qualification

Hardware selection must depend on actual OS/thread support, enabled tagged
mappings and synchronous fault mode. A CPU feature name alone is insufficient.
Every executing guest thread must have the required policy. An invalid tagged
store must fault before changing the destination.

AArch64 QEMU can validate MTE instruction and synchronous-fault behavior. Such
runs are emulation evidence, not hardware acceleration or native performance
evidence. Native x86_64 VTune results describe software checks only. MTE
generations and eMTE require explicit capability evidence; unsupported features
must not be advertised as tested.

Linux synchronous MTE reports a tag mismatch before performing the offending
access. Asynchronous and asymmetric modes do not meet that requirement for
writes. Requesting both SYNC and ASYNC lets the kernel choose a mode and is
therefore insufficient. Linux also documents that kernel accesses to user buffers
are only checked on a best-effort basis, even in SYNC mode; VM host-buffer proofs
remain required. See the [Linux MTE userspace ABI](https://cdn.kernel.org/doc/html/latest/arch/arm64/memory-tagging-extension.html).

Arm's MTE4 feature names describe distinct capabilities, including canonical-tag,
no-address-tag and store-only checking. A generation label is not evidence that
every feature is available through the host OS, nor does store-only checking
satisfy the required read-and-write policy. Selection and tests must identify
the actual capability and ABI. See [Arm's feature descriptions](https://support.arm.com/documentation/ddi0487/mb/-Part-A-Arm-Architecture-Introduction-and-Overview/-Chapter-A2-A-profile-Architecture-Extensions/-A2-2-Armv8-A-architecture-extensions/-A2-2-10-The-Armv8-9-architecture-extension).

## Release acceptance

Acceptance remains contingent on complete scalar, SIMD, atomic, bulk and
host-interface coverage; memory growth and shared/imported memory lifecycle;
interpreter/JIT trap parity; debugger tag displays and diagnostics; hot-update
policy preservation; five-platform execution; actual Arm hardware performance;
disabled-mode code equivalence; and measured SDK-level overhead.

All language runtimes and allocators need an auditable instrumentation path.
Mandatory SDK MTE builds must follow those results, rather than replace them.
