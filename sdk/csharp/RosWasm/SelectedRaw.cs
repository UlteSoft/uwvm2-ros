using System.Runtime.InteropServices;
namespace Uwvm.RosWasm;
// Numeric ABI surface: offsets are guest linear-memory offsets, never managed object headers.
// Only ONE backend is compiled into this assembly and the paired C object.
#if UWVM_ROS1
internal static class SelectedRaw {
    /// <summary>Return ABI version 1. This does not open a capability or authorize a resource.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_abi_version",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint abi_version();
    /// <summary>Open one explicitly granted alias in the caller Instance and selected backend. Write an opaque eight-byte little-endian handle on success.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_capability_open",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint capability_open(uint memory, ulong alias_offset, ulong alias_length, ulong handle_out_offset);
    /// <summary>Retire this handle generation. The handle may already be retired if later Gateway cleanup returns an error.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_capability_close",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint capability_close(ulong handle);
    /// <summary>Revoke the alias for this Instance/backend, invalidating all its open sibling handles. Reopening cannot restore the revoked grant.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_capability_revoke",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint capability_revoke(ulong handle);
    /// <summary>Publish one canonical generated message through a separately granted publisher alias.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_topic_publish",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint topic_publish(ulong handle, uint memory, ulong payload_offset, ulong payload_length);
    /// <summary>Activate a separately granted subscription queue. Receive messages through event_poll on the guest execution thread.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_topic_subscribe",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint topic_subscribe(ulong handle);
    /// <summary>Poll a topic, service-request, incoming-goal or ROS2 parameter-event queue. Error 11 means no event; no native callback enters Wasm.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_event_poll",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint event_poll(ulong handle, uint memory, ulong output_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Bind separately granted event.observe and source owners. Correlation is nonzero only when an owned outgoing Action data kind is selected; metadata-only watches use zero and activate/consume no native payload. No aliases or rights are created.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_event_watch",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint event_watch(ulong observer_handle, ulong source_handle, ulong correlation);
    /// <summary>Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_event_poll_v1",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint event_poll_v1(ulong handle, uint memory, ulong output_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_event_ready",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint event_ready(ulong observer_handle, uint memory, ulong output_offset);
    /// <summary>Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_service_submit",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint service_submit(ulong source_handle, ulong observer_handle, uint memory, ulong request_offset, ulong request_length, ulong request_id_out_offset);
    /// <summary>Call the explicitly selected service using its canonical request/response type and configured deadline. Do not automatically replay a timed-out call.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_service_call",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint service_call(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Activate the explicitly granted service server. Poll owned request IDs through event_poll.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_service_advertise",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint service_advertise(ulong handle);
    /// <summary>Respond to an owned live request using a separate response grant and u64 request-ID-prefixed canonical response.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_service_respond",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint service_respond(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Send the selected trusted Action goal codec and receive an opaque u64 Goal ID. Builtin Fibonacci uses one canonical i32 order. Generated bounded Actions require their registered goal codec; all bounds remain explicit.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_action_send_goal",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_send_goal(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Request cancellation of one owned Goal using its u64 ID and a separately authorized cancel capability.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_action_cancel_own_goal",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_cancel_own_goal(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Use the separately granted any-cancel operation. Goal ID zero is valid only for this independently authorized operation.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_action_cancel_any_goal",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_cancel_any_goal(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Receive ID8 plus the selected trusted Action feedback codec for an owned Goal. Builtin Fibonacci appends count32/int32 values; an empty queue returns 11.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_action_receive_feedback",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_receive_feedback(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Receive ID8/outcome8 plus the selected trusted terminal result codec. Builtin Fibonacci appends count32/int32 values. Successful terminal consumption retires ownership.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_action_receive_result",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_receive_result(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Activate the explicitly granted Action server with an empty command. Poll ID8 plus the selected trusted goal codec; builtin Fibonacci uses i32 order. Responses require another grant.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_action_advertise",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_advertise(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Send ID8/kind8 plus the selected trusted feedback/result codec. Builtin Fibonacci appends count32/int32 values. Advertiser and responder require the same explicit ROS2 QoS contract.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_action_respond",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_respond(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Request the separately configured source/target frame pair at u64 nanoseconds; zero selects the latest admitted transform.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_tf_lookup",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint tf_lookup(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Broadcast one canonical TransformStamped for the separately configured frame pair. Do not normalize its quaternion implicitly.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_tf_broadcast",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint tf_broadcast(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Read the value fixed by the capability type. An empty request does not select a different node, name or value kind.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_parameter_get",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_get(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Set a canonical value of the explicitly granted parameter type. Numeric and element bounds remain enforced by the host.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_parameter_set",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_set(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Delete the explicitly granted ROS1 parameter using an empty request; no implicit parent-name authority is added.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_parameter_delete",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_delete(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Query existence of the explicitly granted ROS1 parameter; decode one canonical Bool response.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_parameter_exists",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_exists(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>List the separately authorized parameter scope; decode the bounded canonical String result.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_parameter_list",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_list(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Search the explicitly granted ROS1 resource with an empty request. Parent namespaces are included only with search-parent-namespaces=true, for the same final key and within allowed-name-prefix before every native query.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_parameter_search",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_search(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Inspect the configured topic filter with an empty request and decode sorted canonical String rows.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_graph_inspect_topic",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_inspect_topic(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Inspect the configured node filter with an empty request and decode sorted canonical String rows.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_graph_inspect_node",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_inspect_node(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Inspect the configured service filter with an empty request and decode sorted canonical String rows.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_graph_inspect_service",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_inspect_service(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>List topics within the explicit exact/prefix graph filter; decode sorted canonical String rows.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_graph_list_topics",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_list_topics(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>List nodes within the explicit graph filter; this is not unrestricted discovery authority.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_graph_list_nodes",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_list_nodes(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>List services within the explicit graph filter; decode sorted canonical String rows.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_graph_list_services",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_list_services(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Read the separately authorized ROS clock domain as little-endian u64 nanoseconds.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_time_read_ros",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint time_read_ros(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Read the independently authorized ROS1 wall clock; another time grant does not imply this one.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_time_read_wall",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint time_read_wall(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Write one canonical String to the separately granted ROS logger. This does not open a file or socket.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros1_log_write",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint log_write(ulong handle, uint memory, ulong payload_offset, ulong payload_length);
}
#endif
#if UWVM_ROS2
internal static class SelectedRaw {
    /// <summary>Return ABI version 1. This does not open a capability or authorize a resource.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_abi_version",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint abi_version();
    /// <summary>Open one explicitly granted alias in the caller Instance and selected backend. Write an opaque eight-byte little-endian handle on success.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_capability_open",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint capability_open(uint memory, ulong alias_offset, ulong alias_length, ulong handle_out_offset);
    /// <summary>Retire this handle generation. The handle may already be retired if later Gateway cleanup returns an error.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_capability_close",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint capability_close(ulong handle);
    /// <summary>Revoke the alias for this Instance/backend, invalidating all its open sibling handles. Reopening cannot restore the revoked grant.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_capability_revoke",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint capability_revoke(ulong handle);
    /// <summary>Publish one canonical generated message through a separately granted publisher alias.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_topic_publish",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint topic_publish(ulong handle, uint memory, ulong payload_offset, ulong payload_length);
    /// <summary>Activate a separately granted subscription queue. Receive messages through event_poll on the guest execution thread.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_topic_subscribe",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint topic_subscribe(ulong handle);
    /// <summary>Poll a topic, service-request, incoming-goal or ROS2 parameter-event queue. Error 11 means no event; no native callback enters Wasm.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_event_poll",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint event_poll(ulong handle, uint memory, ulong output_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Bind separately granted event.observe and source owners. Correlation is nonzero only when an owned outgoing Action data kind is selected; metadata-only watches use zero and activate/consume no native payload. No aliases or rights are created.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_event_watch",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint event_watch(ulong observer_handle, ulong source_handle, ulong correlation);
    /// <summary>Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_event_poll_v1",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint event_poll_v1(ulong handle, uint memory, ulong output_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_event_ready",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint event_ready(ulong observer_handle, uint memory, ulong output_offset);
    /// <summary>Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_service_submit",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint service_submit(ulong source_handle, ulong observer_handle, uint memory, ulong request_offset, ulong request_length, ulong request_id_out_offset);
    /// <summary>Call the explicitly selected service using its canonical request/response type and configured deadline. Do not automatically replay a timed-out call.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_service_call",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint service_call(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Activate the explicitly granted service server. Poll owned request IDs through event_poll.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_service_advertise",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint service_advertise(ulong handle);
    /// <summary>Respond to an owned live request using a separate response grant and u64 request-ID-prefixed canonical response.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_service_respond",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint service_respond(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Send the selected trusted Action goal codec and receive an opaque u64 Goal ID. Builtin Fibonacci uses one canonical i32 order. Generated bounded Actions require their registered goal codec; all bounds remain explicit.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_action_send_goal",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_send_goal(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Request cancellation of one owned Goal using its u64 ID and a separately authorized cancel capability.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_action_cancel_own_goal",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_cancel_own_goal(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Use the separately granted any-cancel operation. Goal ID zero is valid only for this independently authorized operation.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_action_cancel_any_goal",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_cancel_any_goal(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Receive ID8 plus the selected trusted Action feedback codec for an owned Goal. Builtin Fibonacci appends count32/int32 values; an empty queue returns 11.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_action_receive_feedback",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_receive_feedback(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Receive ID8/outcome8 plus the selected trusted terminal result codec. Builtin Fibonacci appends count32/int32 values. Successful terminal consumption retires ownership.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_action_receive_result",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_receive_result(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Activate the explicitly granted Action server with an empty command. Poll ID8 plus the selected trusted goal codec; builtin Fibonacci uses i32 order. Responses require another grant.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_action_advertise",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_advertise(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Send ID8/kind8 plus the selected trusted feedback/result codec. Builtin Fibonacci appends count32/int32 values. Advertiser and responder require the same explicit ROS2 QoS contract.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_action_respond",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint action_respond(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Request the separately configured source/target frame pair at u64 nanoseconds; zero selects the latest admitted transform.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_tf_lookup",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint tf_lookup(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Broadcast one canonical TransformStamped for the separately configured frame pair. Do not normalize its quaternion implicitly.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_tf_broadcast",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint tf_broadcast(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Read the value fixed by the capability type. An empty request does not select a different node, name or value kind.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_parameter_get",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_get(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Set a canonical value of the explicitly granted parameter type. Numeric and element bounds remain enforced by the host.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_parameter_set",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_set(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>List the separately authorized parameter scope; decode the bounded canonical String result.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_parameter_list",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_list(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Read the separately authorized ROS2 parameter metadata as a canonical String.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_parameter_describe",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_describe(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Declare a typed parameter on the explicitly selected local ROS2 Gateway node. Remote declaration is unsupported.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_parameter_declare",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_declare(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Undeclare a typed parameter on the explicitly selected local ROS2 Gateway node. A missing remote operation is not emulated.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_parameter_undeclare",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_undeclare(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Inspect the configured topic filter with an empty request and decode sorted canonical String rows.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_graph_inspect_topic",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_inspect_topic(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Inspect the configured node filter with an empty request and decode sorted canonical String rows.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_graph_inspect_node",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_inspect_node(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Inspect the configured service filter with an empty request and decode sorted canonical String rows.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_graph_inspect_service",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_inspect_service(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Inspect the separately configured ROS2 Action filter; ROS1 does not export this function.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_graph_inspect_action",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_inspect_action(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Activate a separate ROS2 parameter-event projection grant. This takes only one handle, not the seven RPC arguments.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_parameter_subscribe_events",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint parameter_subscribe_events(ulong handle);
    /// <summary>List topics within the explicit exact/prefix graph filter; decode sorted canonical String rows.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_graph_list_topics",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_list_topics(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>List nodes within the explicit graph filter; this is not unrestricted discovery authority.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_graph_list_nodes",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_list_nodes(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>List services within the explicit graph filter; decode sorted canonical String rows.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_graph_list_services",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint graph_list_services(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Read the separately selected ROS2 lifecycle node state as u8 state ID followed by canonical String label.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_lifecycle_get_state",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint lifecycle_get_state(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Apply the single ROS2 transition fixed by policy using an empty request; the guest cannot override that transition.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_lifecycle_change_state",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint lifecycle_change_state(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Read the separately authorized ROS clock domain as little-endian u64 nanoseconds.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_time_read_ros",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint time_read_ros(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Read the independently authorized ROS2 system clock as u64 nanoseconds.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_time_read_system",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint time_read_system(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Read the independently authorized ROS2 steady clock as u64 nanoseconds.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_time_read_steady",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint time_read_steady(ulong handle, uint memory, ulong request_offset, ulong request_length, ulong response_offset, ulong capacity, ulong length_out_offset);
    /// <summary>Write one canonical String to the separately granted ROS logger. This does not open a file or socket.</summary>
    [DllImport("roswasm_bridge",EntryPoint="roswasm_ros2_log_write",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]
    internal static extern uint log_write(ulong handle, uint memory, ulong payload_offset, ulong payload_length);
}
#endif
