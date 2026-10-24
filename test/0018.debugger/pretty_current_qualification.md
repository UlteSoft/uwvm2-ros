# Native Rust Rc/Arc/Weak qualification — 2026-10-20

Rc/Arc/Weak code and tests are synchronized across the three repositories. The functionality checks below passed. The complete console/VTune comparison and original Rust source regression batch remain unfinished, so this report does not establish complete GDB/LLDB parity.

The formatter supports genuine Rust1.90 thin sized Rc<T>, Arc<T> and Weak<T>, including nested String/Vec and unit payloads. NonNull/inner/template records prove the payload type. Unsigned target-width Cell/AtomicUsize wrappers, separated count/payload fields, copied pointer-provenance masks and full-allocation arithmetic are checked before guest copies. Unknown or fat/unsized ABI, partial copies, invalid counts, locked Arc weak counts and exhausted budgets preserve the raw object.

Logical weak counts exclude the implicit live weak reference. Expired Weak reads only the count prefix and never copies the dropped payload. Empty dangling Weak requires no guest read. Explicit Rc/Arc Deref uses the same proof; Weak cannot be dereferenced. No guest function or ownership-counter write is introduced.

The three native executables contain nine fresh product translation units built against genuine LLVM23.1.1-uwvm-ros.13 with the installed SDK23.0git Clang. All 13 configured language macros are enabled in these builds; this does not qualify every language producer or compiler ABI. Runtime and driver bytes come from frozen source1357. Publication verified all 15 code/test bytes and modes, all 5,498 actual compiled repository inputs, and all 14,524 public ROS LLVM source/recipe entries.

| Check | Passed result |
| --- | --- |
| R1353 components | 42 genuine C++/Rust DWARF4/5 cases, all3, TLS on/off, 24 fresh test translation units |
| R1383 native products | 3 executables, 9 fresh translation units |
| R1385 Rc source/DAP | 12 actual source-breakpoint sessions, all3, DWARF4/5, instruction/unwind |
| R1395 named modules | 19 named units, full 26-file fast_io provider, linked TinyGo DATA and genuine RustStd/VecDeque/Rc DWARF4/5 consumers |
| R1397 shared C++ indexing | 24 array/index cases, all3, TLS and Rust switches on/off |
| R1371 QEMU | 29 genuine owned-type ELF/ABI replay cells |

The source sessions check breakpoints after the prologue, watch/hover/variables, named pagination, copied strong/weak counts, live/expired/empty Weak, nested and unit payloads, explicit Deref, original natural exit and stale-reference rejection. Each session observes VM16, the matching ELF/PID start time and the actual inherited control-socket FD.

QEMU covers ordinary/stable x86_64, i686, aarch64, arm, armhf, loongarch64, ppc64, ppc64le, mips64, riscv64, sparc64 and s390x. ROS covers x86_64, i686, aarch64, arm and armhf. These cells replay native-exported owned type records across real foreign host ABIs. Foreign LLVM parsing/JIT/source VMs and a genuine Wasm64 Rust producer remain unqualified.

Every compiler and executable test runs through the unchanged private read-only-root/owned-process harness in the existing SSH Linux 64GiB/swap0 cgroup. Native peak memory.current reached that hard bound without an OOM delta; continuous 6GiB spare memory is not claimed. Admission gates remain 6GiB for native builds, 4GiB for source/performance, and 2.5GiB for components. Earlier false producer/build/admission receipts remain false.

## Incomplete current validation

R1411 has completed 4/24 original RustStd/VecDeque source sessions at this snapshot; phase: actual-r1411-uwvm2-rust-deque-dwarf4-instruction. The unfinished sessions remain required.

R1393 has completed 6/10 console controls and 0/10 hardware collections at this snapshot; phase: gdb-b. Actual Hotspots and Microarchitecture sampling for this increment remains required.

