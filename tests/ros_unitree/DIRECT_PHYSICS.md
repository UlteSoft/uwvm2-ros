# Real C, C++ and embedded-JavaScript Go2 control

The direct-control lane compiles and executes the actual SDK applications in
one Wasm principal per command. The C main uses the C SDK and complete generated
GO codecs. The C++ main uses the C++ SDK. The JavaScript main uses the real
QuickJS-in-Wasm SDK, with its complete generated GO JavaScript module embedded
as source. No host JavaScript engine or native ROS relay substitutes for a
Guest publication or subscription.

This source implementation is not an execution result. Compilation, full Wasm
validation, source admission, real Runtime initialization, native DDS delivery,
controlled MuJoCo stepping and performance are separate evidence scopes.

## One authoritative controller

All three applications and the ordinary compiled native C++ controller use
`sdk/robots/unitree/include/roswasm_unitree/simulation_control.h`. This is the
single Go2 stand/lower kernel. Its native and Guest source bytes must agree.
The C example's small C++ formatting TU uses fast_io without owning a ROS handle.
The optional JavaScript C module computes the same kernel and packs the same
official padded LowCmd CRC inside that same Wasm. It returns copied484-byte
records to JavaScript: one CRC word and twenty six-word motor records. It calls
no ROS function and accepts no native handle, linear-memory pointer or authority.

`UWVM_JS_UNITREE_CONTROL` defaults to0. Only the explicit standalone Go2 command
recipe registers `roswasm:unitree_control`. The JavaScript source producer accepts
`--unitree-direct-enabled 0` or `--unitree-direct-enabled 1` only for that exact ROS2 command main, empty
cross-module imports and empty reactor exports. Normal JavaScript examples and
pure libraries retain their existing source map and import contracts.

The behavioral compile selection `UWVM_UNITREE_DIRECT_ENABLED=1` does not grant
permission. Every denied artifact uses0 and must observe denial13 for BOTH real
alias opens. Positive execution separately grants exactly LowCmd publication and
LowState subscription, with actual native type identities, finite capacities,
finite rates and one endpoint per role. All optional WASI rights remain denied.

## Compile six actual standalone products

`prepare_direct_guests.py` consumes a SHA-bound ordinary Unitree config and one
selected input document of kind
`unitree-direct-go2-standalone-build-selection-v1`. The selection has these exact
members:

| Member | Meaning |
| --- | --- |
| `format_version` | Integer1. |
| `tools` | Input labels for `cc`, `cxx`, `python`, `validator`, `libc`, `libm`, `printscan`, `builtins`, `libcxx`, `libcxxabi`. Each selected file is physically opened and covered by a complete immutable package inventory. |
| `paths` | Absolute selected `sysroot`, matching `resource_dir`, actual `wasm_ld_alias` and complete official `quickjs` source root. Preserve the wasm-ld invocation basename independently of its physical multicall target. |
| `typed_build` | Input label for the actual completed9-retained5-fresh Source40 Typed14 report. Its old failed predecessor remains failed. |
| `generated_go` | The real complete GO24 generated directory from that prerequisite. |
| `restart_mappings` | Explicit finite historical-device/current-device/root mappings consumed by the maintained physical evidence accessor. No recursive JSON rewriting is permitted. |

The prerequisite supplies the independently qualified official termination
object, complete native-bound generated GO family and original source labels.
The producer reopens their selected full pins, checks unchanged profile/emitter
bytes and every generated output/definition. It never builds a projected header,
invents a codec, replaces an assertion, or modifies the installed SDK.

Root invokes the producer only in the selected current64GiB/swap0 cgroup:

```text
python3 -B ACTUAL_SOURCE/tests/ros_unitree/prepare_direct_guests.py \
  --source ACTUAL_SOURCE --manifest ACTUAL_COMPLETE_MANIFEST \
  --manifest-sha256 ACTUAL_MANIFEST_SHA256 \
  --config ACTUAL_BUILD_CONFIG --config-sha256 ACTUAL_CONFIG_SHA256 \
  --out FRESH_BUILD_OUTPUT --selection-input direct_build_selection
```

It serially produces C, C++ and JavaScript denied/positive artifacts, for six
products. All real compile commands retain complete `-MD/-MF/-MT` system/header
dependencies, strict `-O3`, active assertions, disabled fast math/contraction,
actual objects and link traces. JavaScript first runs the maintained source-only
preparer, then executes its exact explicit engine/kernel jobs. No guessed engine
job count is used. A real full validator runs for every artifact.

