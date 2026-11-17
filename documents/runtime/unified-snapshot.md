# Unified snapshot archive and dump disclosure

Dump disclosure is selected under the `debug` command-line category. It is
independent of capture completeness and never grants execution, restore, driver,
WASI, ROS or signed-update authority.

`--debug-dump-max diagnostic|full` chooses a ceiling. Omission means `full`.
An individual export may select a lower disclosure. `--debug-dump-force
none|diagnostic|full` forces one disclosure for every export; `none` disables
dumps. The two options are mutually exclusive, including repeated selections.
Both are sealed before guest execution. No console command, DAP request,
reconnection, Wasm import or module replacement can change the policy.

This policy controls VM snapshot exports. It does not change
`--debug-fault-action` or the operating system's native core-dump configuration.
A native core produced by the operating system after a crash is a separate
host-managed artifact; diagnostic disclosure does not redact that native core.

Debugger permissions further narrow disclosure: `full` and `read-only` permit
full data; `limited` permits diagnostics; `none` forbids dumps. A forced selection
above the debugger permission is a launch error, rather than a silent downgrade.
The maximum selection can exceed the debugger permission; the effective ceiling
is the lower of the two. A per-export request above that ceiling or different
from a forced selection is refused before output creation.

```text
uwvm2-ros -Rdbg --debug-dump-max full --run robot.wasm
uwvm2-ros --debug-permissions limited --debug-dump-force diagnostic --run robot.wasm
uwvm2-ros --debug-dump-force none --run robot.wasm

dump policy
dump wasm diagnostic STOP_ID /private/fault.uwsnap
dump wasm full STOP_ID /private/inspection.uwsnap
```

The remaining path text may contain spaces. Saving requires the exact current
cooperative stop. Export does not resume execution. Existing files and symlinks
are not replaced; failed capture is discarded. File output is private to its
host owner. The deployment must keep its output directory outside all guest
preopens and other guest-reachable resources.

## Diagnostic and full disclosure

A diagnostic archive physically contains only original immutable Wasm modules,
committed replacement Wasm bodies and basic logical fault locations. Original
custom sections, including embedded static debug information, are part of the
original module. This intentionally preserves statically knowable program data.
The diagnostic record has no arbitrary text or blob fields: it stores the stop,
classified status, participant count and logical module/function/offset/generation
coordinates. An unknown offset is represented explicitly. It contains no native
PC, stack pointer, register value, live memory, local/global value, GC payload,
allocation tag, WASI environment/resource binding or ROS message content.

Full disclosure permits dynamic state but does not assert that every state
component was captured. At a supported cooperative LLVM stop, the collector
captures the typed Wasm instance and each distinct WASIp1 environment in the
same closed host-admission and GC transaction. The graph includes continuations,
GC identity and cycles, exception values, tables, memories and segment state.
An unavailable continuation instead produces an explicitly **partial** inspection
record, with the executable state marked unavailable. This includes stops for
which a genuine complete parked-worker capture cannot be obtained.

A valid checksum, a full disclosure label or a successful export never proves
that the file is a resumable checkpoint. The restore command separately checks
the complete graph, source, feature policy, current permissions, environment
bindings and genuine parked cohort before retiring any worker.

Joint Wasm/WASIp1 restoration is connected to the runtime and debugger. Stable
ROS v1 imports additionally support `current` and constrained `recorded` local
environment restoration. Current native capability/session owners are handed to
the new module incarnations after physical worker retirement. Full snapshots
capture stable v1 local ROS environments in
the same closed-cohort transaction: policy-group membership, effective connection
and interface configuration, capability definitions, current handles, revocation
state, quota/accounting observations, retained audit records, local event queues,
pinned delivery, outstanding response reservations and availability tombstones.
Capture neither drains queues/audit nor refreshes quotas. Native socket handles,
IPC authentication credentials, waiters and process pointers are excluded.

