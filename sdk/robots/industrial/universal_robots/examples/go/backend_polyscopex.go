//go:build tinygo && (wasm || tinygo.wasm) && polyscopex && !urros1 && !urros2 && ros2 && !ros1

// Explicit source selection; complete records are never mixed in one package.
package main
import r "uwvm2.local/ur-example/records_polyscopex"
type IOState = r.Ros2UrinterfacesMsgDigitalIOStateStamped
const ioCapacity = r.Ros2UrinterfacesMsgDigitalIOStateStampedMaxWireBytes
func decodeIOState(b []byte)(IOState,error){return r.DecodeRos2UrinterfacesMsgDigitalIOStateStamped(b)}
