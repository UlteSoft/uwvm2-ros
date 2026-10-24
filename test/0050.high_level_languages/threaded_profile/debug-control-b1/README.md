# B1 cooperative debug-control qualification source

These four source files have not been compiled, converted or run. They use the
actual source-owned Runtime embedding entry, canonical imported shared memory,
native workers and public debug/retirement APIs. No test constructs a fake
Group, admission, provider or pause participant.

Read the [native control API guide](../../../../sdk/origin/docs/threaded-debug-control.md)
and the [existing profile qualification](../qualification-r3/README.md). Build
this main with the final Runtime's actual object/header/feature/link closure;
do not link it alongside the ordinary CLI main. Compile both WAT files with the
admitted real tool on SSH Linux, then supply the memory provider through the
normal validated profile loader. Keep the final Wasm, executable, linker map,
source/toolchain identities and explicit finite profile limits.

The embedding's argument shape is `none|control|race int|llvm --` followed by
the actual ordinary finite threaded options and main Wasm selection. This is
an argument contract, not a complete executable recipe or a successful build.
`none` requires control refusal and physical join. `control` needs a real linked
wait plus a looping worker, 100 real stopped episodes, stable participant and
incarnation identities, real terminal cancellation, no post-wait continuation,
physical join and an issued stale guard retained through public stop/drain.
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
