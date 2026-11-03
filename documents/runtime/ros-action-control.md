# Asynchronous ROS2 Action control

Six additive `uwvm_ros2_v1` imports preserve the existing exact source grants.
They use `(handle:i64, memory_index:i32, input_offset:i64, input_bytes:i64,
output_offset:i64, output_capacity:i64, required_bytes_offset:i64) -> errno:i32`.
The final pointer addresses an eight-byte little-endian length. Offsets and
lengths remain i64 for both Wasm32 and Wasm64. ROS1 and legacy Action imports
are unchanged.

| Import | IPC operation | Required existing grant | Request / successful reply |
| --- | --- | --- | --- |
| `action_server_is_ready` | `action.server-is-ready` | `action.send-goal` | empty / one boolean byte |
| `action_send_begin` | `action.send-begin` | `action.send-goal` | complete canonical Goal / nonzero u64 pending ID |
| `action_goal_response` | `action.goal-response` | `action.send-goal` | pending ID / response frame |
| `action_goal_status` | `action.goal-status` | `action.send-goal` | pending ID / status frame |
| `action_cancel_begin_own` | `action.cancel-begin-own` | `action.cancel-own-goal`, scope `own` | owned pending ID / same ID |
| `action_cancel_response` | `action.cancel-response` | `action.cancel-own-goal`, scope `own` | owned pending ID / complete cancel-response frame |

Control integers are little-endian. Response frames begin with version byte 1
and the requested u64 ID. A rejected goal response adds a zero byte (10 bytes
total). An accepted response adds 1, a nonzero 16-byte UUID, signed seconds i32,
nanoseconds u32 below one billion, and status u8 in 0..6 (35 bytes total).
A status frame omits the accepted flag (34 bytes). A cancel response adds the
original signed return-code byte, u32 count, and every GoalInfo's UUID and
seconds/nanoseconds (14 + 24 * count bytes). Count must fit `max-active-goals`.

Pending is errno 11; deadline expiry is 110. A pending ID is not an accepted
goal or its UUID. A cancellation response is not a terminal result, callback
quiescence, or quota release. Ownership and goal lifecycle remain enforced by
the native adapter under the checked IPC session. Each call debits the original
operation and byte quotas; the response also debits bytes. Revocation, trusted
instance/generation leases, exact aliases and memory bounds remain in force.

The VM validates the entire native control frame before delivery. Output and
length storage must not overlap. If a validated response exceeds the output
capacity, errno 90 writes only the required length. The native operation may
already have consumed a reply or caused effects: **never automatically replay**
the call. Callers must provide their complete admitted response bound. The
opt-in buffer journal records the actual operation, guest copies, and checked
IPC receipt within its existing source profile; it grants no authority.

These transport primitives require a matching SDK gateway and language bridge.
Their presence does not establish production enrollment, official simulation,
or completion of an asynchronous Action client implementation.
