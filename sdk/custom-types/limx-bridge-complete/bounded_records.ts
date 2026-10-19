/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original uwvm_limx_bridge/msg/NativeHeader; spellings are published in generation proof. */
export class Ros2Uwvm_limx_bridgeMsgNativeHeader {
static readonly maxWireBytes:i32=144;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=144;
/** Exact native field seq, native scalar uint32. */
seq:u32=0;
/** Exact native field stamp_sec, native scalar uint32. */
stamp_sec:u32=0;
/** Exact native field stamp_nsec, native scalar uint32. */
stamp_nsec:u32=0;
/** Exact native field frame_id, native scalar string. */
frame_id:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>144-n)return -90;n+=4;
if(n>144)return -90;
}
{
if(4>144-n)return -90;n+=4;
if(n>144)return -90;
}
{
if(4>144-n)return -90;n+=4;
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
w.integer(<u64>this.stamp_sec,4);
w.integer(<u64>this.stamp_nsec,4);
w.text(this.frame_id);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_limx_bridgeMsgNativeHeader{const value=new Ros2Uwvm_limx_bridgeMsgNativeHeader();
{
const decoded=<u32>r.integer(4);
value.seq=decoded;
}
{
const decoded=<u32>r.integer(4);
value.stamp_sec=decoded;
}
{
const decoded=<u32>r.integer(4);
value.stamp_nsec=decoded;
}
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.frame_id=decoded;
}
return value;}
}
export class Ros2Uwvm_limx_bridgeMsgNativeHeaderCodec implements VendorCodec<Ros2Uwvm_limx_bridgeMsgNativeHeader>{
static readonly codecIdentity:string="UWVMROS1_d86b96b26db6c2a28dfa86e2255cb3cccd6cd24648e877746d8d75394d1635b2";
static readonly maxWireBytes:i32=144;
encodeChecked(value:Ros2Uwvm_limx_bridgeMsgNativeHeader):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_limx_bridgeMsgNativeHeader):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_limx_bridgeMsgNativeHeader>{const out=new VendorDecoded<Ros2Uwvm_limx_bridgeMsgNativeHeader>();if(bytes.length>144){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_limx_bridgeMsgNativeHeader.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_limx_bridgeMsgNativeHeader{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_limx_bridgeMsgNativeHeader():changetype<Ros2Uwvm_limx_bridgeMsgNativeHeader>(out.value);}
}
/** Complete original uwvm_limx_bridge/msg/ImuData; spellings are published in generation proof. */
export class Ros2Uwvm_limx_bridgeMsgImuData {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field header, native scalar uwvm_limx_bridge/msg/NativeHeader. */
header:Ros2Uwvm_limx_bridgeMsgNativeHeader=new Ros2Uwvm_limx_bridgeMsgNativeHeader();
/** Exact native field imustamp, native scalar uint64. */
imustamp:u64=0;
/** Exact native field status, native scalar uint32. */
status:u32=0;
/** Exact native field euler, native scalar float64. */
euler:Array<f64>=new Array<f64>();
/** Exact native field quat, native scalar float64. */
quat:Array<f64>=new Array<f64>();
/** Exact native field acc, native scalar float64. */
acc:Array<f64>=new Array<f64>();
/** Exact native field gyro, native scalar float64. */
gyro:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<3;++i)this.euler.push(0);
for(let i=0;i<4;++i)this.quat.push(0);
for(let i=0;i<3;++i)this.acc.push(0);
for(let i=0;i<3;++i)this.gyro.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>260-n)return -90;n+=child;
if(n>260)return -90;
}
{
if(8>260-n)return -90;n+=8;
if(n>260)return -90;
}
{
if(4>260-n)return -90;n+=4;
if(n>260)return -90;
}
{
if(this.euler.length!=3)return -22;
for(let i=0;i<this.euler.length;++i){
if(!isFinite(this.euler[i]))return -22;
if(8>260-n)return -90;n+=8;
if(n>260)return -90;
}
}
{
if(this.quat.length!=4)return -22;
for(let i=0;i<this.quat.length;++i){
if(!isFinite(this.quat[i]))return -22;
if(8>260-n)return -90;n+=8;
if(n>260)return -90;
}
}
{
if(this.acc.length!=3)return -22;
for(let i=0;i<this.acc.length;++i){
if(!isFinite(this.acc[i]))return -22;
if(8>260-n)return -90;n+=8;
if(n>260)return -90;
}
}
{
if(this.gyro.length!=3)return -22;
for(let i=0;i<this.gyro.length;++i){
if(!isFinite(this.gyro[i]))return -22;
if(8>260-n)return -90;n+=8;
if(n>260)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>this.imustamp,8);
w.integer(<u64>this.status,4);
for(let i=0;i<this.euler.length;++i){
w.integer(<u64>reinterpret<u64>(this.euler[i]),8);
}
for(let i=0;i<this.quat.length;++i){
w.integer(<u64>reinterpret<u64>(this.quat[i]),8);
}
for(let i=0;i<this.acc.length;++i){
w.integer(<u64>reinterpret<u64>(this.acc[i]),8);
}
for(let i=0;i<this.gyro.length;++i){
w.integer(<u64>reinterpret<u64>(this.gyro[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_limx_bridgeMsgImuData{const value=new Ros2Uwvm_limx_bridgeMsgImuData();
{
const decoded=Ros2Uwvm_limx_bridgeMsgNativeHeader.readControl(r);
value.header=decoded;
}
{
const decoded=<u64>r.integer(8);
value.imustamp=decoded;
}
{
const decoded=<u32>r.integer(4);
value.status=decoded;
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.euler=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.euler.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.quat=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.quat.push(decoded);
}
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.acc=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.acc.push(decoded);
}
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.gyro=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.gyro.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_limx_bridgeMsgImuDataCodec implements VendorCodec<Ros2Uwvm_limx_bridgeMsgImuData>{
static readonly codecIdentity:string="UWVMROS1_53ba9eb23b36de77e1793bdd19a31f708aca557b81f4ae72927b2a44f5ce097b";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Uwvm_limx_bridgeMsgImuData):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_limx_bridgeMsgImuData):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_limx_bridgeMsgImuData>{const out=new VendorDecoded<Ros2Uwvm_limx_bridgeMsgImuData>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<132){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_limx_bridgeMsgImuData.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_limx_bridgeMsgImuData{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_limx_bridgeMsgImuData():changetype<Ros2Uwvm_limx_bridgeMsgImuData>(out.value);}
}
/** Complete original uwvm_limx_bridge/msg/JointState; spellings are published in generation proof. */
export class Ros2Uwvm_limx_bridgeMsgJointState {
static readonly maxWireBytes:i32=5544;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5544;
/** Exact native field header, native scalar uwvm_limx_bridge/msg/NativeHeader. */
header:Ros2Uwvm_limx_bridgeMsgNativeHeader=new Ros2Uwvm_limx_bridgeMsgNativeHeader();
/** Exact native field names, native scalar string. */
names:Array<string>=new Array<string>();
/** Exact native field q, native scalar float32. */
q:Array<f32>=new Array<f32>();
/** Exact native field v, native scalar float32. */
v:Array<f32>=new Array<f32>();
/** Exact native field vd, native scalar float32. */
vd:Array<f32>=new Array<f32>();
/** Exact native field tau, native scalar float32. */
tau:Array<f32>=new Array<f32>();
/** Exact native field na, native scalar uint32. */
na:u32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>5544-n)return -90;n+=child;
if(n>5544)return -90;
}
{
if(this.names.length>64)return -22;
if(4>5544-n)return -90;n+=4;
for(let i=0;i<this.names.length;++i){
if(!validUnicode(this.names[i])||this.names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.names[i]);if(size>64)return -22;if(4+size>5544-n)return -90;n+=4+size;
if(n>5544)return -90;
}
}
{
if(this.q.length>64)return -22;
if(4>5544-n)return -90;n+=4;
for(let i=0;i<this.q.length;++i){
if(!isFinite(this.q[i]))return -22;
if(4>5544-n)return -90;n+=4;
if(n>5544)return -90;
}
}
{
if(this.v.length>64)return -22;
if(4>5544-n)return -90;n+=4;
for(let i=0;i<this.v.length;++i){
if(!isFinite(this.v[i]))return -22;
if(4>5544-n)return -90;n+=4;
if(n>5544)return -90;
}
}
{
if(this.vd.length>64)return -22;
if(4>5544-n)return -90;n+=4;
for(let i=0;i<this.vd.length;++i){
if(!isFinite(this.vd[i]))return -22;
if(4>5544-n)return -90;n+=4;
if(n>5544)return -90;
}
}
{
if(this.tau.length>64)return -22;
if(4>5544-n)return -90;n+=4;
for(let i=0;i<this.tau.length;++i){
if(!isFinite(this.tau[i]))return -22;
if(4>5544-n)return -90;n+=4;
if(n>5544)return -90;
}
}
{
if(4>5544-n)return -90;n+=4;
if(n>5544)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.names.length);
for(let i=0;i<this.names.length;++i){
w.text(this.names[i]);
}
w.u32(<u32>this.q.length);
for(let i=0;i<this.q.length;++i){
w.integer(<u64>reinterpret<u32>(this.q[i]),4);
}
w.u32(<u32>this.v.length);
for(let i=0;i<this.v.length;++i){
w.integer(<u64>reinterpret<u32>(this.v[i]),4);
}
w.u32(<u32>this.vd.length);
for(let i=0;i<this.vd.length;++i){
w.integer(<u64>reinterpret<u32>(this.vd[i]),4);
}
w.u32(<u32>this.tau.length);
for(let i=0;i<this.tau.length;++i){
w.integer(<u64>reinterpret<u32>(this.tau[i]),4);
}
w.integer(<u64>this.na,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_limx_bridgeMsgJointState{const value=new Ros2Uwvm_limx_bridgeMsgJointState();
{
const decoded=Ros2Uwvm_limx_bridgeMsgNativeHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.names.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.q=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.q.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.v=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.v.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.vd=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.vd.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.tau=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.tau.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.na=decoded;
}
return value;}
}
export class Ros2Uwvm_limx_bridgeMsgJointStateCodec implements VendorCodec<Ros2Uwvm_limx_bridgeMsgJointState>{
static readonly codecIdentity:string="UWVMROS1_d99e5855c0dd55d4d1b5a724ad5719734a668a512ea09e553c8f4a560b97a3a4";
static readonly maxWireBytes:i32=5544;
encodeChecked(value:Ros2Uwvm_limx_bridgeMsgJointState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_limx_bridgeMsgJointState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_limx_bridgeMsgJointState>{const out=new VendorDecoded<Ros2Uwvm_limx_bridgeMsgJointState>();if(bytes.length>5544){out.status=90;return out;}if(bytes.length<40){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_limx_bridgeMsgJointState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_limx_bridgeMsgJointState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_limx_bridgeMsgJointState():changetype<Ros2Uwvm_limx_bridgeMsgJointState>(out.value);}
}
/** Complete original uwvm_limx_bridge/msg/JointCmd; spellings are published in generation proof. */
export class Ros2Uwvm_limx_bridgeMsgJointCmd {
static readonly maxWireBytes:i32=5872;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5872;
/** Exact native field header, native scalar uwvm_limx_bridge/msg/NativeHeader. */
header:Ros2Uwvm_limx_bridgeMsgNativeHeader=new Ros2Uwvm_limx_bridgeMsgNativeHeader();
/** Exact native field names, native scalar string. */
names:Array<string>=new Array<string>();
/** Exact native field q, native scalar float32. */
q:Array<f32>=new Array<f32>();
/** Exact native field v, native scalar float32. */
v:Array<f32>=new Array<f32>();
/** Exact native field tau, native scalar float32. */
tau:Array<f32>=new Array<f32>();
/** Exact native field kp, native scalar float32. */
kp:Array<f32>=new Array<f32>();
/** Exact native field kd, native scalar float32. */
kd:Array<f32>=new Array<f32>();
/** Exact native field mode, native scalar uint8. */
mode:Array<u8>=new Array<u8>();
/** Exact native field na, native scalar uint32. */
na:u32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>5872-n)return -90;n+=child;
if(n>5872)return -90;
}
{
if(this.names.length>64)return -22;
if(4>5872-n)return -90;n+=4;
for(let i=0;i<this.names.length;++i){
if(!validUnicode(this.names[i])||this.names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.names[i]);if(size>64)return -22;if(4+size>5872-n)return -90;n+=4+size;
if(n>5872)return -90;
}
}
{
if(this.q.length>64)return -22;
if(4>5872-n)return -90;n+=4;
for(let i=0;i<this.q.length;++i){
if(!isFinite(this.q[i]))return -22;
if(4>5872-n)return -90;n+=4;
if(n>5872)return -90;
}
}
{
if(this.v.length>64)return -22;
if(4>5872-n)return -90;n+=4;
for(let i=0;i<this.v.length;++i){
if(!isFinite(this.v[i]))return -22;
if(4>5872-n)return -90;n+=4;
if(n>5872)return -90;
}
}
{
if(this.tau.length>64)return -22;
if(4>5872-n)return -90;n+=4;
for(let i=0;i<this.tau.length;++i){
if(!isFinite(this.tau[i]))return -22;
if(4>5872-n)return -90;n+=4;
if(n>5872)return -90;
}
}
{
if(this.kp.length>64)return -22;
if(4>5872-n)return -90;n+=4;
for(let i=0;i<this.kp.length;++i){
if(!isFinite(this.kp[i]))return -22;
if(4>5872-n)return -90;n+=4;
if(n>5872)return -90;
}
}
{
if(this.kd.length>64)return -22;
if(4>5872-n)return -90;n+=4;
for(let i=0;i<this.kd.length;++i){
if(!isFinite(this.kd[i]))return -22;
if(4>5872-n)return -90;n+=4;
if(n>5872)return -90;
}
}
{
if(this.mode.length>64)return -22;
if(4>5872-n)return -90;n+=4;
for(let i=0;i<this.mode.length;++i){
if(1>5872-n)return -90;n+=1;
if(n>5872)return -90;
}
}
{
if(4>5872-n)return -90;n+=4;
if(n>5872)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.names.length);
for(let i=0;i<this.names.length;++i){
w.text(this.names[i]);
}
w.u32(<u32>this.q.length);
for(let i=0;i<this.q.length;++i){
w.integer(<u64>reinterpret<u32>(this.q[i]),4);
}
w.u32(<u32>this.v.length);
for(let i=0;i<this.v.length;++i){
w.integer(<u64>reinterpret<u32>(this.v[i]),4);
}
w.u32(<u32>this.tau.length);
for(let i=0;i<this.tau.length;++i){
w.integer(<u64>reinterpret<u32>(this.tau[i]),4);
}
w.u32(<u32>this.kp.length);
for(let i=0;i<this.kp.length;++i){
w.integer(<u64>reinterpret<u32>(this.kp[i]),4);
}
w.u32(<u32>this.kd.length);
for(let i=0;i<this.kd.length;++i){
w.integer(<u64>reinterpret<u32>(this.kd[i]),4);
}
w.u32(<u32>this.mode.length);
for(let i=0;i<this.mode.length;++i){
w.integer(<u64>this.mode[i],1);
}
w.integer(<u64>this.na,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_limx_bridgeMsgJointCmd{const value=new Ros2Uwvm_limx_bridgeMsgJointCmd();
{
const decoded=Ros2Uwvm_limx_bridgeMsgNativeHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.names.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.q=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.q.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.v=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.v.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.tau=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.tau.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.kp=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.kp.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.kd=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.kd.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.mode=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.mode.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.na=decoded;
}
return value;}
}
export class Ros2Uwvm_limx_bridgeMsgJointCmdCodec implements VendorCodec<Ros2Uwvm_limx_bridgeMsgJointCmd>{
static readonly codecIdentity:string="UWVMROS1_8c477a93cc192cd3039b285f111a56eb1305707d798587ef7ab9504f714ce6f0";
static readonly maxWireBytes:i32=5872;
encodeChecked(value:Ros2Uwvm_limx_bridgeMsgJointCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_limx_bridgeMsgJointCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_limx_bridgeMsgJointCmd>{const out=new VendorDecoded<Ros2Uwvm_limx_bridgeMsgJointCmd>();if(bytes.length>5872){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_limx_bridgeMsgJointCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_limx_bridgeMsgJointCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_limx_bridgeMsgJointCmd():changetype<Ros2Uwvm_limx_bridgeMsgJointCmd>(out.value);}
}
/** Complete original uwvm_limx_bridge/msg/JointCmdLimx; spellings are published in generation proof. */
export class Ros2Uwvm_limx_bridgeMsgJointCmdLimx {
static readonly maxWireBytes:i32=5940;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5940;
/** Exact native field header, native scalar uwvm_limx_bridge/msg/NativeHeader. */
header:Ros2Uwvm_limx_bridgeMsgNativeHeader=new Ros2Uwvm_limx_bridgeMsgNativeHeader();
/** Exact native field names, native scalar string. */
names:Array<string>=new Array<string>();
/** Exact native field q, native scalar float32. */
q:Array<f32>=new Array<f32>();
/** Exact native field v, native scalar float32. */
v:Array<f32>=new Array<f32>();
/** Exact native field tau, native scalar float32. */
tau:Array<f32>=new Array<f32>();
/** Exact native field kp, native scalar float32. */
kp:Array<f32>=new Array<f32>();
/** Exact native field kd, native scalar float32. */
kd:Array<f32>=new Array<f32>();
/** Exact native field mode, native scalar uint8. */
mode:Array<u8>=new Array<u8>();
/** Exact native field na, native scalar uint32. */
na:u32=0;
/** Exact native field parallel_solver_mode, native scalar uint8. */
parallel_solver_mode:Array<u8>=new Array<u8>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>5940-n)return -90;n+=child;
if(n>5940)return -90;
}
{
if(this.names.length>64)return -22;
if(4>5940-n)return -90;n+=4;
for(let i=0;i<this.names.length;++i){
if(!validUnicode(this.names[i])||this.names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.names[i]);if(size>64)return -22;if(4+size>5940-n)return -90;n+=4+size;
if(n>5940)return -90;
}
}
{
if(this.q.length>64)return -22;
if(4>5940-n)return -90;n+=4;
for(let i=0;i<this.q.length;++i){
if(!isFinite(this.q[i]))return -22;
if(4>5940-n)return -90;n+=4;
if(n>5940)return -90;
}
}
{
if(this.v.length>64)return -22;
if(4>5940-n)return -90;n+=4;
for(let i=0;i<this.v.length;++i){
if(!isFinite(this.v[i]))return -22;
if(4>5940-n)return -90;n+=4;
if(n>5940)return -90;
}
}
{
if(this.tau.length>64)return -22;
if(4>5940-n)return -90;n+=4;
for(let i=0;i<this.tau.length;++i){
if(!isFinite(this.tau[i]))return -22;
if(4>5940-n)return -90;n+=4;
if(n>5940)return -90;
}
}
{
if(this.kp.length>64)return -22;
if(4>5940-n)return -90;n+=4;
for(let i=0;i<this.kp.length;++i){
if(!isFinite(this.kp[i]))return -22;
if(4>5940-n)return -90;n+=4;
if(n>5940)return -90;
}
}
{
if(this.kd.length>64)return -22;
if(4>5940-n)return -90;n+=4;
for(let i=0;i<this.kd.length;++i){
if(!isFinite(this.kd[i]))return -22;
if(4>5940-n)return -90;n+=4;
if(n>5940)return -90;
}
}
{
if(this.mode.length>64)return -22;
if(4>5940-n)return -90;n+=4;
for(let i=0;i<this.mode.length;++i){
if(1>5940-n)return -90;n+=1;
if(n>5940)return -90;
}
}
{
if(4>5940-n)return -90;n+=4;
if(n>5940)return -90;
}
{
if(this.parallel_solver_mode.length>64)return -22;
if(4>5940-n)return -90;n+=4;
for(let i=0;i<this.parallel_solver_mode.length;++i){
if(1>5940-n)return -90;n+=1;
if(n>5940)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.names.length);
for(let i=0;i<this.names.length;++i){
w.text(this.names[i]);
}
w.u32(<u32>this.q.length);
for(let i=0;i<this.q.length;++i){
w.integer(<u64>reinterpret<u32>(this.q[i]),4);
}
w.u32(<u32>this.v.length);
for(let i=0;i<this.v.length;++i){
w.integer(<u64>reinterpret<u32>(this.v[i]),4);
}
w.u32(<u32>this.tau.length);
for(let i=0;i<this.tau.length;++i){
w.integer(<u64>reinterpret<u32>(this.tau[i]),4);
}
w.u32(<u32>this.kp.length);
for(let i=0;i<this.kp.length;++i){
w.integer(<u64>reinterpret<u32>(this.kp[i]),4);
}
w.u32(<u32>this.kd.length);
for(let i=0;i<this.kd.length;++i){
w.integer(<u64>reinterpret<u32>(this.kd[i]),4);
}
w.u32(<u32>this.mode.length);
for(let i=0;i<this.mode.length;++i){
w.integer(<u64>this.mode[i],1);
}
w.integer(<u64>this.na,4);
w.u32(<u32>this.parallel_solver_mode.length);
for(let i=0;i<this.parallel_solver_mode.length;++i){
w.integer(<u64>this.parallel_solver_mode[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_limx_bridgeMsgJointCmdLimx{const value=new Ros2Uwvm_limx_bridgeMsgJointCmdLimx();
{
const decoded=Ros2Uwvm_limx_bridgeMsgNativeHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.names.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.q=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.q.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.v=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.v.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.tau=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.tau.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.kp=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.kp.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.kd=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.kd.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.mode=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.mode.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.na=decoded;
}
{
const count=r.u32();if(!r.valid||count>64){r.valid=false;return value;}
value.parallel_solver_mode=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.parallel_solver_mode.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_limx_bridgeMsgJointCmdLimxCodec implements VendorCodec<Ros2Uwvm_limx_bridgeMsgJointCmdLimx>{
static readonly codecIdentity:string="UWVMROS1_d469dd32f446fb4013144b7e176cdb831c146b6a9279ea776ec4a0c053060be9";
static readonly maxWireBytes:i32=5940;
encodeChecked(value:Ros2Uwvm_limx_bridgeMsgJointCmdLimx):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_limx_bridgeMsgJointCmdLimx):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_limx_bridgeMsgJointCmdLimx>{const out=new VendorDecoded<Ros2Uwvm_limx_bridgeMsgJointCmdLimx>();if(bytes.length>5940){out.status=90;return out;}if(bytes.length<52){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_limx_bridgeMsgJointCmdLimx.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_limx_bridgeMsgJointCmdLimx{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_limx_bridgeMsgJointCmdLimx():changetype<Ros2Uwvm_limx_bridgeMsgJointCmdLimx>(out.value);}
}
