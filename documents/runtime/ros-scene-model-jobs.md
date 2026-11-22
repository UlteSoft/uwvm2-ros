# Bounded model jobs and sealed inputs

`scene/model_jobs.h` supplies a real asynchronous job registry for an approved
native worker. `scene/model_contract.h` encodes the catalogue's existing
`model.open`, `model.submit`, `model.take`, `model.cancel`, `model.describe`,
`request.describe` and `request.release` records. These files do not register
imports, attach an inference engine, authorize remote export, or execute robot
commands. The identity kernel in the core test proves data and job lifetimes;
it is not an LLM, vision or world-model integration claim.

## Deployment and authority

A trusted deployment binds exact model/task profile names and SHA256 digests,
parameter/output message types, encoding and bounded schema validators. Model
text cannot choose a plugin, network destination, credential, tool grant or
validator. A binding remains disabled until the host attaches a nonzero fresh
worker generation. Reconnection requires a strictly larger generation; existing
sessions cannot submit into a replacement generation.

All sessions, requests, retained input bytes/count, parameter bytes, output
bytes/items, workspace and deadline capacities default to zero. They are finite
reservations, independent of optional action rate limiting. `open` also obeys
the authority grant's resource capacity. `reserve_workspace` charges heap input
carriers before allocation; the carrier and copied job parameters coexist and
therefore consume separate parts of the same workspace budget. The token must
be declared before its owned allocation, so memory is freed before refund.

The VM adapter must admit the origin model action and every exact input
`buffer.read` capability atomically, holding that group lease through pinning
and job admission. It must resolve source capabilities in the caller's actual
environment rather than trusting aliases in a request. Before a native worker
uses input or publishes derived output, the adapter must reacquire and check
the current origin/source authority without charging a second submission.
Source registry retirement is additionally checked by real sealed leases and
before new output publication. Immutable cached data does not preserve revoked
permission. Authority changes must retire/fence the corresponding provider
scope; the core metadata alone does not prove a live authority lease.

Remote execution requires independently granted destination-specific data export
for each input and current destination/profile/provenance checks. The current
authority catalogue has no implemented export provider, so a deployment using
this local core must not attach a remote worker. A model grant never implicitly
authorizes arbitrary sockets or HTTP.

## Canonical requests and provenance

The codec preserves the catalogue field order and bounded canonical LE encoding.
Profiles are a UTF-8 identity and exactly 32 digest bytes. Submission contains
the owned session key, exact task, typed/encoded parameter body, at most 64
observations, output/workspace capacity and an absolute steady deadline. All
truncations, trailing bytes, invalid UTF-8 and noncanonical optional flags fail.
The codec bounds carrier copying; it does not itself grant authority or prove
provenance. The host must also compare parameter type and encoding to the plan.

Current guest-created buffers have untrusted origin. Their observations must use
`source=0`, `source_generation=0`, absent stamp and absent frame. They cannot
claim a sensor identity, calibration, transform, capture time or source epoch.
A later trusted sensor publication path must bind these metadata to the actual
sealed allocation. The model core's real pin prevents mutation/freeing of bytes,
but does not create trusted sensor provenance.

`probe_retry` precedes new input pins. It recovers the last exact admission
receipt by SHA256 of the **entire canonical request**, including capacity,
deadline, source references and parameter metadata. Hashing only the parameter
body would permit conflicting retries and is forbidden. Every session accepts
the next sequence only. An exact retry returns historical admission data without
submitting another job; the next successful submission acknowledges the previous
receipt. Replay still requires current origin permission and grants no fresh
source access or execution authority. A failed reply copy is recovered by exact
retry, rather than by undoing an already admitted job.

## Native worker and cleanup lifecycle

`claim` transfers one queued job into running state and returns a move-only work
token. Input access visits actual sealed-buffer leases; guest close cannot free
their storage or prematurely refund buffer object/byte capacity. Parameter and
input visitors execute outside registry/job locks. Visitors may not retain
borrowed spans. An asynchronous native worker retains its token and borrows for
each use, or explicitly retains another separately charged allocation.

The job also retains at most 64 fixed host-only input identities and SHA256
digests after cleanup. `check_inputs` compares the exact count, order and bytes
identity against a successfully completed request before proposal publication.
This evidence cannot reopen a released buffer or grant source authority. A
proposal adapter separately holds its immutable source snapshot tokens, so a
new sensor sample cannot be mislabeled as the input of an older model result.

`emit` appends validated immutable prediction chunks. `finish` acknowledges that
all actual native users have stopped, optionally appending a terminal result.
Output bytes/items are cumulative for the request: consuming a chunk does not
refill the emission allowance. Allocator failure does not publish a partial
chunk or advance its sequence. Outputs remain predictions and carry no target
authority. Native timestamps/publisher identities unavailable to this local
provider remain absent in delivery metadata.

Cancellation of a queued job confirms local cleanup immediately. Cancellation
or timeout of running work records intent and retains its inputs/reservations
until the worker confirms cleanup. Dropping an unfinished work token or losing
the provider records `outcome_unknown`, not a fabricated successful stop. A
trusted supervisor can `reconcile` only after independently proving cleanup for
the exact provider generation. No automatic retry follows an unknown outcome.
Deadlines are checked on operations; a deployment needing autonomous timeout
enforcement must supply a supervisor that cancels and joins its native worker.

`take` reserves one delivery, releases locks during checked output copying, and
consumes the chunk only after a successful sink. Reentrant/concurrent takes
return `again`; failed or throwing sinks preserve it. Revocation or session close
during copying prevents consumption. Terminal receipts without a chunk remain
readable. `request.release` requires a terminal result, confirmed cleanup and no
delivery in progress. The last worker token can still retain request/session
capacity after the registry entry is released. This reflects actual lifetime.

Retirement must be followed by native worker cancellation, join and settlement
before environment destruction. Destroying local metadata is not evidence that
a remote provider or physical system stopped. Debug inspection is a nonblocking
alias aggregate of session/request/input/output/workspace reservations, without
payload, digest, retry keys, pointers, provider I/O or deadline advancement.

## Verification scope

`tools/ros_scene/model_job_checks.py` archives source plus dependency text before
compilation and records workspace/snapshot hashes, gate inode, boot identity,
memory/swap limits and cgroup events. All compilers and executions run inside the
existing 64 GiB, zero-swap cgroup on CPUs 12–15. Qualification covers Clang
ASan/UBSan, GCC, TSan and AArch64, ARM32 and i686 execution.

The ten test groups cover zero/default authority and capacity, exact profiles,
real sealed inputs, native data computation, exact request recovery, deadlines,
cancel intent versus cleanup, unknown outcomes, provider/source fencing,
revocation/fork, concurrent admissions and deliveries, carrier workspace,
canonical schema agreement and truncation, and stepwise strict-allocator failure
rollback. Allocation faults use a test-only linker `malloc` wrapper; production
code contains no injection hook. VM/CLI/debug integration and an actual inference
backend require their own end-to-end qualification before these imports can be
reported as implemented.
