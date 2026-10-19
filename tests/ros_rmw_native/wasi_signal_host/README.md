# Private Source-owned WASI signal Host counterpart

This source-only candidate installs a real Linux nonblocking pipe in the actual
selected WASI Preview1 environment before any threaded compiler/initial-main
submission. It completes the previously explicit Host-map requirement of the
separately peer-closed original-rclcpp Signal platform. It has not been compiled,
linked, run, or combined with an actual SDK33 producer. No Runtime, ROS, libc++,
GraphListener, or original default-Node qualification follows from this source.

## Authority and association

The all-zero `threaded_signal_policy` is OFF. Only an explicit Native Root caller
may configure positive fd/cookie/frame/pipe-storage bounds through the new C++
Host API. There is no Guest import, numeric management namespace, CLI switch,
environment-variable grant, ambient clock permission, or POSIX signal shim.
Actual CLI policy selection and OS-notification sourcing remain separate work.
An ordinary Native thread may call the sender API; a raw async signal handler
must not call it because mutexes, shared owners and Runtime admission are used.

Root selects fd >=3. Both genuine `pipe2` endpoints are O_NONBLOCK|O_CLOEXEC;
only the read endpoint is inserted, with exactly fd_read|poll_fd_readwrite base
rights and zero inheriting rights. The Native writer remains unmapped. No Guest
filesystem path or proc mount is introduced. The receiver must receive Root's
explicit fd/cookie through its existing trusted preparation API, not stdin or
untrusted input. Cookie is public frame association, never authority.

The private constructor of `threaded_signal_sender` captures the Runtime's real
canonical ThreadedSpawnAdmission and generation. Each push checks that exact
sender object, Source admission pointer AND shared-owner control block, current
Source and real private-generation lease. Reusing the same cookie in a later
Source cannot revive an old sender. Native callers cannot mint this owner from
a boolean, generation integer, caller-created shared_ptr, or DATA report.

## Descriptor stability and import admission

Read rights alone cannot protect a WASI slot: fd_close and fd_renumber need no
separate close right. Before pipe creation this candidate traverses the actual
Source's complete imported-function table and accepts only genuine already
admitted ROS/thread builtins and the existing strict WASI leaf set:
args_sizes_get, args_get, environ_sizes_get, environ_get, clock_time_get,
clock_res_get, sched_yield, fd_read, fd_write, poll_oneoff; proc_exit retains its
existing independently authenticated Group route. No fd_close, fd_renumber,
fd_fdstat_set_rights, path open, arbitrary C Host provider, or mutation fallback
is added. The existing interpreter/LLVM complete signature/import census still
runs independently before Guest entry and applies to the whole child World.

This is a finite first producer profile. An actual libc++/WASI producer importing
additional operations is refused until an independently reviewed real policy
exists. This candidate does not claim that a historical SDK24 producer or the
new SDK33 full original-rclcpp producer already satisfies that census.

## Allocation, errors, and lifetime

The actual cold Source/public-admission gate is held before retaining the
selected environment. The existing environment is a process-owned global or a
stable deque-held module override. No selected environment or fd map is cloned.
All sender/owner/map-node/descriptor/reference storage is acquired before
pipe2; no compiler or Guest actor exists at that point. The real Native pipe
capacity is read using the same Linux F_GETPIPE_SZ operation as the existing
threaded WASI fd pin. Capacity plus a conservative 4096-byte fixed-owner
reservation is charged to the actual Group control-storage budget. This is not
an exact allocator/kernel RSS claim; real outer resource accounting is still
required. A pipe whose capacity exceeds the explicit maximum is rolled back.

Map installation/removal takes the actual fd table's writer exclusion. A
placeholder is never Guest-visible before admission. Removal verifies the exact
map cell AND reference-counted OFD identity and the real descriptor handshake.
The private read ref survives map removal until all actual Guest fd pins are
gone. Close uses release-before-close and records independent read/write errno;
a failed close is never replayed against a possibly reused fd. Native close
failure fails retirement qualification. The report retains first/last errors,
last actual write errno and partial progress, attempts, frame/sequence counts,
and both close results. It does not claim per-syscall historical journaling.

