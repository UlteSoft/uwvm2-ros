//go:build ros2 && !ros1

package main
import records "uwvm2.local/xarm-robot-msg-lesson/generated"
type RobotMsg=records.Ros2Xarm_msgsMsgRobotMsg
const maximum=records.Ros2Xarm_msgsMsgRobotMsgMaxWireBytes
func decode(b []byte)(RobotMsg,error){return records.DecodeRos2Xarm_msgsMsgRobotMsg(b)}
func header(v RobotMsg)bool{return v.Header.Stamp.Sec == -2&&v.Header.Stamp.Nanosec==300000000}
