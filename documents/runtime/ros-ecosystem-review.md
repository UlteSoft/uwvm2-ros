# ROS ecosystem source evidence

This review supplies concrete source evidence for the independent
`uwvm_ros_v2` design. It does not change WASIp1, `uwvm_ros1_v1` or
`uwvm_ros2_v1`. The interface contract is in
[ROS host interfaces](ros-host-interfaces.md). Source acquisition and review
records are maintained by [the ecosystem tools](../../tools/ros_ecosystem/README.md).

## Evidence and coverage units

The initial catalog pins ros/rosdistro commit
`7baf55e18e949de7cb02310865538416d48479d2` and records 7,104 distinct
repository URL/ref snapshots across 21 distributions. ROS Index distribution
data contributes 8,553 nonempty package names and one invalid empty-name
entry, preserved separately; release and index metadata together
expect 8,957 names. The separately captured sitemap contains 9,113 package
pages, including dependency-only entries and empty repository summaries.
These counts describe different sets. None is a count of programs already
ported to this VM.

Source data is held under `../uwvm2-ros-sdk/.tmp/ros-ecosystem`. The catalog,
download receipts and generated reconciliation preserve missing sources,
unavailable archives, unsupported historical VCS hosts, missing expected
packages and submodule declarations. Additional source and release-upstream
recovery does not overwrite the primary evidence. Transient TLS/network
failures are retried separately, not classified as absent source.

Each archive receipt records its SHA256. Source refs can be mutable; the
current receipts do not assert a resolved commit for every archive. Exact
reviewed bytes are identified by both archive and file hashes in
[review-cases.json](../../tools/ros_ecosystem/review-cases.json).
That file records sixteen limited source-path reviews, their ranges, design
requirements and outstanding upstream acceptance checks. It is deliberately
not a whole-package review claim.

The generated `reconciliation/summary.json` is the live coverage record.
`package-effect-inventory.json` attributes automated external-effect findings
to the nearest enclosing package manifest. Findings outside package roots stay
unowned. Regex matches are review hints: tests, comments, unreachable paths and
wrappers can match, while indirect or unrecognized APIs can be missed. A file
with no matches is not qualified. Submodule declarations are recorded, but
submodules are not automatically fetched or qualified.

## Network and driver boundaries

