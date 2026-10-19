# Explicit ROS capability schemas

The authoritative schemas are `src/uwvm2/imported/ros/policy/schema.h`, type checks are `common/message.h`, and runtime handles are `capability/manager.h`. This document lists the fields emitted by the actual `operation_schema` implementation. A schema existing for an operation does not establish a working network adapter or a passed network test.

Each ROS version starts with a disabled Global target and empty connection/capability maps. Creating a Single or Group target also leaves it disabled and empty. Only explicit CLI configuration adds grants and enables targets: `enable` changes the enabled state without granting operations, while `grant` and `set` configure a capability without enabling its target. An enabled target with no grants permits no Guest resource operations; configured grants in a disabled target remain inactive.

Each grant explicitly supplies ROS version, target scope/name, alias, operation, absolute resource name, type, and trusted type identity. Each listed field is mandatory. There are no levels, roles, presets, resource wildcards, inherited grants, or implicit QoS values. A Single or Group binding selects that target's complete policy alone, including its enabled state and connection fields; an unbound module selects the corresponding version's Global target. Explicitly granting/enabling Global affects unbound modules only. A disabled or empty explicit target never falls back to Global grants. ROS1, ROS2, and WASI group namespaces are independent. Group policies are shared; instance handle tables, rate buckets, audit sinks and gateway sessions are independent.

Module names match trusted registered runtime UTF-8 names exactly (maximum 1024 bytes, no control characters). Group and capability aliases use ASCII letters, numbers, underscore, hyphen and dot (maximum 255 bytes, excluding `.` and `..`). ROS resource names must be absolute, have ASCII letter/underscore-led segments, and have no duplicate/trailing slash, dot segments, private/relative forms, wildcards, or remapping syntax. Root `/` is only admitted for explicit graph/list prefix scopes.

## Explicit target connections

Enabled targets require `gateway-socket` (canonical absolute Unix socket path, at most107 bytes), `gateway-uid` (integer0..4294967294), `node-name` (ROS basename), `namespace` (absolute canonical namespace), and `connect-timeout-ms` (1..60000). Each also supplies denial audit rate (finite positive <=1000000), burst and retained record count (1..1048576), and retained byte count (1..67108864) through the four `audit-deny-*` fields. Disabled targets may omit all four denial fields or explicitly set all four.

