# Current Source policy scope

The original workflow below is retained verbatim as historical finite-milestone
text. Its2048-byte/no-Guest-decoder statements are superseded by the separate
[full-field C/C++ Source SDK](spot/protobuf/README.md): matched native_proto
capacity16384 and whole canonical response32768, with complete original fields
and strict refusal rather than truncation. Actual original protobuf strings can
contain NUL; the separate canonical projection retains its NUL-forbidden domain.
Every default-deny, paired type/codec proof, Native operator allowlist and
per-resource Runtime permission requirement below remains applicable.

The current Source product has genuine recorded generation attempts; that does
not alter the original milestone's zero-generation record and grants no
compiler/Native/Guest/robot qualification. Use
[full-field qualification](spot/protobuf/qualification/README.md) for the later
explicit producer/import/runtime/telemetry acceptance sequence. Payload tokens
still do not identify a Guest principal or isolate shared Native leases.

## Retained original policy text (verbatim)

# Explicit policy and operator workflow

Installing this package, selecting ROS2, importing a record codec or reading a
telemetry value grants zero permissions. The supplied Native allowlist example
also has an empty operation map. Do not replace a missing-import/product error
with an expected-denial assertion.

## Import and actual full record binding

The two catalogs are original declaration inputs, not generated products or
Native metadata. catalog-fjt-humble.json retains the entire selected FJT Goal,
Result, Feedback and dependency declarations. Its convenience quotas are eight
scalar joints, one trajectory point, eight entries per tolerance array and
bounded strict text; full MultiDOF fields remain present. catalog-spot-bridge.json
retains all five authored ROS services. Complete native protobuf is a bounded
byte member, with one 2048-byte Native bridge/declaration/canonical quota. Oversize is90 without truncation; complete protobuf is not
silently replaced with the typed projection.

After Root qualifies the independent complete vendor-profile adapter, freeze
the generated C11/C++ records, proof, canonical IDs, original source and actual
SDK metadata as one product. The FJT C++ alias is, for example,
uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal; the C record is
roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal. Whole
Goal/Feedback/Result objects pass through actual generated codecs. There is no
hand-written surrogate ROS layout in the Guest examples.

## Per-resource Runtime policy

The actual CLI source declares these forms (the module name must be the actual
loaded module, shown here as main):

~~~sh
--ros2-single-create main
--ros2-single-enable main
--ros2-single-gateway-socket main /trusted/operator/ros2.sock
--ros2-single-grant main telemetry topic.subscribe /joint_states sensor_msgs/msg/JointState ACTUAL_RIHS
--ros2-single-grant main goal action.send-goal /joint_trajectory_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory ACTUAL_FJT_RIHS
--ros2-single-grant main feedback action.receive-feedback /joint_trajectory_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory ACTUAL_FJT_RIHS
--ros2-single-grant main result action.receive-result /joint_trajectory_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory ACTUAL_FJT_RIHS
--ros2-single-grant main cancel action.cancel-own-goal /joint_trajectory_controller/follow_joint_trajectory control_msgs/action/FollowJointTrajectory ACTUAL_FJT_RIHS
--ros2-single-set main goal cancel-scope not-applicable
--ros2-single-set main cancel cancel-scope own
--ros2-check-policy
--ros2-dump-effective-policy
~~~

These are syntax examples, not an executable profile: replace both descriptive
identities with independently exported actual values, add every required field
from the selected actual policy/Gateway schema and pair each alias with its
real compiled adapter-codec-identity. An absent actual Native descriptor must
fail admission; source type spelling and a fixture hash cannot supply it.
No cancel-any-goal, graph, policy management, file, clock, log, update or debug
right is implied. Stretch uses its actual
/stretch_controller/follow_joint_trajectory resource; namespaces are explicit
deployment selections rather than Guest address discovery.

Required Action keys include all goal/cancel/result-service/feedback/status QoS
dimensions, finite active goals/actions, timeout, goal/feedback/result byte
bounds, rate/burst/bandwidth, finite event queue depth/bytes, explicit overflow,
validator and bounded audit fields. Topic keys include its finite queue,
subscriber count, full QoS, rates and structural validation. Complete native
custom types replace the unrelated Fibonacci order key. The caller must use
the actual generated manifest and current operation_schema; this package
does not invent a valid profile while these Native exports remain absent.
For custom data select validator=none; complete compiled structural validation
still runs. A separate control limit/collision validator is not supplied by
that spelling. Verify default-denied opens before adding one reviewed role.

For Spot, use eight separate service.call grants for State/acquire/retain/
stand/velocity/feedback/stop/return, each with the exact authored service type,
compiled canonical ID, real resource, full service QoS and finite request/
response/timeout/rate/inflight/audit limits. The ROS network operator independently
enables only corresponding fixed operations in the Native allowlist. Stop and
ReturnLease are independent business calls, not cleanup authority inferred from
acquire. The Guest sequence still closes all of its raw handles even if either
call fails. Closing a Guest handle does not release the shared Native lease.

## Typed C and C++ calling

examples/fjt_guest.c is an actual WASI C entry. Its six arguments select an
explicit model plus telemetry/send/feedback/result/cancel aliases. FD0 contains
the selected finite joint positions as little-endian f64 values, followed by
actual caller credits: T to receive telemetry, S to send, then F, R or C.
A credit is not a clock/readiness signal. At most64 post-send credits are used;
90 on exhaustion preserves an unknown physical outcome. The caller owns any
real deadline/outer supervision. FD0 is ordinary explicitly admitted input,
not the Root signal channel and not a robot/network permission.

fjt_client.hpp and examples/fjt_lesson.hpp expose the same complete generated
record aliases through a typed C++ interface. The C++ caller supplies real
already-open roles and a noexcept credit reader. Borrowed owners outlive all
operations; caller closes each independently. spot/sdk.hpp and
spot/examples/ros_sequence.hpp expose the complete generated State/Lease/
Command/Velocity/Feedback family with actual fields. spot/examples/guest_sequence.c
is the concrete WASI sequence entry; it separates transport errors from the
provider's error/unknown fields and retains all Stop/Return/close results.

The two real Guest targets and generated C/C++ record contracts are in the
default-OFF CMake project. An actual WASI toolchain, complete generated products,
SDK headers, compiler/link dependencies and final import validation are explicit
inputs. No target is a prebuilt product or a run result.

## Original rcl migration

Keep the native driver, calibration/tool/model, original Node defaults and
genuine generated ROS types. First run the separately supervised original
native/fjt_peer.cc or family native_peer.py against the exact installation.
Then replace only the Guest transport with explicit Runtime aliases and the
complete generated canonical record types. A valid canonical result still must
be compared with the original SDK's real result and independent JointState.
Do not copy a native DDS GUID into a Runtime ticket or claim a local correlation
is a Native sequence number. Native cancel acceptance is not terminal result.

## Native debugging and side effects

Use the actual Root debugger/read-only CLI interface, outside Guest authority:

~~~text
info ros sessions backend=ros2
info ros queues backend=ros2
trace ros backend=ros2 after=0 count=16 error=13
~~~

Inspect the exact module/Instance/session, alias, operation, quota, full canonical
values and retained native failure metadata. These are Native management tools,
not Guest imports or robot-control aliases. Correlate with the original native
driver/SDK logs and actual typed feedback/result values. Spot's full protobuf,
accepted native command ID and Native-only operation audit distinguish real
SDK acceptance from physical completion. This source package includes no
signature/update-policy/debug bypass and has performed no physical effect.
