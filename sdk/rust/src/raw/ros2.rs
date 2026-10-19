//! Exact ROS 2 ABI v1 imports. These functions never infer grants.
//!
//! # Safety
//! All offsets must address the selected guest linear memory. Inputs, outputs
//! and eight-byte little-endian output slots must remain valid through the call.
//! Handles must be obtained from this backend and the current instance. Return
//! codes are unsigned u32 values; ABI version is a value rather than an error.

#[cfg_attr(target_family = "wasm", link(wasm_import_module = "uwvm_ros2_v1"))]
extern "C" {
    /// Read ABI version 1 without opening a capability.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn abi_version() -> u32;
    /// Open an explicitly granted alias; success writes eight LE handle bytes.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn capability_open(
        memory: u32,
        alias_offset: u64,
        alias_length: u64,
        handle_out_offset: u64,
    ) -> u32;
    /// Retire this owned handle; a transport error does not undo retirement.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn capability_close(handle: u64) -> u32;
    /// Revoke the alias and its current handles; reopening returns REVOKED.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn capability_revoke(handle: u64) -> u32;
    /// Invoke topic_publish using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn topic_publish(handle: u64, memory: u32, payload_offset: u64, payload_length: u64)
        -> u32;
    /// Invoke topic_subscribe using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn topic_subscribe(handle: u64) -> u32;
    /// Poll one queued event on the guest thread; AGAIN means no event. A capacity error may consume the event.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn event_poll(
        handle: u64,
        memory: u32,
        output_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Bind separately granted event.observe and source owners; correlation is nonzero only for an owned outgoing Action goal. No aliases or rights are created.
    ///
    /// # Safety
    /// Live same-Instance/backend handles and bounded selected-memory ranges are required; no guest pointer is retained by the native asynchronous task.
    pub fn event_watch(observer_handle: u64, source_handle: u64, correlation: u64) -> u32;
    /// Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay.
    ///
    /// # Safety
    /// Live same-Instance/backend handles and bounded selected-memory ranges are required; no guest pointer is retained by the native asynchronous task.
    pub fn event_poll_v1(handle: u64, memory: u32, output_offset: u64, capacity: u64, length_out_offset: u64) -> u32;
    /// Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation.
    ///
    /// # Safety
    /// Live same-Instance/backend handles and bounded selected-memory ranges are required; no guest pointer is retained by the native asynchronous task.
    pub fn event_ready(observer_handle: u64, memory: u32, output_offset: u64) -> u32;
    /// Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout.
    ///
    /// # Safety
    /// Live same-Instance/backend handles and bounded selected-memory ranges are required; no guest pointer is retained by the native asynchronous task.
    pub fn service_submit(source_handle: u64, observer_handle: u64, memory: u32, request_offset: u64, request_length: u64, request_id_out_offset: u64) -> u32;
    /// Invoke service_call using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn service_call(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke service_advertise using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn service_advertise(handle: u64) -> u32;
    /// Invoke service_respond using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn service_respond(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke action_send_goal using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn action_send_goal(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke action_cancel_own_goal using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn action_cancel_own_goal(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke action_cancel_any_goal using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn action_cancel_any_goal(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke action_receive_feedback using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn action_receive_feedback(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke action_receive_result using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn action_receive_result(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke action_advertise using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn action_advertise(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke action_respond using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn action_respond(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke tf_lookup using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn tf_lookup(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke tf_broadcast using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn tf_broadcast(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke parameter_get using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn parameter_get(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke parameter_set using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn parameter_set(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke parameter_list using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn parameter_list(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke parameter_describe using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn parameter_describe(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke parameter_declare using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn parameter_declare(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke parameter_undeclare using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn parameter_undeclare(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke graph_inspect_topic using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn graph_inspect_topic(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke graph_inspect_node using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn graph_inspect_node(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke graph_inspect_service using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn graph_inspect_service(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke graph_inspect_action using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn graph_inspect_action(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke parameter_subscribe_events using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn parameter_subscribe_events(handle: u64) -> u32;
    /// Invoke graph_list_topics using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn graph_list_topics(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke graph_list_nodes using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn graph_list_nodes(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke graph_list_services using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn graph_list_services(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke lifecycle_get_state using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn lifecycle_get_state(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke lifecycle_change_state using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn lifecycle_change_state(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke time_read_ros using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn time_read_ros(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke time_read_system using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn time_read_system(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke time_read_steady using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn time_read_steady(
        handle: u64,
        memory: u32,
        request_offset: u64,
        request_length: u64,
        response_offset: u64,
        capacity: u64,
        length_out_offset: u64,
    ) -> u32;
    /// Invoke log_write using its separately authorized alias and canonical ABI payload.
    ///
    /// # Safety
    /// Follow the module memory/ownership contract; no host callback enters Wasm.
    pub fn log_write(handle: u64, memory: u32, payload_offset: u64, payload_length: u64) -> u32;
}
