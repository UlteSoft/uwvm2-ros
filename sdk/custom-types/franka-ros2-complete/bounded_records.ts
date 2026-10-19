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
/** Complete original franka_msgs/action/ErrorRecovery_Goal; spellings are published in generation proof. */
export class Ros2Franka_msgsActionErrorRecovery_Goal {
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
static readControl(r:Reader):Ros2Franka_msgsActionErrorRecovery_Goal{const value=new Ros2Franka_msgsActionErrorRecovery_Goal();
return value;}
}
export class Ros2Franka_msgsActionErrorRecovery_GoalCodec implements VendorCodec<Ros2Franka_msgsActionErrorRecovery_Goal>{
static readonly codecIdentity:string="UWVMROS1_2caee3e8d5c04f12239a71f6e9a9259b818b9f1ad75103442bfc9d2343a3a070";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Franka_msgsActionErrorRecovery_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionErrorRecovery_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionErrorRecovery_Goal>{const out=new VendorDecoded<Ros2Franka_msgsActionErrorRecovery_Goal>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionErrorRecovery_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionErrorRecovery_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionErrorRecovery_Goal():changetype<Ros2Franka_msgsActionErrorRecovery_Goal>(out.value);}
}
/** Complete original franka_msgs/action/ErrorRecovery_Result; spellings are published in generation proof. */
export class Ros2Franka_msgsActionErrorRecovery_Result {
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
static readControl(r:Reader):Ros2Franka_msgsActionErrorRecovery_Result{const value=new Ros2Franka_msgsActionErrorRecovery_Result();
return value;}
}
export class Ros2Franka_msgsActionErrorRecovery_ResultCodec implements VendorCodec<Ros2Franka_msgsActionErrorRecovery_Result>{
static readonly codecIdentity:string="UWVMROS1_9112b16c42034737b96214f23249d499ab6a9f0aee529cfec34c67cdc76809e9";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Franka_msgsActionErrorRecovery_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionErrorRecovery_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionErrorRecovery_Result>{const out=new VendorDecoded<Ros2Franka_msgsActionErrorRecovery_Result>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionErrorRecovery_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionErrorRecovery_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionErrorRecovery_Result():changetype<Ros2Franka_msgsActionErrorRecovery_Result>(out.value);}
}
/** Complete original franka_msgs/action/ErrorRecovery_Feedback; spellings are published in generation proof. */
export class Ros2Franka_msgsActionErrorRecovery_Feedback {
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
static readControl(r:Reader):Ros2Franka_msgsActionErrorRecovery_Feedback{const value=new Ros2Franka_msgsActionErrorRecovery_Feedback();
return value;}
}
export class Ros2Franka_msgsActionErrorRecovery_FeedbackCodec implements VendorCodec<Ros2Franka_msgsActionErrorRecovery_Feedback>{
static readonly codecIdentity:string="UWVMROS1_cc817bcbe5790860e83c71e50a8c3610cf3965e34036b3099712cdcdb426127f";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Franka_msgsActionErrorRecovery_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionErrorRecovery_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionErrorRecovery_Feedback>{const out=new VendorDecoded<Ros2Franka_msgsActionErrorRecovery_Feedback>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionErrorRecovery_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionErrorRecovery_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionErrorRecovery_Feedback():changetype<Ros2Franka_msgsActionErrorRecovery_Feedback>(out.value);}
}
/** Complete original franka_msgs/msg/GraspEpsilon; spellings are published in generation proof. */
export class Ros2Franka_msgsMsgGraspEpsilon {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field inner, native scalar float64. */
inner:f64=reinterpret<f64>(u64(0x3f747ae147ae147b));
/** Exact native field outer, native scalar float64. */
outer:f64=reinterpret<f64>(u64(0x3f747ae147ae147b));
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.inner))return -22;
if(8>16-n)return -90;n+=8;
if(n>16)return -90;
}
{
if(!isFinite(this.outer))return -22;
if(8>16-n)return -90;n+=8;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.inner),8);
w.integer(<u64>reinterpret<u64>(this.outer),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsMsgGraspEpsilon{const value=new Ros2Franka_msgsMsgGraspEpsilon();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.inner=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.outer=decoded;
}
return value;}
}
export class Ros2Franka_msgsMsgGraspEpsilonCodec implements VendorCodec<Ros2Franka_msgsMsgGraspEpsilon>{
static readonly codecIdentity:string="UWVMROS1_21787ae5929dc83b9f644f4914e22da9ddd3e4f7ad3d8c6cda6bbcdfeba20c38";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Franka_msgsMsgGraspEpsilon):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsMsgGraspEpsilon):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsMsgGraspEpsilon>{const out=new VendorDecoded<Ros2Franka_msgsMsgGraspEpsilon>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsMsgGraspEpsilon.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsMsgGraspEpsilon{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsMsgGraspEpsilon():changetype<Ros2Franka_msgsMsgGraspEpsilon>(out.value);}
}
/** Complete original franka_msgs/action/Grasp_Goal; spellings are published in generation proof. */
export class Ros2Franka_msgsActionGrasp_Goal {
static readonly maxWireBytes:i32=40;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=40;
/** Exact native field width, native scalar float64. */
width:f64=0;
/** Exact native field epsilon, native scalar franka_msgs/msg/GraspEpsilon. */
epsilon:Ros2Franka_msgsMsgGraspEpsilon=new Ros2Franka_msgsMsgGraspEpsilon();
/** Exact native field speed, native scalar float64. */
speed:f64=0;
/** Exact native field force, native scalar float64. */
force:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.width))return -22;
if(8>40-n)return -90;n+=8;
if(n>40)return -90;
}
{
const child=this.epsilon.wireSize();if(child<0)return child;if(child>40-n)return -90;n+=child;
if(n>40)return -90;
}
{
if(!isFinite(this.speed))return -22;
if(8>40-n)return -90;n+=8;
if(n>40)return -90;
}
{
if(!isFinite(this.force))return -22;
if(8>40-n)return -90;n+=8;
if(n>40)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.width),8);
this.epsilon.writeControl(w);
w.integer(<u64>reinterpret<u64>(this.speed),8);
w.integer(<u64>reinterpret<u64>(this.force),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsActionGrasp_Goal{const value=new Ros2Franka_msgsActionGrasp_Goal();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.width=decoded;
}
{
const decoded=Ros2Franka_msgsMsgGraspEpsilon.readControl(r);
value.epsilon=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.speed=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.force=decoded;
}
return value;}
}
export class Ros2Franka_msgsActionGrasp_GoalCodec implements VendorCodec<Ros2Franka_msgsActionGrasp_Goal>{
static readonly codecIdentity:string="UWVMROS1_b7ef701e9fd93b5da9894499234d3647ecc4cea1936f637105c172f4579b6e1f";
static readonly maxWireBytes:i32=40;
encodeChecked(value:Ros2Franka_msgsActionGrasp_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionGrasp_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionGrasp_Goal>{const out=new VendorDecoded<Ros2Franka_msgsActionGrasp_Goal>();if(bytes.length>40){out.status=90;return out;}if(bytes.length<40){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionGrasp_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionGrasp_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionGrasp_Goal():changetype<Ros2Franka_msgsActionGrasp_Goal>(out.value);}
}
/** Complete original franka_msgs/action/Grasp_Result; spellings are published in generation proof. */
export class Ros2Franka_msgsActionGrasp_Result {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field error, native scalar string. */
error:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.error)||this.error.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.error);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsActionGrasp_Result{const value=new Ros2Franka_msgsActionGrasp_Result();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error=decoded;
}
return value;}
}
export class Ros2Franka_msgsActionGrasp_ResultCodec implements VendorCodec<Ros2Franka_msgsActionGrasp_Result>{
static readonly codecIdentity:string="UWVMROS1_9a4c3150c0c2d5eb8ff3dad403473a9546ee37cc3086a55a628a1a85b905b733";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Franka_msgsActionGrasp_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionGrasp_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionGrasp_Result>{const out=new VendorDecoded<Ros2Franka_msgsActionGrasp_Result>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionGrasp_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionGrasp_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionGrasp_Result():changetype<Ros2Franka_msgsActionGrasp_Result>(out.value);}
}
/** Complete original franka_msgs/action/Grasp_Feedback; spellings are published in generation proof. */
export class Ros2Franka_msgsActionGrasp_Feedback {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field current_width, native scalar float64. */
current_width:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.current_width))return -22;
if(8>8-n)return -90;n+=8;
if(n>8)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.current_width),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsActionGrasp_Feedback{const value=new Ros2Franka_msgsActionGrasp_Feedback();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.current_width=decoded;
}
return value;}
}
export class Ros2Franka_msgsActionGrasp_FeedbackCodec implements VendorCodec<Ros2Franka_msgsActionGrasp_Feedback>{
static readonly codecIdentity:string="UWVMROS1_2e7697286c002e2d891e9c3167aa0dd1664acfc66c649fff6e8f9101c42703fe";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Franka_msgsActionGrasp_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionGrasp_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionGrasp_Feedback>{const out=new VendorDecoded<Ros2Franka_msgsActionGrasp_Feedback>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionGrasp_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionGrasp_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionGrasp_Feedback():changetype<Ros2Franka_msgsActionGrasp_Feedback>(out.value);}
}
/** Complete original franka_msgs/action/Homing_Goal; spellings are published in generation proof. */
export class Ros2Franka_msgsActionHoming_Goal {
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
static readControl(r:Reader):Ros2Franka_msgsActionHoming_Goal{const value=new Ros2Franka_msgsActionHoming_Goal();
return value;}
}
export class Ros2Franka_msgsActionHoming_GoalCodec implements VendorCodec<Ros2Franka_msgsActionHoming_Goal>{
static readonly codecIdentity:string="UWVMROS1_8008426ce3fa8c8d62133704f8738201b8154b324ba07e7b6f605cd3132a885d";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Franka_msgsActionHoming_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionHoming_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionHoming_Goal>{const out=new VendorDecoded<Ros2Franka_msgsActionHoming_Goal>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionHoming_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionHoming_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionHoming_Goal():changetype<Ros2Franka_msgsActionHoming_Goal>(out.value);}
}
/** Complete original franka_msgs/action/Homing_Result; spellings are published in generation proof. */
export class Ros2Franka_msgsActionHoming_Result {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field error, native scalar string. */
error:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.error)||this.error.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.error);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsActionHoming_Result{const value=new Ros2Franka_msgsActionHoming_Result();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error=decoded;
}
return value;}
}
export class Ros2Franka_msgsActionHoming_ResultCodec implements VendorCodec<Ros2Franka_msgsActionHoming_Result>{
static readonly codecIdentity:string="UWVMROS1_74a9677213ffa9ba3a26b3053956350b8f75d5d1769152e5e3d4d26370bcd77c";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Franka_msgsActionHoming_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionHoming_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionHoming_Result>{const out=new VendorDecoded<Ros2Franka_msgsActionHoming_Result>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionHoming_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionHoming_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionHoming_Result():changetype<Ros2Franka_msgsActionHoming_Result>(out.value);}
}
/** Complete original franka_msgs/action/Homing_Feedback; spellings are published in generation proof. */
export class Ros2Franka_msgsActionHoming_Feedback {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field current_width, native scalar float64. */
current_width:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.current_width))return -22;
if(8>8-n)return -90;n+=8;
if(n>8)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.current_width),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsActionHoming_Feedback{const value=new Ros2Franka_msgsActionHoming_Feedback();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.current_width=decoded;
}
return value;}
}
export class Ros2Franka_msgsActionHoming_FeedbackCodec implements VendorCodec<Ros2Franka_msgsActionHoming_Feedback>{
static readonly codecIdentity:string="UWVMROS1_2260e092d369a0d2e4bf97ca2dfa78c02b6836b8167e71d63e76b5c0c40a6b46";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Franka_msgsActionHoming_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionHoming_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionHoming_Feedback>{const out=new VendorDecoded<Ros2Franka_msgsActionHoming_Feedback>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionHoming_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionHoming_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionHoming_Feedback():changetype<Ros2Franka_msgsActionHoming_Feedback>(out.value);}
}
/** Complete original franka_msgs/action/Move_Goal; spellings are published in generation proof. */
export class Ros2Franka_msgsActionMove_Goal {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field width, native scalar float64. */
width:f64=0;
/** Exact native field speed, native scalar float64. */
speed:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.width))return -22;
if(8>16-n)return -90;n+=8;
if(n>16)return -90;
}
{
if(!isFinite(this.speed))return -22;
if(8>16-n)return -90;n+=8;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.width),8);
w.integer(<u64>reinterpret<u64>(this.speed),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsActionMove_Goal{const value=new Ros2Franka_msgsActionMove_Goal();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.width=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.speed=decoded;
}
return value;}
}
export class Ros2Franka_msgsActionMove_GoalCodec implements VendorCodec<Ros2Franka_msgsActionMove_Goal>{
static readonly codecIdentity:string="UWVMROS1_9f9e308ebd034344ee73b22a07d005944975c037943279504b2414bdd9ad206f";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Franka_msgsActionMove_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionMove_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionMove_Goal>{const out=new VendorDecoded<Ros2Franka_msgsActionMove_Goal>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionMove_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionMove_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionMove_Goal():changetype<Ros2Franka_msgsActionMove_Goal>(out.value);}
}
/** Complete original franka_msgs/action/Move_Result; spellings are published in generation proof. */
export class Ros2Franka_msgsActionMove_Result {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field error, native scalar string. */
error:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.error)||this.error.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.error);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsActionMove_Result{const value=new Ros2Franka_msgsActionMove_Result();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error=decoded;
}
return value;}
}
export class Ros2Franka_msgsActionMove_ResultCodec implements VendorCodec<Ros2Franka_msgsActionMove_Result>{
static readonly codecIdentity:string="UWVMROS1_66aa505c035357274c68be773e1b502c7d77f195f3dca848e818811ce95dd266";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Franka_msgsActionMove_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionMove_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionMove_Result>{const out=new VendorDecoded<Ros2Franka_msgsActionMove_Result>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionMove_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionMove_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionMove_Result():changetype<Ros2Franka_msgsActionMove_Result>(out.value);}
}
/** Complete original franka_msgs/action/Move_Feedback; spellings are published in generation proof. */
export class Ros2Franka_msgsActionMove_Feedback {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field current_width, native scalar float64. */
current_width:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.current_width))return -22;
if(8>8-n)return -90;n+=8;
if(n>8)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.current_width),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsActionMove_Feedback{const value=new Ros2Franka_msgsActionMove_Feedback();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.current_width=decoded;
}
return value;}
}
export class Ros2Franka_msgsActionMove_FeedbackCodec implements VendorCodec<Ros2Franka_msgsActionMove_Feedback>{
static readonly codecIdentity:string="UWVMROS1_18d0e658f109492380c135e787adbdcc026b051e9a547fcea70453bbd9750596";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Franka_msgsActionMove_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionMove_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionMove_Feedback>{const out=new VendorDecoded<Ros2Franka_msgsActionMove_Feedback>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionMove_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionMove_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionMove_Feedback():changetype<Ros2Franka_msgsActionMove_Feedback>(out.value);}
}
/** Complete original franka_msgs/action/PTPMotion_Goal; spellings are published in generation proof. */
export class Ros2Franka_msgsActionPTPMotion_Goal {
static readonly maxWireBytes:i32=128;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=128;
/** Exact native field goal_joint_configuration, native scalar float64. */
goal_joint_configuration:Array<f64>=new Array<f64>();
/** Exact native field maximum_joint_velocities, native scalar float64. */
maximum_joint_velocities:Array<f64>=new Array<f64>();
/** Exact native field goal_tolerance, native scalar float64. */
goal_tolerance:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.goal_joint_configuration.length>7)return -22;
if(4>128-n)return -90;n+=4;
for(let i=0;i<this.goal_joint_configuration.length;++i){
if(!isFinite(this.goal_joint_configuration[i]))return -22;
if(8>128-n)return -90;n+=8;
if(n>128)return -90;
}
}
{
if(this.maximum_joint_velocities.length>7)return -22;
if(4>128-n)return -90;n+=4;
for(let i=0;i<this.maximum_joint_velocities.length;++i){
if(!isFinite(this.maximum_joint_velocities[i]))return -22;
if(8>128-n)return -90;n+=8;
if(n>128)return -90;
}
}
{
if(!isFinite(this.goal_tolerance))return -22;
if(8>128-n)return -90;n+=8;
if(n>128)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.goal_joint_configuration.length);
for(let i=0;i<this.goal_joint_configuration.length;++i){
w.integer(<u64>reinterpret<u64>(this.goal_joint_configuration[i]),8);
}
w.u32(<u32>this.maximum_joint_velocities.length);
for(let i=0;i<this.maximum_joint_velocities.length;++i){
w.integer(<u64>reinterpret<u64>(this.maximum_joint_velocities[i]),8);
}
w.integer(<u64>reinterpret<u64>(this.goal_tolerance),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsActionPTPMotion_Goal{const value=new Ros2Franka_msgsActionPTPMotion_Goal();
{
const count=r.u32();if(!r.valid||count>7){r.valid=false;return value;}
value.goal_joint_configuration=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.goal_joint_configuration.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>7){r.valid=false;return value;}
value.maximum_joint_velocities=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.maximum_joint_velocities.push(decoded);
}
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.goal_tolerance=decoded;
}
return value;}
}
export class Ros2Franka_msgsActionPTPMotion_GoalCodec implements VendorCodec<Ros2Franka_msgsActionPTPMotion_Goal>{
static readonly codecIdentity:string="UWVMROS1_7e1b02257a7be37e7bccd0870ebced1e1560e7250eaeaa4547836e721f2ea2cc";
static readonly maxWireBytes:i32=128;
encodeChecked(value:Ros2Franka_msgsActionPTPMotion_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionPTPMotion_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionPTPMotion_Goal>{const out=new VendorDecoded<Ros2Franka_msgsActionPTPMotion_Goal>();if(bytes.length>128){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionPTPMotion_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionPTPMotion_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionPTPMotion_Goal():changetype<Ros2Franka_msgsActionPTPMotion_Goal>(out.value);}
}
/** Complete original franka_msgs/msg/TargetStatus; spellings are published in generation proof. */
export class Ros2Franka_msgsMsgTargetStatus {
static readonly IDLE:u8=0;
static readonly EXECUTING:u8=1;
static readonly TARGET_REACHED:u8=2;
static readonly ABORTED:u8=3;
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar uint8. */
status:u8=0;
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
static readControl(r:Reader):Ros2Franka_msgsMsgTargetStatus{const value=new Ros2Franka_msgsMsgTargetStatus();
{
const decoded=<u8>r.integer(1);
value.status=decoded;
}
return value;}
}
export class Ros2Franka_msgsMsgTargetStatusCodec implements VendorCodec<Ros2Franka_msgsMsgTargetStatus>{
static readonly codecIdentity:string="UWVMROS1_df49eedc13cd8be2055361d8640c519bfa018ed2a421aa2b59b35c8828eac585";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Franka_msgsMsgTargetStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsMsgTargetStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsMsgTargetStatus>{const out=new VendorDecoded<Ros2Franka_msgsMsgTargetStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsMsgTargetStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsMsgTargetStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsMsgTargetStatus():changetype<Ros2Franka_msgsMsgTargetStatus>(out.value);}
}
/** Complete original franka_msgs/action/PTPMotion_Result; spellings are published in generation proof. */
export class Ros2Franka_msgsActionPTPMotion_Result {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field target_status, native scalar franka_msgs/msg/TargetStatus. */
target_status:Ros2Franka_msgsMsgTargetStatus=new Ros2Franka_msgsMsgTargetStatus();
/** Exact native field error_message, native scalar string. */
error_message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.target_status.wireSize();if(child<0)return child;if(child>261-n)return -90;n+=child;
if(n>261)return -90;
}
{
if(!validUnicode(this.error_message)||this.error_message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error_message);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.target_status.writeControl(w);
w.text(this.error_message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsActionPTPMotion_Result{const value=new Ros2Franka_msgsActionPTPMotion_Result();
{
const decoded=Ros2Franka_msgsMsgTargetStatus.readControl(r);
value.target_status=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error_message=decoded;
}
return value;}
}
export class Ros2Franka_msgsActionPTPMotion_ResultCodec implements VendorCodec<Ros2Franka_msgsActionPTPMotion_Result>{
static readonly codecIdentity:string="UWVMROS1_b61fabf13a3d8d910a35a5145e87b4dd80180b0678ef5580784f461e850fa4c1";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Franka_msgsActionPTPMotion_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionPTPMotion_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionPTPMotion_Result>{const out=new VendorDecoded<Ros2Franka_msgsActionPTPMotion_Result>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionPTPMotion_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionPTPMotion_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionPTPMotion_Result():changetype<Ros2Franka_msgsActionPTPMotion_Result>(out.value);}
}
/** Complete original franka_msgs/action/PTPMotion_Feedback; spellings are published in generation proof. */
export class Ros2Franka_msgsActionPTPMotion_Feedback {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field target_status, native scalar franka_msgs/msg/TargetStatus. */
target_status:Ros2Franka_msgsMsgTargetStatus=new Ros2Franka_msgsMsgTargetStatus();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.target_status.wireSize();if(child<0)return child;if(child>1-n)return -90;n+=child;
if(n>1)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.target_status.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsActionPTPMotion_Feedback{const value=new Ros2Franka_msgsActionPTPMotion_Feedback();
{
const decoded=Ros2Franka_msgsMsgTargetStatus.readControl(r);
value.target_status=decoded;
}
return value;}
}
export class Ros2Franka_msgsActionPTPMotion_FeedbackCodec implements VendorCodec<Ros2Franka_msgsActionPTPMotion_Feedback>{
static readonly codecIdentity:string="UWVMROS1_c4dcfeb668595ebc341d76b5cbc606c1ae4c38b28a1b329f8436e61f506a0300";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Franka_msgsActionPTPMotion_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsActionPTPMotion_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsActionPTPMotion_Feedback>{const out=new VendorDecoded<Ros2Franka_msgsActionPTPMotion_Feedback>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsActionPTPMotion_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsActionPTPMotion_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsActionPTPMotion_Feedback():changetype<Ros2Franka_msgsActionPTPMotion_Feedback>(out.value);}
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
/** Complete original franka_msgs/msg/CollisionIndicators; spellings are published in generation proof. */
export class Ros2Franka_msgsMsgCollisionIndicators {
static readonly maxWireBytes:i32=208;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=208;
/** Exact native field is_cartesian_linear_collision, native scalar geometry_msgs/msg/Vector3. */
is_cartesian_linear_collision:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field is_cartesian_angular_collision, native scalar geometry_msgs/msg/Vector3. */
is_cartesian_angular_collision:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field is_cartesian_linear_contact, native scalar geometry_msgs/msg/Vector3. */
is_cartesian_linear_contact:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field is_cartesian_angular_contact, native scalar geometry_msgs/msg/Vector3. */
is_cartesian_angular_contact:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field is_joint_collision, native scalar float64. */
is_joint_collision:Array<f64>=new Array<f64>();
/** Exact native field is_joint_contact, native scalar float64. */
is_joint_contact:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<7;++i)this.is_joint_collision.push(0);
for(let i=0;i<7;++i)this.is_joint_contact.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.is_cartesian_linear_collision.wireSize();if(child<0)return child;if(child>208-n)return -90;n+=child;
if(n>208)return -90;
}
{
const child=this.is_cartesian_angular_collision.wireSize();if(child<0)return child;if(child>208-n)return -90;n+=child;
if(n>208)return -90;
}
{
const child=this.is_cartesian_linear_contact.wireSize();if(child<0)return child;if(child>208-n)return -90;n+=child;
if(n>208)return -90;
}
{
const child=this.is_cartesian_angular_contact.wireSize();if(child<0)return child;if(child>208-n)return -90;n+=child;
if(n>208)return -90;
}
{
if(this.is_joint_collision.length!=7)return -22;
for(let i=0;i<this.is_joint_collision.length;++i){
if(!isFinite(this.is_joint_collision[i]))return -22;
if(8>208-n)return -90;n+=8;
if(n>208)return -90;
}
}
{
if(this.is_joint_contact.length!=7)return -22;
for(let i=0;i<this.is_joint_contact.length;++i){
if(!isFinite(this.is_joint_contact[i]))return -22;
if(8>208-n)return -90;n+=8;
if(n>208)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.is_cartesian_linear_collision.writeControl(w);
this.is_cartesian_angular_collision.writeControl(w);
this.is_cartesian_linear_contact.writeControl(w);
this.is_cartesian_angular_contact.writeControl(w);
for(let i=0;i<this.is_joint_collision.length;++i){
w.integer(<u64>reinterpret<u64>(this.is_joint_collision[i]),8);
}
for(let i=0;i<this.is_joint_contact.length;++i){
w.integer(<u64>reinterpret<u64>(this.is_joint_contact[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsMsgCollisionIndicators{const value=new Ros2Franka_msgsMsgCollisionIndicators();
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.is_cartesian_linear_collision=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.is_cartesian_angular_collision=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.is_cartesian_linear_contact=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.is_cartesian_angular_contact=decoded;
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.is_joint_collision=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.is_joint_collision.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.is_joint_contact=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.is_joint_contact.push(decoded);
}
}
return value;}
}
export class Ros2Franka_msgsMsgCollisionIndicatorsCodec implements VendorCodec<Ros2Franka_msgsMsgCollisionIndicators>{
static readonly codecIdentity:string="UWVMROS1_2bdfb3ece01b47b07fae84a998b23c56fc9482a0518969e43304b318d4ee0ce1";
static readonly maxWireBytes:i32=208;
encodeChecked(value:Ros2Franka_msgsMsgCollisionIndicators):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsMsgCollisionIndicators):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsMsgCollisionIndicators>{const out=new VendorDecoded<Ros2Franka_msgsMsgCollisionIndicators>();if(bytes.length>208){out.status=90;return out;}if(bytes.length<208){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsMsgCollisionIndicators.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsMsgCollisionIndicators{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsMsgCollisionIndicators():changetype<Ros2Franka_msgsMsgCollisionIndicators>(out.value);}
}
/** Complete original franka_msgs/msg/Elbow; spellings are published in generation proof. */
export class Ros2Franka_msgsMsgElbow {
static readonly maxWireBytes:i32=80;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=80;
/** Exact native field position, native scalar float64. */
position:Array<f64>=new Array<f64>();
/** Exact native field desired_position, native scalar float64. */
desired_position:Array<f64>=new Array<f64>();
/** Exact native field commanded_position, native scalar float64. */
commanded_position:Array<f64>=new Array<f64>();
/** Exact native field commanded_velocity, native scalar float64. */
commanded_velocity:Array<f64>=new Array<f64>();
/** Exact native field commanded_acceleration, native scalar float64. */
commanded_acceleration:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<2;++i)this.position.push(0);
for(let i=0;i<2;++i)this.desired_position.push(0);
for(let i=0;i<2;++i)this.commanded_position.push(0);
for(let i=0;i<2;++i)this.commanded_velocity.push(0);
for(let i=0;i<2;++i)this.commanded_acceleration.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.position.length!=2)return -22;
for(let i=0;i<this.position.length;++i){
if(!isFinite(this.position[i]))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
}
{
if(this.desired_position.length!=2)return -22;
for(let i=0;i<this.desired_position.length;++i){
if(!isFinite(this.desired_position[i]))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
}
{
if(this.commanded_position.length!=2)return -22;
for(let i=0;i<this.commanded_position.length;++i){
if(!isFinite(this.commanded_position[i]))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
}
{
if(this.commanded_velocity.length!=2)return -22;
for(let i=0;i<this.commanded_velocity.length;++i){
if(!isFinite(this.commanded_velocity[i]))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
}
{
if(this.commanded_acceleration.length!=2)return -22;
for(let i=0;i<this.commanded_acceleration.length;++i){
if(!isFinite(this.commanded_acceleration[i]))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.position.length;++i){
w.integer(<u64>reinterpret<u64>(this.position[i]),8);
}
for(let i=0;i<this.desired_position.length;++i){
w.integer(<u64>reinterpret<u64>(this.desired_position[i]),8);
}
for(let i=0;i<this.commanded_position.length;++i){
w.integer(<u64>reinterpret<u64>(this.commanded_position[i]),8);
}
for(let i=0;i<this.commanded_velocity.length;++i){
w.integer(<u64>reinterpret<u64>(this.commanded_velocity[i]),8);
}
for(let i=0;i<this.commanded_acceleration.length;++i){
w.integer(<u64>reinterpret<u64>(this.commanded_acceleration[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsMsgElbow{const value=new Ros2Franka_msgsMsgElbow();
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.position=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.position.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.desired_position=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.desired_position.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.commanded_position=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.commanded_position.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.commanded_velocity=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.commanded_velocity.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.commanded_acceleration=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.commanded_acceleration.push(decoded);
}
}
return value;}
}
export class Ros2Franka_msgsMsgElbowCodec implements VendorCodec<Ros2Franka_msgsMsgElbow>{
static readonly codecIdentity:string="UWVMROS1_686cf2da29007046f9855ffeecac304c159245ec3da153990ab83be4ffce53b6";
static readonly maxWireBytes:i32=80;
encodeChecked(value:Ros2Franka_msgsMsgElbow):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsMsgElbow):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsMsgElbow>{const out=new VendorDecoded<Ros2Franka_msgsMsgElbow>();if(bytes.length>80){out.status=90;return out;}if(bytes.length<80){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsMsgElbow.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsMsgElbow{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsMsgElbow():changetype<Ros2Franka_msgsMsgElbow>(out.value);}
}
/** Complete original franka_msgs/msg/Errors; spellings are published in generation proof. */
export class Ros2Franka_msgsMsgErrors {
static readonly maxWireBytes:i32=36;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=36;
/** Exact native field joint_position_limits_violation, native scalar bool. */
joint_position_limits_violation:bool=false;
/** Exact native field cartesian_position_limits_violation, native scalar bool. */
cartesian_position_limits_violation:bool=false;
/** Exact native field self_collision_avoidance_violation, native scalar bool. */
self_collision_avoidance_violation:bool=false;
/** Exact native field joint_velocity_violation, native scalar bool. */
joint_velocity_violation:bool=false;
/** Exact native field cartesian_velocity_violation, native scalar bool. */
cartesian_velocity_violation:bool=false;
/** Exact native field force_control_safety_violation, native scalar bool. */
force_control_safety_violation:bool=false;
/** Exact native field joint_reflex, native scalar bool. */
joint_reflex:bool=false;
/** Exact native field cartesian_reflex, native scalar bool. */
cartesian_reflex:bool=false;
/** Exact native field max_goal_pose_deviation_violation, native scalar bool. */
max_goal_pose_deviation_violation:bool=false;
/** Exact native field max_path_pose_deviation_violation, native scalar bool. */
max_path_pose_deviation_violation:bool=false;
/** Exact native field cartesian_velocity_profile_safety_violation, native scalar bool. */
cartesian_velocity_profile_safety_violation:bool=false;
/** Exact native field joint_position_motion_generator_start_pose_invalid, native scalar bool. */
joint_position_motion_generator_start_pose_invalid:bool=false;
/** Exact native field joint_motion_generator_position_limits_violation, native scalar bool. */
joint_motion_generator_position_limits_violation:bool=false;
/** Exact native field joint_motion_generator_velocity_limits_violation, native scalar bool. */
joint_motion_generator_velocity_limits_violation:bool=false;
/** Exact native field joint_motion_generator_velocity_discontinuity, native scalar bool. */
joint_motion_generator_velocity_discontinuity:bool=false;
/** Exact native field joint_motion_generator_acceleration_discontinuity, native scalar bool. */
joint_motion_generator_acceleration_discontinuity:bool=false;
/** Exact native field cartesian_position_motion_generator_start_pose_invalid, native scalar bool. */
cartesian_position_motion_generator_start_pose_invalid:bool=false;
/** Exact native field cartesian_motion_generator_elbow_limit_violation, native scalar bool. */
cartesian_motion_generator_elbow_limit_violation:bool=false;
/** Exact native field cartesian_motion_generator_velocity_limits_violation, native scalar bool. */
cartesian_motion_generator_velocity_limits_violation:bool=false;
/** Exact native field cartesian_motion_generator_velocity_discontinuity, native scalar bool. */
cartesian_motion_generator_velocity_discontinuity:bool=false;
/** Exact native field cartesian_motion_generator_acceleration_discontinuity, native scalar bool. */
cartesian_motion_generator_acceleration_discontinuity:bool=false;
/** Exact native field cartesian_motion_generator_elbow_sign_inconsistent, native scalar bool. */
cartesian_motion_generator_elbow_sign_inconsistent:bool=false;
/** Exact native field cartesian_motion_generator_start_elbow_invalid, native scalar bool. */
cartesian_motion_generator_start_elbow_invalid:bool=false;
/** Exact native field cartesian_motion_generator_joint_position_limits_violation, native scalar bool. */
cartesian_motion_generator_joint_position_limits_violation:bool=false;
/** Exact native field cartesian_motion_generator_joint_velocity_limits_violation, native scalar bool. */
cartesian_motion_generator_joint_velocity_limits_violation:bool=false;
/** Exact native field cartesian_motion_generator_joint_velocity_discontinuity, native scalar bool. */
cartesian_motion_generator_joint_velocity_discontinuity:bool=false;
/** Exact native field cartesian_motion_generator_joint_acceleration_discontinuity, native scalar bool. */
cartesian_motion_generator_joint_acceleration_discontinuity:bool=false;
/** Exact native field cartesian_position_motion_generator_invalid_frame, native scalar bool. */
cartesian_position_motion_generator_invalid_frame:bool=false;
/** Exact native field force_controller_desired_force_tolerance_violation, native scalar bool. */
force_controller_desired_force_tolerance_violation:bool=false;
/** Exact native field controller_torque_discontinuity, native scalar bool. */
controller_torque_discontinuity:bool=false;
/** Exact native field start_elbow_sign_inconsistent, native scalar bool. */
start_elbow_sign_inconsistent:bool=false;
/** Exact native field communication_constraints_violation, native scalar bool. */
communication_constraints_violation:bool=false;
/** Exact native field power_limit_violation, native scalar bool. */
power_limit_violation:bool=false;
/** Exact native field joint_p2p_insufficient_torque_for_planning, native scalar bool. */
joint_p2p_insufficient_torque_for_planning:bool=false;
/** Exact native field tau_j_range_violation, native scalar bool. */
tau_j_range_violation:bool=false;
/** Exact native field instability_detected, native scalar bool. */
instability_detected:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
{
if(1>36-n)return -90;n+=1;
if(n>36)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.joint_position_limits_violation?1:0);
w.u8(this.cartesian_position_limits_violation?1:0);
w.u8(this.self_collision_avoidance_violation?1:0);
w.u8(this.joint_velocity_violation?1:0);
w.u8(this.cartesian_velocity_violation?1:0);
w.u8(this.force_control_safety_violation?1:0);
w.u8(this.joint_reflex?1:0);
w.u8(this.cartesian_reflex?1:0);
w.u8(this.max_goal_pose_deviation_violation?1:0);
w.u8(this.max_path_pose_deviation_violation?1:0);
w.u8(this.cartesian_velocity_profile_safety_violation?1:0);
w.u8(this.joint_position_motion_generator_start_pose_invalid?1:0);
w.u8(this.joint_motion_generator_position_limits_violation?1:0);
w.u8(this.joint_motion_generator_velocity_limits_violation?1:0);
w.u8(this.joint_motion_generator_velocity_discontinuity?1:0);
w.u8(this.joint_motion_generator_acceleration_discontinuity?1:0);
w.u8(this.cartesian_position_motion_generator_start_pose_invalid?1:0);
w.u8(this.cartesian_motion_generator_elbow_limit_violation?1:0);
w.u8(this.cartesian_motion_generator_velocity_limits_violation?1:0);
w.u8(this.cartesian_motion_generator_velocity_discontinuity?1:0);
w.u8(this.cartesian_motion_generator_acceleration_discontinuity?1:0);
w.u8(this.cartesian_motion_generator_elbow_sign_inconsistent?1:0);
w.u8(this.cartesian_motion_generator_start_elbow_invalid?1:0);
w.u8(this.cartesian_motion_generator_joint_position_limits_violation?1:0);
w.u8(this.cartesian_motion_generator_joint_velocity_limits_violation?1:0);
w.u8(this.cartesian_motion_generator_joint_velocity_discontinuity?1:0);
w.u8(this.cartesian_motion_generator_joint_acceleration_discontinuity?1:0);
w.u8(this.cartesian_position_motion_generator_invalid_frame?1:0);
w.u8(this.force_controller_desired_force_tolerance_violation?1:0);
w.u8(this.controller_torque_discontinuity?1:0);
w.u8(this.start_elbow_sign_inconsistent?1:0);
w.u8(this.communication_constraints_violation?1:0);
w.u8(this.power_limit_violation?1:0);
w.u8(this.joint_p2p_insufficient_torque_for_planning?1:0);
w.u8(this.tau_j_range_violation?1:0);
w.u8(this.instability_detected?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsMsgErrors{const value=new Ros2Franka_msgsMsgErrors();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.joint_position_limits_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_position_limits_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.self_collision_avoidance_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.joint_velocity_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_velocity_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.force_control_safety_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.joint_reflex=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_reflex=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.max_goal_pose_deviation_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.max_path_pose_deviation_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_velocity_profile_safety_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.joint_position_motion_generator_start_pose_invalid=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.joint_motion_generator_position_limits_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.joint_motion_generator_velocity_limits_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.joint_motion_generator_velocity_discontinuity=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.joint_motion_generator_acceleration_discontinuity=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_position_motion_generator_start_pose_invalid=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_motion_generator_elbow_limit_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_motion_generator_velocity_limits_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_motion_generator_velocity_discontinuity=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_motion_generator_acceleration_discontinuity=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_motion_generator_elbow_sign_inconsistent=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_motion_generator_start_elbow_invalid=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_motion_generator_joint_position_limits_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_motion_generator_joint_velocity_limits_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_motion_generator_joint_velocity_discontinuity=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_motion_generator_joint_acceleration_discontinuity=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_position_motion_generator_invalid_frame=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.force_controller_desired_force_tolerance_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.controller_torque_discontinuity=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.start_elbow_sign_inconsistent=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.communication_constraints_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.power_limit_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.joint_p2p_insufficient_torque_for_planning=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.tau_j_range_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.instability_detected=decoded;
}
return value;}
}
export class Ros2Franka_msgsMsgErrorsCodec implements VendorCodec<Ros2Franka_msgsMsgErrors>{
static readonly codecIdentity:string="UWVMROS1_c7927af1abe7710bb686574ad6f2459f8425af1d6cdb3189160bb732b5a6bc8c";
static readonly maxWireBytes:i32=36;
encodeChecked(value:Ros2Franka_msgsMsgErrors):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsMsgErrors):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsMsgErrors>{const out=new VendorDecoded<Ros2Franka_msgsMsgErrors>();if(bytes.length>36){out.status=90;return out;}if(bytes.length<36){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsMsgErrors.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsMsgErrors{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsMsgErrors():changetype<Ros2Franka_msgsMsgErrors>(out.value);}
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
/** Complete original sensor_msgs/msg/JointState; spellings are published in generation proof. */
export class Ros2Sensor_msgsMsgJointState {
static readonly maxWireBytes:i32=1248;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1248;
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
const child=this.header.wireSize();if(child<0)return child;if(child>1248-n)return -90;n+=child;
if(n>1248)return -90;
}
{
if(this.name.length>7)return -22;
if(4>1248-n)return -90;n+=4;
for(let i=0;i<this.name.length;++i){
if(!validUnicode(this.name[i])||this.name[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name[i]);if(size>128)return -22;if(4+size>1248-n)return -90;n+=4+size;
if(n>1248)return -90;
}
}
{
if(this.position.length>7)return -22;
if(4>1248-n)return -90;n+=4;
for(let i=0;i<this.position.length;++i){
if(!isFinite(this.position[i]))return -22;
if(8>1248-n)return -90;n+=8;
if(n>1248)return -90;
}
}
{
if(this.velocity.length>7)return -22;
if(4>1248-n)return -90;n+=4;
for(let i=0;i<this.velocity.length;++i){
if(!isFinite(this.velocity[i]))return -22;
if(8>1248-n)return -90;n+=8;
if(n>1248)return -90;
}
}
{
if(this.effort.length>7)return -22;
if(4>1248-n)return -90;n+=4;
for(let i=0;i<this.effort.length;++i){
if(!isFinite(this.effort[i]))return -22;
if(8>1248-n)return -90;n+=8;
if(n>1248)return -90;
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
const count=r.u32();if(!r.valid||count>7){r.valid=false;return value;}
value.name=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.name.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>7){r.valid=false;return value;}
value.position=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.position.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>7){r.valid=false;return value;}
value.velocity=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.velocity.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>7){r.valid=false;return value;}
value.effort=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.effort.push(decoded);
}
}
if((value.name.length!=0&&value.position.length!=0&&value.name.length!=value.position.length)||(value.name.length!=0&&value.velocity.length!=0&&value.name.length!=value.velocity.length)||(value.name.length!=0&&value.effort.length!=0&&value.name.length!=value.effort.length)||(value.position.length!=0&&value.velocity.length!=0&&value.position.length!=value.velocity.length)||(value.position.length!=0&&value.effort.length!=0&&value.position.length!=value.effort.length)||(value.velocity.length!=0&&value.effort.length!=0&&value.velocity.length!=value.effort.length))r.valid=false;
return value;}
}
export class Ros2Sensor_msgsMsgJointStateCodec implements VendorCodec<Ros2Sensor_msgsMsgJointState>{
static readonly codecIdentity:string="UWVMROS1_912ba4261e2dfbec0e4a543ea8e5d308ef1d26877b4a7d71162a981ac64c40a3";
static readonly maxWireBytes:i32=1248;
encodeChecked(value:Ros2Sensor_msgsMsgJointState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Sensor_msgsMsgJointState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Sensor_msgsMsgJointState>{const out=new VendorDecoded<Ros2Sensor_msgsMsgJointState>();if(bytes.length>1248){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Sensor_msgsMsgJointState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Sensor_msgsMsgJointState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Sensor_msgsMsgJointState():changetype<Ros2Sensor_msgsMsgJointState>(out.value);}
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
/** Complete original geometry_msgs/msg/WrenchStamped; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgWrenchStamped {
static readonly maxWireBytes:i32=188;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=188;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field wrench, native scalar geometry_msgs/msg/Wrench. */
wrench:Ros2Geometry_msgsMsgWrench=new Ros2Geometry_msgsMsgWrench();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>188-n)return -90;n+=child;
if(n>188)return -90;
}
{
const child=this.wrench.wireSize();if(child<0)return child;if(child>188-n)return -90;n+=child;
if(n>188)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.wrench.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgWrenchStamped{const value=new Ros2Geometry_msgsMsgWrenchStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgWrench.readControl(r);
value.wrench=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgWrenchStampedCodec implements VendorCodec<Ros2Geometry_msgsMsgWrenchStamped>{
static readonly codecIdentity:string="UWVMROS1_e23600874a626ba9d52f043e14ed04beb3e316d47c3bca8bd87cfeed3f5d9d66";
static readonly maxWireBytes:i32=188;
encodeChecked(value:Ros2Geometry_msgsMsgWrenchStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgWrenchStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgWrenchStamped>{const out=new VendorDecoded<Ros2Geometry_msgsMsgWrenchStamped>();if(bytes.length>188){out.status=90;return out;}if(bytes.length<60){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgWrenchStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgWrenchStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgWrenchStamped():changetype<Ros2Geometry_msgsMsgWrenchStamped>(out.value);}
}
/** Complete original geometry_msgs/msg/Inertia; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgInertia {
static readonly maxWireBytes:i32=80;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=80;
/** Exact native field m, native scalar float64. */
m:f64=0;
/** Exact native field com, native scalar geometry_msgs/msg/Vector3. */
com:Ros2Geometry_msgsMsgVector3=new Ros2Geometry_msgsMsgVector3();
/** Exact native field ixx, native scalar float64. */
ixx:f64=0;
/** Exact native field ixy, native scalar float64. */
ixy:f64=0;
/** Exact native field ixz, native scalar float64. */
ixz:f64=0;
/** Exact native field iyy, native scalar float64. */
iyy:f64=0;
/** Exact native field iyz, native scalar float64. */
iyz:f64=0;
/** Exact native field izz, native scalar float64. */
izz:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.m))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
{
const child=this.com.wireSize();if(child<0)return child;if(child>80-n)return -90;n+=child;
if(n>80)return -90;
}
{
if(!isFinite(this.ixx))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
{
if(!isFinite(this.ixy))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
{
if(!isFinite(this.ixz))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
{
if(!isFinite(this.iyy))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
{
if(!isFinite(this.iyz))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
{
if(!isFinite(this.izz))return -22;
if(8>80-n)return -90;n+=8;
if(n>80)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.m),8);
this.com.writeControl(w);
w.integer(<u64>reinterpret<u64>(this.ixx),8);
w.integer(<u64>reinterpret<u64>(this.ixy),8);
w.integer(<u64>reinterpret<u64>(this.ixz),8);
w.integer(<u64>reinterpret<u64>(this.iyy),8);
w.integer(<u64>reinterpret<u64>(this.iyz),8);
w.integer(<u64>reinterpret<u64>(this.izz),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgInertia{const value=new Ros2Geometry_msgsMsgInertia();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.m=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgVector3.readControl(r);
value.com=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.ixx=decoded;
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
value.iyy=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.iyz=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.izz=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgInertiaCodec implements VendorCodec<Ros2Geometry_msgsMsgInertia>{
static readonly codecIdentity:string="UWVMROS1_4a1b7a6a2516724a9a12e5eb166a85cd73d33fc6075c8d517ff2a279fd29e12a";
static readonly maxWireBytes:i32=80;
encodeChecked(value:Ros2Geometry_msgsMsgInertia):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgInertia):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgInertia>{const out=new VendorDecoded<Ros2Geometry_msgsMsgInertia>();if(bytes.length>80){out.status=90;return out;}if(bytes.length<80){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgInertia.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgInertia{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgInertia():changetype<Ros2Geometry_msgsMsgInertia>(out.value);}
}
/** Complete original geometry_msgs/msg/InertiaStamped; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgInertiaStamped {
static readonly maxWireBytes:i32=220;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=220;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field inertia, native scalar geometry_msgs/msg/Inertia. */
inertia:Ros2Geometry_msgsMsgInertia=new Ros2Geometry_msgsMsgInertia();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>220-n)return -90;n+=child;
if(n>220)return -90;
}
{
const child=this.inertia.wireSize();if(child<0)return child;if(child>220-n)return -90;n+=child;
if(n>220)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.inertia.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgInertiaStamped{const value=new Ros2Geometry_msgsMsgInertiaStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgInertia.readControl(r);
value.inertia=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgInertiaStampedCodec implements VendorCodec<Ros2Geometry_msgsMsgInertiaStamped>{
static readonly codecIdentity:string="UWVMROS1_19b0b442c5858ff1eee20e4658ecc3a2b76fdd64c4a21a1642fae5ab38198576";
static readonly maxWireBytes:i32=220;
encodeChecked(value:Ros2Geometry_msgsMsgInertiaStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgInertiaStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgInertiaStamped>{const out=new VendorDecoded<Ros2Geometry_msgsMsgInertiaStamped>();if(bytes.length>220){out.status=90;return out;}if(bytes.length<92){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgInertiaStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgInertiaStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgInertiaStamped():changetype<Ros2Geometry_msgsMsgInertiaStamped>(out.value);}
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
/** Complete original geometry_msgs/msg/Accel; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgAccel {
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
static readControl(r:Reader):Ros2Geometry_msgsMsgAccel{const value=new Ros2Geometry_msgsMsgAccel();
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
export class Ros2Geometry_msgsMsgAccelCodec implements VendorCodec<Ros2Geometry_msgsMsgAccel>{
static readonly codecIdentity:string="UWVMROS1_87264cb87970315248345c43cc629bbc0c9f9b2a8c6c4bc089da81c7af552fde";
static readonly maxWireBytes:i32=48;
encodeChecked(value:Ros2Geometry_msgsMsgAccel):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgAccel):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgAccel>{const out=new VendorDecoded<Ros2Geometry_msgsMsgAccel>();if(bytes.length>48){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgAccel.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgAccel{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgAccel():changetype<Ros2Geometry_msgsMsgAccel>(out.value);}
}
/** Complete original geometry_msgs/msg/AccelStamped; spellings are published in generation proof. */
export class Ros2Geometry_msgsMsgAccelStamped {
static readonly maxWireBytes:i32=188;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=188;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field accel, native scalar geometry_msgs/msg/Accel. */
accel:Ros2Geometry_msgsMsgAccel=new Ros2Geometry_msgsMsgAccel();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>188-n)return -90;n+=child;
if(n>188)return -90;
}
{
const child=this.accel.wireSize();if(child<0)return child;if(child>188-n)return -90;n+=child;
if(n>188)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.accel.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Geometry_msgsMsgAccelStamped{const value=new Ros2Geometry_msgsMsgAccelStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgAccel.readControl(r);
value.accel=decoded;
}
return value;}
}
export class Ros2Geometry_msgsMsgAccelStampedCodec implements VendorCodec<Ros2Geometry_msgsMsgAccelStamped>{
static readonly codecIdentity:string="UWVMROS1_c04d3ae310bd82f7c4c9f30c2ff185ab54989aa3adcc759c7d3eb19853819899";
static readonly maxWireBytes:i32=188;
encodeChecked(value:Ros2Geometry_msgsMsgAccelStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Geometry_msgsMsgAccelStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Geometry_msgsMsgAccelStamped>{const out=new VendorDecoded<Ros2Geometry_msgsMsgAccelStamped>();if(bytes.length>188){out.status=90;return out;}if(bytes.length<60){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Geometry_msgsMsgAccelStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Geometry_msgsMsgAccelStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Geometry_msgsMsgAccelStamped():changetype<Ros2Geometry_msgsMsgAccelStamped>(out.value);}
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
/** Complete original franka_msgs/msg/FrankaRobotState; spellings are published in generation proof. */
export class Ros2Franka_msgsMsgFrankaRobotState {
static readonly ROBOT_MODE_OTHER:u8=0;
static readonly ROBOT_MODE_IDLE:u8=1;
static readonly ROBOT_MODE_MOVE:u8=2;
static readonly ROBOT_MODE_GUIDING:u8=3;
static readonly ROBOT_MODE_REFLEX:u8=4;
static readonly ROBOT_MODE_USER_STOPPED:u8=5;
static readonly ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY:u8=6;
static readonly maxWireBytes:i32=10169;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=10169;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field collision_indicators, native scalar franka_msgs/msg/CollisionIndicators. */
collision_indicators:Ros2Franka_msgsMsgCollisionIndicators=new Ros2Franka_msgsMsgCollisionIndicators();
/** Exact native field measured_joint_state, native scalar sensor_msgs/msg/JointState. */
measured_joint_state:Ros2Sensor_msgsMsgJointState=new Ros2Sensor_msgsMsgJointState();
/** Exact native field desired_joint_state, native scalar sensor_msgs/msg/JointState. */
desired_joint_state:Ros2Sensor_msgsMsgJointState=new Ros2Sensor_msgsMsgJointState();
/** Exact native field measured_joint_motor_state, native scalar sensor_msgs/msg/JointState. */
measured_joint_motor_state:Ros2Sensor_msgsMsgJointState=new Ros2Sensor_msgsMsgJointState();
/** Exact native field ddq_d, native scalar float64. */
ddq_d:Array<f64>=new Array<f64>();
/** Exact native field dtau_j, native scalar float64. */
dtau_j:Array<f64>=new Array<f64>();
/** Exact native field tau_ext_hat_filtered, native scalar sensor_msgs/msg/JointState. */
tau_ext_hat_filtered:Ros2Sensor_msgsMsgJointState=new Ros2Sensor_msgsMsgJointState();
/** Exact native field elbow, native scalar franka_msgs/msg/Elbow. */
elbow:Ros2Franka_msgsMsgElbow=new Ros2Franka_msgsMsgElbow();
/** Exact native field k_f_ext_hat_k, native scalar geometry_msgs/msg/WrenchStamped. */
k_f_ext_hat_k:Ros2Geometry_msgsMsgWrenchStamped=new Ros2Geometry_msgsMsgWrenchStamped();
/** Exact native field o_f_ext_hat_k, native scalar geometry_msgs/msg/WrenchStamped. */
o_f_ext_hat_k:Ros2Geometry_msgsMsgWrenchStamped=new Ros2Geometry_msgsMsgWrenchStamped();
/** Exact native field inertia_ee, native scalar geometry_msgs/msg/InertiaStamped. */
inertia_ee:Ros2Geometry_msgsMsgInertiaStamped=new Ros2Geometry_msgsMsgInertiaStamped();
/** Exact native field inertia_load, native scalar geometry_msgs/msg/InertiaStamped. */
inertia_load:Ros2Geometry_msgsMsgInertiaStamped=new Ros2Geometry_msgsMsgInertiaStamped();
/** Exact native field inertia_total, native scalar geometry_msgs/msg/InertiaStamped. */
inertia_total:Ros2Geometry_msgsMsgInertiaStamped=new Ros2Geometry_msgsMsgInertiaStamped();
/** Exact native field o_t_ee, native scalar geometry_msgs/msg/PoseStamped. */
o_t_ee:Ros2Geometry_msgsMsgPoseStamped=new Ros2Geometry_msgsMsgPoseStamped();
/** Exact native field o_t_ee_d, native scalar geometry_msgs/msg/PoseStamped. */
o_t_ee_d:Ros2Geometry_msgsMsgPoseStamped=new Ros2Geometry_msgsMsgPoseStamped();
/** Exact native field o_t_ee_c, native scalar geometry_msgs/msg/PoseStamped. */
o_t_ee_c:Ros2Geometry_msgsMsgPoseStamped=new Ros2Geometry_msgsMsgPoseStamped();
/** Exact native field f_t_ee, native scalar geometry_msgs/msg/PoseStamped. */
f_t_ee:Ros2Geometry_msgsMsgPoseStamped=new Ros2Geometry_msgsMsgPoseStamped();
/** Exact native field ee_t_k, native scalar geometry_msgs/msg/PoseStamped. */
ee_t_k:Ros2Geometry_msgsMsgPoseStamped=new Ros2Geometry_msgsMsgPoseStamped();
/** Exact native field o_dp_ee_d, native scalar geometry_msgs/msg/TwistStamped. */
o_dp_ee_d:Ros2Geometry_msgsMsgTwistStamped=new Ros2Geometry_msgsMsgTwistStamped();
/** Exact native field o_dp_ee_c, native scalar geometry_msgs/msg/TwistStamped. */
o_dp_ee_c:Ros2Geometry_msgsMsgTwistStamped=new Ros2Geometry_msgsMsgTwistStamped();
/** Exact native field o_ddp_ee_c, native scalar geometry_msgs/msg/AccelStamped. */
o_ddp_ee_c:Ros2Geometry_msgsMsgAccelStamped=new Ros2Geometry_msgsMsgAccelStamped();
/** Exact native field time, native scalar float64. */
time:f64=0;
/** Exact native field control_command_success_rate, native scalar float64. */
control_command_success_rate:f64=0;
/** Exact native field robot_mode, native scalar uint8. */
robot_mode:u8=0;
/** Exact native field current_errors, native scalar franka_msgs/msg/Errors. */
current_errors:Ros2Franka_msgsMsgErrors=new Ros2Franka_msgsMsgErrors();
/** Exact native field last_motion_errors, native scalar franka_msgs/msg/Errors. */
last_motion_errors:Ros2Franka_msgsMsgErrors=new Ros2Franka_msgsMsgErrors();
/** Exact native field accelerometer_top, native scalar geometry_msgs/msg/Vector3Stamped. */
accelerometer_top:Array<Ros2Geometry_msgsMsgVector3Stamped>=new Array<Ros2Geometry_msgsMsgVector3Stamped>();
/** Exact native field accelerometer_bottom, native scalar geometry_msgs/msg/Vector3Stamped. */
accelerometer_bottom:Array<Ros2Geometry_msgsMsgVector3Stamped>=new Array<Ros2Geometry_msgsMsgVector3Stamped>();
constructor(){
for(let i=0;i<7;++i)this.ddq_d.push(0);
for(let i=0;i<7;++i)this.dtau_j.push(0);
for(let i=0;i<6;++i)this.accelerometer_top.push(new Ros2Geometry_msgsMsgVector3Stamped());
for(let i=0;i<6;++i)this.accelerometer_bottom.push(new Ros2Geometry_msgsMsgVector3Stamped());
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.collision_indicators.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.measured_joint_state.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.desired_joint_state.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.measured_joint_motor_state.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
if(this.ddq_d.length!=7)return -22;
for(let i=0;i<this.ddq_d.length;++i){
if(!isFinite(this.ddq_d[i]))return -22;
if(8>10169-n)return -90;n+=8;
if(n>10169)return -90;
}
}
{
if(this.dtau_j.length!=7)return -22;
for(let i=0;i<this.dtau_j.length;++i){
if(!isFinite(this.dtau_j[i]))return -22;
if(8>10169-n)return -90;n+=8;
if(n>10169)return -90;
}
}
{
const child=this.tau_ext_hat_filtered.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.elbow.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.k_f_ext_hat_k.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.o_f_ext_hat_k.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.inertia_ee.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.inertia_load.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.inertia_total.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.o_t_ee.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.o_t_ee_d.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.o_t_ee_c.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.f_t_ee.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.ee_t_k.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.o_dp_ee_d.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.o_dp_ee_c.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.o_ddp_ee_c.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
if(!isFinite(this.time))return -22;
if(8>10169-n)return -90;n+=8;
if(n>10169)return -90;
}
{
if(!isFinite(this.control_command_success_rate))return -22;
if(8>10169-n)return -90;n+=8;
if(n>10169)return -90;
}
{
if(1>10169-n)return -90;n+=1;
if(n>10169)return -90;
}
{
const child=this.current_errors.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
const child=this.last_motion_errors.wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
{
if(this.accelerometer_top.length!=6)return -22;
for(let i=0;i<this.accelerometer_top.length;++i){
const child=this.accelerometer_top[i].wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
}
{
if(this.accelerometer_bottom.length!=6)return -22;
for(let i=0;i<this.accelerometer_bottom.length;++i){
const child=this.accelerometer_bottom[i].wireSize();if(child<0)return child;if(child>10169-n)return -90;n+=child;
if(n>10169)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
this.collision_indicators.writeControl(w);
this.measured_joint_state.writeControl(w);
this.desired_joint_state.writeControl(w);
this.measured_joint_motor_state.writeControl(w);
for(let i=0;i<this.ddq_d.length;++i){
w.integer(<u64>reinterpret<u64>(this.ddq_d[i]),8);
}
for(let i=0;i<this.dtau_j.length;++i){
w.integer(<u64>reinterpret<u64>(this.dtau_j[i]),8);
}
this.tau_ext_hat_filtered.writeControl(w);
this.elbow.writeControl(w);
this.k_f_ext_hat_k.writeControl(w);
this.o_f_ext_hat_k.writeControl(w);
this.inertia_ee.writeControl(w);
this.inertia_load.writeControl(w);
this.inertia_total.writeControl(w);
this.o_t_ee.writeControl(w);
this.o_t_ee_d.writeControl(w);
this.o_t_ee_c.writeControl(w);
this.f_t_ee.writeControl(w);
this.ee_t_k.writeControl(w);
this.o_dp_ee_d.writeControl(w);
this.o_dp_ee_c.writeControl(w);
this.o_ddp_ee_c.writeControl(w);
w.integer(<u64>reinterpret<u64>(this.time),8);
w.integer(<u64>reinterpret<u64>(this.control_command_success_rate),8);
w.integer(<u64>this.robot_mode,1);
this.current_errors.writeControl(w);
this.last_motion_errors.writeControl(w);
for(let i=0;i<this.accelerometer_top.length;++i){
this.accelerometer_top[i].writeControl(w);
}
for(let i=0;i<this.accelerometer_bottom.length;++i){
this.accelerometer_bottom[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsMsgFrankaRobotState{const value=new Ros2Franka_msgsMsgFrankaRobotState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=Ros2Franka_msgsMsgCollisionIndicators.readControl(r);
value.collision_indicators=decoded;
}
{
const decoded=Ros2Sensor_msgsMsgJointState.readControl(r);
value.measured_joint_state=decoded;
}
{
const decoded=Ros2Sensor_msgsMsgJointState.readControl(r);
value.desired_joint_state=decoded;
}
{
const decoded=Ros2Sensor_msgsMsgJointState.readControl(r);
value.measured_joint_motor_state=decoded;
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.ddq_d=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.ddq_d.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.dtau_j=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.dtau_j.push(decoded);
}
}
{
const decoded=Ros2Sensor_msgsMsgJointState.readControl(r);
value.tau_ext_hat_filtered=decoded;
}
{
const decoded=Ros2Franka_msgsMsgElbow.readControl(r);
value.elbow=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgWrenchStamped.readControl(r);
value.k_f_ext_hat_k=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgWrenchStamped.readControl(r);
value.o_f_ext_hat_k=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgInertiaStamped.readControl(r);
value.inertia_ee=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgInertiaStamped.readControl(r);
value.inertia_load=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgInertiaStamped.readControl(r);
value.inertia_total=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseStamped.readControl(r);
value.o_t_ee=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseStamped.readControl(r);
value.o_t_ee_d=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseStamped.readControl(r);
value.o_t_ee_c=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseStamped.readControl(r);
value.f_t_ee=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgPoseStamped.readControl(r);
value.ee_t_k=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwistStamped.readControl(r);
value.o_dp_ee_d=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgTwistStamped.readControl(r);
value.o_dp_ee_c=decoded;
}
{
const decoded=Ros2Geometry_msgsMsgAccelStamped.readControl(r);
value.o_ddp_ee_c=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.time=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.control_command_success_rate=decoded;
}
{
const decoded=<u8>r.integer(1);
value.robot_mode=decoded;
}
{
const decoded=Ros2Franka_msgsMsgErrors.readControl(r);
value.current_errors=decoded;
}
{
const decoded=Ros2Franka_msgsMsgErrors.readControl(r);
value.last_motion_errors=decoded;
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.accelerometer_top=new Array<Ros2Geometry_msgsMsgVector3Stamped>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgVector3Stamped.readControl(r);
value.accelerometer_top.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.accelerometer_bottom=new Array<Ros2Geometry_msgsMsgVector3Stamped>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgVector3Stamped.readControl(r);
value.accelerometer_bottom.push(decoded);
}
}
return value;}
}
export class Ros2Franka_msgsMsgFrankaRobotStateCodec implements VendorCodec<Ros2Franka_msgsMsgFrankaRobotState>{
static readonly codecIdentity:string="UWVMROS1_0fdc5c99a098f4c891956bb6cd81505995d5dda41a740f88ee456f48f6e07282";
static readonly maxWireBytes:i32=10169;
encodeChecked(value:Ros2Franka_msgsMsgFrankaRobotState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsMsgFrankaRobotState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsMsgFrankaRobotState>{const out=new VendorDecoded<Ros2Franka_msgsMsgFrankaRobotState>();if(bytes.length>10169){out.status=90;return out;}if(bytes.length<1961){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsMsgFrankaRobotState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsMsgFrankaRobotState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsMsgFrankaRobotState():changetype<Ros2Franka_msgsMsgFrankaRobotState>(out.value);}
}
/** Complete original franka_msgs/srv/SetCartesianStiffness_Request; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetCartesianStiffness_Request {
static readonly maxWireBytes:i32=48;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=48;
/** Exact native field cartesian_stiffness, native scalar float64. */
cartesian_stiffness:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<6;++i)this.cartesian_stiffness.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.cartesian_stiffness.length!=6)return -22;
for(let i=0;i<this.cartesian_stiffness.length;++i){
if(!isFinite(this.cartesian_stiffness[i]))return -22;
if(8>48-n)return -90;n+=8;
if(n>48)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.cartesian_stiffness.length;++i){
w.integer(<u64>reinterpret<u64>(this.cartesian_stiffness[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetCartesianStiffness_Request{const value=new Ros2Franka_msgsSrvSetCartesianStiffness_Request();
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.cartesian_stiffness=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.cartesian_stiffness.push(decoded);
}
}
return value;}
}
export class Ros2Franka_msgsSrvSetCartesianStiffness_RequestCodec implements VendorCodec<Ros2Franka_msgsSrvSetCartesianStiffness_Request>{
static readonly codecIdentity:string="UWVMROS1_ae78b9887a5419225cfe0727bac83d8c53f789acd53bd47f024485b02aa98260";
static readonly maxWireBytes:i32=48;
encodeChecked(value:Ros2Franka_msgsSrvSetCartesianStiffness_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetCartesianStiffness_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetCartesianStiffness_Request>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetCartesianStiffness_Request>();if(bytes.length>48){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetCartesianStiffness_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetCartesianStiffness_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetCartesianStiffness_Request():changetype<Ros2Franka_msgsSrvSetCartesianStiffness_Request>(out.value);}
}
/** Complete original franka_msgs/srv/SetCartesianStiffness_Response; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetCartesianStiffness_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field error, native scalar string. */
error:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.error)||this.error.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.error);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetCartesianStiffness_Response{const value=new Ros2Franka_msgsSrvSetCartesianStiffness_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error=decoded;
}
return value;}
}
export class Ros2Franka_msgsSrvSetCartesianStiffness_ResponseCodec implements VendorCodec<Ros2Franka_msgsSrvSetCartesianStiffness_Response>{
static readonly codecIdentity:string="UWVMROS1_3194f7469bbf228e653f5a93336fd5de4a1b2d7c1fe9542f3fadc8ad4fd51afa";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Franka_msgsSrvSetCartesianStiffness_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetCartesianStiffness_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetCartesianStiffness_Response>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetCartesianStiffness_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetCartesianStiffness_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetCartesianStiffness_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetCartesianStiffness_Response():changetype<Ros2Franka_msgsSrvSetCartesianStiffness_Response>(out.value);}
}
/** Complete original franka_msgs/srv/SetForceTorqueCollisionBehavior_Request; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request {
static readonly maxWireBytes:i32=208;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=208;
/** Exact native field lower_torque_thresholds_nominal, native scalar float64. */
lower_torque_thresholds_nominal:Array<f64>=new Array<f64>();
/** Exact native field upper_torque_thresholds_nominal, native scalar float64. */
upper_torque_thresholds_nominal:Array<f64>=new Array<f64>();
/** Exact native field lower_force_thresholds_nominal, native scalar float64. */
lower_force_thresholds_nominal:Array<f64>=new Array<f64>();
/** Exact native field upper_force_thresholds_nominal, native scalar float64. */
upper_force_thresholds_nominal:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<7;++i)this.lower_torque_thresholds_nominal.push(0);
for(let i=0;i<7;++i)this.upper_torque_thresholds_nominal.push(0);
for(let i=0;i<6;++i)this.lower_force_thresholds_nominal.push(0);
for(let i=0;i<6;++i)this.upper_force_thresholds_nominal.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.lower_torque_thresholds_nominal.length!=7)return -22;
for(let i=0;i<this.lower_torque_thresholds_nominal.length;++i){
if(!isFinite(this.lower_torque_thresholds_nominal[i]))return -22;
if(8>208-n)return -90;n+=8;
if(n>208)return -90;
}
}
{
if(this.upper_torque_thresholds_nominal.length!=7)return -22;
for(let i=0;i<this.upper_torque_thresholds_nominal.length;++i){
if(!isFinite(this.upper_torque_thresholds_nominal[i]))return -22;
if(8>208-n)return -90;n+=8;
if(n>208)return -90;
}
}
{
if(this.lower_force_thresholds_nominal.length!=6)return -22;
for(let i=0;i<this.lower_force_thresholds_nominal.length;++i){
if(!isFinite(this.lower_force_thresholds_nominal[i]))return -22;
if(8>208-n)return -90;n+=8;
if(n>208)return -90;
}
}
{
if(this.upper_force_thresholds_nominal.length!=6)return -22;
for(let i=0;i<this.upper_force_thresholds_nominal.length;++i){
if(!isFinite(this.upper_force_thresholds_nominal[i]))return -22;
if(8>208-n)return -90;n+=8;
if(n>208)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.lower_torque_thresholds_nominal.length;++i){
w.integer(<u64>reinterpret<u64>(this.lower_torque_thresholds_nominal[i]),8);
}
for(let i=0;i<this.upper_torque_thresholds_nominal.length;++i){
w.integer(<u64>reinterpret<u64>(this.upper_torque_thresholds_nominal[i]),8);
}
for(let i=0;i<this.lower_force_thresholds_nominal.length;++i){
w.integer(<u64>reinterpret<u64>(this.lower_force_thresholds_nominal[i]),8);
}
for(let i=0;i<this.upper_force_thresholds_nominal.length;++i){
w.integer(<u64>reinterpret<u64>(this.upper_force_thresholds_nominal[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request{const value=new Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request();
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.lower_torque_thresholds_nominal=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.lower_torque_thresholds_nominal.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.upper_torque_thresholds_nominal=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.upper_torque_thresholds_nominal.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.lower_force_thresholds_nominal=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.lower_force_thresholds_nominal.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.upper_force_thresholds_nominal=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.upper_force_thresholds_nominal.push(decoded);
}
}
return value;}
}
export class Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_RequestCodec implements VendorCodec<Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request>{
static readonly codecIdentity:string="UWVMROS1_829642a9ea418a53f7252f7489bd7f5ed976bf3689443012daf216db895cf99c";
static readonly maxWireBytes:i32=208;
encodeChecked(value:Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request>();if(bytes.length>208){out.status=90;return out;}if(bytes.length<208){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request():changetype<Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Request>(out.value);}
}
/** Complete original franka_msgs/srv/SetForceTorqueCollisionBehavior_Response; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field error, native scalar string. */
error:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.error)||this.error.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.error);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response{const value=new Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error=decoded;
}
return value;}
}
export class Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_ResponseCodec implements VendorCodec<Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response>{
static readonly codecIdentity:string="UWVMROS1_7d5640c34446e4262a8c00f0b242b68c0095688f0208fd692f21d855003eb47a";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response():changetype<Ros2Franka_msgsSrvSetForceTorqueCollisionBehavior_Response>(out.value);}
}
/** Complete original franka_msgs/srv/SetFullCollisionBehavior_Request; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetFullCollisionBehavior_Request {
static readonly maxWireBytes:i32=416;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=416;
/** Exact native field lower_torque_thresholds_acceleration, native scalar float64. */
lower_torque_thresholds_acceleration:Array<f64>=new Array<f64>();
/** Exact native field upper_torque_thresholds_acceleration, native scalar float64. */
upper_torque_thresholds_acceleration:Array<f64>=new Array<f64>();
/** Exact native field lower_torque_thresholds_nominal, native scalar float64. */
lower_torque_thresholds_nominal:Array<f64>=new Array<f64>();
/** Exact native field upper_torque_thresholds_nominal, native scalar float64. */
upper_torque_thresholds_nominal:Array<f64>=new Array<f64>();
/** Exact native field lower_force_thresholds_acceleration, native scalar float64. */
lower_force_thresholds_acceleration:Array<f64>=new Array<f64>();
/** Exact native field upper_force_thresholds_acceleration, native scalar float64. */
upper_force_thresholds_acceleration:Array<f64>=new Array<f64>();
/** Exact native field lower_force_thresholds_nominal, native scalar float64. */
lower_force_thresholds_nominal:Array<f64>=new Array<f64>();
/** Exact native field upper_force_thresholds_nominal, native scalar float64. */
upper_force_thresholds_nominal:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<7;++i)this.lower_torque_thresholds_acceleration.push(0);
for(let i=0;i<7;++i)this.upper_torque_thresholds_acceleration.push(0);
for(let i=0;i<7;++i)this.lower_torque_thresholds_nominal.push(0);
for(let i=0;i<7;++i)this.upper_torque_thresholds_nominal.push(0);
for(let i=0;i<6;++i)this.lower_force_thresholds_acceleration.push(0);
for(let i=0;i<6;++i)this.upper_force_thresholds_acceleration.push(0);
for(let i=0;i<6;++i)this.lower_force_thresholds_nominal.push(0);
for(let i=0;i<6;++i)this.upper_force_thresholds_nominal.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.lower_torque_thresholds_acceleration.length!=7)return -22;
for(let i=0;i<this.lower_torque_thresholds_acceleration.length;++i){
if(!isFinite(this.lower_torque_thresholds_acceleration[i]))return -22;
if(8>416-n)return -90;n+=8;
if(n>416)return -90;
}
}
{
if(this.upper_torque_thresholds_acceleration.length!=7)return -22;
for(let i=0;i<this.upper_torque_thresholds_acceleration.length;++i){
if(!isFinite(this.upper_torque_thresholds_acceleration[i]))return -22;
if(8>416-n)return -90;n+=8;
if(n>416)return -90;
}
}
{
if(this.lower_torque_thresholds_nominal.length!=7)return -22;
for(let i=0;i<this.lower_torque_thresholds_nominal.length;++i){
if(!isFinite(this.lower_torque_thresholds_nominal[i]))return -22;
if(8>416-n)return -90;n+=8;
if(n>416)return -90;
}
}
{
if(this.upper_torque_thresholds_nominal.length!=7)return -22;
for(let i=0;i<this.upper_torque_thresholds_nominal.length;++i){
if(!isFinite(this.upper_torque_thresholds_nominal[i]))return -22;
if(8>416-n)return -90;n+=8;
if(n>416)return -90;
}
}
{
if(this.lower_force_thresholds_acceleration.length!=6)return -22;
for(let i=0;i<this.lower_force_thresholds_acceleration.length;++i){
if(!isFinite(this.lower_force_thresholds_acceleration[i]))return -22;
if(8>416-n)return -90;n+=8;
if(n>416)return -90;
}
}
{
if(this.upper_force_thresholds_acceleration.length!=6)return -22;
for(let i=0;i<this.upper_force_thresholds_acceleration.length;++i){
if(!isFinite(this.upper_force_thresholds_acceleration[i]))return -22;
if(8>416-n)return -90;n+=8;
if(n>416)return -90;
}
}
{
if(this.lower_force_thresholds_nominal.length!=6)return -22;
for(let i=0;i<this.lower_force_thresholds_nominal.length;++i){
if(!isFinite(this.lower_force_thresholds_nominal[i]))return -22;
if(8>416-n)return -90;n+=8;
if(n>416)return -90;
}
}
{
if(this.upper_force_thresholds_nominal.length!=6)return -22;
for(let i=0;i<this.upper_force_thresholds_nominal.length;++i){
if(!isFinite(this.upper_force_thresholds_nominal[i]))return -22;
if(8>416-n)return -90;n+=8;
if(n>416)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.lower_torque_thresholds_acceleration.length;++i){
w.integer(<u64>reinterpret<u64>(this.lower_torque_thresholds_acceleration[i]),8);
}
for(let i=0;i<this.upper_torque_thresholds_acceleration.length;++i){
w.integer(<u64>reinterpret<u64>(this.upper_torque_thresholds_acceleration[i]),8);
}
for(let i=0;i<this.lower_torque_thresholds_nominal.length;++i){
w.integer(<u64>reinterpret<u64>(this.lower_torque_thresholds_nominal[i]),8);
}
for(let i=0;i<this.upper_torque_thresholds_nominal.length;++i){
w.integer(<u64>reinterpret<u64>(this.upper_torque_thresholds_nominal[i]),8);
}
for(let i=0;i<this.lower_force_thresholds_acceleration.length;++i){
w.integer(<u64>reinterpret<u64>(this.lower_force_thresholds_acceleration[i]),8);
}
for(let i=0;i<this.upper_force_thresholds_acceleration.length;++i){
w.integer(<u64>reinterpret<u64>(this.upper_force_thresholds_acceleration[i]),8);
}
for(let i=0;i<this.lower_force_thresholds_nominal.length;++i){
w.integer(<u64>reinterpret<u64>(this.lower_force_thresholds_nominal[i]),8);
}
for(let i=0;i<this.upper_force_thresholds_nominal.length;++i){
w.integer(<u64>reinterpret<u64>(this.upper_force_thresholds_nominal[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetFullCollisionBehavior_Request{const value=new Ros2Franka_msgsSrvSetFullCollisionBehavior_Request();
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.lower_torque_thresholds_acceleration=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.lower_torque_thresholds_acceleration.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.upper_torque_thresholds_acceleration=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.upper_torque_thresholds_acceleration.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.lower_torque_thresholds_nominal=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.lower_torque_thresholds_nominal.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.upper_torque_thresholds_nominal=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.upper_torque_thresholds_nominal.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.lower_force_thresholds_acceleration=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.lower_force_thresholds_acceleration.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.upper_force_thresholds_acceleration=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.upper_force_thresholds_acceleration.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.lower_force_thresholds_nominal=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.lower_force_thresholds_nominal.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.upper_force_thresholds_nominal=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.upper_force_thresholds_nominal.push(decoded);
}
}
return value;}
}
export class Ros2Franka_msgsSrvSetFullCollisionBehavior_RequestCodec implements VendorCodec<Ros2Franka_msgsSrvSetFullCollisionBehavior_Request>{
static readonly codecIdentity:string="UWVMROS1_d6898555d931aee3a5b4f5f97830cb9982f2e378c59c40bcaa0201840d2f2824";
static readonly maxWireBytes:i32=416;
encodeChecked(value:Ros2Franka_msgsSrvSetFullCollisionBehavior_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetFullCollisionBehavior_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetFullCollisionBehavior_Request>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetFullCollisionBehavior_Request>();if(bytes.length>416){out.status=90;return out;}if(bytes.length<416){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetFullCollisionBehavior_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetFullCollisionBehavior_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetFullCollisionBehavior_Request():changetype<Ros2Franka_msgsSrvSetFullCollisionBehavior_Request>(out.value);}
}
/** Complete original franka_msgs/srv/SetFullCollisionBehavior_Response; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetFullCollisionBehavior_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field error, native scalar string. */
error:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.error)||this.error.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.error);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetFullCollisionBehavior_Response{const value=new Ros2Franka_msgsSrvSetFullCollisionBehavior_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error=decoded;
}
return value;}
}
export class Ros2Franka_msgsSrvSetFullCollisionBehavior_ResponseCodec implements VendorCodec<Ros2Franka_msgsSrvSetFullCollisionBehavior_Response>{
static readonly codecIdentity:string="UWVMROS1_76430e384e9ad8b3f3c937e3004e8dc15a51d28abecd108c698e60d39c54f300";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Franka_msgsSrvSetFullCollisionBehavior_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetFullCollisionBehavior_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetFullCollisionBehavior_Response>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetFullCollisionBehavior_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetFullCollisionBehavior_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetFullCollisionBehavior_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetFullCollisionBehavior_Response():changetype<Ros2Franka_msgsSrvSetFullCollisionBehavior_Response>(out.value);}
}
/** Complete original franka_msgs/srv/SetJointStiffness_Request; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetJointStiffness_Request {
static readonly maxWireBytes:i32=56;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=56;
/** Exact native field joint_stiffness, native scalar float64. */
joint_stiffness:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<7;++i)this.joint_stiffness.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.joint_stiffness.length!=7)return -22;
for(let i=0;i<this.joint_stiffness.length;++i){
if(!isFinite(this.joint_stiffness[i]))return -22;
if(8>56-n)return -90;n+=8;
if(n>56)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.joint_stiffness.length;++i){
w.integer(<u64>reinterpret<u64>(this.joint_stiffness[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetJointStiffness_Request{const value=new Ros2Franka_msgsSrvSetJointStiffness_Request();
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.joint_stiffness=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.joint_stiffness.push(decoded);
}
}
return value;}
}
export class Ros2Franka_msgsSrvSetJointStiffness_RequestCodec implements VendorCodec<Ros2Franka_msgsSrvSetJointStiffness_Request>{
static readonly codecIdentity:string="UWVMROS1_65b5d6599d1b97a0356f599102c22e0ae8c33b85f14c5a49b6c097eb227d334c";
static readonly maxWireBytes:i32=56;
encodeChecked(value:Ros2Franka_msgsSrvSetJointStiffness_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetJointStiffness_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetJointStiffness_Request>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetJointStiffness_Request>();if(bytes.length>56){out.status=90;return out;}if(bytes.length<56){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetJointStiffness_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetJointStiffness_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetJointStiffness_Request():changetype<Ros2Franka_msgsSrvSetJointStiffness_Request>(out.value);}
}
/** Complete original franka_msgs/srv/SetJointStiffness_Response; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetJointStiffness_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field error, native scalar string. */
error:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.error)||this.error.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.error);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetJointStiffness_Response{const value=new Ros2Franka_msgsSrvSetJointStiffness_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error=decoded;
}
return value;}
}
export class Ros2Franka_msgsSrvSetJointStiffness_ResponseCodec implements VendorCodec<Ros2Franka_msgsSrvSetJointStiffness_Response>{
static readonly codecIdentity:string="UWVMROS1_1a857ab809079e0eec664250a9521ddc1cdf66e61aa2e0f22eae8442f5231c4d";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Franka_msgsSrvSetJointStiffness_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetJointStiffness_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetJointStiffness_Response>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetJointStiffness_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetJointStiffness_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetJointStiffness_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetJointStiffness_Response():changetype<Ros2Franka_msgsSrvSetJointStiffness_Response>(out.value);}
}
/** Complete original franka_msgs/srv/SetLoad_Request; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetLoad_Request {
static readonly maxWireBytes:i32=104;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=104;
/** Exact native field mass, native scalar float64. */
mass:f64=0;
/** Exact native field center_of_mass, native scalar float64. */
center_of_mass:Array<f64>=new Array<f64>();
/** Exact native field load_inertia, native scalar float64. */
load_inertia:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<3;++i)this.center_of_mass.push(0);
for(let i=0;i<9;++i)this.load_inertia.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.mass))return -22;
if(8>104-n)return -90;n+=8;
if(n>104)return -90;
}
{
if(this.center_of_mass.length!=3)return -22;
for(let i=0;i<this.center_of_mass.length;++i){
if(!isFinite(this.center_of_mass[i]))return -22;
if(8>104-n)return -90;n+=8;
if(n>104)return -90;
}
}
{
if(this.load_inertia.length!=9)return -22;
for(let i=0;i<this.load_inertia.length;++i){
if(!isFinite(this.load_inertia[i]))return -22;
if(8>104-n)return -90;n+=8;
if(n>104)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.mass),8);
for(let i=0;i<this.center_of_mass.length;++i){
w.integer(<u64>reinterpret<u64>(this.center_of_mass[i]),8);
}
for(let i=0;i<this.load_inertia.length;++i){
w.integer(<u64>reinterpret<u64>(this.load_inertia[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetLoad_Request{const value=new Ros2Franka_msgsSrvSetLoad_Request();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.mass=decoded;
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.center_of_mass=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.center_of_mass.push(decoded);
}
}
{
const count:u32=9;if(!r.valid||count>9){r.valid=false;return value;}
value.load_inertia=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.load_inertia.push(decoded);
}
}
return value;}
}
export class Ros2Franka_msgsSrvSetLoad_RequestCodec implements VendorCodec<Ros2Franka_msgsSrvSetLoad_Request>{
static readonly codecIdentity:string="UWVMROS1_a616a844864b7bbf4ee18ac481ddd8406bec47354b4519a0e17f8a305635b7f8";
static readonly maxWireBytes:i32=104;
encodeChecked(value:Ros2Franka_msgsSrvSetLoad_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetLoad_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetLoad_Request>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetLoad_Request>();if(bytes.length>104){out.status=90;return out;}if(bytes.length<104){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetLoad_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetLoad_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetLoad_Request():changetype<Ros2Franka_msgsSrvSetLoad_Request>(out.value);}
}
/** Complete original franka_msgs/srv/SetLoad_Response; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetLoad_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field error, native scalar string. */
error:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.error)||this.error.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.error);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetLoad_Response{const value=new Ros2Franka_msgsSrvSetLoad_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error=decoded;
}
return value;}
}
export class Ros2Franka_msgsSrvSetLoad_ResponseCodec implements VendorCodec<Ros2Franka_msgsSrvSetLoad_Response>{
static readonly codecIdentity:string="UWVMROS1_991ff575719ef9442063a13c45a9660c672f5607e6e40cf6ab5c7a3173fa00bf";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Franka_msgsSrvSetLoad_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetLoad_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetLoad_Response>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetLoad_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetLoad_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetLoad_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetLoad_Response():changetype<Ros2Franka_msgsSrvSetLoad_Response>(out.value);}
}
/** Complete original franka_msgs/srv/SetStiffnessFrame_Request; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetStiffnessFrame_Request {
static readonly maxWireBytes:i32=128;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=128;
/** Exact native field transformation, native scalar float64. */
transformation:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<16;++i)this.transformation.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.transformation.length!=16)return -22;
for(let i=0;i<this.transformation.length;++i){
if(!isFinite(this.transformation[i]))return -22;
if(8>128-n)return -90;n+=8;
if(n>128)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.transformation.length;++i){
w.integer(<u64>reinterpret<u64>(this.transformation[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetStiffnessFrame_Request{const value=new Ros2Franka_msgsSrvSetStiffnessFrame_Request();
{
const count:u32=16;if(!r.valid||count>16){r.valid=false;return value;}
value.transformation=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.transformation.push(decoded);
}
}
return value;}
}
export class Ros2Franka_msgsSrvSetStiffnessFrame_RequestCodec implements VendorCodec<Ros2Franka_msgsSrvSetStiffnessFrame_Request>{
static readonly codecIdentity:string="UWVMROS1_f8ee91daba1642286b63178c18bf2a005597e94951f11a185241193132c9f402";
static readonly maxWireBytes:i32=128;
encodeChecked(value:Ros2Franka_msgsSrvSetStiffnessFrame_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetStiffnessFrame_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetStiffnessFrame_Request>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetStiffnessFrame_Request>();if(bytes.length>128){out.status=90;return out;}if(bytes.length<128){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetStiffnessFrame_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetStiffnessFrame_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetStiffnessFrame_Request():changetype<Ros2Franka_msgsSrvSetStiffnessFrame_Request>(out.value);}
}
/** Complete original franka_msgs/srv/SetStiffnessFrame_Response; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetStiffnessFrame_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field error, native scalar string. */
error:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.error)||this.error.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.error);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetStiffnessFrame_Response{const value=new Ros2Franka_msgsSrvSetStiffnessFrame_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error=decoded;
}
return value;}
}
export class Ros2Franka_msgsSrvSetStiffnessFrame_ResponseCodec implements VendorCodec<Ros2Franka_msgsSrvSetStiffnessFrame_Response>{
static readonly codecIdentity:string="UWVMROS1_e32cb83b1f0f3cda363566525c0b1fe367190f2e66f68282b330f4517f6ea744";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Franka_msgsSrvSetStiffnessFrame_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetStiffnessFrame_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetStiffnessFrame_Response>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetStiffnessFrame_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetStiffnessFrame_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetStiffnessFrame_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetStiffnessFrame_Response():changetype<Ros2Franka_msgsSrvSetStiffnessFrame_Response>(out.value);}
}
/** Complete original franka_msgs/srv/SetTCPFrame_Request; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetTCPFrame_Request {
static readonly maxWireBytes:i32=128;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=128;
/** Exact native field transformation, native scalar float64. */
transformation:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<16;++i)this.transformation.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.transformation.length!=16)return -22;
for(let i=0;i<this.transformation.length;++i){
if(!isFinite(this.transformation[i]))return -22;
if(8>128-n)return -90;n+=8;
if(n>128)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.transformation.length;++i){
w.integer(<u64>reinterpret<u64>(this.transformation[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetTCPFrame_Request{const value=new Ros2Franka_msgsSrvSetTCPFrame_Request();
{
const count:u32=16;if(!r.valid||count>16){r.valid=false;return value;}
value.transformation=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.transformation.push(decoded);
}
}
return value;}
}
export class Ros2Franka_msgsSrvSetTCPFrame_RequestCodec implements VendorCodec<Ros2Franka_msgsSrvSetTCPFrame_Request>{
static readonly codecIdentity:string="UWVMROS1_2ef445bfa494a6bc50ee3337695562255c06890216c30ba17df95e691871441f";
static readonly maxWireBytes:i32=128;
encodeChecked(value:Ros2Franka_msgsSrvSetTCPFrame_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetTCPFrame_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetTCPFrame_Request>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetTCPFrame_Request>();if(bytes.length>128){out.status=90;return out;}if(bytes.length<128){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetTCPFrame_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetTCPFrame_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetTCPFrame_Request():changetype<Ros2Franka_msgsSrvSetTCPFrame_Request>(out.value);}
}
/** Complete original franka_msgs/srv/SetTCPFrame_Response; spellings are published in generation proof. */
export class Ros2Franka_msgsSrvSetTCPFrame_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field error, native scalar string. */
error:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.error)||this.error.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.error);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_msgsSrvSetTCPFrame_Response{const value=new Ros2Franka_msgsSrvSetTCPFrame_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error=decoded;
}
return value;}
}
export class Ros2Franka_msgsSrvSetTCPFrame_ResponseCodec implements VendorCodec<Ros2Franka_msgsSrvSetTCPFrame_Response>{
static readonly codecIdentity:string="UWVMROS1_87e4f5556c292289fe17898a40f1f5dd6161e7cc87ca570c86e9f69080d1df38";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Franka_msgsSrvSetTCPFrame_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_msgsSrvSetTCPFrame_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_msgsSrvSetTCPFrame_Response>{const out=new VendorDecoded<Ros2Franka_msgsSrvSetTCPFrame_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_msgsSrvSetTCPFrame_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_msgsSrvSetTCPFrame_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_msgsSrvSetTCPFrame_Response():changetype<Ros2Franka_msgsSrvSetTCPFrame_Response>(out.value);}
}
/** Complete original franka_spine_msgs/action/MoveAbsolute_Goal; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsActionMoveAbsolute_Goal {
static readonly maxWireBytes:i32=32;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=32;
/** Exact native field position, native scalar float64. */
position:f64=0;
/** Exact native field velocity, native scalar float64. */
velocity:f64=0;
/** Exact native field acceleration, native scalar float64. */
acceleration:f64=0;
/** Exact native field deceleration, native scalar float64. */
deceleration:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.position))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.velocity))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.acceleration))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
{
if(!isFinite(this.deceleration))return -22;
if(8>32-n)return -90;n+=8;
if(n>32)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.position),8);
w.integer(<u64>reinterpret<u64>(this.velocity),8);
w.integer(<u64>reinterpret<u64>(this.acceleration),8);
w.integer(<u64>reinterpret<u64>(this.deceleration),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsActionMoveAbsolute_Goal{const value=new Ros2Franka_spine_msgsActionMoveAbsolute_Goal();
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
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.deceleration=decoded;
}
return value;}
}
export class Ros2Franka_spine_msgsActionMoveAbsolute_GoalCodec implements VendorCodec<Ros2Franka_spine_msgsActionMoveAbsolute_Goal>{
static readonly codecIdentity:string="UWVMROS1_a0c2f9037fd6730641b19b6da6ce1c6ca03bbd517fdd791c159effc93290cf13";
static readonly maxWireBytes:i32=32;
encodeChecked(value:Ros2Franka_spine_msgsActionMoveAbsolute_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsActionMoveAbsolute_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsActionMoveAbsolute_Goal>{const out=new VendorDecoded<Ros2Franka_spine_msgsActionMoveAbsolute_Goal>();if(bytes.length>32){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsActionMoveAbsolute_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsActionMoveAbsolute_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsActionMoveAbsolute_Goal():changetype<Ros2Franka_spine_msgsActionMoveAbsolute_Goal>(out.value);}
}
/** Complete original franka_spine_msgs/action/MoveAbsolute_Result; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsActionMoveAbsolute_Result {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field stop_by, native scalar string. */
stop_by:string="";
/** Exact native field error, native scalar string. */
error:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
{
if(!validUnicode(this.stop_by)||this.stop_by.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.stop_by);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(!validUnicode(this.error)||this.error.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.success?1:0);
w.text(this.stop_by);
w.text(this.error);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsActionMoveAbsolute_Result{const value=new Ros2Franka_spine_msgsActionMoveAbsolute_Result();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.stop_by=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error=decoded;
}
return value;}
}
export class Ros2Franka_spine_msgsActionMoveAbsolute_ResultCodec implements VendorCodec<Ros2Franka_spine_msgsActionMoveAbsolute_Result>{
static readonly codecIdentity:string="UWVMROS1_86b5c823e6e6604595462a76cd259daef420b2dff169f6dbe00f742f079155d5";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Franka_spine_msgsActionMoveAbsolute_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsActionMoveAbsolute_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsActionMoveAbsolute_Result>{const out=new VendorDecoded<Ros2Franka_spine_msgsActionMoveAbsolute_Result>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsActionMoveAbsolute_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsActionMoveAbsolute_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsActionMoveAbsolute_Result():changetype<Ros2Franka_spine_msgsActionMoveAbsolute_Result>(out.value);}
}
/** Complete original franka_spine_msgs/action/MoveAbsolute_Feedback; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsActionMoveAbsolute_Feedback {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field current_position, native scalar float64. */
current_position:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.current_position))return -22;
if(8>8-n)return -90;n+=8;
if(n>8)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.current_position),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsActionMoveAbsolute_Feedback{const value=new Ros2Franka_spine_msgsActionMoveAbsolute_Feedback();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.current_position=decoded;
}
return value;}
}
export class Ros2Franka_spine_msgsActionMoveAbsolute_FeedbackCodec implements VendorCodec<Ros2Franka_spine_msgsActionMoveAbsolute_Feedback>{
static readonly codecIdentity:string="UWVMROS1_cf2f760313ab8c6cc01dd79271619926320021746451f376e56d18f63ffad929";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Franka_spine_msgsActionMoveAbsolute_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsActionMoveAbsolute_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsActionMoveAbsolute_Feedback>{const out=new VendorDecoded<Ros2Franka_spine_msgsActionMoveAbsolute_Feedback>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsActionMoveAbsolute_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsActionMoveAbsolute_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsActionMoveAbsolute_Feedback():changetype<Ros2Franka_spine_msgsActionMoveAbsolute_Feedback>(out.value);}
}
/** Complete original franka_spine_msgs/msg/UserLimits; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsMsgUserLimits {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field lower_limit, native scalar float64. */
lower_limit:f64=0;
/** Exact native field upper_limit, native scalar float64. */
upper_limit:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.lower_limit))return -22;
if(8>16-n)return -90;n+=8;
if(n>16)return -90;
}
{
if(!isFinite(this.upper_limit))return -22;
if(8>16-n)return -90;n+=8;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.lower_limit),8);
w.integer(<u64>reinterpret<u64>(this.upper_limit),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsMsgUserLimits{const value=new Ros2Franka_spine_msgsMsgUserLimits();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.lower_limit=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.upper_limit=decoded;
}
return value;}
}
export class Ros2Franka_spine_msgsMsgUserLimitsCodec implements VendorCodec<Ros2Franka_spine_msgsMsgUserLimits>{
static readonly codecIdentity:string="UWVMROS1_20256e37f0ae710b5cb9ab9438b5ff8144ce2ab6fc606c5335d0f5de5384e368";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Franka_spine_msgsMsgUserLimits):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsMsgUserLimits):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsMsgUserLimits>{const out=new VendorDecoded<Ros2Franka_spine_msgsMsgUserLimits>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsMsgUserLimits.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsMsgUserLimits{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsMsgUserLimits():changetype<Ros2Franka_spine_msgsMsgUserLimits>(out.value);}
}
/** Complete original franka_spine_msgs/msg/SpineParameters; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsMsgSpineParameters {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field user_limits, native scalar franka_spine_msgs/msg/UserLimits. */
user_limits:Ros2Franka_spine_msgsMsgUserLimits=new Ros2Franka_spine_msgsMsgUserLimits();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.user_limits.wireSize();if(child<0)return child;if(child>16-n)return -90;n+=child;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.user_limits.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsMsgSpineParameters{const value=new Ros2Franka_spine_msgsMsgSpineParameters();
{
const decoded=Ros2Franka_spine_msgsMsgUserLimits.readControl(r);
value.user_limits=decoded;
}
return value;}
}
export class Ros2Franka_spine_msgsMsgSpineParametersCodec implements VendorCodec<Ros2Franka_spine_msgsMsgSpineParameters>{
static readonly codecIdentity:string="UWVMROS1_4d4c45cfccf278c99a189e92d438d0b11b2b693012cb0f76b65b6822eb816223";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Franka_spine_msgsMsgSpineParameters):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsMsgSpineParameters):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsMsgSpineParameters>{const out=new VendorDecoded<Ros2Franka_spine_msgsMsgSpineParameters>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsMsgSpineParameters.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsMsgSpineParameters{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsMsgSpineParameters():changetype<Ros2Franka_spine_msgsMsgSpineParameters>(out.value);}
}
/** Complete original franka_spine_msgs/msg/SpinePosition; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsMsgSpinePosition {
static readonly maxWireBytes:i32=149;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=149;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field position, native scalar float64. */
position:f64=0;
/** Exact native field valid, native scalar bool. */
valid:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>149-n)return -90;n+=child;
if(n>149)return -90;
}
{
if(!isFinite(this.position))return -22;
if(8>149-n)return -90;n+=8;
if(n>149)return -90;
}
{
if(1>149-n)return -90;n+=1;
if(n>149)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>reinterpret<u64>(this.position),8);
w.u8(this.valid?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsMsgSpinePosition{const value=new Ros2Franka_spine_msgsMsgSpinePosition();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.position=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.valid=decoded;
}
return value;}
}
export class Ros2Franka_spine_msgsMsgSpinePositionCodec implements VendorCodec<Ros2Franka_spine_msgsMsgSpinePosition>{
static readonly codecIdentity:string="UWVMROS1_71be15c40f39074dfdbe79a3587847ae512c5a3c8df23cc9415d5bfe59d05775";
static readonly maxWireBytes:i32=149;
encodeChecked(value:Ros2Franka_spine_msgsMsgSpinePosition):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsMsgSpinePosition):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsMsgSpinePosition>{const out=new VendorDecoded<Ros2Franka_spine_msgsMsgSpinePosition>();if(bytes.length>149){out.status=90;return out;}if(bytes.length<21){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsMsgSpinePosition.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsMsgSpinePosition{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsMsgSpinePosition():changetype<Ros2Franka_spine_msgsMsgSpinePosition>(out.value);}
}
/** Complete original franka_spine_msgs/msg/SpineState; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsMsgSpineState {
static readonly maxWireBytes:i32=921;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=921;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field state, native scalar string. */
state:string="";
/** Exact native field error_code, native scalar string. */
error_code:string="";
/** Exact native field error_description, native scalar string. */
error_description:string="";
/** Exact native field valid, native scalar bool. */
valid:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>921-n)return -90;n+=child;
if(n>921)return -90;
}
{
if(!validUnicode(this.state)||this.state.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.state);if(size>256)return -22;if(4+size>921-n)return -90;n+=4+size;
if(n>921)return -90;
}
{
if(!validUnicode(this.error_code)||this.error_code.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error_code);if(size>256)return -22;if(4+size>921-n)return -90;n+=4+size;
if(n>921)return -90;
}
{
if(!validUnicode(this.error_description)||this.error_description.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error_description);if(size>256)return -22;if(4+size>921-n)return -90;n+=4+size;
if(n>921)return -90;
}
{
if(1>921-n)return -90;n+=1;
if(n>921)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.text(this.state);
w.text(this.error_code);
w.text(this.error_description);
w.u8(this.valid?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsMsgSpineState{const value=new Ros2Franka_spine_msgsMsgSpineState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.state=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error_description=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.valid=decoded;
}
return value;}
}
export class Ros2Franka_spine_msgsMsgSpineStateCodec implements VendorCodec<Ros2Franka_spine_msgsMsgSpineState>{
static readonly codecIdentity:string="UWVMROS1_ad0b5dc5d79956da854582765c147d3d33edab569c4829e69cc7990edfb89441";
static readonly maxWireBytes:i32=921;
encodeChecked(value:Ros2Franka_spine_msgsMsgSpineState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsMsgSpineState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsMsgSpineState>{const out=new VendorDecoded<Ros2Franka_spine_msgsMsgSpineState>();if(bytes.length>921){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsMsgSpineState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsMsgSpineState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsMsgSpineState():changetype<Ros2Franka_spine_msgsMsgSpineState>(out.value);}
}
/** Complete original franka_spine_msgs/srv/FaultReset_Request; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvFaultReset_Request {
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
static readControl(r:Reader):Ros2Franka_spine_msgsSrvFaultReset_Request{const value=new Ros2Franka_spine_msgsSrvFaultReset_Request();
return value;}
}
export class Ros2Franka_spine_msgsSrvFaultReset_RequestCodec implements VendorCodec<Ros2Franka_spine_msgsSrvFaultReset_Request>{
static readonly codecIdentity:string="UWVMROS1_0f836229e8e8a1713b9e9a8d7f0f43677ad93380e1f7350ff1597d25067b7f33";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Franka_spine_msgsSrvFaultReset_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvFaultReset_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvFaultReset_Request>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvFaultReset_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvFaultReset_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvFaultReset_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvFaultReset_Request():changetype<Ros2Franka_spine_msgsSrvFaultReset_Request>(out.value);}
}
/** Complete original franka_spine_msgs/srv/FaultReset_Response; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvFaultReset_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field state, native scalar string. */
state:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.state)||this.state.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.state);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.state);
w.u8(this.success?1:0);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsSrvFaultReset_Response{const value=new Ros2Franka_spine_msgsSrvFaultReset_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.state=decoded;
}
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
export class Ros2Franka_spine_msgsSrvFaultReset_ResponseCodec implements VendorCodec<Ros2Franka_spine_msgsSrvFaultReset_Response>{
static readonly codecIdentity:string="UWVMROS1_1ebeded0dbe4a4197a52de3fa43dc57fa372bd4e6ef5e2ded2558ccbf691410c";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Franka_spine_msgsSrvFaultReset_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvFaultReset_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvFaultReset_Response>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvFaultReset_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvFaultReset_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvFaultReset_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvFaultReset_Response():changetype<Ros2Franka_spine_msgsSrvFaultReset_Response>(out.value);}
}
/** Complete original franka_spine_msgs/srv/GetParameters_Request; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvGetParameters_Request {
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
static readControl(r:Reader):Ros2Franka_spine_msgsSrvGetParameters_Request{const value=new Ros2Franka_spine_msgsSrvGetParameters_Request();
return value;}
}
export class Ros2Franka_spine_msgsSrvGetParameters_RequestCodec implements VendorCodec<Ros2Franka_spine_msgsSrvGetParameters_Request>{
static readonly codecIdentity:string="UWVMROS1_80ef5d2ef6ff2df9d14f29417d1928429315c0b7be9ec59513b9b3943a970b14";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Franka_spine_msgsSrvGetParameters_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvGetParameters_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvGetParameters_Request>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvGetParameters_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvGetParameters_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvGetParameters_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvGetParameters_Request():changetype<Ros2Franka_spine_msgsSrvGetParameters_Request>(out.value);}
}
/** Complete original franka_spine_msgs/srv/GetParameters_Response; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvGetParameters_Response {
static readonly maxWireBytes:i32=277;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=277;
/** Exact native field parameters, native scalar franka_spine_msgs/msg/SpineParameters. */
parameters:Ros2Franka_spine_msgsMsgSpineParameters=new Ros2Franka_spine_msgsMsgSpineParameters();
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.parameters.wireSize();if(child<0)return child;if(child>277-n)return -90;n+=child;
if(n>277)return -90;
}
{
if(1>277-n)return -90;n+=1;
if(n>277)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>277-n)return -90;n+=4+size;
if(n>277)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.parameters.writeControl(w);
w.u8(this.success?1:0);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsSrvGetParameters_Response{const value=new Ros2Franka_spine_msgsSrvGetParameters_Response();
{
const decoded=Ros2Franka_spine_msgsMsgSpineParameters.readControl(r);
value.parameters=decoded;
}
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
export class Ros2Franka_spine_msgsSrvGetParameters_ResponseCodec implements VendorCodec<Ros2Franka_spine_msgsSrvGetParameters_Response>{
static readonly codecIdentity:string="UWVMROS1_8e648792534d7f4996179f31d353d2594cf63031edef4451fd20a3838dc896ed";
static readonly maxWireBytes:i32=277;
encodeChecked(value:Ros2Franka_spine_msgsSrvGetParameters_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvGetParameters_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvGetParameters_Response>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvGetParameters_Response>();if(bytes.length>277){out.status=90;return out;}if(bytes.length<21){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvGetParameters_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvGetParameters_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvGetParameters_Response():changetype<Ros2Franka_spine_msgsSrvGetParameters_Response>(out.value);}
}
/** Complete original franka_spine_msgs/srv/GetPosition_Request; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvGetPosition_Request {
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
static readControl(r:Reader):Ros2Franka_spine_msgsSrvGetPosition_Request{const value=new Ros2Franka_spine_msgsSrvGetPosition_Request();
return value;}
}
export class Ros2Franka_spine_msgsSrvGetPosition_RequestCodec implements VendorCodec<Ros2Franka_spine_msgsSrvGetPosition_Request>{
static readonly codecIdentity:string="UWVMROS1_66b05d1f08331161cafed301b3ab95a60dbb16aeb1a930ee780db163cb37c6c2";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Franka_spine_msgsSrvGetPosition_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvGetPosition_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvGetPosition_Request>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvGetPosition_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvGetPosition_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvGetPosition_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvGetPosition_Request():changetype<Ros2Franka_spine_msgsSrvGetPosition_Request>(out.value);}
}
/** Complete original franka_spine_msgs/srv/GetPosition_Response; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvGetPosition_Response {
static readonly maxWireBytes:i32=9;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=9;
/** Exact native field position, native scalar float64. */
position:f64=0;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.position))return -22;
if(8>9-n)return -90;n+=8;
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
w.integer(<u64>reinterpret<u64>(this.position),8);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsSrvGetPosition_Response{const value=new Ros2Franka_spine_msgsSrvGetPosition_Response();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.position=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Franka_spine_msgsSrvGetPosition_ResponseCodec implements VendorCodec<Ros2Franka_spine_msgsSrvGetPosition_Response>{
static readonly codecIdentity:string="UWVMROS1_18d9af57cc9f82c3c03d01e850da254e9431820b8291f03dafd2e8f74e849ab8";
static readonly maxWireBytes:i32=9;
encodeChecked(value:Ros2Franka_spine_msgsSrvGetPosition_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvGetPosition_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvGetPosition_Response>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvGetPosition_Response>();if(bytes.length>9){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvGetPosition_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvGetPosition_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvGetPosition_Response():changetype<Ros2Franka_spine_msgsSrvGetPosition_Response>(out.value);}
}
/** Complete original franka_spine_msgs/srv/GetSpineState_Request; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvGetSpineState_Request {
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
static readControl(r:Reader):Ros2Franka_spine_msgsSrvGetSpineState_Request{const value=new Ros2Franka_spine_msgsSrvGetSpineState_Request();
return value;}
}
export class Ros2Franka_spine_msgsSrvGetSpineState_RequestCodec implements VendorCodec<Ros2Franka_spine_msgsSrvGetSpineState_Request>{
static readonly codecIdentity:string="UWVMROS1_c4c456174d7b6647d09d986d37c041b942418941a5e01c447486727e919b8a67";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Franka_spine_msgsSrvGetSpineState_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvGetSpineState_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvGetSpineState_Request>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvGetSpineState_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvGetSpineState_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvGetSpineState_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvGetSpineState_Request():changetype<Ros2Franka_spine_msgsSrvGetSpineState_Request>(out.value);}
}
/** Complete original franka_spine_msgs/srv/GetSpineState_Response; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvGetSpineState_Response {
static readonly maxWireBytes:i32=781;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=781;
/** Exact native field state, native scalar string. */
state:string="";
/** Exact native field error_code, native scalar string. */
error_code:string="";
/** Exact native field error_description, native scalar string. */
error_description:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.state)||this.state.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.state);if(size>256)return -22;if(4+size>781-n)return -90;n+=4+size;
if(n>781)return -90;
}
{
if(!validUnicode(this.error_code)||this.error_code.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error_code);if(size>256)return -22;if(4+size>781-n)return -90;n+=4+size;
if(n>781)return -90;
}
{
if(!validUnicode(this.error_description)||this.error_description.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error_description);if(size>256)return -22;if(4+size>781-n)return -90;n+=4+size;
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
w.text(this.state);
w.text(this.error_code);
w.text(this.error_description);
w.u8(this.success?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsSrvGetSpineState_Response{const value=new Ros2Franka_spine_msgsSrvGetSpineState_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.state=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error_description=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success=decoded;
}
return value;}
}
export class Ros2Franka_spine_msgsSrvGetSpineState_ResponseCodec implements VendorCodec<Ros2Franka_spine_msgsSrvGetSpineState_Response>{
static readonly codecIdentity:string="UWVMROS1_297d70e2c5c24dcea4bdf8c5f9e5c8c32256e8f35f1900d8afd914ab2acfdcd1";
static readonly maxWireBytes:i32=781;
encodeChecked(value:Ros2Franka_spine_msgsSrvGetSpineState_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvGetSpineState_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvGetSpineState_Response>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvGetSpineState_Response>();if(bytes.length>781){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvGetSpineState_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvGetSpineState_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvGetSpineState_Response():changetype<Ros2Franka_spine_msgsSrvGetSpineState_Response>(out.value);}
}
/** Complete original franka_spine_msgs/srv/Halt_Request; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvHalt_Request {
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
static readControl(r:Reader):Ros2Franka_spine_msgsSrvHalt_Request{const value=new Ros2Franka_spine_msgsSrvHalt_Request();
return value;}
}
export class Ros2Franka_spine_msgsSrvHalt_RequestCodec implements VendorCodec<Ros2Franka_spine_msgsSrvHalt_Request>{
static readonly codecIdentity:string="UWVMROS1_50af7625abf7ec20a30ec49b01e8d32de2ee0d4710820c8de659df211be8c111";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Franka_spine_msgsSrvHalt_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvHalt_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvHalt_Request>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvHalt_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvHalt_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvHalt_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvHalt_Request():changetype<Ros2Franka_spine_msgsSrvHalt_Request>(out.value);}
}
/** Complete original franka_spine_msgs/srv/Halt_Response; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvHalt_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field state, native scalar string. */
state:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.state)||this.state.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.state);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.state);
w.u8(this.success?1:0);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsSrvHalt_Response{const value=new Ros2Franka_spine_msgsSrvHalt_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.state=decoded;
}
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
export class Ros2Franka_spine_msgsSrvHalt_ResponseCodec implements VendorCodec<Ros2Franka_spine_msgsSrvHalt_Response>{
static readonly codecIdentity:string="UWVMROS1_150e1d90b485a6ddf79f771379a0e1d3f99a77f88ab1cf1f569d614a63ac3d77";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Franka_spine_msgsSrvHalt_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvHalt_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvHalt_Response>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvHalt_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvHalt_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvHalt_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvHalt_Response():changetype<Ros2Franka_spine_msgsSrvHalt_Response>(out.value);}
}
/** Complete original franka_spine_msgs/srv/SwitchOff_Request; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvSwitchOff_Request {
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
static readControl(r:Reader):Ros2Franka_spine_msgsSrvSwitchOff_Request{const value=new Ros2Franka_spine_msgsSrvSwitchOff_Request();
return value;}
}
export class Ros2Franka_spine_msgsSrvSwitchOff_RequestCodec implements VendorCodec<Ros2Franka_spine_msgsSrvSwitchOff_Request>{
static readonly codecIdentity:string="UWVMROS1_097f00adfdc8d1a49615792158a36052917c41385683b39de6686f591964fc16";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Franka_spine_msgsSrvSwitchOff_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvSwitchOff_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvSwitchOff_Request>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvSwitchOff_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvSwitchOff_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvSwitchOff_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvSwitchOff_Request():changetype<Ros2Franka_spine_msgsSrvSwitchOff_Request>(out.value);}
}
/** Complete original franka_spine_msgs/srv/SwitchOff_Response; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvSwitchOff_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field state, native scalar string. */
state:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.state)||this.state.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.state);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.state);
w.u8(this.success?1:0);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsSrvSwitchOff_Response{const value=new Ros2Franka_spine_msgsSrvSwitchOff_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.state=decoded;
}
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
export class Ros2Franka_spine_msgsSrvSwitchOff_ResponseCodec implements VendorCodec<Ros2Franka_spine_msgsSrvSwitchOff_Response>{
static readonly codecIdentity:string="UWVMROS1_d3ab8516e62428b2caf71673f398ccc7199b7b25aac4c37b42c15a79878e54ad";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Franka_spine_msgsSrvSwitchOff_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvSwitchOff_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvSwitchOff_Response>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvSwitchOff_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvSwitchOff_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvSwitchOff_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvSwitchOff_Response():changetype<Ros2Franka_spine_msgsSrvSwitchOff_Response>(out.value);}
}
/** Complete original franka_spine_msgs/srv/SwitchOn_Request; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvSwitchOn_Request {
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
static readControl(r:Reader):Ros2Franka_spine_msgsSrvSwitchOn_Request{const value=new Ros2Franka_spine_msgsSrvSwitchOn_Request();
return value;}
}
export class Ros2Franka_spine_msgsSrvSwitchOn_RequestCodec implements VendorCodec<Ros2Franka_spine_msgsSrvSwitchOn_Request>{
static readonly codecIdentity:string="UWVMROS1_840d12dbe4d4a8558e72c2facbccfc547de35536f749573b6de60ce9a9238a56";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Franka_spine_msgsSrvSwitchOn_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvSwitchOn_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvSwitchOn_Request>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvSwitchOn_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvSwitchOn_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvSwitchOn_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvSwitchOn_Request():changetype<Ros2Franka_spine_msgsSrvSwitchOn_Request>(out.value);}
}
/** Complete original franka_spine_msgs/srv/SwitchOn_Response; spellings are published in generation proof. */
export class Ros2Franka_spine_msgsSrvSwitchOn_Response {
static readonly maxWireBytes:i32=521;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=521;
/** Exact native field state, native scalar string. */
state:string="";
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.state)||this.state.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.state);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
{
if(1>521-n)return -90;n+=1;
if(n>521)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>521-n)return -90;n+=4+size;
if(n>521)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.state);
w.u8(this.success?1:0);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Franka_spine_msgsSrvSwitchOn_Response{const value=new Ros2Franka_spine_msgsSrvSwitchOn_Response();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.state=decoded;
}
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
export class Ros2Franka_spine_msgsSrvSwitchOn_ResponseCodec implements VendorCodec<Ros2Franka_spine_msgsSrvSwitchOn_Response>{
static readonly codecIdentity:string="UWVMROS1_8c58d63385a5fc6f160585504b3c9c5ca5375c6187f7a494c3db696c5cc7a37d";
static readonly maxWireBytes:i32=521;
encodeChecked(value:Ros2Franka_spine_msgsSrvSwitchOn_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Franka_spine_msgsSrvSwitchOn_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Franka_spine_msgsSrvSwitchOn_Response>{const out=new VendorDecoded<Ros2Franka_spine_msgsSrvSwitchOn_Response>();if(bytes.length>521){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Franka_spine_msgsSrvSwitchOn_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Franka_spine_msgsSrvSwitchOn_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Franka_spine_msgsSrvSwitchOn_Response():changetype<Ros2Franka_spine_msgsSrvSwitchOn_Response>(out.value);}
}
