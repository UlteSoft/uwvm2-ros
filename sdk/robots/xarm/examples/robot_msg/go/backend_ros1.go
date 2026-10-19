//go:build ros1 && !ros2

package main
import records "uwvm2.local/xarm-robot-msg-lesson/generated"
type RobotMsg=records.Ros1Xarm_msgsRobotMsg
const maximum=records.Ros1Xarm_msgsRobotMsgMaxWireBytes
func decode(b []byte)(RobotMsg,error){return records.DecodeRos1Xarm_msgsRobotMsg(b)}
func header(v RobotMsg)bool{return v.Header.Seq==19&&v.Header.Stamp.Sec==42&&v.Header.Stamp.Nsec==300000000}
