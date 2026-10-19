/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original unitree_legged_msgs/BmsCmd; spellings are published in generation proof. */
export class Ros1Unitree_legged_msgsBmsCmd {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field off, native scalar uint8. */
off:u8=0;
/** Exact native field reserve, native scalar uint8. */
reserve:Array<u8>=new Array<u8>();
constructor(){
for(let i=0;i<3;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>4-n)return -90;n+=1;
if(n>4)return -90;
}
{
if(this.reserve.length!=3)return -22;
for(let i=0;i<this.reserve.length;++i){
if(1>4-n)return -90;n+=1;
if(n>4)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.off,1);
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Unitree_legged_msgsBmsCmd{const value=new Ros1Unitree_legged_msgsBmsCmd();
{
const decoded=<u8>r.integer(1);
value.off=decoded;
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.reserve=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.reserve.push(decoded);
}
}
return value;}
}
export class Ros1Unitree_legged_msgsBmsCmdCodec implements VendorCodec<Ros1Unitree_legged_msgsBmsCmd>{
static readonly codecIdentity:string="UWVMROS1_75732753b156fff8db3ba7e39541444bed2b71e139389a9b5bfbc49a4fa14928";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros1Unitree_legged_msgsBmsCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Unitree_legged_msgsBmsCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Unitree_legged_msgsBmsCmd>{const out=new VendorDecoded<Ros1Unitree_legged_msgsBmsCmd>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Unitree_legged_msgsBmsCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Unitree_legged_msgsBmsCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Unitree_legged_msgsBmsCmd():changetype<Ros1Unitree_legged_msgsBmsCmd>(out.value);}
}
/** Complete original unitree_legged_msgs/BmsState; spellings are published in generation proof. */
export class Ros1Unitree_legged_msgsBmsState {
static readonly maxWireBytes:i32=34;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=34;
/** Exact native field version_h, native scalar uint8. */
version_h:u8=0;
/** Exact native field version_l, native scalar uint8. */
version_l:u8=0;
/** Exact native field bms_status, native scalar uint8. */
bms_status:u8=0;
/** Exact native field SOC, native scalar uint8. */
SOC:u8=0;
/** Exact native field current, native scalar int32. */
current:i32=0;
/** Exact native field cycle, native scalar uint16. */
cycle:u16=0;
/** Exact native field BQ_NTC, native scalar int8. */
BQ_NTC:Array<i8>=new Array<i8>();
/** Exact native field MCU_NTC, native scalar int8. */
MCU_NTC:Array<i8>=new Array<i8>();
/** Exact native field cell_vol, native scalar uint16. */
cell_vol:Array<u16>=new Array<u16>();
constructor(){
for(let i=0;i<2;++i)this.BQ_NTC.push(0);
for(let i=0;i<2;++i)this.MCU_NTC.push(0);
for(let i=0;i<10;++i)this.cell_vol.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>34-n)return -90;n+=1;
if(n>34)return -90;
}
{
if(1>34-n)return -90;n+=1;
if(n>34)return -90;
}
{
if(1>34-n)return -90;n+=1;
if(n>34)return -90;
}
{
if(1>34-n)return -90;n+=1;
if(n>34)return -90;
}
{
if(4>34-n)return -90;n+=4;
if(n>34)return -90;
}
{
if(2>34-n)return -90;n+=2;
if(n>34)return -90;
}
{
if(this.BQ_NTC.length!=2)return -22;
for(let i=0;i<this.BQ_NTC.length;++i){
if(1>34-n)return -90;n+=1;
if(n>34)return -90;
}
}
{
if(this.MCU_NTC.length!=2)return -22;
for(let i=0;i<this.MCU_NTC.length;++i){
if(1>34-n)return -90;n+=1;
if(n>34)return -90;
}
}
{
if(this.cell_vol.length!=10)return -22;
for(let i=0;i<this.cell_vol.length;++i){
if(2>34-n)return -90;n+=2;
if(n>34)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.version_h,1);
w.integer(<u64>this.version_l,1);
w.integer(<u64>this.bms_status,1);
w.integer(<u64>this.SOC,1);
w.integer(<u64>this.current,4);
w.integer(<u64>this.cycle,2);
for(let i=0;i<this.BQ_NTC.length;++i){
w.integer(<u64>this.BQ_NTC[i],1);
}
for(let i=0;i<this.MCU_NTC.length;++i){
w.integer(<u64>this.MCU_NTC[i],1);
}
for(let i=0;i<this.cell_vol.length;++i){
w.integer(<u64>this.cell_vol[i],2);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Unitree_legged_msgsBmsState{const value=new Ros1Unitree_legged_msgsBmsState();
{
const decoded=<u8>r.integer(1);
value.version_h=decoded;
}
{
const decoded=<u8>r.integer(1);
value.version_l=decoded;
}
{
const decoded=<u8>r.integer(1);
value.bms_status=decoded;
}
{
const decoded=<u8>r.integer(1);
value.SOC=decoded;
}
{
const decoded=<i32>r.integer(4);
value.current=decoded;
}
{
const decoded=<u16>r.integer(2);
value.cycle=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.BQ_NTC=new Array<i8>();for(let i:u32=0;i<count;++i){
const decoded=<i8>r.integer(1);
value.BQ_NTC.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.MCU_NTC=new Array<i8>();for(let i:u32=0;i<count;++i){
const decoded=<i8>r.integer(1);
value.MCU_NTC.push(decoded);
}
}
{
const count:u32=10;if(!r.valid||count>10){r.valid=false;return value;}
value.cell_vol=new Array<u16>();for(let i:u32=0;i<count;++i){
const decoded=<u16>r.integer(2);
value.cell_vol.push(decoded);
}
}
return value;}
}
export class Ros1Unitree_legged_msgsBmsStateCodec implements VendorCodec<Ros1Unitree_legged_msgsBmsState>{
static readonly codecIdentity:string="UWVMROS1_ef0f93501f54d0ff760039d86bd1ba1739cb0c888b3a873611679484ccec094a";
static readonly maxWireBytes:i32=34;
encodeChecked(value:Ros1Unitree_legged_msgsBmsState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Unitree_legged_msgsBmsState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Unitree_legged_msgsBmsState>{const out=new VendorDecoded<Ros1Unitree_legged_msgsBmsState>();if(bytes.length>34){out.status=90;return out;}if(bytes.length<34){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Unitree_legged_msgsBmsState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Unitree_legged_msgsBmsState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Unitree_legged_msgsBmsState():changetype<Ros1Unitree_legged_msgsBmsState>(out.value);}
}
/** Complete original unitree_legged_msgs/Cartesian; spellings are published in generation proof. */
export class Ros1Unitree_legged_msgsCartesian {
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
static readControl(r:Reader):Ros1Unitree_legged_msgsCartesian{const value=new Ros1Unitree_legged_msgsCartesian();
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
export class Ros1Unitree_legged_msgsCartesianCodec implements VendorCodec<Ros1Unitree_legged_msgsCartesian>{
static readonly codecIdentity:string="UWVMROS1_43c7c62ba2a689751a0a15e326c5c9690a9b1f166b6726a0f803b98d2b3fa722";
static readonly maxWireBytes:i32=12;
encodeChecked(value:Ros1Unitree_legged_msgsCartesian):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Unitree_legged_msgsCartesian):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Unitree_legged_msgsCartesian>{const out=new VendorDecoded<Ros1Unitree_legged_msgsCartesian>();if(bytes.length>12){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Unitree_legged_msgsCartesian.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Unitree_legged_msgsCartesian{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Unitree_legged_msgsCartesian():changetype<Ros1Unitree_legged_msgsCartesian>(out.value);}
}
/** Complete original unitree_legged_msgs/LED; spellings are published in generation proof. */
export class Ros1Unitree_legged_msgsLED {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field r, native scalar uint8. */
r:u8=0;
/** Exact native field g, native scalar uint8. */
g:u8=0;
/** Exact native field b, native scalar uint8. */
b:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>3-n)return -90;n+=1;
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
w.integer(<u64>this.r,1);
w.integer(<u64>this.g,1);
w.integer(<u64>this.b,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Unitree_legged_msgsLED{const value=new Ros1Unitree_legged_msgsLED();
{
const decoded=<u8>r.integer(1);
value.r=decoded;
}
{
const decoded=<u8>r.integer(1);
value.g=decoded;
}
{
const decoded=<u8>r.integer(1);
value.b=decoded;
}
return value;}
}
export class Ros1Unitree_legged_msgsLEDCodec implements VendorCodec<Ros1Unitree_legged_msgsLED>{
static readonly codecIdentity:string="UWVMROS1_b4efac59eaf4d3f09c069e4833dd82b5422980f6a9f27db84950243a47164ea5";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Unitree_legged_msgsLED):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Unitree_legged_msgsLED):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Unitree_legged_msgsLED>{const out=new VendorDecoded<Ros1Unitree_legged_msgsLED>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Unitree_legged_msgsLED.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Unitree_legged_msgsLED{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Unitree_legged_msgsLED():changetype<Ros1Unitree_legged_msgsLED>(out.value);}
}
/** Complete original unitree_legged_msgs/HighCmd; spellings are published in generation proof. */
export class Ros1Unitree_legged_msgsHighCmd {
static readonly maxWireBytes:i32=129;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=129;
/** Exact native field head, native scalar uint8. */
head:Array<u8>=new Array<u8>();
/** Exact native field levelFlag, native scalar uint8. */
levelFlag:u8=0;
/** Exact native field frameReserve, native scalar uint8. */
frameReserve:u8=0;
/** Exact native field SN, native scalar uint32. */
SN:Array<u32>=new Array<u32>();
/** Exact native field version, native scalar uint32. */
version:Array<u32>=new Array<u32>();
/** Exact native field bandWidth, native scalar uint16. */
bandWidth:u16=0;
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact native field gaitType, native scalar uint8. */
gaitType:u8=0;
/** Exact native field speedLevel, native scalar uint8. */
speedLevel:u8=0;
/** Exact native field footRaiseHeight, native scalar float32. */
footRaiseHeight:f32=0;
/** Exact native field bodyHeight, native scalar float32. */
bodyHeight:f32=0;
/** Exact native field position, native scalar float32. */
position:Array<f32>=new Array<f32>();
/** Exact native field euler, native scalar float32. */
euler:Array<f32>=new Array<f32>();
/** Exact native field velocity, native scalar float32. */
velocity:Array<f32>=new Array<f32>();
/** Exact native field yawSpeed, native scalar float32. */
yawSpeed:f32=0;
/** Exact native field bms, native scalar unitree_legged_msgs/BmsCmd. */
bms:Ros1Unitree_legged_msgsBmsCmd=new Ros1Unitree_legged_msgsBmsCmd();
/** Exact native field led, native scalar unitree_legged_msgs/LED. */
led:Array<Ros1Unitree_legged_msgsLED>=new Array<Ros1Unitree_legged_msgsLED>();
/** Exact native field wirelessRemote, native scalar uint8. */
wirelessRemote:Array<u8>=new Array<u8>();
/** Exact native field reserve, native scalar uint32. */
reserve:u32=0;
/** Exact native field crc, native scalar uint32. */
crc:u32=0;
constructor(){
for(let i=0;i<2;++i)this.head.push(0);
for(let i=0;i<2;++i)this.SN.push(0);
for(let i=0;i<2;++i)this.version.push(0);
for(let i=0;i<2;++i)this.position.push(0);
for(let i=0;i<3;++i)this.euler.push(0);
for(let i=0;i<2;++i)this.velocity.push(0);
for(let i=0;i<4;++i)this.led.push(new Ros1Unitree_legged_msgsLED());
for(let i=0;i<40;++i)this.wirelessRemote.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.head.length!=2)return -22;
for(let i=0;i<this.head.length;++i){
if(1>129-n)return -90;n+=1;
if(n>129)return -90;
}
}
{
if(1>129-n)return -90;n+=1;
if(n>129)return -90;
}
{
if(1>129-n)return -90;n+=1;
if(n>129)return -90;
}
{
if(this.SN.length!=2)return -22;
for(let i=0;i<this.SN.length;++i){
if(4>129-n)return -90;n+=4;
if(n>129)return -90;
}
}
{
if(this.version.length!=2)return -22;
for(let i=0;i<this.version.length;++i){
if(4>129-n)return -90;n+=4;
if(n>129)return -90;
}
}
{
if(2>129-n)return -90;n+=2;
if(n>129)return -90;
}
{
if(1>129-n)return -90;n+=1;
if(n>129)return -90;
}
{
if(1>129-n)return -90;n+=1;
if(n>129)return -90;
}
{
if(1>129-n)return -90;n+=1;
if(n>129)return -90;
}
{
if(!isFinite(this.footRaiseHeight))return -22;
if(4>129-n)return -90;n+=4;
if(n>129)return -90;
}
{
if(!isFinite(this.bodyHeight))return -22;
if(4>129-n)return -90;n+=4;
if(n>129)return -90;
}
{
if(this.position.length!=2)return -22;
for(let i=0;i<this.position.length;++i){
if(!isFinite(this.position[i]))return -22;
if(4>129-n)return -90;n+=4;
if(n>129)return -90;
}
}
{
if(this.euler.length!=3)return -22;
for(let i=0;i<this.euler.length;++i){
if(!isFinite(this.euler[i]))return -22;
if(4>129-n)return -90;n+=4;
if(n>129)return -90;
}
}
{
if(this.velocity.length!=2)return -22;
for(let i=0;i<this.velocity.length;++i){
if(!isFinite(this.velocity[i]))return -22;
if(4>129-n)return -90;n+=4;
if(n>129)return -90;
}
}
{
if(!isFinite(this.yawSpeed))return -22;
if(4>129-n)return -90;n+=4;
if(n>129)return -90;
}
{
const child=this.bms.wireSize();if(child<0)return child;if(child>129-n)return -90;n+=child;
if(n>129)return -90;
}
{
if(this.led.length!=4)return -22;
for(let i=0;i<this.led.length;++i){
const child=this.led[i].wireSize();if(child<0)return child;if(child>129-n)return -90;n+=child;
if(n>129)return -90;
}
}
{
if(this.wirelessRemote.length!=40)return -22;
for(let i=0;i<this.wirelessRemote.length;++i){
if(1>129-n)return -90;n+=1;
if(n>129)return -90;
}
}
{
if(4>129-n)return -90;n+=4;
if(n>129)return -90;
}
{
if(4>129-n)return -90;n+=4;
if(n>129)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.head.length;++i){
w.integer(<u64>this.head[i],1);
}
w.integer(<u64>this.levelFlag,1);
w.integer(<u64>this.frameReserve,1);
for(let i=0;i<this.SN.length;++i){
w.integer(<u64>this.SN[i],4);
}
for(let i=0;i<this.version.length;++i){
w.integer(<u64>this.version[i],4);
}
w.integer(<u64>this.bandWidth,2);
w.integer(<u64>this.mode,1);
w.integer(<u64>this.gaitType,1);
w.integer(<u64>this.speedLevel,1);
w.integer(<u64>reinterpret<u32>(this.footRaiseHeight),4);
w.integer(<u64>reinterpret<u32>(this.bodyHeight),4);
for(let i=0;i<this.position.length;++i){
w.integer(<u64>reinterpret<u32>(this.position[i]),4);
}
for(let i=0;i<this.euler.length;++i){
w.integer(<u64>reinterpret<u32>(this.euler[i]),4);
}
for(let i=0;i<this.velocity.length;++i){
w.integer(<u64>reinterpret<u32>(this.velocity[i]),4);
}
w.integer(<u64>reinterpret<u32>(this.yawSpeed),4);
this.bms.writeControl(w);
for(let i=0;i<this.led.length;++i){
this.led[i].writeControl(w);
}
for(let i=0;i<this.wirelessRemote.length;++i){
w.integer(<u64>this.wirelessRemote[i],1);
}
w.integer(<u64>this.reserve,4);
w.integer(<u64>this.crc,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Unitree_legged_msgsHighCmd{const value=new Ros1Unitree_legged_msgsHighCmd();
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.head=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.head.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.levelFlag=decoded;
}
{
const decoded=<u8>r.integer(1);
value.frameReserve=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.SN=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.SN.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.version=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.version.push(decoded);
}
}
{
const decoded=<u16>r.integer(2);
value.bandWidth=decoded;
}
{
const decoded=<u8>r.integer(1);
value.mode=decoded;
}
{
const decoded=<u8>r.integer(1);
value.gaitType=decoded;
}
{
const decoded=<u8>r.integer(1);
value.speedLevel=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.footRaiseHeight=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.bodyHeight=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.position=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.position.push(decoded);
}
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.euler=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.euler.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.velocity=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.velocity.push(decoded);
}
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.yawSpeed=decoded;
}
{
const decoded=Ros1Unitree_legged_msgsBmsCmd.readControl(r);
value.bms=decoded;
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.led=new Array<Ros1Unitree_legged_msgsLED>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsLED.readControl(r);
value.led.push(decoded);
}
}
{
const count:u32=40;if(!r.valid||count>40){r.valid=false;return value;}
value.wirelessRemote=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.wirelessRemote.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.reserve=decoded;
}
{
const decoded=<u32>r.integer(4);
value.crc=decoded;
}
return value;}
}
export class Ros1Unitree_legged_msgsHighCmdCodec implements VendorCodec<Ros1Unitree_legged_msgsHighCmd>{
static readonly codecIdentity:string="UWVMROS1_ec6ec8937b2629a4441adc4a21eed83fca940b18d46b970d96e05c957ac90b38";
static readonly maxWireBytes:i32=129;
encodeChecked(value:Ros1Unitree_legged_msgsHighCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Unitree_legged_msgsHighCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Unitree_legged_msgsHighCmd>{const out=new VendorDecoded<Ros1Unitree_legged_msgsHighCmd>();if(bytes.length>129){out.status=90;return out;}if(bytes.length<129){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Unitree_legged_msgsHighCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Unitree_legged_msgsHighCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Unitree_legged_msgsHighCmd():changetype<Ros1Unitree_legged_msgsHighCmd>(out.value);}
}
/** Complete original unitree_legged_msgs/IMU; spellings are published in generation proof. */
export class Ros1Unitree_legged_msgsIMU {
static readonly maxWireBytes:i32=53;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=53;
/** Exact native field quaternion, native scalar float32. */
quaternion:Array<f32>=new Array<f32>();
/** Exact native field gyroscope, native scalar float32. */
gyroscope:Array<f32>=new Array<f32>();
/** Exact native field accelerometer, native scalar float32. */
accelerometer:Array<f32>=new Array<f32>();
/** Exact native field rpy, native scalar float32. */
rpy:Array<f32>=new Array<f32>();
/** Exact native field temperature, native scalar int8. */
temperature:i8=0;
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
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
}
{
if(this.gyroscope.length!=3)return -22;
for(let i=0;i<this.gyroscope.length;++i){
if(!isFinite(this.gyroscope[i]))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
}
{
if(this.accelerometer.length!=3)return -22;
for(let i=0;i<this.accelerometer.length;++i){
if(!isFinite(this.accelerometer[i]))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
}
{
if(this.rpy.length!=3)return -22;
for(let i=0;i<this.rpy.length;++i){
if(!isFinite(this.rpy[i]))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
}
{
if(1>53-n)return -90;n+=1;
if(n>53)return -90;
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
w.integer(<u64>this.temperature,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Unitree_legged_msgsIMU{const value=new Ros1Unitree_legged_msgsIMU();
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
const decoded=<i8>r.integer(1);
value.temperature=decoded;
}
return value;}
}
export class Ros1Unitree_legged_msgsIMUCodec implements VendorCodec<Ros1Unitree_legged_msgsIMU>{
static readonly codecIdentity:string="UWVMROS1_29d9ec99e676fe8747665da588b78c8ab77a3197e281ba892ab389d2d81fa276";
static readonly maxWireBytes:i32=53;
encodeChecked(value:Ros1Unitree_legged_msgsIMU):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Unitree_legged_msgsIMU):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Unitree_legged_msgsIMU>{const out=new VendorDecoded<Ros1Unitree_legged_msgsIMU>();if(bytes.length>53){out.status=90;return out;}if(bytes.length<53){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Unitree_legged_msgsIMU.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Unitree_legged_msgsIMU{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Unitree_legged_msgsIMU():changetype<Ros1Unitree_legged_msgsIMU>(out.value);}
}
/** Complete original unitree_legged_msgs/MotorState; spellings are published in generation proof. */
export class Ros1Unitree_legged_msgsMotorState {
static readonly maxWireBytes:i32=38;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=38;
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact native field q, native scalar float32. */
q:f32=0;
/** Exact native field dq, native scalar float32. */
dq:f32=0;
/** Exact native field ddq, native scalar float32. */
ddq:f32=0;
/** Exact native field tauEst, native scalar float32. */
tauEst:f32=0;
/** Exact native field q_raw, native scalar float32. */
q_raw:f32=0;
/** Exact native field dq_raw, native scalar float32. */
dq_raw:f32=0;
/** Exact native field ddq_raw, native scalar float32. */
ddq_raw:f32=0;
/** Exact native field temperature, native scalar int8. */
temperature:i8=0;
/** Exact native field reserve, native scalar uint32. */
reserve:Array<u32>=new Array<u32>();
constructor(){
for(let i=0;i<2;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>38-n)return -90;n+=1;
if(n>38)return -90;
}
{
if(!isFinite(this.q))return -22;
if(4>38-n)return -90;n+=4;
if(n>38)return -90;
}
{
if(!isFinite(this.dq))return -22;
if(4>38-n)return -90;n+=4;
if(n>38)return -90;
}
{
if(!isFinite(this.ddq))return -22;
if(4>38-n)return -90;n+=4;
if(n>38)return -90;
}
{
if(!isFinite(this.tauEst))return -22;
if(4>38-n)return -90;n+=4;
if(n>38)return -90;
}
{
if(!isFinite(this.q_raw))return -22;
if(4>38-n)return -90;n+=4;
if(n>38)return -90;
}
{
if(!isFinite(this.dq_raw))return -22;
if(4>38-n)return -90;n+=4;
if(n>38)return -90;
}
{
if(!isFinite(this.ddq_raw))return -22;
if(4>38-n)return -90;n+=4;
if(n>38)return -90;
}
{
if(1>38-n)return -90;n+=1;
if(n>38)return -90;
}
{
if(this.reserve.length!=2)return -22;
for(let i=0;i<this.reserve.length;++i){
if(4>38-n)return -90;n+=4;
if(n>38)return -90;
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
w.integer(<u64>reinterpret<u32>(this.tauEst),4);
w.integer(<u64>reinterpret<u32>(this.q_raw),4);
w.integer(<u64>reinterpret<u32>(this.dq_raw),4);
w.integer(<u64>reinterpret<u32>(this.ddq_raw),4);
w.integer(<u64>this.temperature,1);
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Unitree_legged_msgsMotorState{const value=new Ros1Unitree_legged_msgsMotorState();
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
value.tauEst=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.q_raw=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.dq_raw=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ddq_raw=decoded;
}
{
const decoded=<i8>r.integer(1);
value.temperature=decoded;
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
export class Ros1Unitree_legged_msgsMotorStateCodec implements VendorCodec<Ros1Unitree_legged_msgsMotorState>{
static readonly codecIdentity:string="UWVMROS1_be540cb22db5f08b6b9a8082777c76244e80f3148e9cfd63f510e4b2acdc0abf";
static readonly maxWireBytes:i32=38;
encodeChecked(value:Ros1Unitree_legged_msgsMotorState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Unitree_legged_msgsMotorState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Unitree_legged_msgsMotorState>{const out=new VendorDecoded<Ros1Unitree_legged_msgsMotorState>();if(bytes.length>38){out.status=90;return out;}if(bytes.length<38){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Unitree_legged_msgsMotorState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Unitree_legged_msgsMotorState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Unitree_legged_msgsMotorState():changetype<Ros1Unitree_legged_msgsMotorState>(out.value);}
}
/** Complete original unitree_legged_msgs/HighState; spellings are published in generation proof. */
export class Ros1Unitree_legged_msgsHighState {
static readonly maxWireBytes:i32=1087;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1087;
/** Exact native field head, native scalar uint8. */
head:Array<u8>=new Array<u8>();
/** Exact native field levelFlag, native scalar uint8. */
levelFlag:u8=0;
/** Exact native field frameReserve, native scalar uint8. */
frameReserve:u8=0;
/** Exact native field SN, native scalar uint32. */
SN:Array<u32>=new Array<u32>();
/** Exact native field version, native scalar uint32. */
version:Array<u32>=new Array<u32>();
/** Exact native field bandWidth, native scalar uint16. */
bandWidth:u16=0;
/** Exact native field imu, native scalar unitree_legged_msgs/IMU. */
imu:Ros1Unitree_legged_msgsIMU=new Ros1Unitree_legged_msgsIMU();
/** Exact native field motorState, native scalar unitree_legged_msgs/MotorState. */
motorState:Array<Ros1Unitree_legged_msgsMotorState>=new Array<Ros1Unitree_legged_msgsMotorState>();
/** Exact native field bms, native scalar unitree_legged_msgs/BmsState. */
bms:Ros1Unitree_legged_msgsBmsState=new Ros1Unitree_legged_msgsBmsState();
/** Exact native field footForce, native scalar int16. */
footForce:Array<i16>=new Array<i16>();
/** Exact native field footForceEst, native scalar int16. */
footForceEst:Array<i16>=new Array<i16>();
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact native field progress, native scalar float32. */
progress:f32=0;
/** Exact native field gaitType, native scalar uint8. */
gaitType:u8=0;
/** Exact native field footRaiseHeight, native scalar float32. */
footRaiseHeight:f32=0;
/** Exact native field position, native scalar float32. */
position:Array<f32>=new Array<f32>();
/** Exact native field bodyHeight, native scalar float32. */
bodyHeight:f32=0;
/** Exact native field velocity, native scalar float32. */
velocity:Array<f32>=new Array<f32>();
/** Exact native field yawSpeed, native scalar float32. */
yawSpeed:f32=0;
/** Exact native field rangeObstacle, native scalar float32. */
rangeObstacle:Array<f32>=new Array<f32>();
/** Exact native field footPosition2Body, native scalar unitree_legged_msgs/Cartesian. */
footPosition2Body:Array<Ros1Unitree_legged_msgsCartesian>=new Array<Ros1Unitree_legged_msgsCartesian>();
/** Exact native field footSpeed2Body, native scalar unitree_legged_msgs/Cartesian. */
footSpeed2Body:Array<Ros1Unitree_legged_msgsCartesian>=new Array<Ros1Unitree_legged_msgsCartesian>();
/** Exact native field wirelessRemote, native scalar uint8. */
wirelessRemote:Array<u8>=new Array<u8>();
/** Exact native field reserve, native scalar uint32. */
reserve:u32=0;
/** Exact native field crc, native scalar uint32. */
crc:u32=0;
constructor(){
for(let i=0;i<2;++i)this.head.push(0);
for(let i=0;i<2;++i)this.SN.push(0);
for(let i=0;i<2;++i)this.version.push(0);
for(let i=0;i<20;++i)this.motorState.push(new Ros1Unitree_legged_msgsMotorState());
for(let i=0;i<4;++i)this.footForce.push(0);
for(let i=0;i<4;++i)this.footForceEst.push(0);
for(let i=0;i<3;++i)this.position.push(0);
for(let i=0;i<3;++i)this.velocity.push(0);
for(let i=0;i<4;++i)this.rangeObstacle.push(0);
for(let i=0;i<4;++i)this.footPosition2Body.push(new Ros1Unitree_legged_msgsCartesian());
for(let i=0;i<4;++i)this.footSpeed2Body.push(new Ros1Unitree_legged_msgsCartesian());
for(let i=0;i<40;++i)this.wirelessRemote.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.head.length!=2)return -22;
for(let i=0;i<this.head.length;++i){
if(1>1087-n)return -90;n+=1;
if(n>1087)return -90;
}
}
{
if(1>1087-n)return -90;n+=1;
if(n>1087)return -90;
}
{
if(1>1087-n)return -90;n+=1;
if(n>1087)return -90;
}
{
if(this.SN.length!=2)return -22;
for(let i=0;i<this.SN.length;++i){
if(4>1087-n)return -90;n+=4;
if(n>1087)return -90;
}
}
{
if(this.version.length!=2)return -22;
for(let i=0;i<this.version.length;++i){
if(4>1087-n)return -90;n+=4;
if(n>1087)return -90;
}
}
{
if(2>1087-n)return -90;n+=2;
if(n>1087)return -90;
}
{
const child=this.imu.wireSize();if(child<0)return child;if(child>1087-n)return -90;n+=child;
if(n>1087)return -90;
}
{
if(this.motorState.length!=20)return -22;
for(let i=0;i<this.motorState.length;++i){
const child=this.motorState[i].wireSize();if(child<0)return child;if(child>1087-n)return -90;n+=child;
if(n>1087)return -90;
}
}
{
const child=this.bms.wireSize();if(child<0)return child;if(child>1087-n)return -90;n+=child;
if(n>1087)return -90;
}
{
if(this.footForce.length!=4)return -22;
for(let i=0;i<this.footForce.length;++i){
if(2>1087-n)return -90;n+=2;
if(n>1087)return -90;
}
}
{
if(this.footForceEst.length!=4)return -22;
for(let i=0;i<this.footForceEst.length;++i){
if(2>1087-n)return -90;n+=2;
if(n>1087)return -90;
}
}
{
if(1>1087-n)return -90;n+=1;
if(n>1087)return -90;
}
{
if(!isFinite(this.progress))return -22;
if(4>1087-n)return -90;n+=4;
if(n>1087)return -90;
}
{
if(1>1087-n)return -90;n+=1;
if(n>1087)return -90;
}
{
if(!isFinite(this.footRaiseHeight))return -22;
if(4>1087-n)return -90;n+=4;
if(n>1087)return -90;
}
{
if(this.position.length!=3)return -22;
for(let i=0;i<this.position.length;++i){
if(!isFinite(this.position[i]))return -22;
if(4>1087-n)return -90;n+=4;
if(n>1087)return -90;
}
}
{
if(!isFinite(this.bodyHeight))return -22;
if(4>1087-n)return -90;n+=4;
if(n>1087)return -90;
}
{
if(this.velocity.length!=3)return -22;
for(let i=0;i<this.velocity.length;++i){
if(!isFinite(this.velocity[i]))return -22;
if(4>1087-n)return -90;n+=4;
if(n>1087)return -90;
}
}
{
if(!isFinite(this.yawSpeed))return -22;
if(4>1087-n)return -90;n+=4;
if(n>1087)return -90;
}
{
if(this.rangeObstacle.length!=4)return -22;
for(let i=0;i<this.rangeObstacle.length;++i){
if(!isFinite(this.rangeObstacle[i]))return -22;
if(4>1087-n)return -90;n+=4;
if(n>1087)return -90;
}
}
{
if(this.footPosition2Body.length!=4)return -22;
for(let i=0;i<this.footPosition2Body.length;++i){
const child=this.footPosition2Body[i].wireSize();if(child<0)return child;if(child>1087-n)return -90;n+=child;
if(n>1087)return -90;
}
}
{
if(this.footSpeed2Body.length!=4)return -22;
for(let i=0;i<this.footSpeed2Body.length;++i){
const child=this.footSpeed2Body[i].wireSize();if(child<0)return child;if(child>1087-n)return -90;n+=child;
if(n>1087)return -90;
}
}
{
if(this.wirelessRemote.length!=40)return -22;
for(let i=0;i<this.wirelessRemote.length;++i){
if(1>1087-n)return -90;n+=1;
if(n>1087)return -90;
}
}
{
if(4>1087-n)return -90;n+=4;
if(n>1087)return -90;
}
{
if(4>1087-n)return -90;n+=4;
if(n>1087)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.head.length;++i){
w.integer(<u64>this.head[i],1);
}
w.integer(<u64>this.levelFlag,1);
w.integer(<u64>this.frameReserve,1);
for(let i=0;i<this.SN.length;++i){
w.integer(<u64>this.SN[i],4);
}
for(let i=0;i<this.version.length;++i){
w.integer(<u64>this.version[i],4);
}
w.integer(<u64>this.bandWidth,2);
this.imu.writeControl(w);
for(let i=0;i<this.motorState.length;++i){
this.motorState[i].writeControl(w);
}
this.bms.writeControl(w);
for(let i=0;i<this.footForce.length;++i){
w.integer(<u64>this.footForce[i],2);
}
for(let i=0;i<this.footForceEst.length;++i){
w.integer(<u64>this.footForceEst[i],2);
}
w.integer(<u64>this.mode,1);
w.integer(<u64>reinterpret<u32>(this.progress),4);
w.integer(<u64>this.gaitType,1);
w.integer(<u64>reinterpret<u32>(this.footRaiseHeight),4);
for(let i=0;i<this.position.length;++i){
w.integer(<u64>reinterpret<u32>(this.position[i]),4);
}
w.integer(<u64>reinterpret<u32>(this.bodyHeight),4);
for(let i=0;i<this.velocity.length;++i){
w.integer(<u64>reinterpret<u32>(this.velocity[i]),4);
}
w.integer(<u64>reinterpret<u32>(this.yawSpeed),4);
for(let i=0;i<this.rangeObstacle.length;++i){
w.integer(<u64>reinterpret<u32>(this.rangeObstacle[i]),4);
}
for(let i=0;i<this.footPosition2Body.length;++i){
this.footPosition2Body[i].writeControl(w);
}
for(let i=0;i<this.footSpeed2Body.length;++i){
this.footSpeed2Body[i].writeControl(w);
}
for(let i=0;i<this.wirelessRemote.length;++i){
w.integer(<u64>this.wirelessRemote[i],1);
}
w.integer(<u64>this.reserve,4);
w.integer(<u64>this.crc,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Unitree_legged_msgsHighState{const value=new Ros1Unitree_legged_msgsHighState();
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.head=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.head.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.levelFlag=decoded;
}
{
const decoded=<u8>r.integer(1);
value.frameReserve=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.SN=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.SN.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.version=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.version.push(decoded);
}
}
{
const decoded=<u16>r.integer(2);
value.bandWidth=decoded;
}
{
const decoded=Ros1Unitree_legged_msgsIMU.readControl(r);
value.imu=decoded;
}
{
const count:u32=20;if(!r.valid||count>20){r.valid=false;return value;}
value.motorState=new Array<Ros1Unitree_legged_msgsMotorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsMotorState.readControl(r);
value.motorState.push(decoded);
}
}
{
const decoded=Ros1Unitree_legged_msgsBmsState.readControl(r);
value.bms=decoded;
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.footForce=new Array<i16>();for(let i:u32=0;i<count;++i){
const decoded=<i16>r.integer(2);
value.footForce.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.footForceEst=new Array<i16>();for(let i:u32=0;i<count;++i){
const decoded=<i16>r.integer(2);
value.footForceEst.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.mode=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.progress=decoded;
}
{
const decoded=<u8>r.integer(1);
value.gaitType=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.footRaiseHeight=decoded;
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.position=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.position.push(decoded);
}
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.bodyHeight=decoded;
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.velocity=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.velocity.push(decoded);
}
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.yawSpeed=decoded;
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.rangeObstacle=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rangeObstacle.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.footPosition2Body=new Array<Ros1Unitree_legged_msgsCartesian>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsCartesian.readControl(r);
value.footPosition2Body.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.footSpeed2Body=new Array<Ros1Unitree_legged_msgsCartesian>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsCartesian.readControl(r);
value.footSpeed2Body.push(decoded);
}
}
{
const count:u32=40;if(!r.valid||count>40){r.valid=false;return value;}
value.wirelessRemote=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.wirelessRemote.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.reserve=decoded;
}
{
const decoded=<u32>r.integer(4);
value.crc=decoded;
}
return value;}
}
export class Ros1Unitree_legged_msgsHighStateCodec implements VendorCodec<Ros1Unitree_legged_msgsHighState>{
static readonly codecIdentity:string="UWVMROS1_82ee0efaab21b5cb89e99f5c0f5daec583b48dd5feb2dd6a7e09ba9d9110b53d";
static readonly maxWireBytes:i32=1087;
encodeChecked(value:Ros1Unitree_legged_msgsHighState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Unitree_legged_msgsHighState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Unitree_legged_msgsHighState>{const out=new VendorDecoded<Ros1Unitree_legged_msgsHighState>();if(bytes.length>1087){out.status=90;return out;}if(bytes.length<1087){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Unitree_legged_msgsHighState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Unitree_legged_msgsHighState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Unitree_legged_msgsHighState():changetype<Ros1Unitree_legged_msgsHighState>(out.value);}
}
/** Complete original unitree_legged_msgs/MotorCmd; spellings are published in generation proof. */
export class Ros1Unitree_legged_msgsMotorCmd {
static readonly maxWireBytes:i32=33;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=33;
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact native field q, native scalar float32. */
q:f32=0;
/** Exact native field dq, native scalar float32. */
dq:f32=0;
/** Exact native field tau, native scalar float32. */
tau:f32=0;
/** Exact native field Kp, native scalar float32. */
Kp:f32=0;
/** Exact native field Kd, native scalar float32. */
Kd:f32=0;
/** Exact native field reserve, native scalar uint32. */
reserve:Array<u32>=new Array<u32>();
constructor(){
for(let i=0;i<3;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>33-n)return -90;n+=1;
if(n>33)return -90;
}
{
if(!isFinite(this.q))return -22;
if(4>33-n)return -90;n+=4;
if(n>33)return -90;
}
{
if(!isFinite(this.dq))return -22;
if(4>33-n)return -90;n+=4;
if(n>33)return -90;
}
{
if(!isFinite(this.tau))return -22;
if(4>33-n)return -90;n+=4;
if(n>33)return -90;
}
{
if(!isFinite(this.Kp))return -22;
if(4>33-n)return -90;n+=4;
if(n>33)return -90;
}
{
if(!isFinite(this.Kd))return -22;
if(4>33-n)return -90;n+=4;
if(n>33)return -90;
}
{
if(this.reserve.length!=3)return -22;
for(let i=0;i<this.reserve.length;++i){
if(4>33-n)return -90;n+=4;
if(n>33)return -90;
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
w.integer(<u64>reinterpret<u32>(this.tau),4);
w.integer(<u64>reinterpret<u32>(this.Kp),4);
w.integer(<u64>reinterpret<u32>(this.Kd),4);
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Unitree_legged_msgsMotorCmd{const value=new Ros1Unitree_legged_msgsMotorCmd();
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
value.Kp=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.Kd=decoded;
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
export class Ros1Unitree_legged_msgsMotorCmdCodec implements VendorCodec<Ros1Unitree_legged_msgsMotorCmd>{
static readonly codecIdentity:string="UWVMROS1_632b25f209e72857182d92752aa72e3ab47a45e895cc352ee6dc05f1c1168083";
static readonly maxWireBytes:i32=33;
encodeChecked(value:Ros1Unitree_legged_msgsMotorCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Unitree_legged_msgsMotorCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Unitree_legged_msgsMotorCmd>{const out=new VendorDecoded<Ros1Unitree_legged_msgsMotorCmd>();if(bytes.length>33){out.status=90;return out;}if(bytes.length<33){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Unitree_legged_msgsMotorCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Unitree_legged_msgsMotorCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Unitree_legged_msgsMotorCmd():changetype<Ros1Unitree_legged_msgsMotorCmd>(out.value);}
}
/** Complete original unitree_legged_msgs/LowCmd; spellings are published in generation proof. */
export class Ros1Unitree_legged_msgsLowCmd {
static readonly maxWireBytes:i32=734;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=734;
/** Exact native field head, native scalar uint8. */
head:Array<u8>=new Array<u8>();
/** Exact native field levelFlag, native scalar uint8. */
levelFlag:u8=0;
/** Exact native field frameReserve, native scalar uint8. */
frameReserve:u8=0;
/** Exact native field SN, native scalar uint32. */
SN:Array<u32>=new Array<u32>();
/** Exact native field version, native scalar uint32. */
version:Array<u32>=new Array<u32>();
/** Exact native field bandWidth, native scalar uint16. */
bandWidth:u16=0;
/** Exact native field motorCmd, native scalar unitree_legged_msgs/MotorCmd. */
motorCmd:Array<Ros1Unitree_legged_msgsMotorCmd>=new Array<Ros1Unitree_legged_msgsMotorCmd>();
/** Exact native field bms, native scalar unitree_legged_msgs/BmsCmd. */
bms:Ros1Unitree_legged_msgsBmsCmd=new Ros1Unitree_legged_msgsBmsCmd();
/** Exact native field wirelessRemote, native scalar uint8. */
wirelessRemote:Array<u8>=new Array<u8>();
/** Exact native field reserve, native scalar uint32. */
reserve:u32=0;
/** Exact native field crc, native scalar uint32. */
crc:u32=0;
constructor(){
for(let i=0;i<2;++i)this.head.push(0);
for(let i=0;i<2;++i)this.SN.push(0);
for(let i=0;i<2;++i)this.version.push(0);
for(let i=0;i<20;++i)this.motorCmd.push(new Ros1Unitree_legged_msgsMotorCmd());
for(let i=0;i<40;++i)this.wirelessRemote.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.head.length!=2)return -22;
for(let i=0;i<this.head.length;++i){
if(1>734-n)return -90;n+=1;
if(n>734)return -90;
}
}
{
if(1>734-n)return -90;n+=1;
if(n>734)return -90;
}
{
if(1>734-n)return -90;n+=1;
if(n>734)return -90;
}
{
if(this.SN.length!=2)return -22;
for(let i=0;i<this.SN.length;++i){
if(4>734-n)return -90;n+=4;
if(n>734)return -90;
}
}
{
if(this.version.length!=2)return -22;
for(let i=0;i<this.version.length;++i){
if(4>734-n)return -90;n+=4;
if(n>734)return -90;
}
}
{
if(2>734-n)return -90;n+=2;
if(n>734)return -90;
}
{
if(this.motorCmd.length!=20)return -22;
for(let i=0;i<this.motorCmd.length;++i){
const child=this.motorCmd[i].wireSize();if(child<0)return child;if(child>734-n)return -90;n+=child;
if(n>734)return -90;
}
}
{
const child=this.bms.wireSize();if(child<0)return child;if(child>734-n)return -90;n+=child;
if(n>734)return -90;
}
{
if(this.wirelessRemote.length!=40)return -22;
for(let i=0;i<this.wirelessRemote.length;++i){
if(1>734-n)return -90;n+=1;
if(n>734)return -90;
}
}
{
if(4>734-n)return -90;n+=4;
if(n>734)return -90;
}
{
if(4>734-n)return -90;n+=4;
if(n>734)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.head.length;++i){
w.integer(<u64>this.head[i],1);
}
w.integer(<u64>this.levelFlag,1);
w.integer(<u64>this.frameReserve,1);
for(let i=0;i<this.SN.length;++i){
w.integer(<u64>this.SN[i],4);
}
for(let i=0;i<this.version.length;++i){
w.integer(<u64>this.version[i],4);
}
w.integer(<u64>this.bandWidth,2);
for(let i=0;i<this.motorCmd.length;++i){
this.motorCmd[i].writeControl(w);
}
this.bms.writeControl(w);
for(let i=0;i<this.wirelessRemote.length;++i){
w.integer(<u64>this.wirelessRemote[i],1);
}
w.integer(<u64>this.reserve,4);
w.integer(<u64>this.crc,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Unitree_legged_msgsLowCmd{const value=new Ros1Unitree_legged_msgsLowCmd();
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.head=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.head.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.levelFlag=decoded;
}
{
const decoded=<u8>r.integer(1);
value.frameReserve=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.SN=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.SN.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.version=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.version.push(decoded);
}
}
{
const decoded=<u16>r.integer(2);
value.bandWidth=decoded;
}
{
const count:u32=20;if(!r.valid||count>20){r.valid=false;return value;}
value.motorCmd=new Array<Ros1Unitree_legged_msgsMotorCmd>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsMotorCmd.readControl(r);
value.motorCmd.push(decoded);
}
}
{
const decoded=Ros1Unitree_legged_msgsBmsCmd.readControl(r);
value.bms=decoded;
}
{
const count:u32=40;if(!r.valid||count>40){r.valid=false;return value;}
value.wirelessRemote=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.wirelessRemote.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.reserve=decoded;
}
{
const decoded=<u32>r.integer(4);
value.crc=decoded;
}
return value;}
}
export class Ros1Unitree_legged_msgsLowCmdCodec implements VendorCodec<Ros1Unitree_legged_msgsLowCmd>{
static readonly codecIdentity:string="UWVMROS1_11d3ac7c497f01fbf6624c39ec71fa613c462cd62acb20c1f263cdde672e9181";
static readonly maxWireBytes:i32=734;
encodeChecked(value:Ros1Unitree_legged_msgsLowCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Unitree_legged_msgsLowCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Unitree_legged_msgsLowCmd>{const out=new VendorDecoded<Ros1Unitree_legged_msgsLowCmd>();if(bytes.length>734){out.status=90;return out;}if(bytes.length<734){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Unitree_legged_msgsLowCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Unitree_legged_msgsLowCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Unitree_legged_msgsLowCmd():changetype<Ros1Unitree_legged_msgsLowCmd>(out.value);}
}
/** Complete original unitree_legged_msgs/LowState; spellings are published in generation proof. */
export class Ros1Unitree_legged_msgsLowState {
static readonly maxWireBytes:i32=1069;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1069;
/** Exact native field head, native scalar uint8. */
head:Array<u8>=new Array<u8>();
/** Exact native field levelFlag, native scalar uint8. */
levelFlag:u8=0;
/** Exact native field frameReserve, native scalar uint8. */
frameReserve:u8=0;
/** Exact native field SN, native scalar uint32. */
SN:Array<u32>=new Array<u32>();
/** Exact native field version, native scalar uint32. */
version:Array<u32>=new Array<u32>();
/** Exact native field bandWidth, native scalar uint16. */
bandWidth:u16=0;
/** Exact native field imu, native scalar unitree_legged_msgs/IMU. */
imu:Ros1Unitree_legged_msgsIMU=new Ros1Unitree_legged_msgsIMU();
/** Exact native field motorState, native scalar unitree_legged_msgs/MotorState. */
motorState:Array<Ros1Unitree_legged_msgsMotorState>=new Array<Ros1Unitree_legged_msgsMotorState>();
/** Exact native field bms, native scalar unitree_legged_msgs/BmsState. */
bms:Ros1Unitree_legged_msgsBmsState=new Ros1Unitree_legged_msgsBmsState();
/** Exact native field footForce, native scalar int16. */
footForce:Array<i16>=new Array<i16>();
/** Exact native field footForceEst, native scalar int16. */
footForceEst:Array<i16>=new Array<i16>();
/** Exact native field tick, native scalar uint32. */
tick:u32=0;
/** Exact native field wirelessRemote, native scalar uint8. */
wirelessRemote:Array<u8>=new Array<u8>();
/** Exact native field reserve, native scalar uint32. */
reserve:u32=0;
/** Exact native field crc, native scalar uint32. */
crc:u32=0;
/** Exact native field eeForceRaw, native scalar unitree_legged_msgs/Cartesian. */
eeForceRaw:Array<Ros1Unitree_legged_msgsCartesian>=new Array<Ros1Unitree_legged_msgsCartesian>();
/** Exact native field eeForce, native scalar unitree_legged_msgs/Cartesian. */
eeForce:Array<Ros1Unitree_legged_msgsCartesian>=new Array<Ros1Unitree_legged_msgsCartesian>();
/** Exact native field position, native scalar unitree_legged_msgs/Cartesian. */
position:Ros1Unitree_legged_msgsCartesian=new Ros1Unitree_legged_msgsCartesian();
/** Exact native field velocity, native scalar unitree_legged_msgs/Cartesian. */
velocity:Ros1Unitree_legged_msgsCartesian=new Ros1Unitree_legged_msgsCartesian();
/** Exact native field velocity_w, native scalar unitree_legged_msgs/Cartesian. */
velocity_w:Ros1Unitree_legged_msgsCartesian=new Ros1Unitree_legged_msgsCartesian();
constructor(){
for(let i=0;i<2;++i)this.head.push(0);
for(let i=0;i<2;++i)this.SN.push(0);
for(let i=0;i<2;++i)this.version.push(0);
for(let i=0;i<20;++i)this.motorState.push(new Ros1Unitree_legged_msgsMotorState());
for(let i=0;i<4;++i)this.footForce.push(0);
for(let i=0;i<4;++i)this.footForceEst.push(0);
for(let i=0;i<40;++i)this.wirelessRemote.push(0);
for(let i=0;i<4;++i)this.eeForceRaw.push(new Ros1Unitree_legged_msgsCartesian());
for(let i=0;i<4;++i)this.eeForce.push(new Ros1Unitree_legged_msgsCartesian());
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.head.length!=2)return -22;
for(let i=0;i<this.head.length;++i){
if(1>1069-n)return -90;n+=1;
if(n>1069)return -90;
}
}
{
if(1>1069-n)return -90;n+=1;
if(n>1069)return -90;
}
{
if(1>1069-n)return -90;n+=1;
if(n>1069)return -90;
}
{
if(this.SN.length!=2)return -22;
for(let i=0;i<this.SN.length;++i){
if(4>1069-n)return -90;n+=4;
if(n>1069)return -90;
}
}
{
if(this.version.length!=2)return -22;
for(let i=0;i<this.version.length;++i){
if(4>1069-n)return -90;n+=4;
if(n>1069)return -90;
}
}
{
if(2>1069-n)return -90;n+=2;
if(n>1069)return -90;
}
{
const child=this.imu.wireSize();if(child<0)return child;if(child>1069-n)return -90;n+=child;
if(n>1069)return -90;
}
{
if(this.motorState.length!=20)return -22;
for(let i=0;i<this.motorState.length;++i){
const child=this.motorState[i].wireSize();if(child<0)return child;if(child>1069-n)return -90;n+=child;
if(n>1069)return -90;
}
}
{
const child=this.bms.wireSize();if(child<0)return child;if(child>1069-n)return -90;n+=child;
if(n>1069)return -90;
}
{
if(this.footForce.length!=4)return -22;
for(let i=0;i<this.footForce.length;++i){
if(2>1069-n)return -90;n+=2;
if(n>1069)return -90;
}
}
{
if(this.footForceEst.length!=4)return -22;
for(let i=0;i<this.footForceEst.length;++i){
if(2>1069-n)return -90;n+=2;
if(n>1069)return -90;
}
}
{
if(4>1069-n)return -90;n+=4;
if(n>1069)return -90;
}
{
if(this.wirelessRemote.length!=40)return -22;
for(let i=0;i<this.wirelessRemote.length;++i){
if(1>1069-n)return -90;n+=1;
if(n>1069)return -90;
}
}
{
if(4>1069-n)return -90;n+=4;
if(n>1069)return -90;
}
{
if(4>1069-n)return -90;n+=4;
if(n>1069)return -90;
}
{
if(this.eeForceRaw.length!=4)return -22;
for(let i=0;i<this.eeForceRaw.length;++i){
const child=this.eeForceRaw[i].wireSize();if(child<0)return child;if(child>1069-n)return -90;n+=child;
if(n>1069)return -90;
}
}
{
if(this.eeForce.length!=4)return -22;
for(let i=0;i<this.eeForce.length;++i){
const child=this.eeForce[i].wireSize();if(child<0)return child;if(child>1069-n)return -90;n+=child;
if(n>1069)return -90;
}
}
{
const child=this.position.wireSize();if(child<0)return child;if(child>1069-n)return -90;n+=child;
if(n>1069)return -90;
}
{
const child=this.velocity.wireSize();if(child<0)return child;if(child>1069-n)return -90;n+=child;
if(n>1069)return -90;
}
{
const child=this.velocity_w.wireSize();if(child<0)return child;if(child>1069-n)return -90;n+=child;
if(n>1069)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.head.length;++i){
w.integer(<u64>this.head[i],1);
}
w.integer(<u64>this.levelFlag,1);
w.integer(<u64>this.frameReserve,1);
for(let i=0;i<this.SN.length;++i){
w.integer(<u64>this.SN[i],4);
}
for(let i=0;i<this.version.length;++i){
w.integer(<u64>this.version[i],4);
}
w.integer(<u64>this.bandWidth,2);
this.imu.writeControl(w);
for(let i=0;i<this.motorState.length;++i){
this.motorState[i].writeControl(w);
}
this.bms.writeControl(w);
for(let i=0;i<this.footForce.length;++i){
w.integer(<u64>this.footForce[i],2);
}
for(let i=0;i<this.footForceEst.length;++i){
w.integer(<u64>this.footForceEst[i],2);
}
w.integer(<u64>this.tick,4);
for(let i=0;i<this.wirelessRemote.length;++i){
w.integer(<u64>this.wirelessRemote[i],1);
}
w.integer(<u64>this.reserve,4);
w.integer(<u64>this.crc,4);
for(let i=0;i<this.eeForceRaw.length;++i){
this.eeForceRaw[i].writeControl(w);
}
for(let i=0;i<this.eeForce.length;++i){
this.eeForce[i].writeControl(w);
}
this.position.writeControl(w);
this.velocity.writeControl(w);
this.velocity_w.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Unitree_legged_msgsLowState{const value=new Ros1Unitree_legged_msgsLowState();
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.head=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.head.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.levelFlag=decoded;
}
{
const decoded=<u8>r.integer(1);
value.frameReserve=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.SN=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.SN.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.version=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.version.push(decoded);
}
}
{
const decoded=<u16>r.integer(2);
value.bandWidth=decoded;
}
{
const decoded=Ros1Unitree_legged_msgsIMU.readControl(r);
value.imu=decoded;
}
{
const count:u32=20;if(!r.valid||count>20){r.valid=false;return value;}
value.motorState=new Array<Ros1Unitree_legged_msgsMotorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsMotorState.readControl(r);
value.motorState.push(decoded);
}
}
{
const decoded=Ros1Unitree_legged_msgsBmsState.readControl(r);
value.bms=decoded;
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.footForce=new Array<i16>();for(let i:u32=0;i<count;++i){
const decoded=<i16>r.integer(2);
value.footForce.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.footForceEst=new Array<i16>();for(let i:u32=0;i<count;++i){
const decoded=<i16>r.integer(2);
value.footForceEst.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.tick=decoded;
}
{
const count:u32=40;if(!r.valid||count>40){r.valid=false;return value;}
value.wirelessRemote=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.wirelessRemote.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.reserve=decoded;
}
{
const decoded=<u32>r.integer(4);
value.crc=decoded;
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.eeForceRaw=new Array<Ros1Unitree_legged_msgsCartesian>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsCartesian.readControl(r);
value.eeForceRaw.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.eeForce=new Array<Ros1Unitree_legged_msgsCartesian>();for(let i:u32=0;i<count;++i){
const decoded=Ros1Unitree_legged_msgsCartesian.readControl(r);
value.eeForce.push(decoded);
}
}
{
const decoded=Ros1Unitree_legged_msgsCartesian.readControl(r);
value.position=decoded;
}
{
const decoded=Ros1Unitree_legged_msgsCartesian.readControl(r);
value.velocity=decoded;
}
{
const decoded=Ros1Unitree_legged_msgsCartesian.readControl(r);
value.velocity_w=decoded;
}
return value;}
}
export class Ros1Unitree_legged_msgsLowStateCodec implements VendorCodec<Ros1Unitree_legged_msgsLowState>{
static readonly codecIdentity:string="UWVMROS1_37425ed49c0eebd762b6084ee413c7706fc43defc524414d3f50cdd2d25552c0";
static readonly maxWireBytes:i32=1069;
encodeChecked(value:Ros1Unitree_legged_msgsLowState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Unitree_legged_msgsLowState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Unitree_legged_msgsLowState>{const out=new VendorDecoded<Ros1Unitree_legged_msgsLowState>();if(bytes.length>1069){out.status=90;return out;}if(bytes.length<1069){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Unitree_legged_msgsLowState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Unitree_legged_msgsLowState{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Unitree_legged_msgsLowState():changetype<Ros1Unitree_legged_msgsLowState>(out.value);}
}
