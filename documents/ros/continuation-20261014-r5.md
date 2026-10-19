# ROS continuation: frozen source fixes and incomplete Linux acceptance

The original task remains incomplete. The latest source is frozen as r12 with
4,586 files and 36 changes from r10. Its manifest SHA256 is
`034b0c599ce4aa021feb77908aac843c8306097156974c7a291b68ad9a4c6d6f`.
Linux SSH became unavailable again after the actual r10 work; subsequent
connections timed out. The r12 changes have not been compiled or executed.

[The machine-readable record](continuation-20261014-r5.json) binds the source,
raw earlier results, source-only models and the exact next-run preparation.
The [original comparison](original-task-audit-20261013.md), its detailed JSON,
and all 126 protected earlier files remain byte-for-byte unchanged. This
continuation adds scoped results and corrections without promoting historical
results to a final current-source acceptance.

## Actual results received before SSH disconnected

The r4 record preserves official custom-package builds, three custom contract
suites, six original units, all 17 r9 WASIp1 native rows, four r10 Worker
profiles, six ROS default-deny configurations, two strictly qualified Gateways,
two native custom Topic probes and two raw-import custom Guest loopbacks.

The later r10 build and qualification also produced the closed set of 18 native
products. Both Gateway products use strict full source/SDK dependency evidence;
the other base probes retain their explicitly documented project-only MMD
scope. Compilation and qualification do not prove native probe behavior.

The actual 61-row SDK compiler matrix finished **38 passed, 23 failed**:

| Language/profile | Passed | Failed |
| --- | ---: | ---: |
| C | 6 | 4 import-contract failures |
| C++ | 2 | 6 import-contract and 4 compile/link failures |
| Rust | 10 | 0 |
| TinyGo | 10 | 0 |
| AssemblyScript | 8 | 9 compile failures |
| Concrete JavaScript C bridge | 2 | 0 |

The passing rows include actual binary import inspection and full Wasm
validation. This matrix does not execute ROS operations. Its original commands,
artifacts and failures remain in the raw report. The separate 17-row SDK
native/pure-model matrix requires a successful compiler receipt and has not
run. These 17 SDK rows are distinct from the passed WASIp1 native suite.

Four Action QoS behavior PASS lines and a successful parent lease exit were
received. The raw behavior archive transfer produced zero bytes when SSH
disconnected; that file is retained as a failed transfer, not admitted as an
evidence archive. The ROS2 local-event preparation failed compilation, but its
exact stderr could not be retrieved. Its failure cause remains unknown.

## Source changes prepared for the next Linux run

C/C++ tutorials now export their real helper entry points, retaining the ROS
imports used by those functions through linker garbage collection. The 12
WASIp1 C++ tutorial recipes explicitly disable exceptions; native model recipes
keep exception support. AssemblyScript uses `declareValue` instead of its
reserved method spelling, while the raw parameter import remains unchanged.
Complete import checks and all 61 rows are retained. Current SDK source and
pure-model checks passed for 217 inputs and 85 documentation links.

The ROS1 search controller disables unused Noetic test products and emits an
explicit MF dependency path, preserving its original closed two-source and
full-MD requirements. Two ROS2 local-event fixture formatting calls now use
`fast_io::mnp::os_c_str`. These are source fixes awaiting actual reruns; they do
not establish the disconnected ROS2 compiler failure's cause.

Native audit limits now parse the four explicitly configured numeric values
once per session/grant limiter. The cache is bound to the immutable configuration
and its original capability entry. Mode filtering, global refusal, lifetime
record/byte quotas, token accounting, suppression, close/reopen persistence and
the existing output writer are preserved. Pure models passed 32,671 checks and
34 prior selection comparisons. A separate C++17 native fixture prepares real
cache/binding/quota checks and hot-path C++ allocation observations; it has not
run. There is no measured speed claim. Native C++17 code retains its existing
compatible fixed formatter and descriptor helpers because vendored fast_io
requires C++20.

The ROS2 Gateway now clears the inherited enclave override and checks all
explicit environment edits before creating a context. Jazzy's
[credential selection](https://github.com/ros2/rcl/blob/jazzy/rcl/src/rcl/security.c)
otherwise gives a nonempty `ROS_SECURITY_ENCLAVE_OVERRIDE` precedence over the
supplied enclave. This preserves the configured signed identity and adds no
Guest permission or import. Source/synthetic checks passed 211 assertions; the
separate C++17 unit prepares 13 native assertions. Actual Gateway compilation,
signed Enforce behavior and Guest delivery remain pending.

## Original-task gaps and next execution

Historical G07 evidence was independently reconciled: both ROS backends
actually published from memory1 under ARM/QEMU, checked seven invalid-memory
refusals and the unaligned memory1 sentinel, and reached independent native
observers. The existing fixture generator and discovery gate match the
executed source. No duplicate implementation was added. This does not prove an
untouched memory0 sentinel or a multi-memory subscription output case, and the
complete current source still needs a rerun.

G01 remains a bounded fixed Topic increment, with broader custom types pending.
G02 unified seven-event delivery and G05 generic backpressure remain incomplete.
G03 search and G04 Action QoS need their current receipts/reruns; G06 final
current acceptance, G08 transport recovery, G09 Guest Enforce/revoked reconnect,
G10 attack calibration, G11 queue/IPC metrics, G12 complete historic/CI matrix,
G13 complete original examples/three-Group scenario and G14 final evidence
delivery remain open. L01 has source changes and scoped native PASS evidence;
the current Guest CLI, LLVM and checkpoint matrix is pending.

The six-language custom-codec supplement is rebound to r12 with 18 rows, 46
steps and 5,505 planned assertions. Its source capsule has no executable Linux
`prepared.json`; none of those rows ran. The new Guest Enforce plan requires a
separate signed native witness, literal environment selection, exact mirrored
grants/keystore paths and current live ARM mapping/PID/cleanup evidence.

After SSH recovery, remeasure the original container's boot, StartedAt, cgroup
device/inode, 64 GiB limit, zero swap and ancestor OOM counters. Retrieve the
missing r10 reports, stage the fresh r12 source, then run the SDK compiler and
model matrices, new native units, feature preparation/behavior and full current
product/suite acceptance serially in that cgroup. Earlier counters are not
combined with a new boot, and local compilation does not substitute for these
tests. The [current security description](security-current-20261014.md) records
default deny independently of the unchanged historical security material.
