# Gateway-local ROS2 parameter events

The secure ROS2 Gateway keeps its native `/parameter_events` publisher disabled.
An authorized local `param.declare`, `param.set` or `param.undeclare` therefore
does not grant native event publication. This adapter instead delivers a
selected canonical event to explicitly activated `param.subscribe-events`
queues inside the same Gateway process.

The listener must identify the exact Gateway Node's fully qualified name and
the exact parameter name. Each listener independently selects a builtin value
kind or the existing legacy String projection. Grant presence alone does not
activate a route, and mutation authority does not imply event authority. The
native subscription is created and its entire actual QoS is verified before
the weak local route is registered. Its queue must contain the exact grant
used for that activation. The native DDS callback remains gated until verified
activation. If native QoS or registry capacity refuses activation, the new native
owner is reset and the executor is notified to release its blocking wait-set
reference; a wakeup failure does not change error 95 or 90.
An exception during post-creation QoS verification or registry allocation also
clears the native owner and wakes the executor before preserving that exception.
Remote-node listeners continue to use the native
DDS path, and Nodes with native automatic parameter publication enabled do
not acquire the local route.

Local delivery uses the same projection and `event_queue` as native events:
complete envelope bytes, parameter elements, numeric bounds, finite values,
UTF-8, event-field count, token bucket, bandwidth, depth, byte occupancy and
overflow policy all remain applicable. Strings and arrays are borrowed until
projection quotas pass. A rejected listener does not receive another listener's
projected value. No native publisher, service or callback into Wasm is added.
Bool, Int64, Float64 and NOT_SET projections also read only their active field;
they do not convert an entire native value with unrelated inactive allocations.
The preexisting native `get_parameter` ownership copy and middleware-side
allocations remain separate boundaries; this fixture does not claim to bound
all native allocations before the public SDK returns a parameter.

The process registry holds weak Node and queue references only. It removes
expired or deactivated routes on activation and valid local dispatch, deduplicates
the same queue identity, and accepts at most 65,536 routes. An additional local
activation returns quota error 90. This implementation ceiling grants no
additional resources; normal explicit policy and session limits still apply.
Closing, revoking or ending a session deactivates its queue, clears pending
events and prevents a racing late enqueue. Another session's same alias retains
its independent grant and queue.

These local notifications cover mutations committed through this resources
adapter. External native parameter services or direct Node changes with native
publication disabled are not automatically observed. Registry scans are bounded
by the route ceiling; their overhead has not yet been benchmarked.

Events describe committed native state. A short-lived native post-set observer
captures an owned declaration/set snapshot for only this caller thread and exact
name, including a parameter override or a native pre-set transformation. It copies
only the active field after the hard wire/element ceilings pass, never reenters
the Node or event registry, and suppresses copy failure. Delivery occurs after
the native operation returns. Callback entries are explicitly removed even when
the operation fails. Undeclaration captures and validates the actual prior value
before committing: a matching concrete projection emits kind 3 with the prior
tag and no value payload, after checking prior bounds, elements and UTF-8. A
deletion-only listener emits kind 3/tag 0 without inferring prior metadata.
The prior-value attestation applies to serialized Gateway adapter writers. The
public native get/undeclare calls do not make that check atomic against external
direct Node writers; such writers may race the prior snapshot and are outside
this adapter-only notification contract. The production Gateway disables native
parameter services and serializes its authorized adapter operations.
The timestamp uses the Node clock and must fit the existing wire's nonnegative
signed 32-bit seconds and nanoseconds below one billion.

The snapshot uses Jazzy's public
[post-set callback contract](https://github.com/ros2/rclcpp/blob/jazzy/rclcpp/include/rclcpp/node.hpp)
and its
[native parameter implementation](https://github.com/ros2/rclcpp/blob/jazzy/rclcpp/src/rclcpp/node_interfaces/node_parameters.cpp).
Those upstream references explain the design. Qualification of the actual frozen
SDK headers, libraries and behavior remains part of the pending native build.

Delivery is best effort. Queue/rate/byte rejection or an invalid timestamp can
drop an event. An allocation or listener failure cannot change a completed
native mutation. If the mutation commits and the RPC deadline then expires,
the caller may receive error 110 while an authorized listener still receives
the committed event. This preserves the distinction between a native side
effect and a timely RPC response.

Build and execute these fixtures only on Linux inside the established 64-GiB
cgroup, using a fresh frozen complete source snapshot, the pinned ROS2 Jazzy
SDK and a freshly qualified Gateway. All comments and fixture outcomes below
describe prepared source until an actual execution report establishes them.

```sh
cmake -S SOURCE/tests/ros_local_parameter_events -B FRESH_BUILD
cmake --build FRESH_BUILD --parallel 1
ROS_DOMAIN_ID=203 FRESH_BUILD/local_parameter_events_contract_ros2
ROS_DOMAIN_ID=203 FRESH_BUILD/local_parameter_events_native_ros2 QUALIFIED_GATEWAY
```

The native IPC fixture covers all nine scalar/array kinds through declaration,
change and typed deletion; tag 0 and legacy String; two independent sessions;
absent and unactivated aliases; node/name/type/bounds/elements/envelope filters;
close, revoke and connection teardown; and all three existing overflow policies.
Expected complete envelopes come from the independent SDK encoder. It observes
actual native publisher GIDs and Gateway-origin DDS event traffic for a bounded
one-second final window. This is not a claim about unlimited external traffic,
SROS2 Enforce, concurrent malicious Wasm, or a completed ARM/full acceptance run.

The complementary trusted-adapter fixture checks actual declaration overrides,
a deliberately delayed committed set that returns timeout 110, exact activation
grant identity, actual pre-set transformed value, caller-thread isolation,
active-field snapshot wire/element bounds, weak lifetime, registry ceiling/pruning, native-publication
exclusion, malformed UTF-8, StringArray NUL, nonfinite Float64, timestamp limits
and the shared token bucket. Its full-registry case populates controlled weak
references; it does not create 65,536 DDS readers or make a throughput claim.

`source_review.py` runs only Python source predicates, handcrafted wire oracles
and an independent authority/lifetime model. It does not compile C++, instantiate
Wasm or execute ROS. Keep its output distinct from the two native reports:

```sh
python3 -B SOURCE/tests/ros_local_parameter_events/source_review.py --output FRESH_REPORT.json
```
