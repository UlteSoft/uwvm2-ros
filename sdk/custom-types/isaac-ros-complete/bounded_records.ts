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
/** Complete original std_msgs/msg/Header; spellings are published in generation proof. */
export class Ros2Std_msgsMsgHeader {
static readonly maxWireBytes:i32=76;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=76;
/** Exact native field stamp, native scalar builtin_interfaces/msg/Time. */
stamp:Ros2Builtin_interfacesMsgTime=new Ros2Builtin_interfacesMsgTime();
/** Exact native field frame_id, native scalar string. */
frame_id:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.stamp.wireSize();if(child<0)return child;if(child>76-n)return -90;n+=child;
if(n>76)return -90;
}
{
if(!validUnicode(this.frame_id)||this.frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.frame_id);if(size>64)return -22;if(4+size>76-n)return -90;n+=4+size;
if(n>76)return -90;
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
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.frame_id=decoded;
}
return value;}
}
export class Ros2Std_msgsMsgHeaderCodec implements VendorCodec<Ros2Std_msgsMsgHeader>{
static readonly codecIdentity:string="UWVMROS1_afba9e74be3c5e3321750c1c4c30ba2925269f75b9da43a6d5c1e93d293d7396";
static readonly maxWireBytes:i32=76;
encodeChecked(value:Ros2Std_msgsMsgHeader):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Std_msgsMsgHeader):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Std_msgsMsgHeader>{const out=new VendorDecoded<Ros2Std_msgsMsgHeader>();if(bytes.length>76){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Std_msgsMsgHeader.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Std_msgsMsgHeader{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Std_msgsMsgHeader():changetype<Ros2Std_msgsMsgHeader>(out.value);}
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
/** Complete original geometry_msgs/msg/PoseWithCovariance; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgPoseWithCovariance {
static readonly maxWireBytes:i32=344;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=344;
/** Exact native field pose, native scalar geometry_msgs/msg/Pose. */
pose:Ros2Geometry_msgsMsgPose=new Ros2Geometry_msgsMsgPose();
/** Exact native field covariance, native scalar float64. */
covariance:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<36;++i)this.covariance.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.pose.wireSize();if(child<0)return child;if(child>344-n)return -90;n+=child;
if(n>344)return -90;
}
{
if(this.covariance.length!=36)return -22;
for(let i=0;i<this.covariance.length;++i){
if(!isFinite(this.covariance[i]))return -22;
if(8>344-n)return -90;n+=8;
if(n>344)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.pose.writeControl(w);
for(let i=0;i<this.covariance.length;++i){
w.integer(<u64>reinterpret<u64>(this.covariance[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgPoseWithCovariance{const value=new Ros2Geometry_msgsMsgPoseWithCovariance();
{
const decoded=Ros2Geometry_msgsMsgPose.readControl(r);
value.pose=decoded;
}
{
const count:u32=36;if(!r.valid||count>36){r.valid=false;return value;}
value.covariance=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.covariance.push(decoded);
}
}
return value;}
}
export class Ros2Geometry_msgsMsgPoseWithCovarianceCodec implements VendorCodec<Ros2Geometry_msgsMsgPoseWithCovariance>{
static readonly codecIdentity:string="UWVMROS1_8eb3e25c45769eb29691011c05315ed50f2b97eff488f9e8ee2dc70bddab4dc2";
static readonly maxWireBytes:i32=344;
encodeChecked(value:Ros2Geometry_msgsMsgPoseWithCovariance):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgPoseWithCovariance):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgPoseWithCovariance>{const out=new VendorDecoded<Ros2Geometry_msgsMsgPoseWithCovariance>();if(bytes.length>344){out.status=90;return out;}if(bytes.length<344){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgPoseWithCovariance.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgPoseWithCovariance{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgPoseWithCovariance():changetype<Ros2Geometry_msgsMsgPoseWithCovariance>(out.value);}
}
/** Complete original geometry_msgs/msg/PoseWithCovarianceStamped; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgPoseWithCovarianceStamped {
static readonly maxWireBytes:i32=420;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=420;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field pose, native scalar geometry_msgs/msg/PoseWithCovariance. */
pose:Ros2Geometry_msgsMsgPoseWithCovariance=new Ros2Geometry_msgsMsgPoseWithCovariance();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>420-n)return -90;n+=child;
if(n>420)return -90;
}
{
const child=this.pose.wireSize();if(child<0)return child;if(child>420-n)return -90;n+=child;
if(n>420)return -90;
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
static readControl(r:Reader):Ros2Geometry_msgsMsgPoseWithCovarianceStamped{const value=new Ros2Geometry_msgsMsgPoseWithCovarianceStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseWithCovariance.readControl(r);
value.pose=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgPoseWithCovarianceStampedCodec implements VendorCodec<Ros2Geometry_msgsMsgPoseWithCovarianceStamped>{
static readonly codecIdentity:string="UWVMROS1_ea380172cde0ee5fef319aff50d92a80c314510e3d12efec45e9ae1f74974af3";
static readonly maxWireBytes:i32=420;
encodeChecked(value:Ros2Geometry_msgsMsgPoseWithCovarianceStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgPoseWithCovarianceStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgPoseWithCovarianceStamped>{const out=new VendorDecoded<Ros2Geometry_msgsMsgPoseWithCovarianceStamped>();if(bytes.length>420){out.status=90;return out;}if(bytes.length<356){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgPoseWithCovarianceStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgPoseWithCovarianceStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgPoseWithCovarianceStamped():changetype<Ros2Geometry_msgsMsgPoseWithCovarianceStamped>(out.value);}
}
/** Complete original isaac_ros_apriltag_interfaces/msg/AprilTagDetection; spellings are published in generation proof. */
export class Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection {
static readonly maxWireBytes:i32=612;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=612;
/** Exact native field family, native scalar string. */
family:string="";
/** Exact native field id, native scalar int32. */
id:i32=0;
/** Exact native field center, native scalar geometry_msgs/msg/Point. */
center:Ros2Geometry_msgsMsgPoint=new Ros2Geometry_msgsMsgPoint();
/** Exact native field corners, native scalar geometry_msgs/msg/Point. */
corners:Array<Ros2Geometry_msgsMsgPoint>=new Array<Ros2Geometry_msgsMsgPoint>();
/** Exact native field pose, native scalar geometry_msgs/msg/PoseWithCovarianceStamped. */
pose:Ros2Geometry_msgsMsgPoseWithCovarianceStamped=new Ros2Geometry_msgsMsgPoseWithCovarianceStamped();
constructor(){
for(let i=0;i<4;++i)this.corners.push(new Ros2Geometry_msgsMsgPoint());
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.family)||this.family.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.family);if(size>64)return -22;if(4+size>612-n)return -90;n+=4+size;
if(n>612)return -90;
}
{
if(4>612-n)return -90;n+=4;
if(n>612)return -90;
}
{
const child=this.center.wireSize();if(child<0)return child;if(child>612-n)return -90;n+=child;
if(n>612)return -90;
}
{
if(this.corners.length!=4)return -22;
for(let i=0;i<this.corners.length;++i){
const child=this.corners[i].wireSize();if(child<0)return child;if(child>612-n)return -90;n+=child;
if(n>612)return -90;
}
}
{
const child=this.pose.wireSize();if(child<0)return child;if(child>612-n)return -90;n+=child;
if(n>612)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.family);
w.integer(<u64>this.id,4);
this.center.writeControl(w);
for(let i=0;i<this.corners.length;++i){
this.corners[i].writeControl(w);
}
this.pose.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection{const value=new Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.family=decoded;
}
{
const decoded=<i32>r.integer(4);
value.id=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoint.readControl(r);
value.center=decoded;
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.corners=new Array<Ros2Geometry_msgsMsgPoint>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgPoint.readControl(r);
value.corners.push(decoded);
}
}
{
const decoded=Ros2Geometry_msgsMsgPoseWithCovarianceStamped.readControl(r);
value.pose=decoded;
}
return value;}
}
export class Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionCodec implements VendorCodec<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection>{
static readonly codecIdentity:string="UWVMROS1_5d840f0d9239a948ce5d17bbee6e248db8151ff31070667e506e9c1647ac6fe7";
static readonly maxWireBytes:i32=612;
encodeChecked(value:Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection>{const out=new VendorDecoded<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection>();if(bytes.length>612){out.status=90;return out;}if(bytes.length<484){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection():changetype<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection>(out.value);}
}
/** Complete original isaac_ros_apriltag_interfaces/msg/AprilTagDetectionArray; spellings are published in generation proof. */
export class Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray {
static readonly maxWireBytes:i32=9872;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=9872;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field detections, native scalar isaac_ros_apriltag_interfaces/msg/AprilTagDetection. */
detections:Array<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection>=new Array<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>9872-n)return -90;n+=child;
if(n>9872)return -90;
}
{
if(this.detections.length>16)return -22;
if(4>9872-n)return -90;n+=4;
for(let i=0;i<this.detections.length;++i){
const child=this.detections[i].wireSize();if(child<0)return child;if(child>9872-n)return -90;n+=child;
if(n>9872)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.detections.length);
for(let i=0;i<this.detections.length;++i){
this.detections[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray{const value=new Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.detections=new Array<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetection.readControl(r);
value.detections.push(decoded);
}
}
return value;}
}
export class Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArrayCodec implements VendorCodec<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray>{
static readonly codecIdentity:string="UWVMROS1_60359e405aedd2a627e60bc35e849989fe5ed1d426afb2a884036aef93bfe0a5";
static readonly maxWireBytes:i32=9872;
encodeChecked(value:Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray>{const out=new VendorDecoded<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray>();if(bytes.length>9872){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray():changetype<Ros2Isaac_ros_apriltag_interfacesMsgAprilTagDetectionArray>(out.value);}
}
