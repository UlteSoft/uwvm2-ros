# Owned pidfd / procfs exit-race successor

This `/tmp` source-only patch preserves the currently frozen source and every
raw failed native report. It fixes the concrete observed interleaving in
`Actors.check`: the retained pidfd first polls nonterminal, the original task
then exits, and `/proc/PID/cgroup` disappears before its read. A missing procfs
entry alone never proves termination.

The sole production change adds `_owned_actor_snapshot` to the source-bound
Unitree common helper and calls it in the existing owned-record loop. Each
snapshot uses the **same original retained pidfd**, reads the original birth,
reads its actual cgroup membership, reads the birth again, and checks that same
pidfd again. ENOENT or ESRCH at either identity read or the cgroup read causes
only a recheck of that original pidfd. A confirmed terminal fd retires the
original record. A still-live/invalid fd, unknown errno (including EACCES/EIO),
or a changed/unavailable birth without terminal proof fails closed. No new
numeric PID is adopted or signalled; no numeric PID/process-group signalling is
introduced. A successfully observed cgroup mismatch is checked immediately, before any
trailing identity read or terminal retirement, and remains fatal even if that
trailing birth would vanish or change. Actual own-cgroup read errors propagate. Retirement records retain the original birth,
original post-acquisition birth and the exact terminal-poll phase.

All other existing helper definitions, cgroup/source checks, process spawning,
RSS/deadline/log limits, monotonic per-Frame ledgers, the SerialRunner process
owner and cleanup logic stay byte/AST equivalent. The existing processes and
old complete frozen source are not modified by this patch.

`test_actor_procfs_races.py::suite(common)` is an explicit factory receiving the
actual already admitted new Unitree common module. Its 25 deterministic DATA
cases inject each observation boundary and check terminal versus still-live
results, PID reuse refusal, preserved real errno, invalid pidfds, membership
violations and record-key mismatch. They create no process or pidfd, import no
candidate and establish no kernel or ROS evidence. Root runs the returned
unittest suite under the current64 source-pinned regression owner.

`actor_exit_stress.py::stress(common, frame, count=512, python_input='python')`
is a separate actual Linux entry, with no standalone bootstrap or side effects
on import. Root supplies the same current source/MF-admitted Unitree module,
its current64GiB/zero-swap Frame, actual explicit native Python input, jobs=1,
child RSS at most768MiB, finite command/deadline/log budgets and exclusive owned
output. The entry reopens the actual standard `frame.manifest`, requires its current
full-seven identity and the Frame report's admitted manifest SHA, then verifies the current helper/own-file MF bindings, current coordinator
Python, original cgroup facts and every live guard. It starts **512 real
serial children by default**, each blocked on one stdin byte until the parent
has acquired a live pidfd for that exact original birth. The parent releases
one byte, the child exits immediately, and the normal real Actors.check and
finish paths repeatedly exercise the exit window. Each child must emit the
exact retained marker, exit zero and complete owned pidfd cleanup. Each case
uses its own monotonic retirement ledger; no parallel child batch is created.
The count is explicit and bounded16..4096. Actual start/exit/log/cleanup rows
and retirement evidence remain in the Frame report. No ROS SDK, Wasm module,
Gateway, robot, compiler or unowned process is started.

Root must always call its normal `Frame.finish` for complete source/input/root
preservation and outer cleanup, including on exceptions. A real stress PASS
means the selected number of real owned short children completed; it does not
claim that every theoretical race occurred. The deterministic factory and real
kernel stress are separate evidence scopes. Both are authored but unexecuted
by this source author. There is no expected new source hash or future physical
pin in these sources.
