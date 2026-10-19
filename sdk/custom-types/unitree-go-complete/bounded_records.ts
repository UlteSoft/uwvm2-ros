/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original unitree_go/msg/AudioData; spellings are published in generation proof. */
export class Ros2Unitree_goMsgAudioData {
static readonly maxWireBytes:i32=2060;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2060;
/** Exact native field time_frame, native scalar uint64. */
time_frame:u64=0;
/** Exact native field data, native scalar uint8. */
data:Array<u8>=new Array<u8>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>2060-n)return -90;n+=8;
if(n>2060)return -90;
}
{
if(this.data.length>2048)return -22;
if(4>2060-n)return -90;n+=4;
for(let i=0;i<this.data.length;++i){
if(1>2060-n)return -90;n+=1;
if(n>2060)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.time_frame,8);
w.u32(<u32>this.data.length);
for(let i=0;i<this.data.length;++i){
w.integer(<u64>this.data[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgAudioData{const value=new Ros2Unitree_goMsgAudioData();
{
const decoded=<u64>r.integer(8);
value.time_frame=decoded;
}
{
const count=r.u32();if(!r.valid||count>2048){r.valid=false;return value;}
value.data=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.data.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_goMsgAudioDataCodec implements VendorCodec<Ros2Unitree_goMsgAudioData>{
static readonly codecIdentity:string="UWVMROS1_aa14877a076c7fc6b662efa965cc8a75f42b3f84990a078c7c3c5def01d2c9fe";
static readonly maxWireBytes:i32=2060;
encodeChecked(value:Ros2Unitree_goMsgAudioData):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgAudioData):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgAudioData>{const out=new VendorDecoded<Ros2Unitree_goMsgAudioData>();if(bytes.length>2060){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgAudioData.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgAudioData{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgAudioData():changetype<Ros2Unitree_goMsgAudioData>(out.value);}
}
/** Complete original unitree_go/msg/BmsCmd; spellings are published in generation proof. */
export class Ros2Unitree_goMsgBmsCmd {
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
static readControl(r:Reader):Ros2Unitree_goMsgBmsCmd{const value=new Ros2Unitree_goMsgBmsCmd();
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
export class Ros2Unitree_goMsgBmsCmdCodec implements VendorCodec<Ros2Unitree_goMsgBmsCmd>{
static readonly codecIdentity:string="UWVMROS1_d212c1ab0c92d64801a3b10c3a4d79517ea24a830db3d85ca2bb18e1e0ea5339";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Unitree_goMsgBmsCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgBmsCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgBmsCmd>{const out=new VendorDecoded<Ros2Unitree_goMsgBmsCmd>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgBmsCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgBmsCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgBmsCmd():changetype<Ros2Unitree_goMsgBmsCmd>(out.value);}
}
/** Complete original unitree_go/msg/BmsState; spellings are published in generation proof. */
export class Ros2Unitree_goMsgBmsState {
static readonly maxWireBytes:i32=44;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=44;
/** Exact native field version_high, native scalar uint8. */
version_high:u8=0;
/** Exact native field version_low, native scalar uint8. */
version_low:u8=0;
/** Exact native field status, native scalar uint8. */
status:u8=0;
/** Exact native field soc, native scalar uint8. */
soc:u8=0;
/** Exact native field current, native scalar int32. */
current:i32=0;
/** Exact native field cycle, native scalar uint16. */
cycle:u16=0;
/** Exact native field bq_ntc, native scalar int8. */
bq_ntc:Array<i8>=new Array<i8>();
/** Exact native field mcu_ntc, native scalar int8. */
mcu_ntc:Array<i8>=new Array<i8>();
/** Exact native field cell_vol, native scalar uint16. */
cell_vol:Array<u16>=new Array<u16>();
constructor(){
for(let i=0;i<2;++i)this.bq_ntc.push(0);
for(let i=0;i<2;++i)this.mcu_ntc.push(0);
for(let i=0;i<15;++i)this.cell_vol.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>44-n)return -90;n+=1;
if(n>44)return -90;
}
{
if(1>44-n)return -90;n+=1;
if(n>44)return -90;
}
{
if(1>44-n)return -90;n+=1;
if(n>44)return -90;
}
{
if(1>44-n)return -90;n+=1;
if(n>44)return -90;
}
{
if(4>44-n)return -90;n+=4;
if(n>44)return -90;
}
{
if(2>44-n)return -90;n+=2;
if(n>44)return -90;
}
{
if(this.bq_ntc.length!=2)return -22;
for(let i=0;i<this.bq_ntc.length;++i){
if(1>44-n)return -90;n+=1;
if(n>44)return -90;
}
}
{
if(this.mcu_ntc.length!=2)return -22;
for(let i=0;i<this.mcu_ntc.length;++i){
if(1>44-n)return -90;n+=1;
if(n>44)return -90;
}
}
{
if(this.cell_vol.length!=15)return -22;
for(let i=0;i<this.cell_vol.length;++i){
if(2>44-n)return -90;n+=2;
if(n>44)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.version_high,1);
w.integer(<u64>this.version_low,1);
w.integer(<u64>this.status,1);
w.integer(<u64>this.soc,1);
w.integer(<u64>this.current,4);
w.integer(<u64>this.cycle,2);
for(let i=0;i<this.bq_ntc.length;++i){
w.integer(<u64>this.bq_ntc[i],1);
}
for(let i=0;i<this.mcu_ntc.length;++i){
w.integer(<u64>this.mcu_ntc[i],1);
}
for(let i=0;i<this.cell_vol.length;++i){
w.integer(<u64>this.cell_vol[i],2);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgBmsState{const value=new Ros2Unitree_goMsgBmsState();
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
value.status=decoded;
}
{
const decoded=<u8>r.integer(1);
value.soc=decoded;
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
value.bq_ntc=new Array<i8>();for(let i:u32=0;i<count;++i){
const decoded=<i8>r.integer(1);
value.bq_ntc.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.mcu_ntc=new Array<i8>();for(let i:u32=0;i<count;++i){
const decoded=<i8>r.integer(1);
value.mcu_ntc.push(decoded);
}
}
{
const count:u32=15;if(!r.valid||count>15){r.valid=false;return value;}
value.cell_vol=new Array<u16>();for(let i:u32=0;i<count;++i){
const decoded=<u16>r.integer(2);
value.cell_vol.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_goMsgBmsStateCodec implements VendorCodec<Ros2Unitree_goMsgBmsState>{
static readonly codecIdentity:string="UWVMROS1_7e4d5e3c0a2eddddf39c68b73bff5f7721e1623b0a9322dce23d3e551a07f5b0";
static readonly maxWireBytes:i32=44;
encodeChecked(value:Ros2Unitree_goMsgBmsState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgBmsState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgBmsState>{const out=new VendorDecoded<Ros2Unitree_goMsgBmsState>();if(bytes.length>44){out.status=90;return out;}if(bytes.length<44){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgBmsState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgBmsState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgBmsState():changetype<Ros2Unitree_goMsgBmsState>(out.value);}
}
/** Complete original unitree_go/msg/Error; spellings are published in generation proof. */
export class Ros2Unitree_goMsgError {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field source, native scalar uint32. */
source:u32=0;
/** Exact native field state, native scalar uint32. */
state:u32=0;
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
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.source,4);
w.integer(<u64>this.state,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgError{const value=new Ros2Unitree_goMsgError();
{
const decoded=<u32>r.integer(4);
value.source=decoded;
}
{
const decoded=<u32>r.integer(4);
value.state=decoded;
}
return value;}
}
export class Ros2Unitree_goMsgErrorCodec implements VendorCodec<Ros2Unitree_goMsgError>{
static readonly codecIdentity:string="UWVMROS1_c3e572e08288b6ed0b32575e645fe546f92b5091d62e57db01ec00d27f5e4afd";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Unitree_goMsgError):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgError):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgError>{const out=new VendorDecoded<Ros2Unitree_goMsgError>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgError.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgError{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgError():changetype<Ros2Unitree_goMsgError>(out.value);}
}
/** Complete original unitree_go/msg/Go2FrontVideoData; spellings are published in generation proof. */
export class Ros2Unitree_goMsgGo2FrontVideoData {
static readonly maxWireBytes:i32=3092;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3092;
/** Exact native field time_frame, native scalar uint64. */
time_frame:u64=0;
/** Exact native field video720p, native scalar uint8. */
video720p:Array<u8>=new Array<u8>();
/** Exact native field video360p, native scalar uint8. */
video360p:Array<u8>=new Array<u8>();
/** Exact native field video180p, native scalar uint8. */
video180p:Array<u8>=new Array<u8>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>3092-n)return -90;n+=8;
if(n>3092)return -90;
}
{
if(this.video720p.length>1024)return -22;
if(4>3092-n)return -90;n+=4;
for(let i=0;i<this.video720p.length;++i){
if(1>3092-n)return -90;n+=1;
if(n>3092)return -90;
}
}
{
if(this.video360p.length>1024)return -22;
if(4>3092-n)return -90;n+=4;
for(let i=0;i<this.video360p.length;++i){
if(1>3092-n)return -90;n+=1;
if(n>3092)return -90;
}
}
{
if(this.video180p.length>1024)return -22;
if(4>3092-n)return -90;n+=4;
for(let i=0;i<this.video180p.length;++i){
if(1>3092-n)return -90;n+=1;
if(n>3092)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.time_frame,8);
w.u32(<u32>this.video720p.length);
for(let i=0;i<this.video720p.length;++i){
w.integer(<u64>this.video720p[i],1);
}
w.u32(<u32>this.video360p.length);
for(let i=0;i<this.video360p.length;++i){
w.integer(<u64>this.video360p[i],1);
}
w.u32(<u32>this.video180p.length);
for(let i=0;i<this.video180p.length;++i){
w.integer(<u64>this.video180p[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgGo2FrontVideoData{const value=new Ros2Unitree_goMsgGo2FrontVideoData();
{
const decoded=<u64>r.integer(8);
value.time_frame=decoded;
}
{
const count=r.u32();if(!r.valid||count>1024){r.valid=false;return value;}
value.video720p=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.video720p.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>1024){r.valid=false;return value;}
value.video360p=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.video360p.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>1024){r.valid=false;return value;}
value.video180p=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.video180p.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_goMsgGo2FrontVideoDataCodec implements VendorCodec<Ros2Unitree_goMsgGo2FrontVideoData>{
static readonly codecIdentity:string="UWVMROS1_322b6bd4e563436838fe6a05174d173c0d95eb48108b889dca4f61ce6cc71c60";
static readonly maxWireBytes:i32=3092;
encodeChecked(value:Ros2Unitree_goMsgGo2FrontVideoData):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgGo2FrontVideoData):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgGo2FrontVideoData>{const out=new VendorDecoded<Ros2Unitree_goMsgGo2FrontVideoData>();if(bytes.length>3092){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgGo2FrontVideoData.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgGo2FrontVideoData{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgGo2FrontVideoData():changetype<Ros2Unitree_goMsgGo2FrontVideoData>(out.value);}
}
/** Complete original unitree_go/msg/HeightMap; spellings are published in generation proof. */
export class Ros2Unitree_goMsgHeightMap {
static readonly maxWireBytes:i32=8356;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8356;
/** Exact native field stamp, native scalar float64. */
stamp:f64=0;
/** Exact native field frame_id, native scalar string. */
frame_id:string="";
/** Exact native field resolution, native scalar float32. */
resolution:f32=0;
/** Exact native field width, native scalar uint32. */
width:u32=0;
/** Exact native field height, native scalar uint32. */
height:u32=0;
/** Exact native field origin, native scalar float32. */
origin:Array<f32>=new Array<f32>();
/** Exact native field data, native scalar float32. */
data:Array<f32>=new Array<f32>();
constructor(){
for(let i=0;i<2;++i)this.origin.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.stamp))return -22;
if(8>8356-n)return -90;n+=8;
if(n>8356)return -90;
}
{
if(!validUnicode(this.frame_id)||this.frame_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.frame_id);if(size>128)return -22;if(4+size>8356-n)return -90;n+=4+size;
if(n>8356)return -90;
}
{
if(!isFinite(this.resolution))return -22;
if(4>8356-n)return -90;n+=4;
if(n>8356)return -90;
}
{
if(4>8356-n)return -90;n+=4;
if(n>8356)return -90;
}
{
if(4>8356-n)return -90;n+=4;
if(n>8356)return -90;
}
{
if(this.origin.length!=2)return -22;
for(let i=0;i<this.origin.length;++i){
if(!isFinite(this.origin[i]))return -22;
if(4>8356-n)return -90;n+=4;
if(n>8356)return -90;
}
}
{
if(this.data.length>2048)return -22;
if(4>8356-n)return -90;n+=4;
for(let i=0;i<this.data.length;++i){
if(!isFinite(this.data[i]))return -22;
if(4>8356-n)return -90;n+=4;
if(n>8356)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.stamp),8);
w.text(this.frame_id);
w.integer(<u64>reinterpret<u32>(this.resolution),4);
w.integer(<u64>this.width,4);
w.integer(<u64>this.height,4);
for(let i=0;i<this.origin.length;++i){
w.integer(<u64>reinterpret<u32>(this.origin[i]),4);
}
w.u32(<u32>this.data.length);
for(let i=0;i<this.data.length;++i){
w.integer(<u64>reinterpret<u32>(this.data[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgHeightMap{const value=new Ros2Unitree_goMsgHeightMap();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.stamp=decoded;
}
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.frame_id=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.resolution=decoded;
}
{
const decoded=<u32>r.integer(4);
value.width=decoded;
}
{
const decoded=<u32>r.integer(4);
value.height=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.origin=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.origin.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2048){r.valid=false;return value;}
value.data=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.data.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_goMsgHeightMapCodec implements VendorCodec<Ros2Unitree_goMsgHeightMap>{
static readonly codecIdentity:string="UWVMROS1_f4aed8c4e1ba382c418ca83a10ae617355f1169757128be8b7cca46611bb0012";
static readonly maxWireBytes:i32=8356;
encodeChecked(value:Ros2Unitree_goMsgHeightMap):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgHeightMap):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgHeightMap>{const out=new VendorDecoded<Ros2Unitree_goMsgHeightMap>();if(bytes.length>8356){out.status=90;return out;}if(bytes.length<36){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgHeightMap.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgHeightMap{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgHeightMap():changetype<Ros2Unitree_goMsgHeightMap>(out.value);}
}
/** Complete original unitree_go/msg/IMUState; spellings are published in generation proof. */
export class Ros2Unitree_goMsgIMUState {
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
static readControl(r:Reader):Ros2Unitree_goMsgIMUState{const value=new Ros2Unitree_goMsgIMUState();
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
export class Ros2Unitree_goMsgIMUStateCodec implements VendorCodec<Ros2Unitree_goMsgIMUState>{
static readonly codecIdentity:string="UWVMROS1_e5326e97c3bfaa06962ad1989d1aa13fa0e60af15d39d9ef6da6ac0a37261bbc";
static readonly maxWireBytes:i32=53;
encodeChecked(value:Ros2Unitree_goMsgIMUState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgIMUState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgIMUState>{const out=new VendorDecoded<Ros2Unitree_goMsgIMUState>();if(bytes.length>53){out.status=90;return out;}if(bytes.length<53){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgIMUState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgIMUState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgIMUState():changetype<Ros2Unitree_goMsgIMUState>(out.value);}
}
/** Complete original unitree_go/msg/InterfaceConfig; spellings are published in generation proof. */
export class Ros2Unitree_goMsgInterfaceConfig {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact native field value, native scalar uint8. */
value:u8=0;
/** Exact native field reserve, native scalar uint8. */
reserve:Array<u8>=new Array<u8>();
constructor(){
for(let i=0;i<2;++i)this.reserve.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>4-n)return -90;n+=1;
if(n>4)return -90;
}
{
if(1>4-n)return -90;n+=1;
if(n>4)return -90;
}
{
if(this.reserve.length!=2)return -22;
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
w.integer(<u64>this.mode,1);
w.integer(<u64>this.value,1);
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgInterfaceConfig{const value=new Ros2Unitree_goMsgInterfaceConfig();
{
const decoded=<u8>r.integer(1);
value.mode=decoded;
}
{
const decoded=<u8>r.integer(1);
value.value=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.reserve=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.reserve.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_goMsgInterfaceConfigCodec implements VendorCodec<Ros2Unitree_goMsgInterfaceConfig>{
static readonly codecIdentity:string="UWVMROS1_9b96ed6672f6a3e039edb28dec8384cd195116dda929d5ba7be85ef7e7668110";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros2Unitree_goMsgInterfaceConfig):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgInterfaceConfig):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgInterfaceConfig>{const out=new VendorDecoded<Ros2Unitree_goMsgInterfaceConfig>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgInterfaceConfig.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgInterfaceConfig{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgInterfaceConfig():changetype<Ros2Unitree_goMsgInterfaceConfig>(out.value);}
}
/** Complete original unitree_go/msg/LidarState; spellings are published in generation proof. */
export class Ros2Unitree_goMsgLidarState {
static readonly maxWireBytes:i32=465;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=465;
/** Exact native field stamp, native scalar float64. */
stamp:f64=0;
/** Exact native field firmware_version, native scalar string. */
firmware_version:string="";
/** Exact native field software_version, native scalar string. */
software_version:string="";
/** Exact native field sdk_version, native scalar string. */
sdk_version:string="";
/** Exact native field sys_rotation_speed, native scalar float32. */
sys_rotation_speed:f32=0;
/** Exact native field com_rotation_speed, native scalar float32. */
com_rotation_speed:f32=0;
/** Exact native field error_state, native scalar uint8. */
error_state:u8=0;
/** Exact native field cloud_frequency, native scalar float32. */
cloud_frequency:f32=0;
/** Exact native field cloud_packet_loss_rate, native scalar float32. */
cloud_packet_loss_rate:f32=0;
/** Exact native field cloud_size, native scalar uint32. */
cloud_size:u32=0;
/** Exact native field cloud_scan_num, native scalar uint32. */
cloud_scan_num:u32=0;
/** Exact native field imu_frequency, native scalar float32. */
imu_frequency:f32=0;
/** Exact native field imu_packet_loss_rate, native scalar float32. */
imu_packet_loss_rate:f32=0;
/** Exact native field imu_rpy, native scalar float32. */
imu_rpy:Array<f32>=new Array<f32>();
/** Exact native field serial_recv_stamp, native scalar float64. */
serial_recv_stamp:f64=0;
/** Exact native field serial_buffer_size, native scalar uint32. */
serial_buffer_size:u32=0;
/** Exact native field serial_buffer_read, native scalar uint32. */
serial_buffer_read:u32=0;
constructor(){
for(let i=0;i<3;++i)this.imu_rpy.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.stamp))return -22;
if(8>465-n)return -90;n+=8;
if(n>465)return -90;
}
{
if(!validUnicode(this.firmware_version)||this.firmware_version.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.firmware_version);if(size>128)return -22;if(4+size>465-n)return -90;n+=4+size;
if(n>465)return -90;
}
{
if(!validUnicode(this.software_version)||this.software_version.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.software_version);if(size>128)return -22;if(4+size>465-n)return -90;n+=4+size;
if(n>465)return -90;
}
{
if(!validUnicode(this.sdk_version)||this.sdk_version.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.sdk_version);if(size>128)return -22;if(4+size>465-n)return -90;n+=4+size;
if(n>465)return -90;
}
{
if(!isFinite(this.sys_rotation_speed))return -22;
if(4>465-n)return -90;n+=4;
if(n>465)return -90;
}
{
if(!isFinite(this.com_rotation_speed))return -22;
if(4>465-n)return -90;n+=4;
if(n>465)return -90;
}
{
if(1>465-n)return -90;n+=1;
if(n>465)return -90;
}
{
if(!isFinite(this.cloud_frequency))return -22;
if(4>465-n)return -90;n+=4;
if(n>465)return -90;
}
{
if(!isFinite(this.cloud_packet_loss_rate))return -22;
if(4>465-n)return -90;n+=4;
if(n>465)return -90;
}
{
if(4>465-n)return -90;n+=4;
if(n>465)return -90;
}
{
if(4>465-n)return -90;n+=4;
if(n>465)return -90;
}
{
if(!isFinite(this.imu_frequency))return -22;
if(4>465-n)return -90;n+=4;
if(n>465)return -90;
}
{
if(!isFinite(this.imu_packet_loss_rate))return -22;
if(4>465-n)return -90;n+=4;
if(n>465)return -90;
}
{
if(this.imu_rpy.length!=3)return -22;
for(let i=0;i<this.imu_rpy.length;++i){
if(!isFinite(this.imu_rpy[i]))return -22;
if(4>465-n)return -90;n+=4;
if(n>465)return -90;
}
}
{
if(!isFinite(this.serial_recv_stamp))return -22;
if(8>465-n)return -90;n+=8;
if(n>465)return -90;
}
{
if(4>465-n)return -90;n+=4;
if(n>465)return -90;
}
{
if(4>465-n)return -90;n+=4;
if(n>465)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u64>(this.stamp),8);
w.text(this.firmware_version);
w.text(this.software_version);
w.text(this.sdk_version);
w.integer(<u64>reinterpret<u32>(this.sys_rotation_speed),4);
w.integer(<u64>reinterpret<u32>(this.com_rotation_speed),4);
w.integer(<u64>this.error_state,1);
w.integer(<u64>reinterpret<u32>(this.cloud_frequency),4);
w.integer(<u64>reinterpret<u32>(this.cloud_packet_loss_rate),4);
w.integer(<u64>this.cloud_size,4);
w.integer(<u64>this.cloud_scan_num,4);
w.integer(<u64>reinterpret<u32>(this.imu_frequency),4);
w.integer(<u64>reinterpret<u32>(this.imu_packet_loss_rate),4);
for(let i=0;i<this.imu_rpy.length;++i){
w.integer(<u64>reinterpret<u32>(this.imu_rpy[i]),4);
}
w.integer(<u64>reinterpret<u64>(this.serial_recv_stamp),8);
w.integer(<u64>this.serial_buffer_size,4);
w.integer(<u64>this.serial_buffer_read,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgLidarState{const value=new Ros2Unitree_goMsgLidarState();
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.stamp=decoded;
}
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.firmware_version=decoded;
}
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.software_version=decoded;
}
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.sdk_version=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.sys_rotation_speed=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.com_rotation_speed=decoded;
}
{
const decoded=<u8>r.integer(1);
value.error_state=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cloud_frequency=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.cloud_packet_loss_rate=decoded;
}
{
const decoded=<u32>r.integer(4);
value.cloud_size=decoded;
}
{
const decoded=<u32>r.integer(4);
value.cloud_scan_num=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.imu_frequency=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.imu_packet_loss_rate=decoded;
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.imu_rpy=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.imu_rpy.push(decoded);
}
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.serial_recv_stamp=decoded;
}
{
const decoded=<u32>r.integer(4);
value.serial_buffer_size=decoded;
}
{
const decoded=<u32>r.integer(4);
value.serial_buffer_read=decoded;
}
return value;}
}
export class Ros2Unitree_goMsgLidarStateCodec implements VendorCodec<Ros2Unitree_goMsgLidarState>{
static readonly codecIdentity:string="UWVMROS1_edc9253eb66e466ae5a893b830b0bc5c9102ef18d2bbed7f7d9ead67bcb1a8e6";
static readonly maxWireBytes:i32=465;
encodeChecked(value:Ros2Unitree_goMsgLidarState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgLidarState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgLidarState>{const out=new VendorDecoded<Ros2Unitree_goMsgLidarState>();if(bytes.length>465){out.status=90;return out;}if(bytes.length<81){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgLidarState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgLidarState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgLidarState():changetype<Ros2Unitree_goMsgLidarState>(out.value);}
}
/** Complete original unitree_go/msg/MotorCmd; spellings are published in generation proof. */
export class Ros2Unitree_goMsgMotorCmd {
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
/** Exact native field kp, native scalar float32. */
kp:f32=0;
/** Exact native field kd, native scalar float32. */
kd:f32=0;
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
if(!isFinite(this.kp))return -22;
if(4>33-n)return -90;n+=4;
if(n>33)return -90;
}
{
if(!isFinite(this.kd))return -22;
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
w.integer(<u64>reinterpret<u32>(this.kp),4);
w.integer(<u64>reinterpret<u32>(this.kd),4);
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgMotorCmd{const value=new Ros2Unitree_goMsgMotorCmd();
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
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.reserve=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.reserve.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_goMsgMotorCmdCodec implements VendorCodec<Ros2Unitree_goMsgMotorCmd>{
static readonly codecIdentity:string="UWVMROS1_e5b88c02a6d339ae4ae2706678a3d82f89904f024b6068f4fd174e1e045b0989";
static readonly maxWireBytes:i32=33;
encodeChecked(value:Ros2Unitree_goMsgMotorCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgMotorCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgMotorCmd>{const out=new VendorDecoded<Ros2Unitree_goMsgMotorCmd>();if(bytes.length>33){out.status=90;return out;}if(bytes.length<33){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgMotorCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgMotorCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgMotorCmd():changetype<Ros2Unitree_goMsgMotorCmd>(out.value);}
}
/** Complete original unitree_go/msg/LowCmd; spellings are published in generation proof. */
export class Ros2Unitree_goMsgLowCmd {
static readonly maxWireBytes:i32=749;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=749;
/** Exact native field head, native scalar uint8. */
head:Array<u8>=new Array<u8>();
/** Exact native field level_flag, native scalar uint8. */
level_flag:u8=0;
/** Exact native field frame_reserve, native scalar uint8. */
frame_reserve:u8=0;
/** Exact native field sn, native scalar uint32. */
sn:Array<u32>=new Array<u32>();
/** Exact native field version, native scalar uint32. */
version:Array<u32>=new Array<u32>();
/** Exact native field bandwidth, native scalar uint16. */
bandwidth:u16=0;
/** Exact native field motor_cmd, native scalar unitree_go/msg/MotorCmd. */
motor_cmd:Array<Ros2Unitree_goMsgMotorCmd>=new Array<Ros2Unitree_goMsgMotorCmd>();
/** Exact native field bms_cmd, native scalar unitree_go/msg/BmsCmd. */
bms_cmd:Ros2Unitree_goMsgBmsCmd=new Ros2Unitree_goMsgBmsCmd();
/** Exact native field wireless_remote, native scalar uint8. */
wireless_remote:Array<u8>=new Array<u8>();
/** Exact native field led, native scalar uint8. */
led:Array<u8>=new Array<u8>();
/** Exact native field fan, native scalar uint8. */
fan:Array<u8>=new Array<u8>();
/** Exact native field gpio, native scalar uint8. */
gpio:u8=0;
/** Exact native field reserve, native scalar uint32. */
reserve:u32=0;
/** Exact native field crc, native scalar uint32. */
crc:u32=0;
constructor(){
for(let i=0;i<2;++i)this.head.push(0);
for(let i=0;i<2;++i)this.sn.push(0);
for(let i=0;i<2;++i)this.version.push(0);
for(let i=0;i<20;++i)this.motor_cmd.push(new Ros2Unitree_goMsgMotorCmd());
for(let i=0;i<40;++i)this.wireless_remote.push(0);
for(let i=0;i<12;++i)this.led.push(0);
for(let i=0;i<2;++i)this.fan.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.head.length!=2)return -22;
for(let i=0;i<this.head.length;++i){
if(1>749-n)return -90;n+=1;
if(n>749)return -90;
}
}
{
if(1>749-n)return -90;n+=1;
if(n>749)return -90;
}
{
if(1>749-n)return -90;n+=1;
if(n>749)return -90;
}
{
if(this.sn.length!=2)return -22;
for(let i=0;i<this.sn.length;++i){
if(4>749-n)return -90;n+=4;
if(n>749)return -90;
}
}
{
if(this.version.length!=2)return -22;
for(let i=0;i<this.version.length;++i){
if(4>749-n)return -90;n+=4;
if(n>749)return -90;
}
}
{
if(2>749-n)return -90;n+=2;
if(n>749)return -90;
}
{
if(this.motor_cmd.length!=20)return -22;
for(let i=0;i<this.motor_cmd.length;++i){
const child=this.motor_cmd[i].wireSize();if(child<0)return child;if(child>749-n)return -90;n+=child;
if(n>749)return -90;
}
}
{
const child=this.bms_cmd.wireSize();if(child<0)return child;if(child>749-n)return -90;n+=child;
if(n>749)return -90;
}
{
if(this.wireless_remote.length!=40)return -22;
for(let i=0;i<this.wireless_remote.length;++i){
if(1>749-n)return -90;n+=1;
if(n>749)return -90;
}
}
{
if(this.led.length!=12)return -22;
for(let i=0;i<this.led.length;++i){
if(1>749-n)return -90;n+=1;
if(n>749)return -90;
}
}
{
if(this.fan.length!=2)return -22;
for(let i=0;i<this.fan.length;++i){
if(1>749-n)return -90;n+=1;
if(n>749)return -90;
}
}
{
if(1>749-n)return -90;n+=1;
if(n>749)return -90;
}
{
if(4>749-n)return -90;n+=4;
if(n>749)return -90;
}
{
if(4>749-n)return -90;n+=4;
if(n>749)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.head.length;++i){
w.integer(<u64>this.head[i],1);
}
w.integer(<u64>this.level_flag,1);
w.integer(<u64>this.frame_reserve,1);
for(let i=0;i<this.sn.length;++i){
w.integer(<u64>this.sn[i],4);
}
for(let i=0;i<this.version.length;++i){
w.integer(<u64>this.version[i],4);
}
w.integer(<u64>this.bandwidth,2);
for(let i=0;i<this.motor_cmd.length;++i){
this.motor_cmd[i].writeControl(w);
}
this.bms_cmd.writeControl(w);
for(let i=0;i<this.wireless_remote.length;++i){
w.integer(<u64>this.wireless_remote[i],1);
}
for(let i=0;i<this.led.length;++i){
w.integer(<u64>this.led[i],1);
}
for(let i=0;i<this.fan.length;++i){
w.integer(<u64>this.fan[i],1);
}
w.integer(<u64>this.gpio,1);
w.integer(<u64>this.reserve,4);
w.integer(<u64>this.crc,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgLowCmd{const value=new Ros2Unitree_goMsgLowCmd();
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.head=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.head.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.level_flag=decoded;
}
{
const decoded=<u8>r.integer(1);
value.frame_reserve=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.sn=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.sn.push(decoded);
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
value.bandwidth=decoded;
}
{
const count:u32=20;if(!r.valid||count>20){r.valid=false;return value;}
value.motor_cmd=new Array<Ros2Unitree_goMsgMotorCmd>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Unitree_goMsgMotorCmd.readControl(r);
value.motor_cmd.push(decoded);
}
}
{
const decoded=Ros2Unitree_goMsgBmsCmd.readControl(r);
value.bms_cmd=decoded;
}
{
const count:u32=40;if(!r.valid||count>40){r.valid=false;return value;}
value.wireless_remote=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.wireless_remote.push(decoded);
}
}
{
const count:u32=12;if(!r.valid||count>12){r.valid=false;return value;}
value.led=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.led.push(decoded);
}
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.fan=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.fan.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.gpio=decoded;
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
export class Ros2Unitree_goMsgLowCmdCodec implements VendorCodec<Ros2Unitree_goMsgLowCmd>{
static readonly codecIdentity:string="UWVMROS1_efd061d06d38ecc686edb95d1a891c26860b16da1e35747fc97eeaf4a5d93d63";
static readonly maxWireBytes:i32=749;
encodeChecked(value:Ros2Unitree_goMsgLowCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgLowCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgLowCmd>{const out=new VendorDecoded<Ros2Unitree_goMsgLowCmd>();if(bytes.length>749){out.status=90;return out;}if(bytes.length<749){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgLowCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgLowCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgLowCmd():changetype<Ros2Unitree_goMsgLowCmd>(out.value);}
}
/** Complete original unitree_go/msg/MotorState; spellings are published in generation proof. */
export class Ros2Unitree_goMsgMotorState {
static readonly maxWireBytes:i32=42;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=42;
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
/** Exact native field q_raw, native scalar float32. */
q_raw:f32=0;
/** Exact native field dq_raw, native scalar float32. */
dq_raw:f32=0;
/** Exact native field ddq_raw, native scalar float32. */
ddq_raw:f32=0;
/** Exact native field temperature, native scalar int8. */
temperature:i8=0;
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
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(!isFinite(this.q))return -22;
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(!isFinite(this.dq))return -22;
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(!isFinite(this.ddq))return -22;
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(!isFinite(this.tau_est))return -22;
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(!isFinite(this.q_raw))return -22;
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(!isFinite(this.dq_raw))return -22;
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(!isFinite(this.ddq_raw))return -22;
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(this.reserve.length!=2)return -22;
for(let i=0;i<this.reserve.length;++i){
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
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
w.integer(<u64>reinterpret<u32>(this.q_raw),4);
w.integer(<u64>reinterpret<u32>(this.dq_raw),4);
w.integer(<u64>reinterpret<u32>(this.ddq_raw),4);
w.integer(<u64>this.temperature,1);
w.integer(<u64>this.lost,4);
for(let i=0;i<this.reserve.length;++i){
w.integer(<u64>this.reserve[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgMotorState{const value=new Ros2Unitree_goMsgMotorState();
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
export class Ros2Unitree_goMsgMotorStateCodec implements VendorCodec<Ros2Unitree_goMsgMotorState>{
static readonly codecIdentity:string="UWVMROS1_b0b5e638b9fd9eec6d0019422d417f80c60e61d47f4750fe9adac8c5084a2a62";
static readonly maxWireBytes:i32=42;
encodeChecked(value:Ros2Unitree_goMsgMotorState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgMotorState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgMotorState>{const out=new VendorDecoded<Ros2Unitree_goMsgMotorState>();if(bytes.length>42){out.status=90;return out;}if(bytes.length<42){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgMotorState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgMotorState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgMotorState():changetype<Ros2Unitree_goMsgMotorState>(out.value);}
}
/** Complete original unitree_go/msg/LowState; spellings are published in generation proof. */
export class Ros2Unitree_goMsgLowState {
static readonly maxWireBytes:i32=1050;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1050;
/** Exact native field head, native scalar uint8. */
head:Array<u8>=new Array<u8>();
/** Exact native field level_flag, native scalar uint8. */
level_flag:u8=0;
/** Exact native field frame_reserve, native scalar uint8. */
frame_reserve:u8=0;
/** Exact native field sn, native scalar uint32. */
sn:Array<u32>=new Array<u32>();
/** Exact native field version, native scalar uint32. */
version:Array<u32>=new Array<u32>();
/** Exact native field bandwidth, native scalar uint16. */
bandwidth:u16=0;
/** Exact native field imu_state, native scalar unitree_go/msg/IMUState. */
imu_state:Ros2Unitree_goMsgIMUState=new Ros2Unitree_goMsgIMUState();
/** Exact native field motor_state, native scalar unitree_go/msg/MotorState. */
motor_state:Array<Ros2Unitree_goMsgMotorState>=new Array<Ros2Unitree_goMsgMotorState>();
/** Exact native field bms_state, native scalar unitree_go/msg/BmsState. */
bms_state:Ros2Unitree_goMsgBmsState=new Ros2Unitree_goMsgBmsState();
/** Exact native field foot_force, native scalar int16. */
foot_force:Array<i16>=new Array<i16>();
/** Exact native field foot_force_est, native scalar int16. */
foot_force_est:Array<i16>=new Array<i16>();
/** Exact native field tick, native scalar uint32. */
tick:u32=0;
/** Exact native field wireless_remote, native scalar uint8. */
wireless_remote:Array<u8>=new Array<u8>();
/** Exact native field bit_flag, native scalar uint8. */
bit_flag:u8=0;
/** Exact native field adc_reel, native scalar float32. */
adc_reel:f32=0;
/** Exact native field temperature_ntc1, native scalar int8. */
temperature_ntc1:i8=0;
/** Exact native field temperature_ntc2, native scalar int8. */
temperature_ntc2:i8=0;
/** Exact native field power_v, native scalar float32. */
power_v:f32=0;
/** Exact native field power_a, native scalar float32. */
power_a:f32=0;
/** Exact native field fan_frequency, native scalar uint16. */
fan_frequency:Array<u16>=new Array<u16>();
/** Exact native field reserve, native scalar uint32. */
reserve:u32=0;
/** Exact native field crc, native scalar uint32. */
crc:u32=0;
constructor(){
for(let i=0;i<2;++i)this.head.push(0);
for(let i=0;i<2;++i)this.sn.push(0);
for(let i=0;i<2;++i)this.version.push(0);
for(let i=0;i<20;++i)this.motor_state.push(new Ros2Unitree_goMsgMotorState());
for(let i=0;i<4;++i)this.foot_force.push(0);
for(let i=0;i<4;++i)this.foot_force_est.push(0);
for(let i=0;i<40;++i)this.wireless_remote.push(0);
for(let i=0;i<4;++i)this.fan_frequency.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.head.length!=2)return -22;
for(let i=0;i<this.head.length;++i){
if(1>1050-n)return -90;n+=1;
if(n>1050)return -90;
}
}
{
if(1>1050-n)return -90;n+=1;
if(n>1050)return -90;
}
{
if(1>1050-n)return -90;n+=1;
if(n>1050)return -90;
}
{
if(this.sn.length!=2)return -22;
for(let i=0;i<this.sn.length;++i){
if(4>1050-n)return -90;n+=4;
if(n>1050)return -90;
}
}
{
if(this.version.length!=2)return -22;
for(let i=0;i<this.version.length;++i){
if(4>1050-n)return -90;n+=4;
if(n>1050)return -90;
}
}
{
if(2>1050-n)return -90;n+=2;
if(n>1050)return -90;
}
{
const child=this.imu_state.wireSize();if(child<0)return child;if(child>1050-n)return -90;n+=child;
if(n>1050)return -90;
}
{
if(this.motor_state.length!=20)return -22;
for(let i=0;i<this.motor_state.length;++i){
const child=this.motor_state[i].wireSize();if(child<0)return child;if(child>1050-n)return -90;n+=child;
if(n>1050)return -90;
}
}
{
const child=this.bms_state.wireSize();if(child<0)return child;if(child>1050-n)return -90;n+=child;
if(n>1050)return -90;
}
{
if(this.foot_force.length!=4)return -22;
for(let i=0;i<this.foot_force.length;++i){
if(2>1050-n)return -90;n+=2;
if(n>1050)return -90;
}
}
{
if(this.foot_force_est.length!=4)return -22;
for(let i=0;i<this.foot_force_est.length;++i){
if(2>1050-n)return -90;n+=2;
if(n>1050)return -90;
}
}
{
if(4>1050-n)return -90;n+=4;
if(n>1050)return -90;
}
{
if(this.wireless_remote.length!=40)return -22;
for(let i=0;i<this.wireless_remote.length;++i){
if(1>1050-n)return -90;n+=1;
if(n>1050)return -90;
}
}
{
if(1>1050-n)return -90;n+=1;
if(n>1050)return -90;
}
{
if(!isFinite(this.adc_reel))return -22;
if(4>1050-n)return -90;n+=4;
if(n>1050)return -90;
}
{
if(1>1050-n)return -90;n+=1;
if(n>1050)return -90;
}
{
if(1>1050-n)return -90;n+=1;
if(n>1050)return -90;
}
{
if(!isFinite(this.power_v))return -22;
if(4>1050-n)return -90;n+=4;
if(n>1050)return -90;
}
{
if(!isFinite(this.power_a))return -22;
if(4>1050-n)return -90;n+=4;
if(n>1050)return -90;
}
{
if(this.fan_frequency.length!=4)return -22;
for(let i=0;i<this.fan_frequency.length;++i){
if(2>1050-n)return -90;n+=2;
if(n>1050)return -90;
}
}
{
if(4>1050-n)return -90;n+=4;
if(n>1050)return -90;
}
{
if(4>1050-n)return -90;n+=4;
if(n>1050)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.head.length;++i){
w.integer(<u64>this.head[i],1);
}
w.integer(<u64>this.level_flag,1);
w.integer(<u64>this.frame_reserve,1);
for(let i=0;i<this.sn.length;++i){
w.integer(<u64>this.sn[i],4);
}
for(let i=0;i<this.version.length;++i){
w.integer(<u64>this.version[i],4);
}
w.integer(<u64>this.bandwidth,2);
this.imu_state.writeControl(w);
for(let i=0;i<this.motor_state.length;++i){
this.motor_state[i].writeControl(w);
}
this.bms_state.writeControl(w);
for(let i=0;i<this.foot_force.length;++i){
w.integer(<u64>this.foot_force[i],2);
}
for(let i=0;i<this.foot_force_est.length;++i){
w.integer(<u64>this.foot_force_est[i],2);
}
w.integer(<u64>this.tick,4);
for(let i=0;i<this.wireless_remote.length;++i){
w.integer(<u64>this.wireless_remote[i],1);
}
w.integer(<u64>this.bit_flag,1);
w.integer(<u64>reinterpret<u32>(this.adc_reel),4);
w.integer(<u64>this.temperature_ntc1,1);
w.integer(<u64>this.temperature_ntc2,1);
w.integer(<u64>reinterpret<u32>(this.power_v),4);
w.integer(<u64>reinterpret<u32>(this.power_a),4);
for(let i=0;i<this.fan_frequency.length;++i){
w.integer(<u64>this.fan_frequency[i],2);
}
w.integer(<u64>this.reserve,4);
w.integer(<u64>this.crc,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgLowState{const value=new Ros2Unitree_goMsgLowState();
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.head=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.head.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.level_flag=decoded;
}
{
const decoded=<u8>r.integer(1);
value.frame_reserve=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.sn=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.sn.push(decoded);
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
value.bandwidth=decoded;
}
{
const decoded=Ros2Unitree_goMsgIMUState.readControl(r);
value.imu_state=decoded;
}
{
const count:u32=20;if(!r.valid||count>20){r.valid=false;return value;}
value.motor_state=new Array<Ros2Unitree_goMsgMotorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Unitree_goMsgMotorState.readControl(r);
value.motor_state.push(decoded);
}
}
{
const decoded=Ros2Unitree_goMsgBmsState.readControl(r);
value.bms_state=decoded;
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.foot_force=new Array<i16>();for(let i:u32=0;i<count;++i){
const decoded=<i16>r.integer(2);
value.foot_force.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.foot_force_est=new Array<i16>();for(let i:u32=0;i<count;++i){
const decoded=<i16>r.integer(2);
value.foot_force_est.push(decoded);
}
}
{
const decoded=<u32>r.integer(4);
value.tick=decoded;
}
{
const count:u32=40;if(!r.valid||count>40){r.valid=false;return value;}
value.wireless_remote=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.wireless_remote.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.bit_flag=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.adc_reel=decoded;
}
{
const decoded=<i8>r.integer(1);
value.temperature_ntc1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.temperature_ntc2=decoded;
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
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.fan_frequency=new Array<u16>();for(let i:u32=0;i<count;++i){
const decoded=<u16>r.integer(2);
value.fan_frequency.push(decoded);
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
export class Ros2Unitree_goMsgLowStateCodec implements VendorCodec<Ros2Unitree_goMsgLowState>{
static readonly codecIdentity:string="UWVMROS1_56c01d52b26d7b2ef1fa7475fc8a353777ed6952a6f1ceee01da81f1edad3cab";
static readonly maxWireBytes:i32=1050;
encodeChecked(value:Ros2Unitree_goMsgLowState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgLowState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgLowState>{const out=new VendorDecoded<Ros2Unitree_goMsgLowState>();if(bytes.length>1050){out.status=90;return out;}if(bytes.length<1050){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgLowState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgLowState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgLowState():changetype<Ros2Unitree_goMsgLowState>(out.value);}
}
/** Complete original unitree_go/msg/MotorCmds; spellings are published in generation proof. */
export class Ros2Unitree_goMsgMotorCmds {
static readonly maxWireBytes:i32=664;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=664;
/** Exact native field cmds, native scalar unitree_go/msg/MotorCmd. */
cmds:Array<Ros2Unitree_goMsgMotorCmd>=new Array<Ros2Unitree_goMsgMotorCmd>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.cmds.length>20)return -22;
if(4>664-n)return -90;n+=4;
for(let i=0;i<this.cmds.length;++i){
const child=this.cmds[i].wireSize();if(child<0)return child;if(child>664-n)return -90;n+=child;
if(n>664)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.cmds.length);
for(let i=0;i<this.cmds.length;++i){
this.cmds[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgMotorCmds{const value=new Ros2Unitree_goMsgMotorCmds();
{
const count=r.u32();if(!r.valid||count>20){r.valid=false;return value;}
value.cmds=new Array<Ros2Unitree_goMsgMotorCmd>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Unitree_goMsgMotorCmd.readControl(r);
value.cmds.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_goMsgMotorCmdsCodec implements VendorCodec<Ros2Unitree_goMsgMotorCmds>{
static readonly codecIdentity:string="UWVMROS1_59f6b09147eb868cad93205c27cdd339e55f5928c8eef32da11ec5675c61a671";
static readonly maxWireBytes:i32=664;
encodeChecked(value:Ros2Unitree_goMsgMotorCmds):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgMotorCmds):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgMotorCmds>{const out=new VendorDecoded<Ros2Unitree_goMsgMotorCmds>();if(bytes.length>664){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgMotorCmds.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgMotorCmds{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgMotorCmds():changetype<Ros2Unitree_goMsgMotorCmds>(out.value);}
}
/** Complete original unitree_go/msg/MotorStates; spellings are published in generation proof. */
export class Ros2Unitree_goMsgMotorStates {
static readonly maxWireBytes:i32=844;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=844;
/** Exact native field states, native scalar unitree_go/msg/MotorState. */
states:Array<Ros2Unitree_goMsgMotorState>=new Array<Ros2Unitree_goMsgMotorState>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.states.length>20)return -22;
if(4>844-n)return -90;n+=4;
for(let i=0;i<this.states.length;++i){
const child=this.states[i].wireSize();if(child<0)return child;if(child>844-n)return -90;n+=child;
if(n>844)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.states.length);
for(let i=0;i<this.states.length;++i){
this.states[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgMotorStates{const value=new Ros2Unitree_goMsgMotorStates();
{
const count=r.u32();if(!r.valid||count>20){r.valid=false;return value;}
value.states=new Array<Ros2Unitree_goMsgMotorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Unitree_goMsgMotorState.readControl(r);
value.states.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_goMsgMotorStatesCodec implements VendorCodec<Ros2Unitree_goMsgMotorStates>{
static readonly codecIdentity:string="UWVMROS1_0843528158199493384c36a65b0fb1e15244d21160c33aac82f9a87718c08178";
static readonly maxWireBytes:i32=844;
encodeChecked(value:Ros2Unitree_goMsgMotorStates):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgMotorStates):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgMotorStates>{const out=new VendorDecoded<Ros2Unitree_goMsgMotorStates>();if(bytes.length>844){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgMotorStates.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgMotorStates{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgMotorStates():changetype<Ros2Unitree_goMsgMotorStates>(out.value);}
}
/** Complete original unitree_go/msg/PathPoint; spellings are published in generation proof. */
export class Ros2Unitree_goMsgPathPoint {
static readonly maxWireBytes:i32=28;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=28;
/** Exact native field t_from_start, native scalar float32. */
t_from_start:f32=0;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field yaw, native scalar float32. */
yaw:f32=0;
/** Exact native field vx, native scalar float32. */
vx:f32=0;
/** Exact native field vy, native scalar float32. */
vy:f32=0;
/** Exact native field vyaw, native scalar float32. */
vyaw:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.t_from_start))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.x))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.yaw))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.vx))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.vy))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.vyaw))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.t_from_start),4);
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.yaw),4);
w.integer(<u64>reinterpret<u32>(this.vx),4);
w.integer(<u64>reinterpret<u32>(this.vy),4);
w.integer(<u64>reinterpret<u32>(this.vyaw),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgPathPoint{const value=new Ros2Unitree_goMsgPathPoint();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.t_from_start=decoded;
}
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
value.yaw=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.vx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.vy=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.vyaw=decoded;
}
return value;}
}
export class Ros2Unitree_goMsgPathPointCodec implements VendorCodec<Ros2Unitree_goMsgPathPoint>{
static readonly codecIdentity:string="UWVMROS1_35eb1cc629dcecbace17b342029c287310c30d4e6d988740bf2c212a1251cee4";
static readonly maxWireBytes:i32=28;
encodeChecked(value:Ros2Unitree_goMsgPathPoint):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgPathPoint):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgPathPoint>{const out=new VendorDecoded<Ros2Unitree_goMsgPathPoint>();if(bytes.length>28){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgPathPoint.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgPathPoint{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgPathPoint():changetype<Ros2Unitree_goMsgPathPoint>(out.value);}
}
/** Complete original unitree_go/msg/Req; spellings are published in generation proof. */
export class Ros2Unitree_goMsgReq {
static readonly maxWireBytes:i32=4232;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4232;
/** Exact native field uuid, native scalar string. */
uuid:string="";
/** Exact native field body, native scalar string. */
body:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.uuid)||this.uuid.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.uuid);if(size>128)return -22;if(4+size>4232-n)return -90;n+=4+size;
if(n>4232)return -90;
}
{
if(!validUnicode(this.body)||this.body.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.body);if(size>4096)return -22;if(4+size>4232-n)return -90;n+=4+size;
if(n>4232)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.uuid);
w.text(this.body);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgReq{const value=new Ros2Unitree_goMsgReq();
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.uuid=decoded;
}
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.body=decoded;
}
return value;}
}
export class Ros2Unitree_goMsgReqCodec implements VendorCodec<Ros2Unitree_goMsgReq>{
static readonly codecIdentity:string="UWVMROS1_43e0126993336d81a3f6a6706979c19f3a193f6a1ef504cadcd864be8bfaf84e";
static readonly maxWireBytes:i32=4232;
encodeChecked(value:Ros2Unitree_goMsgReq):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgReq):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgReq>{const out=new VendorDecoded<Ros2Unitree_goMsgReq>();if(bytes.length>4232){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgReq.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgReq{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgReq():changetype<Ros2Unitree_goMsgReq>(out.value);}
}
/** Complete original unitree_go/msg/Res; spellings are published in generation proof. */
export class Ros2Unitree_goMsgRes {
static readonly maxWireBytes:i32=5260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5260;
/** Exact native field uuid, native scalar string. */
uuid:string="";
/** Exact native field data, native scalar uint8. */
data:Array<u8>=new Array<u8>();
/** Exact native field body, native scalar string. */
body:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.uuid)||this.uuid.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.uuid);if(size>128)return -22;if(4+size>5260-n)return -90;n+=4+size;
if(n>5260)return -90;
}
{
if(this.data.length>1024)return -22;
if(4>5260-n)return -90;n+=4;
for(let i=0;i<this.data.length;++i){
if(1>5260-n)return -90;n+=1;
if(n>5260)return -90;
}
}
{
if(!validUnicode(this.body)||this.body.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.body);if(size>4096)return -22;if(4+size>5260-n)return -90;n+=4+size;
if(n>5260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.uuid);
w.u32(<u32>this.data.length);
for(let i=0;i<this.data.length;++i){
w.integer(<u64>this.data[i],1);
}
w.text(this.body);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgRes{const value=new Ros2Unitree_goMsgRes();
{
const size=r.u32();if(!r.valid||size>128){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.uuid=decoded;
}
{
const count=r.u32();if(!r.valid||count>1024){r.valid=false;return value;}
value.data=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.data.push(decoded);
}
}
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.body=decoded;
}
return value;}
}
export class Ros2Unitree_goMsgResCodec implements VendorCodec<Ros2Unitree_goMsgRes>{
static readonly codecIdentity:string="UWVMROS1_53c1d85c0cb17cfdd7d3799d6e92602a70700e8c55da4045f501a6a12ff55bfb";
static readonly maxWireBytes:i32=5260;
encodeChecked(value:Ros2Unitree_goMsgRes):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgRes):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgRes>{const out=new VendorDecoded<Ros2Unitree_goMsgRes>();if(bytes.length>5260){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgRes.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgRes{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgRes():changetype<Ros2Unitree_goMsgRes>(out.value);}
}
/** Complete original unitree_go/msg/SportModeCmd; spellings are published in generation proof. */
export class Ros2Unitree_goMsgSportModeCmd {
static readonly maxWireBytes:i32=887;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=887;
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact native field gait_type, native scalar uint8. */
gait_type:u8=0;
/** Exact native field speed_level, native scalar uint8. */
speed_level:u8=0;
/** Exact native field foot_raise_height, native scalar float32. */
foot_raise_height:f32=0;
/** Exact native field body_height, native scalar float32. */
body_height:f32=0;
/** Exact native field position, native scalar float32. */
position:Array<f32>=new Array<f32>();
/** Exact native field euler, native scalar float32. */
euler:Array<f32>=new Array<f32>();
/** Exact native field velocity, native scalar float32. */
velocity:Array<f32>=new Array<f32>();
/** Exact native field yaw_speed, native scalar float32. */
yaw_speed:f32=0;
/** Exact native field bms_cmd, native scalar unitree_go/msg/BmsCmd. */
bms_cmd:Ros2Unitree_goMsgBmsCmd=new Ros2Unitree_goMsgBmsCmd();
/** Exact native field path_point, native scalar unitree_go/msg/PathPoint. */
path_point:Array<Ros2Unitree_goMsgPathPoint>=new Array<Ros2Unitree_goMsgPathPoint>();
constructor(){
for(let i=0;i<2;++i)this.position.push(0);
for(let i=0;i<3;++i)this.euler.push(0);
for(let i=0;i<2;++i)this.velocity.push(0);
for(let i=0;i<30;++i)this.path_point.push(new Ros2Unitree_goMsgPathPoint());
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>887-n)return -90;n+=1;
if(n>887)return -90;
}
{
if(1>887-n)return -90;n+=1;
if(n>887)return -90;
}
{
if(1>887-n)return -90;n+=1;
if(n>887)return -90;
}
{
if(!isFinite(this.foot_raise_height))return -22;
if(4>887-n)return -90;n+=4;
if(n>887)return -90;
}
{
if(!isFinite(this.body_height))return -22;
if(4>887-n)return -90;n+=4;
if(n>887)return -90;
}
{
if(this.position.length!=2)return -22;
for(let i=0;i<this.position.length;++i){
if(!isFinite(this.position[i]))return -22;
if(4>887-n)return -90;n+=4;
if(n>887)return -90;
}
}
{
if(this.euler.length!=3)return -22;
for(let i=0;i<this.euler.length;++i){
if(!isFinite(this.euler[i]))return -22;
if(4>887-n)return -90;n+=4;
if(n>887)return -90;
}
}
{
if(this.velocity.length!=2)return -22;
for(let i=0;i<this.velocity.length;++i){
if(!isFinite(this.velocity[i]))return -22;
if(4>887-n)return -90;n+=4;
if(n>887)return -90;
}
}
{
if(!isFinite(this.yaw_speed))return -22;
if(4>887-n)return -90;n+=4;
if(n>887)return -90;
}
{
const child=this.bms_cmd.wireSize();if(child<0)return child;if(child>887-n)return -90;n+=child;
if(n>887)return -90;
}
{
if(this.path_point.length!=30)return -22;
for(let i=0;i<this.path_point.length;++i){
const child=this.path_point[i].wireSize();if(child<0)return child;if(child>887-n)return -90;n+=child;
if(n>887)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.mode,1);
w.integer(<u64>this.gait_type,1);
w.integer(<u64>this.speed_level,1);
w.integer(<u64>reinterpret<u32>(this.foot_raise_height),4);
w.integer(<u64>reinterpret<u32>(this.body_height),4);
for(let i=0;i<this.position.length;++i){
w.integer(<u64>reinterpret<u32>(this.position[i]),4);
}
for(let i=0;i<this.euler.length;++i){
w.integer(<u64>reinterpret<u32>(this.euler[i]),4);
}
for(let i=0;i<this.velocity.length;++i){
w.integer(<u64>reinterpret<u32>(this.velocity[i]),4);
}
w.integer(<u64>reinterpret<u32>(this.yaw_speed),4);
this.bms_cmd.writeControl(w);
for(let i=0;i<this.path_point.length;++i){
this.path_point[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgSportModeCmd{const value=new Ros2Unitree_goMsgSportModeCmd();
{
const decoded=<u8>r.integer(1);
value.mode=decoded;
}
{
const decoded=<u8>r.integer(1);
value.gait_type=decoded;
}
{
const decoded=<u8>r.integer(1);
value.speed_level=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.foot_raise_height=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.body_height=decoded;
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
value.yaw_speed=decoded;
}
{
const decoded=Ros2Unitree_goMsgBmsCmd.readControl(r);
value.bms_cmd=decoded;
}
{
const count:u32=30;if(!r.valid||count>30){r.valid=false;return value;}
value.path_point=new Array<Ros2Unitree_goMsgPathPoint>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Unitree_goMsgPathPoint.readControl(r);
value.path_point.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_goMsgSportModeCmdCodec implements VendorCodec<Ros2Unitree_goMsgSportModeCmd>{
static readonly codecIdentity:string="UWVMROS1_cf90a065007b9b8cedde3f0b58aa79da3b39a1456dc1b5297f4fe799d92ff22c";
static readonly maxWireBytes:i32=887;
encodeChecked(value:Ros2Unitree_goMsgSportModeCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgSportModeCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgSportModeCmd>{const out=new VendorDecoded<Ros2Unitree_goMsgSportModeCmd>();if(bytes.length>887){out.status=90;return out;}if(bytes.length<887){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgSportModeCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgSportModeCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgSportModeCmd():changetype<Ros2Unitree_goMsgSportModeCmd>(out.value);}
}
/** Complete original unitree_go/msg/TimeSpec; spellings are published in generation proof. */
export class Ros2Unitree_goMsgTimeSpec {
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
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.sec,4);
w.integer(<u64>this.nanosec,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgTimeSpec{const value=new Ros2Unitree_goMsgTimeSpec();
{
const decoded=<i32>r.integer(4);
value.sec=decoded;
}
{
const decoded=<u32>r.integer(4);
value.nanosec=decoded;
}
return value;}
}
export class Ros2Unitree_goMsgTimeSpecCodec implements VendorCodec<Ros2Unitree_goMsgTimeSpec>{
static readonly codecIdentity:string="UWVMROS1_9ad3c6ed5a9ff0bba22593b50e1d777ae2f63480a01a744d777ee6b7dad0ecc4";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Unitree_goMsgTimeSpec):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgTimeSpec):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgTimeSpec>{const out=new VendorDecoded<Ros2Unitree_goMsgTimeSpec>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgTimeSpec.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgTimeSpec{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgTimeSpec():changetype<Ros2Unitree_goMsgTimeSpec>(out.value);}
}
/** Complete original unitree_go/msg/SportModeState; spellings are published in generation proof. */
export class Ros2Unitree_goMsgSportModeState {
static readonly maxWireBytes:i32=227;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=227;
/** Exact native field stamp, native scalar unitree_go/msg/TimeSpec. */
stamp:Ros2Unitree_goMsgTimeSpec=new Ros2Unitree_goMsgTimeSpec();
/** Exact native field error_code, native scalar uint32. */
error_code:u32=0;
/** Exact native field imu_state, native scalar unitree_go/msg/IMUState. */
imu_state:Ros2Unitree_goMsgIMUState=new Ros2Unitree_goMsgIMUState();
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact native field progress, native scalar float32. */
progress:f32=0;
/** Exact native field gait_type, native scalar uint8. */
gait_type:u8=0;
/** Exact native field foot_raise_height, native scalar float32. */
foot_raise_height:f32=0;
/** Exact native field position, native scalar float32. */
position:Array<f32>=new Array<f32>();
/** Exact native field body_height, native scalar float32. */
body_height:f32=0;
/** Exact native field velocity, native scalar float32. */
velocity:Array<f32>=new Array<f32>();
/** Exact native field yaw_speed, native scalar float32. */
yaw_speed:f32=0;
/** Exact native field range_obstacle, native scalar float32. */
range_obstacle:Array<f32>=new Array<f32>();
/** Exact native field foot_force, native scalar int16. */
foot_force:Array<i16>=new Array<i16>();
/** Exact native field foot_position_body, native scalar float32. */
foot_position_body:Array<f32>=new Array<f32>();
/** Exact native field foot_speed_body, native scalar float32. */
foot_speed_body:Array<f32>=new Array<f32>();
constructor(){
for(let i=0;i<3;++i)this.position.push(0);
for(let i=0;i<3;++i)this.velocity.push(0);
for(let i=0;i<4;++i)this.range_obstacle.push(0);
for(let i=0;i<4;++i)this.foot_force.push(0);
for(let i=0;i<12;++i)this.foot_position_body.push(0);
for(let i=0;i<12;++i)this.foot_speed_body.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.stamp.wireSize();if(child<0)return child;if(child>227-n)return -90;n+=child;
if(n>227)return -90;
}
{
if(4>227-n)return -90;n+=4;
if(n>227)return -90;
}
{
const child=this.imu_state.wireSize();if(child<0)return child;if(child>227-n)return -90;n+=child;
if(n>227)return -90;
}
{
if(1>227-n)return -90;n+=1;
if(n>227)return -90;
}
{
if(!isFinite(this.progress))return -22;
if(4>227-n)return -90;n+=4;
if(n>227)return -90;
}
{
if(1>227-n)return -90;n+=1;
if(n>227)return -90;
}
{
if(!isFinite(this.foot_raise_height))return -22;
if(4>227-n)return -90;n+=4;
if(n>227)return -90;
}
{
if(this.position.length!=3)return -22;
for(let i=0;i<this.position.length;++i){
if(!isFinite(this.position[i]))return -22;
if(4>227-n)return -90;n+=4;
if(n>227)return -90;
}
}
{
if(!isFinite(this.body_height))return -22;
if(4>227-n)return -90;n+=4;
if(n>227)return -90;
}
{
if(this.velocity.length!=3)return -22;
for(let i=0;i<this.velocity.length;++i){
if(!isFinite(this.velocity[i]))return -22;
if(4>227-n)return -90;n+=4;
if(n>227)return -90;
}
}
{
if(!isFinite(this.yaw_speed))return -22;
if(4>227-n)return -90;n+=4;
if(n>227)return -90;
}
{
if(this.range_obstacle.length!=4)return -22;
for(let i=0;i<this.range_obstacle.length;++i){
if(!isFinite(this.range_obstacle[i]))return -22;
if(4>227-n)return -90;n+=4;
if(n>227)return -90;
}
}
{
if(this.foot_force.length!=4)return -22;
for(let i=0;i<this.foot_force.length;++i){
if(2>227-n)return -90;n+=2;
if(n>227)return -90;
}
}
{
if(this.foot_position_body.length!=12)return -22;
for(let i=0;i<this.foot_position_body.length;++i){
if(!isFinite(this.foot_position_body[i]))return -22;
if(4>227-n)return -90;n+=4;
if(n>227)return -90;
}
}
{
if(this.foot_speed_body.length!=12)return -22;
for(let i=0;i<this.foot_speed_body.length;++i){
if(!isFinite(this.foot_speed_body[i]))return -22;
if(4>227-n)return -90;n+=4;
if(n>227)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.stamp.writeControl(w);
w.integer(<u64>this.error_code,4);
this.imu_state.writeControl(w);
w.integer(<u64>this.mode,1);
w.integer(<u64>reinterpret<u32>(this.progress),4);
w.integer(<u64>this.gait_type,1);
w.integer(<u64>reinterpret<u32>(this.foot_raise_height),4);
for(let i=0;i<this.position.length;++i){
w.integer(<u64>reinterpret<u32>(this.position[i]),4);
}
w.integer(<u64>reinterpret<u32>(this.body_height),4);
for(let i=0;i<this.velocity.length;++i){
w.integer(<u64>reinterpret<u32>(this.velocity[i]),4);
}
w.integer(<u64>reinterpret<u32>(this.yaw_speed),4);
for(let i=0;i<this.range_obstacle.length;++i){
w.integer(<u64>reinterpret<u32>(this.range_obstacle[i]),4);
}
for(let i=0;i<this.foot_force.length;++i){
w.integer(<u64>this.foot_force[i],2);
}
for(let i=0;i<this.foot_position_body.length;++i){
w.integer(<u64>reinterpret<u32>(this.foot_position_body[i]),4);
}
for(let i=0;i<this.foot_speed_body.length;++i){
w.integer(<u64>reinterpret<u32>(this.foot_speed_body[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgSportModeState{const value=new Ros2Unitree_goMsgSportModeState();
{
const decoded=Ros2Unitree_goMsgTimeSpec.readControl(r);
value.stamp=decoded;
}
{
const decoded=<u32>r.integer(4);
value.error_code=decoded;
}
{
const decoded=Ros2Unitree_goMsgIMUState.readControl(r);
value.imu_state=decoded;
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
value.gait_type=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.foot_raise_height=decoded;
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
value.body_height=decoded;
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
value.yaw_speed=decoded;
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.range_obstacle=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.range_obstacle.push(decoded);
}
}
{
const count:u32=4;if(!r.valid||count>4){r.valid=false;return value;}
value.foot_force=new Array<i16>();for(let i:u32=0;i<count;++i){
const decoded=<i16>r.integer(2);
value.foot_force.push(decoded);
}
}
{
const count:u32=12;if(!r.valid||count>12){r.valid=false;return value;}
value.foot_position_body=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.foot_position_body.push(decoded);
}
}
{
const count:u32=12;if(!r.valid||count>12){r.valid=false;return value;}
value.foot_speed_body=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.foot_speed_body.push(decoded);
}
}
return value;}
}
export class Ros2Unitree_goMsgSportModeStateCodec implements VendorCodec<Ros2Unitree_goMsgSportModeState>{
static readonly codecIdentity:string="UWVMROS1_2935b9d838087267840b379582862a15c2e1a72fe8ad2bcbf8f19af2f8054760";
static readonly maxWireBytes:i32=227;
encodeChecked(value:Ros2Unitree_goMsgSportModeState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgSportModeState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgSportModeState>{const out=new VendorDecoded<Ros2Unitree_goMsgSportModeState>();if(bytes.length>227){out.status=90;return out;}if(bytes.length<227){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgSportModeState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgSportModeState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgSportModeState():changetype<Ros2Unitree_goMsgSportModeState>(out.value);}
}
/** Complete original unitree_go/msg/UwbState; spellings are published in generation proof. */
export class Ros2Unitree_goMsgUwbState {
static readonly maxWireBytes:i32=55;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=55;
/** Exact native field version, native scalar uint8. */
version:Array<u8>=new Array<u8>();
/** Exact native field channel, native scalar uint8. */
channel:u8=0;
/** Exact native field joy_mode, native scalar uint8. */
joy_mode:u8=0;
/** Exact native field orientation_est, native scalar float32. */
orientation_est:f32=0;
/** Exact native field pitch_est, native scalar float32. */
pitch_est:f32=0;
/** Exact native field distance_est, native scalar float32. */
distance_est:f32=0;
/** Exact native field yaw_est, native scalar float32. */
yaw_est:f32=0;
/** Exact native field tag_roll, native scalar float32. */
tag_roll:f32=0;
/** Exact native field tag_pitch, native scalar float32. */
tag_pitch:f32=0;
/** Exact native field tag_yaw, native scalar float32. */
tag_yaw:f32=0;
/** Exact native field base_roll, native scalar float32. */
base_roll:f32=0;
/** Exact native field base_pitch, native scalar float32. */
base_pitch:f32=0;
/** Exact native field base_yaw, native scalar float32. */
base_yaw:f32=0;
/** Exact native field joystick, native scalar float32. */
joystick:Array<f32>=new Array<f32>();
/** Exact native field error_state, native scalar uint8. */
error_state:u8=0;
/** Exact native field buttons, native scalar uint8. */
buttons:u8=0;
/** Exact native field enabled_from_app, native scalar uint8. */
enabled_from_app:u8=0;
constructor(){
for(let i=0;i<2;++i)this.version.push(0);
for(let i=0;i<2;++i)this.joystick.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.version.length!=2)return -22;
for(let i=0;i<this.version.length;++i){
if(1>55-n)return -90;n+=1;
if(n>55)return -90;
}
}
{
if(1>55-n)return -90;n+=1;
if(n>55)return -90;
}
{
if(1>55-n)return -90;n+=1;
if(n>55)return -90;
}
{
if(!isFinite(this.orientation_est))return -22;
if(4>55-n)return -90;n+=4;
if(n>55)return -90;
}
{
if(!isFinite(this.pitch_est))return -22;
if(4>55-n)return -90;n+=4;
if(n>55)return -90;
}
{
if(!isFinite(this.distance_est))return -22;
if(4>55-n)return -90;n+=4;
if(n>55)return -90;
}
{
if(!isFinite(this.yaw_est))return -22;
if(4>55-n)return -90;n+=4;
if(n>55)return -90;
}
{
if(!isFinite(this.tag_roll))return -22;
if(4>55-n)return -90;n+=4;
if(n>55)return -90;
}
{
if(!isFinite(this.tag_pitch))return -22;
if(4>55-n)return -90;n+=4;
if(n>55)return -90;
}
{
if(!isFinite(this.tag_yaw))return -22;
if(4>55-n)return -90;n+=4;
if(n>55)return -90;
}
{
if(!isFinite(this.base_roll))return -22;
if(4>55-n)return -90;n+=4;
if(n>55)return -90;
}
{
if(!isFinite(this.base_pitch))return -22;
if(4>55-n)return -90;n+=4;
if(n>55)return -90;
}
{
if(!isFinite(this.base_yaw))return -22;
if(4>55-n)return -90;n+=4;
if(n>55)return -90;
}
{
if(this.joystick.length!=2)return -22;
for(let i=0;i<this.joystick.length;++i){
if(!isFinite(this.joystick[i]))return -22;
if(4>55-n)return -90;n+=4;
if(n>55)return -90;
}
}
{
if(1>55-n)return -90;n+=1;
if(n>55)return -90;
}
{
if(1>55-n)return -90;n+=1;
if(n>55)return -90;
}
{
if(1>55-n)return -90;n+=1;
if(n>55)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
for(let i=0;i<this.version.length;++i){
w.integer(<u64>this.version[i],1);
}
w.integer(<u64>this.channel,1);
w.integer(<u64>this.joy_mode,1);
w.integer(<u64>reinterpret<u32>(this.orientation_est),4);
w.integer(<u64>reinterpret<u32>(this.pitch_est),4);
w.integer(<u64>reinterpret<u32>(this.distance_est),4);
w.integer(<u64>reinterpret<u32>(this.yaw_est),4);
w.integer(<u64>reinterpret<u32>(this.tag_roll),4);
w.integer(<u64>reinterpret<u32>(this.tag_pitch),4);
w.integer(<u64>reinterpret<u32>(this.tag_yaw),4);
w.integer(<u64>reinterpret<u32>(this.base_roll),4);
w.integer(<u64>reinterpret<u32>(this.base_pitch),4);
w.integer(<u64>reinterpret<u32>(this.base_yaw),4);
for(let i=0;i<this.joystick.length;++i){
w.integer(<u64>reinterpret<u32>(this.joystick[i]),4);
}
w.integer(<u64>this.error_state,1);
w.integer(<u64>this.buttons,1);
w.integer(<u64>this.enabled_from_app,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgUwbState{const value=new Ros2Unitree_goMsgUwbState();
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.version=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.version.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.channel=decoded;
}
{
const decoded=<u8>r.integer(1);
value.joy_mode=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.orientation_est=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.pitch_est=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.distance_est=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.yaw_est=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.tag_roll=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.tag_pitch=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.tag_yaw=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.base_roll=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.base_pitch=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.base_yaw=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.joystick=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joystick.push(decoded);
}
}
{
const decoded=<u8>r.integer(1);
value.error_state=decoded;
}
{
const decoded=<u8>r.integer(1);
value.buttons=decoded;
}
{
const decoded=<u8>r.integer(1);
value.enabled_from_app=decoded;
}
return value;}
}
export class Ros2Unitree_goMsgUwbStateCodec implements VendorCodec<Ros2Unitree_goMsgUwbState>{
static readonly codecIdentity:string="UWVMROS1_017fbafd61a006aeaf6cb80456ac89f429d30a562da2e0d394c6cfb406e61a34";
static readonly maxWireBytes:i32=55;
encodeChecked(value:Ros2Unitree_goMsgUwbState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgUwbState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgUwbState>{const out=new VendorDecoded<Ros2Unitree_goMsgUwbState>();if(bytes.length>55){out.status=90;return out;}if(bytes.length<55){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgUwbState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgUwbState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgUwbState():changetype<Ros2Unitree_goMsgUwbState>(out.value);}
}
/** Complete original unitree_go/msg/UwbSwitch; spellings are published in generation proof. */
export class Ros2Unitree_goMsgUwbSwitch {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field enabled, native scalar uint8. */
enabled:u8=0;
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
w.integer(<u64>this.enabled,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgUwbSwitch{const value=new Ros2Unitree_goMsgUwbSwitch();
{
const decoded=<u8>r.integer(1);
value.enabled=decoded;
}
return value;}
}
export class Ros2Unitree_goMsgUwbSwitchCodec implements VendorCodec<Ros2Unitree_goMsgUwbSwitch>{
static readonly codecIdentity:string="UWVMROS1_4546618797829b3911d1533d8d6bacc9da969d40d7752424de34bbd3de4bede5";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Unitree_goMsgUwbSwitch):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgUwbSwitch):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgUwbSwitch>{const out=new VendorDecoded<Ros2Unitree_goMsgUwbSwitch>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgUwbSwitch.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgUwbSwitch{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgUwbSwitch():changetype<Ros2Unitree_goMsgUwbSwitch>(out.value);}
}
/** Complete original unitree_go/msg/WirelessController; spellings are published in generation proof. */
export class Ros2Unitree_goMsgWirelessController {
static readonly maxWireBytes:i32=18;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=18;
/** Exact native field lx, native scalar float32. */
lx:f32=0;
/** Exact native field ly, native scalar float32. */
ly:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field keys, native scalar uint16. */
keys:u16=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.lx))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(!isFinite(this.ly))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(2>18-n)return -90;n+=2;
if(n>18)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.lx),4);
w.integer(<u64>reinterpret<u32>(this.ly),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>this.keys,2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Unitree_goMsgWirelessController{const value=new Ros2Unitree_goMsgWirelessController();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.lx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ly=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=<u16>r.integer(2);
value.keys=decoded;
}
return value;}
}
export class Ros2Unitree_goMsgWirelessControllerCodec implements VendorCodec<Ros2Unitree_goMsgWirelessController>{
static readonly codecIdentity:string="UWVMROS1_536e69ce5459227cb68b3c6d484a96bfa40e1400aab820f8ad0d0d62ff6ba438";
static readonly maxWireBytes:i32=18;
encodeChecked(value:Ros2Unitree_goMsgWirelessController):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Unitree_goMsgWirelessController):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Unitree_goMsgWirelessController>{const out=new VendorDecoded<Ros2Unitree_goMsgWirelessController>();if(bytes.length>18){out.status=90;return out;}if(bytes.length<18){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Unitree_goMsgWirelessController.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Unitree_goMsgWirelessController{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Unitree_goMsgWirelessController():changetype<Ros2Unitree_goMsgWirelessController>(out.value);}
}
