# Sensor, device and control provider boundary

These thirteen registered operations use three distinct provider layers. The
sensor carriers have central VM/CLI/debug integration for the closed
`bounded-sensor` pipe/raw-terminal adapter. Device/control carriers now use the
common framed dispatcher and independent supervisor; actual VM interpreter/AOT
fixtures cover memory32/memory64, authority and malformed memory boundaries.
Historical core cohorts below describe their original narrower scope. They are
not the current integration status. Production vendor module registration,
qualified hardware transport and physical stop validation remain separate.
See [platform extensions](ros-platform-extensions.md) for concrete conversion
scope, live attenuation and performance evidence. SDK production code is outside
this implementation scope; these changes do not alter legacy WASIp1 or ROSv1.

Sensor resources are ingress-only. Their immutable profile fixes transport
identity, message schema/encoding, frame and units, clock provenance and bounded
complete-record storage. A source adapter validates the complete frame before
publishing it. The VM never exposes a general serial/socket send operation
through a sensor resource. `sensor.take` copies a complete canonical delivery
before consumption; rejected/truncated frames and explicit overflow loss are
observable. Source time and host receive time remain distinct. Clock reset,
provider restart and sequence discontinuity cannot silently relabel old data.

Device resources represent one configured protocol and physical target. A
request reserves its entire command and recovery record before dispatch. The
immutable schema describes units, frame, numeric bounds and observation policy.
Nonfinite values and ambiguous/missing quantities fail closed. Enqueue acceptance
is separate from final execution admission, which must again validate current
source/target authority, provider boot generation, controller owner and lease
epoch, deadline and physical limits. The receiver must enforce its fencing
contract too; host-only checks cannot retract an already transmitted command.
Partial transmission, missing acknowledgement and restart preserve an unknown
outcome until qualified reconciliation. Unknown is never an automatic retry.
Controller cancellation while a transmitted command awaits acknowledgement also
resolves to unknown; it must not leave the command permanently running. A stop
receipt cannot prove whether that preceding command executed. See the actual
[MuJoCo and native-fault qualification](ros-native-simulation-qualification.md).

Control resources reserve an exclusive controller lease and a complete
sample/update/write cycle. Ownership, configured priority, authority epoch,
controller lease epoch and provider boot generation are distinct values. A
higher-priority takeover requires explicit deployment policy and fences queued
work. Reopen/reconnect does not renew a lease. A sample sequence and write
deadline belong to the same cycle; missed phases are reported, not silently
skipped by optional throttling. Freshness uses steady host time in addition to
source clock/epoch evidence. Per-joint position, velocity, acceleration, jerk and
effort constraints are applied only for the explicitly selected control modes;
there is no implicit unlimited fallback for omitted bounds.

Stop capacity is separate from normal command, model and unknown-outcome
storage. Duplicate stop requests may coalesce only under the same immutable
stop profile and fencing epoch. A requested stop, transport acceptance, driver
acknowledgement and independent evidence of stopped equipment are separate
states. Optional guest rate restrictions cannot consume the deployment's
independent stop reservation. A core method that checks a deadline when called
is a deadline gate, not an autonomous watchdog. A provider requiring an
independent watchdog must reject activation when its backend cannot establish
that capability. No latch is rearmed by capability reopening or lifecycle
activation.

All deployment limits start at zero. Granting a named operation establishes its
stable scope. Optional call/byte attenuation is a later explicit restriction;
missing rate limits are unlimited within the granted scope, while explicit zero
refuses an operation. Stable storage, worker, stop and recovery capacity remain
separate from rate attenuation. Hot paths use preallocated or explicitly
reserved storage and the recoverable fast_io allocator; allocation failure is a
reported error and cannot terminate the host.

## Upstream semantic evidence

The checked-out `ros2_control-jazzy` source under ecosystem source key
`5e34a1e5517c912902203359` provides the inspected `SystemInterface`,
`HardwareComponentInterface`, `ResourceManager` and joint-limit definitions.
`SystemInterface` distinguishes inactive read access from active motion-command
availability. `ResourceManager::claim_command_interface` rejects an already
claimed command interface, and command-mode changes have separate prepare and
perform stages. The proposed controller boundary preserves exclusive ownership
and separate validation/effect stages while adding explicit epochs and leases.

