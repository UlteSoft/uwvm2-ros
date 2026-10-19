# Fresh LLVM-full debugger Worker preparation

`build_runtime.py` is the native Linux compile-only producer for the separate
ROS debugger integration controller. It creates **three independent fresh
Workers**, `ros1`, `ros2`, and `dual`. Each profile compiles `runtime`, `main`,
and `host_api` as separate C++26 translation units with original `-MD -MF -MT`
records, then links only those new objects. It does not execute these Workers,
run a Guest or ROS, borrow interpreter objects, or change any permission default.
The existing four interpreter profiles and original regression matrix remain
separate acceptance inputs.

Root runs the producer only after freezing the implementation and qualifying
the explicit dependency packages. The implementing agent has not executed the
producer, a compiler, a helper, a model, or an integration test.

```sh
PYTHONDONTWRITEBYTECODE=1 /absolute/python3 -B \
  "$FROZEN/tests/ros_debugger/build_runtime.py" \
  --source "$FROZEN" --manifest "$MANIFEST" \
  --config /absolute/reviewed/runtime-build-config.json \
  --out /absolute/fresh-owned/runtime-llvm-full
```

The output directory must not exist and must be outside the source and every
selected dependency root. There is no reuse or resume option. Failures keep all
objects, dependency files, bounded raw logs, catalogs, and diagnostics. A missing
qualified LLVM package is `BLOCKED` before a compiler starts; it is not a build
success. Invalid CLI/configuration can fail before an output directory exists.

## Configuration and existing dependency admission

The closed configuration is described by
[`build-runtime-config.schema.json`](build-runtime-config.schema.json).
All tool, include, package, sysroot, and source paths are explicit absolute
paths. `compiler` contains one real clang++ executable; `linker` names the real
native LLVM linker. The native host architecture, ELF class, byte order and
machine must agree with `target`. The initial debugger execution adapter admits
native Linux x86_64/ELF62; this builder also detects matching supported native
hosts without claiming that the integration adapter supports them. A cross
compiler or QEMU target needs its own execution/ownership adapter.

`roots.toolchain`, `roots.llvm`, `roots.sdk` and `roots.stdlib` each contain at
least one existing directory. Their complete files and symlink identities are
streamed into before/after JSONL catalogs; large file bodies are never retained
in Python memory. Include roots, compiler/linker binaries, actual system headers,
crypto libraries, startup objects, linker scripts, and transitive library paths
must be covered. If a symlink resolves to another physical library directory,
select that physical directory as well. No dependency may be silently resolved
from an unrecorded directory. `library_inputs` explicitly names existing native
crypto and other extra library files; do not supply fake/stub symbols to make a
link succeed. `compile_flags` and `link_flags` must select the same intended
standard-library ABI: `stdlib` explicitly selects `libc++` or `libstdc++`, and
both flag lists contain its single matching `-stdlib=` option. The LLVM cache
must use the same actual compiler and standard-library selection; explicit
`_GLIBCXX_USE_CXX11_ABI` flags must match its original CXX flags. ABI, unwinder,
compiler-rt and rpaths remain explicit caller-owned choices.

`environment` is the complete compiler-only environment, not an overlay on the
process environment. `PATH` and optional `LD_LIBRARY_PATH` contain only nonempty
absolute directories inside inventoried roots. This does not grant a Guest
environment or pass these variables to a Worker. `LANG`/`LC_ALL` may be explicit.
Do not depend on an ambient shell setup, `CPATH`, `LIBRARY_PATH`, or user cache.
`profile_compile_flags` has all three keys and normally three empty arrays. It
may carry explicit custom-registry `-DUWVM_ROS...` definitions; registry inputs
must belong to the frozen source and appear in the retained full dependencies.
It cannot change the selected ROS backend or LLVM/interpreter selection.

## The current patched LLVM contract is mandatory

