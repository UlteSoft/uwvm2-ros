# Numeric GC code generation

The LLVM backend emits checked native access for numeric GC struct fields and array elements, including packed i8/i16, i32/f32, i64/f64 and v128, plus array length. Reference-valued fields retain the collector's original barriers. The interpreter keeps its semantic implementation and uses the complete reference carrier for null operations.

## Authentication and fallback

A Wasm GC reference remains an opaque, non-recycled token. Generated code obtains only the immutable identity and membership-index base of the current retained store. It acquires the relevant bucket head and checks the complete token, canonical owner, object kind, exact local type and logical extent before accessing numeric data. A collision, nonlocal owner, subtype or contended lock uses the existing checked operation. Guest linear memory cannot supply a native object address.

The generated layout is tied to the actual native C++ layout and guarded by static assertions. Struct fields and reference arrays retain the canonical carrier representation. Numeric arrays use native-width byte tails: one byte for i8, two for i16, four for i32/f32, eight for i64/f64 and sixteen for v128. The interpreter, checked helper, direct LLVM IR, staged global initialization and debugger mutation all use the same representation. Narrow values are truncated as integers before native-byte copying, preserving both byte orders. Floating-point and vector accesses preserve their exact bits. Raw object pointers never become Wasm values.

Array construction validates the full allocation product before reserving the original byte-array tail. Numeric tails do not pretend to contain C++ carrier objects. Fill checks the complete destination extent and any reference lease before mutation, then copies a bounded repeating pattern under the original object lock. Copy retains overlap-safe semantics. These changes do not reduce a guest memory limit or weaken a GC reference check.

## Scoped optimization

Mutable objects retain acquire/release exclusion by default. Exclusion can be omitted only for an ordinary full compilation whose entire validated function has no incoming reference parameters, calls, global/table access, external-reference conversion, imported element references or unrecognized operations that could obtain or publish a pre-existing object. Cold and unreachable instructions participate in this proof. Debug, checkpoint, pending-numeric, threaded and non-full compilation profiles retain the original exclusion.

An authenticated numeric access may establish a compiler-private witness for a following access to the same reference, type and element. This witness is restricted to straight-line, callback-free code. Every control-flow boundary, allocation, call and unsupported instruction invalidates it. Local assignments track the identity of the actual SSA value; they do not treat a local slot as a stable reference. Previously loaded stack values keep their original identity after a reassignment. Copies of the same value into another local can reuse a witness, while changing a reference or index cannot reuse evidence for its previous value. The complete helper edge carries a null witness. A witness cannot cross a collection safepoint, a code-generation transition or a debugger mutation.

Adjacent locked accesses may share one acquire/release pair only when the compiler has proved the exact same reference, canonical type, element index and storage width, and has checked the emitted LLVM path from the first release to the next access. The path must contain only side-effect-free SSA instructions and unconditional forward edges: calls, memory effects, conditional exits, terminators and cycles refuse coalescing. The first release is removed only after the final matching release exists. A previous slow path produces a null witness and cannot inherit a lock. Private-object lock elimination remains a separate proof, and debug/checkpoint/threaded code does not use coalescing.

Pure SIMD events preserve the same straight-line witness as scalar arithmetic. The authoritative validated event classifies the operation; SIMD memory, unknown event kinds and any event carrying a memory effect revoke the witness before lowering. This does not preserve a native pointer across a loop, call, collection or debugger stop.

A separate logical-value optimization propagates immutable array lengths from successful checked construction through reference local assignments. It propagates only a scalar length and a validity bit, not an object address or an ownership lease. Unknown parameters, call results, selects and control-flow values remain checked; assigning an unknown reference clears the local fact, and self-tail entry clears all parameter facts. The optimization is restricted to ordinary full, precisely rooted functions. Debug mutation, checkpoint, threaded execution and patchable targets do not use it. Imported bindings alone do not disable the local scalar fact.

A third, independent proof identifies ordinary numeric leaf functions with no collection or observation point in their complete validated body. Such a function can receive reference parameters without establishing a redundant root frame. Numeric fields, array length and i31 operations are admitted; constructors, calls, loops, linear-memory helpers, reference-field operations, EH and unknown instructions revoke the proof. Threaded, debug, checkpoint and patchable-target profiles are excluded. The outer native entry or Wasm caller still pins its execution participation and input values. This proof does **not** remove shared-object locks or inline a replaceable function entry.

In the same proven closed ordinary function, a loop with no observation or collection handshake does not need a separate root publication at its header. The compiler stages a conservative decision and seals it only after validating the whole function. Root publication before real allocations, calls, exceptions and observation points is preserved. This does not disable collection or remove reference roots at a collection point. Other profiles keep the original loop publication.

