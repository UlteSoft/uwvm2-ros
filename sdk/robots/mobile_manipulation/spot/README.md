# Boston Dynamics Spot: fixed SDK 5.0.1 and controlled ROS2 bridge

The selected official Source is annotated tag v5.0.1, commit
16a98aae3b818082288504c6e53b59a67666aaeb, tree
252cf3ba2a0616883175cb5f117465b5388e381f. All151 original proto files,
including service/choreography declarations, are preserved. Their six imported
Google well-known types come from the actual protobuf submodule selected by
grpc1.46.3. This is Source membership evidence, not generated wheels, a whole
installed SDK, a descriptor-set run or hardware compatibility. Official current
5.2.0 material does not change this pinned5.0.1 scope.

## Two independent permission layers

Native SDK networking is confined to this explicitly started trusted ROS2 bridge.
A Guest uses the existing ROS ABI with separate real service.call aliases,
actual complete compiled types/canonical IDs and bounded Runtime authority.
It gets no credential, robot URL, raw socket, native symbol, Host policy/update
or debug import. The Native ROS operator must separately isolate the ROS network.
NativeGate is an additional fixed operation allowlist; it is not Runtime
per-Instance permission or a substitute for Host resource/type/alias checking.

The default NativeGate map is empty. ros_bridge.main rejects13 before importing
bosdyn/rclpy, reading a credential, creating SDK/Robot/client/auth/channel/Node,
connecting, starting a worker or invoking an SDK RPC. The example allowlist stays
empty. Configuration has only a closed format, finite history and fixed numeric
per-operation RPC maxima. It cannot name a callable, Python path or SDK method.

The Native profile is SHARED. Payload token and nonce are independent nonzero
full-u64 anti-replay keys, not authenticated Runtime Source/session/Instance
identities. Each is retained without reuse, including unknown outcomes. The
shared bridge holds one real body lease and accepted-command history. Do not
claim per-Guest lease or feedback isolation: that requires a future authenticated
principal injection protocol, not a caller-supplied token. A controlled Native
ROS network is part of this deployment's operator trust boundary.

## Fixed services and complete typed objects

| Native operation | Fixed ROS resource | Actual authored ROS service |
|---|---|---|
| state | /uwvm/spot/v5/state | uwvm_spot_bridge/srv/State |
| body lease acquire | /uwvm/spot/v5/lease/acquire | uwvm_spot_bridge/srv/Lease |
| retain/renew real lease | /uwvm/spot/v5/lease/retain | uwvm_spot_bridge/srv/Lease |
| return real lease | /uwvm/spot/v5/lease/return | uwvm_spot_bridge/srv/Lease |
| stand | /uwvm/spot/v5/command/stand | uwvm_spot_bridge/srv/Command |
| sit | /uwvm/spot/v5/command/sit | uwvm_spot_bridge/srv/Command |
| finite velocity | /uwvm/spot/v5/command/velocity | uwvm_spot_bridge/srv/Velocity |
| Stop | /uwvm/spot/v5/command/stop | uwvm_spot_bridge/srv/Command |
| actual command feedback | /uwvm/spot/v5/command/feedback | uwvm_spot_bridge/srv/Feedback |

These are this project's original ROS bridge declarations, not Boston Dynamics
ROS interfaces. Their full sources live in ros2_bridge/srv. The state response
contains actual acquisition time, bounded complete joint names/position/velocity/
load values, motor-power enum and the complete original RobotState protobuf.
Feedback records the real native uint32 command ID, actual native field
presence/status and whole original feedback protobuf. Zero IDs/status/timestamps
are not fabricated absence proofs. The Native bridge, authored ROS declaration and
supplied complete vendor catalog share one finite16384-byte protobuf quota in the separate full-field successor.
Oversize90 does not truncate, retry or discard remaining physical uncertainty.

C11 sdk.h and C++ sdk.hpp consume the ACTUAL complete generated records from
catalog-spot-bridge.json. The caller distinguishes ROS transport status from
response.error and response.unknown. Full response decode/correlation validation
precedes public object assignment. The concrete C WASI entry and C/C++ sequence
use telemetry → acquire → retain → stand → finite velocity → actual feedback;
Stop and ReturnLease are separately selected, attempted independently and
reported separately from all raw-handle close results. There is no opaque
command byte facade or auto-inferred cleanup grant.

## Actual SDK lifecycle and finite operations

prepare_native checks real bosdyn-client/api5.0.1 packages. It reads an explicit
pre-obtained Native operator token from an owner-only NOFOLLOW regular file.
It deliberately avoids authenticate/update_user_token, which start the SDK's
token-refresh thread. Expiration fails; credentials are not refreshed implicitly.
One real directory RPC has an actual timeout and supplies the exact service list
before channel selection; missing required services fail95 without an unbounded
directory fallback. Every channel uses the actual grpc.enable_retries=0 option.

