# Module and VM observation

The `uwvm_ros_v2` observation imports expose detached metadata. They cannot
change a policy, open a capability, enroll a runtime caller, freeze deployment,
start a provider, renew a lease or invoke a module export. They do not inspect
linear-memory contents, native object memory, process identifiers, credentials
or host paths. A returned permission description is not an execution capability.

The [CLI contract](../command-line/ros-scene.md#module-observation-and-registered-messages)
defines exposure and grants. The [operation reference](ros-operation-reference.md)
defines the canonical request/result records.

The two default queries for a bound module are `abi` and `permission.report`.
The former returns the public major version. The latter reports the caller's
identity and own policy. The host can deny or hide either independently.
Read-only queries of other modules, VM configuration, ROS graphs, sensor data
and driver state require explicit authority for the corresponding target.

## Self scope

`permission.report` is allowed after the caller is explicitly bound to an
enabled ROSv2 environment. It reports only that caller's selected policy,
logical module identity and permission-group identity. It does not enumerate
the group's other members. Configuration can deny a whole report family, an
exact function or the import itself. An aggregate query is refused when it
would bypass an exact-function redaction; an independently allowed exact
query remains usable.

The families are `preloaddl`, `preloadwasm`, `wasip1`, `ros1v1`, `ros2v1` and
`rosv2`. Compiled support and enabled permission are separate facts. A supported
but disabled feature must not be represented as an unavailable build feature.
The observer cannot constrain preloaded libraries or alter another host module.
In particular, inspecting a legacy native library grants it no isolation.

## Other modules and reverse dependencies

`module.describe`, `module.imports`, `module.exports`, `module.group`,
`module.permission.report` and `module.importers` each require explicit exposure
and their exact action in a `module` / `metadata` grant. Trusted configuration
selects one registered target; the guest passes the grant alias. A module
observation grant does not imply VM observation, messaging or export invocation.

`module.importers` reports direct Wasm declarations importing the selected
target. Each row contains the importer module name, declared symbol, zero value
and `declared-import` annotation. It may target a registered builtin module.
It does not recursively expand reexports or inspect native dynamic-linker
dependencies. Asking who imports the caller still needs this independent grant:
the result discloses peer identities. Default self observation cannot make that
query, even when modules share a permission group.

Scanning counts every visited module and import, including nonmatches, against
the 65,536-unit work limit. Pagination does not waive this limit. Filling the
32-row page does not permit returning a partially checked scan; exhaustion
refuses the complete result without modifying the guest result or length slot.

## VM scope

`vm.describe` requires explicit exposure and a separate `vm` / `runtime` /
`metadata` grant. The carrier capability is zero, as for module observation.
`vm` is intentionally not a self-report family.

The current closed projection returns 17 scalar rows: query-only status, host
address width, selected interpreter/compiler, compilation mode, resolved compile
thread ceiling, compiled support flags and counts of registered module kinds.
The counts describe declarations, not live instances or threads. The projection
returns no module-name enumeration or command-line arguments. Compiled support
never overrides per-module permission.

## Publication and retirement

The VM authenticates the immediate runtime caller, checks the selected guest
memory and rejects overlapping request, result and length spans. An initial
authorization check precedes allocation. Final delivery holds the current
observation authorization, including a live grant lease where present. A
completed revocation prevents subsequent delivery; a previously successful
preflight is insufficient. Queries use nonblocking policy acquisition and may
return `again` under contention. They never replenish or consume execution
quotas merely to answer a report.

The declaration projection is synchronous under the VM execution lifetime
lease. It cannot call guest or provider code, retain a guest pointer, or observe
an owner after replacement. Native dl imports remain unsupported because the
VM registry describes Wasm-facing exports rather than the platform linker.

## Temporary storage and performance

Report storage uses recoverable `fast_io` allocation. The page writer is its
only writer. Before deallocation, the owner securely overwrites the fixed
40-byte header and every encoded field covered by the writer cursor, including
fields preceding a later failed row. The uninitialized allocation tail is
neither exposed nor interpreted and contains no report data from this owner.
Clearing this written extent avoids a fixed 64 KiB erase for a short report.
Registered cross-Wasm messages retain their separate full-region erasure and
byte-atomic shared-memory rules.

`observation_allocation.cpp` checks actual allocation/free boundaries, partial
row failure, invalid cursor, scan exhaustion, maximum-sized pages and recoverable
allocation failure. `permission_report.cpp` checks independent grants, redaction,
nonmutation and live revocation. `run_module_security_vm.py` exercises real
interpreter/AOT calls, Wasm32/64, memories zero/one and scans with 65,536 matching
or nonmatching declarations. `run_module_exchange_dap.py` independently checks
debugger access and retained-message nonmutation.

`tools/ros_scene/observation_performance.py` compares the actual Wasm carrier
with a trusted native registry fixture performing authorization, projection,
encoding, copying and erasure. The native fixture excludes runtime Wasm caller
identity, memory checks and dispatch. The ratio measures that boundary overhead;
it is not a comparison against an unrestricted direct C++ call or proof of
native parity for a robot application. Default timing uses three alternating
native/Wasm rounds, 51 batches of 10,000 calls and 1,000 warm-up calls. Batch
means are not individual-request tail latency guarantees.

Both binaries use `-O3 -g`. Timing, hardware Hotspots and detailed
Microarchitecture collection run separately from compilation and simulation.
The runner retains input sources, executable and manifest hashes, commands,
host identity, kernel sampling settings and source/instruction reports. Explicit
counter sampling refuses absent, clamped, lost or throttled period evidence.
A requested 0.01 ms interval alone is not a successful 0.01 ms measurement.

The `module-observation-final-timing/results.json` cohort measured batch-median
Wasm times from 342.64 ns (`permission.report`) to 806.15 ns (`vm.describe`),
with native policy/projection references from 128.73 ns to 531.26 ns. Its VM
SHA256 is `30fce00d58e69773fe4d435d0f08fcd5532735c1b1e643ae1182a99a4db486b9`.
The matching `module-observation-vtune-final-qualified/results.json` contains
eight hardware collections: native/Wasm `module.importers` and `vm.describe`,
each with Hotspots and detailed Microarchitecture analysis. Instruction reports
are in `module-observation-vtune-assembly`. These use a requested 0.1 ms
interval and a 240,000-reference-cycle period; they are not 10 microsecond
sampling qualification. All paths are relative to SDK
`.tmp/ros-scene-security`.

The subsequent FP-control cohort uses VM
`09dad11ebf764980f3527d7f71b9e27cd21dbe1223c87f9e1bec22e8a766798b`.
Its serial A/B/B/A comparison reduced the eight observation workloads by
8.5–21.2%, with unchanged authorization and full-regression results retained in
`observation-control-guard-performance-cohort.json` and
`observation-control-guard-final-runner-results.json`.

Closed VM labels are now constructed as checked canonical ASCII prefixes at
compile time. Runtime text keeps full UTF-8 validation. Pagination, scan limits,
capacity checks, current authority and written-extent erasure are identical.
`observation_literals.cpp` checks byte-exact equivalence, all short capacities,
external canaries, embedded NUL, failed-prefix cleanup and cursor/scan limits;
three negative-compilation cases reject non-ASCII, missing terminators and
oversized labels. Clang ASan/UBSan, GCC and both C++ module configurations pass.

The literal-prefix VM is
`b49d3d7192d2bb456ab6c0d4073ccc120340bc00c649ddfea09e67037b684d9c`.
`observation-literal-performance-cohort.json` retains a second serial A/B/B/A
comparison: `vm.describe` changed from 671.50 ns to 376.95 ns (43.86% less time).
The other seven query medians changed between -0.24% and +2.80% in elapsed time;
this is a targeted VM-description optimization, not an improvement of every
query. The native projection fixture changed from roughly 0.54 to 0.18
microseconds; it excludes VM identity and memory machinery.
`observation-literal-message-comparison` separately found 8-byte complete
message trips at 1,108.95/1,103.27 ns before/after, with 256/4,096-byte changes
below 1.1%. These measurements do not establish a new message optimization.

`observation-literal-vtune/results.json` retains four native/Wasm hardware
Hotspots and detailed Microarchitecture collections for `vm.describe`, plus
source and instruction reports. They request 0.1 ms and record 240,000 reference
cycles; they do not qualify 0.01 ms. The optimized instruction report still
contains bounded stores and publication checks. The dominant remaining paths
include declaration/memory resolution, authority checks, registry traversal,
allocation and erasure. The selected native and Wasm instruction reports contain 238 and 237
address rows respectively, with source line attribution. The final binary
adds 3,904 text bytes relative to the FP-control build; read-only data, writable
data and BSS sizes are unchanged. See `observation-literal-size-evidence.json`.

The literal VM passed the entire retained fault/denial/message/physics cohort,
including 58 guest FP cases with 319 checked query call sites. See
`observation-literal-final-runner-results.json`; its 128 MuJoCo cases remain the
bounded scalar humanoid fixture, not whole-robot or physical-stop qualification.

The explicit import-denial suite `run_scene_all_import_denials.py` generates
cases from the entire registered catalogue. It verifies unchanged request,
result and length canaries on interface denial, including invalid arguments;
tests preloaded callers invoked by a separately authorized main module; and
loads each hidden import independently. This qualifies exposure and refusal,
not successful provider semantics or the complete resource-grant matrix.

`observation_wrapper_fp.cpp` separately exercises every observation wrapper
with 48 x86 FP-control combinations and callbacks that change controls during
identity lookup, range validation, partial reads/writes, allocation and free.
`run_module_security_vm.py --queries-only --fp-controls` checks actual Wasm
arithmetic after queries for both engines and Wasm32/64 with either memory
index. It adds twelve rounding/subnormal/Infinity/NaN checks per call site;
these are not native callback injection tests. The optimized query boundary is
restricted to LLVM builds on targets with the existing register-control guard.
Other configurations retain the complete runtime FP guard.

All qualification must use `tools/ros_scene/in_existing_cgroup.py` and the
existing shared 64 GiB, zero-swap cgroup. These observation checks do not
qualify general vendor launching, durable driver replacement, complete ROS1
bridging, all ecosystem packages or physical functional safety.

## Current bounded native comparison

The full `-O3 -g` VM with SHA-256
`d62905bcd7d580897dbf7b846421920d9737c93fac3faedf9d530d46da3e49ed`
is compared with the matching production-code native carrier in
`component-identity-fenced-observation-native/results.json`. Native/Wasm
batch-mean medians in nanoseconds are: `permission.report` 126.72/262.56,
`module.describe` 189.12/348.28, `module.imports` 130.25/268.58,
`module.exports` 140.97/291.42, `module.group` 172.31/339.62,
`module.permission.report` 250.16/437.57, `module.importers` 167.02/350.17
and `vm.describe` 182.81/373.45. These are the retained fixture's bounded
pages, not arbitrary inventory sizes, request-tail bounds or whole-robot parity.

`component-identity-fenced-observation-hardware-completed/results.json`
contains eight successful native/Wasm hardware Hotspots and microarchitecture
profiles for `permission.report` and `vm.describe`. It reuses one previously
successful collection without overwriting the earlier incomplete report.
The supplemental `component-identity-fenced-observation-path-assembly` receipt
selects the actual query entry points from each emitted function table. This
avoids mistaking a sampled startup configuration function for the query path.
The requested 0.1 ms interval and actual reference periods remain distinct
from the earlier, unsuccessful 0.01 ms configuration.

The subsequent paired-erasure VM has its own 82 security, four DAP and 58
observation/FP cases, retained as `module-exchange-paired-erase-vm-*`.
Performance numbers above belong to their explicitly named binary; a later
safety regression does not transfer timing or hardware results to another hash.
