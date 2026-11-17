# Complete bounded-object publication proposal

This document records the earlier prototype stage. The production registry and
compound authority implementation is now described in
[Complete volatile-object publication](ros-artifact-publication.md). Statements
below about unregistered status and unchanged production files describe that
prototype qualification, not the current implementation's full integration
status. Its native 66/40 trace remains a topology result, not VM timing evidence.

`artifact.publish` is a proposed, **unregistered** `uwvm_ros_v2` operation for
complete small volatile-cache objects. It is not part of the 35 registered
imports, does not change the accepted VM binary, and supplies no SDK production
API. The prototype is in
`test/0084.ros-runtime/prototype/complete_publication.h`; it is an algorithm and
ordering experiment, not an alternative production registry. Existing
`artifact.begin`, `artifact.append` and `artifact.commit` remain necessary for
large or incrementally produced objects.

## Scope and independent authority

Publication accepts only bytes for an exact already-deployed `volatile-output`
cache alias. It creates no file, descriptor, device, ROS endpoint or executable
model. Its result is immutable within that live deployment and is not durable,
validated, trusted or authorized to control a robot.

The new operation requires explicit `artifact.publish` exposure and action
authority. In addition, **all three** `artifact.begin`, `artifact.append` and
`artifact.commit` actions must be exposed and granted on that same capability,
alias and owner generation. These checks also apply to empty objects. Another
alias's actions cannot complete this set. Default exposure and grants remain
zero. Unknown exposure or grant mask bits are rejected during configuration.
Possessing the new action alone cannot bypass a denied underlying step.
Read and remove remain separate operations with independent actions; publication
does not grant either. A caller can explicitly be allowed to create an object
without being allowed to read or remove it.

Final integration must make the four-action check and optional budget charge
one atomic admission under the existing authority lease. Calling the old
single-action admission function three times could partially charge a refused
operation or observe inconsistent revocation state; the prototype does not
endorse that implementation.

## Proposed carrier and work accounting

The usual seven-argument carrier would carry little-endian
`[key:u64, payload_size:u64, payload]`. The declared payload size must exactly
equal the copied request extent minus 16 bytes. No flag selects a weaker check
or a different durability mode. The successful result uses the existing
nine-word receipt shape: key, operation, object, effect status, accepted bytes,
total bytes, retained bytes, state and flags. The test-only operation tag is 8;
the ABI table has not reserved or registered it. Success publishes state 2,
flag 1, and exactly the complete payload length. An empty object still consumes
one object slot.

The proposed budget charges **three logical work units** and `payload_size +
280` bytes per admitted attempt, including exact replay. This equals the current
begin + one append + commit charging for a nonempty object: `(16+72) +
(24+payload_size+72) + (24+72)`. For an empty object, the old path skips append
and charges two calls and 184 bytes. Charging three units and 280 bytes for the
new empty path is deliberately conservative, **not** an equal-cost assertion.

These are proposal semantics, not a change to the existing CLI's call counter.
Before integration, the public distinction between carrier calls and logical
work units must be specified explicitly in the authority schema, CLI help,
debug counters and documentation. A possible implementation uses a trusted
fixed operation cost while keeping existing individual operations at one unit.
It must not accept a guest-supplied cost or change existing limits silently.
Absent optional limits permit full work within the granted scope; explicit
zero denies admission. A window without enough allowance for all three units
refuses the entire operation without a partial debit. Admitted cancellation,
provider failure, conflicting replay and exact replay retain their admission
charge. Short receipt queries and preflight refusals remain uncharged.

## Memory and effect ordering

Deployment supplies finite payload, object-count, per-object, transfer,
handle-count, in-flight scratch-byte and in-flight scratch-call reservations.
Zero never means unlimited. The prototype's object allocation is bounded by
`payload_bytes + 16*object_slots`, because each object retains its complete
owned wire record. The shared exact replay buffer must keep the larger old
append framing: `max(legacy_transfer + 24, publish_max_payload + 16)` bytes,
plus the fixed receipt. With the prototype's common transfer bound, that is
`max_payload + 24`; configuration rejects a smaller buffer even though publish
alone would fit. Preallocated slot metadata is an additional
known deployment cost. Scratch reservations account for full framing and
payload across concurrent requests; per-call transfer bounds alone would not
bound concurrent allocation.

The intended ordering is:

1. Check process identity before locking, exact owner/capability and all four
   actions. Check raw transfer extent, receipt capacity and optional budget
   availability without charging or refreshing a window. A short receipt
   returns the required 72-byte size without reading the request.
2. Reserve a finite in-flight slot and scratch bytes. Outside all registry
   locks, allocate an owned request and perform the complete checked copy.
   A partial or throwing reader produces no parsed request or object effect.
3. Validate the copied length and recheck process identity. Under the final
   shared authority/registry
   transaction, revalidate the exact scope, current revocation and all actions,
   atomically charge the proposed work cost, and read the trusted runtime's
   `std::stop_token`. This atomic observation invokes no callback and catches
   cancellation requested while waiting for the final lock. An empty token
   explicitly models a trusted noncancellable caller; guest data cannot choose
   the token. Do not call a potentially reentrant cancellation callback here.
4. Match the one shared mutation stream. Only key 1 or the exact next key may
   create a new effect. An identical last request and operation return the last
   receipt. Changed bytes, a different operation, old/skipped keys and key
   exhaustion do not replace the receipt.
5. After complete copying, reserve object payload capacity and one preallocated
   slot. Prepare receipt metadata. Move the owned request into that slot and
   replace the exact replay record under the same mutex. No allocation or
   external callback occurs after this final commit preparation. Other readers
   observe both the published object and its receipt together.
