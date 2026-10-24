# Six fresh ROS1 legacy command builds

`prepare_legacy_guests.py` is the executable producer for
`sdk/origin/robots/unitree/recipes/legacy-command-recipes.json`. It builds C, C++, Rust,
TinyGo, AssemblyScript and QuickJS commands. Every application and its command
entry execute within one eventual main Wasm module. This producer runs only
compilers, metadata preparation and full Wasm validation; it never initializes
a Wasm instance, starts ROS, contacts a robot, or grants an operation.

This implementation is source-authored. A source manifest, reviewed recipe or
successful native metadata exporter is not a result of these six builds.
Root alone performs actual builds and later twelve-case denied/positive ROS1
transport acceptance inside the original 64 GiB cgroup with zero swap.

## Closed selection and invocation

Freeze the complete current project, including this producer,
`command_build_support.py`, all SDK/example/entry sources, the ABI manifest,
and normal Unitree/common/restart readers. Use the standard Unitree Frame
configuration with actual current cgroup observations, complete read-only tool,
SDK, generated/native and original official-object input roots, and explicit
thin input labels (`path`, `bytes`, `sha256`). The whole source manifest and
configuration each require an actual complete SHA256:

```text
<actual-python> -B <source>/test/0073.ros-tests/support/source/ros_unitree/prepare_legacy_guests.py
  --source <source> --manifest <actual-complete-manifest>
  --manifest-sha256 <actual-manifest-SHA256>
  --config <actual-Unitree-config> --config-sha256 <actual-config-SHA256>
  --selection-input <label-of-closed-selection> --out <fresh-owned-output>
```

The selection has exactly these fields:

| Field | Meaning |
| --- | --- |
| `format_version` | Integer `1`. |
| `kind` | `uwvm-unitree-legacy-command-build-selection-v1`. |
| `native_products` | Frame input label for the actual successful new Noetic native products report. |
| `typed_build` | Frame input label for the actual completed Typed14 report; only its independently qualified official C++ library object is consumed. |
| `tools` | Closed mapping of the fifteen names below to actual Frame input labels. |
| `paths` | Closed mapping of the seven names below to actual absolute SDK/package paths. |
| `restart_mappings` | Explicit finite distinct-device mappings for consumed historical official-object provenance; no mapping changes a raw report or bypasses changed size/hash/inode/timestamps. |
| `build_seconds` | Integer `600..14400`, bounding active build and preservation. Owned cleanup has its own at-most600-second grace. |

`acceptance/legacy_guest_builds/selection.template.json` is deliberately
incomplete: null report labels/tool labels/paths must be replaced by Root's
actual current observations. It is not runnable configuration and contains no
forecast inode, receipt pin or future successful build identity.

The exact tool/archive names are `wasi_clang`, `wasi_clangxx`, `wasm_tools`,
`python`, `rustc`, `tinygo`, `go`, `node`, `asc_js`, `wasi_libc`, `wasi_libm`,
`wasi_full_printscan`, `wasi_builtins`, `wasi_libcxx`, `wasi_libcxxabi`.
`asc_js` selects the actual compiler JS entry; Node executes it. The other
compiler executable paths and the Wasm linker invocation alias must actually
be executable and covered by complete package catalogs.

The exact path names are `wasi_sysroot`, `wasi_resource_dir`, `wasm_ld_alias`,
`rust_sysroot`, `tinygo_root`, `go_root`, `quickjs_root`. Preserve the real
`wasm-ld` multicall basename while separately pinning its physical executable.
The C archives must be the actual selected sysroot's direct nonthread
`lib/wasm32-wasip1` archives. The explicitly selected `wasi_libcxx` and
`wasi_libcxxabi` files form a separate pair: both must be physical ordinary
`libc++.a`/`libc++abi.a` in exactly that directory, its `noeh` subdirectory or its
`eh` subdirectory. Mixing profiles, SDKs, thread/WASIp2 paths or LTO directories
is refused. No missing path selects another profile and the SDK is never
modified. The selected whole SDK catalogs and actual full-seven file inputs
remain mandatory; the real C++ compile/link target must be `wasm32-wasip1` and
its actual linker trace must contain both selected archive pins before the
ordinary full binary validation can qualify the product.

For the genuinely observed SDK33 clang-22, Root's actual default and explicit
`-fno-exceptions` driver-only `-###` plans both placed the `noeh` library path
before the direct path and used `-lc++`/`-lc++abi`. The default cc1 plan still
listed `-fcxx-exceptions`/`-fexceptions` without `-mexception-handling`; therefore
`noeh` here is the SDK library profile, not a claim that language exception
flags were changed. The original six-language recipe and official abort TU
remain unchanged. The external metadata constructor successor accepts Root's
actual current noeh pair full-seven pins explicitly and records only selection,
not compiler/link/Runtime success. A future explicit eh selection must actually
appear in the real link trace; it is never substituted for noeh by this producer.

