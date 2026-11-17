# ROS scene volatile cache outputs

`uwvm_ros_v2` exposes bounded, transactional cache output through the
`volatile-output` binding and the `cache` purpose. A deployment explicitly
creates an environment, assigns its modules, exposes individual imports,
grants exact actions to an alias, and supplies finite output capacities. All
these steps are required before a guest can obtain a usable capability.
WASIp1 preopens and ROS v1 configuration confer no scene authority.

This profile owns private heap byte arrays. It accepts no paths, native file
descriptors, socket addresses or provider callbacks. Publication makes
initialized bytes immutable and readable within the live deployment. It does
not persist a file, validate a schema or content digest, trust generated
content, authorize a model or actuator, or promise crash recovery. A persistent
recording, log, model or robot configuration requires its own reviewed provider
and purpose. The publication receipt always leaves the durability flag clear.

## Configuration and permission flow

The relevant configuration records are:

```text
--ros-scene-output-budget ENV BYTES OBJECTS RECEIPT_BYTES
--ros-scene-output ENV ALIAS BYTES OBJECTS OBJECT_BYTES TRANSFER
--ros-scene-grant ENV ALIAS cache RESOURCE volatile-output ACTIONS
```

The aggregate environment budget must cover the sum of every configured
output binding's byte reservation, object slots and retained receipt storage.
Output bindings are prepared before guest activation; static validation does
not allocate their payloads or execute an input provider. Failed preparation
does not expose a partially prepared environment.

The binding's allowed actions are `artifact.begin`, `artifact.append`,
`artifact.commit`, `artifact.abort`, `artifact.remove`, `artifact.status`,
`artifact.receipt` and `artifact.read`. Each action still needs its exact
interface exposure and capability grant. The profile does not accept
`artifact.open`, `artifact.describe` or `resource.close`. Capability close
releases the capability handle only; it does not acknowledge a receipt,
delete an object, replenish an allowance or reset the sequence.
Publication does not grant read authority: `artifact.read` must be granted
separately even to the producer. A status or receipt grant exposes bounded
metadata only and does not authorize content access.

This follows zero authority → explicit grant → stable authority → optional
attenuation. Finite retained capacity is an explicit deployment reservation.
Optional call/byte rate attenuation separately requires `delivery=may-reject`;
it can reject an otherwise authorized request and is never silently enabled.
Retries and read-only queries require current authority and consume normal
admissions. Reopening a capability does not reset the grant's rate accounting.

## Wire requests and operation results

All header fields are little-endian unsigned 64-bit integers. Imported output
operations use the existing checked carrier with a capability, memory
selector, request address and size, response address and capacity, and length
address. The operation-specific request records are:

- `artifact.begin`: `[sequence, reservation]` (16 bytes).
- `artifact.append`: `[sequence, object, expected_offset]` followed by raw
  payload (at least 24 bytes).
- `artifact.commit`: `[sequence, object, expected_size]` (24 bytes).
- `artifact.abort` and `artifact.remove`: `[sequence, object]` (16 bytes).
- `artifact.status`: `[object]` (8 bytes).
- `artifact.receipt`: `[sequence]` (8 bytes).
- `artifact.read` uses its existing range carrier request
  `[object, offset, requested_bytes]` (24 bytes) and returns raw bytes.

The first seven operations return a 72-byte record of nine little-endian
unsigned 64-bit fields, in this exact order:

```text
sequence, operation, object, effect_status, accepted_bytes,
total_bytes, retained_bytes, state, flags
```

Mutation operation codes are begin=1, append=2, commit=3, abort=4, remove=5.
`artifact.status` identifies that object's last successful mutation; a later
failed operation does not overwrite this identity. Status always reports zero
`accepted_bytes`. `artifact.receipt` returns the saved mutation record exactly.

States are absent=0, staged=1, published=2, aborted=3 and removed=4. Flag bit 0
means published. Bit 1 is reserved for durability and is always zero here.
Unknown or foreign object failures redact the supplied ID and object metadata.
Terminal abort/remove receipts retain the owned object's ID and content length
but report zero retained allocation and no publication flag.

