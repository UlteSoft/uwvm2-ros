# Platform acceptance procedure

This document defines required work and acceptance oracles. It records **no
execution results**. Every new platform and every changed release source must
have its own reviewed evidence ledger. A completed checklist cannot substitute
for raw commands, source and artifact hashes, observed processes and results.

The current uwvm2-ros product contract is Linux, an individually qualified ROS
distribution/ABI tuple, LLVM execution, and mandatory checked native unwind.
There is no public unwind selector. Ordinary upstream Uwvm2 has its own platform
and interpreter acceptance; do not transfer ROS restrictions to that product.

## 1. Define the subject before scheduling

Create one ledger per exact product, OS/kernel, architecture, endian, address
width, float ABI, libc/C++ ABI, compiler, LLVM/runtime provider, ROS distribution,
RMW implementation and SDK revision. CPU execution under QEMU and real native
ROS compatibility are separate subjects. An architecture name alone never
establishes a supported ROS configuration.

Record seven separate dimensions for each row:

| Dimension | Required witness |
| --- | --- |
| Implementation | Reviewed source paths and SHA256; actual feature gates, ownership and failure semantics. |
| Compilation | Configure, compiler and linker argv, exit codes, include and provider provenance, target ABI and final binary SHA256. |
| Wasm artifact | Real language source and producer identity, flags, validation, imports/exports, artifact kind and emitted bytes. |
| VM execution | Exact VM and guest SHA256, actual command, expected result/trap, side effects and cleanup. |
| Native ROS | Exact Gateway and native peer; observable semantic response, callback, graph or resource transition. |
| QEMU | User/full-system mode, emulator/CPU, loader/sysroot or kernel/rootfs, actual execution and facility limitations. |
| Performance | Identified native hardware, matched correct workloads, raw samples, dispersion and separately labelled profiler evidence. |

Use `UNTESTED`, `BLOCKED`, `FAIL`, `PASS`, `NOT_IMPLEMENTED` or
`NOT_APPLICABLE` per dimension. Explain every block and inapplicable cell.
Required missing implementations prevent acceptance. Skips, compilation alone,
successful source copying and exit-zero collectors cannot become execution
passes. Preserve original failures under their original attempt IDs.

## 2. Resource, source and evidence admission

- [ ] Read repository instructions and current state; compare them with Git
  status, actual files, raw receipts and live processes. Identify other owners'
  changes before editing. Never reset or overwrite them.
- [ ] Author source, fixtures and controllers on the Mac. Transfer the entire
  thin packet by SCP before executing it on SSH Linux. Do not build or run
  repository tests on the Mac. Keep SDK test drivers and results in this VM
  repository; preserve the existing separate SDK source/document/component
  layout.
- [ ] Reconcile the Linux boot, container full ID, image, restart count, init
  PID and birth time. Inventory actual tmpfs and persistent inputs. A missing
  old directory is unavailable evidence, not evidence that its job never ran.
- [ ] Hold `/home/macromodel/Documents/uwvm2-ros-test/control/test.lock` for
  every heavy batch. Verify the kernel flock owner and original lock inode.
  An independent guardian must retain the lease until all owned writers and
  descendants retire after controller/SSH loss. Prove this using harmless
  owned interruption cases before trusting a new controller.
- [ ] Read `/proc/PID/cgroup` for actual compiler, VM, native peer, emulator
  and profiler processes. Resolve their actual cgroup mounts and confirm
  `memory.max=68719476736` and `memory.swap.max=0`; also record ancestor limits,
  memory events, peak usage, cpuset and CPU throttling. Requested Docker flags
  or a different similarly sized cgroup do not prove actor containment.
- [ ] Admit complete inputs and output bounds against current space/memory
  headroom. Preserve the established 12 GiB work and 512 MiB host-root reserve
  plus job-specific metadata/export headroom. Fail before exhausting space.
  Do not stop unrelated jobs, lower the reserve or delete unarchived failures.