The first A controls below are preliminary. The common source reads Rc<String> and Arc<Vec<i32>> through human-console pipes, including one read-only print-limit acknowledgement per query. Each control measures 3,000 queries. UWVM uses Wasm32 while native GDB/LLDB use native64; output syntax and bytes differ. GDB uses the Rust1.90 toolchain Python printers and LLDB21 uses that toolchain's shipped Rust Python integration. Provider pins, unchanged stop/counts and natural exit passed in the completed controls. The shared host and incomplete crossed experiment keep performance_eligible=false.

| Engine | Preliminary A median (us) | p95 (us) |
| --- | ---: | ---: |
| uwvm2 | 69.658 | 80.938 |
| uwvm2-ros | 72.009 | 84.525 |
| uwvm2-stable | 73.539 | 85.206 |
| gdb | 76.445 | 88.569 |
| lldb | 1230.986 | 1671.542 |

These figures do not establish formatter-only cost, a before-Rc speedup or general debugger parity. Earlier 208→128us and VecDeque results belong to other workloads/cohorts and remain separate.

Closed historical outputs were archived under exact path/type/mode/link/content manifests for resource cleanup. R1455 remains failed after its 3GiB disk-reserve refusal; separate R1457 verified and retired only its complete 44-root subset. The partial root and external relocated bodies were retained. Historical replay requires the matching archives and pinned external bodies. Current P1383 executables, frozen source1357, genuine LLVM1191/1193 and actual producer fixtures remain in place.

## Remaining scope

Genuine remaining language/compiler ABIs, Rust maps/async/fat layouts, actual ROS1/ROS2 middleware/network and ROS-level combinations, the sealed typed ROS mutation contract, broader Wasm3/hot-update/EH/cache/dynamic-provider checks, full foreign LLVM/source VMs and complete endpoint/platform security review remain open. A historical rare stable coroutine startup SIGSEGV remains unrooted.

## Resource recovery and pending continuations

A later resource snapshot still has6/10 console controls,0/10 current hardware collections and4/24 original Rust source regressions complete. The unchanged4GiB source/performance admission gate remains required. One continuation is queued for each original pipeline; it can run only after an owned zero-child admission timeout, preserves that original failure and references the already successful prefix. It does not retry an actual test failure or change memory/namespace/result qualifiers.

A genuine Rust async producer is queued through the existing2.5GiB component gate to inspect Unresumed/Pending/Returned state metadata and a saved String across suspension. Compilation has not started at this snapshot; async formatting remains unimplemented.

Historical performance cleanup1477 preserves241 original aliases and their relocated bodies in a single160,523,922-byte exact-path archive at SSH Linux `/home/macromodel/Documents/uwvm-dbg-archive-store-20261020-r1477/closed-performance.tar.gz`, SHA256 `746437991d05adad93ec27954bd5470a8a8d8c92c3a883f68a4e800983a5bd57`. Every archived body/type/mode/link was verified on Linux and independently copied/readback/fsynced on Mac before the2,823,690,411-byte old ext4 bodies were retired; the final archive was then written/readback/fsynced on ext4. Historical replay must restore both original alias paths and relocated bodies from its verified manifest. The original1475 archive path remains an alias to this final archive.

Cleanup1479 and1483 additionally relocated134/2,664 closed output/metadata files (510,843,321/846,343,537 bytes) with exact direct-I/O copy/readback and atomic original-path aliases. Current source, executables, LLVM provider and actual fixtures remain present. These are file-body relocation totals, not net memory reductions; other shared cgroup data continues to vary. No cgroup controls, cache eviction, mounts, signals or other agents' data changed.

## Prior VecDeque qualification

The following dated evidence belongs to its own cohort and is retained as historical context.


This records the new VecDeque batch alongside the earlier metadata-index evidence. Complete GDB/LLDB parity, all genuine native producers/compiler ABIs, external ROS middleware and full foreign LLVM/source VMs remain open.

