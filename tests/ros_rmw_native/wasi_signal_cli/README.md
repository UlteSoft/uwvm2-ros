# Explicit standalone CLI signal delivery, reimplementation source 1

This package is authored source and has not been imported, configured, compiled,
linked, generated, validated or run. It is a new implementation based on the
actual current public Source. The previously unintegrated private CLI draft was
lost with the old temporary scratch; its bytes and SHA were not recovered and
are not reused here. The public Host14 pipe/sender/lifecycle implementation is
an actual dependency, not a replacement or a new claimed runtime qualification.

## Admission and default behavior

The new Linux option is available only in the actual threaded Runtime feature
build:

```
--wasi-thread-signals <read-fd> <cookie> <max-frames> <max-pipe-bytes> <grace-ms> <retire-ms>
```

All six values must be positive decimal integers without signs/trailing text.
The read descriptor is 3..INT32_MAX, the cookie is 1..UINT64_MAX, the frame
budget is 1..1048576, and the pipe budget is 32..1048576 bytes. Both time budgets
are 1..60000 milliseconds. Actual pipe capacity must independently fit the
selected Host14/child-storage quotas; a syntactically legal32-byte budget does
not promise that Linux can create such a small pipe. Insufficient actual
capacity is a startup refusal, never an implicit larger grant.

The option requires the separate complete `--wasi-threads` policy and the
existing full-compile ordinary run mode. All remaining options are parsed before
any signal disposition changes. The callback publishes only actual cold Host
policy data after all six fields pass; it installs no handler. Zero defaults
install no handler, pipe, descriptor, clock, ROS alias or management capability.
This adds no Guest imports or ABI tuples. It does not alter any Runtime code,
NodeOptions, SDK defaults, cache behavior or ThreadedSpawnAdmission authority.

This is a standalone CLI profile, not an embedding API. The actual SIGINT and
SIGTERM dispositions must both be SIG_DFL; a debugger, existing custom handler
or inherited SIG_IGN causes a real refusal before Guest launch. The real Native
thread mask is queried before installation; blocked SIGINT/SIGTERM is refused
without implicitly unblocking the caller. pthread_sigmask returns its own error
code, which is retained directly instead of being misread from errno. Both queried
states and each install/restore result have separate diagnostics. The two
installs are transactional: failure restores every already installed handler;
a rollback failure contains the process instead of claiming successful cleanup.
The original dispositions are restored only after actual physical Native join
and actual Source profile retirement. Foreign replacement is detected during
restoration and is not silently overwritten.

## Signal thread and Source ownership

The raw SA_SIGINFO handler only reads/writes compile-time-proven lock-free
atomic unsigned state and preserves errno. It never obtains a sender, invokes
Runtime/ROS/Source APIs, allocates, prints, waits, locks or writes the pipe.
Real kernel terminal SIGINT and actual external same-effective-UID SIGINT or
SIGTERM notifications are accepted. Signals raised by this process itself or
an unaccepted origin retain real default-signal behavior rather than being
passed off as Root control. Restoring/defaulting a signal uses actual POSIX
signal operations; this is not a generic POSIX emulation for a Guest.

The notification mailbox has two pending event bits. Repeated standard signals
can coalesce; duplicate pending bits latch a saturation diagnostic. No exact
kernel signal count or order is invented. The ordinary CLI thread observes the
bits, selects terminate before interrupt when both are pending, obtains the
actual typed sender once, and uses the real Host14 push API. The bounded
nonblocking write may return EAGAIN or actual maintenance EBUSY before effects;
the pending bit is retained for a later ordinary-thread pump. Other push errors
request actual stop/retirement. Partial/ambiguous Host writes retain Host14's
real poison/stop behavior and are not retried as a successful frame.

No Source token, generation, boolean or public cookie can mint authority. The
actual sender internally binds the real ThreadedSpawnAdmission control block
and Source generation. It is never replaced by a new sender after acquisition.
This standalone process permits only one OS-producer installation attempt,
with a process-lived POD mailbox; no later Source can consume an earlier
installation's notifications. Actual joined/retired transitions disable and
clear the mailbox. Root's existing Host14 stale-sender contracts remain the
independent proof that a retained old sender cannot address a new Source even
with the same cookie. This CLI does not add a second sender issuance path.

## Timing, stop and containment

Cold loader/source preparation is not interrupted or timed by this feature.
The handler can record a notification during preparation, and the ordinary
thread checks it before asynchronous initial-main admission. A prelaunch
notification skips launch and follows actual wait/reset cleanup. An external
Root whole-command deadline is still required for cold parsing/compilation or
an underlying native operation that has not returned to the CLI. The grace
clock begins at the first ordinary-thread observation, not a fabricated kernel
arrival timestamp.

All waits use steady_clock absolute time points checked for arithmetic overflow.
At most100ms normal wait slices let the ordinary thread collect notifications;
the slice count is never treated as elapsed time or as successful retirement.
The single grace deadline is captured once and is not extended by repeated
signals. Grace expiry invokes the actual stop-only retirement API, preserving
all real Native/compiler/cleanup pending counts. A separate single retirement
deadline is captured at this transition and is not extended by retries. Actual
complete retirement requires physically_joined, native_error0 and zero actual
preparing/native/compiler/cleanup owners. A body return or cancellation request
alone does not meet that predicate.

