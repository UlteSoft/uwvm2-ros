# Isaac Sim complete standard JointState lane

This lane preserves the complete official sensor_msgs/JointState Source plus
Header/Time. All names, positions, velocities and efforts are canonical typed
values; maximum64 names/values and64 UTF8 bytes per name/frame are quotas, not
native capacities. The original ROS schema permits empty velocity/effort lists;
the record keeps that general API. This particular actual Sim subscriber requires
all three arrays to match named joints, so complete_joint_command.cpp checks that
stronger real sink contract before opening/publishing. It does not replace the
message schema with a position-only projection.

Actual fixed87c95274 Source files live in isaacsim.ros2.nodes, but the registered
OmniGraph identifiers remain `isaacsim.ros2.bridge.ROS2PublishJointState` and
`isaacsim.ros2.bridge.ROS2SubscribeJointState`. Do not derive node IDs from a
folder name. Native graph code follows the real ReadJointState sensor links,
including jointDofTypes/stageMetersPerUnit/sensorTime, and the real controller's
jointNames/positionCommand/velocityCommand/effortCommand inputs. Angular units
are the actual controller's radians conversion, not raw USD degrees guessed by
this package. The native subscriber's real controller semantics do not use every
Header field as actuation input; Guest telemetry and canonical transport still
retain the entire Header, and no full Native metadata equivalence is fabricated.

Prepare/bind the genuine standard interface records with the actual exporter.
Build each future C++ Guest with examples/CMakeLists.txt, actual bound bundle,
real WASI C++20 SDK and vendored fast_io. Defaults expect13. The positive command
reads one length-prefixed complete canonical message, fully validates it before
opening an owner, prints every typed command and publishes exactly once. Unknown
publish result is never retried. Telemetry prints Header and every sequence
entry, up to64 caller-driven R observations, then independently closes.

Run native_graph.py only in the selected real Isaac Sim Python environment,
under a bounded process/cgroup owner. Supply a physical SDK root containing the
actual imported isaacsim package, a frozen local USD articulation, explicit
absolute prim/topics/domain, actual native QoS JSON and finite frames1..64 /
lifetime1..10000ms. A future explicit invocation is:

```
SDK_PYTHON native_graph.py --enable-native-simulation \
 --sdk-root PHYSICAL_SDK --usd PHYSICAL_LOCAL_USD --articulation /ActualRobot \
 --domain ACTUAL_DOMAIN --state-topic /uwvm_sim/state --command-topic /uwvm_sim/command \
 --state-qos-json ACTUAL_QOS_JSON --command-qos-json ACTUAL_QOS_JSON \
 --frames 32 --lifetime-ms 5000
```

Add `--enable-sim-commands` only for a separately approved simulation command
session. Without it the graph creates no command subscriber/controller. Without
--enable-native-simulation a well-formed call returns13 before Isaac/Kit imports.
Keep ROS topics separate from all hardware bridge resources. The native graph
runs the actual simulation loop, closes the timeline and SimulationApp
independently, and retains real outcomes in main.last. Frame count is not command
count and not elapsed ns. A Kit update/model load may block; an external deadline
must bound the actual process. Timeline stop is simulation stop, not a robot
hardware safety stop. Default context0 uses the explicitly set native
ROS_DOMAIN_ID and confers no Guest Runtime authority.

The original docs' bridge tutorials are background, not an installed SDK or a
Native exchange receipt. Actual USD/Kit extensions/PhysX or supported sensor
backend/ROS serialization/domain/QoS/hardware and full binary dependency closure
remain qualification inputs. Nothing was run in this source package.
