//! Compile/import-inspection fixture retaining every raw function of one backend.
//! This is NOT a runnable ROS example: never invoke roswasm_abi_coverage with a
//! valid index. Its deliberately zero-valued raw arguments are not real buffers,
//! aliases, handles or resource requests. main performs no ROS call.
//!
//! Pass -C link-arg=--export=roswasm_abi_coverage to the Rust linker so the probe
//! remains externally reachable and every selected import survives dead stripping.
//! Compilation/import-table inspection is separate from guest/ROS execution.
#[cfg(all(feature = "ros1", not(feature = "ros2")))]
use roswasm::raw::ros1 as imports;
#[cfg(all(feature = "ros2", not(feature = "ros1")))]
use roswasm::raw::ros2 as imports;
#[cfg(any(
    all(feature = "ros1", feature = "ros2"),
    not(any(feature = "ros1", feature = "ros2"))
))]
compile_error!("Choose exactly one coverage feature: ros1 or ros2");

/// Retain all ABI imports for a compiler-only coverage module. Do not execute
/// the probe: zero arguments are placeholders, not host-authorized operations.
#[no_mangle]
#[inline(never)]
pub extern "C" fn roswasm_abi_coverage(index: u32) -> u32 {
    #[cfg(feature = "ros1")]
    {
        return match index {
            0 => unsafe { imports::abi_version() },
            1 => unsafe { imports::capability_open(0u32, 0u64, 0u64, 0u64) },
            2 => unsafe { imports::capability_close(0u64) },
            3 => unsafe { imports::capability_revoke(0u64) },
            4 => unsafe { imports::topic_publish(0u64, 0u32, 0u64, 0u64) },
            5 => unsafe { imports::topic_subscribe(0u64) },
            6 => unsafe { imports::event_poll(0u64, 0u32, 0u64, 0u64, 0u64) },
            7 => unsafe { imports::service_call(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            8 => unsafe { imports::service_advertise(0u64) },
            9 => unsafe { imports::service_respond(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            10 => unsafe { imports::action_send_goal(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            11 => unsafe {
                imports::action_cancel_own_goal(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            12 => unsafe {
                imports::action_cancel_any_goal(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            13 => unsafe {
                imports::action_receive_feedback(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            14 => unsafe {
                imports::action_receive_result(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            15 => unsafe { imports::action_advertise(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            16 => unsafe { imports::action_respond(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            17 => unsafe { imports::tf_lookup(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            18 => unsafe { imports::tf_broadcast(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            19 => unsafe { imports::parameter_get(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            20 => unsafe { imports::parameter_set(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            21 => unsafe { imports::parameter_delete(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            22 => unsafe { imports::parameter_exists(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            23 => unsafe { imports::parameter_list(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            24 => unsafe { imports::parameter_search(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            25 => unsafe { imports::graph_inspect_topic(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            26 => unsafe { imports::graph_inspect_node(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            27 => unsafe {
                imports::graph_inspect_service(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            28 => unsafe { imports::graph_list_topics(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            29 => unsafe { imports::graph_list_nodes(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            30 => unsafe { imports::graph_list_services(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            31 => unsafe { imports::time_read_ros(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            32 => unsafe { imports::time_read_wall(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            33 => unsafe { imports::log_write(0u64, 0u32, 0u64, 0u64) },
            34 => unsafe { imports::event_watch(0u64, 0u64, 0u64) },
            35 => unsafe { imports::event_poll_v1(0u64, 0u32, 0u64, 0u64, 0u64) },
            36 => unsafe { imports::event_ready(0u64, 0u32, 0u64) },
            37 => unsafe { imports::service_submit(0u64, 0u64, 0u32, 0u64, 0u64, 0u64) },
            _ => roswasm::codes::UNSUPPORTED,
        };
    }
    #[cfg(feature = "ros2")]
    {
        return match index {
            0 => unsafe { imports::abi_version() },
            1 => unsafe { imports::capability_open(0u32, 0u64, 0u64, 0u64) },
            2 => unsafe { imports::capability_close(0u64) },
            3 => unsafe { imports::capability_revoke(0u64) },
            4 => unsafe { imports::topic_publish(0u64, 0u32, 0u64, 0u64) },
            5 => unsafe { imports::topic_subscribe(0u64) },
            6 => unsafe { imports::event_poll(0u64, 0u32, 0u64, 0u64, 0u64) },
            7 => unsafe { imports::service_call(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            8 => unsafe { imports::service_advertise(0u64) },
            9 => unsafe { imports::service_respond(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            10 => unsafe { imports::action_send_goal(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            11 => unsafe {
                imports::action_cancel_own_goal(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            12 => unsafe {
                imports::action_cancel_any_goal(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            13 => unsafe {
                imports::action_receive_feedback(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            14 => unsafe {
                imports::action_receive_result(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            15 => unsafe { imports::action_advertise(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            16 => unsafe { imports::action_respond(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            17 => unsafe { imports::tf_lookup(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            18 => unsafe { imports::tf_broadcast(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            19 => unsafe { imports::parameter_get(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            20 => unsafe { imports::parameter_set(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            21 => unsafe { imports::parameter_list(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            22 => unsafe { imports::parameter_describe(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            23 => unsafe { imports::parameter_declare(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            24 => unsafe { imports::parameter_undeclare(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            25 => unsafe { imports::graph_inspect_topic(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            26 => unsafe { imports::graph_inspect_node(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            27 => unsafe {
                imports::graph_inspect_service(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            28 => unsafe {
                imports::graph_inspect_action(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            29 => unsafe { imports::parameter_subscribe_events(0u64) },
            30 => unsafe { imports::graph_list_topics(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            31 => unsafe { imports::graph_list_nodes(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            32 => unsafe { imports::graph_list_services(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            33 => unsafe { imports::lifecycle_get_state(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            34 => unsafe {
                imports::lifecycle_change_state(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64)
            },
            35 => unsafe { imports::time_read_ros(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            36 => unsafe { imports::time_read_system(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            37 => unsafe { imports::time_read_steady(0u64, 0u32, 0u64, 0u64, 0u64, 0u64, 0u64) },
            38 => unsafe { imports::log_write(0u64, 0u32, 0u64, 0u64) },
            39 => unsafe { imports::event_watch(0u64, 0u64, 0u64) },
            40 => unsafe { imports::event_poll_v1(0u64, 0u32, 0u64, 0u64, 0u64) },
            41 => unsafe { imports::event_ready(0u64, 0u32, 0u64) },
            42 => unsafe { imports::service_submit(0u64, 0u64, 0u32, 0u64, 0u64, 0u64) },
            _ => roswasm::codes::UNSUPPORTED,
        };
    }
}

fn main() { /* Compile/import inspection only. No raw ROS probe is invoked. */
}
