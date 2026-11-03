# Platform and product acceptance

This is an executable acceptance checklist and an extensible evidence format.
It specifies work to perform; it is not a declaration that every item has
passed. Generate a fresh ledger for each exact OS, architecture, endian, ABI,
libc, compiler, VM, SDK, ROS distribution and RMW tuple. Carry no result from a
different tuple or an older executable into that ledger.

The ROS product is Linux, LLVM AOT and mandatory checked native unwinding.
There is no public ROS unwind-control option. A runtime must verify the real
generated call chain before executing applications and print accurate Wasm
frames after a crash. An interpreter, a macOS product, an unchecked native
path, or instrumentation replacing the native check cannot qualify this
product. Ordinary upstream Uwvm2 remains a separate capability subject.

## Evidence rules

Every acceptance row has seven independent dimensions:

| Dimension | Required evidence |
| --- | --- |
| `implementation` | Source paths and hashes, implemented semantics, feature gates, ownership and failure-path review, and applicable unit results. |
| `build` | Actual configure/compile/link commands and return codes; compiler, headers, dependency sources, target ABI and final artifact hashes. |
| `wasm_compilation` | Actual language source, producer executable/version/hash, flags, imports, exports, target width, validation and emitted Wasm hash. |
| `execution` | The identified VM executed those exact Wasm bytes; oracle, output, return status, traps and cleanup were checked. |
| `native_ros` | A real matching native ROS peer and actual Gateway were observed; request/result/callback identity and discovery are verified. |
| `qemu` | Exact emulator, CPU model, loader, sysroot or kernel/rootfs, guest binary and execution receipts; user and full-system modes are separate. |
| `performance` | Native hardware, matched peers/workload/permissions, correctness, warmups, raw samples, statistics and environment; emulator time is excluded. |

Use `PASS`, `FAIL`, `BLOCKED`, `UNTESTED`, `NOT_IMPLEMENTED`, or `NOT_APPLICABLE`
in a reviewed ledger. `NOT_IMPLEMENTED` needs an implementation audit;
`NOT_APPLICABLE` needs a scoped reason. Neither satisfies a required feature.
Skipped, disabled, undiscovered or filtered tests stay visible and cannot
become passes. Preserve the original failed receipt and identify its replacement.
An exit code of 77 means blocked, never success. A unit model is not native ROS
evidence; a source file is not a compiled module; a compiled module is not an
executed module; a QEMU result is not native performance.

`receipt.py` records actual commands using `PASS`/`FAIL`/`BLOCKED`, and leaves
unrun dimensions `UNTESTED`. Its overall result covers only the named commands.
It does not automatically certify every generated ledger row. The artifact
manifest hashes logs, subreceipts and the release ELF; `receipt.json` records
that manifest's digest. These hashes identify bytes, not a remote attestation.

## Start a Linux acceptance packet

Use absolute paths. Keep outputs outside the source trees. Do not compile or
execute qualification tests on the macOS authoring host. The Linux coordinator
holds the existing shared test lease for the complete batch, verifies the
original live 64 GiB cgroup with zero swap, and limits builds to one compiler.
The admission must pin the leaf, device, inode and boot ID; a new similarly
sized cgroup is not a substitute. Required ancestor memory events must remain
unchanged, and every observed owned process must remain in that same cgroup.

