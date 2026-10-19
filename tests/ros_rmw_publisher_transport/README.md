# Owned native publisher transport

This package implements the additive `uwvm_ros2_rmw_v1` publisher path. It is
separate from the original81 business imports and the finite source facades.
The original RMW provider can use the new raw imports to retain one actual
native publisher across creation, QoS/GID inspection, publication and retirement.
Merely linking a namespace, obtaining a dispatch table or receiving a protocol
receipt does not grant authority or prove ROS compatibility.

These sources are being integrated with the independently maintained native
endpoint helpers, custom provider shards and concrete Guest C provider. The
source package has not been compiled or executed. Every native, Guest, default
denial, original-upstream, distribution, vendor and performance result remains
unqualified for these new bytes. The19 value groups and five C11 numeric import
declarations are authored contracts, not reported test passes.

## Exact authority and native ownership

The caller opens an explicitly configured `topic.publish` alias using its real
ROS2 business backend. Names, original type identity, complete canonical codec,
explicit QoS, bounds, rate and publisher count come from that immutable grant.
The new imports accept a source handle, native incarnation and linear-memory
offsets. They accept no ROS name, native type name, QoS setting, policy selector,
update request, debug request or process-management operation.

The Runtime recognizes this namespace in both real local-import caller paths
and selects its existing copy-based memory access guard. The Host obtains the
actual Runtime instance/generation, authorizes the full source handle, and keeps
that live lease through the bounded RPC and controlled output copy. The same
backend mutex serializes close/revoke and these operations. A busy new call
returns11 before effects. A missing source grant opens no native resource.

The existing authenticated Unix IPC verifies worker PID/UID, exact policy
manifest, session and monotonic request IDs. UMRE-v1 receipts preserve backend,
full source handle, capability instance, Runtime generation, native session and
nonreusing native incarnation. They contain two complete native QoS slots and a
complete native GID extent; the publisher uses one slot and reports its actual
native GID. A genuine all-zero GID is data. Different native/Guest GID widths
must be handled by the concrete provider without truncating or inventing a GID.

Gateway activation allocates history/object slots before real entity creation.
It obtains the actual selected built-in or full custom typed publisher factory.
Custom factories instantiate only their owned existing shard; their complete
original name, native hash/schema, codec identity and bounds remain mandatory.
The eager factory publishes no dummy message and has no old lazy-publication
fallback. Missing actual QoS/GID/utility APIs return95. The actual native object,
resource name, QoS and complete GID must stay the same on inspection.

One alias selects either the original legacy publication route or the new owned
route until its final close. Mixing routes returns17; a Guest cannot create an
additional lazy publisher behind the owned path's publisher-count limit. Several
actual owned handles are allowed only within the source grant's real count.

## Publication, retirement and bounds

UMRP-v1 transports one complete canonical body with its exact binding and
incarnation. The Host copies Guest bytes once into the final owned envelope,
validates the body against the real source grant, and debits that source's
business quota. The Gateway separately debits the canonical body and the actual
typed endpoint validates/decodes every field before its one native publication.
No type-erased native struct or arbitrary CDR payload is forwarded. The protocol
envelope is not charged as ROS payload. Fixed control receipts are exempt from
business payload-byte limits: a valid one-byte Bool grant can obtain metadata.
Native creation still uses the Gateway's independent activation quota.

Actual payload bytes must also fit the existing IPC request limit after its
fixed envelope and token allowance. Oversized input returns90 before native
publication. Zero logical canonical payloads remain zero; no dummy physical byte
is sent. The selected complete codec determines whether a type is really empty.

History keeps at most65,536 lifetime associations, including failed/retired
ones. Tombstones never evict or permit the same full handle to reactivate.
An indexed per-alias active count makes creation logarithmic in history size;
constructing many owners does not repeatedly scan the entire history. Allocation
failures roll back unpublished count entries before any native entity exists.
Native incarnations never wrap. Diagnostics retain the original native receipt,
retirement reason and separate once-only cleanup outcome.

Explicit retirement tombstones the local incarnation before one native cleanup
attempt. It leaves the raw capability open; the RMW owner must close that source
afterwards, including on cleanup error. Raw close/revoke first retires every
matching local row, then uses one absolute deadline for its entire cleanup
sweep. Exhaustion or a transport failure abandons the session, whose Gateway
cleanup guard releases retained native owners. There is no per-row fresh timeout
and no automatic business retry. Generic old RPC failures/reconnects and trusted
management retirement also invalidate these owned rows; an old session never
silently reopens an existing native source.

A metadata-only inspect that reaches its entry deadline before an RPC preserves
the live object for explicit later cleanup. A retirement at that same boundary
closes its transport to arrange native cleanup. After an RPC may have reached
native code, failure or malformed response abandons the session. A timeout can
mean a publication already happened; no status makes replay safe.

The native SDK itself can block in its publication or destruction machinery.
The resource owner must independently enforce the whole Gateway/process-tree
deadline and retirement. Source-level finite arguments do not prove that the
native SDK returned, that a subscriber received data or that native ownership
was ultimately released. Those require actual bounded runs and original logs.

## Development and later validation

`sdk/include/roswasm/rmw_publisher_v1.h` documents all five raw signatures. The
concrete original-RMW C provider is a separate source dependency and must use
these imports, the full256-byte parser and same-object receipts. It must not
fall back to original `topic_publish` when any new operation is unavailable.
Its bootstrap/profile, selected complete generated codecs, source aliases and
original installed SDK must outlive every RMW owner. All consumers require a
coherent new source build; earlier Runtime products do not implement this path.

Build SDK-free value contracts only after source implementation closes, in an
owned `/tmp` build directory:

```sh
cmake -S tests/ros_rmw_publisher_transport -B /tmp/owned-rmw-values \
  -DUWVM_BUILD_RMW_PUBLISHER_VALUE_CONTRACTS=ON
cmake --build /tmp/owned-rmw-values --parallel 1
```

Run each named group from the source's explicit `groups` table in a fresh
bounded process. Checks use always-active requirements, so Release cannot
remove setup or assertions. These groups cover actual wire values, malformed
frames, source/generation/session binding, native snapshot consistency, legal
zero GIDs, count/tombstone limits and cleanup claims. Their logical receipts
are labelled fixtures and are not native metadata evidence.

After a real original-header Guest build, inspect its actual binary with
`sdk/tools/check_rmw_publisher_imports.py`, explicitly allowing required WASI
modules and selecting `--require-all` for a full namespace witness. The checker
does not execute or fully validate Wasm. Use the real independent validator,
default-denied Runtime runs, exact-grant native delivery and final owner-discovery
checks afterwards. Native witnesses must verify no publication during activation
or inspection, one full-value publication on the admitted GID, refusal for a
foreign incarnation, and disappearance after complete ordinary cleanup.

Actual all-distro/vendor/QEMU runs and native/Wasm comparisons with hardware
VTune Hotspots/microarchitecture remain required. A small value model, import
inventory or native publisher witness is not complete ROS or rcl compatibility.
