# Current ARM fixed custom topic binding fixtures

`run_custom_topics.py` prepares and runs exactly two real native binding fixtures:
the ROS1 `Packet` fixture (23 canonical bytes, Noetic/GCC9/C++17) and ROS2
`FixedSample` fixture (45 canonical bytes, Jazzy/GCC13/C++20). The latter uses
the vendored `fast_io` for diagnostics. Both compile the original fixture source
with enabled assertions, `-O1 -g0`, actual system-inclusive `-MD` dependencies,
and a constructor barrier for live pre-main ARM mapping evidence.

The source and every executed helper must appear in the current exact frozen
source manifest. Select the official installed message package prefix
`/opt/uwvm-custom-types` in both canonical task-owned SDK roots. Both common and
native provider headers are required, with explicit native packages for each
version. Headers may be within frozen `/work` or immutable SDK `/opt`; arbitrary
external or `/tmp` provider headers are refused. The controller records opened
file identities and SHA-256 values for the headers and selected package configs.

Run this controller only through the parent-proven Linux root/private mount
namespace in the existing 64 GiB, zero-swap cgroup. The parent must provide the
current container `StartedAt`, verified x86 QEMU path, and active task-owned
ELF183 binfmt lease evidence/environment. Never compile or run locally to bypass
a missing Linux connection. Compilation is serial and every owned command has a
2 GiB subtree RSS limit, a deadline and a bounded log. Immutable source, full SDK
inventories, file pins, cgroup incarnation, swap limits and OOM counters are
rechecked. PID/birth/pidfd ownership covers launchers and observed descendants.

Use these explicit arguments in the parent cgroup launch:

```text
--mode prepare
--source <current frozen source>
--sdk-workspace <canonical SDK workspace>
--out <fresh preparation directory>
--qemu <verified QEMU>
--binfmt-evidence <current parent lease JSON>
--container-started-at <current Docker State.StartedAt>
--custom-registry-header /work/test/0073.ros-tests/support/source/ros_custom_types/deployment-r1/generated/manifest.hpp
--custom-native-header 1=/work/test/0073.ros-tests/support/source/ros_custom_types/deployment-r1/generated/native_ros1.hpp
--custom-native-header 2=/work/test/0073.ros-tests/support/source/ros_custom_types/deployment-r1/generated/native_ros2.hpp
--custom-packages 1=uwvm_custom_ros1_fixture
--custom-packages 2=uwvm_ros_custom_test_msgs
--custom-prefix /opt/uwvm-custom-types
```

Preparation success is `PREPARED`, with two qualified fixture binaries and zero
native behavior results. To execute that preparation, use a fresh output
directory, replace `--mode prepare` with `--mode run`, and add
`--prepared <preparation directory>/report.json`. All other selection inputs and
the current cgroup identity must match. The `all` mode performs both phases in
one fresh run without `--prepared`.

Execution acquires the same ROS2 domain-203 lock as the local parameter event
driver. ROS1 uses a separate exclusive Master-port lock (`11389` by default),
starts an owned Master, and binds XMLRPC readiness to a currently mapped owned
ARM Python/QEMU process. Each fixture stops in its constructor; the controller
verifies the current ARM image, QEMU image, birth and cgroup before pidfd resume.
The exact original fixture completion marker is required, together with exit
code zero. A complete behavior PASS has exactly two native fixture rows, zero
remaining owned processes/mounts, preserved inputs and one unchanged cgroup
incarnation. Preparation alone must never be reported as that behavior PASS.

The ROS1 fixture forks its native peer before either ROS initialization. Its
inbound malformed-data witness and outbound exact-byte witness use opposite
processes and caller IDs. The fixture checks actual connected TCPROS bus rows;
the controller additionally binds the printed peer PID to an observed owned
ARM/QEMU descendant birth in the same cgroup. A child success acknowledgement
and successful reaping are mandatory before the original main PASS marker.

If one native behavior probe fails, the controller preserves its failure and
retires every current owned process, including the Master and fork peer, through
the existing bounded pidfd cleanup. It rechecks the cgroup, active binfmt lease,
headroom and qualified input pins before starting the other version, using a new
ownership tracker over the same complete architecture evidence. This collects
independent diagnostics. Any such failure keeps the overall result FAIL; exactly
two successful native behavior probes remain necessary for PASS. Interruptions,
failed cleanup, or a changed execution environment abort further launches.

These probes exercise typed native topic adapters directly. They do not run a
Wasm Guest or an authenticated Gateway IPC session. They cannot establish SDK
language Guest interoperability, cross-instance handle denial, IPC authority,
default permission denial, production SROS2, custom Service/Action support, or
completion of original task G01. Those remain separate required acceptance work.
Generation/package build results and prior native failures remain immutable;
select fresh preparation/run output directories instead of overwriting reports.
