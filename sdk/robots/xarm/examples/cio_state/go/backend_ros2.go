//go:build ros2 && !ros1

package main
import records "uwvm2.local/xarm-cio-state-lesson/generated"
type CIOState=records.Ros2Xarm_msgsMsgCIOState
const maximum=records.Ros2Xarm_msgsMsgCIOStateMaxWireBytes
func decode(b []byte)(CIOState,error){return records.DecodeRos2Xarm_msgsMsgCIOState(b)}
func header(v CIOState)bool{return v.Header.Stamp.Sec == -2&&v.Header.Stamp.Nanosec==300000000}
