//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2 && clearpath_noetic && !clearpath_noetic_husky && !clearpath_noetic_jackal && !clearpath_humble && !clearpath_jazzy && !clearpath_main

// Exactly one complete original application state and controlled ROS backend.
package main
import r "uwvm2.local/clearpath-example/records_noetic"
type State = r.Ros1Clearpath_platform_msgsStatus
const stateCapacity = r.Ros1Clearpath_platform_msgsStatusMaxWireBytes
func decodeState(b []byte)(State,error){return r.DecodeRos1Clearpath_platform_msgsStatus(b)}