ROS1 additionally requires `master-uri` (http://host:explicit-port1..65535), `advertise-host` (explicit native address), and `transport=tcpros`. ROS2 additionally requires `domain-id` (0..232), `rmw-implementation` (qualified installed RMW), `security-mode` (enforce or disabled-test), `keystore`, and `enclave`. Enforce needs explicit absolute credential paths and a canonical enclave; disabled-test requires both paths to be exactly not-applicable. The gateway validates real loaded RMW and security material before startup.

The binding manifest serializes version and all backend connection, alias, operation, resource, type identity and capability limit values in deterministic key order. Socket/peer UID/connect timeout and Worker denial audit fields are local and excluded. The gateway verifies exact manifest equality before constructing native resources.

## Operation schemas

`AUDIT` means all five mandatory fields `audit`, `audit-max-rate-hz`, `audit-burst`, `audit-max-records`, `audit-max-bytes`. The backend column states whether the policy engine can admit an active grant; successful real-network qualification is reported separately in the testing report. Unknown operations return UNSUPPORTED. Known schemas whose adapters are absent also fail before an enabled target can start. Disabled targets may retain a complete inactive unsupported schema for diagnosis.

| ROS | Operation | Mandatory fields (plus AUDIT) | Backend |
| --- | --- | --- | --- |
| ros1 | `topic.publish` | `max-message-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-queue-bytes`, `overflow-policy`, `validator`, `max-publishers`, `queue-depth`, `transport`, `latch` | IMPLEMENTED |
| ros1 | `topic.subscribe` | `max-message-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-queue-bytes`, `overflow-policy`, `validator`, `max-subscribers`, `queue-depth`, `transport` | IMPLEMENTED |
| ros1 | `service.call` | `timeout-ms`, `max-inflight`, `max-request-bytes`, `max-response-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator` | IMPLEMENTED |
| ros1 | `service.advertise` | `timeout-ms`, `max-inflight`, `max-request-bytes`, `max-response-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `max-services`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy` | IMPLEMENTED |
| ros1 | `service.respond` | `timeout-ms`, `max-inflight`, `max-request-bytes`, `max-response-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator` | IMPLEMENTED |
| ros1 | `param.get` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator` | IMPLEMENTED |
| ros1 | `param.set` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator` | IMPLEMENTED |
| ros1 | `param.delete` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator` | IMPLEMENTED |
| ros1 | `param.exists` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator` | IMPLEMENTED |
| ros1 | `param.search` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `allowed-name-prefix` | IMPLEMENTED |
| ros1 | `param.list` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `allowed-name-prefix` | IMPLEMENTED |
| ros1 | `graph.list-topics` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros1 | `graph.list-nodes` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros1 | `graph.list-services` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros1 | `graph.inspect-topic` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros1 | `graph.inspect-node` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros1 | `graph.inspect-service` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros1 | `action.send-goal` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator` | IMPLEMENTED |
| ros1 | `action.cancel-own-goal` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator` | IMPLEMENTED |
| ros1 | `action.cancel-any-goal` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator` | IMPLEMENTED |
| ros1 | `action.receive-feedback` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator` | IMPLEMENTED |
| ros1 | `action.receive-result` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator` | IMPLEMENTED |
| ros1 | `action.advertise` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator` | IMPLEMENTED |
| ros1 | `action.respond` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator` | IMPLEMENTED |
| ros1 | `tf.lookup` | `source-frame`, `target-frame`, `tf.max-translation-abs`, `tf.quaternion-norm-tolerance`, `timeout-ms`, `max-message-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-inflight`, `max-queue-bytes`, `overflow-policy`, `buffer-cache-ms`, `validator`, `queue-depth`, `transport` | IMPLEMENTED |
| ros1 | `tf.broadcast` | `source-frame`, `target-frame`, `tf.max-translation-abs`, `tf.quaternion-norm-tolerance`, `timeout-ms`, `max-message-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-inflight`, `max-queue-bytes`, `overflow-policy`, `buffer-cache-ms`, `validator`, `queue-depth`, `transport`, `latch` | IMPLEMENTED |
| ros1 | `time.read-ros` | `max-inflight`, `max-response-bytes`, `max-bytes-per-second`, `max-rate-hz`, `burst` | IMPLEMENTED |
| ros1 | `time.read-wall` | `max-inflight`, `max-response-bytes`, `max-bytes-per-second`, `max-rate-hz`, `burst` | IMPLEMENTED |
| ros1 | `log.write` | `max-inflight`, `max-message-bytes`, `max-bytes-per-second`, `max-log-bytes-per-second`, `max-rate-hz`, `burst`, `severity` | IMPLEMENTED |
| ros2 | `topic.publish` | `max-message-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-queue-bytes`, `overflow-policy`, `validator`, `max-publishers`, `max-inflight`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `topic.subscribe` | `max-message-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-queue-bytes`, `overflow-policy`, `validator`, `max-subscribers`, `max-inflight`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `service.call` | `timeout-ms`, `max-inflight`, `max-request-bytes`, `max-response-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `service.advertise` | `timeout-ms`, `max-inflight`, `max-request-bytes`, `max-response-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `max-services`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `service.respond` | `timeout-ms`, `max-inflight`, `max-request-bytes`, `max-response-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `param.get` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `parameter-node`, `parameter-name`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `param.set` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `parameter-node`, `parameter-name`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `param.list` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `parameter-node`, `parameter-name`, `list-depth`, `allowed-name-prefix`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `param.declare` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `parameter-node`, `parameter-name` | IMPLEMENTED |
| ros2 | `param.undeclare` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `parameter-node`, `parameter-name` | IMPLEMENTED |
| ros2 | `param.describe` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `parameter-node`, `parameter-name`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `param.subscribe-events` | `max-parameter-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `validator`, `parameter-node`, `parameter-name`, `max-subscribers`, `max-event-fields`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `graph.list-topics` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros2 | `graph.list-nodes` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros2 | `graph.list-services` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros2 | `graph.inspect-topic` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros2 | `graph.inspect-node` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros2 | `graph.inspect-service` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros2 | `graph.inspect-action` | `allowed-name-prefix`, `max-graph-result-bytes`, `timeout-ms`, `max-inflight`, `max-rate-hz`, `burst`, `max-bytes-per-second` | IMPLEMENTED |
| ros2 | `action.send-goal` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator`, `qos.goal-service.reliability`, `qos.goal-service.durability`, `qos.goal-service.history`, `qos.goal-service.depth`, `qos.goal-service.deadline`, `qos.goal-service.lifespan`, `qos.goal-service.liveliness`, `qos.goal-service.liveliness-lease-duration`, `qos.cancel-service.reliability`, `qos.cancel-service.durability`, `qos.cancel-service.history`, `qos.cancel-service.depth`, `qos.cancel-service.deadline`, `qos.cancel-service.lifespan`, `qos.cancel-service.liveliness`, `qos.cancel-service.liveliness-lease-duration`, `qos.result-service.reliability`, `qos.result-service.durability`, `qos.result-service.history`, `qos.result-service.depth`, `qos.result-service.deadline`, `qos.result-service.lifespan`, `qos.result-service.liveliness`, `qos.result-service.liveliness-lease-duration`, `qos.feedback.reliability`, `qos.feedback.durability`, `qos.feedback.history`, `qos.feedback.depth`, `qos.feedback.deadline`, `qos.feedback.lifespan`, `qos.feedback.liveliness`, `qos.feedback.liveliness-lease-duration`, `qos.status.reliability`, `qos.status.durability`, `qos.status.history`, `qos.status.depth`, `qos.status.deadline`, `qos.status.lifespan`, `qos.status.liveliness`, `qos.status.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `action.cancel-own-goal` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator`, `qos.goal-service.reliability`, `qos.goal-service.durability`, `qos.goal-service.history`, `qos.goal-service.depth`, `qos.goal-service.deadline`, `qos.goal-service.lifespan`, `qos.goal-service.liveliness`, `qos.goal-service.liveliness-lease-duration`, `qos.cancel-service.reliability`, `qos.cancel-service.durability`, `qos.cancel-service.history`, `qos.cancel-service.depth`, `qos.cancel-service.deadline`, `qos.cancel-service.lifespan`, `qos.cancel-service.liveliness`, `qos.cancel-service.liveliness-lease-duration`, `qos.result-service.reliability`, `qos.result-service.durability`, `qos.result-service.history`, `qos.result-service.depth`, `qos.result-service.deadline`, `qos.result-service.lifespan`, `qos.result-service.liveliness`, `qos.result-service.liveliness-lease-duration`, `qos.feedback.reliability`, `qos.feedback.durability`, `qos.feedback.history`, `qos.feedback.depth`, `qos.feedback.deadline`, `qos.feedback.lifespan`, `qos.feedback.liveliness`, `qos.feedback.liveliness-lease-duration`, `qos.status.reliability`, `qos.status.durability`, `qos.status.history`, `qos.status.depth`, `qos.status.deadline`, `qos.status.lifespan`, `qos.status.liveliness`, `qos.status.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `action.cancel-any-goal` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator`, `qos.goal-service.reliability`, `qos.goal-service.durability`, `qos.goal-service.history`, `qos.goal-service.depth`, `qos.goal-service.deadline`, `qos.goal-service.lifespan`, `qos.goal-service.liveliness`, `qos.goal-service.liveliness-lease-duration`, `qos.cancel-service.reliability`, `qos.cancel-service.durability`, `qos.cancel-service.history`, `qos.cancel-service.depth`, `qos.cancel-service.deadline`, `qos.cancel-service.lifespan`, `qos.cancel-service.liveliness`, `qos.cancel-service.liveliness-lease-duration`, `qos.result-service.reliability`, `qos.result-service.durability`, `qos.result-service.history`, `qos.result-service.depth`, `qos.result-service.deadline`, `qos.result-service.lifespan`, `qos.result-service.liveliness`, `qos.result-service.liveliness-lease-duration`, `qos.feedback.reliability`, `qos.feedback.durability`, `qos.feedback.history`, `qos.feedback.depth`, `qos.feedback.deadline`, `qos.feedback.lifespan`, `qos.feedback.liveliness`, `qos.feedback.liveliness-lease-duration`, `qos.status.reliability`, `qos.status.durability`, `qos.status.history`, `qos.status.depth`, `qos.status.deadline`, `qos.status.lifespan`, `qos.status.liveliness`, `qos.status.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `action.receive-feedback` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator`, `qos.goal-service.reliability`, `qos.goal-service.durability`, `qos.goal-service.history`, `qos.goal-service.depth`, `qos.goal-service.deadline`, `qos.goal-service.lifespan`, `qos.goal-service.liveliness`, `qos.goal-service.liveliness-lease-duration`, `qos.cancel-service.reliability`, `qos.cancel-service.durability`, `qos.cancel-service.history`, `qos.cancel-service.depth`, `qos.cancel-service.deadline`, `qos.cancel-service.lifespan`, `qos.cancel-service.liveliness`, `qos.cancel-service.liveliness-lease-duration`, `qos.result-service.reliability`, `qos.result-service.durability`, `qos.result-service.history`, `qos.result-service.depth`, `qos.result-service.deadline`, `qos.result-service.lifespan`, `qos.result-service.liveliness`, `qos.result-service.liveliness-lease-duration`, `qos.feedback.reliability`, `qos.feedback.durability`, `qos.feedback.history`, `qos.feedback.depth`, `qos.feedback.deadline`, `qos.feedback.lifespan`, `qos.feedback.liveliness`, `qos.feedback.liveliness-lease-duration`, `qos.status.reliability`, `qos.status.durability`, `qos.status.history`, `qos.status.depth`, `qos.status.deadline`, `qos.status.lifespan`, `qos.status.liveliness`, `qos.status.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `action.receive-result` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator`, `qos.goal-service.reliability`, `qos.goal-service.durability`, `qos.goal-service.history`, `qos.goal-service.depth`, `qos.goal-service.deadline`, `qos.goal-service.lifespan`, `qos.goal-service.liveliness`, `qos.goal-service.liveliness-lease-duration`, `qos.cancel-service.reliability`, `qos.cancel-service.durability`, `qos.cancel-service.history`, `qos.cancel-service.depth`, `qos.cancel-service.deadline`, `qos.cancel-service.lifespan`, `qos.cancel-service.liveliness`, `qos.cancel-service.liveliness-lease-duration`, `qos.result-service.reliability`, `qos.result-service.durability`, `qos.result-service.history`, `qos.result-service.depth`, `qos.result-service.deadline`, `qos.result-service.lifespan`, `qos.result-service.liveliness`, `qos.result-service.liveliness-lease-duration`, `qos.feedback.reliability`, `qos.feedback.durability`, `qos.feedback.history`, `qos.feedback.depth`, `qos.feedback.deadline`, `qos.feedback.lifespan`, `qos.feedback.liveliness`, `qos.feedback.liveliness-lease-duration`, `qos.status.reliability`, `qos.status.durability`, `qos.status.history`, `qos.status.depth`, `qos.status.deadline`, `qos.status.lifespan`, `qos.status.liveliness`, `qos.status.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `action.advertise` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator`, `qos.goal-service.reliability`, `qos.goal-service.durability`, `qos.goal-service.history`, `qos.goal-service.depth`, `qos.goal-service.deadline`, `qos.goal-service.lifespan`, `qos.goal-service.liveliness`, `qos.goal-service.liveliness-lease-duration`, `qos.cancel-service.reliability`, `qos.cancel-service.durability`, `qos.cancel-service.history`, `qos.cancel-service.depth`, `qos.cancel-service.deadline`, `qos.cancel-service.lifespan`, `qos.cancel-service.liveliness`, `qos.cancel-service.liveliness-lease-duration`, `qos.result-service.reliability`, `qos.result-service.durability`, `qos.result-service.history`, `qos.result-service.depth`, `qos.result-service.deadline`, `qos.result-service.lifespan`, `qos.result-service.liveliness`, `qos.result-service.liveliness-lease-duration`, `qos.feedback.reliability`, `qos.feedback.durability`, `qos.feedback.history`, `qos.feedback.depth`, `qos.feedback.deadline`, `qos.feedback.lifespan`, `qos.feedback.liveliness`, `qos.feedback.liveliness-lease-duration`, `qos.status.reliability`, `qos.status.durability`, `qos.status.history`, `qos.status.depth`, `qos.status.deadline`, `qos.status.lifespan`, `qos.status.liveliness`, `qos.status.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `action.respond` | `max-active-goals`, `max-actions`, `timeout-ms`, `max-goal-bytes`, `max-feedback-bytes`, `max-result-bytes`, `fibonacci.max-order`, `cancel-scope`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-event-queue-depth`, `max-event-queue-bytes`, `overflow-policy`, `validator`, `qos.goal-service.reliability`, `qos.goal-service.durability`, `qos.goal-service.history`, `qos.goal-service.depth`, `qos.goal-service.deadline`, `qos.goal-service.lifespan`, `qos.goal-service.liveliness`, `qos.goal-service.liveliness-lease-duration`, `qos.cancel-service.reliability`, `qos.cancel-service.durability`, `qos.cancel-service.history`, `qos.cancel-service.depth`, `qos.cancel-service.deadline`, `qos.cancel-service.lifespan`, `qos.cancel-service.liveliness`, `qos.cancel-service.liveliness-lease-duration`, `qos.result-service.reliability`, `qos.result-service.durability`, `qos.result-service.history`, `qos.result-service.depth`, `qos.result-service.deadline`, `qos.result-service.lifespan`, `qos.result-service.liveliness`, `qos.result-service.liveliness-lease-duration`, `qos.feedback.reliability`, `qos.feedback.durability`, `qos.feedback.history`, `qos.feedback.depth`, `qos.feedback.deadline`, `qos.feedback.lifespan`, `qos.feedback.liveliness`, `qos.feedback.liveliness-lease-duration`, `qos.status.reliability`, `qos.status.durability`, `qos.status.history`, `qos.status.depth`, `qos.status.deadline`, `qos.status.lifespan`, `qos.status.liveliness`, `qos.status.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `tf.lookup` | `source-frame`, `target-frame`, `tf.max-translation-abs`, `tf.quaternion-norm-tolerance`, `timeout-ms`, `max-message-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-inflight`, `max-queue-bytes`, `overflow-policy`, `buffer-cache-ms`, `validator`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `tf.broadcast` | `source-frame`, `target-frame`, `tf.max-translation-abs`, `tf.quaternion-norm-tolerance`, `timeout-ms`, `max-message-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `max-inflight`, `max-queue-bytes`, `overflow-policy`, `buffer-cache-ms`, `validator`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `time.read-ros` | `max-inflight`, `max-response-bytes`, `max-bytes-per-second`, `max-rate-hz`, `burst` | IMPLEMENTED |
| ros2 | `time.read-steady` | `max-inflight`, `max-response-bytes`, `max-bytes-per-second`, `max-rate-hz`, `burst` | IMPLEMENTED |
| ros2 | `time.read-system` | `max-inflight`, `max-response-bytes`, `max-bytes-per-second`, `max-rate-hz`, `burst` | IMPLEMENTED |
| ros2 | `log.write` | `max-inflight`, `max-message-bytes`, `max-bytes-per-second`, `max-log-bytes-per-second`, `max-rate-hz`, `burst`, `severity` | IMPLEMENTED |
| ros2 | `lifecycle.get-state` | `timeout-ms`, `max-inflight`, `max-request-bytes`, `max-response-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |
| ros2 | `lifecycle.change-state` | `timeout-ms`, `max-inflight`, `max-request-bytes`, `max-response-bytes`, `max-rate-hz`, `burst`, `max-bytes-per-second`, `allowed-transition`, `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness`, `qos.liveliness-lease-duration` | IMPLEMENTED |

## Field types and accepted values

Numeric parsing consumes the complete argument and never supplies a missing value. Unsigned quota/count fields reject integer signs, and floating limits reject NaN/infinity. The signed numeric parameter bounds below have their own explicit grammar. Limits below are public protocol safety ceilings, not a resource authorization or implicit quota. Actual caller-supplied values remain mandatory.

| Field | Type | Accepted values / range |
| --- | --- | --- |
| `allowed-name-prefix` | canonical absolute name | segment boundary prefix; / explicitly permitted |
| `allowed-transition` | enum | configure  /  activate  /  deactivate  /  cleanup  /  shutdown |
| `audit` | enum | allow-and-deny  /  deny-only |
| `audit-burst` | integer | 1..1048576 |
| `audit-max-bytes` | integer bytes | 1..67108864 retained bytes per instance/capability |
| `audit-max-rate-hz` | finite number | 0 < value <= 1000000 |
| `audit-max-records` | integer | 1..1048576 retained records per instance/capability |
| `burst` | integer | 1..1048576 |
| `cancel-scope` | enum | own for cancel-own-goal; any for cancel-any-goal; not-applicable otherwise |
| `latch` | boolean | true  /  false (ROS1 publisher only) |
| `max-actions` | integer | 1..1048576 |
| `max-active-goals` | integer | 1..1048576 |
| `max-bytes-per-second` | integer bytes/second | 1..1073741824 |
| `max-event-queue-bytes` | integer bytes | 1..67108864 |
| `max-event-queue-depth` | integer | 1..1048576 |
| `max-feedback-bytes` | integer bytes | 1..4193786 (4 MiB IPC frame minus maximum request envelope); time response requires >=8 |
| `max-goal-bytes` | integer bytes | 1..4193786 (4 MiB IPC frame minus maximum request envelope); time response requires >=8 |
| `max-graph-result-bytes` | integer bytes | 1..4193786 (4 MiB IPC frame minus maximum request envelope); time response requires >=8 |
| `max-inflight` | integer | 1..1048576 |
| `max-log-bytes-per-second` | integer bytes/second | 1..1073741824 |
| `max-message-bytes` | integer bytes | 1..4193786 (4 MiB IPC frame minus maximum request envelope); time response requires >=8 |
| `max-parameter-bytes` | integer bytes | 1..4193786 (4 MiB IPC frame minus maximum request envelope); time response requires >=8 |
| `max-parameter-elements` | integer elements | 1..1048576, mandatory for typed parameter arrays and ROS2 typed array event projections; scalar, metadata and legacy event grants reject it |
| `max-publishers` | integer | 1..1048576 |
| `max-queue-bytes` | integer bytes | 1..67108864 |
| `max-rate-hz` | finite number | 0 < value <= 1000000; trusted monotonic token bucket |
| `max-request-bytes` | integer bytes | 1..4193786 (4 MiB IPC frame minus maximum request envelope); time response requires >=8 |
| `max-response-bytes` | integer bytes | 1..4193786 (4 MiB IPC frame minus maximum request envelope); time response requires >=8 |
| `max-result-bytes` | integer bytes | 1..4193786 (4 MiB IPC frame minus maximum request envelope); time response requires >=8 |
| `max-services` | integer | 1..1048576 |
| `max-subscribers` | integer | 1..1048576 |
| `overflow-policy` | enum | drop-oldest  /  drop-newest  /  reject; backpressure is UNSUPPORTED |
| `qos.deadline` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.depth` | integer | 1..1048576 |
| `qos.durability` | enum | volatile  /  transient-local |
| `qos.history` | enum | keep-last; keep-all is UNSUPPORTED |
| `qos.lifespan` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.liveliness` | enum | automatic  /  manual-by-topic |
| `qos.liveliness-lease-duration` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.reliability` | enum | reliable  /  best-effort |
| `qos.goal-service.reliability` | enum | reliable  /  best-effort |
| `qos.goal-service.durability` | enum | volatile  /  transient-local |
| `qos.goal-service.history` | enum | keep-last; keep-all is UNSUPPORTED |
| `qos.goal-service.depth` | integer | 1..1048576 |
| `qos.goal-service.deadline` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.goal-service.lifespan` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.goal-service.liveliness` | enum | automatic  /  manual-by-topic |
| `qos.goal-service.liveliness-lease-duration` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.cancel-service.reliability` | enum | reliable  /  best-effort |
| `qos.cancel-service.durability` | enum | volatile  /  transient-local |
| `qos.cancel-service.history` | enum | keep-last; keep-all is UNSUPPORTED |
| `qos.cancel-service.depth` | integer | 1..1048576 |
| `qos.cancel-service.deadline` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.cancel-service.lifespan` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.cancel-service.liveliness` | enum | automatic  /  manual-by-topic |
| `qos.cancel-service.liveliness-lease-duration` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.result-service.reliability` | enum | reliable  /  best-effort |
| `qos.result-service.durability` | enum | volatile  /  transient-local |
| `qos.result-service.history` | enum | keep-last; keep-all is UNSUPPORTED |
| `qos.result-service.depth` | integer | 1..1048576 |
| `qos.result-service.deadline` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.result-service.lifespan` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.result-service.liveliness` | enum | automatic  /  manual-by-topic |
| `qos.result-service.liveliness-lease-duration` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.feedback.reliability` | enum | reliable  /  best-effort |
| `qos.feedback.durability` | enum | volatile  /  transient-local |
| `qos.feedback.history` | enum | keep-last; keep-all is UNSUPPORTED |
| `qos.feedback.depth` | integer | 1..1048576 |
| `qos.feedback.deadline` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.feedback.lifespan` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.feedback.liveliness` | enum | automatic  /  manual-by-topic |
| `qos.feedback.liveliness-lease-duration` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.status.reliability` | enum | reliable  /  best-effort |
| `qos.status.durability` | enum | volatile  /  transient-local |
| `qos.status.history` | enum | keep-last; keep-all is UNSUPPORTED |
| `qos.status.depth` | integer | 1..1048576 |
| `qos.status.deadline` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.status.lifespan` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `qos.status.liveliness` | enum | automatic  /  manual-by-topic |
| `qos.status.liveliness-lease-duration` | integer nanoseconds or enum | 1..86400000000000  /  infinite |
| `queue-depth` | integer | 1..1048576 |
| `severity` | enum | debug  /  info  /  warn  /  error  /  fatal |
| `source-frame` | canonical frame name | ASCII ROS name segments; no leading slash, relative/private/remapped paths rejected |
| `target-frame` | canonical frame name | same syntax as source-frame |
| `timeout-ms` | integer milliseconds | 1..60000 |
| `transport` | enum | tcpros (ROS1 only) |
| `validator` | enum | none / finite / twist-bounds / transform-bounds; numeric validators apply only to their exact generated type |
| `parameter-node` | canonical absolute name | exact remote node or explicit gateway node |
| `parameter-name` | dotted ASCII segments | exact parameter name; param.list explicitly uses not-applicable |
| `list-depth` | integer | 1..32 |
| `max-event-fields` | integer | 1..1048576 total native ParameterEvent fields examined |
| `buffer-cache-ms` | integer milliseconds | 1..60000 |
| `tf.max-translation-abs` | finite number | 0 < value <= 1000000 |
| `tf.quaternion-norm-tolerance` | finite number | 0 < value <= 0.001 |
| `fibonacci.max-order` | integer | 0..46; mandatory for generated Fibonacci Action adapter |

`twist-bounds` additionally requires both `twist.linear-max-abs` and `twist.angular-max-abs`, finite positive numbers <=1000000. These conditional fields are forbidden for other validator selections. Every Twist field rejects NaN/infinity regardless of the selected validator. `none` skips the optional application validator; it never skips trusted type, structure, length, finite-number checks or mandatory numeric parameter bounds.

Numeric scalar/array parameter adapters and numeric ROS2 parameter-event projections additionally require both conditional fields `parameter-min-value` and `parameter-max-value`, including the local ROS2 undeclare contract. Bounds are inclusive, min must be <= max, and each numeric array element is checked. Integer bounds use complete signed decimal arguments without a plus sign, fractional part or exponent; int32 uses -2147483648..2147483647 and int64 uses -9223372036854775808..9223372036854775807. Integers are never converted to double for comparison. Float64 bounds use finite decimal arguments with optional minus sign, fraction and exponent; whitespace, hex, NaN, infinity, overflow and nonzero underflow to zero are rejected with locale-independent parsing. Nonnumeric scalar/array values, legacy String events, metadata and other operations reject these bounds as inapplicable. Ordinary scalar payload quotas must allow their exact 1/4/8/8-byte width. Event quotas bound the entire selected envelope and may deliberately be smaller than it, causing delivery rejection/drop without expanding authority. There remain63 operation schemas and94 unconditional field names, plus five conditional fields (two Twist bounds, two numeric parameter bounds and the array element limit).

Remote ROS2 non-String scalar set supports existing statically typed parameters only; dynamically typed or undeclared remote parameters return95 before a generated SetParameters request. The adapter checks the fixed resource's native type and descriptor internally under the same explicit QoS and original deadline. These checks do not grant Guest get/describe authority or disclose their values. ROS1 explicitly granted typed set may create a missing parameter; an existing parameter must match the granted native type. ROS2 local operations share a deadline-bounded mutex across authenticated sessions; the Gateway's native parameter services and parameter-event publisher are disabled. External ROS nodes do not expose an atomic compare-and-set across multiple parameter services, and ROS1 XMLRPC type checks likewise cannot serialize external native writers; neither preflight claims atomicity or rolls back a remotely admitted operation after timeout.

Fibonacci Action envelopes require max-goal-bytes >=4, max-feedback-bytes >=12, max-result-bytes >=13, and max-event-queue-bytes >=12. Feedback/result payload quotas include the ID/status/count envelope. ROS2 Action has five independently explicit QoS profiles as listed above.

Topic grants reject `max-queue-bytes < max-message-bytes`, `max-message-bytes > max-bytes-per-second`, and `burst > queue-depth` (ROS1) or `burst > qos.depth` (ROS2). Multiple grants for the same operation/resource/type under different aliases are rejected. Topic/service/action resources cannot use incompatible types or identities within a target. Services allow only actual generated Trigger/SetBool adapters. Other dynamic/custom messages have no trusted adapter and are UNSUPPORTED. ROS2 parameter declaration/undeclaration act on the exact configured Gateway node; get/set/describe/list use direct local node APIs for that node, or generated remote parameter services for another explicit node. Get/set and local ROS2 declare/undeclare admit the explicitly selected String/Bool/backend-specific integer/finite Float64 adapter. The closed typed array adapters listed below are available under separate explicit array grants. Other structured values remain UNSUPPORTED. Native read-only local undeclaration refuses with13 and does not retire the session or its other grants; static parameter undeclaration remains95. These native constraints do not grant additional parameter access.

Time/log require explicit `max-inflight` to bound open capability handles. Time has an empty request and exactly eight bytes of u64 nanoseconds in its response. Logging has a canonical UTF-8 string request; the gateway's explicit severity is fixed by the capability. Logging byte consumption is bounded by the minimum of independently supplied `max-bytes-per-second` and `max-log-bytes-per-second`. Parameter get has an empty request and returns the capability's exact canonical value; set/declare consume that value and return an empty successful response. ROS1 delete/exists/search/list and ROS2 describe/list keep their existing `builtin/string-v1` metadata contract; exists returns canonical Bool. They do not grant get or set. ROS2 declare/undeclare remain local-only and declarations use a removable dynamic descriptor. Undeclare has an empty request but checks the resident native type and numeric bounds before removal. No String/numeric conversion or integer widening across a transport's advertised type is implicit. Parameter lists filter both resource and allowed-name-prefix on segment boundaries. ROS1 search returns only the explicitly granted absolute parameter, without searching ancestors outside that grant. Graph inspect operations return an exact resource snapshot, while lists return filtered rows; ROS1 Master does not report service types, represented by `not-applicable`.

## Trusted initial type identities

ROS1 uses generated message/service MD5 traits. ROS2 uses the Jazzy-generated RIHS01 type-description hashes, checked again against generated rosidl type support by the gateway. These identities are selected by trusted configuration and never accepted from guest claims. Builtin virtual-operation types identify a specific bounded adapter protocol, rather than a ROS serialized message.

| ROS | Type | Trusted identity |
| --- | --- | --- |
| ros1 | `std_msgs/String` | `992ce8a1687cec8c8bd883ec73ca41d1` |
| ros1 | `std_msgs/Bool` | `8b94c1b53db61fb6aed406028ad6332a` |
| ros1 | `geometry_msgs/Twist` | `9f195f881246fdfa2798d1d3eebca84a` |
| ros1 | `sensor_msgs/Image` | `060021388200f6f0f447d0fcd9c64743` |
| ros1 | `std_srvs/Trigger` | `937c9679a518e3a18d831e57125ea522` |
| ros1 | `std_srvs/SetBool` | `09fb03525b03e7ea1fd3992bafd87e16` |
| ros2 | `std_msgs/msg/String` | `RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18` |
| ros2 | `std_msgs/msg/Bool` | `RIHS01_feb91e995ff9ebd09c0cb3d2aed18b11077585839fb5db80193b62d74528f6c9` |
| ros2 | `geometry_msgs/msg/Twist` | `RIHS01_9c45bf16fe0983d80e3cfe750d6835843d265a9a6c46bd2e609fcddde6fb8d2a` |
| ros2 | `sensor_msgs/msg/Image` | `RIHS01_d31d41a9a4c4bc8eae9be757b0beed306564f7526c88ea6a4588fb9582527d47` |
| ros2 | `std_srvs/srv/Trigger` | `RIHS01_eeff2cd6fa5ad9d27cdf4dec64818317839b62f212a91e6b5304b634b2062c5f` |
| ros2 | `std_srvs/srv/SetBool` | `RIHS01_abe9e4bb6b41b40e6789712c00ec8871923e089af3f667a79992a428cff2da0a` |
| ros1 + ros2 | `builtin/time-v1` | `builtin:time-nanoseconds-v1` |
| ros1 + ros2 | `builtin/log-v1` | `builtin:log-utf8-v1` |
| ros1 + ros2 | `builtin/string-v1` | `builtin:param-string-v1` |
| ros1 + ros2 | `builtin/bool-v1` | `builtin:param-bool-v1` |
| ros1 | `builtin/int32-v1` | `builtin:param-int32-v1` |
| ros2 | `builtin/int64-v1` | `builtin:param-int64-v1` |
| ros1 + ros2 | `builtin/float64-v1` | `builtin:param-float64-v1` |
| ros1 + ros2 | `builtin/bool-array-v1` | `builtin:param-bool-array-v1` |
| ros1 | `builtin/int32-array-v1` | `builtin:param-int32-array-v1` |
| ros2 | `builtin/int64-array-v1` | `builtin:param-int64-array-v1` |
| ros1 + ros2 | `builtin/float64-array-v1` | `builtin:param-float64-array-v1` |
| ros1 + ros2 | `builtin/string-array-v1` | `builtin:param-string-array-v1` |
| ros2 | `builtin/byte-array-v1` | `builtin:param-byte-array-v1` |
| ros1 | `geometry_msgs/TransformStamped` | `b5764a33bfeb3588febc2682852579b0` |
| ros2 | `geometry_msgs/msg/TransformStamped` | `RIHS01_0a241f87d04668d94099cbb5ba11691d5ad32c2f29682e4eb5653424bd275206` |
| ros2 | `rcl_interfaces/msg/ParameterEvent` | `RIHS01_043e627780fcad87a22d225bc2a037361dba713fca6a6b9f4b869a5aa0393204` |
| ros2 | `builtin/param-event-bool-v1` | `builtin:param-event-bool-v1` |
| ros2 | `builtin/param-event-int64-v1` | `builtin:param-event-int64-v1` |
| ros2 | `builtin/param-event-float64-v1` | `builtin:param-event-float64-v1` |
| ros2 | `builtin/param-event-string-v1` | `builtin:param-event-string-v1` |
| ros2 | `builtin/param-event-byte-array-v1` | `builtin:param-event-byte-array-v1` |
| ros2 | `builtin/param-event-bool-array-v1` | `builtin:param-event-bool-array-v1` |
| ros2 | `builtin/param-event-int64-array-v1` | `builtin:param-event-int64-array-v1` |
| ros2 | `builtin/param-event-float64-array-v1` | `builtin:param-event-float64-array-v1` |
| ros2 | `builtin/param-event-string-array-v1` | `builtin:param-event-string-array-v1` |
| ros2 | `builtin/param-event-deletion-v1` | `builtin:param-event-deletion-v1` |
| ros2 | `lifecycle_msgs/srv/GetState` | `RIHS01_800a0a5aae599782b02932de0caf563f6dc4e7e94b794eadde075ba2cbef9795` |
| ros2 | `lifecycle_msgs/srv/ChangeState` | `RIHS01_356fe34f0475a43acf54542013af4167b0e729f77ea22ffb045c6ad8e20668e5` |
| ros1 + ros2 | `builtin/graph-list-v1` | `builtin:graph-list-v1` |
| ros1 | `actionlib_tutorials/Fibonacci` | `f59df5767bf7634684781c92598b2406` |
| ros2 | `action_tutorials_interfaces/action/Fibonacci` | `RIHS01_525d2c3c8a3979003645ffa01186e360347194550426993cad2fb950058ab5a0` |

## Payload validation

Scalar parameter payloads retain the existing imports: Bool is exactly one byte 0/1, ROS1 int32 is four little-endian two's-complement bytes, ROS2 int64 is eight, and Float64 is eight little-endian IEEE754 bytes. String remains u32 length plus exact UTF-8 bytes. `max-parameter-bytes` must explicitly fit at least 1/4/8/8 bytes respectively (String minimum four); successful get values also satisfy the numeric bounds. Malformed width/trailing data/Bool2/NaN/infinity returns22, quota excess90, numeric bound excess13, and a mismatched native type95. Invalid decodes preserve output arguments, and the host copies no response value to guest memory until validation succeeds. Float64 remains finite under validator=none and validator=finite. ROS1 integers map only to XMLRPC TypeInt/int32; ROS2 integer values preserve the full int64 range, including values above 2^53.

Guest payloads use a separate canonical little-endian protocol. String is u32 length plus exact UTF-8 bytes; Bool is exactly one byte 0/1; Twist is six finite f64 fields. Image is u32 sequence/seconds/nanoseconds, length-prefixed frame ID, u32 height/width, length-prefixed encoding, one endian byte, u32 step, and length-prefixed data. Image checks supported encoding, frame syntax, nonzero dimensions, nanoseconds <1e9, overflow-safe `step*height == data length`, `step >= width*bytes-per-pixel`, and exact consumption. ROS2 seconds must fit nonnegative i32; negative ROS2 timestamps are currently UNSUPPORTED. ROS2 ignores the explicit canonical ROS1 sequence field during generated-type conversion.

TransformStamped uses u32 sequence/seconds/nanoseconds, length-prefixed parent and child frames, then translation x/y/z and quaternion x/y/z/w as seven f64 values. The wire parent must equal target-frame and child must equal source-frame. ROS2 sequence must be zero; seconds are nonnegative i32, and nanoseconds are <1e9. Every field is finite, the quaternion is never implicitly normalized, and both explicit translation/quaternion bounds apply. Lookup requests contain u64 nanoseconds (zero means latest).

For the explicitly selected frame pair, `/tf_static` retains a single latest admissible sample. A changed value replaces that sample even at the same timestamp, while an exactly identical sample may be ignored. Replacement updates retained byte accounting and still requires normal frame/bounds, message/queue byte and rate admission under every overflow policy; replacing one static value does not grow the history. Dynamic `/tf` keeps its existing duplicate-stamp rule, depth/byte history bounds and overflow handling.

The legacy `rcl_interfaces/msg/ParameterEvent` grant keeps its filtered single String wire unchanged: u32 seconds/nanoseconds, one kind byte (1=new, 2=changed, 3=deleted), length-prefixed exact absolute node, dotted parameter name, and UTF-8 value. Deleted values are empty; legacy native NOT_SET deletion remains accepted.

Nine closed ROS2-only `param.subscribe-events` projections select Bool, int64, finite Float64, String, byte array, Bool array, int64 array, finite Float64 array or String array using the `builtin/param-event-*-v1` types and matching trusted identities above. They require the same explicit node/name, limits, audit and eight QoS fields as the legacy grant. Each array projection additionally requires `max-parameter-elements` (1..1048576); scalar and legacy projections reject it. Numeric scalar values and every numeric array element require inclusive `parameter-min-value`/`parameter-max-value` even with `validator=none`; `finite` is additionally accepted for Float64 scalar/array projections. Nonnumeric projections reject numeric bounds. ROS1/int32 event projections remain unsupported, and parameter read/write or metadata grants do not imply an event subscription.

Typed event wire is `u32 seconds`, `u32 nanoseconds`, `u8 event-kind`, `u8 version=2`, `u8 value-tag` (1 Bool, 2 int64, 3 Float64, 4 String, 5 byte array, 6 Bool array, 7 int64 array, 8 Float64 array, 9 String array), followed by **three** u32 byte lengths and their exact node, dotted name and raw selected payload. Fixed overhead is23 bytes; delivered size is `23 + node.size() + name.size() + payload.size()`. Scalar widths remain1/8/8. String payload has its own u32 UTF-8 byte length and permits NUL in the canonical SDK/IPC format; array payload has a u32 count followed by canonical elements as documented below. String array elements are strict UTF-8 and NUL-free. Empty String/array payloads still contain the four-byte zero length/count. Seconds are0..2147483647 and nanoseconds are<1000000000. Existing tag1..3 and legacy bytes are unchanged; all typed decoders preserve outputs on failure.

The tested Jazzy `rmw_fastrtps_cpp` native `rcl_interfaces/msg/ParameterEvent` scalar String publication rejects embedded NUL in FastCDR with `BadParamException` (`The string contains null characters`); an ordinary UTF-8 publication succeeds. The canonical allowance does not establish embedded-NUL DDS interoperability. This measured boundary does not generalize to other RMW implementations.

The same RMW takes a native StringArray sender's element `a\0b` as `a`. The Gateway cannot check the sender's original bytes after that truncation, so native/Wasm tests do not claim rejection of the original embedded NUL. SDK/IPC canonical StringArray elements still reject NUL strictly. This measured boundary does not establish other RMW behavior.

Deletion retains its selected tag but has payload length zero. The Gateway prechecks the borrowed prior native value's exact kind, explicit element quota, canonical hard payload limit (4193786 bytes), UTF-8/finite validity and applicable numeric bounds without copying old arrays. Typed NOT_SET is ignored. `max-parameter-bytes` applies to the actual empty-payload deletion envelope; a valid old value may therefore exceed that delivery quota. Worker validation rechecks envelope/type/node/name/quota and nondeleted contents/bounds; empty deleted wire cannot re-prove the prior native value.

An independent ROS2-only `param.subscribe-events` grant with type `builtin/param-event-deletion-v1` and identity `builtin:param-event-deletion-v1` delivers native NOT_SET deletion for the exact configured node and name. Its version2 wire uses kind3, tag0 and an empty payload. Other kinds, concrete typed deletion and nonempty payloads are rejected. The notification carries no previous type/value and makes no prior-value, numeric-bound or cache attestation. This grant rejects numeric bounds, `finite` and `max-parameter-elements`; it retains explicit eight-field QoS, audit, event-field, envelope-byte, queue and rate limits. It grants no get/set/declare/undeclare, new/changed-event or Topic authority. Existing nine typed projections continue to ignore native NOT_SET deletion. ROS1 does not admit this new projection.

The Gateway separately verifies the installed generated ParameterEvent RIHS even for a builtin projection. `max-event-fields` counts all native Parameter entries across new/changed/deleted arrays before selecting the exact name. Before copying a selected new/changed value, the Gateway prechecks its count, canonical contents, numeric bounds and byte size against the complete envelope budget; rate and queue limits govern enqueue. The Worker validates the queued payload before Guest output copying. A positive `max-parameter-bytes` below the actual envelope is valid explicit policy and drops/rejects that delivery (90). Internal subscription to `/parameter_events` creates no Guest Topic authority.

Fibonacci Action goals contain i32 order, bounded by explicit fibonacci.max-order (0..46). Sending returns a nonzero opaque u64 goal ID. Feedback/results require an independently granted operation and that owned ID; feedback encodes ID, u32 sequence count and i32 values, while results add one status byte (2=success, 3=abort, 4=cancel) after ID. Incoming advertised-goal events encode ID and order. Responding independently authorizes ID plus kind (1=feedback, 2=success, 3=abort, 4=cancel), count and values. IDs belong to the actual gateway session, use a cryptographically randomized process generation and are never reused during that process. A later process rejects prior IDs. Own cancellation requires ownership; explicit any cancellation may select a known foreign goal of the exact authorized Action resource and type, or zero for backend-specific all-goal cancellation. ROS2 zero selects only such registered goals. ROS1 zero emits the standard empty native GoalID and cancels all native goals at the exact granted Action resource, including external native clients. All queues/goals and operation lifetimes use explicit quotas/deadlines.

ROS2 Action config supplies five independent profiles: qos.goal-service.*, qos.cancel-service.*, qos.result-service.*, qos.feedback.* and qos.status.*, each with all eight mandatory properties. Each profile is copied to its matching raw rcl_action options; no rclcpp Action QoS default supplies a permission field. Status durability can independently use transient-local for native compatibility.

Lifecycle requests are empty; change-state chooses only the independently configured allowed-transition. GetState responds with state ID u8 plus canonical UTF-8 label; ChangeState responds with canonical Bool.

Trigger requests are empty. SetBool requests are canonical Bool. Service responses are canonical Bool plus canonical UTF-8 string. Service-server events prefix the request with nonzero u64 request ID; independently authorized responses prefix the response with that ID. IDs belong to the bound gateway instance session and cannot transfer across instances. Request/response configured byte quotas exclude that eight-byte ID prefix; policy validation reserves wire capacity for it. Request/response payloads are copied and checked against the exact capability before gateway side effects or guest delivery. Gateway-generated serialization is independent for ROS1 and ROS2; raw ROS1 wire bytes are never treated as ROS2 CDR.

## Handles, revocation and quotas

A host-registered instance identity is the stable runtime object plus runtime generation. Guest module names never choose a principal. Each instance/backend has its own tables, while globally unique slot numbers prevent coincident numeric handles in different tables. Handles combine u32 slot and u32 generation; close increments generation and generation exhaustion retires the slot. Slots never transfer to another instance/backend. Version, operation and current revocation state are checked on every use.

Message-rate, byte-rate and resource-creation buckets are aggregated per instance/capability alias, including all its open handles. Closing/reopening cannot refill them. Resource-creation and actual-operation rate buckets independently use the explicit rate/burst ceilings so creation cannot consume the first publication's token. Byte buckets have capacity equal to the explicit one-second byte rate. Responses debit byte quota without charging a second operation token.

Authorization leases keep the instance/backend lock through each external side effect or guest delivery. Revocation waits for previously admitted operations of that backend, then invalidates handles and permanently prevents reopening its alias. Reusing an invalidated handle returns BAD_HANDLE (9); trying to open its revoked alias returns REVOKED (125). The alias-addressed Gateway protocol returns REVOKED for a revoked alias directly. The other ROS backend and other instances have independent locks. Runtime teardown removes trusted registry entries, marks both backends dead and retains state until earlier leases finish. Multi-module ancestry is checked separately by the runtime host layer; a policy alone does not authorize implicit delegation.

## Audit retention

Known configured capabilities record allow, deny, resource-create, resource-close, capability-revoke and gateway results/errors as JSON with Unix time, version, trusted module/instance identity, operation, resource, result and error code. Each alias has its own explicit retained-record/byte and rate/burst quotas. Retention overflow drops oldest records; quota suppression increments a visible counter. `deny-only` suppresses successful allow/gateway-result decisions while retaining lifecycle events. Trusted runtime code drains this sink; guests cannot read or set it. Unknown aliases, forged handles and rejected cross-module ancestry use an independent target-level denial sink configured by all four mandatory `audit-deny-max-rate-hz`, `audit-deny-burst`, `audit-deny-max-records`, `audit-deny-max-bytes` fields. These fields remain local to the Worker and are omitted from the backend binding manifest. The same rate/count/byte ceilings and suppression counters apply.

## Error codes

| Error | Numeric code | Meaning |
| --- | --- | --- |
| SUCCESS | 0 | Request admitted; adapter result still determines side effect success |
| UNKNOWN_MODULE | 2 | Binding refers to a missing loaded module |
| BAD_HANDLE | 9 | Forged/stale/closed/cross-instance/cross-version handle |
| RATE_LIMITED | 11 | Explicit rate, byte or open-resource count exhausted |
| PERMISSION_DENIED | 13 | Missing capability, wrong operation/type identity or unregistered caller |
| CONFLICT | 17 | Duplicate grant, target, binding or incompatible resource type |
| INVALID_ARGUMENT | 22 | Missing/inapplicable field, invalid name, value or payload structure |
| TOO_LARGE | 90 | Payload exceeds explicit limit or handle namespace exhausted |
| UNSUPPORTED | 95 | Unknown operation/type or absent safe adapter/option |
| DISCONNECTED | 107 | Gateway connection unavailable |
| TIMEOUT | 110 | Explicit bounded operation deadline expired |
| REVOKED | 125 | Revoked capability cannot reopen |

## Explicit ROS2 service QoS

All eight flat `qos.*` fields are mandatory for ROS2 Topic, TF, parameter-event subscriptions, ordinary Service call/advertise/respond, Parameter get/set/describe/list, and Lifecycle get-state/change-state. A missing field rejects startup; no system-default or best-available value supplies an omission. Action channels require five independently explicit eight-field profiles, including their generated goal/cancel/result services.

Ordinary generated service clients/servers receive the explicit profile through the native `create_client`/`create_service` API. The Gateway compares all eight effective QoS properties on the client's request publisher and response subscription before sending an authorized request, or on the server's request subscription and response publisher before retaining the server and enabling queued request delivery. Creating the native endpoint precedes this comparison because the getters need that endpoint. A middleware-adjusted profile that differs from the policy fails closed. This configuration does not grant any additional resource or operation. Request/response sizes, inflight counts, operation deadlines, rates and audit limits remain independent of QoS.

`service.respond` creates no DDS endpoint. Its eight mandatory fields declare the exact QoS contract of the independently authorized `service.advertise` server. If both grants have the same resource, generated type and type identity, different QoS values reject startup. A respond-only grant remains valid and never implies advertise permission; actually answering still requires an existing server and request owned by that session, equal configured QoS, and equal effective native server QoS.

Parameter get/set/describe/list require the same explicit profile even when their fixed `parameter-node` selects the Gateway's local fast path; that path creates no DDS client. Parameter declare/undeclare only operate on the explicitly named local Gateway node, have no DDS operation, and reject QoS fields as inapplicable. Parameter-event subscription keeps its independent eight-field publisher/subscriber profile. Time, Graph and Log schemas do not gain service QoS.

This section specifies the implemented validation and native endpoint checks. Actual service QoS/native interoperability qualification must refer to a rebuilt product and Gateway report; historical fixed-service-profile PASS does not verify the new Schema or all QoS combinations.

Topic, TF and parameter-event adapters also compare all eight effective RMW QoS properties returned by their native publishers/subscribers with the explicit policy. Action copies its five explicit profiles into raw rcl_action endpoint options; Jazzy exposes no public endpoint actual-QoS getter for all five Action channels. Native Action interoperability verifies the qualified test profiles, but does not establish arbitrary-profile effective-QoS equality.


## Typed parameter arrays

Array grants use the closed builtin identities above for `param.get` and `param.set`, plus ROS2 Gateway-local `param.declare`/`param.undeclare`. ROS1 admits Bool/int32/Float64/String arrays; ROS2 admits Bool/int64/Float64/String/byte arrays. Scalar, metadata and event grants confer no array read/write authority. ROS2 array event subscriptions require their separate projection grants above; remote ROS2 declare/undeclare remain unsupported.

`max-parameter-elements` is an additional mandatory, positive array field (1..1048576), set through the existing explicit capability-set CLI; it also applies to the separately granted ROS2 array event projections. Scalar, metadata and legacy event grants reject it. For array get/set/declare/undeclare, `max-parameter-bytes` limits the canonical value payload and must permit at least the four-byte count; event quotas instead cover the complete event envelope. Empty arrays are valid even though the explicit maximum is positive. A u32 little-endian element count is followed by exactly that many values: Bool and byte use one byte, int32 uses four, int64 and finite Float64 use eight, and each String has its own u32 byte length plus strict UTF-8 bytes. Bool must be0/1; byte admits0..255. Strings are NUL-free in this adapter; ROS1 additionally rejects XML1.0-forbidden C0 controls (except TAB/LF/CR) and U+FFFE/U+FFFF. Existing scalar String semantics are unchanged.

Every numeric array element must satisfy the two explicitly supplied inclusive `parameter-min-value`/`parameter-max-value` bounds. Int64 comparisons preserve the full signed range. Float64 elements must remain finite with validator=none or finite. Bool/String/byte arrays reject numeric bounds. Whole-byte quota is checked before decoding; an impossible/truncated/trailing/noncanonical payload returns22, explicit element/byte quota excess90, numeric bound excess13, and native type mismatch95. Decode and failed Guest reads preserve output storage. Counts are checked against actual available bytes before allocation; canonical field checks precede output assignment.

The native Gateway checks array count and canonical envelope size before copying into its bridge-owned representation. ROS1 requires an actual XMLRPC Array and exact element kinds; mixed arrays are refused. Empty XMLRPC arrays do not carry an element-kind descriptor, so their empty value can be represented under any selected admitted array grant; no type information is inferred from non-existent elements. ROS2 checks the exact generated ParameterType array kind even for an empty array, and remote writes retain the existing static-existing GetParameters/DescribeParameters precondition. This precondition is not CAS. Native middleware allocation before this bridge check remains part of the documented TCB.
