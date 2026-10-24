# Prepare the actual patched static LLVM dependency

`prepare_llvm.py` builds the current source-owned LLVM dependency in a new owned
directory. It is a source-prepared executable entry, not an observed test result.
No implementing agent has run it, a compiler, Ninja, CMake, a model or SSH.
Root runs it only after the complete implementation freeze and input admission.

```sh
PYTHONDONTWRITEBYTECODE=1 /absolute/python3 -B \
  "$FROZEN/test/0073.ros-tests/support/source/ros_debugger/prepare_llvm.py" \
  --source "$FROZEN" --manifest "$MANIFEST" \
  --config /absolute/reviewed/llvm-prepare-config.json \
  --out /absolute/fresh-owned/llvm-package
```

The native initial adapter is Linux x86_64/ELF62. The compiler may be an installed
stock clang, including LLVM23git, but **the Runtime dependency is freshly built
from the current patched source**. No stock `libLLVM.so`, existing archive, old
object directory or invented ros.9 certificate is admitted as that dependency.
There is no network/download/install, object cache, resume, target `llvm-config`
execution, metadata-consumer execution, Worker, Guest or ROS execution.

## Explicit configuration

The JSON object has exactly the following fields. The paths in this illustrative
shape are placeholders; Root must supply its inventoried physical inputs and
actual original cgroup identity. In particular, a same-sized different cgroup
does not qualify.

```json
{
  "format_version": 1,
  "source_root": "/absolute/existing/patched-llvm-source",
  "tools": {
    "cc": "/absolute/clang", "cxx": "/absolute/clang++",
    "cmake": "/absolute/cmake", "ninja": "/absolute/ninja",
    "ar": "/absolute/llvm-ar", "ranlib": "/absolute/llvm-ranlib",
    "linker": "/absolute/ld.lld", "python": "/absolute/python3"
  },
  "target": {
    "triple": "x86_64-unknown-linux-gnu", "machine": 62,
    "bits": 64, "endian": "little", "host_machine": "x86_64"
  },
  "stdlib": "libc++",
  "cflags": [],
  "cxxflags": ["-stdlib=libc++"],
  "link_flags": ["-stdlib=libc++"],
  "sysroot": null,
  "input_roots": [
    "/absolute/existing/patched-llvm-source",
    "/absolute/compiler-package",
    "/absolute/selected-system-and-standard-library-root"
  ],
  "environment": {
    "PATH": "/absolute/compiler-package/bin:/absolute/other-inventoried-bin",
    "LD_LIBRARY_PATH": "/absolute/compiler-package/lib",
    "LANG": "C.UTF-8", "LC_ALL": "C.UTF-8"
  },
  "cgroup": {
    "memory_max": 68719476736, "memory_swap_max": 0,
    "expected_leaf": "/sys/fs/cgroup/ACTUAL_ORIGINAL_LEAF",
    "expected_device": 29, "expected_inode": 1,
    "expected_boot_id": "00000000-0000-0000-0000-000000000000",
    "expected_cpus": "0-31"
  },
  "build_seconds": 28800
}
```

Every tool must be a genuine executable ELF62 binary inside selected complete
roots. Symlink invocation basenames such as `clang++`, `llvm-ranlib` and `ld.lld`
are preserved and separately inventoried; compiler driver modes are explicit.
The C++ standard-library option occurs exactly once in both `cxxflags` and
`link_flags`. Select ABI macros, compiler-rt, unwinder, rpath and any extra link
options explicitly and consistently. These same choices must be used in the
later Worker configuration. `sysroot`, when selected, is an existing directory
covered by `input_roots`. Include actual system headers, startup objects,
standard-library and dynamic-loader libraries, shell/CMake/Python/Ninja inputs
and compiler resources in those roots. The environment is complete, not merged
with ambient user variables. Its PATH/LD directories cannot be empty, relative,
or outside the selected roots. No environment or ROS permission is given to a
Guest.
Generic link flags may not add LLVM libraries/plugins or hide them in forwarded
response/config/full-linker-script operands. They are checked by the same
frozen Worker builder helper. Current LLVM archives come only from this new
source build's actual static CMake consumer closure.

