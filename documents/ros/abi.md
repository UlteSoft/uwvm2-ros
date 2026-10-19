# Guest ABI v1

The imports `uwvm_ros1_v1` and `uwvm_ros2_v1` are distinct modules. Every function below exists independently in each compiled backend; no function forwards to the other backend. The C SDK is in `sdk/ros1/abi.h` and `sdk/ros2/abi.h`; C++ RAII helpers are in adjacent `abi.hpp` files. Guest code does not link roscpp or rclcpp.

All calls return an i32 error code except `abi_version`, which returns 1. Memory index is i32; every memory offset, length, handle and output capacity is i64 in both wasm32 and memory64. Memory reads and writes use existing uwvm2 controlled copy APIs. Unaligned buffers are supported, memory indices are explicit, integer wrap and out-of-bounds ranges fail before access, and no Guest pointer survives the call or memory growth.

| Function | Wasm parameters |
| --- | --- |
| `abi_version` | none |
| `capability_open` | memory:i32, alias-offset:i64, alias-length:i64, handle-out-offset:i64 |
| `capability_close` | handle:i64 |
| `capability_revoke` | handle:i64 |
| `topic_publish` | handle:i64, memory:i32, payload-offset:i64, payload-length:i64 |
| `topic_subscribe` | handle:i64 |
| `event_poll` | handle:i64, memory:i32, output-offset:i64, capacity:i64, length-out-offset:i64 |
| `service_call` | handle:i64, memory:i32, request-offset:i64, request-length:i64, response-offset:i64, capacity:i64, length-out-offset:i64 |

Handles and returned lengths occupy eight little-endian bytes. A handle is `(generation << 32) | slot`, scoped to trusted Instance identity and backend. Aliases identify an explicitly configured grant; they cannot replace the caller identity or choose a resource not in that grant. Closing invalidates the generation. Revocation invalidates every handle to that alias for that Instance/backend and cannot be undone by Guest reopening.

| Error | Meaning |
| --- | --- |
| 0 | Success |
| 9 | Forged, stale, wrong Instance or wrong ROS-version handle |
| 11 | No queued event or explicit quota exhausted |
| 13 | No authority, invalid caller ancestry or policy/type binding mismatch |
| 22 | Invalid argument, canonical structure or memory range |
| 90 | Payload or output capacity limit exceeded |
| 95 | Unsupported operation, type or backend |
| 107 | Gateway disconnected |
| 110 | Timeout; external admitted effects may not be reversible |
| 125 | Revoked capability |

`event_poll` delivers the next validated topic message and returns 11 when empty. It consumes only bounded Gateway queue data; the Guest polls on its own execution thread. Closing/revoking a subscription deactivates the queue. The requested buffer capacity is checked before delivery, and the required length is written if a validated message exceeds output capacity.

Messages use a canonical typed wire representation, decoded into official ROS generated objects in each Gateway. It is not ROS2 CDR and must not be confused with arbitrary ROS1 serialized bytes. String is u32 byte length followed by exactly that UTF-8 text; Bool is one byte 0 or 1; Twist is six little-endian IEEE-754 f64 fields, all finite, with optional explicitly configured linear/angular magnitude bounds. Trigger request is empty; SetBool request is one canonical boolean. Their responses are one boolean followed by a length-prefixed UTF-8 message. The Image adapter describes its checked bounded fields in the capability documentation.

A service call is bounded by its explicit timeout, inflight limit and request/response lengths. No native object is exposed and no asynchronous ROS callback enters Wasm. Service and Action servers deliver validated request/goal events through polling. Responses require a separate explicit grant on the same resource/type and an owned live request/goal ID. No native callback enters Wasm.

## Additional imports

All RPC-form imports below take `handle:i64, memory:i32, request-offset:i64, request-length:i64, response-offset:i64, capacity:i64, length-out-offset:i64`. Empty requests still require explicit zero lengths. Functions that have no response write a zero output length. `service_advertise` takes one handle; `log_write` takes handle, memory, request offset and request length.

