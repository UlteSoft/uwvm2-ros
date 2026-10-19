//go:build ros1 && !ros2

package main
import records "uwvm2.local/xarm-cio-state-lesson/generated"
type CIOState=records.Ros1Xarm_msgsCIOState
const maximum=records.Ros1Xarm_msgsCIOStateMaxWireBytes
func decode(b []byte)(CIOState,error){return records.DecodeRos1Xarm_msgsCIOState(b)}
func header(v CIOState)bool{return v.Header.Seq==19&&v.Header.Stamp.Sec==42&&v.Header.Stamp.Nsec==300000000}