```sh
VM_ROOT=/absolute/uwvm2-ros
SDK_ROOT=/absolute/uwvm2-ros-sdk
PACKET=/absolute/fresh-qualification-packet
mkdir -m 700 "$PACKET"
python3 -B "$VM_ROOT/test/0080.product-qualification/inventory.py" \
  --source "$VM_ROOT" --sdk-root "$SDK_ROOT" \
  --platform linux-x86_64-little-gnu-libc-PINNED --out "$PACKET/ledger.json"
python3 -B "$VM_ROOT/test/0080.product-qualification/receipt.py" init \
  --source "$VM_ROOT" --out "$PACKET/product" --lane linux-x86_64-release-candidate \
  --require resource --require toolchain --require module-source --require module-checker-tests \
  --require ros-profile-tests --require qualification-helper-tests --require ros-security-units --require ros-ipc-units \
  --require configure --require build --require target --require closure --require version --require basic
python3 -B "$VM_ROOT/test/0080.product-qualification/receipt.py" command \
  --out "$PACKET/product" --id resource --timeout 19500 -- \
  python3 -B "$VM_ROOT/test/0080.product-qualification/guarded.py" \
  --source "$VM_ROOT" --admission /absolute/reviewed/runner-admission.json \
  --out "$PACKET/product/resources.json" -- \
  python3 -B "$VM_ROOT/test/0080.product-qualification/product.py" \
  --source "$VM_ROOT" --out "$PACKET/product" --llvm-prefix /absolute/reviewed/llvm23-prefix
```

The ledger generator reads the current public ROS and WASI import tuples and
the selected SDK's public language adapter map. It starts every cell untested.
A new public import or language route therefore introduces explicit new work.
It does not grant capabilities or launch compilers, ROS, QEMU or hardware.

The runner administrator supplies reviewed files following
[runner-admission.example.json](runner-admission.example.json) and
[provision-manifest.example.json](provision-manifest.example.json). Replace all
placeholders with actual identities. The global lock must already exist, belong
to the runner UID, have restrictive permissions, and be the same lease used by
other test work. Its device/inode is pinned; qualification never creates a
replacement lock or changes a cgroup. Pin the provisioning manifest's digest
in the admission. Inventory the compiler/linker/archive tools, Xmake, matching
headers and their actual dynamic library closure; the example lists only the
minimum required actor names and is deliberately not an admitted installation.
The guard checks every supplied file before starting actors. Toolchain admission
also compiles and executes a finite C++26 probe. That probe does not certify the
separate bundled LLVM/runtimes build.

The product coordinator also runs the real module/checker, ROS profile and
qualification-helper tests plus finite security/IPC units inside that same
guarded batch. The preprovisioned compiler probe precedes them; no persistent
runner package installation is performed. Disposable hosted PR work is limited
to static file analysis and supplies no functional or execution acceptance.

After all commands, seal and retain the packet even when a stage failed:

```sh
python3 -B "$VM_ROOT/test/0080.product-qualification/receipt.py" seal \
  --out "$PACKET/product" --job-status success
```

Use `--job-status failure` or `cancelled` when applicable. Missing required
commands prevent a pass even with `success`. Record each additional case using
`receipt.py command --id UNIQUE --dimension DIMENSION --subject ARTIFACT
--timeout SECONDS -- COMMAND ARGUMENTS...`; use a new receipt for retries.
Associate a row with the receipt and exact command, inspect its oracle, and
update only the dimensions that command actually proves.

## Acceptance checklist

### Source, toolchain and product boundaries

- [ ] Freeze the complete VM, SDK, test drivers, generators, official-suite
  locks and native peer sources. Verify every manifest entry before execution
  and recheck after the batch. Hash the actual ELF and emitted Wasm, not a
  version string, a path, an mtime, or a Git SHA alone.
- [ ] Record compiler, linker, assembler, Xmake, CMake, native loader and C++ ABI
  identities. Verify paired vendored LLVM, libc++, libc++abi, libunwind and
  OpenSSL sources and the actual link commands. No ambient system LLVM may
  satisfy the product dependency contract.
- [ ] Build an ordinary release product with `--execution-int=none`,
  `--execution-jit=llvm`, both selected ROS imports and the required native
  debugger. Run `xmake basic_test` against that exact ELF. Retain missing
  keyring, ptrace or kernel facilities as actual failures/blocks.
- [ ] Reject unsupported ROS OS/architecture/ABI and public interpreter
  configurations. Distinguish Linux x86_64 primary release qualification from
  AArch64 and little-endian ARMv7 hard-float candidates. ARM native ROS,
  debugger, libc and endpoint qualifications require their own witnesses.
  i686, soft-float, big-endian Arm, Windows and macOS cannot inherit ROS support
  from ordinary upstream capabilities.
