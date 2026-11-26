# WasmMTE extension integration

## Qualification status

The current wire is deliberately incompatible with historical mt-2 output:
only `FF ULEB32(version/domain/operation) ULEB32(memoryidx)` and
`uwvm.memtag.linear.globals` version 2 are accepted. Rebuild guest modules and
their libraries. Historical numerical results below describe the snapshots in
which they were measured; they do not qualify the new packed wire or subsequent
fault-policy changes without rebuilding and rerunning the corresponding tests.

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

A WasmMTE memory declares WasmADM and at least four ignored address bits.
The parser accepts both extensions by default. The host can disable each one
through a Wasm module policy.

In a memory import or definition, the ADM flag `0x10` adds one raw byte
immediately after the flags. It gives the number of ignored high bits, before
the encoded page limits. The MTE flag `0x20` requests tag storage. Custom page
sizes use flag `0x08` and append a u32 LEB page-size exponent after the limits.
[Address masking and linear memory reservations](adm-memory-reservation.md)

`--wasm-policy-global runtime.memtag-checks off` changes the default for ordinary access comparisons.
The accessing module can override it with `runtime.memtag-checks on|off`.
Both settings retain address masking, bounds, explicit tag operations, zeroing
and global tag initialization. The defining module selects the backend when
its memory is initialized. Imports refer to that same owner and tag directory.
[Module policy keys and precedence](../command-line/wasm-module-policy.md)

## Fault timing and reporting

With the default `--runtime-memtag-action crash`, `--runtime-memtag-mode sync` rejects a mismatched access before
performing the load or store. A selected physical Arm owner requests synchronous
faults and verifies the effective thread policy.
This is the mode required when the deployment contract forbids a rejected
store from changing its destination.

With crash reporting, `--runtime-memtag-mode async` can deliver a fault after
the offending access, including a write, has executed. Execution then enters
terminal fault handling. Ordinary
linear-memory bounds checks and checked host-buffer transfers remain immediate
in all modes. Pending tag faults are drained at the instrumented execution and
host-effect boundaries; entering terminal handling still follows the immutable
launch-selected fault action. See [Terminal faults](../command-line/crash-policy.md).

