/** Generated fixed topic values; no I/O, native serialization or authority.
 * Import path assumes output is sdk/custom-types/generated; adjust only the
 * module path if moving it. Codec.valid must succeed before exposing values. */
import {Codec,Reader,Writer} from "../../assemblyscript/codecs";
/** Fixed ROS1 uwvm_custom_ros1_fixture/Packet value. */
export class Ros1Uwvm_custom_ros1_fixturePacket {
/** Canonical schema identity; host grants remain separate. */
static readonly codecIdentity:string="UWVMROS1_eb0eef4515ec546678bae4030eb858c151af98afa1bfa3ca08b3a0e02bd48544";
/** Native fixed field sequence. */
sequence:i32=0;
/** Native fixed field active. */
active:bool=false;
/** Native fixed field measurement. */
measurement:f64=0;
/** Native field tag: exactly 4 values. */
tag:Array<u8>=[0,0,0,0];
/** Native field samples: exactly 3 values. */
samples:Array<i16>=[0,0,0];
}
/** Checked SDK Codec for uwvm_custom_ros1_fixture/Packet; empty output signals invalid encoding. */
export class Ros1Uwvm_custom_ros1_fixturePacketCodec implements Codec<Ros1Uwvm_custom_ros1_fixturePacket> {
encode(value:Ros1Uwvm_custom_ros1_fixturePacket):Uint8Array {
if(value.tag.length!=4)return new Uint8Array(0);
if(value.samples.length!=3)return new Uint8Array(0);
if(!isFinite(value.measurement))return new Uint8Array(0);
const w=new Writer(23);
w.integer(<u64>value.sequence,4);
w.u8(value.active?1:0);
w.integer(<u64>reinterpret<u64>(value.measurement),8);
w.integer(<u64>value.tag[0],1);
w.integer(<u64>value.tag[1],1);
w.integer(<u64>value.tag[2],1);
w.integer(<u64>value.tag[3],1);
w.integer(<u64>value.samples[0],2);
w.integer(<u64>value.samples[1],2);
w.integer(<u64>value.samples[2],2);
return w.finish();}
/** Reject malformed bytes before decode is used by the subscription. */
valid(bytes:Uint8Array):bool {if(bytes.length!=23)return false;const r=new Reader(bytes);
r.integer(4);
if(r.u8()>1)return false;
if(!isFinite(reinterpret<f64>(<u64>r.integer(8))))return false;
r.integer(1);
r.integer(1);
r.integer(1);
r.integer(1);
r.integer(2);
r.integer(2);
r.integer(2);
return r.done();}
/** Decode previously validated bytes; malformed input returns a default value. */
decode(bytes:Uint8Array):Ros1Uwvm_custom_ros1_fixturePacket {const value=new Ros1Uwvm_custom_ros1_fixturePacket();if(!this.valid(bytes))return value;const r=new Reader(bytes);
value.sequence=<i32>r.integer(4);
value.active=r.u8()!=0;
value.measurement=reinterpret<f64>(<u64>r.integer(8));
value.tag[0]=<u8>r.integer(1);
value.tag[1]=<u8>r.integer(1);
value.tag[2]=<u8>r.integer(1);
value.tag[3]=<u8>r.integer(1);
value.samples[0]=<i16>r.integer(2);
value.samples[1]=<i16>r.integer(2);
value.samples[2]=<i16>r.integer(2);
return value;}
}
/** Fixed ROS2 uwvm_ros_custom_test_msgs/msg/FixedSample value. */
export class Ros2Uwvm_ros_custom_test_msgsFixedSample {
/** Canonical schema identity; host grants remain separate. */
static readonly codecIdentity:string="UWVMROS1_3f390efa2dccaf91a53c14a50fcead8790bb1bbc103c6ac6b29f176d045e2516";
/** Native fixed field sequence. */
sequence:u32=0;
/** Native fixed field enabled. */
enabled:bool=false;
/** Native fixed field temperature. */
temperature:f32=0;
/** Native field acceleration: exactly 3 values. */
acceleration:Array<f64>=[0,0,0];
/** Native field tag: exactly 8 values. */
tag:Array<u8>=[0,0,0,0,0,0,0,0];
/** Native field thresholds: exactly 2 values. */
thresholds:Array<i16>=[0,0];
}
/** Checked SDK Codec for uwvm_ros_custom_test_msgs/msg/FixedSample; empty output signals invalid encoding. */
export class Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec implements Codec<Ros2Uwvm_ros_custom_test_msgsFixedSample> {
encode(value:Ros2Uwvm_ros_custom_test_msgsFixedSample):Uint8Array {
if(value.acceleration.length!=3)return new Uint8Array(0);
if(value.tag.length!=8)return new Uint8Array(0);
if(value.thresholds.length!=2)return new Uint8Array(0);
if(!isFinite(value.temperature))return new Uint8Array(0);
if(!isFinite(value.acceleration[0]))return new Uint8Array(0);
if(!isFinite(value.acceleration[1]))return new Uint8Array(0);
if(!isFinite(value.acceleration[2]))return new Uint8Array(0);
const w=new Writer(45);
w.integer(<u64>value.sequence,4);
w.u8(value.enabled?1:0);
w.integer(<u64>reinterpret<u32>(value.temperature),4);
w.integer(<u64>reinterpret<u64>(value.acceleration[0]),8);
w.integer(<u64>reinterpret<u64>(value.acceleration[1]),8);
w.integer(<u64>reinterpret<u64>(value.acceleration[2]),8);
w.integer(<u64>value.tag[0],1);
w.integer(<u64>value.tag[1],1);
w.integer(<u64>value.tag[2],1);
w.integer(<u64>value.tag[3],1);
w.integer(<u64>value.tag[4],1);
w.integer(<u64>value.tag[5],1);
w.integer(<u64>value.tag[6],1);
w.integer(<u64>value.tag[7],1);
w.integer(<u64>value.thresholds[0],2);
w.integer(<u64>value.thresholds[1],2);
return w.finish();}
/** Reject malformed bytes before decode is used by the subscription. */
valid(bytes:Uint8Array):bool {if(bytes.length!=45)return false;const r=new Reader(bytes);
r.integer(4);
if(r.u8()>1)return false;
if(!isFinite(reinterpret<f32>(<u32>r.integer(4))))return false;
if(!isFinite(reinterpret<f64>(<u64>r.integer(8))))return false;
if(!isFinite(reinterpret<f64>(<u64>r.integer(8))))return false;
if(!isFinite(reinterpret<f64>(<u64>r.integer(8))))return false;
r.integer(1);
r.integer(1);
r.integer(1);
r.integer(1);
r.integer(1);
r.integer(1);
r.integer(1);
r.integer(1);
r.integer(2);
r.integer(2);
return r.done();}
/** Decode previously validated bytes; malformed input returns a default value. */
decode(bytes:Uint8Array):Ros2Uwvm_ros_custom_test_msgsFixedSample {const value=new Ros2Uwvm_ros_custom_test_msgsFixedSample();if(!this.valid(bytes))return value;const r=new Reader(bytes);
value.sequence=<u32>r.integer(4);
value.enabled=r.u8()!=0;
value.temperature=reinterpret<f32>(<u32>r.integer(4));
value.acceleration[0]=reinterpret<f64>(<u64>r.integer(8));
value.acceleration[1]=reinterpret<f64>(<u64>r.integer(8));
value.acceleration[2]=reinterpret<f64>(<u64>r.integer(8));
value.tag[0]=<u8>r.integer(1);
value.tag[1]=<u8>r.integer(1);
value.tag[2]=<u8>r.integer(1);
value.tag[3]=<u8>r.integer(1);
value.tag[4]=<u8>r.integer(1);
value.tag[5]=<u8>r.integer(1);
value.tag[6]=<u8>r.integer(1);
value.tag[7]=<u8>r.integer(1);
value.thresholds[0]=<i16>r.integer(2);
value.thresholds[1]=<i16>r.integer(2);
return value;}
}