Rust must be a complete real SDK with
`bin/rustc` and `lib/rustlib/wasm32-unknown-unknown/lib`; neither a rustup network
shim nor a WASIp1 target substitutes for that profile. The existing reviewed
same-PID Rust linker wrapper explicitly uses the selected `/usr/bin/python3.14`.

The supplied cgroup must be the original leaf, effective CPUs `0-31`, hard
memory limit `68719476736` and swap `0`, with the actual current boot/device/inode.
The producer performs a stdlib current-identity check before any manifest-bound
project helper executes. Its Frame then validates all visible ancestors and
complete source/package inventories. It never creates, raises, or switches a
cgroup. No historical boot ID is hardcoded as a new observation.

The exact complete Noetic producer report parent must itself be a selected
whole root. Its copied Focal SDK contains absolute CHROOT symlink spellings.
A per-Frame catalog adapter uses the reviewed literal descriptor walker for
that one report root after ordinary cgroup/upstream/input admission. It does
not resolve those spellings as host SDK authority, expand other roots, alter
physical file pins, or change executable/map-files admission. All other root
catalogs retain their original implementation.

## Required native family and official C++ object

The native prerequisite must have schema
`uwvm-unitree-noetic-native-products-v1` and status
`PREPARED_NOETIC_LEGACY_NATIVE_PRODUCTS_NOT_TCPROS_EXECUTED`, from this same
complete current source. Its three primary native TUs, successful stopped
identity exporter, original SDK/upstream preservation, complete execution view
and full input/source/root preservation remain separate native evidence.

The Guest producer independently opens the original eleven-message
`unitree_legged_msgs` profile at official commit
`b989870124913091fbe75e0dcfb047eb4ca00a09`, the native metadata, bound catalog,
binding receipt, all original message bodies, `generation.json`, and every
generated output. All eleven ROS1 native MD5 identities and canonical codec
identities must agree across records, type metadata and the actual export.
The exact current emitters must agree with the generation receipt. The eleven
required output files are:

```text
bounded_records.h    bounded_records.hpp  bounded_records.rs
bounded_records.go   bounded_records.ts   bounded_records.mjs
bounded_interfaces.h bounded_interfaces.hpp
manifest.hpp         native_ros1.hpp      native_ros2.hpp
```

The complete family is retained even when an example uses only LowState and
LowCmd. Source33 guest-only N/A identities, GO/HG/API families and old ROS2
Gateway results cannot satisfy this prerequisite.

C++ consumes only the actual completed Typed14 qualification's independent
official termination object. The original successful overlay report, official
four-source/lock provenance, real full-MD dependencies and object are reopened.
The overlay policy must be
`OFFICIAL_LIBCXXABI_BAREMETAL_ABORT_TU_AND_HEADER_VERBOSE_TRAP_V1` at the exact
official LLVM commit `4434dabb69916856b824f68a64b029c67175e532`. Its compiler
physical path and WASI sysroot must equal this build's actual selection.
Only the isolated official abort TU used `NDEBUG`/`LIBCXXABI_BAREMETAL`;
Guest C++ keeps `-UNDEBUG` and the header verbose-abort trap. Actual link trace
and `--why-extract` must include the official object and exclude the default
verbose/abort diagnostic archive members. No stdio or successful abort stub is
created, no whole Guest assertions are disabled, and no GO-specific Direct
prerequisite is invoked. Original reports keep their original source and status.

## Real compilation and source staging

Every language stage contains the complete actual SDK tree it uses, the real
legacy command support tree, exact full application/value files, and all eleven
generated files under `native-family/`. Relevant complete modules are also
copied into their real relative include/import locations. Every source/copy
association records both actual full-seven pins and identical size/SHA; copies
have independent inodes. No application, SDK, command body or main is rewritten.

Receipts expose `source_bindings` with original-MF-relative names and original
and compiled-copy full pins. `generated_bindings` separately binds complete
family output names/generation to their copies. Actual Clang/Rust dependencies
are mapped back through those exact source-copy associations; Go/AS/JS retain
their full explicit staged module scope with that conservative basis labelled.
The actual example is mandatory in every original project input closure. This
producer compiles all six fresh products. A future harness-only consumer must
still verify the whole original source/manifest, current counterpart bytes and
actual native family before considering reuse; it must keep original labels.

For TinyGo, only actual Go application/value files and `go.mod` occupy the main
package, alongside copied real `command.go`/`protocol.go`. The full generated
Go module lives in `customtypes/`. Unrelated C/C++ example siblings or a
different-package generated `.go` file are never placed in the main directory.
All original source remains in the complete source manifest; this separation
prevents a genuine mixed-package/cgo staging failure without editing Go code.

