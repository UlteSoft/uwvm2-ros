/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
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
/** Complete original std_msgs/msg/String; spellings are published in generation proof. */
export class Ros2Std_msgsMsgString {
static readonly maxWireBytes:i32=1028;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1028;
/** Exact native field data, native scalar string. */
data:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.data)||this.data.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.data);if(size>1024)return -22;if(4+size>1028-n)return -90;n+=4+size;
if(n>1028)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.data);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Std_msgsMsgString{const value=new Ros2Std_msgsMsgString();
{
const size=r.u32();if(!r.valid||size>1024){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.data=decoded;
}
return value;}
}
export class Ros2Std_msgsMsgStringCodec implements VendorCodec<Ros2Std_msgsMsgString>{
static readonly codecIdentity:string="UWVMROS1_c48b5497cbf487f56eb073a982b93acd501c6bb72f0e9ef9dce2175ce7ac15c5";
static readonly maxWireBytes:i32=1028;
encodeChecked(value:Ros2Std_msgsMsgString):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Std_msgsMsgString):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Std_msgsMsgString>{const out=new VendorDecoded<Ros2Std_msgsMsgString>();if(bytes.length>1028){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Std_msgsMsgString.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Std_msgsMsgString{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Std_msgsMsgString():changetype<Ros2Std_msgsMsgString>(out.value);}
}
/** Complete original urinterfaces/msg/RobotMode; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgRobotMode {
static readonly DISCONNECTED:i8=0;
static readonly CONFIRM_SAFETY:i8=1;
static readonly BOOTING:i8=2;
static readonly POWER_OFF:i8=3;
static readonly POWER_ON:i8=4;
static readonly IDLE:i8=5;
static readonly BACKDRIVE:i8=6;
static readonly RUNNING:i8=7;
static readonly UPDATING_FIRMWARE:i8=8;
static readonly maxWireBytes:i32=141;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=141;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar int8. */
data:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>141-n)return -90;n+=child;
if(n>141)return -90;
}
{
if(1>141-n)return -90;n+=1;
if(n>141)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>this.data,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgRobotMode{const value=new Ros2UrinterfacesMsgRobotMode();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=<i8>r.integer(1);
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgRobotModeCodec implements VendorCodec<Ros2UrinterfacesMsgRobotMode>{
static readonly codecIdentity:string="UWVMROS1_a16ec0ed8caa1a2b48cd4116d88877911921848469cd139f4783e4f2c549e7ea";
static readonly maxWireBytes:i32=141;
encodeChecked(value:Ros2UrinterfacesMsgRobotMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgRobotMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgRobotMode>{const out=new VendorDecoded<Ros2UrinterfacesMsgRobotMode>();if(bytes.length>141){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgRobotMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgRobotMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgRobotMode():changetype<Ros2UrinterfacesMsgRobotMode>(out.value);}
}
/** Complete original urinterfaces/action/SetMode_Goal; spellings are published in generation proof. */
export class Ros2UrinterfacesActionSetMode_Goal {
static readonly maxWireBytes:i32=143;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=143;
/** Exact native field target_robot_mode, native scalar urinterfaces/msg/RobotMode. */
target_robot_mode:Ros2UrinterfacesMsgRobotMode=new Ros2UrinterfacesMsgRobotMode();
/** Exact native field stop_program, native scalar bool. */
stop_program:bool=false;
/** Exact native field play_program, native scalar bool. */
play_program:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.target_robot_mode.wireSize();if(child<0)return child;if(child>143-n)return -90;n+=child;
if(n>143)return -90;
}
{
if(1>143-n)return -90;n+=1;
if(n>143)return -90;
}
{
if(1>143-n)return -90;n+=1;
if(n>143)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.target_robot_mode.writeControl(w);
w.u8(this.stop_program?1:0);
w.u8(this.play_program?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesActionSetMode_Goal{const value=new Ros2UrinterfacesActionSetMode_Goal();
{
const decoded=Ros2UrinterfacesMsgRobotMode.readControl(r);
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
export class Ros2UrinterfacesActionSetMode_GoalCodec implements VendorCodec<Ros2UrinterfacesActionSetMode_Goal>{
static readonly codecIdentity:string="UWVMROS1_d1d998e217d6f3d3eefd8ffccecff8463580346150a59a1c7a2dac8d609e0e09";
static readonly maxWireBytes:i32=143;
encodeChecked(value:Ros2UrinterfacesActionSetMode_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesActionSetMode_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesActionSetMode_Goal>{const out=new VendorDecoded<Ros2UrinterfacesActionSetMode_Goal>();if(bytes.length>143){out.status=90;return out;}if(bytes.length<15){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesActionSetMode_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesActionSetMode_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesActionSetMode_Goal():changetype<Ros2UrinterfacesActionSetMode_Goal>(out.value);}
}
/** Complete original urinterfaces/action/SetMode_Result; spellings are published in generation proof. */
export class Ros2UrinterfacesActionSetMode_Result {
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
static readControl(r:Reader):Ros2UrinterfacesActionSetMode_Result{const value=new Ros2UrinterfacesActionSetMode_Result();
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
export class Ros2UrinterfacesActionSetMode_ResultCodec implements VendorCodec<Ros2UrinterfacesActionSetMode_Result>{
static readonly codecIdentity:string="UWVMROS1_500243a508824c42fe2da424040aa60bf2426ac607f2f6016fab014f81518af7";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2UrinterfacesActionSetMode_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesActionSetMode_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesActionSetMode_Result>{const out=new VendorDecoded<Ros2UrinterfacesActionSetMode_Result>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesActionSetMode_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesActionSetMode_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesActionSetMode_Result():changetype<Ros2UrinterfacesActionSetMode_Result>(out.value);}
}
/** Complete original urinterfaces/action/SetMode_Feedback; spellings are published in generation proof. */
export class Ros2UrinterfacesActionSetMode_Feedback {
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
static readControl(r:Reader):Ros2UrinterfacesActionSetMode_Feedback{const value=new Ros2UrinterfacesActionSetMode_Feedback();
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
export class Ros2UrinterfacesActionSetMode_FeedbackCodec implements VendorCodec<Ros2UrinterfacesActionSetMode_Feedback>{
static readonly codecIdentity:string="UWVMROS1_22fb71c17066829bd66c98bc631c82b2bfef3c501e08960a6bfad3d902f76537";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2UrinterfacesActionSetMode_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesActionSetMode_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesActionSetMode_Feedback>{const out=new VendorDecoded<Ros2UrinterfacesActionSetMode_Feedback>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesActionSetMode_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesActionSetMode_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesActionSetMode_Feedback():changetype<Ros2UrinterfacesActionSetMode_Feedback>(out.value);}
}
/** Complete original urinterfaces/msg/ActiveTCP; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgActiveTCP {
static readonly maxWireBytes:i32=400;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=400;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar string. */
data:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>400-n)return -90;n+=child;
if(n>400)return -90;
}
{
if(!validUnicode(this.data)||this.data.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.data);if(size>256)return -22;if(4+size>400-n)return -90;n+=4+size;
if(n>400)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.text(this.data);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgActiveTCP{const value=new Ros2UrinterfacesMsgActiveTCP();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgActiveTCPCodec implements VendorCodec<Ros2UrinterfacesMsgActiveTCP>{
static readonly codecIdentity:string="UWVMROS1_e62d31728eca201dee256d88d1d268bf2ecb9607ed5c48783432f966edf459ae";
static readonly maxWireBytes:i32=400;
encodeChecked(value:Ros2UrinterfacesMsgActiveTCP):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgActiveTCP):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgActiveTCP>{const out=new VendorDecoded<Ros2UrinterfacesMsgActiveTCP>();if(bytes.length>400){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgActiveTCP.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgActiveTCP{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgActiveTCP():changetype<Ros2UrinterfacesMsgActiveTCP>(out.value);}
}
/** Complete original urinterfaces/msg/Analog; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgAnalog {
static readonly CURRENT:u8=0;
static readonly VOLTAGE:u8=1;
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field pin, native scalar uint8. */
pin:u8=0;
/** Exact native field domain, native scalar uint8. */
domain:u8=0;
/** Exact native field value, native scalar float32. */
value:f32=0;
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
if(!isFinite(this.value))return -22;
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
w.integer(<u64>reinterpret<u32>(this.value),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgAnalog{const value=new Ros2UrinterfacesMsgAnalog();
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
value.value=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgAnalogCodec implements VendorCodec<Ros2UrinterfacesMsgAnalog>{
static readonly codecIdentity:string="UWVMROS1_e81cffd909f3bc387ee00c37537434341ef93fd160e465be033aa9bc412b979b";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2UrinterfacesMsgAnalog):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgAnalog):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgAnalog>{const out=new VendorDecoded<Ros2UrinterfacesMsgAnalog>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgAnalog.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgAnalog{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgAnalog():changetype<Ros2UrinterfacesMsgAnalog>(out.value);}
}
/** Complete original urinterfaces/msg/AnalogIO; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgAnalogIO {
static readonly maxWireBytes:i32=200;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=200;
/** Exact native field analog_inputs, native scalar urinterfaces/msg/Analog. */
analog_inputs:Array<Ros2UrinterfacesMsgAnalog>=new Array<Ros2UrinterfacesMsgAnalog>();
/** Exact native field analog_outputs, native scalar urinterfaces/msg/Analog. */
analog_outputs:Array<Ros2UrinterfacesMsgAnalog>=new Array<Ros2UrinterfacesMsgAnalog>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.analog_inputs.length>16)return -22;
if(4>200-n)return -90;n+=4;
for(let i=0;i<this.analog_inputs.length;++i){
const child=this.analog_inputs[i].wireSize();if(child<0)return child;if(child>200-n)return -90;n+=child;
if(n>200)return -90;
}
}
{
if(this.analog_outputs.length>16)return -22;
if(4>200-n)return -90;n+=4;
for(let i=0;i<this.analog_outputs.length;++i){
const child=this.analog_outputs[i].wireSize();if(child<0)return child;if(child>200-n)return -90;n+=child;
if(n>200)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.analog_inputs.length);
for(let i=0;i<this.analog_inputs.length;++i){
this.analog_inputs[i].writeControl(w);
}
w.u32(<u32>this.analog_outputs.length);
for(let i=0;i<this.analog_outputs.length;++i){
this.analog_outputs[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgAnalogIO{const value=new Ros2UrinterfacesMsgAnalogIO();
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.analog_inputs=new Array<Ros2UrinterfacesMsgAnalog>();for(let i:u32=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgAnalog.readControl(r);
value.analog_inputs.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.analog_outputs=new Array<Ros2UrinterfacesMsgAnalog>();for(let i:u32=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgAnalog.readControl(r);
value.analog_outputs.push(decoded);
}
}
return value;}
}
export class Ros2UrinterfacesMsgAnalogIOCodec implements VendorCodec<Ros2UrinterfacesMsgAnalogIO>{
static readonly codecIdentity:string="UWVMROS1_3130f3e3857f21bab5092d9c6abea8c45965c07ebb9c1ab14b28fa51b7d71e0c";
static readonly maxWireBytes:i32=200;
encodeChecked(value:Ros2UrinterfacesMsgAnalogIO):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgAnalogIO):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgAnalogIO>{const out=new VendorDecoded<Ros2UrinterfacesMsgAnalogIO>();if(bytes.length>200){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgAnalogIO.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgAnalogIO{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgAnalogIO():changetype<Ros2UrinterfacesMsgAnalogIO>(out.value);}
}
/** Complete original urinterfaces/msg/AnalogIOStateStamped; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgAnalogIOStateStamped {
static readonly maxWireBytes:i32=340;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=340;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field io_state, native scalar urinterfaces/msg/AnalogIO. */
io_state:Ros2UrinterfacesMsgAnalogIO=new Ros2UrinterfacesMsgAnalogIO();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>340-n)return -90;n+=child;
if(n>340)return -90;
}
{
const child=this.io_state.wireSize();if(child<0)return child;if(child>340-n)return -90;n+=child;
if(n>340)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.io_state.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgAnalogIOStateStamped{const value=new Ros2UrinterfacesMsgAnalogIOStateStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2UrinterfacesMsgAnalogIO.readControl(r);
value.io_state=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgAnalogIOStateStampedCodec implements VendorCodec<Ros2UrinterfacesMsgAnalogIOStateStamped>{
static readonly codecIdentity:string="UWVMROS1_a40e839bb4acfcb26503c79f01216b7aea9ab8ff25c50dca42578b206ab40a89";
static readonly maxWireBytes:i32=340;
encodeChecked(value:Ros2UrinterfacesMsgAnalogIOStateStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgAnalogIOStateStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgAnalogIOStateStamped>{const out=new VendorDecoded<Ros2UrinterfacesMsgAnalogIOStateStamped>();if(bytes.length>340){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgAnalogIOStateStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgAnalogIOStateStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgAnalogIOStateStamped():changetype<Ros2UrinterfacesMsgAnalogIOStateStamped>(out.value);}
}
/** Complete original urinterfaces/msg/AnalogStamped; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgAnalogStamped {
static readonly maxWireBytes:i32=146;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=146;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar urinterfaces/msg/Analog. */
data:Ros2UrinterfacesMsgAnalog=new Ros2UrinterfacesMsgAnalog();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>146-n)return -90;n+=child;
if(n>146)return -90;
}
{
const child=this.data.wireSize();if(child<0)return child;if(child>146-n)return -90;n+=child;
if(n>146)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.data.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgAnalogStamped{const value=new Ros2UrinterfacesMsgAnalogStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2UrinterfacesMsgAnalog.readControl(r);
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgAnalogStampedCodec implements VendorCodec<Ros2UrinterfacesMsgAnalogStamped>{
static readonly codecIdentity:string="UWVMROS1_749557c6e79b882385927c62d3c45963797ce116d7be6235bd9f186a7b4da76c";
static readonly maxWireBytes:i32=146;
encodeChecked(value:Ros2UrinterfacesMsgAnalogStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgAnalogStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgAnalogStamped>{const out=new VendorDecoded<Ros2UrinterfacesMsgAnalogStamped>();if(bytes.length>146){out.status=90;return out;}if(bytes.length<18){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgAnalogStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgAnalogStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgAnalogStamped():changetype<Ros2UrinterfacesMsgAnalogStamped>(out.value);}
}
/** Complete original urinterfaces/msg/BoolStamped; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgBoolStamped {
static readonly maxWireBytes:i32=141;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=141;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar bool. */
data:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>141-n)return -90;n+=child;
if(n>141)return -90;
}
{
if(1>141-n)return -90;n+=1;
if(n>141)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u8(this.data?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgBoolStamped{const value=new Ros2UrinterfacesMsgBoolStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgBoolStampedCodec implements VendorCodec<Ros2UrinterfacesMsgBoolStamped>{
static readonly codecIdentity:string="UWVMROS1_1a4128638df10bd9a8d33af0ed3c2ee9b4deb771d45dfe83f2a9a75c7f77de66";
static readonly maxWireBytes:i32=141;
encodeChecked(value:Ros2UrinterfacesMsgBoolStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgBoolStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgBoolStamped>{const out=new VendorDecoded<Ros2UrinterfacesMsgBoolStamped>();if(bytes.length>141){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgBoolStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgBoolStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgBoolStamped():changetype<Ros2UrinterfacesMsgBoolStamped>(out.value);}
}
/** Complete original urinterfaces/msg/ControlCycle; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgControlCycle {
static readonly maxWireBytes:i32=148;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=148;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar uint64. */
data:u64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>148-n)return -90;n+=child;
if(n>148)return -90;
}
{
if(8>148-n)return -90;n+=8;
if(n>148)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>this.data,8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgControlCycle{const value=new Ros2UrinterfacesMsgControlCycle();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=<u64>r.integer(8);
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgControlCycleCodec implements VendorCodec<Ros2UrinterfacesMsgControlCycle>{
static readonly codecIdentity:string="UWVMROS1_99a4d96e5673f5ac49eab13cd8b28af55ac83e1626c39ed20ff46655df4d6278";
static readonly maxWireBytes:i32=148;
encodeChecked(value:Ros2UrinterfacesMsgControlCycle):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgControlCycle):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgControlCycle>{const out=new VendorDecoded<Ros2UrinterfacesMsgControlCycle>();if(bytes.length>148){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgControlCycle.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgControlCycle{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgControlCycle():changetype<Ros2UrinterfacesMsgControlCycle>(out.value);}
}
/** Complete original urinterfaces/msg/Current; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgCurrent {
static readonly maxWireBytes:i32=144;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=144;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar float32. */
data:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>144-n)return -90;n+=child;
if(n>144)return -90;
}
{
if(!isFinite(this.data))return -22;
if(4>144-n)return -90;n+=4;
if(n>144)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>reinterpret<u32>(this.data),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgCurrent{const value=new Ros2UrinterfacesMsgCurrent();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgCurrentCodec implements VendorCodec<Ros2UrinterfacesMsgCurrent>{
static readonly codecIdentity:string="UWVMROS1_759f2139335bcb720f7f4a38cd8b1413f203f2dd2b72149fa0de0fb7a0a778df";
static readonly maxWireBytes:i32=144;
encodeChecked(value:Ros2UrinterfacesMsgCurrent):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgCurrent):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgCurrent>{const out=new VendorDecoded<Ros2UrinterfacesMsgCurrent>();if(bytes.length>144){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgCurrent.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgCurrent{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgCurrent():changetype<Ros2UrinterfacesMsgCurrent>(out.value);}
}
/** Complete original urinterfaces/msg/Digital; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgDigital {
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
static readControl(r:Reader):Ros2UrinterfacesMsgDigital{const value=new Ros2UrinterfacesMsgDigital();
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
export class Ros2UrinterfacesMsgDigitalCodec implements VendorCodec<Ros2UrinterfacesMsgDigital>{
static readonly codecIdentity:string="UWVMROS1_c60c73df0383bb9a3e23ae3148cff9d6eb49b88c2d10add0af6a86ec9af54512";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2UrinterfacesMsgDigital):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgDigital):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgDigital>{const out=new VendorDecoded<Ros2UrinterfacesMsgDigital>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgDigital.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgDigital{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgDigital():changetype<Ros2UrinterfacesMsgDigital>(out.value);}
}
/** Complete original urinterfaces/msg/DigitalIO; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgDigitalIO {
static readonly maxWireBytes:i32=136;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=136;
/** Exact native field digital_inputs, native scalar urinterfaces/msg/Digital. */
digital_inputs:Array<Ros2UrinterfacesMsgDigital>=new Array<Ros2UrinterfacesMsgDigital>();
/** Exact native field digital_outputs, native scalar urinterfaces/msg/Digital. */
digital_outputs:Array<Ros2UrinterfacesMsgDigital>=new Array<Ros2UrinterfacesMsgDigital>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.digital_inputs.length>32)return -22;
if(4>136-n)return -90;n+=4;
for(let i=0;i<this.digital_inputs.length;++i){
const child=this.digital_inputs[i].wireSize();if(child<0)return child;if(child>136-n)return -90;n+=child;
if(n>136)return -90;
}
}
{
if(this.digital_outputs.length>32)return -22;
if(4>136-n)return -90;n+=4;
for(let i=0;i<this.digital_outputs.length;++i){
const child=this.digital_outputs[i].wireSize();if(child<0)return child;if(child>136-n)return -90;n+=child;
if(n>136)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.digital_inputs.length);
for(let i=0;i<this.digital_inputs.length;++i){
this.digital_inputs[i].writeControl(w);
}
w.u32(<u32>this.digital_outputs.length);
for(let i=0;i<this.digital_outputs.length;++i){
this.digital_outputs[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgDigitalIO{const value=new Ros2UrinterfacesMsgDigitalIO();
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.digital_inputs=new Array<Ros2UrinterfacesMsgDigital>();for(let i:u32=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgDigital.readControl(r);
value.digital_inputs.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.digital_outputs=new Array<Ros2UrinterfacesMsgDigital>();for(let i:u32=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgDigital.readControl(r);
value.digital_outputs.push(decoded);
}
}
return value;}
}
export class Ros2UrinterfacesMsgDigitalIOCodec implements VendorCodec<Ros2UrinterfacesMsgDigitalIO>{
static readonly codecIdentity:string="UWVMROS1_11235b6010feb7e3e5e6c2be8acd3fbba1c25ecbf97ae5e0c899a0e3de3c4e4a";
static readonly maxWireBytes:i32=136;
encodeChecked(value:Ros2UrinterfacesMsgDigitalIO):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgDigitalIO):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgDigitalIO>{const out=new VendorDecoded<Ros2UrinterfacesMsgDigitalIO>();if(bytes.length>136){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgDigitalIO.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgDigitalIO{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgDigitalIO():changetype<Ros2UrinterfacesMsgDigitalIO>(out.value);}
}
/** Complete original urinterfaces/msg/DigitalIOStateStamped; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgDigitalIOStateStamped {
static readonly maxWireBytes:i32=412;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=412;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field configurable_io, native scalar urinterfaces/msg/DigitalIO. */
configurable_io:Ros2UrinterfacesMsgDigitalIO=new Ros2UrinterfacesMsgDigitalIO();
/** Exact native field standard_io, native scalar urinterfaces/msg/DigitalIO. */
standard_io:Ros2UrinterfacesMsgDigitalIO=new Ros2UrinterfacesMsgDigitalIO();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>412-n)return -90;n+=child;
if(n>412)return -90;
}
{
const child=this.configurable_io.wireSize();if(child<0)return child;if(child>412-n)return -90;n+=child;
if(n>412)return -90;
}
{
const child=this.standard_io.wireSize();if(child<0)return child;if(child>412-n)return -90;n+=child;
if(n>412)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.configurable_io.writeControl(w);
this.standard_io.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgDigitalIOStateStamped{const value=new Ros2UrinterfacesMsgDigitalIOStateStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2UrinterfacesMsgDigitalIO.readControl(r);
value.configurable_io=decoded;
}
{
const decoded=Ros2UrinterfacesMsgDigitalIO.readControl(r);
value.standard_io=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgDigitalIOStateStampedCodec implements VendorCodec<Ros2UrinterfacesMsgDigitalIOStateStamped>{
static readonly codecIdentity:string="UWVMROS1_542db76916f234e79216804426dc4e8d386a0ef547907391c921f51ed45e64c9";
static readonly maxWireBytes:i32=412;
encodeChecked(value:Ros2UrinterfacesMsgDigitalIOStateStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgDigitalIOStateStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgDigitalIOStateStamped>{const out=new VendorDecoded<Ros2UrinterfacesMsgDigitalIOStateStamped>();if(bytes.length>412){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgDigitalIOStateStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgDigitalIOStateStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgDigitalIOStateStamped():changetype<Ros2UrinterfacesMsgDigitalIOStateStamped>(out.value);}
}
/** Complete original urinterfaces/msg/DigitalStamped; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgDigitalStamped {
static readonly maxWireBytes:i32=142;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=142;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar urinterfaces/msg/Digital. */
data:Ros2UrinterfacesMsgDigital=new Ros2UrinterfacesMsgDigital();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>142-n)return -90;n+=child;
if(n>142)return -90;
}
{
const child=this.data.wireSize();if(child<0)return child;if(child>142-n)return -90;n+=child;
if(n>142)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.data.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgDigitalStamped{const value=new Ros2UrinterfacesMsgDigitalStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2UrinterfacesMsgDigital.readControl(r);
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgDigitalStampedCodec implements VendorCodec<Ros2UrinterfacesMsgDigitalStamped>{
static readonly codecIdentity:string="UWVMROS1_dc86d5a04c0801089ddf6b61b5fd33d63df8b5599550c6a0e84e19bbff375865";
static readonly maxWireBytes:i32=142;
encodeChecked(value:Ros2UrinterfacesMsgDigitalStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgDigitalStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgDigitalStamped>{const out=new VendorDecoded<Ros2UrinterfacesMsgDigitalStamped>();if(bytes.length>142){out.status=90;return out;}if(bytes.length<14){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgDigitalStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgDigitalStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgDigitalStamped():changetype<Ros2UrinterfacesMsgDigitalStamped>(out.value);}
}
/** Complete original urinterfaces/msg/DoubleMultiArrayStamped; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgDoubleMultiArrayStamped {
static readonly maxWireBytes:i32=272;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=272;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar float64. */
data:Array<f64>=new Array<f64>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>272-n)return -90;n+=child;
if(n>272)return -90;
}
{
if(this.data.length>16)return -22;
if(4>272-n)return -90;n+=4;
for(let i=0;i<this.data.length;++i){
if(!isFinite(this.data[i]))return -22;
if(8>272-n)return -90;n+=8;
if(n>272)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.data.length);
for(let i=0;i<this.data.length;++i){
w.integer(<u64>reinterpret<u64>(this.data[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgDoubleMultiArrayStamped{const value=new Ros2UrinterfacesMsgDoubleMultiArrayStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.data=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.data.push(decoded);
}
}
return value;}
}
export class Ros2UrinterfacesMsgDoubleMultiArrayStampedCodec implements VendorCodec<Ros2UrinterfacesMsgDoubleMultiArrayStamped>{
static readonly codecIdentity:string="UWVMROS1_82b52f88248ea8d6cd7b809d1fd5aa55b4bbe1618044e739998a7ec74a23a8ae";
static readonly maxWireBytes:i32=272;
encodeChecked(value:Ros2UrinterfacesMsgDoubleMultiArrayStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgDoubleMultiArrayStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgDoubleMultiArrayStamped>{const out=new VendorDecoded<Ros2UrinterfacesMsgDoubleMultiArrayStamped>();if(bytes.length>272){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgDoubleMultiArrayStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgDoubleMultiArrayStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgDoubleMultiArrayStamped():changetype<Ros2UrinterfacesMsgDoubleMultiArrayStamped>(out.value);}
}
/** Complete original urinterfaces/msg/Float32Stamped; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgFloat32Stamped {
static readonly maxWireBytes:i32=144;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=144;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar float32. */
data:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>144-n)return -90;n+=child;
if(n>144)return -90;
}
{
if(!isFinite(this.data))return -22;
if(4>144-n)return -90;n+=4;
if(n>144)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>reinterpret<u32>(this.data),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgFloat32Stamped{const value=new Ros2UrinterfacesMsgFloat32Stamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgFloat32StampedCodec implements VendorCodec<Ros2UrinterfacesMsgFloat32Stamped>{
static readonly codecIdentity:string="UWVMROS1_5989940d19890b6d3b78172e9920708745b78e35c1c4fb238412c37a813d30d3";
static readonly maxWireBytes:i32=144;
encodeChecked(value:Ros2UrinterfacesMsgFloat32Stamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgFloat32Stamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgFloat32Stamped>{const out=new VendorDecoded<Ros2UrinterfacesMsgFloat32Stamped>();if(bytes.length>144){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgFloat32Stamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgFloat32Stamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgFloat32Stamped():changetype<Ros2UrinterfacesMsgFloat32Stamped>(out.value);}
}
/** Complete original urinterfaces/msg/Float64Stamped; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgFloat64Stamped {
static readonly maxWireBytes:i32=148;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=148;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar float64. */
data:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>148-n)return -90;n+=child;
if(n>148)return -90;
}
{
if(!isFinite(this.data))return -22;
if(8>148-n)return -90;n+=8;
if(n>148)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>reinterpret<u64>(this.data),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgFloat64Stamped{const value=new Ros2UrinterfacesMsgFloat64Stamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgFloat64StampedCodec implements VendorCodec<Ros2UrinterfacesMsgFloat64Stamped>{
static readonly codecIdentity:string="UWVMROS1_4f8950feff6f20dbdbf9cac1f01168824be3698a4bd90355a470b580930edb21";
static readonly maxWireBytes:i32=148;
encodeChecked(value:Ros2UrinterfacesMsgFloat64Stamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgFloat64Stamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgFloat64Stamped>{const out=new VendorDecoded<Ros2UrinterfacesMsgFloat64Stamped>();if(bytes.length>148){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgFloat64Stamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgFloat64Stamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgFloat64Stamped():changetype<Ros2UrinterfacesMsgFloat64Stamped>(out.value);}
}
/** Complete original urinterfaces/msg/JointMode; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgJointMode {
static readonly JOINT_MODE_RESET:u8=235;
static readonly JOINT_MODE_SHUTTING_DOWN:u8=236;
static readonly JOINT_MODE_PART_D_CALIBRATION:u8=237;
static readonly JOINT_MODE_BACKDRIVE:u8=238;
static readonly JOINT_MODE_POWER_OFF:u8=239;
static readonly JOINT_MODE_READY_FOR_POWER_OFF:u8=240;
static readonly JOINT_MODE_NOT_RESPONDING:u8=245;
static readonly JOINT_MODE_MOTOR_INITIALISATION:u8=246;
static readonly JOINT_MODE_BOOTING:u8=247;
static readonly JOINT_MODE_PART_D_CALIBRATION_ERROR:u8=248;
static readonly JOINT_MODE_BOOTLOADER:u8=249;
static readonly JOINT_MODE_CALIBRATION:u8=250;
static readonly JOINT_MODE_VIOLATION:u8=251;
static readonly JOINT_MODE_FAULT:u8=252;
static readonly JOINT_MODE_RUNNING:u8=253;
static readonly JOINT_MODE_IDLE:u8=255;
static readonly maxWireBytes:i32=147;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=147;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field joint_modes, native scalar uint8. */
joint_modes:Array<u8>=new Array<u8>();
/** Exact native field tool_mode, native scalar uint8. */
tool_mode:u8=0;
constructor(){
for(let i=0;i<6;++i)this.joint_modes.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>147-n)return -90;n+=child;
if(n>147)return -90;
}
{
if(this.joint_modes.length!=6)return -22;
for(let i=0;i<this.joint_modes.length;++i){
if(1>147-n)return -90;n+=1;
if(n>147)return -90;
}
}
{
if(1>147-n)return -90;n+=1;
if(n>147)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
for(let i=0;i<this.joint_modes.length;++i){
w.integer(<u64>this.joint_modes[i],1);
}
w.integer(<u64>this.tool_mode,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgJointMode{const value=new Ros2UrinterfacesMsgJointMode();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.joint_modes=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.joint_modes.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.tool_mode=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgJointModeCodec implements VendorCodec<Ros2UrinterfacesMsgJointMode>{
static readonly codecIdentity:string="UWVMROS1_6d766192096ced3c8568a98fb286c309e00cfb97e34b2611fff70e5bb1eaa95c";
static readonly maxWireBytes:i32=147;
encodeChecked(value:Ros2UrinterfacesMsgJointMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgJointMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgJointMode>{const out=new VendorDecoded<Ros2UrinterfacesMsgJointMode>();if(bytes.length>147){out.status=90;return out;}if(bytes.length<19){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgJointMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgJointMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgJointMode():changetype<Ros2UrinterfacesMsgJointMode>(out.value);}
}
/** Complete original urinterfaces/msg/JointTemperature; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgJointTemperature {
static readonly maxWireBytes:i32=164;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=164;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar float32. */
data:Array<f32>=new Array<f32>();
constructor(){
for(let i=0;i<6;++i)this.data.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>164-n)return -90;n+=child;
if(n>164)return -90;
}
{
if(this.data.length!=6)return -22;
for(let i=0;i<this.data.length;++i){
if(!isFinite(this.data[i]))return -22;
if(4>164-n)return -90;n+=4;
if(n>164)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
for(let i=0;i<this.data.length;++i){
w.integer(<u64>reinterpret<u32>(this.data[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgJointTemperature{const value=new Ros2UrinterfacesMsgJointTemperature();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.data=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.data.push(decoded);
}
}
return value;}
}
export class Ros2UrinterfacesMsgJointTemperatureCodec implements VendorCodec<Ros2UrinterfacesMsgJointTemperature>{
static readonly codecIdentity:string="UWVMROS1_af77f1ec4c6b4d71e6387315d2388e7493c229dbf216848e61d5638cb308d017";
static readonly maxWireBytes:i32=164;
encodeChecked(value:Ros2UrinterfacesMsgJointTemperature):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgJointTemperature):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgJointTemperature>{const out=new VendorDecoded<Ros2UrinterfacesMsgJointTemperature>();if(bytes.length>164){out.status=90;return out;}if(bytes.length<36){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgJointTemperature.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgJointTemperature{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgJointTemperature():changetype<Ros2UrinterfacesMsgJointTemperature>(out.value);}
}
/** Complete original urinterfaces/msg/JointVoltage; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgJointVoltage {
static readonly maxWireBytes:i32=188;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=188;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar float64. */
data:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<6;++i)this.data.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>188-n)return -90;n+=child;
if(n>188)return -90;
}
{
if(this.data.length!=6)return -22;
for(let i=0;i<this.data.length;++i){
if(!isFinite(this.data[i]))return -22;
if(8>188-n)return -90;n+=8;
if(n>188)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
for(let i=0;i<this.data.length;++i){
w.integer(<u64>reinterpret<u64>(this.data[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgJointVoltage{const value=new Ros2UrinterfacesMsgJointVoltage();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.data=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.data.push(decoded);
}
}
return value;}
}
export class Ros2UrinterfacesMsgJointVoltageCodec implements VendorCodec<Ros2UrinterfacesMsgJointVoltage>{
static readonly codecIdentity:string="UWVMROS1_9fa2179402b5dc29b898ac1321c1f6f86676300939776a6e32de4529f3776b61";
static readonly maxWireBytes:i32=188;
encodeChecked(value:Ros2UrinterfacesMsgJointVoltage):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgJointVoltage):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgJointVoltage>{const out=new VendorDecoded<Ros2UrinterfacesMsgJointVoltage>();if(bytes.length>188){out.status=90;return out;}if(bytes.length<60){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgJointVoltage.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgJointVoltage{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgJointVoltage():changetype<Ros2UrinterfacesMsgJointVoltage>(out.value);}
}
/** Complete original urinterfaces/msg/JsonStringType; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgJsonStringType {
static readonly maxWireBytes:i32=1028;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1028;
/** Exact native field json_string, native scalar std_msgs/msg/String. */
json_string:Ros2Std_msgsMsgString=new Ros2Std_msgsMsgString();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.json_string.wireSize();if(child<0)return child;if(child>1028-n)return -90;n+=child;
if(n>1028)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.json_string.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgJsonStringType{const value=new Ros2UrinterfacesMsgJsonStringType();
{
const decoded=Ros2Std_msgsMsgString.readControl(r);
value.json_string=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgJsonStringTypeCodec implements VendorCodec<Ros2UrinterfacesMsgJsonStringType>{
static readonly codecIdentity:string="UWVMROS1_05da0dcf2728cd73f875421c457804a146ceffe76e9d4619be71c2c92efdd728";
static readonly maxWireBytes:i32=1028;
encodeChecked(value:Ros2UrinterfacesMsgJsonStringType):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgJsonStringType):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgJsonStringType>{const out=new VendorDecoded<Ros2UrinterfacesMsgJsonStringType>();if(bytes.length>1028){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgJsonStringType.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgJsonStringType{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgJsonStringType():changetype<Ros2UrinterfacesMsgJsonStringType>(out.value);}
}
/** Complete original urinterfaces/msg/RuntimeState; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgRuntimeState {
static readonly PROGRAM_STATE_UNDEFINED:i8=-1;
static readonly PROGRAM_STATE_STOPPING:i8=0;
static readonly PROGRAM_STATE_STOPPED:i8=1;
static readonly PROGRAM_STATE_RUNNING:i8=2;
static readonly PROGRAM_STATE_PAUSING:i8=3;
static readonly PROGRAM_STATE_PAUSED:i8=4;
static readonly PROGRAM_STATE_RESUMING:i8=5;
static readonly PROGRAM_STATE_RETRACTING:i8=6;
static readonly maxWireBytes:i32=141;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=141;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar int8. */
data:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>141-n)return -90;n+=child;
if(n>141)return -90;
}
{
if(1>141-n)return -90;n+=1;
if(n>141)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>this.data,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgRuntimeState{const value=new Ros2UrinterfacesMsgRuntimeState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=<i8>r.integer(1);
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgRuntimeStateCodec implements VendorCodec<Ros2UrinterfacesMsgRuntimeState>{
static readonly codecIdentity:string="UWVMROS1_711ca0e324cbeb36fcedea4881b8658a4b453402c410c8d3efaa57205aaf74b6";
static readonly maxWireBytes:i32=141;
encodeChecked(value:Ros2UrinterfacesMsgRuntimeState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgRuntimeState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgRuntimeState>{const out=new VendorDecoded<Ros2UrinterfacesMsgRuntimeState>();if(bytes.length>141){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgRuntimeState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgRuntimeState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgRuntimeState():changetype<Ros2UrinterfacesMsgRuntimeState>(out.value);}
}
/** Complete original urinterfaces/msg/SafetyStatus; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgSafetyStatus {
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
static readonly maxWireBytes:i32=141;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=141;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar int8. */
data:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>141-n)return -90;n+=child;
if(n>141)return -90;
}
{
if(1>141-n)return -90;n+=1;
if(n>141)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>this.data,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgSafetyStatus{const value=new Ros2UrinterfacesMsgSafetyStatus();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=<i8>r.integer(1);
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgSafetyStatusCodec implements VendorCodec<Ros2UrinterfacesMsgSafetyStatus>{
static readonly codecIdentity:string="UWVMROS1_8f266e1ee8ab2ee9791ff4b52f9dae8ef982844c8d24a0bdfb4bb93cc55b78d4";
static readonly maxWireBytes:i32=141;
encodeChecked(value:Ros2UrinterfacesMsgSafetyStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgSafetyStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgSafetyStatus>{const out=new VendorDecoded<Ros2UrinterfacesMsgSafetyStatus>();if(bytes.length>141){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgSafetyStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgSafetyStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgSafetyStatus():changetype<Ros2UrinterfacesMsgSafetyStatus>(out.value);}
}
/** Complete original urinterfaces/msg/SharedVariablesEditResponse; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgSharedVariablesEditResponse {
static readonly maxWireBytes:i32=1029;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1029;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field result_message, native scalar std_msgs/msg/String. */
result_message:Ros2Std_msgsMsgString=new Ros2Std_msgsMsgString();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>1029-n)return -90;n+=1;
if(n>1029)return -90;
}
{
const child=this.result_message.wireSize();if(child<0)return child;if(child>1029-n)return -90;n+=child;
if(n>1029)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
this.result_message.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgSharedVariablesEditResponse{const value=new Ros2UrinterfacesMsgSharedVariablesEditResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const decoded=Ros2Std_msgsMsgString.readControl(r);
value.result_message=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgSharedVariablesEditResponseCodec implements VendorCodec<Ros2UrinterfacesMsgSharedVariablesEditResponse>{
static readonly codecIdentity:string="UWVMROS1_870ce8f649642b6edcce8ba5c7cb27818ec138d7afb2cde926638d21ea748212";
static readonly maxWireBytes:i32=1029;
encodeChecked(value:Ros2UrinterfacesMsgSharedVariablesEditResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgSharedVariablesEditResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgSharedVariablesEditResponse>{const out=new VendorDecoded<Ros2UrinterfacesMsgSharedVariablesEditResponse>();if(bytes.length>1029){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgSharedVariablesEditResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgSharedVariablesEditResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgSharedVariablesEditResponse():changetype<Ros2UrinterfacesMsgSharedVariablesEditResponse>(out.value);}
}
/** Complete original urinterfaces/msg/SpeedSliderFraction; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgSpeedSliderFraction {
static readonly maxWireBytes:i32=148;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=148;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar float64. */
data:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>148-n)return -90;n+=child;
if(n>148)return -90;
}
{
if(!isFinite(this.data))return -22;
if(8>148-n)return -90;n+=8;
if(n>148)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>reinterpret<u64>(this.data),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgSpeedSliderFraction{const value=new Ros2UrinterfacesMsgSpeedSliderFraction();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgSpeedSliderFractionCodec implements VendorCodec<Ros2UrinterfacesMsgSpeedSliderFraction>{
static readonly codecIdentity:string="UWVMROS1_55158782c2bc4d4741e8f2850efddf09d27666f9c3aeeded762d8d8e4198fccd";
static readonly maxWireBytes:i32=148;
encodeChecked(value:Ros2UrinterfacesMsgSpeedSliderFraction):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgSpeedSliderFraction):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgSpeedSliderFraction>{const out=new VendorDecoded<Ros2UrinterfacesMsgSpeedSliderFraction>();if(bytes.length>148){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgSpeedSliderFraction.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgSpeedSliderFraction{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgSpeedSliderFraction():changetype<Ros2UrinterfacesMsgSpeedSliderFraction>(out.value);}
}
/** Complete original urinterfaces/msg/TCPState; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgTCPState {
static readonly maxWireBytes:i32=244;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=244;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field pose, native scalar geometry_msgs/msg/Pose. */
pose:Ros2Geometry_msgsMsgPose=new Ros2Geometry_msgsMsgPose();
/** Exact native field speed, native scalar geometry_msgs/msg/Twist. */
speed:Ros2Geometry_msgsMsgTwist=new Ros2Geometry_msgsMsgTwist();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>244-n)return -90;n+=child;
if(n>244)return -90;
}
{
const child=this.pose.wireSize();if(child<0)return child;if(child>244-n)return -90;n+=child;
if(n>244)return -90;
}
{
const child=this.speed.wireSize();if(child<0)return child;if(child>244-n)return -90;n+=child;
if(n>244)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.pose.writeControl(w);
this.speed.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgTCPState{const value=new Ros2UrinterfacesMsgTCPState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPose.readControl(r);
value.pose=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwist.readControl(r);
value.speed=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgTCPStateCodec implements VendorCodec<Ros2UrinterfacesMsgTCPState>{
static readonly codecIdentity:string="UWVMROS1_49d8f678cc32669dd81528bb4c5d97f28019745159ef5e7267c8a076cd315b92";
static readonly maxWireBytes:i32=244;
encodeChecked(value:Ros2UrinterfacesMsgTCPState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgTCPState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgTCPState>{const out=new VendorDecoded<Ros2UrinterfacesMsgTCPState>();if(bytes.length>244){out.status=90;return out;}if(bytes.length<116){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgTCPState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgTCPState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgTCPState():changetype<Ros2UrinterfacesMsgTCPState>(out.value);}
}
/** Complete original urinterfaces/msg/ToolOutputMode; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgToolOutputMode {
static readonly TOOL_OUTPUT:u8=0;
static readonly TOOL_POWER:u8=1;
static readonly TOOL_SAFE:u8=2;
static readonly OUTPUT_MODE_DISABLED:u8=0;
static readonly OUTPUT_MODE_SINKING:u8=1;
static readonly OUTPUT_MODE_SOURCING:u8=2;
static readonly OUTPUT_MODE_PUSH_PULL:u8=3;
static readonly maxWireBytes:i32=143;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=143;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field output_mode, native scalar uint8. */
output_mode:u8=0;
/** Exact native field output_pin_mode, native scalar uint8. */
output_pin_mode:Array<u8>=new Array<u8>();
constructor(){
for(let i=0;i<2;++i)this.output_pin_mode.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>143-n)return -90;n+=child;
if(n>143)return -90;
}
{
if(1>143-n)return -90;n+=1;
if(n>143)return -90;
}
{
if(this.output_pin_mode.length!=2)return -22;
for(let i=0;i<this.output_pin_mode.length;++i){
if(1>143-n)return -90;n+=1;
if(n>143)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>this.output_mode,1);
for(let i=0;i<this.output_pin_mode.length;++i){
w.integer(<u64>this.output_pin_mode[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgToolOutputMode{const value=new Ros2UrinterfacesMsgToolOutputMode();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=<u8>r.integer(1);
value.output_mode=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.output_pin_mode=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.output_pin_mode.push(decoded);
}
}
return value;}
}
export class Ros2UrinterfacesMsgToolOutputModeCodec implements VendorCodec<Ros2UrinterfacesMsgToolOutputMode>{
static readonly codecIdentity:string="UWVMROS1_21df545459050dc86de1fb111c9a4b2c8bb50c3729137b8d5c5134dd33b84660";
static readonly maxWireBytes:i32=143;
encodeChecked(value:Ros2UrinterfacesMsgToolOutputMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgToolOutputMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgToolOutputMode>{const out=new VendorDecoded<Ros2UrinterfacesMsgToolOutputMode>();if(bytes.length>143){out.status=90;return out;}if(bytes.length<15){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgToolOutputMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgToolOutputMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgToolOutputMode():changetype<Ros2UrinterfacesMsgToolOutputMode>(out.value);}
}
/** Complete original urinterfaces/msg/VariableUpdate; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgVariableUpdate {
static readonly maxWireBytes:i32=1168;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1168;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar urinterfaces/msg/JsonStringType. */
data:Ros2UrinterfacesMsgJsonStringType=new Ros2UrinterfacesMsgJsonStringType();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>1168-n)return -90;n+=child;
if(n>1168)return -90;
}
{
const child=this.data.wireSize();if(child<0)return child;if(child>1168-n)return -90;n+=child;
if(n>1168)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.data.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgVariableUpdate{const value=new Ros2UrinterfacesMsgVariableUpdate();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2UrinterfacesMsgJsonStringType.readControl(r);
value.data=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgVariableUpdateCodec implements VendorCodec<Ros2UrinterfacesMsgVariableUpdate>{
static readonly codecIdentity:string="UWVMROS1_7028ea5c674be886f4fac1d2ac0452167489f2c23d7ea759cdbec90feccbdb20";
static readonly maxWireBytes:i32=1168;
encodeChecked(value:Ros2UrinterfacesMsgVariableUpdate):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgVariableUpdate):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgVariableUpdate>{const out=new VendorDecoded<Ros2UrinterfacesMsgVariableUpdate>();if(bytes.length>1168){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgVariableUpdate.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgVariableUpdate{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgVariableUpdate():changetype<Ros2UrinterfacesMsgVariableUpdate>(out.value);}
}
/** Complete original urinterfaces/msg/VoltageCurrentStamped; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgVoltageCurrentStamped {
static readonly maxWireBytes:i32=156;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=156;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field voltage, native scalar float64. */
voltage:f64=0;
/** Exact native field current, native scalar float64. */
current:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>156-n)return -90;n+=child;
if(n>156)return -90;
}
{
if(!isFinite(this.voltage))return -22;
if(8>156-n)return -90;n+=8;
if(n>156)return -90;
}
{
if(!isFinite(this.current))return -22;
if(8>156-n)return -90;n+=8;
if(n>156)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>reinterpret<u64>(this.voltage),8);
w.integer(<u64>reinterpret<u64>(this.current),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgVoltageCurrentStamped{const value=new Ros2UrinterfacesMsgVoltageCurrentStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.voltage=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.current=decoded;
}
return value;}
}
export class Ros2UrinterfacesMsgVoltageCurrentStampedCodec implements VendorCodec<Ros2UrinterfacesMsgVoltageCurrentStamped>{
static readonly codecIdentity:string="UWVMROS1_bcc3f6871a7bf0ff691d0c9c1b6596a633597de795b486c2b82ac922ede76805";
static readonly maxWireBytes:i32=156;
encodeChecked(value:Ros2UrinterfacesMsgVoltageCurrentStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgVoltageCurrentStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgVoltageCurrentStamped>{const out=new VendorDecoded<Ros2UrinterfacesMsgVoltageCurrentStamped>();if(bytes.length>156){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgVoltageCurrentStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgVoltageCurrentStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgVoltageCurrentStamped():changetype<Ros2UrinterfacesMsgVoltageCurrentStamped>(out.value);}
}
/** Complete original urinterfaces/msg/WorldModelNames; spellings are published in generation proof. */
export class Ros2UrinterfacesMsgWorldModelNames {
static readonly maxWireBytes:i32=16592;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16592;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field data, native scalar std_msgs/msg/String. */
data:Array<Ros2Std_msgsMsgString>=new Array<Ros2Std_msgsMsgString>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>16592-n)return -90;n+=child;
if(n>16592)return -90;
}
{
if(this.data.length>16)return -22;
if(4>16592-n)return -90;n+=4;
for(let i=0;i<this.data.length;++i){
const child=this.data[i].wireSize();if(child<0)return child;if(child>16592-n)return -90;n+=child;
if(n>16592)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.data.length);
for(let i=0;i<this.data.length;++i){
this.data[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesMsgWorldModelNames{const value=new Ros2UrinterfacesMsgWorldModelNames();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.data=new Array<Ros2Std_msgsMsgString>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Std_msgsMsgString.readControl(r);
value.data.push(decoded);
}
}
return value;}
}
export class Ros2UrinterfacesMsgWorldModelNamesCodec implements VendorCodec<Ros2UrinterfacesMsgWorldModelNames>{
static readonly codecIdentity:string="UWVMROS1_499f722fc97565bd0124888d7c20f4e9f8947893c49b1608dcf201fe3538d7a9";
static readonly maxWireBytes:i32=16592;
encodeChecked(value:Ros2UrinterfacesMsgWorldModelNames):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesMsgWorldModelNames):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesMsgWorldModelNames>{const out=new VendorDecoded<Ros2UrinterfacesMsgWorldModelNames>();if(bytes.length>16592){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesMsgWorldModelNames.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesMsgWorldModelNames{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesMsgWorldModelNames():changetype<Ros2UrinterfacesMsgWorldModelNames>(out.value);}
}
/** Complete original urinterfaces/srv/SetAnalogOutput_Request; spellings are published in generation proof. */
export class Ros2UrinterfacesSrvSetAnalogOutput_Request {
static readonly maxWireBytes:i32=100;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=100;
/** Exact native field data, native scalar urinterfaces/msg/Analog. */
data:Array<Ros2UrinterfacesMsgAnalog>=new Array<Ros2UrinterfacesMsgAnalog>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.data.length>16)return -22;
if(4>100-n)return -90;n+=4;
for(let i=0;i<this.data.length;++i){
const child=this.data[i].wireSize();if(child<0)return child;if(child>100-n)return -90;n+=child;
if(n>100)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.data.length);
for(let i=0;i<this.data.length;++i){
this.data[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesSrvSetAnalogOutput_Request{const value=new Ros2UrinterfacesSrvSetAnalogOutput_Request();
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.data=new Array<Ros2UrinterfacesMsgAnalog>();for(let i:u32=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgAnalog.readControl(r);
value.data.push(decoded);
}
}
return value;}
}
export class Ros2UrinterfacesSrvSetAnalogOutput_RequestCodec implements VendorCodec<Ros2UrinterfacesSrvSetAnalogOutput_Request>{
static readonly codecIdentity:string="UWVMROS1_8be07a94dd334a03871180a9c73bab11377664e60500ac68d312398d51acd32d";
static readonly maxWireBytes:i32=100;
encodeChecked(value:Ros2UrinterfacesSrvSetAnalogOutput_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesSrvSetAnalogOutput_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesSrvSetAnalogOutput_Request>{const out=new VendorDecoded<Ros2UrinterfacesSrvSetAnalogOutput_Request>();if(bytes.length>100){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesSrvSetAnalogOutput_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesSrvSetAnalogOutput_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesSrvSetAnalogOutput_Request():changetype<Ros2UrinterfacesSrvSetAnalogOutput_Request>(out.value);}
}
/** Complete original urinterfaces/srv/SetAnalogOutput_Response; spellings are published in generation proof. */
export class Ros2UrinterfacesSrvSetAnalogOutput_Response {
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
static readControl(r:Reader):Ros2UrinterfacesSrvSetAnalogOutput_Response{const value=new Ros2UrinterfacesSrvSetAnalogOutput_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2UrinterfacesSrvSetAnalogOutput_ResponseCodec implements VendorCodec<Ros2UrinterfacesSrvSetAnalogOutput_Response>{
static readonly codecIdentity:string="UWVMROS1_14016138286129e4f155ac2f82f5573e8a4af7d7c5ede55ce57f3ad76cb82e4e";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2UrinterfacesSrvSetAnalogOutput_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesSrvSetAnalogOutput_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesSrvSetAnalogOutput_Response>{const out=new VendorDecoded<Ros2UrinterfacesSrvSetAnalogOutput_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesSrvSetAnalogOutput_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesSrvSetAnalogOutput_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesSrvSetAnalogOutput_Response():changetype<Ros2UrinterfacesSrvSetAnalogOutput_Response>(out.value);}
}
/** Complete original urinterfaces/srv/SetDigitalOutput_Request; spellings are published in generation proof. */
export class Ros2UrinterfacesSrvSetDigitalOutput_Request {
static readonly maxWireBytes:i32=36;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=36;
/** Exact native field data, native scalar urinterfaces/msg/Digital. */
data:Array<Ros2UrinterfacesMsgDigital>=new Array<Ros2UrinterfacesMsgDigital>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.data.length>16)return -22;
if(4>36-n)return -90;n+=4;
for(let i=0;i<this.data.length;++i){
const child=this.data[i].wireSize();if(child<0)return child;if(child>36-n)return -90;n+=child;
if(n>36)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.data.length);
for(let i=0;i<this.data.length;++i){
this.data[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesSrvSetDigitalOutput_Request{const value=new Ros2UrinterfacesSrvSetDigitalOutput_Request();
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.data=new Array<Ros2UrinterfacesMsgDigital>();for(let i:u32=0;i<count;++i){
const decoded=Ros2UrinterfacesMsgDigital.readControl(r);
value.data.push(decoded);
}
}
return value;}
}
export class Ros2UrinterfacesSrvSetDigitalOutput_RequestCodec implements VendorCodec<Ros2UrinterfacesSrvSetDigitalOutput_Request>{
static readonly codecIdentity:string="UWVMROS1_ec6c587810af7f3d04b10058e69c8095775e8b6feede5fc0f60ca1dfda0b5e47";
static readonly maxWireBytes:i32=36;
encodeChecked(value:Ros2UrinterfacesSrvSetDigitalOutput_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesSrvSetDigitalOutput_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesSrvSetDigitalOutput_Request>{const out=new VendorDecoded<Ros2UrinterfacesSrvSetDigitalOutput_Request>();if(bytes.length>36){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesSrvSetDigitalOutput_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesSrvSetDigitalOutput_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesSrvSetDigitalOutput_Request():changetype<Ros2UrinterfacesSrvSetDigitalOutput_Request>(out.value);}
}
/** Complete original urinterfaces/srv/SetDigitalOutput_Response; spellings are published in generation proof. */
export class Ros2UrinterfacesSrvSetDigitalOutput_Response {
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
static readControl(r:Reader):Ros2UrinterfacesSrvSetDigitalOutput_Response{const value=new Ros2UrinterfacesSrvSetDigitalOutput_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2UrinterfacesSrvSetDigitalOutput_ResponseCodec implements VendorCodec<Ros2UrinterfacesSrvSetDigitalOutput_Response>{
static readonly codecIdentity:string="UWVMROS1_0c5f0b8a7f1d14c6223838c3a28cb2236848b81ecfda29d2b704557579e9162d";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2UrinterfacesSrvSetDigitalOutput_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesSrvSetDigitalOutput_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesSrvSetDigitalOutput_Response>{const out=new VendorDecoded<Ros2UrinterfacesSrvSetDigitalOutput_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesSrvSetDigitalOutput_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesSrvSetDigitalOutput_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesSrvSetDigitalOutput_Response():changetype<Ros2UrinterfacesSrvSetDigitalOutput_Response>(out.value);}
}
/** Complete original urinterfaces/srv/SetSpeedFraction_Request; spellings are published in generation proof. */
export class Ros2UrinterfacesSrvSetSpeedFraction_Request {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field value, native scalar float32. */
value:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.value))return -22;
if(4>4-n)return -90;n+=4;
if(n>4)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.value),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesSrvSetSpeedFraction_Request{const value=new Ros2UrinterfacesSrvSetSpeedFraction_Request();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
return value;}
}
export class Ros2UrinterfacesSrvSetSpeedFraction_RequestCodec implements VendorCodec<Ros2UrinterfacesSrvSetSpeedFraction_Request>{
static readonly codecIdentity:string="UWVMROS1_21fca7e952e32f24c905094c09776968c8cd80b7712a03de7c2a693ea79a7d63";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2UrinterfacesSrvSetSpeedFraction_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesSrvSetSpeedFraction_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesSrvSetSpeedFraction_Request>{const out=new VendorDecoded<Ros2UrinterfacesSrvSetSpeedFraction_Request>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesSrvSetSpeedFraction_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesSrvSetSpeedFraction_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesSrvSetSpeedFraction_Request():changetype<Ros2UrinterfacesSrvSetSpeedFraction_Request>(out.value);}
}
/** Complete original urinterfaces/srv/SetSpeedFraction_Response; spellings are published in generation proof. */
export class Ros2UrinterfacesSrvSetSpeedFraction_Response {
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
static readControl(r:Reader):Ros2UrinterfacesSrvSetSpeedFraction_Response{const value=new Ros2UrinterfacesSrvSetSpeedFraction_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2UrinterfacesSrvSetSpeedFraction_ResponseCodec implements VendorCodec<Ros2UrinterfacesSrvSetSpeedFraction_Response>{
static readonly codecIdentity:string="UWVMROS1_386b067db70c17a88297d005650f6a05c0fb72eb5ee142a25c3e51c70b80c6e6";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2UrinterfacesSrvSetSpeedFraction_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesSrvSetSpeedFraction_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesSrvSetSpeedFraction_Response>{const out=new VendorDecoded<Ros2UrinterfacesSrvSetSpeedFraction_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesSrvSetSpeedFraction_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesSrvSetSpeedFraction_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesSrvSetSpeedFraction_Response():changetype<Ros2UrinterfacesSrvSetSpeedFraction_Response>(out.value);}
}
/** Complete original urinterfaces/srv/SharedVariablesEditor_Request; spellings are published in generation proof. */
export class Ros2UrinterfacesSrvSharedVariablesEditor_Request {
static readonly maxWireBytes:i32=1028;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1028;
/** Exact native field request, native scalar urinterfaces/msg/JsonStringType. */
request:Ros2UrinterfacesMsgJsonStringType=new Ros2UrinterfacesMsgJsonStringType();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.request.wireSize();if(child<0)return child;if(child>1028-n)return -90;n+=child;
if(n>1028)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.request.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesSrvSharedVariablesEditor_Request{const value=new Ros2UrinterfacesSrvSharedVariablesEditor_Request();
{
const decoded=Ros2UrinterfacesMsgJsonStringType.readControl(r);
value.request=decoded;
}
return value;}
}
export class Ros2UrinterfacesSrvSharedVariablesEditor_RequestCodec implements VendorCodec<Ros2UrinterfacesSrvSharedVariablesEditor_Request>{
static readonly codecIdentity:string="UWVMROS1_c32d00237370b2eeb596c76bd9cda1ccfcab7618c71dec77d273f712462f5fa3";
static readonly maxWireBytes:i32=1028;
encodeChecked(value:Ros2UrinterfacesSrvSharedVariablesEditor_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesSrvSharedVariablesEditor_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesSrvSharedVariablesEditor_Request>{const out=new VendorDecoded<Ros2UrinterfacesSrvSharedVariablesEditor_Request>();if(bytes.length>1028){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesSrvSharedVariablesEditor_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesSrvSharedVariablesEditor_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesSrvSharedVariablesEditor_Request():changetype<Ros2UrinterfacesSrvSharedVariablesEditor_Request>(out.value);}
}
/** Complete original urinterfaces/srv/SharedVariablesEditor_Response; spellings are published in generation proof. */
export class Ros2UrinterfacesSrvSharedVariablesEditor_Response {
static readonly maxWireBytes:i32=1029;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1029;
/** Exact native field response, native scalar urinterfaces/msg/SharedVariablesEditResponse. */
response:Ros2UrinterfacesMsgSharedVariablesEditResponse=new Ros2UrinterfacesMsgSharedVariablesEditResponse();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.response.wireSize();if(child<0)return child;if(child>1029-n)return -90;n+=child;
if(n>1029)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.response.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesSrvSharedVariablesEditor_Response{const value=new Ros2UrinterfacesSrvSharedVariablesEditor_Response();
{
const decoded=Ros2UrinterfacesMsgSharedVariablesEditResponse.readControl(r);
value.response=decoded;
}
return value;}
}
export class Ros2UrinterfacesSrvSharedVariablesEditor_ResponseCodec implements VendorCodec<Ros2UrinterfacesSrvSharedVariablesEditor_Response>{
static readonly codecIdentity:string="UWVMROS1_4134f7ed9715b3f6e3fd7ecca44fcae67cdd5dc2fa432b07ab75485901d1d5ea";
static readonly maxWireBytes:i32=1029;
encodeChecked(value:Ros2UrinterfacesSrvSharedVariablesEditor_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesSrvSharedVariablesEditor_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesSrvSharedVariablesEditor_Response>{const out=new VendorDecoded<Ros2UrinterfacesSrvSharedVariablesEditor_Response>();if(bytes.length>1029){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesSrvSharedVariablesEditor_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesSrvSharedVariablesEditor_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesSrvSharedVariablesEditor_Response():changetype<Ros2UrinterfacesSrvSharedVariablesEditor_Response>(out.value);}
}
/** Complete original urinterfaces/srv/VariablesViewer_Request; spellings are published in generation proof. */
export class Ros2UrinterfacesSrvVariablesViewer_Request {
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
static readControl(r:Reader):Ros2UrinterfacesSrvVariablesViewer_Request{const value=new Ros2UrinterfacesSrvVariablesViewer_Request();
return value;}
}
export class Ros2UrinterfacesSrvVariablesViewer_RequestCodec implements VendorCodec<Ros2UrinterfacesSrvVariablesViewer_Request>{
static readonly codecIdentity:string="UWVMROS1_6ecb0db6328d813bfb987ae987d3b7f2c6913dba2cf2a62f644a8eb2f760152a";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2UrinterfacesSrvVariablesViewer_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesSrvVariablesViewer_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesSrvVariablesViewer_Request>{const out=new VendorDecoded<Ros2UrinterfacesSrvVariablesViewer_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesSrvVariablesViewer_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesSrvVariablesViewer_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesSrvVariablesViewer_Request():changetype<Ros2UrinterfacesSrvVariablesViewer_Request>(out.value);}
}
/** Complete original urinterfaces/srv/VariablesViewer_Response; spellings are published in generation proof. */
export class Ros2UrinterfacesSrvVariablesViewer_Response {
static readonly maxWireBytes:i32=1168;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1168;
/** Exact native field response, native scalar urinterfaces/msg/VariableUpdate. */
response:Ros2UrinterfacesMsgVariableUpdate=new Ros2UrinterfacesMsgVariableUpdate();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.response.wireSize();if(child<0)return child;if(child>1168-n)return -90;n+=child;
if(n>1168)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.response.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2UrinterfacesSrvVariablesViewer_Response{const value=new Ros2UrinterfacesSrvVariablesViewer_Response();
{
const decoded=Ros2UrinterfacesMsgVariableUpdate.readControl(r);
value.response=decoded;
}
return value;}
}
export class Ros2UrinterfacesSrvVariablesViewer_ResponseCodec implements VendorCodec<Ros2UrinterfacesSrvVariablesViewer_Response>{
static readonly codecIdentity:string="UWVMROS1_e8cb16ca847189ac38c0d972afbdd2208d8641c29a0ee0cce8284e9e0dd249a2";
static readonly maxWireBytes:i32=1168;
encodeChecked(value:Ros2UrinterfacesSrvVariablesViewer_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2UrinterfacesSrvVariablesViewer_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2UrinterfacesSrvVariablesViewer_Response>{const out=new VendorDecoded<Ros2UrinterfacesSrvVariablesViewer_Response>();if(bytes.length>1168){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2UrinterfacesSrvVariablesViewer_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2UrinterfacesSrvVariablesViewer_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2UrinterfacesSrvVariablesViewer_Response():changetype<Ros2UrinterfacesSrvVariablesViewer_Response>(out.value);}
}
