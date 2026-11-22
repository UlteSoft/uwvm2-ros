# Native driver lifecycle and recovery contract

> Current product boundary: [external driver attachment](ros-driver-attachment.md).
> VM-owned component launch/stop interfaces in historical sections below are
> removed. Their receipts qualify the historical implementation only. The VM
> does not implement the deployment process supervisor.

This design extends the [native driver boundary](ros-native-driver-isolation.md).
The final driver boundary is attachment-only: the deployment layer creates,
terminates, restarts and replaces native driver processes. The VM connects to
registered instances, observes explicitly permitted state and makes authorized
typed calls. It cannot acquire process-lifecycle authority through a ROSv2
grant. The former closed component launch/stop imports and launcher options
have been removed from the current product. Their retained qualification below
is historical evidence. The implemented registered-driver imports and CLI are
specified in the attachment document; arbitrary vendor ABI adapters and the
deployment supervisor are not implemented by that carrier.

The VM contains its own validated code only. Every native executable, library,
ROS plugin, model runtime and vendor dependency runs in an external confinement
domain. Open source and signatures do not waive this rule. Platform extensions
provide permission and constraint descriptions; they cannot register native
callbacks or link vendor implementation code into the VM. A Wasm controller
can exchange registered messages with other Wasm principals without exporting
its private memories.