| Import | ROS versions | Form |
| --- | --- | --- |
| `service_advertise` | 1, 2 | handle |
| `service_respond` | 1, 2 | RPC |
| `action_send_goal` | 1, 2 | RPC |
| `action_cancel_own_goal` | 1, 2 | RPC |
| `action_cancel_any_goal` | 1, 2 | RPC |
| `action_receive_feedback` | 1, 2 | RPC |
| `action_receive_result` | 1, 2 | RPC |
| `action_advertise` | 1, 2 | RPC |
| `action_respond` | 1, 2 | RPC |
| `tf_lookup` | 1, 2 | RPC |
| `tf_broadcast` | 1, 2 | RPC |
| `parameter_get` | 1, 2 | RPC |
| `parameter_set` | 1, 2 | RPC |
| `parameter_delete` | 1 | RPC |
| `parameter_exists` | 1 | RPC |
| `parameter_list` | 1, 2 | RPC |
| `parameter_search` | 1 | RPC |
| `graph_inspect_topic` | 1, 2 | RPC |
| `graph_inspect_node` | 1, 2 | RPC |
| `graph_inspect_service` | 1, 2 | RPC |
| `graph_list_topics` | 1, 2 | RPC |
| `graph_list_nodes` | 1, 2 | RPC |
| `graph_list_services` | 1, 2 | RPC |
| `time_read_ros` | 1, 2 | RPC |
| `time_read_wall` | 1 | RPC |
| `log_write` | 1, 2 | write |
| `parameter_describe` | 2 | RPC |
| `parameter_declare` | 2 | RPC |
| `parameter_undeclare` | 2 | RPC |
| `graph_inspect_action` | 2 | RPC |
| `parameter_subscribe_events` | 2 | RPC |
| `lifecycle_get_state` | 2 | RPC |
| `lifecycle_change_state` | 2 | RPC |
| `time_read_system` | 2 | RPC |
| `time_read_steady` | 2 | RPC |

## Canonical resource layouts

All integers and doubles are little endian. Strings below are u32 length plus exact UTF-8 bytes, with no terminator. Unknown generated types are rejected at policy validation; the network adapters never deserialize Guest-provided arbitrary CDR or XML-RPC objects.

| Resource/operation | Request | Response/event |
| --- | --- | --- |
| Service server | advertise: empty; respond: u64 request ID + generated response | event: u64 request ID + generated request |
| ROS1 param get/delete/exists/search/list | empty; set: selected scalar | get: selected scalar; search/list: String; exists: u8 boolean; writes: empty |
| ROS2 param get/describe/list/undeclare | empty; set/declare: selected scalar | get: selected scalar; describe/list: String; writes: empty |
| ROS2 legacy String param events | subscribe: empty | u32 seconds, u32 nanoseconds, u8 kind (new1/change2/delete3), String node, String name, String value (empty on delete) |
| ROS2 typed param events | subscribe: empty | u32 seconds/nanoseconds, u8 kind, u8 version2, u8 selected value tag, then three u32-length fields: node/name/raw selected payload |
| Graph list/inspect | empty | String containing sorted newline rows; optional type follows a tab; exact/prefix filters apply before serialization |
| Time | empty | u64 nanoseconds in the separately authorized clock domain |
| Log | String | empty |
| TF lookup | u64 timestamp nanoseconds, zero means latest | TransformStamped below |
| TF broadcast | TransformStamped | empty |
| Lifecycle get/change | empty; configured transition cannot be overridden by Guest | get: u8 state ID + String label; change: u8 success |
| Fibonacci send Goal | i32 order, 0..46 and configured maximum | u64 opaque Goal ID |
| Action own/any cancel | u64 Goal ID; zero is allowed only with separately authorized any-cancel | u8 accepted |
| Action feedback/result | u64 owned Goal ID | feedback: u64 Goal ID + u32 count + i32 values; result: u64 Goal ID + u8 outcome + u32 count + i32 values |
| Action server | advertise: empty; respond: u64 Goal ID + u8 kind + u32 count + i32 values | Goal event: u64 Goal ID + i32 order |

TransformStamped is u32 Header sequence, u32 seconds, u32 nanoseconds, String parent frame, String child frame, followed by seven finite f64 values: translation xyz and quaternion xyzw. ROS2 sequence is zero. Frames must exactly equal the separately configured target/source pair. There is no implicit quaternion normalization. ROS1's native roscpp transport may generate its own Header sequence; stamp/frame and all transform fields are preserved.

The `/tf_static` lookup cache retains one latest admissible transform for the explicitly selected frame pair. A changed sample replaces it even when the timestamp is unchanged; an exactly identical sample may be ignored. Frame permissions, transform bounds, message/queue byte limits and rate budgets apply before replacement, and retained byte accounting follows the current sample. Dynamic `/tf` retains its existing duplicate-timestamp and bounded-history overflow behavior.

Fibonacci sequence envelopes contain at most48 nonnegative i32 values and obey separately configured byte, rate and queue limits. Server response kind1 emits feedback,2 succeeds,3 aborts,4 reports cancellation only for a goal actually cancelling. ROS1 native cancellation completes and retires the goal directly; a later Guest cancellation response is stale. Goal IDs are process-randomized, bound to the actual Gateway session, resource and verified type, and are never reallocated during that Gateway process. Foreign feedback/result and own-cancel requests fail; any-cancel has an independent explicit grant. Terminal consumption, revocation, deadline or disconnect retires ownership.

