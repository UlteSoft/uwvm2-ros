# B1 cooperative debug-control qualification source

These qualification sources use the
actual source-owned Runtime embedding entry, canonical imported shared memory,
native workers and public debug/retirement APIs. No test constructs a fake
Group, admission, provider or pause participant.

Read the [native control API guide](../../../../sdk/origin/docs/threaded-debug-control.md)
and the [existing profile qualification](../qualification-r3/README.md). Build
this main with the final Runtime's actual object/header/feature/link closure;
do not link it alongside the ordinary CLI main. Compile the selected WAT files with the
admitted real tool on SSH Linux, then supply the memory provider through the
normal validated profile loader. Keep the final Wasm, executable, linker map,
source/toolchain identities and explicit finite profile limits.

The embedding's argument shape is `none|control|race|branches|turn-timeout|mte-stop int|llvm --` followed by
an explicit `--debug-permissions` selection, the ordinary finite threaded
options and main Wasm selection. It seals that actual host launch policy because
it deliberately does not call the CLI `run()` wrapper. A real early one-worker
stop before Guest `thread-spawn` is resumed until the required two-worker cohort
exists. The first capacity query returns counts only; metadata availability is
checked after the subsequent successful read.
`none` requires control refusal and physical join. `control` needs a real linked
wait plus a looping worker, 100 real stopped episodes, stable participant and
incarnation identities, real terminal cancellation, no post-wait continuation,
physical join and an issued stale guard retained through public stop/drain.
Both engines must provide an actual `before_instruction` location for the
looping worker; LLVM function-only stops cannot satisfy the test. Every emitted
logical site must have a valid per-function ordinal and a file-relative Wasm
coordinate in its original body. Real pending waits retain their distinct wait
status, without inventing an instruction position. Read-only, limited and none
launch permissions must refuse control enablement.
`control` additionally advances the looping worker 128 times while the sibling
remains in the same real native wait. Every step must invalidate the prior
observation, preserve participant/incarnation identity, and reach a fresh actual
instruction. Invalid identities, expired deadlines and native-wait advances are
refused without claiming that an instruction executed.

`branches` uses `b1_turn_branches.wat` and records 512 exact locations, including
alternating if/else arms, a missing else, a branch that skips a lexical end, a
nested call and early return. `compare_instruction_traces.py` compares complete
ordered periodic traces for the interpreter and LLVM; it reports any excluded
initial transient explicitly. It is a comparator for these periodic fixtures,
not a general Wasm semantic oracle.

`turn-timeout` uses `memory_provider_large.wat` (64 MiB) and
`b1_turn_timeout.wat` (a real 32 MiB `memory.fill`). Supply a shared-memory budget
of at least 67,108,864 bytes. A one-millisecond advance deadline must expire with
the transfer still pending; reading must report busy with no old rows, another
participant cannot steal the transfer, and retrying the same participant must
join that original operation. No synthetic wait or forced successful timeout is
used. The outer process deadline must allow the slower emulated targets.

`mte-stop` uses `make_memtag_stop_fixture.py` to compile `b1_memtag_stop.wat` and
replace exactly one complete marker function with the actual WasmMTE encoding.
Select a real MTE backend, sync/async mode and `silent-log path FILE`.
The main worker accesses a live tag-three allocation with a tag-zero pointer.
The host holds the real diagnostic sink mutex during one instruction advance.
No coherent stopped row may be exposed until the report finishes; after releasing
the sink, the same transfer must complete with exactly one additional report.
This tests real worker execution, enqueueing and reporting, including an
asynchronous report that cannot be acknowledged yet. It does not fake a pause
participant or inject a fabricated fault record.

`race` attempts real concurrent resume/stop with an independent joined native
stopper. It does not prove that the precise early-token scheduling window ran.

All actual work must use the coordinated SSH Linux window in the existing
64 GiB, zero-swap cgroup. Admit the current boot, membership and inode; use an
owned external wall deadline, bounded RSS, genuine pid/process identity and
physical descendant reclamation. A timeout is a failure, never a stopped or
joined result. Elapsed checks after a call returns cannot detect its hang.
The void public drain can refuse other management contention; verify its actual
no-contention preconditions and retirement instead of treating every return as
a universal completed-drain receipt.

These cases do not cover the whole wait64/FIFO/deadline/spawn/compiler/TLS/ROS2
matrix or language source debugging. Ordinary threaded CLI DBG/checkpoint gates
stay closed. Qualify x86_64 and AArch64 separately, then the additional required
QEMU platforms. Measure default-none, observation and enabled idle/armed control
with actual VTune hardware Hotspots and Microarchitecture Exploration; none of
those execution or performance results is supplied by these sources.
