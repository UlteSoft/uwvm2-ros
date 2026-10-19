# Build the actual CPython language-frame probe

This is Source for an additive, non-ROS CPython3.14.8 WASI debug build. No
preparation tool, parser, freezer, compiler, linker, new interpreter, Guest or
debugger has run for this delivery. A successful Source review is distinct from
an actual build, a normal runtime result, ROS interoperability and usable DBG.

The two new tools stage exact Source and check necessary final-artifact facts.
They do not rebuild CPython, execute command plans, authorize toolchains, grant
ROS permissions or register logical frames with the debugger. Read the shared
[snapshot contract](../../debug/README.md) and [Python SDK](../README.md) first.

## The real baseline and the missing current admission

The retained FullOz build receipt is
`test/0050.high_level_languages/python/evidence/updates-r1/debug-full-Oz-r1/build-receipt.json`,
200130 bytes, SHA256
`f081533714842d25c78c9c4627a7c976b1c1e9484ea1e403ba0cdee65acf68be`.
Its original base receipt has SHA256
`4eb71c6a10490a270b895cf2d66b542ab70fd2511d7dfae5d24e81065c020816`.
The complete actual link argv is present, including relative archive operands
and repeated libraries in their original order. It is not reconstructed from a
generic library list. The executed producer and dependency capture are retained
in [REBUILD.md](../../../test/0050.high_level_languages/python/debug-producer/REBUILD.md)
and its evidence-sources tree. Those historical boot/cgroup/PID/path receipts
must never be reused as current process ownership or current filesystem proof.

The baseline has71 builtins, wasm32, attached-GIL full CPython ABI, C sources
compiled `-Oz -g`, Python code objects optimize0, and unchanged GC/assertion
configuration. Its config.c SHA is
`1664efaae522119f80d2a5b0a38579482846c976a53b324afabcab7de0d64df3`;
target pyconfig.h SHA is
`8a086ca4cddaf1115c0e573ca13dcff1eb3405cefc80d91cad3325647abab07d`.
It uses matching native3.14.8 SHA `f36bef8c90257efac58a0ebc1e5c0f5231ab0f5c00246fdac9c014de123d837c`
and SDK24 clang SHA `64280d1cf3e55fe73bffe55deed0e61936c84a1b538a5435e50ad8e028eb2e3b`.
An official source archive or an upstream tag does not prove the actual adapted
archive/config/compiler corresponds to it; retain that separate build evidence.

Historical receipts do NOT contain a complete current native startup/compiler
helper/header/sysroot/archive closure. The later dependency capture is explicitly
post-run; it does not retroactively fill that historical gap. In particular,
this Source package does not invent a hash for the actual retained libpython
archive or all its current dependent archives. Preparation remains HOLD until
an independent operator supplies and admits the complete actual current inputs,
including any path relocation and actual original `-l` archive resolution.
There is no included fake ready admission file, empty file inventory or null
toolchain pin masquerading as a working producer.

## Trust boundary and finite admission formats

Use the existing coordinated SSH Linux64GiB/swap0 cgroup only. The keeper must
check current boot, membership, limits, free space and exclusive handoff; admit
this entry, Python startup/stdlib/C extensions, native interpreter, compiler,
linker and libraries BEFORE their first instruction; freeze every input across
later reads; impose finite wall/CPU/aggregate-RSS/PIPE limits and EOF; retain
actual natural exits and physical pidfd retirement. These tools provide none
of that process authority. Old lifecycle booleans, a `passed` field, a JSON
assertion or a file's self SHA cannot authorize or authenticate that boundary.

All JSON rejects duplicate keys and nonfinite numbers. Paths are canonical
absolute regular files; symlink spellings and two different paths to the same
inode are refused. A file is listed once; several role references may refer to
that one file ID. There is no wildcard directory/module approval. The external
full closure review establishes completeness; the Source tools can bind/check
the listed bytes and required roles but cannot prove completeness or authority
of their own input manifest. This profile is explicitly not hermetic.

The Source closure has exactly these top-level keys:

```json
{"format":"python-debug-source-closure-v1","files":[]}
```

