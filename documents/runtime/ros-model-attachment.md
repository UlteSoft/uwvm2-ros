# ROSv2 externally provisioned model attachment

The VM connects to an already running model provider. Deployment owns the
provider executable, runtime, weights, sandbox, CPU/NUMA policy, cgroup, recovery
and termination. The VM owns only a duplicate of a registered channel and a
local supervisor thread. The deployment implementation is outside this change.

## Authority and registration

The existing model and request imports remain in `uwvm_ros_v2`. Exposure and
exact model/input grants remain mandatory. Registration grants no guest access.
Model bytes are data, never an implicit actuator command. Configure model
identity, codec, aggregate budget and per-alias capacities before registration:

```text
--ros-scene-model-channel ENV ALIAS FD PEER_PID PEER_UID PEER_GID DEPLOYMENT_ID VM_ID SCOPE_ID PROVIDER_ID SESSION_ID CONTRACT_SHA256 REQUEST_BYTES RESPONSE_BYTES STARTUP_NS POLL_NS SUPERVISOR_BYTES SESSION_UNTIL_NS IDENTITY_FD PEER_TIME_NAMESPACE_FD
```

The `IDENTITY_FD` argument names a sealed Linux memfd containing the
32-byte local Ed25519 seed followed by the exact 32-byte peer public key.
The provisioner creates a distinct identity pair for each session, applies
all four seals (write, grow, shrink and seal), and passes only the local
identity descriptor to each endpoint. CLI logs contain its descriptor number.
The seed remains outside Wasm memory. Both endpoints authenticate with TLS 1.3
raw public keys, derive session keys with ephemeral X25519 and protect records
with AES-128-GCM. The authenticated ALPN transcript binds the service purpose,
deployment, VM, scope, provider, session and contract digest. The receiver
also checks kernel credentials and the pinned process incarnation. Registration
and a successful handshake confer no guest operation permissions.

Budget for the TLS records and handshake in addition to the application
payload. Backpressure may require retrying the same frame under current
authority. Production registration fails when the TLS backend or the sealed
identity is unavailable. The plaintext kernel-channel option belongs to
explicitly compiled historical test fixtures.

`PEER_TIME_NAMESPACE_FD` supplies trusted, read-only clock provenance from the
provisioner. The same [namespace evidence contract](ros-driver-attachment.md#registration)
applies to model providers: the descriptor must refer to the pinned peer's time
namespace, match the VM's clock and remain open through provider preparation.
Neither the VM nor the provider needs ptrace rights over the other user. An
invalid descriptor, wrong namespace type or different clock is rejected. This
descriptor is not a socket, process-control grant or proof of vendor containment.


IDs are nonzero 128-bit values encoded as 32 lowercase hex characters. The
nonzero digest is 64 lowercase hex characters; hex bytes retain wire order.
The trusted contract maps scope to the admitted model/task/schema and authority
environment. Guest bytes cannot choose the binding.

The inherited Linux AF_UNIX SOCK_SEQPACKET endpoint must have SO_PASSCRED
enabled. Every incoming packet needs the configured PID/UID/GID. A pidfd pins
the peer incarnation. Descriptor passing, unknown ancillary records and
truncation are rejected. The VM duplicates only its endpoint with CLOEXEC.
The provisioner keeps endpoints exclusive. Duplicate configured descriptor
numbers and session identities are refused across model aliases; provisioning
must also exclude different descriptors referencing the same kernel socket.
The monotonic time namespace must match. Other namespaces may differ when
peer identity remains observable. Cgroup membership is neither an address nor
proof of sandboxing. No cgroup, namespace or process-control permission follows.

Request/response reservations range from 24 bytes to 4 MiB. Fragments contain
at most 65536 bytes. Startup is positive and at most 60 seconds; polling is
positive and at most 100 ms. Local supervisor stack is 64 KiB through 16 MiB
and must be page aligned during preparation. Session expiry is a positive
signed-64-bit monotonic timestamp. These finite reservations do not promise
real-time execution. Kernel queues and external RSS/CPU belong to deployment.
Optional rate reduction remains separate capability policy.

## Framing and outcomes

The 176-byte little-endian header contains magic `0x4d534f52564d5755`, header
size, kind, reserved zero, five 128-bit identities, 256-bit digest, sequence,
deadline, payload length and fragment offset. The model magic is distinct from
driver traffic. Unknown kinds, flags, trailing data and invalid lengths refuse.

Kinds are hello (1), ready (2), release (3), request fragment (4), request end
(5), response fragment (6), response end (7). Hello/ready have zero sequence,
deadline and offset. Ready echoes the exact model and task profiles, parameter
type, output type, encoding, request/response bounds and expiry from hello.
Agreement is not an attestation of the peer's sandbox or implementation.

One job is outstanding. Sequences start at one and never wrap. All data frames
echo the complete binding, sequence and deadline. Fragment offsets must be
contiguous. End frames have no payload and bind the exact nonzero assembled
extent. Request bodies use `model_worker_protocol` invocation encoding. Each
response is one bounded partial or terminal record with the model-job identity.
Streaming outputs consume the existing cumulative item/byte reservations.

Current model and input authority is held across each bounded nonblocking send.
Polling happens outside authority locks. Copying input never authorizes sending
it after revocation. Output still passes the existing authority-aware facade.
Revocation cannot recall bytes already disclosed to an external process.

A valid terminal response is provider-reported completion, not proof of remote
erasure or physical safety. Timeout, crash, malformed response or revocation
after disclosure retains `outcome_unknown` and its input reservations. Only
trusted external reconciliation can settle cleanup debt. Local retirement
closes the duplicate and joins the VM thread; it never signals or reaps the
provider. Best-effort release abandons this session only. Independent external
expiry and recovery remain mandatory when release is lost.

VM-owned request, response and fragment buffers use recoverable fast_io storage
and secure erasure before reuse or release. This cleanup is mandatory. The
own-input erasure option for cross-Wasm exchange is a different interface.

## Migration and qualification

The unreleased model-worker launcher, image, placement and limit CLI options
are removed. Historical source fixtures and receipts describe the former
VM-owned launcher and do not qualify this attachment path. Native model
packages need an external protocol adapter; arbitrary binaries do not
automatically speak this protocol.

`model_attachment.cpp` exercises real processes, streaming, crash, timeout,
changed identity/sequence/deadline/offset, forbidden descriptor passing,
revocation and retained cleanup debt. `run_model_attachment_vm.py` exercises
actual Wasm imports across both memory widths, memory indices and VM engines.
Receipts bind the exact executable and source; these focused fixtures do not
establish arbitrary LLM/GPU integration, industrial certification or complete
robotics package coverage.