The current source lock and public headers come from the frozen implementation.
The full separate `source_root` must contain the identical `sources.sha256` and
every actual upstream body it names. All retained source bodies and unrecorded
upstream entries are checked. Root has prepared a source SDK under its task's
`acceptance-storage-r1/llvm-source13-r1`; its actual transport/byte-verification
receipt remains separate evidence. The preparer still reopens the actual inputs.
Do not copy the entire dependency source/build into the core freeze. The freeze
does need current `third-parties/llvm/sources.sha256`, public
`third-parties/llvm/llvm/include`, both debugger producer sources, normal xmake
contract files and the required `tools/ros_distro` extra.

## Real build and finite ownership

The configure uses the current `xmake/llvm/contract.cmake` through
`CMAKE_PROJECT_LLVM_INCLUDE`, Release, X86 native backend, matching explicit
`LLVM_HOST_TRIPLE`/`LLVM_DEFAULT_TARGET_TRIPLE`, real C/C++/ASM compilers,
static libraries, PIC and CMake's file-API query. Its version suffix is derived
from the current frozen contract. Optional projects/runtimes/tests/docs/tools,
LLVM dylib, external zlib/zstd/xml2/curl/libedit probes, RTTI, EH, LTO, PCH and
ccache are disabled as in `xmake/utility/bundled_llvm.lua`. A real TableGen host
tool needed by the library dependency graph is built and used; this is necessary
source generation, not a Runtime or ROS acceptance test.

Ninja runs the genuine `uwvm_ros_llvm_contract_dependencies` target with `-j1`,
`-v`, `-d keepdepfile` and `-d keeprsp`. It builds CMake's real transitive static
closure, rather than a manually maintained library list. The metadata-only
`uwvm_ros_llvm_contract` executable is never built or run. All LLVM object and
archive products are new because the output directory must not exist.

Configuration has a 600-second command deadline. The long serial Ninja
coordinator has the explicit finite `build_seconds` deadline (600 to 86400
seconds). **Every other proven live owned child has an actual birth-age limit of
600 seconds**, including compiler frontends, TableGen, archivers and linkers.
The whole owned command tree remains bounded to 2 GiB RSS and 32 MiB merged raw
log, with linker `--threads=1`; these bounds are not raised for Ninja. The outer
Root coordinator needs its own small metadata allowance and sufficiently long
deadline. The original 64-GiB/swap-zero leaf/device/inode/boot/CPUs and every
ancestor OOM baseline are checked throughout, retaining a 4-GiB cgroup reserve.

The inherited pidfd runner retains each Popen before admission. During the long
build, completed pidfds are closed only after terminal readiness and child
discovery; acquisition/retirement facts stream to `actor-births.jsonl`, avoiding
thousands of stale descriptors. Live unprovable processes cause explicit
cleanup failure. No numeric PID or process-group kill is used. Checked child
cleanup and whole source/SDK/tool before/after preservation precede success.

## Package and evidence products

The preparer takes the actual Release CMake file-API consumer link fragments,
using the same attributed-flag filtering and complete order/repetitions as the
project's Lua helper. It writes `consumer-link.rsp` and a retained
`package-candidate.json`, then calls the frozen Worker builder's exact static
package verifier. This independently checks current source/version/native
macros, actual STATIC_LIBRARY target artifacts, every archive member's native
ELF identity, native AsmParser/Disassembler/DebugInfoDWARF and standard-library
ABI. A successful compiler invocation alone is not a package certificate.

`full-dependencies.jsonl` binds every original verbose Ninja `-MD -MT -MF`
compile recipe to its source, cwd, object, original dependency file and complete
physical header set. Relative Ninja spellings are resolved against that exact
build cwd. The raw verbose log is authoritative; compile_commands.json is
retained separately and is not mistaken for implicit dependency options. Pins
are deduplicated in memory and all unique bodies are freshly rehashed before
finalization. All original objects, `.d`/response files, compile database,
Ninja databases, complete streamed root catalogs and logs are retained.

