# SDK verification and remaining acceptance

The user restored Linux SSH and selected an exact 64 GiB, zero-swap cgroup
budget for the next unified acceptance run. Finish the implementation first;
then perform all compilation and actual tests serially in that freshly verified
Linux leaf. Local test/build/cache/log artifacts remain under `/tmp`, with one
build job and bounded memory. Reinspect the actual connection, boot, cgroup and
toolchains before running. The earlier Linux revisions below retain their
original source hashes, budgets and receipts; none qualifies the current source.

The SDK sources, complete ABI declarations, canonical codecs, owned resources,
progressive examples and English usage guides are implemented. That statement
describes source availability. It is not a claim that every language has been
compiled, that ROS delivered a message, or that the aarch64 acceptance task has
finished.

## Repeatable source and pure-model checks

From a complete source checkout, run:

```sh
python3 -B sdk/tools/verify_sources.py \
  --report /absolute/new-output/sdk-source-models.json
```

The report records input SHA256 hashes before and after the checks, exact child
arguments, exit codes and raw output. A missing tool, failing check or changing
source makes the result incomplete. It creates a fresh report and never
overwrites an earlier failure. Its verdict is `SOURCE_AND_MODEL_PASS`, not a
language build or Runtime verdict.

| Check | Actual scope |
| --- | --- |
| ABI extractor | Actual Runtime registry: 38 ROS1 and 43 ROS2 imports. |
| Binding checker | 405 backend-qualified source declarations across C/C++, Rust, TinyGo, AssemblyScript and JS metadata, plus 88 C backend dispatch slots checked against the Runtime inventory. |
| Wire specification | Independent Python byte derivation: 57 positive and 11 negative samples. |
| Python import reader tests | Handcrafted type/import sections; no compiled code or instantiation. |
| Python bounded artifact reader | 24 extracted-reader/synthetic-descriptor cases; no actual FIFO or device is opened. |
| JavaScript tests | Real JS codec and fake in-memory bridge tests; no production ROS host or guest instantiation. |
| Pure-model runner options | 11 closed-option/counter inventory checks; importing the runner never compiles or instantiates Wasm. |
| Pure-model cgroup reader | 21 extracted-function/mock-filesystem cases, including an absent root memory.max; no actual cgroup or Wasm execution. |
| JavaScript bounded artifact reader | 27 extracted-reader/mock-descriptor cases, including nonblocking open, regular-file admission, size/mutation/hash checks and cleanup; no guest instantiation. |
| Rust/TinyGo source checker | Exact raw declarations, dispatch presence, source contracts, documentation and prepared fixture presence; no compiler. |
| Markdown links | Existence of relative guide/example/library links; no attestation of compiler behavior. |
| Source links | Existence of relative JS/AssemblyScript imports and quoted C/C++ forwarding includes; no compiler/module-resolution verdict. |

The language reports under `javascript/tests/` preserve successive revisions
and exact owned source hashes. Later source changes require a later report;
earlier reports retain their original scope. C/C++ model sources, Rust/TinyGo
goldens and AssemblyScript adapter/executor fixtures are prepared for actual
compiler/model runs. A fixture being present is never counted as executing it.

Independent peer review also checks policy generation and the concrete JS
bridge source. Those models verify exact explicitly selected publish/subscribe
grant sets, source forwarding widths and handcrafted binary refusals. They
cannot prove admission by the real CLI or authenticated embedding.

The [C dispatch mutation models](../tests/c_dispatch_peer_models.py) independently
exercise 170 in-memory source variants, including wrong table slots, member
order, result/parameter widths and unsupported entries. Run them separately with
`--root /absolute/repository --report /absolute/new-output/c-dispatch.json`.
They never create mutated source files or execute a real indirect call.

## Compile and inspect each actual guest

Use the [cgroup build matrix](build-matrix.md) to prepare or execute the serial
language matrix. Execution requires the existing verified Linux 64 GiB cgroup,
explicit pinned cgroup identity and supplied compiler paths. The driver does
not download compilers or packages. Missing tools are reported as blocked.

Each emitted Wasm file needs both a full validator and binary import inspection.
The import inspector accepts only exact backend names and i32/i64 signatures;
non-ROS modules require an explicit allowance. Raw-coverage fixtures intentionally
retain the complete backend inventory. A progressive example may import only
the operations it uses. Neither kind of import check grants a resource.

The JavaScript row compiles the concrete C forwarding bridge. It does not
pretend a JavaScript application is a standalone uwvm Wasm guest. The production
embedding still has to supply authenticated imports and caller identity. See
[BRIDGE.md](../javascript/BRIDGE.md).

## Historical Linux acceptance boundary

The frozen r10 SDK completed one real serial compiler matrix on 2026-10-14 in
the original Linux 64 GiB, zero-swap cgroup. Of its 61 rows, 38 passed compiler,
binary import inspection and full Wasm validation; 23 failed. The immutable
[raw compiler report](../../documents/ros/continuation-20261014-r4-evidence/sdk61-and-ros1-search-fail-r10/sdk-compiler-matrix-r1/report.json)
preserves every command and failure. These rows do not execute ROS operations.

