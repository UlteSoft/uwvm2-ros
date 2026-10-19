# Remaining original-task acceptance

This directory implements bounded, source-bound fixtures for the original
task's recovery, memory isolation, native side effects, queue measurements,
twenty examples and six-language native delivery. New source preparation is
not a test result. No file in this directory claims that its new tests ran.

Use the existing Linux task container and the parent's exclusive serial lane.
The current user-selected budget is 64 GiB with swap disabled. Verify the
current boot, StartedAt, cgroup leaf device/inode, every readable ancestor and
OOM counters again after each reboot or budget change; historical 48 GiB and
64 GiB receipts remain historical. Run each driver through the existing `binfmt_lease.py` wrapper and
a private mount namespace. Native compilers run with one job and a 2 GiB
observed process-tree RSS limit. Each actor has a finite deadline, pinned
product, pidfd-retained birth and real stopped ARM mapping before its input
gate is released. No command uses an ambient ROS or WASI permission preset.

`protocol.py` contains strict receipt predicates. `run.py` executes the closed
case inventory serially from an explicit physical launch manifest and retains
every failure. It never converts preparation, native-only results, SKIP or
BLOCKED into Guest acceptance. The original regressions and their receipts
are separate mandatory inputs to final acceptance, not replaced by these
fixtures. Source-only checks must use `python3 -B` with
`PYTHONDONTWRITEBYTECODE=1` and must not write into a frozen source tree.

Recovery uses only owned native actors. Stopping a ROS1 Master does not imply
that existing TCPROS connections disappear. A peer transport fault disposes
the owned TCPROS subscriptions/publishers or DDS context, waits for real
remote endpoint retirement, then creates a fresh peer. Publishing into an
unmatched topic may still return success; independent native counts decide
delivery. No system firewall, shared network namespace, unrelated Master,
non-owned PID or external ROS participant is changed.

The JS native delivery path retains one real uwvm Instance in a Wasm C relay.
Node receives copied binary responses over its private pipes and never opens
Gateway IPC, sends a caller identity or substitutes a fabricated Instance or
Memory. JS host code runs outside the Wasm engine. The only relay WASI imports
are basic stdin/stdout and process exit. Monotonic clock is an explicit grant
only in fixtures that import it. The default-denied native rows create no
Guest ROS resource and require both errno 13 and independently observed zero
native effects.

Queue occupancy and high-water are sampled from the real native queue under
its mutex. Configured queue depth is not a measured occupancy. Isolated
socketpair framing overhead is reported separately from authenticated
Worker/Gateway round trips and native ROS latency. QEMU measurements are
simulation measurements; they establish no physical ARM speed claim.

Read [SELECTION.md](SELECTION.md) for source derivation, closed serial launch,
legacy raw-receipt normalization and complete inventories. New preparation
and actual-run entries are `prepare_native.py`, `build_delivery.py`,
`run_delivery.py`, `run_recovery.py`, `run_attacks.py`, `run_metrics.py` and
`run_bounded_service.py`. `freeze_source.py` is the source-only immutable
SourceA/SourceB copier. `selection_template.py` fixes concrete original case
selectors and all numbered crosswalks; `make_manifest.py` renders the closed
serial launch plan. The root-owned `run_events.py` provides real authenticated
Guest/native seven-kind exchanges for both versions. Bound custom client/server
rows use `../ros_custom_types/bounded-v2/run_custom_bounded.py` after official
derivation and full composite freezing. Only Root's later guarded executions can
produce successful terminal behavior results.
