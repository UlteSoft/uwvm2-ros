//go:build ros2 && !ros1

package main
import records "uwvm2.local/joint-state-lesson/generated"
type JointState=records.Ros2Sensor_msgsMsgJointState
const maximum=records.Ros2Sensor_msgsMsgJointStateMaxWireBytes
func decode(b []byte)(JointState,error){return records.DecodeRos2Sensor_msgsMsgJointState(b)}
func header(v JointState)bool{return v.Header.Stamp.Sec == -2&&v.Header.Stamp.Nanosec==300000000}