The array MUST contain all ten actual files, not the empty illustration: the six
existing `sdk/python/debug` files, shared
`sdk/debug/include/uwvm_language_snapshot.h`, maintained
`test/0050.high_level_languages/inspect_wasm.py`, and these two new tools.
Each row is exactly `source` (canonical absolute file), `target` (normalized
sdk/... or test/... relative path), integer `bytes` and full lowercase `sha256`.
The tool pins the actual eight foundation bodies and captures every staged
Source byte before writing. The two new tool hashes are supplied externally.
This finite profile chooses the actual reviewed `non_ros_probe.py`; a different
application requires a separately reviewed Source successor rather than an
unadmitted arbitrary entry hidden behind the same provenance.

The build admission has exactly:

```text
format: python-debug-cpython3148-build-admission-v1
files: nonempty full current input rows
roles: exact role-to-file-ID object below
directories: cpython_include, wasi_build, sysroot
link_operands: one index/file_id row per original -l or .a operand
closure_evidence: nonempty reviewed evidence file IDs
link_resolution_evidence: nonempty actual resolution evidence file IDs
```

Each file row has exactly `id`, `path`, integer `bytes`, full `sha256`.
There are16..32768 rows, maximum512MiB each and2GiB total. Every input is read
and hashed with stable fd/named-file stats; large binaries/archives are streamed,
not duplicated into the new Source stage. The actual native/compiler/loader/
startup/stdlib/compiler-resource/header/sysroot/include/archive/config closure
must be complete and frozen by the external keeper. Evidence IDs bind actual
review/acquisition/build/read-trace files; they are not an automatic grant.

Required roles are `base_receipt`, `oz_receipt`, `native`, `clang`, `pyconfig`,
`config_c`, `makefile`, `python_h`, `patchlevel`, `libpython`, `cpython_archive`,
`sdk_archive`, `encodings`, `aliases`, `utf8`.
Directory roles are the actual current target Include, wasm32 build directory
and WASI sysroot. Python.h/patchlevel.h, pyconfig.h, Modules/config.c and
libpython3.14.a and Makefile must have their real directory association. Source
archive/config/SDK observed archive identities must match the actual retained
origins; an archive alone does not authenticate extracted toolchain completeness.
The last three
encoding roles supply the exact matching `encodings/__init__.py`, `aliases.py`
and `utf_8.py` bytes; their frozen-input copies are staged separately from the
native interpreter's own independently admitted startup library.

`link_operands` preserves the actual original argv indices and multiplicity.
Every `-l...` and `.a` token, including repeated `-lm`, must map to a real fully
pinned static archive file ID. The libpython operand must map to the role's
actual libpython3.14.a. The independent resolution evidence establishes that
these are the original library choices, not just any archive with the same name.
The tool replaces each original library operand by its admitted canonical
archive path at the SAME position. It also uses the explicit admitted sysroot,
current compiler/header paths and fresh output paths. It never adds a guessed
library, old object, dynamic plugin, response file, shell command or new CPython
configuration step. Unknown file-bearing original operands cause HOLD.

## Stage exact inputs and run only the emitted argv

The following are future Linux keeper commands. `SOURCE` is an independently
admitted composed Source tree, `ADMISSION` and `CLOSURE` are the real reviewed
files above, and `STAGE` is fresh, task-owned and outside immutable inputs.
The keeper supplies each actual full SHA value; the tool does not mint them.

```sh
"$ADMITTED_PYTHON" -I -S "$SOURCE/sdk/python/debug/prepare_debug.py" \
  --operator-sha256 "$PREPARER_SHA" \
  --source-closure "$CLOSURE" --source-closure-sha256 "$CLOSURE_SHA" \
  --admission "$ADMISSION" --admission-sha256 "$ADMISSION_SHA" \
  --entry "$SOURCE/sdk/python/debug/non_ros_probe.py" --entry-sha256 "$PROBE_SHA" \
  --stage "$STAGE"
```

Exit3/HOLD means missing/mismatched inputs, not a skipped PASS. No helper or
compiler is invoked. Every listed input is recaptured before the first output
write; staged Source comes from those captured bytes. The resulting
`build-plan.json` contains exact argv arrays and cwd. A partially written failed
stage is evidence to retain/clean under keeper ownership; never resume/reuse its
objects as a successful build. There is no `--resume-built` in this operator.

The plan has exactly five real execution steps:

1. Run the matching actual native CPython3.14.8 with `-I -S` and staged
   `freeze_debug.py`. It compiles, then marshals the real probe and three exact
   encoding sources without importing/executing the application. Freeze output
   is a new `debug_frozen.h` and a new source/bytecode receipt. This future marshal
   generation is execution and must be recorded; it has not happened here.
