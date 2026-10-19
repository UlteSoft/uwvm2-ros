/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original abb_egm_msgs/EGMChannelState; spellings are published in generation proof. */
export class Ros1Abb_egm_msgsEGMChannelState {
static readonly EGM_UNDEFINED:u8=1;
static readonly EGM_ERROR:u8=2;
static readonly EGM_STOPPED:u8=3;
static readonly EGM_RUNNING:u8=4;
static readonly MOTORS_UNDEFINED:u8=1;
static readonly MOTORS_ON:u8=2;
static readonly MOTORS_OFF:u8=3;
static readonly RAPID_UNDEFINED:u8=1;
static readonly RAPID_STOPPED:u8=2;
static readonly RAPID_RUNNING:u8=3;
static readonly maxWireBytes:i32=81;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=81;
/** Exact native field name, native scalar string. */
name:string="";
/** Exact native field active, native scalar bool. */
active:bool=false;
/** Exact native field egm_convergence_met, native scalar bool. */
egm_convergence_met:bool=false;
/** Exact native field egm_client_state, native scalar uint8. */
egm_client_state:u8=0;
/** Exact native field motor_state, native scalar uint8. */
motor_state:u8=0;
/** Exact native field rapid_execution_state, native scalar uint8. */
rapid_execution_state:u8=0;
/** Exact native field utilization_rate, native scalar float64. */
utilization_rate:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.name)||this.name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name);if(size>64)return -22;if(4+size>81-n)return -90;n+=4+size;
if(n>81)return -90;
}
{
if(1>81-n)return -90;n+=1;
if(n>81)return -90;
}
{
if(1>81-n)return -90;n+=1;
if(n>81)return -90;
}
{
if(1>81-n)return -90;n+=1;
if(n>81)return -90;
}
{
if(1>81-n)return -90;n+=1;
if(n>81)return -90;
}
{
if(1>81-n)return -90;n+=1;
if(n>81)return -90;
}
{
if(!isFinite(this.utilization_rate))return -22;
if(8>81-n)return -90;n+=8;
if(n>81)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.name);
w.u8(this.active?1:0);
w.u8(this.egm_convergence_met?1:0);
w.integer(<u64>this.egm_client_state,1);
w.integer(<u64>this.motor_state,1);
w.integer(<u64>this.rapid_execution_state,1);
w.integer(<u64>reinterpret<u64>(this.utilization_rate),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_egm_msgsEGMChannelState{const value=new Ros1Abb_egm_msgsEGMChannelState();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.name=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.active=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.egm_convergence_met=decoded;
}
{
const decoded=<u8>r.integer(1);
value.egm_client_state=decoded;
}
{
const decoded=<u8>r.integer(1);
value.motor_state=decoded;
}
{
const decoded=<u8>r.integer(1);
value.rapid_execution_state=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.utilization_rate=decoded;
}
return value;}
}
export class Ros1Abb_egm_msgsEGMChannelStateCodec implements VendorCodec<Ros1Abb_egm_msgsEGMChannelState>{
static readonly codecIdentity:string="UWVMROS1_452d2adde3b1475fe3ff09ea11ff2ec7105a58c32e20d764e0c2ff152cb432f2";
static readonly maxWireBytes:i32=81;
encodeChecked(value:Ros1Abb_egm_msgsEGMChannelState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_egm_msgsEGMChannelState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_egm_msgsEGMChannelState>{const out=new VendorDecoded<Ros1Abb_egm_msgsEGMChannelState>();if(bytes.length>81){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_egm_msgsEGMChannelState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_egm_msgsEGMChannelState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_egm_msgsEGMChannelState():changetype<Ros1Abb_egm_msgsEGMChannelState>(out.value);}
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
static readonly maxWireBytes:i32=80;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=80;
/** Exact native field seq, native scalar uint32. */
seq:u32=0;
/** Exact native field stamp, native scalar time. */
stamp:Ros1Time=new Ros1Time();
/** Exact native field frame_id, native scalar string. */
frame_id:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>80-n)return -90;n+=4;
if(n>80)return -90;
}
{
const child=this.stamp.wireSize();if(child<0)return child;if(child>80-n)return -90;n+=child;
if(n>80)return -90;
}
{
if(!validUnicode(this.frame_id)||this.frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.frame_id);if(size>64)return -22;if(4+size>80-n)return -90;n+=4+size;
if(n>80)return -90;
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
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.frame_id=decoded;
}
return value;}
}
export class Ros1Std_msgsHeaderCodec implements VendorCodec<Ros1Std_msgsHeader>{
static readonly codecIdentity:string="UWVMROS1_ca1b8ffce276bdc4f86dccae58ede9f860ffe8778c6d4ffe0fb70675777801db";
static readonly maxWireBytes:i32=80;
encodeChecked(value:Ros1Std_msgsHeader):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Std_msgsHeader):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Std_msgsHeader>{const out=new VendorDecoded<Ros1Std_msgsHeader>();if(bytes.length>80){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Std_msgsHeader.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Std_msgsHeader{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Std_msgsHeader():changetype<Ros1Std_msgsHeader>(out.value);}
}
/** Complete original abb_egm_msgs/EGMState; spellings are published in generation proof. */
export class Ros1Abb_egm_msgsEGMState {
static readonly maxWireBytes:i32=1380;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1380;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field egm_channels, native scalar abb_egm_msgs/EGMChannelState. */
egm_channels:Array<Ros1Abb_egm_msgsEGMChannelState>=new Array<Ros1Abb_egm_msgsEGMChannelState>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>1380-n)return -90;n+=child;
if(n>1380)return -90;
}
{
if(this.egm_channels.length>16)return -22;
if(4>1380-n)return -90;n+=4;
for(let i=0;i<this.egm_channels.length;++i){
const child=this.egm_channels[i].wireSize();if(child<0)return child;if(child>1380-n)return -90;n+=child;
if(n>1380)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.egm_channels.length);
for(let i=0;i<this.egm_channels.length;++i){
this.egm_channels[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_egm_msgsEGMState{const value=new Ros1Abb_egm_msgsEGMState();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.egm_channels=new Array<Ros1Abb_egm_msgsEGMChannelState>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Abb_egm_msgsEGMChannelState.readControl(r);
value.egm_channels.push(decoded);
}
}
return value;}
}
export class Ros1Abb_egm_msgsEGMStateCodec implements VendorCodec<Ros1Abb_egm_msgsEGMState>{
static readonly codecIdentity:string="UWVMROS1_f7fe1d3924dc4e497c58aab27da69cfa6ab1a11f64b970b42e2d040a1312c4d8";
static readonly maxWireBytes:i32=1380;
encodeChecked(value:Ros1Abb_egm_msgsEGMState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_egm_msgsEGMState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_egm_msgsEGMState>{const out=new VendorDecoded<Ros1Abb_egm_msgsEGMState>();if(bytes.length>1380){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_egm_msgsEGMState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_egm_msgsEGMState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_egm_msgsEGMState():changetype<Ros1Abb_egm_msgsEGMState>(out.value);}
}
/** Complete original abb_rapid_msgs/ConfData; spellings are published in generation proof. */
export class Ros1Abb_rapid_msgsConfData {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field cf1, native scalar float32. */
cf1:f32=0;
/** Exact native field cf4, native scalar float32. */
cf4:f32=0;
/** Exact native field cf6, native scalar float32. */
cf6:f32=0;
/** Exact native field cfx, native scalar float32. */
cfx:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.cf1))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.cf4))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.cf6))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.cfx))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.cf1),4);
w.integer(<u64>reinterpret<u32>(this.cf4),4);
w.integer(<u64>reinterpret<u32>(this.cf6),4);
w.integer(<u64>reinterpret<u32>(this.cfx),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_msgsConfData{const value=new Ros1Abb_rapid_msgsConfData();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cf1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cf4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cf6=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cfx=decoded;
}
return value;}
}
export class Ros1Abb_rapid_msgsConfDataCodec implements VendorCodec<Ros1Abb_rapid_msgsConfData>{
static readonly codecIdentity:string="UWVMROS1_293eb01e8fa150a8a3c741626b76804255d0eb321a0484aac402201447392af2";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Abb_rapid_msgsConfData):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_msgsConfData):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_msgsConfData>{const out=new VendorDecoded<Ros1Abb_rapid_msgsConfData>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_msgsConfData.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_msgsConfData{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_msgsConfData():changetype<Ros1Abb_rapid_msgsConfData>(out.value);}
}
/** Complete original abb_rapid_msgs/ExtJoint; spellings are published in generation proof. */
export class Ros1Abb_rapid_msgsExtJoint {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field eax_a, native scalar float32. */
eax_a:f32=0;
/** Exact native field eax_b, native scalar float32. */
eax_b:f32=0;
/** Exact native field eax_c, native scalar float32. */
eax_c:f32=0;
/** Exact native field eax_d, native scalar float32. */
eax_d:f32=0;
/** Exact native field eax_e, native scalar float32. */
eax_e:f32=0;
/** Exact native field eax_f, native scalar float32. */
eax_f:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.eax_a))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.eax_b))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.eax_c))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.eax_d))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.eax_e))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.eax_f))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.eax_a),4);
w.integer(<u64>reinterpret<u32>(this.eax_b),4);
w.integer(<u64>reinterpret<u32>(this.eax_c),4);
w.integer(<u64>reinterpret<u32>(this.eax_d),4);
w.integer(<u64>reinterpret<u32>(this.eax_e),4);
w.integer(<u64>reinterpret<u32>(this.eax_f),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_msgsExtJoint{const value=new Ros1Abb_rapid_msgsExtJoint();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.eax_a=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.eax_b=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.eax_c=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.eax_d=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.eax_e=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.eax_f=decoded;
}
return value;}
}
export class Ros1Abb_rapid_msgsExtJointCodec implements VendorCodec<Ros1Abb_rapid_msgsExtJoint>{
static readonly codecIdentity:string="UWVMROS1_653a47c44ab85151fd9e6215901fb154962c32fe1082386733ec6205eaae613a";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Abb_rapid_msgsExtJoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_msgsExtJoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_msgsExtJoint>{const out=new VendorDecoded<Ros1Abb_rapid_msgsExtJoint>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_msgsExtJoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_msgsExtJoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_msgsExtJoint():changetype<Ros1Abb_rapid_msgsExtJoint>(out.value);}
}
/** Complete original abb_rapid_msgs/RobJoint; spellings are published in generation proof. */
export class Ros1Abb_rapid_msgsRobJoint {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field rax_1, native scalar float32. */
rax_1:f32=0;
/** Exact native field rax_2, native scalar float32. */
rax_2:f32=0;
/** Exact native field rax_3, native scalar float32. */
rax_3:f32=0;
/** Exact native field rax_4, native scalar float32. */
rax_4:f32=0;
/** Exact native field rax_5, native scalar float32. */
rax_5:f32=0;
/** Exact native field rax_6, native scalar float32. */
rax_6:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.rax_1))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rax_2))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rax_3))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rax_4))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rax_5))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rax_6))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.rax_1),4);
w.integer(<u64>reinterpret<u32>(this.rax_2),4);
w.integer(<u64>reinterpret<u32>(this.rax_3),4);
w.integer(<u64>reinterpret<u32>(this.rax_4),4);
w.integer(<u64>reinterpret<u32>(this.rax_5),4);
w.integer(<u64>reinterpret<u32>(this.rax_6),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_msgsRobJoint{const value=new Ros1Abb_rapid_msgsRobJoint();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rax_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rax_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rax_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rax_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rax_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rax_6=decoded;
}
return value;}
}
export class Ros1Abb_rapid_msgsRobJointCodec implements VendorCodec<Ros1Abb_rapid_msgsRobJoint>{
static readonly codecIdentity:string="UWVMROS1_5d45bc0efae8c4f5b26e4043fe537e163c4cc4c6458d1eac9be429c37aaf82f0";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Abb_rapid_msgsRobJoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_msgsRobJoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_msgsRobJoint>{const out=new VendorDecoded<Ros1Abb_rapid_msgsRobJoint>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_msgsRobJoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_msgsRobJoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_msgsRobJoint():changetype<Ros1Abb_rapid_msgsRobJoint>(out.value);}
}
/** Complete original abb_rapid_msgs/JointTarget; spellings are published in generation proof. */
export class Ros1Abb_rapid_msgsJointTarget {
static readonly maxWireBytes:i32=48;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=48;
/** Exact native field robax, native scalar abb_rapid_msgs/RobJoint. */
robax:Ros1Abb_rapid_msgsRobJoint=new Ros1Abb_rapid_msgsRobJoint();
/** Exact native field extax, native scalar abb_rapid_msgs/ExtJoint. */
extax:Ros1Abb_rapid_msgsExtJoint=new Ros1Abb_rapid_msgsExtJoint();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.robax.wireSize();if(child<0)return child;if(child>48-n)return -90;n+=child;
if(n>48)return -90;
}
{
const child=this.extax.wireSize();if(child<0)return child;if(child>48-n)return -90;n+=child;
if(n>48)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.robax.writeControl(w);
this.extax.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_msgsJointTarget{const value=new Ros1Abb_rapid_msgsJointTarget();
{
const decoded=Ros1Abb_rapid_msgsRobJoint.readControl(r);
value.robax=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsExtJoint.readControl(r);
value.extax=decoded;
}
return value;}
}
export class Ros1Abb_rapid_msgsJointTargetCodec implements VendorCodec<Ros1Abb_rapid_msgsJointTarget>{
static readonly codecIdentity:string="UWVMROS1_4a1cc175468da9204c5f2f399b76712617daec21a0d1675a6e73c84ff170dc24";
static readonly maxWireBytes:i32=48;
encodeChecked(value:Ros1Abb_rapid_msgsJointTarget):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_msgsJointTarget):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_msgsJointTarget>{const out=new VendorDecoded<Ros1Abb_rapid_msgsJointTarget>();if(bytes.length>48){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_msgsJointTarget.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_msgsJointTarget{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_msgsJointTarget():changetype<Ros1Abb_rapid_msgsJointTarget>(out.value);}
}
/** Complete original abb_rapid_msgs/Pos; spellings are published in generation proof. */
export class Ros1Abb_rapid_msgsPos {
static readonly maxWireBytes:i32=12;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=12;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>12-n)return -90;n+=4;
if(n>12)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>12-n)return -90;n+=4;
if(n>12)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>12-n)return -90;n+=4;
if(n>12)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_msgsPos{const value=new Ros1Abb_rapid_msgsPos();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.x=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.y=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.z=decoded;
}
return value;}
}
export class Ros1Abb_rapid_msgsPosCodec implements VendorCodec<Ros1Abb_rapid_msgsPos>{
static readonly codecIdentity:string="UWVMROS1_79edd4553fad8a8f833a3399e354c93538d4971bc3820cc14cf540e960356c01";
static readonly maxWireBytes:i32=12;
encodeChecked(value:Ros1Abb_rapid_msgsPos):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_msgsPos):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_msgsPos>{const out=new VendorDecoded<Ros1Abb_rapid_msgsPos>();if(bytes.length>12){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_msgsPos.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_msgsPos{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_msgsPos():changetype<Ros1Abb_rapid_msgsPos>(out.value);}
}
/** Complete original abb_rapid_msgs/Orient; spellings are published in generation proof. */
export class Ros1Abb_rapid_msgsOrient {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field q1, native scalar float32. */
q1:f32=0;
/** Exact native field q2, native scalar float32. */
q2:f32=0;
/** Exact native field q3, native scalar float32. */
q3:f32=0;
/** Exact native field q4, native scalar float32. */
q4:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.q1))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.q2))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.q3))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.q4))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.q1),4);
w.integer(<u64>reinterpret<u32>(this.q2),4);
w.integer(<u64>reinterpret<u32>(this.q3),4);
w.integer(<u64>reinterpret<u32>(this.q4),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_msgsOrient{const value=new Ros1Abb_rapid_msgsOrient();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.q1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.q2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.q3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.q4=decoded;
}
return value;}
}
export class Ros1Abb_rapid_msgsOrientCodec implements VendorCodec<Ros1Abb_rapid_msgsOrient>{
static readonly codecIdentity:string="UWVMROS1_5a9c29323a5e3bc0c4c0afa7d305738cd52b49ae339e72f6b34b35b165f3d270";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Abb_rapid_msgsOrient):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_msgsOrient):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_msgsOrient>{const out=new VendorDecoded<Ros1Abb_rapid_msgsOrient>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_msgsOrient.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_msgsOrient{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_msgsOrient():changetype<Ros1Abb_rapid_msgsOrient>(out.value);}
}
/** Complete original abb_rapid_msgs/LoadData; spellings are published in generation proof. */
export class Ros1Abb_rapid_msgsLoadData {
static readonly maxWireBytes:i32=44;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=44;
/** Exact native field mass, native scalar float32. */
mass:f32=0;
/** Exact native field cog, native scalar abb_rapid_msgs/Pos. */
cog:Ros1Abb_rapid_msgsPos=new Ros1Abb_rapid_msgsPos();
/** Exact native field aom, native scalar abb_rapid_msgs/Orient. */
aom:Ros1Abb_rapid_msgsOrient=new Ros1Abb_rapid_msgsOrient();
/** Exact native field ix, native scalar float32. */
ix:f32=0;
/** Exact native field iy, native scalar float32. */
iy:f32=0;
/** Exact native field iz, native scalar float32. */
iz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.mass))return -22;
if(4>44-n)return -90;n+=4;
if(n>44)return -90;
}
{
const child=this.cog.wireSize();if(child<0)return child;if(child>44-n)return -90;n+=child;
if(n>44)return -90;
}
{
const child=this.aom.wireSize();if(child<0)return child;if(child>44-n)return -90;n+=child;
if(n>44)return -90;
}
{
if(!isFinite(this.ix))return -22;
if(4>44-n)return -90;n+=4;
if(n>44)return -90;
}
{
if(!isFinite(this.iy))return -22;
if(4>44-n)return -90;n+=4;
if(n>44)return -90;
}
{
if(!isFinite(this.iz))return -22;
if(4>44-n)return -90;n+=4;
if(n>44)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.mass),4);
this.cog.writeControl(w);
this.aom.writeControl(w);
w.integer(<u64>reinterpret<u32>(this.ix),4);
w.integer(<u64>reinterpret<u32>(this.iy),4);
w.integer(<u64>reinterpret<u32>(this.iz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_msgsLoadData{const value=new Ros1Abb_rapid_msgsLoadData();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.mass=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPos.readControl(r);
value.cog=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsOrient.readControl(r);
value.aom=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ix=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.iy=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.iz=decoded;
}
return value;}
}
export class Ros1Abb_rapid_msgsLoadDataCodec implements VendorCodec<Ros1Abb_rapid_msgsLoadData>{
static readonly codecIdentity:string="UWVMROS1_2cc985cc188a03e7c6de3d22310802cac1e0a748bae7f828d957436c1e03303d";
static readonly maxWireBytes:i32=44;
encodeChecked(value:Ros1Abb_rapid_msgsLoadData):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_msgsLoadData):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_msgsLoadData>{const out=new VendorDecoded<Ros1Abb_rapid_msgsLoadData>();if(bytes.length>44){out.status=90;return out;}if(bytes.length<44){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_msgsLoadData.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_msgsLoadData{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_msgsLoadData():changetype<Ros1Abb_rapid_msgsLoadData>(out.value);}
}
/** Complete original abb_rapid_msgs/Pose; spellings are published in generation proof. */
export class Ros1Abb_rapid_msgsPose {
static readonly maxWireBytes:i32=28;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=28;
/** Exact native field trans, native scalar abb_rapid_msgs/Pos. */
trans:Ros1Abb_rapid_msgsPos=new Ros1Abb_rapid_msgsPos();
/** Exact native field rot, native scalar abb_rapid_msgs/Orient. */
rot:Ros1Abb_rapid_msgsOrient=new Ros1Abb_rapid_msgsOrient();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.trans.wireSize();if(child<0)return child;if(child>28-n)return -90;n+=child;
if(n>28)return -90;
}
{
const child=this.rot.wireSize();if(child<0)return child;if(child>28-n)return -90;n+=child;
if(n>28)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.trans.writeControl(w);
this.rot.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_msgsPose{const value=new Ros1Abb_rapid_msgsPose();
{
const decoded=Ros1Abb_rapid_msgsPos.readControl(r);
value.trans=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsOrient.readControl(r);
value.rot=decoded;
}
return value;}
}
export class Ros1Abb_rapid_msgsPoseCodec implements VendorCodec<Ros1Abb_rapid_msgsPose>{
static readonly codecIdentity:string="UWVMROS1_3e0cfbb7e25376639d1ff6e555a86e69f8c9ed177c1ef14b301d90b206a19c99";
static readonly maxWireBytes:i32=28;
encodeChecked(value:Ros1Abb_rapid_msgsPose):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_msgsPose):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_msgsPose>{const out=new VendorDecoded<Ros1Abb_rapid_msgsPose>();if(bytes.length>28){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_msgsPose.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_msgsPose{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_msgsPose():changetype<Ros1Abb_rapid_msgsPose>(out.value);}
}
/** Complete original abb_rapid_msgs/RobTarget; spellings are published in generation proof. */
export class Ros1Abb_rapid_msgsRobTarget {
static readonly maxWireBytes:i32=68;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=68;
/** Exact native field trans, native scalar abb_rapid_msgs/Pos. */
trans:Ros1Abb_rapid_msgsPos=new Ros1Abb_rapid_msgsPos();
/** Exact native field rot, native scalar abb_rapid_msgs/Orient. */
rot:Ros1Abb_rapid_msgsOrient=new Ros1Abb_rapid_msgsOrient();
/** Exact native field robconf, native scalar abb_rapid_msgs/ConfData. */
robconf:Ros1Abb_rapid_msgsConfData=new Ros1Abb_rapid_msgsConfData();
/** Exact native field extax, native scalar abb_rapid_msgs/ExtJoint. */
extax:Ros1Abb_rapid_msgsExtJoint=new Ros1Abb_rapid_msgsExtJoint();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.trans.wireSize();if(child<0)return child;if(child>68-n)return -90;n+=child;
if(n>68)return -90;
}
{
const child=this.rot.wireSize();if(child<0)return child;if(child>68-n)return -90;n+=child;
if(n>68)return -90;
}
{
const child=this.robconf.wireSize();if(child<0)return child;if(child>68-n)return -90;n+=child;
if(n>68)return -90;
}
{
const child=this.extax.wireSize();if(child<0)return child;if(child>68-n)return -90;n+=child;
if(n>68)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.trans.writeControl(w);
this.rot.writeControl(w);
this.robconf.writeControl(w);
this.extax.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_msgsRobTarget{const value=new Ros1Abb_rapid_msgsRobTarget();
{
const decoded=Ros1Abb_rapid_msgsPos.readControl(r);
value.trans=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsOrient.readControl(r);
value.rot=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsConfData.readControl(r);
value.robconf=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsExtJoint.readControl(r);
value.extax=decoded;
}
return value;}
}
export class Ros1Abb_rapid_msgsRobTargetCodec implements VendorCodec<Ros1Abb_rapid_msgsRobTarget>{
static readonly codecIdentity:string="UWVMROS1_0abb3aee8b1303ccf9419f2774829d1231de3ac033e4b644d585f195972f09ce";
static readonly maxWireBytes:i32=68;
encodeChecked(value:Ros1Abb_rapid_msgsRobTarget):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_msgsRobTarget):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_msgsRobTarget>{const out=new VendorDecoded<Ros1Abb_rapid_msgsRobTarget>();if(bytes.length>68){out.status=90;return out;}if(bytes.length<68){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_msgsRobTarget.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_msgsRobTarget{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_msgsRobTarget():changetype<Ros1Abb_rapid_msgsRobTarget>(out.value);}
}
/** Complete original abb_rapid_msgs/ToolData; spellings are published in generation proof. */
export class Ros1Abb_rapid_msgsToolData {
static readonly maxWireBytes:i32=73;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=73;
/** Exact native field robhold, native scalar bool. */
robhold:bool=false;
/** Exact native field tframe, native scalar abb_rapid_msgs/Pose. */
tframe:Ros1Abb_rapid_msgsPose=new Ros1Abb_rapid_msgsPose();
/** Exact native field tload, native scalar abb_rapid_msgs/LoadData. */
tload:Ros1Abb_rapid_msgsLoadData=new Ros1Abb_rapid_msgsLoadData();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>73-n)return -90;n+=1;
if(n>73)return -90;
}
{
const child=this.tframe.wireSize();if(child<0)return child;if(child>73-n)return -90;n+=child;
if(n>73)return -90;
}
{
const child=this.tload.wireSize();if(child<0)return child;if(child>73-n)return -90;n+=child;
if(n>73)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.robhold?1:0);
this.tframe.writeControl(w);
this.tload.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_msgsToolData{const value=new Ros1Abb_rapid_msgsToolData();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.robhold=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPose.readControl(r);
value.tframe=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsLoadData.readControl(r);
value.tload=decoded;
}
return value;}
}
export class Ros1Abb_rapid_msgsToolDataCodec implements VendorCodec<Ros1Abb_rapid_msgsToolData>{
static readonly codecIdentity:string="UWVMROS1_e2feaee4bda891f594542384d70883184262777e359fb037a648b7526ef76cba";
static readonly maxWireBytes:i32=73;
encodeChecked(value:Ros1Abb_rapid_msgsToolData):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_msgsToolData):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_msgsToolData>{const out=new VendorDecoded<Ros1Abb_rapid_msgsToolData>();if(bytes.length>73){out.status=90;return out;}if(bytes.length<73){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_msgsToolData.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_msgsToolData{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_msgsToolData():changetype<Ros1Abb_rapid_msgsToolData>(out.value);}
}
/** Complete original abb_rapid_msgs/WObjData; spellings are published in generation proof. */
export class Ros1Abb_rapid_msgsWObjData {
static readonly maxWireBytes:i32=574;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=574;
/** Exact native field robhold, native scalar bool. */
robhold:bool=false;
/** Exact native field ufprog, native scalar bool. */
ufprog:bool=false;
/** Exact native field ufmec, native scalar string. */
ufmec:string="";
/** Exact native field uframe, native scalar abb_rapid_msgs/Pose. */
uframe:Ros1Abb_rapid_msgsPose=new Ros1Abb_rapid_msgsPose();
/** Exact native field oframe, native scalar abb_rapid_msgs/Pose. */
oframe:Ros1Abb_rapid_msgsPose=new Ros1Abb_rapid_msgsPose();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>574-n)return -90;n+=1;
if(n>574)return -90;
}
{
if(1>574-n)return -90;n+=1;
if(n>574)return -90;
}
{
if(!validUnicode(this.ufmec)||this.ufmec.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.ufmec);if(size>512)return -22;if(4+size>574-n)return -90;n+=4+size;
if(n>574)return -90;
}
{
const child=this.uframe.wireSize();if(child<0)return child;if(child>574-n)return -90;n+=child;
if(n>574)return -90;
}
{
const child=this.oframe.wireSize();if(child<0)return child;if(child>574-n)return -90;n+=child;
if(n>574)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.robhold?1:0);
w.u8(this.ufprog?1:0);
w.text(this.ufmec);
this.uframe.writeControl(w);
this.oframe.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_msgsWObjData{const value=new Ros1Abb_rapid_msgsWObjData();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.robhold=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.ufprog=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.ufmec=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPose.readControl(r);
value.uframe=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPose.readControl(r);
value.oframe=decoded;
}
return value;}
}
export class Ros1Abb_rapid_msgsWObjDataCodec implements VendorCodec<Ros1Abb_rapid_msgsWObjData>{
static readonly codecIdentity:string="UWVMROS1_6de4cf654a15ef3b63d9b0f3d0984438f21963c10ffd1b00238113b6fae8454f";
static readonly maxWireBytes:i32=574;
encodeChecked(value:Ros1Abb_rapid_msgsWObjData):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_msgsWObjData):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_msgsWObjData>{const out=new VendorDecoded<Ros1Abb_rapid_msgsWObjData>();if(bytes.length>574){out.status=90;return out;}if(bytes.length<62){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_msgsWObjData.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_msgsWObjData{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_msgsWObjData():changetype<Ros1Abb_rapid_msgsWObjData>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/EGMActivateSettings; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings {
static readonly maxWireBytes:i32=716;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=716;
/** Exact native field tool, native scalar abb_rapid_msgs/ToolData. */
tool:Ros1Abb_rapid_msgsToolData=new Ros1Abb_rapid_msgsToolData();
/** Exact native field wobj, native scalar abb_rapid_msgs/WObjData. */
wobj:Ros1Abb_rapid_msgsWObjData=new Ros1Abb_rapid_msgsWObjData();
/** Exact native field correction_frame, native scalar abb_rapid_msgs/Pose. */
correction_frame:Ros1Abb_rapid_msgsPose=new Ros1Abb_rapid_msgsPose();
/** Exact native field sensor_frame, native scalar abb_rapid_msgs/Pose. */
sensor_frame:Ros1Abb_rapid_msgsPose=new Ros1Abb_rapid_msgsPose();
/** Exact native field cond_min_max, native scalar float32. */
cond_min_max:f32=0;
/** Exact native field lp_filter, native scalar float32. */
lp_filter:f32=0;
/** Exact native field sample_rate, native scalar uint8. */
sample_rate:u8=0;
/** Exact native field max_speed_deviation, native scalar float32. */
max_speed_deviation:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.tool.wireSize();if(child<0)return child;if(child>716-n)return -90;n+=child;
if(n>716)return -90;
}
{
const child=this.wobj.wireSize();if(child<0)return child;if(child>716-n)return -90;n+=child;
if(n>716)return -90;
}
{
const child=this.correction_frame.wireSize();if(child<0)return child;if(child>716-n)return -90;n+=child;
if(n>716)return -90;
}
{
const child=this.sensor_frame.wireSize();if(child<0)return child;if(child>716-n)return -90;n+=child;
if(n>716)return -90;
}
{
if(!isFinite(this.cond_min_max))return -22;
if(4>716-n)return -90;n+=4;
if(n>716)return -90;
}
{
if(!isFinite(this.lp_filter))return -22;
if(4>716-n)return -90;n+=4;
if(n>716)return -90;
}
{
if(1>716-n)return -90;n+=1;
if(n>716)return -90;
}
{
if(!isFinite(this.max_speed_deviation))return -22;
if(4>716-n)return -90;n+=4;
if(n>716)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.tool.writeControl(w);
this.wobj.writeControl(w);
this.correction_frame.writeControl(w);
this.sensor_frame.writeControl(w);
w.integer(<u64>reinterpret<u32>(this.cond_min_max),4);
w.integer(<u64>reinterpret<u32>(this.lp_filter),4);
w.integer(<u64>this.sample_rate,1);
w.integer(<u64>reinterpret<u32>(this.max_speed_deviation),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings{const value=new Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings();
{
const decoded=Ros1Abb_rapid_msgsToolData.readControl(r);
value.tool=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsWObjData.readControl(r);
value.wobj=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPose.readControl(r);
value.correction_frame=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPose.readControl(r);
value.sensor_frame=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cond_min_max=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.lp_filter=decoded;
}
{
const decoded=<u8>r.integer(1);
value.sample_rate=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.max_speed_deviation=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsEGMActivateSettingsCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings>{
static readonly codecIdentity:string="UWVMROS1_c50be5682021e5d1a7a938e6fe3595cec7c0b0ae727070c0924da731c2c73d4d";
static readonly maxWireBytes:i32=716;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings>();if(bytes.length>716){out.status=90;return out;}if(bytes.length<204){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings():changetype<Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/EGMRunSettings; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsEGMRunSettings {
static readonly maxWireBytes:i32=40;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=40;
/** Exact native field cond_time, native scalar float32. */
cond_time:f32=0;
/** Exact native field ramp_in_time, native scalar float32. */
ramp_in_time:f32=0;
/** Exact native field offset, native scalar abb_rapid_msgs/Pose. */
offset:Ros1Abb_rapid_msgsPose=new Ros1Abb_rapid_msgsPose();
/** Exact native field pos_corr_gain, native scalar float32. */
pos_corr_gain:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.cond_time))return -22;
if(4>40-n)return -90;n+=4;
if(n>40)return -90;
}
{
if(!isFinite(this.ramp_in_time))return -22;
if(4>40-n)return -90;n+=4;
if(n>40)return -90;
}
{
const child=this.offset.wireSize();if(child<0)return child;if(child>40-n)return -90;n+=child;
if(n>40)return -90;
}
{
if(!isFinite(this.pos_corr_gain))return -22;
if(4>40-n)return -90;n+=4;
if(n>40)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.cond_time),4);
w.integer(<u64>reinterpret<u32>(this.ramp_in_time),4);
this.offset.writeControl(w);
w.integer(<u64>reinterpret<u32>(this.pos_corr_gain),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsEGMRunSettings{const value=new Ros1Abb_rapid_sm_addin_msgsEGMRunSettings();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cond_time=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ramp_in_time=decoded;
}
{
const decoded=Ros1Abb_rapid_msgsPose.readControl(r);
value.offset=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.pos_corr_gain=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsEGMRunSettingsCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsEGMRunSettings>{
static readonly codecIdentity:string="UWVMROS1_09a843da7aa89c60e6404f8d36b9a1a57f75e330329ac043449fd438b7029506";
static readonly maxWireBytes:i32=40;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsEGMRunSettings):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsEGMRunSettings):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsEGMRunSettings>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsEGMRunSettings>();if(bytes.length>40){out.status=90;return out;}if(bytes.length<40){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsEGMRunSettings.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsEGMRunSettings{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsEGMRunSettings():changetype<Ros1Abb_rapid_sm_addin_msgsEGMRunSettings>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/EGMSetupSettings; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings {
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field use_filtering, native scalar bool. */
use_filtering:bool=false;
/** Exact native field comm_timeout, native scalar float32. */
comm_timeout:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
{
if(!isFinite(this.comm_timeout))return -22;
if(4>5-n)return -90;n+=4;
if(n>5)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.use_filtering?1:0);
w.integer(<u64>reinterpret<u32>(this.comm_timeout),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings{const value=new Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_filtering=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.comm_timeout=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsEGMSetupSettingsCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings>{
static readonly codecIdentity:string="UWVMROS1_ece31a7631bb3bb248aece52d4d5f6e190a94dcc3052b2761b759bed24513779";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings():changetype<Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/EGMStopSettings; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsEGMStopSettings {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field ramp_out_time, native scalar float32. */
ramp_out_time:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.ramp_out_time))return -22;
if(4>4-n)return -90;n+=4;
if(n>4)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.ramp_out_time),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsEGMStopSettings{const value=new Ros1Abb_rapid_sm_addin_msgsEGMStopSettings();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ramp_out_time=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsEGMStopSettingsCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsEGMStopSettings>{
static readonly codecIdentity:string="UWVMROS1_64009cc452b9b2b12b3501d150e8c88c9cc07c796df06775195fa17c2a103a08";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsEGMStopSettings):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsEGMStopSettings):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsEGMStopSettings>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsEGMStopSettings>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsEGMStopSettings.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsEGMStopSettings{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsEGMStopSettings():changetype<Ros1Abb_rapid_sm_addin_msgsEGMStopSettings>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/EGMSettings; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsEGMSettings {
static readonly maxWireBytes:i32=767;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=767;
/** Exact native field allow_egm_motions, native scalar bool. */
allow_egm_motions:bool=false;
/** Exact native field use_presync, native scalar bool. */
use_presync:bool=false;
/** Exact native field setup_uc, native scalar abb_rapid_sm_addin_msgs/EGMSetupSettings. */
setup_uc:Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings=new Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings();
/** Exact native field activate, native scalar abb_rapid_sm_addin_msgs/EGMActivateSettings. */
activate:Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings=new Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings();
/** Exact native field run, native scalar abb_rapid_sm_addin_msgs/EGMRunSettings. */
run:Ros1Abb_rapid_sm_addin_msgsEGMRunSettings=new Ros1Abb_rapid_sm_addin_msgsEGMRunSettings();
/** Exact native field stop, native scalar abb_rapid_sm_addin_msgs/EGMStopSettings. */
stop:Ros1Abb_rapid_sm_addin_msgsEGMStopSettings=new Ros1Abb_rapid_sm_addin_msgsEGMStopSettings();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>767-n)return -90;n+=1;
if(n>767)return -90;
}
{
if(1>767-n)return -90;n+=1;
if(n>767)return -90;
}
{
const child=this.setup_uc.wireSize();if(child<0)return child;if(child>767-n)return -90;n+=child;
if(n>767)return -90;
}
{
const child=this.activate.wireSize();if(child<0)return child;if(child>767-n)return -90;n+=child;
if(n>767)return -90;
}
{
const child=this.run.wireSize();if(child<0)return child;if(child>767-n)return -90;n+=child;
if(n>767)return -90;
}
{
const child=this.stop.wireSize();if(child<0)return child;if(child>767-n)return -90;n+=child;
if(n>767)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.allow_egm_motions?1:0);
w.u8(this.use_presync?1:0);
this.setup_uc.writeControl(w);
this.activate.writeControl(w);
this.run.writeControl(w);
this.stop.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsEGMSettings{const value=new Ros1Abb_rapid_sm_addin_msgsEGMSettings();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.allow_egm_motions=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_presync=decoded;
}
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMSetupSettings.readControl(r);
value.setup_uc=decoded;
}
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMActivateSettings.readControl(r);
value.activate=decoded;
}
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMRunSettings.readControl(r);
value.run=decoded;
}
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMStopSettings.readControl(r);
value.stop=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsEGMSettingsCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsEGMSettings>{
static readonly codecIdentity:string="UWVMROS1_ebc0ca24a54451f863964c74bf0257b5b25546d0d4b8a75ac75c265e1d62317b";
static readonly maxWireBytes:i32=767;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsEGMSettings):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsEGMSettings):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsEGMSettings>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsEGMSettings>();if(bytes.length>767){out.status=90;return out;}if(bytes.length<255){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsEGMSettings.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsEGMSettings{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsEGMSettings():changetype<Ros1Abb_rapid_sm_addin_msgsEGMSettings>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/GetEGMSettingsRequest; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest {
static readonly maxWireBytes:i32=68;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=68;
/** Exact native field task, native scalar string. */
task:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.task)||this.task.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.task);if(size>64)return -22;if(4+size>68-n)return -90;n+=4+size;
if(n>68)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.task);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest{const value=new Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.task=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequestCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest>{
static readonly codecIdentity:string="UWVMROS1_42cca857367e08720f989416d18d6a1f7d677f7e5bf49435b9735a9eb10a8f9c";
static readonly maxWireBytes:i32=68;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest>();if(bytes.length>68){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest():changetype<Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsRequest>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/GetEGMSettingsResponse; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse {
static readonly maxWireBytes:i32=1285;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1285;
/** Exact native field settings, native scalar abb_rapid_sm_addin_msgs/EGMSettings. */
settings:Ros1Abb_rapid_sm_addin_msgsEGMSettings=new Ros1Abb_rapid_sm_addin_msgsEGMSettings();
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.settings.wireSize();if(child<0)return child;if(child>1285-n)return -90;n+=child;
if(n>1285)return -90;
}
{
if(2>1285-n)return -90;n+=2;
if(n>1285)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>1285-n)return -90;n+=4+size;
if(n>1285)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.settings.writeControl(w);
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse{const value=new Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse();
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMSettings.readControl(r);
value.settings=decoded;
}
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponseCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse>{
static readonly codecIdentity:string="UWVMROS1_094fd8b0b992c26d708bbd09952d36a3921ad5e5e66f45edb9d19af5ec3472bc";
static readonly maxWireBytes:i32=1285;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse>();if(bytes.length>1285){out.status=90;return out;}if(bytes.length<261){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse():changetype<Ros1Abb_rapid_sm_addin_msgsGetEGMSettingsResponse>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/StateMachineState; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsStateMachineState {
static readonly SM_STATE_UNKNOWN:u8=1;
static readonly SM_STATE_IDLE:u8=2;
static readonly SM_STATE_INITIALIZE:u8=3;
static readonly SM_STATE_RUN_RAPID_ROUTINE:u8=4;
static readonly SM_STATE_RUN_EGM_ROUTINE:u8=5;
static readonly EGM_ACTION_UNKNOWN:u8=1;
static readonly EGM_ACTION_NONE:u8=2;
static readonly EGM_ACTION_RUN_JOINT:u8=3;
static readonly EGM_ACTION_RUN_POSE:u8=4;
static readonly EGM_ACTION_STOP:u8=5;
static readonly EGM_ACTION_START_STREAM:u8=6;
static readonly EGM_ACTION_STOP_STREAM:u8=7;
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field rapid_task, native scalar string. */
rapid_task:string="";
/** Exact native field sm_state, native scalar uint8. */
sm_state:u8=0;
/** Exact native field egm_action, native scalar uint8. */
egm_action:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.rapid_task)||this.rapid_task.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.rapid_task);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
{
if(1>518-n)return -90;n+=1;
if(n>518)return -90;
}
{
if(1>518-n)return -90;n+=1;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.rapid_task);
w.integer(<u64>this.sm_state,1);
w.integer(<u64>this.egm_action,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsStateMachineState{const value=new Ros1Abb_rapid_sm_addin_msgsStateMachineState();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.rapid_task=decoded;
}
{
const decoded=<u8>r.integer(1);
value.sm_state=decoded;
}
{
const decoded=<u8>r.integer(1);
value.egm_action=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsStateMachineStateCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsStateMachineState>{
static readonly codecIdentity:string="UWVMROS1_9ef8cc3a0438ad892acc1a2f485039ca49abce6fedf40cc5d9b2fe03dd9490bb";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsStateMachineState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsStateMachineState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsStateMachineState>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsStateMachineState>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsStateMachineState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsStateMachineState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsStateMachineState():changetype<Ros1Abb_rapid_sm_addin_msgsStateMachineState>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/RuntimeState; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsRuntimeState {
static readonly maxWireBytes:i32=8372;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8372;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field state_machines, native scalar abb_rapid_sm_addin_msgs/StateMachineState. */
state_machines:Array<Ros1Abb_rapid_sm_addin_msgsStateMachineState>=new Array<Ros1Abb_rapid_sm_addin_msgsStateMachineState>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>8372-n)return -90;n+=child;
if(n>8372)return -90;
}
{
if(this.state_machines.length>16)return -22;
if(4>8372-n)return -90;n+=4;
for(let i=0;i<this.state_machines.length;++i){
const child=this.state_machines[i].wireSize();if(child<0)return child;if(child>8372-n)return -90;n+=child;
if(n>8372)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.state_machines.length);
for(let i=0;i<this.state_machines.length;++i){
this.state_machines[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsRuntimeState{const value=new Ros1Abb_rapid_sm_addin_msgsRuntimeState();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.state_machines=new Array<Ros1Abb_rapid_sm_addin_msgsStateMachineState>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Abb_rapid_sm_addin_msgsStateMachineState.readControl(r);
value.state_machines.push(decoded);
}
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsRuntimeStateCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsRuntimeState>{
static readonly codecIdentity:string="UWVMROS1_50d7eab2eed3ce0b55160e85b44b4108d966290f6b8f126980ee452846f3660c";
static readonly maxWireBytes:i32=8372;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsRuntimeState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsRuntimeState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsRuntimeState>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsRuntimeState>();if(bytes.length>8372){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsRuntimeState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsRuntimeState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsRuntimeState():changetype<Ros1Abb_rapid_sm_addin_msgsRuntimeState>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/SetEGMSettingsRequest; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest {
static readonly maxWireBytes:i32=835;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=835;
/** Exact native field task, native scalar string. */
task:string="";
/** Exact native field settings, native scalar abb_rapid_sm_addin_msgs/EGMSettings. */
settings:Ros1Abb_rapid_sm_addin_msgsEGMSettings=new Ros1Abb_rapid_sm_addin_msgsEGMSettings();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.task)||this.task.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.task);if(size>64)return -22;if(4+size>835-n)return -90;n+=4+size;
if(n>835)return -90;
}
{
const child=this.settings.wireSize();if(child<0)return child;if(child>835-n)return -90;n+=child;
if(n>835)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.task);
this.settings.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest{const value=new Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.task=decoded;
}
{
const decoded=Ros1Abb_rapid_sm_addin_msgsEGMSettings.readControl(r);
value.settings=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequestCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest>{
static readonly codecIdentity:string="UWVMROS1_fe85c34f8dff2c0a547f89385d75771af13a03d997b880ab7be2eb84ea800155";
static readonly maxWireBytes:i32=835;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest>();if(bytes.length>835){out.status=90;return out;}if(bytes.length<259){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest():changetype<Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsRequest>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/SetEGMSettingsResponse; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse{const value=new Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponseCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse>{
static readonly codecIdentity:string="UWVMROS1_c0d4e594a51a6cdd245ad03b6e27bbdc298c5c9fbe26cc25fa3594deef9b2591";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse():changetype<Ros1Abb_rapid_sm_addin_msgsSetEGMSettingsResponse>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/SetRAPIDRoutineRequest; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest {
static readonly maxWireBytes:i32=584;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=584;
/** Exact native field task, native scalar string. */
task:string="";
/** Exact native field routine, native scalar string. */
routine:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.task)||this.task.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.task);if(size>64)return -22;if(4+size>584-n)return -90;n+=4+size;
if(n>584)return -90;
}
{
if(!validUnicode(this.routine)||this.routine.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.routine);if(size>512)return -22;if(4+size>584-n)return -90;n+=4+size;
if(n>584)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.task);
w.text(this.routine);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest{const value=new Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.task=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.routine=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequestCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest>{
static readonly codecIdentity:string="UWVMROS1_b16ba3e98ee1d45864532b01f914746f041e49463b9330ef6056e427d199a5a3";
static readonly maxWireBytes:i32=584;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest>();if(bytes.length>584){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest():changetype<Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineRequest>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/SetRAPIDRoutineResponse; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse{const value=new Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponseCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse>{
static readonly codecIdentity:string="UWVMROS1_44de2385f12a8612fa75ca7c3f97b07f22d04e920bdeca0f0bd7fed5322c680e";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse():changetype<Ros1Abb_rapid_sm_addin_msgsSetRAPIDRoutineResponse>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/SetSGCommandRequest; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest {
static readonly SG_COMMAND_UNKNOWN:u8=1;
static readonly SG_COMMAND_NONE:u8=2;
static readonly SG_COMMAND_INITIALIZE:u8=3;
static readonly SG_COMMAND_CALIBRATE:u8=4;
static readonly SG_COMMAND_MOVE_TO:u8=5;
static readonly SG_COMMAND_GRIP_IN:u8=6;
static readonly SG_COMMAND_GRIP_OUT:u8=7;
static readonly SG_COMMAND_BLOW_ON_1:u8=8;
static readonly SG_COMMAND_BLOW_ON_2:u8=9;
static readonly SG_COMMAND_BLOW_OFF_1:u8=10;
static readonly SG_COMMAND_BLOW_OFF_2:u8=11;
static readonly SG_COMMAND_VACUUM_ON_1:u8=12;
static readonly SG_COMMAND_VACUUM_ON_2:u8=13;
static readonly SG_COMMAND_VACUUM_OFF_1:u8=14;
static readonly SG_COMMAND_VACUUM_OFF_2:u8=15;
static readonly maxWireBytes:i32=73;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=73;
/** Exact native field task, native scalar string. */
task:string="";
/** Exact native field command, native scalar uint8. */
command:u8=0;
/** Exact native field target_position, native scalar float32. */
target_position:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.task)||this.task.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.task);if(size>64)return -22;if(4+size>73-n)return -90;n+=4+size;
if(n>73)return -90;
}
{
if(1>73-n)return -90;n+=1;
if(n>73)return -90;
}
{
if(!isFinite(this.target_position))return -22;
if(4>73-n)return -90;n+=4;
if(n>73)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.task);
w.integer(<u64>this.command,1);
w.integer(<u64>reinterpret<u32>(this.target_position),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest{const value=new Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.task=decoded;
}
{
const decoded=<u8>r.integer(1);
value.command=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.target_position=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequestCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest>{
static readonly codecIdentity:string="UWVMROS1_e39d7fee66c1efb9ea963216823f6e10bd632d78634d8c47f3283a1a1779f0c2";
static readonly maxWireBytes:i32=73;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest>();if(bytes.length>73){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest():changetype<Ros1Abb_rapid_sm_addin_msgsSetSGCommandRequest>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/SetSGCommandResponse; spellings are published in generation proof. */
export class Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse{const value=new Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponseCodec implements VendorCodec<Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse>{
static readonly codecIdentity:string="UWVMROS1_2f22b2098d86dd0764277247eb7f64dccecebf0cddce85654b01c1928eb9c192";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse>{const out=new VendorDecoded<Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse():changetype<Ros1Abb_rapid_sm_addin_msgsSetSGCommandResponse>(out.value);}
}
/** Complete original abb_robot_msgs/GetFileContentsRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetFileContentsRequest {
static readonly maxWireBytes:i32=516;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=516;
/** Exact native field filename, native scalar string. */
filename:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.filename)||this.filename.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.filename);if(size>512)return -22;if(4+size>516-n)return -90;n+=4+size;
if(n>516)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.filename);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetFileContentsRequest{const value=new Ros1Abb_robot_msgsGetFileContentsRequest();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.filename=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetFileContentsRequestCodec implements VendorCodec<Ros1Abb_robot_msgsGetFileContentsRequest>{
static readonly codecIdentity:string="UWVMROS1_75f5802b385cc1efb524fadea657fda1d5cc4298c5720222d33cd3dd39656f21";
static readonly maxWireBytes:i32=516;
encodeChecked(value:Ros1Abb_robot_msgsGetFileContentsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetFileContentsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetFileContentsRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetFileContentsRequest>();if(bytes.length>516){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetFileContentsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetFileContentsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetFileContentsRequest():changetype<Ros1Abb_robot_msgsGetFileContentsRequest>(out.value);}
}
/** Complete original abb_robot_msgs/GetFileContentsResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetFileContentsResponse {
static readonly maxWireBytes:i32=4618;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4618;
/** Exact native field contents, native scalar string. */
contents:string="";
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.contents)||this.contents.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.contents);if(size>4096)return -22;if(4+size>4618-n)return -90;n+=4+size;
if(n>4618)return -90;
}
{
if(2>4618-n)return -90;n+=2;
if(n>4618)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>4618-n)return -90;n+=4+size;
if(n>4618)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.contents);
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetFileContentsResponse{const value=new Ros1Abb_robot_msgsGetFileContentsResponse();
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.contents=decoded;
}
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetFileContentsResponseCodec implements VendorCodec<Ros1Abb_robot_msgsGetFileContentsResponse>{
static readonly codecIdentity:string="UWVMROS1_693b53ed168f81a7c93ac8083977c4f8bd6f4a3999d079f5b47029699ba68fdb";
static readonly maxWireBytes:i32=4618;
encodeChecked(value:Ros1Abb_robot_msgsGetFileContentsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetFileContentsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetFileContentsResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetFileContentsResponse>();if(bytes.length>4618){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetFileContentsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetFileContentsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetFileContentsResponse():changetype<Ros1Abb_robot_msgsGetFileContentsResponse>(out.value);}
}
/** Complete original abb_robot_msgs/GetIOSignalRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetIOSignalRequest {
static readonly maxWireBytes:i32=516;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=516;
/** Exact native field signal, native scalar string. */
signal:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.signal)||this.signal.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.signal);if(size>512)return -22;if(4+size>516-n)return -90;n+=4+size;
if(n>516)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.signal);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetIOSignalRequest{const value=new Ros1Abb_robot_msgsGetIOSignalRequest();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.signal=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetIOSignalRequestCodec implements VendorCodec<Ros1Abb_robot_msgsGetIOSignalRequest>{
static readonly codecIdentity:string="UWVMROS1_d98f7577756b476cac0dd325d45f333fcd2b735df7dce58fb660790b5a91f83b";
static readonly maxWireBytes:i32=516;
encodeChecked(value:Ros1Abb_robot_msgsGetIOSignalRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetIOSignalRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetIOSignalRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetIOSignalRequest>();if(bytes.length>516){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetIOSignalRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetIOSignalRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetIOSignalRequest():changetype<Ros1Abb_robot_msgsGetIOSignalRequest>(out.value);}
}
/** Complete original abb_robot_msgs/GetIOSignalResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetIOSignalResponse {
static readonly maxWireBytes:i32=1034;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1034;
/** Exact native field value, native scalar string. */
value:string="";
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.value)||this.value.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.value);if(size>512)return -22;if(4+size>1034-n)return -90;n+=4+size;
if(n>1034)return -90;
}
{
if(2>1034-n)return -90;n+=2;
if(n>1034)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>1034-n)return -90;n+=4+size;
if(n>1034)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.value);
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetIOSignalResponse{const value=new Ros1Abb_robot_msgsGetIOSignalResponse();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.value=decoded;
}
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetIOSignalResponseCodec implements VendorCodec<Ros1Abb_robot_msgsGetIOSignalResponse>{
static readonly codecIdentity:string="UWVMROS1_10a66146c4815b0d758ec81dde1c552695c97a8727ee995299018b23f129d79e";
static readonly maxWireBytes:i32=1034;
encodeChecked(value:Ros1Abb_robot_msgsGetIOSignalResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetIOSignalResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetIOSignalResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetIOSignalResponse>();if(bytes.length>1034){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetIOSignalResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetIOSignalResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetIOSignalResponse():changetype<Ros1Abb_robot_msgsGetIOSignalResponse>(out.value);}
}
/** Complete original abb_robot_msgs/RAPIDSymbolPath; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsRAPIDSymbolPath {
static readonly maxWireBytes:i32=204;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=204;
/** Exact native field task, native scalar string. */
task:string="";
/** Exact native field module, native scalar string. */
module:string="";
/** Exact native field symbol, native scalar string. */
symbol:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.task)||this.task.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.task);if(size>64)return -22;if(4+size>204-n)return -90;n+=4+size;
if(n>204)return -90;
}
{
if(!validUnicode(this.module)||this.module.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.module);if(size>64)return -22;if(4+size>204-n)return -90;n+=4+size;
if(n>204)return -90;
}
{
if(!validUnicode(this.symbol)||this.symbol.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.symbol);if(size>64)return -22;if(4+size>204-n)return -90;n+=4+size;
if(n>204)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.task);
w.text(this.module);
w.text(this.symbol);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsRAPIDSymbolPath{const value=new Ros1Abb_robot_msgsRAPIDSymbolPath();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.task=decoded;
}
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.module=decoded;
}
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.symbol=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsRAPIDSymbolPathCodec implements VendorCodec<Ros1Abb_robot_msgsRAPIDSymbolPath>{
static readonly codecIdentity:string="UWVMROS1_07308d9ce1e71a02f5cc3546cdbaaa0b24a06534a55aa3ee3354b498038faad2";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros1Abb_robot_msgsRAPIDSymbolPath):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsRAPIDSymbolPath):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsRAPIDSymbolPath>{const out=new VendorDecoded<Ros1Abb_robot_msgsRAPIDSymbolPath>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsRAPIDSymbolPath.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsRAPIDSymbolPath{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsRAPIDSymbolPath():changetype<Ros1Abb_robot_msgsRAPIDSymbolPath>(out.value);}
}
/** Complete original abb_robot_msgs/GetRAPIDBoolRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRAPIDBoolRequest {
static readonly maxWireBytes:i32=204;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=204;
/** Exact native field path, native scalar abb_robot_msgs/RAPIDSymbolPath. */
path:Ros1Abb_robot_msgsRAPIDSymbolPath=new Ros1Abb_robot_msgsRAPIDSymbolPath();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.path.wireSize();if(child<0)return child;if(child>204-n)return -90;n+=child;
if(n>204)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.path.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetRAPIDBoolRequest{const value=new Ros1Abb_robot_msgsGetRAPIDBoolRequest();
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetRAPIDBoolRequestCodec implements VendorCodec<Ros1Abb_robot_msgsGetRAPIDBoolRequest>{
static readonly codecIdentity:string="UWVMROS1_46e49c0d6796175d0c56460dd4c7d2557deaa8db4f1a3bec40b53636d3b44b96";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros1Abb_robot_msgsGetRAPIDBoolRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRAPIDBoolRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRAPIDBoolRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRAPIDBoolRequest>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRAPIDBoolRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRAPIDBoolRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRAPIDBoolRequest():changetype<Ros1Abb_robot_msgsGetRAPIDBoolRequest>(out.value);}
}
/** Complete original abb_robot_msgs/GetRAPIDBoolResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRAPIDBoolResponse {
static readonly maxWireBytes:i32=519;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=519;
/** Exact native field value, native scalar bool. */
value:bool=false;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>519-n)return -90;n+=1;
if(n>519)return -90;
}
{
if(2>519-n)return -90;n+=2;
if(n>519)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>519-n)return -90;n+=4+size;
if(n>519)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.value?1:0);
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetRAPIDBoolResponse{const value=new Ros1Abb_robot_msgsGetRAPIDBoolResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.value=decoded;
}
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetRAPIDBoolResponseCodec implements VendorCodec<Ros1Abb_robot_msgsGetRAPIDBoolResponse>{
static readonly codecIdentity:string="UWVMROS1_bfe21e95cb79f1d9bc1bc1dc2f61c744a2a7dc49e5a5ed07b6c8619344b97b3f";
static readonly maxWireBytes:i32=519;
encodeChecked(value:Ros1Abb_robot_msgsGetRAPIDBoolResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRAPIDBoolResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRAPIDBoolResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRAPIDBoolResponse>();if(bytes.length>519){out.status=90;return out;}if(bytes.length<7){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRAPIDBoolResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRAPIDBoolResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRAPIDBoolResponse():changetype<Ros1Abb_robot_msgsGetRAPIDBoolResponse>(out.value);}
}
/** Complete original abb_robot_msgs/GetRAPIDDnumRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRAPIDDnumRequest {
static readonly maxWireBytes:i32=204;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=204;
/** Exact native field path, native scalar abb_robot_msgs/RAPIDSymbolPath. */
path:Ros1Abb_robot_msgsRAPIDSymbolPath=new Ros1Abb_robot_msgsRAPIDSymbolPath();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.path.wireSize();if(child<0)return child;if(child>204-n)return -90;n+=child;
if(n>204)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.path.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetRAPIDDnumRequest{const value=new Ros1Abb_robot_msgsGetRAPIDDnumRequest();
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetRAPIDDnumRequestCodec implements VendorCodec<Ros1Abb_robot_msgsGetRAPIDDnumRequest>{
static readonly codecIdentity:string="UWVMROS1_9058d8774de0da657344a00d75a7f75878da2b0453bf25708bc8ed7f45c90c9d";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros1Abb_robot_msgsGetRAPIDDnumRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRAPIDDnumRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRAPIDDnumRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRAPIDDnumRequest>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRAPIDDnumRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRAPIDDnumRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRAPIDDnumRequest():changetype<Ros1Abb_robot_msgsGetRAPIDDnumRequest>(out.value);}
}
/** Complete original abb_robot_msgs/GetRAPIDDnumResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRAPIDDnumResponse {
static readonly maxWireBytes:i32=526;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=526;
/** Exact native field value, native scalar float64. */
value:f64=0;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.value))return -22;
if(8>526-n)return -90;n+=8;
if(n>526)return -90;
}
{
if(2>526-n)return -90;n+=2;
if(n>526)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>526-n)return -90;n+=4+size;
if(n>526)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.value),8);
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetRAPIDDnumResponse{const value=new Ros1Abb_robot_msgsGetRAPIDDnumResponse();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetRAPIDDnumResponseCodec implements VendorCodec<Ros1Abb_robot_msgsGetRAPIDDnumResponse>{
static readonly codecIdentity:string="UWVMROS1_9c209b4b12d733e33b76cc33ae93b6d561ce1eb7f8864429a56bd073317a01b3";
static readonly maxWireBytes:i32=526;
encodeChecked(value:Ros1Abb_robot_msgsGetRAPIDDnumResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRAPIDDnumResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRAPIDDnumResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRAPIDDnumResponse>();if(bytes.length>526){out.status=90;return out;}if(bytes.length<14){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRAPIDDnumResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRAPIDDnumResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRAPIDDnumResponse():changetype<Ros1Abb_robot_msgsGetRAPIDDnumResponse>(out.value);}
}
/** Complete original abb_robot_msgs/GetRAPIDNumRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRAPIDNumRequest {
static readonly maxWireBytes:i32=204;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=204;
/** Exact native field path, native scalar abb_robot_msgs/RAPIDSymbolPath. */
path:Ros1Abb_robot_msgsRAPIDSymbolPath=new Ros1Abb_robot_msgsRAPIDSymbolPath();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.path.wireSize();if(child<0)return child;if(child>204-n)return -90;n+=child;
if(n>204)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.path.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetRAPIDNumRequest{const value=new Ros1Abb_robot_msgsGetRAPIDNumRequest();
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetRAPIDNumRequestCodec implements VendorCodec<Ros1Abb_robot_msgsGetRAPIDNumRequest>{
static readonly codecIdentity:string="UWVMROS1_a316395c8a19b7c0a9c3a555e625fffc7d647c7b567f38c53b1c3735b50ee910";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros1Abb_robot_msgsGetRAPIDNumRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRAPIDNumRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRAPIDNumRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRAPIDNumRequest>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRAPIDNumRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRAPIDNumRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRAPIDNumRequest():changetype<Ros1Abb_robot_msgsGetRAPIDNumRequest>(out.value);}
}
/** Complete original abb_robot_msgs/GetRAPIDNumResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRAPIDNumResponse {
static readonly maxWireBytes:i32=522;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=522;
/** Exact native field value, native scalar float32. */
value:f32=0;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.value))return -22;
if(4>522-n)return -90;n+=4;
if(n>522)return -90;
}
{
if(2>522-n)return -90;n+=2;
if(n>522)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>522-n)return -90;n+=4+size;
if(n>522)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.value),4);
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetRAPIDNumResponse{const value=new Ros1Abb_robot_msgsGetRAPIDNumResponse();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetRAPIDNumResponseCodec implements VendorCodec<Ros1Abb_robot_msgsGetRAPIDNumResponse>{
static readonly codecIdentity:string="UWVMROS1_1573cba9a9046a6b304a49e3a77266fa0d6f615d2ea9a24692c25616970fd9b0";
static readonly maxWireBytes:i32=522;
encodeChecked(value:Ros1Abb_robot_msgsGetRAPIDNumResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRAPIDNumResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRAPIDNumResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRAPIDNumResponse>();if(bytes.length>522){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRAPIDNumResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRAPIDNumResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRAPIDNumResponse():changetype<Ros1Abb_robot_msgsGetRAPIDNumResponse>(out.value);}
}
/** Complete original abb_robot_msgs/GetRAPIDStringRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRAPIDStringRequest {
static readonly maxWireBytes:i32=204;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=204;
/** Exact native field path, native scalar abb_robot_msgs/RAPIDSymbolPath. */
path:Ros1Abb_robot_msgsRAPIDSymbolPath=new Ros1Abb_robot_msgsRAPIDSymbolPath();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.path.wireSize();if(child<0)return child;if(child>204-n)return -90;n+=child;
if(n>204)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.path.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetRAPIDStringRequest{const value=new Ros1Abb_robot_msgsGetRAPIDStringRequest();
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetRAPIDStringRequestCodec implements VendorCodec<Ros1Abb_robot_msgsGetRAPIDStringRequest>{
static readonly codecIdentity:string="UWVMROS1_56514142e9e064aa43d09078b49ed8ee0366189727c35a9bc130b35007da4c9c";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros1Abb_robot_msgsGetRAPIDStringRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRAPIDStringRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRAPIDStringRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRAPIDStringRequest>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRAPIDStringRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRAPIDStringRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRAPIDStringRequest():changetype<Ros1Abb_robot_msgsGetRAPIDStringRequest>(out.value);}
}
/** Complete original abb_robot_msgs/GetRAPIDStringResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRAPIDStringResponse {
static readonly maxWireBytes:i32=1034;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1034;
/** Exact native field value, native scalar string. */
value:string="";
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.value)||this.value.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.value);if(size>512)return -22;if(4+size>1034-n)return -90;n+=4+size;
if(n>1034)return -90;
}
{
if(2>1034-n)return -90;n+=2;
if(n>1034)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>1034-n)return -90;n+=4+size;
if(n>1034)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.value);
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetRAPIDStringResponse{const value=new Ros1Abb_robot_msgsGetRAPIDStringResponse();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.value=decoded;
}
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetRAPIDStringResponseCodec implements VendorCodec<Ros1Abb_robot_msgsGetRAPIDStringResponse>{
static readonly codecIdentity:string="UWVMROS1_d1188827af3c307a721f8128b6f14fa6020f3187afce8a4162ce6562cdd1c854";
static readonly maxWireBytes:i32=1034;
encodeChecked(value:Ros1Abb_robot_msgsGetRAPIDStringResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRAPIDStringResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRAPIDStringResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRAPIDStringResponse>();if(bytes.length>1034){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRAPIDStringResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRAPIDStringResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRAPIDStringResponse():changetype<Ros1Abb_robot_msgsGetRAPIDStringResponse>(out.value);}
}
/** Complete original abb_robot_msgs/GetRAPIDSymbolRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRAPIDSymbolRequest {
static readonly maxWireBytes:i32=204;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=204;
/** Exact native field path, native scalar abb_robot_msgs/RAPIDSymbolPath. */
path:Ros1Abb_robot_msgsRAPIDSymbolPath=new Ros1Abb_robot_msgsRAPIDSymbolPath();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.path.wireSize();if(child<0)return child;if(child>204-n)return -90;n+=child;
if(n>204)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.path.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetRAPIDSymbolRequest{const value=new Ros1Abb_robot_msgsGetRAPIDSymbolRequest();
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetRAPIDSymbolRequestCodec implements VendorCodec<Ros1Abb_robot_msgsGetRAPIDSymbolRequest>{
static readonly codecIdentity:string="UWVMROS1_31302ad912a8ff39b56a429bc9dae22cbfc2c2101b8cd05162d00d9206aecf5b";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros1Abb_robot_msgsGetRAPIDSymbolRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRAPIDSymbolRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRAPIDSymbolRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRAPIDSymbolRequest>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRAPIDSymbolRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRAPIDSymbolRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRAPIDSymbolRequest():changetype<Ros1Abb_robot_msgsGetRAPIDSymbolRequest>(out.value);}
}
/** Complete original abb_robot_msgs/GetRAPIDSymbolResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRAPIDSymbolResponse {
static readonly maxWireBytes:i32=1034;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1034;
/** Exact native field value, native scalar string. */
value:string="";
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.value)||this.value.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.value);if(size>512)return -22;if(4+size>1034-n)return -90;n+=4+size;
if(n>1034)return -90;
}
{
if(2>1034-n)return -90;n+=2;
if(n>1034)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>1034-n)return -90;n+=4+size;
if(n>1034)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.value);
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetRAPIDSymbolResponse{const value=new Ros1Abb_robot_msgsGetRAPIDSymbolResponse();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.value=decoded;
}
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetRAPIDSymbolResponseCodec implements VendorCodec<Ros1Abb_robot_msgsGetRAPIDSymbolResponse>{
static readonly codecIdentity:string="UWVMROS1_182dc465ea5fb9543791b6fb76520313cee1ed472ab0296c8ef7a9ed3d4d110f";
static readonly maxWireBytes:i32=1034;
encodeChecked(value:Ros1Abb_robot_msgsGetRAPIDSymbolResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRAPIDSymbolResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRAPIDSymbolResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRAPIDSymbolResponse>();if(bytes.length>1034){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRAPIDSymbolResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRAPIDSymbolResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRAPIDSymbolResponse():changetype<Ros1Abb_robot_msgsGetRAPIDSymbolResponse>(out.value);}
}
/** Complete original abb_robot_msgs/GetRobotControllerDescriptionRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest {
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
static readControl(r:Reader):Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest{const value=new Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest();
return value;}
}
export class Ros1Abb_robot_msgsGetRobotControllerDescriptionRequestCodec implements VendorCodec<Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest>{
static readonly codecIdentity:string="UWVMROS1_bc5f87f4d0f33cbc640c8188fbb15c9b0f43b3067f2406700d6f5bd2475089b6";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest():changetype<Ros1Abb_robot_msgsGetRobotControllerDescriptionRequest>(out.value);}
}
/** Complete original abb_robot_msgs/GetRobotControllerDescriptionResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse {
static readonly maxWireBytes:i32=4618;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4618;
/** Exact native field description, native scalar string. */
description:string="";
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.description)||this.description.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.description);if(size>4096)return -22;if(4+size>4618-n)return -90;n+=4+size;
if(n>4618)return -90;
}
{
if(2>4618-n)return -90;n+=2;
if(n>4618)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>4618-n)return -90;n+=4+size;
if(n>4618)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.description);
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse{const value=new Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse();
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.description=decoded;
}
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetRobotControllerDescriptionResponseCodec implements VendorCodec<Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse>{
static readonly codecIdentity:string="UWVMROS1_7db41283fbe12a010e526b87d77a5bb7faecdf85595ef2dc13f14bfed56b6e39";
static readonly maxWireBytes:i32=4618;
encodeChecked(value:Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse>();if(bytes.length>4618){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse():changetype<Ros1Abb_robot_msgsGetRobotControllerDescriptionResponse>(out.value);}
}
/** Complete original abb_robot_msgs/GetSpeedRatioRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetSpeedRatioRequest {
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
static readControl(r:Reader):Ros1Abb_robot_msgsGetSpeedRatioRequest{const value=new Ros1Abb_robot_msgsGetSpeedRatioRequest();
return value;}
}
export class Ros1Abb_robot_msgsGetSpeedRatioRequestCodec implements VendorCodec<Ros1Abb_robot_msgsGetSpeedRatioRequest>{
static readonly codecIdentity:string="UWVMROS1_82230aebf6fa923e09bcd413ecb3b4803a6f1cfd3e8334e0ce0c496b73008373";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Abb_robot_msgsGetSpeedRatioRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetSpeedRatioRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetSpeedRatioRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetSpeedRatioRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetSpeedRatioRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetSpeedRatioRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetSpeedRatioRequest():changetype<Ros1Abb_robot_msgsGetSpeedRatioRequest>(out.value);}
}
/** Complete original abb_robot_msgs/GetSpeedRatioResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsGetSpeedRatioResponse {
static readonly maxWireBytes:i32=519;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=519;
/** Exact native field speed_ratio, native scalar uint8. */
speed_ratio:u8=0;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>519-n)return -90;n+=1;
if(n>519)return -90;
}
{
if(2>519-n)return -90;n+=2;
if(n>519)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>519-n)return -90;n+=4+size;
if(n>519)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.speed_ratio,1);
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsGetSpeedRatioResponse{const value=new Ros1Abb_robot_msgsGetSpeedRatioResponse();
{
const decoded=<u8>r.integer(1);
value.speed_ratio=decoded;
}
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsGetSpeedRatioResponseCodec implements VendorCodec<Ros1Abb_robot_msgsGetSpeedRatioResponse>{
static readonly codecIdentity:string="UWVMROS1_7784360ddafa1b8ad9626fde0afa2b2335f6a294507036c3aee953df99e91940";
static readonly maxWireBytes:i32=519;
encodeChecked(value:Ros1Abb_robot_msgsGetSpeedRatioResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsGetSpeedRatioResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsGetSpeedRatioResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsGetSpeedRatioResponse>();if(bytes.length>519){out.status=90;return out;}if(bytes.length<7){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsGetSpeedRatioResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsGetSpeedRatioResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsGetSpeedRatioResponse():changetype<Ros1Abb_robot_msgsGetSpeedRatioResponse>(out.value);}
}
/** Complete original abb_robot_msgs/MechanicalUnitState; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsMechanicalUnitState {
static readonly maxWireBytes:i32=69;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=69;
/** Exact native field name, native scalar string. */
name:string="";
/** Exact native field activated, native scalar bool. */
activated:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.name)||this.name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name);if(size>64)return -22;if(4+size>69-n)return -90;n+=4+size;
if(n>69)return -90;
}
{
if(1>69-n)return -90;n+=1;
if(n>69)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.name);
w.u8(this.activated?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsMechanicalUnitState{const value=new Ros1Abb_robot_msgsMechanicalUnitState();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.name=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.activated=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsMechanicalUnitStateCodec implements VendorCodec<Ros1Abb_robot_msgsMechanicalUnitState>{
static readonly codecIdentity:string="UWVMROS1_872cc6e8bdf32426bb9e24322c951452aab6ebcbc1f2998f4fb19c34abece8fa";
static readonly maxWireBytes:i32=69;
encodeChecked(value:Ros1Abb_robot_msgsMechanicalUnitState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsMechanicalUnitState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsMechanicalUnitState>{const out=new VendorDecoded<Ros1Abb_robot_msgsMechanicalUnitState>();if(bytes.length>69){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsMechanicalUnitState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsMechanicalUnitState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsMechanicalUnitState():changetype<Ros1Abb_robot_msgsMechanicalUnitState>(out.value);}
}
/** Complete original abb_robot_msgs/RAPIDTaskState; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsRAPIDTaskState {
static readonly EXECUTION_STATE_UNKNOWN:u8=1;
static readonly EXECUTION_STATE_READY:u8=2;
static readonly EXECUTION_STATE_STOPPED:u8=3;
static readonly EXECUTION_STATE_STARTED:u8=4;
static readonly EXECUTION_STATE_UNINITIALIZED:u8=5;
static readonly maxWireBytes:i32=71;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=71;
/** Exact native field name, native scalar string. */
name:string="";
/** Exact native field activated, native scalar bool. */
activated:bool=false;
/** Exact native field execution_state, native scalar uint8. */
execution_state:u8=0;
/** Exact native field motion_task, native scalar bool. */
motion_task:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.name)||this.name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name);if(size>64)return -22;if(4+size>71-n)return -90;n+=4+size;
if(n>71)return -90;
}
{
if(1>71-n)return -90;n+=1;
if(n>71)return -90;
}
{
if(1>71-n)return -90;n+=1;
if(n>71)return -90;
}
{
if(1>71-n)return -90;n+=1;
if(n>71)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.name);
w.u8(this.activated?1:0);
w.integer(<u64>this.execution_state,1);
w.u8(this.motion_task?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsRAPIDTaskState{const value=new Ros1Abb_robot_msgsRAPIDTaskState();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.name=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.activated=decoded;
}
{
const decoded=<u8>r.integer(1);
value.execution_state=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.motion_task=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsRAPIDTaskStateCodec implements VendorCodec<Ros1Abb_robot_msgsRAPIDTaskState>{
static readonly codecIdentity:string="UWVMROS1_ab1245cc352efa91cdcc8de801392857e25a4101a141439554b761525b31b947";
static readonly maxWireBytes:i32=71;
encodeChecked(value:Ros1Abb_robot_msgsRAPIDTaskState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsRAPIDTaskState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsRAPIDTaskState>{const out=new VendorDecoded<Ros1Abb_robot_msgsRAPIDTaskState>();if(bytes.length>71){out.status=90;return out;}if(bytes.length<7){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsRAPIDTaskState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsRAPIDTaskState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsRAPIDTaskState():changetype<Ros1Abb_robot_msgsRAPIDTaskState>(out.value);}
}
/** Complete original abb_robot_msgs/ServiceResponses; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsServiceResponses {
static readonly SUCCESS:string="";
static readonly RC_SUCCESS:u16=1;
static readonly FAILED:string="Service failed (e.g. timeout or resource not found, see the DEBUG log for details)";
static readonly RC_FAILED:u16=2;
static readonly SERVER_IS_BUSY:string="Server is busy";
static readonly RC_SERVER_IS_BUSY:u16=1001;
static readonly EMPTY_FILENAME:string="Filename is empty";
static readonly RC_EMPTY_FILENAME:u16=2001;
static readonly EMPTY_SIGNAL_NAME:string="Signal name is empty";
static readonly RC_EMPTY_SIGNAL_NAME:u16=2002;
static readonly EMPTY_RAPID_TASK_NAME:string="RAPID task name is empty";
static readonly RC_EMPTY_RAPID_TASK_NAME:u16=2003;
static readonly EMPTY_RAPID_MODULE_NAME:string="RAPID module name is empty";
static readonly RC_EMPTY_RAPID_MODULE_NAME:u16=2004;
static readonly EMPTY_RAPID_SYMBOL_NAME:string="RAPID symbol name is empty";
static readonly RC_EMPTY_RAPID_SYMBOL_NAME:u16=2005;
static readonly NOT_IN_AUTO_MODE:string="Robot controller is not in AUTO mode";
static readonly RC_NOT_IN_AUTO_MODE:u16=3001;
static readonly MOTORS_ARE_OFF:string="Motors are off";
static readonly RC_MOTORS_ARE_OFF:u16=3002;
static readonly MOTORS_ARE_ON:string="Motors are on";
static readonly RC_MOTORS_ARE_ON:u16=3003;
static readonly RAPID_NOT_STOPPED:string="RAPID has not been stopped";
static readonly RC_RAPID_NOT_STOPPED:u16=3004;
static readonly RAPID_NOT_RUNNING:string="RAPID is not running";
static readonly RC_RAPID_NOT_RUNNING:u16=3005;
static readonly SM_RUNTIME_STATES_MISSING:string="No runtime states received for any StateMachine Add-In instance";
static readonly RC_SM_RUNTIME_STATES_MISSING:u16=4001;
static readonly SM_UNKNOWN_RAPID_TASK:string="RAPID task not found among known StateMachine Add-In instances";
static readonly RC_SM_UNKNOWN_RAPID_TASK:u16=4002;
static readonly SM_UNINITIALIZED:string="StateMachine Add-In instance has not been initialized";
static readonly RC_SM_UNINITIALIZED:u16=4003;
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
static readControl(r:Reader):Ros1Abb_robot_msgsServiceResponses{const value=new Ros1Abb_robot_msgsServiceResponses();
return value;}
}
export class Ros1Abb_robot_msgsServiceResponsesCodec implements VendorCodec<Ros1Abb_robot_msgsServiceResponses>{
static readonly codecIdentity:string="UWVMROS1_f0a3c4b160bf0ca52dbbb4e26d2c8a7d3c6a40ac70787ba59cf181945e4276d4";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Abb_robot_msgsServiceResponses):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsServiceResponses):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsServiceResponses>{const out=new VendorDecoded<Ros1Abb_robot_msgsServiceResponses>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsServiceResponses.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsServiceResponses{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsServiceResponses():changetype<Ros1Abb_robot_msgsServiceResponses>(out.value);}
}
/** Complete original abb_robot_msgs/SetFileContentsRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetFileContentsRequest {
static readonly maxWireBytes:i32=4616;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4616;
/** Exact native field filename, native scalar string. */
filename:string="";
/** Exact native field contents, native scalar string. */
contents:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.filename)||this.filename.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.filename);if(size>512)return -22;if(4+size>4616-n)return -90;n+=4+size;
if(n>4616)return -90;
}
{
if(!validUnicode(this.contents)||this.contents.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.contents);if(size>4096)return -22;if(4+size>4616-n)return -90;n+=4+size;
if(n>4616)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.filename);
w.text(this.contents);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetFileContentsRequest{const value=new Ros1Abb_robot_msgsSetFileContentsRequest();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.filename=decoded;
}
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.contents=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetFileContentsRequestCodec implements VendorCodec<Ros1Abb_robot_msgsSetFileContentsRequest>{
static readonly codecIdentity:string="UWVMROS1_f764448d9754e9ebc5e5fab64f0c8ebb01c534ec7b95751ed7d58e8ce0bd3012";
static readonly maxWireBytes:i32=4616;
encodeChecked(value:Ros1Abb_robot_msgsSetFileContentsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetFileContentsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetFileContentsRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetFileContentsRequest>();if(bytes.length>4616){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetFileContentsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetFileContentsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetFileContentsRequest():changetype<Ros1Abb_robot_msgsSetFileContentsRequest>(out.value);}
}
/** Complete original abb_robot_msgs/SetFileContentsResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetFileContentsResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetFileContentsResponse{const value=new Ros1Abb_robot_msgsSetFileContentsResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetFileContentsResponseCodec implements VendorCodec<Ros1Abb_robot_msgsSetFileContentsResponse>{
static readonly codecIdentity:string="UWVMROS1_3de2a08b8e158e0612898880d6d9c98f1995828eb53bd5639b47f730c6cbefd0";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_robot_msgsSetFileContentsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetFileContentsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetFileContentsResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetFileContentsResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetFileContentsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetFileContentsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetFileContentsResponse():changetype<Ros1Abb_robot_msgsSetFileContentsResponse>(out.value);}
}
/** Complete original abb_robot_msgs/SetIOSignalRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetIOSignalRequest {
static readonly maxWireBytes:i32=1032;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1032;
/** Exact native field signal, native scalar string. */
signal:string="";
/** Exact native field value, native scalar string. */
value:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.signal)||this.signal.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.signal);if(size>512)return -22;if(4+size>1032-n)return -90;n+=4+size;
if(n>1032)return -90;
}
{
if(!validUnicode(this.value)||this.value.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.value);if(size>512)return -22;if(4+size>1032-n)return -90;n+=4+size;
if(n>1032)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.signal);
w.text(this.value);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetIOSignalRequest{const value=new Ros1Abb_robot_msgsSetIOSignalRequest();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.signal=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.value=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetIOSignalRequestCodec implements VendorCodec<Ros1Abb_robot_msgsSetIOSignalRequest>{
static readonly codecIdentity:string="UWVMROS1_57a0197e2a5de7efd2fc91d945d699be24559baec6651179a1256f1d9d35bbc9";
static readonly maxWireBytes:i32=1032;
encodeChecked(value:Ros1Abb_robot_msgsSetIOSignalRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetIOSignalRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetIOSignalRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetIOSignalRequest>();if(bytes.length>1032){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetIOSignalRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetIOSignalRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetIOSignalRequest():changetype<Ros1Abb_robot_msgsSetIOSignalRequest>(out.value);}
}
/** Complete original abb_robot_msgs/SetIOSignalResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetIOSignalResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetIOSignalResponse{const value=new Ros1Abb_robot_msgsSetIOSignalResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetIOSignalResponseCodec implements VendorCodec<Ros1Abb_robot_msgsSetIOSignalResponse>{
static readonly codecIdentity:string="UWVMROS1_becc0f844ce6a7fabd8a683175edd8033ba5c943c2de152991755ee112b460e5";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_robot_msgsSetIOSignalResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetIOSignalResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetIOSignalResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetIOSignalResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetIOSignalResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetIOSignalResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetIOSignalResponse():changetype<Ros1Abb_robot_msgsSetIOSignalResponse>(out.value);}
}
/** Complete original abb_robot_msgs/SetRAPIDBoolRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetRAPIDBoolRequest {
static readonly maxWireBytes:i32=205;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=205;
/** Exact native field path, native scalar abb_robot_msgs/RAPIDSymbolPath. */
path:Ros1Abb_robot_msgsRAPIDSymbolPath=new Ros1Abb_robot_msgsRAPIDSymbolPath();
/** Exact native field value, native scalar bool. */
value:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.path.wireSize();if(child<0)return child;if(child>205-n)return -90;n+=child;
if(n>205)return -90;
}
{
if(1>205-n)return -90;n+=1;
if(n>205)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.path.writeControl(w);
w.u8(this.value?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetRAPIDBoolRequest{const value=new Ros1Abb_robot_msgsSetRAPIDBoolRequest();
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.value=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetRAPIDBoolRequestCodec implements VendorCodec<Ros1Abb_robot_msgsSetRAPIDBoolRequest>{
static readonly codecIdentity:string="UWVMROS1_d8e5420678fe5feeffa20ad9ab8431678ec346f764c94871c4496c62e9dd8e8d";
static readonly maxWireBytes:i32=205;
encodeChecked(value:Ros1Abb_robot_msgsSetRAPIDBoolRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetRAPIDBoolRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetRAPIDBoolRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetRAPIDBoolRequest>();if(bytes.length>205){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetRAPIDBoolRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetRAPIDBoolRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetRAPIDBoolRequest():changetype<Ros1Abb_robot_msgsSetRAPIDBoolRequest>(out.value);}
}
/** Complete original abb_robot_msgs/SetRAPIDBoolResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetRAPIDBoolResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetRAPIDBoolResponse{const value=new Ros1Abb_robot_msgsSetRAPIDBoolResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetRAPIDBoolResponseCodec implements VendorCodec<Ros1Abb_robot_msgsSetRAPIDBoolResponse>{
static readonly codecIdentity:string="UWVMROS1_da2ae1231cb0c1ec452fb2733d5c99dcae78ad0bfd10771241708931b271bf13";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_robot_msgsSetRAPIDBoolResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetRAPIDBoolResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetRAPIDBoolResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetRAPIDBoolResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetRAPIDBoolResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetRAPIDBoolResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetRAPIDBoolResponse():changetype<Ros1Abb_robot_msgsSetRAPIDBoolResponse>(out.value);}
}
/** Complete original abb_robot_msgs/SetRAPIDDnumRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetRAPIDDnumRequest {
static readonly maxWireBytes:i32=212;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=212;
/** Exact native field path, native scalar abb_robot_msgs/RAPIDSymbolPath. */
path:Ros1Abb_robot_msgsRAPIDSymbolPath=new Ros1Abb_robot_msgsRAPIDSymbolPath();
/** Exact native field value, native scalar float64. */
value:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.path.wireSize();if(child<0)return child;if(child>212-n)return -90;n+=child;
if(n>212)return -90;
}
{
if(!isFinite(this.value))return -22;
if(8>212-n)return -90;n+=8;
if(n>212)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.path.writeControl(w);
w.integer(<u64>reinterpret<u64>(this.value),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetRAPIDDnumRequest{const value=new Ros1Abb_robot_msgsSetRAPIDDnumRequest();
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetRAPIDDnumRequestCodec implements VendorCodec<Ros1Abb_robot_msgsSetRAPIDDnumRequest>{
static readonly codecIdentity:string="UWVMROS1_0f17e3068f86bca6effb99f3bc850234b0163028f372340c450fce31657b9cf9";
static readonly maxWireBytes:i32=212;
encodeChecked(value:Ros1Abb_robot_msgsSetRAPIDDnumRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetRAPIDDnumRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetRAPIDDnumRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetRAPIDDnumRequest>();if(bytes.length>212){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetRAPIDDnumRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetRAPIDDnumRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetRAPIDDnumRequest():changetype<Ros1Abb_robot_msgsSetRAPIDDnumRequest>(out.value);}
}
/** Complete original abb_robot_msgs/SetRAPIDDnumResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetRAPIDDnumResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetRAPIDDnumResponse{const value=new Ros1Abb_robot_msgsSetRAPIDDnumResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetRAPIDDnumResponseCodec implements VendorCodec<Ros1Abb_robot_msgsSetRAPIDDnumResponse>{
static readonly codecIdentity:string="UWVMROS1_1905aed9683e03494baa54627ddbaf4aed0b92357823c0c042f9b7c6a970c8ed";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_robot_msgsSetRAPIDDnumResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetRAPIDDnumResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetRAPIDDnumResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetRAPIDDnumResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetRAPIDDnumResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetRAPIDDnumResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetRAPIDDnumResponse():changetype<Ros1Abb_robot_msgsSetRAPIDDnumResponse>(out.value);}
}
/** Complete original abb_robot_msgs/SetRAPIDNumRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetRAPIDNumRequest {
static readonly maxWireBytes:i32=208;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=208;
/** Exact native field path, native scalar abb_robot_msgs/RAPIDSymbolPath. */
path:Ros1Abb_robot_msgsRAPIDSymbolPath=new Ros1Abb_robot_msgsRAPIDSymbolPath();
/** Exact native field value, native scalar float32. */
value:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.path.wireSize();if(child<0)return child;if(child>208-n)return -90;n+=child;
if(n>208)return -90;
}
{
if(!isFinite(this.value))return -22;
if(4>208-n)return -90;n+=4;
if(n>208)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.path.writeControl(w);
w.integer(<u64>reinterpret<u32>(this.value),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetRAPIDNumRequest{const value=new Ros1Abb_robot_msgsSetRAPIDNumRequest();
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetRAPIDNumRequestCodec implements VendorCodec<Ros1Abb_robot_msgsSetRAPIDNumRequest>{
static readonly codecIdentity:string="UWVMROS1_9982d02cb0b6ee5813f5fb38072d665226b7885aeb90517af881f4fc990f392f";
static readonly maxWireBytes:i32=208;
encodeChecked(value:Ros1Abb_robot_msgsSetRAPIDNumRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetRAPIDNumRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetRAPIDNumRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetRAPIDNumRequest>();if(bytes.length>208){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetRAPIDNumRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetRAPIDNumRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetRAPIDNumRequest():changetype<Ros1Abb_robot_msgsSetRAPIDNumRequest>(out.value);}
}
/** Complete original abb_robot_msgs/SetRAPIDNumResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetRAPIDNumResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetRAPIDNumResponse{const value=new Ros1Abb_robot_msgsSetRAPIDNumResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetRAPIDNumResponseCodec implements VendorCodec<Ros1Abb_robot_msgsSetRAPIDNumResponse>{
static readonly codecIdentity:string="UWVMROS1_af1181242b4afdb6d387d39c24d0bc343e872fb9ab21c0ec1e451785786a4e64";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_robot_msgsSetRAPIDNumResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetRAPIDNumResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetRAPIDNumResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetRAPIDNumResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetRAPIDNumResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetRAPIDNumResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetRAPIDNumResponse():changetype<Ros1Abb_robot_msgsSetRAPIDNumResponse>(out.value);}
}
/** Complete original abb_robot_msgs/SetRAPIDStringRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetRAPIDStringRequest {
static readonly maxWireBytes:i32=720;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=720;
/** Exact native field path, native scalar abb_robot_msgs/RAPIDSymbolPath. */
path:Ros1Abb_robot_msgsRAPIDSymbolPath=new Ros1Abb_robot_msgsRAPIDSymbolPath();
/** Exact native field value, native scalar string. */
value:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.path.wireSize();if(child<0)return child;if(child>720-n)return -90;n+=child;
if(n>720)return -90;
}
{
if(!validUnicode(this.value)||this.value.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.value);if(size>512)return -22;if(4+size>720-n)return -90;n+=4+size;
if(n>720)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.path.writeControl(w);
w.text(this.value);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetRAPIDStringRequest{const value=new Ros1Abb_robot_msgsSetRAPIDStringRequest();
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.value=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetRAPIDStringRequestCodec implements VendorCodec<Ros1Abb_robot_msgsSetRAPIDStringRequest>{
static readonly codecIdentity:string="UWVMROS1_e06839324f8abfa8780b683386f87bab1ec04f1e6463e63c0599f5c2f8a75f0c";
static readonly maxWireBytes:i32=720;
encodeChecked(value:Ros1Abb_robot_msgsSetRAPIDStringRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetRAPIDStringRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetRAPIDStringRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetRAPIDStringRequest>();if(bytes.length>720){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetRAPIDStringRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetRAPIDStringRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetRAPIDStringRequest():changetype<Ros1Abb_robot_msgsSetRAPIDStringRequest>(out.value);}
}
/** Complete original abb_robot_msgs/SetRAPIDStringResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetRAPIDStringResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetRAPIDStringResponse{const value=new Ros1Abb_robot_msgsSetRAPIDStringResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetRAPIDStringResponseCodec implements VendorCodec<Ros1Abb_robot_msgsSetRAPIDStringResponse>{
static readonly codecIdentity:string="UWVMROS1_e175bdff5a150722b8c50cf2e1c0f28a99289a0b148339efdee4b034ea3b96b4";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_robot_msgsSetRAPIDStringResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetRAPIDStringResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetRAPIDStringResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetRAPIDStringResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetRAPIDStringResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetRAPIDStringResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetRAPIDStringResponse():changetype<Ros1Abb_robot_msgsSetRAPIDStringResponse>(out.value);}
}
/** Complete original abb_robot_msgs/SetRAPIDSymbolRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetRAPIDSymbolRequest {
static readonly maxWireBytes:i32=720;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=720;
/** Exact native field path, native scalar abb_robot_msgs/RAPIDSymbolPath. */
path:Ros1Abb_robot_msgsRAPIDSymbolPath=new Ros1Abb_robot_msgsRAPIDSymbolPath();
/** Exact native field value, native scalar string. */
value:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.path.wireSize();if(child<0)return child;if(child>720-n)return -90;n+=child;
if(n>720)return -90;
}
{
if(!validUnicode(this.value)||this.value.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.value);if(size>512)return -22;if(4+size>720-n)return -90;n+=4+size;
if(n>720)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.path.writeControl(w);
w.text(this.value);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetRAPIDSymbolRequest{const value=new Ros1Abb_robot_msgsSetRAPIDSymbolRequest();
{
const decoded=Ros1Abb_robot_msgsRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.value=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetRAPIDSymbolRequestCodec implements VendorCodec<Ros1Abb_robot_msgsSetRAPIDSymbolRequest>{
static readonly codecIdentity:string="UWVMROS1_0654a5a8e5ec42f4feab54b086a008e02a74d44df65d2fcf60f007736e3fec7d";
static readonly maxWireBytes:i32=720;
encodeChecked(value:Ros1Abb_robot_msgsSetRAPIDSymbolRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetRAPIDSymbolRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetRAPIDSymbolRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetRAPIDSymbolRequest>();if(bytes.length>720){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetRAPIDSymbolRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetRAPIDSymbolRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetRAPIDSymbolRequest():changetype<Ros1Abb_robot_msgsSetRAPIDSymbolRequest>(out.value);}
}
/** Complete original abb_robot_msgs/SetRAPIDSymbolResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetRAPIDSymbolResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetRAPIDSymbolResponse{const value=new Ros1Abb_robot_msgsSetRAPIDSymbolResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetRAPIDSymbolResponseCodec implements VendorCodec<Ros1Abb_robot_msgsSetRAPIDSymbolResponse>{
static readonly codecIdentity:string="UWVMROS1_a4ca0a8821337d1f16ba1f2ad2b232aceaba419308ca03b27f43dac427a32846";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_robot_msgsSetRAPIDSymbolResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetRAPIDSymbolResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetRAPIDSymbolResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetRAPIDSymbolResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetRAPIDSymbolResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetRAPIDSymbolResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetRAPIDSymbolResponse():changetype<Ros1Abb_robot_msgsSetRAPIDSymbolResponse>(out.value);}
}
/** Complete original abb_robot_msgs/SetSpeedRatioRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetSpeedRatioRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field speed_ratio, native scalar uint8. */
speed_ratio:u8=0;
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
w.integer(<u64>this.speed_ratio,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetSpeedRatioRequest{const value=new Ros1Abb_robot_msgsSetSpeedRatioRequest();
{
const decoded=<u8>r.integer(1);
value.speed_ratio=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetSpeedRatioRequestCodec implements VendorCodec<Ros1Abb_robot_msgsSetSpeedRatioRequest>{
static readonly codecIdentity:string="UWVMROS1_a6ac368d243d488901b03d5e4303600d5f0eaf62150801103e5f5f9c0863fab5";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Abb_robot_msgsSetSpeedRatioRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetSpeedRatioRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetSpeedRatioRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetSpeedRatioRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetSpeedRatioRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetSpeedRatioRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetSpeedRatioRequest():changetype<Ros1Abb_robot_msgsSetSpeedRatioRequest>(out.value);}
}
/** Complete original abb_robot_msgs/SetSpeedRatioResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSetSpeedRatioResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSetSpeedRatioResponse{const value=new Ros1Abb_robot_msgsSetSpeedRatioResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsSetSpeedRatioResponseCodec implements VendorCodec<Ros1Abb_robot_msgsSetSpeedRatioResponse>{
static readonly codecIdentity:string="UWVMROS1_9775dc1029993d1dfdf5f3080e6ae4262177ba96cdc0fbe9851dc4fea16c70d8";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_robot_msgsSetSpeedRatioResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSetSpeedRatioResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSetSpeedRatioResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsSetSpeedRatioResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSetSpeedRatioResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSetSpeedRatioResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSetSpeedRatioResponse():changetype<Ros1Abb_robot_msgsSetSpeedRatioResponse>(out.value);}
}
/** Complete original abb_robot_msgs/SystemState; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsSystemState {
static readonly maxWireBytes:i32=2331;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2331;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field motors_on, native scalar bool. */
motors_on:bool=false;
/** Exact native field auto_mode, native scalar bool. */
auto_mode:bool=false;
/** Exact native field rapid_running, native scalar bool. */
rapid_running:bool=false;
/** Exact native field rapid_tasks, native scalar abb_robot_msgs/RAPIDTaskState. */
rapid_tasks:Array<Ros1Abb_robot_msgsRAPIDTaskState>=new Array<Ros1Abb_robot_msgsRAPIDTaskState>();
/** Exact native field mechanical_units, native scalar abb_robot_msgs/MechanicalUnitState. */
mechanical_units:Array<Ros1Abb_robot_msgsMechanicalUnitState>=new Array<Ros1Abb_robot_msgsMechanicalUnitState>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>2331-n)return -90;n+=child;
if(n>2331)return -90;
}
{
if(1>2331-n)return -90;n+=1;
if(n>2331)return -90;
}
{
if(1>2331-n)return -90;n+=1;
if(n>2331)return -90;
}
{
if(1>2331-n)return -90;n+=1;
if(n>2331)return -90;
}
{
if(this.rapid_tasks.length>16)return -22;
if(4>2331-n)return -90;n+=4;
for(let i=0;i<this.rapid_tasks.length;++i){
const child=this.rapid_tasks[i].wireSize();if(child<0)return child;if(child>2331-n)return -90;n+=child;
if(n>2331)return -90;
}
}
{
if(this.mechanical_units.length>16)return -22;
if(4>2331-n)return -90;n+=4;
for(let i=0;i<this.mechanical_units.length;++i){
const child=this.mechanical_units[i].wireSize();if(child<0)return child;if(child>2331-n)return -90;n+=child;
if(n>2331)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u8(this.motors_on?1:0);
w.u8(this.auto_mode?1:0);
w.u8(this.rapid_running?1:0);
w.u32(<u32>this.rapid_tasks.length);
for(let i=0;i<this.rapid_tasks.length;++i){
this.rapid_tasks[i].writeControl(w);
}
w.u32(<u32>this.mechanical_units.length);
for(let i=0;i<this.mechanical_units.length;++i){
this.mechanical_units[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsSystemState{const value=new Ros1Abb_robot_msgsSystemState();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.motors_on=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.auto_mode=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.rapid_running=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.rapid_tasks=new Array<Ros1Abb_robot_msgsRAPIDTaskState>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Abb_robot_msgsRAPIDTaskState.readControl(r);
value.rapid_tasks.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.mechanical_units=new Array<Ros1Abb_robot_msgsMechanicalUnitState>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Abb_robot_msgsMechanicalUnitState.readControl(r);
value.mechanical_units.push(decoded);
}
}
return value;}
}
export class Ros1Abb_robot_msgsSystemStateCodec implements VendorCodec<Ros1Abb_robot_msgsSystemState>{
static readonly codecIdentity:string="UWVMROS1_b57aa1fdf098aa78c51a96bd2978e02f97cbae67148c0028557d2a3e1e8630f2";
static readonly maxWireBytes:i32=2331;
encodeChecked(value:Ros1Abb_robot_msgsSystemState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsSystemState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsSystemState>{const out=new VendorDecoded<Ros1Abb_robot_msgsSystemState>();if(bytes.length>2331){out.status=90;return out;}if(bytes.length<27){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsSystemState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsSystemState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsSystemState():changetype<Ros1Abb_robot_msgsSystemState>(out.value);}
}
/** Complete original abb_robot_msgs/TriggerWithResultCodeRequest; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsTriggerWithResultCodeRequest {
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
static readControl(r:Reader):Ros1Abb_robot_msgsTriggerWithResultCodeRequest{const value=new Ros1Abb_robot_msgsTriggerWithResultCodeRequest();
return value;}
}
export class Ros1Abb_robot_msgsTriggerWithResultCodeRequestCodec implements VendorCodec<Ros1Abb_robot_msgsTriggerWithResultCodeRequest>{
static readonly codecIdentity:string="UWVMROS1_999f63504788e522f7d0108a895cf35cd101e9a50856e480ae9edabf791bcf94";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Abb_robot_msgsTriggerWithResultCodeRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsTriggerWithResultCodeRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsTriggerWithResultCodeRequest>{const out=new VendorDecoded<Ros1Abb_robot_msgsTriggerWithResultCodeRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsTriggerWithResultCodeRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsTriggerWithResultCodeRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsTriggerWithResultCodeRequest():changetype<Ros1Abb_robot_msgsTriggerWithResultCodeRequest>(out.value);}
}
/** Complete original abb_robot_msgs/TriggerWithResultCodeResponse; spellings are published in generation proof. */
export class Ros1Abb_robot_msgsTriggerWithResultCodeResponse {
static readonly maxWireBytes:i32=518;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=518;
/** Exact native field result_code, native scalar uint16. */
result_code:u16=0;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(2>518-n)return -90;n+=2;
if(n>518)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>518-n)return -90;n+=4+size;
if(n>518)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.result_code,2);
w.text(this.message);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Abb_robot_msgsTriggerWithResultCodeResponse{const value=new Ros1Abb_robot_msgsTriggerWithResultCodeResponse();
{
const decoded=<u16>r.integer(2);
value.result_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros1Abb_robot_msgsTriggerWithResultCodeResponseCodec implements VendorCodec<Ros1Abb_robot_msgsTriggerWithResultCodeResponse>{
static readonly codecIdentity:string="UWVMROS1_4ef5c987592e849766b82b7fd99a65b7a1e17605f3e3341c99803181887967ff";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros1Abb_robot_msgsTriggerWithResultCodeResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Abb_robot_msgsTriggerWithResultCodeResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Abb_robot_msgsTriggerWithResultCodeResponse>{const out=new VendorDecoded<Ros1Abb_robot_msgsTriggerWithResultCodeResponse>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Abb_robot_msgsTriggerWithResultCodeResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Abb_robot_msgsTriggerWithResultCodeResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Abb_robot_msgsTriggerWithResultCodeResponse():changetype<Ros1Abb_robot_msgsTriggerWithResultCodeResponse>(out.value);}
}
/** Complete original duration; spellings are published in generation proof. */
export class Ros1Duration {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field sec, native scalar int32. */
sec:i32=0;
/** Exact native field nsec, native scalar int32. */
nsec:i32=0;
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
if(this.nsec<0||this.nsec>=1000000000)return -22;
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.sec,4);
w.integer(<u64>this.nsec,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Duration{const value=new Ros1Duration();
{
const decoded=<i32>r.integer(4);
value.sec=decoded;
}
{
const decoded=<i32>r.integer(4);
value.nsec=decoded;
}
if(value.nsec<0||value.nsec>=1000000000)r.valid=false;
return value;}
}
export class Ros1DurationCodec implements VendorCodec<Ros1Duration>{
static readonly codecIdentity:string="UWVMROS1_8dbec6cdfef79495208abb358c5e8283ec8d9d471f984424afcda997d142cca8";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros1Duration):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Duration):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Duration>{const out=new VendorDecoded<Ros1Duration>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Duration.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Duration{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Duration():changetype<Ros1Duration>(out.value);}
}
/** Complete original trajectory_msgs/JointTrajectoryPoint; spellings are published in generation proof. */
export class Ros1Trajectory_msgsJointTrajectoryPoint {
static readonly maxWireBytes:i32=408;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=408;
/** Exact native field positions, native scalar float64. */
positions:Array<f64>=new Array<f64>();
/** Exact native field velocities, native scalar float64. */
velocities:Array<f64>=new Array<f64>();
/** Exact native field accelerations, native scalar float64. */
accelerations:Array<f64>=new Array<f64>();
/** Exact native field effort, native scalar float64. */
effort:Array<f64>=new Array<f64>();
/** Exact native field time_from_start, native scalar duration. */
time_from_start:Ros1Duration=new Ros1Duration();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.positions.length>12)return -22;
if(4>408-n)return -90;n+=4;
for(let i=0;i<this.positions.length;++i){
if(!isFinite(this.positions[i]))return -22;
if(8>408-n)return -90;n+=8;
if(n>408)return -90;
}
}
{
if(this.velocities.length>12)return -22;
if(4>408-n)return -90;n+=4;
for(let i=0;i<this.velocities.length;++i){
if(!isFinite(this.velocities[i]))return -22;
if(8>408-n)return -90;n+=8;
if(n>408)return -90;
}
}
{
if(this.accelerations.length>12)return -22;
if(4>408-n)return -90;n+=4;
for(let i=0;i<this.accelerations.length;++i){
if(!isFinite(this.accelerations[i]))return -22;
if(8>408-n)return -90;n+=8;
if(n>408)return -90;
}
}
{
if(this.effort.length>12)return -22;
if(4>408-n)return -90;n+=4;
for(let i=0;i<this.effort.length;++i){
if(!isFinite(this.effort[i]))return -22;
if(8>408-n)return -90;n+=8;
if(n>408)return -90;
}
}
{
const child=this.time_from_start.wireSize();if(child<0)return child;if(child>408-n)return -90;n+=child;
if(n>408)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.positions.length);
for(let i=0;i<this.positions.length;++i){
w.integer(<u64>reinterpret<u64>(this.positions[i]),8);
}
w.u32(<u32>this.velocities.length);
for(let i=0;i<this.velocities.length;++i){
w.integer(<u64>reinterpret<u64>(this.velocities[i]),8);
}
w.u32(<u32>this.accelerations.length);
for(let i=0;i<this.accelerations.length;++i){
w.integer(<u64>reinterpret<u64>(this.accelerations[i]),8);
}
w.u32(<u32>this.effort.length);
for(let i=0;i<this.effort.length;++i){
w.integer(<u64>reinterpret<u64>(this.effort[i]),8);
}
this.time_from_start.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Trajectory_msgsJointTrajectoryPoint{const value=new Ros1Trajectory_msgsJointTrajectoryPoint();
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.positions=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.positions.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.velocities=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.velocities.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.accelerations=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.accelerations.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.effort=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.effort.push(decoded);
}
}
{
const decoded=Ros1Duration.readControl(r);
value.time_from_start=decoded;
}
return value;}
}
export class Ros1Trajectory_msgsJointTrajectoryPointCodec implements VendorCodec<Ros1Trajectory_msgsJointTrajectoryPoint>{
static readonly codecIdentity:string="UWVMROS1_9773572fff8616235c4d7e23fccbe01f28dcfa62c7c80323a65c8c305feba3c5";
static readonly maxWireBytes:i32=408;
encodeChecked(value:Ros1Trajectory_msgsJointTrajectoryPoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Trajectory_msgsJointTrajectoryPoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Trajectory_msgsJointTrajectoryPoint>{const out=new VendorDecoded<Ros1Trajectory_msgsJointTrajectoryPoint>();if(bytes.length>408){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Trajectory_msgsJointTrajectoryPoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Trajectory_msgsJointTrajectoryPoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Trajectory_msgsJointTrajectoryPoint():changetype<Ros1Trajectory_msgsJointTrajectoryPoint>(out.value);}
}
/** Complete original trajectory_msgs/JointTrajectory; spellings are published in generation proof. */
export class Ros1Trajectory_msgsJointTrajectory {
static readonly maxWireBytes:i32=4168;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4168;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field points, native scalar trajectory_msgs/JointTrajectoryPoint. */
points:Array<Ros1Trajectory_msgsJointTrajectoryPoint>=new Array<Ros1Trajectory_msgsJointTrajectoryPoint>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>4168-n)return -90;n+=child;
if(n>4168)return -90;
}
{
if(this.joint_names.length>12)return -22;
if(4>4168-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>64)return -22;if(4+size>4168-n)return -90;n+=4+size;
if(n>4168)return -90;
}
}
{
if(this.points.length>8)return -22;
if(4>4168-n)return -90;n+=4;
for(let i=0;i<this.points.length;++i){
const child=this.points[i].wireSize();if(child<0)return child;if(child>4168-n)return -90;n+=child;
if(n>4168)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.joint_names.length);
for(let i=0;i<this.joint_names.length;++i){
w.text(this.joint_names[i]);
}
w.u32(<u32>this.points.length);
for(let i=0;i<this.points.length;++i){
this.points[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Trajectory_msgsJointTrajectory{const value=new Ros1Trajectory_msgsJointTrajectory();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.joint_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_names.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>8){r.valid=false;return value;}
value.points=new Array<Ros1Trajectory_msgsJointTrajectoryPoint>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Trajectory_msgsJointTrajectoryPoint.readControl(r);
value.points.push(decoded);
}
}
return value;}
}
export class Ros1Trajectory_msgsJointTrajectoryCodec implements VendorCodec<Ros1Trajectory_msgsJointTrajectory>{
static readonly codecIdentity:string="UWVMROS1_079e6c432f0a858d6bd47c87aaf353a8b0676706809e0f984705b1a688de282b";
static readonly maxWireBytes:i32=4168;
encodeChecked(value:Ros1Trajectory_msgsJointTrajectory):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Trajectory_msgsJointTrajectory):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Trajectory_msgsJointTrajectory>{const out=new VendorDecoded<Ros1Trajectory_msgsJointTrajectory>();if(bytes.length>4168){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Trajectory_msgsJointTrajectory.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Trajectory_msgsJointTrajectory{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Trajectory_msgsJointTrajectory():changetype<Ros1Trajectory_msgsJointTrajectory>(out.value);}
}
/** Complete original control_msgs/JointTolerance; spellings are published in generation proof. */
export class Ros1Control_msgsJointTolerance {
static readonly maxWireBytes:i32=92;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=92;
/** Exact native field name, native scalar string. */
name:string="";
/** Exact native field position, native scalar float64. */
position:f64=0;
/** Exact native field velocity, native scalar float64. */
velocity:f64=0;
/** Exact native field acceleration, native scalar float64. */
acceleration:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.name)||this.name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name);if(size>64)return -22;if(4+size>92-n)return -90;n+=4+size;
if(n>92)return -90;
}
{
if(!isFinite(this.position))return -22;
if(8>92-n)return -90;n+=8;
if(n>92)return -90;
}
{
if(!isFinite(this.velocity))return -22;
if(8>92-n)return -90;n+=8;
if(n>92)return -90;
}
{
if(!isFinite(this.acceleration))return -22;
if(8>92-n)return -90;n+=8;
if(n>92)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.name);
w.integer(<u64>reinterpret<u64>(this.position),8);
w.integer(<u64>reinterpret<u64>(this.velocity),8);
w.integer(<u64>reinterpret<u64>(this.acceleration),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Control_msgsJointTolerance{const value=new Ros1Control_msgsJointTolerance();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.name=decoded;
}
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
return value;}
}
export class Ros1Control_msgsJointToleranceCodec implements VendorCodec<Ros1Control_msgsJointTolerance>{
static readonly codecIdentity:string="UWVMROS1_c9f71f98726d4a6621dcc8db96a394a24941a79d9041c8ad7a07724ee64d3c88";
static readonly maxWireBytes:i32=92;
encodeChecked(value:Ros1Control_msgsJointTolerance):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Control_msgsJointTolerance):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Control_msgsJointTolerance>{const out=new VendorDecoded<Ros1Control_msgsJointTolerance>();if(bytes.length>92){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Control_msgsJointTolerance.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Control_msgsJointTolerance{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Control_msgsJointTolerance():changetype<Ros1Control_msgsJointTolerance>(out.value);}
}
/** Complete original control_msgs/FollowJointTrajectoryGoal; spellings are published in generation proof. */
export class Ros1Control_msgsFollowJointTrajectoryGoal {
static readonly maxWireBytes:i32=6392;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6392;
/** Exact native field trajectory, native scalar trajectory_msgs/JointTrajectory. */
trajectory:Ros1Trajectory_msgsJointTrajectory=new Ros1Trajectory_msgsJointTrajectory();
/** Exact native field path_tolerance, native scalar control_msgs/JointTolerance. */
path_tolerance:Array<Ros1Control_msgsJointTolerance>=new Array<Ros1Control_msgsJointTolerance>();
/** Exact native field goal_tolerance, native scalar control_msgs/JointTolerance. */
goal_tolerance:Array<Ros1Control_msgsJointTolerance>=new Array<Ros1Control_msgsJointTolerance>();
/** Exact native field goal_time_tolerance, native scalar duration. */
goal_time_tolerance:Ros1Duration=new Ros1Duration();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.trajectory.wireSize();if(child<0)return child;if(child>6392-n)return -90;n+=child;
if(n>6392)return -90;
}
{
if(this.path_tolerance.length>12)return -22;
if(4>6392-n)return -90;n+=4;
for(let i=0;i<this.path_tolerance.length;++i){
const child=this.path_tolerance[i].wireSize();if(child<0)return child;if(child>6392-n)return -90;n+=child;
if(n>6392)return -90;
}
}
{
if(this.goal_tolerance.length>12)return -22;
if(4>6392-n)return -90;n+=4;
for(let i=0;i<this.goal_tolerance.length;++i){
const child=this.goal_tolerance[i].wireSize();if(child<0)return child;if(child>6392-n)return -90;n+=child;
if(n>6392)return -90;
}
}
{
const child=this.goal_time_tolerance.wireSize();if(child<0)return child;if(child>6392-n)return -90;n+=child;
if(n>6392)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.trajectory.writeControl(w);
w.u32(<u32>this.path_tolerance.length);
for(let i=0;i<this.path_tolerance.length;++i){
this.path_tolerance[i].writeControl(w);
}
w.u32(<u32>this.goal_tolerance.length);
for(let i=0;i<this.goal_tolerance.length;++i){
this.goal_tolerance[i].writeControl(w);
}
this.goal_time_tolerance.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Control_msgsFollowJointTrajectoryGoal{const value=new Ros1Control_msgsFollowJointTrajectoryGoal();
{
const decoded=Ros1Trajectory_msgsJointTrajectory.readControl(r);
value.trajectory=decoded;
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.path_tolerance=new Array<Ros1Control_msgsJointTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Control_msgsJointTolerance.readControl(r);
value.path_tolerance.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.goal_tolerance=new Array<Ros1Control_msgsJointTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Control_msgsJointTolerance.readControl(r);
value.goal_tolerance.push(decoded);
}
}
{
const decoded=Ros1Duration.readControl(r);
value.goal_time_tolerance=decoded;
}
return value;}
}
export class Ros1Control_msgsFollowJointTrajectoryGoalCodec implements VendorCodec<Ros1Control_msgsFollowJointTrajectoryGoal>{
static readonly codecIdentity:string="UWVMROS1_e9fda6f9e732450f2cf1af046ceb67815e6e1a22105eda2fee38f8f470252d40";
static readonly maxWireBytes:i32=6392;
encodeChecked(value:Ros1Control_msgsFollowJointTrajectoryGoal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Control_msgsFollowJointTrajectoryGoal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Control_msgsFollowJointTrajectoryGoal>{const out=new VendorDecoded<Ros1Control_msgsFollowJointTrajectoryGoal>();if(bytes.length>6392){out.status=90;return out;}if(bytes.length<40){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Control_msgsFollowJointTrajectoryGoal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Control_msgsFollowJointTrajectoryGoal{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Control_msgsFollowJointTrajectoryGoal():changetype<Ros1Control_msgsFollowJointTrajectoryGoal>(out.value);}
}
/** Complete original control_msgs/FollowJointTrajectoryResult; spellings are published in generation proof. */
export class Ros1Control_msgsFollowJointTrajectoryResult {
static readonly SUCCESSFUL:i32=0;
static readonly INVALID_GOAL:i32=-1;
static readonly INVALID_JOINTS:i32=-2;
static readonly OLD_HEADER_TIMESTAMP:i32=-3;
static readonly PATH_TOLERANCE_VIOLATED:i32=-4;
static readonly GOAL_TOLERANCE_VIOLATED:i32=-5;
static readonly maxWireBytes:i32=4104;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4104;
/** Exact native field error_code, native scalar int32. */
error_code:i32=0;
/** Exact native field error_string, native scalar string. */
error_string:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>4104-n)return -90;n+=4;
if(n>4104)return -90;
}
{
if(!validUnicode(this.error_string)||this.error_string.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.error_string);if(size>4096)return -22;if(4+size>4104-n)return -90;n+=4+size;
if(n>4104)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.error_code,4);
w.text(this.error_string);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Control_msgsFollowJointTrajectoryResult{const value=new Ros1Control_msgsFollowJointTrajectoryResult();
{
const decoded=<i32>r.integer(4);
value.error_code=decoded;
}
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.error_string=decoded;
}
return value;}
}
export class Ros1Control_msgsFollowJointTrajectoryResultCodec implements VendorCodec<Ros1Control_msgsFollowJointTrajectoryResult>{
static readonly codecIdentity:string="UWVMROS1_71da832be83c08804668aa63576768bbe831f21f5f270cdc8d851b30d2fc4281";
static readonly maxWireBytes:i32=4104;
encodeChecked(value:Ros1Control_msgsFollowJointTrajectoryResult):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Control_msgsFollowJointTrajectoryResult):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Control_msgsFollowJointTrajectoryResult>{const out=new VendorDecoded<Ros1Control_msgsFollowJointTrajectoryResult>();if(bytes.length>4104){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Control_msgsFollowJointTrajectoryResult.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Control_msgsFollowJointTrajectoryResult{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Control_msgsFollowJointTrajectoryResult():changetype<Ros1Control_msgsFollowJointTrajectoryResult>(out.value);}
}
/** Complete original control_msgs/FollowJointTrajectoryFeedback; spellings are published in generation proof. */
export class Ros1Control_msgsFollowJointTrajectoryFeedback {
static readonly maxWireBytes:i32=2124;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2124;
/** Exact native field header, native scalar std_msgs/Header. */
header:Ros1Std_msgsHeader=new Ros1Std_msgsHeader();
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field desired, native scalar trajectory_msgs/JointTrajectoryPoint. */
desired:Ros1Trajectory_msgsJointTrajectoryPoint=new Ros1Trajectory_msgsJointTrajectoryPoint();
/** Exact native field actual, native scalar trajectory_msgs/JointTrajectoryPoint. */
actual:Ros1Trajectory_msgsJointTrajectoryPoint=new Ros1Trajectory_msgsJointTrajectoryPoint();
/** Exact native field error, native scalar trajectory_msgs/JointTrajectoryPoint. */
error:Ros1Trajectory_msgsJointTrajectoryPoint=new Ros1Trajectory_msgsJointTrajectoryPoint();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>2124-n)return -90;n+=child;
if(n>2124)return -90;
}
{
if(this.joint_names.length>12)return -22;
if(4>2124-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>64)return -22;if(4+size>2124-n)return -90;n+=4+size;
if(n>2124)return -90;
}
}
{
const child=this.desired.wireSize();if(child<0)return child;if(child>2124-n)return -90;n+=child;
if(n>2124)return -90;
}
{
const child=this.actual.wireSize();if(child<0)return child;if(child>2124-n)return -90;n+=child;
if(n>2124)return -90;
}
{
const child=this.error.wireSize();if(child<0)return child;if(child>2124-n)return -90;n+=child;
if(n>2124)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.joint_names.length);
for(let i=0;i<this.joint_names.length;++i){
w.text(this.joint_names[i]);
}
this.desired.writeControl(w);
this.actual.writeControl(w);
this.error.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Control_msgsFollowJointTrajectoryFeedback{const value=new Ros1Control_msgsFollowJointTrajectoryFeedback();
{
const decoded=Ros1Std_msgsHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.joint_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_names.push(decoded);
}
}
{
const decoded=Ros1Trajectory_msgsJointTrajectoryPoint.readControl(r);
value.desired=decoded;
}
{
const decoded=Ros1Trajectory_msgsJointTrajectoryPoint.readControl(r);
value.actual=decoded;
}
{
const decoded=Ros1Trajectory_msgsJointTrajectoryPoint.readControl(r);
value.error=decoded;
}
return value;}
}
export class Ros1Control_msgsFollowJointTrajectoryFeedbackCodec implements VendorCodec<Ros1Control_msgsFollowJointTrajectoryFeedback>{
static readonly codecIdentity:string="UWVMROS1_a1ad5a1f8a00b60f97d06209737993bd395d83d1a008705d65193b63933b62a8";
static readonly maxWireBytes:i32=2124;
encodeChecked(value:Ros1Control_msgsFollowJointTrajectoryFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Control_msgsFollowJointTrajectoryFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Control_msgsFollowJointTrajectoryFeedback>{const out=new VendorDecoded<Ros1Control_msgsFollowJointTrajectoryFeedback>();if(bytes.length>2124){out.status=90;return out;}if(bytes.length<92){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Control_msgsFollowJointTrajectoryFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Control_msgsFollowJointTrajectoryFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Control_msgsFollowJointTrajectoryFeedback():changetype<Ros1Control_msgsFollowJointTrajectoryFeedback>(out.value);}
}
/** Complete original sensor_msgs/JointState; spellings are published in generation proof. */
export class Ros1Sensor_msgsJointState {
static readonly maxWireBytes:i32=1200;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1200;
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
const child=this.header.wireSize();if(child<0)return child;if(child>1200-n)return -90;n+=child;
if(n>1200)return -90;
}
{
if(this.name.length>12)return -22;
if(4>1200-n)return -90;n+=4;
for(let i=0;i<this.name.length;++i){
if(!validUnicode(this.name[i])||this.name[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name[i]);if(size>64)return -22;if(4+size>1200-n)return -90;n+=4+size;
if(n>1200)return -90;
}
}
{
if(this.position.length>12)return -22;
if(4>1200-n)return -90;n+=4;
for(let i=0;i<this.position.length;++i){
if(!isFinite(this.position[i]))return -22;
if(8>1200-n)return -90;n+=8;
if(n>1200)return -90;
}
}
{
if(this.velocity.length>12)return -22;
if(4>1200-n)return -90;n+=4;
for(let i=0;i<this.velocity.length;++i){
if(!isFinite(this.velocity[i]))return -22;
if(8>1200-n)return -90;n+=8;
if(n>1200)return -90;
}
}
{
if(this.effort.length>12)return -22;
if(4>1200-n)return -90;n+=4;
for(let i=0;i<this.effort.length;++i){
if(!isFinite(this.effort[i]))return -22;
if(8>1200-n)return -90;n+=8;
if(n>1200)return -90;
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
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.name=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.name.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.position=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.position.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.velocity=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.velocity.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.effort=new Array<f64>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.effort.push(decoded);
}
}
if((value.name.length!=0&&value.position.length!=0&&value.name.length!=value.position.length)||(value.name.length!=0&&value.velocity.length!=0&&value.name.length!=value.velocity.length)||(value.name.length!=0&&value.effort.length!=0&&value.name.length!=value.effort.length)||(value.position.length!=0&&value.velocity.length!=0&&value.position.length!=value.velocity.length)||(value.position.length!=0&&value.effort.length!=0&&value.position.length!=value.effort.length)||(value.velocity.length!=0&&value.effort.length!=0&&value.velocity.length!=value.effort.length))r.valid=false;
return value;}
}
export class Ros1Sensor_msgsJointStateCodec implements VendorCodec<Ros1Sensor_msgsJointState>{
static readonly codecIdentity:string="UWVMROS1_8d1bb9c4903dd4b070a5ab157b3590815722d6b574d610c9b9fb5ead8be7a057";
static readonly maxWireBytes:i32=1200;
encodeChecked(value:Ros1Sensor_msgsJointState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Sensor_msgsJointState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Sensor_msgsJointState>{const out=new VendorDecoded<Ros1Sensor_msgsJointState>();if(bytes.length>1200){out.status=90;return out;}if(bytes.length<32){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Sensor_msgsJointState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Sensor_msgsJointState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Sensor_msgsJointState():changetype<Ros1Sensor_msgsJointState>(out.value);}
}