The Velodyne input path binds a UDP socket, polls and receives packets, checks
packet size and optionally checks source IP. This supports a receive-only
sensor capability: a deployment-approved source policy, packet schema, queue
bound, timestamp policy and loss counters. The guest needs neither a socket
descriptor nor permission to send. See case `sensor-datagram-ingress` and
[upstream Velodyne input](https://github.com/ros-drivers/velodyne/blob/ros2/velodyne_driver/src/lib/input.cpp).

SocketCAN shows why direction alone is insufficient. A permitted sender can
still choose the CAN identifier and data that command an actuator. An approved
driver binding must additionally constrain bus, identifier/mask, frame type,
payload size and command values. Native interface lookup, ioctl and socket
configuration belong to the trusted provider. Command, telemetry and stop
authorization are distinct. See `can-command-envelope` and
[upstream SocketCAN sender](https://github.com/autowarefoundation/ros2_socketcan/blob/main/ros2_socketcan/src/socket_can_sender.cpp).

MAVROS selects serial, UDP, broadcast, TCP client or TCP listener from a URL.
The portable guest instead selects an approved vehicle-link profile; the
profile binds the native transport and protocol identity. Message identifiers
and command semantics remain separately checked. This review covers URL
selection and parsing, not every native transport implementation. See
`vehicle-link-profile` and
[upstream MAVROS interface](https://github.com/mavlink/mavros/blob/ros2/libmavconn/src/interface.cpp).

Purpose-specific communication does not imply a single static port for all
ROS traffic. TCPROS uses connect/listen/accept and negotiated host/port values.
The trusted ROS provider can perform that negotiation within its deployment
domain and peer policy. The guest still sees authorized topic/service/action
operations, with no unrestricted socket fallback. See
`ros-transport-negotiation` and
[upstream TCPROS transport](https://github.com/ros/ros_comm/blob/groovy-devel/clients/roscpp/src/libros/transport/transport_tcp.cpp).

## Storage workflows

rosbag2's maximum bag-file size triggers rotation. It does not bound the sum of
all output files. A recording capability therefore needs total accounting for
every rotated segment, metadata, index, transaction journal/WAL, staging file,
cache and temporary compression output. An append reserves the trusted
backend's worst-case envelope before acceptance. Success cannot be reported
for data silently dropped after quota exhaustion. Approved backend profiles
replace arbitrary guest storage settings such as SQL PRAGMA strings. See
`recording-total-space` and
[upstream sequential writer](https://github.com/ros2/rosbag2/blob/humble/rosbag2_cpp/src/rosbag2_cpp/writers/sequential_writer.cpp).

Nav2 reads an image named by map YAML and saves image data and YAML metadata
through separate writes. An immutable map bundle preserves this relationship
without granting recursive access to the YAML file's directory. Saving creates
one new generation only after every member succeeds; a failed metadata write
cannot expose a new image with old YAML. Decoded dimensions and staging output
also require bounds. See `map-generation-bundle` and
[upstream map I/O](https://github.com/ros-planning/navigation2/blob/humble/nav2_map_server/src/map_io.cpp).

These are semantic storage workflows, not renamed unrestricted filesystem
operations. File traversal, symlinks, repeated rotation and concurrent writers
must all remain inside the same admitted resource accounting.

## Model, media and bridge boundaries

The reviewed `robosoft_openai` server obtains a credential from the environment,
serializes prompts/images, accumulates a curl response, logs content and turns
text into a boolean. The safe portable binding keeps endpoint and credentials
host-side, separately authorizes data egress, and bounds encoded images,
request/response bytes, tokens, concurrency and time. Prompt/response inspection
requires private-data permission. A model result is data or a proposal; parsing
it never grants actuator, process, network or storage authority. See
`model-provider-data-egress` for exact source identity and reviewed bytes.

`web_video_server` provisions an HTTP listener, selects streamers and topics
from requests and provides stream shutdown. Camera reads, listener provisioning,
per-viewer egress and session shutdown must be distinct grants. Admission binds
topic, codec, resolution, concurrent viewers and bounded backpressure. See
`camera-media-egress` and
[upstream video server](https://github.com/RobotWebTools/web_video_server/blob/ros2/src/web_video_server.cpp).

The reviewed rosbridge paths accept client-selected names, types, QoS,
throttling/queue options and service arguments, with optional name filters.
A portable bridge must authorize both resolved endpoints independently,
including direction and type. Connection permission does not grant access to
the ROS graph. Correlation, cancellation, deadline, replay and loop prevention
need explicit semantics. Remapping is resolved before either endpoint check.
See `bridge-bilateral-authorization` and
[upstream rosbridge subscription](https://github.com/RobotWebTools/rosbridge_suite/blob/ros2/rosbridge_library/src/rosbridge_library/capabilities/subscribe.py).

`image_transport` selects plugins through class identifiers and configurable
publisher lists. The portable replacement selects approved immutable codec
IDs with declared dependencies and capability closure. It does not pass a
native library path to the guest. Each codec still needs its own bounds and
qualification; examining the loader is not examining all codecs. See
`approved-image-codecs` and
[upstream image transport publisher](https://github.com/ros-perception/image_common/blob/humble/image_transport/src/publisher.cpp).

## Execution guarantees and deployment

The reviewed ros2_control loop performs read/update/write with a measured
period, scheduling configuration and either simulation or wall time. This
motivates a separate execution-guarantee state in addition to authorization:

`zero authority -> explicit elevation -> stable granted authority -> explicit attenuation -> potentially uncertain guarantees`.

An admitted continuous cycle reserves its required capacity. Optional rate
attenuation cannot silently skip a phase in the middle of that contract.
Deployment chooses an explicit drain, cancel or safe-stop transition and a
watchdog behavior. Scheduling privilege remains with the trusted host, and
simulation-time jumps are distinguished from wall-clock deadlines. Stable
admission is not a guarantee against hardware or provider failure. See
`stable-control-cycle` and
[upstream control node](https://github.com/ros-controls/ros2_control/blob/humble/controller_manager/src/ros2_control_node.cpp).

Nav2 launch constructs a component/executable graph with package resources,
rewritten configuration, remapping, environment inputs and respawn behavior.
The portable semantic equivalent is an approved immutable deployment manifest
with explicit lifecycle operations. It confers no ambient shell or process
authority. Respawn cannot resurrect a revoked grant generation; rewritten
configuration uses a bounded private storage transaction. See
`deployment-graph` and
[upstream Nav2 launch](https://github.com/ros-planning/navigation2/blob/humble/nav2_bringup/launch/navigation_launch.py).

## Original native plugins and effectful initialization

The captured Universal Robots Humble driver declares a
`hardware_interface::SystemInterface` plugin. Its `on_configure` reads the
robot address, reverse/script/trajectory ports, script and RTDE recipe resources,
constructs the connected driver and starts an asynchronous thread. Its separate
`on_activate` returns success without establishing those connections. A host
therefore cannot treat configure as inert simply because ROS lifecycle activation
has not occurred. Headless mode may transmit a robot program during setup.
An unchanged plugin needs its matching external ROS/pluginlib host, complete
immutable dependencies and individually admitted transport roles. Platform
extensions carry permission metadata only. See `vendor-ur-effectful-configuration`
in the source review ledger; its hashes identify the exact captured files.

The captured Franka Humble plugin constructs a native robot connection and
model. Control initialization catches `ControlException`, performs automatic
error recovery and retries control. Its constructor also selects a less strict
real-time configuration when the kernel check fails. These are meaningful
security and scheduling transitions: start authority cannot silently include
fault recovery/rearm, and a stable execution profile cannot silently downgrade.
The supervisor must refuse the profile or rely on a separately enforced command
gate that the unchanged native binary cannot bypass. Merely wrapping the plugin
entry point cannot prevent its internal network actions. See
`vendor-franka-recovery-is-effectful` for reviewed ranges and required tests.

Neither review executes the vendor binary or qualifies its simulator/hardware.
The current closed component ABI cannot directly load these original pluginlib
classes. A typed role fixture is useful isolation evidence, but it is not a
vendor ABI adapter or proof that recovery and motion semantics are enforced.

## Qualification still required

The recorded acceptance checks are requirements, not upstream test results.
Each candidate needs dependency closure, public API adaptation, compilation,
execution and observable semantic comparison before a portability claim.
Physical drivers additionally require target hardware acceptance. A reviewed
provider must prove denial before effects, revocation, ownership, resource
accounting, cancellation and failure reporting across every supported backend.

The source acquisition tools have separate offline security tests covering
archive escape and expansion, source URL validation, retry classification,
manifest bounds/dependencies, atomic evidence writing and honest coverage
accounting. These tests run only in the existing `uwvm-sdk-resource-gate-20261024`
container with `memory.max=68719476736` and `memory.swap.max=0`. Passing them
does not certify the acquired ROS packages or the native VM providers.

## Launch and plugin recovery boundaries

The Jazzy `launch` source accepts an executable command, working directory,
environment and optional shell. Its respawn loop can have an unlimited retry
count. Neither a successful subprocess launch nor its signal escalation proves
that robot effects are settled. Case `launch-process-recovery-boundary` therefore
requires finite deployment-selected restarts, fresh incarnations, separate
activation authority and independent cleanup/rearm evidence. This is a review
of the specified source paths, not a qualification of ROS launch workloads.

The Jazzy `pluginlib` loader creates C++ instances after loading the requested
class library. Its search candidates combine a package prefix, several library
folders and platform filename alternatives. Case `pluginlib-immutable-external-host`
requires a fixed package/ABI/dependency closure in an external host. The existing
ROSv2 message-role bootstrap cannot accept an arbitrary pluginlib binary merely
because it is a shared library. Vendor qualification must cover its original
ABI, constructors, dependencies and teardown; the VM must never load it.
