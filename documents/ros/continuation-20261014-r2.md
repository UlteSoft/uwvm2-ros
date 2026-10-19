# Current-source continuation: ROS parameters and acceptance

The original task remains incomplete. The latest Linux probe records an offline
Tailscale peer and SSH exit 255. Current-source compilation, actual ROS behavior,
SDK Guest execution and ARM/QEMU acceptance remain **BLOCKED**. The companion
[source and evidence record](continuation-20261014-r2.json) binds this revision
without upgrading source/model results into runtime execution evidence.

## Runtime changes

ROS2 now delivers process-local parameter events for successful local adapter
declarations, sets and undeclarations. Its native parameter-event publisher
remains disabled. Only explicitly activated exact-node/name event grants receive
events, after actual subscription QoS verification. The existing type, numeric,
element, UTF-8, envelope, rate, bandwidth, depth and overflow restrictions apply.
Weak routes are pruned and independently bounded; close, revoke and session
teardown deactivate queues before late delivery. A committed mutation can still
produce an event if its RPC subsequently expires. External native parameter
service/direct Node changes with publication disabled are outside this local
path. See the [local event contract and prepared native fixtures](../../tests/ros_local_parameter_events/README.md).

Independent review replaced a borrowed declaration result with an owned,
active-field snapshot captured under the native post-set callback. The observer
selects only this writer thread and parameter, bounds copies before allocation,
never enters the registry/queue while holding the native mutation lock, and is
removed on completion/refusal. Event delivery cannot change committed RPC state.
The exact frozen Jazzy SDK still needs actual compilation and native testing.

ROS1 adds `search-parent-namespaces=true` as an explicit optional authority for
`param.search`, including CLI, Worker and Gateway validation. Absent/false keeps
exact search. The fixed final key can be checked in ancestor namespaces only
within its segment-aware prefix, before each native query. The Worker verifies
returned names against the same contract. The empty request ABI remains fixed;
native multi-component relative-key partial matching is outside this adapter.
ROS1 Action feedback now excludes only its respond framing byte from the
advertiser feedback budget; responder requests and terminal results retain
their complete lengths. See the [ROS1 implementation and test record](ros1-search-20261014-r1.md).

These additions grant no default ROS resource or WASIp1 permission. The preceding
WASIp1 directional stdio, empty environment inheritance and zero non-FD authority
changes remain implemented in source and pending actual validation.

## Acceptance and SDK preparation

The ARM complete entry now retains the six original ROS native unit rows and
all four Worker profiles' nine-command runtime regression contracts. Together
these preserve 36 commands and 16 runtime launches. Unit compilation forces
`-UNDEBUG` after configured flags. Parent admission binds exact original command
JSON, QEMU launch argv, Worker/profile/source/addon/config bytes, live ARM mapping
and current complete ELF identity. These join the existing mandatory Action QoS,
17 WASIp1 native rows and 32-WAT/140-recipe CLI stages. See the
[mandatory-stage guide](../../tests/ros_aarch64/README.current-regressions.md).

The parent pins leaf device/inode and boot before accepting its OOM baseline,
brackets final probes, and never subtracts across replaced cgroup incarnations.
A normal controllerless hierarchy root is skipped only with qualified cgroup2
mount-root/device/inode and explicit ENOENT evidence. Nonroot or unreadable
counters still fail. Optional mount propagation tags do not change root identity.

The six newly prepared ROS1/ROS2 feature fixtures still require fresh qualified
compilation/execution and their own architecture evidence. Their presence does
not expand the existing 26-product pool into a qualified current-feature PASS.
The existing runtime regression subset also does not satisfy the full historic
LLVM/JIT/checkpoint/CI requirement.

SDK bounded artifact/report reads now open nonblocking and reject nonregular or
oversized files before reading, with same-descriptor consistency and cleanup.
SDK guides/ABI descriptions reflect conditional ROS1 parent search, local ROS2
event boundaries and the current WASIp1 defaults. The latest whole-SDK source
report is [revision 9](../../sdk/tests/verification-20261014-r9.json). The verified
source-only SDK archive has 193 files, SHA256
`b09d338e9f3f61362060bab5f76aca68f819f78549b05b8fa790d85ebf66d465`,
and resides at `/tmp/uwvm-sdk-20261014-source-r4/sdk.tar.gz`.

## Evidence boundary and remaining work

The companion record lists the exact source/model counts and hashes after final
peer review. These Python/JavaScript checks use source predicates, independent
wire/authority models, mocked descriptors/identity and handwritten nonexecuted
ELF/Wasm inputs. They invoke no compiler, actual Guest, ROS, QEMU or live cgroup.
All 126 protected prior evidence files remain byte-identical. Historical failures
and the local ENOSPC/connectivity records remain preserved.

G01 custom IDL/registry, G02 unified seven-event protocol and G13 complete
examples/three-Group material remain incomplete. G03 now has a source-level
fixed-key parent projection; full native relative-key equivalence and actual
execution remain incomplete. G05 generic backpressure remains unsupported.
G04 native QoS, G06 full current acceptance, current G07 rerun, G08 transport
recovery, G09 Guest SROS2/revoke/reconnect, G10 attack calibration, G11 metrics,
G12 full regressions/CI, G14 final unified delivery and L01 real default-deny
validation remain blocked or incomplete. The immutable
[237-row original audit](original-task-audit-20261013.md) remains the baseline;
historical limited QEMU results retain their original source scope.