Command preparation uses one real shared SDK lease wallet and a real
TimeSyncEndpoint. Each time-sync RPC is bounded by one monotonic startup deadline.
It avoids Robot.time_sync's automatic background thread using an exact5.0.1
adapter of its real BaseClient/wallet setup, retaining the genuine endpoint.
This is a version-specific Native Source adapter, not a generic SDK monkey patch.
No LeaseKeepAlive or token/time-sync worker is created. gRPC channels still own
native infrastructure; actual close/join timing must be independently qualified.

Acquisition never uses TakeLease. Commands use the real SDK wallet processor,
which advances that wallet's current sequence. Retain/return read its actual
current Lease object, not a stale acquisition proto supplied by Guest. Unknown
acquire/return prevents implicit reacquisition. Finite velocity is bounded to
one second, 0.3m/s per x/y axis and0.5rad/s yaw in the selected profile.
Its end timestamp uses actual SDK wall-time/time-sync conversion; RPC deadlines
separately use monotonic time. After60 seconds the selected velocity sync profile
fails95 until an explicit operator restart; no hidden resync occurs.

The accepted native command ID is stored separately from token/nonce. Feedback
requires the token of an actually accepted command in this shared profile and
queries exactly that SDK command ID; it cannot manufacture an ID from a ticket.
Stop is another independently authorized real command, not native cancellation
of a prior uncertain command or inverse rollback. No command here powers on,
changes estop, clears faults or certifies the physical environment as safe.

Two Native cleanup records are reserved before business calls so ordinary
history exhaustion cannot block independently permitted Stop and ReturnLease.
Neither is silently granted. Each is attempted once and its error is retained;
an uncertain prior return is not replayed. The concrete bootstrap attempts every
truly owned SDK channel close and ROS service/Node/context retirement despite
earlier cleanup errors. Provider clients/channels are borrowed; Guest SDK roles
are likewise explicitly borrowed or independently owned by the sequence caller.
No local retirement claims physical Stop, successful ReturnLease or native join.

## Real bootstrap and qualification boundary

Build the separate rosidl interface project and Native Python installation
project using the recipe. The split avoids combining rosidl generation and
ament_python_install_package in one project. Both retain their actual inputs.
Then an operator may explicitly choose:

~~~sh
python3 -m uwvm_mobile_manipulation.spot.ros_bridge \
  --native-allowlist /trusted/operator/spot-allowlist.json \
  --robot-hostname EXPLICIT_NATIVE_HOST \
  --native-token-file /trusted/operator/spot-token.json
~~~

No such command was run here. The token file is exactly an owner-only JSON object
with user_token; never put it into a Guest, catalog, log or capability alias.
The allowlist enables each desired fixed operation using maximum_rpc_ns≤5e9;
copying the empty example enables nothing. Only enabled ROS services are created.
Original rclpy Node defaults remain unchanged. Whole SDK dependencies, complete
Native metadata/RIHS/canonical binding, real Gateway admission, concurrent
profiles, robot/controller versions, cleanup/timeout/lost-reply trials and an
independent finite outer watchdog remain necessary. No physical EStop,
per-Guest lease isolation, positive Guest/Gateway delivery or hardware
qualification is claimed by this native-bridge recipe.

## Full original protobuf field successor

The separate protobuf/ package contains genuine generated C nanopb and C++ Lite
Source for every production Spot5.0.1 message, preserving complete RobotState
fields beyond this bridge's convenience joint projection. See protobuf/README.md
and RECIPE.md for exact original descriptors, STRING/NUL and unknown-enum C
representations, hard bounded C ownership, actual C++ threaded-EH requirements
and real ROS Guest entry examples. Its16384-byte Native/declaration quota and
32768-byte canonical aggregate remain explicit; oversize90 is never truncation.
The [r36 successor qualification](../../../../documents/ros/continuation-20261018-evidence/spot-full-field-sdk-and-wasm-r36/README.md)
now records genuine full native C/C++ compilation/contracts, complete C decoder
contracts in Wasm and four x86/Arm QEMU lanes, plus three default-denied C/C++
business applications. It fixes the generated C keyword-name failure. The
[canonical six-language bridge SDK](../../../custom-types/spot-bridge-complete/README.md)
installs the existing complete selected records and RPC owners without changing
the original producer receipt. Full operation/language examples, the genuine
Lite C++ threaded-EH Wasm lane, positive Native/robot traffic, official upstream
suites and performance remain open. The earlier2048-byte milestone HOLD and
all its original counters remain historical.
