/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original geometry_msgs/Point; spellings are published in generation proof. */
export class Ros1Geometry_msgsPoint {
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
static readControl(r:Reader):Ros1Geometry_msgsPoint{const value=new Ros1Geometry_msgsPoint();
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
export class Ros1Geometry_msgsPointCodec implements VendorCodec<Ros1Geometry_msgsPoint>{
static readonly codecIdentity:string="UWVMROS1_b55e2bde60f6cedb426cada54bc7baefc536c7bc731bd49fb152d89a892de62b";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Geometry_msgsPoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsPoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsPoint>{const out=new VendorDecoded<Ros1Geometry_msgsPoint>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsPoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsPoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsPoint():changetype<Ros1Geometry_msgsPoint>(out.value);}
}
/** Complete original geometry_msgs/Quaternion; spellings are published in generation proof. */
export class Ros1Geometry_msgsQuaternion {
static readonly maxWireBytes:i32=32;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=32;
/** Exact native field x, native scalar float64. */
x:f64=0;
/** Exact native field y, native scalar float64. */
y:f64=0;
/** Exact native field z, native scalar float64. */
z:f64=0;
/** Exact native field w, native scalar float64. */
w:f64=0;
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
static readControl(r:Reader):Ros1Geometry_msgsQuaternion{const value=new Ros1Geometry_msgsQuaternion();
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
export class Ros1Geometry_msgsQuaternionCodec implements VendorCodec<Ros1Geometry_msgsQuaternion>{
static readonly codecIdentity:string="UWVMROS1_48fe20d5d29a8ca1a630605a58f80baacc52d9a65b1ca87efd50ffdd52882934";
static readonly maxWireBytes:i32=32;
encodeChecked(value:Ros1Geometry_msgsQuaternion):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsQuaternion):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsQuaternion>{const out=new VendorDecoded<Ros1Geometry_msgsQuaternion>();if(bytes.length>32){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsQuaternion.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsQuaternion{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsQuaternion():changetype<Ros1Geometry_msgsQuaternion>(out.value);}
}
/** Complete original geometry_msgs/Pose; spellings are published in generation proof. */
export class Ros1Geometry_msgsPose {
static readonly maxWireBytes:i32=56;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=56;
/** Exact native field position, native scalar geometry_msgs/Point. */
position:Ros1Geometry_msgsPoint=new Ros1Geometry_msgsPoint();
/** Exact native field orientation, native scalar geometry_msgs/Quaternion. */
orientation:Ros1Geometry_msgsQuaternion=new Ros1Geometry_msgsQuaternion();
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
static readControl(r:Reader):Ros1Geometry_msgsPose{const value=new Ros1Geometry_msgsPose();
{
const decoded=Ros1Geometry_msgsPoint.readControl(r);
value.position=decoded;
}
{
const decoded=Ros1Geometry_msgsQuaternion.readControl(r);
value.orientation=decoded;
}
return value;}
}
export class Ros1Geometry_msgsPoseCodec implements VendorCodec<Ros1Geometry_msgsPose>{
static readonly codecIdentity:string="UWVMROS1_e49ed287cd0de31a16c8cd49ad8f41f81d44e23306b8dcb61da6e0e789595471";
static readonly maxWireBytes:i32=56;
encodeChecked(value:Ros1Geometry_msgsPose):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsPose):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsPose>{const out=new VendorDecoded<Ros1Geometry_msgsPose>();if(bytes.length>56){out.status=90;return out;}if(bytes.length<56){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsPose.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsPose{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsPose():changetype<Ros1Geometry_msgsPose>(out.value);}
}
/** Complete original geometry_msgs/PoseWithCovariance; spellings are published in generation proof. */
export class Ros1Geometry_msgsPoseWithCovariance {
static readonly maxWireBytes:i32=344;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=344;
/** Exact native field pose, native scalar geometry_msgs/Pose. */
pose:Ros1Geometry_msgsPose=new Ros1Geometry_msgsPose();
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
static readControl(r:Reader):Ros1Geometry_msgsPoseWithCovariance{const value=new Ros1Geometry_msgsPoseWithCovariance();
{
const decoded=Ros1Geometry_msgsPose.readControl(r);
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
export class Ros1Geometry_msgsPoseWithCovarianceCodec implements VendorCodec<Ros1Geometry_msgsPoseWithCovariance>{
static readonly codecIdentity:string="UWVMROS1_17b25963aafb3adfd0432febe040c9b38a531a7a667949b89e0f3937585ef8cf";
static readonly maxWireBytes:i32=344;
encodeChecked(value:Ros1Geometry_msgsPoseWithCovariance):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsPoseWithCovariance):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsPoseWithCovariance>{const out=new VendorDecoded<Ros1Geometry_msgsPoseWithCovariance>();if(bytes.length>344){out.status=90;return out;}if(bytes.length<344){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsPoseWithCovariance.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsPoseWithCovariance{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsPoseWithCovariance():changetype<Ros1Geometry_msgsPoseWithCovariance>(out.value);}
}
/** Complete original geometry_msgs/Vector3; spellings are published in generation proof. */
export class Ros1Geometry_msgsVector3 {
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
static readControl(r:Reader):Ros1Geometry_msgsVector3{const value=new Ros1Geometry_msgsVector3();
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
export class Ros1Geometry_msgsVector3Codec implements VendorCodec<Ros1Geometry_msgsVector3>{
static readonly codecIdentity:string="UWVMROS1_e90efcf4803d82763e0b83f8ef5502b5d042158873fcc91be32543d25d7632bc";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Geometry_msgsVector3):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsVector3):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsVector3>{const out=new VendorDecoded<Ros1Geometry_msgsVector3>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsVector3.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsVector3{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsVector3():changetype<Ros1Geometry_msgsVector3>(out.value);}
}
/** Complete original geometry_msgs/Twist; spellings are published in generation proof. */
export class Ros1Geometry_msgsTwist {
static readonly maxWireBytes:i32=48;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=48;
/** Exact native field linear, native scalar geometry_msgs/Vector3. */
linear:Ros1Geometry_msgsVector3=new Ros1Geometry_msgsVector3();
/** Exact native field angular, native scalar geometry_msgs/Vector3. */
angular:Ros1Geometry_msgsVector3=new Ros1Geometry_msgsVector3();
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
static readControl(r:Reader):Ros1Geometry_msgsTwist{const value=new Ros1Geometry_msgsTwist();
{
const decoded=Ros1Geometry_msgsVector3.readControl(r);
value.linear=decoded;
}
{
const decoded=Ros1Geometry_msgsVector3.readControl(r);
value.angular=decoded;
}
return value;}
}
export class Ros1Geometry_msgsTwistCodec implements VendorCodec<Ros1Geometry_msgsTwist>{
static readonly codecIdentity:string="UWVMROS1_d5a69cfff4380fcf3e9c131ddb3dfb6def041cd7308a7ebdbc150b6483dfd921";
static readonly maxWireBytes:i32=48;
encodeChecked(value:Ros1Geometry_msgsTwist):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsTwist):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsTwist>{const out=new VendorDecoded<Ros1Geometry_msgsTwist>();if(bytes.length>48){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsTwist.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsTwist{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsTwist():changetype<Ros1Geometry_msgsTwist>(out.value);}
}
/** Complete original geometry_msgs/TwistWithCovariance; spellings are published in generation proof. */
export class Ros1Geometry_msgsTwistWithCovariance {
static readonly maxWireBytes:i32=336;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=336;
/** Exact native field twist, native scalar geometry_msgs/Twist. */
twist:Ros1Geometry_msgsTwist=new Ros1Geometry_msgsTwist();
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
static readControl(r:Reader):Ros1Geometry_msgsTwistWithCovariance{const value=new Ros1Geometry_msgsTwistWithCovariance();
{
const decoded=Ros1Geometry_msgsTwist.readControl(r);
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
export class Ros1Geometry_msgsTwistWithCovarianceCodec implements VendorCodec<Ros1Geometry_msgsTwistWithCovariance>{
static readonly codecIdentity:string="UWVMROS1_ff6790d181503b424d84726c8f4a7852cef14c6dd93e6d32cf3c37023f74bcdf";
static readonly maxWireBytes:i32=336;
encodeChecked(value:Ros1Geometry_msgsTwistWithCovariance):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsTwistWithCovariance):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsTwistWithCovariance>{const out=new VendorDecoded<Ros1Geometry_msgsTwistWithCovariance>();if(bytes.length>336){out.status=90;return out;}if(bytes.length<336){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsTwistWithCovariance.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsTwistWithCovariance{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsTwistWithCovariance():changetype<Ros1Geometry_msgsTwistWithCovariance>(out.value);}
}
/** Complete original time; spellings are published in generation proof. */
export class Ros1Time {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field sec, native scalar uint32. */
sec:u32=0;
/** Exact native field nsec, native scalar uint32. */
nsec:u32=0;
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
if(this.nsec>=1000000000)return -22;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.sec,4);
w.integer(<u64>this.nsec,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Time{const value=new Ros1Time();
{
const decoded=<u32>r.integer(4);
value.sec=decoded;
}
{
const decoded=<u32>r.integer(4);
value.nsec=decoded;
}
if(value.nsec>=1000000000)r.valid=false;
return value;}
}
export class Ros1TimeCodec implements VendorCodec<Ros1Time>{
static readonly codecIdentity:string="UWVMROS1_90b75571d2bcc21b824eb993b485a001bf97e281adda7907e1b2e3357e4804b0";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros1Time):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Time):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Time>{const out=new VendorDecoded<Ros1Time>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Time.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Time{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Time():changetype<Ros1Time>(out.value);}
}
/** Complete original std_msgs/Header; spellings are published in generation proof. */
export class Ros1Std_msgsHeader {
static readonly maxWireBytes:i32=144;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=144;
/** Exact native field seq, native scalar uint32. */
seq:u32=0;
/** Exact native field stamp, native scalar time. */
stamp:Ros1Time=new Ros1Time();
/** Exact native field frame_id, native scalar string. */
frame_id:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>144-n)return -90;n+=4;
if(n>144)return -90;
}
{
const child=this.stamp.wireSize();if(child<0)return child;if(child>144-n)return -90;n+=child;
if(n>144)return -90;
}
{
if(!validUnicode(this.frame_id)||this.frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.frame_id);if(size>128)return -22;if(4+size>144-n)return -90;n+=4+size;
if(n>144)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.seq,4);
this.stamp.writeControl(w);
w.text(this.frame_id);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Std_msgsHeader{const value=new Ros1Std_msgsHeader();
{
const decoded=<u32>r.integer(4);
value.seq=decoded;
}
{
const decoded=Ros1Time.readControl(r);
value.stamp=decoded;
}
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.frame_id=decoded;
}
return value;}
}
export class Ros1Std_msgsHeaderCodec implements VendorCodec<Ros1Std_msgsHeader>{
static readonly codecIdentity:string="UWVMROS1_ee2b4e736d36ebcdb562d3ef7832777233d39d366837e9148a76aa2c36756435";
static readonly maxWireBytes:i32=144;
encodeChecked(value:Ros1Std_msgsHeader):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Std_msgsHeader):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Std_msgsHeader>{const out=new VendorDecoded<Ros1Std_msgsHeader>();if(bytes.length>144){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Std_msgsHeader.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Std_msgsHeader{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Std_msgsHeader():changetype<Ros1Std_msgsHeader>(out.value);}
}
/** Complete original nav_msgs/Odometry; spellings are published in generation proof. */
export class Ros1Nav_msgsOdometry {
static readonly maxWireBytes:i32=956;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=956;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field child_frame_id, native scalar string. */
child_frame_id:string="";
/** Exact native field pose, native scalar geometry_msgs/PoseWithCovariance. */
pose:Ros1Geometry_msgsPoseWithCovariance=new Ros1Geometry_msgsPoseWithCovariance();
/** Exact native field twist, native scalar geometry_msgs/TwistWithCovariance. */
twist:Ros1Geometry_msgsTwistWithCovariance=new Ros1Geometry_msgsTwistWithCovariance();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>956-n)return -90;n+=child;
if(n>956)return -90;
}
{
if(!validUnicode(this.child_frame_id)||this.child_frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.child_frame_id);if(size>128)return -22;if(4+size>956-n)return -90;n+=4+size;
if(n>956)return -90;
}
{
const child=this.pose.wireSize();if(child<0)return child;if(child>956-n)return -90;n+=child;
if(n>956)return -90;
}
{
const child=this.twist.wireSize();if(child<0)return child;if(child>956-n)return -90;n+=child;
if(n>956)return -90;
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
static readControl(r:Reader):Ros1Nav_msgsOdometry{const value=new Ros1Nav_msgsOdometry();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.child_frame_id=decoded;
}
{
const decoded=Ros1Geometry_msgsPoseWithCovariance.readControl(r);
value.pose=decoded;
}
{
const decoded=Ros1Geometry_msgsTwistWithCovariance.readControl(r);
value.twist=decoded;
}
return value;}
}
export class Ros1Nav_msgsOdometryCodec implements VendorCodec<Ros1Nav_msgsOdometry>{
static readonly codecIdentity:string="UWVMROS1_c10d3ce1f2824c33d728d0006fb83134330d0332af122f179cf2dac32181a2ca";
static readonly maxWireBytes:i32=956;
encodeChecked(value:Ros1Nav_msgsOdometry):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Nav_msgsOdometry):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Nav_msgsOdometry>{const out=new VendorDecoded<Ros1Nav_msgsOdometry>();if(bytes.length>956){out.status=90;return out;}if(bytes.length<700){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Nav_msgsOdometry.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Nav_msgsOdometry{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Nav_msgsOdometry():changetype<Ros1Nav_msgsOdometry>(out.value);}
}
/** Complete original scout_msgs/ScoutBmsStatus; spellings are published in generation proof. */
export class Ros1Scout_msgsScoutBmsStatus {
static readonly maxWireBytes:i32=44;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=44;
/** Exact native field SOC, native scalar float64. */
SOC:f64=0;
/** Exact native field SOH, native scalar float64. */
SOH:f64=0;
/** Exact native field battery_voltage, native scalar float64. */
battery_voltage:f64=0;
/** Exact native field battery_current, native scalar float64. */
battery_current:f64=0;
/** Exact native field battery_temperature, native scalar float64. */
battery_temperature:f64=0;
/** Exact native field Alarm_Status_1, native scalar uint8. */
Alarm_Status_1:u8=0;
/** Exact native field Alarm_Status_2, native scalar uint8. */
Alarm_Status_2:u8=0;
/** Exact native field Warning_Status_1, native scalar uint8. */
Warning_Status_1:u8=0;
/** Exact native field Warning_Status_2, native scalar uint8. */
Warning_Status_2:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.SOC))return -22;
if(8>44-n)return -90;n+=8;
if(n>44)return -90;
}
{
if(!isFinite(this.SOH))return -22;
if(8>44-n)return -90;n+=8;
if(n>44)return -90;
}
{
if(!isFinite(this.battery_voltage))return -22;
if(8>44-n)return -90;n+=8;
if(n>44)return -90;
}
{
if(!isFinite(this.battery_current))return -22;
if(8>44-n)return -90;n+=8;
if(n>44)return -90;
}
{
if(!isFinite(this.battery_temperature))return -22;
if(8>44-n)return -90;n+=8;
if(n>44)return -90;
}
{
if(1>44-n)return -90;n+=1;
if(n>44)return -90;
}
{
if(1>44-n)return -90;n+=1;
if(n>44)return -90;
}
{
if(1>44-n)return -90;n+=1;
if(n>44)return -90;
}
{
if(1>44-n)return -90;n+=1;
if(n>44)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.SOC),8);
w.integer(<u64>reinterpret<u64>(this.SOH),8);
w.integer(<u64>reinterpret<u64>(this.battery_voltage),8);
w.integer(<u64>reinterpret<u64>(this.battery_current),8);
w.integer(<u64>reinterpret<u64>(this.battery_temperature),8);
w.integer(<u64>this.Alarm_Status_1,1);
w.integer(<u64>this.Alarm_Status_2,1);
w.integer(<u64>this.Warning_Status_1,1);
w.integer(<u64>this.Warning_Status_2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Scout_msgsScoutBmsStatus{const value=new Ros1Scout_msgsScoutBmsStatus();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.SOC=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.SOH=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.battery_voltage=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.battery_current=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.battery_temperature=decoded;
}
{
const decoded=<u8>r.integer(1);
value.Alarm_Status_1=decoded;
}
{
const decoded=<u8>r.integer(1);
value.Alarm_Status_2=decoded;
}
{
const decoded=<u8>r.integer(1);
value.Warning_Status_1=decoded;
}
{
const decoded=<u8>r.integer(1);
value.Warning_Status_2=decoded;
}
return value;}
}
export class Ros1Scout_msgsScoutBmsStatusCodec implements VendorCodec<Ros1Scout_msgsScoutBmsStatus>{
static readonly codecIdentity:string="UWVMROS1_6921538c08e86ab5b9ecdbf974c6cb5b580488f4ed96ccbdb8b7824d51e9cf95";
static readonly maxWireBytes:i32=44;
encodeChecked(value:Ros1Scout_msgsScoutBmsStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Scout_msgsScoutBmsStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Scout_msgsScoutBmsStatus>{const out=new VendorDecoded<Ros1Scout_msgsScoutBmsStatus>();if(bytes.length>44){out.status=90;return out;}if(bytes.length<44){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Scout_msgsScoutBmsStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Scout_msgsScoutBmsStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Scout_msgsScoutBmsStatus():changetype<Ros1Scout_msgsScoutBmsStatus>(out.value);}
}
/** Complete original scout_msgs/ScoutDriverState; spellings are published in generation proof. */
export class Ros1Scout_msgsScoutDriverState {
static readonly maxWireBytes:i32=17;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=17;
/** Exact native field driver_voltage, native scalar float64. */
driver_voltage:f64=0;
/** Exact native field driver_temperature, native scalar float64. */
driver_temperature:f64=0;
/** Exact native field driver_state, native scalar uint8. */
driver_state:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.driver_voltage))return -22;
if(8>17-n)return -90;n+=8;
if(n>17)return -90;
}
{
if(!isFinite(this.driver_temperature))return -22;
if(8>17-n)return -90;n+=8;
if(n>17)return -90;
}
{
if(1>17-n)return -90;n+=1;
if(n>17)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.driver_voltage),8);
w.integer(<u64>reinterpret<u64>(this.driver_temperature),8);
w.integer(<u64>this.driver_state,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Scout_msgsScoutDriverState{const value=new Ros1Scout_msgsScoutDriverState();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.driver_voltage=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.driver_temperature=decoded;
}
{
const decoded=<u8>r.integer(1);
value.driver_state=decoded;
}
return value;}
}
export class Ros1Scout_msgsScoutDriverStateCodec implements VendorCodec<Ros1Scout_msgsScoutDriverState>{
static readonly codecIdentity:string="UWVMROS1_4c56c71ae621b2b8822d742894d19946492002453c93b72faccb4f2abc9d4275";
static readonly maxWireBytes:i32=17;
encodeChecked(value:Ros1Scout_msgsScoutDriverState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Scout_msgsScoutDriverState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Scout_msgsScoutDriverState>{const out=new VendorDecoded<Ros1Scout_msgsScoutDriverState>();if(bytes.length>17){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Scout_msgsScoutDriverState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Scout_msgsScoutDriverState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Scout_msgsScoutDriverState():changetype<Ros1Scout_msgsScoutDriverState>(out.value);}
}
/** Complete original scout_msgs/ScoutLightCmd; spellings are published in generation proof. */
export class Ros1Scout_msgsScoutLightCmd {
static readonly LIGHT_CONST_OFF:u8=0;
static readonly LIGHT_CONST_ON:u8=1;
static readonly LIGHT_BREATH:u8=2;
static readonly LIGHT_CUSTOM:u8=3;
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field enable_cmd_light_control, native scalar bool. */
enable_cmd_light_control:bool=false;
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
w.u8(this.enable_cmd_light_control?1:0);
w.integer(<u64>this.front_mode,1);
w.integer(<u64>this.front_custom_value,1);
w.integer(<u64>this.rear_mode,1);
w.integer(<u64>this.rear_custom_value,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Scout_msgsScoutLightCmd{const value=new Ros1Scout_msgsScoutLightCmd();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.enable_cmd_light_control=decoded;
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
export class Ros1Scout_msgsScoutLightCmdCodec implements VendorCodec<Ros1Scout_msgsScoutLightCmd>{
static readonly codecIdentity:string="UWVMROS1_cf08cccf454f88111d5cc4a4b1ace08de72cf1ae9a9400c23a148126da95942a";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros1Scout_msgsScoutLightCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Scout_msgsScoutLightCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Scout_msgsScoutLightCmd>{const out=new VendorDecoded<Ros1Scout_msgsScoutLightCmd>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Scout_msgsScoutLightCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Scout_msgsScoutLightCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Scout_msgsScoutLightCmd():changetype<Ros1Scout_msgsScoutLightCmd>(out.value);}
}
/** Complete original scout_msgs/ScoutLightState; spellings are published in generation proof. */
export class Ros1Scout_msgsScoutLightState {
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
static readControl(r:Reader):Ros1Scout_msgsScoutLightState{const value=new Ros1Scout_msgsScoutLightState();
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
export class Ros1Scout_msgsScoutLightStateCodec implements VendorCodec<Ros1Scout_msgsScoutLightState>{
static readonly codecIdentity:string="UWVMROS1_af55048f9200cfa80fc419664204a515f628cc610a352ab3ed8443dd7ac0ae51";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros1Scout_msgsScoutLightState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Scout_msgsScoutLightState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Scout_msgsScoutLightState>{const out=new VendorDecoded<Ros1Scout_msgsScoutLightState>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Scout_msgsScoutLightState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Scout_msgsScoutLightState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Scout_msgsScoutLightState():changetype<Ros1Scout_msgsScoutLightState>(out.value);}
}
/** Complete original scout_msgs/ScoutMotorState; spellings are published in generation proof. */
export class Ros1Scout_msgsScoutMotorState {
static readonly maxWireBytes:i32=32;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=32;
/** Exact native field current, native scalar float64. */
current:f64=0;
/** Exact native field rpm, native scalar float64. */
rpm:f64=0;
/** Exact native field temperature, native scalar float64. */
temperature:f64=0;
/** Exact native field motor_pose, native scalar float64. */
motor_pose:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.current))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.rpm))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.temperature))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.motor_pose))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.current),8);
w.integer(<u64>reinterpret<u64>(this.rpm),8);
w.integer(<u64>reinterpret<u64>(this.temperature),8);
w.integer(<u64>reinterpret<u64>(this.motor_pose),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Scout_msgsScoutMotorState{const value=new Ros1Scout_msgsScoutMotorState();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.current=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.rpm=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.temperature=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.motor_pose=decoded;
}
return value;}
}
export class Ros1Scout_msgsScoutMotorStateCodec implements VendorCodec<Ros1Scout_msgsScoutMotorState>{
static readonly codecIdentity:string="UWVMROS1_ca67a704d7bb4ba6b2839db1fdb8a75bc753868ec871d101c24c85d8202a79b7";
static readonly maxWireBytes:i32=32;
encodeChecked(value:Ros1Scout_msgsScoutMotorState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Scout_msgsScoutMotorState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Scout_msgsScoutMotorState>{const out=new VendorDecoded<Ros1Scout_msgsScoutMotorState>();if(bytes.length>32){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Scout_msgsScoutMotorState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Scout_msgsScoutMotorState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Scout_msgsScoutMotorState():changetype<Ros1Scout_msgsScoutMotorState>(out.value);}
}
/** Complete original scout_msgs/ScoutRsStatus; spellings are published in generation proof. */
export class Ros1Scout_msgsScoutRsStatus {
static readonly maxWireBytes:i32=153;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=153;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field stick_left_h, native scalar int8. */
stick_left_h:i8=0;
/** Exact native field stick_left_v, native scalar int8. */
stick_left_v:i8=0;
/** Exact native field stick_right_h, native scalar int8. */
stick_right_h:i8=0;
/** Exact native field stick_right_v, native scalar int8. */
stick_right_v:i8=0;
/** Exact native field swa, native scalar uint8. */
swa:u8=0;
/** Exact native field swb, native scalar uint8. */
swb:u8=0;
/** Exact native field swc, native scalar uint8. */
swc:u8=0;
/** Exact native field swd, native scalar uint8. */
swd:u8=0;
/** Exact native field var_a, native scalar uint8. */
var_a:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>153-n)return -90;n+=child;
if(n>153)return -90;
}
{
if(1>153-n)return -90;n+=1;
if(n>153)return -90;
}
{
if(1>153-n)return -90;n+=1;
if(n>153)return -90;
}
{
if(1>153-n)return -90;n+=1;
if(n>153)return -90;
}
{
if(1>153-n)return -90;n+=1;
if(n>153)return -90;
}
{
if(1>153-n)return -90;n+=1;
if(n>153)return -90;
}
{
if(1>153-n)return -90;n+=1;
if(n>153)return -90;
}
{
if(1>153-n)return -90;n+=1;
if(n>153)return -90;
}
{
if(1>153-n)return -90;n+=1;
if(n>153)return -90;
}
{
if(1>153-n)return -90;n+=1;
if(n>153)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>this.stick_left_h,1);
w.integer(<u64>this.stick_left_v,1);
w.integer(<u64>this.stick_right_h,1);
w.integer(<u64>this.stick_right_v,1);
w.integer(<u64>this.swa,1);
w.integer(<u64>this.swb,1);
w.integer(<u64>this.swc,1);
w.integer(<u64>this.swd,1);
w.integer(<u64>this.var_a,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Scout_msgsScoutRsStatus{const value=new Ros1Scout_msgsScoutRsStatus();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const decoded=<i8>r.integer(1);
value.stick_left_h=decoded;
}
{
const decoded=<i8>r.integer(1);
value.stick_left_v=decoded;
}
{
const decoded=<i8>r.integer(1);
value.stick_right_h=decoded;
}
{
const decoded=<i8>r.integer(1);
value.stick_right_v=decoded;
}
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
const decoded=<u8>r.integer(1);
value.var_a=decoded;
}
return value;}
}
export class Ros1Scout_msgsScoutRsStatusCodec implements VendorCodec<Ros1Scout_msgsScoutRsStatus>{
static readonly codecIdentity:string="UWVMROS1_deb068b75fb629df3724c141ed3be9f6ee8a4aac7727cdaa28769cb61211f7de";
static readonly maxWireBytes:i32=153;
encodeChecked(value:Ros1Scout_msgsScoutRsStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Scout_msgsScoutRsStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Scout_msgsScoutRsStatus>{const out=new VendorDecoded<Ros1Scout_msgsScoutRsStatus>();if(bytes.length>153){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Scout_msgsScoutRsStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Scout_msgsScoutRsStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Scout_msgsScoutRsStatus():changetype<Ros1Scout_msgsScoutRsStatus>(out.value);}
}
/** Complete original scout_msgs/ScoutStatus; spellings are published in generation proof. */
export class Ros1Scout_msgsScoutStatus {
static readonly MOTOR_ID_FRONT_RIGHT:i8=0;
static readonly MOTOR_ID_FRONT_LEFT:i8=1;
static readonly MOTOR_ID_REAR_RIGHT:i8=2;
static readonly MOTOR_ID_REAR_LEFT:i8=3;
static readonly LIGHT_ID_FRONT:i8=0;
static readonly LIGHT_ID_REAR:i8=1;
static readonly maxWireBytes:i32=381;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=381;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field linear_velocity, native scalar float64. */
linear_velocity:f64=0;
/** Exact native field angular_velocity, native scalar float64. */
angular_velocity:f64=0;
/** Exact native field lateral_velocity, native scalar float64. */
lateral_velocity:f64=0;
/** Exact native field base_state, native scalar uint8. */
base_state:u8=0;
/** Exact native field control_mode, native scalar uint8. */
control_mode:u8=0;
/** Exact native field fault_code, native scalar uint16. */
fault_code:u16=0;
/** Exact native field battery_voltage, native scalar float64. */
battery_voltage:f64=0;
/** Exact native field motor_states, native scalar scout_msgs/ScoutMotorState. */
motor_states:Array<Ros1Scout_msgsScoutMotorState>=new Array<Ros1Scout_msgsScoutMotorState>();
/** Exact native field driver_states, native scalar scout_msgs/ScoutDriverState. */
driver_states:Array<Ros1Scout_msgsScoutDriverState>=new Array<Ros1Scout_msgsScoutDriverState>();
/** Exact native field light_control_enabled, native scalar bool. */
light_control_enabled:bool=false;
/** Exact native field front_light_state, native scalar scout_msgs/ScoutLightState. */
front_light_state:Ros1Scout_msgsScoutLightState=new Ros1Scout_msgsScoutLightState();
/** Exact native field rear_light_state, native scalar scout_msgs/ScoutLightState. */
rear_light_state:Ros1Scout_msgsScoutLightState=new Ros1Scout_msgsScoutLightState();
constructor(){
for(let i=0;i<4;++i)this.motor_states.push(new Ros1Scout_msgsScoutMotorState());
for(let i=0;i<4;++i)this.driver_states.push(new Ros1Scout_msgsScoutDriverState());
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>381-n)return -90;n+=child;
if(n>381)return -90;
}
{
if(!isFinite(this.linear_velocity))return -22;
if(8>381-n)return -90;n+=8;
if(n>381)return -90;
}
{
if(!isFinite(this.angular_velocity))return -22;
if(8>381-n)return -90;n+=8;
if(n>381)return -90;
}
{
if(!isFinite(this.lateral_velocity))return -22;
if(8>381-n)return -90;n+=8;
if(n>381)return -90;
}
{
if(1>381-n)return -90;n+=1;
if(n>381)return -90;
}
{
if(1>381-n)return -90;n+=1;
if(n>381)return -90;
}
{
if(2>381-n)return -90;n+=2;
if(n>381)return -90;
}
{
if(!isFinite(this.battery_voltage))return -22;
if(8>381-n)return -90;n+=8;
if(n>381)return -90;
}
{
if(this.motor_states.length!=4)return -22;
for(let i=0;i<this.motor_states.length;++i){
const child=this.motor_states[i].wireSize();if(child<0)return child;if(child>381-n)return -90;n+=child;
if(n>381)return -90;
}
}
{
if(this.driver_states.length!=4)return -22;
for(let i=0;i<this.driver_states.length;++i){
const child=this.driver_states[i].wireSize();if(child<0)return child;if(child>381-n)return -90;n+=child;
if(n>381)return -90;
}
}
{
if(1>381-n)return -90;n+=1;
if(n>381)return -90;
}
{
const child=this.front_light_state.wireSize();if(child<0)return child;if(child>381-n)return -90;n+=child;
if(n>381)return -90;
}
{
const child=this.rear_light_state.wireSize();if(child<0)return child;if(child>381-n)return -90;n+=child;
if(n>381)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>reinterpret<u64>(this.linear_velocity),8);
w.integer(<u64>reinterpret<u64>(this.angular_velocity),8);
w.integer(<u64>reinterpret<u64>(this.lateral_velocity),8);
w.integer(<u64>this.base_state,1);
w.integer(<u64>this.control_mode,1);
w.integer(<u64>this.fault_code,2);
w.integer(<u64>reinterpret<u64>(this.battery_voltage),8);
for(let i=0;i<this.motor_states.length;++i){
this.motor_states[i].writeControl(w);
}
for(let i=0;i<this.driver_states.length;++i){
this.driver_states[i].writeControl(w);
}
w.u8(this.light_control_enabled?1:0);
this.front_light_state.writeControl(w);
this.rear_light_state.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Scout_msgsScoutStatus{const value=new Ros1Scout_msgsScoutStatus();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
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
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.lateral_velocity=decoded;
}
{
const decoded=<u8>r.integer(1);
value.base_state=decoded;
}
{
const decoded=<u8>r.integer(1);
value.control_mode=decoded;
}
{
const decoded=<u16>r.integer(2);
value.fault_code=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.battery_voltage=decoded;
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.motor_states=new Array<Ros1Scout_msgsScoutMotorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Scout_msgsScoutMotorState.readControl(r);
value.motor_states.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.driver_states=new Array<Ros1Scout_msgsScoutDriverState>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Scout_msgsScoutDriverState.readControl(r);
value.driver_states.push(decoded);
}
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.light_control_enabled=decoded;
}
{
const decoded=Ros1Scout_msgsScoutLightState.readControl(r);
value.front_light_state=decoded;
}
{
const decoded=Ros1Scout_msgsScoutLightState.readControl(r);
value.rear_light_state=decoded;
}
return value;}
}
export class Ros1Scout_msgsScoutStatusCodec implements VendorCodec<Ros1Scout_msgsScoutStatus>{
static readonly codecIdentity:string="UWVMROS1_ff62854d4de0aae3dfbbc312822c6f912294f0feb04a7fff943df602d40bcbe2";
static readonly maxWireBytes:i32=381;
encodeChecked(value:Ros1Scout_msgsScoutStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Scout_msgsScoutStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Scout_msgsScoutStatus>{const out=new VendorDecoded<Ros1Scout_msgsScoutStatus>();if(bytes.length>381){out.status=90;return out;}if(bytes.length<253){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Scout_msgsScoutStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Scout_msgsScoutStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Scout_msgsScoutStatus():changetype<Ros1Scout_msgsScoutStatus>(out.value);}
}
