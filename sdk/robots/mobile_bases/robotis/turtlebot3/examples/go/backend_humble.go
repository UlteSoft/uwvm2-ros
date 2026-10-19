//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && tb3_humble && !tb3_noetic && !tb3_jazzy && !tb3_main

// Select one exact full original state and the genuine controlled backend.
package main
import r "uwvm2.local/turtlebot3-example/records_humble"
type State = r.Ros2Turtlebot3_msgsMsgSensorState
const stateCapacity = r.Ros2Turtlebot3_msgsMsgSensorStateMaxWireBytes
func decodeState(b []byte)(State,error){return r.DecodeRos2Turtlebot3_msgsMsgSensorState(b)}
