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
/** Complete original geometry_msgs/PoseStamped; spellings are published in generation proof. */
export class Ros1Geometry_msgsPoseStamped {
static readonly maxWireBytes:i32=200;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=200;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field pose, native scalar geometry_msgs/Pose. */
pose:Ros1Geometry_msgsPose=new Ros1Geometry_msgsPose();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>200-n)return -90;n+=child;
if(n>200)return -90;
}
{
const child=this.pose.wireSize();if(child<0)return child;if(child>200-n)return -90;n+=child;
if(n>200)return -90;
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
static readControl(r:Reader):Ros1Geometry_msgsPoseStamped{const value=new Ros1Geometry_msgsPoseStamped();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros1Geometry_msgsPose.readControl(r);
value.pose=decoded;
}
return value;}
}
export class Ros1Geometry_msgsPoseStampedCodec implements VendorCodec<Ros1Geometry_msgsPoseStamped>{
static readonly codecIdentity:string="UWVMROS1_bb59c4e82f561266c0f10500bfbbd1a809e9c1122ecbf559a812326af3eb2e95";
static readonly maxWireBytes:i32=200;
encodeChecked(value:Ros1Geometry_msgsPoseStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsPoseStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsPoseStamped>{const out=new VendorDecoded<Ros1Geometry_msgsPoseStamped>();if(bytes.length>200){out.status=90;return out;}if(bytes.length<72){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsPoseStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsPoseStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsPoseStamped():changetype<Ros1Geometry_msgsPoseStamped>(out.value);}
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
/** Complete original geometry_msgs/Wrench; spellings are published in generation proof. */
export class Ros1Geometry_msgsWrench {
static readonly maxWireBytes:i32=48;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=48;
/** Exact native field force, native scalar geometry_msgs/Vector3. */
force:Ros1Geometry_msgsVector3=new Ros1Geometry_msgsVector3();
/** Exact native field torque, native scalar geometry_msgs/Vector3. */
torque:Ros1Geometry_msgsVector3=new Ros1Geometry_msgsVector3();
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
static readControl(r:Reader):Ros1Geometry_msgsWrench{const value=new Ros1Geometry_msgsWrench();
{
const decoded=Ros1Geometry_msgsVector3.readControl(r);
value.force=decoded;
}
{
const decoded=Ros1Geometry_msgsVector3.readControl(r);
value.torque=decoded;
}
return value;}
}
export class Ros1Geometry_msgsWrenchCodec implements VendorCodec<Ros1Geometry_msgsWrench>{
static readonly codecIdentity:string="UWVMROS1_b0938c4e084df68cf7c46445070ac3e9305d06f678275845aec6055db77617c3";
static readonly maxWireBytes:i32=48;
encodeChecked(value:Ros1Geometry_msgsWrench):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Geometry_msgsWrench):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Geometry_msgsWrench>{const out=new VendorDecoded<Ros1Geometry_msgsWrench>();if(bytes.length>48){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Geometry_msgsWrench.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Geometry_msgsWrench{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Geometry_msgsWrench():changetype<Ros1Geometry_msgsWrench>(out.value);}
}
/** Complete original ur_dashboard_msgs/AddToLogRequest; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsAddToLogRequest {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsAddToLogRequest{const value=new Ros1Ur_dashboard_msgsAddToLogRequest();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Ur_dashboard_msgsAddToLogRequestCodec implements VendorCodec<Ros1Ur_dashboard_msgsAddToLogRequest>{
static readonly codecIdentity:string="UWVMROS1_faf6c1ab9c749a05ae3f4e6196cc1c3574560daae1463ebe8684bf13ea938ac4";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros1Ur_dashboard_msgsAddToLogRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsAddToLogRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsAddToLogRequest>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsAddToLogRequest>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsAddToLogRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsAddToLogRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsAddToLogRequest():changetype<Ros1Ur_dashboard_msgsAddToLogRequest>(out.value);}
}
/** Complete original ur_dashboard_msgs/AddToLogResponse; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsAddToLogResponse {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsAddToLogResponse{const value=new Ros1Ur_dashboard_msgsAddToLogResponse();
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
export class Ros1Ur_dashboard_msgsAddToLogResponseCodec implements VendorCodec<Ros1Ur_dashboard_msgsAddToLogResponse>{
static readonly codecIdentity:string="UWVMROS1_d495f7609d04abde44cb35ec5114618a2875e0a30986d2f7cc62c8ce99354e7c";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Ur_dashboard_msgsAddToLogResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsAddToLogResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsAddToLogResponse>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsAddToLogResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsAddToLogResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsAddToLogResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsAddToLogResponse():changetype<Ros1Ur_dashboard_msgsAddToLogResponse>(out.value);}
}
/** Complete original ur_dashboard_msgs/GetLoadedProgramRequest; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsGetLoadedProgramRequest {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsGetLoadedProgramRequest{const value=new Ros1Ur_dashboard_msgsGetLoadedProgramRequest();
return value;}
}
export class Ros1Ur_dashboard_msgsGetLoadedProgramRequestCodec implements VendorCodec<Ros1Ur_dashboard_msgsGetLoadedProgramRequest>{
static readonly codecIdentity:string="UWVMROS1_54eecd0b652707f9c16dcbacbef05902112db993500404e1adaf26618e1d3a1a";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Ur_dashboard_msgsGetLoadedProgramRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsGetLoadedProgramRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsGetLoadedProgramRequest>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsGetLoadedProgramRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsGetLoadedProgramRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsGetLoadedProgramRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsGetLoadedProgramRequest():changetype<Ros1Ur_dashboard_msgsGetLoadedProgramRequest>(out.value);}
}
/** Complete original ur_dashboard_msgs/GetLoadedProgramResponse; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsGetLoadedProgramResponse {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsGetLoadedProgramResponse{const value=new Ros1Ur_dashboard_msgsGetLoadedProgramResponse();
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
export class Ros1Ur_dashboard_msgsGetLoadedProgramResponseCodec implements VendorCodec<Ros1Ur_dashboard_msgsGetLoadedProgramResponse>{
static readonly codecIdentity:string="UWVMROS1_1680747345d40412a4731c48f08c402ac97fb12dd871fb320c2a9d002453f87a";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros1Ur_dashboard_msgsGetLoadedProgramResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsGetLoadedProgramResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsGetLoadedProgramResponse>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsGetLoadedProgramResponse>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsGetLoadedProgramResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsGetLoadedProgramResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsGetLoadedProgramResponse():changetype<Ros1Ur_dashboard_msgsGetLoadedProgramResponse>(out.value);}
}
/** Complete original ur_dashboard_msgs/GetProgramStateRequest; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsGetProgramStateRequest {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsGetProgramStateRequest{const value=new Ros1Ur_dashboard_msgsGetProgramStateRequest();
return value;}
}
export class Ros1Ur_dashboard_msgsGetProgramStateRequestCodec implements VendorCodec<Ros1Ur_dashboard_msgsGetProgramStateRequest>{
static readonly codecIdentity:string="UWVMROS1_cf14c6f86ef1904f80e88117dc2f2d29a003b52e21bc6569795771f04cf072a2";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Ur_dashboard_msgsGetProgramStateRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsGetProgramStateRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsGetProgramStateRequest>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsGetProgramStateRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsGetProgramStateRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsGetProgramStateRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsGetProgramStateRequest():changetype<Ros1Ur_dashboard_msgsGetProgramStateRequest>(out.value);}
}
/** Complete original ur_dashboard_msgs/ProgramState; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsProgramState {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsProgramState{const value=new Ros1Ur_dashboard_msgsProgramState();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.state=decoded;
}
return value;}
}
export class Ros1Ur_dashboard_msgsProgramStateCodec implements VendorCodec<Ros1Ur_dashboard_msgsProgramState>{
static readonly codecIdentity:string="UWVMROS1_97a57db64f20abcb9929b5296c75d66d994f2e9e26449810197c657694d3850e";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros1Ur_dashboard_msgsProgramState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsProgramState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsProgramState>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsProgramState>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsProgramState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsProgramState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsProgramState():changetype<Ros1Ur_dashboard_msgsProgramState>(out.value);}
}
/** Complete original ur_dashboard_msgs/GetProgramStateResponse; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsGetProgramStateResponse {
static readonly maxWireBytes:i32=781;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=781;
/** Exact native field state, native scalar ur_dashboard_msgs/ProgramState. */
state:Ros1Ur_dashboard_msgsProgramState=new Ros1Ur_dashboard_msgsProgramState();
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsGetProgramStateResponse{const value=new Ros1Ur_dashboard_msgsGetProgramStateResponse();
{
const decoded=Ros1Ur_dashboard_msgsProgramState.readControl(r);
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
export class Ros1Ur_dashboard_msgsGetProgramStateResponseCodec implements VendorCodec<Ros1Ur_dashboard_msgsGetProgramStateResponse>{
static readonly codecIdentity:string="UWVMROS1_366caf89303bfff48d3c0f89aa2a640c3d29af5a0c19d76ceccf26d50c3ab2ed";
static readonly maxWireBytes:i32=781;
encodeChecked(value:Ros1Ur_dashboard_msgsGetProgramStateResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsGetProgramStateResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsGetProgramStateResponse>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsGetProgramStateResponse>();if(bytes.length>781){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsGetProgramStateResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsGetProgramStateResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsGetProgramStateResponse():changetype<Ros1Ur_dashboard_msgsGetProgramStateResponse>(out.value);}
}
/** Complete original ur_dashboard_msgs/GetRobotModeRequest; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsGetRobotModeRequest {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsGetRobotModeRequest{const value=new Ros1Ur_dashboard_msgsGetRobotModeRequest();
return value;}
}
export class Ros1Ur_dashboard_msgsGetRobotModeRequestCodec implements VendorCodec<Ros1Ur_dashboard_msgsGetRobotModeRequest>{
static readonly codecIdentity:string="UWVMROS1_b9ffe283d7dbcb85879dbf6964d393f5d44661227bb26e1acc34308edb288609";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Ur_dashboard_msgsGetRobotModeRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsGetRobotModeRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsGetRobotModeRequest>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsGetRobotModeRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsGetRobotModeRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsGetRobotModeRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsGetRobotModeRequest():changetype<Ros1Ur_dashboard_msgsGetRobotModeRequest>(out.value);}
}
/** Complete original ur_dashboard_msgs/RobotMode; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsRobotMode {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsRobotMode{const value=new Ros1Ur_dashboard_msgsRobotMode();
{
const decoded=<i8>r.integer(1);
value.mode=decoded;
}
return value;}
}
export class Ros1Ur_dashboard_msgsRobotModeCodec implements VendorCodec<Ros1Ur_dashboard_msgsRobotMode>{
static readonly codecIdentity:string="UWVMROS1_b2772853eff2ec9b849b6de4a90ce98487a8eff49d1371283dce503908702066";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ur_dashboard_msgsRobotMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsRobotMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsRobotMode>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsRobotMode>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsRobotMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsRobotMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsRobotMode():changetype<Ros1Ur_dashboard_msgsRobotMode>(out.value);}
}
/** Complete original ur_dashboard_msgs/GetRobotModeResponse; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsGetRobotModeResponse {
static readonly maxWireBytes:i32=262;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=262;
/** Exact native field robot_mode, native scalar ur_dashboard_msgs/RobotMode. */
robot_mode:Ros1Ur_dashboard_msgsRobotMode=new Ros1Ur_dashboard_msgsRobotMode();
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsGetRobotModeResponse{const value=new Ros1Ur_dashboard_msgsGetRobotModeResponse();
{
const decoded=Ros1Ur_dashboard_msgsRobotMode.readControl(r);
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
export class Ros1Ur_dashboard_msgsGetRobotModeResponseCodec implements VendorCodec<Ros1Ur_dashboard_msgsGetRobotModeResponse>{
static readonly codecIdentity:string="UWVMROS1_f6c0d0d03a956a34b00fd63540226b6abb8b106e2a35cb912b4886a5c4cecc3f";
static readonly maxWireBytes:i32=262;
encodeChecked(value:Ros1Ur_dashboard_msgsGetRobotModeResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsGetRobotModeResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsGetRobotModeResponse>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsGetRobotModeResponse>();if(bytes.length>262){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsGetRobotModeResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsGetRobotModeResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsGetRobotModeResponse():changetype<Ros1Ur_dashboard_msgsGetRobotModeResponse>(out.value);}
}
/** Complete original ur_dashboard_msgs/GetSafetyModeRequest; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsGetSafetyModeRequest {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsGetSafetyModeRequest{const value=new Ros1Ur_dashboard_msgsGetSafetyModeRequest();
return value;}
}
export class Ros1Ur_dashboard_msgsGetSafetyModeRequestCodec implements VendorCodec<Ros1Ur_dashboard_msgsGetSafetyModeRequest>{
static readonly codecIdentity:string="UWVMROS1_f697e2ff2bcd56523563060a46bca2e1b7af487128f05562fcb18bfa73cbca8b";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Ur_dashboard_msgsGetSafetyModeRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsGetSafetyModeRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsGetSafetyModeRequest>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsGetSafetyModeRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsGetSafetyModeRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsGetSafetyModeRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsGetSafetyModeRequest():changetype<Ros1Ur_dashboard_msgsGetSafetyModeRequest>(out.value);}
}
/** Complete original ur_dashboard_msgs/SafetyMode; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsSafetyMode {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsSafetyMode{const value=new Ros1Ur_dashboard_msgsSafetyMode();
{
const decoded=<u8>r.integer(1);
value.mode=decoded;
}
return value;}
}
export class Ros1Ur_dashboard_msgsSafetyModeCodec implements VendorCodec<Ros1Ur_dashboard_msgsSafetyMode>{
static readonly codecIdentity:string="UWVMROS1_5a0c2737166cf2e2bd4c17b46c597f674b8d806b802d4e86ee0d76fc55f29cf7";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ur_dashboard_msgsSafetyMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsSafetyMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsSafetyMode>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsSafetyMode>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsSafetyMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsSafetyMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsSafetyMode():changetype<Ros1Ur_dashboard_msgsSafetyMode>(out.value);}
}
/** Complete original ur_dashboard_msgs/GetSafetyModeResponse; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsGetSafetyModeResponse {
static readonly maxWireBytes:i32=262;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=262;
/** Exact native field safety_mode, native scalar ur_dashboard_msgs/SafetyMode. */
safety_mode:Ros1Ur_dashboard_msgsSafetyMode=new Ros1Ur_dashboard_msgsSafetyMode();
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsGetSafetyModeResponse{const value=new Ros1Ur_dashboard_msgsGetSafetyModeResponse();
{
const decoded=Ros1Ur_dashboard_msgsSafetyMode.readControl(r);
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
export class Ros1Ur_dashboard_msgsGetSafetyModeResponseCodec implements VendorCodec<Ros1Ur_dashboard_msgsGetSafetyModeResponse>{
static readonly codecIdentity:string="UWVMROS1_70121af922588eaab9a34855e3da0cde837b1449d2a2d6c28addc70916df1a15";
static readonly maxWireBytes:i32=262;
encodeChecked(value:Ros1Ur_dashboard_msgsGetSafetyModeResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsGetSafetyModeResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsGetSafetyModeResponse>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsGetSafetyModeResponse>();if(bytes.length>262){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsGetSafetyModeResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsGetSafetyModeResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsGetSafetyModeResponse():changetype<Ros1Ur_dashboard_msgsGetSafetyModeResponse>(out.value);}
}
/** Complete original ur_dashboard_msgs/IsInRemoteControlRequest; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsIsInRemoteControlRequest {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsIsInRemoteControlRequest{const value=new Ros1Ur_dashboard_msgsIsInRemoteControlRequest();
return value;}
}
export class Ros1Ur_dashboard_msgsIsInRemoteControlRequestCodec implements VendorCodec<Ros1Ur_dashboard_msgsIsInRemoteControlRequest>{
static readonly codecIdentity:string="UWVMROS1_4b3c39f36f953904eb582d8c5f91161f2864782d4a4ab209a7dab32a117175da";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Ur_dashboard_msgsIsInRemoteControlRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsIsInRemoteControlRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsIsInRemoteControlRequest>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsIsInRemoteControlRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsIsInRemoteControlRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsIsInRemoteControlRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsIsInRemoteControlRequest():changetype<Ros1Ur_dashboard_msgsIsInRemoteControlRequest>(out.value);}
}
/** Complete original ur_dashboard_msgs/IsInRemoteControlResponse; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsIsInRemoteControlResponse {
static readonly maxWireBytes:i32=262;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=262;
/** Exact native field answer, native scalar string. */
answer:string="";
/** Exact native field in_remote_control, native scalar bool. */
in_remote_control:bool=false;
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
w.u8(this.in_remote_control?1:0);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ur_dashboard_msgsIsInRemoteControlResponse{const value=new Ros1Ur_dashboard_msgsIsInRemoteControlResponse();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.in_remote_control=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros1Ur_dashboard_msgsIsInRemoteControlResponseCodec implements VendorCodec<Ros1Ur_dashboard_msgsIsInRemoteControlResponse>{
static readonly codecIdentity:string="UWVMROS1_6c4af27571b48a5b8e8f1f857b2c02a333e7b4b360f8f3645a39630594c1663a";
static readonly maxWireBytes:i32=262;
encodeChecked(value:Ros1Ur_dashboard_msgsIsInRemoteControlResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsIsInRemoteControlResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsIsInRemoteControlResponse>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsIsInRemoteControlResponse>();if(bytes.length>262){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsIsInRemoteControlResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsIsInRemoteControlResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsIsInRemoteControlResponse():changetype<Ros1Ur_dashboard_msgsIsInRemoteControlResponse>(out.value);}
}
/** Complete original ur_dashboard_msgs/IsProgramRunningRequest; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsIsProgramRunningRequest {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsIsProgramRunningRequest{const value=new Ros1Ur_dashboard_msgsIsProgramRunningRequest();
return value;}
}
export class Ros1Ur_dashboard_msgsIsProgramRunningRequestCodec implements VendorCodec<Ros1Ur_dashboard_msgsIsProgramRunningRequest>{
static readonly codecIdentity:string="UWVMROS1_0be481297028886e15f07ace4178ca0aec595724cd83ab01d0155e7c6aa6666f";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Ur_dashboard_msgsIsProgramRunningRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsIsProgramRunningRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsIsProgramRunningRequest>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsIsProgramRunningRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsIsProgramRunningRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsIsProgramRunningRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsIsProgramRunningRequest():changetype<Ros1Ur_dashboard_msgsIsProgramRunningRequest>(out.value);}
}
/** Complete original ur_dashboard_msgs/IsProgramRunningResponse; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsIsProgramRunningResponse {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsIsProgramRunningResponse{const value=new Ros1Ur_dashboard_msgsIsProgramRunningResponse();
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
export class Ros1Ur_dashboard_msgsIsProgramRunningResponseCodec implements VendorCodec<Ros1Ur_dashboard_msgsIsProgramRunningResponse>{
static readonly codecIdentity:string="UWVMROS1_4d9de4f7e7071d142c09e3184d4a6f29d4044d003ad11ba3b72e1129ecf144ac";
static readonly maxWireBytes:i32=262;
encodeChecked(value:Ros1Ur_dashboard_msgsIsProgramRunningResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsIsProgramRunningResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsIsProgramRunningResponse>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsIsProgramRunningResponse>();if(bytes.length>262){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsIsProgramRunningResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsIsProgramRunningResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsIsProgramRunningResponse():changetype<Ros1Ur_dashboard_msgsIsProgramRunningResponse>(out.value);}
}
/** Complete original ur_dashboard_msgs/IsProgramSavedRequest; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsIsProgramSavedRequest {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsIsProgramSavedRequest{const value=new Ros1Ur_dashboard_msgsIsProgramSavedRequest();
return value;}
}
export class Ros1Ur_dashboard_msgsIsProgramSavedRequestCodec implements VendorCodec<Ros1Ur_dashboard_msgsIsProgramSavedRequest>{
static readonly codecIdentity:string="UWVMROS1_45ce1466c35adc812096bfb8b2cd84dfa87940109831c9f797b5786afa14f5a8";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Ur_dashboard_msgsIsProgramSavedRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsIsProgramSavedRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsIsProgramSavedRequest>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsIsProgramSavedRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsIsProgramSavedRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsIsProgramSavedRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsIsProgramSavedRequest():changetype<Ros1Ur_dashboard_msgsIsProgramSavedRequest>(out.value);}
}
/** Complete original ur_dashboard_msgs/IsProgramSavedResponse; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsIsProgramSavedResponse {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsIsProgramSavedResponse{const value=new Ros1Ur_dashboard_msgsIsProgramSavedResponse();
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
export class Ros1Ur_dashboard_msgsIsProgramSavedResponseCodec implements VendorCodec<Ros1Ur_dashboard_msgsIsProgramSavedResponse>{
static readonly codecIdentity:string="UWVMROS1_eb53e077fe7b216e27fc57f838666a54a84e479e02e508d41df5cff9f0637a52";
static readonly maxWireBytes:i32=522;
encodeChecked(value:Ros1Ur_dashboard_msgsIsProgramSavedResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsIsProgramSavedResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsIsProgramSavedResponse>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsIsProgramSavedResponse>();if(bytes.length>522){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsIsProgramSavedResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsIsProgramSavedResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsIsProgramSavedResponse():changetype<Ros1Ur_dashboard_msgsIsProgramSavedResponse>(out.value);}
}
/** Complete original ur_dashboard_msgs/LoadRequest; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsLoadRequest {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsLoadRequest{const value=new Ros1Ur_dashboard_msgsLoadRequest();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.filename=decoded;
}
return value;}
}
export class Ros1Ur_dashboard_msgsLoadRequestCodec implements VendorCodec<Ros1Ur_dashboard_msgsLoadRequest>{
static readonly codecIdentity:string="UWVMROS1_dfbde7b2376c7c9ba3926e4803f16b22f7ade3b2f768a25886a1490656edd2bd";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros1Ur_dashboard_msgsLoadRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsLoadRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsLoadRequest>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsLoadRequest>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsLoadRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsLoadRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsLoadRequest():changetype<Ros1Ur_dashboard_msgsLoadRequest>(out.value);}
}
/** Complete original ur_dashboard_msgs/LoadResponse; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsLoadResponse {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsLoadResponse{const value=new Ros1Ur_dashboard_msgsLoadResponse();
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
export class Ros1Ur_dashboard_msgsLoadResponseCodec implements VendorCodec<Ros1Ur_dashboard_msgsLoadResponse>{
static readonly codecIdentity:string="UWVMROS1_0ed3b3f2a0066248734e64c0706e52ebf07113569a98baf249a99adfff59be02";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Ur_dashboard_msgsLoadResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsLoadResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsLoadResponse>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsLoadResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsLoadResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsLoadResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsLoadResponse():changetype<Ros1Ur_dashboard_msgsLoadResponse>(out.value);}
}
/** Complete original ur_dashboard_msgs/PopupRequest; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsPopupRequest {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsPopupRequest{const value=new Ros1Ur_dashboard_msgsPopupRequest();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Ur_dashboard_msgsPopupRequestCodec implements VendorCodec<Ros1Ur_dashboard_msgsPopupRequest>{
static readonly codecIdentity:string="UWVMROS1_11556bfb811a28141b6c5e9c11720920a389d735b8c09ed1a8f47d05f293e90d";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros1Ur_dashboard_msgsPopupRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsPopupRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsPopupRequest>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsPopupRequest>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsPopupRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsPopupRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsPopupRequest():changetype<Ros1Ur_dashboard_msgsPopupRequest>(out.value);}
}
/** Complete original ur_dashboard_msgs/PopupResponse; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsPopupResponse {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsPopupResponse{const value=new Ros1Ur_dashboard_msgsPopupResponse();
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
export class Ros1Ur_dashboard_msgsPopupResponseCodec implements VendorCodec<Ros1Ur_dashboard_msgsPopupResponse>{
static readonly codecIdentity:string="UWVMROS1_33ac5b2128d6e7c2e562af7e2b8a8e907c16c07034b377920cba4fbd04ca9e14";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Ur_dashboard_msgsPopupResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsPopupResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsPopupResponse>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsPopupResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsPopupResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsPopupResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsPopupResponse():changetype<Ros1Ur_dashboard_msgsPopupResponse>(out.value);}
}
/** Complete original ur_dashboard_msgs/RawRequestRequest; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsRawRequestRequest {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsRawRequestRequest{const value=new Ros1Ur_dashboard_msgsRawRequestRequest();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.query=decoded;
}
return value;}
}
export class Ros1Ur_dashboard_msgsRawRequestRequestCodec implements VendorCodec<Ros1Ur_dashboard_msgsRawRequestRequest>{
static readonly codecIdentity:string="UWVMROS1_ca8e3fdb45888bf4feb3c0f4ed05b7b9499a74493aaa0a8727f9f513922adfb4";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros1Ur_dashboard_msgsRawRequestRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsRawRequestRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsRawRequestRequest>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsRawRequestRequest>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsRawRequestRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsRawRequestRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsRawRequestRequest():changetype<Ros1Ur_dashboard_msgsRawRequestRequest>(out.value);}
}
/** Complete original ur_dashboard_msgs/RawRequestResponse; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsRawRequestResponse {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsRawRequestResponse{const value=new Ros1Ur_dashboard_msgsRawRequestResponse();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.answer=decoded;
}
return value;}
}
export class Ros1Ur_dashboard_msgsRawRequestResponseCodec implements VendorCodec<Ros1Ur_dashboard_msgsRawRequestResponse>{
static readonly codecIdentity:string="UWVMROS1_76dbb0c8d0143bd4d39bbf66c6866b0771013fe67fbad76887c6490c91f39e80";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros1Ur_dashboard_msgsRawRequestResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsRawRequestResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsRawRequestResponse>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsRawRequestResponse>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsRawRequestResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsRawRequestResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsRawRequestResponse():changetype<Ros1Ur_dashboard_msgsRawRequestResponse>(out.value);}
}
/** Complete original ur_dashboard_msgs/SetModeGoal; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsSetModeGoal {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field target_robot_mode, native scalar ur_dashboard_msgs/RobotMode. */
target_robot_mode:Ros1Ur_dashboard_msgsRobotMode=new Ros1Ur_dashboard_msgsRobotMode();
/** Exact native field stop_program, native scalar bool. */
stop_program:bool=false;
/** Exact native field play_program, native scalar bool. */
play_program:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.target_robot_mode.wireSize();if(child<0)return child;if(child>3-n)return -90;n+=child;
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
this.target_robot_mode.writeControl(w);
w.u8(this.stop_program?1:0);
w.u8(this.play_program?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ur_dashboard_msgsSetModeGoal{const value=new Ros1Ur_dashboard_msgsSetModeGoal();
{
const decoded=Ros1Ur_dashboard_msgsRobotMode.readControl(r);
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
export class Ros1Ur_dashboard_msgsSetModeGoalCodec implements VendorCodec<Ros1Ur_dashboard_msgsSetModeGoal>{
static readonly codecIdentity:string="UWVMROS1_27ba58b57af47e8efff15a70fb0562d5904d019c17e39a499bf003730434a528";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Ur_dashboard_msgsSetModeGoal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsSetModeGoal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsSetModeGoal>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsSetModeGoal>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsSetModeGoal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsSetModeGoal{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsSetModeGoal():changetype<Ros1Ur_dashboard_msgsSetModeGoal>(out.value);}
}
/** Complete original ur_dashboard_msgs/SetModeResult; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsSetModeResult {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsSetModeResult{const value=new Ros1Ur_dashboard_msgsSetModeResult();
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
export class Ros1Ur_dashboard_msgsSetModeResultCodec implements VendorCodec<Ros1Ur_dashboard_msgsSetModeResult>{
static readonly codecIdentity:string="UWVMROS1_e558d01061183cabca7022bd66aeb1958e14dfc62226a701c86020e73ffa1114";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Ur_dashboard_msgsSetModeResult):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsSetModeResult):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsSetModeResult>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsSetModeResult>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsSetModeResult.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsSetModeResult{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsSetModeResult():changetype<Ros1Ur_dashboard_msgsSetModeResult>(out.value);}
}
/** Complete original ur_dashboard_msgs/SetModeFeedback; spellings are published in generation proof. */
export class Ros1Ur_dashboard_msgsSetModeFeedback {
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
static readControl(r:Reader):Ros1Ur_dashboard_msgsSetModeFeedback{const value=new Ros1Ur_dashboard_msgsSetModeFeedback();
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
export class Ros1Ur_dashboard_msgsSetModeFeedbackCodec implements VendorCodec<Ros1Ur_dashboard_msgsSetModeFeedback>{
static readonly codecIdentity:string="UWVMROS1_d29b507dcc5d72ee086c8a5a9a7a762d677ed408ec02dbe661abc17e2de5c7e2";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros1Ur_dashboard_msgsSetModeFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_dashboard_msgsSetModeFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_dashboard_msgsSetModeFeedback>{const out=new VendorDecoded<Ros1Ur_dashboard_msgsSetModeFeedback>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_dashboard_msgsSetModeFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_dashboard_msgsSetModeFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_dashboard_msgsSetModeFeedback():changetype<Ros1Ur_dashboard_msgsSetModeFeedback>(out.value);}
}
/** Complete original ur_msgs/Analog; spellings are published in generation proof. */
export class Ros1Ur_msgsAnalog {
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
static readControl(r:Reader):Ros1Ur_msgsAnalog{const value=new Ros1Ur_msgsAnalog();
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
export class Ros1Ur_msgsAnalogCodec implements VendorCodec<Ros1Ur_msgsAnalog>{
static readonly codecIdentity:string="UWVMROS1_638dc3b30def8193a501b4c1c778792a60c8c683a96880e9fa6db6c01774f6a7";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros1Ur_msgsAnalog):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsAnalog):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsAnalog>{const out=new VendorDecoded<Ros1Ur_msgsAnalog>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsAnalog.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsAnalog{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsAnalog():changetype<Ros1Ur_msgsAnalog>(out.value);}
}
/** Complete original ur_msgs/Digital; spellings are published in generation proof. */
export class Ros1Ur_msgsDigital {
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
static readControl(r:Reader):Ros1Ur_msgsDigital{const value=new Ros1Ur_msgsDigital();
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
export class Ros1Ur_msgsDigitalCodec implements VendorCodec<Ros1Ur_msgsDigital>{
static readonly codecIdentity:string="UWVMROS1_c5840cfc5e26fe2f18817e46a0322a92e02ca21983920c71ebeb585769f5622c";
static readonly maxWireBytes:i32=2;
encodeChecked(value:Ros1Ur_msgsDigital):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsDigital):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsDigital>{const out=new VendorDecoded<Ros1Ur_msgsDigital>();if(bytes.length>2){out.status=90;return out;}if(bytes.length<2){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsDigital.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsDigital{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsDigital():changetype<Ros1Ur_msgsDigital>(out.value);}
}
/** Complete original ur_msgs/GetRobotSoftwareVersionRequest; spellings are published in generation proof. */
export class Ros1Ur_msgsGetRobotSoftwareVersionRequest {
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
static readControl(r:Reader):Ros1Ur_msgsGetRobotSoftwareVersionRequest{const value=new Ros1Ur_msgsGetRobotSoftwareVersionRequest();
return value;}
}
export class Ros1Ur_msgsGetRobotSoftwareVersionRequestCodec implements VendorCodec<Ros1Ur_msgsGetRobotSoftwareVersionRequest>{
static readonly codecIdentity:string="UWVMROS1_bbcdd98ee8d951a8a4fa873ad1c755f16c5cce14ceb7a614343dd44ccf84b9c8";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Ur_msgsGetRobotSoftwareVersionRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsGetRobotSoftwareVersionRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsGetRobotSoftwareVersionRequest>{const out=new VendorDecoded<Ros1Ur_msgsGetRobotSoftwareVersionRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsGetRobotSoftwareVersionRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsGetRobotSoftwareVersionRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsGetRobotSoftwareVersionRequest():changetype<Ros1Ur_msgsGetRobotSoftwareVersionRequest>(out.value);}
}
/** Complete original ur_msgs/GetRobotSoftwareVersionResponse; spellings are published in generation proof. */
export class Ros1Ur_msgsGetRobotSoftwareVersionResponse {
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
static readControl(r:Reader):Ros1Ur_msgsGetRobotSoftwareVersionResponse{const value=new Ros1Ur_msgsGetRobotSoftwareVersionResponse();
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
export class Ros1Ur_msgsGetRobotSoftwareVersionResponseCodec implements VendorCodec<Ros1Ur_msgsGetRobotSoftwareVersionResponse>{
static readonly codecIdentity:string="UWVMROS1_36ab51ce55e3630f8ca71f364f4c5cc1f4b010f678777e4fd7b03dd480f92e64";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Ur_msgsGetRobotSoftwareVersionResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsGetRobotSoftwareVersionResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsGetRobotSoftwareVersionResponse>{const out=new VendorDecoded<Ros1Ur_msgsGetRobotSoftwareVersionResponse>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsGetRobotSoftwareVersionResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsGetRobotSoftwareVersionResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsGetRobotSoftwareVersionResponse():changetype<Ros1Ur_msgsGetRobotSoftwareVersionResponse>(out.value);}
}
/** Complete original ur_msgs/IOStates; spellings are published in generation proof. */
export class Ros1Ur_msgsIOStates {
static readonly maxWireBytes:i32=404;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=404;
/** Exact native field digital_in_states, native scalar ur_msgs/Digital. */
digital_in_states:Array<Ros1Ur_msgsDigital>=new Array<Ros1Ur_msgsDigital>();
/** Exact native field digital_out_states, native scalar ur_msgs/Digital. */
digital_out_states:Array<Ros1Ur_msgsDigital>=new Array<Ros1Ur_msgsDigital>();
/** Exact native field flag_states, native scalar ur_msgs/Digital. */
flag_states:Array<Ros1Ur_msgsDigital>=new Array<Ros1Ur_msgsDigital>();
/** Exact native field analog_in_states, native scalar ur_msgs/Analog. */
analog_in_states:Array<Ros1Ur_msgsAnalog>=new Array<Ros1Ur_msgsAnalog>();
/** Exact native field analog_out_states, native scalar ur_msgs/Analog. */
analog_out_states:Array<Ros1Ur_msgsAnalog>=new Array<Ros1Ur_msgsAnalog>();
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
static readControl(r:Reader):Ros1Ur_msgsIOStates{const value=new Ros1Ur_msgsIOStates();
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.digital_in_states=new Array<Ros1Ur_msgsDigital>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Ur_msgsDigital.readControl(r);
value.digital_in_states.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.digital_out_states=new Array<Ros1Ur_msgsDigital>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Ur_msgsDigital.readControl(r);
value.digital_out_states.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.flag_states=new Array<Ros1Ur_msgsDigital>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Ur_msgsDigital.readControl(r);
value.flag_states.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.analog_in_states=new Array<Ros1Ur_msgsAnalog>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Ur_msgsAnalog.readControl(r);
value.analog_in_states.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.analog_out_states=new Array<Ros1Ur_msgsAnalog>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Ur_msgsAnalog.readControl(r);
value.analog_out_states.push(decoded);
}
}
return value;}
}
export class Ros1Ur_msgsIOStatesCodec implements VendorCodec<Ros1Ur_msgsIOStates>{
static readonly codecIdentity:string="UWVMROS1_4b10f6efdd2f129267c407bc1f8caed03776fef6d49f0a1b9dfbf2451ad8f033";
static readonly maxWireBytes:i32=404;
encodeChecked(value:Ros1Ur_msgsIOStates):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsIOStates):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsIOStates>{const out=new VendorDecoded<Ros1Ur_msgsIOStates>();if(bytes.length>404){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsIOStates.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsIOStates{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsIOStates():changetype<Ros1Ur_msgsIOStates>(out.value);}
}
/** Complete original ur_msgs/MasterboardDataMsg; spellings are published in generation proof. */
export class Ros1Ur_msgsMasterboardDataMsg {
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
/** Exact native field robot_voltage_48V, native scalar float32. */
robot_voltage_48V:f32=0;
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
if(!isFinite(this.robot_voltage_48V))return -22;
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
w.integer(<u64>reinterpret<u32>(this.robot_voltage_48V),4);
w.integer(<u64>reinterpret<u32>(this.robot_current),4);
w.integer(<u64>reinterpret<u32>(this.master_io_current),4);
w.integer(<u64>this.master_safety_state,1);
w.integer(<u64>this.master_onoff_state,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ur_msgsMasterboardDataMsg{const value=new Ros1Ur_msgsMasterboardDataMsg();
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
value.robot_voltage_48V=decoded;
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
export class Ros1Ur_msgsMasterboardDataMsgCodec implements VendorCodec<Ros1Ur_msgsMasterboardDataMsg>{
static readonly codecIdentity:string="UWVMROS1_f80f92b90cb33dd5b118df81447dfcc33c1ab230a91bb915a1be684401da265f";
static readonly maxWireBytes:i32=62;
encodeChecked(value:Ros1Ur_msgsMasterboardDataMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsMasterboardDataMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsMasterboardDataMsg>{const out=new VendorDecoded<Ros1Ur_msgsMasterboardDataMsg>();if(bytes.length>62){out.status=90;return out;}if(bytes.length<62){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsMasterboardDataMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsMasterboardDataMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsMasterboardDataMsg():changetype<Ros1Ur_msgsMasterboardDataMsg>(out.value);}
}
/** Complete original ur_msgs/RobotModeDataMsg; spellings are published in generation proof. */
export class Ros1Ur_msgsRobotModeDataMsg {
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
static readControl(r:Reader):Ros1Ur_msgsRobotModeDataMsg{const value=new Ros1Ur_msgsRobotModeDataMsg();
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
export class Ros1Ur_msgsRobotModeDataMsgCodec implements VendorCodec<Ros1Ur_msgsRobotModeDataMsg>{
static readonly codecIdentity:string="UWVMROS1_64c4247d865e0731b13ca13ce2e355df664cabf7f3d646051d9d3ef48b05a231";
static readonly maxWireBytes:i32=15;
encodeChecked(value:Ros1Ur_msgsRobotModeDataMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsRobotModeDataMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsRobotModeDataMsg>{const out=new VendorDecoded<Ros1Ur_msgsRobotModeDataMsg>();if(bytes.length>15){out.status=90;return out;}if(bytes.length<15){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsRobotModeDataMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsRobotModeDataMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsRobotModeDataMsg():changetype<Ros1Ur_msgsRobotModeDataMsg>(out.value);}
}
/** Complete original ur_msgs/RobotStateRTMsg; spellings are published in generation proof. */
export class Ros1Ur_msgsRobotStateRTMsg {
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
static readControl(r:Reader):Ros1Ur_msgsRobotStateRTMsg{const value=new Ros1Ur_msgsRobotStateRTMsg();
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
export class Ros1Ur_msgsRobotStateRTMsgCodec implements VendorCodec<Ros1Ur_msgsRobotStateRTMsg>{
static readonly codecIdentity:string="UWVMROS1_e3d93b6141f749b93c229d8eabebbeb2782a16621fea4d7d39096794e7a67ce7";
static readonly maxWireBytes:i32=768;
encodeChecked(value:Ros1Ur_msgsRobotStateRTMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsRobotStateRTMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsRobotStateRTMsg>{const out=new VendorDecoded<Ros1Ur_msgsRobotStateRTMsg>();if(bytes.length>768){out.status=90;return out;}if(bytes.length<96){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsRobotStateRTMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsRobotStateRTMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsRobotStateRTMsg():changetype<Ros1Ur_msgsRobotStateRTMsg>(out.value);}
}
/** Complete original ur_msgs/SetAnalogOutputRequest; spellings are published in generation proof. */
export class Ros1Ur_msgsSetAnalogOutputRequest {
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field data, native scalar ur_msgs/Analog. */
data:Ros1Ur_msgsAnalog=new Ros1Ur_msgsAnalog();
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
static readControl(r:Reader):Ros1Ur_msgsSetAnalogOutputRequest{const value=new Ros1Ur_msgsSetAnalogOutputRequest();
{
const decoded=Ros1Ur_msgsAnalog.readControl(r);
value.data=decoded;
}
return value;}
}
export class Ros1Ur_msgsSetAnalogOutputRequestCodec implements VendorCodec<Ros1Ur_msgsSetAnalogOutputRequest>{
static readonly codecIdentity:string="UWVMROS1_1830a63c8853ebe8842f0e8e6870fbdee136dfe78726ba581fd1c54f910c4789";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros1Ur_msgsSetAnalogOutputRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsSetAnalogOutputRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsSetAnalogOutputRequest>{const out=new VendorDecoded<Ros1Ur_msgsSetAnalogOutputRequest>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsSetAnalogOutputRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsSetAnalogOutputRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsSetAnalogOutputRequest():changetype<Ros1Ur_msgsSetAnalogOutputRequest>(out.value);}
}
/** Complete original ur_msgs/SetAnalogOutputResponse; spellings are published in generation proof. */
export class Ros1Ur_msgsSetAnalogOutputResponse {
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
static readControl(r:Reader):Ros1Ur_msgsSetAnalogOutputResponse{const value=new Ros1Ur_msgsSetAnalogOutputResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros1Ur_msgsSetAnalogOutputResponseCodec implements VendorCodec<Ros1Ur_msgsSetAnalogOutputResponse>{
static readonly codecIdentity:string="UWVMROS1_bb84c969f904082699f67d385b093b0c6de97bfacc33eab86932f4564a823580";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ur_msgsSetAnalogOutputResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsSetAnalogOutputResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsSetAnalogOutputResponse>{const out=new VendorDecoded<Ros1Ur_msgsSetAnalogOutputResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsSetAnalogOutputResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsSetAnalogOutputResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsSetAnalogOutputResponse():changetype<Ros1Ur_msgsSetAnalogOutputResponse>(out.value);}
}
/** Complete original ur_msgs/SetForceModeRequest; spellings are published in generation proof. */
export class Ros1Ur_msgsSetForceModeRequest {
static readonly TCP_TO_ORIGIN:u8=1;
static readonly NO_TRANSFORM:u8=2;
static readonly TCP_VELOCITY_TO_X_Y:u8=3;
static readonly maxWireBytes:i32=335;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=335;
/** Exact native field task_frame, native scalar geometry_msgs/PoseStamped. */
task_frame:Ros1Geometry_msgsPoseStamped=new Ros1Geometry_msgsPoseStamped();
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
/** Exact native field wrench, native scalar geometry_msgs/Wrench. */
wrench:Ros1Geometry_msgsWrench=new Ros1Geometry_msgsWrench();
/** Exact native field type, native scalar uint8. */
ros_type:u8=0;
/** Exact native field speed_limits, native scalar geometry_msgs/Twist. */
speed_limits:Ros1Geometry_msgsTwist=new Ros1Geometry_msgsTwist();
/** Exact native field deviation_limits, native scalar float32. */
deviation_limits:Array<f32>=new Array<f32>();
/** Exact native field damping_factor, native scalar float32. */
damping_factor:f32=0;
/** Exact native field gain_scaling, native scalar float32. */
gain_scaling:f32=0;
constructor(){
for(let i=0;i<6;++i)this.deviation_limits.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.task_frame.wireSize();if(child<0)return child;if(child>335-n)return -90;n+=child;
if(n>335)return -90;
}
{
if(1>335-n)return -90;n+=1;
if(n>335)return -90;
}
{
if(1>335-n)return -90;n+=1;
if(n>335)return -90;
}
{
if(1>335-n)return -90;n+=1;
if(n>335)return -90;
}
{
if(1>335-n)return -90;n+=1;
if(n>335)return -90;
}
{
if(1>335-n)return -90;n+=1;
if(n>335)return -90;
}
{
if(1>335-n)return -90;n+=1;
if(n>335)return -90;
}
{
const child=this.wrench.wireSize();if(child<0)return child;if(child>335-n)return -90;n+=child;
if(n>335)return -90;
}
{
if(1>335-n)return -90;n+=1;
if(n>335)return -90;
}
{
const child=this.speed_limits.wireSize();if(child<0)return child;if(child>335-n)return -90;n+=child;
if(n>335)return -90;
}
{
if(this.deviation_limits.length!=6)return -22;
for(let i=0;i<this.deviation_limits.length;++i){
if(!isFinite(this.deviation_limits[i]))return -22;
if(4>335-n)return -90;n+=4;
if(n>335)return -90;
}
}
{
if(!isFinite(this.damping_factor))return -22;
if(4>335-n)return -90;n+=4;
if(n>335)return -90;
}
{
if(!isFinite(this.gain_scaling))return -22;
if(4>335-n)return -90;n+=4;
if(n>335)return -90;
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
static readControl(r:Reader):Ros1Ur_msgsSetForceModeRequest{const value=new Ros1Ur_msgsSetForceModeRequest();
{
const decoded=Ros1Geometry_msgsPoseStamped.readControl(r);
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
const decoded=Ros1Geometry_msgsWrench.readControl(r);
value.wrench=decoded;
}
{
const decoded=<u8>r.integer(1);
value.ros_type=decoded;
}
{
const decoded=Ros1Geometry_msgsTwist.readControl(r);
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
export class Ros1Ur_msgsSetForceModeRequestCodec implements VendorCodec<Ros1Ur_msgsSetForceModeRequest>{
static readonly codecIdentity:string="UWVMROS1_c148d6eb37258e10f5d16e76e41c334fc57a3d751c0c35c47be4cabf4c015a6b";
static readonly maxWireBytes:i32=335;
encodeChecked(value:Ros1Ur_msgsSetForceModeRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsSetForceModeRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsSetForceModeRequest>{const out=new VendorDecoded<Ros1Ur_msgsSetForceModeRequest>();if(bytes.length>335){out.status=90;return out;}if(bytes.length<207){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsSetForceModeRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsSetForceModeRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsSetForceModeRequest():changetype<Ros1Ur_msgsSetForceModeRequest>(out.value);}
}
/** Complete original ur_msgs/SetForceModeResponse; spellings are published in generation proof. */
export class Ros1Ur_msgsSetForceModeResponse {
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
static readControl(r:Reader):Ros1Ur_msgsSetForceModeResponse{const value=new Ros1Ur_msgsSetForceModeResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros1Ur_msgsSetForceModeResponseCodec implements VendorCodec<Ros1Ur_msgsSetForceModeResponse>{
static readonly codecIdentity:string="UWVMROS1_bb1eeb0b38d81d642066bed1510245423f62d25983a2de89e985d1fd1932308c";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ur_msgsSetForceModeResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsSetForceModeResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsSetForceModeResponse>{const out=new VendorDecoded<Ros1Ur_msgsSetForceModeResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsSetForceModeResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsSetForceModeResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsSetForceModeResponse():changetype<Ros1Ur_msgsSetForceModeResponse>(out.value);}
}
/** Complete original ur_msgs/SetIORequest; spellings are published in generation proof. */
export class Ros1Ur_msgsSetIORequest {
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
static readControl(r:Reader):Ros1Ur_msgsSetIORequest{const value=new Ros1Ur_msgsSetIORequest();
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
export class Ros1Ur_msgsSetIORequestCodec implements VendorCodec<Ros1Ur_msgsSetIORequest>{
static readonly codecIdentity:string="UWVMROS1_3dbd2f05686ddaac16b37dc8ec98303acb672d176be34fc906cf32f2d4cd1872";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros1Ur_msgsSetIORequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsSetIORequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsSetIORequest>{const out=new VendorDecoded<Ros1Ur_msgsSetIORequest>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsSetIORequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsSetIORequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsSetIORequest():changetype<Ros1Ur_msgsSetIORequest>(out.value);}
}
/** Complete original ur_msgs/SetIOResponse; spellings are published in generation proof. */
export class Ros1Ur_msgsSetIOResponse {
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
static readControl(r:Reader):Ros1Ur_msgsSetIOResponse{const value=new Ros1Ur_msgsSetIOResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros1Ur_msgsSetIOResponseCodec implements VendorCodec<Ros1Ur_msgsSetIOResponse>{
static readonly codecIdentity:string="UWVMROS1_7fc137aa3db3178a450b8b2396b7821c911c79656e52bf48814745c261aecf61";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ur_msgsSetIOResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsSetIOResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsSetIOResponse>{const out=new VendorDecoded<Ros1Ur_msgsSetIOResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsSetIOResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsSetIOResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsSetIOResponse():changetype<Ros1Ur_msgsSetIOResponse>(out.value);}
}
/** Complete original ur_msgs/SetPayloadRequest; spellings are published in generation proof. */
export class Ros1Ur_msgsSetPayloadRequest {
static readonly maxWireBytes:i32=28;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=28;
/** Exact native field mass, native scalar float32. */
mass:f32=0;
/** Exact native field center_of_gravity, native scalar geometry_msgs/Vector3. */
center_of_gravity:Ros1Geometry_msgsVector3=new Ros1Geometry_msgsVector3();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.mass))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
const child=this.center_of_gravity.wireSize();if(child<0)return child;if(child>28-n)return -90;n+=child;
if(n>28)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.mass),4);
this.center_of_gravity.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ur_msgsSetPayloadRequest{const value=new Ros1Ur_msgsSetPayloadRequest();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.mass=decoded;
}
{
const decoded=Ros1Geometry_msgsVector3.readControl(r);
value.center_of_gravity=decoded;
}
return value;}
}
export class Ros1Ur_msgsSetPayloadRequestCodec implements VendorCodec<Ros1Ur_msgsSetPayloadRequest>{
static readonly codecIdentity:string="UWVMROS1_63ae68b44ce6aaad37cbda779fb51dee78923df27c582b4aab4104b16a307ee3";
static readonly maxWireBytes:i32=28;
encodeChecked(value:Ros1Ur_msgsSetPayloadRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsSetPayloadRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsSetPayloadRequest>{const out=new VendorDecoded<Ros1Ur_msgsSetPayloadRequest>();if(bytes.length>28){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsSetPayloadRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsSetPayloadRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsSetPayloadRequest():changetype<Ros1Ur_msgsSetPayloadRequest>(out.value);}
}
/** Complete original ur_msgs/SetPayloadResponse; spellings are published in generation proof. */
export class Ros1Ur_msgsSetPayloadResponse {
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
static readControl(r:Reader):Ros1Ur_msgsSetPayloadResponse{const value=new Ros1Ur_msgsSetPayloadResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros1Ur_msgsSetPayloadResponseCodec implements VendorCodec<Ros1Ur_msgsSetPayloadResponse>{
static readonly codecIdentity:string="UWVMROS1_a3e991f1357f2d3017ccbb7636a6248e8774434d723f5f6c77ff251dd61fce73";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ur_msgsSetPayloadResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsSetPayloadResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsSetPayloadResponse>{const out=new VendorDecoded<Ros1Ur_msgsSetPayloadResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsSetPayloadResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsSetPayloadResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsSetPayloadResponse():changetype<Ros1Ur_msgsSetPayloadResponse>(out.value);}
}
/** Complete original ur_msgs/SetSpeedSliderFractionRequest; spellings are published in generation proof. */
export class Ros1Ur_msgsSetSpeedSliderFractionRequest {
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
static readControl(r:Reader):Ros1Ur_msgsSetSpeedSliderFractionRequest{const value=new Ros1Ur_msgsSetSpeedSliderFractionRequest();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.speed_slider_fraction=decoded;
}
return value;}
}
export class Ros1Ur_msgsSetSpeedSliderFractionRequestCodec implements VendorCodec<Ros1Ur_msgsSetSpeedSliderFractionRequest>{
static readonly codecIdentity:string="UWVMROS1_f192d6d655b2ed7725bf5afe6977a352870abfd4d07e0c3380a09d280a0a0a32";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros1Ur_msgsSetSpeedSliderFractionRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsSetSpeedSliderFractionRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsSetSpeedSliderFractionRequest>{const out=new VendorDecoded<Ros1Ur_msgsSetSpeedSliderFractionRequest>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsSetSpeedSliderFractionRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsSetSpeedSliderFractionRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsSetSpeedSliderFractionRequest():changetype<Ros1Ur_msgsSetSpeedSliderFractionRequest>(out.value);}
}
/** Complete original ur_msgs/SetSpeedSliderFractionResponse; spellings are published in generation proof. */
export class Ros1Ur_msgsSetSpeedSliderFractionResponse {
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
static readControl(r:Reader):Ros1Ur_msgsSetSpeedSliderFractionResponse{const value=new Ros1Ur_msgsSetSpeedSliderFractionResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros1Ur_msgsSetSpeedSliderFractionResponseCodec implements VendorCodec<Ros1Ur_msgsSetSpeedSliderFractionResponse>{
static readonly codecIdentity:string="UWVMROS1_cb08c6758ce2eeeac93bc579d7895b19b84e74b7e2d19d87831023df579288b0";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ur_msgsSetSpeedSliderFractionResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsSetSpeedSliderFractionResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsSetSpeedSliderFractionResponse>{const out=new VendorDecoded<Ros1Ur_msgsSetSpeedSliderFractionResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsSetSpeedSliderFractionResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsSetSpeedSliderFractionResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsSetSpeedSliderFractionResponse():changetype<Ros1Ur_msgsSetSpeedSliderFractionResponse>(out.value);}
}
/** Complete original ur_msgs/ToolDataMsg; spellings are published in generation proof. */
export class Ros1Ur_msgsToolDataMsg {
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
static readControl(r:Reader):Ros1Ur_msgsToolDataMsg{const value=new Ros1Ur_msgsToolDataMsg();
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
export class Ros1Ur_msgsToolDataMsgCodec implements VendorCodec<Ros1Ur_msgsToolDataMsg>{
static readonly codecIdentity:string="UWVMROS1_1fad1aba069bc474306dc421a62950ffeb3a0837668f08a9adf41ac1bc2bb390";
static readonly maxWireBytes:i32=32;
encodeChecked(value:Ros1Ur_msgsToolDataMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ur_msgsToolDataMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ur_msgsToolDataMsg>{const out=new VendorDecoded<Ros1Ur_msgsToolDataMsg>();if(bytes.length>32){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ur_msgsToolDataMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ur_msgsToolDataMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ur_msgsToolDataMsg():changetype<Ros1Ur_msgsToolDataMsg>(out.value);}
}