The inspected `sensor_msgs/JointState.msg` from `common_interfaces-humble`,
source key `7d85f255500a93b11e5dfac5`, distinguishes angular/linear position,
velocity and torque/force units; joint arrays must either match the name array or
be absent. The provider schema must retain this distinction rather than infer a
unit or substitute zero for an unavailable measurement.

The checked-out `transport_drivers-main` serial implementation, source key
`a589e7fc954f8bd9a63cb7b4`, uses `read_some` / `write_some` and asynchronous
counterparts. A successful partial write is not a complete protocol command or
remote execution. The bounded adapter must retain framing progress and distinguish
no bytes written, partial frame, full frame, acknowledgement and unknown outcome.
Native paths/endpoints are selected only by trusted deployment configuration.

## Qualification boundaries

Core tests will exercise finite slots and actual recoverable allocator failures,
failed delivery/retry, expiry, explicit zero rates, reentrant/concurrent revoke,
provider/controller epoch changes and independent stop reservation. Actual OS
protocol tests will use a configured PTY and a separate protocol receiver process,
with partial frames, late/missing acknowledgements and reconnection injected.
Those tests qualify the portable state machine and OS protocol boundary only.
They are not evidence of a physical actuator, a certified stop category, SIL/PL,
real-time schedulability or industrial hardware qualification. Native ROS
controller/plugin/driver adapters need their own concrete tests before any
near-complete ecosystem portability claim.

## Implemented portable cores

`device_command.h` implements the registered `robot.scalar.command` / `le64`
protocol. Every command repeats the exact configured frame, ordered field names
and SI quantity names; all fields are present and finite, and each represented
value must lie within its explicit finite range. There is no implicit unit
conversion or omitted-field default. This scalar schema does not by itself
enforce coupled kinematics, collision avoidance or trajectory derivatives. Such
commands require a separately registered complete validator and a corresponding
qualified receiver. `device-command-first/results.json` records four test groups
and nine successful Clang ASan/UBSan, GCC 13 and TSan commands with `-O3 -g` in the
existing 64 GiB zero-swap cgroup, including NaN/sNaN and floating-point state.

`device_core.h` provides a fixed request ledger with at most sixteen handles,
sixteen retained normal requests and eight actual sealed-buffer pins per request.
The configured storage reservation includes the entire fixed core, even when
deployment limits are smaller. No hot operation allocates. Commands validate
their complete scalar body and compute an exact fingerprint over the device,
key, deadline, controller principal/epoch/lease, schema and all observation
witnesses. A successful next key acknowledges the prior replay receipt; rejected
submissions never consume keys. Historical exact retry does not acquire fresh
pins, execute again or need a still-fresh observation.

Claiming a device request produces detached fixed metadata, while the original
sealed allocations remain pinned. `cross_effect` is a separate final local gate;
the actual provider must validate current controller ownership and the complete
source authority group, then hold that group across this gate and the bounded
nonblocking native operation. A local `running` or `queued` receipt is not remote
execution. The core checks actual buffer identities and digests, but only a
trusted source facade can establish source/capture provenance. Guest numbers
cannot establish it. Unknown completion retains normal storage and pins, latches
the target, cancels unstarted commands and reserves an independent stop request.
Provider restart invalidates old completion evidence; fresh trusted
reconciliation is required before unknown debt can be released.

Stop acknowledgement and separately witnessed stopped state are distinct. Explicit
trusted rearm requires both, no remaining unknown/worker debt, and a replacement
stop identity reserved before clearing the latch. Closing a handle retains its
slot while command or stop recovery records remain. Releasing a terminal receipt
hides ordinary lookup but preserves the last exact-retry tombstone where its
handle still exists; it never rearms equipment. The ledger grants no authority,
has no native transport or autonomous watchdog, and is not durable crash recovery.
`device-core-stop-retention/results.json` records six groups and nine successful
ASan/UBSan, GCC and TSan commands, including real sealed pins, allocator-disabled
hot paths, concurrent claims and fork rejection. It is core evidence only; the
actual provider and the thirteen public imports still require integration.