The three repositories share checked Rust VecDeque layout selection, bounded ring-prefix copies and logical indexing. Owning VecDeque<T>/RawVec<T> template DIEs establish the element type even when RawVecInner erases its pointer to u8. Unsigned counters, distinct bounded fields, copied pointer masks and overflow across the complete allocation are checked before any guest payload read. A wrapped prefix needs at most two owned reads. Failed prefix copies preserve the raw object. Nested String values reuse the existing bounded reader; unit elements perform no payload reads.

All nine native product translation units were freshly compiled against the genuinely built LLVM 23.1.1-uwvm-ros.13 dependency. The bootstrap compiler remains the installed SDK23.0git Clang. No fake capability define or mixed LLVM headers/library provider bypassed the ROS contract. The native dependency is X86 only. The current public ROS vendor and CMake recipe matched all 14,524 captured entries before and after code publication.

Publication checks all 15 replacement bytes/modes and all 5,498 actual compiled repository source inputs. Runtime headers are from source1163; the updated Python driver and separately compiled component/module consumers are explicit overlays, so the complete frozen runtime manifest is not relabeled as their byte-identical snapshot.

| Current check | Passed result |
| --- | --- |
| P1201 native products | 3 executables, 9 fresh translation units |
| R1207 genuine producer metadata | 30 C++/Rust cases, all3, TLS on/off, 18 fresh test translation units |
| R1239 shared indexing | 24 C++ array/sequence checks, all3, TLS and Rust switches on/off |
| R1255 named modules | 19 named units, full 26-file fast_io provider, linked TinyGo DATA and Rust DWARF4/5 consumers |
| R1299 C++/Rust source/DAP | 33 real sessions: 21 original regressions plus 12 VecDeque DWARF4/5 instruction/unwind combinations |
| R1299 TinyGo source/DAP/CLI | 24 real objects/collections/arrays/builtins sessions, all3, instruction/unwind |
| R1213 endpoint lifecycle | 36 actual inherited-endpoint/proc_exit retirement cases |
| R1187 QEMU | 29 fresh ELF/ABI cells replaying genuine Rust type metadata and ring/index/nested/ZST checks |

The source/DAP driver checks watch, hover, variables, indexed pagination through the physical wrap, logical expressions, natural exit and stale-reference rejection. All sessions match the current product ELF and observed VM16/process/control-FD facts; the CLI builtin cases use their applicable process witness. The aggregate keeps 29+4 source sessions and 19+5 TinyGo sessions after inode-only prelaunch refusals. It verifies every successful script, runner, driver and actual-result hash. The original failed aggregates remain failed; their limits were not weakened.

QEMU covers ordinary/stable x86_64, i686, aarch64, arm, armhf, loongarch64, ppc64, ppc64le, mips64, riscv64, sparc64 and s390x; ROS covers x86_64, i686, aarch64, arm and armhf. These are metadata/DATA replay checks across host ABIs, not full foreign LLVM parsing/JIT/source VM qualification or a genuine Wasm64 producer.

All executable checks use the existing SSH Linux 64GiB/swap0 cgroup, pinned private read-only root and owned-process cleanup. Native build memory.current reached the existing hard limit; OOM counters did not increase. No cgroup control or cache-eviction setting changed. Closed historical source1059 was archived with path/type/mode/link/content verification when inode admission refused further guests; historical replay requires restoring its exact files. Four original TinyGo783 source fixtures were separately restored from the verified source717 archive.

Closed P1021/P1061 ELF/objects, historical R1123 raw collections and failed R1209 PCMs were subsequently archived after full path/mode/link/content verification. Their 2.70GB bodies occupy 668MB compressed. Historical replay must restore their exact paths from /dev/shm/uwvm-pretty-storage-r1321/closed-artifacts.tar.gz (SHA256 1c53a4e3316464e9de2d6cc339a4ac81bfddb48e6057bee92ab8df489a6cdce4). Historical R1123 per-run receipts, closed old test ELF/objects and qualified R1255 PCMs were subsequently archived at /dev/shm/uwvm-pretty-storage-r1325/closed-artifacts.tar.gz (SHA256 b0aca80532f0408bbe778af292cefffa5669a4c770a6bead4e5a2413ad217870). Restore those exact paths for historical/module replay. Pipeline receipts, selected summary CSVs, native producer fixtures, all new P1201/R1301/R1303 artifacts and the current LLVM provider remain in place.

