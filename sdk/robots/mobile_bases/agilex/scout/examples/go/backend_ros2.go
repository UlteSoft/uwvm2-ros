//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1

// This compile-time alias selects the actual complete generated backend value.
package main
import custom "uwvm2.local/scout-example/customtypes"
type Status = custom.Ros2Scout_msgsMsgScoutStatus
const statusCapacity = custom.Ros2Scout_msgsMsgScoutStatusMaxWireBytes
var decodeStatus = custom.DecodeRos2Scout_msgsMsgScoutStatus
