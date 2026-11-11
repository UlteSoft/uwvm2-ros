# Threaded CLI storage accounting

`--wasi-threads` explicitly selects the threaded execution profile. It is not
an unused permission grant: the selected module must satisfy the shared-memory,
thread-entry and initialization contracts. Unsupported instance shapes and
instance storage admission failures return parameter-error status **126** with
a diagnostic before Guest execution. They do not trigger runtime-internal crash
status 123 or crash-pause handling, and never fall back to ordinary execution.
Explicit invalid-source, invalid-admission, declaration-mismatch and invalid-store
outcomes retain their internal-failure classification, as do native failures.

`--wasi-threads` specifies per-child storage and active/spawn count policies.
Both counts accept `unlimited`; the four byte capacities remain positive and
finite. For a finite active-child count, its
aggregate storage allowance includes the initial-main slot, every declared child
reservation, the actual Group/slot/compiler-queue objects, runtime profile and
admission objects, concrete compiler submissions, and the bounded initial-main
ABI input/output storage. Compiler submissions for spawned children carry no
parameter buffer. The real parameter/result capacities are checked against the
existing value-stack bound.

The CLI computes the finite sum with overflow rejection. With an unlimited
active count it imposes no aggregate traffic/count ceiling. Stable slot and
compiler-submission storage grows on demand; allocation failure rejects a spawn
without invalidating running children. A slot is reusable only after its actual
native thread has joined. Finite attempt counters remain charged, while an
unlimited attempt policy can reuse a retired ABI thread ID; live IDs stay unique.
Native address-space, the thread-ID representation and explicitly selected byte
capacities remain real resource bounds.
At actual cold startup,
after option ordering is resolved, explicitly selected debugger control storage
and the selected signal-pipe allowance plus its bookkeeping are added under the
real registry and maintenance locks. The genuine Group still charges each object,
submission, pipe and child reservation and rejects any excess. Initial-main and
compiler native stacks retain their separate existing accounting.

Native embedders that call `runtime_threaded_profile_configure_host_api` retain
their exact explicit aggregate cap. Only the CLI-specific configuration entry
uses this derived aggregate; it grants no new host operation, source identity,
thread capability or Guest cancellation authority.

## Native JIT frame accounting

The native-stack argument bounds each actual Guest and compiler stack mapping,
including its guard pages. The interpreter operand stack and the initial-main
ABI buffers are separate storage resources. A JIT frame is not charged against
the interpreter operand-buffer allowance a second time.

The threaded JIT checks final target machine frames against the generated ELF
stack-size records, including the target-specific call return address and any
permitted fixed alignment slack. It then records the real optimized LLVM call
graph. A complete acyclic graph is bounded by its longest weighted call path;
large nonrecursive functions are therefore not multiplied by the maximum
recursion depth. Recursive graphs, unresolved indirect calls, or incomplete
native-function correspondence retain the conservative maximum-frame/depth
envelope. Both proofs remain subject to the usable bytes in the actual native
stack mapping. No frame-pointer heuristic or instruction-based unwind is used.

A native stack quota rejection reports the proven frame-byte requirement and
the usable native allowance. Increasing the native-stack argument does not
increase Guest memory, interface permissions, executable storage, or the
interpreter operand allowance. Code and relocation reservations remain
separately charged against the explicitly supplied per-child code quota.

## Multiple Wasm modules

A threaded child owns private mutable globals, tables and GC state for every
reachable module. Cross-module calls stay inside that child's authenticated
module graph. A Wasm-defined function executes with its defining module's source
and policy identity. A raw re-exported host import keeps the actual executing
caller's policy, linear memory and audit identity; only provider authentication
follows the import chain. Sharing linear storage does not transfer authority.
An import-only side module retains its private instance without inventing a
native entry or an executable allocation. The current threaded graph requires
a single common imported shared memory and
the supported WASI-thread passive initialization contract. Unsupported graph
shapes are rejected before execution.

Executable storage is charged for each module's conservative object, alignment
and relocation reservation, then checked against the per-child total. The
whole total is not preallocated again for every module. Cross-module native
stack paths retain the conservative depth/frame bound.
