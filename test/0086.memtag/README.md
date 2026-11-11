# WasmMTE extension contract checks

`instruction_contract.cc` pins 20 experimental descriptors and their memory-dependent integer signatures. Nineteen operations are defined; `memtag.add` is reserved and rejected. Their semantic identities follow the referenced LLVM `mt-2` revision, but the sole accepted wire is now `FF ULEB32((1<<24)|(1<<16)|operation) ULEB32(memoryidx)`. Historical FC MTE, abandoned FF/vendor fields and `memtag.globals` version 1 are not compatibility modes. The contract test also checks reserved holes, wide invalid values, tag geometry, byte-length roles, memory effects and globals-v2 identity at compile time and in the executable.

That component alone does not establish parser, interpreter, JIT, memory-check or sandbox coverage. Separate executable suites exercise those paths. The private opcode table deliberately lives outside standard Core opcode enums; standard FC bulk/table operations remain standard.

`namespaced_encoding.cc` tests the single packed word, including every 8-bit version/domain and 16-bit operation value, legal padding, truncated and overflowing LEBs, cursor rollback, debug listing/event classification, and rejection-only detection of historical FC bytes. `validator.cc` exercises both validators with memory32/memory64, canonical/padded words, stack types, memory ownership, feature gates and obsolete encodings. `globals_section.cc` and `globals_v2.cc` test the sole globals-v2 parser, immutable all-input validation and canonical-owner granule overlaps.

`debug_view.cc` covers allocation-tag copies from private/shared wasm32/wasm64 owners, actual-length and overflow boundaries, unaligned granules, disabled state, moved owners, isolated memories, console selectors, and color rendering. `debug_console.cc` checks the native numeric/hex/RGB protocol format consumed by DAP. The matching Python tests are `test/0018.debugger/test_dap_memory_tags.py`; their modeled protocol checks do not replace actual owner activation or hardware qualification.


The executable VM suites are separate from those component claims:

- `namespaced_vm_matrix.py` generates 181 fixtures: canonical/padded packed
  words, padded memory-index feature controls, unsupported namespaces,
  reserved operations, malformed words, all old FC/FF operation encodings in
  reachable/unreachable/unused functions, and unchanged standard FC bulk
  behavior. Rejection needs exit 127 with the expected validation diagnostic;
  a trap signal cannot substitute for a controlled rejection.
- `core3_linear_matrix.py --encoding both` generates 1,686 fixtures covering
  scalar/SIMD accesses to both memory indices at both widths, source/destination
  bulk checks, globals-v2 owner selection and conflicts, and explicit old-v1
  rejection with checking enabled or disabled. `packed-padded` pads the one
  instruction word; it does not introduce another encoding grammar.
- `core3_gc_matrix.py --encoding both` generates 32 ordinary semantic cases
  plus six AOT-only collector cases. The positive examples reach all 31 Core 3
  GC aggregate/reference subopcodes, including pure GC modules without a
  memory, data/element arrays, packed fields, casts, table64, and preserved
  tagged numeric pointers. Negative cases cover selected bounds/null/type/tag
  failures. One positive sample per opcode is not exhaustive GC, root-lifetime,
  recursive-subtyping, cross-module, EH or concurrent-collector qualification.
  GC domain 2 is explicitly rejected, not implemented as guest GC tagging.
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
- `scalar_llvm_ir.cc` verifies and executes the actual tag-check IR through
  MCJIT, covering byte widths 1 through 16, every address alignment, both
  address widths, and ordinary/atomic shadow loads. A mismatch must suppress
  the entire data store; the failure granule is its starting byte offset.
- `async_llvm_ir.cc` adds a returning fault recorder, verifies its control
  flow, then executes synchronous and deferred variants. It checks captured
  pointer/tag/granule evidence, immediate bounds rejection, and full/partial
  comparison masks. Deferred mismatch deliberately permits the data write;
  this component does not claim synchronous suppression for async mode or
  physical Arm MTE qualification. LLVM 23 `getTerminator()` requires an
  already terminated block, so open-block tests inspect the last instruction
  instead. The old nullable-query assumption loses both the mismatch branch
  and the separate native source-boundary calls.