`device_protocol.h` defines separate command, stop and acknowledgement frames.
The entire fixed header and complete canonical payload are covered by SHA256;
this detects corruption but does not authenticate a peer. The configured transport
must isolate/authenticate that peer independently. The receiver checks the exact
provider generation, monotonic channel sequence, separately installed controller
lease and authority epochs, deadline, schema, frame, units and bounds before its
native effect. Acknowledgements bind the complete original frame digest as well
as request and generation. A stop latches the receiver and clears the installed
controller; a fresh trusted stopped witness and a newer controller epoch are
required for subsequent commands. Command input cannot install or renew a lease;
framed controller installation uses its separately trusted management lane.

For this scalar carrier the accepted command deadline also bounds its active
lifetime. The independent native watchdog must enforce the earlier of that
deadline and controller expiry. A replacement controller preserves the active
command deadline, and a rejected conversion cannot extend it. A command or
controller arriving after expiration is refused while preserving the pending
watchdog event. Calling `expire` then latches the gate and requests the fixed
stop exactly once; a successful zero request still does not prove physical
stopped state. Trajectories with different completion semantics need a separate
qualified contract.

The fixed incremental decoder accepts fragmented input without interpreting a
partial command. Malformed framing permanently poisons that decoder session;
it never scans an untrusted stream for a later magic value. The shared-clock
profile requires a verified host-steady clock domain. Cross-machine execution
needs a separately qualified clock mapping and authenticated transport, not a
blind comparison of unrelated clocks.

`device-protocol-second/results.json` records five groups and nine successful
ASan/UBSan, GCC and TSan commands. Its real OS test creates three PTYs and a
separate receiver process. A complete command changes a memfd value and produces
an acknowledgement on the isolated channel bound to the complete frame digest. A second command is
left at a partial header while the independent stop channel is delivered; the
stop changes that value to zero, and completing the old command is refused.
This qualifies framing, process isolation and an OS fixture effect only. The
fixture is not a physical actuator, native ROS driver, autonomous watchdog or
industrial safety qualification.

`device_channel.h` supplies the actual bounded nonblocking stream primitives.
The sender accepts a trusted preopened PTY only; the acknowledgement receiver
accepts a PTY or an explicitly read-only anonymous pipe. Regular files, generic
sockets, other character devices and pipe writes are refused. Refusing pipe
writes avoids an implicit process-wide SIGPIPE policy. Each object owns one
close-on-exec duplicate and has a fixed frame buffer. A descriptor exhaustion
failure is recoverable and does not leak a descriptor or alter the output object.
Multiplexed PTY identity includes the kernel PTY number, since `/dev/ptmx` inode
and device numbers alone do not distinguish two terminals.

A send pump performs one write of at most its explicit bound (hard maximum
4096 bytes), rechecking nonblocking mode beforehand. Deployment must not retain
uncontrolled aliases that concurrently alter the shared file-description flags.
Expiry before any write is a local no-write failure; expiry, retirement or error
after a partial write retains unknown debt. A complete write waits for an ACK
bound to the original digest. An invalid ACK locks that sender in unknown state;
there is no blind reset/retry method. Normal and stop senders are separate
objects; deployment must additionally prove independent channels, workers and
capacity. No thread or watchdog is created implicitly by these primitives.

`device-channel-second/results.json` records nine successful ASan/UBSan, GCC and
TSan commands, rerunning the five protocol groups and four additional channel
groups. The actual OS tests cover file-kind/direction refusal, EMFILE injection,
descriptor lifetime/leaks, partial writes, independent stop progress, ACK
corruption, concurrent pumps and inherited-process rejection. The public device
authority dispatcher and autonomous backend supervision are qualified separately
from these earlier channel-only cohorts.