A successful carrier means the receipt reached guest memory. The guest must
inspect `effect_status` to learn whether its mutation succeeded. Accepted
effect failures, such as exhausted object capacity, still produce a receipt
and consume the sequence. An invalid shape, sequence conflict or failed
authorization does not replace the previous receipt. Begin allocation failure
returns `no_memory` before either an object effect or receipt replacement.

## One ordered recoverable mutation stream

Each alias has one stream shared by all its capability handles and callers.
Its first mutation sequence is 1. A request with the next sequence explicitly
acknowledges the last receipt and replaces it with the new outcome. Exact
replay of the last sequence, operation and request bytes returns the saved
receipt without repeating the effect. Changing one payload byte under that
sequence is a conflict. Zero, older, skipped or future sequences also fail
without effects. Once the final representable sequence is used, it cannot wrap.

For example, a guest may begin with sequence 1, append at offset zero with
sequence 2, and commit the exact initialized length with sequence 3. If guest
delivery of sequence 2 fails, repeating those same bytes returns the original
append outcome. `artifact.receipt(2)` also recovers that outcome without
executing append. Sending sequence 3 acknowledges receipt 2; receipt 2 is then
unavailable. Status and reads never acknowledge a receipt.

The guest must recover a required result before submitting the next sequence.
Concurrent producers sharing an alias must coordinate this single stream. Use
separate deployment aliases when independent producers need independent
ordering, quotas and recovery. This deliberately avoids an unbounded ledger
of historical retries.

## Publication, capacity and lifetime

Begin allocates the entire requested reservation before recording an effect.
Append must match the current initialized length and fit the remaining
reservation and transfer allowance. Commit must supply the exact initialized
length; it makes those bytes immutable. A staged object cannot be read.
Published objects cannot be appended or committed again under a new sequence.

The complete allocation remains charged after commit, including unused tail
space. Abort releases a staged object; remove releases a published object.
Both free the actual byte array before refunding retained byte capacity and
the object slot. A zero-byte object still occupies an object slot. Zero byte,
object, extent or transfer bounds always mean zero allowance, never unlimited.
The grant's `max-resources` is also enforced for new objects across its handles.

Each binding preallocates `TRANSFER + 24` bytes for the last exact request and
retains a fixed 72-byte receipt, so its recovery reservation is
`TRANSFER + 96` bytes even before the first mutation. Receipt replacement uses
that fixed storage and cannot fail by allocating after publication. Each
binding retains at most one recovery receipt; terminal receipts do not need a
live object allocation.

The payload and receipt budgets do not represent total process RSS. Registry
metadata is bounded by configured bindings/object slots; debugger snapshots
have their own row bounds. A host append copy and published-read copy are
temporary working memory. Before allocating an append copy, the host checks
exact live authority and the binding's request bound without charging; the
host also checks available call/byte allowance when a complete receipt can be
returned. This preflight neither refreshes a rate window nor changes admission
or refusal counters. A short response buffer reports the 72-byte requirement
without allocating the payload copy or consuming allowance. The effect path
repeats admission and validation and atomically charges; a concurrent operation
can therefore exhaust allowance after a successful preflight. Rate limits
govern admitted operations and do not promise a bound on total VM CPU or RSS.
For a published read, `copy_read` acquires the output registry lock once and
verifies ownership, publication, range and the original requested transfer
before allocating its owned copy. The same lock protects the object through
copying the actual available extent. It then releases the lock; cancellation
and checked guest delivery operate on the detached initialized copy. No guest
memory callback executes under the output registry lock. Thus an excessive
request at EOF cannot evade the transfer bound, and a zero-transfer binding
cannot force a large copy allocation. A concurrent remove cannot free the
source during the copy, and no live source pointer escapes to guest delivery.

