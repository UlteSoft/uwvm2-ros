# WasmMTE extension contract checks

`instruction_contract.cc` pins the 20 experimental instructions and their memory-dependent integer signatures to the referenced LLVM `mt-2` revision. It also checks every single-byte subopcode for accidental acceptance, reserved holes, wide invalid values, tag geometry, byte-length roles, memory effects, and global-descriptor identity. The assertions run at compile time and the executable repeats the contract check.

These tests do not claim parser, interpreter, JIT, memory-tag checking, or sandbox coverage. Those need executable adversarial Wasm tests once the runtime is connected. The experimental opcode table deliberately lives outside the standard Wasm opcode enums.

`debug_view.cc` covers allocation-tag copies from private/shared wasm32/wasm64 owners, actual-length and overflow boundaries, unaligned granules, disabled state, moved owners, isolated memories, console selectors, and color rendering. `debug_console.cc` checks the native numeric/hex/RGB protocol format consumed by DAP. The matching Python tests are `test/0018.debugger/test_dap_memory_tags.py`; their modeled protocol checks do not replace actual owner activation or hardware qualification.


The executable VM suites are separate from those component claims:

- `vm_matrix.py` generates experimental encodings directly and executes
  scalar, SIMD, atomic, bulk and marker cases with checking enabled and disabled.
  Rejected accesses must produce the managed exit status 122 and the expected
  diagnostic; an arbitrary signal or parser failure is not a passing trap.
- `wasi_host_matrix.py` runs 42 real WASIp1 tagged host-buffer cases using `wasi_host_buffers.c`. It covers nested iovecs, payload and result pointers, random output, and interface denial before memory access, with memory32/memory64 and both execution backends. Tag failures require exact exit 122, the mismatch diagnostic, and no output; arbitrary signals or validation failures do not pass. Use `--clang` with an existing WasmMTE compiler, or `--fixtures` to reuse a checked compilation manifest and exact binary hashes. Shared-memory declarations in this suite do not qualify concurrent threads-profile calls.
- `minimum_runtime.py` compiles standard WAT with an existing `wat2wasm` and
  runs both execution backends. It checks zero/nonzero initial minima,
  `memory.size` before and after successful/zero/failed growth, preservation of
  old bytes, zero-filled new pages, separate owners with different minima and
  accesses crossing the grown boundary. Its optional `--wasmtime` runs the same
  binaries with Wasmtime, including explicit shared-memory enablement.
- `minimum_proof_ir.cc` accepts only the compiler's versioned bounds-read
  witness after its current-size result becomes unused. Used, unmarked,
  volatile, differently ordered or malformed reads remain. This test alone
  does not establish the memory owner's lifetime/publication precondition.

- `threaded_matrix.py` uses a genuine imported shared-memory owner and the
  production threads profile. It covers both backends, explicitly disables the
  persistent JIT cache, and separates interface exposure from the resource
  grant for real `wasi.thread-spawn` calls. The suite explicitly grants a
  64 MiB code budget for its large unoptimized SIMD/bulk fixtures;
  `--code-budget` controls that test grant without changing VM policy.
  Child-thread cases check accepted
  tags, rejected mismatches and tag zero, wait/notify completion, and growth.
  This suite currently targets shared memory32; it does not claim shared
  memory64 support or a race-free retire/check/access transaction.
- `hardware_initialization.py` exercises ordered preload starts followed by
  deferred active data segments. It checks imported aliases and independent
  owners with globals descriptors, memory32/memory64, and enabled/disabled MTE.
  Native x86_64 runs qualify software behavior only. Hardware qualification
  requires an AArch64 execution environment that explicitly reports MTE.
- `hardware_plan.cc` checks that the unpublished owner's hardware selection
  is stable before activation and through growth and moves. Compiler and cache
  policy must use that selection; a later activation failure must prevent
  guest publication. The component test does not replace actual CLI execution.

Run these suites through the configured resource-control launcher and place
`--out` under the repository's `.tmp/`. A shared memory declaration in a
single-threaded CLI fixture does not qualify concurrent guest execution,
retirement races, or every shared-profile host API.


`hot_replacement_runtime.cc` is a native host test driver linked to the same
runtime and provider objects as the VM under test. Its companion
`hot_replacement.py --fixture PATH --out DIR` emits extension-bearing Wasm
and complete replacement bodies, then checks both startup policies at both
address widths. The host driver explicitly selects the DEV launch profile;
guest modules receive no management import. It obtains an authentic stop and
activation before preparing and publishing the new generation. A real managed
fault must leave the result carrier untouched and is returned as status 122.
The suite separates ordinary success, tag rejection and zeroing with checking
disabled. It does not claim release-signature or whole-world restore coverage.