- [ ] Use [the Linux CI lanes](../../.github/CI.md) for static file analysis, guarded contracts, release
  candidates and a distinct frozen ordinary-upstream full interpreter run.
  Never rename the latter as an uwvm2-ros interpreter product.

### Parser, validator and initialization

- [ ] Discover and run every applicable test under `test/0001.parser`,
  `test/0005.section_details`, `test/0011.initializer`, `test/0012.validator`
  and their maintained declaration/zero-body audits. Preserve discovered,
  selected, executed, passed, failed, skipped and unsupported counts.
- [ ] Check magic/version, section order/duplicates, custom/unknown sections,
  unsigned and signed LEB width/overflow, truncation, length arithmetic,
  UTF-8/names, function/code count, indices, type/stack/control validation,
  resource quotas and failed-allocation rollback. Cross-check malformed cases
  against the locked official suite and a named reference engine.
- [ ] Check all local/imported function, memory, table, global and tag forms;
  start functions; constant expressions; imported/provider ownership; active,
  passive and declarative elements/data; nullable/non-null references; memory64
  and table64 offsets; zero-body modules; and atomic initialization failure.
  Parsing, validation, initialization and execution need separate evidence.

### Ordinary upstream interpreter and LLVM execution

- [ ] Run the complete discovered strict interpreter suite from the immutable
  ordinary-upstream checkout, including all ABI modes and full basic/coverage
  cases. Retain its own source/ELF identity. Do not reduce it to a ROS smoke
  subset. Tiered, lazy, combine/delay, stack, numeric, memory, call, table,
  branch, trap and invalid-input variants need explicit inventories.
- [ ] For the ROS LLVM path, run all applicable IR verification, native ABI,
  argument/result, direct/indirect/reference/tail-call, floating environment,
  NaN/signed-zero/rounding, alignment, memory/table bounds and failure cases.
  Header-only/translation-only tests do not establish native execution.
  Interpreter-dependent LLVM test cases stay in the upstream subject.
- [ ] Audit the selected LLVM suite's discovered targets and exclusions. The
  current reduced ROS AOT driver is a finite suite, not all upstream LLVM
  capability coverage. Use an explicit target list and real built executables;
  a missing executable must fail rather than print a successful skip.

### WebAssembly 1, project 1.1, 2 and 3 feature coverage

- [ ] For Wasm 1, cover every implemented numeric/control/call/local/global,
  memory/table and conversion instruction, validation rejection and exact
  traps, including boundary values and imported/local combinations.
- [ ] For the project's `wasm1p1` profile, inventory sign extensions,
  nontrapping float-to-int conversions and producer expectations. The profile
  name is not a separate blanket standards-conformance statement.
- [ ] For Wasm 2, inventory implemented multi-value, bulk-memory,
  reference-type and SIMD behavior. Check enabled and disabled feature gates,
  imports/exports, initializer forms, null/bounds failures and all applicable
  official assertions.
- [ ] For Wasm 3 and separately implemented proposals, inventory typed function
  references/call_ref, recursive/GC types and subtyping, ref branches/casts,
  tail calls, exceptions/tags/try-table, extended constants, multi-memory,
  memory64, table64, relaxed SIMD, shared-memory and atomics. Assign each
  feature to its actual source/spec profile; never infer support merely from
  a version label or parser acceptance.
- [ ] Run the maintained initializer, branch, self-tail and tail-transfer
  drivers with `--ros` and the AOT backend; retain reference-engine and exact
  expected-trap evidence. No ROS command may pass an unwind-control flag.
  Existing drivers that still pass a removed flag require migration before
  they can supply current product evidence.
- [ ] Keep official WAST discovery/assertion results separate from self-checking
  generated witnesses. Pin official suite commits and enumerate every filter,
  unsupported proposal and missing assembler grammar. A finite Core 3 witness
  set does not mean full Core 3 conformance.

### Exceptions, traps, native checks and signed cache