### Fresh console and hardware comparison

The workload uses the same original Rust1.90 source and standard library: a wrapped eight-i32 queue and four String values. Native GDB/LLDB use native64 while UWVM uses Wasm32. Each control times 3,000 human-console queries; each hardware run measures 60,000. Read-only print-limit acknowledgements are included in latency. Setup, compilation and first stops are outside the latency timer. Hardware windows also include surrounding debugger/status/finish work. Unit queues are checked separately and excluded from common shipped-printer timing.

GDB uses the Rust1.90 toolchain Python printers; LLDB21 loads that toolchain's lldb_lookup/lldb_commands/providers, as used by its Rust integration. This Rust comparison does not measure the native LLDB C++ container formatter. Actual mapped debugger libraries and loaded GDB printer modules are pinned before/after.

ROS/GDB/LLDB controls run in crossed order. Ordinary/stable controls follow as additional runs; this is not a fully counterbalanced five-engine experiment. The shared host, different output byte counts and ABI differences keep performance_eligible=false. These measurements do not establish formatter-only cost, general parity or a speedup over a previous implementation that lacked VecDeque.

| Engine | A median (us) | B median (us) | Mean of medians (us) | Bytes/query |
| --- | ---: | ---: | ---: | ---: |
| uwvm2 | 89.112 | 93.774 | 91.443 | 315.500 |
| uwvm2-ros | 89.240 | 90.022 | 89.631 | 315.500 |
| uwvm2-stable | 90.319 | 92.915 | 91.617 | 315.500 |
| gdb | 147.518 | 156.601 | 152.060 | 130.175 |
| lldb | 3273.827 | 3388.721 | 3331.274 | 424.500 |

Every engine/repository has actual hardware Hotspots and detailed Microarchitecture collections. Qualified target-PID/start/TID/P-Core/PMU evidence and positive retired-instruction/clocktick samples are required. The following main-TID values are scaled sampled VTune estimates, not exact counters or whole-process totals. An absent sampled function row can reflect inlining or sampling and does not mean zero cost.

| Engine | Main-TID CPU seconds | Instructions (billions) | Clockticks (billions) | CPI |
| --- | ---: | ---: | ---: | ---: |
| uwvm2 | 5.406746 | 41.592 | 24.240 | 0.582804 |
| uwvm2-ros | 5.317460 | 37.296 | 24.408 | 0.654440 |
| uwvm2-stable | 5.128968 | 42.336 | 24.888 | 0.587868 |
| gdb | 7.876984 | 74.928 | 36.840 | 0.491672 |
| lldb | 28.253966 | 284.496 | 128.064 | 0.450143 |

The machine-readable companion preserves p95/category data, target identities, loaded-provider counts, raw report hashes and comparison limits. All 20 performance runner OOM deltas are zero. The older 208→128us and index-batch results belong to different workloads/cohorts and are not combined with this batch.

### Remaining scope

Genuine remaining native-language/compiler profiles, Rust maps/reference-counted collections/async and other ABIs, actual ROS1/ROS2 middleware/network and ROS2-level combinations, the separately typed sealed ROS mutation contract, broader Wasm3/hot-update/EH combinations, full foreign LLVM/source VMs and endpoint/platform security review remain open. The strict ROS mutation census continues to refuse source writes. Static-provider executable identity was reviewed; cache-on/hot-update/dynamic-provider behavior still needs runtime qualification. A historical rare stable coroutine startup SIGSEGV remains unrooted; later passing checks do not establish its repair.

