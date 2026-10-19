/** Shared AssemblyScript ABI concepts. All managed convenience wrappers use memory 0.
 * Raw declarations in ros1/raw.ts and ros2/raw.ts support explicit memory indices.
 * Every class executes inside the Wasm guest and opens only CLI-configured aliases.
 */
export enum Status { Success=0, UnknownModule=2, BadHandle=9, Again=11, Denied=13,
  Conflict=17, Invalid=22, TooLarge=90, Unsupported=95, Disconnected=107, Timeout=110, Revoked=125 }
/** Exact SDK operation selectors; selecting one grants no rights. Events use their dedicated one-argument import. */
export enum Operation {
  service_call,
  service_respond,
  action_send_goal,
  action_cancel_own_goal,
  action_cancel_any_goal,
  action_receive_feedback,
  action_receive_result,
  action_advertise,
  action_respond,
  tf_lookup,
  tf_broadcast,
  parameter_get,
  parameter_set,
  parameter_delete,
  parameter_exists,
  parameter_list,
  parameter_search,
  graph_inspect_topic,
  graph_inspect_node,
  graph_inspect_service,
  graph_list_topics,
  graph_list_nodes,
  graph_list_services,
  time_read_ros,
  time_read_wall,
  parameter_describe,
  parameter_declare,
  parameter_undeclare,
  graph_inspect_action,
  lifecycle_get_state,
  lifecycle_change_state,
  time_read_system,
  time_read_steady,
  parameter_subscribe_events,
}
/** Safe common interface implemented by the version-specific NativeApi classes.
 * The base returns unsupported and never emulates native ROS or opens a network.
 * Do not instantiate Api for production; choose ros1/roswasm or ros2/roswasm.
 */
export class Api {
  /** Selected ROS backend; ABI version is separately checked by Node. */
  constructor(public backend: u32) {}
  /** Return imported ABI version, or zero for the unavailable base. */
  version(): u32 { return 0; }
  /** Whether this backend registers the selected SDK operation, checked before opening an alias. */
  supports(operation: Operation): bool { return false; }
  /** Open an alias and write a handle to the supplied eight-byte slot. */
  open(memory: u32, offset: u64, length: u64, slot: u64): u32 { return Status.Unsupported; }
  /** Close ownership exactly once. Backend errors do not imply a live handle. */
  close(handle: u64): u32 { return Status.Unsupported; }
  /** Revoke using existing runtime authority. */
  revoke(handle: u64): u32 { return Status.Unsupported; }
  /** Copy one canonical message through a topic.publish capability. */
  publish(handle: u64, memory: u32, offset: u64, length: u64): u32 { return Status.Unsupported; }
  /** Start an explicitly configured subscription. */
  subscribe(handle: u64): u32 { return Status.Unsupported; }
  /** Poll one copied event without native callbacks. */
  poll(handle: u64, memory: u32, offset: u64, capacity: u64, slot: u64): u32 { return Status.Unsupported; }
  /** Bind an explicit observer/source pair; no aliases or rights are inferred. */
  watchEvents(observer:u64,source:u64,correlation:u64):u32 {return Status.Unsupported;}
  /** Capacity-checked versioned poll retains a record on90. */
  pollEventV1(observer:u64,memory:u32,output:u64,capacity:u64,slot:u64):u32 {return Status.Unsupported;}
  /** Write32 bytes of bounded readiness; it is not reserved credit. */
  eventReady(observer:u64,memory:u32,output:u64):u32 {return Status.Unsupported;}
  /** Successful acknowledgement is admission only; never replay107/110. */
  submitService(source:u64,observer:u64,memory:u32,input:u64,length:u64,slot:u64):u32 {return Status.Unsupported;}
  /** Advertise an explicitly configured service. */
  advertiseService(handle: u64): u32 { return Status.Unsupported; }
  /** Start ROS 2 parameter events; unsupported for ROS 1. */
  parameterEvents(handle: u64): u32 { return Status.Unsupported; }
  /** Copy a canonical length-prefixed log string through log.write authority. */
  log(handle: u64, memory: u32, offset: u64, length: u64): u32 { return Status.Unsupported; }
  /** Invoke one exact request operation once; wrappers must never replay an event on code 90. */
  request(operation: Operation, handle: u64, memory: u32, input: u64, length: u64, output: u64, capacity: u64, slot: u64): u32 { return Status.Unsupported; }
}
/** Default abort override for standalone guests: trap locally instead of importing env.abort.
 * Compile with --use abort=<entry-module>/abortGuest after re-exporting this symbol.
 * A trap is an unrecoverable guest failure, not a ROS log, clock or authority grant.
 */
export function abortGuest(message: usize, fileName: usize, line: u32, column: u32): void { unreachable(); }
