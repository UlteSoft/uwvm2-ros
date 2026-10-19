# Linux IPC cancellation contracts

All source here is authored and UNRUN. There was no candidate import, configure,
compiler, probe, test, Runtime, Native ROS, SSH or repository write by the author.
The actual executable source uses Linux eventfd, nonblocking socketpair, poll,
real send/recv and physical joined std::thread handles. It is not a fake Runtime
or native ROS provider. No test redefines the true private bridge to access its
friend constructor; compile-time traits verify that int/copy/move construction
is unavailable to ordinary code. Default no-scope getters remain ordinary -1.

Twelve named groups cover ordinary no-scope transfer, eventfd staying readable
and open, -2 refusal, invalid positive fd refusal, zero extent stop, read wait,
one level event waking two actual waiters, a real kernel-readable socket prefix
before cancellation, stopped retry without send, cancelled connection readiness,
ordinary readiness and pre-frame stop preserving the initial private frame.
The partial-write group waits for actual peer POLLIN before writing stop; it
never treats a false transfer as evidence of zero send. The read-wait races are
valid whether stop arrives just before poll or while blocked; they do not claim
a scheduler-observed syscall entry that was not measured. Eventfd owners outlive
all joined workers. Test-only owner destructors close their OWN fds; Host code
never closes or reads the borrowed stopfd.

A separately authorized executor may configure with the actual complete reviewed
`UWVM_SOURCE_ROOT`, build `ipc-native-cancellation`, then run it in one fresh
owned finite Linux process/cgroup with bounded output and a whole timeout that
covers setup/retirement. Each worker transfer has a real two-second deadline;
this does not make arbitrary native mutex/TLS failures interruptible. The actual
owner must physically drain all threads/processes on failure and preserve logs,
return/marker/check count, actual compiler/MD/link/native target evidence. CMake
contains no automatic test execution or source/network download. The expected
marker names groups=12 and the actual executed positive check count; neither
source method count nor sampled RSS is an execution/performance claim.

Scope nesting/rejection through the REAL Runtime bridge and complete connect/
hello/main RPC cancellation are mandatory separate integration tests. This lower
transport suite cannot instantiate the private constructor, fabricate World or
caller identity, or award those tests. Full threaded bridge/source/memory/
management retirement remains independently admitted and genuinely executed work.
