# Host allocation budgets

[简体中文](resource-limits.zh.md)

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
| `parser` | `input-live-bytes` | 1 GiB | Simultaneous VM-owned Wasm input mapping and immutable source extents across the process |
| `parser` | `type-metadata-bytes` | 512 MiB | Cumulative native type records per module, including recursive definitions and flat ABI projections |
| `parser` | `function-body-bytes` | 8 MiB | Maximum encoded body size of one function |
| `parser` | `module-code-bytes` | 64 MiB | Sum of encoded function body sizes in one module |
| `parser` | `single-bytes` | 256 MiB | Maximum backing allocation for parsed sections, names, constant expressions and Core 3 type records |
| `parser` | `live-bytes` | 1 GiB | Process-wide live parser backing bytes, including old and new buffers during growth |
| `validation` | `single-bytes` | 256 MiB | Maximum backing allocation for bounded validation/compiler stacks |
| `validation` | `live-bytes` | 512 MiB | Process-wide live backing storage for these stacks |

The parser charges metadata buffers and hash buckets when it allocates them and releases the charge when their owner frees them. A shared module keeps one charge while it is live. Interned recursive type keys and IDs stay charged until their registry is destroyed. Reusing an existing group uses validation scratch for lookup and retains its existing canonical ID.

Scoped `parser.limit.type-metadata-bytes`, `parser.limit.function-body-bytes` and `parser.limit.module-code-bytes` may tighten the corresponding process settings for a module. A host setting continues to apply when it is lower than the scoped value.

A module can further limit its source using `parser.limit.input-bytes` at global, group or single-module scope. Size is checked on the open handle before mapping or copying. Distinct old and new images share the process live budget during replacement and restoration. Parser views and source pins share their original owner's charge. Checkpoint envelope files use their own file quota.

Module parser count limits from `parser.limit.TYPE` also apply. In
particular `code_locals` checks both the declaration run count and the expanded
local variable count. A single compressed local declaration cannot evade it.
Type counts are checked before recursive type vectors are reserved.

Validation stack budgets cover the independent validators and the fused
interpreter and LLVM translation paths, including control/operand snapshots. Optional parser warnings borrow the parsed sections. Their duplicate-name, duplicate-type and unused-type tables share the validation scratch budget.
They are not a replacement for the encoded input limits, and do not account
for LLVM's internal IR, optimizer, object code or allocator overhead.
CLI input rejection and validation storage refusal exit with status 127. In exception-enabled builds, private function replacement and checkpoint compilation discard a candidate when its parser backing or parser-derived validation scratch is refused and report exhaustion. Structural signature comparisons that use a native Boolean callback record scratch refusal on the preparing thread before any candidate can be published. Staged threaded compilation reports its budget status. Canonical type interning publishes all new groups together after admitting their storage. A refusal preserves the existing registry and caller mapping.

## Initialization and execution

| Stage | Resource | Single default | Live default |
|---|---|---:|---:|
| `initializer` | `metadata-single-bytes`, `metadata-live-bytes` | 256 MiB | 1 GiB |
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
  backing. During guest execution an allocation refused by the host budget
  follows the execution-fault policy (121 with `--debug-fault-action exit`).
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
  Exception reference registry entries, per-token roots, cross-module store
  leases and external-reference bridges also draw from this budget. Each
  store allocates a 1,024-bucket index when it first roots an exception. The
  store owns the index charge until it frees the index.
