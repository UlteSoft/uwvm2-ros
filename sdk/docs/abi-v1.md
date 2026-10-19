# Exact guest import reference

This reference is derived from the current host registration table and the reviewed
machine-readable [ABI schema](../abi/abi-v1.json). Every function below returns
an i32 error code, except `abi_version`, which returns the version value 1.
Names and widths are identical for wasm32 and memory64. Pointer conveniences
use memory zero; the raw functions retain their explicit memory index.

No function forwards to another backend. If your Runtime build omits one backend,
a guest importing that module cannot be satisfied by the other backend.

## Parameter conventions

| Name | Meaning |
| --- | --- |
| memory | i32 linear-memory index; not a native pointer. |
| handle | i64 opaque Instance/backend-owned capability. |
| alias_offset/alias_length | i64 offset and UTF-8 byte count of one explicit alias. |
| handle_out_offset | i64 offset to an eight-byte little-endian u64 slot. |
| payload_offset/payload_length | i64 offset and canonical input byte count. |
| request_offset/request_length | i64 offset and canonical request byte count; empty still has explicit zero length. |
| output_offset/response_offset | i64 destination offset in the selected memory. |
| capacity | i64 caller-supplied bounded output capacity. |
| length_out_offset | i64 offset to an eight-byte conditional result-length slot. |

Output lengths and error 90 follow [the conditional length contract](porting-and-errors.md).
The SDK does not automatically replay operations after a capacity refusal or timeout.

## uwvm_ros1_v1

38 functions are registered for ROS1.

### abi_version

```text
uwvm_ros1_v1.abi_version() -> i32
```

Return ABI version 1. This does not open a capability or authorize a resource.

### capability_open

```text
uwvm_ros1_v1.capability_open(memory:i32, alias_offset:i64, alias_length:i64, handle_out_offset:i64) -> i32
```

Open one explicitly granted alias in the caller Instance and selected backend. Write an opaque eight-byte little-endian handle on success.

### capability_close

```text
uwvm_ros1_v1.capability_close(handle:i64) -> i32
```

Retire this handle generation. The handle may already be retired if later Gateway cleanup returns an error.

### capability_revoke

```text
uwvm_ros1_v1.capability_revoke(handle:i64) -> i32
```

Revoke the alias for this Instance/backend, invalidating all its open sibling handles. Reopening cannot restore the revoked grant.

### topic_publish

```text
uwvm_ros1_v1.topic_publish(handle:i64, memory:i32, payload_offset:i64, payload_length:i64) -> i32
```

Publish one canonical generated message through a separately granted publisher alias.

### topic_subscribe

```text
uwvm_ros1_v1.topic_subscribe(handle:i64) -> i32
```

Activate a separately granted subscription queue. Receive messages through event_poll on the guest execution thread.

### event_poll