Each binary is separately parsed for original ROS2 ABI signatures, only basic
fd_read/fd_write/proc_exit, one owned bounded64MiB memory, no implicit Wasm start,
and an actual local void `_start`. `_start` first emits `READY_UNITREE_MAIN` and
waits for byte `G`, before language constructors, JS initialization or user main.
Root proves the actual Worker and held exec target before releasing that gate.
The reported local-defined entry index is derived from the real function import
count and export table, not guessed from a compiler flag or absolute export index.

## Keep producer source roles separate

The completed source freeze containing this lane is distinct from the original
Source41 bootstrap/callback repair, Source39 native Gateway/controller builds,
Source37 interpreter builds and Source40 retained Guest/object builds. Their
manifests, compilation rows, process facts and cgroup snapshots keep their real
labels. A file/device bridge qualifies current content/identity only. It does
not rerun an old process, promote an old failure or establish cgroup continuity
across a reboot.

`direct_guest_receipts.py` reopens actual standalone compilation/MD/link/validator
records and reparses the artifact. For JavaScript it also reconstructs the exact
module table from the actual complete source-copy ledger, so embedding a small
replacement module cannot masquerade as the generated full codec/application.
The native controller consumer independently opens the Native74 one-TU full-MD
receipt, strict C++20/Go2 definitions, actual linker inputs and unchanged native/
current kernel source counterparts. It keeps that native source label separate.
If compilation and linking completed but an active-view catalog failed, the
consumer requires the separate successful `uwvm-native74-retained-products-at-rest-v1`
admission. It opens its unchanged original failed report, all actual retained
build evidence and fresh ordinary at-rest catalogs. The original status remains
`FAIL`. No mounted-device observation or prior process is thereby qualified.

## Execute controlled physics

The separate private COPY/namespace owner prepares a fresh CP310 content view,
then provides actual current Python/subtree pins in a completed view receipt.
The host-Python coordinator stays outside that chroot so its host-linked Worker
does not accidentally load an older Humble libc. Only the actual simulator and
independent Python state witness enter the admitted CP310 view. All source,
manifest, config, upstream, wheel and provider inputs appear at their genuine
absolute names through read-only binds. Only owned test outputs are writable.

The owner sets up a private loopback namespace and calls
`common.current_loopback(expected_namespace)`. This uses actual socket interface
enumeration and read-only SIOCGIFFLAGS, requiring only `lo` and UP/LOOPBACK. An
inherited `/sys/class/net` mount cannot prove the new namespace. The native check
sends no packet, changes no interface and grants no Guest network right.

`run_direct_physics.py` consumes the actual `physics_selection` labels documented
in its closed `prepare` function. Root selects actual `steps` (1–32), warmup
(0–2) and slowdown trigger, rather than deriving a desired result from timing.
The immutable config supplies the real model/actuator/sensor map and no direct
job. The coordinator records each derived language/job/policy selection beside
the unchanged raw config. It uses one fresh serial case per language.

Every case first runs the denied Guest while an independent real command
subscriber observes at least200ms/30 samples with zero commands and zero physics
steps. The native and Wasm positive controllers then run against separate real
simulator/witness instances. Initial tick1 is published only after actual state
subscribers match. The first genuine Guest callback creates its lazy LowCmd
publisher; its real publisher match is required after that publication and before
physics step1. Each step compares native state CRC, every active binary32 word,
independently received full command CRC/fields and real subsequent qpos/qvel.

At most4096 caller-paced polls are admitted. Exactly the selected count must
publish before `Q`, normal checked closure and the final PASS count. Complete
transcripts, real pidfd births, kernel-held exec target, current map_files/DSOs,
Gateway SO_PEERCRED/audit and all owned exits/cleanup remain required. A Guest
stdout marker alone never counts as DDS delivery or a physics step.

Total physics steps are derived from actual completed roles and selected counts.
The lane does not presume a historical96-step run or a missing old physics owner.
Fewer than30 post-warmup samples remain insufficient for performance. A correct
but slow result sets `profiling_required`; it cannot become a native-speed claim
until the separately controlled official VTune/optimization work is completed.
The measured interval includes caller-paced ROS delivery, control barriers and
supervisor scheduling. It does not isolate interpreter or kernel computation.
