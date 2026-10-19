package org.uwvm.roswasm;

import org.teavm.interop.Import;
import org.teavm.interop.Unmanaged;

/** Complete ROS 2 ABI v1 imports for TeaVM WEBASSEMBLY_WASI.
 * Numeric offsets are long/i64; memory indices are int/i32.
 * No method grants authority. Do not pass WasmGC references as offsets.
 * Generated from sdk/abi/abi-v1.json; edit the generator instead. */
public final class Ros2Raw {
    private Ros2Raw() {}
    /** Return ABI version 1. This does not open a capability or authorize a resource. */
    @Import(module="uwvm_ros2_v1", name="abi_version")
    @Unmanaged
    public static native int abi_version();
    /** Open one explicitly granted alias in the caller Instance and selected backend. Write an opaque eight-byte little-endian handle on success. */
    @Import(module="uwvm_ros2_v1", name="capability_open")
    @Unmanaged
    public static native int capability_open(int memory, long alias_offset, long alias_length, long handle_out_offset);
    /** Retire this handle generation. The handle may already be retired if later Gateway cleanup returns an error. */
    @Import(module="uwvm_ros2_v1", name="capability_close")
    @Unmanaged
    public static native int capability_close(long handle);
    /** Revoke the alias for this Instance/backend, invalidating all its open sibling handles. Reopening cannot restore the revoked grant. */
    @Import(module="uwvm_ros2_v1", name="capability_revoke")
    @Unmanaged
    public static native int capability_revoke(long handle);
    /** Publish one canonical generated message through a separately granted publisher alias. */
    @Import(module="uwvm_ros2_v1", name="topic_publish")
    @Unmanaged
    public static native int topic_publish(long handle, int memory, long payload_offset, long payload_length);
    /** Activate a separately granted subscription queue. Receive messages through event_poll on the guest execution thread. */
    @Import(module="uwvm_ros2_v1", name="topic_subscribe")
    @Unmanaged
    public static native int topic_subscribe(long handle);
    /** Poll a topic, service-request, incoming-goal or ROS2 parameter-event queue. Error 11 means no event; no native callback enters Wasm. */
    @Import(module="uwvm_ros2_v1", name="event_poll")
    @Unmanaged
    public static native int event_poll(long handle, int memory, long output_offset, long capacity, long length_out_offset);
    /** Bind separately granted event.observe and source owners. Correlation is nonzero only when an owned outgoing Action data kind is selected; metadata-only watches use zero and activate/consume no native payload. No aliases or rights are created. */
    @Import(module="uwvm_ros2_v1", name="event_watch")
    @Unmanaged
    public static native int event_watch(long observer_handle, long source_handle, long correlation);
    /** Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay. */
    @Import(module="uwvm_ros2_v1", name="event_poll_v1")
    @Unmanaged
    public static native int event_poll_v1(long handle, int memory, long output_offset, long capacity, long length_out_offset);
    /** Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation. */
    @Import(module="uwvm_ros2_v1", name="event_ready")
    @Unmanaged
    public static native int event_ready(long observer_handle, int memory, long output_offset);
    /** Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout. */
    @Import(module="uwvm_ros2_v1", name="service_submit")
    @Unmanaged
    public static native int service_submit(long source_handle, long observer_handle, int memory, long request_offset, long request_length, long request_id_out_offset);
    /** Call the explicitly selected service using its canonical request/response type and configured deadline. Do not automatically replay a timed-out call. */
    @Import(module="uwvm_ros2_v1", name="service_call")
    @Unmanaged
    public static native int service_call(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Activate the explicitly granted service server. Poll owned request IDs through event_poll. */
    @Import(module="uwvm_ros2_v1", name="service_advertise")
    @Unmanaged
    public static native int service_advertise(long handle);
    /** Respond to an owned live request using a separate response grant and u64 request-ID-prefixed canonical response. */
    @Import(module="uwvm_ros2_v1", name="service_respond")
    @Unmanaged
    public static native int service_respond(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Send the selected trusted Action goal codec and receive an opaque u64 Goal ID. Builtin Fibonacci uses one canonical i32 order. Generated bounded Actions require their registered goal codec; all bounds remain explicit. */
    @Import(module="uwvm_ros2_v1", name="action_send_goal")
    @Unmanaged
    public static native int action_send_goal(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Request cancellation of one owned Goal using its u64 ID and a separately authorized cancel capability. */
    @Import(module="uwvm_ros2_v1", name="action_cancel_own_goal")
    @Unmanaged
    public static native int action_cancel_own_goal(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Use the separately granted any-cancel operation. Goal ID zero is valid only for this independently authorized operation. */
    @Import(module="uwvm_ros2_v1", name="action_cancel_any_goal")
    @Unmanaged
    public static native int action_cancel_any_goal(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Receive ID8 plus the selected trusted Action feedback codec for an owned Goal. Builtin Fibonacci appends count32/int32 values; an empty queue returns 11. */
    @Import(module="uwvm_ros2_v1", name="action_receive_feedback")
    @Unmanaged
    public static native int action_receive_feedback(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Receive ID8/outcome8 plus the selected trusted terminal result codec. Builtin Fibonacci appends count32/int32 values. Successful terminal consumption retires ownership. */
    @Import(module="uwvm_ros2_v1", name="action_receive_result")
    @Unmanaged
    public static native int action_receive_result(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Activate the explicitly granted Action server with an empty command. Poll ID8 plus the selected trusted goal codec; builtin Fibonacci uses i32 order. Responses require another grant. */
    @Import(module="uwvm_ros2_v1", name="action_advertise")
    @Unmanaged
    public static native int action_advertise(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Send ID8/kind8 plus the selected trusted feedback/result codec. Builtin Fibonacci appends count32/int32 values. Advertiser and responder require the same explicit ROS2 QoS contract. */
    @Import(module="uwvm_ros2_v1", name="action_respond")
    @Unmanaged
    public static native int action_respond(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Request the separately configured source/target frame pair at u64 nanoseconds; zero selects the latest admitted transform. */
    @Import(module="uwvm_ros2_v1", name="tf_lookup")
    @Unmanaged
    public static native int tf_lookup(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Broadcast one canonical TransformStamped for the separately configured frame pair. Do not normalize its quaternion implicitly. */
    @Import(module="uwvm_ros2_v1", name="tf_broadcast")
    @Unmanaged
    public static native int tf_broadcast(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Read the value fixed by the capability type. An empty request does not select a different node, name or value kind. */
    @Import(module="uwvm_ros2_v1", name="parameter_get")
    @Unmanaged
    public static native int parameter_get(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Set a canonical value of the explicitly granted parameter type. Numeric and element bounds remain enforced by the host. */
    @Import(module="uwvm_ros2_v1", name="parameter_set")
    @Unmanaged
    public static native int parameter_set(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** List the separately authorized parameter scope; decode the bounded canonical String result. */
    @Import(module="uwvm_ros2_v1", name="parameter_list")
    @Unmanaged
    public static native int parameter_list(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Read the separately authorized ROS2 parameter metadata as a canonical String. */
    @Import(module="uwvm_ros2_v1", name="parameter_describe")
    @Unmanaged
    public static native int parameter_describe(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Declare a typed parameter on the explicitly selected local ROS2 Gateway node. Remote declaration is unsupported. */
    @Import(module="uwvm_ros2_v1", name="parameter_declare")
    @Unmanaged
    public static native int parameter_declare(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Undeclare a typed parameter on the explicitly selected local ROS2 Gateway node. A missing remote operation is not emulated. */
    @Import(module="uwvm_ros2_v1", name="parameter_undeclare")
    @Unmanaged
    public static native int parameter_undeclare(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Inspect the configured topic filter with an empty request and decode sorted canonical String rows. */
    @Import(module="uwvm_ros2_v1", name="graph_inspect_topic")
    @Unmanaged
    public static native int graph_inspect_topic(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Inspect the configured node filter with an empty request and decode sorted canonical String rows. */
    @Import(module="uwvm_ros2_v1", name="graph_inspect_node")
    @Unmanaged
    public static native int graph_inspect_node(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Inspect the configured service filter with an empty request and decode sorted canonical String rows. */
    @Import(module="uwvm_ros2_v1", name="graph_inspect_service")
    @Unmanaged
    public static native int graph_inspect_service(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Inspect the separately configured ROS2 Action filter; ROS1 does not export this function. */
    @Import(module="uwvm_ros2_v1", name="graph_inspect_action")
    @Unmanaged
    public static native int graph_inspect_action(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Activate a separate ROS2 parameter-event projection grant. This takes only one handle, not the seven RPC arguments. */
    @Import(module="uwvm_ros2_v1", name="parameter_subscribe_events")
    @Unmanaged
    public static native int parameter_subscribe_events(long handle);
    /** List topics within the explicit exact/prefix graph filter; decode sorted canonical String rows. */
    @Import(module="uwvm_ros2_v1", name="graph_list_topics")
    @Unmanaged
    public static native int graph_list_topics(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** List nodes within the explicit graph filter; this is not unrestricted discovery authority. */
    @Import(module="uwvm_ros2_v1", name="graph_list_nodes")
    @Unmanaged
    public static native int graph_list_nodes(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** List services within the explicit graph filter; decode sorted canonical String rows. */
    @Import(module="uwvm_ros2_v1", name="graph_list_services")
    @Unmanaged
    public static native int graph_list_services(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Read the separately selected ROS2 lifecycle node state as u8 state ID followed by canonical String label. */
    @Import(module="uwvm_ros2_v1", name="lifecycle_get_state")
    @Unmanaged
    public static native int lifecycle_get_state(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Apply the single ROS2 transition fixed by policy using an empty request; the guest cannot override that transition. */
    @Import(module="uwvm_ros2_v1", name="lifecycle_change_state")
    @Unmanaged
    public static native int lifecycle_change_state(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Read the separately authorized ROS clock domain as little-endian u64 nanoseconds. */
    @Import(module="uwvm_ros2_v1", name="time_read_ros")
    @Unmanaged
    public static native int time_read_ros(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Read the independently authorized ROS2 system clock as u64 nanoseconds. */
    @Import(module="uwvm_ros2_v1", name="time_read_system")
    @Unmanaged
    public static native int time_read_system(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Read the independently authorized ROS2 steady clock as u64 nanoseconds. */
    @Import(module="uwvm_ros2_v1", name="time_read_steady")
    @Unmanaged
    public static native int time_read_steady(long handle, int memory, long request_offset, long request_length, long response_offset, long capacity, long length_out_offset);
    /** Write one canonical String to the separately granted ROS logger. This does not open a file or socket. */
    @Import(module="uwvm_ros2_v1", name="log_write")
    @Unmanaged
    public static native int log_write(long handle, int memory, long payload_offset, long payload_length);
}
