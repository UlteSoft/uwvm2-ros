# WASI Threads execution reconstruction and replay

This path records real private WASI Threads workers. It reconstructs their Wasm
execution by running the original, revalidated modules under a recorded logical
schedule. The interpreter and LLVM use their actual child engines, compiler
workers, memory owners, and native threads. A decoded archive never constructs
an execution lease, a native thread, or a permission grant by itself.

## Starting a recording

Add the following to an otherwise authorized, finite `--wasi-threads` launch:

```text
--debug-permissions full
--debug-thread-history record /absolute/path/run.uwsnap 4194304
```

The last argument reserves bytes for cooperative thread control. It does not
grant WASI, ROS, or thread-spawn permission. Configure those interfaces and
resource grants separately. The immutable dump ceiling must permit full data.
Only the trusted launch selects this mode; Wasm cannot enable or disable it.

A normally completed main, an authorized `proc_exit`, or a recorded Wasm trap
can seal the history. For a stable process exit after a classified Wasm trap,
use `--debug-fault-action exit`; the default crash policy is unchanged. Output is
published only after the archive is complete. A refused capture does not publish
an apparently complete file. Ordinary execution does not install this journal
or generate its per-instruction control points.

Replay the completed interval with the original modules and currently authorized
thread/storage configuration:

```text
--debug-permissions full
--debug-thread-history replay /absolute/path/run.uwsnap 4194304
```

The CLI checks the archive before starting private execution. The actual source,
initial memory and tags, globals, tables, entry, and parameters are checked again
when the real world is prepared. An incompatible origin or divergent execution
is refused; the runtime never falls back to live I/O. Replay failure has status
125. Malformed launch/input remains a parameter/input rejection. Guest
`proc_exit` and classified trap statuses retain their existing semantics.

## Reconstructing a nonterminal checkpoint

Native embedders use the host API in
`uwvm_runtime_threaded_profile_api.h`. Obtain an actual
`threaded_debug_all_stop` from the selected group's control session, then call
`runtime_threaded_profile_snapshot_capture_host_api`. This copies a finite known
prefix, retaining immutable source modules and logical events.

Before reconstructing, physically retire and join the original workers,
compiler jobs, pending native cleanup and source owners. Complete the full Source
reset. Configure the saved archive with
`runtime_threaded_profile_snapshot_replay_configure_host_api`, independently
prepare the authorized source, and start its real main through
`runtime_threaded_profile_start_main_host_api`. Actual replayed `thread-spawn`
instructions create real children. Local values, operand/control stacks, GC
objects, and shared memory are produced by executing the checked Wasm prefix;
serialized native stack bytes and machine addresses are never restored.

The end of a nonterminal prefix parks execution. Obtain a new all-stop guard;
old guards and worker identities cannot address the new generation.
`runtime_threaded_profile_replay_continue_host_api` can advance only if the
archive contains a further recorded interval. It refuses an unknown future.
There is no automatic branch from replay to real robot control or filesystem I/O.
The unattended process CLI accepts terminal histories. The authenticated debug
channel and native embedding APIs also accept nonterminal recorded prefixes.

## DAP and IDE control

Use LLVM AOT for live thread control through the secure broker, DAP adapter
or VS Code Diagnostics view. The launch requires `--wasi-threads`, an
authenticated debugger endpoint, full debugger permission and full dump
disclosure. Recording starts before the first Wasm instruction. For an
interpreter launch, use `--debug-thread-history` in ordinary run mode to record
or replay a completed history.

A managed launch can also select `--debug-thread-history replay`. Its initial
debug stop waits for the actual journal boundary, rather than confusing an
earlier cooperative pause with that boundary. A history saved at its completed
terminal endpoint can attach in the joined state; the IDE remains connected and
can select an earlier recorded cursor. Neither launch form dispatches new native
I/O while replaying.

`initialize`, `attach`, `configurationDone`, `threads`, `pause`, `continue` and
`stackTrace` use the actual private execution driver. Its stack location is the
authentic current Wasm instruction, not a fabricated native backtrace. The DAP
capability response does not advertise ordinary variable editing, machine
disassembly or breakpoints for this replay driver. The ordinary debugger retains
its existing capabilities in ordinary sessions.

Use these custom DAP requests from any IDE using the adapter:

- `uwvm/recordSnapshot` with `action: status`, `save`, or `mark` observes the
  journal, saves it, or captures an environment sample. `start` reports the
  existing launch recording; it never silently restarts the program.
- `uwvm/dump` saves a full thread archive at the exact `stopId`.
- `uwvm/restoreSnapshot` with `environment: replay`, or `uwvm/threadReplay`
  with `action: replay`, reconstructs a checked prefix. `path` is absolute on
  the VM host. Optional `cursor` selects a recorded event boundary; zero uses
  the saved endpoint. Live descriptor rebinding is not a replay operation.
- `uwvm/snapshotHistory` opens and inspects detached environments as described
  below. Remote IDEs use the same host channel.