The [deployment and external-peer model](ros-robot-capabilities.md#managed-processes-and-external-ros-peers)
permits multiple modules per VM, multiple VM processes per deployment, shared
external drivers and separately operated ROS networks. Driver dependencies,
process ownership and per-module authority remain distinct relationships.

## Principals and immutable packages

An installation record binds a logical driver to an immutable package digest,
target OS/ABI, exact executable or matching external plugin host, dependency
closure, device/firmware identities, typed role schemas and configuration
digest. A trusted deployment selects the record; a guest supplies neither a
path, shell command, native symbol nor loader environment. Package installation,
instance creation, termination and replacement belong to the deployment layer.
Connection, permitted state observation and typed device operations belong to
the VM's ROSv2 permission boundary. Device activation is distinct from process
creation; a deployment update does not authorize firmware changes, safety-limit
changes or motion.

Executable packages include their interpreter and dynamic dependency closure.
Shared-library packages include the external host, its ABI and plugin metadata;
the original library is loaded only after confinement. Static vendor archives
require a matching external executable and are never linked into the VM.
Kernel drivers are separately provisioned OS components, not confined plugins.
An unavailable ABI, unsealed dependency or incompatible host fails before
device authority exists. Copying a Linux binary to another architecture is not
a compatibility mechanism.

Installation verifies bytes and produces an immutable, non-writable image view.
Execution must use that view, not reopen an interchangeable guest pathname.
Loader search paths, executable mappings, interpreters, helper executables and
configuration sources are part of the verified closure. Inherited descriptors,
credentials, home directories, proxy settings, debug agents and plugin search
environment are absent unless individually declared. Neither a guest nor a
driver can choose a fallback host with broader authority.

The historical component contract covered sealed images and closed typed roles.
General vendor hosts additionally require a reviewed package resolver and a
confinement profile for their real syscall, device and network needs. These
requirements cannot be satisfied merely by changing a fixture executable name.

## Confinement and resource placement

Each driver incarnation has a non-reusable identity independent of PID reuse.
The external deployment supervisor owns the whole process tree, including
grandchildren, daemonized children, helper executables and threads. Strong
isolation requires pinned process identities and verified process, filesystem,
network and syscall boundaries. A dedicated resource subgroup is a separate,
optional deployment decision. Declaring the entire incarnation stopped requires
evidence that its complete execution domain has exited; process groups and a
single `waitpid` result alone are insufficient. The supervisor must detect a
broken required isolation invariant and fence the instance.

### Operator-owned cgroups and explicit isolation guarantees

Driver origin and execution isolation are independent. An original vendor
binary, including an open-source or signed binary, still runs outside the VM.
An external plugin host loads a vendor `.so`/`.dll`; a vendor executable runs
as an external process. Neither packaging form grants filesystem, network,
device, motion or model authority.

The driver-management design permits both an ordinary user process without a
dedicated resource group and a process placed in a cgroup by the operator or
deployment service. On Linux every process already belongs to the cgroup
hierarchy; "without cgroup" means no separately configured resource group or
resource-controller guarantee. The VM does not create, delete, mount or configure
cgroups, change controller limits, or obtain a Docker daemon's administration
socket. User provisioning owns those actions. Where dedicated containment is
requested, an operator-controlled launcher or service must establish it before
untrusted instructions run; post-start migration is not an equivalent boundary.

Existing VM-managed model/DDS worker paths inherit the preparing VM's cgroup.
These separate providers have not been migrated to external provisioning by
the registered-driver change. The removed component launcher is not available
through the current product. The existing Linux worker launchers use bubblewrap with user/PID/network/
IPC/UTS/cgroup namespaces, dropped capabilities, sealed read-only image files,
bounded scratch and seccomp policies. A cgroup namespace virtualizes its view;
it does not create a resource group or establish memory/CPU quotas. The current
registered-driver carrier qualifies dedicated preconnected local channels and
exact process credentials. It does not qualify arbitrary vendor binaries or
per-driver cgroup-controller admission. These remain separate requirements,
not properties implied by a channel or by these worker launch flags.

Resource containment must be reported separately from process and access
isolation. The design requires observation of the actual process incarnation,
group membership, effective relevant controller limits, namespace boundaries,
syscall policy and device/network grants. A worker's self-reported assertion is
insufficient evidence. Unknown or externally mutable facts must remain visible
as such. A deployment requiring a missing guarantee refuses activation; an
explicit ordinary-process deployment is not rejected merely for lacking a
dedicated cgroup. Neither mode may bypass ROSv2 identity, schema, direction,
operation, incarnation or revocation checks. No implicit downgrade is permitted.

Linux resource controls (`memory`, `pids`, `cpu`, `cpuset` and `io`) complement
namespaces, capability removal, `no_new_privs`, seccomp and filesystem/device
access policy. Landlock, AppArmor or SELinux may supply additional access
controls when their actual requirements are met; support is not inferred from
an OS name or a container brand. A backend advertises verified mechanisms and
their scope, not a blanket "Docker sandbox" or "platform sandbox" guarantee.
All these process sandboxes still share their kernel with the host.

The existing 64 GiB test cgroup constrains the aggregate VM/test workload. It
does not prove independent memory or CPU containment for each driver. If the
VM and a driver share one resource group, driver resource exhaustion can still
affect the VM; an operator-managed per-driver resource boundary is needed to
claim that stronger guarantee. Ordinary process separation alone does not
prevent an otherwise authorized same-user process from accessing host resources.

References: [Docker isolation and resource controls](https://docs.docker.com/engine/security/),
[Linux cgroup v2](https://docs.kernel.org/admin-guide/cgroup-v2.html),
[seccomp](https://docs.docker.com/engine/security/seccomp/), and
[Landlock](https://docs.kernel.org/userspace-api/landlock.html).

### Future wros-deployer integration

`wros-deployer` is not implemented. Its intended responsibility is to provision
an individual resource group for each VM and native driver, and a separate
budget for supervision and recovery. The VM must support an explicit attachment
to a deployment-approved driver execution domain without creating those groups.
The attachment binds the actual process identity, immutable package and protocol,
instance generation, intended cgroup/namespace identities and narrowly scoped
IPC channels. The descriptor or authenticated endpoint conveys communication;
a cgroup path is not an IPC endpoint or an authorization credential.

The VM itself does not join the driver's namespaces. The deployment service
establishes the namespaces and passes the approved communication endpoints.
Existing-process attachment needs independently checked identity and readiness;
launching a new driver requires confinement before its native instructions run.
Only the trusted deployment layer selects these host objects. Guest modules
cannot submit arbitrary cgroup paths, namespace descriptors or target PIDs.
Driver replacement invalidates the old generation and channels; restart must
not silently restore motion authority or clear an unresolved physical effect.

For deployment containment attestation, a future integration must verify an instance
record against the actual execution domain before publishing any ROSv2
capability. The current channel pins the process identity and checks its time
namespace; it does not yet implement this full attestation. The additional
Linux requirements include pinned cgroup
and namespace objects, effective ancestor limits, the admitted package and
configuration, and the exact channel role and protocol. A numeric PID, a
reusable pathname or an untrusted worker's readiness message is insufficient.
Namespace-visible PIDs and cgroup paths may differ between the supervisor,
VM and driver; the deployment service must perform the checks from its trusted
observation domain. Missing visibility means unverified containment, not success.

An approved, already connected local channel can be passed to the VM and the
external host during setup. Namespace entry and cgroup migration are not data
plane operations. Each endpoint still needs bounded framing, generation and
sequence validation, revocation and channel lifetime ownership; possession of
an unrelated same-user socket is not authority to attach a driver. Changed
process membership, required isolation guarantees or deployment generation
fence the attachment and require fresh admission. A one-time identity check
cannot prove that an independently privileged administrator never changes a
running domain.

The independent supervisor owns external process termination and restart.
Losing a VM must invalidate its control lease even when the driver remains
alive; losing the driver must cancel or mark pending effects unknown without
terminating an unrelated VM. Communication failure, CPU starvation and OOM are
separate fault records. Reconnection must not replay motion commands whose
physical outcome is unknown. A driver watchdog and the independently qualified
stop mechanism must remain effective when either endpoint or the supervisor
cannot run; process survival alone does not establish a physical safe state.

Admission budgets include guest linear-memory maxima, VM/JIT and code storage,
stacks and threads, metadata, queues, model/driver workspace, image preparation,
recovery bookkeeping and explicit margin. Linear memory alone is insufficient.
Arbitrary native packages have no universally computable peak; declared hard
budgets and measured workload qualifications remain necessary. Shared mappings,
kernel-charged resources and device memory require their own accounting rules.
A cgroup maximum is a ceiling, not reserved physical RAM. Recovery must retain
resources independently of a failed VM or driver, with parent-level admission
preventing aggregate overcommit of the budget being promised.

### Attachment-only driver ownership

Connecting to a driver does not make its resource group a child of the VM's
resource group. The intended deployment layout uses process-free parent
domains and separate leaves for each VM and driver. The recovery service has a
separate leaf outside each application's failure domain. An application budget
may still cover its VM and driver siblings; nesting under that application
parent is permitted, but it must not imply that a driver consumes only the VM's
individual budget. Normal cgroup-v2 domain controllers cannot distribute
resources from a non-root group that still contains processes; a VM process
must not occupy the parent that distributes those resources to drivers.

Only the deployment service owns creation, deletion, controller configuration,
delegation and migration. Provisioning one leaf for a VM does not delegate a
writable subtree to that VM. If a deployment explicitly supplies a subtree,
the deployment service still prepares the process leaves and their limits.
The VM itself receives no generic cgroup administration interface. This also
avoids a second independent resource allocator inside every VM.

The final VM has one driver acquisition path: attach to an already registered
external instance. Local driver launch and VM-requested deployment launch are
not part of that interface. A deployment service or an explicitly chosen manual
operator starts the process before attachment, owns its package and resource
configuration, and retains all process-control authority. A shared-library
driver's external plugin host follows the same rule as a standalone executable.

Attachment preserves the driver's actual cgroup and namespace membership. It
supports an ordinary user process as well as an independently isolated driver;
neither case inherits membership from the connecting VM. Whether the external
launcher inherited a group, selected a dedicated leaf or used a permitted
subtree is a deployment decision. The VM's containment contract either accepts
explicitly observed guarantees or requires an exact approved domain/profile.
Unmet requirements reject attachment, with no silent downgrade. There is no
combination of VM startup switches for each external resource arrangement.

The VM may acquire and release its own connection, observe authorized state,
call registered operations and revoke its own authority. It cannot create,
terminate, restart, update, migrate or reconfigure a driver process, nor request
those actions indirectly through a privileged lifecycle RPC. Process-control
operations must not be hidden inside a generic typed-call schema. Disconnecting
one VM must not terminate a driver used by another VM. The deployment layer may
observe connection/health events and act under its own independent policy;
an untrusted disconnect or fault report alone is not authority for a restart.

External hosts must handle peer EOF, malformed messages and failed writes as
bounded connection failures. They must prevent a protocol write's `SIGPIPE`
from terminating the driver and preserve unrelated sessions when one VM
disconnects. A vendor host that exits all service on one peer's disconnect
cannot claim this shared-driver contract without an appropriate external
adapter and qualification. Neither connection close nor a generic management
message is an alternate process-control interface.

A safe-stop command is a separately authorized device operation, not process
termination. The driver and independent safety supervisor own the physical
response to lease expiry, lost control ownership or unavailable callers. VM
death must not leave motion authority live, and killing a process must never
stand in for verified physical stopping. Replacement creates a new instance
generation; connections and control authority require fresh admission and
unknown physical outcomes remain unresolved until independently reconciled.

Namespace requirements are a separate profile: required process, mount,
network, IPC and user boundaries are checked independently of cgroup placement.
Creating a cgroup namespace proves neither a new resource group nor controller
limits. Neither the guest nor an ordinary message can request `setns`, choose
a host PID or pass arbitrary namespace descriptors. Driver discovery, protocol
messages, permissions and typed operation checks remain identical for ordinary
and independently isolated external instances.

Guest-visible observation uses assigned logical instance/domain identifiers
and verified or unknown guarantees, not host paths, namespace descriptors or
administrative handles. The existing self-observation and explicitly granted
foreign-observation boundaries still apply. Connecting a driver does not
authorize enumerating unrelated cgroup members, namespaces or processes.

Per-leaf limits alone do not establish independent recovery. The deployer must
validate effective ancestor budgets, aggregate admission, OOM group scope and
the service's independent CPU, memory, descriptor and log capacity. Putting the
recovery service under the same exhausted or group-killed application ancestor
would defeat that design. A parent limit or global host failure can still affect
multiple leaves; verified guarantees must state that boundary.

Acceptance tests for this future path must cover externally launched ordinary
and dedicated instances, attachment with matching and mismatched domains, PID/path reuse,
membership changes, namespace identity mismatch, revoked or stale channels,
refusal of every driver process-control operation and controller unavailability.
They must also show that a VM connection's closure cannot terminate a shared
driver and that a VM crash expires its control lease. Separate
resource-fault tests must exhaust a driver, a VM and their parent in turn and
check the supervisor's ability to record, fence and recover the intended
instance. They must also verify that CPU throttling and restart never silently
restore motion authority. All local qualification remains inside the existing
64 GiB aggregate gate; future subgroups cannot evade that outer limit.

These are acceptance requirements for the future deployment attachment path.
The current inherited-cgroup launcher does not provide these external-domain
checks or per-driver resource reservations.

The package profile independently specifies:

- The exact devices, direction-specific peers, discovery routes and operation
  classes. A ROS/DDS role grants neither arbitrary Internet access nor a host
  network namespace. Vendor services requiring an unrestricted device ioctl
  surface are refused by a deployment that requires a finer boundary.
- Process/thread, descriptor, memory, scratch-byte, durable-byte, inode and log
  reservations; startup, heartbeat, shutdown and forced-exit deadlines.
- CPU set, scheduling policy, maximum real-time runtime, IRQ/worker placement,
  NUMA node set, allocation policy and cross-node fallback behavior.
- Bounded command, sample and diagnostic queues, their overwrite/reject rules,
  message age, sequence rules and independent stop/revocation capacity.

Resource admission happens before activation. Reserving a CPU is distinct from
pinning a task; pinning alone promises no scheduler or IRQ latency. The VM must
not hold an authority/publication lock while waiting for a process, filesystem
flush, socket, device or page-in. No peer process can increase its parent's
budget. Qualification on this host uses the existing 64 GiB, zero-swap cgroup for
the VM, descendants, simulators, builds and profilers; a sibling unbounded
worker is not an acceptable timing or fault test.

A CPU/NUMA profile is selected before a control epoch starts. Runtime migration
requires a quiescent transition and fresh timing admission. A NUMA allocation
failure follows the declared refusal/fallback policy and cannot silently change
a profile claiming bounded local-node latency. Native vendor loops that must
stay local run entirely in the external host; ROSv2 exchanges bounded state and
commands instead of turning every vendor C++ method into synchronous IPC.

### Exact finite resource ceilings

The GNU C Library resource-limit contract defines RLIM_INFINITY as unlimited
and distinguishes native and large-file resource-limit types. Reference:
https://sourceware.org/glibc/manual/latest/html_node/Limits-on-Resources.html .
The implementation therefore refuses ambiguous narrowing before any setter.

The model/component and DDS launchers validate each configured process ceiling
against the target libc resource-limit type before narrowing. A finite unsigned
64-bit amount that cannot be represented, or that equals the ABI's unlimited
sentinel, is refused during preparation. Startup image copies must also fit the
finite file-size ceiling; deferred model weight descriptors do not participate
in that startup-copy limit. Zero remains a valid hard ceiling for core dumps
and locked memory, not permission to launch another worker.

Immediately before sealed exec, the owned namespace-init child repeats each
conversion, sets equal soft/hard limits, and reads both back. Refused setters,
failed reads and either mismatching value exit before external code. There is
no unbounded retry or fallback, and the VM's own limits are unchanged. These
checks are startup-only and add no system call to message transport. They do
not replace aggregate per-incarnation cgroup admission or turn RLIMIT_AS into
an aggregate physical-memory guarantee.

The frozen source in `worker-finite-limits-final-source` passes
`worker-finite-limits-final-checks/results.json`: 39 positive native executions
across optimized Clang, GCC and ASan/UBSan, plus six expected negative controls
against the earlier source. Cases include finite 32/64-bit integer boundaries,
refused setter/readback, altered soft/hard observations, normal launch and real
model inference/DDS directional-pipe regressions. Every fault child is reaped
and the owner's ceiling remains unchanged. The ABI integer checks do not claim
native execution on other Linux architectures. Whole-VM/DAP evidence is tracked
separately from this native-core receipt.

The separate `worker-finite-limits-abis/results.json` executes seven target
libc variants: AArch64; ARM soft/hard float and IA32, each with native and
large-file widths where applicable. The narrow targets actually report a
four-byte rlim_t; large-file mode reports eight bytes. ARM execution uses QEMU
and IA32 uses its matching loader. These are ABI/resource semantics checks,
not full worker or hardware performance qualification on those architectures.

The complete optimized VM with SHA-256
`5b2ad6662f1412c87fd6ca319233fcd8f02148649cdc8443046462ad0c745e67`
passes 800 cases across 17 receipts in `worker-finite-limits-qualification.json`.
The new resource-readback scenario contributes 11 Wasm32/64, interpreter/AOT,
memory-index and full/read-only/limited DAP checks. An external seccomp notifier
refuses the actual born child's RLIMIT_NOFILE getter; its pinned identity must
exit before fixture rescue. The existing first-failure value remains startup
protocol failure, alongside confirmed exit; retry and observation stay inert.
Other receipts exercise component crashes/stops, real models, two isolated ROS
workers, module messages/observation, denied imports and the bounded MuJoCo
fixture. Earlier larger cohorts are not reassigned to this executable.

The full build stayed inside the original 64 GiB zero-swap cgroup. Its
memory.max event counter increased by five, with no OOM or OOM-kill events.
This pressure is retained in the build receipt; it must not be described as
zero resource events. Existing editor/debugger processes also share that gate.
No unrelated process was terminated and no limit or device policy was widened.

### Transactional component supervisor preparation

`component_dispatch::prepare()` publishes readiness only after every reserved
supervisor has started. A stack guard, pthread attribute or thread-creation
failure permanently retires the private dispatcher. It joins every previously
started idle supervisor before returning the original error. Joining holds no
configuration, binding or authority lock; callers must not retain an authority
lease across preparation. Failed preparation cannot accept another binding or
retry. Sealed images, buffers and unused stack reservations remain owned until
dispatcher destruction. No component process, grant or native effect is created
by this preparation step.

The enclosing deployment already discards private brokers on preparation
failure. This additional rollback closes the dispatcher-level lifecycle gap,
including direct trusted use outside that deployment entry point. The frozen
`component-prepare-transaction-scoped-checks/results.json` contains 68 passing
native executions across Clang, GCC, ASan/UBSan and ThreadSanitizer, with 15
expected failures against the previous implementation. It injects both guard
pages and each pthread boundary across three binding positions, checks unlocked
inspection during join, verifies inert retries, and races retirement against
preparation. The sanitizer wrappers exclude sanitizer-internal pthread calls;
the earlier unscoped injection attempt is retained as a failed fixture run.

The optimized VM with SHA-256
`fc8b0e46bbce53469b73b78bc64f7f14c1fd22214438079aa178caafd8ee4315`
passes 779 actual VM cases across 15 receipts in
`component-prepare-transaction-qualification.json`. The native rollback receipt
is separate from this count. This cohort includes original-library metadata
loading, component faults/stops, resource readback refusal, import denial,
cross-module messages and observation, isolated models, two ROS workers, and
128 bounded MuJoCo cases. It does not implement arbitrary vendor hosting,
durable restart/update or physical stop qualification. Its full build remained
inside the existing 64 GiB gate; `memory.max` events increased from 550 to 831,
with no OOM or OOM-kill. The native rollback tests caused no memory-event delta.

### Unchanged original vendor library qualification

The RealMan fixture uses `libapi_cpp.so` from upstream revision
`68119369176d77c9d0c07bdba5193e71095d8d3a`. The acquisition receipt pins archive
SHA-256 `5f8e44ad4e3e6954e65210c037c843a71147b2f6185c49ba40515c249724a7b9`;
the original x86-64 library has SHA-256
`75898b22d63f0f2c50d9ca1f3fba66a2452e571061ea382af47991460bdad3ef`.
The runner checks the archive bytes, selected member bytes, ELF ABI, exact
headers and dependency closure without executing the input. A separate test
adapter links the unchanged library. Both are sealed into the normal component
image and loaded only after namespace and startup syscall confinement.

The adapter calls the documented `rm_api_version` entry point. It does not
initialize robot transport, acquire a controller or request motion. After the
final filter, socket creation, fork, executable mappings and exec are refused;
the host password-file path is absent. Four scenarios pass 44 Wasm32/64,
interpreter/AOT, memory-index and full/read-only/limited DAP cases: completion,
authorized stop, an adapter call fault and an adapter constructor fault. The
faults are deliberately injected in the external adapter and are not attributed
to the vendor binary. Independent stopped-VM mapping observations find no named
vendor mapping. That observation supports, but does not replace, the separate
process/code-loading boundary.

Receipts are `component-prepare-original-realman-completion-closed/results.json`
and `component-prepare-qualified-original-realman-{call,constructor,controlled-stop}/results.json`.
The first completion attempt incorrectly expected EPERM; the production filter
deliberately returns ENOSYS. The corrected fixture asserts that exact refusal
without weakening the filter. This is concrete original-library load/lifecycle
evidence, not full RealMan driver, firmware, transport or hardware acceptance.

## State and authority transitions

Authority starts at zero. A stable grant authorizes an exact role and resource
within fixed physical, storage, queue, lifetime and freshness constraints.
Optional traffic limiting is a later reduction and explicitly permits
refusal. It cannot reduce the reserved stop/revocation lane or invalidate an
already committed command by silently dropping its remaining phases.

The deployment supervisor owns the following process lifecycle. A VM can read
its authorized projection; these are not guest process-control operations.
The VM's own attachment and control-lease state is separate from process state.

1. **Installed to prepared:** verify package/configuration identity; reserve
   process-tree, IPC, audit, recovery and stop resources. No driver instructions
   or device effects have occurred yet.
2. **Prepared to starting:** persist the fresh incarnation intent, establish
   confinement, then execute the exact host. Wait for a bounded authenticated
   handshake containing the expected schemas and instance nonce. Libraries and
   their constructors remain inside this boundary.
3. **Starting to ready:** verify dependency health, initial state, clock/freshness
   assumptions and independent stop readiness. Readiness grants no control.
4. **Ready to active:** require a fresh activation decision, exact controller
   ownership, current safety evidence and receiver acceptance of the new epoch.
   Persist the potentially active epoch before enabling effects. Publish the
   new authority only after these steps complete.
5. **Active to draining:** fence new work first, resolve outstanding receipts
   conservatively, revoke the controller and request independent stop. Never
   hold the normal command queue open while waiting for its own capacity.
6. **Draining to exited:** terminate within the declared grace period, escalate
   to whole-domain termination, and observe every process gone. Keep unknown
   physical outcomes and cleanup debt after exit.
7. **Any live state to failed:** retain the first fault and last trustworthy
   observations; fence the incarnation, propagate dependency revocation and
   activate the reserved stop path. Failure and confirmed exit are independent
   facts. Neither timeout nor a successful kill clears failure.

A deployment lifecycle request identity includes the incarnation and canonical
operation fields.
An exact retry returns the retained decision; changed fields conflict. Reusing
an old start key cannot start another process. Launch/start admission does not
mean that startup or activation completed. No read-only observation performs
lazy startup, recovery, retry, lease renewal or stop acknowledgement.

### Retained first-failure observation

The closed component supervisor records `first_failure` for the current
incarnation. It latches the first detected cause under the component state
lock. A later stop request, successful process-tree reap, repeated observation,
historical start retry or logical close cannot replace or clear that cause.
The field is a host observation, not an inferred native signal or exception.
It contains no stderr, executable path, PID or native memory. This in-memory
record does not survive VM termination and does not implement the durable
recovery contract below.

The closed unsigned 64-bit values are: `0` no observed failure; `1` launch
admission refused; `2` launch failed; `3` startup protocol failed; `4` channel
failed; `5` message protocol violated; `6` deadline elapsed; `7` authority lost;
`8` diagnostic limit exceeded; `9` the backend explicitly reported failure.
For example, EOF after a crashed native call establishes channel failure; it
does not establish which signal ended the native process. The first observation
may differ from the underlying cause. An intentional stop with no previously
observed failure retains zero.

`component.describe` inserts this field immediately after `exit_observed` and
before the optional last receipt. An independently authorized description can
report failure and confirmed exit together. Consumers reject unknown codes,
`failed` with zero, and nonzero codes in states other than `failed`, `retired`
or `outcome_unknown`. Retirement and unresolved effects may hide the failed
state label but preserve its cause. This unreleased wire change intentionally
has no compatibility fallback.

A stop, close or retirement that wins before native launch cancels the queued
start with `effect=none`. It must not invent a launch fault or leave a start
receipt permanently queued. The supervisor rechecks this decision under its
state lock after obtaining role authority. An actually expired startup is
recorded as `deadline_elapsed`; a refused admission remains a separate cause.

The debugger's component `status_code` carries the same closed value. Console,
DAP and IDE inspection validate the state/code combination and remain detached,
read-only projections under their existing debug permissions. Granting
observation cannot terminate, restart, update or activate a driver.

### Failure after process creation

A failed launch can still own a real child. The supervisor distinguishes
pre-fork refusal from successful fork followed by failed pidfd acquisition or
launch authorization. It retains ownership and reaps outside the authority
and component-state locks. `exit_observed` becomes true only after actual
cleanup; the retained first cause remains `launch_failed`. Replaying the start
receipt neither relaunches the image nor clears the failure.

The common model/component launcher prepares all nonblocking descriptors before
fork. Its one-use Unix sequenced-packet gate authorizes execution only after
pinned process ownership is published. Failure shuts down the gate's write side;
EOF rejects the child even when another fork inherited a duplicate endpoint.
`MSG_NOSIGNAL` prevents a dead child from turning a refused gate send into a host
SIGPIPE. An absent pidfd never falls back to signalling a numeric PID or waiting
under authority. The cleanup owner still waits for kernel-confirmed exit.

`component_launch_failure_checks.py` installs real per-test seccomp refusals for
pidfd acquisition, gate delivery, both, and nonblocking setup. The old snapshot
fails all four; Clang ASan/UBSan and GCC pass the corrected behavior. The tests
verify no publication, retained first cause, exact retry, stop, and no child
remaining after retirement. The common launcher also passes the existing real
model-inference, allocation-rollback and component concurrency suites. These
checks exercise actual fork/IPC/reap, not a mocked process provider.

The complete `ca4b0937cbbaf494ca0b5f694350d610ef1f6a4637a69c04aff8deaf7dcfacc8`
VM cohort additionally passes 759 bounded cases across component faults,
permission denials, module observation/exchange, models, debugger inspection and
MuJoCo. `component-launch-cleanup-qualification.json` records each receipt and
its runtime hash. All cgroup memory event counters remain unchanged. The ten
component scenarios cover constructor/call crashes, unannounced exit, explicit
failure, malformed frames, completion, controlled stop and three launch faults.
These counts include CLI/refusal cases and the narrow scalar MuJoCo fixture;
they are not 759 vendor drivers or evidence of industrial certification.

### Historical missing-identity failure

The following records the earlier launcher design and its retained evidence.
The kernel-owned birth domain described below supersedes this startup boundary.

The earlier model/component launcher distinguishes authorization of its sealed launcher
from observation of the namespace-init process. An unapproved child is clean
only after its actual exit; an approved launcher additionally needs the pinned
namespace-init exit. `reap()` observes the launcher and any already acquired
namespace identity; `cleanup_confirmed()` provides the stronger proof required
to release process reservations. A timeout or failed identity acquisition does
not supply that proof.

If namespace identity cannot be acquired, a component retains `outcome_unknown`,
one conservative outstanding process reservation, its first startup failure and
all retained bytes. The count means cleanup debt, not a fresh OS liveness probe.
`component.stop` reports `exit.unconfirmed`; repeated starts and stops cannot
clear the debt. Model startup fences the binding and refuses later attachment
or replacement. Cancellation is latched before signalling, including before
fork and between fork and pidfd publication; later startup cannot lose it.

The installed bubblewrap 0.11.0 places its `--block-fd` read before the final
child `PR_SET_PDEATHSIG`. Its launcher can therefore exit while the namespace
child is still blocked. `--die-with-parent` alone is insufficient evidence for
this startup failure window. The product must retain uncertainty. A qualified
general vendor supervisor still needs independent per-instance process-domain
cleanup, such as a delegated resource subgroup with confirmed empty membership;
this correction does not supply that missing general-host mechanism.

`component_identity_failure_checks.py` injects a real kernel refusal immediately
after the launcher reports its child. Independent fixture pidfds clean the
otherwise unresolved descendants. They are never passed to production and do
not become product cleanup evidence. The prior source fails assertions for
false exit, replacement admission and lost cancellation. Corrected Clang
ASan/UBSan and GCC runs pass. The VM runner's `namespace-identity` case uses an
external seccomp notification supervisor, with no injected library in the VM,
to exercise the same condition through Wasm and read-only DAP projections.

### Namespace-release receiver failure

Both model/component and DDS launchers use a private, one-direction Unix
sequenced-packet socket for the one-byte namespace-release permit. The parent
sends with `MSG_NOSIGNAL`. A receiver can exit after the pidfd liveness check;
that race must return a controlled I/O failure, latch cancellation and retain
cleanup ownership. It must not deliver SIGPIPE to the host or require changing
the process-wide signal disposition. This release lane is separate from the
earlier launch gate and carries no guest-selected address or network traffic.

`worker_release_failure_checks.py` kills the actual pinned launcher and
namespace init immediately after the real liveness poll. The previous source
terminates with SIGPIPE for both launchers. Corrected optimized Clang, GCC and
ASan/UBSan builds return the expected error, publish no readiness, retain the
default SIGPIPE disposition and confirm actual cleanup. This is a direct
production-launcher regression; by itself it does not supply process-domain
ownership when namespace identity has never been acquired.

### Kernel-owned process domain from birth

The model/component and DDS launchers now create an outer PID namespace with
`clone3(CLONE_NEWUSER | CLONE_NEWPID | CLONE_PIDFD)`. The kernel returns the
namespace-init pidfd atomically with creation, before any launcher or driver
instruction executes. Startup no longer acquires ownership by interpreting a
PID reported by the launcher. A late denial of `pidfd_open` cannot disconnect
the process tree from its cleanup owner.

The preparing owner is pinned first. Signals are blocked across the raw clone,
the fixed child resets inherited dispositions, installs its parent-death signal
and verifies the pinned owner is still alive. The parent maps exactly its own
effective UID/GID. The birth pidfd must remain live across opening the target
proc directory; that directory descriptor pins subsequent map writes. An
already-exited or automatically-reaped child cannot redirect those writes via
numeric PID reuse. Mapping, clone, placement or gate failure refuses startup.
There is no fallback to an ordinary unconfined fork.

The child applies the prepared CPU/NUMA policy using namespace-aware parent
provenance, verifies the selected masks, closes unrelated descriptors and
immediately executes the sealed launcher. It does not remain a forked guardian
retaining the VM's address space. The launcher's child-PID field is bounded
syntax in that namespace; it grants no host process-control authority. Startup
duplicates the already-owned birth pidfd and uses the existing directional
`MSG_NOSIGNAL` release channel.

Killing and observing this outer init covers daemonized and nested PID-namespace
descendants. `cleanup_confirmed()` requires actual reap of that init; neither a
signal request, timeout, startup receipt nor test-fixture rescue supplies the
proof. Initial owner-pin refusal creates no process: public status retains
`launch_failed`, zero members and `exit_observed=false`. Gate refusal after
creation instead retains the same first cause and reports exit only after
cleanup. Exact start retries remain inert in both cases. Console/DAP/IDE views
use these independent failure and exit fields without reading native payloads.

`worker-born-domain-pinned-qualification.json` binds 823 actual VM cases to
runtime SHA-256
`d865f87e3e759a7710f6e878450f9bdbf388c20f623a8ae96278901ad09fa039`.
Eight component variants contribute 88 cases: three launch refusals, constructor
and call crashes, startup descriptor refusal, launcher death and release-peer
death. Each covers Wasm32/64, memory zero/one, interpreter/AOT and full/read-only/
limited DAP. External fault observers independently pin descendants and verify
they are gone before any fixture rescue. The aggregate also retains the
separate model, DDS, MuJoCo, module-exchange and default-denial suites.

Three separate native receipts qualify optimized Clang, GCC and sanitizer
launchers; inherited signal-mask preservation; real nested/daemonized descendants;
owner death; stale mapping identity; and immediate exec. The exec fixture reserves
1 GiB in the parent and verifies the external child does not retain it. These
are Linux x86-64 process-lifecycle results. They do not establish per-incarnation
cgroup memory/CPU admission, arbitrary vendor-host compatibility, durable
replacement, real-time scheduling, or physical stopped state.

### Owner death at real worker startup boundaries

The follow-up `worker-born-domain-pinned-extended-qualification.json` verifies
**2,914 actual VM cases across 30 receipts** against the same executable hash.
Four separate native-core receipts are retained without adding them to that
VM count. The additional 2,091 cases cover action selection, action budgets,
bounded types, graph, logs/diagnostics, model boundaries, parameters, proposals,
recording/replay, revocation, process controls, local robot bindings, sensors
and transforms. The aggregate verifies every runtime binding and receipt hash
and confirms unchanged OOM event counters. Closed providers, denied operations
and narrow simulation cases remain distinct from full vendor qualification.

`worker-born-domain-owner-death-outlined/results.json` adds 18 actual owner-kill
cases against the same frozen birth-domain source. Both the model/component
launcher and the DDS launcher are tested under optimized Clang, GCC and
Clang ASan/UBSan. Every compilation uses `-O3 -g`, assertions and warnings as
errors. The three kill points are before the real startup report is consumed,
after readiness, and after attachment. The first point interrupts the production
read boundary; it does not replace the launcher's ownership decision.

An independent observer receives root and worker pidfds before killing the
actual owner with `SIGKILL`. Both descriptors must report exit before fixture
rescue; rescue cannot turn a product failure into success. The observer acts
as a subreaper to collect remaining descendants. These checks use the same
64 GiB, zero-swap gate, retain the image/source hashes, and leave memory-event
counters unchanged. They are native-core evidence, not additional executions
of the full VM or evidence that physical actuators stopped.

## Durable recovery without synchronous storage in each servo cycle

Persistence records authority epochs and lifecycle transactions, not live
pointers, descriptors, native objects or reusable guest capability handles.
Records bind the deployment, package/configuration digests, monotonic transaction
sequence, previous-record digest, instance/epoch, stage, first fault and unknown
effect state. Lengths, entry count and bytes are bounded before parsing. A hash
chain detects corruption relative to a trusted head; it does not authenticate an
attacker-controlled history or prevent rollback by itself. Durable authorization
requires a separately trusted integrity key and monotonic checkpoint or refusal
when that trust is unavailable.

The deployment service's trusted storage provider owns an exclusive journal
location with reserved bytes/inodes and no guest filesystem path. It writes the intent, completes the
required file and directory durability barriers, then publishes the commit.
The actual filesystem and storage-device durability contract must be qualified;
successful `write` alone is insufficient. Full storage, a flush error, malformed
records, an incomplete commit, an unknown schema or conflicting history keeps
the instance fenced. Logs cannot consume recovery or stop reservations.

Do not put a filesystem flush in every high-frequency control iteration. Before
activation, durably record an epoch as potentially effectful. After an abnormal
VM/supervisor exit, that entire epoch is conservatively unresolved. Volatile
per-command receipts are recovered only if an authenticated external receiver
retains them; otherwise their outcomes remain unknown. This permits bounded
in-memory control transport without pretending to provide exactly-once physical
effects across power loss. A deployment demanding durable per-command receipts
must explicitly select and qualify their additional latency.

Recovery validates history, reestablishes observation of old process domains,
fences old receiver epochs and obtains current independent state evidence. It
does not restore grants, controllers, deadlines or pending motion from disk.
It exposes a bounded reconciliation report to the trusted operator. A fresh
authorization and incarnation are required for subsequent activation. Expired
deadlines are not extended; wall-clock rollback does not create fresh leases.
Crash records exclude raw guest/native memory and credentials by default.

## Replacement, rollback and dependent robots

The deployment supervisor stages a replacement with an independent package/
configuration digest and no effect authority. Validate its closed protocol and resource demand while the
old incarnation is still identifiable. Both instances must fit the admitted
resource budget; inability to reserve both requires an explicit cold transition.

Fence and drain the old instance, obtain current independent stopped-state
evidence, commit the replacement intent, and create a fresh receiver epoch.
Only one incarnation may hold the hardware control lease. The receiver must
reject commands from the old epoch even if an old process resumes or buffered
packets arrive late. A vendor protocol without this guarantee cannot use a live
handover profile; use a qualified cold stop and operator reconciliation.

Atomic publication selects the new identity; it does not make device effects
transactional. Failure before publication leaves the candidate inert. Failure
after publication fences the new instance and retains unknown outcomes.
Rollback is another authorized replacement using a fresh epoch, never revival
of old handles or replay of the previous command queue. Version pinning and
firmware compatibility are checked independently of package authenticity.

For cooperating robots, controller ownership, reference frames, clocks,
freshness and each participant's original authority are separate dependencies.
Failure of one participant fences only the configured dependency closure and
applies the declared coordinated stop policy. A common group does not confer
peer device authority. A disconnected participant cannot renew a lease through
another participant's identity. Distributed success is not inferred from local
IPC acceptance, and a partial multi-robot effect remains explicit uncertainty.

## Independent stop and fault evidence

Keep three observations separate: a stop request was accepted; the driver or
namespace exited; the physical equipment reached its configured stopped state.
None implies either of the others. The observation path must not depend solely
on the failed driver, VM scheduler, normal IPC queue or model inference output.
Deployment specifies the independently powered/clocked receiver or safety
controller, expiring authority, stop signal, feedback source, freshness and
required state predicate. Host-process isolation alone cannot establish this.

A missed heartbeat, VM `SIGKILL`/pause, driver crash/hang, protocol corruption,
full command queue, CPU starvation or network loss fences normal control and
causes the external lease/watchdog to act. Repeated stop requests are bounded
and idempotent. An ACK never clears earlier unknown motion. Re-arm requires an
explicit trusted decision and fresh independent feedback; a guest, new model
output, automatic restart or recovered journal cannot re-arm the equipment.

Tests must measure fault-to-fence, fault-to-stop-signal and fault-to-physical-state
separately. The retained MuJoCo tests exercise actual simulated dynamics and
injected process faults, but their watchdog is a simulation fixture. They do not
validate an independent hardware safety circuit, electrical power loss,
mechanical stopping distance or an industrial certification claim.

## Qualification gates and observability

Before advertising a general host, retain evidence for original executable and
shared-library packages: successful launch, constructor crash, nested process
crash, daemonization, exec attempts outside the closure, descriptor leaks,
resource exhaustion, PID reuse, failed handshakes and shutdown escalation.
Observe that VM authority and unrelated instances survive each native failure.
Repeat on every claimed OS/ABI with actual available vendor binaries.

The current `run_scene_component_crash.py` regression offers `--fault call`,
`--fault constructor`, `--fault unannounced-exit`, `--fault reported-failure`
and `--fault malformed-frame`. The first two fault inside
an actual isolated shared library; the constructor case precedes readiness.
The third exits with OS status zero without the required completion frame and
must still retain channel failure. The remaining cases distinguish an explicit
negative completion from an unknown frame kind. All require failed state and
confirmed process exit, preserve the first cause across a successful stop,
refuse to relaunch on an exact start retry, and check detached
full/read-only DAP inspection and limited-access denial. They qualify the closed
component host, not an arbitrary vendor ABI or an independent physical stop.
`--fault completion` and `--fault controlled-stop` are positive controls: their
first-failure value remains zero, including the retired debugger projection.
The retained `component-first-failure-runner-results.json` cohort passed all
seven variants (77 VM/DAP cases), 340 explicit interface-denial/hidden-import
cases, 82 module observation/message cases, four message DAP cases and 128
MuJoCo scenarios. Its VM SHA256 is
`47bebd06664f2b9d8cbc29d77190d7286b2566e6317b1d7e91ecf72c6396bb9a`.
These receipts are under the SDK `.tmp/ros-scene-security` directory. The later
start/stop race correction is separately covered by
`component-start-stop-race/results.json`, with Clang ASan/UBSan, GCC and TSan;
it is not retroactively included in that earlier VM hash.

Inject interruption before and after each journal/promotion boundary, including
short writes, full disk, failed flush, truncated/corrupt/replayed history, old
worker resurrection and delayed old-epoch messages. Recovery must remain fenced
with no automatic motion, authority resurrection or duplicate activation.
Test update failures before and after device ownership changes and demonstrate
that rollback cannot reactivate stale authority.

Use identical workloads to compare native in-process, isolated native, and
Wasm plus isolated native. Report clock source, affinity/NUMA, scheduler/IRQ
load, frequencies, payload, copies, queue age, throughput, per-request latency
and deadline misses. Separate batch-mean distributions from individual tails.
Collect long `-O3 -g` workloads for VTune source/instruction attribution; verify
the actual counter period, losses and throttling. Sampling interval does not
set the minimum measurable latency of a timestamped batch.

Guest observations expose only authorized logical identities and detached
lifecycle, fault, resource, update and reconciliation states. They never return
native pointers, executable paths, descriptors, credentials or process-control
tokens. Debugger full/read-only/limited policy applies separately. Adding a
lifecycle field requires corresponding CLI, debug text, DAP and IDE decoding
plus nonmutation/denial tests before release.

The existing receipts establish specific component/device and message
properties in the current worker implementation. The attachment-only VM
boundary, deployment-owned general vendor launch, durable recovery/promotion,
hardware stop qualification and ecosystem-wide package coverage remain
separate acceptance gates. Neither this contract nor a count of indexed ROS
packages closes them.