Only after real Native join does the existing full Source reset phase run. It
has its own one-time absolute retirement budget; repeated reset/status checks
do not restart that budget. Return requires the real Runtime profile result to
be disabled. The controller's lexical order precedes the ROS cleanup destructor
on unwind, so an unexpected exit with potentially live Native/Source owners
contains the process before ordinary owner destructors could unload them.

If physical or Source retirement remains pending at its deadline, this explicit
standalone CLI uses actual `_exit(125)`. That is process containment and bypasses
C++/Runtime global destruction; it is not successful physical join, Source
completion, ROS cleanup or permission to reuse storage. In this terminal phase
only, stderr must first be an actual pipe or socket (regular files are skipped because
O_NONBLOCK does not bound their write latency). Current-thread SIGPIPE is blocked
and that actual OFD is made nonblocking before best-effort fast_io diagnostics. A broken/full/unusable
stderr may prevent a diagnostic; it never replaces or postpones the final
actual process exit. A parent retaining the same stderr OFD must account for
that terminal nonblocking flag change. Normal completion leaves OFD flags
unchanged. Root's external process/cgroup owner must observe whole-process
retirement and retain raw output/error facts; the library embedding API gains
no process-exit contract.

## Source contracts and remaining qualification

`policy_contract.cc` contains19 pure failure checkpoints for actual parse/derive,
output atomicity, arithmetic and the actual retirement predicate. Attach it to
a genuine Runtime executable; it intentionally provides no fake main or
provider and is not automatically executed. `NativeProbe.cmake` supplies a
fresh-cache executable compile-and-link probe for actual lock-free atomics,
POSIX signal APIs, fd flags and pthread signal masks. The probe refuses explicit
unresolved-symbol permissive tokens in its visible flag/library inputs. Actual
producer flags, MD/link/import inventory and behavior remain independent gates.
Nothing here claims a catch-all link-flag validator or an installed tool result.

The two WAT command fixtures import a genuine separate shared memory, because
the current actual CLI thread limits forbid locally defined memory. They export
actual wasi_thread_start and a real zero-result `_start`. The positive command
writes an actual Guest readiness line, then performs real FD_READ poll_oneoff
before its one32-byte read. It validates the actual48-byte subscription and
32-byte event/nevents layout, userdata, errno, type, flags and nbytes before
reading URS1/version/extent/cookie/sequence/event/reserved fields. This avoids
assuming that asynchronous main starts after the Host push. Failures use real
proc_exit, not a discarded Wasm return value. The forced-stop command writes
readiness and deliberately remains in actual Wasm code without consuming a
frame; it exercises actual Group cancellation, not an invented pending Native
owner.

`os_contract.py` is an unexecuted Root-only Linux harness source. It requires
complete Root-selected actual argv plus file hashes and explicit finite harness
deadlines. It supplies no grants, compiler outputs or forecast identities. It
uses private nonblocking stdout/stderr pipes, waits for a real complete Guest
readiness line, and sends the real external OS signal with kill(). A harness
SIGKILL/deadline is a failure and cannot pass a graceful case. It records bounded
raw output and refuses to claim Native physical join. The actual binary and
Wasm copies must remain Root-owned immutable inputs through execution; static
hash admission is not an open-file executable/loader lease. Root must separately
verify actual execution identities and terminal rehashes in its real harness.

Still required: actual producer/link/probe/MD and full import checks; invalid
CLI/default-off/nondefault-disposition/partial-install/restoration failure cases;
actual positive SIGINT and SIGTERM command cases; coalescing and stale epoch
cases; genuine grace cancellation; a real retained Native/Source-owner fault
case for deadline125 with external whole-process retirement; actual SDK33 and
original rclcpp/ROS/default-endpoint composition. No native pending owner is
faked merely to make the125 branch appear tested. All new executions remain0.

## Scanner successor and actual named-module boundary

Source revision2 delegates positive decimal conversion to the actual vendored
fast_io parse_by_scan/dec_get scanner. It first checks a real ASCII digit,
requires complete input consumption and keeps a local uint64 temporary until
success. noskipws=true and skipzero=true retain revision1's valid positive
leading-zero spelling while rejecting whitespace, signs, radix prefixes,
suffixes, zero and overflow. The actual UINT64_MAX cookie remains accepted.
This preserves the old admitted numeric domain without a duplicate manual
conversion loop. The pure source contract adds whitespace/radix negatives and
a positive leading-zero regression; its19 failure checkpoints remain unrun.

The actual existing threaded Host API is guarded by !defined(UWVM_MODULE).
The real params/callback partition graphs do not expose the existing wasi_threads
interface, and named-module imports do not export preprocessor macros. This
cohort therefore admits only the existing Linux nonmodule/header Runtime
profile. It neither claims the threaded named-module CLI works nor tries to
create duplicate module-owned Runtime/container types through textual includes.
The existing module source/partition graph is unchanged. Full genuine module
support requires a separate coherent Runtime API/export, global native-header,
params/callback partitions and entry/registration composition with real module
producer acceptance; it remains required work, not a silently supported path.

The revision2 controller also defers its actual Native-thread identity read until
a positive selected prepare. Ordinary default-OFF runs retain zero controller
data and execute no new get_id/pthread_self operation. This is a source fact,
not a measured performance result.
