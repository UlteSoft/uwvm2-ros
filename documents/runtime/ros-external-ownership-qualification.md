# External ownership qualification, 2026-11-18

This record concerns the attachment-only Linux x86-64 VM built from
`driver-control-guard-source` into `vm-driver-control-guard/uwvm2-ros`, under
`../uwvm2-ros-sdk/.tmp/ros-scene-security/`. Its SHA-256 is
`b52184eed477829b06c8f790dda1e62612fa02bd6083b03fa742a19715808bfa`.
The frozen manifest contains 4,643 files. The compiler uses `-O3 -g`; native
security fixtures retain assertions. Evidence for another binary is historical,
even when the executable has the same filename.

The [ownership contract](ros-external-ownership.md) defines the boundary:
deployment provisions processes, cgroups, namespaces, images and recovery;
the VM authenticates pre-existing endpoints and manages its own authorized
sessions. The deployment simulators live under `test/0084.ros-runtime` and are
not an industrial deployment product. External ROS providers, models, drivers
and MuJoCo never become VM plugins.

## Floating-point control optimization

Hardware samples of the preceding `a686867e...` binary identified repeated
`fegetenv`/`fesetenv` work at the five driver import wrappers. Those wrappers now
use the existing local register guard in LLVM builds on supported architectures.
The guard precedes caller identity and memory admission and remains active until
local cleanup returns. Other compilation profiles retain the complete guard.
Peer authentication, capability checks, schema bounds, deadline checks and
secure buffer overwrite are unchanged.

This classification depends on the driver path containing only reviewed VM code
and channel syscalls. Adding a vendor callback, extensible native decoder or
in-process provider invalidates it. Native code must remain external.

`driver-control-guard-wrapper/results.json` records 240 calls through the actual
five wrappers under 48 hostile floating-point control profiles. The identity
hook changes rounding, flush and exception controls and denies the call. Every
wrapper returns the denial and restores the caller's control state. This is a
native boundary regression; actual Wasm success/failure checks are recorded
separately.

## Evidence interpretation

All workloads inherit the existing aggregate cgroup:

```text
/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope
memory.max = 68719476736
memory.swap.max = 0
```

`driver-control-guard-checked/results.json` records the serialized driver,
allocation, module-message, all-import refusal, debugger and MuJoCo suite.
Its completion field covers that suite alone. In particular, the 342 import
refusal cases are not 342 fully implemented providers or ROS package ports.
The source/manifest hashes, actual commands, subprocess results and failed
attempts are retained; a failing or unfinished receipt must not be promoted
to a successful run because a neighboring test passed.

The native performance reference uses the same frozen dispatcher, schemas,
authority and external process protocol. A driver round trip carries a 17-byte
request and a 24-byte response. Receiver timing excludes process startup and
LLVM compilation. It is not an eight-byte direct function call. Hardware
samples must be filtered to the recorded client PID; the external receiver is
not an alternative native client baseline. Sample-after counts, throttling,
lost samples and requested wall-clock intervals are separate observations.

## Coverage limits that remain explicit

The 2026-11-18 ecosystem inventory contains 9,432 observed package names and
30,712 source instances. The platform join contains 32 platform identifiers
and 116 acquired source repositories. These are inventory counts. Missing
sources, unresolved manifests and unreviewed semantics remain explicit in
`deployment-obligations-20261118/results.json`; there is no all-package
compatibility claim. Platform foundation profiles constrain the common driver
permission path and do not implement or qualify vendor drivers.

Current local driver/model/ROS-provider attachments are authenticated local
IPC. They must not be described as DDS transport. External ROS network
interoperability is a different boundary. ROS 1 requires a qualified bridge;
ROS 2 interoperability also depends on the middleware, schema, QoS and security
configuration, not solely on whether a peer uses rcl, rclcpp or rclpy.

MuJoCo tests establish simulated behavior and failure containment. They do not
prove a physical emergency stop, device-side fencing or hard real-time response.
The cooperative test device-ownership registry does not constrain a malicious
privileged driver. Independent administrators must share an enforced resource
authority or device-side fencing; separate cgroups/namespaces do not establish
global hardware exclusivity.

Separate per-VM and per-driver memory cgroups require an externally delegated
empty subtree with memory and pids controllers below the same aggregate gate.
Without that delegation, these runs qualify aggregate confinement only. No
ancestor limits were raised and no test was moved outside the gate. Complete
vendor/OS/language qualification and industrial safety certification are not
established by this record.
