# WasmMTE compiler integration

This directory records the pinned experimental compiler and the downstream
stack-lifetime and global-operand corrections used by uwvm2-ros development. It does not certify the
runtime or the SDK for production MTE deployment. Build directories, source
checkouts, profiles and receipts belong under the repository's `.tmp/` directory
and must not be committed.

`llvm-reference.json` pins the upstream revision. Apply
the patches listed in that manifest, in order, to a checkout of that revision.
The stack restoration patch changes compiler-generated
stack lifetime cleanup to `memtag.store` with the **original** padded alloca
pointer, explicitly restoring the stack allocation tag. Heap code may continue
to use `memtag.untagstore`. A VM must not infer stack-versus-heap semantics from
whether optimization makes a length constant.

## Breaking wire migration

The final `llvm-packed-wire-v1.patch` replaces the historical FC encoding with
one private primary byte and two bounded unsigned LEB32 integers:

```text
FF ULEB32((1 << 24) | (1 << 16) | operation) ULEB32(memoryidx)
```

Version 1 uses linear-memory domain 1. Unknown domains, including domain 2, are rejected; GC objects do not
require tagging opcodes. Full-width GC integer carriers retain linear pointer
tags, which remain checked at a later linear or host-buffer access. The low 16 bits keep operation identities
`0x20..0x2f` and `0x36..0x38`. `0x33` (`memtag.add`) remains undefined and is
rejected by both the producer's machine-code emitter and the VM. Standard FC
instructions retain their standard encoding. Memory indices are unsigned u32
even with memory64; they are not signed i32 literals.

TableGen retains compact internal `0xffXX` selectors to avoid expanding the
disassembler tables. The MC emitter serializes the packed word, and the MC
disassembler checks its version, domain, width and operation before selecting
the compact internal entry. The patch changes LLD output to the sole accepted
`uwvm.memtag.linear.globals` version-2 format, including an explicit memory
index before every address/length pair. This pinned linker places these symbols
in memory zero; it is not an automatic multi-memory instrumentation producer.

Rebuild the guest compiler, objects, archives and linked modules together. The
VM rejects old FC MTE instructions, the abandoned FF/vendor multi-field format,
and the old `memtag.globals` section, even when tag checking is disabled. Do not
patch compiled Wasm bytes in place: instruction lengths, body sizes, debug
offsets and custom sections all need regeneration. Metadata-only modules also
need deployment capability checks or an actual new instruction marker when
targeting older VMs, because generic Wasm engines may ignore unknown custom
sections.

The source revision and the seven changed upstream files' Git blob hashes were
checked before creating this patch, and `git apply --check` passes on those
pinned sources. The added MC and updated linker lit tests still require a full
rebuilt mt-2 toolchain; source/application checks are not that qualification.
`xmake/utility/bundled_llvm.lua` provisions the VM's native LLVM JIT provider.
It is separate from this guest instrumentation compiler and must not be patched
as a substitute for rebuilding mt-2.

Run development commands through the host's configured resource-control launcher.
The qualification environment uses a cgroup with 64 GiB memory, zero swap, and
CPU 0 excluded. Do not assume that launching an IDE inside a cgroup is sufficient:
check the actual extension host, broker and VM membership after startup.

Example CMake configuration for a native Linux compiler:

```sh
cmake -S .tmp/wasmmte/llvm-project/llvm -B .tmp/wasmmte/llvm-build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_PROJECTS='clang;lld' \
  -DLLVM_TARGETS_TO_BUILD='X86;AArch64;WebAssembly' \
  -DLLVM_ENABLE_ASSERTIONS=ON -DLLVM_ENABLE_LTO=OFF \
  -DLLVM_INCLUDE_BENCHMARKS=OFF -DLLVM_INCLUDE_EXAMPLES=OFF \
  -DLLVM_BUILD_TESTS=OFF -DLLVM_PARALLEL_COMPILE_JOBS=6 \
  -DLLVM_PARALLEL_LINK_JOBS=1 -DLLVM_USE_LINKER=lld \
  -DCMAKE_EXE_LINKER_FLAGS=-Wl,--threads=4
ninja -C .tmp/wasmmte/llvm-build -j6 \
  clang lld llc opt llvm-mc llvm-objdump llvm-readobj llvm-dwarfdump \
  llvm-ar llvm-ranlib FileCheck
```

Use an existing compatible host C/C++ compiler; this procedure does not install
or replace system toolchains. The selected targets describe this compiler
build's code generators, not the runtime's five-platform qualification result.

The compiler accepts `-fsanitize=memtag` for Wasm targets. Qualification must
inspect actual linked Wasm, including `uwvm.memtag.linear.globals`, rather than treating a
successful compiler invocation as proof of instrumentation coverage. Stack,
globals, heap allocator, TLS, preloaded libraries, language runtime and exception
cleanup coverage must each be established.

See [the extension integration document](../../documents/runtime/memtag-extension.md)
for the current contract, unresolved reference differences and acceptance work.

The global materialization patch expands PHI incoming constant expressions in
predecessor blocks and expands nested expressions before their users. It also
tags external data and data aliases in instrumented functions, so that a
translation unit does not silently access another unit's tagged globals through
untagged addresses. Linker-defined address symbols may denote the end of linear
memory; their tag lookup is guarded by the current memory page count. A symbol
outside the current memory keeps its numeric address, and any subsequent access
still undergoes the complete runtime bounds and tag checks. This is not a symbol
name exception or permission to dereference an out-of-range address. An unresolved
external weak data symbol remains a null pointer even if granule zero has a
nonzero allocation tag; its nullness is checked before tag materialization.

The regressions run the LLVM verifier, check the transformed IR and generate
object files. Complete instrumented-libc runtime qualification remains separate.
The historical mt-2 wire is no longer accepted. Its instruction semantics remain
the research baseline for the defined operations; this does not establish
complete allocator, SDK or language-runtime coverage.