- `async_vm_matrix.py` runs genuine scalar/SIMD/atomic/bulk, multi-memory,
  host-exit, growth, loop and optional child-thread cases in the explicitly
  selected fault modes. Software-async faults must identify the exact
  generator-known function, instruction offset, memory and granule, with an
  available module identity. Wrong or missing provenance fails even if exit
  122 is correct. Hardware-async delivery cannot promise the same precise
  faulting-instruction attribution. Signals/timeouts never satisfy its oracle.

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
- `hardware_signal_mask.cc` checks real Linux/AArch64 per-thread admission in
  both synchronous and asynchronous hardware modes. A main thread, a worker
  that independently blocks `SIGSEGV`, and a worker inheriting that mask must
  each receive the explicit setup error before Guest execution. The test
  verifies that rejection preserves the caller's mask, process handler, and
  tagged-address control, and that explicit caller restoration permits valid
  tagged access. A blocked signal is never accepted as a managed fault exit.
- `async_pool_start.cc` forces a real `pthread_create` failure after two live
  workers have started, then contends with a second `pool.start` while failure
  cleanup joins those workers. Link a static C++ runtime and
  `-Wl,--wrap=pthread_create`; a shared runtime may bypass the interposition.
  The test checks that lifecycle serialization covers the join/clear interval,
  that a later retry succeeds, and that an established worker count is stable.

All tag-fault oracles use `vm_diagnostics.py`: the process must return 122 and
the single human fault report must say `memory-protection`, carry exit status
122 and the expected reason. Execution faults analogously require 121 and
`execution`. Bare signals and JSON-looking text are never successful fault
evidence. The presentation parser is a test helper, not a deployment API;
hosts consume the authenticated diagnostic channel or structured snapshot API.

The real-VM runners accept `--runner 'qemu-aarch64-static -cpu max -L SYSROOT'`
where supported; an emulator run is functional evidence, not native timing.
Use `--generate-only` to inspect immutable fixtures and their SHA-256 manifest
before a VM build is available. A generated manifest is not an execution pass.

`scalar_bench_generate.py` creates real Wasm scalar workloads for linear,
sequential, random and dependent-chase access, at both address widths. Each
checks the complete expected footprint and relevant pointer entries, not only
a checksum. It supports `packed` and `packed-padded`. `scalar_bench_run.py`
checks manifest hashes and wire identity and records repeated timing runs.
This hand-encoded execution benchmark is not proof of end-to-end C/C++ SDK
instrumentation. Compiler-produced heap/WASI suites require the rebuilt guest
producer and libraries described in `tools/memtag/README.md`; old artifacts
must fail the new VM rather than be accepted as a compatibility shortcut.

## Experimental private-loop versioning

`loop_versioning_vm.py` uses the current packed encoding and executes the same
actual AOT candidate twice per fixture: once with the experiment removed from
the environment and once with
`UWVM_STUDY_MEMTAG_LOOP_VERSIONING=exclusive-private`. The optimization remains
off by default. This exact opt-in is a host promise that a private memory owner
has only one executor for the complete check-and-access interval. A lifetime
lease alone does not establish that promise. Do not enable it for concurrent
host mutation, shared owners, or asynchronous fault delivery.

From the repository root, run the semantic checks with an ordinary, unmodified
AOT build. Use a fresh output directory for every invocation:

```sh
python3 test/0086.memtag/loop_versioning_vm.py \
  --candidate /absolute/path/to/uwvm2-ros \
  --out .tmp/loop-semantics
```

The default 44 executions cover memory32 and memory64. Successful fixtures
check every byte of the working footprint and its guards, an independently
computed checksum, and a completion exit of 37. Failure fixtures require the
controlled exit and precise tag/granule evidence. They include an interior bad
granule, a differently tagged hole that the guest never accesses, an earlier
`unreachable` before a later bad granule, and a tag failure before a later
out-of-bounds access. The early-trap condition comes from active data loaded
before the loop, so it is known to the test without becoming an LLVM constant.
The default 262,144 iterations leave enough counted latches after the observed
four-way ordinary unrolling to exceed the experiment's 32,768-latch admission
floor; a smaller custom count is not guaranteed to qualify. Calls, in-loop
retagging, atomic accesses, shared memory,
and asynchronous mode are exclusion controls. A failed preflight must select
the original checked loop; it must not report a future fault early. Signals,
timeouts and validation rejection do not satisfy these runtime oracles.

Semantic success does not establish that LLVM emitted a versioned loop. For
that separate check, prepare a test-only source overlay against the exact
source snapshot used by an existing build:

```sh
python3 test/0086.memtag/loop_versioning_vm.py \
  --prepare-observer /absolute/path/to/build/source \
  --build-receipt /absolute/path/to/build/receipt.json \
  --out .tmp/loop-observer
```

