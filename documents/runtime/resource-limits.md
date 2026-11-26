# Host allocation budgets

uwvm2-ros applies host-owned resource limits before allocating storage derived
from a Wasm module. These limits are independent of WASI and ROS permissions:
an allowed import does not change the resource policy, and no guest interface
can raise a limit. Configure limits before `--run`.

```text
--wasm-set-resource-limit STAGE RESOURCE BYTES
```

`BYTES` is an unsigned decimal byte count, at most `PTRDIFF_MAX` for the host.
Zero refuses new storage in that category; it does not mean unlimited.
Unknown stages/resources, negative values, trailing characters and overflow
are command-line errors (exit 126). Repeating a setting replaces its value
before any module is loaded. A single limit and a live limit are independent;
the tighter applicable limit wins.

## Parser and validator

| Stage | Resource | Default | Meaning |
|---|---|---:|---|
| `parser` | `input-bytes` | 256 MiB | Maximum size of one input module image |
| `parser` | `type-metadata-bytes` | 512 MiB | Cumulative native type records per module, including recursive definitions and flat ABI projections |
| `parser` | `function-body-bytes` | 8 MiB | Maximum encoded body size of one function |
| `parser` | `module-code-bytes` | 64 MiB | Sum of encoded function body sizes in one module |
| `parser` | `single-bytes` | 256 MiB | Maximum backing allocation for parsed sections, names, constant expressions and Core 3 type records |
| `parser` | `live-bytes` | 1 GiB | Process-wide live parser backing bytes, including old and new buffers during growth |
| `validation` | `single-bytes` | 256 MiB | Maximum backing allocation for bounded validation/compiler stacks |
| `validation` | `live-bytes` | 512 MiB | Process-wide live backing storage for these stacks |

The parser charges metadata buffers and hash buckets when it allocates them and releases the charge when their owner frees them. A shared module keeps one charge while it is live. Interned recursive type keys and IDs stay charged until their registry is destroyed. Reusing an existing group uses validation scratch for lookup and retains its existing canonical ID.

Scoped `parser.limit.type-metadata-bytes`, `parser.limit.function-body-bytes` and `parser.limit.module-code-bytes` may tighten the corresponding process settings for a module. A host setting continues to apply when it is lower than the scoped value.

Module parser count limits from `parser.limit.TYPE` also apply. In
particular `code_locals` checks both the declaration run count and the expanded
local variable count. A single compressed local declaration cannot evade it.
Type counts are checked before recursive type vectors are reserved.

Validation stack budgets cover the independent validators and the fused
interpreter and LLVM translation paths, including control/operand snapshots.
They are not a replacement for the encoded input limits, and do not account
for LLVM's internal IR, optimizer, object code or allocator overhead.
Input rejection and validation storage refusal exit with status 127.

## Initialization and execution

| Stage | Resource | Single default | Live default |
|---|---|---:|---:|
| `initializer` | `allocation-bytes` | 4 GiB | Per initial memory/table allocation; runtime pool limits also apply |
| `runtime` | `gc-single-bytes`, `gc-live-bytes` | 256 MiB | 1 GiB |
| `runtime` | `table-single-bytes`, `table-live-bytes` | 128 MiB | 512 MiB |
| `runtime` | `memory-single-bytes`, `memory-live-bytes` | 4 GiB | 8 GiB |
| `runtime` | `memtag-single-bytes`, `memtag-live-bytes` | 256 MiB | 512 MiB |
| `runtime` | `auxiliary-single-bytes`, `auxiliary-live-bytes` | 256 MiB | 1 GiB |
| `runtime` | `threads-single-bytes`, `threads-live-bytes` | 256 MiB | 4 GiB |

Defaults larger than the host's `PTRDIFF_MAX` are clamped to it. All live
counters are process-wide, including different module instances, guest thread
worlds and overlapping hot-update generations. This is a host allocation
budget, not a WASI/ROS policy-group usage counter. Imported aliases retain the
original backing owner's charge and do not charge the same memory twice.

* **GC:** actual native object/array storage, including packed and vector
  element widths, allocation headers and numeric page/slab backing. The byte
  allocator's accounting header is also charged. Arrays are checked before
  allocation, zero-initialization or publication. Collection releases retired
  backing; an allocation refused by the host budget follows the existing
  classified execution-fault policy (121 with `--debug-fault-action exit`).
* **Tables:** allocated backing capacity, including the overlap of old and
  replacement storage during growth. A refused `table.grow` returns the
  standard failure value and preserves the original table.
* **Linear memory:** committed page bytes for mmap backends, logical backing
  bytes for allocator backends. Guard reservations with no accessible pages
  are not charged as committed memory. A relocating allocator conservatively
  reserves the peak of both old and new buffers. A refused `memory.grow`
  returns the standard failure value without publishing a new size.
* **MTE:** committed/allocated shadow storage and shared shadow control
  storage. Replacement shadows account for old/new overlap. Failed shadow
  growth does not publish an invalid pointer or partially initialize guest
  memory. Page guards and existing load/store fast paths remain active.
* **Auxiliary:** covered GC layout/trace/collector/segment scratch arrays and
  recoverable WASI host vectors/scatter arrays and the threaded poll/I/O/socket
  scratch arrays. Existing WASI bad-allocation
  handling returns `ENOMEM`; do not interpret it as permission refusal.
  Exception reference registry entries and module roots also draw from this
  budget.
