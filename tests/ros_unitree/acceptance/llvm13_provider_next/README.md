# Patched LLVM13 provider and eight-architecture continuation

This is a source-only next-step plan, not a build control or an acceptance
receipt. No candidate module, compiler, test, QEMU, SSH command or provider was
executed to prepare it. Source43 and all previously staged producers remain
unchanged. Root alone performs any actual collection, build or execution.

The required dependency is **23.1.1-uwvm-ros.13**, compiled from the actual
patched source lock and current CMake contract. The successful Source37 four
interpreter builds used a stock Clang23 host compiler with an explicit
`-mllvm -disable-machine-dce` option on each main TU. They did not build or link
this patched Runtime dependency. A host compiler and a Runtime LLVM provider
have separate identities and qualification requirements.

## Existing maintained entries

| Entry | Actual source contract | Remaining work |
| --- | --- | --- |
| `tests/ros_debugger/prepare_llvm.py` | Fresh native ELF62/X86 Release static LLVM closure, current lock/contract, real TableGen, CMake file API, archive-member ELF, full original MD, input/product preservation and pidfd cleanup. Produces no Worker or ROS execution. | Actual complete provider build has no qualified receipt in the evidence reviewed here. Fixed `-j1`,2-GiB aggregate RSS and600-second non-Ninja actor birth-age limit remain unchanged. |
| `tests/ros_debugger/build_runtime.py` | Three fresh LLVM-full Workers (`ros1`, `ros2`, `dual`),9 TUs/3 links,8-GiB aggregate per compile/link; verifies the selected static package against its physical source/build/archive closure. | Needs the actual package manifest/response/generated headers and current explicit configuration. No interpreter objects may supply these Workers. |
| `tests/ros_debugger/integration_runtime.py` and `integration-jobs.json` | Six actual default-denied/retained jobs on native ELF62 LLVM-full Workers, real ROS debugger console, native-step requirement and independently mapped ROS peers. | Actual same-source Worker/package/native SDK receipts and a live owned launch context. Initial adapter is native x86_64, not a foreign Worker adapter. |
| `tests/ros_multiarch_runtime/{profile,prepare,run,matrix}.py` | Eight actual interpreter architectures, both ROS backends,16 explicit cells,13 cases/cell,208 cases total. Each cell compiles3 Worker TUs and actual independent native fixtures. | All16 current explicit profiles/tools/rootfs/loaders/QEMU/Guest/native endpoint inputs and actual completed receipts. This lane explicitly disables JIT. |
| `tools/ci/build_wasm3_cross_llvm_cli.sh` | Existing ROS13 cross-host library/CLI build recipe, native TableGen and per-target LLVM CMake/file-API closure. | It is outside Source43's manifest, is not the current complete ROS acceptance owner, and lacks the current finite full-MD/map/whole-input/pidfd/disk proof. Reuse the reviewed recipe through a separately pinned owner, not its descriptive PASS line. |

The cross shell recipe's job option1..4 is a CPU/request cap, not a memory
reservation. Its cgroup shell check accepts an actual leaf at most64GiB, but
does not itself bind Root's original closed seven-field cgroup incarnation or
retain the current ancestor-OOM baseline. Those stricter existing Root checks
must surround any future use. Its VM selection also does not supply the new
ROS debugger integration's full ROS behavior matrix. Earlier tail/reference
runners that explicitly require ros.9 are historical entries, not ROS13
qualification by changing a label.

## Genuine retained inputs and reuse boundary

The actual Root size inventory is
`/tmp/uwvm-boot3-owned-artifact-size-inventory-actual-root-20261017-r1.json`,
106,337 bytes, SHA256
`a71a58494316b8780e8bdbbcf8105c91c2774113ad7be443b690bfc20265ea99`.
It records these existing Root-owned package directories, not their complete
source integrity or current tool qualification:

* `M/llvm-source13-r1`:212,012,837 regular bytes,12,882 files,1,635 directories,
  no symlink or special node. `M` is
  `/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1`.
* `M/host-compiler-r1`:1,452,419,119 regular bytes. This is the stock host
  compiler package and may bootstrap the patched libraries after current input
  admission. It cannot replace those libraries.
* `M/extra-cross-sdk-inputs-r1` and `r3`:64,972,015 and598,920,157 regular bytes.
  No selected architecture/sysroot/loader/compiler ABI follows from these names.
* The two `M/llvmros13-input-admission-root-supervisor-20261016-r{1,2}` roots
  contain small historical admission metadata. Root's finite current packet
  `5d7088a8...` reopens their original r1 FAIL and r2
  `PASS_CURRENT31_LLVM_INPUT_ADMISSION_ONLY` receipts. The latter published
  an unexecuted preparer argv; it is not a provider build. Exact current raw
  pins and untouched historical control/report references are retained in
  `root-observed-metadata.json`. Source13's current lock full7/hash also agrees;
  Root did not requalify all source bodies in this small passive observation.