These ROS records are explicitly **partial**: external peer state and already
committed robot effects are not captured. The local fields are available to
offline inspection and are validated before either restoration choice. They
cannot issue native authority. Recorded local delivery can rewind only against
independently validated current owners, as described below. The explicit execution-replay adapter below consumes a qualified worker-cohort
instruction/import journal. Ordinary restoration still refuses replay-event
records and unknown ROS payload schemas before retirement. This also applies to event
objects embedded inside the typed Wasm graph, not just a separate outer events
component. The native preparation API repeats the event-history check, so
bypassing archive decoding cannot silently discard a journal and execute live
host effects. A refusal leaves the original cooperative stop intact.

Each linear-memory image records its full page count independently of its sparse
page payloads. An omitted page represents zero bytes at that page's actual offset;
it does not shorten the memory or move later pages. Allocation tags are separate
per-memory data. Inspection and replay verification must compare the reconstructed
extent, bytes and tags for each memory, including zero-filled pages added by grow.

## Restoring execution

Restoration requires `full` debugger permission and an exact current cooperative
stop. `read-only`, `limited` and `none` do not authorize it. Diagnostic archives
and partial inspection exports cannot restore execution.

```text
dump restore current STOP_ID /private/robot.uwsnap
dump restore recorded STOP_ID 0:0:1,0:1:3 /private/robot.uwsnap
dump restore continue
```

`current` preserves the current WASIp1 environment selection. Its managed
resources are prepared privately; already-owned external resources remain bound
to the current host. External file contents, network peers and completed I/O
effects are not rolled back.

For stable `uwvm_ros1_v1`, `uwvm_ros2_v1` and their original client v1 imports,
`current` also preserves the actual live native sessions, capability handles,
queued work, alias revocations and consumed quota accounts. Group/global owners
stay shared and single owners stay separate. The whole registered cohort must
match the source/module mapping; duplicate or foreign bindings, intervening
retirement and unknown provider identities reject preparation. Old module
registrations are removed when the new ones are published. A permanently
retired runtime member stays retired even when it never opened a ROS handle.
There is no reconnect, re-grant, replay or robot command in this handoff.
Newer RMW, portable and scene interfaces require their own adapters and are
outside this stable v1 restoration path.

`recorded` reconstructs saved arguments, environment strings, descriptor aliases,
free-list order, flags, rights and managed-file bytes/cursors. Each saved resource
requires an explicit `module:resource:descriptor` binding to an independently
authorized current guest descriptor in the same environment. These are **WASI
descriptor numbers, not arbitrary operating-system FDs**. `-` denotes an empty
binding list when the archive has no resources. Source hashes, group membership,
resource kinds and rights are validated; bindings cannot move a capability across
groups or increase the current rights. Current policy and consumed quotas remain
authoritative. Shared group/global environments stay shared; single environments
stay separate. External resources still require current bindings and their
external side effects are not rewound.

For stable ROS v1, `recorded` privately reconstructs retained local event queues,
pending/control records, pinned delivery and outstanding response reservations.
It requires the same current policy-group membership, capability definitions,
effective interface configuration, revocations, handle generations, native
session and external input/request identities. The saved external input cursor
must still match the current cursor; only retained local delivery may rewind.
Changed or closed handles, newly revoked authority, changed external input and
already consumed service responses reject preparation without retiring workers.
This mode cannot recreate a closed native session or resurrect an external request.

Current permissions, consumed quotas, audit history and the native request-ID
high-water mark remain authoritative. Restoration neither reconnects the peer nor
repeats a publish, service call or robot command. Local queue statistics describe
the selected local queue image; they are not restored capability quota credits.
All native session witnesses and privately prepared queue replacements are checked
again under the registry/session locks before publication. Both ROS1 and ROS2
use their stable v1 adapters; newer interface versions are outside this path.

After a restored worker returns from an authenticated builtin WASIp1 or stable
ROS v1 call, a fresh complete cooperative stop may capture the new state again.
An unrecorded external effect remains a permanent gap in replay history; it does
not substitute for a deterministic replay adapter. Unknown native providers still
invalidate executable recording, and in-flight host operations still exclude
capture. No failure flag is cleared to manufacture a complete checkpoint.

Preparation allocates and validates the replacement world before retirement.
The runtime then drains and physically joins the old guest workers, publishes
the new source/code/resource generation, and pauses the restored continuations.
Source maps and unwind ranges are rebound to the new compiled functions. Old
thread, frame, variable and native-code references cannot authorize new queries.
Logical breakpoints are revalidated; native breakpoints and activation-bound
value watches cannot retain their old code/activation authority.