* **Threads:** native Wasm-thread stacks and the existing per-child bounded
  construction account. The construction account conservatively retains all
  charged requests until its last owner dies, even if construction scratch
  has already been freed. The existing per-child and per-group limits remain
  additional constraints.

Limits follow actual ownership through moves, failed allocation, destruction,
module retirement and hot update. Guest code cannot reset counters. Resource
checks run when backing storage is allocated or grows, not on ordinary memory
loads/stores or GC field access.

Initial memory/table/GC metadata budget refusal is an input rejection (127).
An inadmissible WASI Threads startup profile returns 126, including stack or
construction budget refusal before the first guest entry. A refused later
thread spawn is returned to the guest through the spawn failure result.
Guest `memory.grow`/`table.grow` use their standard recoverable failure result.
GC allocation failure is a classified execution fault: the default action is
`crash`; `--debug-fault-action exit` returns 121, and `pause` retains the
configured diagnostic pause behavior. No failed allocation is treated as a
successful allocation or silently retried with reduced protection.

Publishing the exception reference for `catch_ref` or `catch_all_ref` uses the
same execution fault policy when the auxiliary budget or the exception
reference identity space is exhausted. In the standard collector build, the
receiving module keeps its exception roots until the store is retired.
Overwriting a table or dropping a reference therefore does not immediately
refund that registry allocation.

Moving an exception reference in a typed exception payload can also consume
auxiliary budget. The receiving module needs a root for each token it retains
for the first time. Later transfers of the same token into that module reuse
the existing root. A resource refusal while retaining a payload reference uses
the GC allocation fault policy. A handler that discards the payload does not
create roots for its fields.

## Collection pressure and measurement

Large managed arrays contribute to the normal collection-pressure decision
by backing size, in addition to the allocation-count trigger. This avoids
waiting for thousands of allocations when a few arrays already occupy a large
amount of storage. It does not relax any root-ownership admission checks.
Automatic collection is still limited to execution paths whose complete roots
are authenticated; a resource budget does not make an unqualified path eligible.

Measure both process and execution time. `--log-verbose` reports
`Total WASM execution time` using the host monotonic clock after compilation
and before runtime retirement. Do not add a WASI clock import to an otherwise
import-free collector benchmark: doing so can change collector admission.
Compare the same original module and entry, and report engine configuration,
cache state, warmup and repetition counts separately.

## Scope

These counters bound specified requested native allocations, not total RSS.
They do not include every C++ library allocation, LLVM allocation, allocator
bookkeeping or operating-system page-table cost. Encoded input/count limits
bound additional parser/initialization structures; existing protocol and
queue limits still apply to imports and diagnostics. Deployment should retain
an OS resource boundary such as a Linux memory cgroup with an explicit swap
policy. Raising limits is an explicit host deployment decision.

## Native linear-memory address space

Initial reservation and page-commit failures are resource refusals (exit 127),
with an initialization diagnostic when the selected output can accept it.
They do not deliberately trigger a native crash. Broken internal invariants
remain terminal VM failures.

On 32-bit hosts, an unsuccessful full partial-guard reservation is retried with
smaller page-aligned windows, never smaller than the declared initial memory.
A smaller window selects explicit bounds before either engine translates code;
growth remains limited to that owner's actual reservation. This permits many
small memories without demanding a fixed 256 MiB window for every instance.
If the minimum reservation still fails, initialization is refused. Memory32 on
64-bit hosts retains the full unsigned guard domain and its existing fast path.

LLVM object caches hash the generated IR, including bounds selection and native
reservation extents. Objects generated for a larger guard window cannot satisfy
the identity of a smaller owner requiring software checks. Reservation changes
do not authorize bypassing cache signatures or owner binding.

## Native allocator ownership

VM-owned runtime objects, arrays, owning containers and shared/weak control
blocks use fast_io allocation. The existing fatal global allocator remains
available for paths that cannot recover. The strict global allocator in
`uwvm2::utils::allocator::fast_io_strict` reports allocation failure to its
caller; its standard-container adapter throws `std::bad_alloc` so existing
exception rollback and recoverable error handlers continue to work. A strict
allocation is not implicitly retried through the fatal allocator.

Owned arrays retain their concrete allocation alignment and element count.
Construction failure destroys the completed prefix in reverse order before
freeing the backing block. Shared ownership allocates both the object and its
control block through the same allocator family. Polymorphic unique ownership
retains the concrete destructor and allocation address across base-class moves.

`--wasm-memory-grow-strict` (alias `-Wmemstrict`) continues to select recoverable
linear-memory growth. It is not a global allocator switch, does not change the
host kernel's overcommit policy, and cannot prevent a later OS out-of-memory
kill after an allocation has already succeeded. Wasm limit failures return
`-1` in either growth mode.

LLVM and other external providers retain their own object factories and
matching release APIs. Their externally specified C++ ABI types are not
rebound to a different allocator. Constant-evaluation-only allocation and
placement construction do not allocate from the C++ runtime heap.

Host allocation exhaustion while preparing the memory-guard fault-delivery
stack, or in the LLVM unwind capability probe, is diagnosed as a resource
refusal (127). The resource diagnostic does not require another allocation
and does not block indefinitely on an unavailable stderr. Unrelated setup
errors and broken internal invariants retain their distinct failure policy.