The pipe owner is retained by the real profile_state, not by the sender. Startup
rollback retires the exact map before releasing admission. Every launched-path
retirement waits for genuine Group compiler/initial-main/child physical joins;
pending or failed joins preserve the map, pipe, Source and provider owners.
After physical join, the Native pipe mutex serializes cleanup with an already
admitted push. A still-busy cleanup remains pending and preserves owners. Only
real completed pipe retirement precedes `state->admitted.reset()`.

The existing process lifetime_guard calls actual threaded process_shutdown
before namespace-static import caches/environment teardown; the runtime-global
destructor repeats that real drain before Source/provider destruction. Selected
override storage is append-only/stable during admitted execution. Root must not
mutate WASI configuration outside its actual cold/configuration contract. No
caller token extends an unrelated environment's lifetime.

## Sending and stopping

The owner serializes one 32-byte URS1 frame per syscall. 32 <= Linux PIPE_BUF;
the known O_NONBLOCK pipe therefore either accepts the whole record or reports
zero-progress EAGAIN. EAGAIN consumes an actual write attempt, but no frame or
sequence, and may be retried only by a new explicit Root call. The helper never
performs write-all, loops, or replays an ambiguous effect. A short or other
failed write poisons the owner and requests stop on the same actual Group. A
frame/sequence/attempt bound exhaustion is also terminal. Actual Group forced
stop/reset is independent of the Guest receiver and cannot be blocked by a
Guest handler. The helper never turns a terminal Native effect into an ordinary
Guest errno that permits execution to continue.

Push obtains the real maintenance mutex and Source-generation lease, then
releases registry/maintenance locks BEFORE its bounded syscall. This is the live
maintenance exclusion, not the cold-only management gate that would reject all
legitimate initial-main execution. Root retirement obtains the pipe owner's
short mutex and verifies physical join before removing that same descriptor.

## Private source composition

Two actual existing headers have complete before/after copies: the Native API
and threaded lifecycle. Four new helper headers hold the implementation. Runtime
is supplied ONLY as `Runtime-includes-only.patch`, two include insertions and a
virtual after hash. There is deliberately no whole Runtime after artifact.
Root must compose these two hunks with HLL's actual cache/source changes against
their exact current preimages; this package does not own cache/factory/LLVM code.
The ordinary unthreaded execution path gains no signal branch or syscall.

The fifth API, signal_status_host_api, copies current/last bounded diagnostics
for failed startup or completed retirement. It is observation only; zero DATA
does not mean installed/qualified. Root archives an epoch's diagnostics before
starting another Source. An old sender is rejected after replacement, including
for observation, while its same-epoch retired report remains readable.

## Contracts and remaining real qualification

The new tests subtree contains two-source stale-sender contracts and real WAT
fixtures. See RECIPE.md for the required actual loader/producer/driver. The
positive fixture uses one actual poll_oneoff FD_READ subscription before the nonblocking read;
the 48-byte subscription and 32-byte event layouts are checked against the
repository's ABI-compatible structs and fixed SDK33 wasi-libc header. It verifies
the actual return code, one event, userdata, errno, type, flags, and advisory
readable byte count before independently checking fd_read and the full frame.
Only the trusted read fd's read|poll rights are required: there is no clock
subscription, clock grant, sleep, counted polling or fabricated ready event.
The dedicated fixture has one reader. Actual forced Group cancellation remains
the independent escape from its real blocking poll if Root never sends.
This subtree contains no fake Runtime, Source, admission, Group, pthread, WASI header or test
success main. Only Root may compile/invoke it in a coherent batch. Required
pending cases include real pending physical join retention, startup allocation/
pipe/fcntl/close failures, actual multi-Root push/retire races, full rclcpp
receiver/deferred callbacks, complete default endpoints, and actual SDK33
threads+EH import/ABI/whole-memory qualification. A source review cannot award
these outcomes. Historical SDK24 and prior ROS records remain unchanged.
