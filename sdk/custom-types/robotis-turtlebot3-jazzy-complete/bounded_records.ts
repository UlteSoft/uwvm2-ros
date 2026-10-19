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
/** Complete original geometry_msgs/msg/Transform; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgTransform {
static readonly maxWireBytes:i32=56;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=56;
/** Exact native field translation, native scalar geometry_msgs/msg/Vector3. */
translation:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field rotation, native scalar geometry_msgs/msg/Quaternion. */
rotation:Ros2Geometry_msgsMsgQuaternion=new Ros2Geometry_msgsMsgQuaternion();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.translation.wireSize();if(child<0)return child;if(child>56-n)return -90;n+=child;
if(n>56)return -90;
}
{
const child=this.rotation.wireSize();if(child<0)return child;if(child>56-n)return -90;n+=child;
if(n>56)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.translation.writeControl(w);
this.rotation.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgTransform{const value=new Ros2Geometry_msgsMsgTransform();
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.translation=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgQuaternion.readControl(r);
value.rotation=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgTransformCodec implements VendorCodec<Ros2Geometry_msgsMsgTransform>{
static readonly codecIdentity:string="UWVMROS1_7ddcb1c3fca43f4a0299d43fb6fac1b77a8f26001af809dc13307f4f9c61d65b";
static readonly maxWireBytes:i32=56;
encodeChecked(value:Ros2Geometry_msgsMsgTransform):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgTransform):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgTransform>{const out=new VendorDecoded<Ros2Geometry_msgsMsgTransform>();if(bytes.length>56){out.status=90;return out;}if(bytes.length<56){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgTransform.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgTransform{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgTransform():changetype<Ros2Geometry_msgsMsgTransform>(out.value);}
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
/** Complete original geometry_msgs/msg/TransformStamped; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgTransformStamped {
static readonly maxWireBytes:i32=456;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=456;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field child_frame_id, native scalar string. */
child_frame_id:string="";
/** Exact native field transform, native scalar geometry_msgs/msg/Transform. */
transform:Ros2Geometry_msgsMsgTransform=new Ros2Geometry_msgsMsgTransform();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>456-n)return -90;n+=child;
if(n>456)return -90;
}
{
if(!validUnicode(this.child_frame_id)||this.child_frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.child_frame_id);if(size>256)return -22;if(4+size>456-n)return -90;n+=4+size;
if(n>456)return -90;
}
{
const child=this.transform.wireSize();if(child<0)return child;if(child>456-n)return -90;n+=child;
if(n>456)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.text(this.child_frame_id);
this.transform.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgTransformStamped{const value=new Ros2Geometry_msgsMsgTransformStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.child_frame_id=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTransform.readControl(r);
value.transform=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgTransformStampedCodec implements VendorCodec<Ros2Geometry_msgsMsgTransformStamped>{
static readonly codecIdentity:string="UWVMROS1_1cb6033612e0632fe87e6cf44bf5a899220370c372ba25b989a066171925ee7a";
static readonly maxWireBytes:i32=456;
encodeChecked(value:Ros2Geometry_msgsMsgTransformStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgTransformStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgTransformStamped>{const out=new VendorDecoded<Ros2Geometry_msgsMsgTransformStamped>();if(bytes.length>456){out.status=90;return out;}if(bytes.length<72){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgTransformStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgTransformStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgTransformStamped():changetype<Ros2Geometry_msgsMsgTransformStamped>(out.value);}
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
/** Complete original geometry_msgs/msg/TwistStamped; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgTwistStamped {
static readonly maxWireBytes:i32=188;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=188;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field twist, native scalar geometry_msgs/msg/Twist. */
twist:Ros2Geometry_msgsMsgTwist=new Ros2Geometry_msgsMsgTwist();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>188-n)return -90;n+=child;
if(n>188)return -90;
}
{
const child=this.twist.wireSize();if(child<0)return child;if(child>188-n)return -90;n+=child;
if(n>188)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.twist.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgTwistStamped{const value=new Ros2Geometry_msgsMsgTwistStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwist.readControl(r);
value.twist=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgTwistStampedCodec implements VendorCodec<Ros2Geometry_msgsMsgTwistStamped>{
static readonly codecIdentity:string="UWVMROS1_7448b885388ff3454de1a35b8c5c895ee58b791267b435df1e4cf7c1eee0f573";
static readonly maxWireBytes:i32=188;
encodeChecked(value:Ros2Geometry_msgsMsgTwistStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgTwistStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgTwistStamped>{const out=new VendorDecoded<Ros2Geometry_msgsMsgTwistStamped>();if(bytes.length>188){out.status=90;return out;}if(bytes.length<60){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgTwistStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgTwistStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgTwistStamped():changetype<Ros2Geometry_msgsMsgTwistStamped>(out.value);}
}
/** Complete original geometry_msgs/msg/TwistWithCovariance; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgTwistWithCovariance {
static readonly maxWireBytes:i32=336;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=336;
/** Exact native field twist, native scalar geometry_msgs/msg/Twist. */
twist:Ros2Geometry_msgsMsgTwist=new Ros2Geometry_msgsMsgTwist();
/** Exact native field covariance, native scalar float64. */
covariance:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<36;++i)this.covariance.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.twist.wireSize();if(child<0)return child;if(child>336-n)return -90;n+=child;
if(n>336)return -90;
}
{
if(this.covariance.length!=36)return -22;
for(let i=0;i<this.covariance.length;++i){
if(!isFinite(this.covariance[i]))return -22;
if(8>336-n)return -90;n+=8;
if(n>336)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.twist.writeControl(w);
for(let i=0;i<this.covariance.length;++i){
w.integer(<u64>reinterpret<u64>(this.covariance[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgTwistWithCovariance{const value=new Ros2Geometry_msgsMsgTwistWithCovariance();
{
const decoded=Ros2Geometry_msgsMsgTwist.readControl(r);
value.twist=decoded;
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
export class Ros2Geometry_msgsMsgTwistWithCovarianceCodec implements VendorCodec<Ros2Geometry_msgsMsgTwistWithCovariance>{
static readonly codecIdentity:string="UWVMROS1_96c9fddad7b7c240e850d8c27ddb5f8343764cac791ef9e210196f7941aaf404";
static readonly maxWireBytes:i32=336;
encodeChecked(value:Ros2Geometry_msgsMsgTwistWithCovariance):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgTwistWithCovariance):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgTwistWithCovariance>{const out=new VendorDecoded<Ros2Geometry_msgsMsgTwistWithCovariance>();if(bytes.length>336){out.status=90;return out;}if(bytes.length<336){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgTwistWithCovariance.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgTwistWithCovariance{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgTwistWithCovariance():changetype<Ros2Geometry_msgsMsgTwistWithCovariance>(out.value);}
}
/** Complete original nav_msgs/msg/Odometry; spellings are published in generation proof. */
export class Ros2Nav_msgsMsgOdometry {
static readonly maxWireBytes:i32=1080;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1080;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field child_frame_id, native scalar string. */
child_frame_id:string="";
/** Exact native field pose, native scalar geometry_msgs/msg/PoseWithCovariance. */
pose:Ros2Geometry_msgsMsgPoseWithCovariance=new Ros2Geometry_msgsMsgPoseWithCovariance();
/** Exact native field twist, native scalar geometry_msgs/msg/TwistWithCovariance. */
twist:Ros2Geometry_msgsMsgTwistWithCovariance=new Ros2Geometry_msgsMsgTwistWithCovariance();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>1080-n)return -90;n+=child;
if(n>1080)return -90;
}
{
if(!validUnicode(this.child_frame_id)||this.child_frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.child_frame_id);if(size>256)return -22;if(4+size>1080-n)return -90;n+=4+size;
if(n>1080)return -90;
}
{
const child=this.pose.wireSize();if(child<0)return child;if(child>1080-n)return -90;n+=child;
if(n>1080)return -90;
}
{
const child=this.twist.wireSize();if(child<0)return child;if(child>1080-n)return -90;n+=child;
if(n>1080)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.text(this.child_frame_id);
this.pose.writeControl(w);
this.twist.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Nav_msgsMsgOdometry{const value=new Ros2Nav_msgsMsgOdometry();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.child_frame_id=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseWithCovariance.readControl(r);
value.pose=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwistWithCovariance.readControl(r);
value.twist=decoded;
}
return value;}
}
export class Ros2Nav_msgsMsgOdometryCodec implements VendorCodec<Ros2Nav_msgsMsgOdometry>{
static readonly codecIdentity:string="UWVMROS1_4eb9aa4edb3d288080deaae0326c26a75923e69d40c9b10219baf6d441ea8111";
static readonly maxWireBytes:i32=1080;
encodeChecked(value:Ros2Nav_msgsMsgOdometry):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Nav_msgsMsgOdometry):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Nav_msgsMsgOdometry>{const out=new VendorDecoded<Ros2Nav_msgsMsgOdometry>();if(bytes.length>1080){out.status=90;return out;}if(bytes.length<696){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Nav_msgsMsgOdometry.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Nav_msgsMsgOdometry{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Nav_msgsMsgOdometry():changetype<Ros2Nav_msgsMsgOdometry>(out.value);}
}
/** Complete original sensor_msgs/msg/BatteryState; spellings are published in generation proof. */
export class Ros2Sensor_msgsMsgBatteryState {
static readonly POWER_SUPPLY_STATUS_UNKNOWN:u8=0;
static readonly POWER_SUPPLY_STATUS_CHARGING:u8=1;
static readonly POWER_SUPPLY_STATUS_DISCHARGING:u8=2;
static readonly POWER_SUPPLY_STATUS_NOT_CHARGING:u8=3;
static readonly POWER_SUPPLY_STATUS_FULL:u8=4;
static readonly POWER_SUPPLY_HEALTH_UNKNOWN:u8=0;
static readonly POWER_SUPPLY_HEALTH_GOOD:u8=1;
static readonly POWER_SUPPLY_HEALTH_OVERHEAT:u8=2;
static readonly POWER_SUPPLY_HEALTH_DEAD:u8=3;
static readonly POWER_SUPPLY_HEALTH_OVERVOLTAGE:u8=4;
static readonly POWER_SUPPLY_HEALTH_UNSPEC_FAILURE:u8=5;
static readonly POWER_SUPPLY_HEALTH_COLD:u8=6;
static readonly POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE:u8=7;
static readonly POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE:u8=8;
static readonly POWER_SUPPLY_TECHNOLOGY_UNKNOWN:u8=0;
static readonly POWER_SUPPLY_TECHNOLOGY_NIMH:u8=1;
static readonly POWER_SUPPLY_TECHNOLOGY_LION:u8=2;
static readonly POWER_SUPPLY_TECHNOLOGY_LIPO:u8=3;
static readonly POWER_SUPPLY_TECHNOLOGY_LIFE:u8=4;
static readonly POWER_SUPPLY_TECHNOLOGY_NICD:u8=5;
static readonly POWER_SUPPLY_TECHNOLOGY_LIMN:u8=6;
static readonly POWER_SUPPLY_TECHNOLOGY_TERNARY:u8=7;
static readonly POWER_SUPPLY_TECHNOLOGY_VRLA:u8=8;
static readonly maxWireBytes:i32=828;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=828;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field voltage, native scalar float32. */
voltage:f32=0;
/** Exact native field temperature, native scalar float32. */
temperature:f32=0;
/** Exact native field current, native scalar float32. */
current:f32=0;
/** Exact native field charge, native scalar float32. */
charge:f32=0;
/** Exact native field capacity, native scalar float32. */
capacity:f32=0;
/** Exact native field design_capacity, native scalar float32. */
design_capacity:f32=0;
/** Exact native field percentage, native scalar float32. */
percentage:f32=0;
/** Exact native field power_supply_status, native scalar uint8. */
power_supply_status:u8=0;
/** Exact native field power_supply_health, native scalar uint8. */
power_supply_health:u8=0;
/** Exact native field power_supply_technology, native scalar uint8. */
power_supply_technology:u8=0;
/** Exact native field present, native scalar bool. */
present:bool=false;
/** Exact native field cell_voltage, native scalar float32. */
cell_voltage:Array<f32>=new Array<f32>();
/** Exact native field cell_temperature, native scalar float32. */
cell_temperature:Array<f32>=new Array<f32>();
/** Exact native field location, native scalar string. */
location:string="";
/** Exact native field serial_number, native scalar string. */
serial_number:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>828-n)return -90;n+=child;
if(n>828)return -90;
}
{
if(!isFinite(this.voltage))return -22;
if(4>828-n)return -90;n+=4;
if(n>828)return -90;
}
{
if(!isFinite(this.temperature))return -22;
if(4>828-n)return -90;n+=4;
if(n>828)return -90;
}
{
if(!isFinite(this.current))return -22;
if(4>828-n)return -90;n+=4;
if(n>828)return -90;
}
{
if(!isFinite(this.charge))return -22;
if(4>828-n)return -90;n+=4;
if(n>828)return -90;
}
{
if(!isFinite(this.capacity))return -22;
if(4>828-n)return -90;n+=4;
if(n>828)return -90;
}
{
if(!isFinite(this.design_capacity))return -22;
if(4>828-n)return -90;n+=4;
if(n>828)return -90;
}
{
if(!isFinite(this.percentage))return -22;
if(4>828-n)return -90;n+=4;
if(n>828)return -90;
}
{
if(1>828-n)return -90;n+=1;
if(n>828)return -90;
}
{
if(1>828-n)return -90;n+=1;
if(n>828)return -90;
}
{
if(1>828-n)return -90;n+=1;
if(n>828)return -90;
}
{
if(1>828-n)return -90;n+=1;
if(n>828)return -90;
}
{
if(this.cell_voltage.length>16)return -22;
if(4>828-n)return -90;n+=4;
for(let i=0;i<this.cell_voltage.length;++i){
if(!isFinite(this.cell_voltage[i]))return -22;
if(4>828-n)return -90;n+=4;
if(n>828)return -90;
}
}
{
if(this.cell_temperature.length>16)return -22;
if(4>828-n)return -90;n+=4;
for(let i=0;i<this.cell_temperature.length;++i){
if(!isFinite(this.cell_temperature[i]))return -22;
if(4>828-n)return -90;n+=4;
if(n>828)return -90;
}
}
{
if(!validUnicode(this.location)||this.location.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.location);if(size>256)return -22;if(4+size>828-n)return -90;n+=4+size;
if(n>828)return -90;
}
{
if(!validUnicode(this.serial_number)||this.serial_number.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.serial_number);if(size>256)return -22;if(4+size>828-n)return -90;n+=4+size;
if(n>828)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>reinterpret<u32>(this.voltage),4);
w.integer(<u64>reinterpret<u32>(this.temperature),4);
w.integer(<u64>reinterpret<u32>(this.current),4);
w.integer(<u64>reinterpret<u32>(this.charge),4);
w.integer(<u64>reinterpret<u32>(this.capacity),4);
w.integer(<u64>reinterpret<u32>(this.design_capacity),4);
w.integer(<u64>reinterpret<u32>(this.percentage),4);
w.integer(<u64>this.power_supply_status,1);
w.integer(<u64>this.power_supply_health,1);
w.integer(<u64>this.power_supply_technology,1);
w.u8(this.present?1:0);
w.u32(<u32>this.cell_voltage.length);
for(let i=0;i<this.cell_voltage.length;++i){
w.integer(<u64>reinterpret<u32>(this.cell_voltage[i]),4);
}
w.u32(<u32>this.cell_temperature.length);
for(let i=0;i<this.cell_temperature.length;++i){
w.integer(<u64>reinterpret<u32>(this.cell_temperature[i]),4);
}
w.text(this.location);
w.text(this.serial_number);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Sensor_msgsMsgBatteryState{const value=new Ros2Sensor_msgsMsgBatteryState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.voltage=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.temperature=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.current=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.charge=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.capacity=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.design_capacity=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.percentage=decoded;
}
{
const decoded=<u8>r.integer(1);
value.power_supply_status=decoded;
}
{
const decoded=<u8>r.integer(1);
value.power_supply_health=decoded;
}
{
const decoded=<u8>r.integer(1);
value.power_supply_technology=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.present=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.cell_voltage=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cell_voltage.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.cell_temperature=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cell_temperature.push(decoded);
}
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.location=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.serial_number=decoded;
}
return value;}
}
export class Ros2Sensor_msgsMsgBatteryStateCodec implements VendorCodec<Ros2Sensor_msgsMsgBatteryState>{
static readonly codecIdentity:string="UWVMROS1_9083c57dde00bba912db4dd736027cb92bbed2b1a79f965570dee86e1ff5a96e";
static readonly maxWireBytes:i32=828;
encodeChecked(value:Ros2Sensor_msgsMsgBatteryState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Sensor_msgsMsgBatteryState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Sensor_msgsMsgBatteryState>{const out=new VendorDecoded<Ros2Sensor_msgsMsgBatteryState>();if(bytes.length>828){out.status=90;return out;}if(bytes.length<60){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Sensor_msgsMsgBatteryState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Sensor_msgsMsgBatteryState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Sensor_msgsMsgBatteryState():changetype<Ros2Sensor_msgsMsgBatteryState>(out.value);}
}
/** Complete original sensor_msgs/msg/Imu; spellings are published in generation proof. */
export class Ros2Sensor_msgsMsgImu {
static readonly maxWireBytes:i32=436;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=436;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field orientation, native scalar geometry_msgs/msg/Quaternion. */
orientation:Ros2Geometry_msgsMsgQuaternion=new Ros2Geometry_msgsMsgQuaternion();
/** Exact native field orientation_covariance, native scalar float64. */
orientation_covariance:Array<f64>=new Array<f64>();
/** Exact native field angular_velocity, native scalar geometry_msgs/msg/Vector3. */
angular_velocity:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field angular_velocity_covariance, native scalar float64. */
angular_velocity_covariance:Array<f64>=new Array<f64>();
/** Exact native field linear_acceleration, native scalar geometry_msgs/msg/Vector3. */
linear_acceleration:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field linear_acceleration_covariance, native scalar float64. */
linear_acceleration_covariance:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<9;++i)this.orientation_covariance.push(0);
for(let i=0;i<9;++i)this.angular_velocity_covariance.push(0);
for(let i=0;i<9;++i)this.linear_acceleration_covariance.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>436-n)return -90;n+=child;
if(n>436)return -90;
}
{
const child=this.orientation.wireSize();if(child<0)return child;if(child>436-n)return -90;n+=child;
if(n>436)return -90;
}
{
if(this.orientation_covariance.length!=9)return -22;
for(let i=0;i<this.orientation_covariance.length;++i){
if(!isFinite(this.orientation_covariance[i]))return -22;
if(8>436-n)return -90;n+=8;
if(n>436)return -90;
}
}
{
const child=this.angular_velocity.wireSize();if(child<0)return child;if(child>436-n)return -90;n+=child;
if(n>436)return -90;
}
{
if(this.angular_velocity_covariance.length!=9)return -22;
for(let i=0;i<this.angular_velocity_covariance.length;++i){
if(!isFinite(this.angular_velocity_covariance[i]))return -22;
if(8>436-n)return -90;n+=8;
if(n>436)return -90;
}
}
{
const child=this.linear_acceleration.wireSize();if(child<0)return child;if(child>436-n)return -90;n+=child;
if(n>436)return -90;
}
{
if(this.linear_acceleration_covariance.length!=9)return -22;
for(let i=0;i<this.linear_acceleration_covariance.length;++i){
if(!isFinite(this.linear_acceleration_covariance[i]))return -22;
if(8>436-n)return -90;n+=8;
if(n>436)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.orientation.writeControl(w);
for(let i=0;i<this.orientation_covariance.length;++i){
w.integer(<u64>reinterpret<u64>(this.orientation_covariance[i]),8);
}
this.angular_velocity.writeControl(w);
for(let i=0;i<this.angular_velocity_covariance.length;++i){
w.integer(<u64>reinterpret<u64>(this.angular_velocity_covariance[i]),8);
}
this.linear_acceleration.writeControl(w);
for(let i=0;i<this.linear_acceleration_covariance.length;++i){
w.integer(<u64>reinterpret<u64>(this.linear_acceleration_covariance[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Sensor_msgsMsgImu{const value=new Ros2Sensor_msgsMsgImu();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgQuaternion.readControl(r);
value.orientation=decoded;
}
{
const count:u32=9;if(!r.valid||count>9){r.valid=false;return value;}
value.orientation_covariance=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.orientation_covariance.push(decoded);
}
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.angular_velocity=decoded;
}
{
const count:u32=9;if(!r.valid||count>9){r.valid=false;return value;}
value.angular_velocity_covariance=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.angular_velocity_covariance.push(decoded);
}
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.linear_acceleration=decoded;
}
{
const count:u32=9;if(!r.valid||count>9){r.valid=false;return value;}
value.linear_acceleration_covariance=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.linear_acceleration_covariance.push(decoded);
}
}
return value;}
}
export class Ros2Sensor_msgsMsgImuCodec implements VendorCodec<Ros2Sensor_msgsMsgImu>{
static readonly codecIdentity:string="UWVMROS1_e38d1c936a2a4a870b2f069af8d1796f69e6ce7b8cae0a792925ed2f6c0cae1b";
static readonly maxWireBytes:i32=436;
encodeChecked(value:Ros2Sensor_msgsMsgImu):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Sensor_msgsMsgImu):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Sensor_msgsMsgImu>{const out=new VendorDecoded<Ros2Sensor_msgsMsgImu>();if(bytes.length>436){out.status=90;return out;}if(bytes.length<308){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Sensor_msgsMsgImu.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Sensor_msgsMsgImu{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Sensor_msgsMsgImu():changetype<Ros2Sensor_msgsMsgImu>(out.value);}
}
/** Complete original sensor_msgs/msg/JointState; spellings are published in generation proof. */
export class Ros2Sensor_msgsMsgJointState {
static readonly maxWireBytes:i32=468;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=468;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field name, native scalar string. */
name:Array<string>=new Array<string>();
/** Exact native field position, native scalar float64. */
position:Array<f64>=new Array<f64>();
/** Exact native field velocity, native scalar float64. */
velocity:Array<f64>=new Array<f64>();
/** Exact native field effort, native scalar float64. */
effort:Array<f64>=new Array<f64>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>468-n)return -90;n+=child;
if(n>468)return -90;
}
{
if(this.name.length>2)return -22;
if(4>468-n)return -90;n+=4;
for(let i=0;i<this.name.length;++i){
if(!validUnicode(this.name[i])||this.name[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name[i]);if(size>128)return -22;if(4+size>468-n)return -90;n+=4+size;
if(n>468)return -90;
}
}
{
if(this.position.length>2)return -22;
if(4>468-n)return -90;n+=4;
for(let i=0;i<this.position.length;++i){
if(!isFinite(this.position[i]))return -22;
if(8>468-n)return -90;n+=8;
if(n>468)return -90;
}
}
{
if(this.velocity.length>2)return -22;
if(4>468-n)return -90;n+=4;
for(let i=0;i<this.velocity.length;++i){
if(!isFinite(this.velocity[i]))return -22;
if(8>468-n)return -90;n+=8;
if(n>468)return -90;
}
}
{
if(this.effort.length>2)return -22;
if(4>468-n)return -90;n+=4;
for(let i=0;i<this.effort.length;++i){
if(!isFinite(this.effort[i]))return -22;
if(8>468-n)return -90;n+=8;
if(n>468)return -90;
}
}
if((this.name.length!=0&&this.position.length!=0&&this.name.length!=this.position.length)||(this.name.length!=0&&this.velocity.length!=0&&this.name.length!=this.velocity.length)||(this.name.length!=0&&this.effort.length!=0&&this.name.length!=this.effort.length)||(this.position.length!=0&&this.velocity.length!=0&&this.position.length!=this.velocity.length)||(this.position.length!=0&&this.effort.length!=0&&this.position.length!=this.effort.length)||(this.velocity.length!=0&&this.effort.length!=0&&this.velocity.length!=this.effort.length))return -22;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.name.length);
for(let i=0;i<this.name.length;++i){
w.text(this.name[i]);
}
w.u32(<u32>this.position.length);
for(let i=0;i<this.position.length;++i){
w.integer(<u64>reinterpret<u64>(this.position[i]),8);
}
w.u32(<u32>this.velocity.length);
for(let i=0;i<this.velocity.length;++i){
w.integer(<u64>reinterpret<u64>(this.velocity[i]),8);
}
w.u32(<u32>this.effort.length);
for(let i=0;i<this.effort.length;++i){
w.integer(<u64>reinterpret<u64>(this.effort[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Sensor_msgsMsgJointState{const value=new Ros2Sensor_msgsMsgJointState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.name=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.name.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.position=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.position.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.velocity=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.velocity.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.effort=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.effort.push(decoded);
}
}
if((value.name.length!=0&&value.position.length!=0&&value.name.length!=value.position.length)||(value.name.length!=0&&value.velocity.length!=0&&value.name.length!=value.velocity.length)||(value.name.length!=0&&value.effort.length!=0&&value.name.length!=value.effort.length)||(value.position.length!=0&&value.velocity.length!=0&&value.position.length!=value.velocity.length)||(value.position.length!=0&&value.effort.length!=0&&value.position.length!=value.effort.length)||(value.velocity.length!=0&&value.effort.length!=0&&value.velocity.length!=value.effort.length))r.valid=false;
return value;}
}
export class Ros2Sensor_msgsMsgJointStateCodec implements VendorCodec<Ros2Sensor_msgsMsgJointState>{
static readonly codecIdentity:string="UWVMROS1_5c4d1ef0128c748e20f6b30c9c3bb5ef6e3c529a0b05b81764ac4774ce765bcf";
static readonly maxWireBytes:i32=468;
encodeChecked(value:Ros2Sensor_msgsMsgJointState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Sensor_msgsMsgJointState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Sensor_msgsMsgJointState>{const out=new VendorDecoded<Ros2Sensor_msgsMsgJointState>();if(bytes.length>468){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Sensor_msgsMsgJointState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Sensor_msgsMsgJointState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Sensor_msgsMsgJointState():changetype<Ros2Sensor_msgsMsgJointState>(out.value);}
}
/** Complete original sensor_msgs/msg/LaserScan; spellings are published in generation proof. */
export class Ros2Sensor_msgsMsgLaserScan {
static readonly maxWireBytes:i32=8368;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8368;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field angle_min, native scalar float32. */
angle_min:f32=0;
/** Exact native field angle_max, native scalar float32. */
angle_max:f32=0;
/** Exact native field angle_increment, native scalar float32. */
angle_increment:f32=0;
/** Exact native field time_increment, native scalar float32. */
time_increment:f32=0;
/** Exact native field scan_time, native scalar float32. */
scan_time:f32=0;
/** Exact native field range_min, native scalar float32. */
range_min:f32=0;
/** Exact native field range_max, native scalar float32. */
range_max:f32=0;
/** Exact native field ranges, native scalar float32. */
ranges:Array<f32>=new Array<f32>();
/** Exact native field intensities, native scalar float32. */
intensities:Array<f32>=new Array<f32>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>8368-n)return -90;n+=child;
if(n>8368)return -90;
}
{
if(!isFinite(this.angle_min))return -22;
if(4>8368-n)return -90;n+=4;
if(n>8368)return -90;
}
{
if(!isFinite(this.angle_max))return -22;
if(4>8368-n)return -90;n+=4;
if(n>8368)return -90;
}
{
if(!isFinite(this.angle_increment))return -22;
if(4>8368-n)return -90;n+=4;
if(n>8368)return -90;
}
{
if(!isFinite(this.time_increment))return -22;
if(4>8368-n)return -90;n+=4;
if(n>8368)return -90;
}
{
if(!isFinite(this.scan_time))return -22;
if(4>8368-n)return -90;n+=4;
if(n>8368)return -90;
}
{
if(!isFinite(this.range_min))return -22;
if(4>8368-n)return -90;n+=4;
if(n>8368)return -90;
}
{
if(!isFinite(this.range_max))return -22;
if(4>8368-n)return -90;n+=4;
if(n>8368)return -90;
}
{
if(this.ranges.length>1024)return -22;
if(4>8368-n)return -90;n+=4;
for(let i=0;i<this.ranges.length;++i){
if(!isFinite(this.ranges[i]))return -22;
if(4>8368-n)return -90;n+=4;
if(n>8368)return -90;
}
}
{
if(this.intensities.length>1024)return -22;
if(4>8368-n)return -90;n+=4;
for(let i=0;i<this.intensities.length;++i){
if(!isFinite(this.intensities[i]))return -22;
if(4>8368-n)return -90;n+=4;
if(n>8368)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>reinterpret<u32>(this.angle_min),4);
w.integer(<u64>reinterpret<u32>(this.angle_max),4);
w.integer(<u64>reinterpret<u32>(this.angle_increment),4);
w.integer(<u64>reinterpret<u32>(this.time_increment),4);
w.integer(<u64>reinterpret<u32>(this.scan_time),4);
w.integer(<u64>reinterpret<u32>(this.range_min),4);
w.integer(<u64>reinterpret<u32>(this.range_max),4);
w.u32(<u32>this.ranges.length);
for(let i=0;i<this.ranges.length;++i){
w.integer(<u64>reinterpret<u32>(this.ranges[i]),4);
}
w.u32(<u32>this.intensities.length);
for(let i=0;i<this.intensities.length;++i){
w.integer(<u64>reinterpret<u32>(this.intensities[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Sensor_msgsMsgLaserScan{const value=new Ros2Sensor_msgsMsgLaserScan();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.angle_min=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.angle_max=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.angle_increment=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.time_increment=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.scan_time=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.range_min=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.range_max=decoded;
}
{
const count=r.u32();if(!r.valid||count>1024){r.valid=false;return value;}
value.ranges=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ranges.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>1024){r.valid=false;return value;}
value.intensities=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.intensities.push(decoded);
}
}
return value;}
}
export class Ros2Sensor_msgsMsgLaserScanCodec implements VendorCodec<Ros2Sensor_msgsMsgLaserScan>{
static readonly codecIdentity:string="UWVMROS1_89faa8e14fc7f79a62a576d35c0bb377caf0cbaef6a6f181a5af2da0ead6cce4";
static readonly maxWireBytes:i32=8368;
encodeChecked(value:Ros2Sensor_msgsMsgLaserScan):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Sensor_msgsMsgLaserScan):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Sensor_msgsMsgLaserScan>{const out=new VendorDecoded<Ros2Sensor_msgsMsgLaserScan>();if(bytes.length>8368){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Sensor_msgsMsgLaserScan.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Sensor_msgsMsgLaserScan{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Sensor_msgsMsgLaserScan():changetype<Ros2Sensor_msgsMsgLaserScan>(out.value);}
}
/** Complete original sensor_msgs/msg/MagneticField; spellings are published in generation proof. */
export class Ros2Sensor_msgsMsgMagneticField {
static readonly maxWireBytes:i32=236;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=236;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field magnetic_field, native scalar geometry_msgs/msg/Vector3. */
magnetic_field:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field magnetic_field_covariance, native scalar float64. */
magnetic_field_covariance:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<9;++i)this.magnetic_field_covariance.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>236-n)return -90;n+=child;
if(n>236)return -90;
}
{
const child=this.magnetic_field.wireSize();if(child<0)return child;if(child>236-n)return -90;n+=child;
if(n>236)return -90;
}
{
if(this.magnetic_field_covariance.length!=9)return -22;
for(let i=0;i<this.magnetic_field_covariance.length;++i){
if(!isFinite(this.magnetic_field_covariance[i]))return -22;
if(8>236-n)return -90;n+=8;
if(n>236)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.magnetic_field.writeControl(w);
for(let i=0;i<this.magnetic_field_covariance.length;++i){
w.integer(<u64>reinterpret<u64>(this.magnetic_field_covariance[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Sensor_msgsMsgMagneticField{const value=new Ros2Sensor_msgsMsgMagneticField();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.magnetic_field=decoded;
}
{
const count:u32=9;if(!r.valid||count>9){r.valid=false;return value;}
value.magnetic_field_covariance=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.magnetic_field_covariance.push(decoded);
}
}
return value;}
}
export class Ros2Sensor_msgsMsgMagneticFieldCodec implements VendorCodec<Ros2Sensor_msgsMsgMagneticField>{
static readonly codecIdentity:string="UWVMROS1_56ae1437e0fbe7a6722ecf4795483d15f4c616e2e891372c30fa78f18be2cf8b";
static readonly maxWireBytes:i32=236;
encodeChecked(value:Ros2Sensor_msgsMsgMagneticField):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Sensor_msgsMsgMagneticField):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Sensor_msgsMsgMagneticField>{const out=new VendorDecoded<Ros2Sensor_msgsMsgMagneticField>();if(bytes.length>236){out.status=90;return out;}if(bytes.length<108){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Sensor_msgsMsgMagneticField.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Sensor_msgsMsgMagneticField{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Sensor_msgsMsgMagneticField():changetype<Ros2Sensor_msgsMsgMagneticField>(out.value);}
}
/** Complete original std_srvs/srv/SetBool_Request; spellings are published in generation proof. */
export class Ros2Std_srvsSrvSetBool_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field data, native scalar bool. */
data:bool=false;
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
w.u8(this.data?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Std_srvsSrvSetBool_Request{const value=new Ros2Std_srvsSrvSetBool_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.data=decoded;
}
return value;}
}
export class Ros2Std_srvsSrvSetBool_RequestCodec implements VendorCodec<Ros2Std_srvsSrvSetBool_Request>{
static readonly codecIdentity:string="UWVMROS1_2523e7915bb78fe01b68f2a856611b32a88f624a6e197a6ff312b613e8c65910";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Std_srvsSrvSetBool_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Std_srvsSrvSetBool_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Std_srvsSrvSetBool_Request>{const out=new VendorDecoded<Ros2Std_srvsSrvSetBool_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Std_srvsSrvSetBool_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Std_srvsSrvSetBool_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Std_srvsSrvSetBool_Request():changetype<Ros2Std_srvsSrvSetBool_Request>(out.value);}
}
/** Complete original std_srvs/srv/SetBool_Response; spellings are published in generation proof. */
export class Ros2Std_srvsSrvSetBool_Response {
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
static readControl(r:Reader):Ros2Std_srvsSrvSetBool_Response{const value=new Ros2Std_srvsSrvSetBool_Response();
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
export class Ros2Std_srvsSrvSetBool_ResponseCodec implements VendorCodec<Ros2Std_srvsSrvSetBool_Response>{
static readonly codecIdentity:string="UWVMROS1_f2d46a8440538094221a151716d8ec44db02455f59845c50aaf4e7964c27ebe4";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Std_srvsSrvSetBool_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Std_srvsSrvSetBool_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Std_srvsSrvSetBool_Response>{const out=new VendorDecoded<Ros2Std_srvsSrvSetBool_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Std_srvsSrvSetBool_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Std_srvsSrvSetBool_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Std_srvsSrvSetBool_Response():changetype<Ros2Std_srvsSrvSetBool_Response>(out.value);}
}
/** Complete original std_srvs/srv/Trigger_Request; spellings are published in generation proof. */
export class Ros2Std_srvsSrvTrigger_Request {
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
static readControl(r:Reader):Ros2Std_srvsSrvTrigger_Request{const value=new Ros2Std_srvsSrvTrigger_Request();
return value;}
}
export class Ros2Std_srvsSrvTrigger_RequestCodec implements VendorCodec<Ros2Std_srvsSrvTrigger_Request>{
static readonly codecIdentity:string="UWVMROS1_f1d8ad4ab7cf476cada6301f71ae24e046c244ba62bb6735183efe8e8c530a77";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Std_srvsSrvTrigger_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Std_srvsSrvTrigger_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Std_srvsSrvTrigger_Request>{const out=new VendorDecoded<Ros2Std_srvsSrvTrigger_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Std_srvsSrvTrigger_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Std_srvsSrvTrigger_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Std_srvsSrvTrigger_Request():changetype<Ros2Std_srvsSrvTrigger_Request>(out.value);}
}
/** Complete original std_srvs/srv/Trigger_Response; spellings are published in generation proof. */
export class Ros2Std_srvsSrvTrigger_Response {
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
static readControl(r:Reader):Ros2Std_srvsSrvTrigger_Response{const value=new Ros2Std_srvsSrvTrigger_Response();
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
export class Ros2Std_srvsSrvTrigger_ResponseCodec implements VendorCodec<Ros2Std_srvsSrvTrigger_Response>{
static readonly codecIdentity:string="UWVMROS1_58806c592727ad1011b32edb6bca3ca32d8e5dbe7f2fa20c86220daf64a10392";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Std_srvsSrvTrigger_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Std_srvsSrvTrigger_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Std_srvsSrvTrigger_Response>{const out=new VendorDecoded<Ros2Std_srvsSrvTrigger_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Std_srvsSrvTrigger_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Std_srvsSrvTrigger_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Std_srvsSrvTrigger_Response():changetype<Ros2Std_srvsSrvTrigger_Response>(out.value);}
}
/** Complete original turtlebot3_msgs/action/Patrol_Goal; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsActionPatrol_Goal {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field goal, native scalar geometry_msgs/msg/Vector3. */
goal:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.goal.wireSize();if(child<0)return child;if(child>24-n)return -90;n+=child;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.goal.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Turtlebot3_msgsActionPatrol_Goal{const value=new Ros2Turtlebot3_msgsActionPatrol_Goal();
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.goal=decoded;
}
return value;}
}
export class Ros2Turtlebot3_msgsActionPatrol_GoalCodec implements VendorCodec<Ros2Turtlebot3_msgsActionPatrol_Goal>{
static readonly codecIdentity:string="UWVMROS1_929abdbc422698464043e7e8d12c2a5edd418dcd52bbde4a542b33e6862ba96f";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Turtlebot3_msgsActionPatrol_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsActionPatrol_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsActionPatrol_Goal>{const out=new VendorDecoded<Ros2Turtlebot3_msgsActionPatrol_Goal>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsActionPatrol_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsActionPatrol_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsActionPatrol_Goal():changetype<Ros2Turtlebot3_msgsActionPatrol_Goal>(out.value);}
}
/** Complete original turtlebot3_msgs/action/Patrol_Result; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsActionPatrol_Result {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field result, native scalar string. */
result:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.result)||this.result.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.result);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.result);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Turtlebot3_msgsActionPatrol_Result{const value=new Ros2Turtlebot3_msgsActionPatrol_Result();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.result=decoded;
}
return value;}
}
export class Ros2Turtlebot3_msgsActionPatrol_ResultCodec implements VendorCodec<Ros2Turtlebot3_msgsActionPatrol_Result>{
static readonly codecIdentity:string="UWVMROS1_1d1fa277be54e1df3bd314a632ff80d79f22a306fc232219a7c23cce7a7703a3";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Turtlebot3_msgsActionPatrol_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsActionPatrol_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsActionPatrol_Result>{const out=new VendorDecoded<Ros2Turtlebot3_msgsActionPatrol_Result>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsActionPatrol_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsActionPatrol_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsActionPatrol_Result():changetype<Ros2Turtlebot3_msgsActionPatrol_Result>(out.value);}
}
/** Complete original turtlebot3_msgs/action/Patrol_Feedback; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsActionPatrol_Feedback {
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
static readControl(r:Reader):Ros2Turtlebot3_msgsActionPatrol_Feedback{const value=new Ros2Turtlebot3_msgsActionPatrol_Feedback();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.state=decoded;
}
return value;}
}
export class Ros2Turtlebot3_msgsActionPatrol_FeedbackCodec implements VendorCodec<Ros2Turtlebot3_msgsActionPatrol_Feedback>{
static readonly codecIdentity:string="UWVMROS1_7c2f3824653a3048e1b93fa95584543a01efae8bb67253d3371a2988407a601e";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Turtlebot3_msgsActionPatrol_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsActionPatrol_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsActionPatrol_Feedback>{const out=new VendorDecoded<Ros2Turtlebot3_msgsActionPatrol_Feedback>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsActionPatrol_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsActionPatrol_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsActionPatrol_Feedback():changetype<Ros2Turtlebot3_msgsActionPatrol_Feedback>(out.value);}
}
/** Complete original turtlebot3_msgs/msg/SensorState; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsMsgSensorState {
static readonly BUMPER_FORWARD:u8=1;
static readonly BUMPER_BACKWARD:u8=2;
static readonly CLIFF:u8=1;
static readonly SONAR:u8=1;
static readonly ILLUMINATION:u8=1;
static readonly BUTTON0:u8=1;
static readonly BUTTON1:u8=2;
static readonly ERROR_LEFT_MOTOR:u8=1;
static readonly ERROR_RIGHT_MOTOR:u8=2;
static readonly TORQUE_ON:u8=1;
static readonly TORQUE_OFF:u8=2;
static readonly maxWireBytes:i32=168;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=168;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field bumper, native scalar uint8. */
bumper:u8=0;
/** Exact native field cliff, native scalar float32. */
cliff:f32=0;
/** Exact native field sonar, native scalar float32. */
sonar:f32=0;
/** Exact native field illumination, native scalar float32. */
illumination:f32=0;
/** Exact native field led, native scalar uint8. */
led:u8=0;
/** Exact native field button, native scalar uint8. */
button:u8=0;
/** Exact native field torque, native scalar bool. */
torque:bool=false;
/** Exact native field left_encoder, native scalar int32. */
left_encoder:i32=0;
/** Exact native field right_encoder, native scalar int32. */
right_encoder:i32=0;
/** Exact native field battery, native scalar float32. */
battery:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>168-n)return -90;n+=child;
if(n>168)return -90;
}
{
if(1>168-n)return -90;n+=1;
if(n>168)return -90;
}
{
if(!isFinite(this.cliff))return -22;
if(4>168-n)return -90;n+=4;
if(n>168)return -90;
}
{
if(!isFinite(this.sonar))return -22;
if(4>168-n)return -90;n+=4;
if(n>168)return -90;
}
{
if(!isFinite(this.illumination))return -22;
if(4>168-n)return -90;n+=4;
if(n>168)return -90;
}
{
if(1>168-n)return -90;n+=1;
if(n>168)return -90;
}
{
if(1>168-n)return -90;n+=1;
if(n>168)return -90;
}
{
if(1>168-n)return -90;n+=1;
if(n>168)return -90;
}
{
if(4>168-n)return -90;n+=4;
if(n>168)return -90;
}
{
if(4>168-n)return -90;n+=4;
if(n>168)return -90;
}
{
if(!isFinite(this.battery))return -22;
if(4>168-n)return -90;n+=4;
if(n>168)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>this.bumper,1);
w.integer(<u64>reinterpret<u32>(this.cliff),4);
w.integer(<u64>reinterpret<u32>(this.sonar),4);
w.integer(<u64>reinterpret<u32>(this.illumination),4);
w.integer(<u64>this.led,1);
w.integer(<u64>this.button,1);
w.u8(this.torque?1:0);
w.integer(<u64>this.left_encoder,4);
w.integer(<u64>this.right_encoder,4);
w.integer(<u64>reinterpret<u32>(this.battery),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Turtlebot3_msgsMsgSensorState{const value=new Ros2Turtlebot3_msgsMsgSensorState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=<u8>r.integer(1);
value.bumper=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cliff=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.sonar=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.illumination=decoded;
}
{
const decoded=<u8>r.integer(1);
value.led=decoded;
}
{
const decoded=<u8>r.integer(1);
value.button=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.torque=decoded;
}
{
const decoded=<i32>r.integer(4);
value.left_encoder=decoded;
}
{
const decoded=<i32>r.integer(4);
value.right_encoder=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.battery=decoded;
}
return value;}
}
export class Ros2Turtlebot3_msgsMsgSensorStateCodec implements VendorCodec<Ros2Turtlebot3_msgsMsgSensorState>{
static readonly codecIdentity:string="UWVMROS1_7974ef7ca70ababf0e3f7a5e4099335a1398933833260798482f155291a62358";
static readonly maxWireBytes:i32=168;
encodeChecked(value:Ros2Turtlebot3_msgsMsgSensorState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsMsgSensorState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsMsgSensorState>{const out=new VendorDecoded<Ros2Turtlebot3_msgsMsgSensorState>();if(bytes.length>168){out.status=90;return out;}if(bytes.length<40){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsMsgSensorState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsMsgSensorState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsMsgSensorState():changetype<Ros2Turtlebot3_msgsMsgSensorState>(out.value);}
}
/** Complete original turtlebot3_msgs/msg/Sound; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsMsgSound {
static readonly OFF:u8=0;
static readonly ON:u8=1;
static readonly LOW_BATTERY:u8=2;
static readonly ERROR:u8=3;
static readonly BUTTON1:u8=4;
static readonly BUTTON2:u8=5;
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field value, native scalar uint8. */
value:u8=0;
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
w.integer(<u64>this.value,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Turtlebot3_msgsMsgSound{const value=new Ros2Turtlebot3_msgsMsgSound();
{
const decoded=<u8>r.integer(1);
value.value=decoded;
}
return value;}
}
export class Ros2Turtlebot3_msgsMsgSoundCodec implements VendorCodec<Ros2Turtlebot3_msgsMsgSound>{
static readonly codecIdentity:string="UWVMROS1_e8bd9a5fdf84ad1df0f264bd13a0e8966918de19ab62e91c7524dd8fc17a4e4b";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Turtlebot3_msgsMsgSound):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsMsgSound):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsMsgSound>{const out=new VendorDecoded<Ros2Turtlebot3_msgsMsgSound>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsMsgSound.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsMsgSound{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsMsgSound():changetype<Ros2Turtlebot3_msgsMsgSound>(out.value);}
}
/** Complete original turtlebot3_msgs/msg/VersionInfo; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsMsgVersionInfo {
static readonly maxWireBytes:i32=780;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=780;
/** Exact native field hardware, native scalar string. */
hardware:string="";
/** Exact native field firmware, native scalar string. */
firmware:string="";
/** Exact native field software, native scalar string. */
software:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.hardware)||this.hardware.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.hardware);if(size>256)return -22;if(4+size>780-n)return -90;n+=4+size;
if(n>780)return -90;
}
{
if(!validUnicode(this.firmware)||this.firmware.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.firmware);if(size>256)return -22;if(4+size>780-n)return -90;n+=4+size;
if(n>780)return -90;
}
{
if(!validUnicode(this.software)||this.software.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.software);if(size>256)return -22;if(4+size>780-n)return -90;n+=4+size;
if(n>780)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.hardware);
w.text(this.firmware);
w.text(this.software);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Turtlebot3_msgsMsgVersionInfo{const value=new Ros2Turtlebot3_msgsMsgVersionInfo();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.hardware=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.firmware=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.software=decoded;
}
return value;}
}
export class Ros2Turtlebot3_msgsMsgVersionInfoCodec implements VendorCodec<Ros2Turtlebot3_msgsMsgVersionInfo>{
static readonly codecIdentity:string="UWVMROS1_7668c77b01a05eeb8ba3c5f259f3f3a9dfad3d4fdf8aa566d65f6e8f1c0f2d6a";
static readonly maxWireBytes:i32=780;
encodeChecked(value:Ros2Turtlebot3_msgsMsgVersionInfo):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsMsgVersionInfo):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsMsgVersionInfo>{const out=new VendorDecoded<Ros2Turtlebot3_msgsMsgVersionInfo>();if(bytes.length>780){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsMsgVersionInfo.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsMsgVersionInfo{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsMsgVersionInfo():changetype<Ros2Turtlebot3_msgsMsgVersionInfo>(out.value);}
}
/** Complete original turtlebot3_msgs/srv/Dqn_Request; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsSrvDqn_Request {
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field action, native scalar uint8. */
action:u8=0;
/** Exact native field init, native scalar bool. */
init:bool=false;
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
w.integer(<u64>this.action,1);
w.u8(this.init?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Turtlebot3_msgsSrvDqn_Request{const value=new Ros2Turtlebot3_msgsSrvDqn_Request();
{
const decoded=<u8>r.integer(1);
value.action=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.init=decoded;
}
return value;}
}
export class Ros2Turtlebot3_msgsSrvDqn_RequestCodec implements VendorCodec<Ros2Turtlebot3_msgsSrvDqn_Request>{
static readonly codecIdentity:string="UWVMROS1_14042e65db9f4feef7a43bfb44255795470bb0ed91a3cf301ebec98c2d4e2d0e";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Turtlebot3_msgsSrvDqn_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsSrvDqn_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsSrvDqn_Request>{const out=new VendorDecoded<Ros2Turtlebot3_msgsSrvDqn_Request>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsSrvDqn_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsSrvDqn_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsSrvDqn_Request():changetype<Ros2Turtlebot3_msgsSrvDqn_Request>(out.value);}
}
/** Complete original turtlebot3_msgs/srv/Dqn_Response; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsSrvDqn_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field state, native scalar float32. */
state:Array<f32>=new Array<f32>();
/** Exact native field reward, native scalar float32. */
reward:f32=0;
/** Exact native field done, native scalar bool. */
done:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.state.length>128)return -22;
if(4>521-n)return -90;n+=4;
for(let i=0;i<this.state.length;++i){
if(!isFinite(this.state[i]))return -22;
if(4>521-n)return -90;n+=4;
if(n>521)return -90;
}
}
{
if(!isFinite(this.reward))return -22;
if(4>521-n)return -90;n+=4;
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
w.u32(<u32>this.state.length);
for(let i=0;i<this.state.length;++i){
w.integer(<u64>reinterpret<u32>(this.state[i]),4);
}
w.integer(<u64>reinterpret<u32>(this.reward),4);
w.u8(this.done?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Turtlebot3_msgsSrvDqn_Response{const value=new Ros2Turtlebot3_msgsSrvDqn_Response();
{
const count=r.u32();if(!r.valid||count>128){r.valid=false;return value;}
value.state=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.state.push(decoded);
}
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.reward=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.done=decoded;
}
return value;}
}
export class Ros2Turtlebot3_msgsSrvDqn_ResponseCodec implements VendorCodec<Ros2Turtlebot3_msgsSrvDqn_Response>{
static readonly codecIdentity:string="UWVMROS1_5d256e9c87d7959917f07e7cfcc0049fc18f79f547d83b72bd388a63c1417c6e";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Turtlebot3_msgsSrvDqn_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsSrvDqn_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsSrvDqn_Response>{const out=new VendorDecoded<Ros2Turtlebot3_msgsSrvDqn_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsSrvDqn_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsSrvDqn_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsSrvDqn_Response():changetype<Ros2Turtlebot3_msgsSrvDqn_Response>(out.value);}
}
/** Complete original turtlebot3_msgs/srv/Goal_Request; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsSrvGoal_Request {
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
static readControl(r:Reader):Ros2Turtlebot3_msgsSrvGoal_Request{const value=new Ros2Turtlebot3_msgsSrvGoal_Request();
return value;}
}
export class Ros2Turtlebot3_msgsSrvGoal_RequestCodec implements VendorCodec<Ros2Turtlebot3_msgsSrvGoal_Request>{
static readonly codecIdentity:string="UWVMROS1_f76094f3cf00772bb80b59e846f1160ae6255fc0d931bbcba362104081698ded";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Turtlebot3_msgsSrvGoal_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsSrvGoal_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsSrvGoal_Request>{const out=new VendorDecoded<Ros2Turtlebot3_msgsSrvGoal_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsSrvGoal_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsSrvGoal_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsSrvGoal_Request():changetype<Ros2Turtlebot3_msgsSrvGoal_Request>(out.value);}
}
/** Complete original turtlebot3_msgs/srv/Goal_Response; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsSrvGoal_Response {
static readonly maxWireBytes:i32=9;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=9;
/** Exact native field pose_x, native scalar float32. */
pose_x:f32=0;
/** Exact native field pose_y, native scalar float32. */
pose_y:f32=0;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.pose_x))return -22;
if(4>9-n)return -90;n+=4;
if(n>9)return -90;
}
{
if(!isFinite(this.pose_y))return -22;
if(4>9-n)return -90;n+=4;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.pose_x),4);
w.integer(<u64>reinterpret<u32>(this.pose_y),4);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Turtlebot3_msgsSrvGoal_Response{const value=new Ros2Turtlebot3_msgsSrvGoal_Response();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.pose_x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.pose_y=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Turtlebot3_msgsSrvGoal_ResponseCodec implements VendorCodec<Ros2Turtlebot3_msgsSrvGoal_Response>{
static readonly codecIdentity:string="UWVMROS1_8b9022a7fb8c68a0b063b080cd05c230b653169badbe205bcd8051b8020f7990";
static readonly maxWireBytes:i32=9;
encodeChecked(value:Ros2Turtlebot3_msgsSrvGoal_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsSrvGoal_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsSrvGoal_Response>{const out=new VendorDecoded<Ros2Turtlebot3_msgsSrvGoal_Response>();if(bytes.length>9){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsSrvGoal_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsSrvGoal_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsSrvGoal_Response():changetype<Ros2Turtlebot3_msgsSrvGoal_Response>(out.value);}
}
/** Complete original turtlebot3_msgs/srv/Sound_Request; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsSrvSound_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field value, native scalar uint8. */
value:u8=0;
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
w.integer(<u64>this.value,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Turtlebot3_msgsSrvSound_Request{const value=new Ros2Turtlebot3_msgsSrvSound_Request();
{
const decoded=<u8>r.integer(1);
value.value=decoded;
}
return value;}
}
export class Ros2Turtlebot3_msgsSrvSound_RequestCodec implements VendorCodec<Ros2Turtlebot3_msgsSrvSound_Request>{
static readonly codecIdentity:string="UWVMROS1_0dcba95f5ba646d9a24b0054ba603daf1d1e5e4e9a6ffe7387cd61bff2e4151a";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Turtlebot3_msgsSrvSound_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsSrvSound_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsSrvSound_Request>{const out=new VendorDecoded<Ros2Turtlebot3_msgsSrvSound_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsSrvSound_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsSrvSound_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsSrvSound_Request():changetype<Ros2Turtlebot3_msgsSrvSound_Request>(out.value);}
}
/** Complete original turtlebot3_msgs/srv/Sound_Response; spellings are published in generation proof. */
export class Ros2Turtlebot3_msgsSrvSound_Response {
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
static readControl(r:Reader):Ros2Turtlebot3_msgsSrvSound_Response{const value=new Ros2Turtlebot3_msgsSrvSound_Response();
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
export class Ros2Turtlebot3_msgsSrvSound_ResponseCodec implements VendorCodec<Ros2Turtlebot3_msgsSrvSound_Response>{
static readonly codecIdentity:string="UWVMROS1_bc5a5fd3e2bf733a278b8a1315c8905b27fff49b4844df238d5574149068998e";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Turtlebot3_msgsSrvSound_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Turtlebot3_msgsSrvSound_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Turtlebot3_msgsSrvSound_Response>{const out=new VendorDecoded<Ros2Turtlebot3_msgsSrvSound_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Turtlebot3_msgsSrvSound_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Turtlebot3_msgsSrvSound_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Turtlebot3_msgsSrvSound_Response():changetype<Ros2Turtlebot3_msgsSrvSound_Response>(out.value);}
}
