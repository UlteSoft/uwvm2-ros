# Device, control, media, bridge and isolated component bindings

All 22 operations in these five families are registered under `uwvm_ros_v2`.
This document specifies the concrete bindings that execute them. Registration
does not imply that every protocol, codec, middleware distribution, operating
system or robot has a qualified adapter. The [complete operation reference](ros-operation-reference.md)
defines the canonical wire fields. The [provider procedures](ros-robot-provider-procedures.md)
define the requirements for additional bindings.

## Authority, capacity and recovery

An environment starts without exposed imports, resource grants or provider
capacity. Create it, expose each required operation, grant each exact alias and
operation, then provide its complete immutable profile and finite reservations.
Grants for source observations, source buffers, output buffers, configuration,
ROS endpoints and component roles remain independent. Neither a profile hash nor
an allocated provider supplies a missing permission.

Unspecified rate limits are unlimited within an existing grant. Explicit zero is
zero. Rates limit work within an existing grant. Configure them separately from
queue, buffer, process, stack or stop reservations. Control admission reserves a
complete sample/write phase. Its submit/result paths revalidate the original
authority without charging a second phase allowance. Explicit stop operations
use their independently granted cleanup path, even after normal traffic reaches
its limit. Revocation remains effective while ordinary output delivery fails.

A request key belongs to one owned resource. Preserve the original key and exact
request bytes when recovering a lost receipt. A changed request with the same
key conflicts. An unknown native result must not be retried as a new effect.
Receipt state and effect are separate: queued/running work is locally accepted;
successful local changes are committed; an authenticated native acknowledgement
is observed remotely. A native acknowledgement does not establish physical
execution, physical stopping or functional-safety certification.

All allocation-bearing production state uses recoverable project `fast_io`
allocation. The silent allocation path is checked and translated to an admission
failure; ordinary terminating allocation is not used to admit untrusted work.
Failed request copies, short outputs and insufficient reservations are rejected
before their corresponding native effects. Owned requests never retain guest
pointers. Inspection reads cached state without advancing deadlines, consuming
messages or performing native I/O.

## Framed scalar devices and control phases

`device` grants use `framed-scalar`; `control` grants use `framed-control`.
The device exposes `device.open`, `device.submit`, `device.result`, `device.stop`
and `device.describe`. Control exposes `control.open`, `control.sample`,
`control.submit`, `control.result` and `control.stop`. Owned resources and retained
terminal request records use the separately granted `resource.close` operation.

Each device has four trusted deployment endpoints: command transmit,
acknowledgement receive, stop transmit and stop acknowledgement receive. They
are not guest descriptors. Linux activation verifies nonblocking raw PTYs or
the supported receive pipe direction, actual endpoint identity and separation
of all four lanes. Opposite ends of the same PTY do not count as separate lanes.
Unused bindings do not write a stop frame merely because they were configured.

The immutable schema selects a named frame and ordered scalar fields with exact
SI unit labels and finite inclusive bounds. Each command is validated as a whole.
The receiver installs the exact host-issued controller lease before accepting a
command, then checks provider incarnation, controller and authority epochs,
sequence, deadline and complete frame digest. The framing SHA256 detects
corruption; deployment must independently authenticate and isolate the peer.
Host-steady deadlines require a shared verified clock domain. Network adapters
must establish a qualified clock mapping instead of treating remote timestamps
as local steady time.

`control.open` supplies the device capability and handle, followed by strictly
ordered source slots containing sensor capability, sensor handle and independent
buffer capability. The host captures actual ingress witnesses and pins their
sealed allocations. Guest timestamps and buffer identifiers cannot create
provenance. `control.sample` publishes the controller epoch, one sequence,
write deadline and complete original observations. `control.submit` must match
that phase, schema and epoch; direct `device.submit` must match the same original
authorized source cohort. Stale sources, changed generations and out-of-range
commands fail closed.

