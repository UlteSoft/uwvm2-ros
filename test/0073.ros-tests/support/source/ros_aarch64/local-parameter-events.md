# AArch64 ROS2 local parameter event qualification

`run_local_parameter_events.py` prepares and runs exactly two fixtures from
[`ros_local_parameter_events`](../ros_local_parameter_events/README.md):
`local-parameter-events-contract` and `local-parameter-events-ipc`. Both are
required. The IPC fixture starts an independently qualified current ROS2 Gateway;
the contract fixture exercises the trusted resources adapter directly.

These commands belong on Linux under the parent's existing 48-GiB cgroup, with
`memory.swap.max=0` and effective CPUs `0-31`. The driver never creates or modifies
a cgroup or binfmt registration. The parent must hold its exact fixed-interpreter
QEMU lease, export `UWVM_ROS_ARM_BINFMT_ENTRY`, and launch the driver in a private
mount namespace using `unshare -m`. `--container-started-at` is the actual Docker
start time captured by the parent, recorded alongside the live leaf identity
and kernel boot ID. It is not an independently queried Docker identity.

Run the controller from `SOURCE/test/0073.ros-tests/support/source/ros_aarch64`. The complete frozen source
manifest at `SOURCE/../source-manifest.json` must include that controller folder
and all current helper bytes, including `gateway_qualification.py`. A separate
historical drivers directory is not an admissible current recipe. Source and
original SDK are mounted read-only inside a private owned clone; `/tmp` and
`/root` use fresh owned directories. Every output path must be new and separate
from the source and original SDK. The canonical SDK must carry its existing
`.uwvm-task-owned` marker. Source, SDK and output path spellings must contain no
whitespace or backslash, because mountinfo/dependency identity checks use the
original unescaped paths.

```sh
unshare -m python3 -B SOURCE/test/0073.ros-tests/support/source/ros_aarch64/run_local_parameter_events.py \
  --mode prepare --source SOURCE --rootfs FROZEN_JAZZY_ROOTFS \
  --out FRESH_PREPARATION --qemu PINNED_QEMU \
  --binfmt-evidence PARENT_LEASE_REPORT \
  --container-started-at ACTUAL_DOCKER_STARTED_AT

unshare -m python3 -B SOURCE/test/0073.ros-tests/support/source/ros_aarch64/run_local_parameter_events.py \
  --mode execute --source SOURCE --rootfs FROZEN_JAZZY_ROOTFS \
  --prepared FRESH_PREPARATION/report.json --out FRESH_EXECUTION \
  --gateway CURRENT_STRICT_ROS2_GATEWAY \
  --gateway-report CURRENT_TARGETED_GATEWAY_BUILD_REPORT \
  --qemu PINNED_QEMU --binfmt-evidence PARENT_LEASE_REPORT \
  --container-started-at ACTUAL_DOCKER_STARTED_AT
```

Preparation compiles both unchanged fixture sources with native ARM GCC13 and
the frozen Jazzy SDK, explicitly selecting C++20, `-O1`, `-g0`, `-UNDEBUG` and
system-inclusive `-MD`. Only one compiler job runs at a time; the observed compile
subtree RSS is limited to 2 GiB, configuration to 120 seconds, and compilation to
600 seconds. Both commands use the same 2-GiB observed RSS ceiling. The
actual verbose command supplies the final effective `-MF`; its code generation
arguments must match `compile_commands.json`. The raw dependency rule and full
project, generated barrier, standard-library and ROS SDK inputs are preserved
with `flags.make`, `link.txt`, generated CMake, compiler database and compiler log.
The complete static SDK keyset, file bytes, symlinks and root aliases are checked
before and after. Preparation returns `PREPARED`, with no ROS behavior verdict.
There must be exactly two compiler database sources, one original compile and
link command per fixture, and the exact generated constructor/CMake recipe.

Execution rechecks that exact preparation and its actual compiler architecture
evidence. Raw 48-GiB, same-incarnation, ancestor OOM and cleanup evidence is
independently recomputed; successful compile PID/birth and actual GCC13,
frontend and CMake mappings must match the recorded tools and products. The
preparation and behavior stages must retain the same inherited leaf/boot/Docker
start binding. Original helpers and their archives are bound by complete file
identity, as are the raw metadata inputs. The shared strict
`gateway_qualification.py` additionally rechecks the
Gateway's current source, original actual `-MD`/final `-MF`, raw recipe, compiled
object rule, standard/ROS headers, link inputs, tools, compiler mappings, full
SDK inventory and archived executed drivers. The selected product must match
the complete qualified ARM file identity, before and after its independent copy.
Parent pool copies do not substitute for the actual qualified input product.

Each fixture has a constructor stop barrier. Before resuming, the driver proves
the actual live ARM mapping by file device/inode, process birth, owned ancestry,
current cgroup membership and separate QEMU host executable identity. A chroot
launcher and an owned direct ARM child remain distinct process identities. The
IPC fixture's actual Gateway child is separately paused with its pinned pidfd,
checked for the same mapping/birth/parent/cgroup facts, and resumed without
modifying the Gateway executable. Command-line text alone is insufficient.
Fixture evidence is revalidated after acquiring its pidfd; Gateway host image,
live cgroup membership and parent birth are checked again immediately before
resume. Private fixture/Gateway copies must have distinct device/inode identity.

Native execution uses fixed ROS domain 203 and an exclusively created domain
lock. Each fixture has a 120-second behavior deadline and an 8-GiB observed
subtree RSS ceiling; overall cgroup current memory must remain below 60 GiB (61,440 MiB).
Completion requires exactly one complete original semantic line from each
fixture, including `observed_gateway_dds_events=0` from IPC, actual mappings, unchanged
source/SDK/tools/control-input bindings and same-incarnation zero OOM deltas.
Prepared report JSON is parsed and hashed from the same bounded descriptor.
Raw input files use the shared streaming regular-file descriptor binder.

Owned descendants are discovered through every native thread's children list.
Birth and parent checks precede registration; a process is recorded before its
cgroup membership is checked so an escaping owned child can still be cleaned
up. Process state changes between descriptor acquisition reads are allowed;
PID, parent and birth changes are rejected and the incomplete descriptor closed.
A task disappearing during exit has unavailable live evidence; a still-live
cgroup departure fails. Observed RSS uses every pinned live birth, including
worker-forked children. Cleanup signals only held pidfds, never a potentially reused numeric process
group. Successful reports require no owned process, mount or domain-lock residue.
Errors and partial command facts remain in the fresh attempt's report.

The two behavior fixtures cover the adapter-only notification contract described
in their README. Their native publisher observation is bounded, and their output
checks are not allocation measurements. This suite does not establish Wasm guest
execution, SROS2 Enforce, physical ARM performance, all external native writer
semantics or completion of the original runtime task. Actual compilation and
behavior remain pending until Linux reports qualify them.

The local review entry point is pure Python metadata/source/ownership modeling:

```sh
python3 -B SOURCE/test/0073.ros-tests/support/source/ros_aarch64/test_local_parameter_events_driver.py \
  --report FRESH_PURE_MODEL_REPORT.json
```

Those controls launch no process, invoke no compiler, send no signal, instantiate
no Wasm and create no ROS context. Their result must remain separate from the
driver's actual `PREPARED` or behavior `PASS` reports.
The optional report records full input bindings before and after the pure
checks; an existing report path is refused so earlier evidence is preserved.