The actual work is:

| Language | Executed work and retained evidence |
| --- | --- |
| C | Real C11 command entry and actual example compiled separately with full `-MD -MF -MT`, followed by a genuine traced command link. |
| C++ | Real C11 entry plus actual C++20 example, full MD, matched official object, traced link and extraction table. |
| Rust | Real C11 entry, real ROS1+alloc SDK rlib and actual command cdylib. Actual Rust dep-info is retained as Rust evidence, not called Clang MD. An owned Python wrapper captures every genuine linker file argument before Rust may remove temporary objects, then execs the opened real linker in the same PID. Every disappeared object must have that real pre-exec pin and appear in the actual trace; missing files are never silently skipped. |
| TinyGo | Actual offline `build -buildmode=default -scheduler=none -tags=ros1 -p 1`. A new exact copy of the pinned790-byte `wasm-unknown.json` appends only the64MiB max-memory ldflag; original buildmode remains unchanged in JSON and the real CLI selects command startup. |
| AssemblyScript | Actual Node/ASC command with its real `legacy_command_entry` initializer and complete staged SDK/module graph. |
| JavaScript | Exact copied `prepare.py` from the stage, backend1 command manifest with15 real modules and no scalar exports. Its HERE/source-root invariant is satisfied by explicitly expanding SOURCE to that exact stage for preparation. All thirteen prepared real QuickJS/engine/stdio/native-ROS/command/generated compile jobs, traced link and full validator run with their prepared argv unchanged. |

TinyGo/Go dependency lookup is offline: `GOPROXY=off`, `GOSUMDB=off`,
`GOTOOLCHAIN=local`, `GOENV=off`, `GOWORK=off`, `GOFLAGS=-mod=readonly` and
explicit actual GOROOT/TINYGOROOT/package-covered PATH. Caches and temporary
directories are fresh owned output. The producer does not install packages,
modify SDKs, run a shell, or treat an opaque package label as compatibility.

## Resource, binary and result contracts

Compilation is serial. Root selects a finite Frame command RSS cap of at most
2 GiB; each command keeps its actual pidfd owner, deadline, log/output caps and
no-overlapping-descendants terminal barrier. Admission before each tool reserves
that command cap plus2 GiB parent headroom below60 GiB current cgroup use and
retains8 GiB host available memory independently. The immutable kernel boundary
is64 GiB/swap0. The coordinator stays below1 GiB. All backing filesystem
ancestors retain the existing8 GiB disk floor; storage exhaustion does not block
attempting owned child cleanup. Success still requires a new final guard,
complete source/root/pin preservation and clean terminal pidfds.
All six products require exactly32 actual compiler/preparer/link/validator jobs,
each with its own successful return code; the official object is a retained
qualified library input and does not add a fabricated new compile command.

Each real artifact is separately validated by the actual selected wasm-tools,
then reparsed for actual ABI/memory/entry facts. Only unique function imports
from `uwvm_ros1_v1` at exact original ABI signatures and WASIp1 `fd_read`,
`fd_write`, `proc_exit` are admitted. Capability-open and all three basic I/O
imports must actually be present. Foreign imports, optional WASI, imported or
unbounded/shared/memory64 memory, implicit Wasm start, and imported/non-void
entry functions are refused. The one owned memory0 must be exported with a
maximum of1024 pages. AssemblyScript's entry is `legacy_command_entry`; all
others use local `_start`. No preloaded application or changed cross-module
guard supplies a positive operation.

Only all six complete products can publish
`PREPARED_SIX_LEGACY_COMMAND_PRODUCTS_BINARY_VALIDATED_NOT_EXECUTED`, kind
`uwvm-unitree-legacy-same-main-command-products-v1`. Each row and its
`uwvm-unitree-legacy-same-main-command-guest-build-receipt-v1` retain actual
artifact/generation/source/command/MD/link/validator pins, same-main flags,
empty granted rights and `observed_execution=NOT_EXECUTED`. The top report has
`actual_Runtime_cases_executed=0`, normal Frame cleanup/source/input/root fields
and an explicit product ledger whose pins are included in the terminal full
pin-preservation check. A partial build remains failed; no historical batch or
product is promoted. `run_legacy_transport.py` must independently consume these
receipts and perform its twelve fresh denied/positive cases later.

## Separate policy regressions

`test_legacy_guest_build_policy.py` provides sixteen small source-policy cases.
Root invokes it with the same `--source`, `--manifest`, `--manifest-sha256`,
`--config`, `--config-sha256`, and fresh `--out` shape, without
`--selection-input`. Its own source and each candidate helper must belong to
that complete current manifest, and actual original cgroup admission precedes
candidate imports. It has a600-second/512MiB metadata budget and the unchanged
8GiB disk reserve. No compiler, validator, Guest or ROS process starts.

