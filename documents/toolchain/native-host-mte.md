# Native host MTE prerequisites

Native host MTE protects allocations made by the VM executable and its native libraries. Guest WasmMTE protects guest linear memories. These are separate configurations: successful guest MTE tests do not qualify the native executable, and native compiler instrumentation does not establish guest object provenance.

The build option is:

~~~sh
xmake f --host-mte=off
xmake f -a arm64 --target=aarch64-linux-gnu --host-mte=required
~~~

The default, `off`, preserves the ordinary host build. `required` is currently a **prerequisite gate, not an available complete protection mode**. No complete native host provider has been qualified. It therefore rejects configuration before acquiring or building compiler dependencies. Other architectures receive a target-specific rejection. There is no silent fallback to stack-only instrumentation.

Raw native MTE sanitizer or linker flags are rejected by the product policy, because they cannot establish the complete provider contract. Enabling the AArch64 instruction set with `-march=...+memtag` remains distinct from enabling protection. Runtime guest WasmMTE policy is unchanged.

## What must qualify

A supported configuration needs evidence for each of the following:

- The selected compiler instruments the intended native stack and global objects, and the selected linker emits the corresponding ELF metadata without ignored-option warnings.
- The process loader creates tagged mappings, applies global relocations and allocation tags, and prepares the initial stack before instrumented code executes.
- Every native worker, host callback thread and alternate stack starts with the intended mapping and strict synchronous checking. Checking must not be disabled around library calls.
- The allocator preserves ordinary allocation semantics across first allocation/free, per-thread cache initialization, calloc, realloc, alignment, failure and thread cleanup. Invalid accesses must fail at the intended access, not during an earlier valid operation.
- The VM, LLVM, libc++/libc++abi/libunwind and ROS native dependencies have an explicit instrumentation and allocator contract. Successful application probes do not establish these libraries' coverage. TLS, assembly, custom allocators, coroutine stacks and dynamic libraries require separate analysis.
- Fault handling, native unwind, debugger and update machinery remain correct. A fault inside the VM must not be confused with an admitted guest fault.

This does not promise complete memory safety: finite tags and granules have collision and spatial limitations. Native MTE also does not automatically implement the stronger pointer provenance semantics proposed for a successor guest ISA.

## Observed provider behavior

The qualification probes are in [test/0086.memtag/host_native](../../test/0086.memtag/host_native/README.md). They compile actual executables, inspect ELF dynamic entries and run positive and negative accesses. Merely accepting `-S` or `-c` is insufficient.

The installed Clang/LLD 23 pair tested here accepts native AArch64 MTE object generation but rejects the GNU sanitizer link. The existing private Clang/LLD 24 pair successfully links all four profiles: stack, heap, globals, and combined MTE. The latter emits the expected AArch64 MTE dynamic entries.

With the existing Ubuntu glibc 2.43 AArch64 provider under QEMU `-cpu max`, the resulting combined-profile executable does not automatically gain protected process/worker stacks or global mappings. Wrong-tag accesses to those mappings succeed even when strict synchronous checking is enabled through the glibc tunable. An explicit `PROT_MTE` worker stack and synchronous thread setup do stop incorrect tag reads and writes; that bounded demonstration does not repair the loader.

The same glibc provider has a reproducible cold-cache allocator fault. The initial valid `free` validates a nonzero tag, resets the allocation tag, initializes an inactive thread cache, then re-enters `free` with the original tagged pointer. Its second validation faults against allocation tag zero. The binary trace, fault address, physical tag and current upstream source agree on this sequence. A four-byte change in an isolated diagnostic copy passes the normalized pointer on that cold branch. With the same QEMU and probes, valid free, calloc/realloc/alignment lifecycle, worker heap cleanup and explicit-stack thread cleanup then succeed; wrong-tag access, use after free and double free remain rejected. This is causal diagnostic evidence, **not an installed, source-built or qualified provider fix**. The original provider is unchanged.

The probe report distinguishes collection success from qualification. `provider_prerequisites_pass` is false for the current provider. `full_vm_qualified` remains false: these small executables do not instrument or exercise the full VM and its dependencies. QEMU measurements establish correctness observations only, never native performance.

## Reproducing and interpreting results

The probe accepts an existing compiler, explicit cross-compiler flags, an existing runtime prefix and an optional existing QEMU executable. It installs nothing. Run it within the task's 64 GiB, zero-swap cgroup, with CPU 0 excluded, and place output under the repository's `.tmp` directory.

The report includes exact commands, compiler/source hashes, compile and link outcomes, ELF records, main/worker checking modes, and individual execution logs. Bad-access cases print a marker immediately before the deliberate access; qualification requires both that marker and synchronous MTE signal code 9. A crash while freeing a valid allocation therefore cannot be mistaken for a successful use-after-free or double-free check.

Future admission must be based on a reviewed provider integration and VM-level regressions, not on accepting a user-supplied “passed” file or recognizing a compiler version number. No such bypass is provided by the current gate.

## Primary references

The [Arm Memtag ELF ABI](https://github.com/ARM-software/abi-aa/blob/main/memtagabielf64/memtagabielf64.rst) assigns responsibilities across the compiler, linker and loader. The [LLVM generic MTE linker flag change](https://github.com/llvm/llvm-project/pull/187055) explains why GNU support must be tested through the actual link step.

The [Linux AArch64 MTE userspace ABI](https://docs.kernel.org/arch/arm64/memory-tagging-extension.html) defines tagged mappings and per-thread checking modes. The [glibc memory tagging tunable](https://sourceware.org/glibc/manual/2.38/html_node/Memory-Related-Tunables.html) controls allocator tagging and fault mode; it is not proof of complete stack or global coverage. The allocator cold path is visible in [glibc malloc.c](https://github.com/bminor/glibc/blob/04e750e75b73957cf1c791535a3f4319534a52fc/malloc/malloc.c#L3329).
