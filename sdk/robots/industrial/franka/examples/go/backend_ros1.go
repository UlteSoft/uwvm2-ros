//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2

// Select one exact full original state and the genuine controlled backend.
package main
import r "uwvm2.local/franka-example/records_ros1"
type State = r.Ros1Franka_msgsFrankaState
const stateCapacity = r.Ros1Franka_msgsFrankaStateMaxWireBytes
func decodeState(b []byte)(State,error){return r.DecodeRos1Franka_msgsFrankaState(b)}
