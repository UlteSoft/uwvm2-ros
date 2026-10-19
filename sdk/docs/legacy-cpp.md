# UWVM2 ROS guest SDK

The C headers `ros1/abi.h` and `ros2/abi.h` import `uwvm_ros1_v1` and
`uwvm_ros2_v1`. The C++20 wrappers in `abi.hpp` own a capability handle and close
it on destruction. `ros1/freestanding.hpp` and `ros2/freestanding.hpp` provide
C++11 ownership and the full operation API without a C++ standard library.
Neither SDK requires native ROS headers or libraries.

A capability alias is granted explicitly by the host command line. Opening an
alias obtains an opaque, instance-owned handle. Each import uses that handle;
the guest cannot select a ROS endpoint, provide a module identity, or transfer
its handle to another module or ROS backend.

When any ROS target is enabled, every WASI-enabled module must explicitly use
`--wasip1-global-noinherit-system-environment` or its module/group equivalent.
Directory/socket preopens, socket standard descriptors, native DL plugins and
weak-symbol plugins are rejected before guest execution. The network runners
set this explicit WASI isolation option for their valid examples.

Every import returns a numeric error. Zero means success; 11 means that an event
queue is currently empty, 13 means denied, 9 means invalid ownership or stale
handle, 22 means invalid memory or payload, 90 means a size limit was exceeded,
107 means disconnected, 110 means timed out, and 125 means a revoked alias.
Revoking retires its current handles, so later uses of those handles return 9;
reopening the revoked alias returns 125.

## Memory ABI

Offsets, lengths, handles, and output lengths use `i64` in both wasm32 and
memory64. The memory index uses `i32`. Returned handles and lengths occupy eight
little-endian bytes. The host copies input and output during the import call.
The convenience C and C++ wrappers use memory zero; direct imports accept an
explicit memory index for multi-memory modules.

The request/response imports use this signature:

```c
uint32_t operation(uint64_t handle, uint32_t memory,
    uint64_t request_offset, uint64_t request_length,
    uint64_t response_offset, uint64_t response_capacity,
    uint64_t response_length_offset);
```

Topic publish and log imports take only the handle, memory, input offset and
input length. Subscription and service-advertisement imports take a handle.
`event_poll` takes the handle, memory, output offset, capacity and output-length
offset. Polling never invokes a guest callback from a native ROS thread.

## Canonical values

`common/codec.hpp` encodes and decodes values without ROS dependencies:

- String: a little-endian `u32` UTF-8 byte length followed by the bytes.
- Bool: one byte, zero or one.
- Scalar parameters: `parameter_bool`, `parameter_int32` (ROS1 only),
  `parameter_int64` (ROS2 only) and `parameter_float64` encode exactly 1/4/8/8
  little-endian bytes. Matching `decode_parameter_*` helpers preserve the output
  on failure. Float64 rejects NaN/infinity; numeric capabilities require explicit
  inclusive `parameter-min-value` and `parameter-max-value`. Existing String
  parameters and imports retain their format. A scalar get/set grant never grants
  parameter-event subscription.
- Twist: six little-endian IEEE754 doubles, linear XYZ followed by angular XYZ.
- Image: `u32` sequence, seconds and nanoseconds; a String frame ID; `u32`
  height and width; a String encoding; Bool endianness; `u32` step and data
  length; then image bytes. ROS2 has no Header sequence field.
- Transform: `u32` sequence, seconds and nanoseconds; String parent and child
  frame IDs; translation XYZ and quaternion XYZW as seven doubles.
  The explicitly authorized `/tf_static` lookup cache keeps one latest admitted
  sample and accepts changed values at the same timestamp. Exact duplicates may
  be ignored; frame/bounds, message/queue bytes and rate budgets still apply.
  Dynamic `/tf` retains its existing duplicate-stamp and history semantics.
- Trigger: an empty request. SetBool: a Bool request. Both responses contain a
  Bool followed by a String.
- Service-server event: a nonzero little-endian `u64` request ID followed by the
  request value. A reply contains that ID followed by the response value. The
  gateway assigns the request ID and checks its pending request ownership.
- Time: an empty request and a little-endian `u64` nanosecond response. A TF
  lookup request also carries a `u64` time; zero selects the latest transform.
- Legacy String parameter event: `u32` seconds and nanoseconds, one-byte kind (1 new, 2
  changed, 3 deleted), then String node, parameter name and value. Deleted
  values are empty. The capability selects one exact ROS2 node and parameter.
- Typed ROS2 parameter event: an independently granted
  `builtin/param-event-<suffix>-v1` capability selects one exact node/name/type,
  with identity `builtin:param-event-<suffix>-v1`. The suffixes are `bool`,
  `int64`, `float64`, `string`, `byte-array`, `bool-array`, `int64-array`,
  `float64-array` and `string-array`. `typed_parameter_event` and
  `encode_typed_parameter_event` / `decode_typed_parameter_event` use `u32`
  seconds, `u32` nanoseconds, one-byte event kind, version byte `2` and the
  selected tag listed below. Three `u32` byte lengths precede node, name and raw
  selected payload: size is `23 + node.size() + name.size() + payload.size()`.
  The outer payload length is distinct from a String's inner byte length or an
  array's count. Empty String/array payloads contain a four-byte zero length/count;
  deletion retains its tag but has an empty outer payload. Existing tag1..3 and
  legacy String event bytes remain unchanged. No ROS1 or int32 typed event is
  admitted, and a scalar/metadata/legacy event grant never grants a typed event
  subscription.
