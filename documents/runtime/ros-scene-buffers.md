# Bounded buffers and sealed input leases

The buffer provider implements `buffer.create`, `buffer.write`, `buffer.seal`,
`buffer.read` and `buffer.describe` as purpose-bound in-memory resources. The
broker must independently admit the exact operation, environment, caller and
alias before invoking the provider. The provider does not grant permissions or
export a native pointer, file descriptor, device address or filesystem path.
Registration and VM execution qualification are tracked separately in the
semantic catalogue.

## Configuration and layout admission

A trusted binding supplies one immutable schema profile: its exact identity and
32-byte SHA256 descriptor digest. It also specifies finite object count, retained
data bytes, per-object bytes, transfer bytes, aggregate active copy workspace,
sealed leases, rank and element count. Dtype, byte-order and color-space masks
are explicit. Every zero is a zero allowance. Fixed capacities are independent
of optional broker rate attenuation and cannot be replenished by closing and
reopening a capability.

Profile identity text is bounded to 96 bytes and must be valid UTF-8 without
NUL. Maximum rank is eight. The implementation recognizes `uint8`, `int8`,
`uint16`, `int16`, `uint32`, `int32`, `uint64`, `int64`, `float16`, `bfloat16`,
`float32`, `float64` and `bool`. Byte order is explicitly `little` or `big`.
Color space is absent or one of `mono`, `rgb`, `bgr`, `rgba`, `bgra`, `depth`,
`yuv`; the binding must separately permit the selected value. These layout
labels do not themselves validate a model's image semantics, units or task
constraints. The consuming provider must validate those against its schema.

Strides are in bytes. Rank zero represents one scalar; a zero shape dimension
represents an empty tensor with zero bytes. Each dimension and the total element
count are checked before allocation. For nonempty layouts, sorting axes by
stride and checking each accumulated extent prevents overlap and overflow.
Padded and transposed layouts are supported. Broadcast/overlapping layouts are
rejected. The complete addressed extent must fit the declared byte count, which
must fit the exact per-object and aggregate reservations.

Object and data capacity is reserved before allocation using the current strict
project allocator. Object count bounds fixed metadata and allocation headers;
the byte counter is the declared data extent. Transfer copies have their own
workspace reservation. Whole-process memory and thread limits remain separate
deployment limits; these logical byte counters are not a replacement for the
configured cgroup.

## Canonical records

The five operations use the shared canonical scene codec: little-endian fixed
scalars, length-prefixed UTF-8 text/bytes, counted arrays and optional presence
encoded as `u64` 0 or 1. The exact ordered fields are those in
[`interface-catalogue.json`](../../tools/ros_scene/interface-catalogue.json).
[`buffer_contract.h`](../../src/uwvm2/imported/ros/scene/buffer_contract.h)
rejects trailing data, malformed UTF-8, unknown dtype/order/color names, unequal
shape/stride ranks, excessive extents and noncanonical option tags before any
provider effect.

`buffer.create` receives a `buffer_layout` and returns one resource handle.
`buffer.write` receives a handle, expected contiguous offset and bytes; its
result is the complete accepted byte count. `buffer.seal` receives a handle and
exactly 32 digest bytes, returning a capability/resource `buffer_ref` supplied
by the independently admitted broker. `buffer.read` receives a handle, offset
and exact byte count and returns a length-prefixed byte sequence. Describe
returns only layout, sealed status and provenance. Internal written-byte and
digest metadata are not silently added to the describe wire record.

The host asks `transfer_limit` before allocating/copying the incoming request;
the contract computes an action-specific ceiling. The codec itself does not
allocate. The complete result capacity must be checked before any effect.
Owned request copies of at most 512 bytes use fixed stack storage. Larger
requests must reserve their entire encoded extent from the same workspace
budget as detached reads, before allocation and guest copying. For a large
`buffer.write`, this includes its 24-byte framing, so a transfer of `N` bytes
needs at least `N + 24` workspace bytes. Concurrent copies add their charges.
The move-only workspace lease retains shared accounting after registry
destruction; copied storage is released before the charge is returned.
Revocation/fork invalidate use without preventing cleanup. Short fixed-result
queries require authority but allocate no large input copy and debit no rate.

## Creation, writes and immutable publication

A newly created object remains unpublished while its handle is copied through
the caller's sink. That sink runs outside the registry mutex. Reentry cannot
read or mutate the unpublished object. A false/throwing sink or revocation
before publication rolls the object back and returns its reservations.

