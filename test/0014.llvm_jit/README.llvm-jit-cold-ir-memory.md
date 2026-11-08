# LLVM JIT cold-path IR ownership and digest checks

The parallel compiler serializes each joined fragment, copies its diagnostic
module name, destroys that fragment's LLVM Module and then its LLVMContext,
and reparses the owned bitcode into the merged context. This avoids retaining
the original and replacement IR graphs simultaneously. Every input byte,
link operation and final verifier remains part of the existing pipeline.
A parse or link failure still leads to fresh serial emission from the retained
Wasm source. This change does not admit executable code or alter debugger
permissions.

The persistent-cache module digest hashes the complete existing serialization
directly. It does not make a second vector containing identical bytes. The
borrow lasts only for the synchronous SHA-256 calculation. Cache identity,
signature verification, process origin and source admission are unchanged.

The existing Xmake test discovery registers these fixtures in the LLVM test
group when the LLVM backend and LLVM tests are enabled:

- llvm_jit_fragment_reparse: independent contexts, functions, globals, branch
  and PHI operands, debug metadata, long module names, empty/missing owners,
  duplicate-definition refusal, and a fresh attempt after failure. All final
  modules pass the LLVM verifier. An optional output path writes bitcode for
  exact comparison across builds.
- llvm_module_hash_view: empty and block-boundary byte ranges, independent
  OpenSSL SHA-256 verification, and nine actual LLVM module states. Global,
  layout, function and metadata changes must affect the digest; the original
  vector-based digest must match the borrowed-range digest.

The fragment fixture accepts OUTPUT.bc FRAGMENTS FUNCTIONS INSTRUCTIONS for
a bounded component workload. Those bounds protect the benchmark generator;
they are not limits on guest programs or debugger records. Its RSS measurement
uses the Linux resource-usage API. The five Linux ROS architecture tests use
the actual target binary; QEMU results establish behavior, never performance.

Current qualification covers x86_64, i686, AArch64, ARM soft-float and ARM
hard-float, plus a native ASan/UBSan run. The LLVM and OpenSSL dependencies in
these tests are existing unsanitized builds. Native component comparisons
preserve identical bitcode and reduce peak RSS; observed link time is
essentially unchanged. These measurements do not establish a whole-program
speedup, complete IDE variable support, or the separate MTE concurrency
contract.