### Prior metadata-index batch

The following dated evidence is retained as historical context. Its missing original HLL provider and numbers apply to that cohort, not the new complete ROS13 dependency or VecDeque workload.


This records the published R1109 metadata-index batch and its functional/performance evidence. Complete GDB/LLDB parity, all native producer/compiler combinations, external ROS middleware and full foreign LLVM/source VMs remain open.

#### Published source and functional checks

The three repositories share five updated files each: an immutable physical-subprogram candidate index for numeric queries, reuse of the same validated physical result during named lookup, explicit `<algorithm>`/`<optional>` CPPM imports, native-endian copied-i32 producer tests, and physical-selection/boundary/failure regressions. The index retains exact immutable-span identity, original variable order, PC/range/lexical selection, metadata/result budgets and a complete-scan fallback. No value bytes, stopped frame, read permission or source-generation authority are cached.

Runtime source R1059 manifest: 8cc52c2a50ed8f11ff92c488cd6e25c8eae24d890ed3959199a257e57552b5c9.
R1061 build receipt: bf8d87e3f20db6a8f05144e52a03a6c24991d3355ba88e3aaa2da0098a863f78.
All nine product translation units were freshly compiled. Publication verified all 15 replacement hashes/modes and all 5,498 actual compiled source dependencies against the three public trees.

The CPPM and test changes are separately qualified overlays outside those native compile inputs: R1079 compiles the changed CPPM with identical R1059 runtime headers; R1101 uses the corrected producer unit; R1099 adds the query regression. The complete R1059 source manifest is therefore not relabeled as a byte-identical snapshot of the later module/test/document files.

| Check | Result |
| --- | --- |
| R1061 products | 3 products, 9 fresh translation units |
| R1079 named modules | 19 partitions, full 26-file fast_io provider and linked TinyGo-map DATA smoke |
| R1081 protocol suites | 159 scripts, 1,584 tests, no optional packet skips; 9 fresh native packet generators |
| R1083 original TinyGo source/DAP/CLI | 24 sessions, all 3 products, instruction/unwind |
| R1085 original C++/Rust source/DAP | 21 sessions, including coroutine and DWARF4/5 |
| R1101 producer-metadata components | 18 TLS/no-TLS C++/Rust cases; caps, foreign owners, fresh values, scope boundaries and 6 concurrent readers |
| R1099 QEMU queries | 87 copied-DATA checks: 3 units × 29 fresh ELF/ABI cells |

QEMU covers ordinary/stable x86_64, i686, aarch64, arm, armhf, loongarch64, ppc64, ppc64le, mips64, riscv64, sparc64 and s390x. ROS covers its supported x86_64, i686, aarch64, arm and armhf. These new cells execute scalar/object/global query paths, physical-result clearing and host-native carrier checks. They do not execute the private LLVM-parsed candidate-index branch or a full foreign debugger VM. H1095's attempted foreign parsed-index compilation failed for lack of configured foreign LLVM; H1089's generated-script syntax failure occurred before tests. Both are retained.

The prior R1025 baseline provides 36 inherited-channel lifecycle cases; it was not counted as a new R1061 run. The index batch does not alter that lifecycle code. Earlier R1037 TinyGo map ABI, registered-worker retirement, explicit DAP domain continuation and ROS reference-lifetime improvements remain present.

All executable checks used the original SSH Linux 64GiB/swap0 cgroup, H371 private read-only root and retained-pidfd cleanup. R1061 observed memory.current=68,719,476,736 bytes at the kernel limit; OOM counters did not increase. Kernel limits and cache-eviction controls were unchanged.

#### Fresh crossed GDB/LLDB comparison