The current binding permits one controller incarnation per device generation.
It does not automatically preempt, rearm or restart. A finite guarded supervisor
advances lease expiry, command deadlines and independent stop traffic even when
the guest makes no more calls. Missing or corrupt acknowledgements latch new
commands and preserve request identity, pins and uncertainty. Stop acceptance is
reported separately from independently observed physical stopping; this binding
does not manufacture physical evidence. A qualified external receiver watchdog
and actual robot safe-state mechanism remain deployment obligations.

Configure in this order:

```text
--ros-scene-device-budget ENV DEVICES CONTROLLERS RETAINED_BYTES STACK_BYTES POLL_NS SHUTDOWN_NS
--ros-scene-device-profile ENV ALIAS IDENTITY SHA256
--ros-scene-device-definition-hex ENV ALIAS HEX
--ros-scene-device-limit ENV ALIAS NAME VALUE
--ros-scene-device-endpoints ENV ALIAS COMMAND_TX ACK_RX STOP_TX STOP_ACK_RX
--ros-scene-control-profile ENV ALIAS IDENTITY SHA256
--ros-scene-control-definition-hex ENV ALIAS HEX
--ros-scene-control-limit ENV ALIAS NAME VALUE
```

Each alias must already have its grant. Both definition commands also have a
`-definition ENV ALIAS PATH BYTES SHA256` form for an explicitly requested bounded
configuration-file read. Definitions use the canonical little-endian wire format,
not JSON. Their complete bytes must hash to the selected profile digest.

The device definition is `uwvm.ros.device.scalar`, identity, mode profile,
watchdog profile, stop profile, command schema profile, frame, and a counted
list of `(name, unit, minimum:f64, maximum:f64)` fields. Every device limit is
mandatory: `handles`, `requests`, `command.bytes`, `observations`,
`observations.minimum`, `observation.age.ns`, `command.deadline.ns`, `ledger.bytes`,
`retained.bytes`, `transport.write.bytes`, `stop.ack.ns`, `controller.lease.ns`,
`controller.prepare.ns`, and `controller.priority`.

The control definition is `uwvm.ros.control.scalar`, identity, device alias,
stop profile, and counted source rules `(sensor alias, sensor profile,
buffer alias, buffer profile, frame)`. Its mandatory limits are `lease.ns`,
`priority`, `write.window.ns`, and `retained.bytes`. Active bindings require
consistent finite storage and timing reservations. Omitted values never acquire
defaults. Supported fixed maxima are checked without truncation.

## Immutable raw media conversion

The `immutable-buffer-media` binding implements `media.open`, `media.submit`,
`media.take` and `media.close`. It supports packed uint8 mono/RGB/BGR copying
and RGB/BGR channel exchange. It does not claim encoded image/video codecs,
audio processing, viewers, listeners or network egress. A requested egress role
is refused by this binding.

The input must be sealed under its exact declared profile. Opening independently
checks `buffer.read` and `data.export` on the source and `buffer.create`,
`buffer.write` and `buffer.seal` on the destination. Submitting validates the
complete shape, planes, sequence and deadline before copying into a bounded
private output. The output is sealed before it can be published. `media.take`
publishes the complete result and buffer reference only after successful delivery;
failed copies retain the same result. Closing never closes an unrelated input.

```text
--ros-scene-media-budget ENV BINDINGS RETAINED_BYTES
--ros-scene-media-profile ENV ALIAS IDENTITY SHA256
--ros-scene-media-definition-hex ENV ALIAS HEX
--ros-scene-media-limit ENV ALIAS NAME VALUE
```

The definition fields are `uwvm.ros.media.raw`, identity, input alias/profile,
output alias/profile, codec (`copy` or `swap_red_blue`), input/output color names,
width, height and channels. The six mandatory limits are `streams`, `frames`,
`retained.bytes`, `workspace.bytes`, `workspace.calls`, and `deadline.ns`.
Both buffer plans must independently cover the actual shape and transfer size.

## Directed typed topic bridges

The `topic-forwarder` binding implements `bridge.create`, `bridge.start`,
`bridge.stop` and `bridge.describe`. It connects a dedicated subscription to a
dedicated publisher using the compiled `std-string` or `std-bool` canonical codec.
The current native adapter is the isolated ROS 2 worker. ROS 1 conversion and
service/action bridging are not provided by this binding.