Compiler LLVM and Runtime LLVM are different inputs. An installed stock clang
may bootstrap the Runtime, but an ordinary stock `libLLVM.so` is not the patched
Runtime dependency. The producer reads the exact required package version from
the frozen `xmake/llvm/contract.cmake`; it does not hard-code an older ros.9 name.
The current source contract requires static LLVM components, so `llvm.mode`
must be `static-response`, and `shared_library` must be null. A shared/stock
provider is rejected before compilation.
Generic `link_flags` and `library_inputs` cannot add another LLVM search name,
DSO, archive or plugin. Linker-forwarded operands are inspected too, and generic
response/config/full-script indirection is refused. The only LLVM response is
the package's independently checked one. Actual link-trace LLVM inputs must
also belong to that exact static archive set. Standard-library, crypto and
ordinary system-library flags remain explicit and available.

The owned link adds one `--print-archive-stats` output at
`PROFILE/archive-statistics.tsv`; callers and CMake response tokens cannot
override that path. There are still twelve commands, three TUs and one link
per profile. The raw `-t` log is preserved independently. LLD traces extracted
members rather than every offered archive, so an unused archive cannot be
required to appear as a fabricated trace line. The actual Source59 attempt
compiled and linked its first profile successfully but then failed this old
verification assumption for eight of its sixty-four offered LLVM archives.
That original whole-build FAIL remains unchanged.

`llvm_archive_statistics(common, filename, *, archives, member_counts,
trace_paths, expected_archive_operands, roots, target, guard=...)` is the shared
actor-free replay API in the current MF-bound builder. It checks LLD's exact
tab-separated header, bounded ordinary raw sidecar/full7, actual native archive
members and the original selected full7s. Every selected archive must appear;
its statistics sequence and repetitions must match the ordered response.
Repeated rows retain the same member count and only the first occurrence of
each raw spelling carries that spelling's extracted total. Distinct implicit
driver aliases retain separate raw rows and their counts are aggregated by
actual physical archive; this is not a claim of distinct physical members.
A selected archive may be absent from the actual trace only
when its extracted count is zero. Every positive extraction requires real trace
evidence, and every traced archive must occur in the statistics table. All
explicit objects and nonarchive libraries still require trace evidence; extra
or shared LLVM providers remain forbidden.

LLD also records implicit compiler-driver/system archives. These are separately
labelled, confined to the original whole roots, fully reopened and checked for
native members. An exactly eight-byte empty ordinary ar may have zero members
and extraction, with `native_architecture_qualified=false`; it cannot stand in
for a selected LLVM archive. Absolute driver aliases or parent components keep
their original `reported_archive` text plus stable actual naming-component
observations and canonical full7. Selected LLVM spellings stay canonical and
literal. This grants no additional roots or Guest authority.

Each profile's `link.archive_statistics` is the raw sidecar full7 and
`link.llvm_archive_statistics` is the replay receipt. The sidecar is an original
product preserved through terminal cleanup, and all archive input pins remain
in the original input ledger. Downstream consumers must replay both actual
sidecar and trace; newly linked mains may extract different offered members
without changing the original ordered provider/response identity. The original
and new extraction receipts remain separate facts.