The real bounded parser reads small synthetic core-Wasm sections for positive
local entry and negative foreign ROS backend, optional WASI, wrong ROS ABI,
imported/shared/unbounded memory, implicit start and imported entry cases.
Actual filesystem fixtures exercise unchanged complete module copies, the
TinyGo main-package/cgo sibling trap, AssemblyScript relative imports, genuine
multi-rule Rust dep-info, and refusal of missing artifact rules, missing files
or files outside selected roots. A real created-then-retired object fixture
also exercises the exact production temporary-name predicate: ordinary
cell-local `*.rcgu.o` and owned TMPDIR objects remain eligible only with the
separate genuine pre-exec/link proof; wrong names, suffixes, parents and
backslash spellings are refused. Dummy SDK fixture bodies are labelled policy
data and never called native or compiler evidence. Actual language compilation
and full Wasm validation remain separate required jobs.


## Independent official C++ object input (selection version 2)

Selection version 1 and its `typed_build` predicate remain unchanged. It still
requires the genuine completed fourteen-product report and all original flags.
An unrelated RUNNING/FAIL Typed14 main report is never accepted on that path.

Version 2 has the same closed eight-member selection except that
`format_version` is integer `2`, `kind` is
`uwvm-unitree-legacy-command-build-selection-v2`, and `official_cpp_report`
replaces `typed_build`. That value is a Frame input label for the actual
standalone `wasi-cpp-official-termination-overlay` report whose own status is
`PASS_OFFICIAL_CPP_TERMINATION_OBJECT_PREPARATION_ONLY`. It is not a label for a
Typed14 main report, an extracted fabricated PASS, or a forecast report.
`acceptance/legacy_guest_builds/selection-v2.template.json` contains nulls only
as documentation and must be filled from Root's actual current input pins.

The new manifest-bound `independent_cpp_object.py` consumes only this qualified
library input. It starts no actor, rebuilds no object and changes no SDK or raw
report. It uses the existing Evidence accessor for current full7 observations
and explicitly selected device-only restart mappings. Original source/manifest,
configuration/cgroup, producer and overlay facts remain original compilation
provenance. Current cgroup admission uses the actual current Frame, never the
old cgroup observations or an assertion of cross-reboot actor continuity.

Root must retain and select the complete original compilation source and its
manifest, every original object-producer prerequisite root, original report and
catalogs, compiler/sysroot and complete official LLVM source acquisition data.
Every one of the original root catalogs is compared row-by-row against the
current Frame's whole-root catalog, with complete names/content/inodes/times,
literal/resolved links and target-file pins unchanged except explicitly mapped
filesystem devices. The catalog's original file/link scope is retained; empty
directory or hardlink-count metadata absent from that schema is not invented.
A missing root, changed byte/inode/time/link, absent input or ambiguous current
catalog fails closed. Retained catalogs are streamed through held no-follow
file descriptors with full-byte digests and named-entry stability checks.

The producer remains exactly 27,601 bytes with SHA256
`31340e874df8848a9e7710d824483ae4588fbb9c320fe79ca12f056a33407981`.
The original reviewed full-MD helper is separately pinned. The official source
validator is copied literally from that exact producer; its commit/tree/blob,
license/source-layout and diagnostic-guard checks remain intact. Only its
opened-file primitive uses the existing current Evidence accessor. The exact
LLVM commit is `4434dabb69916856b824f68a64b029c67175e532`; no hand-written abort
implementation, assertion removal elsewhere, fake header, new NDEBUG scope or
changed object substitutes for that source. Selected current WASI compiler and
sysroot must match the original proof. Actual full-MD source/object/depfile/target
associations and every recorded dependency are independently reopened again.

After actual admission the helper writes
`independent-official-cpp-current-qualification.json` inside the fresh Legacy
output. Its status is
`PASS_INDEPENDENT_OFFICIAL_CPP_LIBRARY_INPUT_QUALIFICATION_ONLY`, and it binds
actual current object/producer/input/catalog/MF pins separately from unchanged
original report/source/cgroup facts. It does not claim any Legacy Guest has
compiled, any Runtime executed, any right was granted, or Typed14 terminated.
Subsequent compiler receipts bind this sidecar; ordinary Frame finalization
still verifies current input/source/root preservation for the actual six builds.
This removes an unrelated main-batch dependency without weakening object proof.

`test_independent_cpp_object_policy.py::cases(helper)` is a small callable DATA
policy factory for Root's source-pinned test owner. Its eleven authored cases
exercise nonterminal reports, preservation/cleanup refusals, device mapping,
changed file/link identity, incomplete catalogs and full-MD/compiler/source
associations. Synthetic records in those tests are predicate fixtures, never
authentic report/object qualifications. No candidate import, policy case,
compiler, Runtime, ROS or SSH execution occurred while authoring this change.