2. Compile staged `frame_snapshot.c` separately with `-std=c17 -O0 -g -fno-lto`,
   actual target pyconfig/Include and explicit admitted sysroot, to a NEW object.
3. Compile staged `snapshot_anchor.c` separately with the same flags, preserving
   the real noinline/optnone anchor in a distinct opaque translation unit.
4. Compile staged `non_ros_main.c` separately with the same flags and the fresh
   frozen header. It registers no `_roswasm_native`/owned-Service module.
5. Link those THREE new objects with the exact original FullOz ordered static
   closure and stack-first16MiB/initial-memory40MiB flags. The old embedded C main
   is replaced, the frozen include is fresh, and the output is new
   `python-debug.wasm`. The old Oz interpreter/archives are not rebuilt or edited.

The keeper must execute each argv array directly with its exact cwd, not `eval`,
string concatenation or an ambient shell. Pin/retain all generated files and
each new compile/link receipt. The old libpython remains Oz; these three small
translation units are intentionally O0 and no-LTO. That distinction does not
qualify arbitrary optimized/LTO providers or every implicit compiler safepoint.

## Inspect the final artifact and independently validate the whole module

Prepare an external full-signature startup table:

```text
format: python-debug-wasi-startup-v1
imports: exact nonempty rows {module,name,kind,params,results}
```

Every row must be a numeric function (`kind=0`) from
`wasi_snapshot_preview1`, with exact i32/i64/f32/f64 parameter/result arrays.
The operator independently reviews the intended full startup set; do not simply
accept an unreviewed artifact's imports as policy. Missing, duplicate, wrong-
signature, extra WASI functions, ALL ROS imports, foreign modules, imported
memory/global/table/tags and namespace allowances fail. No directory, socket,
clock, environment or random grant follows from an import table.

```sh
"$ADMITTED_PYTHON" -I -S "$STAGE/sdk/python/debug/check_debug_wasm.py" \
  --checker-sha256 "$CHECKER_SHA" \
  --wasm "$STAGE/python-debug.wasm" --wasm-sha256 "$FINAL_WASM_SHA" \
  --inspector "$STAGE/test/0050.high_level_languages/inspect_wasm.py" \
  --build-plan "$STAGE/build-plan.json" --build-plan-sha256 "$PLAN_SHA" \
  --startup-imports "$STARTUP" --startup-sha256 "$STARTUP_SHA"
```

Without an independently admitted whole-validator receipt this returns HOLD3
even when every structural subcheck succeeds. The strict subparser checks real
numeric import signatures, a single defined nonshared memory32, actual minimum640
pages/no max, memory0 export, real defined `[] -> []` `_start`, function/code count,
section ordering and no implicit start. Table/global/element/code/DATA instruction
validity, ABI, memory safety and the complete Wasm are outside this subcheck.

The maintained inspector's exact captured Source is evaluated in a fresh private
module only by this future checker. Its Path reader is bound to the SAME captured
Wasm bytes; no ambient candidate module/PYC or second unverified file read is
used. Necessary nonempty name/.debug_info/.debug_abbrev/.debug_line sections and
a named actual defined anchor must exist. A section, label or function name is
NOT evidence of a DWARF parameter location, typed local, current stop, source
identity, memory owner, GC safety or controller integration.

To bind real independent validation, additionally pass `--validator-receipt`
and `--validator-receipt-sha256`. Its closed DATA format is
`independent-python-debug-whole-wasm-validation-v1`, with exact final
wasm_sha256/wasm_bytes, build_plan_sha256, checker_sha256, inspector_sha256,
`validator` row (path/bytes/sha256), exact `argv`, integer `exit_code=0`, and
nonempty `evidence` file rows (path/bytes/sha256). Retain actual raw validator
output, command receipt and physical retirement. The argv must name the actual
validator and this exact Wasm. The keeper independently admits the tool, feature
configuration, evidence authenticity and complete execution; a user-created
JSON exit0 cannot do that. Checker exit0 means only structural subcheck and
external-evidence byte binding, never Runtime/DBG or full memory-safety PASS.

## Run the actual non-ROS probe, then qualify DBG separately

The intended ordinary result uses real argv17, the actual exported WASI `_start`
once and no ROS aliases/policies. Use the currently admitted UWVM binary and
its actual current CLI help; in the existing command syntax:

