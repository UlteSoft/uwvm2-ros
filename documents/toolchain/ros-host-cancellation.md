# Trusted worker cancellation for ROS Host IPC

This is a Native-only source interface, not a Guest ABI or grant. Ordinary calls
have no scope and preserve the existing -1 cancellation descriptor behavior.
No ROS numeric import, capability, policy preset, graph access, clock or logger
is added. It has not been compiled, tested or executed by the source author.

The sole scope constructor friend is the actual
`uwvm2::runtime::lib::runtime_threaded_profile_bridge`. The bridge first verifies
the real ChildLease/native thread, current canonical source/generation/World,
actual selected builtin and complete numeric ABI sizes. Only then may it create
`ipc::cancellation_scope{lease.cancellation_fd()}` around the genuine Host module
call. A rejected scope must refuse business and stop the group for a native
invariant failure. Constructor/TLS state are private; no public adapter, test
backdoor, Guest setter or fd import exists. The Runtime bridge is separately
owned source and must be independently admitted; a forward friend declaration
is not evidence that the whole threaded profile is runnable or integrated.

The actual lease supplies a borrowed Linux eventfd with EFD_NONBLOCK and
EFD_CLOEXEC. The controller publishes the stop payload/atomics before writing it.
It stays readable for every waiter: no Host code reads, drains, duplicates or
closes it. The scope and each synchronous call borrow it only until return. Its
owner must retain it until every physical worker/call, TLS destructor and actual
native thread handle has returned and joined. A body-return flag, deadline or
logical cancellation is not a physical join. Pending workers keep reachable
World/code/provider/group owners. There is no detached cleanup or false ACK.

Nested -1 scopes inherit the actual parent. A same-fd nesting is permitted. A
conflicting live fd, an invalid negative value or already rejected parent
publishes private TLS -2. `valid()` is false and IPC admission explicitly treats
that sentinel as cancellation refusal before poll; it cannot be ignored as a
negative pollfd. Nested scopes cannot hide a parent cancellation by selecting
-1 or another live descriptor. Destruction restores the lexical prior state.
Integer fd equality is not independent ownership proof; the true lease remains
mandatory. Zero-wait probe errors/POLLNVAL fail closed and are separately labelled
as rejection/probe failure rather than fabricated evidence of a stop payload.

## Actual cancellable phases

`client::connect` captures the current borrowed fd. Its one absolute entry budget
includes its real mutex wait and setup. It checks cancellation before socket
work, around connection readiness/credentials, and before/after hello. The
nonblocking connect poll watches both socket and stopfd. Hello header/body reads
and writes pass the same stopfd through actual transfer calls. The original
peer credential, response correlation, backend, nonzero session, empty hello
body and error checks remain mandatory. An unvalidated reply session is not
recorded as an accepted owned session.

`client::call` likewise captures/passes the same fd through request write and
response read, with pre/post checks. It never reconnects/replays. Once a request
write can have started, a false `write_frame` is not a zero-send proof: a prefix or
whole frame might already have reached the peer. Cancellation closes the OLD
socket/session, returns125, and keeps a fixed Native diagnostic with possible
peer effects, first/last accepted old sessions, saturating count, phase and cause.
A descriptor close does not prove rollback, Native fini, ROS cancellation or
completion. Independent Host transaction/cancel/commit/cleanup journals remain
separate. Non-cancellation I/O keeps the real timeout110/disconnected107 rules.
No `call_result` layout or wire format is changed by the Native diagnostics.

Each transfer checks cancellation even for a zero body, before waiting, after a
physical prefix and after completion. The stopfd has priority when both watched
fds are ready. No fd is drained. Frame buffers remain private owned data until
the ordinary complete protocol/correlation checks succeed. Invalid descriptors
or interrupted metadata probes can refuse admission; those facts are not called
an actual controller stop or successful Native cancellation.

## Physical stop and management

The real client mutex, Host backend mutex and capability-manager mutex remain
ordinary locks. They are not made interruptible by an eventfd. Group stop must
first wake the held RPC, then genuinely drain/join all physical workers before
retiring storage or the borrowed eventfd. Uncontrolled Native blocking, OS mutex
failure, fatal allocation/trap or pending join cannot become a safe-retirement
claim; the controller must retain owners and fail closed.

The real threaded bridge can stop an atomic-byte shared-memory copy after a
partial prefix. That terminal group stop is not an ordinary returned Guest errno
with successful output. Its post-call stop path must prevent resumed Guest use.
Dirty shared memory from an ever-launched profile cannot be resumed merely after
code replacement/join: incremental mutation must refuse until a true complete
old-source/shared-memory destruction, domain reset and new generation. Source
publication guards must not join workers while holding a lock they require.
Actual cold management integration belongs to a separately reviewed cohort.

## Required execution evidence

The authored Linux contracts exercise actual eventfd/socketpair/poll transfer
primitives, including level stop for two waiters and a physically observed partial
socket write. They never define a fake trusted bridge, inject a scope setter,
produce a fake ROS provider or claim full Runtime/native acceptance. The actual
bridge/World/source admission, same-fd/conflicting-scope and main-module
connect/hello/call stop cases need genuine integrated tests with physical thread
join/source retention evidence. Preserve all outputs, failures and separate
unknown effects. No group-spawn/update/debug/restore capability is inferred.