A pending result retains the operation and keeps execution closed. Use
`dump restore continue` to finish it. A management-channel timeout does not prove
that retirement failed: the DAP adapter retains a pending state, retires old
references and exposes the continuation action in Diagnostics. It does not
interpret the temporary closed execution domain as process termination. A
successful acknowledgement names the new stop and epoch; ordinary Continue is
an explicit subsequent operation.

The archive carries source-platform metadata and canonical logical state, not a
native stack image. Restoring on another architecture still requires compatible
source, features, current resources and freshly generated continuations; platform
metadata alone is never a compatibility or execution-authorization certificate.

## Recording and replaying an executable interval

The runtime now has a separate, opt-in execution journal in the unified archive:

```text
dump record start STOP_ID
# Continue or step to another cooperative instruction boundary.
dump record save STOP_ID /private/interval.uwsnap
dump record status
dump replay STOP_ID /private/interval.uwsnap
# The restored origin is paused; Continue or step consumes the journal.
```

The start and save commands require `full` debugger permission, full dump
permission and the exact current cooperative stop. The archive retains the
original platform, source and environment records and replaces its typed state
with the recording origin plus an ordered executable journal. Saving does not
resume execution. If allocation, graph validation or stop validation fails
before the capture is published, any newly appended environment and endpoint
records are discarded together; the prior recording mode, history selection and
monotonic timestamp are retained. A retry cannot inherit a partial endpoint set.
A failed file write can retry the same sealed journal; it does
not append another endpoint. Ordinary `dump restore` rejects this file rather
than discarding the journal.

Each recorded opcode has its logical module, function, Wasm offset and thread.
An imported call adds its authenticated caller/import index, typed numeric
arguments and results, and canonical memory effects. The adapter resolves the
actual current retained builtin provider for direct and table-mediated calls;
a string naming an import does not qualify a native provider. Replay compares
the exact instruction and arguments, checks all memory extents and before/after
hashes, and prepares every memory patch before any write. It then copies the
recorded memory/result effects instead of calling the native provider. A later
patch failure cannot partially apply earlier writes.

Preparation also resolves journal import records against the current canonical
builtin bindings and checks their parameter/result types and transaction shape.
If a provider now records separate memory phases, an older indivisible-import
record is refused before the original workers retire. A matching import name
or public ABI signature alone does not establish replay compatibility.

An endpoint or opcode divergence produces a stopped world and denies Continue,
stepping and guest-state changes. An import divergence or allocation failure
inside the executing bridge exits with **125** before dispatching that
unrecorded native call. No failure, exhausted journal, reconnect or ordinary
Continue switches back to live imports. Returning to live execution requires an
explicit ordinary snapshot restoration. Permissions, native sessions, audit and
consumed quotas do not rewind.

The adapter schedules the authentically captured native worker cohort at Wasm
instruction boundaries. Shared memory uses the same logical memory identity
across module aliases. Atomic wait/notify records enqueue, native readiness and
result-delivery order;
replay does not call the live wait/notify provider. An origin may contain genuine
pending waits, including their FIFO positions and remaining monotonic durations.
Restoration pre-registers these waiters against the new owned memories before
releasing any worker. It does not repeat an already completed value comparison.
A journal may end with a waiter still pending; that worker remains paused at its
recorded wait endpoint. Repeated restoration retires it at that exact instruction.
Intermediate pauses also park virtual waiters and produce restorable queue
state. Native readiness is distinct from delivery of the result: a notified or
timed-out worker may still be waiting for its recorded instruction turn. A
completed timeout restores as a completed operation, with zero remaining time;
it is never requeued or granted another timeout. The version 2 execution journal
carries a monotonic timestamp on each event. Recording uses the host
`fast_io::posix_clock_gettime` adapter, while replay uses recorded time, so pausing
for inspection or changing wall time cannot spend historical wait credit.
Wait opcode, memory, operands and queue geometry are checked before retirement.
Each newly entered wait and pending endpoint also carries its memory extent,
effective address, compared word and allocation tag. Replay checks this preimage
before consuming the recorded result. Memory grown during the interval is
checked against both its declared maximum and its actual replay-time extent.

