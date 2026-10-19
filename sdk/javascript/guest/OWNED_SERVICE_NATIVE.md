# Optional owned ROS2 Service bridge

Status: **Source implementation; not compiled, executed or qualified.**
The bridge is absent from ordinary embedded JavaScript builds. It requires the
explicit ROS2 backend and optional producer selection, the exact catalog SHA256,
the complete original Wasm signatures, and a separate full binary import check.
Linker undefined-symbol admission alone does not check signatures or grant ROS
permissions. Existing business interfaces keep their own contracts.

The embedded module exports one function:

```javascript
import {invoke} from 'roswasm:rmw_service_native';
const receipt = new Uint8Array(256); // allocate before attempting activation
const status = invoke(1, [callOwner, observerOwner], new Uint8Array(0), receipt);
```

`callOwner` and `observerOwner` are full unsigned 64-bit `BigInt` values from
separately opened roles. This low-level example omits the owner ledger and
receipt validation required by the high-level owned Service SDK. Do not use it
as a complete application lifecycle. Numeric handles never grant authority;
the actual Runtime caller, policy, operation, live generation and native session
are checked by the original imports.

## Exact call table

`invoke(index, scalarArgs, input, output)` returns an unsigned 32-bit Number.
`scalarArgs` must be a genuine Array. Every listed argument is an unsigned
64-bit BigInt except `expectedWriter`, which is an exact unsigned 32-bit Number.
Both byte arguments must be genuine Uint8Array objects. Proxies, coercible
objects, clamped arrays and signed arrays are refused. C supplies memory0,
private scratch offsets and byte extents; JavaScript cannot choose an address.

| Index | Operation | Scalar arguments | Input | Output |
| --- | --- | --- | --- | --- |
| 0 | abi_version | none | empty | empty |
| 1 | client_activate | source, observer | empty | exactly 256 bytes |
| 2 | service_activate | advertise, responder | empty | exactly 256 bytes |
| 3 | endpoint_inspect | source, incarnation | empty | exactly 256 bytes |
| 4 | client_submit | source, incarnation | complete canonical request | exactly 8 bytes |
| 5 | client_abandon | source, incarnation, ticket | empty | empty |
| 6 | service_respond | advertise, incarnation, responder, ticket | complete canonical response | empty |
| 7 | endpoint_peek | source, incarnation, expectedWriter | empty | selected capacity, at least 352 bytes |
| 8 | endpoint_commit | source, incarnation, sequence, reservation | empty | empty |
| 9 | endpoint_cancel | source, incarnation, sequence, reservation | empty | empty |
| 10 | client_available | source, incarnation, availability, waitNs | empty | exactly 1 byte |
| 11 | endpoint_retire | source, incarnation | empty | empty |

Index0 returns the discovered ABI version, currently1. Other operations use
status0 for success. Any nonzero status preserves the caller's entire output
array. Validation throws before an original business import. The bridge copies
input to separate C storage before preparing output, so overlapping input and
output views cannot overwrite the submitted bytes. Scratch guards cover the
selected ranges and their fixed allocation boundaries. They supplement the
actual Host's range validation; they do not establish native memory safety.

Input and output capacities are the explicit existing producer limits. This
bridge retains their maximum4193786 bytes, which is narrower than the optional
ABI's4194304-byte delivery ceiling. A large configured capacity does not prove
that a body satisfies any endpoint's smaller canonical-message or grant quota.
Setup work scales with actual selected bytes plus fixed guards; ACK operations
do not clear both maximum-capacity arrays. No performance measurement exists.

## Owning records and uncertain outcomes

Activation/inspection writes one UMRE256 receipt; submission writes the full
eight-byte little-endian ticket. Availability writes one canonical byte0/1 and
requires its separate capability and finite deadline. A client GID differs from
the actual request-writer GUID. Validate complete receipts and their actual
source, incarnation, native session, types and both QoS profiles in the SDK.

Peek returns UMRC160 + URMI192 + a complete canonical body from the same take.
There is no second length slot. Validate UMRC `record_bytes` and the exact
associated metadata/body extent before using the prefix. The caller's capacity
tail is initialized with0xa5 on success and is not another record. Zero-valued
metadata is data; do not invent validity from nonzero tests alone.

Preallocate the high-level owner, result and cleanup ledger before calling an
operation that can acquire ownership or have a native effect. The bridge does
not construct result objects, BigInts or new byte arrays after the import.
It keeps both genuine backing buffers rooted through the call and copies
output only on status0. A Host trap or fatal scratch-guard failure leaves an
**unknown outcome** and requires instance/session retirement. Never retry an
uncertain send, response, commit or cancel. Keep primary, borrow/commit/cancel,
retire and each raw-owner close result separately. Local abandonment does not
claim remote DDS cancellation; activation does not imply response completion.

These checks are Guest-side conveniences. Code in one JavaScript realm can call
its own raw grants; a wrapper object is not a security membrane within that
principal. The Host must independently reject foreign, stale, wrong-operation
and ungranted owners. The optional module adds no update, policy, debug,
management or native plugin authority.

## Qualification fixtures

`owned_service_native_refusals.mjs` imports the real embedded C builtin and
exports27 deterministic validation cases, high-bit u64 conversion and a getter
reentry case, detached/OOB buffers and current resizable-buffer extents.
Separate successful ABI-version observations check tracking shrink-to-empty
and an explicit fixed-length view. It invokes only ABI-version discovery in that pass. Its separate
`deniedActivation()` uses invalid zero owners and retains the actual Host error
while checking unchanged caller output. These are unexecuted fixtures.

The Linux owner must still provide closed manifests, final full binary imports,
genuine QuickJS execution including the authored detached/resizable cases,
success/error output copying, aliasing, resource/allocator failures, trap and
unknown-outcome retirement, and real native ROS client/server delivery. Source
tests or a synthetic import provider do not qualify DDS, native ownership,
permissions, clock deadlines, or mixed ROS applications. DBG must distinguish
C bridge frames from JavaScript source/locals. x86_64/aarch64 QEMU, ROS DBG,
cache compatibility, hardware VTune Hotspots/microarchitecture and cross-engine
comparisons remain separate pending results, all in the existing SSH Linux
64 GiB cgroup.