R1123 passed all 16 jobs: 8 controls and 8 actual VTune hardware collections. The control order is baseline/index/GDB/LLDB/LLDB/GDB/index/baseline. Each control times 3,000 vector16/two-21-byte-string queries through human-console pipes, with one read-only print-limit acknowledgement per query. Startup, compilation, stops and 30 warmup rounds are outside the latency timer. Original object values, unchanged stops, natural guest exits, actual loaded executable libraries and GDB Python printer files are checked before/after.

| Engine | A median (us) | B median (us) | Mean of medians (us) | Bytes/query |
| --- | ---: | ---: | ---: | ---: |
| UWVM baseline | 76.007 | 75.549 | 75.778 | 183.333 |
| UWVM index | 68.701 | 72.428 | 70.565 | 183.333 |
| GDB | 39.931 | 39.309 | 39.620 | 127.696 |
| LLDB | 109.306 | 109.359 | 109.332 | 232.000 |

The observed index reduction is 6.88% (75.778→70.565us). The index/GDB median ratio is 1.78 and index/LLDB is 0.65. The GDB gap remains. Earlier 208→128us and R1045 results belong to their earlier cohorts; they are not combined with this fresh baseline.

Each hardware job measures 60,000 queries with actual Hotspots or detailed Microarchitecture collection. Hardware windows also include warmup and finishing/status work. PID/start/TID/CPU/P-Core evidence is qualified for every engine. These main-TID numbers are scaled sampled estimates, including debugger command handling; they are not exact instruction counts or whole-process totals.

| Engine | Main-TID CPU seconds | Instructions (billions) | Clockticks (billions) | CPI |
| --- | ---: | ---: | ---: | ---: |
| UWVM baseline | 4.107142 | 36.696 | 23.064 | 0.628515 |
| UWVM index | 3.750000 | 30.960 | 20.640 | 0.666667 |
| GDB | 3.204365 | 33.480 | 17.832 | 0.532616 |
| LLDB | 26.825395 | 280.224 | 146.976 | 0.524495 |

The sampled main-TID index reductions are 15.63% instructions, 10.51% clockticks and 8.70% CPU time. CPI increases as the remaining work changes. Missing separate sampled function rows may reflect inlining/statistics and do not mean zero cost.

The host is shared, native GDB/LLDB use the original g++15/libstdc++ target and UWVM uses unchanged SDK24/libc++ Wasm. Display syntax, byte counts and p95 differ. `performance_eligible` remains false for exclusive-host/general parity claims; this is a specific end-to-end console comparison, not formatter-only cost.

#### Provenance and remaining work

Both product builds record exactly equal 1,679 external compile-input pins and 342 compile/link-provider pins, with each build's before/actual/after checks passing. The historical HLL compile/link workspace disappeared after those builds and functional checks. R1111 stopped before measurement because 438 original header paths were absent; R1093 had an input-record schema error and also stopped before measurement. R1123 runs the unchanged existing ELF products, rechecks all present source/provider inputs and all actual loaded runtime/printer files, and records those absent historical headers as a rebuild limitation. A mixed SDK/LLVM provider was not substituted. Future rebuilds require recovery of the matching provider or fresh qualification of a complete current provider.

The compact [machine-readable results](pretty_current_qualification.json) preserve receipt hashes, per-run p95, loaded provider counts, target identities, main-TID metrics and comparison limits. Full receipts/raw VTune data remain under `/tmp/uwvm-pretty-20261018/scope-console-r1123-*`; actual MD/publication proof and exact rollback copies are retained separately. Closed historical sources/object/module copies were archived with path/type/mode/link/content verification; historical replay requires restoring the indicated archives.

Real ROS1/ROS2 middleware/network and ROS2-level combinations, a separately typed sealed copy-only ROS mutation contract, full foreign LLVM/CLI/source VMs, remaining native producer/compiler profiles and Rust collections/async, more Wasm3/hot-update/exit combinations and endpoint/platform security review remain open. The strict ROS mutation census continues to refuse source writes. A historical rare startup SIGSEGV in an older stable coroutine DWARF4 cut remains unrooted; passing later checks does not establish its cause or repair.
