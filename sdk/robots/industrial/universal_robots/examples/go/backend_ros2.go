//go:build tinygo && (wasm || tinygo.wasm) && urros2 && !urros1 && !polyscopex && ros2 && !ros1

// Explicit source selection; complete records are never mixed in one package.
package main
import r "uwvm2.local/ur-example/records_ros2"
type IOState = r.Ros2Ur_msgsMsgIOStates
const ioCapacity = r.Ros2Ur_msgsMsgIOStatesMaxWireBytes
func decodeIOState(b []byte)(IOState,error){return r.DecodeRos2Ur_msgsMsgIOStates(b)}