`sensor_core.h` now provides fixed-source configuration, finite open/take/close,
validated complete-frame ingestion and cached inspection. Source identity, native
sequence availability and exact source-clock identity are explicit; backwards
epoch/time and stale sequence fail closed. A new provider generation requires a
new core, never renewal of an old handle. All open handles on one configured core
share an explicit competing-consumer queue; broadcast requires distinct queues.
Loss counts copied with a delivery include eviction already observed after that
surviving frame was captured. The qualifying receipt is
`../uwvm2-ros-sdk/.tmp/ros-scene-security/sensor-core-delivery-loss/results.json`: six
safety groups across nine successful Clang ASan/UBSan, GCC13 and TSan commands.
Two actual fast_io configuration failures recover; open/ingest/take/close require
no further allocation. Later native transport and VM integration have separate
receipts; this core receipt alone does not establish either.

`control_arbitration.h` provides prepare/copy/commit ownership, bounded priority
preemption, final-effect fencing, unknown-effect debt, and separate stop
bookkeeping. A prepared replacement cannot bridge expiry of the old controller.
Stop identity is reserved before normal ownership is enabled and replenished
before trusted rearm. Its internal slot remains scoped by provider generation; a
reconnect invalidates old acknowledgements and preserves unresolved effects. The
slot is not itself a guest request receipt or a physical stop certificate. Trusted
reconciliation requires the current provider generation and fresh evidence ID.
A stop acknowledgement and separately witnessed stopped state are both required
for trusted rearm, with no outstanding effect debt. The final pure-core receipt
is `control-arbitration-final/results.json`: five groups and nine successful
optimized/debug sanitizer/compiler commands. The core has no transport or timer
thread; expiry and latched state never imply equipment has stopped.

## Typed sensor provenance provider

`sensor_source.h` now owns the actual `sensor_core` and binds each opened resource
to the originating caller and exact source capability. Native ingress is refused
until an explicitly authorized source handle exists. The `bounded-sensor` grant
permits only the selected sensor operations and independent `resource.observe` /
`resource.close` actions; it cannot select a transport endpoint or send bytes.

A successful typed `take` obtains the core's detached complete validated frame,
creates and seals a byte-layout buffer in the exact preconfigured auxiliary
buffer alias, and copies the delivery before publishing its provenance. A failed
copy, allocation, source/buffer retirement or reentrant close preserves the queue
item and closes the new storage. No capability is opened implicitly. The typed
operation's optional byte budget charges the actual frame body; the guest
carrier accounts for its canonical request/reply framing separately. Explicit
zero byte allowance rejects a nonempty frame before allocation or delivery.

The finite history contains at most sixteen real sealed allocations. Replacing a
full history needs an additional object slot; separately retained model pins may
still exhaust the configured buffer budget. Closing an evicted handle never
refunds a surviving pin's physical storage. The source facade supplies actual
resource ownership, provider generation, capture sequence, source/receive clock,
calibration/transform epochs and a handle-specific authority epoch. It does not
accept a caller-selected buffer as proof of native provenance. Proposal capture
independently requires the caller's explicit buffer capability and exact profile.

`latest_required` rejects an observation as soon as a newer native frame is
committed, including before that frame is taken. `bounded_age` permits only a
still-retained historical witness in the same source clock incarnation; actual
age limits remain the proposal/device policy's responsibility. History eviction,
source close, provider retirement and clock-epoch changes refuse validation.
An unstamped source explicitly uses its host receive clock and host capture time.
Host steady values outside the signed time range are refused before ingestion.

`sensor-source-second/results.json` retains the source closure for fourteen
core/provider groups and nine successful Clang ASan/UBSan, GCC13 and TSan commands,
all with `-O3 -g` in the existing 64 GiB zero-swap cgroup. It includes three actual
recoverable fast_io allocation failures, pinned-capacity exhaustion, failed-copy
replay, default-zero and explicit-zero authority, wrong-caller/capability refusal,
reentrant and concurrent retirement, clock boundaries and fork rejection.
This qualifies a typed source provider. The three sensor guest carriers, native
transport identity, CLI deployment and debugger integration have their own
qualifications beyond this earlier core cohort.
Teardown must retire and join active callers before destroying this provider or
its borrowed buffer registry. The incomplete first cohort records a GCC strict
indentation warning and is not the qualification receipt.