The receipt must contain `stages` with the actual compiler/linker `command`
argument arrays and their `cwd`. The preparation checks unique source anchors,
records the original and observed source hashes, and writes `overlay.json`
plus `observer-plan.json`. The overlay adds one include and two cold compiler
snapshots, before and after optimization; it does not alter production files
or build anything. The plan preserves the original runtime compiler flags,
reuses the identified main/host objects, and names a separate observed runtime
object and executable. Execute its `compile_command` in `compile_cwd`, then
its `link_command` in `link_cwd`, with checked return codes. Retain both build
logs, hashes of the reused objects, and the resulting executable's SHA-256.

Use that independently identified executable only for IR observation:

```sh
python3 test/0086.memtag/loop_versioning_vm.py \
  --candidate /absolute/path/to/uwvm2-ros-loop-observer \
  --require-ir --out .tmp/loop-ir
```

The runner disables the persistent JIT cache, creates a new IR directory per
execution, and records the IR file hashes. Eligible enabled fixtures require
the versioned-function attribute, the preflight scan, a cloned fast loop, and
absence of byte tag loads in the cloned blocks. Disabled and excluded
fixtures must not carry the versioning attribute. This is evidence about the
optimized IR emitted by that build, not a timing inference or a claim that
every eligible real program takes the fast branch.

Run these suites through the configured resource-control launcher and place
`--out` under the repository's `.tmp/`. A shared memory declaration in a
single-threaded CLI fixture does not qualify concurrent guest execution,
retirement races, or every shared-profile host API.


`hot_replacement_runtime.cc` is a native host test driver linked to the same
runtime and provider objects as the VM under test. Its companion
`hot_replacement.py --fixture PATH --out DIR` emits extension-bearing Wasm
and complete replacement bodies, then checks both startup policies at both
address widths. The host driver explicitly selects full debugger authority;
guest modules receive no management import. It obtains an authentic stop and
activation before preparing and publishing the new generation. A real managed
fault must leave the result carrier untouched and is returned as status 122.
The suite separates ordinary success, tag rejection and zeroing with checking
disabled. It does not claim release-signature or whole-world restore coverage.

`reporting_vm.py` runs explicit silent-log (record-and-continue) checks for scalar, SIMD, atomic,
bulk copy/fill and GC-carried pointers, both memory widths and interpreter/JIT.
It checks stdout/stderr/append-file logs, every touched mismatching granule,
continued byte effects, default crash behavior and hard sandbox bounds.
`reporting_host_vm.py` checks WASI buffer continuation while interface/resource
denials suppress I/O and ordinary bounds still reject.
`reporting_component.cc` exercises eight concurrent execution streams and
32,000 complete reports per sync/async run, suitable for native ThreadSanitizer.
Generated outputs must be directed to the project's `.tmp` under the admitted
64 GiB / zero-swap cgroup. Hardware-async QEMU runs are correctness checks only.

`gc_carrier_edges.py` distinguishes complete i32/i64 carriers from `ref.i31`,
packed i8 fields and i64-to-i32 truncation. It checks retained tags and rejection
of lossy reconstructed pointers, with explicit silent-log (record-and-continue) as a separate
host policy. No GC tagging instruction or implicit retagging is introduced.
`reporting_output_failure.py` checks a full output device, closed receiving pipe
and missing output directory. Accepted log delivery errors use a controlled
internal status; report-path configuration errors use configuration status.
`reporting_cache.py` warms actual authenticated native objects under each report
action and then triggers a mismatch on cache hits; test output/cache ancestors
must satisfy the existing private-directory admission rules.

`disabled_wasm_ir.cc` validates and lowers all nineteen defined instructions,
both address widths and both report actions with MTE execution disabled. It
checks the actual generated IR, rather than inferring absence of instrumentation
from a timing sample. This does not establish zero whole-process performance
cost: native handler placement, startup and translation still require measurement.

`../0017.runtime/run_signed_policy_sequence.py` executes consecutive signed
policy/code transactions with all four debugger permissions and synchronous /
software-asynchronous crash / log contracts. Current and projected policy
fingerprints must include the same immutable MTE execution-safety contract.

`origin_ir.cc` checks all 19 defined operations at both address widths through the production parser and LLVM translator. Synchronous crash mode emits no MTE origin calls; synchronous log mode emits one per operation, with no adjacent duplicates. `disabled_wasm_ir.cc` also checks the unoptimized lowering directly: identities, constants, subtraction and required zero fills introduce no hidden tag work.
