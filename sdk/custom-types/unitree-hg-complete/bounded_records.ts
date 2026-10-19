/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original unitree_hg/msg/BmsCmd; spellings are published in generation proof. */
export class Ros2Unitree_hgMsgBmsCmd {
static readonly maxWireBytes:i32=41;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=41;
/** Exact native field cmd, native scalar uint8. */
cmd:u8=0;
/** Exact native field reserve, native scalar uint8. */
reserve:Array<u8>=new Array<u8>();
constructor(){
for(let i=0;i<40;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
{
if(this.reserve.length!=40)return -22;
for(let i=0;i<this.reserve.length;++i){
if(1>41-n)return -90;n+=1;
if(n>41)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.cmd,1);
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_hgMsgBmsCmd{const value=new Ros2Unitree_hgMsgBmsCmd();
{
const decoded=<u8>r.integer(1);
value.cmd=decoded;
}
{
const count:u32=40;if(!r.valid||count>40){r.valid=false;return value;}
value.reserve=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.reserve.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_hgMsgBmsCmdCodec implements VendorCodec<Ros2Unitree_hgMsgBmsCmd>{
static readonly codecIdentity:string="UWVMROS1_a267c92d22aa04a15f7d68c87b7638b15a368ac973f49a42021dc91f991b13de";
static readonly maxWireBytes:i32=41;
encodeChecked(value:Ros2Unitree_hgMsgBmsCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_hgMsgBmsCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_hgMsgBmsCmd>{const out=new VendorDecoded<Ros2Unitree_hgMsgBmsCmd>();if(bytes.length>41){out.status=90;return out;}if(bytes.length<41){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_hgMsgBmsCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_hgMsgBmsCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_hgMsgBmsCmd():changetype<Ros2Unitree_hgMsgBmsCmd>(out.value);}
}
/** Complete original unitree_hg/msg/BmsState; spellings are published in generation proof. */
export class Ros2Unitree_hgMsgBmsState {
static readonly maxWireBytes:i32=161;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=161;
/** Exact native field version_high, native scalar uint8. */
version_high:u8=0;
/** Exact native field version_low, native scalar uint8. */
version_low:u8=0;
/** Exact native field fn, native scalar uint8. */
fn:u8=0;
/** Exact native field cell_vol, native scalar uint16. */
cell_vol:Array<u16>=new Array<u16>();
/** Exact native field bmsvoltage, native scalar uint32. */
bmsvoltage:Array<u32>=new Array<u32>();
/** Exact native field current, native scalar int32. */
current:i32=0;
/** Exact native field soc, native scalar uint8. */
soc:u8=0;
/** Exact native field soh, native scalar uint8. */
soh:u8=0;
/** Exact native field temperature, native scalar int16. */
temperature:Array<i16>=new Array<i16>();
/** Exact native field cycle, native scalar uint16. */
cycle:u16=0;
/** Exact native field manufacturer_date, native scalar uint16. */
manufacturer_date:u16=0;
/** Exact native field bmsstate, native scalar uint32. */
bmsstate:Array<u32>=new Array<u32>();
/** Exact native field reserve, native scalar uint32. */
reserve:Array<u32>=new Array<u32>();
constructor(){
for(let i=0;i<40;++i)this.cell_vol.push(0);
for(let i=0;i<3;++i)this.bmsvoltage.push(0);
for(let i=0;i<12;++i)this.temperature.push(0);
for(let i=0;i<5;++i)this.bmsstate.push(0);
for(let i=0;i<3;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>161-n)return -90;n+=1;
if(n>161)return -90;
}
{
if(1>161-n)return -90;n+=1;
if(n>161)return -90;
}
{
if(1>161-n)return -90;n+=1;
if(n>161)return -90;
}
{
if(this.cell_vol.length!=40)return -22;
for(let i=0;i<this.cell_vol.length;++i){
if(2>161-n)return -90;n+=2;
if(n>161)return -90;
}
}
{
if(this.bmsvoltage.length!=3)return -22;
for(let i=0;i<this.bmsvoltage.length;++i){
if(4>161-n)return -90;n+=4;
if(n>161)return -90;
}
}
{
if(4>161-n)return -90;n+=4;
if(n>161)return -90;
}
{
if(1>161-n)return -90;n+=1;
if(n>161)return -90;
}
{
if(1>161-n)return -90;n+=1;
if(n>161)return -90;
}
{
if(this.temperature.length!=12)return -22;
for(let i=0;i<this.temperature.length;++i){
if(2>161-n)return -90;n+=2;
if(n>161)return -90;
}
}
{
if(2>161-n)return -90;n+=2;
if(n>161)return -90;
}
{
if(2>161-n)return -90;n+=2;
if(n>161)return -90;
}
{
if(this.bmsstate.length!=5)return -22;
for(let i=0;i<this.bmsstate.length;++i){
if(4>161-n)return -90;n+=4;
if(n>161)return -90;
}
}
{
if(this.reserve.length!=3)return -22;
for(let i=0;i<this.reserve.length;++i){
if(4>161-n)return -90;n+=4;
if(n>161)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.version_high,1);
w.integer(<u64>this.version_low,1);
w.integer(<u64>this.fn,1);
for(let i=0;i<this.cell_vol.length;++i){
w.integer(<u64>this.cell_vol[i],2);
}
for(let i=0;i<this.bmsvoltage.length;++i){
w.integer(<u64>this.bmsvoltage[i],4);
}
w.integer(<u64>this.current,4);
w.integer(<u64>this.soc,1);
w.integer(<u64>this.soh,1);
for(let i=0;i<this.temperature.length;++i){
w.integer(<u64>this.temperature[i],2);
}
w.integer(<u64>this.cycle,2);
w.integer(<u64>this.manufacturer_date,2);
for(let i=0;i<this.bmsstate.length;++i){
w.integer(<u64>this.bmsstate[i],4);
}
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_hgMsgBmsState{const value=new Ros2Unitree_hgMsgBmsState();
{
const decoded=<u8>r.integer(1);
value.version_high=decoded;
}
{
const decoded=<u8>r.integer(1);
value.version_low=decoded;
}
{
const decoded=<u8>r.integer(1);
value.fn=decoded;
}
{
const count:u32=40;if(!r.valid||count>40){r.valid=false;return value;}
value.cell_vol=new Array<u16>();for(let i:u32=0;i<count;++i){
const decoded=<u16>r.integer(2);
value.cell_vol.push(decoded);
}
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.bmsvoltage=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.bmsvoltage.push(decoded);
}
}
{
const decoded=<i32>r.integer(4);
value.current=decoded;
}
{
const decoded=<u8>r.integer(1);
value.soc=decoded;
}
{
const decoded=<u8>r.integer(1);
value.soh=decoded;
}
{
const count:u32=12;if(!r.valid||count>12){r.valid=false;return value;}
value.temperature=new Array<i16>();for(let i:u32=0;i<count;++i){
const decoded=<i16>r.integer(2);
value.temperature.push(decoded);
}
}
{
const decoded=<u16>r.integer(2);
value.cycle=decoded;
}
{
const decoded=<u16>r.integer(2);
value.manufacturer_date=decoded;
}
{
const count:u32=5;if(!r.valid||count>5){r.valid=false;return value;}
value.bmsstate=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.bmsstate.push(decoded);
}
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.reserve=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.reserve.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_hgMsgBmsStateCodec implements VendorCodec<Ros2Unitree_hgMsgBmsState>{
static readonly codecIdentity:string="UWVMROS1_b7ac163c4ab4786d37a9ec027767258adcfc06637ed8b4ef67e30c517397761d";
static readonly maxWireBytes:i32=161;
encodeChecked(value:Ros2Unitree_hgMsgBmsState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_hgMsgBmsState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_hgMsgBmsState>{const out=new VendorDecoded<Ros2Unitree_hgMsgBmsState>();if(bytes.length>161){out.status=90;return out;}if(bytes.length<161){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_hgMsgBmsState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_hgMsgBmsState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_hgMsgBmsState():changetype<Ros2Unitree_hgMsgBmsState>(out.value);}
}
/** Complete original unitree_hg/msg/MotorCmd; spellings are published in generation proof. */
export class Ros2Unitree_hgMsgMotorCmd {
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
/** Exact native field reserve, native scalar uint32. */
reserve:u32=0;
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
w.integer(<u64>this.reserve,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_hgMsgMotorCmd{const value=new Ros2Unitree_hgMsgMotorCmd();
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
const decoded=<u32>r.integer(4);
value.reserve=decoded;
}
return value;}
}
export class Ros2Unitree_hgMsgMotorCmdCodec implements VendorCodec<Ros2Unitree_hgMsgMotorCmd>{
static readonly codecIdentity:string="UWVMROS1_f3e472df5d061a1515ac185f87b01f702ef2f2bbeff39e42c6c7088d1da26cbb";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Unitree_hgMsgMotorCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_hgMsgMotorCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_hgMsgMotorCmd>{const out=new VendorDecoded<Ros2Unitree_hgMsgMotorCmd>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_hgMsgMotorCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_hgMsgMotorCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_hgMsgMotorCmd():changetype<Ros2Unitree_hgMsgMotorCmd>(out.value);}
}
/** Complete original unitree_hg/msg/HandCmd; spellings are published in generation proof. */
export class Ros2Unitree_hgMsgHandCmd {
static readonly maxWireBytes:i32=895;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=895;
/** Exact native field motor_cmd, native scalar unitree_hg/msg/MotorCmd. */
motor_cmd:Array<Ros2Unitree_hgMsgMotorCmd>=new Array<Ros2Unitree_hgMsgMotorCmd>();
/** Exact native field reserve, native scalar uint32. */
reserve:Array<u32>=new Array<u32>();
constructor(){
for(let i=0;i<4;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.motor_cmd.length>35)return -22;
if(4>895-n)return -90;n+=4;
for(let i=0;i<this.motor_cmd.length;++i){
const child=this.motor_cmd[i].wireSize();if(child<0)return child;if(child>895-n)return -90;n+=child;
if(n>895)return -90;
}
}
{
if(this.reserve.length!=4)return -22;
for(let i=0;i<this.reserve.length;++i){
if(4>895-n)return -90;n+=4;
if(n>895)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.motor_cmd.length);
for(let i=0;i<this.motor_cmd.length;++i){
this.motor_cmd[i].writeControl(w);
}
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_hgMsgHandCmd{const value=new Ros2Unitree_hgMsgHandCmd();
{
const count=r.u32();if(!r.valid||count>35){r.valid=false;return value;}
value.motor_cmd=new Array<Ros2Unitree_hgMsgMotorCmd>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Unitree_hgMsgMotorCmd.readControl(r);
value.motor_cmd.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.reserve=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.reserve.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_hgMsgHandCmdCodec implements VendorCodec<Ros2Unitree_hgMsgHandCmd>{
static readonly codecIdentity:string="UWVMROS1_2ebc0e32e831a48e21f0b15f39cb477d999bf3a415e7863713ce37416020a835";
static readonly maxWireBytes:i32=895;
encodeChecked(value:Ros2Unitree_hgMsgHandCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_hgMsgHandCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_hgMsgHandCmd>{const out=new VendorDecoded<Ros2Unitree_hgMsgHandCmd>();if(bytes.length>895){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_hgMsgHandCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_hgMsgHandCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_hgMsgHandCmd():changetype<Ros2Unitree_hgMsgHandCmd>(out.value);}
}
/** Complete original unitree_hg/msg/MotorState; spellings are published in generation proof. */
export class Ros2Unitree_hgMsgMotorState {
static readonly maxWireBytes:i32=53;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=53;
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
/** Exact native field temperature, native scalar int16. */
temperature:Array<i16>=new Array<i16>();
/** Exact native field vol, native scalar float32. */
vol:f32=0;
/** Exact native field sensor, native scalar uint32. */
sensor:Array<u32>=new Array<u32>();
/** Exact native field motorstate, native scalar uint32. */
motorstate:u32=0;
/** Exact native field reserve, native scalar uint32. */
reserve:Array<u32>=new Array<u32>();
constructor(){
for(let i=0;i<2;++i)this.temperature.push(0);
for(let i=0;i<2;++i)this.sensor.push(0);
for(let i=0;i<4;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>53-n)return -90;n+=1;
if(n>53)return -90;
}
{
if(!isFinite(this.q))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.dq))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.ddq))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.tau_est))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(this.temperature.length!=2)return -22;
for(let i=0;i<this.temperature.length;++i){
if(2>53-n)return -90;n+=2;
if(n>53)return -90;
}
}
{
if(!isFinite(this.vol))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(this.sensor.length!=2)return -22;
for(let i=0;i<this.sensor.length;++i){
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
}
{
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(this.reserve.length!=4)return -22;
for(let i=0;i<this.reserve.length;++i){
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
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
for(let i=0;i<this.temperature.length;++i){
w.integer(<u64>this.temperature[i],2);
}
w.integer(<u64>reinterpret<u32>(this.vol),4);
for(let i=0;i<this.sensor.length;++i){
w.integer(<u64>this.sensor[i],4);
}
w.integer(<u64>this.motorstate,4);
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_hgMsgMotorState{const value=new Ros2Unitree_hgMsgMotorState();
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
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.temperature=new Array<i16>();for(let i:u32=0;i<count;++i){
const decoded=<i16>r.integer(2);
value.temperature.push(decoded);
}
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.vol=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.sensor=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.sensor.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.motorstate=decoded;
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.reserve=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.reserve.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_hgMsgMotorStateCodec implements VendorCodec<Ros2Unitree_hgMsgMotorState>{
static readonly codecIdentity:string="UWVMROS1_65497908fe0136a5c90f11b4a2cda2110507bcc8805f9347ac82be3b32fe5934";
static readonly maxWireBytes:i32=53;
encodeChecked(value:Ros2Unitree_hgMsgMotorState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_hgMsgMotorState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_hgMsgMotorState>{const out=new VendorDecoded<Ros2Unitree_hgMsgMotorState>();if(bytes.length>53){out.status=90;return out;}if(bytes.length<53){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_hgMsgMotorState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_hgMsgMotorState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_hgMsgMotorState():changetype<Ros2Unitree_hgMsgMotorState>(out.value);}
}
/** Complete original unitree_hg/msg/PressSensorState; spellings are published in generation proof. */
export class Ros2Unitree_hgMsgPressSensorState {
static readonly maxWireBytes:i32=104;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=104;
/** Exact native field pressure, native scalar float32. */
pressure:Array<f32>=new Array<f32>();
/** Exact native field temperature, native scalar float32. */
temperature:Array<f32>=new Array<f32>();
/** Exact native field lost, native scalar uint32. */
lost:u32=0;
/** Exact native field reserve, native scalar uint32. */
reserve:u32=0;
constructor(){
for(let i=0;i<12;++i)this.pressure.push(0);
for(let i=0;i<12;++i)this.temperature.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.pressure.length!=12)return -22;
for(let i=0;i<this.pressure.length;++i){
if(!isFinite(this.pressure[i]))return -22;
if(4>104-n)return -90;n+=4;
if(n>104)return -90;
}
}
{
if(this.temperature.length!=12)return -22;
for(let i=0;i<this.temperature.length;++i){
if(!isFinite(this.temperature[i]))return -22;
if(4>104-n)return -90;n+=4;
if(n>104)return -90;
}
}
{
if(4>104-n)return -90;n+=4;
if(n>104)return -90;
}
{
if(4>104-n)return -90;n+=4;
if(n>104)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.pressure.length;++i){
w.integer(<u64>reinterpret<u32>(this.pressure[i]),4);
}
for(let i=0;i<this.temperature.length;++i){
w.integer(<u64>reinterpret<u32>(this.temperature[i]),4);
}
w.integer(<u64>this.lost,4);
w.integer(<u64>this.reserve,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_hgMsgPressSensorState{const value=new Ros2Unitree_hgMsgPressSensorState();
{
const count:u32=12;if(!r.valid||count>12){r.valid=false;return value;}
value.pressure=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.pressure.push(decoded);
}
}
{
const count:u32=12;if(!r.valid||count>12){r.valid=false;return value;}
value.temperature=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.temperature.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.lost=decoded;
}
{
const decoded=<u32>r.integer(4);
value.reserve=decoded;
}
return value;}
}
export class Ros2Unitree_hgMsgPressSensorStateCodec implements VendorCodec<Ros2Unitree_hgMsgPressSensorState>{
static readonly codecIdentity:string="UWVMROS1_4abc296d4acd21132c0c1d185e3fafd9f0db8836a5c16ed040ebcc6127380fe5";
static readonly maxWireBytes:i32=104;
encodeChecked(value:Ros2Unitree_hgMsgPressSensorState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_hgMsgPressSensorState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_hgMsgPressSensorState>{const out=new VendorDecoded<Ros2Unitree_hgMsgPressSensorState>();if(bytes.length>104){out.status=90;return out;}if(bytes.length<104){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_hgMsgPressSensorState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_hgMsgPressSensorState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_hgMsgPressSensorState():changetype<Ros2Unitree_hgMsgPressSensorState>(out.value);}
}
/** Complete original unitree_hg/msg/IMUState; spellings are published in generation proof. */
export class Ros2Unitree_hgMsgIMUState {
static readonly maxWireBytes:i32=54;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=54;
/** Exact native field quaternion, native scalar float32. */
quaternion:Array<f32>=new Array<f32>();
/** Exact native field gyroscope, native scalar float32. */
gyroscope:Array<f32>=new Array<f32>();
/** Exact native field accelerometer, native scalar float32. */
accelerometer:Array<f32>=new Array<f32>();
/** Exact native field rpy, native scalar float32. */
rpy:Array<f32>=new Array<f32>();
/** Exact native field temperature, native scalar int16. */
temperature:i16=0;
constructor(){
for(let i=0;i<4;++i)this.quaternion.push(0);
for(let i=0;i<3;++i)this.gyroscope.push(0);
for(let i=0;i<3;++i)this.accelerometer.push(0);
for(let i=0;i<3;++i)this.rpy.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.quaternion.length!=4)return -22;
for(let i=0;i<this.quaternion.length;++i){
if(!isFinite(this.quaternion[i]))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
}
{
if(this.gyroscope.length!=3)return -22;
for(let i=0;i<this.gyroscope.length;++i){
if(!isFinite(this.gyroscope[i]))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
}
{
if(this.accelerometer.length!=3)return -22;
for(let i=0;i<this.accelerometer.length;++i){
if(!isFinite(this.accelerometer[i]))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
}
{
if(this.rpy.length!=3)return -22;
for(let i=0;i<this.rpy.length;++i){
if(!isFinite(this.rpy[i]))return -22;
if(4>54-n)return -90;n+=4;
if(n>54)return -90;
}
}
{
if(2>54-n)return -90;n+=2;
if(n>54)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.quaternion.length;++i){
w.integer(<u64>reinterpret<u32>(this.quaternion[i]),4);
}
for(let i=0;i<this.gyroscope.length;++i){
w.integer(<u64>reinterpret<u32>(this.gyroscope[i]),4);
}
for(let i=0;i<this.accelerometer.length;++i){
w.integer(<u64>reinterpret<u32>(this.accelerometer[i]),4);
}
for(let i=0;i<this.rpy.length;++i){
w.integer(<u64>reinterpret<u32>(this.rpy[i]),4);
}
w.integer(<u64>this.temperature,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_hgMsgIMUState{const value=new Ros2Unitree_hgMsgIMUState();
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.quaternion=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.quaternion.push(decoded);
}
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.gyroscope=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.gyroscope.push(decoded);
}
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.accelerometer=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.accelerometer.push(decoded);
}
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.rpy=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rpy.push(decoded);
}
}
{
const decoded=<i16>r.integer(2);
value.temperature=decoded;
}
return value;}
}
export class Ros2Unitree_hgMsgIMUStateCodec implements VendorCodec<Ros2Unitree_hgMsgIMUState>{
static readonly codecIdentity:string="UWVMROS1_d83832e8c27273785113c8388f7f22ea1251d6198817255f13a7e542afb26420";
static readonly maxWireBytes:i32=54;
encodeChecked(value:Ros2Unitree_hgMsgIMUState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_hgMsgIMUState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_hgMsgIMUState>{const out=new VendorDecoded<Ros2Unitree_hgMsgIMUState>();if(bytes.length>54){out.status=90;return out;}if(bytes.length<54){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_hgMsgIMUState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_hgMsgIMUState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_hgMsgIMUState():changetype<Ros2Unitree_hgMsgIMUState>(out.value);}
}
/** Complete original unitree_hg/msg/HandState; spellings are published in generation proof. */
export class Ros2Unitree_hgMsgHandState {
static readonly maxWireBytes:i32=2365;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2365;
/** Exact native field motor_state, native scalar unitree_hg/msg/MotorState. */
motor_state:Array<Ros2Unitree_hgMsgMotorState>=new Array<Ros2Unitree_hgMsgMotorState>();
/** Exact native field press_sensor_state, native scalar unitree_hg/msg/PressSensorState. */
press_sensor_state:Array<Ros2Unitree_hgMsgPressSensorState>=new Array<Ros2Unitree_hgMsgPressSensorState>();
/** Exact native field imu_state, native scalar unitree_hg/msg/IMUState. */
imu_state:Ros2Unitree_hgMsgIMUState=new Ros2Unitree_hgMsgIMUState();
/** Exact native field power_v, native scalar float32. */
power_v:f32=0;
/** Exact native field power_a, native scalar float32. */
power_a:f32=0;
/** Exact native field system_v, native scalar float32. */
system_v:f32=0;
/** Exact native field device_v, native scalar float32. */
device_v:f32=0;
/** Exact native field error, native scalar uint32. */
error:Array<u32>=new Array<u32>();
/** Exact native field reserve, native scalar uint32. */
reserve:Array<u32>=new Array<u32>();
constructor(){
for(let i=0;i<2;++i)this.error.push(0);
for(let i=0;i<2;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.motor_state.length>35)return -22;
if(4>2365-n)return -90;n+=4;
for(let i=0;i<this.motor_state.length;++i){
const child=this.motor_state[i].wireSize();if(child<0)return child;if(child>2365-n)return -90;n+=child;
if(n>2365)return -90;
}
}
{
if(this.press_sensor_state.length>4)return -22;
if(4>2365-n)return -90;n+=4;
for(let i=0;i<this.press_sensor_state.length;++i){
const child=this.press_sensor_state[i].wireSize();if(child<0)return child;if(child>2365-n)return -90;n+=child;
if(n>2365)return -90;
}
}
{
const child=this.imu_state.wireSize();if(child<0)return child;if(child>2365-n)return -90;n+=child;
if(n>2365)return -90;
}
{
if(!isFinite(this.power_v))return -22;
if(4>2365-n)return -90;n+=4;
if(n>2365)return -90;
}
{
if(!isFinite(this.power_a))return -22;
if(4>2365-n)return -90;n+=4;
if(n>2365)return -90;
}
{
if(!isFinite(this.system_v))return -22;
if(4>2365-n)return -90;n+=4;
if(n>2365)return -90;
}
{
if(!isFinite(this.device_v))return -22;
if(4>2365-n)return -90;n+=4;
if(n>2365)return -90;
}
{
if(this.error.length!=2)return -22;
for(let i=0;i<this.error.length;++i){
if(4>2365-n)return -90;n+=4;
if(n>2365)return -90;
}
}
{
if(this.reserve.length!=2)return -22;
for(let i=0;i<this.reserve.length;++i){
if(4>2365-n)return -90;n+=4;
if(n>2365)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.motor_state.length);
for(let i=0;i<this.motor_state.length;++i){
this.motor_state[i].writeControl(w);
}
w.u32(<u32>this.press_sensor_state.length);
for(let i=0;i<this.press_sensor_state.length;++i){
this.press_sensor_state[i].writeControl(w);
}
this.imu_state.writeControl(w);
w.integer(<u64>reinterpret<u32>(this.power_v),4);
w.integer(<u64>reinterpret<u32>(this.power_a),4);
w.integer(<u64>reinterpret<u32>(this.system_v),4);
w.integer(<u64>reinterpret<u32>(this.device_v),4);
for(let i=0;i<this.error.length;++i){
w.integer(<u64>this.error[i],4);
}
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_hgMsgHandState{const value=new Ros2Unitree_hgMsgHandState();
{
const count=r.u32();if(!r.valid||count>35){r.valid=false;return value;}
value.motor_state=new Array<Ros2Unitree_hgMsgMotorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Unitree_hgMsgMotorState.readControl(r);
value.motor_state.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.press_sensor_state=new Array<Ros2Unitree_hgMsgPressSensorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Unitree_hgMsgPressSensorState.readControl(r);
value.press_sensor_state.push(decoded);
}
}
{
const decoded=Ros2Unitree_hgMsgIMUState.readControl(r);
value.imu_state=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.power_v=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.power_a=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.system_v=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.device_v=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.error=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.error.push(decoded);
}
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
export class Ros2Unitree_hgMsgHandStateCodec implements VendorCodec<Ros2Unitree_hgMsgHandState>{
static readonly codecIdentity:string="UWVMROS1_5721fdfc155f15ca331dbd398f07cb909196b07e93cabdb6dfbb67b10d4abc4a";
static readonly maxWireBytes:i32=2365;
encodeChecked(value:Ros2Unitree_hgMsgHandState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_hgMsgHandState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_hgMsgHandState>{const out=new VendorDecoded<Ros2Unitree_hgMsgHandState>();if(bytes.length>2365){out.status=90;return out;}if(bytes.length<94){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_hgMsgHandState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_hgMsgHandState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_hgMsgHandState():changetype<Ros2Unitree_hgMsgHandState>(out.value);}
}
/** Complete original unitree_hg/msg/LowCmd; spellings are published in generation proof. */
export class Ros2Unitree_hgMsgLowCmd {
static readonly maxWireBytes:i32=897;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=897;
/** Exact native field mode_pr, native scalar uint8. */
mode_pr:u8=0;
/** Exact native field mode_machine, native scalar uint8. */
mode_machine:u8=0;
/** Exact native field motor_cmd, native scalar unitree_hg/msg/MotorCmd. */
motor_cmd:Array<Ros2Unitree_hgMsgMotorCmd>=new Array<Ros2Unitree_hgMsgMotorCmd>();
/** Exact native field reserve, native scalar uint32. */
reserve:Array<u32>=new Array<u32>();
/** Exact native field crc, native scalar uint32. */
crc:u32=0;
constructor(){
for(let i=0;i<35;++i)this.motor_cmd.push(new Ros2Unitree_hgMsgMotorCmd());
for(let i=0;i<4;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>897-n)return -90;n+=1;
if(n>897)return -90;
}
{
if(1>897-n)return -90;n+=1;
if(n>897)return -90;
}
{
if(this.motor_cmd.length!=35)return -22;
for(let i=0;i<this.motor_cmd.length;++i){
const child=this.motor_cmd[i].wireSize();if(child<0)return child;if(child>897-n)return -90;n+=child;
if(n>897)return -90;
}
}
{
if(this.reserve.length!=4)return -22;
for(let i=0;i<this.reserve.length;++i){
if(4>897-n)return -90;n+=4;
if(n>897)return -90;
}
}
{
if(4>897-n)return -90;n+=4;
if(n>897)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.mode_pr,1);
w.integer(<u64>this.mode_machine,1);
for(let i=0;i<this.motor_cmd.length;++i){
this.motor_cmd[i].writeControl(w);
}
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],4);
}
w.integer(<u64>this.crc,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_hgMsgLowCmd{const value=new Ros2Unitree_hgMsgLowCmd();
{
const decoded=<u8>r.integer(1);
value.mode_pr=decoded;
}
{
const decoded=<u8>r.integer(1);
value.mode_machine=decoded;
}
{
const count:u32=35;if(!r.valid||count>35){r.valid=false;return value;}
value.motor_cmd=new Array<Ros2Unitree_hgMsgMotorCmd>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Unitree_hgMsgMotorCmd.readControl(r);
value.motor_cmd.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.reserve=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.reserve.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.crc=decoded;
}
return value;}
}
export class Ros2Unitree_hgMsgLowCmdCodec implements VendorCodec<Ros2Unitree_hgMsgLowCmd>{
static readonly codecIdentity:string="UWVMROS1_8303ac6e2e3273251662d31daf966a11ced5b5ac10b93aa6253eebc56f4ec461";
static readonly maxWireBytes:i32=897;
encodeChecked(value:Ros2Unitree_hgMsgLowCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_hgMsgLowCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_hgMsgLowCmd>{const out=new VendorDecoded<Ros2Unitree_hgMsgLowCmd>();if(bytes.length>897){out.status=90;return out;}if(bytes.length<897){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_hgMsgLowCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_hgMsgLowCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_hgMsgLowCmd():changetype<Ros2Unitree_hgMsgLowCmd>(out.value);}
}
/** Complete original unitree_hg/msg/LowState; spellings are published in generation proof. */
export class Ros2Unitree_hgMsgLowState {
static readonly maxWireBytes:i32=1983;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1983;
/** Exact native field version, native scalar uint32. */
version:Array<u32>=new Array<u32>();
/** Exact native field mode_pr, native scalar uint8. */
mode_pr:u8=0;
/** Exact native field mode_machine, native scalar uint8. */
mode_machine:u8=0;
/** Exact native field tick, native scalar uint32. */
tick:u32=0;
/** Exact native field imu_state, native scalar unitree_hg/msg/IMUState. */
imu_state:Ros2Unitree_hgMsgIMUState=new Ros2Unitree_hgMsgIMUState();
/** Exact native field motor_state, native scalar unitree_hg/msg/MotorState. */
motor_state:Array<Ros2Unitree_hgMsgMotorState>=new Array<Ros2Unitree_hgMsgMotorState>();
/** Exact native field wireless_remote, native scalar uint8. */
wireless_remote:Array<u8>=new Array<u8>();
/** Exact native field reserve, native scalar uint32. */
reserve:Array<u32>=new Array<u32>();
/** Exact native field crc, native scalar uint32. */
crc:u32=0;
constructor(){
for(let i=0;i<2;++i)this.version.push(0);
for(let i=0;i<35;++i)this.motor_state.push(new Ros2Unitree_hgMsgMotorState());
for(let i=0;i<40;++i)this.wireless_remote.push(0);
for(let i=0;i<4;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.version.length!=2)return -22;
for(let i=0;i<this.version.length;++i){
if(4>1983-n)return -90;n+=4;
if(n>1983)return -90;
}
}
{
if(1>1983-n)return -90;n+=1;
if(n>1983)return -90;
}
{
if(1>1983-n)return -90;n+=1;
if(n>1983)return -90;
}
{
if(4>1983-n)return -90;n+=4;
if(n>1983)return -90;
}
{
const child=this.imu_state.wireSize();if(child<0)return child;if(child>1983-n)return -90;n+=child;
if(n>1983)return -90;
}
{
if(this.motor_state.length!=35)return -22;
for(let i=0;i<this.motor_state.length;++i){
const child=this.motor_state[i].wireSize();if(child<0)return child;if(child>1983-n)return -90;n+=child;
if(n>1983)return -90;
}
}
{
if(this.wireless_remote.length!=40)return -22;
for(let i=0;i<this.wireless_remote.length;++i){
if(1>1983-n)return -90;n+=1;
if(n>1983)return -90;
}
}
{
if(this.reserve.length!=4)return -22;
for(let i=0;i<this.reserve.length;++i){
if(4>1983-n)return -90;n+=4;
if(n>1983)return -90;
}
}
{
if(4>1983-n)return -90;n+=4;
if(n>1983)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.version.length;++i){
w.integer(<u64>this.version[i],4);
}
w.integer(<u64>this.mode_pr,1);
w.integer(<u64>this.mode_machine,1);
w.integer(<u64>this.tick,4);
this.imu_state.writeControl(w);
for(let i=0;i<this.motor_state.length;++i){
this.motor_state[i].writeControl(w);
}
for(let i=0;i<this.wireless_remote.length;++i){
w.integer(<u64>this.wireless_remote[i],1);
}
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],4);
}
w.integer(<u64>this.crc,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_hgMsgLowState{const value=new Ros2Unitree_hgMsgLowState();
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.version=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.version.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.mode_pr=decoded;
}
{
const decoded=<u8>r.integer(1);
value.mode_machine=decoded;
}
{
const decoded=<u32>r.integer(4);
value.tick=decoded;
}
{
const decoded=Ros2Unitree_hgMsgIMUState.readControl(r);
value.imu_state=decoded;
}
{
const count:u32=35;if(!r.valid||count>35){r.valid=false;return value;}
value.motor_state=new Array<Ros2Unitree_hgMsgMotorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Unitree_hgMsgMotorState.readControl(r);
value.motor_state.push(decoded);
}
}
{
const count:u32=40;if(!r.valid||count>40){r.valid=false;return value;}
value.wireless_remote=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.wireless_remote.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.reserve=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.reserve.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.crc=decoded;
}
return value;}
}
export class Ros2Unitree_hgMsgLowStateCodec implements VendorCodec<Ros2Unitree_hgMsgLowState>{
static readonly codecIdentity:string="UWVMROS1_7eed2692672f536a476643f1d3a07affc8e5b3272663e3d282c199fff1c2fa1d";
static readonly maxWireBytes:i32=1983;
encodeChecked(value:Ros2Unitree_hgMsgLowState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_hgMsgLowState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_hgMsgLowState>{const out=new VendorDecoded<Ros2Unitree_hgMsgLowState>();if(bytes.length>1983){out.status=90;return out;}if(bytes.length<1983){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_hgMsgLowState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_hgMsgLowState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_hgMsgLowState():changetype<Ros2Unitree_hgMsgLowState>(out.value);}
}
/** Complete original unitree_hg/msg/MainBoardState; spellings are published in generation proof. */
export class Ros2Unitree_hgMsgMainBoardState {
static readonly maxWireBytes:i32=72;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=72;
/** Exact native field fan_state, native scalar uint16. */
fan_state:Array<u16>=new Array<u16>();
/** Exact native field temperature, native scalar int16. */
temperature:Array<i16>=new Array<i16>();
/** Exact native field value, native scalar float32. */
value:Array<f32>=new Array<f32>();
/** Exact native field state, native scalar uint32. */
state:Array<u32>=new Array<u32>();
constructor(){
for(let i=0;i<6;++i)this.fan_state.push(0);
for(let i=0;i<6;++i)this.temperature.push(0);
for(let i=0;i<6;++i)this.value.push(0);
for(let i=0;i<6;++i)this.state.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.fan_state.length!=6)return -22;
for(let i=0;i<this.fan_state.length;++i){
if(2>72-n)return -90;n+=2;
if(n>72)return -90;
}
}
{
if(this.temperature.length!=6)return -22;
for(let i=0;i<this.temperature.length;++i){
if(2>72-n)return -90;n+=2;
if(n>72)return -90;
}
}
{
if(this.value.length!=6)return -22;
for(let i=0;i<this.value.length;++i){
if(!isFinite(this.value[i]))return -22;
if(4>72-n)return -90;n+=4;
if(n>72)return -90;
}
}
{
if(this.state.length!=6)return -22;
for(let i=0;i<this.state.length;++i){
if(4>72-n)return -90;n+=4;
if(n>72)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.fan_state.length;++i){
w.integer(<u64>this.fan_state[i],2);
}
for(let i=0;i<this.temperature.length;++i){
w.integer(<u64>this.temperature[i],2);
}
for(let i=0;i<this.value.length;++i){
w.integer(<u64>reinterpret<u32>(this.value[i]),4);
}
for(let i=0;i<this.state.length;++i){
w.integer(<u64>this.state[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_hgMsgMainBoardState{const value=new Ros2Unitree_hgMsgMainBoardState();
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.fan_state=new Array<u16>();for(let i:u32=0;i<count;++i){
const decoded=<u16>r.integer(2);
value.fan_state.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.temperature=new Array<i16>();for(let i:u32=0;i<count;++i){
const decoded=<i16>r.integer(2);
value.temperature.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.value=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.value.push(decoded);
}
}
{
const count:u32=6;if(!r.valid||count>6){r.valid=false;return value;}
value.state=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.state.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_hgMsgMainBoardStateCodec implements VendorCodec<Ros2Unitree_hgMsgMainBoardState>{
static readonly codecIdentity:string="UWVMROS1_2a27a979aadc7bef33a757e169680faa632936c2634ee787f0c9f25cb08535a6";
static readonly maxWireBytes:i32=72;
encodeChecked(value:Ros2Unitree_hgMsgMainBoardState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_hgMsgMainBoardState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_hgMsgMainBoardState>{const out=new VendorDecoded<Ros2Unitree_hgMsgMainBoardState>();if(bytes.length>72){out.status=90;return out;}if(bytes.length<72){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_hgMsgMainBoardState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_hgMsgMainBoardState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_hgMsgMainBoardState():changetype<Ros2Unitree_hgMsgMainBoardState>(out.value);}
}