## Reserved control phases over the actual device ledger

`control_cycle.h` binds an already committed controller lease to an already open
device in the same environment/provider generation. A finite write window defines
one complete sample/write phase. Deployment must reserve optional allowance for
that entire phase before sampling; independent attenuation of its sample and
write steps would make an admitted cycle unreliable. Capacity limits remain
separate from that optional allowance.

The sample retains the actual sealed observations and returns an exact sequence
and deadline. Failed delivery releases a new sample reservation without advancing
the sequence. Repeated delivery of an outstanding phase returns that same phase.
Submission checks the same sequence, strict next request key, complete command
hash, original observations, current controller and deadline before queuing into
the actual device ledger. Clock and source callbacks run outside its mutex;
ownership, elapsed time and sealed pins are checked again afterwards.

An unknown result, elapsed phase or lost controller latches both independent
controller and device stop obligations. The unknown effect remains outstanding
until trusted reconciliation. A stop ledger entry is neither native stop delivery
nor physical stop evidence. This component has no timer thread or watchdog and
cannot activate equipment on its own. The integrated authority dispatcher and
native effect boundary recheck the complete source/target group and controller
immediately before writing to the fixed driver channel.

`control-cycle-second/results.json` qualifies thirteen device/cycle groups with
nine successful `-O3 -g` Clang ASan/UBSan, GCC13 and TSan commands in the existing
64 GiB zero-swap cgroup. These tests cover actual ledger submission, receipt
recovery, retained buffers, failed delivery, reentry, missed phases, controller
expiry/preemption, unknown debt and independent stop capacity. They do not qualify
a hardware watchdog, physical controller or industrial safety certification.

## Fixed native receive channel and guest sensor records

`sensor_ingress.h` implements a complete-frame receiver over an actually bound,
nonblocking receive-only pipe or raw terminal. Trusted deployment supplies the
descriptor and isolates its writer. Guest records contain no descriptor, native
path, address, direction or plugin symbol. The receiver duplicates the checked
endpoint; it never changes the original file-description flags. The deployment
must retain exclusive control over those shared flags and terminal settings.
Canonical terminal buffering, echo, signal processing, flow-control/newline
rewriting and non-eight-bit terminal modes are refused at binding.

The frame header is 152 bytes: magic, body extent, source/generation/native
sequence/clock/epoch/source-time/stamp-presence words, exact configured schema
digest and complete-frame SHA256. Scalar fields are little-endian 64-bit words;
digests have the normal length-prefixed byte encoding. The digest covers the
112-byte semantic header prefix and complete payload. It detects corruption; it
does not authenticate the writer. Source incarnation and timestamp availability
are also checked by the private source core. No frame is committed until all
bytes and the schema validator succeed. A malformed, truncated or closed stream
latches the receiver and retires its source. It never scans for another magic
or silently attaches a replacement peer.

Each pump obtains a real live source handle's current `sensor.open` authority
before at most one bounded read. This is validation only: it neither charges
nor consults the original creation-rate allowance. Core clock and schema
callbacks run outside its mutex, followed by a final authority/handle check
before frame commitment. Capability closure, reentrant revocation, source close
and retirement cannot keep ingesting through an old open receipt. Complete frames
blocked by queue capacity stay in the receiver; no new bytes are read until the
retained frame is admitted or the source is retired.

`sensor_dispatch.h` owns one real source and native receiver per private alias.
It requires a finite binding/storage budget and an actual checked endpoint; an
unconnected descriptor entry cannot open a sensor. A trusted scheduler may pump
the fixed binding, and a guest `sensor.take` on an empty queue performs at most
one pump. Neither path creates a polling thread or watchdog. Already queued
deliveries are retried before touching the peer, preserving lost-copy recovery.
The three canonical operations are:

