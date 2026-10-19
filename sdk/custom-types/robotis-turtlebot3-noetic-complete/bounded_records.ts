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
/** Complete original geometry_msgs/Transform; spellings are published in generation proof. */
export class Ros1Geometry_msgsTransform {
static readonly maxWireBytes:i32=56;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=56;
/** Exact native field translation, native scalar geometry_msgs/Vector3. */
translation:Ros1Geometry_msgsVector3=new Ros1Geometry_msgsVector3();
/** Exact native field rotation, native scalar geometry_msgs/Quaternion. */
rotation:Ros1Geometry_msgsQuaternion=new Ros1Geometry_msgsQuaternion();
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
static readControl(r:Reader):Ros1Geometry_msgsTransform{const value=new Ros1Geometry_msgsTransform();
{
const decoded=Ros1Geometry_msgsVector3.readControl(r);
value.translation=decoded;
}
{
const decoded=Ros1Geometry_msgsQuaternion.readControl(r);
value.rotation=decoded;
}
return value;}
}
export class Ros1Geometry_msgsTransformCodec implements VendorCodec<Ros1Geometry_msgsTransform>{
static readonly codecIdentity:string="UWVMROS1_61aabab03e8f766ea05949c9e5f4989186d72a26f793b8d11752e53f1f300085";
static readonly maxWireBytes:i32=56;
encodeChecked(value:Ros1Geometry_msgsTransform):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsTransform):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsTransform>{const out=new VendorDecoded<Ros1Geometry_msgsTransform>();if(bytes.length>56){out.status=90;return out;}if(bytes.length<56){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsTransform.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsTransform{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsTransform():changetype<Ros1Geometry_msgsTransform>(out.value);}
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
/** Complete original geometry_msgs/TransformStamped; spellings are published in generation proof. */
export class Ros1Geometry_msgsTransformStamped {
static readonly maxWireBytes:i32=460;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=460;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field child_frame_id, native scalar string. */
child_frame_id:string="";
/** Exact native field transform, native scalar geometry_msgs/Transform. */
transform:Ros1Geometry_msgsTransform=new Ros1Geometry_msgsTransform();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>460-n)return -90;n+=child;
if(n>460)return -90;
}
{
if(!validUnicode(this.child_frame_id)||this.child_frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.child_frame_id);if(size>256)return -22;if(4+size>460-n)return -90;n+=4+size;
if(n>460)return -90;
}
{
const child=this.transform.wireSize();if(child<0)return child;if(child>460-n)return -90;n+=child;
if(n>460)return -90;
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
static readControl(r:Reader):Ros1Geometry_msgsTransformStamped{const value=new Ros1Geometry_msgsTransformStamped();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.child_frame_id=decoded;
}
{
const decoded=Ros1Geometry_msgsTransform.readControl(r);
value.transform=decoded;
}
return value;}
}
export class Ros1Geometry_msgsTransformStampedCodec implements VendorCodec<Ros1Geometry_msgsTransformStamped>{
static readonly codecIdentity:string="UWVMROS1_9237b5dc5d3beacf4310244ce746a4210b9864855b611d1bf54cf23c28d7b79a";
static readonly maxWireBytes:i32=460;
encodeChecked(value:Ros1Geometry_msgsTransformStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsTransformStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsTransformStamped>{const out=new VendorDecoded<Ros1Geometry_msgsTransformStamped>();if(bytes.length>460){out.status=90;return out;}if(bytes.length<76){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsTransformStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsTransformStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsTransformStamped():changetype<Ros1Geometry_msgsTransformStamped>(out.value);}
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
/** Complete original geometry_msgs/TwistStamped; spellings are published in generation proof. */
export class Ros1Geometry_msgsTwistStamped {
static readonly maxWireBytes:i32=192;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=192;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field twist, native scalar geometry_msgs/Twist. */
twist:Ros1Geometry_msgsTwist=new Ros1Geometry_msgsTwist();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>192-n)return -90;n+=child;
if(n>192)return -90;
}
{
const child=this.twist.wireSize();if(child<0)return child;if(child>192-n)return -90;n+=child;
if(n>192)return -90;
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
static readControl(r:Reader):Ros1Geometry_msgsTwistStamped{const value=new Ros1Geometry_msgsTwistStamped();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros1Geometry_msgsTwist.readControl(r);
value.twist=decoded;
}
return value;}
}
export class Ros1Geometry_msgsTwistStampedCodec implements VendorCodec<Ros1Geometry_msgsTwistStamped>{
static readonly codecIdentity:string="UWVMROS1_27a31bcca6a50fbc6c6259b53d229872336a75628d6afd9de174eee2481946c7";
static readonly maxWireBytes:i32=192;
encodeChecked(value:Ros1Geometry_msgsTwistStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsTwistStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsTwistStamped>{const out=new VendorDecoded<Ros1Geometry_msgsTwistStamped>();if(bytes.length>192){out.status=90;return out;}if(bytes.length<64){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsTwistStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsTwistStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsTwistStamped():changetype<Ros1Geometry_msgsTwistStamped>(out.value);}
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
/** Complete original nav_msgs/Odometry; spellings are published in generation proof. */
export class Ros1Nav_msgsOdometry {
static readonly maxWireBytes:i32=1084;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1084;
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
const child=this.header.wireSize();if(child<0)return child;if(child>1084-n)return -90;n+=child;
if(n>1084)return -90;
}
{
if(!validUnicode(this.child_frame_id)||this.child_frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.child_frame_id);if(size>256)return -22;if(4+size>1084-n)return -90;n+=4+size;
if(n>1084)return -90;
}
{
const child=this.pose.wireSize();if(child<0)return child;if(child>1084-n)return -90;n+=child;
if(n>1084)return -90;
}
{
const child=this.twist.wireSize();if(child<0)return child;if(child>1084-n)return -90;n+=child;
if(n>1084)return -90;
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
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
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
static readonly codecIdentity:string="UWVMROS1_e0ff5eb7aef868891846ac7618249b12c18fcf0969653f24fdeaa7644e636ca0";
static readonly maxWireBytes:i32=1084;
encodeChecked(value:Ros1Nav_msgsOdometry):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Nav_msgsOdometry):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Nav_msgsOdometry>{const out=new VendorDecoded<Ros1Nav_msgsOdometry>();if(bytes.length>1084){out.status=90;return out;}if(bytes.length<700){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Nav_msgsOdometry.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Nav_msgsOdometry{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Nav_msgsOdometry():changetype<Ros1Nav_msgsOdometry>(out.value);}
}
/** Complete original sensor_msgs/BatteryState; spellings are published in generation proof. */
export class Ros1Sensor_msgsBatteryState {
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
static readonly maxWireBytes:i32=832;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=832;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
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
const child=this.header.wireSize();if(child<0)return child;if(child>832-n)return -90;n+=child;
if(n>832)return -90;
}
{
if(!isFinite(this.voltage))return -22;
if(4>832-n)return -90;n+=4;
if(n>832)return -90;
}
{
if(!isFinite(this.temperature))return -22;
if(4>832-n)return -90;n+=4;
if(n>832)return -90;
}
{
if(!isFinite(this.current))return -22;
if(4>832-n)return -90;n+=4;
if(n>832)return -90;
}
{
if(!isFinite(this.charge))return -22;
if(4>832-n)return -90;n+=4;
if(n>832)return -90;
}
{
if(!isFinite(this.capacity))return -22;
if(4>832-n)return -90;n+=4;
if(n>832)return -90;
}
{
if(!isFinite(this.design_capacity))return -22;
if(4>832-n)return -90;n+=4;
if(n>832)return -90;
}
{
if(!isFinite(this.percentage))return -22;
if(4>832-n)return -90;n+=4;
if(n>832)return -90;
}
{
if(1>832-n)return -90;n+=1;
if(n>832)return -90;
}
{
if(1>832-n)return -90;n+=1;
if(n>832)return -90;
}
{
if(1>832-n)return -90;n+=1;
if(n>832)return -90;
}
{
if(1>832-n)return -90;n+=1;
if(n>832)return -90;
}
{
if(this.cell_voltage.length>16)return -22;
if(4>832-n)return -90;n+=4;
for(let i=0;i<this.cell_voltage.length;++i){
if(!isFinite(this.cell_voltage[i]))return -22;
if(4>832-n)return -90;n+=4;
if(n>832)return -90;
}
}
{
if(this.cell_temperature.length>16)return -22;
if(4>832-n)return -90;n+=4;
for(let i=0;i<this.cell_temperature.length;++i){
if(!isFinite(this.cell_temperature[i]))return -22;
if(4>832-n)return -90;n+=4;
if(n>832)return -90;
}
}
{
if(!validUnicode(this.location)||this.location.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.location);if(size>256)return -22;if(4+size>832-n)return -90;n+=4+size;
if(n>832)return -90;
}
{
if(!validUnicode(this.serial_number)||this.serial_number.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.serial_number);if(size>256)return -22;if(4+size>832-n)return -90;n+=4+size;
if(n>832)return -90;
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
static readControl(r:Reader):Ros1Sensor_msgsBatteryState{const value=new Ros1Sensor_msgsBatteryState();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
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
export class Ros1Sensor_msgsBatteryStateCodec implements VendorCodec<Ros1Sensor_msgsBatteryState>{
static readonly codecIdentity:string="UWVMROS1_4b176902362348cd3b7eec6ae21e2035d33df6990b4682ddf5123ef6e0339441";
static readonly maxWireBytes:i32=832;
encodeChecked(value:Ros1Sensor_msgsBatteryState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Sensor_msgsBatteryState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Sensor_msgsBatteryState>{const out=new VendorDecoded<Ros1Sensor_msgsBatteryState>();if(bytes.length>832){out.status=90;return out;}if(bytes.length<64){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Sensor_msgsBatteryState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Sensor_msgsBatteryState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Sensor_msgsBatteryState():changetype<Ros1Sensor_msgsBatteryState>(out.value);}
}
/** Complete original sensor_msgs/Imu; spellings are published in generation proof. */
export class Ros1Sensor_msgsImu {
static readonly maxWireBytes:i32=440;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=440;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field orientation, native scalar geometry_msgs/Quaternion. */
orientation:Ros1Geometry_msgsQuaternion=new Ros1Geometry_msgsQuaternion();
/** Exact native field orientation_covariance, native scalar float64. */
orientation_covariance:Array<f64>=new Array<f64>();
/** Exact native field angular_velocity, native scalar geometry_msgs/Vector3. */
angular_velocity:Ros1Geometry_msgsVector3=new Ros1Geometry_msgsVector3();
/** Exact native field angular_velocity_covariance, native scalar float64. */
angular_velocity_covariance:Array<f64>=new Array<f64>();
/** Exact native field linear_acceleration, native scalar geometry_msgs/Vector3. */
linear_acceleration:Ros1Geometry_msgsVector3=new Ros1Geometry_msgsVector3();
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
const child=this.header.wireSize();if(child<0)return child;if(child>440-n)return -90;n+=child;
if(n>440)return -90;
}
{
const child=this.orientation.wireSize();if(child<0)return child;if(child>440-n)return -90;n+=child;
if(n>440)return -90;
}
{
if(this.orientation_covariance.length!=9)return -22;
for(let i=0;i<this.orientation_covariance.length;++i){
if(!isFinite(this.orientation_covariance[i]))return -22;
if(8>440-n)return -90;n+=8;
if(n>440)return -90;
}
}
{
const child=this.angular_velocity.wireSize();if(child<0)return child;if(child>440-n)return -90;n+=child;
if(n>440)return -90;
}
{
if(this.angular_velocity_covariance.length!=9)return -22;
for(let i=0;i<this.angular_velocity_covariance.length;++i){
if(!isFinite(this.angular_velocity_covariance[i]))return -22;
if(8>440-n)return -90;n+=8;
if(n>440)return -90;
}
}
{
const child=this.linear_acceleration.wireSize();if(child<0)return child;if(child>440-n)return -90;n+=child;
if(n>440)return -90;
}
{
if(this.linear_acceleration_covariance.length!=9)return -22;
for(let i=0;i<this.linear_acceleration_covariance.length;++i){
if(!isFinite(this.linear_acceleration_covariance[i]))return -22;
if(8>440-n)return -90;n+=8;
if(n>440)return -90;
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
static readControl(r:Reader):Ros1Sensor_msgsImu{const value=new Ros1Sensor_msgsImu();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros1Geometry_msgsQuaternion.readControl(r);
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
const decoded=Ros1Geometry_msgsVector3.readControl(r);
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
const decoded=Ros1Geometry_msgsVector3.readControl(r);
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
export class Ros1Sensor_msgsImuCodec implements VendorCodec<Ros1Sensor_msgsImu>{
static readonly codecIdentity:string="UWVMROS1_de9c3f5bb37e1dc20be73cb44f614a9ff8645c4cfbc305dc18425d8d9f130bc3";
static readonly maxWireBytes:i32=440;
encodeChecked(value:Ros1Sensor_msgsImu):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Sensor_msgsImu):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Sensor_msgsImu>{const out=new VendorDecoded<Ros1Sensor_msgsImu>();if(bytes.length>440){out.status=90;return out;}if(bytes.length<312){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Sensor_msgsImu.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Sensor_msgsImu{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Sensor_msgsImu():changetype<Ros1Sensor_msgsImu>(out.value);}
}
/** Complete original sensor_msgs/JointState; spellings are published in generation proof. */
export class Ros1Sensor_msgsJointState {
static readonly maxWireBytes:i32=472;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=472;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
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
const child=this.header.wireSize();if(child<0)return child;if(child>472-n)return -90;n+=child;
if(n>472)return -90;
}
{
if(this.name.length>2)return -22;
if(4>472-n)return -90;n+=4;
for(let i=0;i<this.name.length;++i){
if(!validUnicode(this.name[i])||this.name[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name[i]);if(size>128)return -22;if(4+size>472-n)return -90;n+=4+size;
if(n>472)return -90;
}
}
{
if(this.position.length>2)return -22;
if(4>472-n)return -90;n+=4;
for(let i=0;i<this.position.length;++i){
if(!isFinite(this.position[i]))return -22;
if(8>472-n)return -90;n+=8;
if(n>472)return -90;
}
}
{
if(this.velocity.length>2)return -22;
if(4>472-n)return -90;n+=4;
for(let i=0;i<this.velocity.length;++i){
if(!isFinite(this.velocity[i]))return -22;
if(8>472-n)return -90;n+=8;
if(n>472)return -90;
}
}
{
if(this.effort.length>2)return -22;
if(4>472-n)return -90;n+=4;
for(let i=0;i<this.effort.length;++i){
if(!isFinite(this.effort[i]))return -22;
if(8>472-n)return -90;n+=8;
if(n>472)return -90;
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
static readControl(r:Reader):Ros1Sensor_msgsJointState{const value=new Ros1Sensor_msgsJointState();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
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
export class Ros1Sensor_msgsJointStateCodec implements VendorCodec<Ros1Sensor_msgsJointState>{
static readonly codecIdentity:string="UWVMROS1_f27093d33cca1405ec35a9e525a95aa4002421a579cb78c6da3be3719f3cb66e";
static readonly maxWireBytes:i32=472;
encodeChecked(value:Ros1Sensor_msgsJointState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Sensor_msgsJointState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Sensor_msgsJointState>{const out=new VendorDecoded<Ros1Sensor_msgsJointState>();if(bytes.length>472){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Sensor_msgsJointState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Sensor_msgsJointState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Sensor_msgsJointState():changetype<Ros1Sensor_msgsJointState>(out.value);}
}
/** Complete original sensor_msgs/LaserScan; spellings are published in generation proof. */
export class Ros1Sensor_msgsLaserScan {
static readonly maxWireBytes:i32=8372;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8372;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
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
const child=this.header.wireSize();if(child<0)return child;if(child>8372-n)return -90;n+=child;
if(n>8372)return -90;
}
{
if(!isFinite(this.angle_min))return -22;
if(4>8372-n)return -90;n+=4;
if(n>8372)return -90;
}
{
if(!isFinite(this.angle_max))return -22;
if(4>8372-n)return -90;n+=4;
if(n>8372)return -90;
}
{
if(!isFinite(this.angle_increment))return -22;
if(4>8372-n)return -90;n+=4;
if(n>8372)return -90;
}
{
if(!isFinite(this.time_increment))return -22;
if(4>8372-n)return -90;n+=4;
if(n>8372)return -90;
}
{
if(!isFinite(this.scan_time))return -22;
if(4>8372-n)return -90;n+=4;
if(n>8372)return -90;
}
{
if(!isFinite(this.range_min))return -22;
if(4>8372-n)return -90;n+=4;
if(n>8372)return -90;
}
{
if(!isFinite(this.range_max))return -22;
if(4>8372-n)return -90;n+=4;
if(n>8372)return -90;
}
{
if(this.ranges.length>1024)return -22;
if(4>8372-n)return -90;n+=4;
for(let i=0;i<this.ranges.length;++i){
if(!isFinite(this.ranges[i]))return -22;
if(4>8372-n)return -90;n+=4;
if(n>8372)return -90;
}
}
{
if(this.intensities.length>1024)return -22;
if(4>8372-n)return -90;n+=4;
for(let i=0;i<this.intensities.length;++i){
if(!isFinite(this.intensities[i]))return -22;
if(4>8372-n)return -90;n+=4;
if(n>8372)return -90;
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
static readControl(r:Reader):Ros1Sensor_msgsLaserScan{const value=new Ros1Sensor_msgsLaserScan();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
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
export class Ros1Sensor_msgsLaserScanCodec implements VendorCodec<Ros1Sensor_msgsLaserScan>{
static readonly codecIdentity:string="UWVMROS1_f7aee00335bf7d165004957149192f58abe5a27b5c4884bdf8d1ae9be06b366b";
static readonly maxWireBytes:i32=8372;
encodeChecked(value:Ros1Sensor_msgsLaserScan):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Sensor_msgsLaserScan):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Sensor_msgsLaserScan>{const out=new VendorDecoded<Ros1Sensor_msgsLaserScan>();if(bytes.length>8372){out.status=90;return out;}if(bytes.length<52){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Sensor_msgsLaserScan.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Sensor_msgsLaserScan{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Sensor_msgsLaserScan():changetype<Ros1Sensor_msgsLaserScan>(out.value);}
}
/** Complete original sensor_msgs/MagneticField; spellings are published in generation proof. */
export class Ros1Sensor_msgsMagneticField {
static readonly maxWireBytes:i32=240;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=240;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field magnetic_field, native scalar geometry_msgs/Vector3. */
magnetic_field:Ros1Geometry_msgsVector3=new Ros1Geometry_msgsVector3();
/** Exact native field magnetic_field_covariance, native scalar float64. */
magnetic_field_covariance:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<9;++i)this.magnetic_field_covariance.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>240-n)return -90;n+=child;
if(n>240)return -90;
}
{
const child=this.magnetic_field.wireSize();if(child<0)return child;if(child>240-n)return -90;n+=child;
if(n>240)return -90;
}
{
if(this.magnetic_field_covariance.length!=9)return -22;
for(let i=0;i<this.magnetic_field_covariance.length;++i){
if(!isFinite(this.magnetic_field_covariance[i]))return -22;
if(8>240-n)return -90;n+=8;
if(n>240)return -90;
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
static readControl(r:Reader):Ros1Sensor_msgsMagneticField{const value=new Ros1Sensor_msgsMagneticField();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros1Geometry_msgsVector3.readControl(r);
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
export class Ros1Sensor_msgsMagneticFieldCodec implements VendorCodec<Ros1Sensor_msgsMagneticField>{
static readonly codecIdentity:string="UWVMROS1_1817b7ce8903797288ccf293373f81890c75af59d03d948339943a531ab754fe";
static readonly maxWireBytes:i32=240;
encodeChecked(value:Ros1Sensor_msgsMagneticField):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Sensor_msgsMagneticField):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Sensor_msgsMagneticField>{const out=new VendorDecoded<Ros1Sensor_msgsMagneticField>();if(bytes.length>240){out.status=90;return out;}if(bytes.length<112){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Sensor_msgsMagneticField.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Sensor_msgsMagneticField{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Sensor_msgsMagneticField():changetype<Ros1Sensor_msgsMagneticField>(out.value);}
}
/** Complete original std_srvs/SetBoolRequest; spellings are published in generation proof. */
export class Ros1Std_srvsSetBoolRequest {
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
static readControl(r:Reader):Ros1Std_srvsSetBoolRequest{const value=new Ros1Std_srvsSetBoolRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.data=decoded;
}
return value;}
}
export class Ros1Std_srvsSetBoolRequestCodec implements VendorCodec<Ros1Std_srvsSetBoolRequest>{
static readonly codecIdentity:string="UWVMROS1_1621dbb16e2455a199cc871a0e784642a73bc57e76a4d524778163fab97f6aac";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Std_srvsSetBoolRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Std_srvsSetBoolRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Std_srvsSetBoolRequest>{const out=new VendorDecoded<Ros1Std_srvsSetBoolRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Std_srvsSetBoolRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Std_srvsSetBoolRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Std_srvsSetBoolRequest():changetype<Ros1Std_srvsSetBoolRequest>(out.value);}
}
/** Complete original std_srvs/SetBoolResponse; spellings are published in generation proof. */
export class Ros1Std_srvsSetBoolResponse {
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
static readControl(r:Reader):Ros1Std_srvsSetBoolResponse{const value=new Ros1Std_srvsSetBoolResponse();
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
export class Ros1Std_srvsSetBoolResponseCodec implements VendorCodec<Ros1Std_srvsSetBoolResponse>{
static readonly codecIdentity:string="UWVMROS1_1e12b91618f540070e34e170e12db7659f8686cdf5fb8e1bf098b8d483522cd6";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Std_srvsSetBoolResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Std_srvsSetBoolResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Std_srvsSetBoolResponse>{const out=new VendorDecoded<Ros1Std_srvsSetBoolResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Std_srvsSetBoolResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Std_srvsSetBoolResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Std_srvsSetBoolResponse():changetype<Ros1Std_srvsSetBoolResponse>(out.value);}
}
/** Complete original std_srvs/TriggerRequest; spellings are published in generation proof. */
export class Ros1Std_srvsTriggerRequest {
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
static readControl(r:Reader):Ros1Std_srvsTriggerRequest{const value=new Ros1Std_srvsTriggerRequest();
return value;}
}
export class Ros1Std_srvsTriggerRequestCodec implements VendorCodec<Ros1Std_srvsTriggerRequest>{
static readonly codecIdentity:string="UWVMROS1_d4e98dca9dc9a031d6b0f51937613f4f29c36156f0f05475339149794d47db1e";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Std_srvsTriggerRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Std_srvsTriggerRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Std_srvsTriggerRequest>{const out=new VendorDecoded<Ros1Std_srvsTriggerRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Std_srvsTriggerRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Std_srvsTriggerRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Std_srvsTriggerRequest():changetype<Ros1Std_srvsTriggerRequest>(out.value);}
}
/** Complete original std_srvs/TriggerResponse; spellings are published in generation proof. */
export class Ros1Std_srvsTriggerResponse {
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
static readControl(r:Reader):Ros1Std_srvsTriggerResponse{const value=new Ros1Std_srvsTriggerResponse();
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
export class Ros1Std_srvsTriggerResponseCodec implements VendorCodec<Ros1Std_srvsTriggerResponse>{
static readonly codecIdentity:string="UWVMROS1_e625281ddea64aff38d8647e2673b418b77474cae0501500b280cabb3d247d1d";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Std_srvsTriggerResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Std_srvsTriggerResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Std_srvsTriggerResponse>{const out=new VendorDecoded<Ros1Std_srvsTriggerResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Std_srvsTriggerResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Std_srvsTriggerResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Std_srvsTriggerResponse():changetype<Ros1Std_srvsTriggerResponse>(out.value);}
}
/** Complete original turtlebot3_example/Turtlebot3Goal; spellings are published in generation proof. */
export class Ros1Turtlebot3_exampleTurtlebot3Goal {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field goal, native scalar geometry_msgs/Vector3. */
goal:Ros1Geometry_msgsVector3=new Ros1Geometry_msgsVector3();
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
static readControl(r:Reader):Ros1Turtlebot3_exampleTurtlebot3Goal{const value=new Ros1Turtlebot3_exampleTurtlebot3Goal();
{
const decoded=Ros1Geometry_msgsVector3.readControl(r);
value.goal=decoded;
}
return value;}
}
export class Ros1Turtlebot3_exampleTurtlebot3GoalCodec implements VendorCodec<Ros1Turtlebot3_exampleTurtlebot3Goal>{
static readonly codecIdentity:string="UWVMROS1_f6eb642301e239005c1790cfd5b9ff00388d98d27a17cbb0edb103b6ac7bfad5";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Turtlebot3_exampleTurtlebot3Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Turtlebot3_exampleTurtlebot3Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Turtlebot3_exampleTurtlebot3Goal>{const out=new VendorDecoded<Ros1Turtlebot3_exampleTurtlebot3Goal>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Turtlebot3_exampleTurtlebot3Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Turtlebot3_exampleTurtlebot3Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Turtlebot3_exampleTurtlebot3Goal():changetype<Ros1Turtlebot3_exampleTurtlebot3Goal>(out.value);}
}
/** Complete original turtlebot3_example/Turtlebot3Result; spellings are published in generation proof. */
export class Ros1Turtlebot3_exampleTurtlebot3Result {
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
static readControl(r:Reader):Ros1Turtlebot3_exampleTurtlebot3Result{const value=new Ros1Turtlebot3_exampleTurtlebot3Result();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.result=decoded;
}
return value;}
}
export class Ros1Turtlebot3_exampleTurtlebot3ResultCodec implements VendorCodec<Ros1Turtlebot3_exampleTurtlebot3Result>{
static readonly codecIdentity:string="UWVMROS1_c749a56ed9a0c74d1b167f69cc9b4435f9549d49144ef2951a4c3db501bad528";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros1Turtlebot3_exampleTurtlebot3Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Turtlebot3_exampleTurtlebot3Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Turtlebot3_exampleTurtlebot3Result>{const out=new VendorDecoded<Ros1Turtlebot3_exampleTurtlebot3Result>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Turtlebot3_exampleTurtlebot3Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Turtlebot3_exampleTurtlebot3Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Turtlebot3_exampleTurtlebot3Result():changetype<Ros1Turtlebot3_exampleTurtlebot3Result>(out.value);}
}
/** Complete original turtlebot3_example/Turtlebot3Feedback; spellings are published in generation proof. */
export class Ros1Turtlebot3_exampleTurtlebot3Feedback {
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
static readControl(r:Reader):Ros1Turtlebot3_exampleTurtlebot3Feedback{const value=new Ros1Turtlebot3_exampleTurtlebot3Feedback();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.state=decoded;
}
return value;}
}
export class Ros1Turtlebot3_exampleTurtlebot3FeedbackCodec implements VendorCodec<Ros1Turtlebot3_exampleTurtlebot3Feedback>{
static readonly codecIdentity:string="UWVMROS1_077b5593a1e4c23c89c4d6a87dc1d67d8fa1bf04fa3aa440f54c8eda60b688f8";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros1Turtlebot3_exampleTurtlebot3Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Turtlebot3_exampleTurtlebot3Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Turtlebot3_exampleTurtlebot3Feedback>{const out=new VendorDecoded<Ros1Turtlebot3_exampleTurtlebot3Feedback>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Turtlebot3_exampleTurtlebot3Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Turtlebot3_exampleTurtlebot3Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Turtlebot3_exampleTurtlebot3Feedback():changetype<Ros1Turtlebot3_exampleTurtlebot3Feedback>(out.value);}
}
/** Complete original turtlebot3_msgs/SensorState; spellings are published in generation proof. */
export class Ros1Turtlebot3_msgsSensorState {
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
static readonly maxWireBytes:i32=172;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=172;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
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
const child=this.header.wireSize();if(child<0)return child;if(child>172-n)return -90;n+=child;
if(n>172)return -90;
}
{
if(1>172-n)return -90;n+=1;
if(n>172)return -90;
}
{
if(!isFinite(this.cliff))return -22;
if(4>172-n)return -90;n+=4;
if(n>172)return -90;
}
{
if(!isFinite(this.sonar))return -22;
if(4>172-n)return -90;n+=4;
if(n>172)return -90;
}
{
if(!isFinite(this.illumination))return -22;
if(4>172-n)return -90;n+=4;
if(n>172)return -90;
}
{
if(1>172-n)return -90;n+=1;
if(n>172)return -90;
}
{
if(1>172-n)return -90;n+=1;
if(n>172)return -90;
}
{
if(1>172-n)return -90;n+=1;
if(n>172)return -90;
}
{
if(4>172-n)return -90;n+=4;
if(n>172)return -90;
}
{
if(4>172-n)return -90;n+=4;
if(n>172)return -90;
}
{
if(!isFinite(this.battery))return -22;
if(4>172-n)return -90;n+=4;
if(n>172)return -90;
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
static readControl(r:Reader):Ros1Turtlebot3_msgsSensorState{const value=new Ros1Turtlebot3_msgsSensorState();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
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
export class Ros1Turtlebot3_msgsSensorStateCodec implements VendorCodec<Ros1Turtlebot3_msgsSensorState>{
static readonly codecIdentity:string="UWVMROS1_0ef7dd1a4b965ae027b081a6b34f111731a3adfbabcc6fd547d4da7378fcb81f";
static readonly maxWireBytes:i32=172;
encodeChecked(value:Ros1Turtlebot3_msgsSensorState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Turtlebot3_msgsSensorState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Turtlebot3_msgsSensorState>{const out=new VendorDecoded<Ros1Turtlebot3_msgsSensorState>();if(bytes.length>172){out.status=90;return out;}if(bytes.length<44){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Turtlebot3_msgsSensorState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Turtlebot3_msgsSensorState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Turtlebot3_msgsSensorState():changetype<Ros1Turtlebot3_msgsSensorState>(out.value);}
}
/** Complete original turtlebot3_msgs/Sound; spellings are published in generation proof. */
export class Ros1Turtlebot3_msgsSound {
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
static readControl(r:Reader):Ros1Turtlebot3_msgsSound{const value=new Ros1Turtlebot3_msgsSound();
{
const decoded=<u8>r.integer(1);
value.value=decoded;
}
return value;}
}
export class Ros1Turtlebot3_msgsSoundCodec implements VendorCodec<Ros1Turtlebot3_msgsSound>{
static readonly codecIdentity:string="UWVMROS1_0e5af3d5a2acbfff5bf5ba94c1520490fe281e70aff02cfb6ad31acd89e7536a";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Turtlebot3_msgsSound):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Turtlebot3_msgsSound):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Turtlebot3_msgsSound>{const out=new VendorDecoded<Ros1Turtlebot3_msgsSound>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Turtlebot3_msgsSound.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Turtlebot3_msgsSound{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Turtlebot3_msgsSound():changetype<Ros1Turtlebot3_msgsSound>(out.value);}
}
/** Complete original turtlebot3_msgs/VersionInfo; spellings are published in generation proof. */
export class Ros1Turtlebot3_msgsVersionInfo {
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
static readControl(r:Reader):Ros1Turtlebot3_msgsVersionInfo{const value=new Ros1Turtlebot3_msgsVersionInfo();
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
export class Ros1Turtlebot3_msgsVersionInfoCodec implements VendorCodec<Ros1Turtlebot3_msgsVersionInfo>{
static readonly codecIdentity:string="UWVMROS1_8f41d37cf43fda2e3649d85fcca83ad735dc5a8122f7829a98b18659bcad8b3a";
static readonly maxWireBytes:i32=780;
encodeChecked(value:Ros1Turtlebot3_msgsVersionInfo):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Turtlebot3_msgsVersionInfo):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Turtlebot3_msgsVersionInfo>{const out=new VendorDecoded<Ros1Turtlebot3_msgsVersionInfo>();if(bytes.length>780){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Turtlebot3_msgsVersionInfo.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Turtlebot3_msgsVersionInfo{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Turtlebot3_msgsVersionInfo():changetype<Ros1Turtlebot3_msgsVersionInfo>(out.value);}
}