The synchronous transfer scratch buffer owns up to 512 bytes on its stack.
Larger bounded requests use an owned allocation without first zeroing bytes
that the checked reader will completely overwrite. Neither path exposes a
borrowed guest-memory span to the registry. Parsing starts only after the
entire checked request copy succeeds; read delivery starts only after the
entire returned extent has been initialized. Allocation failure publishes no
new scratch extent. Scratch ownership ends at the call boundary and does not
replace the binding's retained payload or receipt charge.

Artifact and output operations use exact action bits derived from configured
names when authority is frozen. Each dispatch validates its trusted operation
index before a shift. The bit test replaces repeated string-tree searches;
live caller membership, interface exposure, handle ownership, revocation,
optional attenuation and the admission lease are still checked. It does not
accept guest masks or give a family of operations implicit permission. The
independent broker entry retains its request-size check, as well as the host's
pre-allocation check and the provider's effect validation, preserving short
receipt, direct-broker and cancellation error behavior.

The global maximum transfer is 4 MiB. Aggregate process memory is also subject
to the deployment's operating-system resource controls. Qualification uses
the existing 64 GiB, zero-swap cgroup; no test raises or bypasses that limit.

Revocation or environment retirement denies further access, including receipt
recovery. Cached diagnostic metadata and bounded allocations can remain until
the retired registry is destroyed. Revocation does not silently publish,
refund or reset a transaction. On Linux, a fork-inherited registry rejects
operations before acquiring its inherited mutex. Other operating-system
branches currently reject this binding as unsupported.

## Safety and portability qualification

`test/0084.ros-runtime/output_security.cpp` exercises six groups against the
actual registry, broker and deployment: lifecycle/replay, ordering/capacity/
ownership, allocation failure atomicity, delivery/admission, deployment
reservations, and concurrency/fork/revocation. Allocation injection checks
array and map-node failures before begin effects, allocation-free replay and
commit, and rejection of an excessive read or append before staging memory.
Zero/exhausted call and byte allowances reject preflight without allocation;
repeated previews do not reserve allowance, and later charged admission still
rejects a request when another admission has consumed the remaining budget.

Run the complete native, sanitizer, thread and reviewed cross-ABI suite with:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 0-3 \
  /usr/bin/python3 tools/ros_scene/output_security_checks.py \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/output-core-qualified \
  --recipes .tmp/gc-allocation-fix-20261113/build --thread
```

Choose a fresh output directory to preserve each run. The runner records
compiler commands, stdout/stderr, source hashes, cgroup limits and memory
events. Completion requires every selected check to pass and input hashes to
remain unchanged throughout the run. Native Clang ASan/UBSan, GCC and TSan
checks complement actual AArch64, ARM32 and i686 binaries. QEMU-backed runs
qualify ABI semantics only. The native `-U__linux__` probe verifies the explicit
unsupported branch; it does not claim a foreign operating-system qualification.
TSan keeps its own allocator interceptors, so its run omits allocation fault
injection; the native ASan/UBSan, GCC and cross-ABI runs exercise those faults.

The separate [upstream XML output qualification](ros-upstream-output-portability.md)
uses actual upstream serializer source and native/Wasm byte comparisons. These
slices establish bounded cache behavior and selected portability evidence;
they do not establish whole-package ROS compatibility, persistent-storage
semantics or near-native performance for the complete robotics ecosystem.

The final core run for this change is preserved in
[`output-core-final-quota/results.json`](../../../uwvm2-ros-sdk/.tmp/ros-scene-security/output-core-final-quota/results.json).
It records `complete=true`, unchanged input hashes and all 16 compile/run
commands exiting successfully: Clang ASan/UBSan, GCC 13, both unsupported
probes, TSan and AArch64/ARM32/i686 execution. TSan initialized on its first
attempt with no diagnostic. The existing cgroup retained its 64 GiB memory
limit and zero swap; memory events were all zero before and after. The
requested CPU set was 0–3 and the effective inherited affinity was 1–3.