The corresponding authenticated host commands are `thread-history status`,
`thread-history mark STOP`, `thread-history save STOP PATH`, and
`thread-history replay STOP CURSOR PATH`. A physically joined terminal cohort
uses stop 0; live cohorts require the current nonzero stop serial. Old frames
and stopped-value references retire before reconstruction. Rejected input does
not authorize a fallback to current native I/O.

When the cohort finishes, the DAP session stays connected so its recording can
be saved or replayed. `uwvm/threadReplay` reports the joined state separately
from DAP session termination. VS Code Diagnostics provides recording, history
and replay controls, including the event cursor. A new session clears the
previous session's private-thread selection and history display.

## Dynamic lifecycle and host effects

The journal records spawn arguments and success/failure, logical worker births
and departures, Wasm instruction locations, shared-memory import transactions,
atomic wait/notify outcomes, memory growth, MTE entropy, and terminal outcomes.
A recorded spawn failure remains a failure even if the new host has more free
resources. A recorded success still requires actual current permission,
allocation, compilation, and native thread creation. Logical TIDs are bound to
real newly created workers; they do not authorize arbitrary native thread access.
Returned workers are physically joined before their storage can be reused.

Memory growth checks the actual shared provider's maximum as well as current
host limits. An importing module's larger declared maximum cannot enlarge its
provider. A recorded allocation failure is not retried as a successful grow.
A recorded success requires genuine native memory growth and correct MTE state.
Current allocation failure stops replay instead of fabricating accessible pages.

Audited WASIp1 and stable u2ros1 calls record their real inputs, memory effects,
and results. During replay these events supply virtual results after matching
the authentic import and current Wasm call. File writes, random/clock providers,
ROS gateway requests, session creation and publishing are not invoked again.
Current permissions, quota consumption, and external-system history are not
rewound. Here u2ros1/u2ros2 abbreviates the uwvm2-ros v1/v2 interface ABI, independently
of the official ROS 1/ROS 2 backend version. u2ros2 interfaces are outside this
implementation; the v1 tests cover both official ROS backends.

## MTE fault delivery while recording

The actual shared mapping is activated before binding the initial-state digest
or launching any worker. Each native execution thread independently enables the
requested hardware mode. A successful command-line admission alone is not
hardware activation evidence: Linux must accept the checked mapping and the
per-thread control. Unsupported hardware is refused before Guest launch.

Synchronous recording adds a complete tag proof before a leased native memory
callback, including on hardware-backed memory. This allows a classified failure
to leave every C++ memory/tag lease normally and seal the history. The physical
hardware checks remain enabled; ordinary hardware execution retains its original
checking path. The recording proof is additional diagnostic work, not a claim of
zero-cost recording or of hardware-only tag comparison.

Software asynchronous delivery drains its evidence at actual execution
boundaries. Hardware asynchronous delivery retains the real MTE notification in
signal-safe, per-thread flags; the handler performs no journal operation,
allocation, locking or stack unwind. A subsequent native safe boundary seals the
fault and stops the group. Linux asynchronous signals do not identify the exact
access address; the diagnostic preserves that limitation. The journal identifies
the delivery boundary, not a fabricated precise hardware faulting PC. Replay
reproduces the terminal class and does not proceed to later external effects.

Log-only mode keeps its existing software reporting policy, including when a
hardware backend was requested. It must not be advertised as physical MTE
acceleration. Ordinary bounds enforcement remains mandatory in every mode.

## Terminal decisions while a native operation is unresolved

A published Guest trap or authorized `proc_exit` does not wait indefinitely for
an unrelated blocking native import to return. The process CLI observes the
actual group's terminal decision before physical join. For a fault, it applies
the immutable crash/exit/pause policy; `proc_exit` retains its authorized status.
No live native stack is unwound or repurposed. Process termination leaves its
final resource cleanup to the operating system. Pause retains the actual owners
until the host explicitly continues terminal handling.

A recording with an unresolved native result is not published as a completed
history. When all effects are known, ordinary retirement and history sealing
still finish before applying the fault policy. The cold unresolved-operation
query neither adds work to the normal instruction path nor converts a replay
into live I/O. Native embedding APIs still require physical join before storage
reuse; the CLI's process-termination decision does not waive that requirement.

Imported active data is applied by each actual instance on its execution turn,
not by the cold compiler. The main and fresh children use the common validated
active-element/data initializer. Extended constant global/offset expressions
and expression elements use the common evaluator. Normal start functions run
under the same instruction journal and import permissions as the entry function;
preloaded code providers run their starts in dependency order. No libc-specific
initializer bytecode whitelist remains. GC reference globals retain their actual
instance store and are initialized independently in children.
Pure code providers may omit linear memory entirely. Their starts and mutable
globals still belong to each newly instantiated child; the runtime does not
invent a shared-memory import for them.

Unsupported engine features still produce an explicit admission rejection;
removing an initializer restriction does not authorize a native provider or an
unimplemented instruction. Internal identity or reservation failures remain VM
faults. Imported numeric and GC globals and imported exception tags bind to fresh
provider instances in the private graph. They do not copy mutable parent
values or exception identities. Imported tables bind to fresh provider instances;
table32 and table64 growth, copying, typed references, and exception references
retain their declared types and instance ownership. Exception payloads are
reconstructed as rooted, immutable values in the private store, including
`catch_ref`/`throw_ref` and references carried by globals, tables or GC fields.