- [ ] Exercise Wasm and native exceptions, imported calls, host reentry,
  direct/indirect/reference/tail chains, GC/statepoints and boundary cleanup.
  Require the real generated-chain native check and exact leaf/caller/entry
  crash frames. A no-op check, forced success, diagnostic instrumentation or
  fallback execution path fails acceptance.
- [ ] Exercise ordinary explicit traps and actual protected-page memory traps.
  Keep signal-frame, native debugger and unwind coverage independent. A
  user-QEMU signal-unwind failure remains a failure even if full-system QEMU
  succeeds with the same Wasm.
- [ ] Require completed cold cache storage, two fresh-process signature-verified
  hits, unchanged output and no replacement-object enqueue. Cover changed
  source/instructions/ELF, target/CPU/ABI, corrupt/truncated signatures,
  decompression limits, unchanged relocation/timestamps, changed initial data,
  read-only storage, links/pseudo-files, keyring refusal and origin conflict.
- [ ] Revoke ROS/WASI authority between processes using the same cached code.
  Cached objects must never restore grants, handles, sessions or owner state.
  Reboot/persistent-origin tests require actual distinct boot IDs. Cache
  correctness alone does not establish warm-start latency.

### Complete implemented WASI surface

- [ ] Use the generated import inventory for both implemented Preview 1 widths;
  test every listed export. Group by arguments/environment, clock/random,
  descriptor/preopen/path operations, polling, process/scheduling and sockets.
  Include enabled/disabled Wasm64/socket build profiles and actual import
  signatures. No single clock/write sample qualifies every WASI API.
- [ ] For every API, check allowed behavior, missing/revoked rights, pointer and
  length overflow/OOB, zero-length and aliasing cases, bad/stale descriptors,
  partial I/O, host error mapping, deadlines and cleanup. Test guest-visible
  memory before/after denied calls. A crash or a CLI parse error is not a valid
  capability-denial oracle.
- [ ] Cover capability inheritance/intersection, path traversal and symlink
  refusal, preopen isolation, descriptor duplication/renumber/close, environment
  inheritance controls, clock domains, random denial and socket family/endpoint
  restrictions. Use only the explicit test-owned filesystem/network resources.
- [ ] Audit Preview 2/3/component-parser, canonical ABI and interface execution
  independently. Platform selectors named `wasm-wasip2`/`wasm-wasip3` and source
  stubs do not prove those complete runtime interfaces. Mark missing
  implementations explicitly and keep acceptance blocked where required.

### Complete implemented ROS1/ROS2 surface and permissions

- [ ] Execute every namespace/export row generated from the current public
  host tuples. Inventory every selected native distribution, middleware and
  type catalog independently; importing `abi_version` is only admission evidence.
- [ ] Cover capability open/close/revoke, topic publish/subscription, event
  poll/watch/readiness, service client/server/availability, action goals,
  feedback/results/cancellation, TF lookup/broadcast, graph inspection/listing,
  all parameter types/events/operations, clocks and logging. Cover all separate
  ROS2 RMW publisher, subscription and service endpoint lifecycles.
- [ ] Require a real native peer for each native ROS row: decoded semantic
  result, exact endpoint/type/schema, observable callback or graph transition,
  and bounded discovery. Test QoS, empty/large/custom messages, ordering,
  service/action errors, cancellation, deadlines, queue pressure, partial I/O,
  restart/disconnect/reconnect and cleanup. Do not cache native results.
- [ ] For each authority-bearing operation, test absent, partial, overbroad,
  malformed, expired and revoked grants; wrong UID/session/backend/type/schema;
  forged/stale/reused handles; cross-module direct/reference/indirect/tail
  provenance; nested host reentry; exceptions; concurrent close/revoke; and
  cancellation precedence. Denied attempts must have no native side effect.
- [ ] Preserve independently discovered official ROS native-suite results and
  separate Wasm/Gateway differential results for each locked distribution.
  EOL SDK acquisition failures, absent APIs and unsupported hardware stay
  blocked/unimplemented. The existing distribution configuration tests do not
  qualify all 26 catalogued ROS1/ROS2 targets.

