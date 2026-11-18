# ROSv2 deployment and ecosystem coverage

ROSv2 is intended to host a complete robot workflow without granting the Wasm
application WASIp1 imports. This is an architectural target, not a statement
that every acquired ROS package already runs. The operation catalogue,
implemented providers, native interoperability and deployed robot acceptance
must be evaluated independently.

## Three independent host-policy families

WASIp1, deprecated ROSv1 and ROSv2 are peer permission families. Their grants do
not flow between families. WASIp1 remains compiled by default and starts hidden
and without resource authority. ROSv1 defaults to compiled out; it retains
legacy ROS communication APIs and is not the robot-wide authority boundary.
ROSv2 defaults to compiled in with no exposed imports or grants. A ROSv2 platform
extension is a child restriction, not another peer permission owner.

Pure computation needs no host interface. A portable algorithm can consume
bounded values supplied by its caller. A ROSv2 application obtains approved
configuration/maps/model weights as immutable artifacts, writes through bounded
transactional storage, consumes typed sensor/state data, and submits proposals
or commands under separate arbiter/controller/device grants. Models do not
receive actuator authority merely because they can generate tool requests.

A POSIX-oriented library that cannot yet use those semantics may require a
WASIp1 build and a separately authorized module. Limit its imported operations,
resource identities and output capacity; audit the actual compiled imports.
A directory preopen does not express whether bytes represent a map,
calibration, log or model. Socket permission alone does not express a legitimate
motion command. Mount roots and syscall restrictions remain useful confinement,
but do not establish semantic authorization. Do not bypass a missing ROSv2
provider by silently granting the same application broad filesystem/network
access. CPU-only libraries should not gain WASIp1 access merely because their
toolchain targets `wasm32-wasip1`.

## Membership, sharing and attenuation

The current ROSv2 implementation binds exact module names to named environments.
One environment with one member provides single-module policy; several explicit
members form a group sharing its resource accounts and budgets. Unbound modules
remain hidden. A module cannot join two environments, and a failed binding must
publish no partial membership. Interface exposure and resource grants are
independent. Runtime instance/generation and caller ancestry are checked on use.

A deployment-wide policy must enumerate its approved module set explicitly.
The current implementation has no implicit Global fallback and no dedicated
`--ros-scene-global-*` interface. Treat a generated all-module environment as
one explicit group, not an exception that grants later-loaded modules access.
Any future Global/Group/Single convenience syntax must expand into one frozen,
nonoverlapping membership map before providers start. It must reject ambiguous
overrides and preserve shared accounting across aliases. It must not copy
legacy permissive fallback semantics or bypass per-module caller checks.

Stable authority enables exactly the granted operations on their named
resources, subject to fixed capacity/deadline/physical constraints. Optional
call/byte attenuation begins only after that authority exists. Explicit zero
traffic means no new ordinary work, not absence of authorization. Attenuation
requires uncertain-delivery acknowledgement and cannot replenish usage,
resurrect a revoked handle or consume the independent stop reservation. Debugger
inspection observes cached state without admitting a call or starting a worker.

## Multiple ROS environments and existing peers

A ROS deployment identity must distinguish robot/fleet, ROS generation and
distribution, RMW implementation/version, communication domain, enclave and
native process incarnation. Domain IDs and namespaces route names; they are
not peer authentication. Different RMWs and security configurations need
separate external workers and immutable dependency/configuration closures.
Avoid process-global RMW environment mutation after a participant is created.

Interoperability with an existing ROS peer requires matching message identity
and layout, CDR/XTypes policy, topic/service/action name mapping, effective QoS,
discovery, liveliness and security configuration. A custom VM IPC protocol stays
inside the local broker boundary; the peer-facing worker speaks the selected
native ROS transport. ROS1 bridges need separately granted endpoints on both
sides, explicit direction/conversion and failure/replay semantics. A bridge must
not expose every discovered topic automatically or connect a private robot to a
public domain by inheritance.

