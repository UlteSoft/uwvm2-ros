//go:build tinygo && (wasm || tinygo.wasm) && urros1 && !urros2 && !polyscopex && ros1 && !ros2

// Explicit source selection; complete records are never mixed in one package.
package main
import r "uwvm2.local/ur-example/records_ros1"
type IOState = r.Ros1Ur_msgsIOStates
const ioCapacity = r.Ros1Ur_msgsIOStatesMaxWireBytes
func decodeIOState(b []byte)(IOState,error){return r.DecodeRos1Ur_msgsIOStates(b)}