A worker may return during the recorded interval. Its exit is journaled and the
remaining workers can reach the endpoint; restoring the origin recreates its
original worker count. Native retirement separately enumerates both parked Wasm
workers and launch callbacks that have actually returned. Returned callbacks
still retain their native owners until a real OS join completes, including TLS
destructors. A callback between two guest entries is not a returned callback and
cannot be omitted from the retirement proof. A pending native join keeps the new
world unpublished; retrying retirement never executes an extra guest instruction.

Active MTE is supported for memory32/memory64 and shared/private owners. Random
tag entropy is recorded, allocation tags are restored, and imports verify tag
images before committing recorded effects. Hardware mappings and native thread
controls are rebuilt under the current host MTE policy, never copied as native
pointers. Hardware log-only policy and checked hardware mappings are exercised separately.
The replay tests also transfer hardware-origin journals to software hosts, and
software-origin journals to an AArch64 hardware backend. They compare the actual
bytes and full allocation tags and require no new native stdout effect. Wasm64
tests include tags whose upper four bits differ while the low hardware bits
match; hardware acceleration does not remove the full eight-bit check.
Asynchronous reports are drained at real debugger stops.
MTE log-only violations detected inside an authenticated import have their own
journal events. Replay reproduces those numeric diagnostics through the current
host-selected report sink, without reentering the WASI/ROS provider or obtaining
fresh input. Diagnostic source modules are rebound to the current logical source;
no native address, output descriptor or log path comes from the archive. A journal
containing log-only import violations is refused before worker retirement when
the current host policy requires MTE crashes. File data never changes that policy.
The recorder observer is reached only on the existing cold violation path; a
successful normal MTE access acquires no additional recorder state.

The numeric/memory ABI of authenticated WASIp1 and stable ROS v1 imports is
replayed without dispatching their live native providers. Stable ROS v1 calls
record separate import-begin, guest-memory-transaction and import-result phases.
The native provider does not own the guest instruction turn while acquiring its
locks or waiting for a peer. Each actual host memory operation reacquires the
turn, verifies tags and records an indivisible before/after transaction. Other
workers can run between those phases. Replay follows that interleaving without
entering the provider. Read-only or failed transactions cannot contain writes.
The Wasm32 and Wasm64 `poll_oneoff` providers use the same mechanism around
subscription reads and event writes; the native polling wait does not hold an
instruction turn.

The Wasm32/Wasm64 fd read/write and positioned-I/O providers detach their native
scatter buffers during recording. Kernel reads cannot
write guest memory while another worker runs; kernel writes use the captured
input bytes. Completion reacquires the instruction turn, checks every output
range and allocation tag before the first write, then publishes the result.
For relocating allocator owners, the provider also releases its genuine memory
operation pin while native I/O runs. Completion reacquires the instruction turn
and pin, resolves the current allocation and checks offsets/tags again. Old guest
pointers are not reinstalled into native vectors after relocation. Native error
cleanup reacquires the pin before the enclosing RAII guard releases it. Mmap
owners retain their stable-address path; ordinary non-recording I/O keeps its
original buffers and memory guard.
Empty vectors and EOF results participate in the same protocol. Linux socket
receive/send, including the `fd_read`/`fd_write` socket path, use detached native
vectors; both the count and socket-flags outputs are validated before commit.
Socket accept and fd/path metadata operations release the instruction turn
around native locking and work, with separate phases for their actual guest
memory reads and writes. Native descriptors, rights, quota reservations and
audit effects retain their existing authority and lifetime rules.

Arguments, environment variables and clock results also publish explicit memory
phases. Randomness uses detached, bounded transfer batches while recording; the
batch size is not a request limit. No guest pointer or tag lease remains held
while obtaining native entropy. Quota waits run outside the guest instruction
turn. Replaying the saved result does not call the entropy source, reread a native
clock or acquire another quota reservation. The concurrent tests exercise both
unlimited grants and an explicit random-call limit with synchronous waiting, and
compare the real native input/output and quota counters after each replay.