```sh
"$UWVM" --run "$STAGE/python-debug.wasm" 17
```

Require natural exit0, exact application line `PYTHON_DEBUG_COMPLETED 27`, bounded
stderr/PIPE and physical process retirement. No stdin/EOF timeout, signal-kill,
missing import, code95, altered module, nonzero exit or cgroup failure counts as
success. This finite example freezes optimize0 assertions and preserves actual
WASI argc/argv; there is no Python dummy argument or C-style extra null entry
invented by this staging tool. The matching C launcher uses the real bytes argv.

That output proves no debugger behavior. Stop at the compiled
`uwvm_python_debug_anchor` using the actual current supported debugger API.
Resolve its actual pointer parameter's DWARF location in the SAME current
canonical source-memory transaction, retaining physical stop/participant captures,
activation/publication/source/code generation and memory ownership. Copy/parse
the actual59008 bytes with the shared Host seam. Require real recursive logical
frames, full64 unsigned bits, negative zero, NUL/Unicode/binary, nested
containers, truncation, unsupported object and zero property/repr/iterator calls.
Resume through actual gc.collect and refuse prior-stop/sequence/generation/root
copies. Automatic logical-frame publication, exact frozen/nested-code-to-source
mapping and controller selection are separate actual implementations/qualification;
this package neither invents Host imports nor claims those gates are complete.

## ROS embedding and source migration are a different profile

The actual ROS launcher is [embedding/main.c](../embedding/main.c). Its macro is
`UWVM_PYTHON_DEBUG_SNAPSHOT`, default0, accepting exactly0 or1. With1 it includes
the real provider, calls install after interpreter initialization and calls
uninstall before finalization. This is NOT a macro named UWVM_PYTHON_DEBUG.
It freezes `roswasm_app` through the real SDK [tools/freeze.py](../tools/freeze.py),
registers the actual backend C builtin, and requires separately compiled bridge
objects plus real ROS/default-denied/granted/Native/correlation/cleanup tests.
The optional owned-Service bridge separately requires ROSWASM_OWNED_SERVICE=1
and backend2. Do not reuse this non-ROS final gate or its no-ROS startup table as
ROS admission. The existing additive owned-Service preparer is prior Source,
not a complete native/compiler-closure authority for this new tool.

See [the Python migration guide](../README.md#migrate-existing-rclpy-source),
[Service/Future migration](../compat/rclpy/SERVICES.md), the
[compatibility ledger](../../compatibility/rcl/README.md), and
[owned-Service protocol/recipes](../OWNED_SERVICE.md). Port actual rclpy/rcl
source interfaces and complete typed records to the reviewed bounded Guest
protocols. A platform `.so`, native rclpy wheel or native ROS type-support capsule
is not directly loadable as Wasm; compatible C/static libraries must be rebuilt
for WASI with actual ABI/module admission. Native `.so`/`.a` and vendor drivers
that remain outside Wasm belong to a separately authenticated, least-privileged
Gateway/Native peer, with independently audited type support/init/fini and
operation/resource/type/QoS grants. Local aliases, a CRC or a signed module do
not grant those permissions. Full upstream rclpy/rcl feature parity and real
vendor interoperability remain required ongoing work, not a claim of this build.

## Final bytes, signing and retained evidence

Follow the actual shared [WSEA guide](../../../documents/security/wsea.md).
Complete compilation/linking, all DWARF/name/custom sections, any strip or embedded
signing BEFORE detached signing the complete final Wasm. Retain actual wasm,
sig.cose/signer.pem/crl.pem and policy identities. For a single-rule policy use
`--secure module <wasm> <policy>`; shared multi-rule policy requires
`--secure named-module <wasm> <policy> <expected-id>` for EVERY selected module.
Place the current VM options before `--run`; follow actual current help for the
admitted cosign/VM tools, without inventing future algorithms or envelopes.

WSEA requires the signed import-capability mask to fit the matching local
policy rule. Actual WASI/stdio and ROS grants remain separate. A changed debug build must be revalidated/reverified as NEW final
bytes. Current secure active hot replacement is not silently enabled by a source
reload; use the actual verified new-load/instance workflow. Neither signature
nor ordinary output qualifies typed locals, sandbox safety, ROS authority or
moving-GC correctness. Preserve Source/build/runtime/ROS/DBG/VTune/QEMU evidence
as separate levels; benchmark only genuinely qualified comparable configurations.