- `sensor.open`: exact profile → caller/capability-bound sensor handle.
- `sensor.take`: sensor handle → generic delivery, actual sealed buffer handle,
  payload schema profile, raw-buffer profile, frame, units, native source ID and
  optional native sequence. Generic metadata's sequence is the host capture
  ordinal. Native timestamp availability is explicit, receive time names its
  actual host clock/epoch, and publisher identity remains absent.
- `sensor.describe`: sensor handle → selected profile and a bounded
  `ros.sensor.state` / `le64` message containing configured schemas/frame/units,
  sequence/stamp availability, finite capacities and cached source/receiver
  counters. It performs no native read or queue advancement.

`sensor.take` requires both the ordinary action and independent grant-only
`data.export` permission before any native read or body disclosure. It validates
the exact caller/capability-bound handle first. The output span must accommodate
the binding's maximum complete reply, so a short span fails before reading,
allocating or advancing a queue. The eventual charge is once for the request
and actual encoded reply. `open` and `describe` likewise account for their
actual canonical transfer. Failed copy or allocation preserves the source queue
and closes unpublished buffer storage. No buffer capability is opened implicitly;
the returned buffer handle becomes proposal input only after a separate explicit
buffer capability is supplied to `proposal.capture`.

`sensor_configuration.h` parses a digest-bound definition under an exact trusted
schema registration. The definition contains source/clock incarnation,
frame/units, buffer alias/profile and explicit reject/drop-oldest policy. It
cannot select its validation function or endpoint. All eleven capacities must
be assigned exactly once before freezing:

- `source.handles`, `source.records`, `frame.bytes`, `source.storage.bytes`.
- `history.frames`, `buffer.objects`.
- `ingress.frame.bytes`, `ingress.read.bytes`, `ingress.retained.bytes`.
- `workspace.bytes`, `workspace.calls`.

Every zero is literal and missing configuration cannot activate a source.
Native retained bytes include the real fast_io array prefix; source core,
detached carrier workspace, receiver storage and sealed-buffer storage have
separate bounds. Ingress has no additional rate attenuation in this provider:
an explicitly enabled source can continue within its configured scope/capacity.
`ingress.read.bytes` limits one syscall, not reads per second. Optional guest
delivery rate budgets remain independent and missing rate fields are unlimited
only within the explicit grant. Any future ingress attenuation must have an
explicit field and cannot borrow open/take creation fees.

This is a fixed source transport adapter, not an implementation of every ROS
sensor driver. A deployment still needs its real driver to emit the closed
framing and exact registered schema, to isolate the native peer, and to establish
any required physical supervision. The dispatcher has a real guest handle path;
VM import registration and CLI/debugger integration are separately qualified.

`sensor-native-second/results.json` qualifies the existing fourteen core/source
groups and eight native/guest groups across nine successful Clang ASan/UBSan,
GCC13 and TSan commands, with `-O3 -g` in the existing 64 GiB zero-swap cgroup.
All 426 source files and snapshots remained unchanged. Real independent writer
processes use both a read-only pipe and raw PTY; guest open supplies the actual
handle used by take and proposal provenance. Tests cover pre-read export/handle/
capacity refusal, lost-copy retention, independent open-rate validation, malformed
and truncated frames, exact configuration, allocation failures, clock callback
revocation/close/retirement, concurrent retirement and fork rejection. The first
cohort passed ASan/UBSan but stopped at strict GCC formatting warnings; it remains
preserved separately. The final cohort's cgroup memory counters were unchanged.

`sensor-proposal-chain-first/results.json` separately qualifies three complete
native-process → guest sensor → guest capture → actual model.submit/take → guest
publication/read/accept chains. All nine Clang ASan/UBSan, GCC13 and TSan commands
passed. The model verifies bytes unique to the native writer, and the capture
uses the handle returned by the sensor guest carrier. Lost sensor/publication
copies recover; closing the capture retains published pins. A genuinely newer
native sample refuses the old model result under latest-required policy.
Acceptance does not advance the actual lifecycle graph or enqueue a callback.