`prepare-evidence.json` records actual commands, ownership, full dependencies,
source/SDK/tool preservation, cgroup/OOM and cleanup. Only final PASS publishes
`package-manifest.json`, whose `preparation_evidence` is the complete opened
receipt pin, and `runtime-llvm-selection.json`, containing the `llvm` object for
the next producer. Its configuration list includes the preparation receipt;
the later Worker reports therefore bind that same physical proof. There is no
cycle between the receipt and final certificate. Failures leave candidate/raw
products and no final accepted package. Root must preserve these receipts and
then compile all three new Worker profiles using `build_runtime.py`; neither
dependency preparation nor Worker linkage claims debugger behavior or delivery.


## Explicit additional backend archives (configuration version2)

The original version1 schema, native ELF62 target, X86 configure/build recipe
and strict native static-consumer certificate remain available unchanged. A
version2 configuration has exactly those same fields plus a nonempty explicit
`additional_llvm_targets` list, for example:

```json
{"format_version":2,"additional_llvm_targets":["AArch64","ARM","RISCV","LoongArch","PowerPC","SystemZ"]}
```

This fragment illustrates the two differing fields; it is not a complete
configuration and cannot be executed without the original actual tools,
source SDK, roots, target, ABI, limits and cgroup fields. Allowed backend names
are exactly the six shown. Duplicates, `all`, architecture aliases, implicit
native expansion, X86 duplication and missing lists refuse. X86 remains first
and mandatory; its one backend serves both x86_64 and i386 code generation.
No existing version1 JSON may gain additional targets silently.

The complete selected SDK must contain the actual patched source for each
backend under `llvm/lib/Target`. The source lock still hashes every upstream
body. CMake receives the literal finite semicolon-separated target list. Ninja
actually builds `LLVM<Backend>CodeGen`, `LLVM<Backend>AsmParser` and
`LLVM<Backend>Disassembler`, in addition to the unchanged native consumer
dependency target; setting a cache list alone is insufficient. Builds retain
`-j1`, all original RSS/deadline/pidfd limits and full system `-MD` records.

The producer follows the actual Release file-API target dependency graph,
requires the exact named targets and checks every reachable static archive
member against the actual native ELF62 host. Generated target, parser and
disassembler definition files must contain exactly the explicit backend set.
It checks the real cache selection, records newly built host tools separately,
and preserves all discovered artifacts and file-API JSON pins. Live guard
checks also surround this metadata/archive admission. Any missing target,
foreign archive, unresolved artifact or incomplete graph fails closed.

`additional-backends.json` records that actual evidence and is bound by the
package's `additional_backends` fullpin. The strict native response-file
archive list remains exactly CMake's native consumer closure; unrelated
code-generator archives are not smuggled into it. Whole source/SDK/provider
preservation and cleanup still precede the final accepted certificate.

These archives contain **host x86_64 code implementing extra code generators**.
They are useful as an explicit host provider with the eight requested ISA
backends, but are not LoongArch/ARM/etc native-host LLVM libraries. A Runtime
that itself executes under a foreign QEMU still requires its independently
cross-built LLVM package, target C++/unwinder/crypto/sysroot/loader, matching
`LLVM_HOST_TRIPLE`/native macros and actual target ELF archives. The existing
foreign Runtime builder refuses this x86_64 package as a foreign-host SDK.
No target execution, QEMU/JIT pass or native stepping is established here.

The current ROS dependency is LLVM **23.1.1-uwvm-ros.13**: `.13` is the
downstream repair revision, not upstream LLVM13. Stock upstream LLVM13's
[registered target list](https://github.com/llvm/llvm-project/blob/llvmorg-13.0.0/llvm/CMakeLists.txt)
does not include LoongArch. Neither a renamed LLVM13 directory nor an extra
JSON backend label can supply the required patched LoongArch TailCC capability;
the actual current generated header and archives remain mandatory.

`test_prepare_llvm_targets.py` provides eight unexecuted DATA selection cases.
They do not build any LLVM archive. Actual extra backend compilation, all
eight Runtime/loader/QEMU/controller/map proofs and original regression suites
remain separate Root-owned execution requirements.