[SROS2](https://github.com/ros2/sros2) supplies ROS tooling for DDS Security.
Authenticated deployments must force security and enforcement, keep keys inside
the selected worker boundary, authenticate governance/permissions and reject
missing, expired, wrong-enclave or widened credentials. Never silently fall back
to plaintext. ROS-level permissions must account for implicit DDS endpoints
used by services, actions, parameters, discovery and lifecycle operations.
The [SROS2 tutorials](https://github.com/ros2/sros2/blob/rolling/SROS2_Linux.md)
warn that secure communication between DDS vendors is not supported; a standard
or shared topic name does not replace a tested interoperability matrix.

[ESROS2](https://www.csa.iisc.ac.in/~vg/papers/sigops_atc2026/) adds
syscall-context checks, eBPF and Intel MPK around native SROS2 applications.
The authors publish their [wrapper and eBPF sources](https://github.com/Esros2/ESROS2/tree/3d43c3433e43e7d7d6f7267fb4eaa2f1caf26678).
A deployment using this research implementation needs evidence for its actual
CPU, kernel, native application, DDS runtime and enforced policy. The indexed
ROS corpus contains SROS2. Execution of ESROS2 remains unverified.

The [distribution and middleware results](ros-distribution-rmw-inventory.md)
record original TCPROS clients across thirteen releases and the specific
ROS 2 middleware pairs that exchanged messages. Native Jazzy Fast DDS with
SROS2 Enforce has a separate authorized and denied topic test. The retained
Wasm traffic tests use separately provisioned peers on selected topics and
services. Custom service/action mappings, cross-vendor encrypted operation
and hardware deployment each require independent tests.

## Package evidence and semantic gaps

The retained inventory observes 9,432 names in 30,712 package instances and
45,063 interface files. It also records 567 names not yet observed in source,
67 unresolved manifests and 607 unowned interface definitions. These figures
are tied to a captured ROS Index/rosdistro corpus, not to 9,432 officially
authored libraries. Source acquisition and complete semantic review are both
incomplete. Regex effect hits are review hints and never permission grants.

Concrete source-path reviews in `tools/ros_ecosystem/review-cases.json` motivate
the following boundaries:

- Velodyne UDP ingress needs receive-only approved peers, packet schemas,
  timestamp/freshness checks and bounded queues. A socket descriptor is not a
  sensor capability.
- `ros2_socketcan` needs bus/identifier/frame/command constraints. A permitted
  transmit direction still allows unsafe commands unless semantics are checked.
- `rosbag2` file rotation does not bound total disk usage. Recording authority
  must account for all files, metadata, compression, journals and staging.
- Nav2 map output spans image and YAML members. Publication must preserve one
  complete generation and refuse traversal/dependency escape.
- Pluginlib and native hardware plugins require an external process host.
  Package discovery must not become permission to load code inside the VM.

The whole-corpus obligation report preserves every observed package instance,
source hash and unresolved effect, but does not assign a whole-package pass
from these examples. Empty findings do not prove a package is pure computation.
Conditional dependencies, plugins, generated code, dynamic names, hardware
protocols and runtime effects require their own review and execution evidence.

## Acceptance boundary

A qualified robot deployment needs a concrete model-to-control-to-driver loop
using only its declared ROSv2 guest imports, alongside separate tests for every
explicit WASIp1 compatibility module. Exercise multiple robots/domains,
cross-group denial, unknown/stale handles, revoked ancestors, bounded storage,
malformed traffic, model-data disclosure, control handover, driver/VM crashes,
watchdog expiry, update rollback and resource exhaustion. Actual secured peers
must prove refusal as well as successful communication.

Run compiler, semantic, fault and performance tests in the existing 64 GiB
zero-swap cgroup. Use frozen inputs, `-O3 -g`, explicit CPU/NUMA placement and
VTune source/assembly attribution for performance claims. Process isolation,
sanitizer success and a finite simulator run do not establish a safety integrity
level or manufacturer certification. Unqualified deployment profiles stay
unavailable instead of acquiring ambient compatibility authority.

## Distribution and middleware boundaries

The release profiles cover thirteen ROS 1 distributions, thirteen named ROS 2 distributions including Rolling, and eleven historical ROS 2 prereleases. ROS 1 peers use their native graph and transport behind an external bridge. Each direction needs its own approved source, destination and type mapping. A ROS 2 distribution can have several middleware implementations. Record the native library version, peer API, tested message type, QoS, security profile and host ABI with each result. [Distribution profiles](../../tools/ros_distro/README.md) · [RMW inventory and evidence](ros-distribution-rmw-inventory.md)

For native ROS 2 packaging, select the installation with `--ros-root` and `--ros-distro`. The packer checks both upstream declarations for the selected prefix, stores their digests, and places the selected ELF libraries and resources under that prefix in the sealed image. For Ardent through Dashing it can read a literal export from the historical shell hook as data. The external test gateway validates a single common prefix across its sealed ROS image destinations. DDS plugins stay in operator-provisioned external processes.

The native loopback fixture exchanged reliable `std_msgs/msg/Int32` in both directions between Jazzy `rclcpp` with Fast DDS and Humble `rclcpp` with Cyclone DDS. A separate run used the Humble Cyclone C API as the independent peer. Both tests carried plaintext in an isolated network namespace. For the two-`rclcpp` run, the Humble process also logged deserialization errors. The installed `rmw_dds_common/msg/Gid` definition uses 24 octets in Humble and 16 in Jazzy. The test did not identify which incoming sample produced each diagnostic. Qualify graph identity and discovery separately before using them for routing or control. A deployment that needs both graph layouts can place each graph in its own domain and relay the approved application topics through the original deployment. [Reproduce the native wire test](../../test/0084.ros-runtime/dds_interop/README.md) · [Upstream cross-distribution communication policy](https://github.com/ros2/ros2_documentation/blob/rolling/source/Releases.rst#cross-distribution-communications)
