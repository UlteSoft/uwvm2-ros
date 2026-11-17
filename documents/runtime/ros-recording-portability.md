# Bounded recording container qualification

`recording_mcap.h` supplies an allocation-free segment writer and immutable
message index for a closed MCAP profile. `recording_store.h` adds finite owned
storage, immutable segments, exact retry and transactional private replay.
`recording_contract.h` defines seven closed carriers and `recording_dispatch.h`
adds compound source authority and caller ownership. These remain backend
components. The seven `recording.*` and `replay.*` imports now have VM carriers,
deployment/CLI wiring and cached debugger projection. Their new combined VM
cohort is being qualified; the earlier core receipts below do not certify that
integration or a persistent recorder.
CLI, host carrier and debugger integration still need qualification. This
private volatile backend does not imply artifact publication or durability.

The codec emits complete uncompressed segments using the public ROS1 and ROS2
profile/encoding names. It preserves message sequence and both timestamps.
Schemas, channels and QoS metadata must exactly match immutable deployment
definitions. ROS2 channel metadata includes `offered_qos_profiles`. The public
format and profile rules are described by the [MCAP specification](https://mcap.dev/spec)
and [ROS profile registry](https://mcap.dev/spec/registry).

Each caller supplies a finite arena, message index and explicit scan limits.
The writer reserves its complete final trailer before every append. Failure to
append leaves the previous extent unchanged; exhaustion still permits sealing.
The reader verifies a whole segment before publishing its index count. Invalid
lengths, schema substitution, unknown channels, truncated records and excessive
record/message counts fail without allocating. Reading an indexed message does
not publish it, execute a command or change a simulation clock.

This initial profile rejects chunks/compression, attachments, summaries,
unapproved metadata and extension tails. Optional CRC fields are zero; outer
sealed-object SHA256 authentication is enforced by the private store. The
codec does not claim full MCAP-reader compatibility or physical disk quotas.
Compression will require independently reserved workspace and decoded bounds.

`recording-mcap-second/results.json` under SDK `.tmp/ros-scene-security` records
12 successful commands, all compilation at `-O3 -g`: Clang ASan/UBSan and GCC,
each with core and actual upstream interoperability variants. The native
comparison uses retained MCAP 1.3.0 sources from the acquired depthai-core tree,
with exact dependency hashes and copied source. Both ROS profiles are written
by the new codec and read by the upstream library, then written upstream and
indexed by the new codec. Sequence, timestamps, payload, schema and channel
fields are checked. All hot-path allocations are forced to fail during the
codec-only test, with no allocation attempted. External upstream-library
allocation is outside that claim. The first attempt is preserved: GCC rejected
a fixture indentation warning; it was not accepted as a complete qualification.

This evidence covers container semantics and memory bounds, not ROS bag
application portability, disk persistence, throughput or industrial safety.

The store copies all approved channel names, schemas and definitions at trusted
configuration. Nine silent fast_io allocations reserve every segment,
message index, replay selection and result scratch slot, plus the complete last
retry request. Every later core operation allocates nothing. Logical reservation
counts these arrays and owned definitions; allocator bookkeeping and the fixed
store object remain within the independent process/cgroup budget. Zero/default
configuration permits no use. A failed allocation returns `no_memory` directly,
without a terminating allocator or throwing convenience factory; partial local
owners release their storage before return. Capacity is independent of optional rate limits.

One writer uses keys `1, 2, ...`; only an exact retry of the most recent accepted
operation returns its old receipt. The comparison includes every source
correlation, sequence, timestamp and payload byte. The caller must independently
prove those source correlations; numbers alone confer no authority. Rejected
batches do not write a prefix or consume a key. Checkpoint seals a complete MCAP
segment and computes its SHA256. The next append needs a free reserved slot;
there is no eviction, automatic disk growth or hidden message loss. Explicit
stop selects drain or abort. Already sealed segments survive either choice.
These receipts describe volatile process-local acceptance, never durability.

Replay selects an exact ordered list of immutable objects and pins all of them.
Erase refuses pinned objects. A read returns complete messages, and advances
its ordinal only after successful delivery and a live-owner check. Failed or
throwing sinks preserve the cursor. Concurrent close defers pin release until
delivery exits. Retire prevents final consumption but cannot retract bytes a
sink has already seen. Seek changes only a message ordinal; replay does not
publish to ROS, execute a device command or change a simulation clock.

External import copies into an already reserved free slot, verifies the expected
SHA256, and validates the entire closed MCAP profile before publishing a new
local object identity. Truncated or malformed input publishes no object and
consumes no slot. The future dispatcher must hold the actual artifact read lease
and exact profile authority throughout this operation. Imported bytes are owned
by the store, so subsequent input mutation cannot alter replayed messages.

`recording-store-final/results.json` records nine successful commands across
Clang ASan/UBSan, GCC and ThreadSanitizer, each with `-O3 -g` and copied dependency
closure. Nine security groups cover owner/fork isolation, all-or-none batches,
exact retry, actual fast_io failure at each of nine allocations, no hot-path
allocation, complete MCAP indexing, SHA256 mismatch, truncation at every byte,
immutable pins, capacity, EOF/seek, false/throwing delivery and concurrent
close/retire. `recording-store-cross/results.json` adds nine commands that compile
at `-O3 -g` and execute the same nine groups on AArch64 and ARM32 under the
recorded QEMU/loaders, plus i686 under its actual loader. Each architecture uses
copied compiler-discovered dependencies; all run inside the same resource gate.
`recording-mcap-batch/results.json` records twelve further commands
including upstream bidirectional reader/writer comparisons after the batch
append change. These receipts do not substitute for future broker, real-Wasm, CLI,
debugger or persistence qualification.

The dispatcher binds each channel to one exact buffer alias and profile. An
append contains explicit buffer capabilities, handles, byte sizes and SHA256,
not raw guest pointers. The complete atomic authority group checks both
`buffer.read` and `data.export` for every item; repeated sources retain their
independent work costs. Actual sealed pins must contain the configured byte
layout and digest. All pins remain alive through the whole batch effect, and
no borrowed byte span escapes its visit. At most 63 items fit the bounded
128-claim group. Timestamp and sequence fields remain recording metadata;
they do not acquire trusted sensor provenance.

Every writer, checkpoint and replay belongs to its original runtime caller
incarnation. Private aliases do not make another caller's handles readable.
A replay alias selects one configured recording source. Opening a replay
requires the caller's exact ordered object IDs, sizes, message counts and
digests, plus a current source capability with `resource.observe` and
`data.export`. Reads recheck that same source capability; closing it prevents
new disclosure but does not erase retained pins. An independent replay
`resource.close` can still release those pins. Closing an unpinned checkpoint
reclaims its reserved slot; identities are never recycled.

The complete last accepted mutation carrier and receipt are retained per
writer. An exact retry validates current authority, returns the historical
receipt and does not debit work twice, even after the original input buffer
handle closes. Changed fields, skipped keys and earlier receipts are refused.
Open copy failures roll back the new handle; accepted mutation copy failures
retain the effect for exact retry. Checkpoint receipts are 104 bytes and have
explicit append/checkpoint/drain/abort tags. Reads stage a complete bounded
result using silent fast_io allocation; allocation or output failure does not
advance the cursor. Writer plus retained checkpoints also count against the
grant's resource capacity, independently of the physical segment reservation.

`recording-contract-first/results.json` records six successful `-O3 -g`
Clang ASan/UBSan and GCC commands. Its three groups cover all seven carriers,
every input truncation, invalid lengths/tags, duplicate objects, fixed receipt
sizes and forced-allocation-denial execution. Dispatcher qualification is
recorded separately; neither result registers a VM import by itself.

`recording-dispatch-seven-operations/results.json` records nine successful
`-O3 -g` ASan/UBSan, GCC and TSan commands, with 406 frozen source files. Eight
groups exercise every operation, including explicit drain/abort, independent
source permissions, whole-batch rejection, exact receipt recovery without
recharging, caller isolation, source-capability close, failed/throwing copies,
reentrant and concurrent retirement, actual allocation failure and fork. The
preceding `recording-dispatch-cross/results.json` executes the same production
headers on AArch64, ARM32 and i686; its earlier fixture omits the later added
dispatch drain/abort assertions. These source distinctions are retained rather
than merging evidence into a fictitious single build.

The configured native codec registry initially contains four exact schemas:
`ros1-std-bool`, `ros1-std-string`, `ros2-std-bool` and `ros2-std-string`.
They validate original ROS serialized bytes, rather than the separate canonical
scene communication encoding. ROS1 uses little-endian lengths and preserves
embedded NULs. ROS2 accepts only plain big/little-endian CDR with zero options;
its string validator requires a terminal NUL and rejects embedded NULs, matching
the tested Fast-CDR serializer. Unknown schema names, definitions, encodings,
CDR representations and trailing bytes fail closed. This is a finite compiled
registry, not generic ROS IDL support. `recording-native-codecs-third/results.json`
records eight commands comparing actual pinned ROS1 serialization and Fast-CDR
2.2.8 in both byte orders, including hostile lengths and every truncation.
The receipt retains compiler-discovered vendor headers, external system-header
hashes and the exact linked library; it makes no whole-package coverage claim.

Deployment uses `private-recording` and `private-replay`, with separate grants,
profile hashes and finite reservations. All nine recording capacity fields and
the profile must be present before a definition file is opened or hex decoded.
The canonical definition authenticates the complete ordered channel list,
compiled schema identity, schema bytes, source buffer alias/profile and maximum
message extent. A bounded regular-file transport additionally authenticates the
whole file's exact size and digest. No guest path, plugin symbol or general
filesystem authority is created. See the [CLI contract](../command-line/ros-scene.md).

`recording-configuration-storage-final/results.json` and
`recording-dispatch-inspection-final/results.json` each record nine successful
commands with `-O3 -g`, ASan/UBSan, GCC and TSan. They cover retained owned
configuration, allocation failure rollback, pure reservation sizing and cached
inspection without admission or cursor changes. The separate
`recording-sensor-proposal-debug-first/results.json` passed eight console/DAP/IDE
contract checks, including exact 64-bit counts and rejection of payload/effect
requests. It is a protocol/controller test, not a real VM or graphical IDE run.
