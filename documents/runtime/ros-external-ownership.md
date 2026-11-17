# External ownership and deployment boundary

ROSv2 gives a Wasm module access to explicitly registered robot capabilities. It
does not give that module, or its VM-side provider, authority to create, terminate,
restart, update or move a native driver, model server or ROS process. The product
is one VM executable. Native vendor libraries are loaded only by external
adapters; executable drivers are external processes. Platform extensions refine
permission policy and contain no vendor driver implementation.

The compiled foundation catalogue contains 32 platform identifiers. An exact
`uwvm_ros_v2.platform_extension.<platform>.v1` policy is materialized only when
configured, so unused platforms allocate no environment storage. The foundation
revision is a policy boundary, not a vendor protocol version. Its routes use the
existing device/control schemas. No additional native function pointers, driver
code or executable lookup paths are introduced. The catalogue includes ABB,
Agibot, AgileX, Booster, Boston Dynamics, Clearpath, Deep Robotics, Dobot, Doosan,
Elephant, Fanuc, Flexiv, Fourier, Franka, Hello Robot, Kinova, Kuka, Leju, LimX,
NVIDIA (including Groot, Isaac ROS and Isaac Sim), PAL, RealMan, Robotis, Trossen,
UBTech, UFactory, Unitree, Universal Robots and Yaskawa. These declarations are
not vendor-driver qualification.

For example, after explicitly registering an `arm` driver capability and its
channel in environment `robot`, its child restriction is configured with:

```text
--ros-scene robot platform_extension uwvm_ros_v2.platform_extension.franka.v1 enable
--ros-scene robot platform_extension.bind uwvm_ros_v2.platform_extension.franka.v1 arm
```

The child cannot create the parent grant. A disabled child also denies access
through the common driver entry points. An enabled but unbound child fails static
validation. Unknown names and policy revisions are rejected, without fallback.

The current attachment protocol accepts deployment-provisioned, authenticated
local channels. ROS network interoperability is supplied by external ROS/DDS
providers. These are distinct transport boundaries: a local attachment is not
DDS, and the current implementation must not be described as DDS-only. ROS 1
also requires a bridge with independently authorized ingress and egress; ROS 2
language libraries alone do not guarantee middleware or schema compatibility.
See the [communication attachment contract](ros-communication-attachment.md).

## VM authority

The VM can duplicate a provisioned endpoint, authenticate its peer and immutable
contract, apply module/group/resource permissions, submit a bounded typed
operation, retain its outcome and close its own session. It cannot infer a
physical stop from channel closure, process death, timeout or DDS liveliness.
Unknown effects retain their accounting and recovery obligations.

Static freeze rejects duplicate session identities and duplicate kernel endpoint
identities across all driver, model and ROS environments. This includes different
fd numbers produced by dup and both ends of the same pipe. The provisioner keeps
all registrations stable until their private duplicates have been taken; a
registration is not authority to modify the descriptor table concurrently.

The historical process launchers, image configuration, namespace process owners
and CPU/NUMA placement helpers have been removed from the production scene source
directory. Historical regression fixtures remain under
`test/0084.ros-runtime/support/legacy_process`. They are not product features.
`check_vm_boundary.py` checks the ROSv2 header closure and, optionally, the exact
default VM's recorded compiler configuration and dynamic library dependencies.
This structural check does not establish a whole-process syscall sandbox.

## One physical owner, several authorized clients

Physical-device ownership and VM connection cardinality are separate policies.
A single-client instance registers one exact VM session. A shared instance
registers a bounded set of exact sessions. Both configurations start with no
actuation authority. The receiver's control gate admits an independently granted
controller epoch and serializes effects across every client, including clients
from different deployments. Read-only observation can have its own policy.

A globally unique device uses one trusted ownership authority for its canonical
physical identity. It need not reject all additional VM connections. Additional
clients attach through the existing owner and participate in its control policy.
Starting a second hardware-owning driver is refused before device access. The
failure affects the proposed instance; it must not deliberately crash a healthy
VM or the existing controller. DDS discovery names are not device identity,
ownership evidence, or permission to launch another driver.

For local devices, deployment policy must map aliases to the same physical
resource, including device nodes, parent buses and relevant ioctl interfaces.
For network-controlled equipment, the authority must also cover network paths
and the device's own ownership/fencing mechanism. Separate mount/network
namespaces and cgroups alone do not establish global device exclusivity.
Linux cgroup v2 device access uses BPF; resource budgets and namespace separation
serve different purposes. See the
[kernel device-controller documentation](https://www.kernel.org/doc/html/v6.17/admin-guide/cgroup-v2.html#device-controller).

Multiple independently privileged deployment administrators cannot be made
mutually exclusive by a VM-local mutex or an advisory file lock. They must share
an enforced ownership authority, or use a qualified device-side fencing protocol.
An untrusted native driver must not receive alternate device/network paths that
bypass the trusted receiver. Control leases, emergency-stop capacity and trusted
stop observations remain independent of transport rate limits.

## Test deployment fixtures

`test/0084.ros-runtime/deployment_fixture.py` is test scaffolding, not a production
deployment service. Its device registry uses a persistent lock inode and a
durable active-owner record. Closing the lock or crashing a deployment process
does not clear that record. A replacement remains ungranted until the fixture
explicitly supplies both simulated stop and effect-reconciliation observations.
Corrupt records fail closed. The fixture's observation labels are not a physical
safety-evidence authentication mechanism.

The registry directory and its ancestors are trusted, protected deployment
inputs. Drivers and VMs cannot select or modify registry paths. All cooperating
deployers must use the same registry and canonical resource identity. The fixture
does not prevent a malicious privileged owner from bypassing an advisory lock;
that requires the deployment-side access enforcement described above.

The same test module can allocate separate VM and driver cgroup leaves beneath a
predelegated subtree of the existing 64 GiB gate. It requires delegated memory
and pids controllers, an empty parent, finite per-leaf budgets and zero swap.
It neither enables controllers nor changes ancestor limits. It enrolls only a
waiting direct child before releasing its executable gate. Missing delegation
is an explicit test gap, never a reason to run outside the aggregate gate or
silently claim per-process memory isolation. Linux's
[delegation and no-internal-process rules](https://www.kernel.org/doc/html/v6.17/admin-guide/cgroup-v2.html#delegation)
must be satisfied by the external administrator.

## Permission composition

Fused host operations may reduce copies and dispatch overhead. Every constituent
effect still requires its own current permission claim in the same admission
group; revocation, deadlines and partial/unknown outcomes apply to the complete
group. A coarse convenience alias cannot restore a denied constituent operation.
Pure Wasm computation needs no invented host capability. Host-managed storage,
external disclosure, scheduling resources and physical effects do need explicit
policy, even when the corresponding algorithm could run in Wasm.

Self-state observation remains read-only and allowed after explicit ROSv2 module
binding, subject to host attenuation or hiding. Foreign module and VM observation
start denied and require exact independent grants. Observation cannot create a
provider, enroll another caller, consume execution grants or mutate policy.

These mechanisms and simulation tests establish bounded properties of their
recorded configurations. They do not establish all ROS package compatibility,
all vendor-driver behavior, physical emergency-stop performance, hard real-time
deadlines or industrial safety certification.
