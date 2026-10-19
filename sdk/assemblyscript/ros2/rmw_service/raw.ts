/** Optional retained-native ROS2 service ABI. Declaration selects no grant.
 * All offsets/extents/handles are full u64 even in wasm32. Managed callers root
 * actual Uint8Array storage through each synchronous call and select memory0.
 * This is separate from the unchanged original81 business namespace.
 */
@external("uwvm_ros2_rmw_srv_v1", "abi_version")
export declare function abi_version(): u32;
@external("uwvm_ros2_rmw_srv_v1", "client_activate")
export declare function client_activate(source:u64, observer:u64, memory:u32, receipt:u64):u32;
@external("uwvm_ros2_rmw_srv_v1", "service_activate")
export declare function service_activate(source:u64, responder:u64, memory:u32, receipt:u64):u32;
@external("uwvm_ros2_rmw_srv_v1", "endpoint_inspect")
export declare function endpoint_inspect(source:u64, incarnation:u64, memory:u32, receipt:u64):u32;
@external("uwvm_ros2_rmw_srv_v1", "client_submit")
export declare function client_submit(source:u64, incarnation:u64, memory:u32, input:u64, length:u64, ticket:u64):u32;
@external("uwvm_ros2_rmw_srv_v1", "client_abandon")
export declare function client_abandon(source:u64, incarnation:u64, ticket:u64):u32;
@external("uwvm_ros2_rmw_srv_v1", "service_respond")
export declare function service_respond(source:u64, incarnation:u64, responder:u64, ticket:u64, memory:u32, input:u64, length:u64):u32;
@external("uwvm_ros2_rmw_srv_v1", "endpoint_peek")
export declare function endpoint_peek(source:u64, incarnation:u64, expectedWriter:u32, memory:u32, packet:u64, capacity:u64):u32;
@external("uwvm_ros2_rmw_srv_v1", "endpoint_commit")
export declare function endpoint_commit(source:u64, incarnation:u64, sequence:u64, reservation:u64):u32;
@external("uwvm_ros2_rmw_srv_v1", "endpoint_cancel")
export declare function endpoint_cancel(source:u64, incarnation:u64, sequence:u64, reservation:u64):u32;
@external("uwvm_ros2_rmw_srv_v1", "client_available")
export declare function client_available(source:u64, incarnation:u64, availability:u64, waitNs:u64, memory:u32, output:u64):u32;
@external("uwvm_ros2_rmw_srv_v1", "endpoint_retire")
export declare function endpoint_retire(source:u64, incarnation:u64):u32;