- Fibonacci Action goal: an `i32` order; sending returns a gateway-assigned
  nonzero `u64` goal ID. Feedback contains that ID, a `u32` count and `i32`
  sequence values. A result adds one-byte kind (2 succeeded, 3 aborted, 4
  canceled) between ID and count. Cancel requests contain a goal ID and return
  Bool. Advertising receives events containing a goal ID and `i32` order;
  responding uses the result shape, with kind 1 for feedback. Every Action
  operation requires its own explicitly granted capability.
- Lifecycle GetState: an empty request and a one-byte state ID followed by
  String label. ChangeState uses an empty request; its one allowed transition
  comes from the host policy, and the response is Bool.

| Event tag | `parameter_event_value_tag` | Value member | Raw payload |
| --- | --- | --- | --- |
| 1 | `boolean` | `bool_value` | one 0/1 byte |
| 2 | `integer` | `integer_value` | signed eight-byte int64 |
| 3 | `float64` | `double_value` | finite eight-byte IEEE754 |
| 4 | `string` | `string_value` | u32 UTF-8 byte length + bytes; NUL permitted |
| 5 | `byte_array` | `byte_array_value` | u32 count + unsigned bytes |
| 6 | `boolean_array` | `bool_array_value` | u32 count + 0/1 bytes |
| 7 | `integer_array` | `integer_array_value` | u32 count + int64 elements |
| 8 | `float64_array` | `double_array_value` | u32 count + finite Float64 elements |
| 9 | `string_array` | `string_array_value` | u32 count + u32-length UTF-8, NUL-free elements |

Scalar String's NUL allowance belongs to the canonical SDK/IPC format. In a
direct native Jazzy `rmw_fastrtps_cpp` `rcl_interfaces/msg/ParameterEvent`
publication, ordinary UTF-8 scalar String succeeds, while embedded NUL causes
FastCDR `BadParamException` (`The string contains null characters`) before
delivery. The SDK allowance does not establish embedded-NUL DDS interoperability
for that field; this observation does not establish other RMW behavior.

The same tested RMW takes a native StringArray sender's element `a\0b` as `a`.
The Gateway cannot check the original sending bytes after this truncation;
native/Wasm fixtures omit the original-NUL rejection case. SDK/IPC canonical
StringArray elements remain strictly NUL-free and reject NUL before delivery.
This observation does not establish other RMW behavior.

Encoding and decoding preserve their output on failure. Array event grants
require explicit positive `max-parameter-elements` (1..1048576); scalar and
legacy event grants reject it. Every numeric scalar/array element requires
inclusive `parameter-min-value` and `parameter-max-value`, with exact signed
int64 comparisons. Float64 remains finite with `validator=none` or `finite`;
nonnumeric projections reject numeric bounds. `max-parameter-bytes` limits the
complete event envelope and may deliberately be smaller than it.
`max-event-fields` counts total native Parameter entries across the three event
arrays before exact selection.

The Gateway validates a borrowed prior native value's type, element quota,
canonical hard payload byte limit (4193786), UTF-8/finite validity and applicable
bounds before encoding a deletion, without copying old arrays. The delivery byte
quota covers the actual empty-payload deletion envelope, so a valid prior value
may exceed that quota. SDK/Guest decoding cannot prove those prior native
contents from empty wire. The expanded native fixtures distinguish automatic
rclcpp Node new/changed events from controlled typed deleted-event publications;
typed native NOT_SET deletion remains ignored.

The separate ROS2 deletion notification projection is
`builtin/param-event-deletion-v1`, with identity
`builtin:param-event-deletion-v1`. Its SDK tag is
`parameter_event_value_tag::deletion` (0); existing tags 1–9 retain their values.
Only kind 3 with an empty payload is accepted. The configured node/name are
exact, and the notification carries no previous type, value or numeric-bound
attestation. It requires its own explicit `param.subscribe-events` capability
and grants no parameter mutation authority. The existing nine typed projections
continue to ignore native NOT_SET deletion.

The typed event encoder validates the complete envelope and selected values
before allocating its final buffer, then appends fixed-width little-endian
fields and payload directly. Refusal leaves the output unchanged. Deleted
events carry no prior-value payload, so unselected prior fields are ignored.
The permanent allocation and wire regression tests are documented in
[`tests/ros_sdk_event_encode`](../../tests/ros_sdk_event_encode/README.md).

The host and gateway enforce payload validity, type identity, permissions and
quotas independently. SDK decoding is a convenience and does not grant a
capability.

```cpp
#include "ros1/abi.hpp"
#include "common/codec.hpp"

uwvm_ros1::capability publisher;
auto error = publisher.open("pub");
if (!error) {
    auto message = uwvm_ros::string("hello");
    error = publisher.publish(message);
}
```

The C ABI exposes each operation by name, and the C++ wrapper uses matching
methods. ROS1 parameter operations are get, set, delete, exists, search and
list; ROS2 additionally exposes describe, gateway-node declaration and
undeclaration, and exact parameter-event subscription. Graph methods return
bounded String snapshots of explicitly selected names. ROS2 lifecycle methods
and ROS1/ROS2 TF and Fibonacci Action methods use the request/response signature.

The runnable WAT and C samples are in `../examples/ros1`, `../examples/ros2` and
`../../examples/ros_dual`. See the [ROS1 CLI](../../documents/command-line/ros1.md) and [ROS2 CLI](../../documents/command-line/ros2.md) for explicit policy
configuration and the current tested adapter support.

Build the C and freestanding C++ examples with a WebAssembly-capable Clang:

```sh
python3 tests/ros/build_sdk_guests.py --clang /path/to/clang
```

This produces `talker_c.wasm` and `talker_cpp.wasm` for each ROS version. Both
publish canonical String `hello` through the same explicit `pub` capability.
