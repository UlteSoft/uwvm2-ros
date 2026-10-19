/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original uwvm_booster_sdk_bridge/msg/ImuState; spellings are published in generation proof. */
export class Ros2Uwvm_booster_sdk_bridgeMsgImuState {
static readonly maxWireBytes:i32=36;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=36;
/** Exact native field rpy, native scalar float32. */
rpy:Array<f32>=new Array<f32>();
/** Exact native field gyro, native scalar float32. */
gyro:Array<f32>=new Array<f32>();
/** Exact native field acc, native scalar float32. */
acc:Array<f32>=new Array<f32>();
constructor(){
for(let i=0;i<3;++i)this.rpy.push(0);
for(let i=0;i<3;++i)this.gyro.push(0);
for(let i=0;i<3;++i)this.acc.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.rpy.length!=3)return -22;
for(let i=0;i<this.rpy.length;++i){
if(!isFinite(this.rpy[i]))return -22;
if(4>36-n)return -90;n+=4;
if(n>36)return -90;
}
}
{
if(this.gyro.length!=3)return -22;
for(let i=0;i<this.gyro.length;++i){
if(!isFinite(this.gyro[i]))return -22;
if(4>36-n)return -90;n+=4;
if(n>36)return -90;
}
}
{
if(this.acc.length!=3)return -22;
for(let i=0;i<this.acc.length;++i){
if(!isFinite(this.acc[i]))return -22;
if(4>36-n)return -90;n+=4;
if(n>36)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.rpy.length;++i){
w.integer(<u64>reinterpret<u32>(this.rpy[i]),4);
}
for(let i=0;i<this.gyro.length;++i){
w.integer(<u64>reinterpret<u32>(this.gyro[i]),4);
}
for(let i=0;i<this.acc.length;++i){
w.integer(<u64>reinterpret<u32>(this.acc[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_booster_sdk_bridgeMsgImuState{const value=new Ros2Uwvm_booster_sdk_bridgeMsgImuState();
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.rpy=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rpy.push(decoded);
}
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.gyro=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.gyro.push(decoded);
}
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.acc=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.acc.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_booster_sdk_bridgeMsgImuStateCodec implements VendorCodec<Ros2Uwvm_booster_sdk_bridgeMsgImuState>{
static readonly codecIdentity:string="UWVMROS1_858ad4a47d2409da6b4e2513178d7d3b0d4568d6ce488729de0b9a220a100f04";
static readonly maxWireBytes:i32=36;
encodeChecked(value:Ros2Uwvm_booster_sdk_bridgeMsgImuState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_booster_sdk_bridgeMsgImuState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_booster_sdk_bridgeMsgImuState>{const out=new VendorDecoded<Ros2Uwvm_booster_sdk_bridgeMsgImuState>();if(bytes.length>36){out.status=90;return out;}if(bytes.length<36){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_booster_sdk_bridgeMsgImuState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_booster_sdk_bridgeMsgImuState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_booster_sdk_bridgeMsgImuState():changetype<Ros2Uwvm_booster_sdk_bridgeMsgImuState>(out.value);}
}
/** Complete original uwvm_booster_sdk_bridge/msg/MotorCmd; spellings are published in generation proof. */
export class Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact native field q, native scalar float32. */
q:f32=0;
/** Exact native field dq, native scalar float32. */
dq:f32=0;
/** Exact native field tau, native scalar float32. */
tau:f32=0;
/** Exact native field kp, native scalar float32. */
kp:f32=0;
/** Exact native field kd, native scalar float32. */
kd:f32=0;
/** Exact native field weight, native scalar float32. */
weight:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>25-n)return -90;n+=1;
if(n>25)return -90;
}
{
if(!isFinite(this.q))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.dq))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.tau))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.kp))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.kd))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.weight))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.mode,1);
w.integer(<u64>reinterpret<u32>(this.q),4);
w.integer(<u64>reinterpret<u32>(this.dq),4);
w.integer(<u64>reinterpret<u32>(this.tau),4);
w.integer(<u64>reinterpret<u32>(this.kp),4);
w.integer(<u64>reinterpret<u32>(this.kd),4);
w.integer(<u64>reinterpret<u32>(this.weight),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd{const value=new Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd();
{
const decoded=<u8>r.integer(1);
value.mode=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.q=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.dq=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.tau=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.kp=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.kd=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.weight=decoded;
}
return value;}
}
export class Ros2Uwvm_booster_sdk_bridgeMsgMotorCmdCodec implements VendorCodec<Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd>{
static readonly codecIdentity:string="UWVMROS1_66fec912031dfbd29b8e4e747dcdc2dc465506bd003323bc929929bf6a9d31a4";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd>{const out=new VendorDecoded<Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd():changetype<Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd>(out.value);}
}
/** Complete original uwvm_booster_sdk_bridge/msg/MotorState; spellings are published in generation proof. */
export class Ros2Uwvm_booster_sdk_bridgeMsgMotorState {
static readonly maxWireBytes:i32=30;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=30;
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact native field q, native scalar float32. */
q:f32=0;
/** Exact native field dq, native scalar float32. */
dq:f32=0;
/** Exact native field ddq, native scalar float32. */
ddq:f32=0;
/** Exact native field tau_est, native scalar float32. */
tau_est:f32=0;
/** Exact native field temperature, native scalar uint8. */
temperature:u8=0;
/** Exact native field lost, native scalar uint32. */
lost:u32=0;
/** Exact native field reserve, native scalar uint32. */
reserve:Array<u32>=new Array<u32>();
constructor(){
for(let i=0;i<2;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>30-n)return -90;n+=1;
if(n>30)return -90;
}
{
if(!isFinite(this.q))return -22;
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(!isFinite(this.dq))return -22;
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(!isFinite(this.ddq))return -22;
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(!isFinite(this.tau_est))return -22;
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(1>30-n)return -90;n+=1;
if(n>30)return -90;
}
{
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
{
if(this.reserve.length!=2)return -22;
for(let i=0;i<this.reserve.length;++i){
if(4>30-n)return -90;n+=4;
if(n>30)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.mode,1);
w.integer(<u64>reinterpret<u32>(this.q),4);
w.integer(<u64>reinterpret<u32>(this.dq),4);
w.integer(<u64>reinterpret<u32>(this.ddq),4);
w.integer(<u64>reinterpret<u32>(this.tau_est),4);
w.integer(<u64>this.temperature,1);
w.integer(<u64>this.lost,4);
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_booster_sdk_bridgeMsgMotorState{const value=new Ros2Uwvm_booster_sdk_bridgeMsgMotorState();
{
const decoded=<u8>r.integer(1);
value.mode=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.q=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.dq=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ddq=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.tau_est=decoded;
}
{
const decoded=<u8>r.integer(1);
value.temperature=decoded;
}
{
const decoded=<u32>r.integer(4);
value.lost=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.reserve=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.reserve.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_booster_sdk_bridgeMsgMotorStateCodec implements VendorCodec<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>{
static readonly codecIdentity:string="UWVMROS1_958168dc1e32d50827f255080117a82421d7fb6ae4ad6a05763ce2d3173a30af";
static readonly maxWireBytes:i32=30;
encodeChecked(value:Ros2Uwvm_booster_sdk_bridgeMsgMotorState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_booster_sdk_bridgeMsgMotorState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>{const out=new VendorDecoded<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>();if(bytes.length>30){out.status=90;return out;}if(bytes.length<30){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_booster_sdk_bridgeMsgMotorState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_booster_sdk_bridgeMsgMotorState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_booster_sdk_bridgeMsgMotorState():changetype<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>(out.value);}
}
/** Complete original uwvm_booster_sdk_bridge/msg/LowCmd; spellings are published in generation proof. */
export class Ros2Uwvm_booster_sdk_bridgeMsgLowCmd {
static readonly maxWireBytes:i32=733;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=733;
/** Exact native field cmd_type, native scalar int32. */
cmd_type:i32=0;
/** Exact native field motor_cmd, native scalar uwvm_booster_sdk_bridge/msg/MotorCmd. */
motor_cmd:Array<Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd>=new Array<Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>733-n)return -90;n+=4;
if(n>733)return -90;
}
{
if(this.motor_cmd.length>29)return -22;
if(4>733-n)return -90;n+=4;
for(let i=0;i<this.motor_cmd.length;++i){
const child=this.motor_cmd[i].wireSize();if(child<0)return child;if(child>733-n)return -90;n+=child;
if(n>733)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.cmd_type,4);
w.u32(<u32>this.motor_cmd.length);
for(let i=0;i<this.motor_cmd.length;++i){
this.motor_cmd[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_booster_sdk_bridgeMsgLowCmd{const value=new Ros2Uwvm_booster_sdk_bridgeMsgLowCmd();
{
const decoded=<i32>r.integer(4);
value.cmd_type=decoded;
}
{
const count=r.u32();if(!r.valid||count>29){r.valid=false;return value;}
value.motor_cmd=new Array<Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Uwvm_booster_sdk_bridgeMsgMotorCmd.readControl(r);
value.motor_cmd.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_booster_sdk_bridgeMsgLowCmdCodec implements VendorCodec<Ros2Uwvm_booster_sdk_bridgeMsgLowCmd>{
static readonly codecIdentity:string="UWVMROS1_3324a161b056f9282ffa8dce3254a1456b75f6eae5f4a067e844e125e94cba3a";
static readonly maxWireBytes:i32=733;
encodeChecked(value:Ros2Uwvm_booster_sdk_bridgeMsgLowCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_booster_sdk_bridgeMsgLowCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_booster_sdk_bridgeMsgLowCmd>{const out=new VendorDecoded<Ros2Uwvm_booster_sdk_bridgeMsgLowCmd>();if(bytes.length>733){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_booster_sdk_bridgeMsgLowCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_booster_sdk_bridgeMsgLowCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_booster_sdk_bridgeMsgLowCmd():changetype<Ros2Uwvm_booster_sdk_bridgeMsgLowCmd>(out.value);}
}
/** Complete original uwvm_booster_sdk_bridge/msg/LowState; spellings are published in generation proof. */
export class Ros2Uwvm_booster_sdk_bridgeMsgLowState {
static readonly maxWireBytes:i32=1784;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1784;
/** Exact native field imu_state, native scalar uwvm_booster_sdk_bridge/msg/ImuState. */
imu_state:Ros2Uwvm_booster_sdk_bridgeMsgImuState=new Ros2Uwvm_booster_sdk_bridgeMsgImuState();
/** Exact native field motor_state_parallel, native scalar uwvm_booster_sdk_bridge/msg/MotorState. */
motor_state_parallel:Array<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>=new Array<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>();
/** Exact native field motor_state_serial, native scalar uwvm_booster_sdk_bridge/msg/MotorState. */
motor_state_serial:Array<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>=new Array<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.imu_state.wireSize();if(child<0)return child;if(child>1784-n)return -90;n+=child;
if(n>1784)return -90;
}
{
if(this.motor_state_parallel.length>29)return -22;
if(4>1784-n)return -90;n+=4;
for(let i=0;i<this.motor_state_parallel.length;++i){
const child=this.motor_state_parallel[i].wireSize();if(child<0)return child;if(child>1784-n)return -90;n+=child;
if(n>1784)return -90;
}
}
{
if(this.motor_state_serial.length>29)return -22;
if(4>1784-n)return -90;n+=4;
for(let i=0;i<this.motor_state_serial.length;++i){
const child=this.motor_state_serial[i].wireSize();if(child<0)return child;if(child>1784-n)return -90;n+=child;
if(n>1784)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.imu_state.writeControl(w);
w.u32(<u32>this.motor_state_parallel.length);
for(let i=0;i<this.motor_state_parallel.length;++i){
this.motor_state_parallel[i].writeControl(w);
}
w.u32(<u32>this.motor_state_serial.length);
for(let i=0;i<this.motor_state_serial.length;++i){
this.motor_state_serial[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_booster_sdk_bridgeMsgLowState{const value=new Ros2Uwvm_booster_sdk_bridgeMsgLowState();
{
const decoded=Ros2Uwvm_booster_sdk_bridgeMsgImuState.readControl(r);
value.imu_state=decoded;
}
{
const count=r.u32();if(!r.valid||count>29){r.valid=false;return value;}
value.motor_state_parallel=new Array<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Uwvm_booster_sdk_bridgeMsgMotorState.readControl(r);
value.motor_state_parallel.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>29){r.valid=false;return value;}
value.motor_state_serial=new Array<Ros2Uwvm_booster_sdk_bridgeMsgMotorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Uwvm_booster_sdk_bridgeMsgMotorState.readControl(r);
value.motor_state_serial.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_booster_sdk_bridgeMsgLowStateCodec implements VendorCodec<Ros2Uwvm_booster_sdk_bridgeMsgLowState>{
static readonly codecIdentity:string="UWVMROS1_ca4bcb79077b0e026cf45a389981622ff9c80570ee813ff5633c605fe5021a58";
static readonly maxWireBytes:i32=1784;
encodeChecked(value:Ros2Uwvm_booster_sdk_bridgeMsgLowState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_booster_sdk_bridgeMsgLowState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_booster_sdk_bridgeMsgLowState>{const out=new VendorDecoded<Ros2Uwvm_booster_sdk_bridgeMsgLowState>();if(bytes.length>1784){out.status=90;return out;}if(bytes.length<44){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_booster_sdk_bridgeMsgLowState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_booster_sdk_bridgeMsgLowState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_booster_sdk_bridgeMsgLowState():changetype<Ros2Uwvm_booster_sdk_bridgeMsgLowState>(out.value);}
}