No LLVM-named actual build directory or accepted package was listed in that
inventory. This does not prove absence elsewhere; it establishes **no reusable
provider or partial build selected by this plan**. The current preparer
explicitly refuses resume, old objects and an already existing output. Do not
silently add reuse because a static archive, TableGen or version header exists.
A future reuse producer would need the actual complete successful original
source/MD/object/CMake/link/ABI/package evidence plus current requalification;
a failed or partial directory cannot meet the existing fresh-build receipt.

The current core source lock has12,874 entries,1,446,549 bytes, SHA256
`a685b608fd38e86d034ba4ce96d89801bfc69c57b84d514ba1954777b9fa8790`.
The full separate source SDK must match every selected body and have no
unrecorded upstream entry. Share this immutable source SDK between target
builds; do not copy the full LLVM source into each Runtime freeze or output.

The separately opened old r2 configuration is the original2,572-byte closed
17-root preparer control, SHA256 `fe918d59...`. The current full7 of that old
control is recorded separately from the Worker extension. Root's finite
follow-up packet `4c5a3c61...` reopens its89,016-byte admission report
(`2152efab...`) and30,284-byte metadata maker (`9ebd0b67...`). The old report
records12,874 full source bodies totaling210,476,555 bytes and their original
5,116,282-byte body ledger (`a9fb6159...`), all17 source/tool/OS root catalogs,
physical tools/stdlib/loader inputs and successful preservation. It explicitly
records zero subprocesses and `LLVM_package_built=false`. This is useful
genuine input provenance, not a reusable provider. Current re-admission can
share the immutable source/compiler packages without copying their1.66GB again.

The actual Worker37 export is
`/tmp/uwvm-boot3-worker37-registries-source-closure-export-actual-root-20261017-r1.json`,
4,562,755 bytes, SHA256
`a1849c9cc02801628c48c51d52e9557f1452a9ec1efb0895d376ddc368734edd`.
Its exact current Worker report full7 is retained in `plan.json`. Its recovered
configuration is genuine provenance for tool invocations and roots, but has
`profile_compile_flags` in addition to the closed preparer schema and an old
boot1 cgroup. Build a new exact-schema current control from actual selected
physical inputs; never pass that old control unchanged or rewrite its facts.

## Resource admission before any full build

Keep the existing original64GiB hard limit, swap0, cpus0-31, current boot and
leaf device/inode, all ancestor OOM comparisons, and60GiB working ceiling.
The current boot3 identity supplied by Root is documented in `plan.json`; the
future launch must read and verify it, not treat this plan as live evidence.
All retained Root commitments count when admitting another lane. In particular,
observed low RSS while a different lane is hashing does not release its maximum
remaining actor allowance.

The reviewed inventory recorded14,814,789,632 backing bytes free. Keeping the
unchanged8-GiB floor leaves6,224,855,040 bytes (about5.80GiB), before other live
writers. Logical sparse-Btrfs free space and repeated `st_blocks` sums are not
additional host capacity. No full LLVM output-size measurement is available.

A bounded **single-X86 pilot** can be proposed with a4-GiB fresh-output ceiling
and1-GiB additional backing safety allowance. These are explicit proposed
limits, not an estimate proved sufficient. Admission requires actual backing
free at least8+4+1GiB, no competing writer spending that reservation, an owned
output-byte audit, and live checks of every backing ancestor. If it reaches the
output cap, preserve FAIL and all raw evidence; do not lower the8-GiB reserve or
retire inputs to turn the same run into PASS. Root may instead secure more
backing space before starting. The currently running Noetic/Typed lanes make
this pilot **not admitted by this document**.

The maintained serial preparer remains a valid conservative implementation
candidate only under its existing2-GiB aggregate RSS bound. To use the user's
permission for more `-j`, author a **separately versioned and peer-reviewed**
preparer/owner, retaining Source43 and old controls unchanged. A concrete
initial proposal is maximum4 compiler jobs,5GiB reservation per heavy job,
20GiB aggregate compiler tree RSS,24GiB outer allowance and2GiB parent reserve.
Select launch jobs using the existing pure
`tools/ros_distro/build_parallelism.py::plan`: minimum of the explicit4-job cap,
actual admitted CPU affinity/ancestor quota, aggregate20GiB divided by5GiB,
remaining60GiB cgroup headroom after other committed budgets divided by5GiB,
and actual host MemAvailable minus8GiB divided by5GiB. Refuse launch when even
one reservation does not fit. Keep link concurrency1 and linker threads1.
The planner's current API does not subtract other lanes' future commitments;
the owner must subtract them before treating the snapshot as allocatable or
require exclusive build scheduling. No arbitrary CPU-only fanout is allowed.