| Language/profile | Passed rows | Failed rows |
| --- | --- | --- |
| C | 6 | 4 import-contract failures |
| C++ | 2 | 6 import-contract failures and 4 compile/link failures |
| Rust | 10 | 0 |
| TinyGo | 10 | 0 |
| AssemblyScript | 8 | 9 compile failures |
| Concrete JavaScript C bridge | 2 | 0 |

The source now exports the actual C/C++ tutorial helper entry points so linker
garbage collection retains their used ROS imports. The WASIp1 C++ tutorial
profile explicitly uses `-fno-exceptions`; native exception-enabled model
recipes keep their previous flags. AssemblyScript's reserved method name
`declare` is now `declareValue`, with its raw import unchanged. These fixes
passed all 61 compiler/import/full-validation rows in the frozen r13 matrix,
inside the original container's new 48 GiB, zero-swap incarnation. The earlier
failures remain failures in their original receipt. These compiler rows do not
execute a ROS operation.

The [native and pure model driver](model-matrix.md) ran its 17 guarded rows
against that successful r13 report. Sixteen rows passed, including CMake
relocation, native C/C++ and Rust assertions, and all seven AssemblyScript
models. Go's nine top-level tests and 57 positive-vector subtests ran and
passed, but the controller mistakenly counted nested RUN lines as additional
top-level tests and rejected its inventory. That original failed row is
preserved. The corrected parser separately requires every top-level and
literal subtest RUN/PASS once, rejects skips/failures and checks nesting.
The [pure parser mutation models](../tools/go_inventory_models.py) exercise
missing, duplicated, renamed and incorrectly nested results without running Go.
The frozen r14 compiler matrix then passed all 61 rows and its model matrix
passed all 17 rows, including the separately attested nine Go tests and 57
subtests. The [exact SDK acceptance receipts](../../documents/ros/sdk-current-20261014.md)
bind those results to r14, the original container's new 48 GiB/zero-swap
incarnation, and the full report hashes. The separate supplement passed its
18 rows and 46 commands within its pure host codec/tutorial scope; it proves
neither ROS nor ARM execution. Later SDK source changes, including current
documentation, require a fresh complete compiler/model matrix for those new
bytes. Language-specific Runtime/Gateway behavior remains pending. Separate
custom raw-import Guests have exchanged
fixed records through both native backends; that does not establish delivery
from every language SDK.

During those earlier Linux runs, SSH returned after a further reboot. At the user's request, the same
original container then had an exact **48 GiB** memory limit and zero swap. Its
new boot, StartedAt and cgroup device/inode are measured independently; the r10
results above retain their original 64 GiB scope. The compiler/model guards used for those historical runs
required that 48 GiB leaf and reject old or inherited-only limit evidence.
The r13 and r14 compiler matrices passed in this new incarnation; r14's corrected
Go inventory and all 17 SDK models also passed. Current SDK edits require their
own new actual compiler/model run. No local
guest compilation or execution substitutes for the requested Linux cgroup tests.

For every new run, requalify the current boot and live cgroup leaf
before running anything. Preserve its device/inode, 64 GiB effective memory
limit, swap/CPU settings, process ancestry and OOM-event deltas. Freeze the
current source bytes and compiler inputs in a new output directory. Earlier
Runtime/SDK manifests describe earlier bytes and do not qualify this expanded
SDK. The user subsequently restored Linux SSH and requested 64 GiB. Inspect
the live leaf again; do not infer connectivity or completion from these historical statements.

Then run actual native model and guest checks, both backend profiles, default
denial and explicit minimal grants, queue/error/output-slot behavior, owned
service/Action responses, and the repository's real ROS network tests. Continue
the requested aarch64/QEMU behavior matrix with actual ELF183 target mappings
and process cleanup evidence. A QEMU run on an x86 Linux kernel does not establish
physical ARM performance or memory-order behavior.

## Runtime work that installing the SDK does not complete

The original ROS Runtime task remains partial. Full network/RPC/performance
acceptance, pending Linux evidence retrieval and current-source rebuilds remain
tracked in the repository's ROS progress documents. WASIp1 now defaults to
directional standard input/output/error rights, zero inheriting rights, empty
host environment inheritance, and no non-FD authority. The frozen r9 WASIp1
native suite passed all 17 configuration/authority and explicitly granted
legacy rows; its scope is recorded in
[the ARM continuation](../../documents/ros/continuation-20261014-r4.md).
The current complete Guest CLI, LLVM and checkpoint acceptance remains pending.
These 17 WASIp1 rows are separate from the 17 SDK model rows passed at r14. ROS
requires the effective environment to remain isolated; see
[porting-and-errors.md](porting-and-errors.md).

The SDK adds no implicit grants, native socket/file preopens, fallback ROS
backend, permission level or preset. Unsupported custom types, native source
compatibility and operations absent from the selected Runtime are not simulated
as success.

The current ROS2 resources adapter adds process-local events for its own
committed declare/set/undeclare calls, without creating a native parameter-event
publisher. Only explicitly activated exact-node/name event grants receive them;
their existing type, queue, rate and byte restrictions apply. External native
parameter-service/direct Node changes with publication disabled are outside
this local notification path. These source changes and their two prepared
native fixtures still require real Linux qualification; see the
[local event contract](../../tests/ros_local_parameter_events/README.md).
