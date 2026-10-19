/** Generated fixed topic values; no I/O, native serialization or authority.
 * Import path assumes output is sdk/custom-types/generated; adjust only the
 * module path if moving it. Codec.valid must succeed before exposing values. */
import {Codec,Reader,Writer} from "../../assemblyscript/codecs";
/** Fixed ROS1 example_interfaces_custom/Packet value. */
export class Ros1Example_interfaces_customPacket {
/** Canonical schema identity; host grants remain separate. */
static readonly codecIdentity:string="UWVMROS1_e68d0934f545cd81105ac13682379dc84f023c860c070c046e69c7f70e94d407";
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
/** Checked SDK Codec for example_interfaces_custom/Packet; empty output signals invalid encoding. */
export class Ros1Example_interfaces_customPacketCodec implements Codec<Ros1Example_interfaces_customPacket> {
encode(value:Ros1Example_interfaces_customPacket):Uint8Array {
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
decode(bytes:Uint8Array):Ros1Example_interfaces_customPacket {const value=new Ros1Example_interfaces_customPacket();if(!this.valid(bytes))return value;const r=new Reader(bytes);
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
/** Fixed ROS1 example_interfaces_custom/ScalarRecord value. */
export class Ros1Example_interfaces_customScalarRecord {
/** Canonical schema identity; host grants remain separate. */
static readonly codecIdentity:string="UWVMROS1_dfab9988d24ce356a6a914d4ae6f6a7c8c44dc8118cfc768e41fa8bf47f386c6";
/** Native fixed field enabled. */
enabled:bool=false;
/** Native fixed field unsigned_small. */
unsigned_small:u8=0;
/** Native fixed field signed_small. */
signed_small:i8=0;
/** Native fixed field unsigned_medium. */
unsigned_medium:u16=0;
/** Native fixed field signed_medium. */
signed_medium:i16=0;
/** Native fixed field unsigned_word. */
unsigned_word:u32=0;
/** Native fixed field signed_word. */
signed_word:i32=0;
/** Native fixed field unsigned_large. */
unsigned_large:u64=0;
/** Native fixed field signed_large. */
signed_large:i64=0;
/** Native fixed field single_value. */
single_value:f32=0;
/** Native fixed field double_value. */
double_value:f64=0;
}
/** Checked SDK Codec for example_interfaces_custom/ScalarRecord; empty output signals invalid encoding. */
export class Ros1Example_interfaces_customScalarRecordCodec implements Codec<Ros1Example_interfaces_customScalarRecord> {
encode(value:Ros1Example_interfaces_customScalarRecord):Uint8Array {
if(!isFinite(value.single_value))return new Uint8Array(0);
if(!isFinite(value.double_value))return new Uint8Array(0);
const w=new Writer(43);
w.u8(value.enabled?1:0);
w.integer(<u64>value.unsigned_small,1);
w.integer(<u64>value.signed_small,1);
w.integer(<u64>value.unsigned_medium,2);
w.integer(<u64>value.signed_medium,2);
w.integer(<u64>value.unsigned_word,4);
w.integer(<u64>value.signed_word,4);
w.integer(<u64>value.unsigned_large,8);
w.integer(<u64>value.signed_large,8);
w.integer(<u64>reinterpret<u32>(value.single_value),4);
w.integer(<u64>reinterpret<u64>(value.double_value),8);
return w.finish();}
/** Reject malformed bytes before decode is used by the subscription. */
valid(bytes:Uint8Array):bool {if(bytes.length!=43)return false;const r=new Reader(bytes);
if(r.u8()>1)return false;
r.integer(1);
r.integer(1);
r.integer(2);
r.integer(2);
r.integer(4);
r.integer(4);
r.integer(8);
r.integer(8);
if(!isFinite(reinterpret<f32>(<u32>r.integer(4))))return false;
if(!isFinite(reinterpret<f64>(<u64>r.integer(8))))return false;
return r.done();}
/** Decode previously validated bytes; malformed input returns a default value. */
decode(bytes:Uint8Array):Ros1Example_interfaces_customScalarRecord {const value=new Ros1Example_interfaces_customScalarRecord();if(!this.valid(bytes))return value;const r=new Reader(bytes);
value.enabled=r.u8()!=0;
value.unsigned_small=<u8>r.integer(1);
value.signed_small=<i8>r.integer(1);
value.unsigned_medium=<u16>r.integer(2);
value.signed_medium=<i16>r.integer(2);
value.unsigned_word=<u32>r.integer(4);
value.signed_word=<i32>r.integer(4);
value.unsigned_large=<u64>r.integer(8);
value.signed_large=<i64>r.integer(8);
value.single_value=reinterpret<f32>(<u32>r.integer(4));
value.double_value=reinterpret<f64>(<u64>r.integer(8));
return value;}
}
/** Fixed ROS1 example_interfaces_custom/NestedRecord value. */
export class Ros1Example_interfaces_customNestedRecord {
/** Canonical schema identity; host grants remain separate. */
static readonly codecIdentity:string="UWVMROS1_b0856889a47eb923055e44730a21c04d498472c373a1b9539a8f213a7c09ae5c";
/** Native field readings: exactly 2 values. */
readings:Array<Ros1Example_interfaces_customScalarRecord>=[new Ros1Example_interfaces_customScalarRecord(),new Ros1Example_interfaces_customScalarRecord()];
/** Native field positions: exactly 3 values. */
positions:Array<i32>=[0,0,0];
}
/** Checked SDK Codec for example_interfaces_custom/NestedRecord; empty output signals invalid encoding. */
export class Ros1Example_interfaces_customNestedRecordCodec implements Codec<Ros1Example_interfaces_customNestedRecord> {
encode(value:Ros1Example_interfaces_customNestedRecord):Uint8Array {
if(value.readings.length!=2)return new Uint8Array(0);
if(value.positions.length!=3)return new Uint8Array(0);
if(!isFinite(value.readings[0].single_value))return new Uint8Array(0);
if(!isFinite(value.readings[0].double_value))return new Uint8Array(0);
if(!isFinite(value.readings[1].single_value))return new Uint8Array(0);
if(!isFinite(value.readings[1].double_value))return new Uint8Array(0);
const w=new Writer(98);
w.u8(value.readings[0].enabled?1:0);
w.integer(<u64>value.readings[0].unsigned_small,1);
w.integer(<u64>value.readings[0].signed_small,1);
w.integer(<u64>value.readings[0].unsigned_medium,2);
w.integer(<u64>value.readings[0].signed_medium,2);
w.integer(<u64>value.readings[0].unsigned_word,4);
w.integer(<u64>value.readings[0].signed_word,4);
w.integer(<u64>value.readings[0].unsigned_large,8);
w.integer(<u64>value.readings[0].signed_large,8);
w.integer(<u64>reinterpret<u32>(value.readings[0].single_value),4);
w.integer(<u64>reinterpret<u64>(value.readings[0].double_value),8);
w.u8(value.readings[1].enabled?1:0);
w.integer(<u64>value.readings[1].unsigned_small,1);
w.integer(<u64>value.readings[1].signed_small,1);
w.integer(<u64>value.readings[1].unsigned_medium,2);
w.integer(<u64>value.readings[1].signed_medium,2);
w.integer(<u64>value.readings[1].unsigned_word,4);
w.integer(<u64>value.readings[1].signed_word,4);
w.integer(<u64>value.readings[1].unsigned_large,8);
w.integer(<u64>value.readings[1].signed_large,8);
w.integer(<u64>reinterpret<u32>(value.readings[1].single_value),4);
w.integer(<u64>reinterpret<u64>(value.readings[1].double_value),8);
w.integer(<u64>value.positions[0],4);
w.integer(<u64>value.positions[1],4);
w.integer(<u64>value.positions[2],4);
return w.finish();}
/** Reject malformed bytes before decode is used by the subscription. */
valid(bytes:Uint8Array):bool {if(bytes.length!=98)return false;const r=new Reader(bytes);
if(r.u8()>1)return false;
r.integer(1);
r.integer(1);
r.integer(2);
r.integer(2);
r.integer(4);
r.integer(4);
r.integer(8);
r.integer(8);
if(!isFinite(reinterpret<f32>(<u32>r.integer(4))))return false;
if(!isFinite(reinterpret<f64>(<u64>r.integer(8))))return false;
if(r.u8()>1)return false;
r.integer(1);
r.integer(1);
r.integer(2);
r.integer(2);
r.integer(4);
r.integer(4);
r.integer(8);
r.integer(8);
if(!isFinite(reinterpret<f32>(<u32>r.integer(4))))return false;
if(!isFinite(reinterpret<f64>(<u64>r.integer(8))))return false;
r.integer(4);
r.integer(4);
r.integer(4);
return r.done();}
/** Decode previously validated bytes; malformed input returns a default value. */
decode(bytes:Uint8Array):Ros1Example_interfaces_customNestedRecord {const value=new Ros1Example_interfaces_customNestedRecord();if(!this.valid(bytes))return value;const r=new Reader(bytes);
value.readings[0].enabled=r.u8()!=0;
value.readings[0].unsigned_small=<u8>r.integer(1);
value.readings[0].signed_small=<i8>r.integer(1);
value.readings[0].unsigned_medium=<u16>r.integer(2);
value.readings[0].signed_medium=<i16>r.integer(2);
value.readings[0].unsigned_word=<u32>r.integer(4);
value.readings[0].signed_word=<i32>r.integer(4);
value.readings[0].unsigned_large=<u64>r.integer(8);
value.readings[0].signed_large=<i64>r.integer(8);
value.readings[0].single_value=reinterpret<f32>(<u32>r.integer(4));
value.readings[0].double_value=reinterpret<f64>(<u64>r.integer(8));
value.readings[1].enabled=r.u8()!=0;
value.readings[1].unsigned_small=<u8>r.integer(1);
value.readings[1].signed_small=<i8>r.integer(1);
value.readings[1].unsigned_medium=<u16>r.integer(2);
value.readings[1].signed_medium=<i16>r.integer(2);
value.readings[1].unsigned_word=<u32>r.integer(4);
value.readings[1].signed_word=<i32>r.integer(4);
value.readings[1].unsigned_large=<u64>r.integer(8);
value.readings[1].signed_large=<i64>r.integer(8);
value.readings[1].single_value=reinterpret<f32>(<u32>r.integer(4));
value.readings[1].double_value=reinterpret<f64>(<u64>r.integer(8));
value.positions[0]=<i32>r.integer(4);
value.positions[1]=<i32>r.integer(4);
value.positions[2]=<i32>r.integer(4);
return value;}
}
/** Fixed ROS2 example_interfaces_custom/msg/FixedSample value. */
export class Ros2Example_interfaces_customFixedSample {
/** Canonical schema identity; host grants remain separate. */
static readonly codecIdentity:string="UWVMROS1_b794a6eb1863626cf279af1fbad1f071fce945d6585f3fdb22979c8f24e32663";
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
/** Checked SDK Codec for example_interfaces_custom/msg/FixedSample; empty output signals invalid encoding. */
export class Ros2Example_interfaces_customFixedSampleCodec implements Codec<Ros2Example_interfaces_customFixedSample> {
encode(value:Ros2Example_interfaces_customFixedSample):Uint8Array {
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
decode(bytes:Uint8Array):Ros2Example_interfaces_customFixedSample {const value=new Ros2Example_interfaces_customFixedSample();if(!this.valid(bytes))return value;const r=new Reader(bytes);
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
