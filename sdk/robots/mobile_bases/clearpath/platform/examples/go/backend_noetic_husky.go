//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2 && clearpath_noetic_husky && !clearpath_noetic && !clearpath_noetic_jackal && !clearpath_humble && !clearpath_jazzy && !clearpath_main

// Exactly one complete original application state and controlled ROS backend.
package main
import r "uwvm2.local/clearpath-example/records_noetic"
type State = r.Ros1Husky_msgsHuskyStatus
const stateCapacity = r.Ros1Husky_msgsHuskyStatusMaxWireBytes
func decodeState(b []byte)(State,error){return r.DecodeRos1Husky_msgsHuskyStatus(b)}
