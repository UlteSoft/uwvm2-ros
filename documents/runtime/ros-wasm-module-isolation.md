# SDK direction: private and communication memories

This document records an SDK compiler/linker and interoperability requirement.
It does not add VM memory semantics or claim a completed SDK migration. The SDK
production sources are unchanged by this design review.

Each module keeps its stack, allocator state, C/C++ globals, runtime metadata
and private application data in private linear memories. It may have more than
one private memory. Inter-module exchange uses a separate explicitly exported
or imported communication memory, or bounded copying through a semantic broker.
Do not export the default C/C++ heap/stack memory to achieve zero-copy exchange.
Native vendor libraries remain in external isolated processes; they never
receive pointers into a Wasm private memory.

The implemented `uwvm_ros_v2.module.call.*` broker uses the bounded-copy option.
It requires explicitly registered routes and grants, and an owned, unexported
shared memory for each endpoint's message carriers. No peer imports that memory.
The shared declaration permits atomic copying and erasure; it does not imply
WASI-thread creation authority. Nonshared heaps, imported memories and modules
with memory exports are refused by this broker. SDK examples that deliberately
share an exported memory use a separate trust model and do not inherit these
erasure or routing guarantees. See the [CLI contract](../command-line/ros-scene.md#module-observation-and-registered-messages)
and [qualification record](ros-module-exchange-performance.md).

## Exporting memory grants access to its full extent

An imported memory aliases the exported memory object. Wasm bounds checks
prevent access outside that object; they do not enforce per-record ownership
inside it. A peer can inspect or overwrite every accessible byte. Naming it
`communication` is not a protection mechanism. A memory export is not a
read-only view, and two modules may accidentally import the same private memory
even when neither exports it.

Use separate exchange memories for different trust domains/directions when a
peer must not see another peer's records. Never put secrets or private pointers
in such a memory. Receive data is untrusted: validate lengths, offsets, schema,
generation and sequence after copying to private storage or acquiring a
qualified immutable snapshot. A check followed by a second read from mutable
shared bytes is vulnerable to a check/use race. Concurrent shared-memory
protocols need explicit atomics, ownership publication and ABA/reuse rules.
An ownership bit written by an untrusted peer is not authorization.

## C and C++ compilation

Ordinary C/C++ Wasm builds conventionally place data and linear-stack/heap state
in one memory. Enabling multi-memory in a validator or assembler only permits
the feature; it does not partition C/C++ objects automatically. A qualified SDK
toolchain needs deliberate code generation/linking for memory selection, or
small audited Wasm shims that copy between the private default memory and an
independent exchange memory. Do not advertise a universal compiler flag until
the selected compiler/linker and its emitted instructions prove that behavior.

Keep C++ exceptions, allocator ownership, destructors and object representations
inside the module that owns them. Exchange bounded wire records and opaque
logical handles. Raw C++ pointers, vtables, STL objects and a shared libc++ heap
are not cross-module ABIs. A native `.so` is not a Wasm dynamic library. A Wasm
dynamic-linking model that intentionally shares its C heap is a distinct trust
profile and cannot be called private-memory isolation.

## wasm32 and wasm64 qualification

Use an explicitly specified wire layout independent of C/C++ `size_t`, pointer
width, struct padding and host ABI. Offsets and lengths need checked arithmetic
against the selected memory's actual extent; narrowing to wasm32 must reject
values that do not fit. A wasm32 application and a wasm64 application do not
automatically import the same memory type. A bridge may copy records between
different memory types rather than sharing one object. Handles retain owner and
generation; they are never interchangeable with memory offsets.

SDK tests must inspect the final Wasm imports/exports, all memory declarations,
active/passive data segments, linear stack/heap locations and memory indices on
actual load/store/copy instructions. Test private canaries against a malicious
peer, wrong-memory selection, overlapping buffers, overflow, memory growth,
stale offsets, partial records, failed result delivery and concurrent mutation.
Run separate wasm32, wasm64 and mixed-width record-transfer cases. Reuse the VM's
existing multiple-memory validation/runtime checks without treating them as
proof that a compiler placed C/C++ data safely.

Memory access alone grants no ROSv2 resource operation. The receiving module's
caller identity, capability owner, scope and generation must still authorize
every external effect. A model or POSIX compatibility module cannot gain motion
authority by writing into a controller's exchange buffer.