Scalar array slow paths use the common status dispatcher. A successful status reaches the direct-access merge; error classification stays on the cold edge and calls the trap bridge from the original generated frame. Scalar operations do not accept aggregate segment-error statuses: unexpected results retain the runtime-invariant failure path. Debug locations keep per-opcode fault provenance.

## Debugging and threads

The native numeric access region is finite and contains no call or GC poll. Threaded control verifies these properties before preserving a Wasm-instruction-sized region; lock failure enters the complete helper without holding a lock. Threaded bridges retain the actual native-thread, execution-lease, module and world admission checks.

Native instruction stops and cooperative Wasm stops have different inspection authority. GC state operations require the current cooperative stop and complete authenticated participant capture. A native stop inside a numeric access must not reenter an object lock or turn a raw register into a GC inspection capability.

## Cache and validation

The final LLVM IR identity includes changed checks, GEP geometry and function signatures. The actual native-object ABI fingerprint additionally binds the production numeric-array byte-tail contract, because runtime storage interpretation must agree even across differently labeled builds. Objects compiled against the former carrier-array contract are rejected. Cache hits still bind current runtime objects and retain mandatory authentication.

Hot updates retain independent public native function entries and their actual loaded-code ownership. These optimizations do not relax the publication/quiescence protocol or create inlined copies that bypass replacement targets. Debug and checkpoint profiles retain their observation and rooting contracts; native assembly inspection remains restricted to code owned by the actual LLVM JIT publication.

Source review compares the corresponding lowering against Wasmtime v42.0.1 (commit 6844a83b530f86ad13a65175282fe2dbcd33cad1), including read-only array lengths and width-specific numeric loads/stores. This is a design reference, not a performance or conformance certificate. Qualification must compare matching modules and checked outcomes, separate compilation/cache startup from sustained execution, and correlate VTune sample addresses with the same process's code. An unattributed sample bucket must not be presented as a source-level hotspot.

The numeric regression runner is [run_numeric_codegen.py](../../test/0019.gc_statepoint/run_numeric_codegen.py). It requires an existing VM and GC-capable assembler, supports an existing QEMU argv prefix, runs both interpreter and LLVM JIT, and writes artifacts only to an explicit output directory. Its fixtures cover exact bits, null/index traps, foreign canonical owners, subtypes, collisions, collection, aliases and witness invalidation.

These regression tests supplement the official WebAssembly Core 3 specification suite. They are not a substitute for specification conformance testing, five-ABI debugger/cache qualification or application-level measurements. Performance must be measured on native hardware; QEMU is used for correctness.

### Immutable module domain bindings

Ordinary full JIT code binds its retained module's immutable GC store identity
and membership-index allocation through named external-object relocations.
It no longer calls a native helper on every function entry merely to copy
these two identities. The index allocation remains stable for the store
lifetime; collection changes its atomic bucket heads, never the allocation.
Every object access still checks the live bucket, token, owner, exact layout,
bounds and required mutation lock.

The names contain the existing module identity and a domain ABI version.
Persistent objects contain relocations rather than process addresses, and
ordinary IR reconstruction registers the current store before loading a signed
object. Debug, checkpoint, threaded, bridge-routed and unavailable-domain paths
retain the original runtime helper. No individual object address crosses a
safepoint as part of this optimization, and no module or store lifetime is
extended by a borrowed symbol binding.

### Checked-fallback layout hints

Ordinary full JIT authentication, bounds and try-lock branches carry a static
layout hint favoring the successful local path. This is a code-layout heuristic,
not an assumption: every guard and its complete checked fallback remains.
The private/shared decision and debugger control edges receive no such hint.
Cross-module, subtype, null, collision and contended cases continue through the
same runtime operations and must be included in performance qualification.


### Local immutable lengths with imported bindings

The local array-length facts do not require an import-free module. They hold an
SSA reference identity, its constructor-established immutable length and a
validity bit, never a borrowed object address. An imported function can mutate
fields or trigger collection, but cannot resize an existing array or overwrite
a caller's local. Calls keep the precise root publication. Assigning an unknown
parameter, call result, null, select or PHI clears the fact; the ordinary checked
path handles that value. Debug mutation, checkpoint, threaded, patchable-target
and non-full compilation paths remain excluded.

### Alignment of authenticated numeric payloads

The allocator's original byte-array backing and object tail offset guarantee
alignment for `gc_object_value` in ordinary, slab and staged allocations.
Generated scalar accesses use the alignment shared by that guarantee and the
element stride. Packed byte and halfword arrays therefore retain their smaller
alignment; vector accesses are never assumed to have more than the allocator
guarantees. Authentication, bounds, lifetime and locking checks are unchanged.
