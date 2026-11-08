# WasmMTE compiler integration

This directory records the pinned experimental compiler and the downstream
stack-lifetime and global-operand corrections used by uwvm2-ros development. It does not certify the
runtime or the SDK for production MTE deployment. Build directories, source
checkouts, profiles and receipts belong under the repository's `.tmp/` directory
and must not be committed.

`llvm-reference.json` pins the upstream revision. Apply
the patches listed in that manifest, in order, to a checkout of that revision.
The stack restoration patch
preserves the existing binary instruction family. It changes compiler-generated
stack lifetime cleanup to `memtag.store` with the **original** padded alloca
pointer, explicitly restoring the stack allocation tag. Heap code may continue
to use `memtag.untagstore`. A VM must not infer stack-versus-heap semantics from
whether optimization makes a length constant.

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
inspect actual linked Wasm, including `memtag.globals`, rather than treating a
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
The mt-2 opcode set is a measured research baseline; compatibility with it is
not a requirement for the successor ISA.
