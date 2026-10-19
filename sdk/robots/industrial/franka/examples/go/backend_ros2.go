//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1

// Select one exact full original state and the genuine controlled backend.
package main
import r "uwvm2.local/franka-example/records_ros2"
type State = r.Ros2Franka_msgsMsgFrankaRobotState
const stateCapacity = r.Ros2Franka_msgsMsgFrankaRobotStateMaxWireBytes
func decodeState(b []byte)(State,error){return r.DecodeRos2Franka_msgsMsgFrankaRobotState(b)}