ROS2 remote declaration/undeclaration remains UNSUPPORTED by this adapter; the corresponding imports act on the explicitly named Gateway node. Standard Jazzy parameter services have no remote declaration operation. Standard remote deletion does exist through SetParameters/NOT_SET, exposed by AsyncParametersClient `delete_parameters` for dynamically typed parameters, but provides no atomic conditional deletion based on the prior kind/value required by this typed adapter. Parameter get/set and ROS2 local declare/undeclare select an explicit String, Bool, signed int32 (ROS1), signed int64 (ROS2), or float64 adapter. Bool is exactly one byte0/1; integers are exactly4/8 little-endian bytes; float64 is exactly8 little-endian IEEE754 bytes and must be finite. Numeric capabilities require inclusive `parameter-min-value` and `parameter-max-value`; integer bounds are parsed without conversion to floating point. Invalid payload22, byte quota90, value outside bounds13, or native kind mismatch95 leaves the Guest output untouched. Metadata operations retain their String adapter. Read-only describe returns a bounded native description; it does not imply write or declaration authority.

Remote ROS2 non-String scalar set requires an existing statically typed parameter. ROS1 non-String scalar set permits creation of a missing parameter and checks the native kind before replacing an existing value. It checks the exact native kind with GetParameters and rejects dynamic typing using DescribeParameters before SetParameters, sharing one operation deadline. These checks do not grant the Guest read results; they are internal preconditions. No atomicity across those three remote services is claimed. Gateway-local operations share a deadline-bounded mutex, so typed undeclare checks the existing native kind and bounds before deleting under the same lock. A native read-only refusal (`ParameterImmutableException`) returns13 with an empty output and retains the authenticated session and sibling resources. The existing static-parameter refusal (`InvalidParameterTypeException`) remains95; dynamically typed parameters still permit normal local undeclaration. Explicit array adapters use the separate canonical payload and grants documented below.

Typed parameter events use the existing `parameter_subscribe_events` and `event_poll` imports, with no change to Host ABI v1 or to the legacy String grant/wire. ROS2 `param.subscribe-events` admits nine value projections and a separately granted deletion-only projection. Each suffix below selects type `builtin/param-event-<suffix>-v1` and identity `builtin:param-event-<suffix>-v1`; scalar get/set, metadata and legacy String event grants do not imply these subscriptions. ROS1 and int32 typed events remain unsupported.

| Tag | Projection suffix | Nondeleted raw payload |
| --- | --- | --- |
| 0 | `deletion` | only kind3; payload length zero |
| 1 | `bool` | one byte 0/1 |
| 2 | `int64` | eight signed little-endian bytes |
| 3 | `float64` | eight finite IEEE754 bytes |
| 4 | `string` | u32 byte length + strict UTF-8 bytes; NUL is permitted |
| 5 | `byte-array` | u32 count + unsigned bytes |
| 6 | `bool-array` | u32 count + 0/1 bytes |
| 7 | `int64-array` | u32 count + signed eight-byte elements |
| 8 | `float64-array` | u32 count + finite eight-byte elements |
| 9 | `string-array` | u32 count + u32-length strict UTF-8, NUL-free elements |

Scalar String's NUL allowance describes the canonical SDK/IPC format. A direct native `rcl_interfaces/msg/ParameterEvent` publication using Jazzy `rmw_fastrtps_cpp` accepted an ordinary UTF-8 scalar String but rejected an embedded NUL with FastCDR `BadParamException` (`The string contains null characters`) before delivery. This observation applies to that selected native String field and tested RMW; it establishes no embedded-NUL network interoperability or behavior for other RMW implementations.

Under the same tested RMW, a native StringArray sender's element `a\0b` was taken as `a`. The Gateway therefore does not receive the original sending bytes and cannot establish rejection of that original embedded NUL. Canonical SDK/IPC StringArray elements remain strictly NUL-free and reject such bytes before bridge delivery. Native/Wasm fixtures omit this network NUL rejection case; this observation does not establish other RMW behavior.

The typed wire starts with u32 seconds (0..2147483647), u32 nanoseconds (<1000000000), u8 event kind1/2/3 (new/changed/deleted), version byte2 and the selected value tag. Exactly three u32 byte lengths precede the canonical absolute node, dotted parameter name and raw selected payload. Fixed overhead is23 bytes, so size is `23 + node.size() + name.size() + payload.size()`. The outer payload length is separate from a String's inner length or an array's element count. Empty String/array payloads contain a four-byte zero length/count. Deletion retains its tag with outer payload length zero. Existing tag1..3 bytes are unchanged.

