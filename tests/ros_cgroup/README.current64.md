# Current 64 GiB acceptance environment

The user selected **64 GiB** for the unified multi-distribution, multi-architecture
ROS and debugger acceptance run. The original container's live cgroup leaf must
have `memory.max=68719476736` and `memory.swap.max=0`. Verify its actual boot ID,
leaf device/inode, PID membership, effective CPUs `0-31`, readable ancestor
limits and OOM counters before, during and after execution. A historical 48 GiB
or 64 GiB report cannot qualify current source by changing a budget field.
Changing the configured limit is preparation, not a test result.

[`probe.py`](probe.py) requires both the actual leaf and effective memory limit
to equal 64 GiB. Larger leaves limited only by ancestors are refused. Missing or
unreadable nonroot controller data, a partial expected identity, changed boot,
swap, CPU set or process membership all fail closed. The only omitted counter
source is an independently qualified controllerless cgroup-v2 hierarchy root.

Pass fresh `--expected-leaf`, `--expected-device`, `--expected-inode`,
`--expected-boot` and `--expected-memory-max 68719476736` values to SDK drivers.
The SDK's default aggregate RSS guard remains 2,048 MiB; its whole-cgroup current
memory guard is 61,440 MiB (60 GiB), leaving 4 GiB below the hard limit. Per-leaf
compiler limits remain lower where selected (the complete SDK compiler leaf
uses 512 MiB). Never expand parallelism to consume the cgroup allowance: all
actual builds and tests use a single serial owner and `-j1`.

Finish and freeze the implementation before the unified test phase. Runtime,
Gateway, SDK and original regression results require actual execution on those
frozen bytes. Distribution/profile inventory, source adaptation, native SDK
Pub/Sub, Gateway compilation, and Wasm/ROS behavior are separately reported.
Each QEMU target needs its own actual matching ELF, loader, SDK/sysroot and
process evidence; an aarch64 run cannot qualify LoongArch or another target.

The maintained synthetic budget model remains named `limit48_models.py` for
caller compatibility; it now accepts the current 64 GiB contract and rejects
old 48 GiB limits, stale boot/leaf identity and inherited-only limits. Execute
it and the existing `root_counter_models.py` only during the unified test phase;
synthetic success never proves live cgroup, ROS, Guest or QEMU acceptance.

Retain raw failures, exact argv and tool/source hashes, held pidfd birth evidence,
owned descendant cleanup, all lease restoration and unchanged source/SDK pins.
Debugger observation must neither acquire ROS authority nor consume data from
an event stream. Current memory policy never promotes historical receipts.