- [ ] Freeze all source members, patches and patch order, generated headers,
  compilers/tools, archives, native peers, test inputs and final artifacts.
  Store full SHA256 manifests, not only Git IDs or version strings. Verify
  manifests before execution and again after the batch.
- [ ] Retain argv, cwd, relevant non-secret environment, timeouts, exit/signal
  status, raw stdout/stderr, observed PID/birth/cgroup and cleanup proof.
  Protect signing material and credentials; record key identities rather than
  private keys. Seal failure receipts as well as successful ones.

## 3. Normative feature profiles and discovery

Pin the actual specification and official test-suite commits used by a run.
The historical [1.1 SIMD integration draft](https://webassembly.github.io/simd/core/appendix/changes.html)
and the current [Core change history](https://webassembly.github.io/spec/core/appendix/changes.html)
use different release labels. Treat 1.1 as that explicitly identified draft,
not an independent current conformance certificate. The common additions are
sign extension, saturating conversions, multivalue, references/tables, bulk
operations and fixed SIMD. Core 3 adds extended constants, tail calls,
exceptions, multiple memories, 64-bit addressing, typed references, GC,
relaxed SIMD, profiles and text annotations. These labels describe normative
scope; they do not establish this implementation's coverage.

Shared-memory atomics require a separately pinned
[threads profile](https://github.com/WebAssembly/threads/blob/main/proposals/threads/Overview.md).
Do not infer proposal inclusion from a directory named `wasm3`, a compiler
target, or a tool accepting an opcode. WASI and ROS are host interfaces with
their own versioned contracts, separate from Core release numbers.

- [ ] Expand every applicable official test/assertion into a discovered-case
  index. Preserve discovery, selection, execution and exclusion counts. Name
  the exact missing grammar, unsupported feature or resource constraint for
  each exclusion. Review differences against the prior index.
- [ ] Map each feature to actual parser, validator, initializer, interpreter
  and LLVM paths. Record source SHA256 and a maintained driver for each cell.
  Header declarations, generated fixtures and unused translation functions
  establish only implementation evidence until exercised.
- [ ] Execute each feature with its gate enabled and disabled, and with
  incompatible combinations. Require the intended admission diagnostic before
  execution when disabled. Separately verify legal data-dependent traps.
- [ ] Test raw binary decoding independently of the WAT assembler. A producer
  that rejects a case cannot certify the VM's parser or validator for it.
- [ ] Use the locked official suite and an identified reference engine.
  Preserve reference disagreements for review; do not define correctness as
  agreement with a single implementation. Floating-point tests must respect
  the specification's [permitted numeric results](https://webassembly.github.io/spec/core/exec/numerics.html),
  including the applicable NaN and relaxed-operation result sets.

## 4. Core feature acceptance rows

Every row below requires separate decode, validation, initialization where
applicable, and execution evidence. Apply it to the full ordinary-upstream
interpreter and each claimed LLVM mode; use only admitted LLVM configurations
for the ROS product. These are required test designs, not assertions that the
listed cases or all drivers already exist.

| ID | Feature family | Required functional and boundary oracles |
| --- | --- | --- |
| C10-BIN | Core 1 binary structure | Section ordering/uniqueness, function/code counts, UTF-8, bounded LEB decoding, truncated sections, length overflow, custom/unknown-section handling and resource exhaustion with atomic cleanup. |
| C10-INT | Scalar integers | Every width and operation; signed/unsigned edges, wrapping, masked shifts/rotates, bit counts, divide/remainder traps and narrow-load extension. |
| C10-FP | Scalar floating point | Both widths, signed zeros, infinities, NaNs, subnormals, rounding, min/max, comparisons, conversions, reinterpretation and host floating-environment restoration. |
| C10-CTL | Structured control | Nested blocks/loops/if, branches and branch tables, unreachable/polymorphic stacks, local initialization and exact result arities. |
| C10-CALL | Calls and linking | Direct/indirect/imported calls, recursive calls, null/out-of-range tables, type mismatch, cross-module ownership, host reentry and failure cleanup. |
| C10-MEM | Linear memory | All load/store widths, alignment hints, offset/address overflow, page boundaries, zero-size accesses, failed growth and unchanged memory after denied/trapping operations. |
| C10-INIT | Module initialization | Imported/defined globals, tables and memories; start functions; element/data application; conflicting imports; failure rollback without leaked resources. |
| C11-SIGN | Sign extension | Every instruction width, exact sign transition, zero/all-one inputs and validation of invalid operands. |
| C11-SAT | Saturating conversion | Both float and integer widths/signs; finite edges, adjacent representable values, infinities and NaNs; distinguish saturating results from trapping conversions. |
| C11-MULTI | Multivalue | Multiple results, parameterized blocks/loops, branch result order, mixed native ABI registers/stack slots and invalid arity/type combinations. |
| C11-REF | Reference values | Null/function/external references, typed select, reference identity across imports, stale ownership and invalid reference/type combinations. |
| C11-TABLE | Table operations | Multiple imported/defined tables; get/set/size/grow; correct selected table, growth failure, element-type mismatch and exact boundary traps. |
| C11-BULK | Bulk operations | Passive/declarative segments, data count, dropped segments, overlapping copy, zero lengths at boundaries and trap-side-effect rules. |
| C11-SIMD | Fixed SIMD | Every opcode/lane shape, lane immediates, shuffle/swizzle, signed/unsigned saturation, load/store variants, conversions, rounding and bit-level edge values. |
| C20-REG | Core 2 integration | Re-run all applicable common draft additions using the locked Core 2 grammar/rules, official assertions and feature combinations; do not reuse a draft-only pass as final-version evidence. |
| C30-CONST | Extended constants | Each admitted numeric initializer operation; earlier immutable globals, forbidden forward/mutable references, dependency order and failure rollback. |
| C30-TAIL | Tail transfer | Direct/indirect/reference tails; self/cross-module recursion; mixed integer/FP/vector/aggregate ABI; native stack boundedness, callee-pop balance, EH and owner preservation. |
| C30-EH | Exceptions | Tags/imported tag identity, payload order/types, all try-table handlers, catch/rethrow, exception references, uncaught diagnostics, native/host reentry and cleanup. |
| C30-MM | Multiple memories | Nonzero selected memory, mixed imports/definitions, independent growth and cross-memory bulk copies; wrong index/width and no accidental access to memory zero. |
| C30-M64 | Memory64 | i64 addresses and offsets, narrow host size arithmetic, high-address rejection before truncation, mixed-width memories, growth failure, bulk/SIMD access and initializer boundaries. |
| C30-T64 | Table64 | Index/result widths, large-index rejection, mixed table widths, indirect calls, copy/grow/initialization and stale function references. |
| C30-TREF | Typed references | Nullability/subtyping, call_ref, null branches, non-defaultable locals, table initializers and cross-module canonical type identity. |
| C30-GCTYPE | Managed types | Recursive groups, subtype/finality/field mutability rules, equivalent types across modules and precise rejection of invalid recursive/type combinations. |
| C30-GCDATA | Managed values | i31 sign edges, structs/arrays and packed fields; casts/tests/branches, identity, null/bounds traps, bulk array operations and external-reference bridges. |
| C30-GCLIFE | Managed lifetime | Actual collection with globals/tables/locals/results/exception/callback/parked-worker roots; cross-module cycles, overwrite/unload, safepoints, bounded live-set RSS and pause measurements. Zero collections cannot pass this row. |
| C30-RSIMD | Relaxed SIMD | Every admitted relaxed opcode, legal result sets and CPU-policy choices; all lane widths and target lowering paths, without demanding equality where the standard permits alternatives. |
| C30-PROFILE | Profiles | Enumerate supported profile selection and enforcement. If a required deterministic profile is absent, record NOT_IMPLEMENTED rather than inferring it from repeatable samples. |
| C30-TEXT | Text annotations | Pin the actual external text producer; test annotations/name preservation and invalid syntax there. Qualify the VM's binary custom-section handling separately. |
| P-THREAD | Shared memory/atomics | All atomic widths, RMW/cmpxchg/fence, alignment/OOB traps, waits/notifies/timeouts, lost-wakeup and cancellation boundaries, data-race-safe host access and physical worker retirement. |

The tracked [managed-object release blocker](../benchmark/0004.wasm3-core/RELEASE_BLOCKER_GC.md)
must be rechecked against current source and fresh evidence. Historical results
in that document are not current acceptance. Instruction semantics and managed
object reclamation are independent requirements for long-lived workloads.

Initial discovery roots include `0001.parser`, `0005.section_details`,
`0011.initializer`, `0012.validator`, `0013.uwvm_int`, `0014.llvm_jit`,
`0017.runtime` and `0019.gc_statepoint` under this directory. These are search
roots, not a complete executed-test list. Audit newly discovered drivers before
running them, including legacy ROS commands that pass removed options.

## 5. LLVM policy, mandatory unwind, traps and cache

- [ ] Run real supported configuration, compilation and execution. Separately
  reject unsupported OS/ROS/ABI, interpreter selections, disabled TLS or needed
  exceptions, unpaired external unwinder, and all removed unwind controls.
  Test original attached/separate compiler macro flags and forwarding options;
  require rejection before dependency builds rather than a later compiler error.
- [ ] Inspect the real compilation include trace for the pinned vendored
  `unwind.h` and target ABI. Compile assertions using types actually defined by
  that header. Preprocessing assertions without compiling them is insufficient.
  Inspect actual link commands/symbol providers and paired archive SHA256.
- [ ] Run a real generated multi-frame chain once per process before guest
  execution, with exact expected generated frames and ownership. Exercise
  absent CFI, a short chain and incompatible provider through private bounded
  negative fixtures; none may reach the guest-success marker.
- [ ] Measure the entire native self-check, including native-target setup,
  generation, finalization/CFI registration and the real walk. Report raw
  samples and distribution against the 10 ms startup target. It is not a
  forced timeout or permission to skip work. Label QEMU timings non-performance.
- [ ] Test fresh exec, repeated runtime initialization and concurrent first
  use. Audit inherited process state explicitly: supported post-fork behavior
  needs its own check, or a clear refusal requiring exec before runtime use.
  A cached successful probe in the parent is not a child-process check.
- [ ] Verify traps, signal frames and exact Wasm/native ownership for direct,
  indirect, tail, cross-module, host-reentrant and exception chains. Require
  both intentional traps and owned protected-memory faults; inspect crash
  status, leaf/caller/entry diagnostics and cleanup. Preserve QEMU-specific
  failures independently of native and full-system success.
- [ ] Exercise all claimed optimization, CPU, relocation/code-model and LLVM
  modes with actual IR verification and emitted execution. Record rejected
  unverified policies explicitly; do not silently select a different policy.
- [ ] Check cold cache creation, multiple fresh-process verified hits, exact
  unchanged relocation and negative mutations of necessary source, binary,
  dependency, ABI and compilation-policy identities. Unrelated changes must
  not invalidate a cache without a justified dependency.
- [ ] Test signatures, keyring refusal, altered/truncated objects and metadata,
  decompression/resource limits, permission revocation, changed authority and
  stale source/provider identities. Cache loading cannot restore a grant or
  bypass current provenance/owner checks. Require precise verbose refusal
  reasons tied to the actual failed condition.
- [ ] Keep `xmake basic_test` an independent short product check covering
  signed cache behavior, mandatory unwind, necessary debugging and ROS
  admission/capabilities. Passing it does not certify the full feature matrix.

## 6. Every public WASIp1 and ROS interface

Derive the interface inventory from actual public import tuples in the frozen
source. Give every namespace/export/width its own ledger row. Fail discovery
when a declaration format changes or an inventory unexpectedly becomes empty.
Do not hard-code a historical interface count as proof of completeness.

| Surface | Mandatory positive, boundary and denial coverage |
| --- | --- |
| WASIp1 arguments/environment | Counts/encoding, zero-size and aliasing buffers, pointer/length overflow, explicit inheritance policy and denied disclosure. |
| WASIp1 clocks/random/scheduling | Clock domains/resolution, deadlines, zero-length random, invalid IDs, denied rights and actual yield behavior. |
| WASIp1 descriptors/preopens | Open/close/renumber, rights intersection, stale/reused descriptors, fd metadata/flags, preopen confinement and revoke/close races. |
| WASIp1 read/write/seek | Vectored/positioned operations, partial transfers, EOF/errors, offsets, exact byte totals, lifecycle transitions and no ungranted filesystem side effects. |
| WASIp1 paths | Every implemented path operation, directory iteration, symlinks/relative paths, rights, owned test-root isolation and failure rollback. |
| WASIp1 polling/process/sockets | Actual readiness/deadlines, interrupted waits, exit status, each implemented socket API, endpoint restrictions and resource retirement. |
| ROS capability/session | Open/close/revoke, absent/partial/expired grants, UID/backend/type/schema mismatch, stale handles and no native side effect after denial. |
| ROS topics/events | Native publish/subscribe, actual payload semantics, QoS, queue pressure, readiness/poll/watch, reconnect and close/revoke concurrency. |
| ROS services/actions | Real native request/reply, availability, goal/feedback/result/cancel paths, timeout/error precedence, duplicate/stale identifiers and cleanup. |
| ROS TF/graph | Observable native transforms/graph state, time domains, missing frames/endpoints, type identity and restricted names/resources. |
| ROS parameters/clocks/logging | Every declared type/operation/event, native mutation/readback, atomicity, clock selection, bounded logging and denied side effects. |
| ROS2 RMW endpoints | Each separately exported publisher/subscription/service lifecycle, native discovery, serialization and real middleware errors. |

For **each export**, cross normal and denied calls with malformed/stale handles,
memory boundaries, quotas, partial results, host error mapping and cancellation
where applicable. Use exact expected errno/trap/result and owned side-effect
oracles. A CLI parse error is not an interface permission-denial result.
Test direct, indirect, reference, tail, cross-module and nested-host provenance.

Inventory every supported ROS distribution, RMW and type catalog separately.
Disabled ROS tests cannot supply native ROS evidence. Require actual Gateway
and real native peers; precomputed JSON or a synthetic journal is insufficient.
Preview 2/3 names, component-parser stubs and SDK target selectors need separate
implementation audits and cannot establish complete WASI component execution.

## 7. SDK, debugger, audit and live update

- [ ] Discover every declared SDK platform and language producer from its real
  adapter. For each applicable wasm32/wasm64, ROS1/ROS2 and
  binary/static/shared cell, build original language source using its public
  entry point, inspect the actual artifact and execute a real consumer.
  Renaming an executable does not make it a shared library.
- [ ] Verify generated definitions, native dependencies, ABI and source locks;
  use static archive members in an actual link and shared modules in an actual
  load/unload. Test cross-language edges, exceptions, callbacks, threads and
  ownership, then actual native ROS behavior and denied/revoked grants.
- [ ] Identify each source-debug producer's actual DWARF/map support. Test
  source file/line/function, scopes/locals/types, inline frames, optimized-out
  values and breakpoint ownership. C/C++/Rust coverage cannot certify every
  managed language runtime or editor integration.
- [ ] Test Wasm instruction breakpoints/step and native ASM disassembly,
  breakpoints/step, exact code-generation ownership, stale addresses/frames,
  module unload and authorization. Test middle-of-execution attach/detach,
  peer loss, EOF and wrong-process credentials with original process handles.
- [ ] Audit each WASIp1 operation: event sequence/correlation, caller/module,
  operation, descriptor lifetime, requested/actual byte counts, result/error
  and relevant public context. Verify reads/writes and fd transitions against
  actual guest results without recording sensitive payload by default.
- [ ] Audit ROS identity, grant decision, resource/operation, result and failure
  reason against actual authorization/native side effects. Include refusal
  paths, revocation, nested imports and correlation across Gateway boundaries.
- [ ] Force bounded-buffer pressure, sequence exhaustion where implemented,
  reader gaps, sink failure and concurrent writers. Require explicit
  lost/overflow counters and failure states; distinguish ring overwrite from
  durable export. Existing tracing alone does not certify complete audit.
- [ ] Test function replacement and whole-module replacement independently:
  safe-point entry, active/in-flight calls, concurrent workers, exact source
  identity, stale references, permission preservation, atomic publication,
  invalid/incompatible update refusal and committed rollback. Metadata-only
  checkpoint or synthetic-controller tests cannot prove live VM replacement.
- [ ] Measure final native workloads with debugging/audit off, authorized idle,
  and active observation separately. Retain per-process counters and raw
  interleaved samples. Source branches and component microbenchmarks cannot
  establish full-VM zero overhead or a throughput bound.

## 8. Native performance and optional hardware profiling

Correctness and permission checks above are mandatory. A missing PMU facility
may block hardware profiling but cannot convert a failed correctness row into
a pass. Keep CPU-only, physical robot/device and accelerator qualifications
separate, with explicit unavailable cells.

- [ ] Use the same native ROS peer, message/type, QoS, workload, authority and
  correctness oracle for native client, IPC/Gateway and Wasm paths. Freeze
  original Uwvm2 separately for runtime comparisons. Cover every provided SDK
  workload; publish a coverage denominator and every omitted interface.
- [ ] Separate cold compilation, verified cache-hit startup, process/ROS
  discovery, IPC cost, steady execution, shutdown and active-debug overhead.
  An empty guest startup sample cannot establish native ROS latency.
- [ ] Pin actual affinity and record load, CPU frequency/temperature, memory
  events and context switches. Use declared warmups, alternating/randomized
  pairs, enough samples for dispersion, exact operation counts and a stated
  regression criterion. Preserve outliers/failures rather than filtering them
  after seeing the comparison.
- [ ] On available Intel hardware, collect VTune hardware Hotspots and
  Microarchitecture Exploration against the actual scoped processes in their
  admitted cgroup. Review PMU availability, event counts, stack coverage,
  multiplexing, sample loss, attribution and report validity even after exit
  zero. Record unsupported collection as blocked; a software fallback cannot
  stand in for requested hardware counters.
- [ ] Attribute measured excess to compilation, permission/provenance work,
  code generation, allocation, IPC, discovery or scheduling using raw profiles
  and source/disassembly. Optimize a measured cause, then repeat identical
  correctness/denial and native comparison cases. Never weaken signatures,
  bounds, provenance, permissions or unwind checks to improve timing.
- [ ] Run QEMU for ABI/architecture and correctness coverage only. Neither
  emulator wall time nor guest PMU emulation qualifies native performance.

## 9. CI and publication acceptance

- [ ] Inspect actual workflow runs, jobs and failure logs first. Installation
  failures leave later tests unexecuted. Validate the actual compiler package
  or acquisition route and matching standard library/runtime, not just a
  renamed workflow or an assumed package version.
- [ ] Separate ROS release qualification, ordinary upstream retained
  capabilities, candidate platforms, fast PR checks and full trusted matrices.
  Use minimal GitHub permissions. Never run untrusted PR code on a privileged
  self-hosted runner. Static PR analysis supplies no runtime acceptance.
- [ ] Require explicit source/artifact identities and raw evidence on success,
  failure, cancellation and blocked stages. Missing/skipped required commands
  fail acceptance; artifact upload success is not a test pass.
- [ ] Review exact staged diff, ownership, ignore rules and LFS attributes.
  Keep source/docs and required precompiled SDK components in the correct
  repositories. Commit and push only the reviewed, appropriately verified
  scope, with limitations in the commit/report.
- [ ] For a confirmed shared Wasm/LLVM defect, qualify the pure upstream fix
  separately on both `master` and `master-stable`; keep ROS product policy out
  of those patches. Verify pushed remote heads and record exact commits.
- [ ] Publish the coverage ledger and concrete incomplete rows. Do not declare
  the whole SDK, every architecture or all features complete from a finite
  smoke cohort, checklist existence or prepared candidate.