Creation checks both independent endpoint origins, directions, type profiles,
codec, message bounds and deadlines. Endpoints cannot be shared with another
configured bridge or component role. Equal topic names in equal ROS domains are
rejected. Deployment still must review cycles through external native graphs.

The source is reserved first. Its exact record remains retained until the
destination publication produces a known acceptance receipt and source
acknowledgement succeeds. Local/native acceptance does not prove remote delivery.
Partial failure and unknown publication latch the route with its original
transaction identity. Stop fences new forwarding, drains or releases only the
owned reservation, and preserves unresolved debt. It has its own recovery key
and reserved control capacity.

An empty receive result is distinguished from an outstanding asynchronous RPC.
The supervisor uses an internal, separately authorized `topic.release` debt query
before declaring the route idle. That query neither reads data nor starts another
receive. Retained messages, unknown outcomes and lock contention prevent a false
quiescent result; a timeout alone never proves that cleanup completed. Closing
native endpoints and releasing their creation receipts remain explicit caller
steps after the bridge has stopped.

```text
--ros-scene-bridge-budget ENV BINDINGS RETAINED_BYTES STACK_BYTES POLL_NS
--ros-scene-bridge-profile ENV ALIAS IDENTITY SHA256
--ros-scene-bridge-definition-hex ENV ALIAS HEX
--ros-scene-bridge-limit ENV ALIAS NAME VALUE
```

The definition fields are `uwvm.ros.bridge.topic`, identity, source alias/profile,
destination alias/profile, stop profile and codec. Mandatory limits are
`message.bytes`, `retained.bytes`, and `deadline.ns`. `bridge.create` supplies a
single ordered edge with source and destination capabilities and resource handles.
The profile never manufactures those handles or grants.

## Isolated message-role components

The `isolated-roles` binding implements `component.open`, `component.start`,
`component.stop` and `component.describe`. The VM never loads component code into
its address space. Linux uses the existing sealed-image launcher, namespaces,
seccomp and pinned process identities. Required unsupported guarantees fail
activation. There is no automatic fallback to `preloaddl`, WASI sockets or broad
filesystem access. Windows and macOS launchers require separate implementations
and qualification; a Linux translation test cannot establish their isolation.

The exact bootstrap and runtime closure is pinned by file size and SHA256 and
copied into immutable executable snapshots. The approved launcher and loader
closure is part of the deployment trust boundary, including constructors that
run before the final worker filter. Only bootstrap/runtime images are accepted;
model weights or arbitrary data mounts cannot be smuggled into this binding.
Immutable configuration is delivered after the authenticated startup handshake
and final filter, under its separate `artifact.open/read/describe` grant.

The fixed worker entry point is `uwvm_component_run`. Its role protocol carries
one-based slots, bounded canonical message bytes, contiguous request sequences
and explicit receive settlement tokens. It contains no host capability values,
native endpoint identifiers, host function pointers or selectable symbols.
The host resolves each slot to the original independently authorized topic.
Publish, reserve, acknowledge and release revalidate that entire grant cohort.
Subscribers additionally require `data.export`. A child cannot turn a receive
role into a publisher or select a different endpoint in its message.

A receive timeout is reported as known empty only after an independently
authorized `topic.release` inspection confirms that the provider has no
outstanding RPC or retained reservation. An elapsed deadline, denied inspection,
lock contention or inspection exception cannot establish that fact. Otherwise
the component latches uncertainty and retires the child. Closing the component
handle prevents reuse but retains the binding's unknown state and cleanup debt
for inspection; it does not turn an uncertain receive into successful cleanup.

Preparing an environment seals code and reserves buffers plus a guarded idle
supervisor; it does not execute the component. Opening copies the exact immutable
configuration and retains explicit role handles. Starting commits one recoverable
receipt before launching the single approved incarnation. A failed reply does not
launch it again. Readiness requires the actual authenticated generation and
instance handshake. No automatic restart can erase native-effect uncertainty.