### SDK languages and binary/static/shared artifacts

- [ ] Freeze the separate SDK checkout, LFS objects, all public adapter sources,
  tool dependencies and generated definitions. Enumerate every registered
  language route and every declared target/exception/artifact capability.
  Preserve the user's actual source language through its public build entry.
- [ ] For every required language × wasm32/wasm64 × ROS1/ROS2 cell, produce and
  validate a real `binary`, `static` archive and `shared` Wasm module where
  implemented. Inspect archive members/relocations/imports/exports/ABI; link a
  real consumer or load the actual shared module. A renamed executable is not
  a shared-library result, and a generated placeholder is not source compilation.
- [ ] Run executable, linked-static and loaded-shared examples through the
  public SDK launcher with correct stdout/results, guest/runtime hashes,
  exceptions, callbacks, threads, memory ownership and unload/retirement.
  Verify all declared cross-language dependency edges separately.
- [ ] Execute denied clock/random/filesystem/network and ROS grants through
  each launcher/artifact form, then test explicit grants and revocation. Pure
  Wasm with ROS disabled must not create a Gateway. An enabled ROS deployment
  requires the actual selected Gateway and native distribution.
- [ ] Retain language runtime limits, GC/FFI/exception restrictions and
  unavailable library kinds as explicit gaps. JavaScript/TypeScript codec
  output alone does not certify a native robot or every language adapter.

### Debugging, audit, checkpoint and hot update

- [ ] Test matching VM/broker/adapter source identities, authenticated debugger
  access, real native disassembly/stepping, Wasm/source frames, stale reference
  rejection and all implemented language metadata. Separate instruction-level
  access from managed variable/type fidelity and from editor UI acceptance.
- [ ] Test allow/deny audit records against actual decisions and native side
  effects; retain ordering, bounded output, write errors, concurrency,
  redaction, lost-peer cleanup and behavior with audit sinks unavailable.
  Audit failures must not manufacture authorization or a successful operation.
- [ ] Inventory actual checkpoint/hot-update interfaces before claiming support.
  Exercise source/artifact/state identity, continuation ownership, safe points,
  native frame retirement, outstanding calls/events/threads, atomic commit and
  rollback, incompatible-version refusal, revoked authority and crash recovery.
  Serialization-only or synthetic ledger tests cannot prove live hot update.

### Native, QEMU and performance acceptance

- [ ] Record actual ELF class/machine/endian/float ABI and interpreter loader.
  For each candidate, execute the exact built product on native hardware where
  available. Keep native ROS endpoint acceptance independent of CPU execution.
- [ ] Run user-QEMU and full-system QEMU as different cells. Pin emulator,
  CPU/features, sysroot/loader and guest kernel/rootfs. Require positive output,
  expected traps, checked unwind, keyring/cache, debugger, ABI and cleanup in
  the applicable environment. List all skipped guest facilities explicitly.
- [ ] Benchmark only on identified native hardware with fixed affinity,
  controlled load, original resource admission and matched correctness/grants.
  Compare retained native client, IPC-only and Wasm/Gateway paths using the
  same peer. Separate startup, compilation, cold-cache, verified warm-cache,
  discovery and steady-state execution.
- [ ] Preserve warmups, interleaved comparison order, all samples, failures,
  timeouts, callbacks, medians/dispersion and a declared regression threshold.
  Keep profiler instrumentation and PMU reliability separate from native
  latency measurements. Unsupported PMU/GPU/physical-device facilities stay
  blocked; no QEMU timing may qualify native performance.

## Extend and review

Add new families to `FAMILIES` and extend the public-inventory extractor when
the import/adapter declaration format changes. The extractor refuses empty or
ambiguous namespace inventories. Add a stable driver and its explicit oracle,
prerequisites, timeout, required dimensions and expected negative cases. Review
the discovered target count and exclusions before execution. Retain source,
artifact, raw logs and failure evidence together; publish only the exact scope
whose required dimensions passed. No item is complete merely because its
workflow, fixture or checklist row exists.
