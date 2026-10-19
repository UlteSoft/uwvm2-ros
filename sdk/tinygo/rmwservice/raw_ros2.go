//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && rmw_service_v1

// Package rmwservice is the explicit optional retained-native Service SDK.
// This is the real raw12 namespace; there is no old-business fallback/stub.
package rmwservice

// RawABIVersion invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 abi_version
func RawABIVersion() uint32

// RawClientActivate invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 client_activate
func RawClientActivate(a0 uint64, a1 uint64, a2 uint32, a3 uint64) uint32

// RawServiceActivate invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 service_activate
func RawServiceActivate(a0 uint64, a1 uint64, a2 uint32, a3 uint64) uint32

// RawEndpointInspect invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 endpoint_inspect
func RawEndpointInspect(a0 uint64, a1 uint64, a2 uint32, a3 uint64) uint32

// RawClientSubmit invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 client_submit
func RawClientSubmit(a0 uint64, a1 uint64, a2 uint32, a3 uint64, a4 uint64, a5 uint64) uint32

// RawClientAbandon invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 client_abandon
func RawClientAbandon(a0 uint64, a1 uint64, a2 uint64) uint32

// RawServiceRespond invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 service_respond
func RawServiceRespond(a0 uint64, a1 uint64, a2 uint64, a3 uint64, a4 uint32, a5 uint64, a6 uint64) uint32

// RawEndpointPeek invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 endpoint_peek
func RawEndpointPeek(a0 uint64, a1 uint64, a2 uint32, a3 uint32, a4 uint64, a5 uint64) uint32

// RawEndpointCommit invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 endpoint_commit
func RawEndpointCommit(a0 uint64, a1 uint64, a2 uint64, a3 uint64) uint32

// RawEndpointCancel invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 endpoint_cancel
func RawEndpointCancel(a0 uint64, a1 uint64, a2 uint64, a3 uint64) uint32

// RawClientAvailable invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 client_available
func RawClientAvailable(a0 uint64, a1 uint64, a2 uint64, a3 uint64, a4 uint32, a5 uint64) uint32

// RawEndpointRetire invokes only the exact numeric optional ABI.
//go:wasmimport uwvm_ros2_rmw_srv_v1 endpoint_retire
func RawEndpointRetire(a0 uint64, a1 uint64) uint32