The proposed larger RSS/deadline/output limits require explicit reviewed
configuration and guards; setting Ninja `-j4` alone is invalid under the old
2-GiB producer contract. Preserve original compile/optimization/ABI flags,
full-MD/verbose recipe authority, real dependency target and no-reuse semantics.
Do not silently increase the existing600-second per-child lifetime to fix a
failure. Record actual child times first; a different finite limit is a separate
reviewed policy if required. Keep cooperative cleanup and read-preservation
runnable after an active disk-floor failure, while all cgroup/RSS/ownership
checks remain live.

## Ordered concrete continuation

1. Root has completed the selected finite passive collection and its exact
   three-reference follow-up. Preserve original
   source-transfer/admission/raw report bytes and collect fresh current full7
   separately. Determine whether any genuine accepted ROS13 package actually
   exists before proposing reuse. Complete current source13/tool/stdlib/root
   admission and the current Source43 MF proof before compiler execution.
2. Publish a new literal closed LLVM configuration:8 explicitly selected tool
   invocation paths/physical pins, native X86 triple, one consistent
   libstdc++/libc++ ABI, complete source/compiler/system roots, closed compiler
   environment and actual current closed7 cgroup. Remove the four-Worker
   extension fields. The existing old config supplies no current authority.
3. After actual memory/disk admission, run one fresh static dependency target.
   Success must retain `prepare-evidence.json`, `full-dependencies.jsonl`,
   original logs/MD/rsp/object/archive/Ninja/CMake/file-API data, complete
   before/after selected-root catalogs and cleanup/OOM evidence. Only terminal
   success may publish `package-manifest.json`, `consumer-link.rsp` and
   `runtime-llvm-selection.json`. No future file identity is predicted here.
4. Admit that actual package into fresh LLVM-full ROS1/ROS2/dual builds with the
   same compiler/stdlib ABI. Then run the six existing debugger integrations,
   LLVM ROS ancestry/tail/refusal fixtures, components and original regressions
   against those actual Workers. A package or Worker build is not execution.
5. Prepare sixteen explicit interpreter matrix profiles,8 architectures x2 ROS
   backends. Start with one complete x86_64 profile and one LoongArch profile to
   expose generic actor/toolchain/ELF issues, retaining failures, then execute
   every remaining cell. This order does not permit a final partial PASS.
   Memory/storage scheduling stays serial by cell unless separately reviewed.
6. Separately complete actual **native LLVM/JIT providers and Runtime mapping
   adapters** for each requested architecture. The cross library recipe is a
   starting point, not the native-debug acceptance adapter. Build only one
   target's native backend per library package unless a deliberate AOT audit
   needs more. Reuse only a qualified same-source native TableGen as a host
   generation tool, with actual executable/full-MD/tool input evidence.
   Cross-generated objects and interpreter ROS execution do not qualify this
   JIT lane. Each complete target package, target Runtime and behavior receipt
   keeps its own genuine architecture, ABI, source and physical identities.

| Runtime architecture | ELF machine/bits/endian | Explicit user-QEMU for foreign Runtime | Native LLVM backend for a separate JIT provider |
| --- | --- | --- | --- |
| x86_64 |62/64/little |native host |X86 |
| AArch64 |183/64/little |qemu-aarch64 |AArch64 |
| Arm hard-float profile selected explicitly |40/32/little |qemu-arm |ARM |
| i386 |3/32/little |qemu-i386 |X86 |
| RISC-V64 |243/64/little |qemu-riscv64 |RISCV |
| LoongArch64 |258/64/little |qemu-loongarch64 |LoongArch |
| PPC64LE |21/64/little |qemu-ppc64le |PowerPC |
| s390x |22/64/big |qemu-s390x |SystemZ |

These QEMU names describe required tool roles, not selected executable paths or
availability. Root must supply each actual ELF62 emulator and target sysroot,
loader, compiler, ABI, stdlib, source lock and complete physical package roots.
The independent native ROS endpoint may be x86_64 or AArch64. For example,
LoongArch Runtime plus AArch64 DDS establishes that recorded pair, not a native
LoongArch ROS SDK. The current208 cases do not cover parameter/TF/graph/SROS2,
dynamic loading, full original regression families or all distributions.

Before retaining eight provider builds plus sixteen complete matrix outputs,
measure the first target's actual logical/physical allocation and shared CoW
extent facts and compute a new aggregate disk reservation. The current6.22GB
headroom cannot be asserted sufficient for all of them. Do not delete or archive
physical products still required by later current full7/whole-root consumers.
Any recoverable retirement needs a separate exact closure/archive/re-admission
contract; restored bytes do not preserve old inode/ctime/execution evidence.

All Guests still start with no optional ROS or WASI rights. Each positive case
selects only its original explicit fine-grained grants; denied cases select an
empty capability table and require independently quiet native effects.