Stop fences new role messages and signals the pinned child process tree. Exit is
reported only after kernel-confirmed tree retirement. Unacknowledged source
reservations and unknown publications survive that exit. `exit_observed` means
process exit, never physical safety. If the kernel cannot confirm exit before the
diagnostic deadline, the stop receipt becomes `outcome_unknown`; the supervisor
retains ownership and continues reaping. Destruction may consequently wait beyond
that deadline instead of releasing live process resources falsely.

The description includes a bounded `first_failure` unsigned 64-bit field after
`exit_observed` and before the optional last receipt. Zero means no observed
failure. The supervisor preserves a nonzero first cause through later stop,
close and successful reaping; it never infers a signal from EOF. See the
[closed fault codes and observation rules](ros-native-driver-lifecycle.md#retained-first-failure-observation).
This is a volatile observation of one incarnation, not a crash dump, durable
recovery journal or permission to restart. Debug component `status_code` uses
the same codes. Unknown codes and inconsistent state/code combinations are
rejected by the guest-result and debugger decoders.

```text
--ros-scene-component-budget ENV BINDINGS RETAINED_BYTES
--ros-scene-component-profile ENV ALIAS IDENTITY SHA256
--ros-scene-component-definition-hex ENV ALIAS HEX
--ros-scene-component-limit ENV ALIAS NAME VALUE
--ros-scene-component-worker-limit ENV ALIAS NAME VALUE
--ros-scene-component-worker-placement ENV ALIAS CPU_LIST NUMA_POLICY NODE_LIST
--ros-scene-component-worker-launcher ENV ALIAS PATH BYTES SHA256
--ros-scene-component-worker-image ENV ALIAS ROLE DESTINATION PATH BYTES SHA256
```

The definition is `uwvm.ros.component.roles`, identity, stop profile,
configuration alias/resource/generation/SHA256, and counted ordered roles
`(alias, selected profile, publisher:boolean, codec)`. At most 16 roles are
supported by this binding; zero roles are valid. The seven mandatory limits are
`configuration.bytes`, `ipc.bytes`, `message.bytes`, `deadline.ns`, `lifetime.ns`,
`shutdown.ns`, and `retained.bytes`. IPC is explicitly bounded between 4 KiB and
1 MiB, with space reserved for complete metadata and replies.

Every worker limit must also be supplied: `image-bytes`, `address-bytes`,
`cpu-seconds`, `file-bytes`, `open-files`, `scratch-bytes`, `scratch-slots`,
`request-bytes`, `response-bytes`, `stderr-bytes`, `startup-ns`, `poll-ns`, and
`supervisor-bytes`. Request and response limits must match `ipc.bytes`.
Role endpoints must cover the configured native request/reply extents. Configuration
must match the separately prepared immutable input by identity, generation,
digest, exact length and readable transfer bound.

## Process placement, latency and evidence

Separate processes add IPC copies, scheduling, cache/TLB activity, image mappings,
address-space reservations and independently supervised cleanup. They do not
inherit an in-process latency claim. Configure whole admitted message/phase
capacities, bounded polling and deadlines before enabling work. CPU affinity and
NUMA policy are explicit deployment choices within the inherited allowed sets;
they do not create exclusive cores, elevate scheduling priority or escape the
existing cgroup. Device and bridge supervisors inherit VM placement. Component
workers use the configured worker placement. Unsupported placement is refused.

Measure cold start separately from steady state. Compare identical payloads,
security checks, message acknowledgement semantics, QoS and batching. Record tail
latency, queue residence, context switches, CPU, memory and stop latency under
representative competing workloads. Compile both measured implementations with
`-O3 -g`. Official VTune hardware Hotspots must join samples to frozen C++ source;
Microarchitecture analysis must retain actual disassembly and unresolved samples.
All test/profiler processes stay within the existing 64 GiB zero-swap cgroup.

The qualification runners are `run_scene_robot_local.py`,
`run_scene_robot_process.py`, `run_scene_component_topics.py`,
`run_scene_robot_cli.py`, `run_scene_robot_dap_live.py`, the provider/core runners, the robot request/
result codec checks and the closed dbg/DAP/editor checks. Each receipt records its
own source and binary hashes. Core tests, actual Wasm execution, ROS native
interoperability, performance and physical safety are distinct evidence. None of
these receipts alone proves all ROS ecosystem packages portable or establishes
industrial functional-safety certification.

## Qualified Linux binding cohort

The final receive-debt cohort uses VM SHA256
`3d510640a4cd04fcb73d0f752c7bc537e8b078b7bd312c88412b257fe1cd9c6d`
and a frozen manifest of 3582 source files, SHA256
`18b6e3435f44bd6aaf813e0d5464b6b58128f88ae76e28987296ddbf3dd77d3a`.
The three complete VM translation units are compiled with `-O3 -g`. The source
closure remains unchanged through compilation, execution and profiling. The
cohort preserves its qualified runtime baseline separately from concurrent
workspace changes; it does not qualify those unrelated changes.

Receipts are retained under `uwvm2-ros-sdk/.tmp/ros-scene-security/`:

- `robot-final-local`: 626 cases, including 600 actual guest executions and
  26 static policy checks. The matrix covers wasm32/wasm64, memory indices zero
  and one, interpreter and LLVM execution, all 22 operations' request/output/
  length tag mismatches, zero authority, floating-point state and positive
  device/control/media workflows.
- `robot-final-process`: 41 cases, including 32 actual guest executions through
  the sealed component launcher or typed native ROS 2 bridge, plus nine static
  policy checks.
- `robot-final-component-authority`: 23 cases. Sixteen successful executions
  exercise two ordered component roles through real ROS 2 workers across the
  four memory layouts, both execution engines and normal/FP variants. Six
  denied executions independently remove `data.export`, `topic.publish` or
  configuration `artifact.read`; one static check completes the cohort. The
  successful path forwards the exact String payload, acknowledges its original
  reservation, confirms child exit, and explicitly closes native endpoints and
  their creation receipts.
- `robot-final-dap`: three real sessions exercise full, read-only and limited
  debugger permissions. They inspect five closed provider row types, pagination,
  privacy, nonmutation and stale-stop rejection.
- `robot-final-cli`: 33 static configuration checks cover missing worker/bridge
  limits, unknown restriction names, substituted profile digests, invalid CPU
  sets and invalid NUMA policy. Invalid policies produce ordinary errors before
  activation.

These are **726 VM, CLI and DAP checks**, including 654 guest executions and
three debugger sessions; the other 69 are static checks. Separate core receipts
cover Clang ASan/UBSan, GCC, TSan, allocation refusal, recovery and concurrency.
Canonical codec cross execution covers AArch64, ARM32 and i686. The isolated
scene module graph passes 22 commands in each of its LLVM and interpreter
modes; that is not qualification of the entire production named-module graph.

`robot-component-timeout-retirement` exercises real isolated children against
empty, busy, unresolved, denied and throwing receive-debt inspections. The
controlled `robot-component-timeout-regression` experiment substitutes only the
old component dispatcher header and requires the new test to reject it. The
previous `robot-22-acceptance` record is retained as historical evidence and
explicitly superseded by this correction; its passing tests did not expose the
timeout ambiguity. Bridge receive-debt and native endpoint regressions are
retained independently.

The catalogue and design checks now report 160 registered operations and zero
planned operations, across 26 capability domains. The legacy audit accounts for
124 deprecated imports across eight modules with registered replacement
targets. These are finite inventory and implementation claims. Generic ROSIDL,
additional device protocols, ROS 1 conversion, additional codecs, Windows/macOS
isolation, physical safe-state behavior and all-package portability still need
their own concrete adapters and evidence. XML timing and instruction evidence
are recorded separately in the [output portability report](ros-upstream-output-portability.md).

`robot-final-acceptance.json` joins 25 verification receipts to the same binary
and frozen manifest. It checks all recorded source intersections, actual VM
identities, unchanged inputs, the 94-command CLI reference and the cgroup limits.
The retained source files for the ROSv2 providers match the workspace. The
aggregate also keeps explicit false qualification flags for native legacy
equivalence, all-package portability, cross-platform providers, physical
industrial assurance, near-native performance and robot transport latency.
