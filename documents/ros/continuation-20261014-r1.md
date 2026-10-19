# Current-source continuation: default authority and ROS2 cancellation

The original task remains incomplete. The latest Linux SSH probe records an
offline Tailscale peer and SSH timeout. Current-source compilation, real ROS,
SDK guest execution and ARM/QEMU acceptance are **BLOCKED**. The companion
[source/evidence record](continuation-20261014-r1.json) records exact input
hashes and preserves the distinction between source/model checks and execution.

## Implemented changes

WASIp1 now starts without inherited host environment variables or non-FD
permissions. Standard input has READ, FILESTAT_GET and POLL; output/error have
WRITE, FILESTAT_GET and POLL. Inheriting rights are zero. Directory and socket
preopens remain empty until explicitly configured. Import visibility does not
grant authority. Global/Single/Group grant and deny commands select eight finite
permissions; configured target denies override Global grants. The explicit
`stdio-full-rights` compatibility permission broadens descriptor masks.

Underlying wasm32/64 clock, entropy, signal and yield APIs enforce the selected
environment's authority. Both poll APIs preflight the entire clock array before
effects/output, and require the subscribed FD direction. Private checkpoint
worlds retain the current trusted permission mask; snapshot bytes never restore
non-FD authority. See the [CLI guide](../command-line/wasi.md) and
[native entry guide](../../tests/wasip1_default_deny/README.arm-entry.md).

ROS2 cancellation now preflights all eight cancel-service QoS fields across the
complete selected outgoing Goal set before native cancellation. Incoming Goal
cancellation validates actual client QoS before discovery/request. Four prepared
QoS probes now cover response and cancellation; their controlled QoS faults
remain explicitly labeled. See [cancel regressions](../../tests/ros_qos/cancel_README.md).

The complete ARM runner requires a current 26-product runtime pool, four newly
compiled Workers, the four QoS probes, 17 WASIp1 native rows and 140 CLI recipes
using 32 WAT witnesses. It rejects old dependency proofs, missing rows and changed
artifact identity. The CLI retains WAT text/hashes and binary hashes before/after
each invocation. The native suite also includes 14 existing positive regressions.
Fresh source manifests must include these original test sources and their
`fp_control_probe.h`; old source overlays do not include the new closure.

The SDK adds a [17-row model driver](../../sdk/docs/model-matrix.md) for CMake
installation relocation, installed C/C++ models, native Rust/Go test inventories
and import-free AssemblyScript models. It records current cgroup, OOM, source,
tool and artifact guards. Its default mode starts no tools. The SDK source-only
bundle has 186 files and SHA256
`47289c07038eeb144856adb92f786c7d0e2282589e68cd46742499cd56e6b857`;
the bundle and manifest are under `/tmp/uwvm-sdk-20261014-source-r2/`.

## Checks actually performed

The SDK aggregate passed 179 source inputs, 74 documentation links, 365 language
declarations and 88 dispatch slots. Its JavaScript codec/fake-bridge/options/mock
filesystem suites passed 626 checks. Configuration checks passed 49 source checks
and 655,360 Python merge models; function checks passed four methods and 65,536
mask combinations. ARM-entry source models passed 55 checks; cancellation source
models passed 72; SDK native-driver synthetic review passed 43. Two additional
admission-model methods and three existing artifact-binding methods passed.
None of these counts represents compiled C/C++/Rust/Go/AssemblyScript execution.
The 294 AssemblyScript assertions remain prepared for actual execution.

Independent review found and corrected a missing-root `memory.max` assumption
and admission of a same-byte native artifact at a replaced inode. Their pure
models pass. All 126 protected prior evidence files remain byte-identical. Local
ENOSPC was recovered by APFS copy-on-write consolidation of byte-identical,
unpublished staging duplicates; original paths/content and canonical copies were
retained. Earlier failure records were preserved.

## Remaining original requirements

G01 custom IDL/registry, G02 seven-event protocol and G03 parent parameter search
remain **FAIL**. G05 generic backpressure remains **UNSUPPORTED**. G04 native QoS,
G06 full current acceptance, current G07 rerun, G08 transport recovery, G09 Guest
SROS2/revoke/reconnect, G10 attack calibration, G11 metrics/performance, G12 full
regressions/CI, G14 unified final delivery and L01 real default-deny validation
remain **BLOCKED**. G13 complete example/three-Group material remains **FAIL**.
Historical limited QEMU results retain their original source scope. This update
does not combine them into a current-source PASS. The immutable
[237-row original audit](original-task-audit-20261013.md) remains the baseline.
