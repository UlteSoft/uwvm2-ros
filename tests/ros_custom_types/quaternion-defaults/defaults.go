// Actual Go/TinyGo executable over generated customtypes, not a ROS backend.
// Stage in a fresh module default_contract; generated source is customtypes/.
package main
import ct "default_contract/customtypes"
func main(){
 q:=ct.DefaultRos2Geometry_msgsMsgQuaternion();if q.X!=0||q.Y!=0||q.Z!=0||q.W!=1{panic("quaternion defaults")}
 b:=ct.DefaultRos2Default_fixtureMsgBundle();if b.Fixed[0].W!=1||b.Fixed[1].W!=1||b.Pose.Orientation.W!=1||len(b.Dynamic)!=0{panic("nested defaults")}
 b.Fixed[0].W=.25;if b.Fixed[1].W!=1{panic("fixed elements alias")};b.Dynamic=append(b.Dynamic,ct.DefaultRos2Geometry_msgsMsgQuaternion())
 wire,err:=b.Encode();if err!=nil{panic(err)};out,err:=ct.DecodeRos2Default_fixtureMsgBundle(wire);if err!=nil{panic(err)}
 if out.Fixed[0].W!=.25||out.Fixed[1].W!=1||out.Dynamic[0].W!=1{panic("full canonical roundtrip")}
 if _,err=ct.DecodeRos2Geometry_msgsMsgQuaternion(make([]byte,31));err==nil{panic("truncation filled default")}
 zero,err:=ct.DecodeRos2Geometry_msgsMsgQuaternion(make([]byte,32));if err!=nil||zero.W!=0{panic("payload default coercion")}
}
