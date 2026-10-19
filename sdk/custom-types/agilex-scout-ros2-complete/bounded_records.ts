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
/** Complete original nav_msgs/msg/Odometry; spellings are published in generation proof. */
export class Ros2Nav_msgsMsgOdometry {
static readonly maxWireBytes:i32=952;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=952;
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
const child=this.header.wireSize();if(child<0)return child;if(child>952-n)return -90;n+=child;
if(n>952)return -90;
}
{
if(!validUnicode(this.child_frame_id)||this.child_frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.child_frame_id);if(size>128)return -22;if(4+size>952-n)return -90;n+=4+size;
if(n>952)return -90;
}
{
const child=this.pose.wireSize();if(child<0)return child;if(child>952-n)return -90;n+=child;
if(n>952)return -90;
}
{
const child=this.twist.wireSize();if(child<0)return child;if(child>952-n)return -90;n+=child;
if(n>952)return -90;
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
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
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
static readonly codecIdentity:string="UWVMROS1_fdfdd74c211c22bfd167c3e33a098fe21198cb095bbd0313fc765f38b1f334ec";
static readonly maxWireBytes:i32=952;
encodeChecked(value:Ros2Nav_msgsMsgOdometry):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Nav_msgsMsgOdometry):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Nav_msgsMsgOdometry>{const out=new VendorDecoded<Ros2Nav_msgsMsgOdometry>();if(bytes.length>952){out.status=90;return out;}if(bytes.length<696){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Nav_msgsMsgOdometry.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Nav_msgsMsgOdometry{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Nav_msgsMsgOdometry():changetype<Ros2Nav_msgsMsgOdometry>(out.value);}
}
/** Complete original scout_msgs/msg/ScoutActuatorState; spellings are published in generation proof. */
export class Ros2Scout_msgsMsgScoutActuatorState {
static readonly maxWireBytes:i32=19;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=19;
/** Exact native field rpm, native scalar int16. */
rpm:i16=0;
/** Exact native field current, native scalar float64. */
current:f64=0;
/** Exact native field driver_voltage, native scalar float32. */
driver_voltage:f32=0;
/** Exact native field driver_temperature, native scalar float32. */
driver_temperature:f32=0;
/** Exact native field motor_temperature, native scalar int8. */
motor_temperature:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>19-n)return -90;n+=2;
if(n>19)return -90;
}
{
if(!isFinite(this.current))return -22;
if(8>19-n)return -90;n+=8;
if(n>19)return -90;
}
{
if(!isFinite(this.driver_voltage))return -22;
if(4>19-n)return -90;n+=4;
if(n>19)return -90;
}
{
if(!isFinite(this.driver_temperature))return -22;
if(4>19-n)return -90;n+=4;
if(n>19)return -90;
}
{
if(1>19-n)return -90;n+=1;
if(n>19)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.rpm,2);
w.integer(<u64>reinterpret<u64>(this.current),8);
w.integer(<u64>reinterpret<u32>(this.driver_voltage),4);
w.integer(<u64>reinterpret<u32>(this.driver_temperature),4);
w.integer(<u64>this.motor_temperature,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Scout_msgsMsgScoutActuatorState{const value=new Ros2Scout_msgsMsgScoutActuatorState();
{
const decoded=<i16>r.integer(2);
value.rpm=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.current=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.driver_voltage=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.driver_temperature=decoded;
}
{
const decoded=<i8>r.integer(1);
value.motor_temperature=decoded;
}
return value;}
}
export class Ros2Scout_msgsMsgScoutActuatorStateCodec implements VendorCodec<Ros2Scout_msgsMsgScoutActuatorState>{
static readonly codecIdentity:string="UWVMROS1_e7955c07aedaf42f8d97f873d60ae5c0dfc562faa1d64848f11b333f797350c6";
static readonly maxWireBytes:i32=19;
encodeChecked(value:Ros2Scout_msgsMsgScoutActuatorState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Scout_msgsMsgScoutActuatorState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Scout_msgsMsgScoutActuatorState>{const out=new VendorDecoded<Ros2Scout_msgsMsgScoutActuatorState>();if(bytes.length>19){out.status=90;return out;}if(bytes.length<19){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Scout_msgsMsgScoutActuatorState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Scout_msgsMsgScoutActuatorState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Scout_msgsMsgScoutActuatorState():changetype<Ros2Scout_msgsMsgScoutActuatorState>(out.value);}
}
/** Complete original scout_msgs/msg/ScoutLightCmd; spellings are published in generation proof. */
export class Ros2Scout_msgsMsgScoutLightCmd {
static readonly LIGHT_CONST_OFF:u8=0;
static readonly LIGHT_CONST_ON:u8=1;
static readonly LIGHT_BREATH:u8=2;
static readonly LIGHT_CUSTOM:u8=3;
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field cmd_ctrl_allowed, native scalar bool. */
cmd_ctrl_allowed:bool=false;
/** Exact native field front_mode, native scalar uint8. */
front_mode:u8=0;
/** Exact native field front_custom_value, native scalar uint8. */
front_custom_value:u8=0;
/** Exact native field rear_mode, native scalar uint8. */
rear_mode:u8=0;
/** Exact native field rear_custom_value, native scalar uint8. */
rear_custom_value:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.cmd_ctrl_allowed?1:0);
w.integer(<u64>this.front_mode,1);
w.integer(<u64>this.front_custom_value,1);
w.integer(<u64>this.rear_mode,1);
w.integer(<u64>this.rear_custom_value,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Scout_msgsMsgScoutLightCmd{const value=new Ros2Scout_msgsMsgScoutLightCmd();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cmd_ctrl_allowed=decoded;
}
{
const decoded=<u8>r.integer(1);
value.front_mode=decoded;
}
{
const decoded=<u8>r.integer(1);
value.front_custom_value=decoded;
}
{
const decoded=<u8>r.integer(1);
value.rear_mode=decoded;
}
{
const decoded=<u8>r.integer(1);
value.rear_custom_value=decoded;
}
return value;}
}
export class Ros2Scout_msgsMsgScoutLightCmdCodec implements VendorCodec<Ros2Scout_msgsMsgScoutLightCmd>{
static readonly codecIdentity:string="UWVMROS1_a82206813682fbf1a6b41009c12191ad9ce47c98444d8ea9792e7b212ee602d7";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Scout_msgsMsgScoutLightCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Scout_msgsMsgScoutLightCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Scout_msgsMsgScoutLightCmd>{const out=new VendorDecoded<Ros2Scout_msgsMsgScoutLightCmd>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Scout_msgsMsgScoutLightCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Scout_msgsMsgScoutLightCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Scout_msgsMsgScoutLightCmd():changetype<Ros2Scout_msgsMsgScoutLightCmd>(out.value);}
}
/** Complete original scout_msgs/msg/ScoutLightState; spellings are published in generation proof. */
export class Ros2Scout_msgsMsgScoutLightState {
static readonly maxWireBytes:i32=2;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2;
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact native field custom_value, native scalar uint8. */
custom_value:u8=0;
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
w.integer(<u64>this.mode,1);
w.integer(<u64>this.custom_value,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Scout_msgsMsgScoutLightState{const value=new Ros2Scout_msgsMsgScoutLightState();
{
const decoded=<u8>r.integer(1);
value.mode=decoded;
}
{
const decoded=<u8>r.integer(1);
value.custom_value=decoded;
}
return value;}
}
export class Ros2Scout_msgsMsgScoutLightStateCodec implements VendorCodec<Ros2Scout_msgsMsgScoutLightState>{
static readonly codecIdentity:string="UWVMROS1_4f42b9365cc76673b156e725aac9448e0f0d806105d3717cdb2628c384a6d76a";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros2Scout_msgsMsgScoutLightState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Scout_msgsMsgScoutLightState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Scout_msgsMsgScoutLightState>{const out=new VendorDecoded<Ros2Scout_msgsMsgScoutLightState>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Scout_msgsMsgScoutLightState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Scout_msgsMsgScoutLightState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Scout_msgsMsgScoutLightState():changetype<Ros2Scout_msgsMsgScoutLightState>(out.value);}
}
/** Complete original scout_msgs/msg/ScoutRCState; spellings are published in generation proof. */
export class Ros2Scout_msgsMsgScoutRCState {
static readonly maxWireBytes:i32=9;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=9;
/** Exact native field swa, native scalar uint8. */
swa:u8=0;
/** Exact native field swb, native scalar uint8. */
swb:u8=0;
/** Exact native field swc, native scalar uint8. */
swc:u8=0;
/** Exact native field swd, native scalar uint8. */
swd:u8=0;
/** Exact native field stick_right_v, native scalar int8. */
stick_right_v:i8=0;
/** Exact native field stick_right_h, native scalar int8. */
stick_right_h:i8=0;
/** Exact native field stick_left_v, native scalar int8. */
stick_left_v:i8=0;
/** Exact native field stick_left_h, native scalar int8. */
stick_left_h:i8=0;
/** Exact native field var_a, native scalar int8. */
var_a:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(1>9-n)return -90;n+=1;
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
w.integer(<u64>this.swa,1);
w.integer(<u64>this.swb,1);
w.integer(<u64>this.swc,1);
w.integer(<u64>this.swd,1);
w.integer(<u64>this.stick_right_v,1);
w.integer(<u64>this.stick_right_h,1);
w.integer(<u64>this.stick_left_v,1);
w.integer(<u64>this.stick_left_h,1);
w.integer(<u64>this.var_a,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Scout_msgsMsgScoutRCState{const value=new Ros2Scout_msgsMsgScoutRCState();
{
const decoded=<u8>r.integer(1);
value.swa=decoded;
}
{
const decoded=<u8>r.integer(1);
value.swb=decoded;
}
{
const decoded=<u8>r.integer(1);
value.swc=decoded;
}
{
const decoded=<u8>r.integer(1);
value.swd=decoded;
}
{
const decoded=<i8>r.integer(1);
value.stick_right_v=decoded;
}
{
const decoded=<i8>r.integer(1);
value.stick_right_h=decoded;
}
{
const decoded=<i8>r.integer(1);
value.stick_left_v=decoded;
}
{
const decoded=<i8>r.integer(1);
value.stick_left_h=decoded;
}
{
const decoded=<i8>r.integer(1);
value.var_a=decoded;
}
return value;}
}
export class Ros2Scout_msgsMsgScoutRCStateCodec implements VendorCodec<Ros2Scout_msgsMsgScoutRCState>{
static readonly codecIdentity:string="UWVMROS1_ba13e4c728041d14352c021fe5c1c5eee5d531c84ac64c61d944d6192dcd4298";
static readonly maxWireBytes:i32=9;
encodeChecked(value:Ros2Scout_msgsMsgScoutRCState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Scout_msgsMsgScoutRCState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Scout_msgsMsgScoutRCState>{const out=new VendorDecoded<Ros2Scout_msgsMsgScoutRCState>();if(bytes.length>9){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Scout_msgsMsgScoutRCState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Scout_msgsMsgScoutRCState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Scout_msgsMsgScoutRCState():changetype<Ros2Scout_msgsMsgScoutRCState>(out.value);}
}
/** Complete original scout_msgs/msg/ScoutStatus; spellings are published in generation proof. */
export class Ros2Scout_msgsMsgScoutStatus {
static readonly MOTOR_ID_FRONT_RIGHT:i8=0;
static readonly MOTOR_ID_FRONT_LEFT:i8=1;
static readonly MOTOR_ID_REAR_RIGHT:i8=2;
static readonly MOTOR_ID_REAR_LEFT:i8=3;
static readonly LIGHT_ID_FRONT:i8=0;
static readonly LIGHT_ID_REAR:i8=1;
static readonly maxWireBytes:i32=249;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=249;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field linear_velocity, native scalar float64. */
linear_velocity:f64=0;
/** Exact native field angular_velocity, native scalar float64. */
angular_velocity:f64=0;
/** Exact native field vehicle_state, native scalar uint8. */
vehicle_state:u8=0;
/** Exact native field control_mode, native scalar uint8. */
control_mode:u8=0;
/** Exact native field error_code, native scalar uint16. */
error_code:u16=0;
/** Exact native field battery_voltage, native scalar float64. */
battery_voltage:f64=0;
/** Exact native field actuator_states, native scalar scout_msgs/msg/ScoutActuatorState. */
actuator_states:Array<Ros2Scout_msgsMsgScoutActuatorState>=new Array<Ros2Scout_msgsMsgScoutActuatorState>();
/** Exact native field light_control_enabled, native scalar bool. */
light_control_enabled:bool=false;
/** Exact native field front_light_state, native scalar scout_msgs/msg/ScoutLightState. */
front_light_state:Ros2Scout_msgsMsgScoutLightState=new Ros2Scout_msgsMsgScoutLightState();
/** Exact native field rear_light_state, native scalar scout_msgs/msg/ScoutLightState. */
rear_light_state:Ros2Scout_msgsMsgScoutLightState=new Ros2Scout_msgsMsgScoutLightState();
constructor(){
for(let i=0;i<4;++i)this.actuator_states.push(new Ros2Scout_msgsMsgScoutActuatorState());
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>249-n)return -90;n+=child;
if(n>249)return -90;
}
{
if(!isFinite(this.linear_velocity))return -22;
if(8>249-n)return -90;n+=8;
if(n>249)return -90;
}
{
if(!isFinite(this.angular_velocity))return -22;
if(8>249-n)return -90;n+=8;
if(n>249)return -90;
}
{
if(1>249-n)return -90;n+=1;
if(n>249)return -90;
}
{
if(1>249-n)return -90;n+=1;
if(n>249)return -90;
}
{
if(2>249-n)return -90;n+=2;
if(n>249)return -90;
}
{
if(!isFinite(this.battery_voltage))return -22;
if(8>249-n)return -90;n+=8;
if(n>249)return -90;
}
{
if(this.actuator_states.length!=4)return -22;
for(let i=0;i<this.actuator_states.length;++i){
const child=this.actuator_states[i].wireSize();if(child<0)return child;if(child>249-n)return -90;n+=child;
if(n>249)return -90;
}
}
{
if(1>249-n)return -90;n+=1;
if(n>249)return -90;
}
{
const child=this.front_light_state.wireSize();if(child<0)return child;if(child>249-n)return -90;n+=child;
if(n>249)return -90;
}
{
const child=this.rear_light_state.wireSize();if(child<0)return child;if(child>249-n)return -90;n+=child;
if(n>249)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>reinterpret<u64>(this.linear_velocity),8);
w.integer(<u64>reinterpret<u64>(this.angular_velocity),8);
w.integer(<u64>this.vehicle_state,1);
w.integer(<u64>this.control_mode,1);
w.integer(<u64>this.error_code,2);
w.integer(<u64>reinterpret<u64>(this.battery_voltage),8);
for(let i=0;i<this.actuator_states.length;++i){
this.actuator_states[i].writeControl(w);
}
w.u8(this.light_control_enabled?1:0);
this.front_light_state.writeControl(w);
this.rear_light_state.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Scout_msgsMsgScoutStatus{const value=new Ros2Scout_msgsMsgScoutStatus();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.linear_velocity=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.angular_velocity=decoded;
}
{
const decoded=<u8>r.integer(1);
value.vehicle_state=decoded;
}
{
const decoded=<u8>r.integer(1);
value.control_mode=decoded;
}
{
const decoded=<u16>r.integer(2);
value.error_code=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.battery_voltage=decoded;
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.actuator_states=new Array<Ros2Scout_msgsMsgScoutActuatorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Scout_msgsMsgScoutActuatorState.readControl(r);
value.actuator_states.push(decoded);
}
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.light_control_enabled=decoded;
}
{
const decoded=Ros2Scout_msgsMsgScoutLightState.readControl(r);
value.front_light_state=decoded;
}
{
const decoded=Ros2Scout_msgsMsgScoutLightState.readControl(r);
value.rear_light_state=decoded;
}
return value;}
}
export class Ros2Scout_msgsMsgScoutStatusCodec implements VendorCodec<Ros2Scout_msgsMsgScoutStatus>{
static readonly codecIdentity:string="UWVMROS1_75db4db1dcc07210d543b8e6bd6b6a7aadbf23db8043ff372a3b224c5c235b86";
static readonly maxWireBytes:i32=249;
encodeChecked(value:Ros2Scout_msgsMsgScoutStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Scout_msgsMsgScoutStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Scout_msgsMsgScoutStatus>{const out=new VendorDecoded<Ros2Scout_msgsMsgScoutStatus>();if(bytes.length>249){out.status=90;return out;}if(bytes.length<121){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Scout_msgsMsgScoutStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Scout_msgsMsgScoutStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Scout_msgsMsgScoutStatus():changetype<Ros2Scout_msgsMsgScoutStatus>(out.value);}
}
