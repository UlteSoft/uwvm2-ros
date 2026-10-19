// Optional ROS2 retained native endpoint ABI, all twelve exact numeric imports.
// UInt64 is i64 even on wasm32; Int is i32. No Weak/Host transport fallback.
// Importing this package grants nothing. The final default-off producer must
// require backend2 and the exact actual catalog SHA when explicitly enabled.

pub fn raw_abi_version() -> Int =
  "uwvm_ros2_rmw_srv_v1" "abi_version"

pub fn raw_client_activate(source : UInt64, observer : UInt64, memory : Int, receipt : UInt64) -> Int =
  "uwvm_ros2_rmw_srv_v1" "client_activate"

pub fn raw_service_activate(advertise : UInt64, responder : UInt64, memory : Int, receipt : UInt64) -> Int =
  "uwvm_ros2_rmw_srv_v1" "service_activate"

pub fn raw_endpoint_inspect(source : UInt64, incarnation : UInt64, memory : Int, receipt : UInt64) -> Int =
  "uwvm_ros2_rmw_srv_v1" "endpoint_inspect"

pub fn raw_client_submit(source : UInt64, incarnation : UInt64, memory : Int, input : UInt64, length : UInt64, ticket : UInt64) -> Int =
  "uwvm_ros2_rmw_srv_v1" "client_submit"

pub fn raw_client_abandon(source : UInt64, incarnation : UInt64, ticket : UInt64) -> Int =
  "uwvm_ros2_rmw_srv_v1" "client_abandon"

pub fn raw_service_respond(advertise : UInt64, incarnation : UInt64, responder : UInt64, ticket : UInt64, memory : Int, input : UInt64, length : UInt64) -> Int =
  "uwvm_ros2_rmw_srv_v1" "service_respond"

pub fn raw_endpoint_peek(source : UInt64, incarnation : UInt64, expected_writer : Int, memory : Int, packet : UInt64, capacity : UInt64) -> Int =
  "uwvm_ros2_rmw_srv_v1" "endpoint_peek"

pub fn raw_endpoint_commit(source : UInt64, incarnation : UInt64, sequence : UInt64, reservation : UInt64) -> Int =
  "uwvm_ros2_rmw_srv_v1" "endpoint_commit"

pub fn raw_endpoint_cancel(source : UInt64, incarnation : UInt64, sequence : UInt64, reservation : UInt64) -> Int =
  "uwvm_ros2_rmw_srv_v1" "endpoint_cancel"

pub fn raw_client_available(source : UInt64, incarnation : UInt64, availability : UInt64, wait_ns : UInt64, memory : Int, available : UInt64) -> Int =
  "uwvm_ros2_rmw_srv_v1" "client_available"

pub fn raw_endpoint_retire(source : UInt64, incarnation : UInt64) -> Int =
  "uwvm_ros2_rmw_srv_v1" "endpoint_retire"
