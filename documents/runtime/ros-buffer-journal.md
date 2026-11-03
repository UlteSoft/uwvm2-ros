# ROS buffer journal

`--ros2-buffer-audit <run-id> <inherited-fd>` enables a separate, bounded Linux
host journal for the generic ROS operation path. It changes no Wasm imports,
capability grants, memory permissions, ROS results, or native retry policy. It is
disabled by default. This interface is currently supporting integration work;
it does not establish production source approval or official scene acceptance.

The launcher creates a new private regular file, mode `0600`, opens it write
only, and passes that descriptor explicitly to the VM. Descriptors 0–2, pipes,
sockets, read/write or read-only files, nonempty files, multiply linked files,
foreign-owned files and repeated configuration are rejected. The VM consumes
the supplied descriptor in its process and retains a close-on-exec duplicate.
The parent must close its own writable copy after launching. Guest stdout and
stderr cannot serve as this journal. The admitted launcher must separately
authenticate the actual VM executable, source, process birth and output inode;
passing an FD alone is not proof of that admission.

Run IDs contain 1–128 ASCII letters, digits, dots, underscores or hyphens and are
case sensitive. Use a fresh run ID. `uwvm-ros-buffer-journal-v1` JSON Lines records
include a begin marker, ordered operation records, and an end marker with loss
counters. The default limit is 20,000 operation records, 64 MiB total, 32 KiB per record,
and 8 KiB per canonical input or output. A record whose combined data cannot fit
is dropped, never silently truncated. An absent end marker, sequence gaps,
nonzero dropped/suppressed counts, truncated JSON or changed source identity
disqualifies the journal. Journal failure does not change ROS execution or
retry a possibly executed operation.

An explicitly sized invocation may use this sequence (the limits option must
immediately precede the existing audit option):

```
--ros2-buffer-audit-limits <payload-bytes> <operation-records> <total-file-bytes> <record-bytes> <profile-sha256>
--ros2-buffer-audit <run-id> <inherited-fd>
```

The original defaults remain unchanged. Decimal quotas must be positive;
payload capacity is at most 4194304 bytes on each side (the existing IPC ceiling), operation count at most
4294967295, total file capacity8192 through4398046511104 bytes, and record
capacity1024 through33554432 bytes including the newline. These are implementation
ceilings, not allocated resources or recommended task budgets. The launcher
must reserve sufficient disk and prove complete source-specific phase bounds
before execution. A smaller record capacity may reject a combined input/output
record even when each payload fits; rejection is counted, never truncated.

The profile reference is exactly64 lowercase hexadecimal digits, not all zero.
It is recorded as requested_profile_sha256; it is caller-supplied attribution,
not an approval signature. The admitted launcher must separately authenticate
the profile, exact quotas, source/ELF/module identities, runtime and collector.
Limits are configured once before opening the journal. No command can change
them after its begin marker. Missing, duplicate, late or orphaned limit options
fail closed. All modules/preloads/libraries in one process share the same quota;
per-instance allocations must sum into it, never multiply it.

The channel allocates one mutex-protected record buffer at configuration, exactly
its selected record capacity (32 KiB by default, at most 32 MiB). Each active,
observed generic operation may allocate a detached input copy no larger than
its selected payload capacity (at most 4 MiB). Disabled observation allocates
neither buffer. These bounds do not grow with record count or disk quota.
Allocation failure before opening the journal rejects configuration; an input
snapshot allocation failure counts as a capture drop and leaves ROS execution
unchanged. Oversized inputs count as payload suppression, never missing source
identity. Input ownership moves to IPC only after the observed copy is retained.

The channel reserves 4096 bytes for the footer. Begin/footer do not consume
operation records, but count toward total file bytes. The footer records
attempted/emitted operations, source/payload/count suppression, record/byte/I/O/
capture drops, bytes before the footer and complete file size. Any loss
invalidates complete observation even if sequence numbers have no gap. An I/O
failure can prevent a complete footer; absence is always failure. These bounded
allocations do not prove acceptable latency: copying, allocation, disk I/O and
the writer mutex require independent performance and lifecycle testing.

The executable return and WASI `proc_exit` boundaries explicitly finalize the
journal, including nonzero guest exits. A destructor provides an idempotent
fallback. A crash, fatal signal or forced kill may leave no end marker and is
not a complete journal. The end marker proves neither successful guest exit nor
process-tree retirement; the collector must check both independently.

Operation records distinguish:

- A real guest input copy from authorization and IPC transfer.
- A request that might have affected the peer from a checked response frame.
- A checked response from response authorization and actual guest payload and
  output-length writes.
- A short buffer that receives only the required length from successful payload
  delivery. Canonical output bytes are reported only after the payload copy.

The record binds the actual source-owned whole Wasm digest, module name,
capability-manager instance number, runtime generation and builtin function
index. The IPC fields come from the actual client call under its mutex, after
request ID allocation and response correlation checks. They are internal
metadata; the IPC wire ABI is unchanged. A malformed reply preserves possible
peer effects and never claims a checked response or native ROS acceptance.

The private session credential is not printed. Its correlation tag is SHA-256
of the following exact concatenation:

```
ASCII "uwvm-sdk-ros-native-session-tag-v1" + one NUL
uint16_le(run_id byte length) + ASCII run_id
uint8(backend: 1 or 2) + uint64_le(actual authenticated session credential)
```

The native producer must independently record the same run/session/request key,
its authenticated process identity, and its real serialization, callback and
queue metadata. The VM records canonical guest bytes; it does not fabricate
native CDR hashes or infer RMW acceptance from a successful IPC reply. CDR bytes
on publisher and subscription sides may differ while their typed values agree.

Current limits of coverage: the journal covers `host::operation` after a valid
capability description. Unresolved caller/handle or capability-open denials,
the separate unified event ABI, and the RMW-specific APIs are not covered.
Source identity currently uses the existing one-defined-unshared-memory source
facts profile; unsupported module shapes suppress records and cannot qualify.
Static/dynamic/multi-memory scene acceptance, authenticated production enrollment,
full native/guest correlation, and the performance cost of enabled observation
remain separate requirements. No physical robot qualification is implied.
