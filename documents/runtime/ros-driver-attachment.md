# ROSv2 externally provisioned driver attachment

The matched encrypted carrier benchmark and hardware profiling procedure are
documented in [driver performance](ros-driver-performance.md).

The external adapter can consume the deployment-owned fixed bootstrap using
`provider_bootstrap.h`. Its 256-byte sealed record binds the exact local/peer
credentials, deployment context, contract, limits and descriptor identities.
Loading is transactional and allocation-free; it never creates a process,
grants an operation or replaces the subsequent mutual-TLS checks. See the
[Rust provisioning contract](https://github.com/UlteSoft/wasmros-deployer/blob/main/document/en/attachment-provisioning.md)
for the byte layout and one-time descriptor delivery. This helper is for the
separate adapter, and adds no Wasm import or VM lifecycle authority.

`provider_bootstrap_receiver.h` implements the adapter's bounded handoff: it
pins the provisioner's PID, verifies packet credentials, consumes exactly four
descriptors and validates the sealed record before publishing output. Its
noncopyable descriptor bundle closes all owned handles; malformed and truncated
deliveries cannot leak descriptors. The caller supplies the expected provisioner
identity from trusted launch state and closes the borrowed courier on completion.
The receiver never enters the VM executable or the per-message path. A successful
handoff still requires the ordinary TLS and independent operation admission.

The external Rust deployer separately implements [device-node ownership and
kernel filtering](https://github.com/UlteSoft/wasmros-deployer/blob/main/document/en/device-isolation.md).
Its VM startup path denies physical device opens; trusted provider provisioning
can reserve exact character/block device numbers with the original deployment.
These host-only controls add no import or process authority to the VM. Device
ownership, kernel open permission, encrypted session admission and motion
authorization are independent requirements. Existing device FDs and individual
ioctls need separate containment; a device number is not proof of physical
identity after hotplug. Native-provider lifecycle integration remains unfinished.

This is the current VM contract. The deployment program is not implemented by
this change. Historical component-launcher receipts do not qualify this path.
Imports belong to `uwvm_ros_v2`: `driver.connect`, `driver.submit`,
`driver.result`, `driver.describe`, and `driver.disconnect`. Each requires
explicit exposure and an exact `driver` capability with `registered-driver`
binding. Registering a descriptor grants no authority.

## Ownership and topology

The external deployment owns VM/driver creation, native library loading, image
admission, cgroups, namespaces, CPU/NUMA placement, restart, replacement, durable
fault records and termination. The VM owns only a duplicate of a preconnected
channel and bounded session state. The old component.open/start/stop/describe
imports and component launcher CLI are removed. There is no VM driver-spawn,
kill, restart, update, namespace-entry or cgroup-admin API.

Each VM can host multiple Wasm modules. Each driver can serve many VMs, but
every originating module needs a distinct registered channel and session.
Permission-group membership does not transfer another module's channel. The
runtime supplies caller identity; the first effect seals its incarnation.
Shared drivers require receiver-side arbitration across all clients. Several
deployments may share DDS, but network reachability conveys no driver rights.
Externally operated ROS/Python nodes remain outside lifecycle management.
Approved ROS/DDS communication and bridge interfaces are separate from this
local carrier, which is not an arbitrary socket, native FFI or vendor adapter.

## Deployment-to-VM boundary

The provisioning interface is the trusted launch configuration below plus
inherited descriptors. It is not an RPC that lets Wasm request process creation.
The provisioner resolves dependencies, creates endpoints, establishes credential
and clock visibility, and supplies immutable identities, contracts and finite
budgets before configuration freezes. The VM validates and duplicates descriptors
before the guest runs. `--ros-scene-check` sends no hello or activation message.

VM and driver cgroups may be siblings under an externally administered parent;
the driver need not descend from the VM's cgroup. The VM cannot move tasks, create
cgroups or join namespaces. Ordinary user processes are also accepted when the
same authentication contract is met; they must not be described as sandboxed.
A cgroup name is neither a communication address nor process-control authority.
The provisioner accounts for VM/JIT overhead, all Wasm memories, IPC and kernel
resources separately from each driver's memory.

CPU affinity/bandwidth, realtime policy, NUMA placement, memory locality and
device access belong to the external execution profile. VM channel limits bound
retained bytes and deadlines; they do not reserve CPU time or guarantee a control
period. Supervisor and stop resources need independent failure domains. Shared
drivers need aggregate budgets and per-origin queues to prevent one VM consuming
another's reservations.

Replacement supplies fresh driver/session incarnations through a new frozen VM
configuration. The current VM cannot hot-rebind a failed descriptor or resume an
old session. Detached debugger rows expose observed session state and unknown
effects, not descriptors, paths or payloads. They are not a durable crash journal
or evidence that deployment recovery completed.

## Provisioned local transport

Linux AF_UNIX SOCK_SEQPACKET is implemented. A trusted launcher supplies a
connected FD with SO_PASSCRED enabled. The VM duplicates it using F_DUPFD_CLOEXEC
and uses per-call nonblocking sendmsg/recvmsg. It does not change source flags,
connect a path, enter a namespace or change cgroups. Each packet must carry
exact kernel PID/UID/GID credentials. SCM_RIGHTS descriptors are closed and
rejected; unknown ancillary data and truncation are rejected. A pidfd pins and
monitors the peer process incarnation. Namespace-translated PID identity must
be visible to the VM; otherwise registration fails closed.

Absolute monotonic deadlines require the same time namespace, checked at
registration. Other namespace boundaries may differ when the provisioner
supplies a usable channel and observable credentials. Cgroups alone are not a
sandbox: syscall, filesystem, network, device and privilege restrictions belong
to provisioning. A reported cgroup path is never proof of containment.

Qualification includes distinct user, mount, network, IPC and UTS namespaces
with shared PID/time namespaces. It does not qualify arbitrary sibling PID or
time namespaces. Both receivers need usable identities in their own observation
domains; a namespace-invisible sender must not be accepted as PID zero. The
provisioner must choose a compatible observation topology or a separately
reviewed authenticated relay. No relay is implemented by the current carrier.

No other actor may read/write the VM endpoint. Duplicate endpoint/session
registration is refused within a dispatcher; the provisioner must also prevent
duplicates across environments/processes. Closing releases only the VM's FD,
never shutdown on a shared socket. A best-effort release packet retires the
session even if a trusted source duplicate remains. Independent receiver expiry
is mandatory because release may be lost or the VM may crash.

## Registration

First configure environment membership, explicit interface exposure and grant.
Then supply:

```text
--ros-scene-driver-budget ENV BINDINGS BYTES
--ros-scene-driver-channel ENV ALIAS MODULE FD PEER_PID PEER_UID PEER_GID DEPLOYMENT_ID VM_ID MODULE_ID DRIVER_ID SESSION_ID CONTRACT_SHA256 REQUEST_BYTES RESPONSE_BYTES REQUEST_NS SESSION_UNTIL_NS IDENTITY_FD PEER_TIME_NAMESPACE_FD
--ros-scene-driver-schema ENV ALIAS CANONICAL_OPERATION NODES ELEMENTS DEPTH
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

`PEER_TIME_NAMESPACE_FD` is a read-only namespace descriptor opened by the
trusted provisioner from the pinned peer's `/proc/PID/ns/time`. The provisioner
verifies that both endpoints use the same time namespace and prevents subsequent
changes. It must not substitute an unverified local namespace or accept a
guest-selected descriptor. Keep this descriptor open through VM registration;
the VM does not enter or retain it. The VM checks `NS_GET_NSTYPE` and compares
the kernel device/inode identity with its own clock namespace before binding.
This permits separate non-root UID/GID identities without granting either peer
ptrace access. A wrong descriptor type or a different clock is rejected. The
existing pidfd, per-packet credentials, exact session binding and TLS checks
remain independent requirements. Namespace evidence does not prove confinement
of arbitrary vendor code. In particular, cgroups alone do not prevent a
privileged provider from changing namespaces.

Direct native callers that omit namespace evidence retain strict procfs checks;
an inaccessible peer namespace is rejected. The production CLI requires the
explicit descriptor. This is an intentional change to the unreleased ROSv2
registration contract.

Budget for the TLS records and handshake in addition to the application
payload. Backpressure may require retrying the same frame under current
authority. Production registration fails when the TLS backend or the sealed
identity is unavailable. The plaintext kernel-channel option belongs to
explicitly compiled historical test fixtures.


IDs are 32 lowercase hexadecimal characters; the digest is 64. Hex bytes retain
wire order. All five identities and the digest must be nonzero. Session IDs must
be unique. Payloads are at most 65536 bytes. Request duration and session expiry
are positive finite nanoseconds; expiry is observed during operations, not by a
hard real-time VM timer. Traversal depth is at most 64 and node/element budgets
at most 65536. Zero registration budget disables attachment. This owned-buffer
budget excludes allocator metadata, kernel queues and external process memory;
the externally assigned cgroup bounds the complete VM process.

Each channel must select a compiled canonical request/result shape before
configuration freezes. No guest-selected decoder or callback is registered.
Structural validation does not validate physical limits, units, coordinate
frames, device ownership or outcomes. The external receiver must admit the
contract digest against an approved manifest binding resource, operation,
schemas, units, bounds, safety class, arbitration and driver incarnation. The VM
does not authenticate an external manifest merely from its supplied hash.

Native .so/.dll drivers need an externally managed adapter process that loads
the vendor ABI. Executables need an external wrapper or protocol support.
Existing ROS nodes can use approved ROS links. The VM does not load vendor
libraries or claim arbitrary vendor binaries already speak this protocol.
Platform extensions restrict policy and never load native drivers in the VM.
For a child-owned driver alias, its canonical operation must match one of the
child's compiled semantic routes. The frozen child switch applies to the common
driver imports too; calling the parent carrier cannot bypass a disabled child.

## Wire format

One packet contains a 176-byte header and exact bounded body. All scalars are
little-endian u64. Its 22 words are: magic 0x44534f52564d5755, header size, kind,
reserved flags (zero), deployment (2), VM incarnation (2), module incarnation
(2), driver incarnation (2), session (2), contract digest bytes (4), sequence,
absolute deadline, body size, status. No pointer, C++ layout or native handle
crosses the boundary. Unknown kinds, flags and trailing bytes fail closed.

Kinds: hello=1, ready=2, request=3, result=4, release=5. Control frames have no
body, sequence, deadline or status. Request/result sequences start at one and
cannot equal UINT64_MAX. Replies echo the exact binding, sequence and deadline.
Request status is zero; result status is a u32 application value in a u64 word.
It never establishes authority or physical safety. This unreleased contract is
intentionally breaking, without ambiguous per-operation version suffixes.

## Guest transaction rules

All imports use the existing seven-argument checked memory carrier. All extents
and result-length overlap are checked before effects. Guest addresses never
reach the driver. Owned copies use recoverable fast_io allocation and secure
overwriting before reuse/destruction.

- connect: empty request, receipt u64 1; handshake polling may return again.
- submit: u64 sequence, u64 deadline, length-prefixed canonical request blob;
  returns sequence after local transport acceptance.
- result: u64 sequence; returns u64 application status and a length-prefixed
  canonical response blob. Reserve 16 plus the configured response bound before
  receiving; returned length reports the actual encoded size.
- describe: empty request; state, sequence, status, effect_unknown as four u64
  words. States: prepared=0, handshake=1, ready=2, pending=3, result=4, closed=5,
  failed=6. This does not receive packets or update deadlines.
- disconnect: empty request, receipt u64 1; never terminates a shared driver.

One request may be outstanding per session. Exact in-flight retries compare
sequence, deadline and payload and never resend. Changed retries conflict.
Failed guest result delivery retains the response; successful delivery erases
request/result storage. Timeout, malformed response or transport failure after
submission fences the session and preserves uncertainty. There is no automatic
replay/restart or durable deduplication log. Revocation and caller retirement
close affected sessions and erase bytes. Success, timeout and disconnect are
not evidence that physical stop has occurred.

Explicit usage limits can reject subsequent work. Setting calls-per-window to
zero does not block an already authorized disconnect: cleanup bypasses traffic
metering, but still checks the caller, exact grant, interface exposure and
platform policy. Inspection neither sends a handshake nor consumes a packet.

## Shared actuation and recovery

One trusted receiver-side arbiter must serialize actuation across every client
and deployment. Per-VM arbitration is insufficient. The receiver admits origin,
contract and controller lease; lease epochs fence previous owners. Unknown,
revoked and expired sessions are rejected even when their sockets remain open.
Observer fanout and actuator ownership are distinct contracts. Reduction must
not silently turn stable control authority into unannounced rate limiting.

Safety stop needs an independent reserved lane and appropriate hardware or
controller watchdog. It must survive ordinary-queue, VM, adapter and driver
failure. The external supervisor records faults, distinguishes communication
failure from physical outcome, contains restart storms, reconciles uncertain
commands, and admits fresh driver/session identities before resuming. A driver
replacement never inherits an old control lease. The VM cannot establish these
guarantees by killing a process or trusting a reply.

### Shared receiver implementation

`driver_control.h` supplies `drivers::receiver_control` for an external trusted
receiver. Its entry points are C++ integration functions outside the VM import
surface. One instance represents one actuator under one owning deployment. All
VMs controlling that actuator through the owning deployment share the instance.
Separate instances cannot establish global exclusivity.
Vendor-facing adapters must prevent alternate command paths bypassing this gate.
The trusted gate must not share an address space with an untrusted native vendor
library: that library could overwrite the gate or invoke the hardware directly.
A deployment requiring protection against a compromised driver needs a separate
trusted command gateway and an exclusive qualified device access path, with the
vendor ABI worker behind it. If a vendor binary requires unrestricted direct
device access, software admission upstream cannot guarantee its physical behavior;
the selected safety profile needs an independent controller or hardware interlock.
The MuJoCo fixture uses a pinned simulation library and proves the gate semantics,
not containment of malicious code in that same receiver address space.

Trusted provisioning supplies the owning deployment identity to `configure`
with the driver and contract binding, provider incarnation, finite session
count and lease policy. `bind` rejects a foreign deployment before assigning a
route, consuming a registry slot or granting control. It associates each
accepted wire identity with its authenticated credentials. `freeze` ends
registration. Only trusted receiver management calls `grant`. Priority and
preemption apply to enrolled VMs within the owning deployment. A new client
must be provisioned through that deployment. Controller epochs originate at
the shared receiver.

配置接收器时，由原部署提供固定的部署标识、驱动及协议摘要和会话上限。
外部部署的注册在分配路由和占用会话槽之前即被拒绝。同一部署下的多个
VM 可以通过受信仲裁共享执行器。进程间的物理设备独占仍由部署器和获取
设备权限的受信层执行。

The receiver ownership tests exercise five Linux userspace ABIs. In eight
separate two-VM cases, the MuJoCo fixture rejects a foreign deployment before
the first simulation step, then exercises same-deployment contention and
handover over two mutually authenticated TLS channels. The three simulated
steps match the direct native reference. The test requires a pinned MuJoCo
distribution and an OpenSSL prefix supplied with `--openssl-root`.
[MuJoCo 3.4.0 release](https://github.com/google-deepmind/mujoco/releases/tag/3.4.0)

After complete canonical/physical validation, `begin` checks the selected
channel, full origin, packet credentials, strict sequence, session expiry,
bounded command deadline and controller epoch. The command deadline must fit
inside the control lease. The shared arbiter performs final owner, latch and
in-flight checks before reserving one effect. A fresh authenticated request
consumes its sequence even when ownership is refused: replay after a handover
cannot turn that same request into an authorized effect. Unknown outcomes retain
their effect slot.

`release` retires only that origin. An old controller's release after handover
does not stop the current owner. Losing the current owner reserves an independent
stop slot and latches new work; an in-flight effect becomes unknown. Trusted
`reconcile_stopped` requires independent stop observation and, when applicable,
monotonic effect-reconciliation evidence. No wire request invokes that method.
Rearm leaves all clients ungranted. This helper does not execute physical stop,
authenticate its own management caller, run a watchdog or persist recovery across
process crashes.

## Evidence boundary

Tests cover local framing, credentials, pidfd lifetime, schemas, authority,
retained delivery, retirement, multi-client process sharing and FD rejection.
Current VM qualification requires its exact build receipt. This is not an
industrial certification, hard real-time guarantee, complete vendor ABI review,
or proof that every indexed ROS package runs without WASIp1.

The encrypted ordinary-process MuJoCo fixture exercises the pinned humanoid
model under both VM engines and memory widths. It tests 128 accepted control
steps, refusal of an out-of-range setpoint, and driver failure after a step.
The physical-state comparison uses the same MuJoCo library and model directly.

The [qualification record](ros-driver-attachment-qualification.md) separates
actual VM execution, shared-driver tests, physical simulation, source/assembly
sampling and unresolved deployment/vendor qualification. The model and DDS provider registration contracts are documented in the
[model](ros-model-attachment.md) and [communication](ros-communication-attachment.md) references.