Apple describes MIE as using synchronous EMTE checks together with secure
allocators and tag-confidentiality protections. It does not describe a policy
that classifies individual accesses as critical/synchronous or
potential/asynchronous. Linux's asymmetric mode has a different, specific
meaning: reads are synchronous and writes are asynchronous. These properties
must not be conflated. See [Apple's MIE description](https://security.apple.com/blog/memory-integrity-enforcement/)
and [Linux's MTE fault modes](https://www.kernel.org/doc/html/latest/arch/arm64/memory-tagging-extension.html#tag-check-faults).

Deferred metadata delivery and diagnostic export are also distinct from the
fault timing policy. The implemented software delivery pool preserves copied
access evidence and retains stream ownership until delivery completes. It does
not move symbol lookup, arbitrary debugger callbacks or a dump of live Guest
memory to a worker. Rendering and export failures never permit Guest execution
to continue past a terminal fault. The first-fault host event is independent of
stderr; text delivery to a blocking live receiver has no hard completion
deadline, as documented in the terminal-fault contract.

Software reports retain the available coordinates captured at the access.
Synchronous hardware reports can retain a validated memory index and granule,
but a native virtual address is not a complete Guest pointer. The hardware tag
mask is `0x0f`, and a physical allocation tag read after the fault is labelled
as a post-fault snapshot. Hardware asynchronous notifications without an
attributable address or instruction leave those fields unavailable. The site
that delivered a deferred notification is never presented as the fault site.

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

Owner-created shared shadow views also acquire the owner's exclusive tag gate
for tag stores. A direct borrowed-view write therefore cannot bypass an active
checked access. Complete MTE instructions use the internal already-leased view
while retaining the gate through both tag publication and zeroing. Private
(non-shared) views introduce no locking. Raw tag-array views remain caller-owned
and do not prove native memory lifetime; a standalone `check` is not an access
lease. The deterministic `logical_retag_window.cc` regression covers direct
owner-view and actual instruction retags for both address widths.

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

All instructions use exactly one private primary byte, `0xff`, followed by
one unsigned LEB32 instruction word and one unsigned LEB32 memory index:

```text
FF ULEB32((version << 24) | (domain << 16) | operation) ULEB32(memoryidx)
```

The high eight bits are version 1; the next eight bits are linear-memory domain
1; the low 16 bits are the operation. No GC tagging domain is defined; domain 2 is rejected like every other unknown domain.
There is no vendor integer, second extension prefix or legacy decoding mode.
Unknown versions, domains, operations, reserved operations and malformed u32
LEBs fail validation. Legal padding up to five bytes is accepted. The existing
feature policy for padded/nonzero memory-index immediates still applies.

For example, `memtag.status 0` is `FF A0 80 84 08 00` and
`memtag.store 0` is `FF AB 80 84 08 00`. The old `FC 20 00` and abandoned
FF/vendor multi-field grammar are rejected, including in unreachable and unused
function bodies. Standard FC numeric, bulk-memory and table operations are
unchanged. The private prefix is not a standardized Core extension registration.

Every stack operand and result uses the selected memory's address width, even
for status, tag values and lengths. Memory64 does not widen the memory-index
immediate. Unknown opcodes, malformed LEBs, missing memories and wrong stack
types are rejected.

The instruction inventory is maintained in
`src/uwvm2/extension/memtag/instruction.h`, outside the official Core opcode
tables. It defines 19 executable reference instructions (`0x20–0x2f` and
`0x36–0x38`) and five UWVM managed-allocation instructions (`0x40–0x44`).
The decoder rejects reserved descriptor `0x33` and unassigned operation numbers.

| Operation | Mnemonic | Stack inputs, deepest first | Result | Allocation-tag / data effect |
|---|---|---|---|---|
| `20` | `memtag.status` | none | status | Query execution mode |
| `21` | `memtag.extract` | pointer | tag | Extract pointer tag |
| `22` | `memtag.insert` | pointer, tag | pointer | Replace pointer tag |
| `23` | `memtag.tagbits` | none | bit count | Query tag geometry |
| `24` | `memtag.startbit` | none | starting bit | Query tag geometry |
| `25` | `memtag.copy` | pointer, tag-source pointer | pointer | Copy pointer tag |
| `26` | `memtag.sub` | pointer, pointer | difference | Address difference |
| `27` | `memtag.load` | pointer | pointer | Read allocation tag |
| `28` | `memtag.untag` | pointer | pointer | Clear pointer tag |
| `29` | `memtag.untagstore` | pointer, byte length | pointer | Clear allocation tags |
| `2a` | `memtag.untagstorez` | pointer, byte length | pointer | Clear tags and zero data |
| `2b` | `memtag.store` | pointer, byte length | none | Store pointer's tag to allocation |
| `2c` | `memtag.storez` | pointer, byte length | none | Store tag and zero data |
| `2d` | `memtag.random` | pointer | pointer | Generate pointer tag |
| `2e` | `memtag.randomstore` | pointer, byte length | pointer | Generate and store tag |
| `2f` | `memtag.randomstorez` | pointer, byte length | pointer | Generate/store tag and zero data |
| `33` | `memtag.add` | pointer, address offset, tag offset | reserved | Rejected; no invented arithmetic |
| `36` | `memtag.hint` | pointer, tag-source pointer, tag offset | pointer | Derive pointer tag |
| `37` | `memtag.hintstore` | pointer, byte length, tag-source pointer, tag offset | pointer | Derive and store tag |
| `38` | `memtag.hintstorez` | pointer, byte length, tag-source pointer, tag offset | pointer | Derive/store tag and zero data |
| `40` | `memtag.claim` | base, byte length | tagged pointer | Acquire and color an available extent |
| `41` | `memtag.claimz` | base, byte length | tagged pointer | Acquire, color and zero an extent |
| `42` | `memtag.reserve` | base, byte length | reserved pointer | Reserve a color for later activation |
| `43` | `memtag.activate` | reserved pointer, byte length | tagged pointer | Publish the reserved color |
| `44` | `memtag.release` | registered pointer, byte length | base | Cancel a reservation or release an active extent |

### Managed linear allocations

With MTE enabled, a new extent must start on a 16-byte boundary, have a positive
multiple of 16 bytes, fit the selected memory, and present a zero-tagged pointer
matching every granule. Extents within an existing allocation, including the
nonzero backing region of a pthread stack, are rejected. Pass the original
pointer and complete extent when activating or releasing. Registered manifest
globals have permanent ownership. Guest code can read the allocation tag of a
global and use it to access the object. It cannot release its registration.

The memory owner holds one directory shared by imported aliases. Its exclusive
tag lease covers checking, selecting a color, registering the extent and writing
the shadow and any active hardware tags. Each live registration consumes space
from the VM's memtag pool. The VM reports an allocation rejection when that
budget is exhausted. Entropy is obtained outside the relocation and tag locks.
Each retry examines the current directory and shadow again. A failed operation
preserves data, shadow and registrations.

Colors exclude zero and the tags of extents on the same backing memory whose
nearest edges have a gap of 0 through 64 bytes, inclusive. Selection examines
each of the five neighboring granules on either side, plus reservations. An
AArch64 owner configured for physical MTE excludes matching low four bits as
well. Beyond that radius, tags may coincide. A later allocation can reuse a
released tag. This policy detects some stale-pointer accesses and gives no
cryptographic authority within shared Wasm memory.

A checked access through a reserved pointer fails its tag comparison until
activation. The original base remains usable in that interval. Activation and
release preserve the bytes. A successful `claimz` initializes them to zero.
Tag-writing operations reject overlaps with a registered extent and conflicting
colors on a registered neighbor.

Turning ordinary access comparisons off preserves the five managed operations,
their overlap and ownership rules, and their initialization effects. A full
checkpoint records each tagged owner's shadow, active allocations, reservations
and permanent global ranges. Restore binds this state to the same memory layout
and aliases before publishing the owner. The receiving host selects a backend
that can reproduce the saved tag field. Restoring to a fresh VM requires the
matching modules, an admitted debugger stop and the usual host resource bindings.

Older full checkpoints and the separate thread-history origin format carry
only shadow bytes. Enabled MTE admits those formats for sources whose typed
instructions and global descriptors require no managed allocations.
The thread-history origin binds the original layout, physical backend and caller
checks. Restoring a tag-only full checkpoint also checks replacement bodies for a
possible managed instruction. Ordinary cold start installs the manifest
registrations on the canonical memory owners.


Normalization happens during validation and translation. Interpreter handlers,
LLVM memory checks and shadow storage use the same internal operation identities;
they do not decode version/domain on every load or store. Debug listings retain
`uwvm.linear.v1::` provenance. The persistent LLVM cache includes an explicit
`ff-packed-u32-v1-linear1-globals2` wire witness in addition to existing policy
and executable identities.

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

The guest address has width W (32 or 64 bits). The memory's declaration supplies
the number k of ignored high bits. An access clears those bits before computing
the effective range. An MTE owner uses exactly four of the ignored bits as its
tag, with one allocation tag per 16-byte granule.

| New owner | Declared ADM bits | memory32 guest field | memory64 guest field | Native tag field |
| --- | --- | --- | --- | --- |
| Software | k ≥ 4 | [31:28] | [63:60] | Software shadow |
| Arm MTE | 4 ≤ k ≤ 7 | [31:28] | [63:60] | [59:56] |
| Arm MTE | k ≥ 8 | [27:24] | [59:56] | [59:56] |
| Reserved Intel MTT | 4 ≤ k ≤ 6 | [31:28] | [63:60] | [60:57] |
| Reserved Intel MTT | k ≥ 7 | [28:25] | [60:57] | [60:57] |

Bit 0 is the least significant bit. The interpreter and LLVM extract the
owner’s four guest tag bits into the native tag field after address checking.
The direct memory64 field already occupies the native bit positions.
Memory32 always places its tag in a 64-bit native pointer. An Intel MTT request
requires a future CPU/OS backend with checked mappings, allocation tags and
fault delivery. These guest declarations are independent of the SDK instrumented
release ABI, `wasm64_adm16`.

The declared ADM count k sets the address domain to 2^(W−k) bytes. Memory32
ADM4 has a 256 MiB domain and ADM8 has a 16 MiB domain. Tagged memory64 keeps
its existing native mapping limits. `memtag.tagbits` and `memtag.startbit` report
the owner’s guest field. Imports, child memory copies, growth and same-owner
updates preserve it. A full snapshot stores the original start s, with
W−k ≤ s ≤ W−4. Either software or Arm MTE can restore any such field while
preserving the guest pointer values. For example, memory64 ADM16 can restore
bit 49 as its guest start and move those four bits to native [59:56] on Arm.
Finite tags can collide, and two objects can share an addressable granule.

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

Tag state belongs to the backing memory owner. All aliases use it. Host buffer
transfers apply bounds checks and the calling module's tag-check policy before
normalizing pointer values or performing a host effect. This applies to nested
pointers as well as the outer descriptor.

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
code, not a claim that all VM entry paths have been connected. Those historical suites predate the per-owner ADM geometry and module check controls.

## Global initialization

The sole accepted custom-section name is `uwvm.memtag.linear.globals`. Its
payload is ULEB64 version `2`, ULEB64 entry count, then that many triples:
ULEB32 module-local memory index, ULEB64 address, ULEB64 byte length. Memory64
does not widen the memory index. The old `memtag.globals` name is explicitly
rejected with a regeneration diagnostic, even with ordinary access comparisons disabled.
There is no v1 parser/visitor or fallback to memory zero. Duplicate new sections
also fail admission.

Each memory index resolves to its canonical Wasm owner before tags can change.
Imported aliases and separate modules can therefore name the same owner; their
granule ownership is checked together. Independent descriptors may not overlap
even one granule, including byte-disjoint ranges sharing a granule. Admission
sorts the ranges in O(n log n) time and then checks adjacent owner-local ranges,
avoiding quadratic rejection behavior. Empty descriptors make no allocation-tag
claim; an empty descriptor list is valid even in a module with no memory.

The initializer reserves colors for every permanent global before publishing
the first shadow change. A reservation excludes other objects on either side
with at most 64 intervening bytes, including descriptors from another module
importing the same memory. The selection sees both occupied shadow granules and
unpublished reservations. Among the remaining colors, it samples uniformly
from the colors used by the fewest permanent objects on that owner. Hardware
MTE counts colors by their low four bits. Each allocation minimizes the number
of new same-color permanent pairs for the current set of legal colors. Distant
objects can still share a color. Managed heap and stack allocations continue
to sample over all legal colors. A private thread instance uses its own
directory. Threads borrowing a shared memory reuse the existing owner's
directory.

A signed memory reset stages the replacement tag image and its permanent
directory with the same selection policy while execution is drained. A tagged migration may copy a complete
permanent extent to a matching permanent extent. It must preserve one color
across the whole destination and the required distance from its neighbors,
including raw colored padding. Partial transfers and copies of dynamic lifetimes
or outstanding reservations require a format carrying those ownership records.
Admission rejects such copies before publication.

The decoder
validates the complete immutable payload before invoking a visitor. It rejects
integer overflow, truncation, impossible counts, out-of-bounds ranges and
trailing bytes without a fixed section-size ceiling.

Runtime initialization must additionally prove shadow capacity before changing
any tag. The decoder's complete
input validation does not make arbitrary visitor callbacks transactional.
PIC, relocatable output, constant data, TLS and globals referenced by static
pointer initializers require separate coverage checks; the reference compiler
does not automatically instrument every category.

The downstream patch in `tools/memtag/patches/llvm-packed-wire-v1.patch` updates
the pinned guest LLD producer to this format. That producer currently emits
memory index zero because its data-symbol placement is single-memory; the VM
parser and initializer support explicit other owners. External compiler rebuild
and linked SDK qualification remain separate from the hand-encoded VM fixtures.

## GC objects and other Core 3 storage

GC struct/array references are typed opaque references, not arithmetic linear
addresses. Struct field types/mutability, reference membership, null checks,
array bounds and precise roots protect that storage model. `array.new_data` and
`array.init_data` read data segments; their elem counterparts read element
segments. These operations can be valid in modules without any linear memory.
Table64 indices can be i64 while GC array indices and lengths remain i32.
Exception tags describe exception identities, not allocation tags.

Integer fields and arrays preserve the complete tagged pointer as an i32 or i64.
A later linear access applies the selected owner's geometry, the caller's tag-check
policy and the current memory bounds. The four-bit tag can occupy different
positions on different memory64 owners.

No guest `gc.tagstore` instruction is introduced. Giving guest code authority to
recolor VM-managed objects cannot repair missing roots, unregistered native
escapes or a collector lifetime bug. Host allocator MTE/HWASan hardening is a
separate implementation defense. No GC instruction family is defined; unknown domains, including domain 2,
are rejected. This does not claim complete Core 3 combination coverage.

## Stack lifetime and access comparisons

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

Zeroing instructions retain both their tag effects and byte initialization when
ordinary comparisons are disabled. Explicit operations continue to use the
owner's layout across stack allocation and release.

## Hardware qualification

Hardware selection must depend on actual OS/thread support, enabled tagged
mappings and the exact requested fault mode. A CPU feature name alone is
insufficient. Every executing Guest thread must have the required policy and
an unblocked SIGSEGV. Synchronous qualification requires an invalid tagged
store to fault before changing the destination. Asynchronous qualification
instead checks eventual terminal delivery at the required boundaries and must
not claim that the rejected write was prevented.

AArch64 QEMU can exercise MTE instructions and the selected synchronous or
asynchronous fault behavior. Such runs are emulation evidence, not hardware
acceleration or native performance evidence. Native x86_64 VTune results
describe software checks only. MTE generations and eMTE require explicit
capability evidence; unsupported features must not be advertised as tested.

Linux synchronous MTE reports a tag mismatch before performing the offending
access. Asynchronous and asymmetric modes do not meet that requirement for
writes. Requesting both SYNC and ASYNC lets the kernel choose a mode and is
therefore insufficient to establish either exact launch policy. Linux also
documents that kernel accesses to user buffers
are only checked on a best-effort basis, even in SYNC mode; VM host-buffer proofs
remain required. See the [Linux MTE userspace ABI](https://cdn.kernel.org/doc/html/latest/arch/arm64/memory-tagging-extension.html).

Arm's `FEAT_MTE4` includes named capabilities such as
`FEAT_MTE_CANONICAL_TAGS`, `FEAT_MTE_NO_ADDRESS_TAGS`, `FEAT_MTE_TAGGED_FAR`,
`FEAT_MTE_STORE_ONLY` and allocation-tag access permissions (`FEAT_MTE_PERM`).
Linux `HWCAP3_MTE_FAR` advertises `FEAT_MTE_TAGGED_FAR` functionality;
`HWCAP3_MTE_STORE_ONLY` advertises store-only checking. The runtime records
these OS-visible facts separately. Its minimum architectural version is not a
claim that all MTE4 subfeatures are usable or tested, and it does not select
store-only checking in place of the required read-and-write policy. Selection
and tests must identify the actual capability and ABI. See
[Linux's arm64 HWCAP definitions](https://kernel.org/doc/html/next/arch/arm64/elf_hwcaps.html)
and [Arm's feature descriptions](https://support.arm.com/documentation/ddi0487/mb/-Part-A-Arm-Architecture-Introduction-and-Overview/-Chapter-A2-A-profile-Architecture-Extensions/-A2-2-Armv8-A-architecture-extensions/-A2-2-10-The-Armv8-9-architecture-extension).

## Release acceptance

Acceptance remains contingent on complete scalar, SIMD, atomic, bulk and
host-interface coverage; memory growth and shared/imported memory lifecycle;
interpreter/JIT trap parity; debugger tag displays and diagnostics; hot-update
policy preservation; five-platform execution; actual Arm hardware performance;
generated code with comparisons disabled; and measured SDK-level overhead.

All language runtimes and allocators need an auditable instrumentation path.
Mandatory SDK MTE builds must follow those results, rather than replace them.

## Atomic access failures

Tagged atomic loads, stores, read-modify-write operations, waits and notifications
reject natural misalignment before consulting linear-memory bounds or allocation
tags. Interpreter and LLVM paths use the same order for memory32 and memory64,
including shared memories. Tag bits occupy the high address bits and do not affect
natural alignment. This early rejection neither forms a host pointer nor accesses
memory; aligned operations still require their complete bounds and tag checks.
Report-only MTE does not permit an unaligned atomic operation to continue, and an
access rejected for misalignment does not produce a tag-violation report.

LLVM must retain potentially faulting atomic accesses even when their return
values are dropped or their operands leave the stored bits unchanged. A memory
guard can enforce a Wasm trap only if the generated machine code performs the
access. Guest atomic instructions therefore preserve both volatile observability
and sequentially consistent ordering.

## GC and tagged linear addresses

Core Wasm GC objects do not require allocation tags, and this extension adds
no GC tagging opcode. `struct` and unpacked `array` i32/i64 fields carry the
complete integer value, including the linear address tag. Store/load, copying,
function arguments/results, exception payloads and GC relocation must preserve
those bits. GC lifetime and reachability do not authorize a linear-memory access.

A program extracting such a value and using it for a linear load/store, bulk,
SIMD or atomic operation remains subject to the selected memory owner's tag
and bounds checks. Passing it to WASIp1 or ROS also preserves the complete
value until that host buffer is checked against the actual caller's memory.
The memory index chooses an independently owned tag domain; GC references do
not transfer memory ownership or authority across modules. Packed i8/i16 GC
fields truncate ordinary integers by the standard rules and cannot represent
an intact tagged pointer. An integer tag alone is not an unforgeable capability.

Lossy scalar representations are not pointer carriers: `ref.i31` discards bit
31, packed i8/i16 fields narrow their inputs, and `i32.wrap_i64` can discard a
memory64 tag. Standard integer extension does not move a memory32 tag to the
memory64 tag position or establish destination-memory authority. A language
using pointer compression or NaN boxing must preserve the tag separately or
use a complete i32/i64 field. Reloading the current allocation tag merely to
repair a truncated stale pointer can hide a use-after-free and is not a valid
compiler adaptation. These operations remain legal numeric Wasm operations;
subsequent linear access must still check the resulting pointer normally.

There is no reserved GC tagging instruction family in this wire version.
All unimplemented encoding domains, including domain 2, fail validation.

## Violation reporting and delivery

`--wasm-policy-global runtime.memtag-backend software|auto|arm-mte|x86-mtt` and
`--runtime-memtag-mode sync|async` select backend and timing independently.
The defaults are `auto` and `sync`. The action defaults to `crash`.

```text
--runtime-memtag-action crash
--runtime-memtag-action silent-log out
--runtime-memtag-action silent-log err
--runtime-memtag-action silent-log path <PATH>
```

`silent-log` must explicitly select a destination. It records every detected tag
mismatch and permits that access to continue. Bounds, memory-owner, alignment
and host-permission checks still apply. This diagnostic action may let a guest
memory bug change later guest state; it never grants access to host memory.

MTE reports use an independent owned descriptor and serialization lock. `out`
and `err` duplicate the requested standard descriptor; `path` appends complete
JSON records to its own host file. Reconfiguring `--log-output` before or after
this option cannot redirect MTE reports. No ordinary UWVM diagnostics are sent
to the private MTE file. Choosing the same standard endpoint explicitly for both
streams is an operator choice, not implicit routing. Report descriptors are
never guest FD-table entries. Owned report files are held above native descriptors
0, 1 and 2, even when an inherited standard descriptor was closed before launch.
Granting guest stdio cannot implicitly re-export a private report file through
that closed slot. A deliberately shared output endpoint or a separate guest
filesystem grant remains an explicit operator choice. Opening failure exits126;
output failure exits123.

Arm MTE crash mode uses OS-checked physical mappings. Continuing silent-log
diagnostics need precise software comparisons for every affected granule.
With that action, automatic selection uses software and a required physical
backend is rejected.

Orderly execution and host-call boundaries drain async records. Forced process
termination or an immediate whole-process exit can interrupt outstanding async
output. Sync logging writes its record before continuing the access. Neither
mode promises fsync/power-loss durability. Existing report files are appended;
sequence numbers identify order within the current process.

## Independent backend, timing and action axes

The selected timing and action apply across enabled callers.

| Owner | Timing | Action | Access and delivery contract |
| --- | --- | --- | --- |
| Software | Sync | Crash | Compare each touched granule before access and terminate on mismatch. |
| Software | Sync | Silent-log | Report each mismatching granule before continuing. |
| Software | Async | Crash | Detect at the access and terminate at a later delivery boundary. |
| Software | Async | Silent-log | Copy precise evidence and drain at an execution or host boundary. |
| Arm MTE | Sync | Crash | The OS verifies synchronous physical four-bit tag checking. |
| Arm MTE | Async | Crash | The OS verifies asynchronous physical tag checking. A fault can arrive after the data access. |

The default is **auto, sync, crash**. `auto` selects a usable physical
backend for compatible owners and software otherwise. Explicit `arm-mte`
and `x86-mtt` require their corresponding checked mapping implementation.
A module configured to skip comparisons can share a software owner
with modules that check its tags. An explicit physical owner reachable by an
unchecked caller is a launch error.

Direct LLVM accesses still prove the full linear-memory range. Physical Arm
memory accesses use all four tag bits. Marker queries, directory operations,
shared leases and host transfers also consult owner state.

Linux capability admission uses OS HWCAPs and the actual per-thread `prctl`
result. Instruction-only FEAT_MTE is insufficient; checked mappings require the
OS-supported MTE interface. MTE3 asymmetric mode is detected but not requested
for either exact sync/async choice. MTE4 subfeatures are independent bits, not a
permission to assume that the entire generation is available. Apple EMTE/MIE
uses a different OS allocator/entitlement integration and does not make this
Linux mmap backend available on Darwin. Apple describes its enforcement as
strictly synchronous; it is not the Linux read-sync/write-async mode.

Primary specifications: [Linux MTE ABI](https://docs.kernel.org/arch/arm64/memory-tagging-extension.html),
[Linux Arm64 HWCAPs](https://github.com/torvalds/linux/blob/master/arch/arm64/include/uapi/asm/hwcap.h),
[Arm protection architecture](https://developer.arm.com/documentation/102433/0200/),
[Apple MIE](https://security.apple.com/blog/memory-integrity-enforcement/).

Native x86_64 performance must be collected with the existing Intel VTune
installation. Hardware-mode refusals on x86_64 are not hardware execution
benchmarks. QEMU provides Arm functional evidence and simulator throughput
only; LLVM MCA models instruction scheduling, not physical MTE tag-cache cost,
OS delivery latency or Apple EMTE. A published 40%/10% overhead comparison
cannot substitute for native measurements of this implementation.


## Command-line controls

| Control | Accepted values / effect | Default |
| --- | --- | --- |
| `--wasm-policy-global parser.adm off` | Reject ADM memory declarations. | Extension accepted |
| `--wasm-policy-global parser.memtag off` | Reject WasmMTE declarations, instructions and global metadata. | Extension accepted |
| `--wasm-policy-global runtime.memtag-checks off` | Default ordinary access comparisons to off. | Comparisons enabled |
| `--runtime-memtag-mode` | `sync`, `async` | `sync` |
| `--runtime-memtag-action` | `crash`, or `silent-log` with mandatory `out`, `err`, or `path <PATH>` | `crash` |
| `--wasm-policy-global runtime.memtag-backend` | `auto`, `software`, `arm-mte`, `x86-mtt` | `auto` |

Violation records carry `"action":"silent-log"`. The report stream is configured
inside the action, independently of the ordinary `--log-output` stream. No
separate MTE report-output or worker-count command exists. Old `silent`, `log`,
mixed delivery values and spelling aliases are rejected without compatibility.


## Performance invariants of checked execution

The report writer classifies its owned descriptor once at launch. On Linux,
regular files use a complete partial-write/EINTR loop without per-record SIGPIPE
masking. Pipes, sockets and devices retain the protected descriptor writer.
The independent serialization lock, complete JSON records, error handling and
per-process sequence remain in force. Only the initialized record prefix is
written; unused stack storage never reaches the report stream.

Deferred delivery wakes a draining caller when its pending count reaches zero.
This changes wakeup traffic, not the accepted records, first-fault selection,
continuation contract or boundary-drain requirement. Full-queue handling still
publishes the copied record rather than dropping or sampling it.

LLVM mismatch-edge weights are scheduling hints. They do not permit a compiler
to assume a tag match or remove the ordinary bounds proof. The Arm physical
pointer emitter uses the native allocator's complete canonical reservation as
its witness; a guest address or alignment hint cannot supply that witness.
Physical Arm checks use the complete four-bit tag. The owner records its selected
position for marker instructions and host access.

Performance qualification records process CPU time and elapsed time separately,
pins each before/current workload to the same CPU, and coordinates physical-core
affinity and PMU collection domains with other native measurement cohorts. It checks the result, complete memory footprint
and every report before accepting a timing sample. QEMU throughput and LLVM
MCA instruction models remain distinct from native Arm performance evidence.
