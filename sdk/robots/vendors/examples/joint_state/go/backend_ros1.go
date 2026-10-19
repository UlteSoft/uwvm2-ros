//go:build ros1 && !ros2

package main
import records "uwvm2.local/joint-state-lesson/generated"
type JointState=records.Ros1Sensor_msgsJointState
const maximum=records.Ros1Sensor_msgsJointStateMaxWireBytes
func decode(b []byte)(JointState,error){return records.DecodeRos1Sensor_msgsJointState(b)}
func header(v JointState)bool{return v.Header.Seq==19&&v.Header.Stamp.Sec==42&&v.Header.Stamp.Nsec==300000000}
