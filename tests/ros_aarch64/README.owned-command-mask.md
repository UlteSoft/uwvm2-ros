# Owned command signal inheritance and the eleven-case current-source gate

`OwnedCommand.start` must retain its actual direct Popen before ordinary
TERM/INT cancellation can leave launch bookkeeping. Blocking those signals
briefly in the parent solves that ordering problem, but the former direct
Popen also inherited that temporary mask. Restoring only the parent's mask
left target TERM/INT blocked. Eventual pidfd KILL and reaping were useful
ownership evidence and did not establish graceful signal semantics.

The new Linux-only launch path keeps that parent critical section. It invokes
the actual currently executing physical native Python through `-I -B -S -c`,
then execs the original target in that same PID. There is no preexec_fn,
additional process, shell, process group signalling or guessed numeric PID.
Caller-owned `cwd`, stdio, session/group/user/umask and original passed FDs
remain Popen options applied once. The original target argv and executable
bytes, original byte environment, original caller mask and target ignored
signal dispositions are placed in a fresh anonymous memfd. Write/grow/shrink/
seal locks make its original bytes immutable; the bootstrap reads the same
inherited descriptor and full SHA. It closes that extra descriptor before the
target exec, so it does not silently grant a new target FD. A genuinely open
`close_fds=False` call retains its original behavior; ordinary closed-FD calls
pass only the original selected FDs plus the temporary control. Invalid
original passed FDs are rejected before allocating that control.

Python can alter locale environment during startup and sets its own signal
handlers. Therefore the bootstrap never treats its current os.environ as the
requested target environment. It uses the exact original byte mapping from
the sealed control. It reproduces exec's reset-of-caught/preserve-of-ignored
signal dispositions, including the original Popen restore_signals choice,
then restores and rereads the original mask before execvpe. Its actual native
Python identity and bytes are verified through /proc/self/exe and the actual
selected full7. It checks the same live direct parent and has a finite ten
second bootstrap deadline. The original controller RSS/whole command timeout,
64 GiB leaf/swap0, actual pidfd ownership and cleanup remain independently
required. Caller source/SDK/tool inventories are not relaxed.

Shell=True and preexec_fn are explicitly refused by this direct-exec API.
The maintained lease and rebuild call sites use neither. Target arguments and
FDs must be literal/valid, and the serialized original launch data is capped
at8MiB. A bad/nonexistent target now fails inside the already owned bootstrap
and is reported as a failing command; it must never be mistaken for successful
target execution. The normal supported target argv/environment/cwd/session/
stdio/exec behavior is preserved. Existing execute=False or host metadata
facts cannot authorize any target or rights.

`owner.launch` records the actual prepared control/Python/bootstrap source
and later the retained Popen PID. The anonymous memfd record is explicitly
not a pathname full7, and PREPARED_NOT_STARTED does not imply a child exists.
The initial process can still be the host Python bootstrap. It is never
reported as an ARM target mapping or a stopped/ready target. Actual downstream
ELF/map sampling must independently observe the eventual real target and its
same birth/cgroup, as before. Both maintained lease and rebuild reports retain
this launch record while preserving the original requested command. No source,
SDK, cgroup, binfmt registry, mount or QEMU identity is relabelled.

The maintained test factory now has exactly eleven actual Linux tests. The
original ten bodies remain unchanged. The new case deliberately gives the
parent a nonempty mask with USR1 blocked and TERM/INT unblocked, starts the real
original Python target with an explicit environment marker and cwd, then
waits for that target's real stdout readiness. It checks the actual child mask,
original target argv in /proc/cmdline, target cwd/environment marker, real
/proc/exe physical identity and same retained birth. It sends TERM through
that retained pidfd, requires actual -SIGTERM returncode, and verifies no KILL
was needed plus original subreaper/children cleanup. The caller's own mask is
restored in a finally path. This is a real eleventh process test, not a model
or a source-only graceful-TERM result.

Before any actual binfmt lease, Root requires a new actual whole-PASS
current-source eleven-case execution using this controller and fixture.
Historical Source63 ten-case results remain historical; they cannot qualify
this changed controller or the missing graceful-TERM condition. The external
actual runner must update its exact11 inventory and ten direct-Popen plus one
self-prctl accounting, narrowly admit only the actual current bootstrap argv/
sealed control through this registered controller, and preserve normal
Frame.finish outside the bounded test phase. It must publish the actual new
mask/TERM case receipt and full current MF/source/inputs/roots/cleanup facts.
No future MF, PID, output identity or PASS is supplied by source authoring.

## Ordinary cancellation while preparing the sealed control

The separately preserved first source candidate had a TERM/INT window between
returning its newly sealed integer FD and entering the parent's masked cleanup
try. A Python cancellation handler there could leave that FD unclosed. This
successor sets control=None, captures and blocks TERM/INT, then immediately
enters the try before preparation can allocate a descriptor. It passes that
actual original mask to the same control/bootstrap protocol. The finally closes
only a returned non-None control and restores the exact original parent mask.
The helper still owns and closes any descriptor while it has not returned.

Ordinary cancellation is therefore deferred across bounded preparation as well
as activation/Popen retention. This includes hashing the actual selected Python
and a control bounded to8MiB, under the unchanged live guard. The hard SIGALRM
watchdog is never masked or extended. Process death or hard-deadline interruption
is not upgraded to guaranteed graceful cleanup. Target argv, environment,
bootstrap bytes, exact original child mask and all lifecycle algorithms stay
unchanged. The eleventh regression and its actual gate also remain unchanged.
