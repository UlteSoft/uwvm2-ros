# LLVM JIT object-cache dependency contract

The ROS runtime caches relocatable native object files. Cache format 8 separates
**code compatibility** from **current-process admission** and **file integrity**.
Reusing an object never restores a former process, permission policy, memory
image or native driver implementation.

## Code compatibility

The compatibility identity contains:

- The final verified LLVM module immediately before native object emission:
  instructions, constants, types, calling conventions, attributes, emitted
  metadata, inline assembly, global definitions and symbolic relocations.
  Each parallel partition is fingerprinted after its actual partition transforms.
- The effective target triple, CPU, feature string and data layout.
- Explicitly serialized TargetMachine options: native optimization level,
  instruction selection, relocation/code models, floating-point ABI, section
  layout, TLS, exception and unwind emission. C++ padding and pointer values
  are never serialized.
- A content fingerprint of the real bundled LLVM native code generator's
  transitive static-library dependencies, taken from CMake's target graph.
  The roots are the selected native CodeGen/AsmParser and BitWriter targets.
  Only static-library dependencies are traversed; build utilities and unrelated
  installed libraries are excluded. Archive names and content digests, rather
  than their directories or timestamps, form the manifest.
- The native object/runtime boundary contract and reader-owned ROS product
  identity. Checkpoint and whole-runtime build policies are not object ABI
  dependencies. Concrete LLVM signatures, calling conventions, object extents
  and embedded layout offsets are already in the IR. The separate boundary
  revision covers buffer ownership and relocated-storage lifetime contracts
  that cannot be inferred from an LLVM declaration.

Xmake embeds UWVM_RUNTIME_LLVM_JIT_COMPILER_ID from the compiler manifest.
A manual build without that identity runs normally but refuses persistent cache
use with compiler-identity-unavailable. A guessed LLVM version is insufficient.

Optimization runs before object lookup. A change to the Wasm translator or
optimization pipeline is detected through its actual resulting IR. An optimizer
implementation change producing identical IR does not itself require a different
native object. Native O0/O1/O2/O3 levels remain distinct TargetMachine inputs even
when a small module's optimized IR happens to match.

The old Wasm summary and whole-module/partition-count cache shortcut have been
removed. Parallel workers use the same object-cache adapter as serial MCJIT and
key their own final IR and effective target. A hit still requires validation,
translation, optimization and current symbol binding; it avoids native object
generation. It is not a checkpoint or a bypass of Wasm validation.

## Inputs that do not invalidate native code

The cache key does not contain the host executable digest, Git commit/dirty flag,
project release number, source-tree identity, installation location, cache
directory, log destination, current environment, runtime quota counters or the
contents of an ordinary preloaded native driver.

Each invocation validates imports and configures its environment, permissions,
resources, globals, tables and memories. A hidden interface fails current
validation even if an earlier authorized execution populated the cache.
A denied interface remains denied after an object hit.

Native symbols are relocated to the current process on every load. Changing
a driver's implementation while preserving its imported ABI does not invalidate
the generated caller. Changing a declaration, embedded layout/constant, inlined
code or another actual IR/code-generation input does invalidate it. An external
option that changes actual IR or the target remains relevant even if it also
has a runtime effect.

Syntax admission switches are enforced before compilation on each invocation.
Unused syntax switches are not independent object dependencies. Guest data
initialized at runtime is not confused with compiled constants.

## Admission and integrity

The running main executable and runtime anchor are still checked locally.
Its digest proves the current context refers to the current image; it is not
compared with the writer's executable digest and is not a native-code cache key.

The signed-cache build hashes this complete descriptor with OpenSSL SHA-256,
using the already bundled library's runtime CPU dispatch. This changes neither
the digest algorithm nor the bytes covered. Hashing happens once per process;
the captured descriptor and its identity checks remain live. A cryptographic
initialization, update or finalization failure refuses cache admission. There is
no shortcut based on a pathname, timestamp, previous process or partial image.
Standalone identity readers without the signed-cache build retain the portable
SHA-256 implementation.

Cold origin admission proves the actual initialized source owner, its exact
compiled-module record, current registry/provider bindings and live generation
lease. This also permits ordinary preloaded Wasm modules and trusted native
drivers. The old blanket exclusions for descriptors, sockets, mounts, WASI
tracing, unused permission groups and non-singleton module registries are removed.
Those are current runtime bindings, not proof of native-code incompatibility.
The debugger's separate native-memory admission is unchanged.

Process sealing, genuine runtime-origin admission, the kernel keyring secret,
private cache-directory checks, no-follow file handling, bounded structural
parsing and mandatory signature verification remain in force. Format 8 derives
its signing seed from the kernel secret and compiler context, not the entire
host image. ISA/context metadata and object payload are authenticated together.
Verification precedes decompression and loading.

Truncation, malformed lengths, unsupported versions/codecs, context mismatches
and invalid signatures are cache misses. The runtime recompiles the current
validated module; rejected object bytes are never handed to LLVM.

Objects with generation-bound native pointers, active debugger state or other
existing ineligible bindings remain uncached. Removing unrelated key inputs
does not make such objects relocatable or authorize their admission.

Format 7 objects are rejected. Private checkpoint binding records now require
cache format 8; checkpoint state/envelope schemas remain separate. Full checkpoint
compatibility may still require an exact host build because it describes process
state rather than just relocatable generated code.

## Qualification

Generated fixtures, build receipts, QEMU Linux guest images, mutation results
and native VTune measurements are kept under .tmp/llvm-cache-audit.
QEMU-user's failure to prove the main executable identity is a refusal, not a
successful cache test. Positive ARM persistent-cache tests use full-system Linux
guests with genuine procfs and keyrings. Emulated timings are not native
performance measurements.
