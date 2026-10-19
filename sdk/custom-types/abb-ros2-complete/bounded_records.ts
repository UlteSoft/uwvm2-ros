/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original abb_egm_msgs/msg/EGMChannelState; spellings are published in generation proof. */
export class Ros2Abb_egm_msgsMsgEGMChannelState {
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
static readControl(r:Reader):Ros2Abb_egm_msgsMsgEGMChannelState{const value=new Ros2Abb_egm_msgsMsgEGMChannelState();
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
export class Ros2Abb_egm_msgsMsgEGMChannelStateCodec implements VendorCodec<Ros2Abb_egm_msgsMsgEGMChannelState>{
static readonly codecIdentity:string="UWVMROS1_fbd288b93963fccc92aa114570c0a4f6a1fdc5b473cb8e252922720f5d88b519";
static readonly maxWireBytes:i32=81;
encodeChecked(value:Ros2Abb_egm_msgsMsgEGMChannelState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_egm_msgsMsgEGMChannelState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_egm_msgsMsgEGMChannelState>{const out=new VendorDecoded<Ros2Abb_egm_msgsMsgEGMChannelState>();if(bytes.length>81){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_egm_msgsMsgEGMChannelState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_egm_msgsMsgEGMChannelState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_egm_msgsMsgEGMChannelState():changetype<Ros2Abb_egm_msgsMsgEGMChannelState>(out.value);}
}
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
/** Complete original abb_egm_msgs/msg/EGMState; spellings are published in generation proof. */
export class Ros2Abb_egm_msgsMsgEGMState {
static readonly maxWireBytes:i32=1376;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1376;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field egm_channels, native scalar abb_egm_msgs/msg/EGMChannelState. */
egm_channels:Array<Ros2Abb_egm_msgsMsgEGMChannelState>=new Array<Ros2Abb_egm_msgsMsgEGMChannelState>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>1376-n)return -90;n+=child;
if(n>1376)return -90;
}
{
if(this.egm_channels.length>16)return -22;
if(4>1376-n)return -90;n+=4;
for(let i=0;i<this.egm_channels.length;++i){
const child=this.egm_channels[i].wireSize();if(child<0)return child;if(child>1376-n)return -90;n+=child;
if(n>1376)return -90;
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
static readControl(r:Reader):Ros2Abb_egm_msgsMsgEGMState{const value=new Ros2Abb_egm_msgsMsgEGMState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.egm_channels=new Array<Ros2Abb_egm_msgsMsgEGMChannelState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Abb_egm_msgsMsgEGMChannelState.readControl(r);
value.egm_channels.push(decoded);
}
}
return value;}
}
export class Ros2Abb_egm_msgsMsgEGMStateCodec implements VendorCodec<Ros2Abb_egm_msgsMsgEGMState>{
static readonly codecIdentity:string="UWVMROS1_5b10efb13c3be7290c7e391d55679a174969887edd1b8baca682edac311a8e5d";
static readonly maxWireBytes:i32=1376;
encodeChecked(value:Ros2Abb_egm_msgsMsgEGMState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_egm_msgsMsgEGMState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_egm_msgsMsgEGMState>{const out=new VendorDecoded<Ros2Abb_egm_msgsMsgEGMState>();if(bytes.length>1376){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_egm_msgsMsgEGMState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_egm_msgsMsgEGMState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_egm_msgsMsgEGMState():changetype<Ros2Abb_egm_msgsMsgEGMState>(out.value);}
}
/** Complete original abb_rapid_msgs/msg/ConfData; spellings are published in generation proof. */
export class Ros2Abb_rapid_msgsMsgConfData {
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
static readControl(r:Reader):Ros2Abb_rapid_msgsMsgConfData{const value=new Ros2Abb_rapid_msgsMsgConfData();
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
export class Ros2Abb_rapid_msgsMsgConfDataCodec implements VendorCodec<Ros2Abb_rapid_msgsMsgConfData>{
static readonly codecIdentity:string="UWVMROS1_0c22cc29db62f5001ef20ea7b131de63099e31ac5f150d3e9f0bb62120d60571";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Abb_rapid_msgsMsgConfData):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_msgsMsgConfData):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_msgsMsgConfData>{const out=new VendorDecoded<Ros2Abb_rapid_msgsMsgConfData>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_msgsMsgConfData.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_msgsMsgConfData{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_msgsMsgConfData():changetype<Ros2Abb_rapid_msgsMsgConfData>(out.value);}
}
/** Complete original abb_rapid_msgs/msg/ExtJoint; spellings are published in generation proof. */
export class Ros2Abb_rapid_msgsMsgExtJoint {
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
static readControl(r:Reader):Ros2Abb_rapid_msgsMsgExtJoint{const value=new Ros2Abb_rapid_msgsMsgExtJoint();
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
export class Ros2Abb_rapid_msgsMsgExtJointCodec implements VendorCodec<Ros2Abb_rapid_msgsMsgExtJoint>{
static readonly codecIdentity:string="UWVMROS1_8b799c6bec9e4eab0900a633a93375a7792e49fde0bf302f6752115a001a4ae2";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Abb_rapid_msgsMsgExtJoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_msgsMsgExtJoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_msgsMsgExtJoint>{const out=new VendorDecoded<Ros2Abb_rapid_msgsMsgExtJoint>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_msgsMsgExtJoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_msgsMsgExtJoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_msgsMsgExtJoint():changetype<Ros2Abb_rapid_msgsMsgExtJoint>(out.value);}
}
/** Complete original abb_rapid_msgs/msg/RobJoint; spellings are published in generation proof. */
export class Ros2Abb_rapid_msgsMsgRobJoint {
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
static readControl(r:Reader):Ros2Abb_rapid_msgsMsgRobJoint{const value=new Ros2Abb_rapid_msgsMsgRobJoint();
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
export class Ros2Abb_rapid_msgsMsgRobJointCodec implements VendorCodec<Ros2Abb_rapid_msgsMsgRobJoint>{
static readonly codecIdentity:string="UWVMROS1_6d842aebc0a3b222a727063a41ef7e96f2b8c14ffbba5c8d6e140ba2890e69cd";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Abb_rapid_msgsMsgRobJoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_msgsMsgRobJoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_msgsMsgRobJoint>{const out=new VendorDecoded<Ros2Abb_rapid_msgsMsgRobJoint>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_msgsMsgRobJoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_msgsMsgRobJoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_msgsMsgRobJoint():changetype<Ros2Abb_rapid_msgsMsgRobJoint>(out.value);}
}
/** Complete original abb_rapid_msgs/msg/JointTarget; spellings are published in generation proof. */
export class Ros2Abb_rapid_msgsMsgJointTarget {
static readonly maxWireBytes:i32=48;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=48;
/** Exact native field robax, native scalar abb_rapid_msgs/msg/RobJoint. */
robax:Ros2Abb_rapid_msgsMsgRobJoint=new Ros2Abb_rapid_msgsMsgRobJoint();
/** Exact native field extax, native scalar abb_rapid_msgs/msg/ExtJoint. */
extax:Ros2Abb_rapid_msgsMsgExtJoint=new Ros2Abb_rapid_msgsMsgExtJoint();
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
static readControl(r:Reader):Ros2Abb_rapid_msgsMsgJointTarget{const value=new Ros2Abb_rapid_msgsMsgJointTarget();
{
const decoded=Ros2Abb_rapid_msgsMsgRobJoint.readControl(r);
value.robax=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgExtJoint.readControl(r);
value.extax=decoded;
}
return value;}
}
export class Ros2Abb_rapid_msgsMsgJointTargetCodec implements VendorCodec<Ros2Abb_rapid_msgsMsgJointTarget>{
static readonly codecIdentity:string="UWVMROS1_5949cf0121391646c1f04b54d8f7deaae0497f4920f7817739fc25bfddc4e540";
static readonly maxWireBytes:i32=48;
encodeChecked(value:Ros2Abb_rapid_msgsMsgJointTarget):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_msgsMsgJointTarget):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_msgsMsgJointTarget>{const out=new VendorDecoded<Ros2Abb_rapid_msgsMsgJointTarget>();if(bytes.length>48){out.status=90;return out;}if(bytes.length<48){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_msgsMsgJointTarget.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_msgsMsgJointTarget{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_msgsMsgJointTarget():changetype<Ros2Abb_rapid_msgsMsgJointTarget>(out.value);}
}
/** Complete original abb_rapid_msgs/msg/Pos; spellings are published in generation proof. */
export class Ros2Abb_rapid_msgsMsgPos {
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
static readControl(r:Reader):Ros2Abb_rapid_msgsMsgPos{const value=new Ros2Abb_rapid_msgsMsgPos();
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
export class Ros2Abb_rapid_msgsMsgPosCodec implements VendorCodec<Ros2Abb_rapid_msgsMsgPos>{
static readonly codecIdentity:string="UWVMROS1_31d490d6ead5a1a78295dc4e934f0b89e74e4732da2b57d28d297477bf4cbdd2";
static readonly maxWireBytes:i32=12;
encodeChecked(value:Ros2Abb_rapid_msgsMsgPos):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_msgsMsgPos):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_msgsMsgPos>{const out=new VendorDecoded<Ros2Abb_rapid_msgsMsgPos>();if(bytes.length>12){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_msgsMsgPos.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_msgsMsgPos{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_msgsMsgPos():changetype<Ros2Abb_rapid_msgsMsgPos>(out.value);}
}
/** Complete original abb_rapid_msgs/msg/Orient; spellings are published in generation proof. */
export class Ros2Abb_rapid_msgsMsgOrient {
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
static readControl(r:Reader):Ros2Abb_rapid_msgsMsgOrient{const value=new Ros2Abb_rapid_msgsMsgOrient();
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
export class Ros2Abb_rapid_msgsMsgOrientCodec implements VendorCodec<Ros2Abb_rapid_msgsMsgOrient>{
static readonly codecIdentity:string="UWVMROS1_821d887b88298422f9466aaf3926db580f878e964055fbd72226b7f76f2f536e";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Abb_rapid_msgsMsgOrient):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_msgsMsgOrient):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_msgsMsgOrient>{const out=new VendorDecoded<Ros2Abb_rapid_msgsMsgOrient>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_msgsMsgOrient.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_msgsMsgOrient{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_msgsMsgOrient():changetype<Ros2Abb_rapid_msgsMsgOrient>(out.value);}
}
/** Complete original abb_rapid_msgs/msg/LoadData; spellings are published in generation proof. */
export class Ros2Abb_rapid_msgsMsgLoadData {
static readonly maxWireBytes:i32=44;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=44;
/** Exact native field mass, native scalar float32. */
mass:f32=0;
/** Exact native field cog, native scalar abb_rapid_msgs/msg/Pos. */
cog:Ros2Abb_rapid_msgsMsgPos=new Ros2Abb_rapid_msgsMsgPos();
/** Exact native field aom, native scalar abb_rapid_msgs/msg/Orient. */
aom:Ros2Abb_rapid_msgsMsgOrient=new Ros2Abb_rapid_msgsMsgOrient();
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
static readControl(r:Reader):Ros2Abb_rapid_msgsMsgLoadData{const value=new Ros2Abb_rapid_msgsMsgLoadData();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.mass=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPos.readControl(r);
value.cog=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgOrient.readControl(r);
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
export class Ros2Abb_rapid_msgsMsgLoadDataCodec implements VendorCodec<Ros2Abb_rapid_msgsMsgLoadData>{
static readonly codecIdentity:string="UWVMROS1_f73e480a0e6746a4df8f4d62bc123d68d01d3dc48f0bc5f0eac51b859a0750f9";
static readonly maxWireBytes:i32=44;
encodeChecked(value:Ros2Abb_rapid_msgsMsgLoadData):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_msgsMsgLoadData):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_msgsMsgLoadData>{const out=new VendorDecoded<Ros2Abb_rapid_msgsMsgLoadData>();if(bytes.length>44){out.status=90;return out;}if(bytes.length<44){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_msgsMsgLoadData.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_msgsMsgLoadData{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_msgsMsgLoadData():changetype<Ros2Abb_rapid_msgsMsgLoadData>(out.value);}
}
/** Complete original abb_rapid_msgs/msg/Pose; spellings are published in generation proof. */
export class Ros2Abb_rapid_msgsMsgPose {
static readonly maxWireBytes:i32=28;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=28;
/** Exact native field trans, native scalar abb_rapid_msgs/msg/Pos. */
trans:Ros2Abb_rapid_msgsMsgPos=new Ros2Abb_rapid_msgsMsgPos();
/** Exact native field rot, native scalar abb_rapid_msgs/msg/Orient. */
rot:Ros2Abb_rapid_msgsMsgOrient=new Ros2Abb_rapid_msgsMsgOrient();
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
static readControl(r:Reader):Ros2Abb_rapid_msgsMsgPose{const value=new Ros2Abb_rapid_msgsMsgPose();
{
const decoded=Ros2Abb_rapid_msgsMsgPos.readControl(r);
value.trans=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgOrient.readControl(r);
value.rot=decoded;
}
return value;}
}
export class Ros2Abb_rapid_msgsMsgPoseCodec implements VendorCodec<Ros2Abb_rapid_msgsMsgPose>{
static readonly codecIdentity:string="UWVMROS1_8f1535fe21326706fd0b16314857162d36bca91ce8e8f6a8bec3864dafb9e2f2";
static readonly maxWireBytes:i32=28;
encodeChecked(value:Ros2Abb_rapid_msgsMsgPose):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_msgsMsgPose):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_msgsMsgPose>{const out=new VendorDecoded<Ros2Abb_rapid_msgsMsgPose>();if(bytes.length>28){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_msgsMsgPose.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_msgsMsgPose{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_msgsMsgPose():changetype<Ros2Abb_rapid_msgsMsgPose>(out.value);}
}
/** Complete original abb_rapid_msgs/msg/RobTarget; spellings are published in generation proof. */
export class Ros2Abb_rapid_msgsMsgRobTarget {
static readonly maxWireBytes:i32=68;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=68;
/** Exact native field trans, native scalar abb_rapid_msgs/msg/Pos. */
trans:Ros2Abb_rapid_msgsMsgPos=new Ros2Abb_rapid_msgsMsgPos();
/** Exact native field rot, native scalar abb_rapid_msgs/msg/Orient. */
rot:Ros2Abb_rapid_msgsMsgOrient=new Ros2Abb_rapid_msgsMsgOrient();
/** Exact native field robconf, native scalar abb_rapid_msgs/msg/ConfData. */
robconf:Ros2Abb_rapid_msgsMsgConfData=new Ros2Abb_rapid_msgsMsgConfData();
/** Exact native field extax, native scalar abb_rapid_msgs/msg/ExtJoint. */
extax:Ros2Abb_rapid_msgsMsgExtJoint=new Ros2Abb_rapid_msgsMsgExtJoint();
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
static readControl(r:Reader):Ros2Abb_rapid_msgsMsgRobTarget{const value=new Ros2Abb_rapid_msgsMsgRobTarget();
{
const decoded=Ros2Abb_rapid_msgsMsgPos.readControl(r);
value.trans=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgOrient.readControl(r);
value.rot=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgConfData.readControl(r);
value.robconf=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgExtJoint.readControl(r);
value.extax=decoded;
}
return value;}
}
export class Ros2Abb_rapid_msgsMsgRobTargetCodec implements VendorCodec<Ros2Abb_rapid_msgsMsgRobTarget>{
static readonly codecIdentity:string="UWVMROS1_8decb56e8faf0a6a954af974adfea5c500aea8392e6a778ae7394174323489d8";
static readonly maxWireBytes:i32=68;
encodeChecked(value:Ros2Abb_rapid_msgsMsgRobTarget):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_msgsMsgRobTarget):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_msgsMsgRobTarget>{const out=new VendorDecoded<Ros2Abb_rapid_msgsMsgRobTarget>();if(bytes.length>68){out.status=90;return out;}if(bytes.length<68){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_msgsMsgRobTarget.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_msgsMsgRobTarget{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_msgsMsgRobTarget():changetype<Ros2Abb_rapid_msgsMsgRobTarget>(out.value);}
}
/** Complete original abb_rapid_msgs/msg/ToolData; spellings are published in generation proof. */
export class Ros2Abb_rapid_msgsMsgToolData {
static readonly maxWireBytes:i32=73;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=73;
/** Exact native field robhold, native scalar bool. */
robhold:bool=false;
/** Exact native field tframe, native scalar abb_rapid_msgs/msg/Pose. */
tframe:Ros2Abb_rapid_msgsMsgPose=new Ros2Abb_rapid_msgsMsgPose();
/** Exact native field tload, native scalar abb_rapid_msgs/msg/LoadData. */
tload:Ros2Abb_rapid_msgsMsgLoadData=new Ros2Abb_rapid_msgsMsgLoadData();
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
static readControl(r:Reader):Ros2Abb_rapid_msgsMsgToolData{const value=new Ros2Abb_rapid_msgsMsgToolData();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.robhold=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPose.readControl(r);
value.tframe=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgLoadData.readControl(r);
value.tload=decoded;
}
return value;}
}
export class Ros2Abb_rapid_msgsMsgToolDataCodec implements VendorCodec<Ros2Abb_rapid_msgsMsgToolData>{
static readonly codecIdentity:string="UWVMROS1_948fa0bbaaf95f0d88ca03cad687f16102261f54b0da889ae8bca82042fc5237";
static readonly maxWireBytes:i32=73;
encodeChecked(value:Ros2Abb_rapid_msgsMsgToolData):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_msgsMsgToolData):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_msgsMsgToolData>{const out=new VendorDecoded<Ros2Abb_rapid_msgsMsgToolData>();if(bytes.length>73){out.status=90;return out;}if(bytes.length<73){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_msgsMsgToolData.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_msgsMsgToolData{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_msgsMsgToolData():changetype<Ros2Abb_rapid_msgsMsgToolData>(out.value);}
}
/** Complete original abb_rapid_msgs/msg/WObjData; spellings are published in generation proof. */
export class Ros2Abb_rapid_msgsMsgWObjData {
static readonly maxWireBytes:i32=574;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=574;
/** Exact native field robhold, native scalar bool. */
robhold:bool=false;
/** Exact native field ufprog, native scalar bool. */
ufprog:bool=false;
/** Exact native field ufmec, native scalar string. */
ufmec:string="";
/** Exact native field uframe, native scalar abb_rapid_msgs/msg/Pose. */
uframe:Ros2Abb_rapid_msgsMsgPose=new Ros2Abb_rapid_msgsMsgPose();
/** Exact native field oframe, native scalar abb_rapid_msgs/msg/Pose. */
oframe:Ros2Abb_rapid_msgsMsgPose=new Ros2Abb_rapid_msgsMsgPose();
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
static readControl(r:Reader):Ros2Abb_rapid_msgsMsgWObjData{const value=new Ros2Abb_rapid_msgsMsgWObjData();
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
const decoded=Ros2Abb_rapid_msgsMsgPose.readControl(r);
value.uframe=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPose.readControl(r);
value.oframe=decoded;
}
return value;}
}
export class Ros2Abb_rapid_msgsMsgWObjDataCodec implements VendorCodec<Ros2Abb_rapid_msgsMsgWObjData>{
static readonly codecIdentity:string="UWVMROS1_b36f0448512fab6028431d19b39a8ae31b7b81b2615ffc8311139f63b5d8e7cd";
static readonly maxWireBytes:i32=574;
encodeChecked(value:Ros2Abb_rapid_msgsMsgWObjData):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_msgsMsgWObjData):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_msgsMsgWObjData>{const out=new VendorDecoded<Ros2Abb_rapid_msgsMsgWObjData>();if(bytes.length>574){out.status=90;return out;}if(bytes.length<62){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_msgsMsgWObjData.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_msgsMsgWObjData{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_msgsMsgWObjData():changetype<Ros2Abb_rapid_msgsMsgWObjData>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/msg/EGMActivateSettings; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings {
static readonly maxWireBytes:i32=716;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=716;
/** Exact native field tool, native scalar abb_rapid_msgs/msg/ToolData. */
tool:Ros2Abb_rapid_msgsMsgToolData=new Ros2Abb_rapid_msgsMsgToolData();
/** Exact native field wobj, native scalar abb_rapid_msgs/msg/WObjData. */
wobj:Ros2Abb_rapid_msgsMsgWObjData=new Ros2Abb_rapid_msgsMsgWObjData();
/** Exact native field correction_frame, native scalar abb_rapid_msgs/msg/Pose. */
correction_frame:Ros2Abb_rapid_msgsMsgPose=new Ros2Abb_rapid_msgsMsgPose();
/** Exact native field sensor_frame, native scalar abb_rapid_msgs/msg/Pose. */
sensor_frame:Ros2Abb_rapid_msgsMsgPose=new Ros2Abb_rapid_msgsMsgPose();
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings{const value=new Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings();
{
const decoded=Ros2Abb_rapid_msgsMsgToolData.readControl(r);
value.tool=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgWObjData.readControl(r);
value.wobj=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPose.readControl(r);
value.correction_frame=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPose.readControl(r);
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
export class Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettingsCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings>{
static readonly codecIdentity:string="UWVMROS1_fce73aea80d9b8a299b8e2ab742602871146b3ebbd624a2e807e25c1ac107cfa";
static readonly maxWireBytes:i32=716;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings>();if(bytes.length>716){out.status=90;return out;}if(bytes.length<204){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings():changetype<Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/msg/EGMRunSettings; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings {
static readonly maxWireBytes:i32=40;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=40;
/** Exact native field cond_time, native scalar float32. */
cond_time:f32=0;
/** Exact native field ramp_in_time, native scalar float32. */
ramp_in_time:f32=0;
/** Exact native field offset, native scalar abb_rapid_msgs/msg/Pose. */
offset:Ros2Abb_rapid_msgsMsgPose=new Ros2Abb_rapid_msgsMsgPose();
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings{const value=new Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cond_time=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ramp_in_time=decoded;
}
{
const decoded=Ros2Abb_rapid_msgsMsgPose.readControl(r);
value.offset=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.pos_corr_gain=decoded;
}
return value;}
}
export class Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettingsCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings>{
static readonly codecIdentity:string="UWVMROS1_92a2a654eb2eda89077933f955f04ed87884827b8875cf563570eed5d211b0e2";
static readonly maxWireBytes:i32=40;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings>();if(bytes.length>40){out.status=90;return out;}if(bytes.length<40){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings():changetype<Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/msg/EGMSetupSettings; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings {
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings{const value=new Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings();
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
export class Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettingsCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings>{
static readonly codecIdentity:string="UWVMROS1_2565bed5b83b132c69f37d7294c69181800ad2974ddc54f60868da57e28e782c";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings():changetype<Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/msg/EGMStopSettings; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings {
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings{const value=new Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ramp_out_time=decoded;
}
return value;}
}
export class Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettingsCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings>{
static readonly codecIdentity:string="UWVMROS1_0f3f2f6034567a7d49dd140ff9ff620e8567548139597b9b823e63a947673cee";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings():changetype<Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/msg/EGMSettings; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings {
static readonly maxWireBytes:i32=767;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=767;
/** Exact native field allow_egm_motions, native scalar bool. */
allow_egm_motions:bool=false;
/** Exact native field use_presync, native scalar bool. */
use_presync:bool=false;
/** Exact native field setup_uc, native scalar abb_rapid_sm_addin_msgs/msg/EGMSetupSettings. */
setup_uc:Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings=new Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings();
/** Exact native field activate, native scalar abb_rapid_sm_addin_msgs/msg/EGMActivateSettings. */
activate:Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings=new Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings();
/** Exact native field run, native scalar abb_rapid_sm_addin_msgs/msg/EGMRunSettings. */
run:Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings=new Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings();
/** Exact native field stop, native scalar abb_rapid_sm_addin_msgs/msg/EGMStopSettings. */
stop:Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings=new Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings();
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings{const value=new Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.allow_egm_motions=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.use_presync=decoded;
}
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMSetupSettings.readControl(r);
value.setup_uc=decoded;
}
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMActivateSettings.readControl(r);
value.activate=decoded;
}
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMRunSettings.readControl(r);
value.run=decoded;
}
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMStopSettings.readControl(r);
value.stop=decoded;
}
return value;}
}
export class Ros2Abb_rapid_sm_addin_msgsMsgEGMSettingsCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings>{
static readonly codecIdentity:string="UWVMROS1_622d3a975eecb11f7729aecfadc161cada271208d6fe7599f455750b90144ef3";
static readonly maxWireBytes:i32=767;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings>();if(bytes.length>767){out.status=90;return out;}if(bytes.length<255){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings():changetype<Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/msg/StateMachineState; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState {
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState{const value=new Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState();
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
export class Ros2Abb_rapid_sm_addin_msgsMsgStateMachineStateCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState>{
static readonly codecIdentity:string="UWVMROS1_ff2a09e4b6e8ef26cbdc3ecdffdceb33d6b2c82f0d51ed2c38ce9648c6f47711";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState():changetype<Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/msg/RuntimeState; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState {
static readonly maxWireBytes:i32=8368;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8368;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field state_machines, native scalar abb_rapid_sm_addin_msgs/msg/StateMachineState. */
state_machines:Array<Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState>=new Array<Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>8368-n)return -90;n+=child;
if(n>8368)return -90;
}
{
if(this.state_machines.length>16)return -22;
if(4>8368-n)return -90;n+=4;
for(let i=0;i<this.state_machines.length;++i){
const child=this.state_machines[i].wireSize();if(child<0)return child;if(child>8368-n)return -90;n+=child;
if(n>8368)return -90;
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState{const value=new Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.state_machines=new Array<Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgStateMachineState.readControl(r);
value.state_machines.push(decoded);
}
}
return value;}
}
export class Ros2Abb_rapid_sm_addin_msgsMsgRuntimeStateCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState>{
static readonly codecIdentity:string="UWVMROS1_3c0f157544fa82c4e206a4fb42f8ba7f67213d19533f79b77c66dec6fd0460d9";
static readonly maxWireBytes:i32=8368;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState>();if(bytes.length>8368){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState():changetype<Ros2Abb_rapid_sm_addin_msgsMsgRuntimeState>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Request; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request {
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request{const value=new Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.task=decoded;
}
return value;}
}
export class Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_RequestCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request>{
static readonly codecIdentity:string="UWVMROS1_363be2459fcc9c475c6305cbcfebf7689e891eb27645b7667960aace23d56e8e";
static readonly maxWireBytes:i32=68;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request>();if(bytes.length>68){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request():changetype<Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Request>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Response; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response {
static readonly maxWireBytes:i32=1285;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1285;
/** Exact native field settings, native scalar abb_rapid_sm_addin_msgs/msg/EGMSettings. */
settings:Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings=new Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings();
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response{const value=new Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response();
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings.readControl(r);
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
export class Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_ResponseCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response>{
static readonly codecIdentity:string="UWVMROS1_d8f340e1c6d78fc0a782c3279efb34ae5e11df175a82f9dd5ee82e603d6b2ffb";
static readonly maxWireBytes:i32=1285;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response>();if(bytes.length>1285){out.status=90;return out;}if(bytes.length<261){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response():changetype<Ros2Abb_rapid_sm_addin_msgsSrvGetEGMSettings_Response>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Request; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request {
static readonly maxWireBytes:i32=835;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=835;
/** Exact native field task, native scalar string. */
task:string="";
/** Exact native field settings, native scalar abb_rapid_sm_addin_msgs/msg/EGMSettings. */
settings:Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings=new Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings();
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request{const value=new Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.task=decoded;
}
{
const decoded=Ros2Abb_rapid_sm_addin_msgsMsgEGMSettings.readControl(r);
value.settings=decoded;
}
return value;}
}
export class Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_RequestCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request>{
static readonly codecIdentity:string="UWVMROS1_c2f3dd535e9475526bf18bdffa0e9a839049d315c9aebb150f8561ee3361a2bd";
static readonly maxWireBytes:i32=835;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request>();if(bytes.length>835){out.status=90;return out;}if(bytes.length<259){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request():changetype<Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Request>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Response; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response {
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response{const value=new Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response();
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
export class Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_ResponseCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response>{
static readonly codecIdentity:string="UWVMROS1_6a1adeebe31dfe101a6d57019e26ee2f1144b9a6ce30aee08d4c6fe56ea6838d";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response():changetype<Ros2Abb_rapid_sm_addin_msgsSrvSetEGMSettings_Response>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Request; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request {
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request{const value=new Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request();
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
export class Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_RequestCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request>{
static readonly codecIdentity:string="UWVMROS1_4561b7e5e296a1a329d5f5684bcbde1ef41324554a34e9a79a85824286dd477c";
static readonly maxWireBytes:i32=584;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request>();if(bytes.length>584){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request():changetype<Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Request>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Response; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response {
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response{const value=new Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response();
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
export class Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_ResponseCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response>{
static readonly codecIdentity:string="UWVMROS1_980d2a16536289f446161acdf2fc57d459baa2290d74f1dd5aaf8129126cbba1";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response():changetype<Ros2Abb_rapid_sm_addin_msgsSrvSetRAPIDRoutine_Response>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/srv/SetSGCommand_Request; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request {
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request{const value=new Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request();
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
export class Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_RequestCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request>{
static readonly codecIdentity:string="UWVMROS1_5d6983aa85c2c52f8ec06cbd723d5929cfc381db2e0bfffb4c27078497d86c90";
static readonly maxWireBytes:i32=73;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request>();if(bytes.length>73){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request():changetype<Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Request>(out.value);}
}
/** Complete original abb_rapid_sm_addin_msgs/srv/SetSGCommand_Response; spellings are published in generation proof. */
export class Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response {
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
static readControl(r:Reader):Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response{const value=new Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response();
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
export class Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_ResponseCodec implements VendorCodec<Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response>{
static readonly codecIdentity:string="UWVMROS1_87cdc41c81d49930dce7193d4b7af98add266a6e9a7cf5e69457d01ded388cf8";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response>{const out=new VendorDecoded<Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response():changetype<Ros2Abb_rapid_sm_addin_msgsSrvSetSGCommand_Response>(out.value);}
}
/** Complete original abb_robot_msgs/msg/MechanicalUnitState; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsMsgMechanicalUnitState {
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
static readControl(r:Reader):Ros2Abb_robot_msgsMsgMechanicalUnitState{const value=new Ros2Abb_robot_msgsMsgMechanicalUnitState();
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
export class Ros2Abb_robot_msgsMsgMechanicalUnitStateCodec implements VendorCodec<Ros2Abb_robot_msgsMsgMechanicalUnitState>{
static readonly codecIdentity:string="UWVMROS1_1dc7a7dcbaa19e78cd6419f8844287a7fbd9d7a839ab1a8ad97ef0be34f3a3a8";
static readonly maxWireBytes:i32=69;
encodeChecked(value:Ros2Abb_robot_msgsMsgMechanicalUnitState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsMsgMechanicalUnitState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsMsgMechanicalUnitState>{const out=new VendorDecoded<Ros2Abb_robot_msgsMsgMechanicalUnitState>();if(bytes.length>69){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsMsgMechanicalUnitState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsMsgMechanicalUnitState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsMsgMechanicalUnitState():changetype<Ros2Abb_robot_msgsMsgMechanicalUnitState>(out.value);}
}
/** Complete original abb_robot_msgs/msg/RAPIDSymbolPath; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsMsgRAPIDSymbolPath {
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
static readControl(r:Reader):Ros2Abb_robot_msgsMsgRAPIDSymbolPath{const value=new Ros2Abb_robot_msgsMsgRAPIDSymbolPath();
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
export class Ros2Abb_robot_msgsMsgRAPIDSymbolPathCodec implements VendorCodec<Ros2Abb_robot_msgsMsgRAPIDSymbolPath>{
static readonly codecIdentity:string="UWVMROS1_249523197567fbf638f30487a7f5b8d8ff5ea746fcd4a65a35f2dbb18a3adf0e";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros2Abb_robot_msgsMsgRAPIDSymbolPath):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsMsgRAPIDSymbolPath):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsMsgRAPIDSymbolPath>{const out=new VendorDecoded<Ros2Abb_robot_msgsMsgRAPIDSymbolPath>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsMsgRAPIDSymbolPath.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsMsgRAPIDSymbolPath{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsMsgRAPIDSymbolPath():changetype<Ros2Abb_robot_msgsMsgRAPIDSymbolPath>(out.value);}
}
/** Complete original abb_robot_msgs/msg/RAPIDTaskState; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsMsgRAPIDTaskState {
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
static readControl(r:Reader):Ros2Abb_robot_msgsMsgRAPIDTaskState{const value=new Ros2Abb_robot_msgsMsgRAPIDTaskState();
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
export class Ros2Abb_robot_msgsMsgRAPIDTaskStateCodec implements VendorCodec<Ros2Abb_robot_msgsMsgRAPIDTaskState>{
static readonly codecIdentity:string="UWVMROS1_ff698e375b0ff33026cfd248c1ac734f939763aba25a6c4f7cac628d445422fd";
static readonly maxWireBytes:i32=71;
encodeChecked(value:Ros2Abb_robot_msgsMsgRAPIDTaskState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsMsgRAPIDTaskState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsMsgRAPIDTaskState>{const out=new VendorDecoded<Ros2Abb_robot_msgsMsgRAPIDTaskState>();if(bytes.length>71){out.status=90;return out;}if(bytes.length<7){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsMsgRAPIDTaskState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsMsgRAPIDTaskState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsMsgRAPIDTaskState():changetype<Ros2Abb_robot_msgsMsgRAPIDTaskState>(out.value);}
}
/** Complete original abb_robot_msgs/msg/ServiceResponses; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsMsgServiceResponses {
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
static readControl(r:Reader):Ros2Abb_robot_msgsMsgServiceResponses{const value=new Ros2Abb_robot_msgsMsgServiceResponses();
return value;}
}
export class Ros2Abb_robot_msgsMsgServiceResponsesCodec implements VendorCodec<Ros2Abb_robot_msgsMsgServiceResponses>{
static readonly codecIdentity:string="UWVMROS1_4e01b972b67454e851801f28aee09c0774b67907d2241cc67cae7d045c18f646";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Abb_robot_msgsMsgServiceResponses):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsMsgServiceResponses):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsMsgServiceResponses>{const out=new VendorDecoded<Ros2Abb_robot_msgsMsgServiceResponses>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsMsgServiceResponses.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsMsgServiceResponses{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsMsgServiceResponses():changetype<Ros2Abb_robot_msgsMsgServiceResponses>(out.value);}
}
/** Complete original abb_robot_msgs/msg/SystemState; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsMsgSystemState {
static readonly maxWireBytes:i32=2327;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2327;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field motors_on, native scalar bool. */
motors_on:bool=false;
/** Exact native field auto_mode, native scalar bool. */
auto_mode:bool=false;
/** Exact native field rapid_running, native scalar bool. */
rapid_running:bool=false;
/** Exact native field rapid_tasks, native scalar abb_robot_msgs/msg/RAPIDTaskState. */
rapid_tasks:Array<Ros2Abb_robot_msgsMsgRAPIDTaskState>=new Array<Ros2Abb_robot_msgsMsgRAPIDTaskState>();
/** Exact native field mechanical_units, native scalar abb_robot_msgs/msg/MechanicalUnitState. */
mechanical_units:Array<Ros2Abb_robot_msgsMsgMechanicalUnitState>=new Array<Ros2Abb_robot_msgsMsgMechanicalUnitState>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>2327-n)return -90;n+=child;
if(n>2327)return -90;
}
{
if(1>2327-n)return -90;n+=1;
if(n>2327)return -90;
}
{
if(1>2327-n)return -90;n+=1;
if(n>2327)return -90;
}
{
if(1>2327-n)return -90;n+=1;
if(n>2327)return -90;
}
{
if(this.rapid_tasks.length>16)return -22;
if(4>2327-n)return -90;n+=4;
for(let i=0;i<this.rapid_tasks.length;++i){
const child=this.rapid_tasks[i].wireSize();if(child<0)return child;if(child>2327-n)return -90;n+=child;
if(n>2327)return -90;
}
}
{
if(this.mechanical_units.length>16)return -22;
if(4>2327-n)return -90;n+=4;
for(let i=0;i<this.mechanical_units.length;++i){
const child=this.mechanical_units[i].wireSize();if(child<0)return child;if(child>2327-n)return -90;n+=child;
if(n>2327)return -90;
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
static readControl(r:Reader):Ros2Abb_robot_msgsMsgSystemState{const value=new Ros2Abb_robot_msgsMsgSystemState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
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
value.rapid_tasks=new Array<Ros2Abb_robot_msgsMsgRAPIDTaskState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Abb_robot_msgsMsgRAPIDTaskState.readControl(r);
value.rapid_tasks.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>16){r.valid=false;return value;}
value.mechanical_units=new Array<Ros2Abb_robot_msgsMsgMechanicalUnitState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Abb_robot_msgsMsgMechanicalUnitState.readControl(r);
value.mechanical_units.push(decoded);
}
}
return value;}
}
export class Ros2Abb_robot_msgsMsgSystemStateCodec implements VendorCodec<Ros2Abb_robot_msgsMsgSystemState>{
static readonly codecIdentity:string="UWVMROS1_c1cdbf0e578fddc5bf70cc2cc181d786f3e8189afc6a67962de6eb273973a38f";
static readonly maxWireBytes:i32=2327;
encodeChecked(value:Ros2Abb_robot_msgsMsgSystemState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsMsgSystemState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsMsgSystemState>{const out=new VendorDecoded<Ros2Abb_robot_msgsMsgSystemState>();if(bytes.length>2327){out.status=90;return out;}if(bytes.length<23){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsMsgSystemState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsMsgSystemState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsMsgSystemState():changetype<Ros2Abb_robot_msgsMsgSystemState>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetFileContents_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetFileContents_Request {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetFileContents_Request{const value=new Ros2Abb_robot_msgsSrvGetFileContents_Request();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.filename=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvGetFileContents_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetFileContents_Request>{
static readonly codecIdentity:string="UWVMROS1_bdcf73a965bd05356e48019b74d633176fdeda62452bc7185d6b6ff30576016b";
static readonly maxWireBytes:i32=516;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetFileContents_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetFileContents_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetFileContents_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetFileContents_Request>();if(bytes.length>516){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetFileContents_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetFileContents_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetFileContents_Request():changetype<Ros2Abb_robot_msgsSrvGetFileContents_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetFileContents_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetFileContents_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetFileContents_Response{const value=new Ros2Abb_robot_msgsSrvGetFileContents_Response();
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
export class Ros2Abb_robot_msgsSrvGetFileContents_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetFileContents_Response>{
static readonly codecIdentity:string="UWVMROS1_f05617145a40043e87cbc251f833274d2a962bd7cdaabc5f9a8e4591bd56e107";
static readonly maxWireBytes:i32=4618;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetFileContents_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetFileContents_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetFileContents_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetFileContents_Response>();if(bytes.length>4618){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetFileContents_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetFileContents_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetFileContents_Response():changetype<Ros2Abb_robot_msgsSrvGetFileContents_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetIOSignal_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetIOSignal_Request {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetIOSignal_Request{const value=new Ros2Abb_robot_msgsSrvGetIOSignal_Request();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.signal=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvGetIOSignal_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetIOSignal_Request>{
static readonly codecIdentity:string="UWVMROS1_2406d53c2fce44e72ad7e6f1c9e75ea19c4df694ff9f00917229ec758209b1f5";
static readonly maxWireBytes:i32=516;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetIOSignal_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetIOSignal_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetIOSignal_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetIOSignal_Request>();if(bytes.length>516){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetIOSignal_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetIOSignal_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetIOSignal_Request():changetype<Ros2Abb_robot_msgsSrvGetIOSignal_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetIOSignal_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetIOSignal_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetIOSignal_Response{const value=new Ros2Abb_robot_msgsSrvGetIOSignal_Response();
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
export class Ros2Abb_robot_msgsSrvGetIOSignal_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetIOSignal_Response>{
static readonly codecIdentity:string="UWVMROS1_1f668ef78c353f2ec8defdac619d349cda731576f2429cf07107b9b56ba9030e";
static readonly maxWireBytes:i32=1034;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetIOSignal_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetIOSignal_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetIOSignal_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetIOSignal_Response>();if(bytes.length>1034){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetIOSignal_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetIOSignal_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetIOSignal_Response():changetype<Ros2Abb_robot_msgsSrvGetIOSignal_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRAPIDBool_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRAPIDBool_Request {
static readonly maxWireBytes:i32=204;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=204;
/** Exact native field path, native scalar abb_robot_msgs/msg/RAPIDSymbolPath. */
path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath=new Ros2Abb_robot_msgsMsgRAPIDSymbolPath();
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRAPIDBool_Request{const value=new Ros2Abb_robot_msgsSrvGetRAPIDBool_Request();
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvGetRAPIDBool_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRAPIDBool_Request>{
static readonly codecIdentity:string="UWVMROS1_c381080d235c78216a8e64275819ff3fb97e1b1de96e7da758c4889630b8e0ff";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRAPIDBool_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRAPIDBool_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDBool_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDBool_Request>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRAPIDBool_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRAPIDBool_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRAPIDBool_Request():changetype<Ros2Abb_robot_msgsSrvGetRAPIDBool_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRAPIDBool_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRAPIDBool_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRAPIDBool_Response{const value=new Ros2Abb_robot_msgsSrvGetRAPIDBool_Response();
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
export class Ros2Abb_robot_msgsSrvGetRAPIDBool_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRAPIDBool_Response>{
static readonly codecIdentity:string="UWVMROS1_b853e212489fb815866fa29b165eea474a977483158b6203699c4a1293aa6acd";
static readonly maxWireBytes:i32=519;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRAPIDBool_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRAPIDBool_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDBool_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDBool_Response>();if(bytes.length>519){out.status=90;return out;}if(bytes.length<7){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRAPIDBool_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRAPIDBool_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRAPIDBool_Response():changetype<Ros2Abb_robot_msgsSrvGetRAPIDBool_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRAPIDDnum_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request {
static readonly maxWireBytes:i32=204;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=204;
/** Exact native field path, native scalar abb_robot_msgs/msg/RAPIDSymbolPath. */
path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath=new Ros2Abb_robot_msgsMsgRAPIDSymbolPath();
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request{const value=new Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request();
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvGetRAPIDDnum_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request>{
static readonly codecIdentity:string="UWVMROS1_71057c2fe44bc96505ec6e05d998fcb6323b281fff40808ce990207c48626979";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request():changetype<Ros2Abb_robot_msgsSrvGetRAPIDDnum_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRAPIDDnum_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response{const value=new Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response();
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
export class Ros2Abb_robot_msgsSrvGetRAPIDDnum_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response>{
static readonly codecIdentity:string="UWVMROS1_af0420c126368a7e45b728c7fef76062b5ef2f0adea7535ad38e4771244edd56";
static readonly maxWireBytes:i32=526;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response>();if(bytes.length>526){out.status=90;return out;}if(bytes.length<14){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response():changetype<Ros2Abb_robot_msgsSrvGetRAPIDDnum_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRAPIDNum_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRAPIDNum_Request {
static readonly maxWireBytes:i32=204;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=204;
/** Exact native field path, native scalar abb_robot_msgs/msg/RAPIDSymbolPath. */
path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath=new Ros2Abb_robot_msgsMsgRAPIDSymbolPath();
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRAPIDNum_Request{const value=new Ros2Abb_robot_msgsSrvGetRAPIDNum_Request();
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvGetRAPIDNum_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRAPIDNum_Request>{
static readonly codecIdentity:string="UWVMROS1_03d5da34af8d90ed8a20f8ca38fa75d9e777496828978f0f348e3c1c8f67d0a5";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRAPIDNum_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRAPIDNum_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDNum_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDNum_Request>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRAPIDNum_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRAPIDNum_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRAPIDNum_Request():changetype<Ros2Abb_robot_msgsSrvGetRAPIDNum_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRAPIDNum_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRAPIDNum_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRAPIDNum_Response{const value=new Ros2Abb_robot_msgsSrvGetRAPIDNum_Response();
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
export class Ros2Abb_robot_msgsSrvGetRAPIDNum_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRAPIDNum_Response>{
static readonly codecIdentity:string="UWVMROS1_a0da03014dd001e58efc4a5b274cd5c62016e7fa257af1120a8142a79635220d";
static readonly maxWireBytes:i32=522;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRAPIDNum_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRAPIDNum_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDNum_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDNum_Response>();if(bytes.length>522){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRAPIDNum_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRAPIDNum_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRAPIDNum_Response():changetype<Ros2Abb_robot_msgsSrvGetRAPIDNum_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRAPIDString_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRAPIDString_Request {
static readonly maxWireBytes:i32=204;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=204;
/** Exact native field path, native scalar abb_robot_msgs/msg/RAPIDSymbolPath. */
path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath=new Ros2Abb_robot_msgsMsgRAPIDSymbolPath();
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRAPIDString_Request{const value=new Ros2Abb_robot_msgsSrvGetRAPIDString_Request();
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvGetRAPIDString_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRAPIDString_Request>{
static readonly codecIdentity:string="UWVMROS1_c7bc9c788301e619a165c18a1cdb0aca80df8def5be31da5dbf34c0038e73596";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRAPIDString_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRAPIDString_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDString_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDString_Request>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRAPIDString_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRAPIDString_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRAPIDString_Request():changetype<Ros2Abb_robot_msgsSrvGetRAPIDString_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRAPIDString_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRAPIDString_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRAPIDString_Response{const value=new Ros2Abb_robot_msgsSrvGetRAPIDString_Response();
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
export class Ros2Abb_robot_msgsSrvGetRAPIDString_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRAPIDString_Response>{
static readonly codecIdentity:string="UWVMROS1_cf7bfdb2af5b91eb58b19b909f7fa3357dc1dad02716f8fa4465b53f3f4a1458";
static readonly maxWireBytes:i32=1034;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRAPIDString_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRAPIDString_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDString_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDString_Response>();if(bytes.length>1034){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRAPIDString_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRAPIDString_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRAPIDString_Response():changetype<Ros2Abb_robot_msgsSrvGetRAPIDString_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRAPIDSymbol_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request {
static readonly maxWireBytes:i32=204;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=204;
/** Exact native field path, native scalar abb_robot_msgs/msg/RAPIDSymbolPath. */
path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath=new Ros2Abb_robot_msgsMsgRAPIDSymbolPath();
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request{const value=new Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request();
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvGetRAPIDSymbol_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request>{
static readonly codecIdentity:string="UWVMROS1_e216a54cdae70f8147489928ff05a4b172ba4fc47e9ed5df4828b070354ac469";
static readonly maxWireBytes:i32=204;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request>();if(bytes.length>204){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request():changetype<Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRAPIDSymbol_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response{const value=new Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response();
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
export class Ros2Abb_robot_msgsSrvGetRAPIDSymbol_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response>{
static readonly codecIdentity:string="UWVMROS1_b1f6de7ad0640f5645678d327804dc7e732bdad38805e642b13ed7294c21d602";
static readonly maxWireBytes:i32=1034;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response>();if(bytes.length>1034){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response():changetype<Ros2Abb_robot_msgsSrvGetRAPIDSymbol_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRobotControllerDescription_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request{const value=new Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request();
return value;}
}
export class Ros2Abb_robot_msgsSrvGetRobotControllerDescription_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request>{
static readonly codecIdentity:string="UWVMROS1_caf70e11b386a933f62d06a433ca1cedaa3c8ecc75b7e8de8c1d5ae00a29f03e";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request():changetype<Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetRobotControllerDescription_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response{const value=new Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response();
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
export class Ros2Abb_robot_msgsSrvGetRobotControllerDescription_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response>{
static readonly codecIdentity:string="UWVMROS1_1af2591f035379597f376ed1e2343f73a89f9fef5c29269aa420243a82e6c368";
static readonly maxWireBytes:i32=4618;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response>();if(bytes.length>4618){out.status=90;return out;}if(bytes.length<10){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response():changetype<Ros2Abb_robot_msgsSrvGetRobotControllerDescription_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetSpeedRatio_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetSpeedRatio_Request {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetSpeedRatio_Request{const value=new Ros2Abb_robot_msgsSrvGetSpeedRatio_Request();
return value;}
}
export class Ros2Abb_robot_msgsSrvGetSpeedRatio_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetSpeedRatio_Request>{
static readonly codecIdentity:string="UWVMROS1_54ee2c897e2bf2f39c96cc42246ffe5997c8df280280433e8748b3bcf24c299c";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetSpeedRatio_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetSpeedRatio_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetSpeedRatio_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetSpeedRatio_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetSpeedRatio_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetSpeedRatio_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetSpeedRatio_Request():changetype<Ros2Abb_robot_msgsSrvGetSpeedRatio_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/GetSpeedRatio_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvGetSpeedRatio_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvGetSpeedRatio_Response{const value=new Ros2Abb_robot_msgsSrvGetSpeedRatio_Response();
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
export class Ros2Abb_robot_msgsSrvGetSpeedRatio_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvGetSpeedRatio_Response>{
static readonly codecIdentity:string="UWVMROS1_baa271a258c7ca274e0145fea1abb7cbc43e98c82b8e40d67c4743237a19910b";
static readonly maxWireBytes:i32=519;
encodeChecked(value:Ros2Abb_robot_msgsSrvGetSpeedRatio_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvGetSpeedRatio_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvGetSpeedRatio_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvGetSpeedRatio_Response>();if(bytes.length>519){out.status=90;return out;}if(bytes.length<7){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvGetSpeedRatio_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvGetSpeedRatio_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvGetSpeedRatio_Response():changetype<Ros2Abb_robot_msgsSrvGetSpeedRatio_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetFileContents_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetFileContents_Request {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetFileContents_Request{const value=new Ros2Abb_robot_msgsSrvSetFileContents_Request();
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
export class Ros2Abb_robot_msgsSrvSetFileContents_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetFileContents_Request>{
static readonly codecIdentity:string="UWVMROS1_3bfa8ad1846fc31acae47a875d3555a78a64b777923eb9b62e87a836f041b26b";
static readonly maxWireBytes:i32=4616;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetFileContents_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetFileContents_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetFileContents_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetFileContents_Request>();if(bytes.length>4616){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetFileContents_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetFileContents_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetFileContents_Request():changetype<Ros2Abb_robot_msgsSrvSetFileContents_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetFileContents_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetFileContents_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetFileContents_Response{const value=new Ros2Abb_robot_msgsSrvSetFileContents_Response();
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
export class Ros2Abb_robot_msgsSrvSetFileContents_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetFileContents_Response>{
static readonly codecIdentity:string="UWVMROS1_1586344401d43055460b49f2487567638f66838cfe5cac99a47e0a39c0b06ede";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetFileContents_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetFileContents_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetFileContents_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetFileContents_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetFileContents_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetFileContents_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetFileContents_Response():changetype<Ros2Abb_robot_msgsSrvSetFileContents_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetIOSignal_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetIOSignal_Request {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetIOSignal_Request{const value=new Ros2Abb_robot_msgsSrvSetIOSignal_Request();
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
export class Ros2Abb_robot_msgsSrvSetIOSignal_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetIOSignal_Request>{
static readonly codecIdentity:string="UWVMROS1_2d206d0020179263e83e745a428616ed53d809b0c65a79ad495cb882ae9e49b7";
static readonly maxWireBytes:i32=1032;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetIOSignal_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetIOSignal_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetIOSignal_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetIOSignal_Request>();if(bytes.length>1032){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetIOSignal_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetIOSignal_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetIOSignal_Request():changetype<Ros2Abb_robot_msgsSrvSetIOSignal_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetIOSignal_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetIOSignal_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetIOSignal_Response{const value=new Ros2Abb_robot_msgsSrvSetIOSignal_Response();
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
export class Ros2Abb_robot_msgsSrvSetIOSignal_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetIOSignal_Response>{
static readonly codecIdentity:string="UWVMROS1_1fa28cc3160d51f71d0740cbe145663ec360d6fa488c2a6bf14ad6ab92004a75";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetIOSignal_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetIOSignal_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetIOSignal_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetIOSignal_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetIOSignal_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetIOSignal_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetIOSignal_Response():changetype<Ros2Abb_robot_msgsSrvSetIOSignal_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetRAPIDBool_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetRAPIDBool_Request {
static readonly maxWireBytes:i32=205;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=205;
/** Exact native field path, native scalar abb_robot_msgs/msg/RAPIDSymbolPath. */
path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath=new Ros2Abb_robot_msgsMsgRAPIDSymbolPath();
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetRAPIDBool_Request{const value=new Ros2Abb_robot_msgsSrvSetRAPIDBool_Request();
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.value=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvSetRAPIDBool_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetRAPIDBool_Request>{
static readonly codecIdentity:string="UWVMROS1_4e707e429acc860d032d216bcd2c1eb1a0f731fad7e7c0bef7beed810c6e4a52";
static readonly maxWireBytes:i32=205;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetRAPIDBool_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetRAPIDBool_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDBool_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDBool_Request>();if(bytes.length>205){out.status=90;return out;}if(bytes.length<13){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetRAPIDBool_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetRAPIDBool_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetRAPIDBool_Request():changetype<Ros2Abb_robot_msgsSrvSetRAPIDBool_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetRAPIDBool_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetRAPIDBool_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetRAPIDBool_Response{const value=new Ros2Abb_robot_msgsSrvSetRAPIDBool_Response();
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
export class Ros2Abb_robot_msgsSrvSetRAPIDBool_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetRAPIDBool_Response>{
static readonly codecIdentity:string="UWVMROS1_80d2b31c93771cfa8a19ff82a3d9d63951bf9d28c039d2725017327fa3b0114a";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetRAPIDBool_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetRAPIDBool_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDBool_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDBool_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetRAPIDBool_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetRAPIDBool_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetRAPIDBool_Response():changetype<Ros2Abb_robot_msgsSrvSetRAPIDBool_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetRAPIDDnum_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request {
static readonly maxWireBytes:i32=212;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=212;
/** Exact native field path, native scalar abb_robot_msgs/msg/RAPIDSymbolPath. */
path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath=new Ros2Abb_robot_msgsMsgRAPIDSymbolPath();
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request{const value=new Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request();
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvSetRAPIDDnum_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request>{
static readonly codecIdentity:string="UWVMROS1_cece086311a908f12c0b0ab3988ca829e891749c74687c717d2a52bfb8ceaf76";
static readonly maxWireBytes:i32=212;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request>();if(bytes.length>212){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request():changetype<Ros2Abb_robot_msgsSrvSetRAPIDDnum_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetRAPIDDnum_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response{const value=new Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response();
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
export class Ros2Abb_robot_msgsSrvSetRAPIDDnum_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response>{
static readonly codecIdentity:string="UWVMROS1_e5f77fe9850f109d873a2784b9690c8aa0632b9e91d158ed4d14daf1aaf90ea6";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response():changetype<Ros2Abb_robot_msgsSrvSetRAPIDDnum_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetRAPIDNum_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetRAPIDNum_Request {
static readonly maxWireBytes:i32=208;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=208;
/** Exact native field path, native scalar abb_robot_msgs/msg/RAPIDSymbolPath. */
path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath=new Ros2Abb_robot_msgsMsgRAPIDSymbolPath();
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetRAPIDNum_Request{const value=new Ros2Abb_robot_msgsSrvSetRAPIDNum_Request();
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.value=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvSetRAPIDNum_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetRAPIDNum_Request>{
static readonly codecIdentity:string="UWVMROS1_a5bee9a64be69dc4246f68837a44362f52795ae3c6420a2df47685606283ea02";
static readonly maxWireBytes:i32=208;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetRAPIDNum_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetRAPIDNum_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDNum_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDNum_Request>();if(bytes.length>208){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetRAPIDNum_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetRAPIDNum_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetRAPIDNum_Request():changetype<Ros2Abb_robot_msgsSrvSetRAPIDNum_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetRAPIDNum_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetRAPIDNum_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetRAPIDNum_Response{const value=new Ros2Abb_robot_msgsSrvSetRAPIDNum_Response();
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
export class Ros2Abb_robot_msgsSrvSetRAPIDNum_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetRAPIDNum_Response>{
static readonly codecIdentity:string="UWVMROS1_744f3c5fac58ff098c96cedcea94fe6fa87c88ceb89674449f50fbdcd1ca30bb";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetRAPIDNum_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetRAPIDNum_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDNum_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDNum_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetRAPIDNum_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetRAPIDNum_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetRAPIDNum_Response():changetype<Ros2Abb_robot_msgsSrvSetRAPIDNum_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetRAPIDString_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetRAPIDString_Request {
static readonly maxWireBytes:i32=720;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=720;
/** Exact native field path, native scalar abb_robot_msgs/msg/RAPIDSymbolPath. */
path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath=new Ros2Abb_robot_msgsMsgRAPIDSymbolPath();
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetRAPIDString_Request{const value=new Ros2Abb_robot_msgsSrvSetRAPIDString_Request();
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.value=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvSetRAPIDString_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetRAPIDString_Request>{
static readonly codecIdentity:string="UWVMROS1_f21c45f3f8cd4c0b50781bdd487fdc6ec0c0b35adbc7e36e9ca5b7cc47c3d461";
static readonly maxWireBytes:i32=720;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetRAPIDString_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetRAPIDString_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDString_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDString_Request>();if(bytes.length>720){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetRAPIDString_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetRAPIDString_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetRAPIDString_Request():changetype<Ros2Abb_robot_msgsSrvSetRAPIDString_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetRAPIDString_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetRAPIDString_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetRAPIDString_Response{const value=new Ros2Abb_robot_msgsSrvSetRAPIDString_Response();
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
export class Ros2Abb_robot_msgsSrvSetRAPIDString_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetRAPIDString_Response>{
static readonly codecIdentity:string="UWVMROS1_1496ef4f4ffe48fe74b9e25b925c19f10f0e171a9d33fa2e56c15aa5cfbe4774";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetRAPIDString_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetRAPIDString_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDString_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDString_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetRAPIDString_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetRAPIDString_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetRAPIDString_Response():changetype<Ros2Abb_robot_msgsSrvSetRAPIDString_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetRAPIDSymbol_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request {
static readonly maxWireBytes:i32=720;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=720;
/** Exact native field path, native scalar abb_robot_msgs/msg/RAPIDSymbolPath. */
path:Ros2Abb_robot_msgsMsgRAPIDSymbolPath=new Ros2Abb_robot_msgsMsgRAPIDSymbolPath();
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request{const value=new Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request();
{
const decoded=Ros2Abb_robot_msgsMsgRAPIDSymbolPath.readControl(r);
value.path=decoded;
}
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.value=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvSetRAPIDSymbol_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request>{
static readonly codecIdentity:string="UWVMROS1_928d427801b79775595fed6e3779f9ba9e7e8e27f2de66cf79610cb51bb72bcf";
static readonly maxWireBytes:i32=720;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request>();if(bytes.length>720){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request():changetype<Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetRAPIDSymbol_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response{const value=new Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response();
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
export class Ros2Abb_robot_msgsSrvSetRAPIDSymbol_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response>{
static readonly codecIdentity:string="UWVMROS1_e156616a51bf4691a1c3853d62a8b0c43bb17d1a330cd4e7a58eceb5fe442382";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response():changetype<Ros2Abb_robot_msgsSrvSetRAPIDSymbol_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetSpeedRatio_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetSpeedRatio_Request {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetSpeedRatio_Request{const value=new Ros2Abb_robot_msgsSrvSetSpeedRatio_Request();
{
const decoded=<u8>r.integer(1);
value.speed_ratio=decoded;
}
return value;}
}
export class Ros2Abb_robot_msgsSrvSetSpeedRatio_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetSpeedRatio_Request>{
static readonly codecIdentity:string="UWVMROS1_da30970377ba1bc151b544560773f02e17f4eaed56eb4c8d791de72bdef04355";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetSpeedRatio_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetSpeedRatio_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetSpeedRatio_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetSpeedRatio_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetSpeedRatio_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetSpeedRatio_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetSpeedRatio_Request():changetype<Ros2Abb_robot_msgsSrvSetSpeedRatio_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/SetSpeedRatio_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvSetSpeedRatio_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvSetSpeedRatio_Response{const value=new Ros2Abb_robot_msgsSrvSetSpeedRatio_Response();
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
export class Ros2Abb_robot_msgsSrvSetSpeedRatio_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvSetSpeedRatio_Response>{
static readonly codecIdentity:string="UWVMROS1_25e6d7edaa0503a4d45d893e886db4215bfd0af8711307c141bc63552609e3a5";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_robot_msgsSrvSetSpeedRatio_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvSetSpeedRatio_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvSetSpeedRatio_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvSetSpeedRatio_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvSetSpeedRatio_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvSetSpeedRatio_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvSetSpeedRatio_Response():changetype<Ros2Abb_robot_msgsSrvSetSpeedRatio_Response>(out.value);}
}
/** Complete original abb_robot_msgs/srv/TriggerWithResultCode_Request; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request{const value=new Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request();
return value;}
}
export class Ros2Abb_robot_msgsSrvTriggerWithResultCode_RequestCodec implements VendorCodec<Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request>{
static readonly codecIdentity:string="UWVMROS1_fd1d49c3b4e489464c6662f306210137f71d15192f64cd8a8abf93acd5fbff8d";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request():changetype<Ros2Abb_robot_msgsSrvTriggerWithResultCode_Request>(out.value);}
}
/** Complete original abb_robot_msgs/srv/TriggerWithResultCode_Response; spellings are published in generation proof. */
export class Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response {
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
static readControl(r:Reader):Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response{const value=new Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response();
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
export class Ros2Abb_robot_msgsSrvTriggerWithResultCode_ResponseCodec implements VendorCodec<Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response>{
static readonly codecIdentity:string="UWVMROS1_e38422b2a008f07e108505ac3ce65c24189a1d6657e88a6289e17d7d4ae38d51";
static readonly maxWireBytes:i32=518;
encodeChecked(value:Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response>{const out=new VendorDecoded<Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response>();if(bytes.length>518){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response():changetype<Ros2Abb_robot_msgsSrvTriggerWithResultCode_Response>(out.value);}
}
/** Complete original builtin_interfaces/msg/Duration; spellings are published in generation proof. */
export class Ros2Builtin_interfacesMsgDuration {
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
static readControl(r:Reader):Ros2Builtin_interfacesMsgDuration{const value=new Ros2Builtin_interfacesMsgDuration();
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
export class Ros2Builtin_interfacesMsgDurationCodec implements VendorCodec<Ros2Builtin_interfacesMsgDuration>{
static readonly codecIdentity:string="UWVMROS1_3804444fae7a82a57907d25c2c3dc54d9a42dcd305f41ca073b1221dca7d36e4";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Builtin_interfacesMsgDuration):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Builtin_interfacesMsgDuration):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Builtin_interfacesMsgDuration>{const out=new VendorDecoded<Ros2Builtin_interfacesMsgDuration>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Builtin_interfacesMsgDuration.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Builtin_interfacesMsgDuration{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Builtin_interfacesMsgDuration():changetype<Ros2Builtin_interfacesMsgDuration>(out.value);}
}
/** Complete original trajectory_msgs/msg/JointTrajectoryPoint; spellings are published in generation proof. */
export class Ros2Trajectory_msgsMsgJointTrajectoryPoint {
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
/** Exact native field time_from_start, native scalar builtin_interfaces/msg/Duration. */
time_from_start:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
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
static readControl(r:Reader):Ros2Trajectory_msgsMsgJointTrajectoryPoint{const value=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
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
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.time_from_start=decoded;
}
return value;}
}
export class Ros2Trajectory_msgsMsgJointTrajectoryPointCodec implements VendorCodec<Ros2Trajectory_msgsMsgJointTrajectoryPoint>{
static readonly codecIdentity:string="UWVMROS1_4d55f494f060a9a3ff23bedd54da49f9e70e6b77acbdf911878afbf672cbadc5";
static readonly maxWireBytes:i32=408;
encodeChecked(value:Ros2Trajectory_msgsMsgJointTrajectoryPoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Trajectory_msgsMsgJointTrajectoryPoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectoryPoint>{const out=new VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectoryPoint>();if(bytes.length>408){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Trajectory_msgsMsgJointTrajectoryPoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Trajectory_msgsMsgJointTrajectoryPoint():changetype<Ros2Trajectory_msgsMsgJointTrajectoryPoint>(out.value);}
}
/** Complete original trajectory_msgs/msg/JointTrajectory; spellings are published in generation proof. */
export class Ros2Trajectory_msgsMsgJointTrajectory {
static readonly maxWireBytes:i32=4164;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4164;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field points, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
points:Array<Ros2Trajectory_msgsMsgJointTrajectoryPoint>=new Array<Ros2Trajectory_msgsMsgJointTrajectoryPoint>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>4164-n)return -90;n+=child;
if(n>4164)return -90;
}
{
if(this.joint_names.length>12)return -22;
if(4>4164-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>64)return -22;if(4+size>4164-n)return -90;n+=4+size;
if(n>4164)return -90;
}
}
{
if(this.points.length>8)return -22;
if(4>4164-n)return -90;n+=4;
for(let i=0;i<this.points.length;++i){
const child=this.points[i].wireSize();if(child<0)return child;if(child>4164-n)return -90;n+=child;
if(n>4164)return -90;
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
static readControl(r:Reader):Ros2Trajectory_msgsMsgJointTrajectory{const value=new Ros2Trajectory_msgsMsgJointTrajectory();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
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
value.points=new Array<Ros2Trajectory_msgsMsgJointTrajectoryPoint>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.points.push(decoded);
}
}
return value;}
}
export class Ros2Trajectory_msgsMsgJointTrajectoryCodec implements VendorCodec<Ros2Trajectory_msgsMsgJointTrajectory>{
static readonly codecIdentity:string="UWVMROS1_d74956440763bd09c76ec623efeb322aa89be8ef6410b9125d4e2cb1aeeb69c0";
static readonly maxWireBytes:i32=4164;
encodeChecked(value:Ros2Trajectory_msgsMsgJointTrajectory):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Trajectory_msgsMsgJointTrajectory):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectory>{const out=new VendorDecoded<Ros2Trajectory_msgsMsgJointTrajectory>();if(bytes.length>4164){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Trajectory_msgsMsgJointTrajectory.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Trajectory_msgsMsgJointTrajectory{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Trajectory_msgsMsgJointTrajectory():changetype<Ros2Trajectory_msgsMsgJointTrajectory>(out.value);}
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
/** Complete original trajectory_msgs/msg/MultiDOFJointTrajectoryPoint; spellings are published in generation proof. */
export class Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint {
static readonly maxWireBytes:i32=324;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=324;
/** Exact native field transforms, native scalar geometry_msgs/msg/Transform. */
transforms:Array<Ros2Geometry_msgsMsgTransform>=new Array<Ros2Geometry_msgsMsgTransform>();
/** Exact native field velocities, native scalar geometry_msgs/msg/Twist. */
velocities:Array<Ros2Geometry_msgsMsgTwist>=new Array<Ros2Geometry_msgsMsgTwist>();
/** Exact native field accelerations, native scalar geometry_msgs/msg/Twist. */
accelerations:Array<Ros2Geometry_msgsMsgTwist>=new Array<Ros2Geometry_msgsMsgTwist>();
/** Exact native field time_from_start, native scalar builtin_interfaces/msg/Duration. */
time_from_start:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.transforms.length>2)return -22;
if(4>324-n)return -90;n+=4;
for(let i=0;i<this.transforms.length;++i){
const child=this.transforms[i].wireSize();if(child<0)return child;if(child>324-n)return -90;n+=child;
if(n>324)return -90;
}
}
{
if(this.velocities.length>2)return -22;
if(4>324-n)return -90;n+=4;
for(let i=0;i<this.velocities.length;++i){
const child=this.velocities[i].wireSize();if(child<0)return child;if(child>324-n)return -90;n+=child;
if(n>324)return -90;
}
}
{
if(this.accelerations.length>2)return -22;
if(4>324-n)return -90;n+=4;
for(let i=0;i<this.accelerations.length;++i){
const child=this.accelerations[i].wireSize();if(child<0)return child;if(child>324-n)return -90;n+=child;
if(n>324)return -90;
}
}
{
const child=this.time_from_start.wireSize();if(child<0)return child;if(child>324-n)return -90;n+=child;
if(n>324)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.transforms.length);
for(let i=0;i<this.transforms.length;++i){
this.transforms[i].writeControl(w);
}
w.u32(<u32>this.velocities.length);
for(let i=0;i<this.velocities.length;++i){
this.velocities[i].writeControl(w);
}
w.u32(<u32>this.accelerations.length);
for(let i=0;i<this.accelerations.length;++i){
this.accelerations[i].writeControl(w);
}
this.time_from_start.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint{const value=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint();
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.transforms=new Array<Ros2Geometry_msgsMsgTransform>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgTransform.readControl(r);
value.transforms.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.velocities=new Array<Ros2Geometry_msgsMsgTwist>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgTwist.readControl(r);
value.velocities.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.accelerations=new Array<Ros2Geometry_msgsMsgTwist>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Geometry_msgsMsgTwist.readControl(r);
value.accelerations.push(decoded);
}
}
{
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.time_from_start=decoded;
}
return value;}
}
export class Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPointCodec implements VendorCodec<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>{
static readonly codecIdentity:string="UWVMROS1_6590b7ca1cdaa89da04b7a661266e2d4c6e3e2668dc90505a7c24d7be8a43242";
static readonly maxWireBytes:i32=324;
encodeChecked(value:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>{const out=new VendorDecoded<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>();if(bytes.length>324){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint():changetype<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>(out.value);}
}
/** Complete original trajectory_msgs/msg/MultiDOFJointTrajectory; spellings are published in generation proof. */
export class Ros2Trajectory_msgsMsgMultiDOFJointTrajectory {
static readonly maxWireBytes:i32=1516;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1516;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field points, native scalar trajectory_msgs/msg/MultiDOFJointTrajectoryPoint. */
points:Array<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>=new Array<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>1516-n)return -90;n+=child;
if(n>1516)return -90;
}
{
if(this.joint_names.length>2)return -22;
if(4>1516-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>64)return -22;if(4+size>1516-n)return -90;n+=4+size;
if(n>1516)return -90;
}
}
{
if(this.points.length>4)return -22;
if(4>1516-n)return -90;n+=4;
for(let i=0;i<this.points.length;++i){
const child=this.points[i].wireSize();if(child<0)return child;if(child>1516-n)return -90;n+=child;
if(n>1516)return -90;
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
static readControl(r:Reader):Ros2Trajectory_msgsMsgMultiDOFJointTrajectory{const value=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectory();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.joint_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_names.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.points=new Array<Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint.readControl(r);
value.points.push(decoded);
}
}
return value;}
}
export class Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryCodec implements VendorCodec<Ros2Trajectory_msgsMsgMultiDOFJointTrajectory>{
static readonly codecIdentity:string="UWVMROS1_6ea66f146cdce31369361e521be484d2c39bca818cc4b2069e1209d250ec2401";
static readonly maxWireBytes:i32=1516;
encodeChecked(value:Ros2Trajectory_msgsMsgMultiDOFJointTrajectory):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Trajectory_msgsMsgMultiDOFJointTrajectory):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Trajectory_msgsMsgMultiDOFJointTrajectory>{const out=new VendorDecoded<Ros2Trajectory_msgsMsgMultiDOFJointTrajectory>();if(bytes.length>1516){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Trajectory_msgsMsgMultiDOFJointTrajectory.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Trajectory_msgsMsgMultiDOFJointTrajectory{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Trajectory_msgsMsgMultiDOFJointTrajectory():changetype<Ros2Trajectory_msgsMsgMultiDOFJointTrajectory>(out.value);}
}
/** Complete original control_msgs/msg/JointTolerance; spellings are published in generation proof. */
export class Ros2Control_msgsMsgJointTolerance {
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
static readControl(r:Reader):Ros2Control_msgsMsgJointTolerance{const value=new Ros2Control_msgsMsgJointTolerance();
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
export class Ros2Control_msgsMsgJointToleranceCodec implements VendorCodec<Ros2Control_msgsMsgJointTolerance>{
static readonly codecIdentity:string="UWVMROS1_ad7701ac1cb0dd68a947577272964b0d6051d1846019de97e9c9d0f7be7b2fa2";
static readonly maxWireBytes:i32=92;
encodeChecked(value:Ros2Control_msgsMsgJointTolerance):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Control_msgsMsgJointTolerance):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Control_msgsMsgJointTolerance>{const out=new VendorDecoded<Ros2Control_msgsMsgJointTolerance>();if(bytes.length>92){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Control_msgsMsgJointTolerance.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Control_msgsMsgJointTolerance{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Control_msgsMsgJointTolerance():changetype<Ros2Control_msgsMsgJointTolerance>(out.value);}
}
/** Complete original control_msgs/msg/JointComponentTolerance; spellings are published in generation proof. */
export class Ros2Control_msgsMsgJointComponentTolerance {
static readonly X_AXIS:u16=1;
static readonly Y_AXIS:u16=2;
static readonly Z_AXIS:u16=3;
static readonly TRANSLATION:u16=4;
static readonly ROTATION:u16=5;
static readonly maxWireBytes:i32=542;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=542;
/** Exact native field joint_name, native scalar string. */
joint_name:string="";
/** Exact native field component, native scalar uint16. */
component:u16=0;
/** Exact native field position, native scalar float64. */
position:f64=0;
/** Exact native field velocity, native scalar float64. */
velocity:f64=0;
/** Exact native field acceleration, native scalar float64. */
acceleration:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.joint_name)||this.joint_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_name);if(size>512)return -22;if(4+size>542-n)return -90;n+=4+size;
if(n>542)return -90;
}
{
if(2>542-n)return -90;n+=2;
if(n>542)return -90;
}
{
if(!isFinite(this.position))return -22;
if(8>542-n)return -90;n+=8;
if(n>542)return -90;
}
{
if(!isFinite(this.velocity))return -22;
if(8>542-n)return -90;n+=8;
if(n>542)return -90;
}
{
if(!isFinite(this.acceleration))return -22;
if(8>542-n)return -90;n+=8;
if(n>542)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.joint_name);
w.integer(<u64>this.component,2);
w.integer(<u64>reinterpret<u64>(this.position),8);
w.integer(<u64>reinterpret<u64>(this.velocity),8);
w.integer(<u64>reinterpret<u64>(this.acceleration),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Control_msgsMsgJointComponentTolerance{const value=new Ros2Control_msgsMsgJointComponentTolerance();
{
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_name=decoded;
}
{
const decoded=<u16>r.integer(2);
value.component=decoded;
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
export class Ros2Control_msgsMsgJointComponentToleranceCodec implements VendorCodec<Ros2Control_msgsMsgJointComponentTolerance>{
static readonly codecIdentity:string="UWVMROS1_658f3d3f180b301e5c612188390187ec4034064976f133018048bb1fdec55342";
static readonly maxWireBytes:i32=542;
encodeChecked(value:Ros2Control_msgsMsgJointComponentTolerance):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Control_msgsMsgJointComponentTolerance):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Control_msgsMsgJointComponentTolerance>{const out=new VendorDecoded<Ros2Control_msgsMsgJointComponentTolerance>();if(bytes.length>542){out.status=90;return out;}if(bytes.length<30){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Control_msgsMsgJointComponentTolerance.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Control_msgsMsgJointComponentTolerance{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Control_msgsMsgJointComponentTolerance():changetype<Ros2Control_msgsMsgJointComponentTolerance>(out.value);}
}
/** Complete original control_msgs/action/FollowJointTrajectory_Goal; spellings are published in generation proof. */
export class Ros2Control_msgsActionFollowJointTrajectory_Goal {
static readonly maxWireBytes:i32=20920;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=20920;
/** Exact native field trajectory, native scalar trajectory_msgs/msg/JointTrajectory. */
trajectory:Ros2Trajectory_msgsMsgJointTrajectory=new Ros2Trajectory_msgsMsgJointTrajectory();
/** Exact native field multi_dof_trajectory, native scalar trajectory_msgs/msg/MultiDOFJointTrajectory. */
multi_dof_trajectory:Ros2Trajectory_msgsMsgMultiDOFJointTrajectory=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectory();
/** Exact native field path_tolerance, native scalar control_msgs/msg/JointTolerance. */
path_tolerance:Array<Ros2Control_msgsMsgJointTolerance>=new Array<Ros2Control_msgsMsgJointTolerance>();
/** Exact native field component_path_tolerance, native scalar control_msgs/msg/JointComponentTolerance. */
component_path_tolerance:Array<Ros2Control_msgsMsgJointComponentTolerance>=new Array<Ros2Control_msgsMsgJointComponentTolerance>();
/** Exact native field goal_tolerance, native scalar control_msgs/msg/JointTolerance. */
goal_tolerance:Array<Ros2Control_msgsMsgJointTolerance>=new Array<Ros2Control_msgsMsgJointTolerance>();
/** Exact native field component_goal_tolerance, native scalar control_msgs/msg/JointComponentTolerance. */
component_goal_tolerance:Array<Ros2Control_msgsMsgJointComponentTolerance>=new Array<Ros2Control_msgsMsgJointComponentTolerance>();
/** Exact native field goal_time_tolerance, native scalar builtin_interfaces/msg/Duration. */
goal_time_tolerance:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.trajectory.wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
{
const child=this.multi_dof_trajectory.wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
{
if(this.path_tolerance.length>12)return -22;
if(4>20920-n)return -90;n+=4;
for(let i=0;i<this.path_tolerance.length;++i){
const child=this.path_tolerance[i].wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
}
{
if(this.component_path_tolerance.length>12)return -22;
if(4>20920-n)return -90;n+=4;
for(let i=0;i<this.component_path_tolerance.length;++i){
const child=this.component_path_tolerance[i].wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
}
{
if(this.goal_tolerance.length>12)return -22;
if(4>20920-n)return -90;n+=4;
for(let i=0;i<this.goal_tolerance.length;++i){
const child=this.goal_tolerance[i].wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
}
{
if(this.component_goal_tolerance.length>12)return -22;
if(4>20920-n)return -90;n+=4;
for(let i=0;i<this.component_goal_tolerance.length;++i){
const child=this.component_goal_tolerance[i].wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
}
{
const child=this.goal_time_tolerance.wireSize();if(child<0)return child;if(child>20920-n)return -90;n+=child;
if(n>20920)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.trajectory.writeControl(w);
this.multi_dof_trajectory.writeControl(w);
w.u32(<u32>this.path_tolerance.length);
for(let i=0;i<this.path_tolerance.length;++i){
this.path_tolerance[i].writeControl(w);
}
w.u32(<u32>this.component_path_tolerance.length);
for(let i=0;i<this.component_path_tolerance.length;++i){
this.component_path_tolerance[i].writeControl(w);
}
w.u32(<u32>this.goal_tolerance.length);
for(let i=0;i<this.goal_tolerance.length;++i){
this.goal_tolerance[i].writeControl(w);
}
w.u32(<u32>this.component_goal_tolerance.length);
for(let i=0;i<this.component_goal_tolerance.length;++i){
this.component_goal_tolerance[i].writeControl(w);
}
this.goal_time_tolerance.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Control_msgsActionFollowJointTrajectory_Goal{const value=new Ros2Control_msgsActionFollowJointTrajectory_Goal();
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectory.readControl(r);
value.trajectory=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectory.readControl(r);
value.multi_dof_trajectory=decoded;
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.path_tolerance=new Array<Ros2Control_msgsMsgJointTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointTolerance.readControl(r);
value.path_tolerance.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.component_path_tolerance=new Array<Ros2Control_msgsMsgJointComponentTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointComponentTolerance.readControl(r);
value.component_path_tolerance.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.goal_tolerance=new Array<Ros2Control_msgsMsgJointTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointTolerance.readControl(r);
value.goal_tolerance.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>12){r.valid=false;return value;}
value.component_goal_tolerance=new Array<Ros2Control_msgsMsgJointComponentTolerance>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Control_msgsMsgJointComponentTolerance.readControl(r);
value.component_goal_tolerance.push(decoded);
}
}
{
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.goal_time_tolerance=decoded;
}
return value;}
}
export class Ros2Control_msgsActionFollowJointTrajectory_GoalCodec implements VendorCodec<Ros2Control_msgsActionFollowJointTrajectory_Goal>{
static readonly codecIdentity:string="UWVMROS1_8945d5b29832f1190b0a4f8c4af4e82686c4b913413affe9c0945570b4fb1f16";
static readonly maxWireBytes:i32=20920;
encodeChecked(value:Ros2Control_msgsActionFollowJointTrajectory_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Control_msgsActionFollowJointTrajectory_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Goal>{const out=new VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Goal>();if(bytes.length>20920){out.status=90;return out;}if(bytes.length<64){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Control_msgsActionFollowJointTrajectory_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Control_msgsActionFollowJointTrajectory_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Control_msgsActionFollowJointTrajectory_Goal():changetype<Ros2Control_msgsActionFollowJointTrajectory_Goal>(out.value);}
}
/** Complete original control_msgs/action/FollowJointTrajectory_Result; spellings are published in generation proof. */
export class Ros2Control_msgsActionFollowJointTrajectory_Result {
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
static readControl(r:Reader):Ros2Control_msgsActionFollowJointTrajectory_Result{const value=new Ros2Control_msgsActionFollowJointTrajectory_Result();
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
export class Ros2Control_msgsActionFollowJointTrajectory_ResultCodec implements VendorCodec<Ros2Control_msgsActionFollowJointTrajectory_Result>{
static readonly codecIdentity:string="UWVMROS1_c698e3a3a772735ff6027b04af28e9bc88d6b7138863ce5737aa090f543dac70";
static readonly maxWireBytes:i32=4104;
encodeChecked(value:Ros2Control_msgsActionFollowJointTrajectory_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Control_msgsActionFollowJointTrajectory_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Result>{const out=new VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Result>();if(bytes.length>4104){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Control_msgsActionFollowJointTrajectory_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Control_msgsActionFollowJointTrajectory_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Control_msgsActionFollowJointTrajectory_Result():changetype<Ros2Control_msgsActionFollowJointTrajectory_Result>(out.value);}
}
/** Complete original control_msgs/action/FollowJointTrajectory_Feedback; spellings are published in generation proof. */
export class Ros2Control_msgsActionFollowJointTrajectory_Feedback {
static readonly maxWireBytes:i32=4128;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4128;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field joint_names, native scalar string. */
joint_names:Array<string>=new Array<string>();
/** Exact native field desired, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
desired:Ros2Trajectory_msgsMsgJointTrajectoryPoint=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
/** Exact native field actual, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
actual:Ros2Trajectory_msgsMsgJointTrajectoryPoint=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
/** Exact native field error, native scalar trajectory_msgs/msg/JointTrajectoryPoint. */
error:Ros2Trajectory_msgsMsgJointTrajectoryPoint=new Ros2Trajectory_msgsMsgJointTrajectoryPoint();
/** Exact native field multi_dof_joint_names, native scalar string. */
multi_dof_joint_names:Array<string>=new Array<string>();
/** Exact native field multi_dof_desired, native scalar trajectory_msgs/msg/MultiDOFJointTrajectoryPoint. */
multi_dof_desired:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint();
/** Exact native field multi_dof_actual, native scalar trajectory_msgs/msg/MultiDOFJointTrajectoryPoint. */
multi_dof_actual:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint();
/** Exact native field multi_dof_error, native scalar trajectory_msgs/msg/MultiDOFJointTrajectoryPoint. */
multi_dof_error:Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint=new Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
if(this.joint_names.length>12)return -22;
if(4>4128-n)return -90;n+=4;
for(let i=0;i<this.joint_names.length;++i){
if(!validUnicode(this.joint_names[i])||this.joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_names[i]);if(size>64)return -22;if(4+size>4128-n)return -90;n+=4+size;
if(n>4128)return -90;
}
}
{
const child=this.desired.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
const child=this.actual.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
const child=this.error.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
if(this.multi_dof_joint_names.length>2)return -22;
if(4>4128-n)return -90;n+=4;
for(let i=0;i<this.multi_dof_joint_names.length;++i){
if(!validUnicode(this.multi_dof_joint_names[i])||this.multi_dof_joint_names[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.multi_dof_joint_names[i]);if(size>512)return -22;if(4+size>4128-n)return -90;n+=4+size;
if(n>4128)return -90;
}
}
{
const child=this.multi_dof_desired.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
const child=this.multi_dof_actual.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
}
{
const child=this.multi_dof_error.wireSize();if(child<0)return child;if(child>4128-n)return -90;n+=child;
if(n>4128)return -90;
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
w.u32(<u32>this.multi_dof_joint_names.length);
for(let i=0;i<this.multi_dof_joint_names.length;++i){
w.text(this.multi_dof_joint_names[i]);
}
this.multi_dof_desired.writeControl(w);
this.multi_dof_actual.writeControl(w);
this.multi_dof_error.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Control_msgsActionFollowJointTrajectory_Feedback{const value=new Ros2Control_msgsActionFollowJointTrajectory_Feedback();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
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
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.desired=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.actual=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgJointTrajectoryPoint.readControl(r);
value.error=decoded;
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.multi_dof_joint_names=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.multi_dof_joint_names.push(decoded);
}
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint.readControl(r);
value.multi_dof_desired=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint.readControl(r);
value.multi_dof_actual=decoded;
}
{
const decoded=Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint.readControl(r);
value.multi_dof_error=decoded;
}
return value;}
}
export class Ros2Control_msgsActionFollowJointTrajectory_FeedbackCodec implements VendorCodec<Ros2Control_msgsActionFollowJointTrajectory_Feedback>{
static readonly codecIdentity:string="UWVMROS1_68f3809217bd914cd971e1365ca825111c1394b2587c6989736adcaa8a40a7cb";
static readonly maxWireBytes:i32=4128;
encodeChecked(value:Ros2Control_msgsActionFollowJointTrajectory_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Control_msgsActionFollowJointTrajectory_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Feedback>{const out=new VendorDecoded<Ros2Control_msgsActionFollowJointTrajectory_Feedback>();if(bytes.length>4128){out.status=90;return out;}if(bytes.length<152){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Control_msgsActionFollowJointTrajectory_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Control_msgsActionFollowJointTrajectory_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Control_msgsActionFollowJointTrajectory_Feedback():changetype<Ros2Control_msgsActionFollowJointTrajectory_Feedback>(out.value);}
}
/** Complete original sensor_msgs/msg/JointState; spellings are published in generation proof. */
export class Ros2Sensor_msgsMsgJointState {
static readonly maxWireBytes:i32=1196;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1196;
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
const child=this.header.wireSize();if(child<0)return child;if(child>1196-n)return -90;n+=child;
if(n>1196)return -90;
}
{
if(this.name.length>12)return -22;
if(4>1196-n)return -90;n+=4;
for(let i=0;i<this.name.length;++i){
if(!validUnicode(this.name[i])||this.name[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.name[i]);if(size>64)return -22;if(4+size>1196-n)return -90;n+=4+size;
if(n>1196)return -90;
}
}
{
if(this.position.length>12)return -22;
if(4>1196-n)return -90;n+=4;
for(let i=0;i<this.position.length;++i){
if(!isFinite(this.position[i]))return -22;
if(8>1196-n)return -90;n+=8;
if(n>1196)return -90;
}
}
{
if(this.velocity.length>12)return -22;
if(4>1196-n)return -90;n+=4;
for(let i=0;i<this.velocity.length;++i){
if(!isFinite(this.velocity[i]))return -22;
if(8>1196-n)return -90;n+=8;
if(n>1196)return -90;
}
}
{
if(this.effort.length>12)return -22;
if(4>1196-n)return -90;n+=4;
for(let i=0;i<this.effort.length;++i){
if(!isFinite(this.effort[i]))return -22;
if(8>1196-n)return -90;n+=8;
if(n>1196)return -90;
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
export class Ros2Sensor_msgsMsgJointStateCodec implements VendorCodec<Ros2Sensor_msgsMsgJointState>{
static readonly codecIdentity:string="UWVMROS1_88d44f7bdb40f8c45539a041a25903609561fa74db90b9972e1369ebbff6c884";
static readonly maxWireBytes:i32=1196;
encodeChecked(value:Ros2Sensor_msgsMsgJointState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Sensor_msgsMsgJointState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Sensor_msgsMsgJointState>{const out=new VendorDecoded<Ros2Sensor_msgsMsgJointState>();if(bytes.length>1196){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Sensor_msgsMsgJointState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Sensor_msgsMsgJointState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Sensor_msgsMsgJointState():changetype<Ros2Sensor_msgsMsgJointState>(out.value);}
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
static readonly maxWireBytes:i32=517;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=517;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>517-n)return -90;n+=1;
if(n>517)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>512)return -22;if(4+size>517-n)return -90;n+=4+size;
if(n>517)return -90;
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
const size=r.u32();if(!r.valid||size>512){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.message=decoded;
}
return value;}
}
export class Ros2Std_srvsSrvTrigger_ResponseCodec implements VendorCodec<Ros2Std_srvsSrvTrigger_Response>{
static readonly codecIdentity:string="UWVMROS1_95fe65bd59112bb00093f66c6853844b01ccea8e6ded47e73038718aa886f39d";
static readonly maxWireBytes:i32=517;
encodeChecked(value:Ros2Std_srvsSrvTrigger_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Std_srvsSrvTrigger_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Std_srvsSrvTrigger_Response>{const out=new VendorDecoded<Ros2Std_srvsSrvTrigger_Response>();if(bytes.length>517){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Std_srvsSrvTrigger_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Std_srvsSrvTrigger_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Std_srvsSrvTrigger_Response():changetype<Ros2Std_srvsSrvTrigger_Response>(out.value);}
}
