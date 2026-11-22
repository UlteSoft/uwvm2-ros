# ROSv2 native driver boundary

> Current product boundary: [external driver attachment](ros-driver-attachment.md).
> VM-owned component launch/stop interfaces in historical sections below are
> removed. Their receipts qualify the historical implementation only. The VM
> does not implement the deployment process supervisor.

All native vendor code executes outside the VM process. This applies to
official releases, open-source builds, proprietary binaries, shared libraries,
ROS plugins and standalone executables. A signature authenticates an artifact;
it does not confine its instructions. Platform extensions add small permission,
constraint and audit declarations to common ROSv2 resources. They do not contain
vendor runtimes, protocol implementations, native callbacks or plugin loaders.

The `scene/native/` headers reject VM build macros at compilation. This catches
accidental inclusion of external ROS implementations in VM translation units.
The retained AgileX protocol ports are disabled with literal `#if 0`. Neither
measure replaces isolation of an actual deployed native process.

## Real vendor packaging

The acquired SDK platform snapshots demonstrate why a universal `dlopen` entry
is insufficient:

- Universal Robots commit `262330ed9b5467a17c8ceae09567991e22d13944`
  declares `URPositionHardwareInterface` as a `hardware_interface::SystemInterface`
  plugin in `ur_robot_driver/hardware_interface_plugin.xml`. Its matching ROS
  control host must load it in an external process. Upstream's
  [Jazzy migration guide](https://github.com/UniversalRobots/Universal_Robots_ROS2_Driver/blob/main/ur_robot_driver/doc/migration/jazzy.rst)
  replaces the driver-specific host with `controller_manager/ros2_control_node`.
- Franka commit `4acc05c01ebfcdf916ee6b3df966df0ad7b2f9ec` also declares a
  `SystemInterface` plugin. The hardware package links libfranka; its C++ plugin
  ABI, ROS distribution and dependency closure are part of the external host.
- Doosan commit `6c5f3ba622bfa9d6f9cffebf21fa44f57db55b48` builds
  `dsr_hardware2` as a shared plugin and links `dsr_common2::drfl`. The acquired
  tree includes architecture-specific `libDRFL.a` inputs. A shared plugin can
  therefore contain a statically linked proprietary dependency.
- SICK's [build definition](https://github.com/SICKAG/sick_scan_xd/blob/develop/CMakeLists.txt)
  provides a shared API and the `sick_generic_caller` executable. The official
  API example loads `.so` on Linux or `.dll` on Windows. Select the appropriate
  original executable or an external host implementing that exact API.
- Unitree's [SDK build definition](https://github.com/unitreerobotics/unitree_sdk2/blob/main/CMakeLists.txt)
  imports an architecture-specific static SDK library. An archive is not an
  executable or a loadable plugin: it requires an external program already
  linked against the SDK. The VM must not link it.

These are source/packaging observations, not deployment or hardware acceptance
results. The five supported Linux VM ABIs do not imply that every vendor ships
all five corresponding binaries. A Windows DLL is not a Linux ELF object.
Kernel drivers require separate OS provisioning and cannot be isolated by
putting a user-space loader in a subprocess.

The [original-library qualification](ros-native-driver-lifecycle.md#unchanged-original-vendor-library-qualification)
additionally loads an unchanged RealMan x86-64 `libapi_cpp.so` in the production
historical component sandbox. Its 44 actual VM/DAP cases qualify that historical metadata-only adapter,
fault containment and stop. They do not qualify the complete robot driver or
grant network/device authority to the vendor library.

## External host contract

The final driver process owner is the deployment layer. The VM attaches to an
already registered external instance, observes permitted state and makes
authorized typed calls; it cannot launch, terminate, restart or update the
driver. Releasing a connection does not terminate its process. An authorized
device safe-stop operation is distinct from process termination. The former
component launcher imports and CLI are removed. The implemented
[attachment-only boundary](ros-driver-attachment.md) does not load or launch
native vendor code. Existing internal model/DDS workers remain a separate
migration item and are not silently presented as externally provisioned drivers.

An executable deployment selects an exact executable, immutable arguments,
environment, configuration and complete dependency closure. It supervises the
whole process tree, including daemonized children. A library deployment selects
the original binary and a matching external host; pluginlib, C ABI callbacks,
threads, globals and native object lifetimes stay in that process. There is no
generic guest-selected symbol name, signature, arbitrary FFI or host pointer.
Updates replace an immutable process incarnation instead of unloading live
libraries from a running control loop.

The common process manager must authenticate images before constructors run,
establish confinement before granting device/network access, and withhold
control authority until startup and the independent stop path are ready. A
declaration must specify the following separately:

- Logical driver identity, binary/host/dependency digests, target ABI, firmware
  range, configuration generation and explicit device binding.
- Exact device nodes, network peers and directions, required ioctl classes,
  discovery routes and native plugin search roots. No ambient home directory,
  complete `/dev`, generic host network or inherited credentials.
- Process/thread counts, address/RSS accounting, descriptors, scratch bytes,
  total durable output, startup/shutdown deadlines, CPU placement and NUMA
  policy. A driver cannot enlarge or escape the deployment cgroup.
- Closed typed command/result schemas, units, frames, timestamps, freshness,
  controller ownership, command sequence, limits and possible effects.

ROS/plugin hosts that cannot meet the selected confinement profile are refused.
A fallback to unrestricted execution is not a compatibility mode.

The [lifecycle and recovery contract](ros-native-driver-lifecycle.md) specifies
the remaining general-host state transitions, immutable package boundary,
durability ordering, replacement fencing, CPU/NUMA admission and independent
stop qualification. It distinguishes the implemented closed worker from those
general-host acceptance gates.

## Faults, updates and control ownership

The authority progression is zero authority, explicit stable grant, then
optional limits. Finite storage, physical bounds, deadlines and stop
reservations are mandatory safety constraints even during stable authority.
Traffic limiting is a separate explicit choice with uncertain-delivery
semantics; it never applies to the reserved stop/revocation path.

A crash, hang, protocol failure or lost process identity fences the provider
generation and the related device/controller authority. Retain the first
terminal fault, outstanding requests, unknown physical effects and cleanup
debt. Process exit, a transport ACK and physical stopped state are distinct
observations. Repeated failure notification is idempotent. Never automatically
replay motion, clear an unknown outcome, renew a controller or restore a grant.
Component failure also survives kernel-confirmed process exit: `failed` and
`exit_observed` describe independent facts in public status and debug snapshots.
The [native simulation qualification](ros-native-simulation-qualification.md)
includes segmentation faults, hung receivers, unknown command outcomes and VM
death/pause. Those tests use closed fixture bindings, not arbitrary vendor packages.

A restart or update needs a separately authorized fresh incarnation, verified
images, bounded startup and trusted state reconciliation. Old handles, source
witnesses and command sequences remain invalid. Keep crash records bounded and
exclude credentials and raw private memory by default. A VM crash must leave an
independent expiring lease/watchdog and physical stop mechanism able to act;
killing the driver is not proof of safe actuator state.

## Wasm control and native hardware

Control algorithms and portable controller management should run in Wasm and
use ROSv2 sensor, state, arbitration and device operations. The SDK already
contains extracted Wasm static-library ports of `control_toolbox` PID and the
`joint_trajectory_controller::Trajectory` value component. Their READMEs and
`portability.json` explicitly exclude the complete `controller_manager`,
JointTrajectoryController plugin and action server. Existing simulation scripts
also launch original native `ros2_control_node` processes.

A future complete Wasm control port must implement lifecycle transitions,
controller switching/resource claims, update/read/write ordering, clocks,
missed-cycle behavior and action semantics. It must not turn native
`hardware_interface` objects into VM pointers. Hardware effects remain typed
messages to isolated original drivers. The current review does not modify SDK
production sources or claim that complete port exists. See
[Wasm module isolation](ros-wasm-module-isolation.md) for the SDK memory boundary.

## Timing and implementation status

Keep a qualified tight native hardware loop within its own process when the
vendor requires it. Transfer bounded samples and commands across ROSv2; do not
translate every C++ method call into synchronous IPC. A Wasm control loop needs
separate timing qualification across that boundary. Measure in-process native,
isolated native and Wasm plus isolated native with the same messages, rates,
queue limits, CPUs and NUMA placement. Record median/P99/max, jitter, queue age,
copies, drops, memory pressure and fault-to-fence/stop latency. Performance
sources build with `-O3 -g`; VTune Hotspots and Microarchitecture evidence must
join the actual frozen C++ and sampled assembly. No existing codec benchmark
establishes this end-to-end result.

Implemented pieces include policy-only platform ownership, common typed
device/control dispatch, separate command/stop lanes, retained unknown effects,
host-driven provider-failure revocation, and isolated model/component/ROS worker
profiles with explicit CPU/NUMA selection. The current model/communication
sandboxes have deliberately closed startup and syscall contracts. They are
**not a general launcher for arbitrary original vendor binaries**. A general
driver package/host launcher, durable recovery,
controlled restart/update and vendor simulation/hardware qualification remain
required work. Do not mark those requirements complete from a mock or a codec
receipt.

The closed workers now own an outer PID namespace from creation through an
atomically returned pidfd. Actual init reap proves its descendant tree has
terminated even when startup reporting or release fails. Immediate exec removes
the inherited VM address space. The [lifecycle qualification](ros-native-driver-lifecycle.md#kernel-owned-process-domain-from-birth)
records the real fault tests and separate limits; this process-domain proof
does not implement a resource subgroup or an arbitrary vendor host.