The format is supported by the [official LLD archive-statistics test](https://github.com/llvm/llvm-project/blob/main/lld/test/ELF/print-archive-stats.s)
and [linker manual](https://github.com/llvm/llvm-project/blob/main/lld/docs/ld.lld.1).
Those upstream examples are source evidence, not execution of the selected
local linker. The authored owned-filesystem policy factory is
`test_llvm_archive_statistics.py::suite(builder, common, *, scratch, guard)`;
Root must load both modules from the actual complete MF and supply its exclusive
physical scratch directory. It invokes no tools, subprocesses or Runtime.

The separate, existing `llvm.source_root` contains the complete patched LLVM
source SDK, including its `sources.sha256`. Every retained lock entry is checked
against its actual source bytes; no unrecorded file in the four upstream
subtrees is admitted. This source SDK is outside the small core freeze and is
covered by a selected whole LLVM input root. The core freeze needs the current
`third-parties/llvm/sources.sha256`, public `third-parties/llvm/llvm/include`
subtree, `xmake/llvm/contract.cmake`, and normal Runtime inputs. Do not freeze an
unrelated multi-gigabyte LLVM build into the core manifest.

`llvm.build_root` is an already built **Release** dependency build configured
against that exact source SDK with the actual current contract. It retains
`CMakeCache.txt`, `uwvm-llvm-version.json`, and CMake file-API replies. As in
`xmake/utility/bundled_llvm.lua` and `utility.llvm_link_arguments`, the producer
uses the real `uwvm_ros_llvm_contract` target's ordered link fragments, preserving
archive repetitions and attributed link flags. It does not invent a component
graph or execute the metadata-only consumer target. Every selected LLVM archive
must be its actual `STATIC_LIBRARY` target artifact, every member must be a
matching native ELF object, and native AsmParser/Disassembler and DebugInfoDWARF
must be present. Thin archives and LLVM bitcode members are unsupported here.

`llvm.consumer_response_file` contains those exact ordered tokens (absolute
physical archive/library paths); its parsed argv must equal the file-API
derivation. `llvm.generated_include` is exactly `build_root/include`,
`llvm.public_include` is the frozen public LLVM include root, and
`llvm.configuration` explicitly includes the actual generated
`llvm/Config/llvm-config.h`. Generated package version, host triple, native
target, parser and disassembler macros are checked; default and host triples
must agree with the configured native target.

`llvm.package_manifest` is an independently reviewed metadata certificate,
selected with its exact `package_sha256`. It must retain these actual facts:

```json
{
  "version": "ACTUAL_CURRENT_PATCHED_CONTRACT_VERSION",
  "stdlib": "ACTUAL_libc++_OR_libstdc++_SELECTION",
  "source_manifest_sha256": "ACTUAL_CURRENT_SOURCES_SHA256_FILE_SHA",
  "contract_sha256": "ACTUAL_CURRENT_CONTRACT_FILE_SHA",
  "generated_version_header_sha256": "ACTUAL_GENERATED_HEADER_SHA",
  "consumer_link_rsp_sha256": "ACTUAL_ORDERED_RESPONSE_FILE_SHA",
  "archives": [
    {"path": "/absolute/llvm-build/lib/libLLVMCore.a", "size": 123, "sha256": "ACTUAL_ARCHIVE_SHA"}
  ]
}
```

The numbers and strings above are explanatory placeholders, never an accepted
package. `archives` must be the complete unique archive set in the actual
consumer response, not the single illustrative element. The producer checks
these claims against real file-API/source/generated/archive bytes. Certificate
existence alone is not compilation or native debugger acceptance. Retain the
separate actual LLVM dependency build/provenance receipts as additional evidence;
this Worker builder never claims to have built those pre-existing archives.

## Resource, ownership, and receipt boundaries

Every Worker compile/link command is serial, with a 600-second deadline,
8-GiB aggregate owned child RSS cap, 32-MiB merged-log cap, and linker
`--threads=1`. Runtime translation units can exceed 2 GiB even with unchanged
compiler flags. This separate Worker budget leaves the LLVM dependency
preparer's 2-GiB cap unchanged. The complete source,
input-root catalogs, and every actual full-MD/link-trace dependency are checked
again before success. The original supplied Linux cgroup must be exactly
64 GiB (`68719476736`), swap zero, and match explicit leaf path, device, inode,
boot ID and CPUs. The existing probe keeps a 4-GiB cgroup reserve and compares
all ancestor OOM counters. A different same-sized cgroup cannot substitute.

`tools/ros_distro/common.py` supplies the retained-Popen/pidfd ownership runner.
Only proven owned births are signalled; descendant cleanup precedes final
publication. A live unprovable birth is `CLEANUP_INCOMPLETE`, not a numeric/group
kill or a clean verdict. The caller must retain an outer finite coordinator
deadline sufficient for twelve serial commands and streamed inventories; that
outer coordinator budget does not change the per-command 8-GiB/600-second cap.

`build-evidence.json` records actual cgroup, ownership/cleanup, complete source
and package catalog association, raw command records, and failure diagnostics.
Its final pin is included in every profile's `input_pins`; there is no cyclic
hash dependency. On whole-build success, each profile's `report.json` follows
the sibling `runtime-build.schema.json` exactly. Command `stdout` and `stderr`
both bind the **same merged raw log** because the runner redirects stderr into
stdout. They are not two independently captured streams. Every source/object/
dependency/link association is original and explicit, and the final ELF pin has
all seven physical identity fields. A PASS means fresh compile/link and preserved
inputs only. Actual authenticated ROS debugger controls, native peer delivery,
Guest rights and all broader ROS acceptance remain separate required tests.