Private instances support multiple linear memories, including memory32 and
memory64, zero-length memories, active and passive data, mixed-width bulk copies,
SIMD and atomic operations. Each unshared memory starts from its declaration and
initializers rather than the parent's current bytes. An imported unshared memory
is rebound to its freshly instantiated provider. Shared memories retain their
authenticated owners, including multiple providers and aliases; each distinct
shared owner is counted once against the aggregate declared shared maximum.
The WASI Threads ABI still requires its primary memory to be shared memory32;
this does not restrict other memories to that width.

Local memory and committed MTE shadow storage consume the private allocation
budget, and growth respects both declared limits and current host resource
limits. Full-width memory64 deltas and grow results are journaled without
truncation on 32-bit hosts. Original allocation-tag colors are restored only
after validating the actual source, complete initial state and tag geometry.
Fresh child global tags use recorded entropy. Neither tag bytes nor memory
indices in an archive can select an arbitrary native mapping.

## Linux sanitizer builds

AddressSanitizer builds retain the same guarded mmap memory owners. Inaccessible
address-space reservations use the kernel mapping call directly, so ASan does
not eagerly clear shadow for the entire sparse reserved range. Only committed
memory is unpoisoned before it becomes visible to execution; guard pages remain
inaccessible. MTE shadow mappings follow the same reservation and commit rules.
This does not disable ASan instrumentation or replace the production memory
owner with an allocator-backed substitute.

Native stack registration uses the actual machine frame in sanitizer builds.
ASan's optional fake-stack allocation cannot enlarge the authorized pthread
stack range or become a debugger frame. Ordinary builds retain their existing
mapping and stack-probe paths.

## Unknown in-flight results

A worker waiting to admit a recorded call is pausable before any external
operation is dispatched. Retaining that native participant does not by itself
make the result unknown. A checkpoint taken while an admitted real host operation is unresolved is marked
`retained_native`. It can only reattach to its original, still-paused process,
actual group, source owners and exact stop serial using
`runtime_threaded_profile_snapshot_reattach_host_api`. The original native call
may finish, but its result is not delivered to Wasm until that stop is released.
Forged, expired or foreign-process reattachment is rejected. Starting a new
process from an unknown native result is rejected before any external dispatch.

After the original operation has finished and a subsequent complete history has
been captured, the recorded result can be replayed virtually. A pending native
stack is never serialized as a portable continuation.

## Archive and disclosure

Archives use the unified `.uwsnap` binary container, native origin metadata,
logical ABI 7, continuation ABI 0, and private event schema `0x54484a38`.
`UTHJNL7` identifies the journal encoding. Original owned Wasm modules are
included and checked against the origin source digest. Unpublished older journal
encodings are rejected; no compatibility path is retained.

Logical module identifiers follow the bytewise order of their unique source
names. Host-call records bind the call site and the declared receiving module
separately, including cross-module function references. Native registry bucket order and private-world module indices are never
wire identities. The selected root module is part of the initial-state digest.
This allows the same checked history to bind across the supported Linux ABIs
without changing the live debugger's native module mapping.

Instruction identity is the logical module, function and validated Wasm byte
offset. Backend-local decoded instruction ordinals are not serialized as an
execution identity. Terminal faults use the canonical unreachable (120),
execution (121), and MTE (122) classes and the actual Wasm instruction location,
rather than a backend-specific trap enum or unavailable native-PC sentinel. The format contains no executable native pointers or raw
machine contexts. Full disclosure is required because import buffers and shared
memory effects can contain dynamic secrets.

Inspect the archive without executing it:

```sh
python3 tools/debug/snapshot.py info /absolute/path/run.uwsnap
python3 tools/debug/snapshot.py thread-events /absolute/path/run.uwsnap --offset 0 --count 100
```

The reader verifies physical bounds, source checksums, lifecycle transitions,
record shapes and disclosure before returning data. Inspection grants no restore
authority. An archive also contains detached typed environment samples from the
origin and explicit coherent stops or completed execution. Each sample has a
journal cursor and the recording identity. WASIp1 records include arguments,
environment bytes, descriptors, aliases, rights, managed data and cursors.
Stable u2ros1 records include policy-group membership, resource observations,
retained local queues and delivery state for both ROS 1 and ROS 2 backends. Viewing them
never recreates a native session or grants a handle, permission or fresh quota.

`dump history open SAMPLE PATH` selects a sample by index (0 is the origin), in
either the ordinary debugger or the private replay session. `dump history`
returns its sample count, journal cursor, instruction count and environment
index. The usual serial-bound paged transfer serves each complete typed record.
DAP `uwvm/snapshotHistory` returns structured data; the IDE displays it as an
expandable tree. Invalid files and stale transfer serials cannot replace the
previous valid selection or switch a historical query to live state.

Samples are observations, not guesses about unsampled moments. An unresolved
native operation retains the last known coherent samples; its still-locked
environment is not read or labeled as current. Nested sample archives cannot
contain another execution archive. Readers reject obsolete outer ABI/schema
versions rather than silently discarding environment records.
