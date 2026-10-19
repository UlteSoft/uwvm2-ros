/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original builtin_interfaces/msg/Duration; spellings are published in generation proof. */
export class Ros2Builtin_interfacesMsgDuration {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field sec, native scalar int32. */
sec:i32=0;
/** Exact native field nanosec, native scalar uint32. */
nanosec:u32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>8-n)return -90;n+=4;
if(n>8)return -90;
}
{
if(4>8-n)return -90;n+=4;
if(n>8)return -90;
}
if(this.nanosec>=1000000000)return -22;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.sec,4);
w.integer(<u64>this.nanosec,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Builtin_interfacesMsgDuration{const value=new Ros2Builtin_interfacesMsgDuration();
{
const decoded=<i32>r.integer(4);
value.sec=decoded;
}
{
const decoded=<u32>r.integer(4);
value.nanosec=decoded;
}
if(value.nanosec>=1000000000)r.valid=false;
return value;}
}
export class Ros2Builtin_interfacesMsgDurationCodec implements VendorCodec<Ros2Builtin_interfacesMsgDuration>{
static readonly codecIdentity:string="UWVMROS1_3804444fae7a82a57907d25c2c3dc54d9a42dcd305f41ca073b1221dca7d36e4";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Builtin_interfacesMsgDuration):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Builtin_interfacesMsgDuration):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Builtin_interfacesMsgDuration>{const out=new VendorDecoded<Ros2Builtin_interfacesMsgDuration>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Builtin_interfacesMsgDuration.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Builtin_interfacesMsgDuration{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Builtin_interfacesMsgDuration():changetype<Ros2Builtin_interfacesMsgDuration>(out.value);}
}
/** Complete original builtin_interfaces/msg/Time; spellings are published in generation proof. */
export class Ros2Builtin_interfacesMsgTime {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field sec, native scalar int32. */
sec:i32=0;
/** Exact native field nanosec, native scalar uint32. */
nanosec:u32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>8-n)return -90;n+=4;
if(n>8)return -90;
}
{
if(4>8-n)return -90;n+=4;
if(n>8)return -90;
}
if(this.nanosec>=1000000000)return -22;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.sec,4);
w.integer(<u64>this.nanosec,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Builtin_interfacesMsgTime{const value=new Ros2Builtin_interfacesMsgTime();
{
const decoded=<i32>r.integer(4);
value.sec=decoded;
}
{
const decoded=<u32>r.integer(4);
value.nanosec=decoded;
}
if(value.nanosec>=1000000000)r.valid=false;
return value;}
}
export class Ros2Builtin_interfacesMsgTimeCodec implements VendorCodec<Ros2Builtin_interfacesMsgTime>{
static readonly codecIdentity:string="UWVMROS1_0fd520c23fd8cb06e2c27292fdc20694732fa9332d452fc3fa31da90f7605fb7";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Builtin_interfacesMsgTime):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Builtin_interfacesMsgTime):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Builtin_interfacesMsgTime>{const out=new VendorDecoded<Ros2Builtin_interfacesMsgTime>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Builtin_interfacesMsgTime.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Builtin_interfacesMsgTime{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Builtin_interfacesMsgTime():changetype<Ros2Builtin_interfacesMsgTime>(out.value);}
}
/** Complete original control_msgs/msg/JointTolerance; spellings are published in generation proof. */
export class Ros2Control_msgsMsgJointTolerance {
static readonly maxWireBytes:i32=156;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=156;
/** Exact native field name, native scalar string. */
name:string="";
/** Exact native field position, native scalar float64. */
position:f64=0;
/** Exact native field velocity, native scalar float64. */
velocity:f64=0;
/** Exact native field acceleration, native scalar float64. */
acceleration:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.name)||this.name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name);if(size>128)return -22;if(4+size>156-n)return -90;n+=4+size;
if(n>156)return -90;
}
{
if(!isFinite(this.position))return -22;
if(8>156-n)return -90;n+=8;
if(n>156)return -90;
}
{
if(!isFinite(this.velocity))return -22;
if(8>156-n)return -90;n+=8;
if(n>156)return -90;
}
{
if(!isFinite(this.acceleration))return -22;
if(8>156-n)return -90;n+=8;
if(n>156)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.name);
w.integer(<u64>reinterpret<u64>(this.position),8);
w.integer(<u64>reinterpret<u64>(this.velocity),8);
w.integer(<u64>reinterpret<u64>(this.acceleration),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Control_msgsMsgJointTolerance{const value=new Ros2Control_msgsMsgJointTolerance();
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.name=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.position=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.velocity=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.acceleration=decoded;
}
return value;}
}
export class Ros2Control_msgsMsgJointToleranceCodec implements VendorCodec<Ros2Control_msgsMsgJointTolerance>{
static readonly codecIdentity:string="UWVMROS1_3864af88029b3b1f51907b3da63d2d821104c29b9469a146a5a34f73f3598b8a";
static readonly maxWireBytes:i32=156;
encodeChecked(value:Ros2Control_msgsMsgJointTolerance):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Control_msgsMsgJointTolerance):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Control_msgsMsgJointTolerance>{const out=new VendorDecoded<Ros2Control_msgsMsgJointTolerance>();if(bytes.length>156){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Control_msgsMsgJointTolerance.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Control_msgsMsgJointTolerance{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Control_msgsMsgJointTolerance():changetype<Ros2Control_msgsMsgJointTolerance>(out.value);}
}
/** Complete original geometry_msgs/msg/Point; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgPoint {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float64. */
x:f64=0;
/** Exact native field y, native scalar float64. */
y:f64=0;
/** Exact native field z, native scalar float64. */
z:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.x),8);
w.integer(<u64>reinterpret<u64>(this.y),8);
w.integer(<u64>reinterpret<u64>(this.z),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgPoint{const value=new Ros2Geometry_msgsMsgPoint();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.x=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.y=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.z=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgPointCodec implements VendorCodec<Ros2Geometry_msgsMsgPoint>{
static readonly codecIdentity:string="UWVMROS1_80f43d054abe333673c940c6d0fd88e8e26cea4a1cff53ff789cccaa38240616";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Geometry_msgsMsgPoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgPoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgPoint>{const out=new VendorDecoded<Ros2Geometry_msgsMsgPoint>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgPoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgPoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgPoint():changetype<Ros2Geometry_msgsMsgPoint>(out.value);}
}
/** Complete original geometry_msgs/msg/Quaternion; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgQuaternion {
static readonly maxWireBytes:i32=32;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=32;
/** Exact native field x, native scalar float64. */
x:f64=reinterpret<f64>(u64(0x0000000000000000));
/** Exact native field y, native scalar float64. */
y:f64=reinterpret<f64>(u64(0x0000000000000000));
/** Exact native field z, native scalar float64. */
z:f64=reinterpret<f64>(u64(0x0000000000000000));
/** Exact native field w, native scalar float64. */
w:f64=reinterpret<f64>(u64(0x3ff0000000000000));
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.y))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.z))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.w))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.x),8);
w.integer(<u64>reinterpret<u64>(this.y),8);
w.integer(<u64>reinterpret<u64>(this.z),8);
w.integer(<u64>reinterpret<u64>(this.w),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgQuaternion{const value=new Ros2Geometry_msgsMsgQuaternion();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.x=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.y=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.z=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.w=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgQuaternionCodec implements VendorCodec<Ros2Geometry_msgsMsgQuaternion>{
static readonly codecIdentity:string="UWVMROS1_34a32c3470cbfe2ff12340781a5bf22f6c370a8f5c06669f25a34321ade81672";
static readonly maxWireBytes:i32=32;
encodeChecked(value:Ros2Geometry_msgsMsgQuaternion):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgQuaternion):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgQuaternion>{const out=new VendorDecoded<Ros2Geometry_msgsMsgQuaternion>();if(bytes.length>32){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgQuaternion.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgQuaternion{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgQuaternion():changetype<Ros2Geometry_msgsMsgQuaternion>(out.value);}
}
/** Complete original geometry_msgs/msg/Pose; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgPose {
static readonly maxWireBytes:i32=56;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=56;
/** Exact native field position, native scalar geometry_msgs/msg/Point. */
position:Ros2Geometry_msgsMsgPoint=new Ros2Geometry_msgsMsgPoint();
/** Exact native field orientation, native scalar geometry_msgs/msg/Quaternion. */
orientation:Ros2Geometry_msgsMsgQuaternion=new Ros2Geometry_msgsMsgQuaternion();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.position.wireSize();if(child<0)return child;if(child>56-n)return -90;n+=child;
if(n>56)return -90;
}
{
const child=this.orientation.wireSize();if(child<0)return child;if(child>56-n)return -90;n+=child;
if(n>56)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.position.writeControl(w);
this.orientation.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgPose{const value=new Ros2Geometry_msgsMsgPose();
{
const decoded=Ros2Geometry_msgsMsgPoint.readControl(r);
value.position=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgQuaternion.readControl(r);
value.orientation=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgPoseCodec implements VendorCodec<Ros2Geometry_msgsMsgPose>{
static readonly codecIdentity:string="UWVMROS1_9468a10b8b2728fe71a370b4348ffe5e81b884ff957533f8700aa16e70b12777";
static readonly maxWireBytes:i32=56;
encodeChecked(value:Ros2Geometry_msgsMsgPose):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgPose):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgPose>{const out=new VendorDecoded<Ros2Geometry_msgsMsgPose>();if(bytes.length>56){out.status=90;return out;}if(bytes.length<56){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgPose.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgPose{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgPose():changetype<Ros2Geometry_msgsMsgPose>(out.value);}
}
/** Complete original std_msgs/msg/Header; spellings are published in generation proof. */
export class Ros2Std_msgsMsgHeader {
static readonly maxWireBytes:i32=140;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=140;
/** Exact native field stamp, native scalar builtin_interfaces/msg/Time. */
stamp:Ros2Builtin_interfacesMsgTime=new Ros2Builtin_interfacesMsgTime();
/** Exact native field frame_id, native scalar string. */
frame_id:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.stamp.wireSize();if(child<0)return child;if(child>140-n)return -90;n+=child;
if(n>140)return -90;
}
{
if(!validUnicode(this.frame_id)||this.frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.frame_id);if(size>128)return -22;if(4+size>140-n)return -90;n+=4+size;
if(n>140)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.stamp.writeControl(w);
w.text(this.frame_id);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Std_msgsMsgHeader{const value=new Ros2Std_msgsMsgHeader();
{
const decoded=Ros2Builtin_interfacesMsgTime.readControl(r);
value.stamp=decoded;
}
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.frame_id=decoded;
}
return value;}
}
export class Ros2Std_msgsMsgHeaderCodec implements VendorCodec<Ros2Std_msgsMsgHeader>{
static readonly codecIdentity:string="UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96";
static readonly maxWireBytes:i32=140;
encodeChecked(value:Ros2Std_msgsMsgHeader):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Std_msgsMsgHeader):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Std_msgsMsgHeader>{const out=new VendorDecoded<Ros2Std_msgsMsgHeader>();if(bytes.length>140){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Std_msgsMsgHeader.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Std_msgsMsgHeader{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Std_msgsMsgHeader():changetype<Ros2Std_msgsMsgHeader>(out.value);}
}
/** Complete original geometry_msgs/msg/PoseStamped; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgPoseStamped {
static readonly maxWireBytes:i32=196;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=196;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field pose, native scalar geometry_msgs/msg/Pose. */
pose:Ros2Geometry_msgsMsgPose=new Ros2Geometry_msgsMsgPose();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>196-n)return -90;n+=child;
if(n>196)return -90;
}
{
const child=this.pose.wireSize();if(child<0)return child;if(child>196-n)return -90;n+=child;
if(n>196)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.pose.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgPoseStamped{const value=new Ros2Geometry_msgsMsgPoseStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPose.readControl(r);
value.pose=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgPoseStampedCodec implements VendorCodec<Ros2Geometry_msgsMsgPoseStamped>{
static readonly codecIdentity:string="UWVMROS1_71abdc844f18f02f45a354967a5f1c644fb20dc14cbc61c64aa41b927e0712a2";
static readonly maxWireBytes:i32=196;
encodeChecked(value:Ros2Geometry_msgsMsgPoseStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgPoseStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgPoseStamped>{const out=new VendorDecoded<Ros2Geometry_msgsMsgPoseStamped>();if(bytes.length>196){out.status=90;return out;}if(bytes.length<68){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgPoseStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgPoseStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgPoseStamped():changetype<Ros2Geometry_msgsMsgPoseStamped>(out.value);}
}
/** Complete original geometry_msgs/msg/Vector3; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgVector3 {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float64. */
x:f64=0;
/** Exact native field y, native scalar float64. */
y:f64=0;
/** Exact native field z, native scalar float64. */
z:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(8>24-n)return -90;n+=8;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.x),8);
w.integer(<u64>reinterpret<u64>(this.y),8);
w.integer(<u64>reinterpret<u64>(this.z),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgVector3{const value=new Ros2Geometry_msgsMsgVector3();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.x=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.y=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.z=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgVector3Codec implements VendorCodec<Ros2Geometry_msgsMsgVector3>{
static readonly codecIdentity:string="UWVMROS1_5adebf211fe375fba92c0628f9b8ebf05bff9ab22ddc4bbf00629167af295d5e";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Geometry_msgsMsgVector3):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgVector3):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgVector3>{const out=new VendorDecoded<Ros2Geometry_msgsMsgVector3>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgVector3.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgVector3{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgVector3():changetype<Ros2Geometry_msgsMsgVector3>(out.value);}
}
/** Complete original geometry_msgs/msg/Twist; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgTwist {
static readonly maxWireBytes:i32=48;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=48;
/** Exact native field linear, native scalar geometry_msgs/msg/Vector3. */
linear:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field angular, native scalar geometry_msgs/msg/Vector3. */
angular:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.linear.wireSize();if(child<0)return child;if(child>48-n)return -90;n+=child;
if(n>48)return -90;
}
{
const child=this.angular.wireSize();if(child<0)return child;if(child>48-n)return -90;n+=child;
if(n>48)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.linear.writeControl(w);
this.angular.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgTwist{const value=new Ros2Geometry_msgsMsgTwist();
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.linear=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.angular=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgTwistCodec implements VendorCodec<Ros2Geometry_msgsMsgTwist>{
static readonly codecIdentity:string="UWVMROS1_1a6c18d19bb6853a2cff3b775974ba31f8e3674c1bbe566444ccabf0ef9d117e";
static readonly maxWireBytes:i32=48;
encodeChecked(value:Ros2Geometry_msgsMsgTwist):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgTwist):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgTwist>{const out=new VendorDecoded<Ros2Geometry_msgsMsgTwist>();if(bytes.length>48){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgTwist.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgTwist{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgTwist():changetype<Ros2Geometry_msgsMsgTwist>(out.value);}
}
/** Complete original geometry_msgs/msg/Vector3Stamped; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgVector3Stamped {
static readonly maxWireBytes:i32=164;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=164;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field vector, native scalar geometry_msgs/msg/Vector3. */
vector:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>164-n)return -90;n+=child;
if(n>164)return -90;
}
{
const child=this.vector.wireSize();if(child<0)return child;if(child>164-n)return -90;n+=child;
if(n>164)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.vector.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgVector3Stamped{const value=new Ros2Geometry_msgsMsgVector3Stamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.vector=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgVector3StampedCodec implements VendorCodec<Ros2Geometry_msgsMsgVector3Stamped>{
static readonly codecIdentity:string="UWVMROS1_c6b67646d3fc6d29c195637ea353dafa7085b2bdb55a15a8b65009200b5ed31b";
static readonly maxWireBytes:i32=164;
encodeChecked(value:Ros2Geometry_msgsMsgVector3Stamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgVector3Stamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgVector3Stamped>{const out=new VendorDecoded<Ros2Geometry_msgsMsgVector3Stamped>();if(bytes.length>164){out.status=90;return out;}if(bytes.length<36){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgVector3Stamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgVector3Stamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgVector3Stamped():changetype<Ros2Geometry_msgsMsgVector3Stamped>(out.value);}
}
/** Complete original geometry_msgs/msg/Wrench; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgWrench {
static readonly maxWireBytes:i32=48;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=48;
/** Exact native field force, native scalar geometry_msgs/msg/Vector3. */
force:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field torque, native scalar geometry_msgs/msg/Vector3. */
torque:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.force.wireSize();if(child<0)return child;if(child>48-n)return -90;n+=child;
if(n>48)return -90;
}
{
const child=this.torque.wireSize();if(child<0)return child;if(child>48-n)return -90;n+=child;
if(n>48)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.force.writeControl(w);
this.torque.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgWrench{const value=new Ros2Geometry_msgsMsgWrench();
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.force=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.torque=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgWrenchCodec implements VendorCodec<Ros2Geometry_msgsMsgWrench>{
static readonly codecIdentity:string="UWVMROS1_fdfdf2474c0129b0ce1f85972cc87fd80478ac7d2429d9de1ec58951a82a11b6";
static readonly maxWireBytes:i32=48;
encodeChecked(value:Ros2Geometry_msgsMsgWrench):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgWrench):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgWrench>{const out=new VendorDecoded<Ros2Geometry_msgsMsgWrench>();if(bytes.length>48){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgWrench.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgWrench{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgWrench():changetype<Ros2Geometry_msgsMsgWrench>(out.value);}
}
/** Complete original trajectory_msgs/msg/JointTrajectoryPoint; spellings are published in generation proof. */
export class Ros2Trajectory_msgsMsgJointTrajectoryPoint {
static readonly maxWireBytes:i32=216;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=216;
/** Exact native field positions, native scalar float64. */
positions:Array<f64>=new Array<f64>();
/** Exact native field velocities, native scalar float64. */
velocities:Array<f64>=new Array<f64>();
/** Exact native field accelerations, native scalar float64. */
accelerations:Array<f64>=new Array<f64>();
/** Exact native field effort, native scalar float64. */
effort:Array<f64>=new Array<f64>();
/** Exact native field time_from_start, native scalar builtin_interfaces/msg/Duration. */
time_from_start:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.positions.length>6)return -22;
if(4>216-n)return -90;n+=4;
for(let i=0;i<this.positions.length;++i){
if(!isFinite(this.positions[i]))return -22;
if(8>216-n)return -90;n+=8;
if(n>216)return -90;
}
}
{
if(this.velocities.length>6)return -22;
if(4>216-n)return -90;n+=4;
for(let i=0;i<this.velocities.length;++i){
if(!isFinite(this.velocities[i]))return -22;
if(8>216-n)return -90;n+=8;
if(n>216)return -90;
}
}
{
if(this.accelerations.length>6)return -22;
if(4>216-n)return -90;n+=4;
for(let i=0;i<this.accelerations.length;++i){
if(!isFinite(this.accelerations[i]))return -22;
if(8>216-n)return -90;n+=8;
if(n>216)return -90;
}
}
{
if(this.effort.length>6)return -22;
if(4>216-n)return -90;n+=4;
for(let i=0;i<this.effort.length;++i){
if(!isFinite(this.effort[i]))return -22;
if(8>216-n)return -90;n+=8;
if(n>216)return -90;
}
}
{
const child=this.time_from_start.wireSize();if(child<0)return child;if(child>216-n)return -90;n+=child;
if(n>216)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.positions.length);
for(let i=0;i<this.positions.length;++i){
w.integer(<u64>reinterpret<u64>(this.positions[i]),8);
}
w.u32(<u32>this.velocities.length);
for(let i=0;i<this.velocities.length;++i){
w.integer(<u64>reinterpret<u64>(this.velocities[i]),8);
}
w.u32(<u32>this.accelerations.length);
for(let i=0;i<this.accelerations.length;++i){
w.integer(<u64>reinterpret<u64>(this.accelerations[i]),8);
}
w.u32(<u32>this.effort.length);
for(let i=0;i<this.effort.length;++i){
w.integer(<u64>reinterpret<u64>(this.effort[i]),8);
}
this.time_from_start.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Trajectory_msgsMsgJointTrajectoryPoint{const value=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.positions=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.positions.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.velocities=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.velocities.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.accelerations=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.accelerations.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.effort=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.effort.push(decoded);
}
}
{
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.time_from_start=decoded;
}
return value;}
}
export class Ros2Trajectory_msgsMsgJointTrajectoryPointCodec implements VendorCodec<Ros2Trajectory_msgsMsgJointTrajectoryPoint>{
static readonly codecIdentity:string="UWVMROS1_c4eafb6efd22d299b5c6fc21844d8bf69789cf27ee5414a1d7d2371f0b1acbbf";
static readonly maxWireBytes:i32=216;
encodeChecked(value:Ros2Trajectory_msgsMsgJointTrajectoryPoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Trajectory_msgsMsgJointTrajectoryPoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectoryPoint>{const out=new VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectoryPoint>();if(bytes.length>216){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Trajectory_msgsMsgJointTrajectoryPoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Trajectory_msgsMsgJointTrajectoryPoint():changetype<Ros2Trajectory_msgsMsgJointTrajectoryPoint>(out.value);}
}
/** Complete original trajectory_msgs/msg/JointTrajectory; spellings are published in generation proof. */
export class Ros2Trajectory_msgsMsgJointTrajectory {
static readonly maxWireBytes:i32=5164;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5164;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field points, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
points:Array<Ros2Trajectory_msgsMsgJointTrajectoryPoint>=new Array<Ros2Trajectory_msgsMsgJointTrajectoryPoint>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>5164-n)return -90;n+=child;
if(n>5164)return -90;
}
{
if(this.joint_names.length>6)return -22;
if(4>5164-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>256)return -22;if(4+size>5164-n)return -90;n+=4+size;
if(n>5164)return -90;
}
}
{
if(this.points.length>16)return -22;
if(4>5164-n)return -90;n+=4;
for(let i=0;i<this.points.length;++i){
const child=this.points[i].wireSize();if(child<0)return child;if(child>5164-n)return -90;n+=child;
if(n>5164)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.joint_names.length);
for(let i=0;i<this.joint_names.length;++i){
w.text(this.joint_names[i]);
}
w.u32(<u32>this.points.length);
for(let i=0;i<this.points.length;++i){
this.points[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Trajectory_msgsMsgJointTrajectory{const value=new Ros2Trajectory_msgsMsgJointTrajectory();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.joint_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_names.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.points=new Array<Ros2Trajectory_msgsMsgJointTrajectoryPoint>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.points.push(decoded);
}
}
return value;}
}
export class Ros2Trajectory_msgsMsgJointTrajectoryCodec implements VendorCodec<Ros2Trajectory_msgsMsgJointTrajectory>{
static readonly codecIdentity:string="UWVMROS1_e77c37c75123e442edb6a7f6f2a21dcbe33d8bb8fd2577f9906653a6ad5f8225";
static readonly maxWireBytes:i32=5164;
encodeChecked(value:Ros2Trajectory_msgsMsgJointTrajectory):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Trajectory_msgsMsgJointTrajectory):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectory>{const out=new VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectory>();if(bytes.length>5164){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Trajectory_msgsMsgJointTrajectory.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Trajectory_msgsMsgJointTrajectory{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Trajectory_msgsMsgJointTrajectory():changetype<Ros2Trajectory_msgsMsgJointTrajectory>(out.value);}
}
/** Complete original ur_dashboard_msgs/action/SetMode_Goal; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsActionSetMode_Goal {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field target_robot_mode, native scalar int8. */
target_robot_mode:i8=0;
/** Exact native field stop_program, native scalar bool. */
stop_program:bool=false;
/** Exact native field play_program, native scalar bool. */
play_program:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>3-n)return -90;n+=1;
if(n>3)return -90;
}
{
if(1>3-n)return -90;n+=1;
if(n>3)return -90;
}
{
if(1>3-n)return -90;n+=1;
if(n>3)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.target_robot_mode,1);
w.u8(this.stop_program?1:0);
w.u8(this.play_program?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsActionSetMode_Goal{const value=new Ros2Ur_dashboard_msgsActionSetMode_Goal();
{
const decoded=<i8>r.integer(1);
value.target_robot_mode=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.stop_program=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.play_program=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsActionSetMode_GoalCodec implements VendorCodec<Ros2Ur_dashboard_msgsActionSetMode_Goal>{
static readonly codecIdentity:string="UWVMROS1_8a7484a60bad21933df3f847d3fd554e41098ed596dee270335ce9f092dc313d";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros2Ur_dashboard_msgsActionSetMode_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsActionSetMode_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsActionSetMode_Goal>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsActionSetMode_Goal>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsActionSetMode_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsActionSetMode_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsActionSetMode_Goal():changetype<Ros2Ur_dashboard_msgsActionSetMode_Goal>(out.value);}
}
/** Complete original ur_dashboard_msgs/action/SetMode_Result; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsActionSetMode_Result {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsActionSetMode_Result{const value=new Ros2Ur_dashboard_msgsActionSetMode_Result();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsActionSetMode_ResultCodec implements VendorCodec<Ros2Ur_dashboard_msgsActionSetMode_Result>{
static readonly codecIdentity:string="UWVMROS1_ef2a557ff27171319d46170ab16a80f5cba5fbd59e44c21cc1b704d01764b913";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Ur_dashboard_msgsActionSetMode_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsActionSetMode_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsActionSetMode_Result>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsActionSetMode_Result>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsActionSetMode_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsActionSetMode_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsActionSetMode_Result():changetype<Ros2Ur_dashboard_msgsActionSetMode_Result>(out.value);}
}
/** Complete original ur_dashboard_msgs/action/SetMode_Feedback; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsActionSetMode_Feedback {
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field current_robot_mode, native scalar int8. */
current_robot_mode:i8=0;
/** Exact native field current_safety_mode, native scalar int8. */
current_safety_mode:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>2-n)return -90;n+=1;
if(n>2)return -90;
}
{
if(1>2-n)return -90;n+=1;
if(n>2)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.current_robot_mode,1);
w.integer(<u64>this.current_safety_mode,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsActionSetMode_Feedback{const value=new Ros2Ur_dashboard_msgsActionSetMode_Feedback();
{
const decoded=<i8>r.integer(1);
value.current_robot_mode=decoded;
}
{
const decoded=<i8>r.integer(1);
value.current_safety_mode=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsActionSetMode_FeedbackCodec implements VendorCodec<Ros2Ur_dashboard_msgsActionSetMode_Feedback>{
static readonly codecIdentity:string="UWVMROS1_afd74cd0a79174b53499a40b23b040ba96ec3d3e332129999525dcf6ca287ac6";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Ur_dashboard_msgsActionSetMode_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsActionSetMode_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsActionSetMode_Feedback>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsActionSetMode_Feedback>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsActionSetMode_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsActionSetMode_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsActionSetMode_Feedback():changetype<Ros2Ur_dashboard_msgsActionSetMode_Feedback>(out.value);}
}
/** Complete original ur_dashboard_msgs/msg/OperationalMode; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsMsgOperationalMode {
static readonly MANUAL:string="MANUAL";
static readonly AUTOMATIC:string="AUTOMATIC";
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field mode, native scalar string. */
mode:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.mode)||this.mode.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.mode);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.mode);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsMsgOperationalMode{const value=new Ros2Ur_dashboard_msgsMsgOperationalMode();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.mode=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsMsgOperationalModeCodec implements VendorCodec<Ros2Ur_dashboard_msgsMsgOperationalMode>{
static readonly codecIdentity:string="UWVMROS1_7a0be8522cdc20e01d382218499030176a0f6565376fadc5bbdd871a01b9316e";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsMsgOperationalMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsMsgOperationalMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsMsgOperationalMode>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsMsgOperationalMode>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsMsgOperationalMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsMsgOperationalMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsMsgOperationalMode():changetype<Ros2Ur_dashboard_msgsMsgOperationalMode>(out.value);}
}
/** Complete original ur_dashboard_msgs/msg/ProgramInformation; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsMsgProgramInformation {
static readonly PROGRAM_STATE_DRAFT:string="DRAFT";
static readonly PROGRAM_STATE_FINAL:string="FINAL";
static readonly maxWireBytes:i32=676;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=676;
/** Exact native field name, native scalar string. */
name:string="";
/** Exact native field description, native scalar string. */
description:string="";
/** Exact native field created_date, native scalar uint64. */
created_date:u64=0;
/** Exact native field last_saved_date, native scalar uint64. */
last_saved_date:u64=0;
/** Exact native field last_modified_date, native scalar uint64. */
last_modified_date:u64=0;
/** Exact native field program_state, native scalar string. */
program_state:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.name)||this.name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name);if(size>128)return -22;if(4+size>676-n)return -90;n+=4+size;
if(n>676)return -90;
}
{
if(!validUnicode(this.description)||this.description.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.description);if(size>256)return -22;if(4+size>676-n)return -90;n+=4+size;
if(n>676)return -90;
}
{
if(8>676-n)return -90;n+=8;
if(n>676)return -90;
}
{
if(8>676-n)return -90;n+=8;
if(n>676)return -90;
}
{
if(8>676-n)return -90;n+=8;
if(n>676)return -90;
}
{
if(!validUnicode(this.program_state)||this.program_state.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.program_state);if(size>256)return -22;if(4+size>676-n)return -90;n+=4+size;
if(n>676)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.name);
w.text(this.description);
w.integer(<u64>this.created_date,8);
w.integer(<u64>this.last_saved_date,8);
w.integer(<u64>this.last_modified_date,8);
w.text(this.program_state);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsMsgProgramInformation{const value=new Ros2Ur_dashboard_msgsMsgProgramInformation();
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.name=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.description=decoded;
}
{
const decoded=<u64>r.integer(8);
value.created_date=decoded;
}
{
const decoded=<u64>r.integer(8);
value.last_saved_date=decoded;
}
{
const decoded=<u64>r.integer(8);
value.last_modified_date=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.program_state=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsMsgProgramInformationCodec implements VendorCodec<Ros2Ur_dashboard_msgsMsgProgramInformation>{
static readonly codecIdentity:string="UWVMROS1_9299ce545acd0b6ff064998334820fbe503b71dc47a475ef93545d2cdb41f965";
static readonly maxWireBytes:i32=676;
encodeChecked(value:Ros2Ur_dashboard_msgsMsgProgramInformation):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsMsgProgramInformation):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsMsgProgramInformation>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsMsgProgramInformation>();if(bytes.length>676){out.status=90;return out;}if(bytes.length<36){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsMsgProgramInformation.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsMsgProgramInformation{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsMsgProgramInformation():changetype<Ros2Ur_dashboard_msgsMsgProgramInformation>(out.value);}
}
/** Complete original ur_dashboard_msgs/msg/ProgramState; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsMsgProgramState {
static readonly STOPPED:string="STOPPED";
static readonly PLAYING:string="PLAYING";
static readonly PAUSED:string="PAUSED";
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field state, native scalar string. */
state:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.state)||this.state.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.state);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.state);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsMsgProgramState{const value=new Ros2Ur_dashboard_msgsMsgProgramState();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.state=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsMsgProgramStateCodec implements VendorCodec<Ros2Ur_dashboard_msgsMsgProgramState>{
static readonly codecIdentity:string="UWVMROS1_1462a3c2323792afbbfc6d48a600a6ab66a50a6f6f583e57a62da2458ea81f26";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsMsgProgramState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsMsgProgramState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsMsgProgramState>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsMsgProgramState>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsMsgProgramState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsMsgProgramState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsMsgProgramState():changetype<Ros2Ur_dashboard_msgsMsgProgramState>(out.value);}
}
/** Complete original ur_dashboard_msgs/msg/RobotMode; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsMsgRobotMode {
static readonly NO_CONTROLLER:i8=-1;
static readonly DISCONNECTED:i8=0;
static readonly CONFIRM_SAFETY:i8=1;
static readonly BOOTING:i8=2;
static readonly POWER_OFF:i8=3;
static readonly POWER_ON:i8=4;
static readonly IDLE:i8=5;
static readonly BACKDRIVE:i8=6;
static readonly RUNNING:i8=7;
static readonly UPDATING_FIRMWARE:i8=8;
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field mode, native scalar int8. */
mode:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>1-n)return -90;n+=1;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.mode,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsMsgRobotMode{const value=new Ros2Ur_dashboard_msgsMsgRobotMode();
{
const decoded=<i8>r.integer(1);
value.mode=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsMsgRobotModeCodec implements VendorCodec<Ros2Ur_dashboard_msgsMsgRobotMode>{
static readonly codecIdentity:string="UWVMROS1_81a79c84083520d911de108a728ad1877de78a57ec9e27b5cc432590be4616bb";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ur_dashboard_msgsMsgRobotMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsMsgRobotMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsMsgRobotMode>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsMsgRobotMode>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsMsgRobotMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsMsgRobotMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsMsgRobotMode():changetype<Ros2Ur_dashboard_msgsMsgRobotMode>(out.value);}
}
/** Complete original ur_dashboard_msgs/msg/SafetyMode; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsMsgSafetyMode {
static readonly NORMAL:u8=1;
static readonly REDUCED:u8=2;
static readonly PROTECTIVE_STOP:u8=3;
static readonly RECOVERY:u8=4;
static readonly SAFEGUARD_STOP:u8=5;
static readonly SYSTEM_EMERGENCY_STOP:u8=6;
static readonly ROBOT_EMERGENCY_STOP:u8=7;
static readonly VIOLATION:u8=8;
static readonly FAULT:u8=9;
static readonly VALIDATE_JOINT_ID:u8=10;
static readonly UNDEFINED_SAFETY_MODE:u8=11;
static readonly AUTOMATIC_MODE_SAFEGUARD_STOP:u8=12;
static readonly SYSTEM_THREE_POSITION_ENABLING_STOP:u8=13;
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>1-n)return -90;n+=1;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.mode,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsMsgSafetyMode{const value=new Ros2Ur_dashboard_msgsMsgSafetyMode();
{
const decoded=<u8>r.integer(1);
value.mode=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsMsgSafetyModeCodec implements VendorCodec<Ros2Ur_dashboard_msgsMsgSafetyMode>{
static readonly codecIdentity:string="UWVMROS1_2eb3d5075a2d7d1765a604c1d1d198379010867d83a98d0327488ca51f5e1aac";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ur_dashboard_msgsMsgSafetyMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsMsgSafetyMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsMsgSafetyMode>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsMsgSafetyMode>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsMsgSafetyMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsMsgSafetyMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsMsgSafetyMode():changetype<Ros2Ur_dashboard_msgsMsgSafetyMode>(out.value);}
}
/** Complete original ur_dashboard_msgs/msg/SafetyStatus; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsMsgSafetyStatus {
static readonly NORMAL:i8=1;
static readonly REDUCED:i8=2;
static readonly PROTECTIVE_STOP:i8=3;
static readonly RECOVERY:i8=4;
static readonly SAFEGUARD_STOP:i8=5;
static readonly SYSTEM_EMERGENCY_STOP:i8=6;
static readonly ROBOT_EMERGENCY_STOP:i8=7;
static readonly VIOLATION:i8=8;
static readonly FAULT:i8=9;
static readonly VALIDATE_JOINT_ID:i8=10;
static readonly UNDEFINED_SAFETY_MODE:i8=11;
static readonly AUTOMATIC_MODE_SAFEGUARD_STOP:i8=12;
static readonly SYSTEM_THREE_POSITION_ENABLING_STOP:i8=13;
static readonly TP_THREE_POSITION_ENABLING_STOP:i8=14;
static readonly IMMI_EMERGENCY_STOP:i8=15;
static readonly IMMI_SAFEGUARD_STOP:i8=16;
static readonly PROFISAFE_WAITING_FOR_PARAMETERS:i8=17;
static readonly PROFISAFE_AUTOMATIC_MODE_SAFEGUARD_STOP:i8=18;
static readonly PROFISAFE_SAFEGUARD_STOP:i8=19;
static readonly PROFISAFE_EMERGENCY_STOP:i8=20;
static readonly SAFETY_API_SAFEGUARD_STOP:i8=22;
static readonly IO_PLANE_STOP:i8=23;
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar int8. */
status:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>1-n)return -90;n+=1;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.status,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsMsgSafetyStatus{const value=new Ros2Ur_dashboard_msgsMsgSafetyStatus();
{
const decoded=<i8>r.integer(1);
value.status=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsMsgSafetyStatusCodec implements VendorCodec<Ros2Ur_dashboard_msgsMsgSafetyStatus>{
static readonly codecIdentity:string="UWVMROS1_1b1a7257c40d9c62e0b6291f241825d029fe38f3f3695265a182a270899a79e9";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ur_dashboard_msgsMsgSafetyStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsMsgSafetyStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsMsgSafetyStatus>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsMsgSafetyStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsMsgSafetyStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsMsgSafetyStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsMsgSafetyStatus():changetype<Ros2Ur_dashboard_msgsMsgSafetyStatus>(out.value);}
}
/** Complete original ur_dashboard_msgs/msg/UserRole; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsMsgUserRole {
static readonly PROGRAMMER:string="PROGRAMMER";
static readonly OPERATOR:string="OPERATOR";
static readonly NONE:string="NONE";
static readonly LOCKED:string="LOCKED";
static readonly RESTRICTED:string="RESTRICTED";
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field role, native scalar string. */
role:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.role)||this.role.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.role);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.role);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsMsgUserRole{const value=new Ros2Ur_dashboard_msgsMsgUserRole();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.role=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsMsgUserRoleCodec implements VendorCodec<Ros2Ur_dashboard_msgsMsgUserRole>{
static readonly codecIdentity:string="UWVMROS1_7568bc9417685a9bc56cfdd2b41ff81ab99bd8e20fbe5f9e537b5b5d0689440a";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsMsgUserRole):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsMsgUserRole):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsMsgUserRole>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsMsgUserRole>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsMsgUserRole.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsMsgUserRole{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsMsgUserRole():changetype<Ros2Ur_dashboard_msgsMsgUserRole>(out.value);}
}
/** Complete original ur_dashboard_msgs/msg/VersionInformation; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsMsgVersionInformation {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field major, native scalar uint32. */
major:u32=0;
/** Exact native field minor, native scalar uint32. */
minor:u32=0;
/** Exact native field bugfix, native scalar uint32. */
bugfix:u32=0;
/** Exact native field build, native scalar uint32. */
build:u32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.major,4);
w.integer(<u64>this.minor,4);
w.integer(<u64>this.bugfix,4);
w.integer(<u64>this.build,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsMsgVersionInformation{const value=new Ros2Ur_dashboard_msgsMsgVersionInformation();
{
const decoded=<u32>r.integer(4);
value.major=decoded;
}
{
const decoded=<u32>r.integer(4);
value.minor=decoded;
}
{
const decoded=<u32>r.integer(4);
value.bugfix=decoded;
}
{
const decoded=<u32>r.integer(4);
value.build=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsMsgVersionInformationCodec implements VendorCodec<Ros2Ur_dashboard_msgsMsgVersionInformation>{
static readonly codecIdentity:string="UWVMROS1_450c41c4361f1bca51f864dd9ef596851b17ebc79d4f784e7abb8b721752443d";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Ur_dashboard_msgsMsgVersionInformation):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsMsgVersionInformation):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsMsgVersionInformation>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsMsgVersionInformation>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsMsgVersionInformation.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsMsgVersionInformation{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsMsgVersionInformation():changetype<Ros2Ur_dashboard_msgsMsgVersionInformation>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/AddToLog_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvAddToLog_Request {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvAddToLog_Request{const value=new Ros2Ur_dashboard_msgsSrvAddToLog_Request();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvAddToLog_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvAddToLog_Request>{
static readonly codecIdentity:string="UWVMROS1_ecfe5bb0d0db92bc5dfdec9bf82fbe730e3b3b4d65680812d0540b9055590142";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvAddToLog_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvAddToLog_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvAddToLog_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvAddToLog_Request>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvAddToLog_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvAddToLog_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvAddToLog_Request():changetype<Ros2Ur_dashboard_msgsSrvAddToLog_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/AddToLog_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvAddToLog_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvAddToLog_Response{const value=new Ros2Ur_dashboard_msgsSrvAddToLog_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvAddToLog_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvAddToLog_Response>{
static readonly codecIdentity:string="UWVMROS1_57b0c150a353d3fca6e96c4dc86e72f71b452663ede1eeb69e51069992d07e4c";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvAddToLog_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvAddToLog_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvAddToLog_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvAddToLog_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvAddToLog_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvAddToLog_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvAddToLog_Response():changetype<Ros2Ur_dashboard_msgsSrvAddToLog_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/DownloadProgram_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvDownloadProgram_Request {
static readonly maxWireBytes:i32=520;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=520;
/** Exact native field program_name, native scalar string. */
program_name:string="";
/** Exact native field target_path, native scalar string. */
target_path:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.program_name)||this.program_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.program_name);if(size>256)return -22;if(4+size>520-n)return -90;n+=4+size;
if(n>520)return -90;
}
{
if(!validUnicode(this.target_path)||this.target_path.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.target_path);if(size>256)return -22;if(4+size>520-n)return -90;n+=4+size;
if(n>520)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.program_name);
w.text(this.target_path);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvDownloadProgram_Request{const value=new Ros2Ur_dashboard_msgsSrvDownloadProgram_Request();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.program_name=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.target_path=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvDownloadProgram_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvDownloadProgram_Request>{
static readonly codecIdentity:string="UWVMROS1_5c84420393aae13ea448d171a7b2956a55a39f3321f504b9f3bd5b4a38960c2f";
static readonly maxWireBytes:i32=520;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvDownloadProgram_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvDownloadProgram_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvDownloadProgram_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvDownloadProgram_Request>();if(bytes.length>520){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvDownloadProgram_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvDownloadProgram_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvDownloadProgram_Request():changetype<Ros2Ur_dashboard_msgsSrvDownloadProgram_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/DownloadProgram_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvDownloadProgram_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvDownloadProgram_Response{const value=new Ros2Ur_dashboard_msgsSrvDownloadProgram_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvDownloadProgram_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvDownloadProgram_Response>{
static readonly codecIdentity:string="UWVMROS1_46b0a73805282bad5f819f712275b7dd2ca4a6bb8c73c445ea8ae0a3d0f9e8e0";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvDownloadProgram_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvDownloadProgram_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvDownloadProgram_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvDownloadProgram_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvDownloadProgram_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvDownloadProgram_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvDownloadProgram_Response():changetype<Ros2Ur_dashboard_msgsSrvDownloadProgram_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/DownloadSupportFile_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field target_path, native scalar string. */
target_path:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.target_path)||this.target_path.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.target_path);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.target_path);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request{const value=new Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.target_path=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvDownloadSupportFile_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request>{
static readonly codecIdentity:string="UWVMROS1_d919563629f81d5a8c10f1d2921ea16f3c912a49b8d3bcf65d17b8daa0a38140";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request():changetype<Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/DownloadSupportFile_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response {
static readonly maxWireBytes:i32=262;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=262;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field support_files_present, native scalar bool. */
support_files_present:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>262-n)return -90;n+=4+size;
if(n>262)return -90;
}
{
if(1>262-n)return -90;n+=1;
if(n>262)return -90;
}
{
if(1>262-n)return -90;n+=1;
if(n>262)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.u8(this.success?1:0);
w.u8(this.support_files_present?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response{const value=new Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.support_files_present=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvDownloadSupportFile_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response>{
static readonly codecIdentity:string="UWVMROS1_76e190e6e9f1ddfff3082ea2adb0463d21ba9fb6795153fe6bbacee8a3334667";
static readonly maxWireBytes:i32=262;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response>();if(bytes.length>262){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response():changetype<Ros2Ur_dashboard_msgsSrvDownloadSupportFile_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GenerateFlightReport_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request {
static readonly CONTROLLER:string="controller";
static readonly SOFTWARE:string="software";
static readonly SYSTEM:string="system";
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field report_type, native scalar string. */
report_type:string="SYSTEM";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.report_type)||this.report_type.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.report_type);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.report_type);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request{const value=new Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.report_type=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGenerateFlightReport_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request>{
static readonly codecIdentity:string="UWVMROS1_7246d97690c0c827e9cfab2f8297cf27a10e60e41ebb3ba345f361a109057099";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request():changetype<Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GenerateFlightReport_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field report_id, native scalar string. */
report_id:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(!validUnicode(this.report_id)||this.report_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.report_id);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.answer);
w.text(this.report_id);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response{const value=new Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.report_id=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGenerateFlightReport_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response>{
static readonly codecIdentity:string="UWVMROS1_adebf67876b5ae3377cec0aa04138c2ab73ef7c0d7942c77c6c06b248a836445";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response():changetype<Ros2Ur_dashboard_msgsSrvGenerateFlightReport_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GenerateSupportFile_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field dir_path, native scalar string. */
dir_path:string=".";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.dir_path)||this.dir_path.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.dir_path);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.dir_path);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request{const value=new Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.dir_path=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGenerateSupportFile_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request>{
static readonly codecIdentity:string="UWVMROS1_10b0371b77db32d75a77dcb0605b325caa1915ccfb7d0917896fd75234e6ada7";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request():changetype<Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GenerateSupportFile_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field generated_file_name, native scalar string. */
generated_file_name:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(!validUnicode(this.generated_file_name)||this.generated_file_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.generated_file_name);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.answer);
w.text(this.generated_file_name);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response{const value=new Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.generated_file_name=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGenerateSupportFile_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response>{
static readonly codecIdentity:string="UWVMROS1_f370623fe690f218073f8538aa9fcf41c94d4abd3e0507e09607ef11e10edf38";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response():changetype<Ros2Ur_dashboard_msgsSrvGenerateSupportFile_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetLoadedProgram_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request{const value=new Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetLoadedProgram_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request>{
static readonly codecIdentity:string="UWVMROS1_030e5746e6a4da8f266e37d11cc3826715099ff1aa1c845a97042aec987707e3";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request():changetype<Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetLoadedProgram_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field program_name, native scalar string. */
program_name:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(!validUnicode(this.program_name)||this.program_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.program_name);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.text(this.program_name);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response{const value=new Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.program_name=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetLoadedProgram_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response>{
static readonly codecIdentity:string="UWVMROS1_3e8c7f3c8711faea01de37b9a011f34a9c6dd660cc222e46177b2c1898ec2f7e";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response():changetype<Ros2Ur_dashboard_msgsSrvGetLoadedProgram_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetOperationalMode_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request{const value=new Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetOperationalMode_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request>{
static readonly codecIdentity:string="UWVMROS1_3f348b5d9e04a8ba01f5277ad259c34a6816ae0693f47c49e348a5497a90a325";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request():changetype<Ros2Ur_dashboard_msgsSrvGetOperationalMode_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetOperationalMode_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field operational_mode, native scalar ur_dashboard_msgs/msg/OperationalMode. */
operational_mode:Ros2Ur_dashboard_msgsMsgOperationalMode=new Ros2Ur_dashboard_msgsMsgOperationalMode();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
const child=this.operational_mode.wireSize();if(child<0)return child;if(child>521-n)return -90;n+=child;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.answer);
this.operational_mode.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response{const value=new Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const decoded=Ros2Ur_dashboard_msgsMsgOperationalMode.readControl(r);
value.operational_mode=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetOperationalMode_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response>{
static readonly codecIdentity:string="UWVMROS1_6c2da335fd551162020eb394b91e99aaeca17e34e7c2e98de935c81912efb5f6";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response():changetype<Ros2Ur_dashboard_msgsSrvGetOperationalMode_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetPolyScopeVersion_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request{const value=new Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request>{
static readonly codecIdentity:string="UWVMROS1_88193ae202f0cb02746bc5556e8e8ee277fb4fae72ee4b96a6539c980dc86f13";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request():changetype<Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetPolyScopeVersion_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response {
static readonly maxWireBytes:i32=277;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=277;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field version, native scalar ur_dashboard_msgs/msg/VersionInformation. */
version:Ros2Ur_dashboard_msgsMsgVersionInformation=new Ros2Ur_dashboard_msgsMsgVersionInformation();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>277-n)return -90;n+=4+size;
if(n>277)return -90;
}
{
if(1>277-n)return -90;n+=1;
if(n>277)return -90;
}
{
const child=this.version.wireSize();if(child<0)return child;if(child>277-n)return -90;n+=child;
if(n>277)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.u8(this.success?1:0);
this.version.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response{const value=new Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const decoded=Ros2Ur_dashboard_msgsMsgVersionInformation.readControl(r);
value.version=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response>{
static readonly codecIdentity:string="UWVMROS1_c8d53d0a920806e86ceb6d2ac5a692a92e62b057432554bed9dcf57bed0ff9bd";
static readonly maxWireBytes:i32=277;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response>();if(bytes.length>277){out.status=90;return out;}if(bytes.length<21){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response():changetype<Ros2Ur_dashboard_msgsSrvGetPolyScopeVersion_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetProgramState_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetProgramState_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetProgramState_Request{const value=new Ros2Ur_dashboard_msgsSrvGetProgramState_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetProgramState_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetProgramState_Request>{
static readonly codecIdentity:string="UWVMROS1_fe42d1d1aa3d7bdbaa410bce045120d6dd841213ef10c5dd158c2b76fa157f65";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetProgramState_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetProgramState_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetProgramState_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetProgramState_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetProgramState_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetProgramState_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetProgramState_Request():changetype<Ros2Ur_dashboard_msgsSrvGetProgramState_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetProgramState_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetProgramState_Response {
static readonly maxWireBytes:i32=781;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=781;
/** Exact native field state, native scalar ur_dashboard_msgs/msg/ProgramState. */
state:Ros2Ur_dashboard_msgsMsgProgramState=new Ros2Ur_dashboard_msgsMsgProgramState();
/** Exact native field program_name, native scalar string. */
program_name:string="";
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.state.wireSize();if(child<0)return child;if(child>781-n)return -90;n+=child;
if(n>781)return -90;
}
{
if(!validUnicode(this.program_name)||this.program_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.program_name);if(size>256)return -22;if(4+size>781-n)return -90;n+=4+size;
if(n>781)return -90;
}
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>781-n)return -90;n+=4+size;
if(n>781)return -90;
}
{
if(1>781-n)return -90;n+=1;
if(n>781)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.state.writeControl(w);
w.text(this.program_name);
w.text(this.answer);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetProgramState_Response{const value=new Ros2Ur_dashboard_msgsSrvGetProgramState_Response();
{
const decoded=Ros2Ur_dashboard_msgsMsgProgramState.readControl(r);
value.state=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.program_name=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetProgramState_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetProgramState_Response>{
static readonly codecIdentity:string="UWVMROS1_b20731b8552f323837c5939f390060a6c7c92e9447df5e620891708cfce51ff4";
static readonly maxWireBytes:i32=781;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetProgramState_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetProgramState_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetProgramState_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetProgramState_Response>();if(bytes.length>781){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetProgramState_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetProgramState_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetProgramState_Response():changetype<Ros2Ur_dashboard_msgsSrvGetProgramState_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetPrograms_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetPrograms_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetPrograms_Request{const value=new Ros2Ur_dashboard_msgsSrvGetPrograms_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetPrograms_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetPrograms_Request>{
static readonly codecIdentity:string="UWVMROS1_8d7b4f3fb9a15854b59c85c5205bf20d3e0f6a1a972ccbdf77ee8377c9e40d6d";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetPrograms_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetPrograms_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetPrograms_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetPrograms_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetPrograms_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetPrograms_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetPrograms_Request():changetype<Ros2Ur_dashboard_msgsSrvGetPrograms_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetPrograms_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetPrograms_Response {
static readonly maxWireBytes:i32=11081;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=11081;
/** Exact native field programs, native scalar ur_dashboard_msgs/msg/ProgramInformation. */
programs:Array<Ros2Ur_dashboard_msgsMsgProgramInformation>=new Array<Ros2Ur_dashboard_msgsMsgProgramInformation>();
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.programs.length>16)return -22;
if(4>11081-n)return -90;n+=4;
for(let i=0;i<this.programs.length;++i){
const child=this.programs[i].wireSize();if(child<0)return child;if(child>11081-n)return -90;n+=child;
if(n>11081)return -90;
}
}
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>11081-n)return -90;n+=4+size;
if(n>11081)return -90;
}
{
if(1>11081-n)return -90;n+=1;
if(n>11081)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.programs.length);
for(let i=0;i<this.programs.length;++i){
this.programs[i].writeControl(w);
}
w.text(this.answer);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetPrograms_Response{const value=new Ros2Ur_dashboard_msgsSrvGetPrograms_Response();
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.programs=new Array<Ros2Ur_dashboard_msgsMsgProgramInformation>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Ur_dashboard_msgsMsgProgramInformation.readControl(r);
value.programs.push(decoded);
}
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetPrograms_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetPrograms_Response>{
static readonly codecIdentity:string="UWVMROS1_111b67c562ba0c77c2924b97058e4126055ad6deb7f99049b8caf8990b3f979c";
static readonly maxWireBytes:i32=11081;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetPrograms_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetPrograms_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetPrograms_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetPrograms_Response>();if(bytes.length>11081){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetPrograms_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetPrograms_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetPrograms_Response():changetype<Ros2Ur_dashboard_msgsSrvGetPrograms_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetRobotMode_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetRobotMode_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetRobotMode_Request{const value=new Ros2Ur_dashboard_msgsSrvGetRobotMode_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetRobotMode_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetRobotMode_Request>{
static readonly codecIdentity:string="UWVMROS1_d1d9cd94a443b20ec0d41a8db1a6d87291657597abc014e7da0f46a58c6c5614";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetRobotMode_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetRobotMode_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetRobotMode_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetRobotMode_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetRobotMode_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetRobotMode_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetRobotMode_Request():changetype<Ros2Ur_dashboard_msgsSrvGetRobotMode_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetRobotMode_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetRobotMode_Response {
static readonly maxWireBytes:i32=262;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=262;
/** Exact native field robot_mode, native scalar ur_dashboard_msgs/msg/RobotMode. */
robot_mode:Ros2Ur_dashboard_msgsMsgRobotMode=new Ros2Ur_dashboard_msgsMsgRobotMode();
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.robot_mode.wireSize();if(child<0)return child;if(child>262-n)return -90;n+=child;
if(n>262)return -90;
}
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>262-n)return -90;n+=4+size;
if(n>262)return -90;
}
{
if(1>262-n)return -90;n+=1;
if(n>262)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.robot_mode.writeControl(w);
w.text(this.answer);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetRobotMode_Response{const value=new Ros2Ur_dashboard_msgsSrvGetRobotMode_Response();
{
const decoded=Ros2Ur_dashboard_msgsMsgRobotMode.readControl(r);
value.robot_mode=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetRobotMode_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetRobotMode_Response>{
static readonly codecIdentity:string="UWVMROS1_67db750ad95b3b207d61b18513a117c868040c22c93638f07fa74645d87bcfba";
static readonly maxWireBytes:i32=262;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetRobotMode_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetRobotMode_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetRobotMode_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetRobotMode_Response>();if(bytes.length>262){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetRobotMode_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetRobotMode_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetRobotMode_Response():changetype<Ros2Ur_dashboard_msgsSrvGetRobotMode_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetRobotModel_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetRobotModel_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetRobotModel_Request{const value=new Ros2Ur_dashboard_msgsSrvGetRobotModel_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetRobotModel_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetRobotModel_Request>{
static readonly codecIdentity:string="UWVMROS1_9db08f07993bd5786aea5b6bd567f9e9f49329883326ea16e6c49eef1586dc43";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetRobotModel_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetRobotModel_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetRobotModel_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetRobotModel_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetRobotModel_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetRobotModel_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetRobotModel_Request():changetype<Ros2Ur_dashboard_msgsSrvGetRobotModel_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetRobotModel_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetRobotModel_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field robot_model, native scalar string. */
robot_model:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(!validUnicode(this.robot_model)||this.robot_model.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.robot_model);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.answer);
w.text(this.robot_model);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetRobotModel_Response{const value=new Ros2Ur_dashboard_msgsSrvGetRobotModel_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.robot_model=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetRobotModel_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetRobotModel_Response>{
static readonly codecIdentity:string="UWVMROS1_faf0124c542ef7420915527c27e99a7d26c5d6274f55d869bc9416e6b7d9b3c9";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetRobotModel_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetRobotModel_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetRobotModel_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetRobotModel_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetRobotModel_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetRobotModel_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetRobotModel_Response():changetype<Ros2Ur_dashboard_msgsSrvGetRobotModel_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetSafetyMode_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request{const value=new Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetSafetyMode_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request>{
static readonly codecIdentity:string="UWVMROS1_5866cc4b0e10508eeb45ceecf9f9b6036ba55d04c9613622e189ccde2781dcb0";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request():changetype<Ros2Ur_dashboard_msgsSrvGetSafetyMode_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetSafetyMode_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response {
static readonly maxWireBytes:i32=262;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=262;
/** Exact native field safety_mode, native scalar ur_dashboard_msgs/msg/SafetyMode. */
safety_mode:Ros2Ur_dashboard_msgsMsgSafetyMode=new Ros2Ur_dashboard_msgsMsgSafetyMode();
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.safety_mode.wireSize();if(child<0)return child;if(child>262-n)return -90;n+=child;
if(n>262)return -90;
}
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>262-n)return -90;n+=4+size;
if(n>262)return -90;
}
{
if(1>262-n)return -90;n+=1;
if(n>262)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.safety_mode.writeControl(w);
w.text(this.answer);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response{const value=new Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response();
{
const decoded=Ros2Ur_dashboard_msgsMsgSafetyMode.readControl(r);
value.safety_mode=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetSafetyMode_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response>{
static readonly codecIdentity:string="UWVMROS1_f65d7ff83dc97f891032d8ab16bf71a9c2326c6ae6a033fe1d6e06f011c5a40b";
static readonly maxWireBytes:i32=262;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response>();if(bytes.length>262){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response():changetype<Ros2Ur_dashboard_msgsSrvGetSafetyMode_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetSafetyStatus_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request{const value=new Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetSafetyStatus_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request>{
static readonly codecIdentity:string="UWVMROS1_0e2958bcd7b121682361c94b2926bcc4552d7ef07dc2af4609d04b1bb7054602";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request():changetype<Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetSafetyStatus_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response {
static readonly maxWireBytes:i32=262;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=262;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field safety_status, native scalar ur_dashboard_msgs/msg/SafetyStatus. */
safety_status:Ros2Ur_dashboard_msgsMsgSafetyStatus=new Ros2Ur_dashboard_msgsMsgSafetyStatus();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>262-n)return -90;n+=4+size;
if(n>262)return -90;
}
{
if(1>262-n)return -90;n+=1;
if(n>262)return -90;
}
{
const child=this.safety_status.wireSize();if(child<0)return child;if(child>262-n)return -90;n+=child;
if(n>262)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.u8(this.success?1:0);
this.safety_status.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response{const value=new Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const decoded=Ros2Ur_dashboard_msgsMsgSafetyStatus.readControl(r);
value.safety_status=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetSafetyStatus_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response>{
static readonly codecIdentity:string="UWVMROS1_bb96aaa96f5c78b97ad80a14eb96865dc10b5a1d12524fcbcc75fc6d481c53e4";
static readonly maxWireBytes:i32=262;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response>();if(bytes.length>262){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response():changetype<Ros2Ur_dashboard_msgsSrvGetSafetyStatus_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetSerialNumber_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request{const value=new Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetSerialNumber_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request>{
static readonly codecIdentity:string="UWVMROS1_bbb1ca066c77ed459352de69d566f3ea87b94fe593c31c911193416f92a73c9b";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request():changetype<Ros2Ur_dashboard_msgsSrvGetSerialNumber_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetSerialNumber_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response {
static readonly maxWireBytes:i32=269;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=269;
/** Exact native field serial_number, native scalar uint64. */
serial_number:u64=0;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>269-n)return -90;n+=8;
if(n>269)return -90;
}
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>269-n)return -90;n+=4+size;
if(n>269)return -90;
}
{
if(1>269-n)return -90;n+=1;
if(n>269)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.serial_number,8);
w.text(this.answer);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response{const value=new Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response();
{
const decoded=<u64>r.integer(8);
value.serial_number=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetSerialNumber_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response>{
static readonly codecIdentity:string="UWVMROS1_f389641d089fa6fd421348f047732de2122cd714bdb2bd4d1e611592aca349e7";
static readonly maxWireBytes:i32=269;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response>();if(bytes.length>269){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response():changetype<Ros2Ur_dashboard_msgsSrvGetSerialNumber_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetUserRole_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetUserRole_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetUserRole_Request{const value=new Ros2Ur_dashboard_msgsSrvGetUserRole_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetUserRole_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetUserRole_Request>{
static readonly codecIdentity:string="UWVMROS1_a331be09dd2093e4584e6dc80f83caaadd3a8e43cca6a1c8fcfa1e2ea632c9f0";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetUserRole_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetUserRole_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetUserRole_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetUserRole_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetUserRole_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetUserRole_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetUserRole_Request():changetype<Ros2Ur_dashboard_msgsSrvGetUserRole_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/GetUserRole_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvGetUserRole_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field user_role, native scalar ur_dashboard_msgs/msg/UserRole. */
user_role:Ros2Ur_dashboard_msgsMsgUserRole=new Ros2Ur_dashboard_msgsMsgUserRole();
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.user_role.wireSize();if(child<0)return child;if(child>521-n)return -90;n+=child;
if(n>521)return -90;
}
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.user_role.writeControl(w);
w.text(this.answer);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvGetUserRole_Response{const value=new Ros2Ur_dashboard_msgsSrvGetUserRole_Response();
{
const decoded=Ros2Ur_dashboard_msgsMsgUserRole.readControl(r);
value.user_role=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvGetUserRole_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvGetUserRole_Response>{
static readonly codecIdentity:string="UWVMROS1_ed4f53e59fbe4cd33cfbf5d190b60fbe8371dda7da9d7af6e270a882eb142df8";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvGetUserRole_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvGetUserRole_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvGetUserRole_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvGetUserRole_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvGetUserRole_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvGetUserRole_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvGetUserRole_Response():changetype<Ros2Ur_dashboard_msgsSrvGetUserRole_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/IsInRemoteControl_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request{const value=new Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvIsInRemoteControl_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request>{
static readonly codecIdentity:string="UWVMROS1_4b6b86fc4337c36589e8781b46ddbe6992f750b565ff413ea0b069982d87a463";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request():changetype<Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/IsInRemoteControl_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response {
static readonly maxWireBytes:i32=262;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=262;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field remote_control, native scalar bool. */
remote_control:bool=false;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>262-n)return -90;n+=4+size;
if(n>262)return -90;
}
{
if(1>262-n)return -90;n+=1;
if(n>262)return -90;
}
{
if(1>262-n)return -90;n+=1;
if(n>262)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.u8(this.remote_control?1:0);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response{const value=new Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.remote_control=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvIsInRemoteControl_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response>{
static readonly codecIdentity:string="UWVMROS1_f2705c1be319c7c667f4f2224851b06c24b15b4211c2828fab89f09455e4c816";
static readonly maxWireBytes:i32=262;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response>();if(bytes.length>262){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response():changetype<Ros2Ur_dashboard_msgsSrvIsInRemoteControl_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/IsProgramRunning_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request{const value=new Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvIsProgramRunning_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request>{
static readonly codecIdentity:string="UWVMROS1_f28274c0dc01d8aa2f981f969c05a614e6b3922601d578f99c6321a4b20fc80f";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request():changetype<Ros2Ur_dashboard_msgsSrvIsProgramRunning_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/IsProgramRunning_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response {
static readonly maxWireBytes:i32=262;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=262;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field program_running, native scalar bool. */
program_running:bool=false;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>262-n)return -90;n+=4+size;
if(n>262)return -90;
}
{
if(1>262-n)return -90;n+=1;
if(n>262)return -90;
}
{
if(1>262-n)return -90;n+=1;
if(n>262)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.u8(this.program_running?1:0);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response{const value=new Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.program_running=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvIsProgramRunning_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response>{
static readonly codecIdentity:string="UWVMROS1_c57cc7ecd7e81f7359824b43884a58f65748a4c77cacbcc45be17aa9147cf499";
static readonly maxWireBytes:i32=262;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response>();if(bytes.length>262){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response():changetype<Ros2Ur_dashboard_msgsSrvIsProgramRunning_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/IsProgramSaved_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request{const value=new Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request();
return value;}
}
export class Ros2Ur_dashboard_msgsSrvIsProgramSaved_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request>{
static readonly codecIdentity:string="UWVMROS1_c53d57cffc0480d66257150a8450ba90563fda5153742784831d38abcbc0b099";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request():changetype<Ros2Ur_dashboard_msgsSrvIsProgramSaved_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/IsProgramSaved_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response {
static readonly maxWireBytes:i32=522;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=522;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field program_name, native scalar string. */
program_name:string="";
/** Exact native field program_saved, native scalar bool. */
program_saved:bool=false;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>522-n)return -90;n+=4+size;
if(n>522)return -90;
}
{
if(!validUnicode(this.program_name)||this.program_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.program_name);if(size>256)return -22;if(4+size>522-n)return -90;n+=4+size;
if(n>522)return -90;
}
{
if(1>522-n)return -90;n+=1;
if(n>522)return -90;
}
{
if(1>522-n)return -90;n+=1;
if(n>522)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.text(this.program_name);
w.u8(this.program_saved?1:0);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response{const value=new Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.program_name=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.program_saved=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvIsProgramSaved_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response>{
static readonly codecIdentity:string="UWVMROS1_aa171247f31fd00a1b8820ee21cf20d159bd10d0d2887dc96b3a4dd4d372d115";
static readonly maxWireBytes:i32=522;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response>();if(bytes.length>522){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response():changetype<Ros2Ur_dashboard_msgsSrvIsProgramSaved_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/Load_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvLoad_Request {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field filename, native scalar string. */
filename:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.filename)||this.filename.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.filename);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.filename);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvLoad_Request{const value=new Ros2Ur_dashboard_msgsSrvLoad_Request();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.filename=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvLoad_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvLoad_Request>{
static readonly codecIdentity:string="UWVMROS1_1d0cd6cdeaaafce999a7e9924721246b9f4954f22da40fd0a27fb7b657324876";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvLoad_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvLoad_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvLoad_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvLoad_Request>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvLoad_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvLoad_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvLoad_Request():changetype<Ros2Ur_dashboard_msgsSrvLoad_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/Load_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvLoad_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvLoad_Response{const value=new Ros2Ur_dashboard_msgsSrvLoad_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvLoad_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvLoad_Response>{
static readonly codecIdentity:string="UWVMROS1_4e694de8079133a8e789add3640dfe19805cad92991a650167c2aae7067da415";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvLoad_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvLoad_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvLoad_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvLoad_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvLoad_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvLoad_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvLoad_Response():changetype<Ros2Ur_dashboard_msgsSrvLoad_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/Popup_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvPopup_Request {
static readonly maxWireBytes:i32=520;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=520;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact native field title, native scalar string. */
title:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>520-n)return -90;n+=4+size;
if(n>520)return -90;
}
{
if(!validUnicode(this.title)||this.title.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.title);if(size>256)return -22;if(4+size>520-n)return -90;n+=4+size;
if(n>520)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.message);
w.text(this.title);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvPopup_Request{const value=new Ros2Ur_dashboard_msgsSrvPopup_Request();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.title=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvPopup_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvPopup_Request>{
static readonly codecIdentity:string="UWVMROS1_369e6eab4f689a846c52fcc5cb539da63fab3939fab254dd1e04c7b274d5f770";
static readonly maxWireBytes:i32=520;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvPopup_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvPopup_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvPopup_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvPopup_Request>();if(bytes.length>520){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvPopup_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvPopup_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvPopup_Request():changetype<Ros2Ur_dashboard_msgsSrvPopup_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/Popup_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvPopup_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvPopup_Response{const value=new Ros2Ur_dashboard_msgsSrvPopup_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvPopup_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvPopup_Response>{
static readonly codecIdentity:string="UWVMROS1_0b54f7c477c1ddf8ccb18d12d50eaf308225d4927ee377377ed354426e58f984";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvPopup_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvPopup_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvPopup_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvPopup_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvPopup_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvPopup_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvPopup_Response():changetype<Ros2Ur_dashboard_msgsSrvPopup_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/RawRequest_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvRawRequest_Request {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field query, native scalar string. */
query:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.query)||this.query.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.query);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.query);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvRawRequest_Request{const value=new Ros2Ur_dashboard_msgsSrvRawRequest_Request();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.query=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvRawRequest_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvRawRequest_Request>{
static readonly codecIdentity:string="UWVMROS1_2bf66d87b20ca2de9ed94c04c79042a50239728320b43b27c9942405795b4555";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvRawRequest_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvRawRequest_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvRawRequest_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvRawRequest_Request>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvRawRequest_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvRawRequest_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvRawRequest_Request():changetype<Ros2Ur_dashboard_msgsSrvRawRequest_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/RawRequest_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvRawRequest_Response {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvRawRequest_Response{const value=new Ros2Ur_dashboard_msgsSrvRawRequest_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvRawRequest_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvRawRequest_Response>{
static readonly codecIdentity:string="UWVMROS1_7c37f54149bab54f6e7bbfed41992eb54416b31346bc889a9632d5497111267f";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvRawRequest_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvRawRequest_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvRawRequest_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvRawRequest_Response>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvRawRequest_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvRawRequest_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvRawRequest_Response():changetype<Ros2Ur_dashboard_msgsSrvRawRequest_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/SetOperationalMode_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field operational_mode, native scalar ur_dashboard_msgs/msg/OperationalMode. */
operational_mode:Ros2Ur_dashboard_msgsMsgOperationalMode=new Ros2Ur_dashboard_msgsMsgOperationalMode();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.operational_mode.wireSize();if(child<0)return child;if(child>260-n)return -90;n+=child;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.operational_mode.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request{const value=new Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request();
{
const decoded=Ros2Ur_dashboard_msgsMsgOperationalMode.readControl(r);
value.operational_mode=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvSetOperationalMode_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request>{
static readonly codecIdentity:string="UWVMROS1_8cc61f7c64211c46e01e52eee2c05f6e709f4f0900cf24fdfd4079b0f7581a00";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request():changetype<Ros2Ur_dashboard_msgsSrvSetOperationalMode_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/SetOperationalMode_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response{const value=new Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvSetOperationalMode_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response>{
static readonly codecIdentity:string="UWVMROS1_f5009de077224371e9d2f6c0aecbabd24c80148f2c7e1b1383a64f6f214b202a";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response():changetype<Ros2Ur_dashboard_msgsSrvSetOperationalMode_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/SetUserRole_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvSetUserRole_Request {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field user_role, native scalar ur_dashboard_msgs/msg/UserRole. */
user_role:Ros2Ur_dashboard_msgsMsgUserRole=new Ros2Ur_dashboard_msgsMsgUserRole();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.user_role.wireSize();if(child<0)return child;if(child>260-n)return -90;n+=child;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.user_role.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvSetUserRole_Request{const value=new Ros2Ur_dashboard_msgsSrvSetUserRole_Request();
{
const decoded=Ros2Ur_dashboard_msgsMsgUserRole.readControl(r);
value.user_role=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvSetUserRole_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvSetUserRole_Request>{
static readonly codecIdentity:string="UWVMROS1_7bd499947803efca81401a2de496d24e7e54ba97f4f86ec2b3089146a0655f5e";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvSetUserRole_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvSetUserRole_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvSetUserRole_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvSetUserRole_Request>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvSetUserRole_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvSetUserRole_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvSetUserRole_Request():changetype<Ros2Ur_dashboard_msgsSrvSetUserRole_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/SetUserRole_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvSetUserRole_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.answer);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvSetUserRole_Response{const value=new Ros2Ur_dashboard_msgsSrvSetUserRole_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvSetUserRole_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvSetUserRole_Response>{
static readonly codecIdentity:string="UWVMROS1_4cab2bba5030f4e4b9e90252d64ec2e02d6eb87070b75cf43190fecd21da59fd";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvSetUserRole_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvSetUserRole_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvSetUserRole_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvSetUserRole_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvSetUserRole_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvSetUserRole_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvSetUserRole_Response():changetype<Ros2Ur_dashboard_msgsSrvSetUserRole_Response>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/UploadProgram_Request; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvUploadProgram_Request {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field file_path, native scalar string. */
file_path:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.file_path)||this.file_path.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.file_path);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.file_path);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvUploadProgram_Request{const value=new Ros2Ur_dashboard_msgsSrvUploadProgram_Request();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.file_path=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvUploadProgram_RequestCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvUploadProgram_Request>{
static readonly codecIdentity:string="UWVMROS1_f7501d0115f8d36b1942ce610b1bbc197d0276de508ac8175d539f40494f437b";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvUploadProgram_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvUploadProgram_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvUploadProgram_Request>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvUploadProgram_Request>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvUploadProgram_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvUploadProgram_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvUploadProgram_Request():changetype<Ros2Ur_dashboard_msgsSrvUploadProgram_Request>(out.value);}
}
/** Complete original ur_dashboard_msgs/srv/UploadProgram_Response; spellings are published in generation proof. */
export class Ros2Ur_dashboard_msgsSrvUploadProgram_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field program_name, native scalar string. */
program_name:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.answer)||this.answer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.answer);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(!validUnicode(this.program_name)||this.program_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.program_name);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.answer);
w.text(this.program_name);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_dashboard_msgsSrvUploadProgram_Response{const value=new Ros2Ur_dashboard_msgsSrvUploadProgram_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.program_name=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_dashboard_msgsSrvUploadProgram_ResponseCodec implements VendorCodec<Ros2Ur_dashboard_msgsSrvUploadProgram_Response>{
static readonly codecIdentity:string="UWVMROS1_ae9a28c1cd4729f54968d18a16e2f8f417bdc5e9bc15809de7481f1f20e6b398";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Ur_dashboard_msgsSrvUploadProgram_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_dashboard_msgsSrvUploadProgram_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_dashboard_msgsSrvUploadProgram_Response>{const out=new VendorDecoded<Ros2Ur_dashboard_msgsSrvUploadProgram_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_dashboard_msgsSrvUploadProgram_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_dashboard_msgsSrvUploadProgram_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_dashboard_msgsSrvUploadProgram_Response():changetype<Ros2Ur_dashboard_msgsSrvUploadProgram_Response>(out.value);}
}
/** Complete original ur_msgs/action/FollowJointTrajectoryUntil_Goal; spellings are published in generation proof. */
export class Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal {
static readonly TOOL_CONTACT:u8=0;
static readonly maxWireBytes:i32=7053;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=7053;
/** Exact native field trajectory, native scalar trajectory_msgs/msg/JointTrajectory. */
trajectory:Ros2Trajectory_msgsMsgJointTrajectory=new Ros2Trajectory_msgsMsgJointTrajectory();
/** Exact native field path_tolerance, native scalar control_msgs/msg/JointTolerance. */
path_tolerance:Array<Ros2Control_msgsMsgJointTolerance>=new Array<Ros2Control_msgsMsgJointTolerance>();
/** Exact native field goal_tolerance, native scalar control_msgs/msg/JointTolerance. */
goal_tolerance:Array<Ros2Control_msgsMsgJointTolerance>=new Array<Ros2Control_msgsMsgJointTolerance>();
/** Exact native field goal_time_tolerance, native scalar builtin_interfaces/msg/Duration. */
goal_time_tolerance:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
/** Exact native field until_type, native scalar uint8. */
until_type:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.trajectory.wireSize();if(child<0)return child;if(child>7053-n)return -90;n+=child;
if(n>7053)return -90;
}
{
if(this.path_tolerance.length>6)return -22;
if(4>7053-n)return -90;n+=4;
for(let i=0;i<this.path_tolerance.length;++i){
const child=this.path_tolerance[i].wireSize();if(child<0)return child;if(child>7053-n)return -90;n+=child;
if(n>7053)return -90;
}
}
{
if(this.goal_tolerance.length>6)return -22;
if(4>7053-n)return -90;n+=4;
for(let i=0;i<this.goal_tolerance.length;++i){
const child=this.goal_tolerance[i].wireSize();if(child<0)return child;if(child>7053-n)return -90;n+=child;
if(n>7053)return -90;
}
}
{
const child=this.goal_time_tolerance.wireSize();if(child<0)return child;if(child>7053-n)return -90;n+=child;
if(n>7053)return -90;
}
{
if(1>7053-n)return -90;n+=1;
if(n>7053)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.trajectory.writeControl(w);
w.u32(<u32>this.path_tolerance.length);
for(let i=0;i<this.path_tolerance.length;++i){
this.path_tolerance[i].writeControl(w);
}
w.u32(<u32>this.goal_tolerance.length);
for(let i=0;i<this.goal_tolerance.length;++i){
this.goal_tolerance[i].writeControl(w);
}
this.goal_time_tolerance.writeControl(w);
w.integer(<u64>this.until_type,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal{const value=new Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal();
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectory.readControl(r);
value.trajectory=decoded;
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.path_tolerance=new Array<Ros2Control_msgsMsgJointTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointTolerance.readControl(r);
value.path_tolerance.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.goal_tolerance=new Array<Ros2Control_msgsMsgJointTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointTolerance.readControl(r);
value.goal_tolerance.push(decoded);
}
}
{
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.goal_time_tolerance=decoded;
}
{
const decoded=<u8>r.integer(1);
value.until_type=decoded;
}
return value;}
}
export class Ros2Ur_msgsActionFollowJointTrajectoryUntil_GoalCodec implements VendorCodec<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal>{
static readonly codecIdentity:string="UWVMROS1_78fda8f2d20c502e24b1ad4fe55f34bba41de20ff88e71b56d7a0631d7a3905a";
static readonly maxWireBytes:i32=7053;
encodeChecked(value:Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal>{const out=new VendorDecoded<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal>();if(bytes.length>7053){out.status=90;return out;}if(bytes.length<37){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal():changetype<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Goal>(out.value);}
}
/** Complete original ur_msgs/action/FollowJointTrajectoryUntil_Result; spellings are published in generation proof. */
export class Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result {
static readonly SUCCESSFUL:i32=0;
static readonly INVALID_GOAL:i32=-1;
static readonly INVALID_JOINTS:i32=-2;
static readonly OLD_HEADER_TIMESTAMP:i32=-3;
static readonly PATH_TOLERANCE_VIOLATED:i32=-4;
static readonly GOAL_TOLERANCE_VIOLATED:i32=-5;
static readonly TRIGGERED:i32=0;
static readonly NOT_TRIGGERED:i32=1;
static readonly maxWireBytes:i32=268;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=268;
/** Exact native field error_code, native scalar int32. */
error_code:i32=0;
/** Exact native field until_condition_result, native scalar int32. */
until_condition_result:i32=0;
/** Exact native field error_string, native scalar string. */
error_string:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>268-n)return -90;n+=4;
if(n>268)return -90;
}
{
if(4>268-n)return -90;n+=4;
if(n>268)return -90;
}
{
if(!validUnicode(this.error_string)||this.error_string.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error_string);if(size>256)return -22;if(4+size>268-n)return -90;n+=4+size;
if(n>268)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.error_code,4);
w.integer(<u64>this.until_condition_result,4);
w.text(this.error_string);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result{const value=new Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result();
{
const decoded=<i32>r.integer(4);
value.error_code=decoded;
}
{
const decoded=<i32>r.integer(4);
value.until_condition_result=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error_string=decoded;
}
return value;}
}
export class Ros2Ur_msgsActionFollowJointTrajectoryUntil_ResultCodec implements VendorCodec<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result>{
static readonly codecIdentity:string="UWVMROS1_de92b9ccfb9c93f3afae9da5e8e31df9a6d6a606479fe27b9bcf14ff329bc869";
static readonly maxWireBytes:i32=268;
encodeChecked(value:Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result>{const out=new VendorDecoded<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result>();if(bytes.length>268){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result():changetype<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Result>(out.value);}
}
/** Complete original ur_msgs/action/FollowJointTrajectoryUntil_Feedback; spellings are published in generation proof. */
export class Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback {
static readonly maxWireBytes:i32=2352;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2352;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field desired, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
desired:Ros2Trajectory_msgsMsgJointTrajectoryPoint=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
/** Exact native field actual, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
actual:Ros2Trajectory_msgsMsgJointTrajectoryPoint=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
/** Exact native field error, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
error:Ros2Trajectory_msgsMsgJointTrajectoryPoint=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>2352-n)return -90;n+=child;
if(n>2352)return -90;
}
{
if(this.joint_names.length>6)return -22;
if(4>2352-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>256)return -22;if(4+size>2352-n)return -90;n+=4+size;
if(n>2352)return -90;
}
}
{
const child=this.desired.wireSize();if(child<0)return child;if(child>2352-n)return -90;n+=child;
if(n>2352)return -90;
}
{
const child=this.actual.wireSize();if(child<0)return child;if(child>2352-n)return -90;n+=child;
if(n>2352)return -90;
}
{
const child=this.error.wireSize();if(child<0)return child;if(child>2352-n)return -90;n+=child;
if(n>2352)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.joint_names.length);
for(let i=0;i<this.joint_names.length;++i){
w.text(this.joint_names[i]);
}
this.desired.writeControl(w);
this.actual.writeControl(w);
this.error.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback{const value=new Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.joint_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_names.push(decoded);
}
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.desired=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.actual=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.error=decoded;
}
return value;}
}
export class Ros2Ur_msgsActionFollowJointTrajectoryUntil_FeedbackCodec implements VendorCodec<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback>{
static readonly codecIdentity:string="UWVMROS1_5a719f991f5c73784f9dc9e962ecfa0541e00daef7f747c312d4b633d63040ac";
static readonly maxWireBytes:i32=2352;
encodeChecked(value:Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback>{const out=new VendorDecoded<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback>();if(bytes.length>2352){out.status=90;return out;}if(bytes.length<88){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback():changetype<Ros2Ur_msgsActionFollowJointTrajectoryUntil_Feedback>(out.value);}
}
/** Complete original ur_msgs/action/SendScript_Goal; spellings are published in generation proof. */
export class Ros2Ur_msgsActionSendScript_Goal {
static readonly maxWireBytes:i32=8465;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8465;
/** Exact native field program, native scalar string. */
program:string="";
/** Exact native field script_name, native scalar string. */
script_name:string="";
/** Exact native field start_timeout, native scalar builtin_interfaces/msg/Duration. */
start_timeout:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
/** Exact native field fail_on_warnings, native scalar bool. */
fail_on_warnings:bool=true;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.program)||this.program.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.program);if(size>8192)return -22;if(4+size>8465-n)return -90;n+=4+size;
if(n>8465)return -90;
}
{
if(!validUnicode(this.script_name)||this.script_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.script_name);if(size>256)return -22;if(4+size>8465-n)return -90;n+=4+size;
if(n>8465)return -90;
}
{
const child=this.start_timeout.wireSize();if(child<0)return child;if(child>8465-n)return -90;n+=child;
if(n>8465)return -90;
}
{
if(1>8465-n)return -90;n+=1;
if(n>8465)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.program);
w.text(this.script_name);
this.start_timeout.writeControl(w);
w.u8(this.fail_on_warnings?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsActionSendScript_Goal{const value=new Ros2Ur_msgsActionSendScript_Goal();
{
const size=r.u32();if(!r.valid||size>8192){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.program=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.script_name=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.start_timeout=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.fail_on_warnings=decoded;
}
return value;}
}
export class Ros2Ur_msgsActionSendScript_GoalCodec implements VendorCodec<Ros2Ur_msgsActionSendScript_Goal>{
static readonly codecIdentity:string="UWVMROS1_c277cd00663786f95201e61baa658b92def3becc1f37e54aa6093bb11135819c";
static readonly maxWireBytes:i32=8465;
encodeChecked(value:Ros2Ur_msgsActionSendScript_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsActionSendScript_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsActionSendScript_Goal>{const out=new VendorDecoded<Ros2Ur_msgsActionSendScript_Goal>();if(bytes.length>8465){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsActionSendScript_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsActionSendScript_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsActionSendScript_Goal():changetype<Ros2Ur_msgsActionSendScript_Goal>(out.value);}
}
/** Complete original ur_msgs/action/SendScript_Result; spellings are published in generation proof. */
export class Ros2Ur_msgsActionSendScript_Result {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsActionSendScript_Result{const value=new Ros2Ur_msgsActionSendScript_Result();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros2Ur_msgsActionSendScript_ResultCodec implements VendorCodec<Ros2Ur_msgsActionSendScript_Result>{
static readonly codecIdentity:string="UWVMROS1_d4c4d9721c3a576f590ba4967b2444a82969d6ccba94ddbf8c4a974535ee17d2";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Ur_msgsActionSendScript_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsActionSendScript_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsActionSendScript_Result>{const out=new VendorDecoded<Ros2Ur_msgsActionSendScript_Result>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsActionSendScript_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsActionSendScript_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsActionSendScript_Result():changetype<Ros2Ur_msgsActionSendScript_Result>(out.value);}
}
/** Complete original ur_msgs/action/SendScript_Feedback; spellings are published in generation proof. */
export class Ros2Ur_msgsActionSendScript_Feedback {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsActionSendScript_Feedback{const value=new Ros2Ur_msgsActionSendScript_Feedback();
return value;}
}
export class Ros2Ur_msgsActionSendScript_FeedbackCodec implements VendorCodec<Ros2Ur_msgsActionSendScript_Feedback>{
static readonly codecIdentity:string="UWVMROS1_094876d6467bd2b34f74cc6b574393117753add5254285e4e95d85b07328ef2c";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_msgsActionSendScript_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsActionSendScript_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsActionSendScript_Feedback>{const out=new VendorDecoded<Ros2Ur_msgsActionSendScript_Feedback>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsActionSendScript_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsActionSendScript_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsActionSendScript_Feedback():changetype<Ros2Ur_msgsActionSendScript_Feedback>(out.value);}
}
/** Complete original ur_msgs/action/ToolContact_Goal; spellings are published in generation proof. */
export class Ros2Ur_msgsActionToolContact_Goal {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsActionToolContact_Goal{const value=new Ros2Ur_msgsActionToolContact_Goal();
return value;}
}
export class Ros2Ur_msgsActionToolContact_GoalCodec implements VendorCodec<Ros2Ur_msgsActionToolContact_Goal>{
static readonly codecIdentity:string="UWVMROS1_150298ad16b1bc37d47bd3324cd4ec301ad8402c07779e8a435ce61daa7578dd";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_msgsActionToolContact_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsActionToolContact_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsActionToolContact_Goal>{const out=new VendorDecoded<Ros2Ur_msgsActionToolContact_Goal>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsActionToolContact_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsActionToolContact_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsActionToolContact_Goal():changetype<Ros2Ur_msgsActionToolContact_Goal>(out.value);}
}
/** Complete original ur_msgs/action/ToolContact_Result; spellings are published in generation proof. */
export class Ros2Ur_msgsActionToolContact_Result {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsActionToolContact_Result{const value=new Ros2Ur_msgsActionToolContact_Result();
return value;}
}
export class Ros2Ur_msgsActionToolContact_ResultCodec implements VendorCodec<Ros2Ur_msgsActionToolContact_Result>{
static readonly codecIdentity:string="UWVMROS1_b79f6080ac18b67c25131458a2af0a3c235d624c00fc5c13da534cc546c94578";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_msgsActionToolContact_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsActionToolContact_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsActionToolContact_Result>{const out=new VendorDecoded<Ros2Ur_msgsActionToolContact_Result>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsActionToolContact_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsActionToolContact_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsActionToolContact_Result():changetype<Ros2Ur_msgsActionToolContact_Result>(out.value);}
}
/** Complete original ur_msgs/action/ToolContact_Feedback; spellings are published in generation proof. */
export class Ros2Ur_msgsActionToolContact_Feedback {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsActionToolContact_Feedback{const value=new Ros2Ur_msgsActionToolContact_Feedback();
return value;}
}
export class Ros2Ur_msgsActionToolContact_FeedbackCodec implements VendorCodec<Ros2Ur_msgsActionToolContact_Feedback>{
static readonly codecIdentity:string="UWVMROS1_c6a11fd615f72a70a98514703de4c007a2bb161fbd8a953c5e8b899f471f4c5b";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_msgsActionToolContact_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsActionToolContact_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsActionToolContact_Feedback>{const out=new VendorDecoded<Ros2Ur_msgsActionToolContact_Feedback>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsActionToolContact_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsActionToolContact_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsActionToolContact_Feedback():changetype<Ros2Ur_msgsActionToolContact_Feedback>(out.value);}
}
/** Complete original ur_msgs/msg/Analog; spellings are published in generation proof. */
export class Ros2Ur_msgsMsgAnalog {
static readonly CURRENT:u8=0;
static readonly VOLTAGE:u8=1;
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field pin, native scalar uint8. */
pin:u8=0;
/** Exact native field domain, native scalar uint8. */
domain:u8=0;
/** Exact native field state, native scalar float32. */
state:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>6-n)return -90;n+=1;
if(n>6)return -90;
}
{
if(1>6-n)return -90;n+=1;
if(n>6)return -90;
}
{
if(!isFinite(this.state))return -22;
if(4>6-n)return -90;n+=4;
if(n>6)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.pin,1);
w.integer(<u64>this.domain,1);
w.integer(<u64>reinterpret<u32>(this.state),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsMsgAnalog{const value=new Ros2Ur_msgsMsgAnalog();
{
const decoded=<u8>r.integer(1);
value.pin=decoded;
}
{
const decoded=<u8>r.integer(1);
value.domain=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.state=decoded;
}
return value;}
}
export class Ros2Ur_msgsMsgAnalogCodec implements VendorCodec<Ros2Ur_msgsMsgAnalog>{
static readonly codecIdentity:string="UWVMROS1_68873e85836e1a65d7b67a52f5db8b3c3be082f889d2df4342386b282988f2a4";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2Ur_msgsMsgAnalog):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsMsgAnalog):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsMsgAnalog>{const out=new VendorDecoded<Ros2Ur_msgsMsgAnalog>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsMsgAnalog.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsMsgAnalog{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsMsgAnalog():changetype<Ros2Ur_msgsMsgAnalog>(out.value);}
}
/** Complete original ur_msgs/msg/Digital; spellings are published in generation proof. */
export class Ros2Ur_msgsMsgDigital {
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field pin, native scalar uint8. */
pin:u8=0;
/** Exact native field state, native scalar bool. */
state:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>2-n)return -90;n+=1;
if(n>2)return -90;
}
{
if(1>2-n)return -90;n+=1;
if(n>2)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.pin,1);
w.u8(this.state?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsMsgDigital{const value=new Ros2Ur_msgsMsgDigital();
{
const decoded=<u8>r.integer(1);
value.pin=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.state=decoded;
}
return value;}
}
export class Ros2Ur_msgsMsgDigitalCodec implements VendorCodec<Ros2Ur_msgsMsgDigital>{
static readonly codecIdentity:string="UWVMROS1_e5ebd0a17ee528cff1e0a636eb0be9c7e4128d3e77be722c5f1be73df4f4c11e";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Ur_msgsMsgDigital):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsMsgDigital):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsMsgDigital>{const out=new VendorDecoded<Ros2Ur_msgsMsgDigital>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsMsgDigital.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsMsgDigital{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsMsgDigital():changetype<Ros2Ur_msgsMsgDigital>(out.value);}
}
/** Complete original ur_msgs/msg/FrictionModelParameters; spellings are published in generation proof. */
export class Ros2Ur_msgsMsgFrictionModelParameters {
static readonly maxWireBytes:i32=104;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=104;
/** Exact native field viscous_scale, native scalar float64. */
viscous_scale:Array<f64>=new Array<f64>();
/** Exact native field coulomb_scale, native scalar float64. */
coulomb_scale:Array<f64>=new Array<f64>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.viscous_scale.length>6)return -22;
if(4>104-n)return -90;n+=4;
for(let i=0;i<this.viscous_scale.length;++i){
if(!isFinite(this.viscous_scale[i]))return -22;
if(8>104-n)return -90;n+=8;
if(n>104)return -90;
}
}
{
if(this.coulomb_scale.length>6)return -22;
if(4>104-n)return -90;n+=4;
for(let i=0;i<this.coulomb_scale.length;++i){
if(!isFinite(this.coulomb_scale[i]))return -22;
if(8>104-n)return -90;n+=8;
if(n>104)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.viscous_scale.length);
for(let i=0;i<this.viscous_scale.length;++i){
w.integer(<u64>reinterpret<u64>(this.viscous_scale[i]),8);
}
w.u32(<u32>this.coulomb_scale.length);
for(let i=0;i<this.coulomb_scale.length;++i){
w.integer(<u64>reinterpret<u64>(this.coulomb_scale[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsMsgFrictionModelParameters{const value=new Ros2Ur_msgsMsgFrictionModelParameters();
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.viscous_scale=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.viscous_scale.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.coulomb_scale=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.coulomb_scale.push(decoded);
}
}
return value;}
}
export class Ros2Ur_msgsMsgFrictionModelParametersCodec implements VendorCodec<Ros2Ur_msgsMsgFrictionModelParameters>{
static readonly codecIdentity:string="UWVMROS1_7cc9c7af77340f56b0559b4e2ad32972ff65f2a6cf89ebfbe2c48c645f8e9c89";
static readonly maxWireBytes:i32=104;
encodeChecked(value:Ros2Ur_msgsMsgFrictionModelParameters):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsMsgFrictionModelParameters):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsMsgFrictionModelParameters>{const out=new VendorDecoded<Ros2Ur_msgsMsgFrictionModelParameters>();if(bytes.length>104){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsMsgFrictionModelParameters.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsMsgFrictionModelParameters{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsMsgFrictionModelParameters():changetype<Ros2Ur_msgsMsgFrictionModelParameters>(out.value);}
}
/** Complete original ur_msgs/msg/IOStates; spellings are published in generation proof. */
export class Ros2Ur_msgsMsgIOStates {
static readonly maxWireBytes:i32=404;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=404;
/** Exact native field digital_in_states, native scalar ur_msgs/msg/Digital. */
digital_in_states:Array<Ros2Ur_msgsMsgDigital>=new Array<Ros2Ur_msgsMsgDigital>();
/** Exact native field digital_out_states, native scalar ur_msgs/msg/Digital. */
digital_out_states:Array<Ros2Ur_msgsMsgDigital>=new Array<Ros2Ur_msgsMsgDigital>();
/** Exact native field flag_states, native scalar ur_msgs/msg/Digital. */
flag_states:Array<Ros2Ur_msgsMsgDigital>=new Array<Ros2Ur_msgsMsgDigital>();
/** Exact native field analog_in_states, native scalar ur_msgs/msg/Analog. */
analog_in_states:Array<Ros2Ur_msgsMsgAnalog>=new Array<Ros2Ur_msgsMsgAnalog>();
/** Exact native field analog_out_states, native scalar ur_msgs/msg/Analog. */
analog_out_states:Array<Ros2Ur_msgsMsgAnalog>=new Array<Ros2Ur_msgsMsgAnalog>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.digital_in_states.length>32)return -22;
if(4>404-n)return -90;n+=4;
for(let i=0;i<this.digital_in_states.length;++i){
const child=this.digital_in_states[i].wireSize();if(child<0)return child;if(child>404-n)return -90;n+=child;
if(n>404)return -90;
}
}
{
if(this.digital_out_states.length>32)return -22;
if(4>404-n)return -90;n+=4;
for(let i=0;i<this.digital_out_states.length;++i){
const child=this.digital_out_states[i].wireSize();if(child<0)return child;if(child>404-n)return -90;n+=child;
if(n>404)return -90;
}
}
{
if(this.flag_states.length>32)return -22;
if(4>404-n)return -90;n+=4;
for(let i=0;i<this.flag_states.length;++i){
const child=this.flag_states[i].wireSize();if(child<0)return child;if(child>404-n)return -90;n+=child;
if(n>404)return -90;
}
}
{
if(this.analog_in_states.length>16)return -22;
if(4>404-n)return -90;n+=4;
for(let i=0;i<this.analog_in_states.length;++i){
const child=this.analog_in_states[i].wireSize();if(child<0)return child;if(child>404-n)return -90;n+=child;
if(n>404)return -90;
}
}
{
if(this.analog_out_states.length>16)return -22;
if(4>404-n)return -90;n+=4;
for(let i=0;i<this.analog_out_states.length;++i){
const child=this.analog_out_states[i].wireSize();if(child<0)return child;if(child>404-n)return -90;n+=child;
if(n>404)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.digital_in_states.length);
for(let i=0;i<this.digital_in_states.length;++i){
this.digital_in_states[i].writeControl(w);
}
w.u32(<u32>this.digital_out_states.length);
for(let i=0;i<this.digital_out_states.length;++i){
this.digital_out_states[i].writeControl(w);
}
w.u32(<u32>this.flag_states.length);
for(let i=0;i<this.flag_states.length;++i){
this.flag_states[i].writeControl(w);
}
w.u32(<u32>this.analog_in_states.length);
for(let i=0;i<this.analog_in_states.length;++i){
this.analog_in_states[i].writeControl(w);
}
w.u32(<u32>this.analog_out_states.length);
for(let i=0;i<this.analog_out_states.length;++i){
this.analog_out_states[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsMsgIOStates{const value=new Ros2Ur_msgsMsgIOStates();
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.digital_in_states=new Array<Ros2Ur_msgsMsgDigital>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Ur_msgsMsgDigital.readControl(r);
value.digital_in_states.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.digital_out_states=new Array<Ros2Ur_msgsMsgDigital>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Ur_msgsMsgDigital.readControl(r);
value.digital_out_states.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.flag_states=new Array<Ros2Ur_msgsMsgDigital>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Ur_msgsMsgDigital.readControl(r);
value.flag_states.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.analog_in_states=new Array<Ros2Ur_msgsMsgAnalog>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Ur_msgsMsgAnalog.readControl(r);
value.analog_in_states.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.analog_out_states=new Array<Ros2Ur_msgsMsgAnalog>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Ur_msgsMsgAnalog.readControl(r);
value.analog_out_states.push(decoded);
}
}
return value;}
}
export class Ros2Ur_msgsMsgIOStatesCodec implements VendorCodec<Ros2Ur_msgsMsgIOStates>{
static readonly codecIdentity:string="UWVMROS1_c22f9f784c89b6854fa84df416ed3704e3a312b15c8fc41162e7f5f53223fef5";
static readonly maxWireBytes:i32=404;
encodeChecked(value:Ros2Ur_msgsMsgIOStates):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsMsgIOStates):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsMsgIOStates>{const out=new VendorDecoded<Ros2Ur_msgsMsgIOStates>();if(bytes.length>404){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsMsgIOStates.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsMsgIOStates{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsMsgIOStates():changetype<Ros2Ur_msgsMsgIOStates>(out.value);}
}
/** Complete original ur_msgs/msg/MasterboardDataMsg; spellings are published in generation proof. */
export class Ros2Ur_msgsMsgMasterboardDataMsg {
static readonly maxWireBytes:i32=62;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=62;
/** Exact native field digital_input_bits, native scalar uint32. */
digital_input_bits:u32=0;
/** Exact native field digital_output_bits, native scalar uint32. */
digital_output_bits:u32=0;
/** Exact native field analog_input_range0, native scalar int8. */
analog_input_range0:i8=0;
/** Exact native field analog_input_range1, native scalar int8. */
analog_input_range1:i8=0;
/** Exact native field analog_input0, native scalar float64. */
analog_input0:f64=0;
/** Exact native field analog_input1, native scalar float64. */
analog_input1:f64=0;
/** Exact native field analog_output_domain0, native scalar int8. */
analog_output_domain0:i8=0;
/** Exact native field analog_output_domain1, native scalar int8. */
analog_output_domain1:i8=0;
/** Exact native field analog_output0, native scalar float64. */
analog_output0:f64=0;
/** Exact native field analog_output1, native scalar float64. */
analog_output1:f64=0;
/** Exact native field masterboard_temperature, native scalar float32. */
masterboard_temperature:f32=0;
/** Exact native field robot_voltage_48v, native scalar float32. */
robot_voltage_48v:f32=0;
/** Exact native field robot_current, native scalar float32. */
robot_current:f32=0;
/** Exact native field master_io_current, native scalar float32. */
master_io_current:f32=0;
/** Exact native field master_safety_state, native scalar uint8. */
master_safety_state:u8=0;
/** Exact native field master_onoff_state, native scalar uint8. */
master_onoff_state:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>62-n)return -90;n+=4;
if(n>62)return -90;
}
{
if(4>62-n)return -90;n+=4;
if(n>62)return -90;
}
{
if(1>62-n)return -90;n+=1;
if(n>62)return -90;
}
{
if(1>62-n)return -90;n+=1;
if(n>62)return -90;
}
{
if(!isFinite(this.analog_input0))return -22;
if(8>62-n)return -90;n+=8;
if(n>62)return -90;
}
{
if(!isFinite(this.analog_input1))return -22;
if(8>62-n)return -90;n+=8;
if(n>62)return -90;
}
{
if(1>62-n)return -90;n+=1;
if(n>62)return -90;
}
{
if(1>62-n)return -90;n+=1;
if(n>62)return -90;
}
{
if(!isFinite(this.analog_output0))return -22;
if(8>62-n)return -90;n+=8;
if(n>62)return -90;
}
{
if(!isFinite(this.analog_output1))return -22;
if(8>62-n)return -90;n+=8;
if(n>62)return -90;
}
{
if(!isFinite(this.masterboard_temperature))return -22;
if(4>62-n)return -90;n+=4;
if(n>62)return -90;
}
{
if(!isFinite(this.robot_voltage_48v))return -22;
if(4>62-n)return -90;n+=4;
if(n>62)return -90;
}
{
if(!isFinite(this.robot_current))return -22;
if(4>62-n)return -90;n+=4;
if(n>62)return -90;
}
{
if(!isFinite(this.master_io_current))return -22;
if(4>62-n)return -90;n+=4;
if(n>62)return -90;
}
{
if(1>62-n)return -90;n+=1;
if(n>62)return -90;
}
{
if(1>62-n)return -90;n+=1;
if(n>62)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.digital_input_bits,4);
w.integer(<u64>this.digital_output_bits,4);
w.integer(<u64>this.analog_input_range0,1);
w.integer(<u64>this.analog_input_range1,1);
w.integer(<u64>reinterpret<u64>(this.analog_input0),8);
w.integer(<u64>reinterpret<u64>(this.analog_input1),8);
w.integer(<u64>this.analog_output_domain0,1);
w.integer(<u64>this.analog_output_domain1,1);
w.integer(<u64>reinterpret<u64>(this.analog_output0),8);
w.integer(<u64>reinterpret<u64>(this.analog_output1),8);
w.integer(<u64>reinterpret<u32>(this.masterboard_temperature),4);
w.integer(<u64>reinterpret<u32>(this.robot_voltage_48v),4);
w.integer(<u64>reinterpret<u32>(this.robot_current),4);
w.integer(<u64>reinterpret<u32>(this.master_io_current),4);
w.integer(<u64>this.master_safety_state,1);
w.integer(<u64>this.master_onoff_state,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsMsgMasterboardDataMsg{const value=new Ros2Ur_msgsMsgMasterboardDataMsg();
{
const decoded=<u32>r.integer(4);
value.digital_input_bits=decoded;
}
{
const decoded=<u32>r.integer(4);
value.digital_output_bits=decoded;
}
{
const decoded=<i8>r.integer(1);
value.analog_input_range0=decoded;
}
{
const decoded=<i8>r.integer(1);
value.analog_input_range1=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.analog_input0=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.analog_input1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.analog_output_domain0=decoded;
}
{
const decoded=<i8>r.integer(1);
value.analog_output_domain1=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.analog_output0=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.analog_output1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.masterboard_temperature=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.robot_voltage_48v=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.robot_current=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.master_io_current=decoded;
}
{
const decoded=<u8>r.integer(1);
value.master_safety_state=decoded;
}
{
const decoded=<u8>r.integer(1);
value.master_onoff_state=decoded;
}
return value;}
}
export class Ros2Ur_msgsMsgMasterboardDataMsgCodec implements VendorCodec<Ros2Ur_msgsMsgMasterboardDataMsg>{
static readonly codecIdentity:string="UWVMROS1_eb3b0aab27bb5683859356ca7cf9648e53d311c1fd32cac97f68ce1d4622a9e5";
static readonly maxWireBytes:i32=62;
encodeChecked(value:Ros2Ur_msgsMsgMasterboardDataMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsMsgMasterboardDataMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsMsgMasterboardDataMsg>{const out=new VendorDecoded<Ros2Ur_msgsMsgMasterboardDataMsg>();if(bytes.length>62){out.status=90;return out;}if(bytes.length<62){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsMsgMasterboardDataMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsMsgMasterboardDataMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsMsgMasterboardDataMsg():changetype<Ros2Ur_msgsMsgMasterboardDataMsg>(out.value);}
}
/** Complete original ur_msgs/msg/RobotModeDataMsg; spellings are published in generation proof. */
export class Ros2Ur_msgsMsgRobotModeDataMsg {
static readonly maxWireBytes:i32=15;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=15;
/** Exact native field timestamp, native scalar uint64. */
timestamp:u64=0;
/** Exact native field is_robot_connected, native scalar bool. */
is_robot_connected:bool=false;
/** Exact native field is_real_robot_enabled, native scalar bool. */
is_real_robot_enabled:bool=false;
/** Exact native field is_power_on_robot, native scalar bool. */
is_power_on_robot:bool=false;
/** Exact native field is_emergency_stopped, native scalar bool. */
is_emergency_stopped:bool=false;
/** Exact native field is_protective_stopped, native scalar bool. */
is_protective_stopped:bool=false;
/** Exact native field is_program_running, native scalar bool. */
is_program_running:bool=false;
/** Exact native field is_program_paused, native scalar bool. */
is_program_paused:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>15-n)return -90;n+=8;
if(n>15)return -90;
}
{
if(1>15-n)return -90;n+=1;
if(n>15)return -90;
}
{
if(1>15-n)return -90;n+=1;
if(n>15)return -90;
}
{
if(1>15-n)return -90;n+=1;
if(n>15)return -90;
}
{
if(1>15-n)return -90;n+=1;
if(n>15)return -90;
}
{
if(1>15-n)return -90;n+=1;
if(n>15)return -90;
}
{
if(1>15-n)return -90;n+=1;
if(n>15)return -90;
}
{
if(1>15-n)return -90;n+=1;
if(n>15)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.timestamp,8);
w.u8(this.is_robot_connected?1:0);
w.u8(this.is_real_robot_enabled?1:0);
w.u8(this.is_power_on_robot?1:0);
w.u8(this.is_emergency_stopped?1:0);
w.u8(this.is_protective_stopped?1:0);
w.u8(this.is_program_running?1:0);
w.u8(this.is_program_paused?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsMsgRobotModeDataMsg{const value=new Ros2Ur_msgsMsgRobotModeDataMsg();
{
const decoded=<u64>r.integer(8);
value.timestamp=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.is_robot_connected=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.is_real_robot_enabled=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.is_power_on_robot=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.is_emergency_stopped=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.is_protective_stopped=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.is_program_running=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.is_program_paused=decoded;
}
return value;}
}
export class Ros2Ur_msgsMsgRobotModeDataMsgCodec implements VendorCodec<Ros2Ur_msgsMsgRobotModeDataMsg>{
static readonly codecIdentity:string="UWVMROS1_0181d85868dc6251289c6f7bc723757fa573c1345b6502497e335655d328bb31";
static readonly maxWireBytes:i32=15;
encodeChecked(value:Ros2Ur_msgsMsgRobotModeDataMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsMsgRobotModeDataMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsMsgRobotModeDataMsg>{const out=new VendorDecoded<Ros2Ur_msgsMsgRobotModeDataMsg>();if(bytes.length>15){out.status=90;return out;}if(bytes.length<15){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsMsgRobotModeDataMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsMsgRobotModeDataMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsMsgRobotModeDataMsg():changetype<Ros2Ur_msgsMsgRobotModeDataMsg>(out.value);}
}
/** Complete original ur_msgs/msg/RobotStateRTMsg; spellings are published in generation proof. */
export class Ros2Ur_msgsMsgRobotStateRTMsg {
static readonly maxWireBytes:i32=768;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=768;
/** Exact native field time, native scalar float64. */
time:f64=0;
/** Exact native field q_target, native scalar float64. */
q_target:Array<f64>=new Array<f64>();
/** Exact native field qd_target, native scalar float64. */
qd_target:Array<f64>=new Array<f64>();
/** Exact native field qdd_target, native scalar float64. */
qdd_target:Array<f64>=new Array<f64>();
/** Exact native field i_target, native scalar float64. */
i_target:Array<f64>=new Array<f64>();
/** Exact native field m_target, native scalar float64. */
m_target:Array<f64>=new Array<f64>();
/** Exact native field q_actual, native scalar float64. */
q_actual:Array<f64>=new Array<f64>();
/** Exact native field qd_actual, native scalar float64. */
qd_actual:Array<f64>=new Array<f64>();
/** Exact native field i_actual, native scalar float64. */
i_actual:Array<f64>=new Array<f64>();
/** Exact native field tool_acc_values, native scalar float64. */
tool_acc_values:Array<f64>=new Array<f64>();
/** Exact native field tcp_force, native scalar float64. */
tcp_force:Array<f64>=new Array<f64>();
/** Exact native field tool_vector, native scalar float64. */
tool_vector:Array<f64>=new Array<f64>();
/** Exact native field tcp_speed, native scalar float64. */
tcp_speed:Array<f64>=new Array<f64>();
/** Exact native field digital_input_bits, native scalar float64. */
digital_input_bits:f64=0;
/** Exact native field motor_temperatures, native scalar float64. */
motor_temperatures:Array<f64>=new Array<f64>();
/** Exact native field controller_timer, native scalar float64. */
controller_timer:f64=0;
/** Exact native field test_value, native scalar float64. */
test_value:f64=0;
/** Exact native field robot_mode, native scalar float64. */
robot_mode:f64=0;
/** Exact native field joint_modes, native scalar float64. */
joint_modes:Array<f64>=new Array<f64>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.time))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
{
if(this.q_target.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.q_target.length;++i){
if(!isFinite(this.q_target[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(this.qd_target.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.qd_target.length;++i){
if(!isFinite(this.qd_target[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(this.qdd_target.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.qdd_target.length;++i){
if(!isFinite(this.qdd_target[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(this.i_target.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.i_target.length;++i){
if(!isFinite(this.i_target[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(this.m_target.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.m_target.length;++i){
if(!isFinite(this.m_target[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(this.q_actual.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.q_actual.length;++i){
if(!isFinite(this.q_actual[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(this.qd_actual.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.qd_actual.length;++i){
if(!isFinite(this.qd_actual[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(this.i_actual.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.i_actual.length;++i){
if(!isFinite(this.i_actual[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(this.tool_acc_values.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.tool_acc_values.length;++i){
if(!isFinite(this.tool_acc_values[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(this.tcp_force.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.tcp_force.length;++i){
if(!isFinite(this.tcp_force[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(this.tool_vector.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.tool_vector.length;++i){
if(!isFinite(this.tool_vector[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(this.tcp_speed.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.tcp_speed.length;++i){
if(!isFinite(this.tcp_speed[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(!isFinite(this.digital_input_bits))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
{
if(this.motor_temperatures.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.motor_temperatures.length;++i){
if(!isFinite(this.motor_temperatures[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
{
if(!isFinite(this.controller_timer))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
{
if(!isFinite(this.test_value))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
{
if(!isFinite(this.robot_mode))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
{
if(this.joint_modes.length>6)return -22;
if(4>768-n)return -90;n+=4;
for(let i=0;i<this.joint_modes.length;++i){
if(!isFinite(this.joint_modes[i]))return -22;
if(8>768-n)return -90;n+=8;
if(n>768)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.time),8);
w.u32(<u32>this.q_target.length);
for(let i=0;i<this.q_target.length;++i){
w.integer(<u64>reinterpret<u64>(this.q_target[i]),8);
}
w.u32(<u32>this.qd_target.length);
for(let i=0;i<this.qd_target.length;++i){
w.integer(<u64>reinterpret<u64>(this.qd_target[i]),8);
}
w.u32(<u32>this.qdd_target.length);
for(let i=0;i<this.qdd_target.length;++i){
w.integer(<u64>reinterpret<u64>(this.qdd_target[i]),8);
}
w.u32(<u32>this.i_target.length);
for(let i=0;i<this.i_target.length;++i){
w.integer(<u64>reinterpret<u64>(this.i_target[i]),8);
}
w.u32(<u32>this.m_target.length);
for(let i=0;i<this.m_target.length;++i){
w.integer(<u64>reinterpret<u64>(this.m_target[i]),8);
}
w.u32(<u32>this.q_actual.length);
for(let i=0;i<this.q_actual.length;++i){
w.integer(<u64>reinterpret<u64>(this.q_actual[i]),8);
}
w.u32(<u32>this.qd_actual.length);
for(let i=0;i<this.qd_actual.length;++i){
w.integer(<u64>reinterpret<u64>(this.qd_actual[i]),8);
}
w.u32(<u32>this.i_actual.length);
for(let i=0;i<this.i_actual.length;++i){
w.integer(<u64>reinterpret<u64>(this.i_actual[i]),8);
}
w.u32(<u32>this.tool_acc_values.length);
for(let i=0;i<this.tool_acc_values.length;++i){
w.integer(<u64>reinterpret<u64>(this.tool_acc_values[i]),8);
}
w.u32(<u32>this.tcp_force.length);
for(let i=0;i<this.tcp_force.length;++i){
w.integer(<u64>reinterpret<u64>(this.tcp_force[i]),8);
}
w.u32(<u32>this.tool_vector.length);
for(let i=0;i<this.tool_vector.length;++i){
w.integer(<u64>reinterpret<u64>(this.tool_vector[i]),8);
}
w.u32(<u32>this.tcp_speed.length);
for(let i=0;i<this.tcp_speed.length;++i){
w.integer(<u64>reinterpret<u64>(this.tcp_speed[i]),8);
}
w.integer(<u64>reinterpret<u64>(this.digital_input_bits),8);
w.u32(<u32>this.motor_temperatures.length);
for(let i=0;i<this.motor_temperatures.length;++i){
w.integer(<u64>reinterpret<u64>(this.motor_temperatures[i]),8);
}
w.integer(<u64>reinterpret<u64>(this.controller_timer),8);
w.integer(<u64>reinterpret<u64>(this.test_value),8);
w.integer(<u64>reinterpret<u64>(this.robot_mode),8);
w.u32(<u32>this.joint_modes.length);
for(let i=0;i<this.joint_modes.length;++i){
w.integer(<u64>reinterpret<u64>(this.joint_modes[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsMsgRobotStateRTMsg{const value=new Ros2Ur_msgsMsgRobotStateRTMsg();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.time=decoded;
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.q_target=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.q_target.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.qd_target=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.qd_target.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.qdd_target=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.qdd_target.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.i_target=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.i_target.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.m_target=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.m_target.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.q_actual=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.q_actual.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.qd_actual=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.qd_actual.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.i_actual=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.i_actual.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.tool_acc_values=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.tool_acc_values.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.tcp_force=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.tcp_force.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.tool_vector=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.tool_vector.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.tcp_speed=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.tcp_speed.push(decoded);
}
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.digital_input_bits=decoded;
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.motor_temperatures=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.motor_temperatures.push(decoded);
}
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.controller_timer=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.test_value=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.robot_mode=decoded;
}
{
const count=r.u32();if(!r.valid||count>6){r.valid=false;return value;}
value.joint_modes=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.joint_modes.push(decoded);
}
}
return value;}
}
export class Ros2Ur_msgsMsgRobotStateRTMsgCodec implements VendorCodec<Ros2Ur_msgsMsgRobotStateRTMsg>{
static readonly codecIdentity:string="UWVMROS1_a30600c42e98b89da49d63073a5d30b0989aa0cb21f2bafec229dee10591d9cd";
static readonly maxWireBytes:i32=768;
encodeChecked(value:Ros2Ur_msgsMsgRobotStateRTMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsMsgRobotStateRTMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsMsgRobotStateRTMsg>{const out=new VendorDecoded<Ros2Ur_msgsMsgRobotStateRTMsg>();if(bytes.length>768){out.status=90;return out;}if(bytes.length<96){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsMsgRobotStateRTMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsMsgRobotStateRTMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsMsgRobotStateRTMsg():changetype<Ros2Ur_msgsMsgRobotStateRTMsg>(out.value);}
}
/** Complete original ur_msgs/msg/ToolDataMsg; spellings are published in generation proof. */
export class Ros2Ur_msgsMsgToolDataMsg {
static readonly ANALOG_INPUT_RANGE_CURRENT:i8=0;
static readonly ANALOG_INPUT_RANGE_VOLTAGE:i8=1;
static readonly TOOL_BOOTLOADER_MODE:u8=249;
static readonly TOOL_RUNNING_MODE:u8=253;
static readonly TOOL_IDLE_MODE:u8=255;
static readonly maxWireBytes:i32=32;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=32;
/** Exact native field analog_input_range2, native scalar int8. */
analog_input_range2:i8=0;
/** Exact native field analog_input_range3, native scalar int8. */
analog_input_range3:i8=0;
/** Exact native field analog_input2, native scalar float64. */
analog_input2:f64=0;
/** Exact native field analog_input3, native scalar float64. */
analog_input3:f64=0;
/** Exact native field tool_voltage_48v, native scalar float32. */
tool_voltage_48v:f32=0;
/** Exact native field tool_output_voltage, native scalar uint8. */
tool_output_voltage:u8=0;
/** Exact native field tool_current, native scalar float32. */
tool_current:f32=0;
/** Exact native field tool_temperature, native scalar float32. */
tool_temperature:f32=0;
/** Exact native field tool_mode, native scalar uint8. */
tool_mode:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>32-n)return -90;n+=1;
if(n>32)return -90;
}
{
if(1>32-n)return -90;n+=1;
if(n>32)return -90;
}
{
if(!isFinite(this.analog_input2))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.analog_input3))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.tool_voltage_48v))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(1>32-n)return -90;n+=1;
if(n>32)return -90;
}
{
if(!isFinite(this.tool_current))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(!isFinite(this.tool_temperature))return -22;
if(4>32-n)return -90;n+=4;
if(n>32)return -90;
}
{
if(1>32-n)return -90;n+=1;
if(n>32)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.analog_input_range2,1);
w.integer(<u64>this.analog_input_range3,1);
w.integer(<u64>reinterpret<u64>(this.analog_input2),8);
w.integer(<u64>reinterpret<u64>(this.analog_input3),8);
w.integer(<u64>reinterpret<u32>(this.tool_voltage_48v),4);
w.integer(<u64>this.tool_output_voltage,1);
w.integer(<u64>reinterpret<u32>(this.tool_current),4);
w.integer(<u64>reinterpret<u32>(this.tool_temperature),4);
w.integer(<u64>this.tool_mode,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsMsgToolDataMsg{const value=new Ros2Ur_msgsMsgToolDataMsg();
{
const decoded=<i8>r.integer(1);
value.analog_input_range2=decoded;
}
{
const decoded=<i8>r.integer(1);
value.analog_input_range3=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.analog_input2=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.analog_input3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.tool_voltage_48v=decoded;
}
{
const decoded=<u8>r.integer(1);
value.tool_output_voltage=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.tool_current=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.tool_temperature=decoded;
}
{
const decoded=<u8>r.integer(1);
value.tool_mode=decoded;
}
return value;}
}
export class Ros2Ur_msgsMsgToolDataMsgCodec implements VendorCodec<Ros2Ur_msgsMsgToolDataMsg>{
static readonly codecIdentity:string="UWVMROS1_1eec69666515ffbdca162a10967fce03ee4c25f0c27d56c5a812b87c9a229aa0";
static readonly maxWireBytes:i32=32;
encodeChecked(value:Ros2Ur_msgsMsgToolDataMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsMsgToolDataMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsMsgToolDataMsg>{const out=new VendorDecoded<Ros2Ur_msgsMsgToolDataMsg>();if(bytes.length>32){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsMsgToolDataMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsMsgToolDataMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsMsgToolDataMsg():changetype<Ros2Ur_msgsMsgToolDataMsg>(out.value);}
}
/** Complete original ur_msgs/srv/GetRobotSoftwareVersion_Request; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request {
static readonly maxWireBytes:i32=0;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request{const value=new Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request();
return value;}
}
export class Ros2Ur_msgsSrvGetRobotSoftwareVersion_RequestCodec implements VendorCodec<Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request>{
static readonly codecIdentity:string="UWVMROS1_c2d78972cb5b63138d1e780e8ff0c082dd77d5b76b09642d2c655e9f16780101";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request>{const out=new VendorDecoded<Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request():changetype<Ros2Ur_msgsSrvGetRobotSoftwareVersion_Request>(out.value);}
}
/** Complete original ur_msgs/srv/GetRobotSoftwareVersion_Response; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field major, native scalar uint32. */
major:u32=0;
/** Exact native field minor, native scalar uint32. */
minor:u32=0;
/** Exact native field bugfix, native scalar uint32. */
bugfix:u32=0;
/** Exact native field build, native scalar uint32. */
build:u32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.major,4);
w.integer(<u64>this.minor,4);
w.integer(<u64>this.bugfix,4);
w.integer(<u64>this.build,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response{const value=new Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response();
{
const decoded=<u32>r.integer(4);
value.major=decoded;
}
{
const decoded=<u32>r.integer(4);
value.minor=decoded;
}
{
const decoded=<u32>r.integer(4);
value.bugfix=decoded;
}
{
const decoded=<u32>r.integer(4);
value.build=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvGetRobotSoftwareVersion_ResponseCodec implements VendorCodec<Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response>{
static readonly codecIdentity:string="UWVMROS1_bbe9984c34d69f93ad8dbfe6059c6a2e1bf43dbd27eabd97cf0a9507308d814a";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response>{const out=new VendorDecoded<Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response():changetype<Ros2Ur_msgsSrvGetRobotSoftwareVersion_Response>(out.value);}
}
/** Complete original ur_msgs/srv/SetAnalogOutput_Request; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetAnalogOutput_Request {
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field data, native scalar ur_msgs/msg/Analog. */
data:Ros2Ur_msgsMsgAnalog=new Ros2Ur_msgsMsgAnalog();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.data.wireSize();if(child<0)return child;if(child>6-n)return -90;n+=child;
if(n>6)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.data.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetAnalogOutput_Request{const value=new Ros2Ur_msgsSrvSetAnalogOutput_Request();
{
const decoded=Ros2Ur_msgsMsgAnalog.readControl(r);
value.data=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetAnalogOutput_RequestCodec implements VendorCodec<Ros2Ur_msgsSrvSetAnalogOutput_Request>{
static readonly codecIdentity:string="UWVMROS1_71c3eca5c6c90758f084e275fc495feb5d1c08a467c97d941fb5864c54309bea";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2Ur_msgsSrvSetAnalogOutput_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetAnalogOutput_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetAnalogOutput_Request>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetAnalogOutput_Request>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetAnalogOutput_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetAnalogOutput_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetAnalogOutput_Request():changetype<Ros2Ur_msgsSrvSetAnalogOutput_Request>(out.value);}
}
/** Complete original ur_msgs/srv/SetAnalogOutput_Response; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetAnalogOutput_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>1-n)return -90;n+=1;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetAnalogOutput_Response{const value=new Ros2Ur_msgsSrvSetAnalogOutput_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetAnalogOutput_ResponseCodec implements VendorCodec<Ros2Ur_msgsSrvSetAnalogOutput_Response>{
static readonly codecIdentity:string="UWVMROS1_71a5f3580657cb3ca702dbb63b7af3330c922624c9e9774e47e6533e2fc0a05a";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ur_msgsSrvSetAnalogOutput_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetAnalogOutput_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetAnalogOutput_Response>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetAnalogOutput_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetAnalogOutput_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetAnalogOutput_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetAnalogOutput_Response():changetype<Ros2Ur_msgsSrvSetAnalogOutput_Response>(out.value);}
}
/** Complete original ur_msgs/srv/SetForceMode_Request; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetForceMode_Request {
static readonly TCP_TO_ORIGIN:u8=1;
static readonly NO_TRANSFORM:u8=2;
static readonly TCP_VELOCITY_TO_X_Y:u8=3;
static readonly maxWireBytes:i32=331;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=331;
/** Exact native field task_frame, native scalar geometry_msgs/msg/PoseStamped. */
task_frame:Ros2Geometry_msgsMsgPoseStamped=new Ros2Geometry_msgsMsgPoseStamped();
/** Exact native field selection_vector_x, native scalar bool. */
selection_vector_x:bool=false;
/** Exact native field selection_vector_y, native scalar bool. */
selection_vector_y:bool=false;
/** Exact native field selection_vector_z, native scalar bool. */
selection_vector_z:bool=false;
/** Exact native field selection_vector_rx, native scalar bool. */
selection_vector_rx:bool=false;
/** Exact native field selection_vector_ry, native scalar bool. */
selection_vector_ry:bool=false;
/** Exact native field selection_vector_rz, native scalar bool. */
selection_vector_rz:bool=false;
/** Exact native field wrench, native scalar geometry_msgs/msg/Wrench. */
wrench:Ros2Geometry_msgsMsgWrench=new Ros2Geometry_msgsMsgWrench();
/** Exact native field type, native scalar uint8. */
ros_type:u8=2;
/** Exact native field speed_limits, native scalar geometry_msgs/msg/Twist. */
speed_limits:Ros2Geometry_msgsMsgTwist=new Ros2Geometry_msgsMsgTwist();
/** Exact native field deviation_limits, native scalar float32. */
deviation_limits:Array<f32>=new Array<f32>();
/** Exact native field damping_factor, native scalar float32. */
damping_factor:f32=reinterpret<f32>(u32(0x3ccccccd));
/** Exact native field gain_scaling, native scalar float32. */
gain_scaling:f32=reinterpret<f32>(u32(0x3f000000));
constructor(){
this.deviation_limits.push(reinterpret<f32>(u32(0x3c23d70a)));this.deviation_limits.push(reinterpret<f32>(u32(0x3c23d70a)));this.deviation_limits.push(reinterpret<f32>(u32(0x3c23d70a)));this.deviation_limits.push(reinterpret<f32>(u32(0x3c23d70a)));this.deviation_limits.push(reinterpret<f32>(u32(0x3c23d70a)));this.deviation_limits.push(reinterpret<f32>(u32(0x3c23d70a)));
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.task_frame.wireSize();if(child<0)return child;if(child>331-n)return -90;n+=child;
if(n>331)return -90;
}
{
if(1>331-n)return -90;n+=1;
if(n>331)return -90;
}
{
if(1>331-n)return -90;n+=1;
if(n>331)return -90;
}
{
if(1>331-n)return -90;n+=1;
if(n>331)return -90;
}
{
if(1>331-n)return -90;n+=1;
if(n>331)return -90;
}
{
if(1>331-n)return -90;n+=1;
if(n>331)return -90;
}
{
if(1>331-n)return -90;n+=1;
if(n>331)return -90;
}
{
const child=this.wrench.wireSize();if(child<0)return child;if(child>331-n)return -90;n+=child;
if(n>331)return -90;
}
{
if(1>331-n)return -90;n+=1;
if(n>331)return -90;
}
{
const child=this.speed_limits.wireSize();if(child<0)return child;if(child>331-n)return -90;n+=child;
if(n>331)return -90;
}
{
if(this.deviation_limits.length!=6)return -22;
for(let i=0;i<this.deviation_limits.length;++i){
if(!isFinite(this.deviation_limits[i]))return -22;
if(4>331-n)return -90;n+=4;
if(n>331)return -90;
}
}
{
if(!isFinite(this.damping_factor))return -22;
if(4>331-n)return -90;n+=4;
if(n>331)return -90;
}
{
if(!isFinite(this.gain_scaling))return -22;
if(4>331-n)return -90;n+=4;
if(n>331)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.task_frame.writeControl(w);
w.u8(this.selection_vector_x?1:0);
w.u8(this.selection_vector_y?1:0);
w.u8(this.selection_vector_z?1:0);
w.u8(this.selection_vector_rx?1:0);
w.u8(this.selection_vector_ry?1:0);
w.u8(this.selection_vector_rz?1:0);
this.wrench.writeControl(w);
w.integer(<u64>this.ros_type,1);
this.speed_limits.writeControl(w);
for(let i=0;i<this.deviation_limits.length;++i){
w.integer(<u64>reinterpret<u32>(this.deviation_limits[i]),4);
}
w.integer(<u64>reinterpret<u32>(this.damping_factor),4);
w.integer(<u64>reinterpret<u32>(this.gain_scaling),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetForceMode_Request{const value=new Ros2Ur_msgsSrvSetForceMode_Request();
{
const decoded=Ros2Geometry_msgsMsgPoseStamped.readControl(r);
value.task_frame=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.selection_vector_x=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.selection_vector_y=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.selection_vector_z=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.selection_vector_rx=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.selection_vector_ry=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.selection_vector_rz=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgWrench.readControl(r);
value.wrench=decoded;
}
{
const decoded=<u8>r.integer(1);
value.ros_type=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwist.readControl(r);
value.speed_limits=decoded;
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.deviation_limits=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.deviation_limits.push(decoded);
}
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.damping_factor=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.gain_scaling=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetForceMode_RequestCodec implements VendorCodec<Ros2Ur_msgsSrvSetForceMode_Request>{
static readonly codecIdentity:string="UWVMROS1_e43a6c22510a2078de3cb2dc17ba1b7184080a6aa29a1653b7986723c0cfef6e";
static readonly maxWireBytes:i32=331;
encodeChecked(value:Ros2Ur_msgsSrvSetForceMode_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetForceMode_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetForceMode_Request>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetForceMode_Request>();if(bytes.length>331){out.status=90;return out;}if(bytes.length<203){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetForceMode_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetForceMode_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetForceMode_Request():changetype<Ros2Ur_msgsSrvSetForceMode_Request>(out.value);}
}
/** Complete original ur_msgs/srv/SetForceMode_Response; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetForceMode_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>1-n)return -90;n+=1;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetForceMode_Response{const value=new Ros2Ur_msgsSrvSetForceMode_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetForceMode_ResponseCodec implements VendorCodec<Ros2Ur_msgsSrvSetForceMode_Response>{
static readonly codecIdentity:string="UWVMROS1_3f5438d426b7b68557ac8f43dc8efc2e2106782ec0cd4fd0dee7cdd69ddc7de8";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ur_msgsSrvSetForceMode_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetForceMode_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetForceMode_Response>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetForceMode_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetForceMode_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetForceMode_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetForceMode_Response():changetype<Ros2Ur_msgsSrvSetForceMode_Response>(out.value);}
}
/** Complete original ur_msgs/srv/SetFrictionModelParameters_Request; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetFrictionModelParameters_Request {
static readonly maxWireBytes:i32=104;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=104;
/** Exact native field parameters, native scalar ur_msgs/msg/FrictionModelParameters. */
parameters:Ros2Ur_msgsMsgFrictionModelParameters=new Ros2Ur_msgsMsgFrictionModelParameters();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.parameters.wireSize();if(child<0)return child;if(child>104-n)return -90;n+=child;
if(n>104)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.parameters.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetFrictionModelParameters_Request{const value=new Ros2Ur_msgsSrvSetFrictionModelParameters_Request();
{
const decoded=Ros2Ur_msgsMsgFrictionModelParameters.readControl(r);
value.parameters=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetFrictionModelParameters_RequestCodec implements VendorCodec<Ros2Ur_msgsSrvSetFrictionModelParameters_Request>{
static readonly codecIdentity:string="UWVMROS1_e44ad15f9c45d5a65eb13e6bc6e973bb20eebdafa04a5f2e8bcf4b0b54ce91c2";
static readonly maxWireBytes:i32=104;
encodeChecked(value:Ros2Ur_msgsSrvSetFrictionModelParameters_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetFrictionModelParameters_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetFrictionModelParameters_Request>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetFrictionModelParameters_Request>();if(bytes.length>104){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetFrictionModelParameters_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetFrictionModelParameters_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetFrictionModelParameters_Request():changetype<Ros2Ur_msgsSrvSetFrictionModelParameters_Request>(out.value);}
}
/** Complete original ur_msgs/srv/SetFrictionModelParameters_Response; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetFrictionModelParameters_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>1-n)return -90;n+=1;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetFrictionModelParameters_Response{const value=new Ros2Ur_msgsSrvSetFrictionModelParameters_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetFrictionModelParameters_ResponseCodec implements VendorCodec<Ros2Ur_msgsSrvSetFrictionModelParameters_Response>{
static readonly codecIdentity:string="UWVMROS1_1caca29dda3dc32360fd010dfb352b6feb54bba66ee16edd2f4353e3382207b3";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ur_msgsSrvSetFrictionModelParameters_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetFrictionModelParameters_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetFrictionModelParameters_Response>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetFrictionModelParameters_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetFrictionModelParameters_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetFrictionModelParameters_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetFrictionModelParameters_Response():changetype<Ros2Ur_msgsSrvSetFrictionModelParameters_Response>(out.value);}
}
/** Complete original ur_msgs/srv/SetGravity_Request; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetGravity_Request {
static readonly maxWireBytes:i32=164;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=164;
/** Exact native field gravity, native scalar geometry_msgs/msg/Vector3Stamped. */
gravity:Ros2Geometry_msgsMsgVector3Stamped=new Ros2Geometry_msgsMsgVector3Stamped();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.gravity.wireSize();if(child<0)return child;if(child>164-n)return -90;n+=child;
if(n>164)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.gravity.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetGravity_Request{const value=new Ros2Ur_msgsSrvSetGravity_Request();
{
const decoded=Ros2Geometry_msgsMsgVector3Stamped.readControl(r);
value.gravity=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetGravity_RequestCodec implements VendorCodec<Ros2Ur_msgsSrvSetGravity_Request>{
static readonly codecIdentity:string="UWVMROS1_3d7fed6ae8accbddd5b09f93c3e49f2b9f99cae4cf80a951eb31ace30ec5a834";
static readonly maxWireBytes:i32=164;
encodeChecked(value:Ros2Ur_msgsSrvSetGravity_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetGravity_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetGravity_Request>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetGravity_Request>();if(bytes.length>164){out.status=90;return out;}if(bytes.length<36){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetGravity_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetGravity_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetGravity_Request():changetype<Ros2Ur_msgsSrvSetGravity_Request>(out.value);}
}
/** Complete original ur_msgs/srv/SetGravity_Response; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetGravity_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field status, native scalar string. */
status:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.status)||this.status.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.status);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.status);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetGravity_Response{const value=new Ros2Ur_msgsSrvSetGravity_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.status=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetGravity_ResponseCodec implements VendorCodec<Ros2Ur_msgsSrvSetGravity_Response>{
static readonly codecIdentity:string="UWVMROS1_84694df7fbb4fd15bfecaf1aae683ef1795b0c4941d1987d951467448da5f793";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Ur_msgsSrvSetGravity_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetGravity_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetGravity_Response>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetGravity_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetGravity_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetGravity_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetGravity_Response():changetype<Ros2Ur_msgsSrvSetGravity_Response>(out.value);}
}
/** Complete original ur_msgs/srv/SetIO_Request; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetIO_Request {
static readonly PIN_AOUT0:i8=0;
static readonly PIN_AOUT1:i8=1;
static readonly PIN_DOUT0:i8=0;
static readonly PIN_DOUT1:i8=1;
static readonly PIN_DOUT2:i8=2;
static readonly PIN_DOUT3:i8=3;
static readonly PIN_DOUT4:i8=4;
static readonly PIN_DOUT5:i8=5;
static readonly PIN_DOUT6:i8=6;
static readonly PIN_DOUT7:i8=7;
static readonly PIN_CONF_OUT0:i8=8;
static readonly PIN_CONF_OUT1:i8=9;
static readonly PIN_CONF_OUT2:i8=10;
static readonly PIN_CONF_OUT3:i8=11;
static readonly PIN_CONF_OUT4:i8=12;
static readonly PIN_CONF_OUT5:i8=13;
static readonly PIN_CONF_OUT6:i8=14;
static readonly PIN_CONF_OUT7:i8=15;
static readonly PIN_TOOL_DOUT0:i8=16;
static readonly PIN_TOOL_DOUT1:i8=17;
static readonly FUN_SET_DIGITAL_OUT:i8=1;
static readonly FUN_SET_FLAG:i8=2;
static readonly FUN_SET_ANALOG_OUT:i8=3;
static readonly FUN_SET_TOOL_VOLTAGE:i8=4;
static readonly STATE_OFF:i8=0;
static readonly STATE_ON:i8=1;
static readonly STATE_TOOL_VOLTAGE_0V:i8=0;
static readonly STATE_TOOL_VOLTAGE_12V:i8=12;
static readonly STATE_TOOL_VOLTAGE_24V:i8=24;
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field fun, native scalar int8. */
fun:i8=0;
/** Exact native field pin, native scalar int8. */
pin:i8=0;
/** Exact native field state, native scalar float32. */
state:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>6-n)return -90;n+=1;
if(n>6)return -90;
}
{
if(1>6-n)return -90;n+=1;
if(n>6)return -90;
}
{
if(!isFinite(this.state))return -22;
if(4>6-n)return -90;n+=4;
if(n>6)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.fun,1);
w.integer(<u64>this.pin,1);
w.integer(<u64>reinterpret<u32>(this.state),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetIO_Request{const value=new Ros2Ur_msgsSrvSetIO_Request();
{
const decoded=<i8>r.integer(1);
value.fun=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.state=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetIO_RequestCodec implements VendorCodec<Ros2Ur_msgsSrvSetIO_Request>{
static readonly codecIdentity:string="UWVMROS1_c8ac4dddeb474521b4c701a0229e99af954416b7b8d7f32d2a19629d38b378c7";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2Ur_msgsSrvSetIO_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetIO_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetIO_Request>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetIO_Request>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetIO_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetIO_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetIO_Request():changetype<Ros2Ur_msgsSrvSetIO_Request>(out.value);}
}
/** Complete original ur_msgs/srv/SetIO_Response; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetIO_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>1-n)return -90;n+=1;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetIO_Response{const value=new Ros2Ur_msgsSrvSetIO_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetIO_ResponseCodec implements VendorCodec<Ros2Ur_msgsSrvSetIO_Response>{
static readonly codecIdentity:string="UWVMROS1_bd32373034093ccdab140a5ba8ce8a420ef2ce441430e8306b2484f0ca535090";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ur_msgsSrvSetIO_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetIO_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetIO_Response>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetIO_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetIO_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetIO_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetIO_Response():changetype<Ros2Ur_msgsSrvSetIO_Response>(out.value);}
}
/** Complete original ur_msgs/srv/SetPayload_Request; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetPayload_Request {
static readonly maxWireBytes:i32=84;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=84;
/** Exact native field mass, native scalar float32. */
mass:f32=0;
/** Exact native field center_of_gravity, native scalar geometry_msgs/msg/Vector3. */
center_of_gravity:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field ixx, native scalar float64. */
ixx:f64=0;
/** Exact native field iyy, native scalar float64. */
iyy:f64=0;
/** Exact native field izz, native scalar float64. */
izz:f64=0;
/** Exact native field ixy, native scalar float64. */
ixy:f64=0;
/** Exact native field ixz, native scalar float64. */
ixz:f64=0;
/** Exact native field iyz, native scalar float64. */
iyz:f64=0;
/** Exact native field transition_time, native scalar builtin_interfaces/msg/Duration. */
transition_time:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.mass))return -22;
if(4>84-n)return -90;n+=4;
if(n>84)return -90;
}
{
const child=this.center_of_gravity.wireSize();if(child<0)return child;if(child>84-n)return -90;n+=child;
if(n>84)return -90;
}
{
if(!isFinite(this.ixx))return -22;
if(8>84-n)return -90;n+=8;
if(n>84)return -90;
}
{
if(!isFinite(this.iyy))return -22;
if(8>84-n)return -90;n+=8;
if(n>84)return -90;
}
{
if(!isFinite(this.izz))return -22;
if(8>84-n)return -90;n+=8;
if(n>84)return -90;
}
{
if(!isFinite(this.ixy))return -22;
if(8>84-n)return -90;n+=8;
if(n>84)return -90;
}
{
if(!isFinite(this.ixz))return -22;
if(8>84-n)return -90;n+=8;
if(n>84)return -90;
}
{
if(!isFinite(this.iyz))return -22;
if(8>84-n)return -90;n+=8;
if(n>84)return -90;
}
{
const child=this.transition_time.wireSize();if(child<0)return child;if(child>84-n)return -90;n+=child;
if(n>84)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.mass),4);
this.center_of_gravity.writeControl(w);
w.integer(<u64>reinterpret<u64>(this.ixx),8);
w.integer(<u64>reinterpret<u64>(this.iyy),8);
w.integer(<u64>reinterpret<u64>(this.izz),8);
w.integer(<u64>reinterpret<u64>(this.ixy),8);
w.integer(<u64>reinterpret<u64>(this.ixz),8);
w.integer(<u64>reinterpret<u64>(this.iyz),8);
this.transition_time.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetPayload_Request{const value=new Ros2Ur_msgsSrvSetPayload_Request();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.mass=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.center_of_gravity=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.ixx=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.iyy=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.izz=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.ixy=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.ixz=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.iyz=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.transition_time=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetPayload_RequestCodec implements VendorCodec<Ros2Ur_msgsSrvSetPayload_Request>{
static readonly codecIdentity:string="UWVMROS1_e76e256e1accf2d742c4cf529e5af1e830db75f3db703a1f368d28077a87e735";
static readonly maxWireBytes:i32=84;
encodeChecked(value:Ros2Ur_msgsSrvSetPayload_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetPayload_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetPayload_Request>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetPayload_Request>();if(bytes.length>84){out.status=90;return out;}if(bytes.length<84){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetPayload_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetPayload_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetPayload_Request():changetype<Ros2Ur_msgsSrvSetPayload_Request>(out.value);}
}
/** Complete original ur_msgs/srv/SetPayload_Response; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetPayload_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>1-n)return -90;n+=1;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetPayload_Response{const value=new Ros2Ur_msgsSrvSetPayload_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetPayload_ResponseCodec implements VendorCodec<Ros2Ur_msgsSrvSetPayload_Response>{
static readonly codecIdentity:string="UWVMROS1_036dbaa7d42b6cba6ad29a082e078401d507292483d260ba532f7d6947c1624c";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ur_msgsSrvSetPayload_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetPayload_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetPayload_Response>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetPayload_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetPayload_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetPayload_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetPayload_Response():changetype<Ros2Ur_msgsSrvSetPayload_Response>(out.value);}
}
/** Complete original ur_msgs/srv/SetSpeedSliderFraction_Request; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetSpeedSliderFraction_Request {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field speed_slider_fraction, native scalar float64. */
speed_slider_fraction:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.speed_slider_fraction))return -22;
if(8>8-n)return -90;n+=8;
if(n>8)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.speed_slider_fraction),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetSpeedSliderFraction_Request{const value=new Ros2Ur_msgsSrvSetSpeedSliderFraction_Request();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.speed_slider_fraction=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetSpeedSliderFraction_RequestCodec implements VendorCodec<Ros2Ur_msgsSrvSetSpeedSliderFraction_Request>{
static readonly codecIdentity:string="UWVMROS1_d8f19fb240e4b70cad74c5111cdaebad14b480f21da953c95d474064b091abd8";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Ur_msgsSrvSetSpeedSliderFraction_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetSpeedSliderFraction_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetSpeedSliderFraction_Request>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetSpeedSliderFraction_Request>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetSpeedSliderFraction_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetSpeedSliderFraction_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetSpeedSliderFraction_Request():changetype<Ros2Ur_msgsSrvSetSpeedSliderFraction_Request>(out.value);}
}
/** Complete original ur_msgs/srv/SetSpeedSliderFraction_Response; spellings are published in generation proof. */
export class Ros2Ur_msgsSrvSetSpeedSliderFraction_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>1-n)return -90;n+=1;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ur_msgsSrvSetSpeedSliderFraction_Response{const value=new Ros2Ur_msgsSrvSetSpeedSliderFraction_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Ur_msgsSrvSetSpeedSliderFraction_ResponseCodec implements VendorCodec<Ros2Ur_msgsSrvSetSpeedSliderFraction_Response>{
static readonly codecIdentity:string="UWVMROS1_1ab1ad5ab4b1ed302710ec055dff7644cc6e966af919ff311eaea6cddcf7dbb9";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ur_msgsSrvSetSpeedSliderFraction_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ur_msgsSrvSetSpeedSliderFraction_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ur_msgsSrvSetSpeedSliderFraction_Response>{const out=new VendorDecoded<Ros2Ur_msgsSrvSetSpeedSliderFraction_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ur_msgsSrvSetSpeedSliderFraction_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ur_msgsSrvSetSpeedSliderFraction_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ur_msgsSrvSetSpeedSliderFraction_Response():changetype<Ros2Ur_msgsSrvSetSpeedSliderFraction_Response>(out.value);}
}
