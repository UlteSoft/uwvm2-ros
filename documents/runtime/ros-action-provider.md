# Owned native action protocol

`communication_actions.h` implements an independently configured action endpoint.
`native/communication_actions_ros2.h` implements the actual standard ROS 2 action
protocol: `send_goal`, `get_result`, `cancel_goal`, `feedback` and `status`. It
interoperates with official `rclcpp_action` clients and servers. Seven action
carriers are now registered through the owned Linux worker binding, including
explicit cancellation selection. See [graph/event and cancellation qualification](ros-graph-event-portability.md)
for the actual frozen VM evidence and remaining codec/platform limits. Legacy
WASIp1 and ROS v1 implementations are unchanged.

The first native binding implements all `tf2_msgs/action/LookupTransform` goal,
feedback and result fields. This is a qualification fixture and a usable trusted
binding, not evidence that arbitrary ROS IDL is already supported. Generated or
introspection codecs need independent bounded-schema admission before they are
installed as additional trusted factories.

## Authority, schemas and capacity

The deployment fixes the endpoint owner, profile digest, attached node, resolved
action name, role, goal/feedback/result schemas, each protocol channel's QoS,
deadline bound, result retention, transition mask, goal capacity, feedback slots
and cancellation capacity. All capacities and transition permissions start at
zero. The native node is a counted `node_lease`; ordinary close cannot remove a
node while an action owns that attachment. Parent context revocation fences it.

The broker must hold the action capability and each exact secondary transition
action through native effects, together with independently required origin node
and schema authority. A copied `owner` triple is an internal identity, not an
authority token. `action.respond` requires both its primary permission and the
tag's specific permission: accept, reject, execute, feedback, succeed, abort,
finish.cancelled or cancel.respond. UUIDs and native graph names grant nothing.

`max_cancels_per_goal` is explicit and defaults to zero. Critical capacity is
checked as `(4 + 2 * max_cancels_per_goal) * max_goals`, independently of feedback
slots. Every admitted cancellation gets its own retained correlation; rejection
does not erase that record. A profile can reserve repeated lawful cancellation
attempts. The tests reserve two and verify rejection followed by acceptance.
Feedback saturation increments a loss counter and cannot occupy critical slots.

Payload/recovery buffers are preallocated. Native middleware and generated ROS
message objects can allocate before a callback; the dedicated worker must also
enforce its native memory/process limits. Buffer reservations do not prove hard
realtime deadlines or bound the DDS implementation's internal receive allocation.
Traffic limiting remains separate from capacity reservations.

## Deferred decisions and real state

A server receives a goal into an owned bounded proposal record. It sends no
positive native goal response until the guest successfully takes that record and
performs an explicitly admitted accept. Failed output delivery leaves the same
proposal available. This avoids falsely auto-accepting an action while waiting
for a later guest decision. Reject produces an actual negative native response.

The state graph uses pending (0), accepted (1), executing (2), cancelling (3),
succeeded (4), cancelled (5), aborted (6) and locally represented rejected (7).
Execute requires accepted; succeed/abort require executing or cancelling;
cancelled requires cancelling. Feedback is admitted only while executing. A
positive cancel response moves the goal to cancelling; it is not a terminal
result or a physical stop. Accepting a goal never implicitly executes it.

The client supplies an exact nonzero 16-byte UUID, paired with its owned goal
resource. The native adapter sends that exact UUID. On native acceptance it
immediately requests the result, independently of guest polling, so a slow guest
does not delay the result request past the server's retention period. Result,
feedback and status callbacks are correlated with the owned UUID. Malformed or
lost asynchronous native outcomes latch uncertainty instead of manufacturing a
successful result.

The currently qualified cancellation profile supports one exact owned UUID per
native request. Zero-UUID and time-based broad sets are explicitly rejected.
Supporting them needs a configured candidate-set reservation and guest selection
of a bounded owned subset. This remains work to complete; the canonical response
decoder retains a bounded `cancelled_goals` list and does not silently collapse a
broad set into one goal. The current native server also reserves one pending
result waiter per goal; configurable multiple-waiter support remains outstanding.

## Recovery and teardown