Array projections require explicit positive `max-parameter-elements` (1..1048576); scalar and legacy event grants reject that field. Numeric scalar values and every int64/Float64 array element require both inclusive `parameter-min-value` and `parameter-max-value` bounds. Int64 comparisons retain exact signed precision, and all Float64 values must be finite even with `validator=none`. Bool, String and byte projections reject numeric bounds.

`max-parameter-bytes` bounds the entire delivered event envelope and may deliberately be smaller than it, causing that native delivery to be dropped or validation to return90. This differs from ordinary parameter get/set/declare/undeclare value quotas. `max-event-fields` bounds the total native Parameter entries across new/changed/deleted arrays before exact node/name/type selection. Before copying a selected new/changed value, the Gateway checks its count, canonical size, contents and numeric bounds against the remaining envelope budget. Before producing a typed deletion, it checks the borrowed prior native value's exact kind, element quota, canonical hard size limit (4193786 bytes), UTF-8/Bool/finite validity and applicable bounds without copying old arrays. The delivery byte quota applies to the actual empty-payload deletion envelope, so an otherwise valid prior value may exceed that delivery quota. Typed native NOT_SET deletion is ignored; legacy String NOT_SET deletion remains accepted. Worker validation checks complete typed wire, node/name/tag/quota and nondeleted bounds before Guest output copying; empty deleted wire cannot re-prove the prior native contents.

The C++ SDK exposes `uwvm_ros::typed_parameter_event`, `encode_typed_parameter_event(event, bytes&)` and `decode_typed_parameter_event(span, event&)`, plus `encode`/`decode` overloads. `parameter_event_value_tag` names are `boolean`, `integer`, `float64`, `string`, `byte_array`, `boolean_array`, `integer_array`, `float64_array` and `string_array`. Value members are `bool_value`, `integer_value`, `double_value`, `string_value`, `byte_array_value`, `bool_array_value`, `integer_array_value`, `double_array_value` and `string_array_value`. Deletion ignores unused value members. Encoding and decoding preserve outputs on failure. The Gateway additionally verifies the generated native ParameterEvent RIHS independently of the selected builtin projection identity.

The separate `builtin/param-event-deletion-v1` projection uses SDK tag `deletion=0`. It accepts only kind3 with payload length zero, an exact configured node/name, and native `PARAMETER_NOT_SET` in `deleted_parameters`. Tag0 in new/changed events, any nonempty payload, unknown tags, and concrete typed deletion do not satisfy this projection. It authorizes only a deletion notification: the prior type/value are unknown and neither numeric bounds nor a cached prior value are attested. Numeric bounds, `finite` and `max-parameter-elements` are inapplicable. The complete envelope byte quota, timestamp, native field count, explicit QoS, audit, rate and queue checks still apply. Existing tag1..9 projections continue to reject native NOT_SET deletion. Standard SDK remote deletion may emit NOT_SET without any prior concrete value; direct native `Node::undeclare_parameter` does not itself guarantee a ParameterEvent. Gateway-local mutation keeps its existing parameter-event publisher setting.


## Typed parameter array payloads

Array operations retain the existing parameter imports and version1 function ABI. Each explicit array capability fixes its backend, operation, resource, closed builtin identity and positive `max-parameter-elements`; existing scalar grants do not admit arrays. The payload is a u32 little-endian count followed by canonical elements, with no trailing bytes. Bool/byte widths are1 (Bool0/1); ROS1 int32 width4 and ROS2 int64 width8 use two's-complement little-endian; finite Float64 width8 preserves all bits including signed zero. Each String element is u32 byte length followed by strict UTF-8, NUL-free bytes. ROS1 String arrays additionally require XML1.0-valid controls/codepoints. Count0 encodes as four zero bytes. Whole envelope bytes obey max-parameter-bytes; every numeric element obeys explicitly supplied inclusive bounds.

ROS1 admits Bool/int32/Float64/String, ROS2 admits Bool/int64/Float64/String/byte. Array get/set use independently granted parameter capabilities. ROS2 array declare/undeclare selects only the explicitly configured local Gateway node. The SDK offers explicit `encode_parameter_{bool,int32,int64,float64,string,byte}_array(values, bytes&)` and `decode_parameter_{bool,int32,int64,float64,string,byte}_array(span, values&)` functions; Bool and byte are separate helpers even though both use vectors of unsigned bytes. Failed decode/encode keeps caller output unchanged.

Malformed shape, invalid Bool, nonfinite Float64 or invalid String returns22. Explicit byte/element quota excess returns90, per-element numeric bounds excess13, native kind mismatch95, missing2, denied13 and stale/revoked handle9/125 retain existing meanings. The Worker validates before copying returned bytes into Guest memory. Existing scalar payloads and scalar/legacy ParameterEvent formats are unchanged; ROS2 array event subscriptions require the separate typed event grants above.
