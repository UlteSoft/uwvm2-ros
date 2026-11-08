# Threaded CLI storage accounting

`--wasi-threads` specifies per-child storage and a maximum child count. Its
aggregate storage allowance includes the initial-main slot, every declared child
reservation, the actual Group/slot/compiler-queue objects, runtime profile and
admission objects, concrete compiler submissions, and the bounded initial-main
ABI input/output storage. Compiler submissions for spawned children carry no
parameter buffer. The real parameter/result capacities are checked against the
existing value-stack bound.

The CLI computes this finite sum with overflow rejection. At actual cold startup,
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