```text
uwvm_ros1_v1.event_poll(handle:i64, memory:i32, output_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Poll a topic, service-request, incoming-goal or ROS2 parameter-event queue. Error 11 means no event; no native callback enters Wasm.

### event_watch

```text
uwvm_ros1_v1.event_watch(observer_handle:i64, source_handle:i64, correlation:i64) -> i32
```

Bind separately granted event.observe and source owners. Correlation is nonzero only when an owned outgoing Action data kind is selected; metadata-only watches use zero and activate/consume no native payload. No aliases or rights are created.

### event_poll_v1

```text
uwvm_ros1_v1.event_poll_v1(handle:i64, memory:i32, output_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay.

### event_ready

```text
uwvm_ros1_v1.event_ready(observer_handle:i64, memory:i32, output_offset:i64) -> i32
```

Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation.

### service_submit

```text
uwvm_ros1_v1.service_submit(source_handle:i64, observer_handle:i64, memory:i32, request_offset:i64, request_length:i64, request_id_out_offset:i64) -> i32
```

Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout.

### service_call

```text
uwvm_ros1_v1.service_call(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Call the explicitly selected service using its canonical request/response type and configured deadline. Do not automatically replay a timed-out call.

### service_advertise

```text
uwvm_ros1_v1.service_advertise(handle:i64) -> i32
```

Activate the explicitly granted service server. Poll owned request IDs through event_poll.

### service_respond

```text
uwvm_ros1_v1.service_respond(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Respond to an owned live request using a separate response grant and u64 request-ID-prefixed canonical response.

### action_send_goal

```text
uwvm_ros1_v1.action_send_goal(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Send the selected trusted Action goal codec and receive an opaque u64 Goal ID. Builtin Fibonacci uses one canonical i32 order. Generated bounded Actions require their registered goal codec; all bounds remain explicit.

### action_cancel_own_goal

```text
uwvm_ros1_v1.action_cancel_own_goal(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Request cancellation of one owned Goal using its u64 ID and a separately authorized cancel capability.

### action_cancel_any_goal

```text
uwvm_ros1_v1.action_cancel_any_goal(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Use the separately granted any-cancel operation. Goal ID zero is valid only for this independently authorized operation.

### action_receive_feedback

```text
uwvm_ros1_v1.action_receive_feedback(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Receive ID8 plus the selected trusted Action feedback codec for an owned Goal. Builtin Fibonacci appends count32/int32 values; an empty queue returns 11.

### action_receive_result

```text
uwvm_ros1_v1.action_receive_result(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Receive ID8/outcome8 plus the selected trusted terminal result codec. Builtin Fibonacci appends count32/int32 values. Successful terminal consumption retires ownership.

### action_advertise

```text
uwvm_ros1_v1.action_advertise(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Activate the explicitly granted Action server with an empty command. Poll ID8 plus the selected trusted goal codec; builtin Fibonacci uses i32 order. Responses require another grant.

### action_respond

```text
uwvm_ros1_v1.action_respond(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Send ID8/kind8 plus the selected trusted feedback/result codec. Builtin Fibonacci appends count32/int32 values. Advertiser and responder require the same explicit ROS2 QoS contract.

### tf_lookup

```text
uwvm_ros1_v1.tf_lookup(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Request the separately configured source/target frame pair at u64 nanoseconds; zero selects the latest admitted transform.

### tf_broadcast

```text
uwvm_ros1_v1.tf_broadcast(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Broadcast one canonical TransformStamped for the separately configured frame pair. Do not normalize its quaternion implicitly.

### parameter_get

```text
uwvm_ros1_v1.parameter_get(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Read the value fixed by the capability type. An empty request does not select a different node, name or value kind.

### parameter_set

```text
uwvm_ros1_v1.parameter_set(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Set a canonical value of the explicitly granted parameter type. Numeric and element bounds remain enforced by the host.

### parameter_delete

```text
uwvm_ros1_v1.parameter_delete(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Delete the explicitly granted ROS1 parameter using an empty request; no implicit parent-name authority is added.

### parameter_exists

```text
uwvm_ros1_v1.parameter_exists(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Query existence of the explicitly granted ROS1 parameter; decode one canonical Bool response.

### parameter_list

```text
uwvm_ros1_v1.parameter_list(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

List the separately authorized parameter scope; decode the bounded canonical String result.

### parameter_search

```text
uwvm_ros1_v1.parameter_search(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Search the explicitly granted ROS1 resource with an empty request. Parent namespaces are included only with search-parent-namespaces=true, for the same final key and within allowed-name-prefix before every native query.

### graph_inspect_topic

```text
uwvm_ros1_v1.graph_inspect_topic(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Inspect the configured topic filter with an empty request and decode sorted canonical String rows.

### graph_inspect_node

```text
uwvm_ros1_v1.graph_inspect_node(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Inspect the configured node filter with an empty request and decode sorted canonical String rows.

### graph_inspect_service

```text
uwvm_ros1_v1.graph_inspect_service(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Inspect the configured service filter with an empty request and decode sorted canonical String rows.

### graph_list_topics

```text
uwvm_ros1_v1.graph_list_topics(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

List topics within the explicit exact/prefix graph filter; decode sorted canonical String rows.

### graph_list_nodes

```text
uwvm_ros1_v1.graph_list_nodes(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

List nodes within the explicit graph filter; this is not unrestricted discovery authority.

### graph_list_services

```text
uwvm_ros1_v1.graph_list_services(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

List services within the explicit graph filter; decode sorted canonical String rows.

### time_read_ros

```text
uwvm_ros1_v1.time_read_ros(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Read the separately authorized ROS clock domain as little-endian u64 nanoseconds.

### time_read_wall

```text
uwvm_ros1_v1.time_read_wall(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Read the independently authorized ROS1 wall clock; another time grant does not imply this one.

### log_write

```text
uwvm_ros1_v1.log_write(handle:i64, memory:i32, payload_offset:i64, payload_length:i64) -> i32
```

Write one canonical String to the separately granted ROS logger. This does not open a file or socket.

## uwvm_ros2_v1

43 functions are registered for ROS2.

### abi_version

```text
uwvm_ros2_v1.abi_version() -> i32
```

Return ABI version 1. This does not open a capability or authorize a resource.

### capability_open

```text
uwvm_ros2_v1.capability_open(memory:i32, alias_offset:i64, alias_length:i64, handle_out_offset:i64) -> i32
```

Open one explicitly granted alias in the caller Instance and selected backend. Write an opaque eight-byte little-endian handle on success.

### capability_close

```text
uwvm_ros2_v1.capability_close(handle:i64) -> i32
```

Retire this handle generation. The handle may already be retired if later Gateway cleanup returns an error.

### capability_revoke

```text
uwvm_ros2_v1.capability_revoke(handle:i64) -> i32
```

Revoke the alias for this Instance/backend, invalidating all its open sibling handles. Reopening cannot restore the revoked grant.

### topic_publish

```text
uwvm_ros2_v1.topic_publish(handle:i64, memory:i32, payload_offset:i64, payload_length:i64) -> i32
```

Publish one canonical generated message through a separately granted publisher alias.

### topic_subscribe

```text
uwvm_ros2_v1.topic_subscribe(handle:i64) -> i32
```

Activate a separately granted subscription queue. Receive messages through event_poll on the guest execution thread.

### event_poll

```text
uwvm_ros2_v1.event_poll(handle:i64, memory:i32, output_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Poll a topic, service-request, incoming-goal or ROS2 parameter-event queue. Error 11 means no event; no native callback enters Wasm.

### event_watch

```text
uwvm_ros2_v1.event_watch(observer_handle:i64, source_handle:i64, correlation:i64) -> i32
```

Bind separately granted event.observe and source owners. Correlation is nonzero only when an owned outgoing Action data kind is selected; metadata-only watches use zero and activate/consume no native payload. No aliases or rights are created.

### event_poll_v1

```text
uwvm_ros2_v1.event_poll_v1(handle:i64, memory:i32, output_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay.

### event_ready

```text
uwvm_ros2_v1.event_ready(observer_handle:i64, memory:i32, output_offset:i64) -> i32
```

Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation.

### service_submit

```text
uwvm_ros2_v1.service_submit(source_handle:i64, observer_handle:i64, memory:i32, request_offset:i64, request_length:i64, request_id_out_offset:i64) -> i32
```

Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout.

### service_call

```text
uwvm_ros2_v1.service_call(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Call the explicitly selected service using its canonical request/response type and configured deadline. Do not automatically replay a timed-out call.

### service_advertise

```text
uwvm_ros2_v1.service_advertise(handle:i64) -> i32
```

Activate the explicitly granted service server. Poll owned request IDs through event_poll.

### service_respond

```text
uwvm_ros2_v1.service_respond(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Respond to an owned live request using a separate response grant and u64 request-ID-prefixed canonical response.

### action_send_goal

```text
uwvm_ros2_v1.action_send_goal(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Send the selected trusted Action goal codec and receive an opaque u64 Goal ID. Builtin Fibonacci uses one canonical i32 order. Generated bounded Actions require their registered goal codec; all bounds remain explicit.

### action_cancel_own_goal

```text
uwvm_ros2_v1.action_cancel_own_goal(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Request cancellation of one owned Goal using its u64 ID and a separately authorized cancel capability.

### action_cancel_any_goal

```text
uwvm_ros2_v1.action_cancel_any_goal(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Use the separately granted any-cancel operation. Goal ID zero is valid only for this independently authorized operation.

### action_receive_feedback

```text
uwvm_ros2_v1.action_receive_feedback(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Receive ID8 plus the selected trusted Action feedback codec for an owned Goal. Builtin Fibonacci appends count32/int32 values; an empty queue returns 11.

### action_receive_result

```text
uwvm_ros2_v1.action_receive_result(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Receive ID8/outcome8 plus the selected trusted terminal result codec. Builtin Fibonacci appends count32/int32 values. Successful terminal consumption retires ownership.

### action_advertise

```text
uwvm_ros2_v1.action_advertise(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Activate the explicitly granted Action server with an empty command. Poll ID8 plus the selected trusted goal codec; builtin Fibonacci uses i32 order. Responses require another grant.

### action_respond

```text
uwvm_ros2_v1.action_respond(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Send ID8/kind8 plus the selected trusted feedback/result codec. Builtin Fibonacci appends count32/int32 values. Advertiser and responder require the same explicit ROS2 QoS contract.

### tf_lookup

```text
uwvm_ros2_v1.tf_lookup(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Request the separately configured source/target frame pair at u64 nanoseconds; zero selects the latest admitted transform.

### tf_broadcast

```text
uwvm_ros2_v1.tf_broadcast(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Broadcast one canonical TransformStamped for the separately configured frame pair. Do not normalize its quaternion implicitly.

### parameter_get

```text
uwvm_ros2_v1.parameter_get(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Read the value fixed by the capability type. An empty request does not select a different node, name or value kind.

### parameter_set

```text
uwvm_ros2_v1.parameter_set(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Set a canonical value of the explicitly granted parameter type. Numeric and element bounds remain enforced by the host.

### parameter_list

```text
uwvm_ros2_v1.parameter_list(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

List the separately authorized parameter scope; decode the bounded canonical String result.

### parameter_describe

```text
uwvm_ros2_v1.parameter_describe(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Read the separately authorized ROS2 parameter metadata as a canonical String.

### parameter_declare

```text
uwvm_ros2_v1.parameter_declare(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Declare a typed parameter on the explicitly selected local ROS2 Gateway node. Remote declaration is unsupported.

### parameter_undeclare

```text
uwvm_ros2_v1.parameter_undeclare(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Undeclare a typed parameter on the explicitly selected local ROS2 Gateway node. A missing remote operation is not emulated.

### graph_inspect_topic

```text
uwvm_ros2_v1.graph_inspect_topic(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Inspect the configured topic filter with an empty request and decode sorted canonical String rows.

### graph_inspect_node

```text
uwvm_ros2_v1.graph_inspect_node(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Inspect the configured node filter with an empty request and decode sorted canonical String rows.

### graph_inspect_service

```text
uwvm_ros2_v1.graph_inspect_service(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Inspect the configured service filter with an empty request and decode sorted canonical String rows.

### graph_inspect_action

```text
uwvm_ros2_v1.graph_inspect_action(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Inspect the separately configured ROS2 Action filter; ROS1 does not export this function.

### parameter_subscribe_events

```text
uwvm_ros2_v1.parameter_subscribe_events(handle:i64) -> i32
```

Activate a separate ROS2 parameter-event projection grant. This takes only one handle, not the seven RPC arguments.

### graph_list_topics

```text
uwvm_ros2_v1.graph_list_topics(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

List topics within the explicit exact/prefix graph filter; decode sorted canonical String rows.

### graph_list_nodes

```text
uwvm_ros2_v1.graph_list_nodes(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

List nodes within the explicit graph filter; this is not unrestricted discovery authority.

### graph_list_services

```text
uwvm_ros2_v1.graph_list_services(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

List services within the explicit graph filter; decode sorted canonical String rows.

### lifecycle_get_state

```text
uwvm_ros2_v1.lifecycle_get_state(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Read the separately selected ROS2 lifecycle node state as u8 state ID followed by canonical String label.

### lifecycle_change_state

```text
uwvm_ros2_v1.lifecycle_change_state(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Apply the single ROS2 transition fixed by policy using an empty request; the guest cannot override that transition.

### time_read_ros

```text
uwvm_ros2_v1.time_read_ros(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Read the separately authorized ROS clock domain as little-endian u64 nanoseconds.

### time_read_system

```text
uwvm_ros2_v1.time_read_system(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Read the independently authorized ROS2 system clock as u64 nanoseconds.

### time_read_steady

```text
uwvm_ros2_v1.time_read_steady(handle:i64, memory:i32, request_offset:i64, request_length:i64, response_offset:i64, capacity:i64, length_out_offset:i64) -> i32
```

Read the independently authorized ROS2 steady clock as u64 nanoseconds.

### log_write

```text
uwvm_ros2_v1.log_write(handle:i64, memory:i32, payload_offset:i64, payload_length:i64) -> i32
```

Write one canonical String to the separately granted ROS logger. This does not open a file or socket.

## Checking this reference

Run `python3 -B sdk/tools/abi_contract.py` from a source checkout. That checker
compares every module/name/type with `src/uwvm2/uwvm/imported/ros/host.h`; it
does not invoke ROS or prove a compiled language binding. Inspect actual Wasm
imports with the SDK binary checker after compilation, then run the guest in
the required real Runtime/Gateway integration environment.

## Additive unified event source contract

[Unified event v1](unified-events.md) specifies the separately granted
observer, complete64-byte envelope, seven kinds, Source/observer lease checks,
capacity-retaining poll, finite readiness and real bounded `service_submit`.
The new APIs are additive; local deferred tasks still advance synchronously
when explicitly scheduled, and do not gain native asynchronous authority.
No implicit alias, clock, thread, retry or stronger cancellation grant is added.
The six runnable unified-event lessons and source fixtures are prepared for the
unified serial test stage; dated historical34/39 receipts certify their original
revisions only.