Mutation keys use the endpoint session and monotonic sequence. An identical last
request returns the same retained receipt without another native call. Different
input under an old key is rejected. A rejected pre-effect request does not consume
the key. A committed mutation whose guest result copy fails is recovered by exact
retry, not by rolling back an already issued native operation.

An uncertain submit still permits a separately admitted exact-goal cancellation
attempt within its reserved capacity. Cancellation cannot erase the uncertainty
latch. Close reports busy while uncertainty remains or a goal has not reached a
terminal state. Revocation fences callback delivery and marks outstanding goals
uncertain. The deployment must retain a retired profile while `cleanup_debt()` is
true; destroying a C++ object or restarting a worker is not reconciliation.
Durable restart fencing and qualified remote-state reconstruction remain required
deployment work. No physical actuator completion or independently powered stop is
inferred from action results.

Fork children cannot reuse the endpoint. Inherited ownership is abandoned without
running native middleware cleanup in the child; only exec/exit is supported for
that child. The originating process retains normal cleanup and attachment counts.

## Canonical records

`communication_action_contract.h` declares `action.open`, `action.submit`,
`action.take`, `action.respond`, `action.cancel` and `action.describe`. They use
the common canonical seven-argument carrier and bounded record schemas. Names
inside the implementation describe semantics and contain no arbitrary version
suffixes.

An action delivery's outer `correlation` is its request token. Its `kind` is one
of `action.goal.proposal`, `action.goal.acceptance`, `action.goal.status`,
`action.feedback`, `action.result`, `action.cancel.request`, or
`action.cancel.response`. Its message is `uwvm/action/Event`, encoding
`scene-canonical`, with fields in this order:

1. Owned goal handle (`u64`).
2. Exact UUID (`bytes`, exactly 16 bytes).
3. Closed state (`u64`, values above).
4. Optional complete `message` payload.

Proposal, feedback and result payloads preserve the corresponding approved
schema identity and encoding. Acceptance/status/exact cancel requests carry no
payload. A cancel response carries `uwvm/action/CancelResponse`, whose body is the
actual closed ROS return code as `u64`: 0 accepted, 1 rejected, 2 unknown goal,
3 terminated goal. A cancellation code is not inferred from a state snapshot.
The event's goal handle and UUID are never substituted for its request token.

`action.describe` returns `uwvm/action/State`: profile, resolved name, action
type, role text, goal capacity, cancellation capacity, retention, uncertainty and
an optional owned goal record. The goal record is handle, UUID, state, proposal
request, latest cancel request, proposal-delivered, latest-cancel-delivered,
uncertainty and retention deadline. It exposes no unrelated goals or native
request headers. New event/state schemas must be described in the central
catalogue when the imports are registered.

Metadata has no fabricated source timestamp or clock epoch. The common encoder
omits a timestamp if its native value has no deployment-proven clock mapping.
All output is measured and completely bounded before committing delivery; short
output keeps the original event and sequence for retry.

## Qualification

All compilers, native tests and traces run on CPUs 4–7 in the existing 64 GiB,
zero-swap cgroup. A fresh immutable source snapshot and dependency hashes are
retained for every run. Use a new output directory:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 4-7 \
  /usr/bin/python3 tools/ros_scene/communication_checks.py \
  --suite actions --mode all --trace-native \
  --ros-root ../uwvm2-ros-sdk/.tmp/ros-scene-security/communication-ros-sysroot \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/NEW-ACTION-OUTPUT
```

`communication-action-native-qos/results.json` passed ten commands: Clang
ASan/UBSan, GCC and TSan core compilation/execution, native compilation/link,
actual native execution and a network trace. Official `rclcpp_action` peers
exercise both directions, deferred acceptance, execution, feedback, rejection
followed by a second cancellation, terminal result, exact UUID/replay and failed
result delivery recovery. The binary SHA256 is
`9d015f3f8143e6f334f08a9677caa113c5e143addd2f1a5b3fa4efc00abfcc20`.
All 38 traced IPv4 bind/connect/send operations used `127.0.0.1`. The shared
cgroup's `memory.events max` increased from 7383 to 7767 during this run; OOM and
kill counters remained zero. This is a safety/semantic `-O1 -g` fixture, not a
performance claim or an industrial safety certification. Later canonical
encoding changes require their separate qualification receipt.
