# ROSv2 externally provisioned communication attachment

The VM owns a session with an existing ROS provider. A deployment script or an
independent deployment service owns process creation, sandbox policy, CPU/NUMA
placement, resource domains, termination, restart and updates. The VM neither
loads provider code nor launches, signals, reaps or moves the provider. Native
libraries, including original vendor shared libraries, require an external
adapter process. Registering a binary is not sufficient to implement a protocol.

## Scope and authority

The current implementation attaches a pair of explicitly configured native ROS
participants. It routes bounded RTPS discovery and user packets between their
approved directional pipes. Context/node authority is required on both sides
before activation. Topics, services, actions, schemas and graph observations have
separate grants and dependency checks. Registration alone grants no Wasm access.
Shared native drivers use the separate driver protocol and receiver-side control
arbitration. This two-participant communication fixture is not an arbitrary ROS
network, a DDS Security implementation or a complete ROS1 bridge.

The provisioner supplies one connected AF_UNIX/SOCK_SEQPACKET control endpoint
with SO_PASSCRED, and seven exclusive nonblocking anonymous pipes. Directions
are specified from the VM's perspective:

1. RPC request write.
2. RPC response read.
3. Diagnostics read.
4. Discovery write.
5. Discovery read.
6. User-traffic write.
7. User-traffic read.

The VM duplicates these descriptors with CLOEXEC. It validates pipe kernel type,
access direction, nonblocking status and distinct device/inode identity. It pins
the peer process with pidfds and verifies every control packet's kernel-supplied
PID/UID/GID. Descriptor passing, truncation and unexpected ancillary data are
rejected. The provisioner must exclude other writers/readers and duplicated
registrations across independent VM processes. Pipe identity receipts prove
agreement, not containment or honesty of an untrusted native process.

Absolute deadlines require the same monotonic time namespace. PID, user, mount
and network namespaces may differ if the configured credentials remain visible
and valid from the VM. A cgroup is a resource domain, not a connection address.
The protocol grants no authority to create cgroups or enter namespaces.

## Trusted CLI registration

Configure aggregate communication budgets, load exact hashed semantic startup
definitions and set every local RPC/adapter reservation. Then register each peer:

    --ros-scene-communication-channel ENV WORKER CONTROL_FD PEER_PID PEER_UID PEER_GID REQUEST_WRITE RESPONSE_READ DIAGNOSTIC_READ DISCOVERY_WRITE DISCOVERY_READ USER_WRITE USER_READ DEPLOYMENT_ID VM_ID SCOPE_ID PROVIDER_ID SESSION_ID CONTRACT_SHA256 STARTUP_NS SESSION_UNTIL_NS STDERR_BYTES

Identity fields contain nonzero 128-bit values in 32 lowercase hexadecimal
characters. The nonzero digest contains 64 hexadecimal characters. Startup is
positive and at most 60 seconds. Session expiry is a positive signed-64-bit
monotonic nanosecond value. The cumulative diagnostics allowance is at most
65536 bytes. Zero capacity grants no extra resource or implicit traffic rate.

The deployment configuration rejects duplicate descriptor numbers and session
identities across registered communication environments. Pair preparation also
rejects duplicated kernel pipe/control identities, including dup aliases.
Definitions and endpoint registrations are frozen before use. A new provider
incarnation requires a fresh registration and session identity.

The former worker-launcher, worker-image and worker-placement VM options are
removed. Native image and process limits belong to deployment. The remaining
worker-limit command configures local RPC, endpoint and graph reservations only.

## Control framing

Control records use a 176-byte little-endian header: magic 0x43534f52564d5755,
header size, control kind, reserved zero, deployment/VM/scope/provider/session
identities, contract digest, zero sequence, exact session expiry, payload length
and zero reserved offset. This domain magic is distinct from driver and model
protocols. Kinds are hello (1), ready (2), activate (3), attached (4), release (5).

Hello carries the startup profile identity and SHA256, then the seven pipe
roles, device/inode identities and VM-side directions, followed by the diagnostics
allowance. Ready must echo every immutable byte. Activation and its receipt reuse
the same identity. A mismatch permanently fences the session. Begin and activate
advance one bounded nonblocking operation at a time under current authority.

The separate RPC and RTPS carriers retain exact generation, sequence, size,
schema and direction checks. One local RPC is in flight per peer. Expiry and
session-stop state are checked before each bounded I/O step, including relay
writes after poll. Checks cannot recall bytes already accepted by the kernel;
the receiver must independently enforce command deadlines and authority epochs.
Any relay framing, identity, pipe or expiry failure first fences both shared
RPC sessions. The supervisor then retires the pair's creation ledgers, including
pairs with no graph capabilities. Completed retained receipts stay inspectable;
failure does not turn an unknown effect into a confirmed cancellation.

## Failure, erasure and lifecycle

Every partial write, unverified result or lost peer is conservatively classified.
A closed connection, expired deadline or dead pidfd does not prove that an effect
was rolled back or that remote copies were erased. Once attachment information
has been disclosed, session retirement retains remote cleanup debt. Local
endpoint-close receipts are distinct from process-wide cleanup evidence.

The relay erases forwarded/discarded queue bytes and assembled frames. RPC erases
sent request slices, discarded frames and acknowledged replies; retained complete
receipts remain available until acknowledged. Failure and destruction clear
owned staging storage. Diagnostics and rejected control packets are cleared on
all exits. Secure clearing uses fast_io and is not removed by optimization.
Erasure concerns VM-owned memory; it does not promise removal from other processes,
kernel queues, CPU registers, swap or device memory.

Disconnect sends a best-effort session release and closes only local duplicate
handles after users drain. Release never grants a native kill/restart operation.
Independent provider expiry is required because a crashed VM cannot deliver
release. Trusted deployment must reconcile unknown effects before resuming motion.

## Qualification and test deployment

communication_test_deployer.py is a test-only deployment simulator. It starts
external gateways, gives the stopped VM its exclusive descriptors, registers the
exact VM PID with each gateway, then executes the VM. It verifies VM exit did
not kill the gateways and performs its own bounded cleanup. The gateway owns
the pinned original Jazzy/Fast DDS sandbox. No production deployer is added.

The attachment security fixture checks hostile identity/header/profile records,
forbidden descriptor passing, direction errors, duplicate pipe aliases, failed
preparation rollback, expiry, close fencing and observable buffer erasure.
Provider fixtures exercise native topic/service/action/graph semantics. The
actual VM fixture additionally covers memory32/memory64, nonzero memory indices
and interpreter/AOT entry points. Receipts qualify only their exact source and
binary cohorts; they do not establish complete package/platform/language coverage,
hard real-time behavior or industrial safety certification.

The current regression entry point is
tools/ros_scene/external_attachment_checks.py. It freezes the source closure
and serially compiles Clang ASan/UBSan and GCC variants with -O3 -g, including
recoverable fast_io allocation failures for relay and node-stage setup. The
test provisioner can remain in the same process for deterministic hostile-frame
checks; the actual-VM deployment fixture uses separate native gateway processes.
All executable qualification runs require the existing 64 GiB zero-swap gate.