6. Release the lock before receipt delivery. Failed or throwing delivery leaves
   the receipt available for an authorized exact retry. Release in-flight
   scratch accounting on every exit. Already published effects remain committed
   if a later delivery callback requests revocation.

Allocation failure before publication consumes neither capacity nor a key and
preserves the previous receipt. An admitted object-capacity refusal produces
a retained failure receipt and consumes its key, matching the existing begin
transaction's recoverability model. Its exact retry cannot allocate an object
after capacity later becomes available. A subsequent successful request must
use the next key. Failed copying does not charge admission; cancellation after
final admission does. Revocation observed before final admission denies the
operation without charging or publishing.

The stop-token read under the final mutation lock is the cancellation decision's
linearization point. A stop request after that point cannot be claimed to undo
the already accepted publication, even if receipt delivery has not occurred.
Independent safe-stop and actuator cancellation remain outside this cache API.

The prototype checks fork identity before each public lock acquisition and
before final publication. It does not cache a PID across calls or remove any
existing production fork check. Its scratch destructor refuses to touch an
inherited mutex in a child process. The final VM carrier must preserve complete
memory-index, alias, tag and bounds validation, and must use the existing full
FP protection unless every allocation-handler and nested-provider path has
separately passed the audited preservation contract.

## Shared state and integration requirements

`ordered_receipt` is a nonallocating replay helper with one shared mutation
mutex. The test coordinator borrows its state and mutex; it does not duplicate
begin/append/commit implementations. Tests seed an earlier operation into that
same stream and interleave publish/remove, verifying that operation kind and
exact bytes participate in replay identity.

Production integration must refactor the existing registry to share **its one
stream, object index, identities, capacity accounting and lock** across all
mutations. Installing this test coordinator beside the real registry would be
incorrect. No production helper, broker, authority, host entry, CLI, debugger,
catalogue registration or SDK implementation has been changed by the prototype.
Debugger state, recovery schema, compound admission, runtime memory validation,
native allocation policy and all existing output regression suites remain
mandatory integration work.

## Evidence and limits

`tools/ros_scene/complete_publication_checks.py` compiles the prototype against
the **frozen** qualified VM source, validates its full source manifest, and
generates embedded payloads from the fourteen exact previously qualified XML
outputs. Each payload's SHA256, FNV value, size and identity are checked against
the existing correctness receipt. The old adapter calls the actual frozen
broker's begin/append/commit/read/remove path. The new native adapter calls the
prototype publish/read/remove path and verifies identical readback bytes.

At a 4096-byte bound, that changes the actual native adapter invocation count
from 66 to 40: twelve nonempty objects use publish/read/remove, and two empty
objects use publish/remove. The once-only capability open is excluded from
both counts. This demonstrates the proposed reduction in carrier transitions;
it does **not** execute a new VM import, measure VM time or establish a speedup.
At a 64-byte bound, the twelve nonempty XML outputs exceed the proposed maximum
complete payload and must retain the old chunked path. Only the empty-object
path can benefit there; this proposal is not a blanket solution to that case.

Safety cases cover missing exposure/action dependencies, independent read/remove
authority, wrong owners and generations, short receipt and transfer bounds,
zero and insufficient optional budgets, exact owned copies, partial copies,
allocation and callback faults, exact replay, shared ordering, capacity failure,
empty objects, late cancellation/revocation, reentrant callbacks, competing
threads and fork rejection. Sanitizer/GCC/TSan results are separate prototype
receipts. They cannot be merged into the accepted VM's interface count or used
to certify an industrial deployment.

`complete-publication-prototype-controlled-final/results.json` records six successful
compile/execution commands: Clang ASan/UBSan, GCC 13 and Clang TSan. All ten
groups passed in each executable, with the real 66/40 native-adapter trace.
Allocation-injection cases run in the ASan/UBSan and GCC builds; TSan keeps its
own allocation interceptors. The allocation proof permits exactly the owned
request allocation and forces any subsequent allocation to fail. It also checks
that a failed later request preserves an already accepted receipt, object and
capacity. All prototype inputs and all 2641 frozen baseline source files were
unchanged across qualification. A maximum-size old append request is replayed
in the same bounded stream before publication; a stream sized only for the
smaller new framing is rejected. A stop request after checked copying, while the
shared final mutex is held by another thread, is rejected before publication.
Unknown action/exposure mask bits are rejected at construction. The observed
cgroup inode was 12409, with zero
memory events before and after. The earlier compile-failure receipt remains
preserved and is not the accepted prototype result. Historical `final` and
`shared-final` directories have separate `qualification-status.json` records
marking them superseded; their original receipts remain byte-for-byte unchanged.

All compilation and execution must remain inside the existing 64 GiB, zero-swap
cgroup. A reproduction uses a new output directory:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 10-11 \
  /usr/bin/python3 tools/ros_scene/complete_publication_checks.py \
  --source-root ../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-source-safe \
  --vm-build ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-lifecycle-safe/full-build-results.json \
  --xml-prepared ../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-xml-prepared \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/complete-publication-prototype-controlled-final \
  --thread
```

The receipt retains the prototype header, C++ driver and Python runner under
`source/`. Compiler commands use that C++ snapshot, while `-I` still points to
the manifest-verified frozen VM `src` tree. Workspace inputs and snapshots are
hashed before and after execution. To replay a retained runner after workspace
changes, pass `--repository-root` for this repository and
`--prototype-source-root` for the retained `source/` directory, together with
the same frozen-source, build and XML arguments and a **new** output directory.
The receipt separately reports whether those retained sources match the current
workspace; reproducing a prior version does not qualify later workspace edits.
