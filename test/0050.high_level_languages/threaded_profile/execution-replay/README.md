# Dynamic WASI Threads replay integration fixtures

`native.cc` is an actual embedding main. Compile it with the product's native
main flags and link it with the matching real Runtime and Host objects. Do not
link a second ordinary CLI main, substitute a fake provider, construct Group
credentials, or enable recording by changing private runtime fields.

The fixtures use a real source-owned shared-memory provider and dynamically
created native children. Run both `int` and `llvm` with current explicit
`--wasi-threads`, thread-spawn and WASI interface grants. `control` opts into real
cooperative observation, all-stop, and the production execution journal.

- `UWVM_TEST_HISTORY`: output/input unified history path.
- `UWVM_TEST_REPLAY=1`: decode and execute that history using real workers.
- `UWVM_TEST_SAME_PROCESS=1`: after recording, physically retire and destroy the
  original source, then instantiate and replay it again in the same process.
- `UWVM_TEST_PREFIX=1`: park at a recorded intermediate endpoint, obtain an
  actual all-stop, then continue only through the remaining known interval.
- `UWVM_TEST_CHECKPOINT=1`: capture a live three-worker nonterminal checkpoint;
  destroy its threads and source; reconstruct fresh real threads; compare the
  full shared memory and Wasm PCs; refuse to continue into an unknown future.
- `UWVM_TEST_PENDING=1`: use a genuine blocking input pipe and two in-flight
  `fd_read` calls; capture/reattach in process; reject a forked owner, forged
  record and expired stop; forbid result publication while paused.
- `UWVM_TEST_TRAP=1`: expect a real Wasm trap while other workers are waiting.
- `UWVM_TEST_EXIT_CODE=73`: expect an authorized worker `proc_exit(73)`.

`finite`, `wasi`, `pending`, `grow`, `provider-limit`, `nested`, `capacity`,
`proc-exit`, `trap`, `bounds`, `gc-eh`, and `checkpoint` cover worker birth/departure, actual output
suppression, entropy, shared growth, the provider maximum, nested spawning,
failed spawning and physical slot reuse, terminal cancellation and reconstruction. `gc-eh` retains a cyclic GC object across
a caught exception and an indirect Wasm function call in each native child.

`active-data` verifies reapplication of imported active data in each fresh
instance. `start-expressions` uses extended constant expressions, expression
elements and a normal start function containing calls and branches. `peer-start`
adds a preloaded Wasm code provider and checks dependency-ordered starts.
`peer-memoryless` uses a provider declaring no linear memory and verifies that
parent and child mutable globals remain independent without a synthetic memory
import.
`gc-global` checks that GC reference globals and their object graphs are
initialized privately; children must not inherit the parent's mutated object.

Use `owner-grow.wat` and a 262144-byte shared-memory budget for positive growth.
Use `owner.wat` for the negative provider-limit fixture: the importer declares a
larger maximum, but the actual one-page provider must still refuse growth.

`mte`, `mte-wait`, `mte-fault`, and `mte-log` cover tag entropy, shared growth,
32/64-bit infinite waits and notifications, not-equal and zero-timeout results,
terminal tag faults and actual report-only violations. Run `mte-log` with
`--runtime-memtag-action silent-log err` (or another explicit report sink) to
verify continued access and later events. Its crash-mode counterpart must stop
on the first mismatch. Both fault fixtures synchronize the real children before
the deliberately invalid access, so scheduler timing cannot skip a required
worker birth in the reconstruction test.

Build the fixtures with the installed Core 3 assembler:

```sh
python3 make_fixtures.py --wasm-tools /existing/path/wasm-tools --out /project/.tmp/threaded-fixtures
```

The generator validates the standard intermediate modules, then patches only
the deliberate MTE placeholders. Run the final binaries through the VM.

`mte.wat` uses a named assembler placeholder for the extension instruction. Its
sole call to `$tag` must be encoded as `FF AF 80 84 08 00` (`memtag.randomstorez`,
packed opcode `0x0101002f`, memory index 0). Recompute the code-body and section
lengths after substitution; do not run the unmodified no-op placeholder as an
MTE test. The resulting history must contain MTE entropy events.

Run the actual embedding matrix after compiling `native.cc` against the matching
Runtime/Host objects (the binary is the embedding fixture, not the CLI executable):

```sh
python3 run.py --binary /project/.tmp/native-fixture --fixtures /project/.tmp/threaded-fixtures --output /project/.tmp/threaded-results
```

For another ABI, pass `--runner-json /existing/runner.json` containing the exact
installed loader/QEMU argv. `--case` and `--engine` may be repeated for a focused
regression. The runner requires a new output directory and preserves the exact
binary/guest hashes, command arguments, logs, exit results and external-write
counts. The `checkpoint` and real native ROS gateway cases use the host API
modes above and independently deployed, isolated gateway receipts.

After the per-ABI recordings finish, `cross_replay.py --matrix DIR --output OUT`
replays five selected histories in both directions between x86_64 and each other
ABI, with both engines. `DIR/ARCH` contains the corresponding `run.py` output.
Each replay uses the origin's exact guest modules and authorized options with
the target's actual runner and binary. Semantically equivalent Wasm rebuilt
with different bytes is intentionally not substituted for the recorded source.

The offline reader has an independent regression that consumes the real
`pending` fixture's completed and still-in-flight archives:

```sh
python3 test/0018.debugger/test_snapshot_threaded_reader.py --complete /project/.tmp/threaded-results/pending-int.history --pending /project/.tmp/threaded-results/pending-int.history.pending
```

Use the actual history paths reported by your runner. This checks disclosure,
pagination, immutable ownership, lifecycle corruption, obsolete encodings and
unknown-result labels without invoking the VM or a native ROS gateway.

