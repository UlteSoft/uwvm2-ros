/** Semantic/import inventory fixture for ROS 2.
 * Compilation must retain every registered raw import and exact i32/i64 signature.
 * invokeRaw is deliberately unsafe with zero arguments: inspect the compiled binary,
 * do not execute it against real granted capabilities. _start performs no ROS calls.
 * This fixture is prepared; it has no compilation/runtime verdict until cgroup validation.
 */
import * as raw from "../ros2/raw";
export { abortGuest } from "../api";

/** Export a branch for each raw import to defeat compiler dead-code removal. */
export function invokeRaw(index:u32):u32 {
switch(index) {
case 0: return raw.abi_version();
case 1: return raw.capability_open((<u32>0), (<u64>0), (<u64>0), (<u64>0));
case 2: return raw.capability_close((<u64>0));
case 3: return raw.capability_revoke((<u64>0));
case 4: return raw.topic_publish((<u64>0), (<u32>0), (<u64>0), (<u64>0));
case 5: return raw.topic_subscribe((<u64>0));
case 6: return raw.event_poll((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0));
case 7: return raw.service_call((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 8: return raw.service_advertise((<u64>0));
case 9: return raw.service_respond((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 10: return raw.action_send_goal((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 11: return raw.action_cancel_own_goal((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 12: return raw.action_cancel_any_goal((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 13: return raw.action_receive_feedback((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 14: return raw.action_receive_result((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 15: return raw.action_advertise((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 16: return raw.action_respond((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 17: return raw.tf_lookup((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 18: return raw.tf_broadcast((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 19: return raw.parameter_get((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 20: return raw.parameter_set((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 21: return raw.parameter_list((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 22: return raw.parameter_describe((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 23: return raw.parameter_declare((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 24: return raw.parameter_undeclare((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 25: return raw.graph_inspect_topic((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 26: return raw.graph_inspect_node((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 27: return raw.graph_inspect_service((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 28: return raw.graph_inspect_action((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 29: return raw.parameter_subscribe_events((<u64>0));
case 30: return raw.graph_list_topics((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 31: return raw.graph_list_nodes((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 32: return raw.graph_list_services((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 33: return raw.lifecycle_get_state((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 34: return raw.lifecycle_change_state((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 35: return raw.time_read_ros((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 36: return raw.time_read_system((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 37: return raw.time_read_steady((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0), (<u64>0));
case 38: return raw.log_write((<u64>0), (<u32>0), (<u64>0), (<u64>0));
case 39: return raw.event_watch((<u64>0), (<u64>0), (<u64>0));
case 40: return raw.event_poll_v1((<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0));
case 41: return raw.event_ready((<u64>0), (<u32>0), (<u64>0));
case 42: return raw.service_submit((<u64>0), (<u64>0), (<u32>0), (<u64>0), (<u64>0), (<u64>0));
default: return 95;
}
}
/** Empty command entry; raw branches above are never called implicitly. */
export function _start():void {}