* **Threads:** native Wasm-thread stacks and the existing per-child bounded
  construction account. The construction account conservatively retains all
  charged requests until its last owner dies, even if construction scratch
  has already been freed. The existing per-child and per-group limits remain
  additional constraints. Native Linux x86-64 CLI AOT can also reserve up to
  16 MiB per group for a relocatable root object.
  [Threaded CLI storage accounting](threaded-cli-storage-budget.md#llvm-object-reuse-across-pthreads)

Limits follow actual ownership through moves, failed allocation, destruction,
module retirement and hot update. Guest code cannot reset counters. Resource
checks run when backing storage is allocated or grows, not on ordinary memory
loads/stores or GC field access.

If the main module and explicit preloads already exceed the graph node limit,
CLI argument processing returns 126 before opening a preload. Once loading
starts, initial memory, table or GC metadata budget refusal returns 127.
A GC constant expression uses the runtime GC pool for the aggregate and the
runtime auxiliary pool for its reference roots. Initializing a global, a table
or an element segment from another module's GC, external or exception reference
can also require an auxiliary allocation. Ordinary startup reports the affected
pool and exits with status 127 on refusal.
If the ordinary initializer finds a module entity count above its configured
reserve limit, it rejects the runtime configuration with status 126.
An inadmissible WASI Threads startup profile returns 126, including stack or
construction budget refusal before the first guest entry. A refused later
thread spawn is returned to the guest through the spawn failure result.
A root allocation refused during private checkpoint restore discards the
candidate and leaves the previous generation active. A fresh WASI worker
records `ENOMEM` during preparation and returns the guest spawn failure value
(`-1`). If the actual threaded main has begun applying shared active segments, an allocation refusal also
requires a runtime reset before that startup can be retried.
Guest `memory.grow`/`table.grow` use their standard recoverable failure result.
Once guest code is executing, GC allocation failure is a classified execution fault. The default action is
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

Follow each declaration through to its storage owner. A custom section borrows
the input image for its payload and charges the parser pool for the capacity of
its record vector. The initializer metadata pool covers the stable length and
growth mutex of each local linear memory and one fault-region record per
registered mapping. Imported aliases share the memory owner's charge.
The same pool covers the actual objects and shared control blocks for ordinary
GC stores, their lease owners and local exception-tag identities. Setup and
retirement account for these allocations. Guest loads and stores continue to
use their existing addressing and bounds checks.

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

## Initializer metadata

The initializer charges each runtime record vector, expression-form element
payload, declared-function reference vector, LLVM table-view vector and
constant-expression operand or GC input buffer when it allocates backing.
Fast-io vectors charge their aligned allocation prefix. Replacement storage
overlaps the original charge until the original block is freed. Staged restore
also charges its resource roster and element plan. A fresh worker draws from
the same process pool and its construction account. Each linked module owns
its descriptors, while imported table and memory aliases share the original
element or page backing.

`--wasm-process-initializer-limit element_expressions N` bounds the sum of
expression-form entries in all segments of each input module. Its default is
1,048,576. Module policy can lower it with `init.element-expressions`. Each
instance allocates only the required function, opaque or GC element slots.
Table and segment construction quotas for a worker apply across its provider
graph. References to the same provider consume its backing once.

The defining module may lower the maximum initial backing of a single table
or memory with `init.allocation-bytes`. Memory uses its configured initial
page count and the declared page width. Restore checks the saved extent and
workers use the retained defining module policy. An importing module's budget
covers its own definitions. Address-space reservation policy and runtime
growth use their respective process limits.

A constant expression can read an immutable global that still needs initialization.
The initializer keeps the pending definitions and partially evaluated operands
in the process metadata pool. The `--wasm-depend-recursion-limit N` setting
also limits the number of pending definitions below the expression being evaluated.
Its default is 2,048. A value of zero leaves the metadata byte limits in force.

## Loader graph

The process `runtime_modules` ceiling includes the executable, Wasm
preloads, native drivers and local providers. The command line applies the
final host settings to all explicit preloads, in either option order.
Registration checks again with the actual local providers. Private restore
checks its newly built declarations before reserving the dependency graph.

`--wasm-process-initializer-limit dependency_edges N` limits distinct
importing-module/provider pairs across that graph (default 1,048,576).
When dependency warnings are enabled, `cycle_path_entries` limits the sum of
module names retained in cycle diagnostics, including the closing name
(default 65,536). A self-loop uses two entries. Cycle enumeration, graph and
export tables, the runtime registry, module preload records and linked
memory-tag plans draw from the process initializer metadata byte pool.
