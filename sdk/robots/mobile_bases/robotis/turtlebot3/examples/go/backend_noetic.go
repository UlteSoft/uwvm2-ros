//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2 && tb3_noetic && !tb3_humble && !tb3_jazzy && !tb3_main

// Select one exact full original state and the genuine controlled backend.
package main
import r "uwvm2.local/turtlebot3-example/records_noetic"
type State = r.Ros1Turtlebot3_msgsSensorState
const stateCapacity = r.Ros1Turtlebot3_msgsSensorStateMaxWireBytes
func decodeState(b []byte)(State,error){return r.DecodeRos1Turtlebot3_msgsSensorState(b)}
