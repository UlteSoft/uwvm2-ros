// Exact ABI-v1 numeric imports. No managed object or pointer conversion.
// UInt64 lowers to Wasm i64; memory:Int lowers to i32.
// Calls grant no authority. All result/handle slots are eight LE bytes.
///|
// Return ABI version 1. This does not open a capability or authorize a resource.
pub fn raw_abi_version() -> Int =
  "uwvm_ros1_v1" "abi_version"

///|
// Open one explicitly granted alias in the caller Instance and selected backend. Write an opaque eight-byte little-endian handle on success.
pub fn raw_capability_open(memory : Int, alias_offset : UInt64, alias_length : UInt64, handle_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "capability_open"

///|
// Retire this handle generation. The handle may already be retired if later Gateway cleanup returns an error.
pub fn raw_capability_close(handle : UInt64) -> Int =
  "uwvm_ros1_v1" "capability_close"

///|
// Revoke the alias for this Instance/backend, invalidating all its open sibling handles. Reopening cannot restore the revoked grant.
pub fn raw_capability_revoke(handle : UInt64) -> Int =
  "uwvm_ros1_v1" "capability_revoke"

///|
// Publish one canonical generated message through a separately granted publisher alias.
pub fn raw_topic_publish(handle : UInt64, memory : Int, payload_offset : UInt64, payload_length : UInt64) -> Int =
  "uwvm_ros1_v1" "topic_publish"

///|
// Activate a separately granted subscription queue. Receive messages through event_poll on the guest execution thread.
pub fn raw_topic_subscribe(handle : UInt64) -> Int =
  "uwvm_ros1_v1" "topic_subscribe"

///|
// Poll a topic, service-request, incoming-goal or ROS2 parameter-event queue. Error 11 means no event; no native callback enters Wasm.
pub fn raw_event_poll(handle : UInt64, memory : Int, output_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "event_poll"

///|
// Bind separately granted event.observe and source owners. Correlation is nonzero only when an owned outgoing Action data kind is selected; metadata-only watches use zero and activate/consume no native payload. No aliases or rights are created.
pub fn raw_event_watch(observer_handle : UInt64, source_handle : UInt64, correlation : UInt64) -> Int =
  "uwvm_ros1_v1" "event_watch"

///|
// Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay.
pub fn raw_event_poll_v1(handle : UInt64, memory : Int, output_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "event_poll_v1"

///|
// Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation.
pub fn raw_event_ready(observer_handle : UInt64, memory : Int, output_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "event_ready"

///|
// Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout.
pub fn raw_service_submit(source_handle : UInt64, observer_handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, request_id_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "service_submit"

///|
// Call the explicitly selected service using its canonical request/response type and configured deadline. Do not automatically replay a timed-out call.
pub fn raw_service_call(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "service_call"

///|
// Activate the explicitly granted service server. Poll owned request IDs through event_poll.
pub fn raw_service_advertise(handle : UInt64) -> Int =
  "uwvm_ros1_v1" "service_advertise"

///|
// Respond to an owned live request using a separate response grant and u64 request-ID-prefixed canonical response.
pub fn raw_service_respond(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "service_respond"

///|
// Send the selected trusted Action goal codec and receive an opaque u64 Goal ID. Builtin Fibonacci uses one canonical i32 order. Generated bounded Actions require their registered goal codec; all bounds remain explicit.
pub fn raw_action_send_goal(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "action_send_goal"

///|
// Request cancellation of one owned Goal using its u64 ID and a separately authorized cancel capability.
pub fn raw_action_cancel_own_goal(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "action_cancel_own_goal"

///|
// Use the separately granted any-cancel operation. Goal ID zero is valid only for this independently authorized operation.
pub fn raw_action_cancel_any_goal(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "action_cancel_any_goal"

///|
// Receive ID8 plus the selected trusted Action feedback codec for an owned Goal. Builtin Fibonacci appends count32/int32 values; an empty queue returns 11.
pub fn raw_action_receive_feedback(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "action_receive_feedback"

///|
// Receive ID8/outcome8 plus the selected trusted terminal result codec. Builtin Fibonacci appends count32/int32 values. Successful terminal consumption retires ownership.
pub fn raw_action_receive_result(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "action_receive_result"

///|
// Activate the explicitly granted Action server with an empty command. Poll ID8 plus the selected trusted goal codec; builtin Fibonacci uses i32 order. Responses require another grant.
pub fn raw_action_advertise(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "action_advertise"

///|
// Send ID8/kind8 plus the selected trusted feedback/result codec. Builtin Fibonacci appends count32/int32 values. Advertiser and responder require the same explicit ROS2 QoS contract.
pub fn raw_action_respond(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "action_respond"

///|
// Request the separately configured source/target frame pair at u64 nanoseconds; zero selects the latest admitted transform.
pub fn raw_tf_lookup(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "tf_lookup"

///|
// Broadcast one canonical TransformStamped for the separately configured frame pair. Do not normalize its quaternion implicitly.
pub fn raw_tf_broadcast(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "tf_broadcast"

///|
// Read the value fixed by the capability type. An empty request does not select a different node, name or value kind.
pub fn raw_parameter_get(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "parameter_get"

///|
// Set a canonical value of the explicitly granted parameter type. Numeric and element bounds remain enforced by the host.
pub fn raw_parameter_set(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "parameter_set"

///|
// Delete the explicitly granted ROS1 parameter using an empty request; no implicit parent-name authority is added.
pub fn raw_parameter_delete(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "parameter_delete"

///|
// Query existence of the explicitly granted ROS1 parameter; decode one canonical Bool response.
pub fn raw_parameter_exists(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "parameter_exists"

///|
// List the separately authorized parameter scope; decode the bounded canonical String result.
pub fn raw_parameter_list(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "parameter_list"

///|
// Search the explicitly granted ROS1 resource with an empty request. Parent namespaces are included only with search-parent-namespaces=true, for the same final key and within allowed-name-prefix before every native query.
pub fn raw_parameter_search(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "parameter_search"

///|
// Inspect the configured topic filter with an empty request and decode sorted canonical String rows.
pub fn raw_graph_inspect_topic(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "graph_inspect_topic"

///|
// Inspect the configured node filter with an empty request and decode sorted canonical String rows.
pub fn raw_graph_inspect_node(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "graph_inspect_node"

///|
// Inspect the configured service filter with an empty request and decode sorted canonical String rows.
pub fn raw_graph_inspect_service(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "graph_inspect_service"

///|
// List topics within the explicit exact/prefix graph filter; decode sorted canonical String rows.
pub fn raw_graph_list_topics(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "graph_list_topics"

///|
// List nodes within the explicit graph filter; this is not unrestricted discovery authority.
pub fn raw_graph_list_nodes(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "graph_list_nodes"

///|
// List services within the explicit graph filter; decode sorted canonical String rows.
pub fn raw_graph_list_services(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "graph_list_services"

///|
// Read the separately authorized ROS clock domain as little-endian u64 nanoseconds.
pub fn raw_time_read_ros(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "time_read_ros"

///|
// Read the independently authorized ROS1 wall clock; another time grant does not imply this one.
pub fn raw_time_read_wall(handle : UInt64, memory : Int, request_offset : UInt64, request_length : UInt64, response_offset : UInt64, capacity : UInt64, length_out_offset : UInt64) -> Int =
  "uwvm_ros1_v1" "time_read_wall"

///|
// Write one canonical String to the separately granted ROS logger. This does not open a file or socket.
pub fn raw_log_write(handle : UInt64, memory : Int, payload_offset : UInt64, payload_length : UInt64) -> Int =
  "uwvm_ros1_v1" "log_write"

