//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2

// This compile-time alias selects the actual complete generated backend value.
package main
import custom "uwvm2.local/scout-example/customtypes"
type Status = custom.Ros1Scout_msgsScoutStatus
const statusCapacity = custom.Ros1Scout_msgsScoutStatusMaxWireBytes
var decodeStatus = custom.DecodeRos1Scout_msgsScoutStatus