The ordinary debugger adapter and private WASI Threads adapter have distinct
native ownership domains. Dynamic child-world reconstruction uses the real
thread-group execution journal described in [WASI Threads execution replay](wasi-threads-replay.md);
it is not obtained by accepting private workers into the ordinary continuation
ABI. In-flight audited native imports use the same-process retained continuation;
their native stacks and unknown results are never portable replay input. Historical
WASIp1 and stable ROS v1 environment inspection is available at recorded coherent
checkpoints, as described below; it does not infer unsampled intermediate host state. The private
WASI Threads LLVM compiler issues exact Wasm instruction control points when
the host explicitly enables cooperative thread debugging. Their offsets and
ordinals belong to the actual child-owned code image; loop, else and end points
use the shared structured-control lowering. Observation-only and default builds
do not emit these calls. Control targets are selected during compilation, leaving
the default native poll/entry path unchanged. These points do not borrow ordinary
checkpoint activation credentials or constitute a saved child continuation.
The host-owned all-stop guard can also advance one authenticated worker to its
next cooperative boundary while the other workers remain stopped. A transfer
invalidates the old observation until a fresh complete cohort and a newer poll
sequence have been proved. Timeout retains the pending transfer; retrying that
same participant waits for it instead of executing another instruction. Native
waits and host boundaries are not mislabeled as completed Wasm instructions.
Before a private child publishes an armed stop, its actual native worker also
synchronizes pending MTE delivery. A queued software report must be acknowledged;
hardware asynchronous faults are synchronized using the active hardware policy.
This boundary runs outside Group, pause-domain, provider and wait-shard locks.
It does not add a poll to default or idle control execution. An MTE fault that
requires termination cannot be hidden behind an apparently restorable stop.
This scheduling primitive is not itself a portable execution recording.
A replayed virtual import responds to a requested cooperative pause while it
waits for another participant's recorded turn. The stopped location identifies
the authentic Wasm caller; the native replay helper is never an assembly landing.
Continuing consumes the remaining recorded phases without entering the native
provider. This intermediate observation cannot be exported as an ordinary
executable pre-call checkpoint: doing so could repeat an already recorded effect.
A new portable origin or endpoint must let a native host call
finish; its native stack is not a portable saved continuation. Live environment
panes describe the bound current environment. The separate historical view shows
only the selected recorded observation and never reconnects a peer or descriptor.
These are separate adapters;
fixed-cohort shared-memory tests do not qualify them. Unsupported origins are
refused before recording or retirement. An unknown
provider or an exceptional/nonreturning import invalidates the interval; a
fresh recording cannot relabel that history as complete. Recording and replay
reject debugger memory/value changes, code replacement and instruction relocation.

The instruction/import journal is full dynamic data. Diagnostic disclosure
never exports it. Recording introduces work only in selected debug checkpoint
bridges; ordinary uninstrumented LLVM execution has no new opcode hook or memory
scan. Recording deliberately copies and hashes memories around imported calls;
this is a diagnostic cost and is not a normal-execution optimization claim.

VS Code Diagnostics includes Start recording, Save recording, Recording status,
and a replay choice in the restoration selector. DAP clients use
`uwvm/recordSnapshot` and `uwvm/restoreSnapshot` with `environment: "replay"`.
These use the same stopped-identity and post-publication reference invalidation
rules as ordinary restoration.

