# Current-source mandatory regression stages

The complete `run_all.py` entry now retains the six existing native ROS unit
rows and the nine existing runtime regression commands for **each** freshly
compiled Worker profile: `none`, `ros1`, `ros2`, and `dual`. Selecting a smaller
`--suites` list still appends these mandatory stages, together with Action QoS,
WASIp1 default authority, CLI validation, the two ROS1 search contracts, and both
current ROS1/ROS2 feature stages. This execution recipe requires independently
qualified current-source Linux results; prepared source is not execution proof.

## Native units

`run_unit.py --suite original` selects exactly `security`, `ipc`,
`ipc-client-deadline`, `sorted-rows`, `sorted-rows-cpp17`, and `sdk`. It avoids
repeating the 17 WASIp1 rows, which retain their separate mandatory stage.
Each test uses the existing SIGSTOP barrier, original source dependency record,
qualified ARM image, sampled live mapping before resume, and result marker.
The compiler arguments force `-UNDEBUG` after configured flags so `-DNDEBUG`
cannot suppress the existing fixtures' assertions while leaving their markers.
The parent verifier rejects missing/duplicate rows, failure results, missing
barrier evidence, changed images, and an image at a substituted inode.

The unit sources must appear in the current frozen source manifest. All actual
compilation and execution must occur inside the provided Linux 64 GiB cgroup
(`memory.max=68719476736`, `memory.swap.max=0`, effective CPUs `0-31`);
source inspection and synthetic records cannot satisfy this stage.

## Existing runtime regressions

The stages `regression-none`, `regression-ros1`, `regression-ros2`, and
`regression-dual` call the existing `run_regression.py` wrapper with the exact
freshly compiled Worker for that profile. Each runs the unchanged nine-command
contract in `test/0073.ros-tests/support/source/ros/regression.py`: three WAT compilations and their WASI,
memory64 and interpreter tail-call executions, plus native Trusted-DL plugin
compilation, its WAT compilation, and its actual runtime invocation. The four
profiles therefore retain 36 commands and 16 runtime launches.

The wrapper needs `--source-addon` and `--source-addon-manifest` for the original
Trusted-DL example inputs if they are outside the main frozen source tree.
These are already required inputs of `run_all.py`. The parent checks their
manifest binding, exact profile and Worker hash, all nine distinct successful
commands, all four launch names, live Worker/plugin mappings, the current
complete ELF identities, the original `commands.json` hash/content and exact
argv, the real QEMU-prefixed launch argv, and the raw result-file hash/content. A report's
`PASS` label alone does not satisfy the stage.

This existing subset does **not** establish the entire historic interpreter,
LLVM/JIT, checkpoint restore or hosted CI requirement. In particular, the LLVM
clock floating-point-control and current-policy checkpoint-restore regressions
remain separate pending work. G12 remains incomplete until its entire original
scope is verified.

## Parent cgroup counters

The complete entry now discovers the leaf without accepting that probe's OOM
counters. It pins leaf device/inode and kernel boot, then captures its baseline
between two identity checks. The final probe is bracketed against the same
identity and original membership. Changed or unavailable identity fails without
subtracting counters from different cgroup incarnations. The shared counter
validator also rejects missing OOM counters, changed memory limits and nonzero
deltas. A controllerless hierarchy root is skipped only with actual cgroup2
mount-root, stable device/inode and explicit ENOENT evidence for its absent
memory controller files; unreadable/nonroot counters still fail. Optional
mount propagation tags may change during the private-mount setup without
changing that qualified root identity. Nested suites retain their independent
architecture/cgroup records. The shared sampler enumerates every thread's
`children` file, so a descendant created by a non-main thread can contribute
to actual mapping evidence. Unit subtree RSS uses the same enumeration.

## Local preparation checks

`test/0073.ros-tests/support/source/wasip1_default_deny/check_acceptance_sources.py` uses handwritten ELF
headers, synthetic reports, mocked identity callbacks, and small temporary
files to exercise admission failures. It never executes those headers, invokes
a compiler or QEMU, instantiates Wasm, or contacts ROS. Its six methods cover
L01 native admission, six original units, all four runtime profile reports,
pinned counter baselines, the two explicit-standard ROS1 search contracts,
and the 32-WAT/140-recipe CLI binding. Actual Linux
results must remain separate from these preparation checks.