Data allocation starts uninitialized. `buffer.write` appends a completely
checked contiguous range. Gaps, writes past the declared extent, partial writes
and mutation after sealing are rejected. Reads may access only bytes already
initialized; they never expose unused allocation memory.

Repeating an already written identical range returns the same accepted count
without another write or hash update. A different byte sequence or a range
crossing the current write cursor conflicts. This allows exact recovery when a
write succeeded but its result could not be delivered. It does not provide
arbitrary rewriting or seek behavior.

SHA256 is updated incrementally during each new contiguous write using the
bundled `fast_io` implementation. Seal requires the complete declared extent
to have been written, finalizes a copy of the hash state and compares the exact
expected digest. A mismatching digest leaves the object unsealed. A matching
digest makes its bytes immutable. Repeating seal with the same digest succeeds;
a different digest conflicts. Seal does not rescan the whole allocation.

Guest creation always produces `guest` provenance. A schema name, layout,
color-space label, buffer capability or digest cannot relabel those bytes as a
trusted sensor observation or authoritative robot state.

## Leases, closure and revocation

`pin` requires an independently admitted `buffer.read` operation and a sealed
object. It returns a move-only host lease backed by the real allocation. The
number of leases is finite. A lease can be retained in a model job or proposal;
merely copying a numeric `buffer_ref` cannot replace it.

Closing the guest resource removes that handle but leaves all data/object
capacity charged while a lease retains the allocation. The last retained owner
frees the data before returning its byte reservation. A lease can outlive the
registry safely because its data and accounting have independent shared
lifetime; destroying the registry retires the scope first.

Lease visits require current source authority to be held independently by the
caller. The visitor sees an immutable span and cached layout/digest only for
the duration of the visit. It must not retain the span after that visit; an
asynchronous provider retains the lease instead. Revocation prevents new visits
and closes guest handles while retaining the allocation until existing leases
are released. It does not recall data already borrowed or copied, stop an
accelerator or establish physical cancellation. Native providers must implement
their own cancellation/cleanup acknowledgement before dropping inputs.

Forked children cannot use inherited registry handles or sealed leases.
Revoked aliases cannot be rebound to reset resource accounting or rights.

## Exact reads and debugger inspection

Read requires the entire requested span to fit the initialized extent and
transfer bound. It either returns exactly that many bytes or fails; there is
no EOF clipping. A detached copy is made under the registry lock and delivered
outside it. Its workspace remains charged until the copy is destroyed, so
reentrant reads cannot bypass the aggregate workspace limit. A failed/throwing
sink consumes nothing.

`try_inspect` uses a nonblocking lock and returns `again` on contention without
changing its output. It reports alias-level retained bytes, objects, leases,
workspace and active handles. It reads no payload and exposes no digest,
native pointer, retry key or per-object private identity. The debugger should
use an aggregate row with `id=0`. Counters may reflect concurrent final lease
retirement; this snapshot is diagnostic data, never an admission decision.

## Verification

[`buffers.cpp`](../../test/0084.ros-runtime/buffers.cpp) exercises default-denied
bindings, exact ownership, layout/overflow/overlap rejection, unpublished-handle
rollback, partial-initialization protection, write and seal recovery, SHA256
vectors, retained capacity after close, lease revocation/lifetime, detached read
workspace, concurrency, fork isolation and canonical record parsing.

[`buffer_checks.py`](../../tools/ros_scene/buffer_checks.py) archives all local
dependencies and the bundled `fast_io` headers, then compiles from that snapshot.
Every compiler and execution runs inside the existing 64 GiB, zero-swap gate.
It supports Clang ASan/UBSan, GCC, TSan, AArch64, ARM32 and i686. Allocation
rollback is tested by lowering the actual test child process's host
`RLIMIT_DATA`, retaining the enclosing cgroup. This avoids falsely relying on
QEMU's guest `RLIMIT_AS`, which was observed accepting a mapping larger than
that guest limit. ARM32 QEMU can reuse its pre-reserved arena without increasing
the host data limit; that target additionally uses an explicitly recorded,
test-only malloc linker wrapper to trigger the same strict allocator's
null-return path. Production headers contain no allocation fault hook. Source snapshots, gate identity, before/after memory events
and the actual child limit are recorded with the results.