An explicit private WASI Threads launch is recognized by the same DAP/broker
entry point. Its launch journal, prefix cursor, joined-cohort lifetime and
detached environment samples are described in
[WASI Threads execution reconstruction](wasi-threads-replay.md#dap-and-ide-control).
The ordinary continuation adapter and private instance reconstruction remain
separate execution implementations; a UI selection cannot exchange their
ownership credentials.

### Historical WASI and ROS environments

Recording retains the origin's detached environment data. `dump record mark
STOP_ID` adds a coherent environment observation without sealing the interval;
`dump record save` adds the final environment observation before its endpoints.
Each sample is taken under the same complete native stop, host-operation closure,
GC admission and publication proof as the Wasm census. A live in-flight native
provider still prevents this operation; a saved observation never fabricates its
continuation or pending result.

`dump history` lists the latest environment checkpoint reached by replay (or the
origin before the first checkpoint), its recording identity, event ordinal and
logical instruction count. `dump history SERIAL KIND ID OFFSET COUNT` transfers
its canonical bytes in pages of at most 4096 bytes. Every page is bound to the
same non-reusable selection serial. Replacing the selection rejects stale reads;
there is no fallback to current WASI or ROS state.

The archive and journal are pre-release formats. Readers require the current
schema; there is no legacy execution-state conversion or migration path.

To inspect a file without changing execution, use `dump history open EVENT PATH`.
Event 0 selects the origin; a nonzero event must be an environment-checkpoint
record in a validated execution journal. This selection remains detached from
subsequent live execution. A new recording or executable restoration switches
history back to that recording's runtime position. Opening a file does not
restore its state, authorize its interfaces or open its saved resources.

Full or read-only debugger permission and full dump disclosure are required.
Limited/none debugging or a diagnostic/none dump ceiling refuses dynamic
historical environment data. All checks are repeated in the VM. DAP clients use
`uwvm/snapshotHistory` with no arguments for the index, `{kind,id}` for a selected
WASIp1/ROS1/ROS2 environment, or `{path,event}` to open host-side archived data.
Remote clients use the same host channel; no client-side file path is resolved.
The Diagnostics view provides these operations and a recording checkpoint button.

### Retaining an in-flight native operation

During explicit execution recording, audited phased WASIp1 and stable ROS v1
imports can relinquish all guest-memory access while waiting on a native
descriptor, quota or RPC. A pause can then retain the original operation and
stop guest execution. Native work may finish while paused; its memory phases
and return delivery must wait for continuation. A paused guest does not cancel
an already accepted native request or roll back its external effects.

At this stop, `dump record save STOP PATH` or `dump wasm full STOP PATH` saves
the known origin and execution prefix plus a `retained-native` record. The
record is explicitly partial: the native result and stack are not serialized.
The archive contains platform and embedded Wasm identity as usual. WASI/ROS
environment records describe the recording origin; they are not an invented
observation of a provider that still owns native locks. `dump history` remains
available. Live WASI/ROS environment queries refuse until the original operation
continues, rather than blocking the debugger on those locks.

`dump history open 0 PATH` also reads the origin in a retained-native archive
from another process. A nonzero event may select an already completed
environment observation in its known prefix. The reader validates the prefix's
instruction/import ordering and the same source/environment manifest used by
ordinary snapshots. It neither fills in the missing native result nor converts
the partial journal into an executable replay plan. Invalid input leaves the
previous history selection unchanged. The offline `snapshot.py info` output
identifies this archive as requiring its original live stop for continuation.

`dump restore current STOP PATH` and `dump replay STOP PATH` recognize this
archive only in its original process, with the same retained journal and exact
live pause. They authenticate the complete saved bytes and the runtime's opaque
native owner, then reply `snapshot-reattached`. `continue` releases that original
operation. No second syscall, RPC, permission reservation, connection or worker
is created. Completion that arrives while paused remains in the original
worker; reattachment does not lose or duplicate it. The debugger never exposes
the native provider PC as a JIT assembly stop.

A foreign process, altered archive, expired pause, replaced journal or recorded
environment rebinding is rejected before retirement or any external operation.
This includes a fork child: inherited native-worker state is rejected before
accessing the parent's copied journal or pause-domain locks. Fork does not
transfer a native continuation; an independently executed process can inspect
the saved history but cannot adopt an unknown native result.
The process-local registration expires on resume. To replay after execution has
advanced, save a completed journal with known results at a coherent endpoint;
that journal uses the normal portable replay path. These rules apply equally
to x86_64, i686, AArch64, ARM soft-float and ARM hard-float. They do not turn a
retained native call into a portable serialized native continuation.

Full debugger permission and full dump disclosure are required for retained
continuations. Diagnostic dumps never contain the journal or native-operation
registration. DAP reports `retainedNativePending: true` for a retained save or
reattachment; it still invalidates old value references and authenticates the
current stop before publishing it to the IDE.

Arguments and environment bytes, descriptor aliases and rights, managed-content
metadata, and stable ROS v1 local resource observations are shown as structured
data. These are historical observations, not current permissions or replayed
external-system state. Permission grants, quota usage, audit history, native
sessions and external effects are never rolled back by viewing them.

## Binary container

All integers are fixed-width little endian. The 64-byte header begins with
`UWSNAP1\0`, u32 version 1 and u32 header size 64. It records u16 architecture,
u16 operating system, u8 source endianness, u8 pointer width, u8 disclosure,
u8 reserved zero, four u32 product version fields, u32 logical-state ABI,
u32 continuation ABI, u64 feature mask and u64 record count. Zero ABI/feature
fields mean that no resumable profile is attested, not unrestricted compatibility.

Each 32-byte record header contains u16 component, u8 completeness, five reserved
zero bytes, u64 file-local identifier, u64 schema and u64 payload size, followed
by the exact payload. Records are strictly ordered by component and identifier.
Components are diagnostic (1), resumable Wasm state (2), original Wasm module
(3), effective replacement body (4), static debug source (5), WASIp1 (6), ROS1
(7), ROS2 (8), replay events (9), Wasm inspection (10) and retained native
operation (11). Not every component is
currently collected. Completeness is complete (0), partial (1), redacted (2),
or unavailable (3); unavailable records have no payload.

A retained-native marker has identifier 0, schema 1, partial completeness and
exactly 32 opaque random label bytes. Its label conveys no authority. Only an
independent runtime registration with an identical complete archive digest,
original process identity and still-current pause can permit reattachment.
The archive contains no native pointer, descriptor ownership token or unknown
result. Diagnostic disclosure cannot include this marker.

A collected ROS v1 local environment has outer schema 2, identifier 0 and partial
completeness. Its payload begins with `UWROS1\0\0`, u32 payload version 1, u32
reserved zero, a 16-byte recording label, u8 backend (1 or 2) and a u64 group
count. Group, capability, handle, queue and availability records use fixed-width
little-endian scalars; strings, byte buffers and vectors have u64 lengths. Boolean
values must be 0 or 1. Groups, members, field maps and handles have canonical
order and reject duplicates. Every count-derived allocation is bounded by the
remaining physical input and the element's minimum encoded extent. Event payloads
stream as contiguous byte spans, not per-byte output calls.

The backend inventory and recording label must match the actual typed Wasm graph;
module membership must be complete and nonoverlapping. Files cannot cross a
policy boundary or restore a retired capability. Schema-1 empty unavailable
markers from earlier captures remain explicit absence data accepted only for
`current`; a populated unavailable marker, unknown schema, contradictory backend,
missing group member or completeness upgrade is refused before retirement.

The 48-byte commit footer contains `UWSEND1\0`, u64 total file length and
SHA-256 of every preceding byte including the first 16 footer bytes. SHA-256
detects corruption; it is not a signature or restore authorization. Readers
validate all lengths, order, reserved fields, platform fields and disclosure
before publishing parsed data. Diagnostic archives reject every dynamic
component even if an attacker recomputes the checksum. Typed payload validation
is separate from outer framing. No architecture-specific pointer or native
instruction is reconstructed from the file.

Streaming avoids a second memory-sized output buffer. Transfer chunk sizes do
not limit aggregate archive size. Explicit deployment resource budgets and real
allocation/I/O failures remain enforceable. The format records the originating
ABI and uses canonical bytes on all five supported Linux architectures.

## Inspection and IDE access

```text
python3 tools/debug/snapshot.py info /private/fault.uwsnap
python3 tools/debug/snapshot.py extract-wasm /private/fault.uwsnap --module 0 --output /private/original.wasm
```

The offline reader validates the archive and typed diagnostic metadata. It does
not execute Wasm, reconnect resources, replay robot commands or restore a VM.
Extraction requires an explicit new destination and refuses replacement.

The authenticated console exposes `dump policy`. DAP exposes `uwvm/dumpPolicy`
with the immutable selection, effective disclosure and allowed export choices.
`uwvm/dump` accepts an optional `disclosure` (`diagnostic` or `full`) alongside
the exact `stopId` and absolute path on the VM host. The Diagnostics window uses
the VM response to offer allowed choices or disable Save. Its selector cannot
widen launch authority. Local and remote IDE imports preserve the same launch
arguments and reject conflicting policy before starting the VM.

`uwvm/restoreSnapshot` accepts `path`, exact `stopId`, explicit `environment`
(`current` or `recorded`) and a `bindings` array of `{module, resource, descriptor}`
objects. The continuation request accepts only `{"continue": true}`. The
Diagnostics window exposes the same choices and a pending-operation action.
The adapter authenticates the new complete stopped cohort before publishing its
replacement frames. Local and remote paths refer to the VM host, not necessarily
the IDE computer.

A rejected restoration reports its phase, native preparation status and typed
state error. `retained=1` means the current request was refused before retirement
and the original cooperative stop remains available. The adapter retires old
view handles and obtains fresh status before creating replacements. This also
applies when an archive passes binary decoding but fails native source/type or
environment validation. A stale stop ID is refused without trapping the IDE in
a pending restoration. `retained=0`, an incomplete reply or a transport timeout
does not establish that the original world survived; the adapter keeps execution
access closed until the pending operation has an authenticated outcome.


## Deterministic replay memory transaction component

The internal `checkpoint_replay_memory.h` component records canonical memory
deltas for one host operation. It is the data codec and transaction primitive
used by the explicit execution-replay adapter described above. Observational
call traces, ordinary restoration and a matching byte checksum alone do not
establish deterministic execution. Multi-worker replay still requires a
recorded scheduler.

A payload begins with `UWRMEM1\0` and a little-endian u64 memory count. Each
memory descriptor contains its file-local object ID, byte length, 32-byte
SHA-256 input and output digests, and a u64 patch count. Each patch contains a
u64 offset, u64 byte count and the exact replacement bytes. Descriptors are
strictly ordered by object ID. Patches are nonempty, increasing and separated
by unchanged bytes; overlapping, adjacent or out-of-range patches are rejected.
All memory identities remain distinct from module-local memory indices and
native addresses.

The adapter must first authorize actual, pinned memory views in a detached
replay world. Each view must belong to the exact matched event's capability
list. The transaction verifies every input digest, length, patch and projected
output digest before committing any memory or typed result. A malformed final
patch, mismatched input, typed-result refusal, allocation exception or nested
application consumes no event and applies no earlier write. Distinct memory
IDs may not alias native storage.

The primitive never calls a native provider, reconnects a peer, modifies a
file, changes a permission or rewinds quota/audit accounting. It introduces no
implicit recording-size quota; a host can supply an explicit payload budget.
Memory growth, MTE tag state, WASIp1 descriptors, ROS handles and deterministic
thread scheduling still require their own actual runtime adapters. These
component checks do not establish complete runtime replay.


### Recording capacity and scheduling

Checkpoint graph limits default to native addressability, rather than an implicit
one-million-object or 256 MiB quota. An embedding host can supply explicit
`checkpoint::limits`; those limits remain transactional rejections. Counts from
an archive are still checked against its authenticated byte extent before any
allocation or pointer arithmetic. Native allocation failure does not authorize a
partial restore or a fallback to live external operations.

While execution recording is selected, instruction and audited host-memory turns
are admitted in request order. This prevents a runnable worker from repeatedly
beating already waiting peers to the journal mutex. The recording still follows
actual worker availability; it does not promise that the operating system has
scheduled every participant. A worker entering an authentic debugger park gives
up its queued request and rejoins after resumption, so a paused peer cannot block
another worker released for one step. Replay follows the saved ordering. This
queue is not used by normal execution without recording.

A recording command reports the actual journal mode even when a running or stale
stop prevents capture. Its execution suffix distinguishes a stopped guest,
running guest, incomplete pause, exited guest and closed debug domain. A failed
pause of an outstanding native call never constitutes a complete stop.
The DAP `uwvm/recordSnapshot` response also carries this state as `execution`
(`stopped`, `running`, `stopping`, `exited`, or `closed`). Start, mark and save
acknowledgements require a genuine stopped state; a refusal preserves the VM's
actual diagnostic in the DAP error response. The previous unconditional
"guest stop retained" recording acknowledgement is not accepted.
