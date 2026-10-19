> Historical 48 GiB instructions: the current user budget is **64 GiB**.
> Use [README.current64.md](README.current64.md) for every new run. Existing
> 48 GiB reports and these historical instructions keep their original scope.

# Current 48 GiB acceptance environment

The user selected **48 GiB** after rebooting the original Linux test host. New
source and SDK acceptance therefore require `memory.max = 51539607552` and
`memory.swap.max = 0` in the original container's actual leaf. Earlier 64 GiB
reports retain their original meaning and hashes. They cannot qualify a new
48 GiB execution by replacing a report field or reusing a frozen helper.

`probe.py` proves the executing PID's visible cgroup-v2 membership, reads the
actual leaf and ancestor limits, and brackets leaf device/inode, boot ID,
membership, live PID membership and exact leaf limit. Both the leaf and effective
ancestor limit must equal 48 GiB; a larger leaf limited by an ancestor is refused.
The only skipped memory counter source is the independently qualified
controllerless hierarchy root. Unreadable nonroot counters and missing swap or
boot evidence are failures. The optional expected identity is all-or-nothing.

SDK execution additionally requires explicit `--expected-leaf`,
`--expected-device`, `--expected-inode`, `--expected-boot` and
`--expected-memory-max 51539607552`. Zero swap and CPUs `0-31` are closed options.
Read the values from the freshly verified Linux environment; do not copy a
historical inode or boot ID. The SDK defaults are a 2,048 MiB aggregate sampled RSS
guard and a 45,056 MiB (44 GiB) current-memory guard. The latter must remain
strictly below 49,152 MiB (48 GiB). Existing tool/held-launch/birth/pidfd cleanup,
artifact identity, exact import, OOM counter and command deadline checks remain
required. Native SDK models do not establish ARM or ROS behavior.

The standalone AssemblyScript runner receives the same explicit memory and
incarnation controls. It refuses old 64 GiB evidence and inherits no additional
imports. The compiler matrix retains 61 rows, the SDK model matrix retains 17
rows, and all seven AssemblyScript fixture assertion contracts are unchanged.

Source verification only:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 -B tests/ros_cgroup/limit48_models.py
PYTHONDONTWRITEBYTECODE=1 python3 -B tests/ros_cgroup/root_counter_models.py
PYTHONDONTWRITEBYTECODE=1 python3 -B sdk/tools/verify_sources.py
```

These commands use synthetic files, handcrafted data or fake bridges. They do
not compile or execute a Guest, start ROS/QEMU, or prove live cgroup acceptance.
Actual compilation, validation and SDK execution belong to the root agent's
serial Linux lane in the original freshly verified 48 GiB container.
