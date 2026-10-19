/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original franka_example_controllers/JointTorqueComparison; spellings are published in generation proof. */
export class Ros1Franka_example_controllersJointTorqueComparison {
static readonly maxWireBytes:i32=176;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=176;
/** Exact native field tau_error, native scalar float64. */
tau_error:Array<f64>=new Array<f64>();
/** Exact native field tau_commanded, native scalar float64. */
tau_commanded:Array<f64>=new Array<f64>();
/** Exact native field tau_measured, native scalar float64. */
tau_measured:Array<f64>=new Array<f64>();
/** Exact native field root_mean_square_error, native scalar float64. */
root_mean_square_error:f64=0;
constructor(){
for(let i=0;i<7;++i)this.tau_error.push(0);
for(let i=0;i<7;++i)this.tau_commanded.push(0);
for(let i=0;i<7;++i)this.tau_measured.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.tau_error.length!=7)return -22;
for(let i=0;i<this.tau_error.length;++i){
if(!isFinite(this.tau_error[i]))return -22;
if(8>176-n)return -90;n+=8;
if(n>176)return -90;
}
}
{
if(this.tau_commanded.length!=7)return -22;
for(let i=0;i<this.tau_commanded.length;++i){
if(!isFinite(this.tau_commanded[i]))return -22;
if(8>176-n)return -90;n+=8;
if(n>176)return -90;
}
}
{
if(this.tau_measured.length!=7)return -22;
for(let i=0;i<this.tau_measured.length;++i){
if(!isFinite(this.tau_measured[i]))return -22;
if(8>176-n)return -90;n+=8;
if(n>176)return -90;
}
}
{
if(!isFinite(this.root_mean_square_error))return -22;
if(8>176-n)return -90;n+=8;
if(n>176)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.tau_error.length;++i){
w.integer(<u64>reinterpret<u64>(this.tau_error[i]),8);
}
for(let i=0;i<this.tau_commanded.length;++i){
w.integer(<u64>reinterpret<u64>(this.tau_commanded[i]),8);
}
for(let i=0;i<this.tau_measured.length;++i){
w.integer(<u64>reinterpret<u64>(this.tau_measured[i]),8);
}
w.integer(<u64>reinterpret<u64>(this.root_mean_square_error),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Franka_example_controllersJointTorqueComparison{const value=new Ros1Franka_example_controllersJointTorqueComparison();
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.tau_error=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.tau_error.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.tau_commanded=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.tau_commanded.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.tau_measured=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.tau_measured.push(decoded);
}
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.root_mean_square_error=decoded;
}
return value;}
}
export class Ros1Franka_example_controllersJointTorqueComparisonCodec implements VendorCodec<Ros1Franka_example_controllersJointTorqueComparison>{
static readonly codecIdentity:string="UWVMROS1_dde6eaf404002642e64af59eb5367948b00bc07c0b2fd8a57e359e9df3084254";
static readonly maxWireBytes:i32=176;
encodeChecked(value:Ros1Franka_example_controllersJointTorqueComparison):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_example_controllersJointTorqueComparison):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_example_controllersJointTorqueComparison>{const out=new VendorDecoded<Ros1Franka_example_controllersJointTorqueComparison>();if(bytes.length>176){out.status=90;return out;}if(bytes.length<176){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_example_controllersJointTorqueComparison.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_example_controllersJointTorqueComparison{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_example_controllersJointTorqueComparison():changetype<Ros1Franka_example_controllersJointTorqueComparison>(out.value);}
}
/** Complete original franka_gripper/GraspEpsilon; spellings are published in generation proof. */
export class Ros1Franka_gripperGraspEpsilon {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field inner, native scalar float64. */
inner:f64=0;
/** Exact native field outer, native scalar float64. */
outer:f64=0;
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
static readControl(r:Reader):Ros1Franka_gripperGraspEpsilon{const value=new Ros1Franka_gripperGraspEpsilon();
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
export class Ros1Franka_gripperGraspEpsilonCodec implements VendorCodec<Ros1Franka_gripperGraspEpsilon>{
static readonly codecIdentity:string="UWVMROS1_538ac801e9874841f232909d2d9a16893a8d29c8380cd623c00c408690f978f9";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Franka_gripperGraspEpsilon):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperGraspEpsilon):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperGraspEpsilon>{const out=new VendorDecoded<Ros1Franka_gripperGraspEpsilon>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperGraspEpsilon.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperGraspEpsilon{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperGraspEpsilon():changetype<Ros1Franka_gripperGraspEpsilon>(out.value);}
}
/** Complete original franka_gripper/GraspGoal; spellings are published in generation proof. */
export class Ros1Franka_gripperGraspGoal {
static readonly maxWireBytes:i32=40;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=40;
/** Exact native field width, native scalar float64. */
width:f64=0;
/** Exact native field epsilon, native scalar franka_gripper/GraspEpsilon. */
epsilon:Ros1Franka_gripperGraspEpsilon=new Ros1Franka_gripperGraspEpsilon();
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
static readControl(r:Reader):Ros1Franka_gripperGraspGoal{const value=new Ros1Franka_gripperGraspGoal();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.width=decoded;
}
{
const decoded=Ros1Franka_gripperGraspEpsilon.readControl(r);
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
export class Ros1Franka_gripperGraspGoalCodec implements VendorCodec<Ros1Franka_gripperGraspGoal>{
static readonly codecIdentity:string="UWVMROS1_d3dd0d3f47e49e06a95d907a6219335b35234749518ab645e102257595ed7bcf";
static readonly maxWireBytes:i32=40;
encodeChecked(value:Ros1Franka_gripperGraspGoal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperGraspGoal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperGraspGoal>{const out=new VendorDecoded<Ros1Franka_gripperGraspGoal>();if(bytes.length>40){out.status=90;return out;}if(bytes.length<40){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperGraspGoal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperGraspGoal{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperGraspGoal():changetype<Ros1Franka_gripperGraspGoal>(out.value);}
}
/** Complete original franka_gripper/GraspResult; spellings are published in generation proof. */
export class Ros1Franka_gripperGraspResult {
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
static readControl(r:Reader):Ros1Franka_gripperGraspResult{const value=new Ros1Franka_gripperGraspResult();
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
export class Ros1Franka_gripperGraspResultCodec implements VendorCodec<Ros1Franka_gripperGraspResult>{
static readonly codecIdentity:string="UWVMROS1_9ce1e4f95f16fd6c1603f0b30a9fcc33e4f8436cdb96710ea91eb20b38a1e02e";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Franka_gripperGraspResult):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperGraspResult):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperGraspResult>{const out=new VendorDecoded<Ros1Franka_gripperGraspResult>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperGraspResult.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperGraspResult{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperGraspResult():changetype<Ros1Franka_gripperGraspResult>(out.value);}
}
/** Complete original franka_gripper/GraspFeedback; spellings are published in generation proof. */
export class Ros1Franka_gripperGraspFeedback {
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
static readControl(r:Reader):Ros1Franka_gripperGraspFeedback{const value=new Ros1Franka_gripperGraspFeedback();
return value;}
}
export class Ros1Franka_gripperGraspFeedbackCodec implements VendorCodec<Ros1Franka_gripperGraspFeedback>{
static readonly codecIdentity:string="UWVMROS1_24ca23625df0bd9fb984e65c8ecd7a3af162a3db00c15951225df5e18d3a4d0d";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Franka_gripperGraspFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperGraspFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperGraspFeedback>{const out=new VendorDecoded<Ros1Franka_gripperGraspFeedback>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperGraspFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperGraspFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperGraspFeedback():changetype<Ros1Franka_gripperGraspFeedback>(out.value);}
}
/** Complete original franka_gripper/HomingGoal; spellings are published in generation proof. */
export class Ros1Franka_gripperHomingGoal {
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
static readControl(r:Reader):Ros1Franka_gripperHomingGoal{const value=new Ros1Franka_gripperHomingGoal();
return value;}
}
export class Ros1Franka_gripperHomingGoalCodec implements VendorCodec<Ros1Franka_gripperHomingGoal>{
static readonly codecIdentity:string="UWVMROS1_4b4f861fa9ff44afda4217cf613720e90353b0d43c1fc3425d5937157353910f";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Franka_gripperHomingGoal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperHomingGoal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperHomingGoal>{const out=new VendorDecoded<Ros1Franka_gripperHomingGoal>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperHomingGoal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperHomingGoal{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperHomingGoal():changetype<Ros1Franka_gripperHomingGoal>(out.value);}
}
/** Complete original franka_gripper/HomingResult; spellings are published in generation proof. */
export class Ros1Franka_gripperHomingResult {
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
static readControl(r:Reader):Ros1Franka_gripperHomingResult{const value=new Ros1Franka_gripperHomingResult();
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
export class Ros1Franka_gripperHomingResultCodec implements VendorCodec<Ros1Franka_gripperHomingResult>{
static readonly codecIdentity:string="UWVMROS1_1ddf202b4e7dbf100d304b4eb32aa0ce8dca6ffadcf720820f2d7932af056477";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Franka_gripperHomingResult):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperHomingResult):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperHomingResult>{const out=new VendorDecoded<Ros1Franka_gripperHomingResult>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperHomingResult.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperHomingResult{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperHomingResult():changetype<Ros1Franka_gripperHomingResult>(out.value);}
}
/** Complete original franka_gripper/HomingFeedback; spellings are published in generation proof. */
export class Ros1Franka_gripperHomingFeedback {
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
static readControl(r:Reader):Ros1Franka_gripperHomingFeedback{const value=new Ros1Franka_gripperHomingFeedback();
return value;}
}
export class Ros1Franka_gripperHomingFeedbackCodec implements VendorCodec<Ros1Franka_gripperHomingFeedback>{
static readonly codecIdentity:string="UWVMROS1_4a1118d8ab544b2bc99ff878356aae49b4d3ecb4cb5ba7e2f93f7c0c7a29512e";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Franka_gripperHomingFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperHomingFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperHomingFeedback>{const out=new VendorDecoded<Ros1Franka_gripperHomingFeedback>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperHomingFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperHomingFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperHomingFeedback():changetype<Ros1Franka_gripperHomingFeedback>(out.value);}
}
/** Complete original franka_gripper/MoveGoal; spellings are published in generation proof. */
export class Ros1Franka_gripperMoveGoal {
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
static readControl(r:Reader):Ros1Franka_gripperMoveGoal{const value=new Ros1Franka_gripperMoveGoal();
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
export class Ros1Franka_gripperMoveGoalCodec implements VendorCodec<Ros1Franka_gripperMoveGoal>{
static readonly codecIdentity:string="UWVMROS1_c83296f2540854e78cc159691c229f5d50267b3fdb31789e325a55fcd7d6830a";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Franka_gripperMoveGoal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperMoveGoal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperMoveGoal>{const out=new VendorDecoded<Ros1Franka_gripperMoveGoal>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperMoveGoal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperMoveGoal{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperMoveGoal():changetype<Ros1Franka_gripperMoveGoal>(out.value);}
}
/** Complete original franka_gripper/MoveResult; spellings are published in generation proof. */
export class Ros1Franka_gripperMoveResult {
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
static readControl(r:Reader):Ros1Franka_gripperMoveResult{const value=new Ros1Franka_gripperMoveResult();
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
export class Ros1Franka_gripperMoveResultCodec implements VendorCodec<Ros1Franka_gripperMoveResult>{
static readonly codecIdentity:string="UWVMROS1_527adf0720dabcc91866a0858cddfbfae1ed4203d4eb34a3965ebe085d0ac3b3";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Franka_gripperMoveResult):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperMoveResult):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperMoveResult>{const out=new VendorDecoded<Ros1Franka_gripperMoveResult>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperMoveResult.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperMoveResult{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperMoveResult():changetype<Ros1Franka_gripperMoveResult>(out.value);}
}
/** Complete original franka_gripper/MoveFeedback; spellings are published in generation proof. */
export class Ros1Franka_gripperMoveFeedback {
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
static readControl(r:Reader):Ros1Franka_gripperMoveFeedback{const value=new Ros1Franka_gripperMoveFeedback();
return value;}
}
export class Ros1Franka_gripperMoveFeedbackCodec implements VendorCodec<Ros1Franka_gripperMoveFeedback>{
static readonly codecIdentity:string="UWVMROS1_8e02ca1d66ae38a5d71ca5f708aa2c57bb3df573d37b692e24986dba384c5f80";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Franka_gripperMoveFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperMoveFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperMoveFeedback>{const out=new VendorDecoded<Ros1Franka_gripperMoveFeedback>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperMoveFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperMoveFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperMoveFeedback():changetype<Ros1Franka_gripperMoveFeedback>(out.value);}
}
/** Complete original franka_gripper/StopGoal; spellings are published in generation proof. */
export class Ros1Franka_gripperStopGoal {
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
static readControl(r:Reader):Ros1Franka_gripperStopGoal{const value=new Ros1Franka_gripperStopGoal();
return value;}
}
export class Ros1Franka_gripperStopGoalCodec implements VendorCodec<Ros1Franka_gripperStopGoal>{
static readonly codecIdentity:string="UWVMROS1_ecb09ccc03cb2140a457a11c174a614392ce1f9263e46c5b82427a99f7ff0245";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Franka_gripperStopGoal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperStopGoal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperStopGoal>{const out=new VendorDecoded<Ros1Franka_gripperStopGoal>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperStopGoal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperStopGoal{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperStopGoal():changetype<Ros1Franka_gripperStopGoal>(out.value);}
}
/** Complete original franka_gripper/StopResult; spellings are published in generation proof. */
export class Ros1Franka_gripperStopResult {
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
static readControl(r:Reader):Ros1Franka_gripperStopResult{const value=new Ros1Franka_gripperStopResult();
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
export class Ros1Franka_gripperStopResultCodec implements VendorCodec<Ros1Franka_gripperStopResult>{
static readonly codecIdentity:string="UWVMROS1_343e16d9b839b61ed8f8c1ce93005cb3a21511a220095200eb075dfb62a40c61";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Franka_gripperStopResult):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperStopResult):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperStopResult>{const out=new VendorDecoded<Ros1Franka_gripperStopResult>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperStopResult.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperStopResult{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperStopResult():changetype<Ros1Franka_gripperStopResult>(out.value);}
}
/** Complete original franka_gripper/StopFeedback; spellings are published in generation proof. */
export class Ros1Franka_gripperStopFeedback {
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
static readControl(r:Reader):Ros1Franka_gripperStopFeedback{const value=new Ros1Franka_gripperStopFeedback();
return value;}
}
export class Ros1Franka_gripperStopFeedbackCodec implements VendorCodec<Ros1Franka_gripperStopFeedback>{
static readonly codecIdentity:string="UWVMROS1_62b637081e95f59df491f71b0ca84394a7e1f36137ca389a8f6d0d3a0f19038d";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Franka_gripperStopFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_gripperStopFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_gripperStopFeedback>{const out=new VendorDecoded<Ros1Franka_gripperStopFeedback>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_gripperStopFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_gripperStopFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_gripperStopFeedback():changetype<Ros1Franka_gripperStopFeedback>(out.value);}
}
/** Complete original franka_msgs/ErrorRecoveryGoal; spellings are published in generation proof. */
export class Ros1Franka_msgsErrorRecoveryGoal {
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
static readControl(r:Reader):Ros1Franka_msgsErrorRecoveryGoal{const value=new Ros1Franka_msgsErrorRecoveryGoal();
return value;}
}
export class Ros1Franka_msgsErrorRecoveryGoalCodec implements VendorCodec<Ros1Franka_msgsErrorRecoveryGoal>{
static readonly codecIdentity:string="UWVMROS1_b4f11555bc4919bf59aa548bd21046bdc7b1e2ed5cce6dc46298eb7486ef7b64";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Franka_msgsErrorRecoveryGoal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsErrorRecoveryGoal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsErrorRecoveryGoal>{const out=new VendorDecoded<Ros1Franka_msgsErrorRecoveryGoal>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsErrorRecoveryGoal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsErrorRecoveryGoal{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsErrorRecoveryGoal():changetype<Ros1Franka_msgsErrorRecoveryGoal>(out.value);}
}
/** Complete original franka_msgs/ErrorRecoveryResult; spellings are published in generation proof. */
export class Ros1Franka_msgsErrorRecoveryResult {
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
static readControl(r:Reader):Ros1Franka_msgsErrorRecoveryResult{const value=new Ros1Franka_msgsErrorRecoveryResult();
return value;}
}
export class Ros1Franka_msgsErrorRecoveryResultCodec implements VendorCodec<Ros1Franka_msgsErrorRecoveryResult>{
static readonly codecIdentity:string="UWVMROS1_6f06297f59f06dc46e3f983b72101ae3c25cd054bca44cea1f6dd11a9aaa7794";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Franka_msgsErrorRecoveryResult):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsErrorRecoveryResult):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsErrorRecoveryResult>{const out=new VendorDecoded<Ros1Franka_msgsErrorRecoveryResult>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsErrorRecoveryResult.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsErrorRecoveryResult{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsErrorRecoveryResult():changetype<Ros1Franka_msgsErrorRecoveryResult>(out.value);}
}
/** Complete original franka_msgs/ErrorRecoveryFeedback; spellings are published in generation proof. */
export class Ros1Franka_msgsErrorRecoveryFeedback {
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
static readControl(r:Reader):Ros1Franka_msgsErrorRecoveryFeedback{const value=new Ros1Franka_msgsErrorRecoveryFeedback();
return value;}
}
export class Ros1Franka_msgsErrorRecoveryFeedbackCodec implements VendorCodec<Ros1Franka_msgsErrorRecoveryFeedback>{
static readonly codecIdentity:string="UWVMROS1_3faee6d8afdc0524598cec0fe30fd51bd723e8e44849f035368723e59be87a21";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Franka_msgsErrorRecoveryFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsErrorRecoveryFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsErrorRecoveryFeedback>{const out=new VendorDecoded<Ros1Franka_msgsErrorRecoveryFeedback>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsErrorRecoveryFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsErrorRecoveryFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsErrorRecoveryFeedback():changetype<Ros1Franka_msgsErrorRecoveryFeedback>(out.value);}
}
/** Complete original franka_msgs/Errors; spellings are published in generation proof. */
export class Ros1Franka_msgsErrors {
static readonly maxWireBytes:i32=41;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=41;
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
/** Exact native field joint_move_in_wrong_direction, native scalar bool. */
joint_move_in_wrong_direction:bool=false;
/** Exact native field cartesian_spline_motion_generator_violation, native scalar bool. */
cartesian_spline_motion_generator_violation:bool=false;
/** Exact native field joint_via_motion_generator_planning_joint_limit_violation, native scalar bool. */
joint_via_motion_generator_planning_joint_limit_violation:bool=false;
/** Exact native field base_acceleration_initialization_timeout, native scalar bool. */
base_acceleration_initialization_timeout:bool=false;
/** Exact native field base_acceleration_invalid_reading, native scalar bool. */
base_acceleration_invalid_reading:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
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
w.u8(this.joint_move_in_wrong_direction?1:0);
w.u8(this.cartesian_spline_motion_generator_violation?1:0);
w.u8(this.joint_via_motion_generator_planning_joint_limit_violation?1:0);
w.u8(this.base_acceleration_initialization_timeout?1:0);
w.u8(this.base_acceleration_invalid_reading?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Franka_msgsErrors{const value=new Ros1Franka_msgsErrors();
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
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.joint_move_in_wrong_direction=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.cartesian_spline_motion_generator_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.joint_via_motion_generator_planning_joint_limit_violation=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.base_acceleration_initialization_timeout=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.base_acceleration_invalid_reading=decoded;
}
return value;}
}
export class Ros1Franka_msgsErrorsCodec implements VendorCodec<Ros1Franka_msgsErrors>{
static readonly codecIdentity:string="UWVMROS1_80e3d1157d53bf80d4db3621f9f22b35a1ef665382974599ff016709c6a629ca";
static readonly maxWireBytes:i32=41;
encodeChecked(value:Ros1Franka_msgsErrors):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsErrors):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsErrors>{const out=new VendorDecoded<Ros1Franka_msgsErrors>();if(bytes.length>41){out.status=90;return out;}if(bytes.length<41){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsErrors.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsErrors{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsErrors():changetype<Ros1Franka_msgsErrors>(out.value);}
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
/** Complete original franka_msgs/FrankaState; spellings are published in generation proof. */
export class Ros1Franka_msgsFrankaState {
static readonly ROBOT_MODE_OTHER:u8=0;
static readonly ROBOT_MODE_IDLE:u8=1;
static readonly ROBOT_MODE_MOVE:u8=2;
static readonly ROBOT_MODE_GUIDING:u8=3;
static readonly ROBOT_MODE_REFLEX:u8=4;
static readonly ROBOT_MODE_USER_STOPPED:u8=5;
static readonly ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY:u8=6;
static readonly maxWireBytes:i32=2619;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2619;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field cartesian_collision, native scalar float64. */
cartesian_collision:Array<f64>=new Array<f64>();
/** Exact native field cartesian_contact, native scalar float64. */
cartesian_contact:Array<f64>=new Array<f64>();
/** Exact native field q, native scalar float64. */
q:Array<f64>=new Array<f64>();
/** Exact native field q_d, native scalar float64. */
q_d:Array<f64>=new Array<f64>();
/** Exact native field dq, native scalar float64. */
dq:Array<f64>=new Array<f64>();
/** Exact native field dq_d, native scalar float64. */
dq_d:Array<f64>=new Array<f64>();
/** Exact native field ddq_d, native scalar float64. */
ddq_d:Array<f64>=new Array<f64>();
/** Exact native field theta, native scalar float64. */
theta:Array<f64>=new Array<f64>();
/** Exact native field dtheta, native scalar float64. */
dtheta:Array<f64>=new Array<f64>();
/** Exact native field tau_J, native scalar float64. */
tau_J:Array<f64>=new Array<f64>();
/** Exact native field dtau_J, native scalar float64. */
dtau_J:Array<f64>=new Array<f64>();
/** Exact native field tau_J_d, native scalar float64. */
tau_J_d:Array<f64>=new Array<f64>();
/** Exact native field K_F_ext_hat_K, native scalar float64. */
K_F_ext_hat_K:Array<f64>=new Array<f64>();
/** Exact native field elbow, native scalar float64. */
elbow:Array<f64>=new Array<f64>();
/** Exact native field elbow_d, native scalar float64. */
elbow_d:Array<f64>=new Array<f64>();
/** Exact native field elbow_c, native scalar float64. */
elbow_c:Array<f64>=new Array<f64>();
/** Exact native field delbow_c, native scalar float64. */
delbow_c:Array<f64>=new Array<f64>();
/** Exact native field ddelbow_c, native scalar float64. */
ddelbow_c:Array<f64>=new Array<f64>();
/** Exact native field joint_collision, native scalar float64. */
joint_collision:Array<f64>=new Array<f64>();
/** Exact native field joint_contact, native scalar float64. */
joint_contact:Array<f64>=new Array<f64>();
/** Exact native field O_F_ext_hat_K, native scalar float64. */
O_F_ext_hat_K:Array<f64>=new Array<f64>();
/** Exact native field O_dP_EE_d, native scalar float64. */
O_dP_EE_d:Array<f64>=new Array<f64>();
/** Exact native field O_ddP_O, native scalar float64. */
O_ddP_O:Array<f64>=new Array<f64>();
/** Exact native field O_dP_EE_c, native scalar float64. */
O_dP_EE_c:Array<f64>=new Array<f64>();
/** Exact native field O_ddP_EE_c, native scalar float64. */
O_ddP_EE_c:Array<f64>=new Array<f64>();
/** Exact native field tau_ext_hat_filtered, native scalar float64. */
tau_ext_hat_filtered:Array<f64>=new Array<f64>();
/** Exact native field m_ee, native scalar float64. */
m_ee:f64=0;
/** Exact native field F_x_Cee, native scalar float64. */
F_x_Cee:Array<f64>=new Array<f64>();
/** Exact native field I_ee, native scalar float64. */
I_ee:Array<f64>=new Array<f64>();
/** Exact native field m_load, native scalar float64. */
m_load:f64=0;
/** Exact native field F_x_Cload, native scalar float64. */
F_x_Cload:Array<f64>=new Array<f64>();
/** Exact native field I_load, native scalar float64. */
I_load:Array<f64>=new Array<f64>();
/** Exact native field m_total, native scalar float64. */
m_total:f64=0;
/** Exact native field F_x_Ctotal, native scalar float64. */
F_x_Ctotal:Array<f64>=new Array<f64>();
/** Exact native field I_total, native scalar float64. */
I_total:Array<f64>=new Array<f64>();
/** Exact native field O_T_EE, native scalar float64. */
O_T_EE:Array<f64>=new Array<f64>();
/** Exact native field O_T_EE_d, native scalar float64. */
O_T_EE_d:Array<f64>=new Array<f64>();
/** Exact native field O_T_EE_c, native scalar float64. */
O_T_EE_c:Array<f64>=new Array<f64>();
/** Exact native field F_T_EE, native scalar float64. */
F_T_EE:Array<f64>=new Array<f64>();
/** Exact native field F_T_NE, native scalar float64. */
F_T_NE:Array<f64>=new Array<f64>();
/** Exact native field NE_T_EE, native scalar float64. */
NE_T_EE:Array<f64>=new Array<f64>();
/** Exact native field EE_T_K, native scalar float64. */
EE_T_K:Array<f64>=new Array<f64>();
/** Exact native field time, native scalar float64. */
time:f64=0;
/** Exact native field control_command_success_rate, native scalar float64. */
control_command_success_rate:f64=0;
/** Exact native field robot_mode, native scalar uint8. */
robot_mode:u8=0;
/** Exact native field current_errors, native scalar franka_msgs/Errors. */
current_errors:Ros1Franka_msgsErrors=new Ros1Franka_msgsErrors();
/** Exact native field last_motion_errors, native scalar franka_msgs/Errors. */
last_motion_errors:Ros1Franka_msgsErrors=new Ros1Franka_msgsErrors();
constructor(){
for(let i=0;i<6;++i)this.cartesian_collision.push(0);
for(let i=0;i<6;++i)this.cartesian_contact.push(0);
for(let i=0;i<7;++i)this.q.push(0);
for(let i=0;i<7;++i)this.q_d.push(0);
for(let i=0;i<7;++i)this.dq.push(0);
for(let i=0;i<7;++i)this.dq_d.push(0);
for(let i=0;i<7;++i)this.ddq_d.push(0);
for(let i=0;i<7;++i)this.theta.push(0);
for(let i=0;i<7;++i)this.dtheta.push(0);
for(let i=0;i<7;++i)this.tau_J.push(0);
for(let i=0;i<7;++i)this.dtau_J.push(0);
for(let i=0;i<7;++i)this.tau_J_d.push(0);
for(let i=0;i<6;++i)this.K_F_ext_hat_K.push(0);
for(let i=0;i<2;++i)this.elbow.push(0);
for(let i=0;i<2;++i)this.elbow_d.push(0);
for(let i=0;i<2;++i)this.elbow_c.push(0);
for(let i=0;i<2;++i)this.delbow_c.push(0);
for(let i=0;i<2;++i)this.ddelbow_c.push(0);
for(let i=0;i<7;++i)this.joint_collision.push(0);
for(let i=0;i<7;++i)this.joint_contact.push(0);
for(let i=0;i<6;++i)this.O_F_ext_hat_K.push(0);
for(let i=0;i<6;++i)this.O_dP_EE_d.push(0);
for(let i=0;i<3;++i)this.O_ddP_O.push(0);
for(let i=0;i<6;++i)this.O_dP_EE_c.push(0);
for(let i=0;i<6;++i)this.O_ddP_EE_c.push(0);
for(let i=0;i<7;++i)this.tau_ext_hat_filtered.push(0);
for(let i=0;i<3;++i)this.F_x_Cee.push(0);
for(let i=0;i<9;++i)this.I_ee.push(0);
for(let i=0;i<3;++i)this.F_x_Cload.push(0);
for(let i=0;i<9;++i)this.I_load.push(0);
for(let i=0;i<3;++i)this.F_x_Ctotal.push(0);
for(let i=0;i<9;++i)this.I_total.push(0);
for(let i=0;i<16;++i)this.O_T_EE.push(0);
for(let i=0;i<16;++i)this.O_T_EE_d.push(0);
for(let i=0;i<16;++i)this.O_T_EE_c.push(0);
for(let i=0;i<16;++i)this.F_T_EE.push(0);
for(let i=0;i<16;++i)this.F_T_NE.push(0);
for(let i=0;i<16;++i)this.NE_T_EE.push(0);
for(let i=0;i<16;++i)this.EE_T_K.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>2619-n)return -90;n+=child;
if(n>2619)return -90;
}
{
if(this.cartesian_collision.length!=6)return -22;
for(let i=0;i<this.cartesian_collision.length;++i){
if(!isFinite(this.cartesian_collision[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.cartesian_contact.length!=6)return -22;
for(let i=0;i<this.cartesian_contact.length;++i){
if(!isFinite(this.cartesian_contact[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.q.length!=7)return -22;
for(let i=0;i<this.q.length;++i){
if(!isFinite(this.q[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.q_d.length!=7)return -22;
for(let i=0;i<this.q_d.length;++i){
if(!isFinite(this.q_d[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.dq.length!=7)return -22;
for(let i=0;i<this.dq.length;++i){
if(!isFinite(this.dq[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.dq_d.length!=7)return -22;
for(let i=0;i<this.dq_d.length;++i){
if(!isFinite(this.dq_d[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.ddq_d.length!=7)return -22;
for(let i=0;i<this.ddq_d.length;++i){
if(!isFinite(this.ddq_d[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.theta.length!=7)return -22;
for(let i=0;i<this.theta.length;++i){
if(!isFinite(this.theta[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.dtheta.length!=7)return -22;
for(let i=0;i<this.dtheta.length;++i){
if(!isFinite(this.dtheta[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.tau_J.length!=7)return -22;
for(let i=0;i<this.tau_J.length;++i){
if(!isFinite(this.tau_J[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.dtau_J.length!=7)return -22;
for(let i=0;i<this.dtau_J.length;++i){
if(!isFinite(this.dtau_J[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.tau_J_d.length!=7)return -22;
for(let i=0;i<this.tau_J_d.length;++i){
if(!isFinite(this.tau_J_d[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.K_F_ext_hat_K.length!=6)return -22;
for(let i=0;i<this.K_F_ext_hat_K.length;++i){
if(!isFinite(this.K_F_ext_hat_K[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.elbow.length!=2)return -22;
for(let i=0;i<this.elbow.length;++i){
if(!isFinite(this.elbow[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.elbow_d.length!=2)return -22;
for(let i=0;i<this.elbow_d.length;++i){
if(!isFinite(this.elbow_d[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.elbow_c.length!=2)return -22;
for(let i=0;i<this.elbow_c.length;++i){
if(!isFinite(this.elbow_c[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.delbow_c.length!=2)return -22;
for(let i=0;i<this.delbow_c.length;++i){
if(!isFinite(this.delbow_c[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.ddelbow_c.length!=2)return -22;
for(let i=0;i<this.ddelbow_c.length;++i){
if(!isFinite(this.ddelbow_c[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.joint_collision.length!=7)return -22;
for(let i=0;i<this.joint_collision.length;++i){
if(!isFinite(this.joint_collision[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.joint_contact.length!=7)return -22;
for(let i=0;i<this.joint_contact.length;++i){
if(!isFinite(this.joint_contact[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.O_F_ext_hat_K.length!=6)return -22;
for(let i=0;i<this.O_F_ext_hat_K.length;++i){
if(!isFinite(this.O_F_ext_hat_K[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.O_dP_EE_d.length!=6)return -22;
for(let i=0;i<this.O_dP_EE_d.length;++i){
if(!isFinite(this.O_dP_EE_d[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.O_ddP_O.length!=3)return -22;
for(let i=0;i<this.O_ddP_O.length;++i){
if(!isFinite(this.O_ddP_O[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.O_dP_EE_c.length!=6)return -22;
for(let i=0;i<this.O_dP_EE_c.length;++i){
if(!isFinite(this.O_dP_EE_c[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.O_ddP_EE_c.length!=6)return -22;
for(let i=0;i<this.O_ddP_EE_c.length;++i){
if(!isFinite(this.O_ddP_EE_c[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.tau_ext_hat_filtered.length!=7)return -22;
for(let i=0;i<this.tau_ext_hat_filtered.length;++i){
if(!isFinite(this.tau_ext_hat_filtered[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(!isFinite(this.m_ee))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
{
if(this.F_x_Cee.length!=3)return -22;
for(let i=0;i<this.F_x_Cee.length;++i){
if(!isFinite(this.F_x_Cee[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.I_ee.length!=9)return -22;
for(let i=0;i<this.I_ee.length;++i){
if(!isFinite(this.I_ee[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(!isFinite(this.m_load))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
{
if(this.F_x_Cload.length!=3)return -22;
for(let i=0;i<this.F_x_Cload.length;++i){
if(!isFinite(this.F_x_Cload[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.I_load.length!=9)return -22;
for(let i=0;i<this.I_load.length;++i){
if(!isFinite(this.I_load[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(!isFinite(this.m_total))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
{
if(this.F_x_Ctotal.length!=3)return -22;
for(let i=0;i<this.F_x_Ctotal.length;++i){
if(!isFinite(this.F_x_Ctotal[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.I_total.length!=9)return -22;
for(let i=0;i<this.I_total.length;++i){
if(!isFinite(this.I_total[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.O_T_EE.length!=16)return -22;
for(let i=0;i<this.O_T_EE.length;++i){
if(!isFinite(this.O_T_EE[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.O_T_EE_d.length!=16)return -22;
for(let i=0;i<this.O_T_EE_d.length;++i){
if(!isFinite(this.O_T_EE_d[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.O_T_EE_c.length!=16)return -22;
for(let i=0;i<this.O_T_EE_c.length;++i){
if(!isFinite(this.O_T_EE_c[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.F_T_EE.length!=16)return -22;
for(let i=0;i<this.F_T_EE.length;++i){
if(!isFinite(this.F_T_EE[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.F_T_NE.length!=16)return -22;
for(let i=0;i<this.F_T_NE.length;++i){
if(!isFinite(this.F_T_NE[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.NE_T_EE.length!=16)return -22;
for(let i=0;i<this.NE_T_EE.length;++i){
if(!isFinite(this.NE_T_EE[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(this.EE_T_K.length!=16)return -22;
for(let i=0;i<this.EE_T_K.length;++i){
if(!isFinite(this.EE_T_K[i]))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
}
{
if(!isFinite(this.time))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
{
if(!isFinite(this.control_command_success_rate))return -22;
if(8>2619-n)return -90;n+=8;
if(n>2619)return -90;
}
{
if(1>2619-n)return -90;n+=1;
if(n>2619)return -90;
}
{
const child=this.current_errors.wireSize();if(child<0)return child;if(child>2619-n)return -90;n+=child;
if(n>2619)return -90;
}
{
const child=this.last_motion_errors.wireSize();if(child<0)return child;if(child>2619-n)return -90;n+=child;
if(n>2619)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
for(let i=0;i<this.cartesian_collision.length;++i){
w.integer(<u64>reinterpret<u64>(this.cartesian_collision[i]),8);
}
for(let i=0;i<this.cartesian_contact.length;++i){
w.integer(<u64>reinterpret<u64>(this.cartesian_contact[i]),8);
}
for(let i=0;i<this.q.length;++i){
w.integer(<u64>reinterpret<u64>(this.q[i]),8);
}
for(let i=0;i<this.q_d.length;++i){
w.integer(<u64>reinterpret<u64>(this.q_d[i]),8);
}
for(let i=0;i<this.dq.length;++i){
w.integer(<u64>reinterpret<u64>(this.dq[i]),8);
}
for(let i=0;i<this.dq_d.length;++i){
w.integer(<u64>reinterpret<u64>(this.dq_d[i]),8);
}
for(let i=0;i<this.ddq_d.length;++i){
w.integer(<u64>reinterpret<u64>(this.ddq_d[i]),8);
}
for(let i=0;i<this.theta.length;++i){
w.integer(<u64>reinterpret<u64>(this.theta[i]),8);
}
for(let i=0;i<this.dtheta.length;++i){
w.integer(<u64>reinterpret<u64>(this.dtheta[i]),8);
}
for(let i=0;i<this.tau_J.length;++i){
w.integer(<u64>reinterpret<u64>(this.tau_J[i]),8);
}
for(let i=0;i<this.dtau_J.length;++i){
w.integer(<u64>reinterpret<u64>(this.dtau_J[i]),8);
}
for(let i=0;i<this.tau_J_d.length;++i){
w.integer(<u64>reinterpret<u64>(this.tau_J_d[i]),8);
}
for(let i=0;i<this.K_F_ext_hat_K.length;++i){
w.integer(<u64>reinterpret<u64>(this.K_F_ext_hat_K[i]),8);
}
for(let i=0;i<this.elbow.length;++i){
w.integer(<u64>reinterpret<u64>(this.elbow[i]),8);
}
for(let i=0;i<this.elbow_d.length;++i){
w.integer(<u64>reinterpret<u64>(this.elbow_d[i]),8);
}
for(let i=0;i<this.elbow_c.length;++i){
w.integer(<u64>reinterpret<u64>(this.elbow_c[i]),8);
}
for(let i=0;i<this.delbow_c.length;++i){
w.integer(<u64>reinterpret<u64>(this.delbow_c[i]),8);
}
for(let i=0;i<this.ddelbow_c.length;++i){
w.integer(<u64>reinterpret<u64>(this.ddelbow_c[i]),8);
}
for(let i=0;i<this.joint_collision.length;++i){
w.integer(<u64>reinterpret<u64>(this.joint_collision[i]),8);
}
for(let i=0;i<this.joint_contact.length;++i){
w.integer(<u64>reinterpret<u64>(this.joint_contact[i]),8);
}
for(let i=0;i<this.O_F_ext_hat_K.length;++i){
w.integer(<u64>reinterpret<u64>(this.O_F_ext_hat_K[i]),8);
}
for(let i=0;i<this.O_dP_EE_d.length;++i){
w.integer(<u64>reinterpret<u64>(this.O_dP_EE_d[i]),8);
}
for(let i=0;i<this.O_ddP_O.length;++i){
w.integer(<u64>reinterpret<u64>(this.O_ddP_O[i]),8);
}
for(let i=0;i<this.O_dP_EE_c.length;++i){
w.integer(<u64>reinterpret<u64>(this.O_dP_EE_c[i]),8);
}
for(let i=0;i<this.O_ddP_EE_c.length;++i){
w.integer(<u64>reinterpret<u64>(this.O_ddP_EE_c[i]),8);
}
for(let i=0;i<this.tau_ext_hat_filtered.length;++i){
w.integer(<u64>reinterpret<u64>(this.tau_ext_hat_filtered[i]),8);
}
w.integer(<u64>reinterpret<u64>(this.m_ee),8);
for(let i=0;i<this.F_x_Cee.length;++i){
w.integer(<u64>reinterpret<u64>(this.F_x_Cee[i]),8);
}
for(let i=0;i<this.I_ee.length;++i){
w.integer(<u64>reinterpret<u64>(this.I_ee[i]),8);
}
w.integer(<u64>reinterpret<u64>(this.m_load),8);
for(let i=0;i<this.F_x_Cload.length;++i){
w.integer(<u64>reinterpret<u64>(this.F_x_Cload[i]),8);
}
for(let i=0;i<this.I_load.length;++i){
w.integer(<u64>reinterpret<u64>(this.I_load[i]),8);
}
w.integer(<u64>reinterpret<u64>(this.m_total),8);
for(let i=0;i<this.F_x_Ctotal.length;++i){
w.integer(<u64>reinterpret<u64>(this.F_x_Ctotal[i]),8);
}
for(let i=0;i<this.I_total.length;++i){
w.integer(<u64>reinterpret<u64>(this.I_total[i]),8);
}
for(let i=0;i<this.O_T_EE.length;++i){
w.integer(<u64>reinterpret<u64>(this.O_T_EE[i]),8);
}
for(let i=0;i<this.O_T_EE_d.length;++i){
w.integer(<u64>reinterpret<u64>(this.O_T_EE_d[i]),8);
}
for(let i=0;i<this.O_T_EE_c.length;++i){
w.integer(<u64>reinterpret<u64>(this.O_T_EE_c[i]),8);
}
for(let i=0;i<this.F_T_EE.length;++i){
w.integer(<u64>reinterpret<u64>(this.F_T_EE[i]),8);
}
for(let i=0;i<this.F_T_NE.length;++i){
w.integer(<u64>reinterpret<u64>(this.F_T_NE[i]),8);
}
for(let i=0;i<this.NE_T_EE.length;++i){
w.integer(<u64>reinterpret<u64>(this.NE_T_EE[i]),8);
}
for(let i=0;i<this.EE_T_K.length;++i){
w.integer(<u64>reinterpret<u64>(this.EE_T_K[i]),8);
}
w.integer(<u64>reinterpret<u64>(this.time),8);
w.integer(<u64>reinterpret<u64>(this.control_command_success_rate),8);
w.integer(<u64>this.robot_mode,1);
this.current_errors.writeControl(w);
this.last_motion_errors.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Franka_msgsFrankaState{const value=new Ros1Franka_msgsFrankaState();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.cartesian_collision=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.cartesian_collision.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.cartesian_contact=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.cartesian_contact.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.q=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.q.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.q_d=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.q_d.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.dq=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.dq.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.dq_d=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.dq_d.push(decoded);
}
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
value.theta=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.theta.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.dtheta=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.dtheta.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.tau_J=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.tau_J.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.dtau_J=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.dtau_J.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.tau_J_d=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.tau_J_d.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.K_F_ext_hat_K=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.K_F_ext_hat_K.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.elbow=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.elbow.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.elbow_d=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.elbow_d.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.elbow_c=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.elbow_c.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.delbow_c=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.delbow_c.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.ddelbow_c=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.ddelbow_c.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.joint_collision=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.joint_collision.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.joint_contact=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.joint_contact.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.O_F_ext_hat_K=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.O_F_ext_hat_K.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.O_dP_EE_d=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.O_dP_EE_d.push(decoded);
}
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.O_ddP_O=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.O_ddP_O.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.O_dP_EE_c=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.O_dP_EE_c.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.O_ddP_EE_c=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.O_ddP_EE_c.push(decoded);
}
}
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.tau_ext_hat_filtered=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.tau_ext_hat_filtered.push(decoded);
}
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.m_ee=decoded;
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.F_x_Cee=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.F_x_Cee.push(decoded);
}
}
{
const count:u32=9;if(!r.valid||count>9){r.valid=false;return value;}
value.I_ee=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.I_ee.push(decoded);
}
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.m_load=decoded;
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.F_x_Cload=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.F_x_Cload.push(decoded);
}
}
{
const count:u32=9;if(!r.valid||count>9){r.valid=false;return value;}
value.I_load=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.I_load.push(decoded);
}
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.m_total=decoded;
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.F_x_Ctotal=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.F_x_Ctotal.push(decoded);
}
}
{
const count:u32=9;if(!r.valid||count>9){r.valid=false;return value;}
value.I_total=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.I_total.push(decoded);
}
}
{
const count:u32=16;if(!r.valid||count>16){r.valid=false;return value;}
value.O_T_EE=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.O_T_EE.push(decoded);
}
}
{
const count:u32=16;if(!r.valid||count>16){r.valid=false;return value;}
value.O_T_EE_d=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.O_T_EE_d.push(decoded);
}
}
{
const count:u32=16;if(!r.valid||count>16){r.valid=false;return value;}
value.O_T_EE_c=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.O_T_EE_c.push(decoded);
}
}
{
const count:u32=16;if(!r.valid||count>16){r.valid=false;return value;}
value.F_T_EE=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.F_T_EE.push(decoded);
}
}
{
const count:u32=16;if(!r.valid||count>16){r.valid=false;return value;}
value.F_T_NE=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.F_T_NE.push(decoded);
}
}
{
const count:u32=16;if(!r.valid||count>16){r.valid=false;return value;}
value.NE_T_EE=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.NE_T_EE.push(decoded);
}
}
{
const count:u32=16;if(!r.valid||count>16){r.valid=false;return value;}
value.EE_T_K=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.EE_T_K.push(decoded);
}
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
const decoded=Ros1Franka_msgsErrors.readControl(r);
value.current_errors=decoded;
}
{
const decoded=Ros1Franka_msgsErrors.readControl(r);
value.last_motion_errors=decoded;
}
return value;}
}
export class Ros1Franka_msgsFrankaStateCodec implements VendorCodec<Ros1Franka_msgsFrankaState>{
static readonly codecIdentity:string="UWVMROS1_79f22f36ce2c11f519ee29e65a248ec1b67240e27712b2667e9b803a2562ee71";
static readonly maxWireBytes:i32=2619;
encodeChecked(value:Ros1Franka_msgsFrankaState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsFrankaState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsFrankaState>{const out=new VendorDecoded<Ros1Franka_msgsFrankaState>();if(bytes.length>2619){out.status=90;return out;}if(bytes.length<2491){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsFrankaState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsFrankaState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsFrankaState():changetype<Ros1Franka_msgsFrankaState>(out.value);}
}
/** Complete original franka_msgs/SetCartesianImpedanceRequest; spellings are published in generation proof. */
export class Ros1Franka_msgsSetCartesianImpedanceRequest {
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
static readControl(r:Reader):Ros1Franka_msgsSetCartesianImpedanceRequest{const value=new Ros1Franka_msgsSetCartesianImpedanceRequest();
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.cartesian_stiffness=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.cartesian_stiffness.push(decoded);
}
}
return value;}
}
export class Ros1Franka_msgsSetCartesianImpedanceRequestCodec implements VendorCodec<Ros1Franka_msgsSetCartesianImpedanceRequest>{
static readonly codecIdentity:string="UWVMROS1_ff604aae9ab1ef36b5af107966e64ee2115ee17618a4ece6f15997c889cacbdf";
static readonly maxWireBytes:i32=48;
encodeChecked(value:Ros1Franka_msgsSetCartesianImpedanceRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetCartesianImpedanceRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetCartesianImpedanceRequest>{const out=new VendorDecoded<Ros1Franka_msgsSetCartesianImpedanceRequest>();if(bytes.length>48){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetCartesianImpedanceRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetCartesianImpedanceRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetCartesianImpedanceRequest():changetype<Ros1Franka_msgsSetCartesianImpedanceRequest>(out.value);}
}
/** Complete original franka_msgs/SetCartesianImpedanceResponse; spellings are published in generation proof. */
export class Ros1Franka_msgsSetCartesianImpedanceResponse {
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
static readControl(r:Reader):Ros1Franka_msgsSetCartesianImpedanceResponse{const value=new Ros1Franka_msgsSetCartesianImpedanceResponse();
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
export class Ros1Franka_msgsSetCartesianImpedanceResponseCodec implements VendorCodec<Ros1Franka_msgsSetCartesianImpedanceResponse>{
static readonly codecIdentity:string="UWVMROS1_b091d2fe98c84528460ba3d84524c38355e725c161456edf0034e62c6f63ab5b";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Franka_msgsSetCartesianImpedanceResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetCartesianImpedanceResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetCartesianImpedanceResponse>{const out=new VendorDecoded<Ros1Franka_msgsSetCartesianImpedanceResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetCartesianImpedanceResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetCartesianImpedanceResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetCartesianImpedanceResponse():changetype<Ros1Franka_msgsSetCartesianImpedanceResponse>(out.value);}
}
/** Complete original franka_msgs/SetEEFrameRequest; spellings are published in generation proof. */
export class Ros1Franka_msgsSetEEFrameRequest {
static readonly maxWireBytes:i32=128;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=128;
/** Exact native field NE_T_EE, native scalar float64. */
NE_T_EE:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<16;++i)this.NE_T_EE.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.NE_T_EE.length!=16)return -22;
for(let i=0;i<this.NE_T_EE.length;++i){
if(!isFinite(this.NE_T_EE[i]))return -22;
if(8>128-n)return -90;n+=8;
if(n>128)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.NE_T_EE.length;++i){
w.integer(<u64>reinterpret<u64>(this.NE_T_EE[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Franka_msgsSetEEFrameRequest{const value=new Ros1Franka_msgsSetEEFrameRequest();
{
const count:u32=16;if(!r.valid||count>16){r.valid=false;return value;}
value.NE_T_EE=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.NE_T_EE.push(decoded);
}
}
return value;}
}
export class Ros1Franka_msgsSetEEFrameRequestCodec implements VendorCodec<Ros1Franka_msgsSetEEFrameRequest>{
static readonly codecIdentity:string="UWVMROS1_1bc740d3998a14dd6c0604d5024ebda2b4916a8497950841b14a8073c6c1ecfc";
static readonly maxWireBytes:i32=128;
encodeChecked(value:Ros1Franka_msgsSetEEFrameRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetEEFrameRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetEEFrameRequest>{const out=new VendorDecoded<Ros1Franka_msgsSetEEFrameRequest>();if(bytes.length>128){out.status=90;return out;}if(bytes.length<128){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetEEFrameRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetEEFrameRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetEEFrameRequest():changetype<Ros1Franka_msgsSetEEFrameRequest>(out.value);}
}
/** Complete original franka_msgs/SetEEFrameResponse; spellings are published in generation proof. */
export class Ros1Franka_msgsSetEEFrameResponse {
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
static readControl(r:Reader):Ros1Franka_msgsSetEEFrameResponse{const value=new Ros1Franka_msgsSetEEFrameResponse();
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
export class Ros1Franka_msgsSetEEFrameResponseCodec implements VendorCodec<Ros1Franka_msgsSetEEFrameResponse>{
static readonly codecIdentity:string="UWVMROS1_b40c9595d57d6ed52ea4eca78e21f2c3d81f5a0afaad2420511211931f7d1c67";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Franka_msgsSetEEFrameResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetEEFrameResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetEEFrameResponse>{const out=new VendorDecoded<Ros1Franka_msgsSetEEFrameResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetEEFrameResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetEEFrameResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetEEFrameResponse():changetype<Ros1Franka_msgsSetEEFrameResponse>(out.value);}
}
/** Complete original franka_msgs/SetForceTorqueCollisionBehaviorRequest; spellings are published in generation proof. */
export class Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest {
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
static readControl(r:Reader):Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest{const value=new Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest();
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
export class Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequestCodec implements VendorCodec<Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest>{
static readonly codecIdentity:string="UWVMROS1_2513f0ea74bf6a871a85ce00c0f822891f2c0bed890b87e35d25a637aa62c4b5";
static readonly maxWireBytes:i32=208;
encodeChecked(value:Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest>{const out=new VendorDecoded<Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest>();if(bytes.length>208){out.status=90;return out;}if(bytes.length<208){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest():changetype<Ros1Franka_msgsSetForceTorqueCollisionBehaviorRequest>(out.value);}
}
/** Complete original franka_msgs/SetForceTorqueCollisionBehaviorResponse; spellings are published in generation proof. */
export class Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse {
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
static readControl(r:Reader):Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse{const value=new Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse();
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
export class Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponseCodec implements VendorCodec<Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse>{
static readonly codecIdentity:string="UWVMROS1_72418d73d504ae385d7ddce7e3cef859a104346dbfea7fc9da82e0f7e33ca2f5";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse>{const out=new VendorDecoded<Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse():changetype<Ros1Franka_msgsSetForceTorqueCollisionBehaviorResponse>(out.value);}
}
/** Complete original franka_msgs/SetFullCollisionBehaviorRequest; spellings are published in generation proof. */
export class Ros1Franka_msgsSetFullCollisionBehaviorRequest {
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
static readControl(r:Reader):Ros1Franka_msgsSetFullCollisionBehaviorRequest{const value=new Ros1Franka_msgsSetFullCollisionBehaviorRequest();
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
export class Ros1Franka_msgsSetFullCollisionBehaviorRequestCodec implements VendorCodec<Ros1Franka_msgsSetFullCollisionBehaviorRequest>{
static readonly codecIdentity:string="UWVMROS1_47a86620ec5abdd6cc820b095d8f5f4214c1d9af00ac1ff0740137bbdda7217c";
static readonly maxWireBytes:i32=416;
encodeChecked(value:Ros1Franka_msgsSetFullCollisionBehaviorRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetFullCollisionBehaviorRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetFullCollisionBehaviorRequest>{const out=new VendorDecoded<Ros1Franka_msgsSetFullCollisionBehaviorRequest>();if(bytes.length>416){out.status=90;return out;}if(bytes.length<416){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetFullCollisionBehaviorRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetFullCollisionBehaviorRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetFullCollisionBehaviorRequest():changetype<Ros1Franka_msgsSetFullCollisionBehaviorRequest>(out.value);}
}
/** Complete original franka_msgs/SetFullCollisionBehaviorResponse; spellings are published in generation proof. */
export class Ros1Franka_msgsSetFullCollisionBehaviorResponse {
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
static readControl(r:Reader):Ros1Franka_msgsSetFullCollisionBehaviorResponse{const value=new Ros1Franka_msgsSetFullCollisionBehaviorResponse();
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
export class Ros1Franka_msgsSetFullCollisionBehaviorResponseCodec implements VendorCodec<Ros1Franka_msgsSetFullCollisionBehaviorResponse>{
static readonly codecIdentity:string="UWVMROS1_26f46e286c42d51a8ef45db7c8b3bf315fb02faf7b2be5d3741563d43169a0e2";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Franka_msgsSetFullCollisionBehaviorResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetFullCollisionBehaviorResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetFullCollisionBehaviorResponse>{const out=new VendorDecoded<Ros1Franka_msgsSetFullCollisionBehaviorResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetFullCollisionBehaviorResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetFullCollisionBehaviorResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetFullCollisionBehaviorResponse():changetype<Ros1Franka_msgsSetFullCollisionBehaviorResponse>(out.value);}
}
/** Complete original franka_msgs/SetJointImpedanceRequest; spellings are published in generation proof. */
export class Ros1Franka_msgsSetJointImpedanceRequest {
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
static readControl(r:Reader):Ros1Franka_msgsSetJointImpedanceRequest{const value=new Ros1Franka_msgsSetJointImpedanceRequest();
{
const count:u32=7;if(!r.valid||count>7){r.valid=false;return value;}
value.joint_stiffness=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.joint_stiffness.push(decoded);
}
}
return value;}
}
export class Ros1Franka_msgsSetJointImpedanceRequestCodec implements VendorCodec<Ros1Franka_msgsSetJointImpedanceRequest>{
static readonly codecIdentity:string="UWVMROS1_29b5419f698b39452d38fbc58a0e3c572b8d285b70c8acf7d326c440eb417c53";
static readonly maxWireBytes:i32=56;
encodeChecked(value:Ros1Franka_msgsSetJointImpedanceRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetJointImpedanceRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetJointImpedanceRequest>{const out=new VendorDecoded<Ros1Franka_msgsSetJointImpedanceRequest>();if(bytes.length>56){out.status=90;return out;}if(bytes.length<56){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetJointImpedanceRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetJointImpedanceRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetJointImpedanceRequest():changetype<Ros1Franka_msgsSetJointImpedanceRequest>(out.value);}
}
/** Complete original franka_msgs/SetJointImpedanceResponse; spellings are published in generation proof. */
export class Ros1Franka_msgsSetJointImpedanceResponse {
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
static readControl(r:Reader):Ros1Franka_msgsSetJointImpedanceResponse{const value=new Ros1Franka_msgsSetJointImpedanceResponse();
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
export class Ros1Franka_msgsSetJointImpedanceResponseCodec implements VendorCodec<Ros1Franka_msgsSetJointImpedanceResponse>{
static readonly codecIdentity:string="UWVMROS1_1ae0cd7c4ad89de09ccffd6c4999435fa6965509e99a9adb98b7f3ebb3ea4c55";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Franka_msgsSetJointImpedanceResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetJointImpedanceResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetJointImpedanceResponse>{const out=new VendorDecoded<Ros1Franka_msgsSetJointImpedanceResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetJointImpedanceResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetJointImpedanceResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetJointImpedanceResponse():changetype<Ros1Franka_msgsSetJointImpedanceResponse>(out.value);}
}
/** Complete original franka_msgs/SetKFrameRequest; spellings are published in generation proof. */
export class Ros1Franka_msgsSetKFrameRequest {
static readonly maxWireBytes:i32=128;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=128;
/** Exact native field EE_T_K, native scalar float64. */
EE_T_K:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<16;++i)this.EE_T_K.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.EE_T_K.length!=16)return -22;
for(let i=0;i<this.EE_T_K.length;++i){
if(!isFinite(this.EE_T_K[i]))return -22;
if(8>128-n)return -90;n+=8;
if(n>128)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.EE_T_K.length;++i){
w.integer(<u64>reinterpret<u64>(this.EE_T_K[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Franka_msgsSetKFrameRequest{const value=new Ros1Franka_msgsSetKFrameRequest();
{
const count:u32=16;if(!r.valid||count>16){r.valid=false;return value;}
value.EE_T_K=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.EE_T_K.push(decoded);
}
}
return value;}
}
export class Ros1Franka_msgsSetKFrameRequestCodec implements VendorCodec<Ros1Franka_msgsSetKFrameRequest>{
static readonly codecIdentity:string="UWVMROS1_36afcdc6402259fb5a71ae1b27be0bb8623a64dd93caffa376adea7862c86446";
static readonly maxWireBytes:i32=128;
encodeChecked(value:Ros1Franka_msgsSetKFrameRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetKFrameRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetKFrameRequest>{const out=new VendorDecoded<Ros1Franka_msgsSetKFrameRequest>();if(bytes.length>128){out.status=90;return out;}if(bytes.length<128){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetKFrameRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetKFrameRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetKFrameRequest():changetype<Ros1Franka_msgsSetKFrameRequest>(out.value);}
}
/** Complete original franka_msgs/SetKFrameResponse; spellings are published in generation proof. */
export class Ros1Franka_msgsSetKFrameResponse {
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
static readControl(r:Reader):Ros1Franka_msgsSetKFrameResponse{const value=new Ros1Franka_msgsSetKFrameResponse();
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
export class Ros1Franka_msgsSetKFrameResponseCodec implements VendorCodec<Ros1Franka_msgsSetKFrameResponse>{
static readonly codecIdentity:string="UWVMROS1_4ea77d3e827a9b9eaf3c3f9805f78c38bfe91789555d0469cffd4ff4266cd443";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Franka_msgsSetKFrameResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetKFrameResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetKFrameResponse>{const out=new VendorDecoded<Ros1Franka_msgsSetKFrameResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetKFrameResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetKFrameResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetKFrameResponse():changetype<Ros1Franka_msgsSetKFrameResponse>(out.value);}
}
/** Complete original franka_msgs/SetLoadRequest; spellings are published in generation proof. */
export class Ros1Franka_msgsSetLoadRequest {
static readonly maxWireBytes:i32=104;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=104;
/** Exact native field mass, native scalar float64. */
mass:f64=0;
/** Exact native field F_x_center_load, native scalar float64. */
F_x_center_load:Array<f64>=new Array<f64>();
/** Exact native field load_inertia, native scalar float64. */
load_inertia:Array<f64>=new Array<f64>();
constructor(){
for(let i=0;i<3;++i)this.F_x_center_load.push(0);
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
if(this.F_x_center_load.length!=3)return -22;
for(let i=0;i<this.F_x_center_load.length;++i){
if(!isFinite(this.F_x_center_load[i]))return -22;
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
for(let i=0;i<this.F_x_center_load.length;++i){
w.integer(<u64>reinterpret<u64>(this.F_x_center_load[i]),8);
}
for(let i=0;i<this.load_inertia.length;++i){
w.integer(<u64>reinterpret<u64>(this.load_inertia[i]),8);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Franka_msgsSetLoadRequest{const value=new Ros1Franka_msgsSetLoadRequest();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.mass=decoded;
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.F_x_center_load=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.F_x_center_load.push(decoded);
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
export class Ros1Franka_msgsSetLoadRequestCodec implements VendorCodec<Ros1Franka_msgsSetLoadRequest>{
static readonly codecIdentity:string="UWVMROS1_d7c08dd9105cb7b83cb251e80c7d2dd1e9f7bb2a43f614bff6d00d808cccbcb5";
static readonly maxWireBytes:i32=104;
encodeChecked(value:Ros1Franka_msgsSetLoadRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetLoadRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetLoadRequest>{const out=new VendorDecoded<Ros1Franka_msgsSetLoadRequest>();if(bytes.length>104){out.status=90;return out;}if(bytes.length<104){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetLoadRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetLoadRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetLoadRequest():changetype<Ros1Franka_msgsSetLoadRequest>(out.value);}
}
/** Complete original franka_msgs/SetLoadResponse; spellings are published in generation proof. */
export class Ros1Franka_msgsSetLoadResponse {
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
static readControl(r:Reader):Ros1Franka_msgsSetLoadResponse{const value=new Ros1Franka_msgsSetLoadResponse();
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
export class Ros1Franka_msgsSetLoadResponseCodec implements VendorCodec<Ros1Franka_msgsSetLoadResponse>{
static readonly codecIdentity:string="UWVMROS1_f18952407bf9e2813deeaa3734a781e90b74cb79892919c1218812ad71255cf3";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros1Franka_msgsSetLoadResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Franka_msgsSetLoadResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Franka_msgsSetLoadResponse>{const out=new VendorDecoded<Ros1Franka_msgsSetLoadResponse>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Franka_msgsSetLoadResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Franka_msgsSetLoadResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Franka_msgsSetLoadResponse():changetype<Ros1Franka_msgsSetLoadResponse>(out.value);}
}
