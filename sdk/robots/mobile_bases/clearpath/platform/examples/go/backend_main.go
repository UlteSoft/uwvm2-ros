//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && clearpath_main && !clearpath_noetic && !clearpath_noetic_husky && !clearpath_noetic_jackal && !clearpath_humble && !clearpath_jazzy

// Exactly one complete original application state and controlled ROS backend.
package main
import r "uwvm2.local/clearpath-example/records_main"
type State = r.Ros2Clearpath_platform_msgsMsgStatus
const stateCapacity = r.Ros2Clearpath_platform_msgsMsgStatusMaxWireBytes
func decodeState(b []byte)(State,error){return r.DecodeRos2Clearpath_platform_msgsMsgStatus(b)}
