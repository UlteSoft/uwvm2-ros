/** ROS 1 AssemblyScript binding: actual Wasm imports, no native ROS dependency.
 * Import this module only for the selected backend; no automatic cross-version fallback.
 */
import * as raw from "./raw";
import { Api, Operation, Status } from "../api";
export { Operation, Status, abortGuest } from "../api";
export { Node, Capability, Publisher, Subscription, EventSubscription, Executor, Client, Service, ActionClient, ActionServer, Parameters, Graph, Clock, Logger, Tf, Lifecycle, Resource, Reply, Session, HandleReply } from "../roswasm";
export * from "../codecs";
export * from "../events";

/** Complete adapter from managed SDK operations to exact raw imports. */
export class NativeApi extends Api {
/** Select ROS 1; constructor creates no native endpoint and opens no alias. */
constructor() { super(1); }
/** Delegate version to the exact ABI; no retry, allocation or authority change. */
version(): u32 { return raw.abi_version(); }
/** Delegate open to the exact ABI; no retry, allocation or authority change. */
open(memory:u32, offset:u64, length:u64, slot:u64):u32 { return raw.capability_open(memory,offset,length,slot); }
/** Delegate close to the exact ABI; no retry, allocation or authority change. */
close(handle:u64):u32 { return raw.capability_close(handle); }
/** Delegate revoke to the exact ABI; no retry, allocation or authority change. */
revoke(handle:u64):u32 { return raw.capability_revoke(handle); }
/** Delegate publish to the exact ABI; no retry, allocation or authority change. */
publish(handle:u64,memory:u32,offset:u64,length:u64):u32 { return raw.topic_publish(handle,memory,offset,length); }
/** Delegate subscribe to the exact ABI; no retry, allocation or authority change. */
subscribe(handle:u64):u32 { return raw.topic_subscribe(handle); }
/** Delegate poll to the exact ABI; no retry, allocation or authority change. */
poll(handle:u64,memory:u32,offset:u64,capacity:u64,slot:u64):u32 { return raw.event_poll(handle,memory,offset,capacity,slot); }
/** Exact additive observer association; does not create a grant. */
watchEvents(observer:u64,source:u64,correlation:u64):u32{return raw.event_watch(observer,source,correlation);}
/** Exact versioned poll; unlike legacy poll,90 retains the queued event. */
pollEventV1(observer:u64,memory:u32,output:u64,capacity:u64,slot:u64):u32{return raw.event_poll_v1(observer,memory,output,capacity,slot);}
/** Exact bounded readiness snapshot. */
eventReady(observer:u64,memory:u32,output:u64):u32{return raw.event_ready(observer,memory,output);}
/** Exact bounded asynchronous native service admission. */
submitService(source:u64,observer:u64,memory:u32,input:u64,length:u64,slot:u64):u32{return raw.service_submit(source,observer,memory,input,length,slot);}
/** Delegate advertiseService to the exact ABI; no retry, allocation or authority change. */
advertiseService(handle:u64):u32 { return raw.service_advertise(handle); }
/** Delegate log to the exact ABI; no retry, allocation or authority change. */
log(handle:u64,memory:u32,offset:u64,length:u64):u32 { return raw.log_write(handle,memory,offset,length); }
/** Delegate parameterEvents to the exact ABI; no retry, allocation or authority change. */
parameterEvents(handle:u64):u32 { return Status.Unsupported; }
/** Check exact backend request availability without opening an alias or calling ROS. */
supports(operation:Operation):bool {
switch(operation) {
case Operation.service_call:
case Operation.service_respond:
case Operation.action_send_goal:
case Operation.action_cancel_own_goal:
case Operation.action_cancel_any_goal:
case Operation.action_receive_feedback:
case Operation.action_receive_result:
case Operation.action_advertise:
case Operation.action_respond:
case Operation.tf_lookup:
case Operation.tf_broadcast:
case Operation.parameter_get:
case Operation.parameter_set:
case Operation.parameter_delete:
case Operation.parameter_exists:
case Operation.parameter_list:
case Operation.parameter_search:
case Operation.graph_inspect_topic:
case Operation.graph_inspect_node:
case Operation.graph_inspect_service:
case Operation.graph_list_topics:
case Operation.graph_list_nodes:
case Operation.graph_list_services:
case Operation.time_read_ros:
case Operation.time_read_wall:
return true;
default: return false;
}
}
/** Dispatch all registered request imports; unsupported backend operations return 95. */
request(operation:Operation,handle:u64,memory:u32,input:u64,length:u64,output:u64,capacity:u64,slot:u64):u32 {
switch(operation) {
case Operation.service_call: return raw.service_call(handle,memory,input,length,output,capacity,slot);
case Operation.service_respond: return raw.service_respond(handle,memory,input,length,output,capacity,slot);
case Operation.action_send_goal: return raw.action_send_goal(handle,memory,input,length,output,capacity,slot);
case Operation.action_cancel_own_goal: return raw.action_cancel_own_goal(handle,memory,input,length,output,capacity,slot);
case Operation.action_cancel_any_goal: return raw.action_cancel_any_goal(handle,memory,input,length,output,capacity,slot);
case Operation.action_receive_feedback: return raw.action_receive_feedback(handle,memory,input,length,output,capacity,slot);
case Operation.action_receive_result: return raw.action_receive_result(handle,memory,input,length,output,capacity,slot);
case Operation.action_advertise: return raw.action_advertise(handle,memory,input,length,output,capacity,slot);
case Operation.action_respond: return raw.action_respond(handle,memory,input,length,output,capacity,slot);
case Operation.tf_lookup: return raw.tf_lookup(handle,memory,input,length,output,capacity,slot);
case Operation.tf_broadcast: return raw.tf_broadcast(handle,memory,input,length,output,capacity,slot);
case Operation.parameter_get: return raw.parameter_get(handle,memory,input,length,output,capacity,slot);
case Operation.parameter_set: return raw.parameter_set(handle,memory,input,length,output,capacity,slot);
case Operation.parameter_delete: return raw.parameter_delete(handle,memory,input,length,output,capacity,slot);
case Operation.parameter_exists: return raw.parameter_exists(handle,memory,input,length,output,capacity,slot);
case Operation.parameter_list: return raw.parameter_list(handle,memory,input,length,output,capacity,slot);
case Operation.parameter_search: return raw.parameter_search(handle,memory,input,length,output,capacity,slot);
case Operation.graph_inspect_topic: return raw.graph_inspect_topic(handle,memory,input,length,output,capacity,slot);
case Operation.graph_inspect_node: return raw.graph_inspect_node(handle,memory,input,length,output,capacity,slot);
case Operation.graph_inspect_service: return raw.graph_inspect_service(handle,memory,input,length,output,capacity,slot);
case Operation.graph_list_topics: return raw.graph_list_topics(handle,memory,input,length,output,capacity,slot);
case Operation.graph_list_nodes: return raw.graph_list_nodes(handle,memory,input,length,output,capacity,slot);
case Operation.graph_list_services: return raw.graph_list_services(handle,memory,input,length,output,capacity,slot);
case Operation.time_read_ros: return raw.time_read_ros(handle,memory,input,length,output,capacity,slot);
case Operation.time_read_wall: return raw.time_read_wall(handle,memory,input,length,output,capacity,slot);
default: return Status.Unsupported;
}
}
}

/** Pure local scheduling concepts: no ROS or clock imports. */
export { Context, Future, FutureReply, Timer, TimerReply, CallbackGroup } from "../local";