All test outputs and frozen builds belong under `.tmp`; they are not source
fixtures. The caller must place the process and all its descendants in the
required memory/swap cgroup before launching native tools or QEMU.

Fault-delivery and exact-prefix regression options are explicit:

```sh
python3 test/0050.high_level_languages/threaded_profile/execution-replay/run.py \
  --binary /project/.tmp/native-replay-test --fixtures /project/.tmp/fixtures \
  --output /project/.tmp/mte-async-results --case mte-fault \
  --memtag-backend software --memtag-mode async
```

Use the existing AArch64 QEMU runner and `QEMU_CPU=max` with
`--memtag-backend hardware` for functional hardware checks. Verify accepted
`PROT_MTE`, per-thread `prctl`, and real asynchronous MTE signals separately;
option acceptance is insufficient. Log-only checks use `--case mte-log` and
`--memtag-action silent-log`. QEMU timing is not native performance evidence.

`--prefix-at EVENT_INDEX` adds an exact stop/inspect/continue trial from the
recorded event list. Include positions immediately before spawn and host-call
admission, inside virtual import effects and waits, and after child departure.
The native fixture also accepts `UWVM_TEST_PREFIX_AT` with
`UWVM_TEST_PREFIX=1`. An invalid index is refused by the test fixture.

Check the actual product CLI separately from the embedding fixture:

```sh
python3 test/0050.high_level_languages/threaded_profile/execution-replay/cli_fault_actions.py \
  --binary /project/.tmp/uwvm2-ros --options-json /project/.tmp/authorized-trap-options.json \
  --expected-status 120 --output /project/.tmp/threaded-cli-faults
```

The JSON contains the ordinary, explicitly authorized argument array through
`--run /absolute/path/trap.wasm`, excluding the binary and QEMU runner. Use
`--expected-status 121` with `bounds.wasm`, or 122 with the actual MTE-fault fixture. The test records and
replays default, explicit crash, stable exit and pause/host-continuation cases.
It verifies a real process stop, classified exit or crash, and a sealed archive;
core files are disabled only in the test subprocess environment. Repeat with
both engines and all five existing ABI runners. A native embedding that skips
`run()` cannot substitute for this CLI policy regression.

`terminal-inflight.wat` combines an actual blocking `fd_read` with a trap in a
second child. `terminal-inflight-exit.wat` uses authorized `proc_exit(73)` instead.
Run the real CLI with `cli_inflight_fault.py` and an explicit authorized options
JSON; add `--proc-exit` for the latter guest. The parent retains the pipe writer,
observes an actual kernel pipe wait, and verifies exit/crash/pause without
publishing an incomplete archive. These are process-termination tests, not
synthetic native-call providers. Imported active data is supported: do not
classify a valid active segment as an unsupported input. The `active-data`
fixture checks actual reinitialization rather than copying parent bytes.
On an LLVM launch, add `--managed` to verify the trusted terminal decision
while an IDE is connected through the authenticated debugger channel.

Ordinary DAP integration uses `test/0018.debugger/run_thread_replay_dap.py`.
It consumes an actual LLVM embedding `*.argv.json`, reuses only the authorized
options after `--`, and starts the product CLI with its authenticated channel.
`--pending` uses a real blocking pipe and same-process retained-call reattachment.
`run_thread_replay_broker.py` additionally launches the normal secure broker and
the DAP adapter as separate processes, performs initialize/attach/configuration,
and verifies that a joined cohort does not terminate the IDE's replay session.
Both runners replay twice and require unchanged real external-write counts.
The DAP runner also accepts `--replay-history PATH` to test an explicitly
replaying managed launch, requiring zero real writes from process start. Add
`--initial-state joined` when using the completed archive saved by that test;
this verifies that a terminal replay endpoint keeps the IDE session available.
`run_thread_replay_ros_v1_dap.py` reuses the real native ROS matrix's authorized
options and gateway installation. Interpreter cases check the authenticated
endpoint rejection before gateway startup. LLVM cases pause the gateway after
startup, capture an admitted unresolved u2ros1 call through DAP, reattach it
in process, and verify that both subsequent replays leave the gateway's publish
and bind counters unchanged. A retained native participant waiting for admission
is distinguished from a genuinely unknown native result. An already-dispatched
publish can finish externally while paused, but the Wasm instruction locations
must remain unchanged until host continuation.
The u2ros1/u2ros2 designation is the uwvm2-ros v1/v2 interface ABI, not the
official ROS 1/ROS 2 backend version; the test selects each backend independently.

`run_thread_history_inspection.py` opens those archives in an unrelated ordinary
debugger, including with read-only permission. It compares each transferred
typed WASI/ROS environment to the offline archive, tests repeated sample changes,
and rejects limited permission, invalid samples and stale transfers without
changing the previous selection. Pass `--history` for each real WASI/ROS archive
and `--repeat` to stress the nested sample's lifetime. This inspection never
recreates the recorded program's native resources.

Additional private-input fixtures cover Core 3 reference operations and declared
function subtyping (`gc-references`, `gc-exception-references`,
`gc-cast-fault`, `typed-functions`), imported mutable/GC globals and dependency
start order (`peer-globals`), imported exception identity (`peer-tags`),
cross-module reference-valued functions (`peer-functions`), and nonnull initial
GC graphs with aliases, arrays, i31 values and external wrappers
(`gc-global-initial`). These run through actual worker creation, recording,
reconstruction and a stopped prefix in both engines.

The embedding fixture explicitly resets the runtime after physical worker join
and verifies that its generation advances. Stop/drain alone intentionally keeps
compiled code reusable. Reset releases JIT ownership while LLVM's registries
are alive, before the process-exit static-destruction fallback retains it.
Keep LeakSanitizer enabled when testing this normal reusable teardown path.
