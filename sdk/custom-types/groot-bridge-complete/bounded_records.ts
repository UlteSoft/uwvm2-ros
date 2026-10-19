/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original uwvm_groot_bridge/msg/RGBFrame; spellings are published in generation proof. */
export class Ros2Uwvm_groot_bridgeMsgRGBFrame {
static readonly maxWireBytes:i32=860;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=860;
/** Exact native field key, native scalar string. */
key:string="";
/** Exact native field shape, native scalar uint32. */
shape:Array<u32>=new Array<u32>();
/** Exact native field pixels, native scalar uint8. */
pixels:Array<u8>=new Array<u8>();
constructor(){
for(let i=0;i<5;++i)this.shape.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.key)||this.key.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.key);if(size>64)return -22;if(4+size>860-n)return -90;n+=4+size;
if(n>860)return -90;
}
{
if(this.shape.length!=5)return -22;
for(let i=0;i<this.shape.length;++i){
if(4>860-n)return -90;n+=4;
if(n>860)return -90;
}
}
{
if(this.pixels.length>768)return -22;
if(4>860-n)return -90;n+=4;
for(let i=0;i<this.pixels.length;++i){
if(1>860-n)return -90;n+=1;
if(n>860)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.key);
for(let i=0;i<this.shape.length;++i){
w.integer(<u64>this.shape[i],4);
}
w.u32(<u32>this.pixels.length);
for(let i=0;i<this.pixels.length;++i){
w.integer(<u64>this.pixels[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_groot_bridgeMsgRGBFrame{const value=new Ros2Uwvm_groot_bridgeMsgRGBFrame();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.key=decoded;
}
{
const count:u32=5;if(!r.valid||count>5){r.valid=false;return value;}
value.shape=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.shape.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>768){r.valid=false;return value;}
value.pixels=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.pixels.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_groot_bridgeMsgRGBFrameCodec implements VendorCodec<Ros2Uwvm_groot_bridgeMsgRGBFrame>{
static readonly codecIdentity:string="UWVMROS1_db7172fe7a7f0672c907f31ca0b1cb6286b804246d62e0414ac4313ae1096364";
static readonly maxWireBytes:i32=860;
encodeChecked(value:Ros2Uwvm_groot_bridgeMsgRGBFrame):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_groot_bridgeMsgRGBFrame):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_groot_bridgeMsgRGBFrame>{const out=new VendorDecoded<Ros2Uwvm_groot_bridgeMsgRGBFrame>();if(bytes.length>860){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_groot_bridgeMsgRGBFrame.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_groot_bridgeMsgRGBFrame{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_groot_bridgeMsgRGBFrame():changetype<Ros2Uwvm_groot_bridgeMsgRGBFrame>(out.value);}
}
/** Complete original uwvm_groot_bridge/msg/FloatTensor; spellings are published in generation proof. */
export class Ros2Uwvm_groot_bridgeMsgFloatTensor {
static readonly maxWireBytes:i32=1364;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1364;
/** Exact native field key, native scalar string. */
key:string="";
/** Exact native field shape, native scalar uint32. */
shape:Array<u32>=new Array<u32>();
/** Exact native field values, native scalar float32. */
values:Array<f32>=new Array<f32>();
constructor(){
for(let i=0;i<3;++i)this.shape.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.key)||this.key.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.key);if(size>64)return -22;if(4+size>1364-n)return -90;n+=4+size;
if(n>1364)return -90;
}
{
if(this.shape.length!=3)return -22;
for(let i=0;i<this.shape.length;++i){
if(4>1364-n)return -90;n+=4;
if(n>1364)return -90;
}
}
{
if(this.values.length>320)return -22;
if(4>1364-n)return -90;n+=4;
for(let i=0;i<this.values.length;++i){
if(!isFinite(this.values[i]))return -22;
if(4>1364-n)return -90;n+=4;
if(n>1364)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.key);
for(let i=0;i<this.shape.length;++i){
w.integer(<u64>this.shape[i],4);
}
w.u32(<u32>this.values.length);
for(let i=0;i<this.values.length;++i){
w.integer(<u64>reinterpret<u32>(this.values[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_groot_bridgeMsgFloatTensor{const value=new Ros2Uwvm_groot_bridgeMsgFloatTensor();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.key=decoded;
}
{
const count:u32=3;if(!r.valid||count>3){r.valid=false;return value;}
value.shape=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.shape.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>320){r.valid=false;return value;}
value.values=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.values.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_groot_bridgeMsgFloatTensorCodec implements VendorCodec<Ros2Uwvm_groot_bridgeMsgFloatTensor>{
static readonly codecIdentity:string="UWVMROS1_a091d322ab737d2a9415f889fb78a18df7aec6ce74cfab0dcd5bb2a61ae3abae";
static readonly maxWireBytes:i32=1364;
encodeChecked(value:Ros2Uwvm_groot_bridgeMsgFloatTensor):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_groot_bridgeMsgFloatTensor):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_groot_bridgeMsgFloatTensor>{const out=new VendorDecoded<Ros2Uwvm_groot_bridgeMsgFloatTensor>();if(bytes.length>1364){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_groot_bridgeMsgFloatTensor.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_groot_bridgeMsgFloatTensor{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_groot_bridgeMsgFloatTensor():changetype<Ros2Uwvm_groot_bridgeMsgFloatTensor>(out.value);}
}
/** Complete original uwvm_groot_bridge/msg/TextSequence; spellings are published in generation proof. */
export class Ros2Uwvm_groot_bridgeMsgTextSequence {
static readonly maxWireBytes:i32=1120;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1120;
/** Exact native field key, native scalar string. */
key:string="";
/** Exact native field shape, native scalar uint32. */
shape:Array<u32>=new Array<u32>();
/** Exact native field texts, native scalar string. */
texts:Array<string>=new Array<string>();
constructor(){
for(let i=0;i<2;++i)this.shape.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.key)||this.key.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.key);if(size>64)return -22;if(4+size>1120-n)return -90;n+=4+size;
if(n>1120)return -90;
}
{
if(this.shape.length!=2)return -22;
for(let i=0;i<this.shape.length;++i){
if(4>1120-n)return -90;n+=4;
if(n>1120)return -90;
}
}
{
if(this.texts.length>4)return -22;
if(4>1120-n)return -90;n+=4;
for(let i=0;i<this.texts.length;++i){
if(!validUnicode(this.texts[i])||this.texts[i].indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.texts[i]);if(size>256)return -22;if(4+size>1120-n)return -90;n+=4+size;
if(n>1120)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.key);
for(let i=0;i<this.shape.length;++i){
w.integer(<u64>this.shape[i],4);
}
w.u32(<u32>this.texts.length);
for(let i=0;i<this.texts.length;++i){
w.text(this.texts[i]);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_groot_bridgeMsgTextSequence{const value=new Ros2Uwvm_groot_bridgeMsgTextSequence();
{
const size=r.u32();if(!r.valid||size>64){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.key=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.shape=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.shape.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.texts=new Array<string>();for(let i:u32=0;i<count;++i){
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.texts.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_groot_bridgeMsgTextSequenceCodec implements VendorCodec<Ros2Uwvm_groot_bridgeMsgTextSequence>{
static readonly codecIdentity:string="UWVMROS1_895be974f2560f0afff7614f5400ff7205e013cda269825a8443628621c8e80d";
static readonly maxWireBytes:i32=1120;
encodeChecked(value:Ros2Uwvm_groot_bridgeMsgTextSequence):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_groot_bridgeMsgTextSequence):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_groot_bridgeMsgTextSequence>{const out=new VendorDecoded<Ros2Uwvm_groot_bridgeMsgTextSequence>();if(bytes.length>1120){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_groot_bridgeMsgTextSequence.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_groot_bridgeMsgTextSequence{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_groot_bridgeMsgTextSequence():changetype<Ros2Uwvm_groot_bridgeMsgTextSequence>(out.value);}
}
/** Complete original uwvm_groot_bridge/srv/InferAction_Request; spellings are published in generation proof. */
export class Ros2Uwvm_groot_bridgeSrvInferAction_Request {
static readonly maxWireBytes:i32=10296;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=10296;
/** Exact native field application_nonce, native scalar uint64. */
application_nonce:u64=0;
/** Exact native field video, native scalar uwvm_groot_bridge/msg/RGBFrame. */
video:Array<Ros2Uwvm_groot_bridgeMsgRGBFrame>=new Array<Ros2Uwvm_groot_bridgeMsgRGBFrame>();
/** Exact native field state, native scalar uwvm_groot_bridge/msg/FloatTensor. */
state:Array<Ros2Uwvm_groot_bridgeMsgFloatTensor>=new Array<Ros2Uwvm_groot_bridgeMsgFloatTensor>();
/** Exact native field language, native scalar uwvm_groot_bridge/msg/TextSequence. */
language:Array<Ros2Uwvm_groot_bridgeMsgTextSequence>=new Array<Ros2Uwvm_groot_bridgeMsgTextSequence>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>10296-n)return -90;n+=8;
if(n>10296)return -90;
}
{
if(this.video.length>3)return -22;
if(4>10296-n)return -90;n+=4;
for(let i=0;i<this.video.length;++i){
const child=this.video[i].wireSize();if(child<0)return child;if(child>10296-n)return -90;n+=child;
if(n>10296)return -90;
}
}
{
if(this.state.length>4)return -22;
if(4>10296-n)return -90;n+=4;
for(let i=0;i<this.state.length;++i){
const child=this.state[i].wireSize();if(child<0)return child;if(child>10296-n)return -90;n+=child;
if(n>10296)return -90;
}
}
{
if(this.language.length>2)return -22;
if(4>10296-n)return -90;n+=4;
for(let i=0;i<this.language.length;++i){
const child=this.language[i].wireSize();if(child<0)return child;if(child>10296-n)return -90;n+=child;
if(n>10296)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.application_nonce,8);
w.u32(<u32>this.video.length);
for(let i=0;i<this.video.length;++i){
this.video[i].writeControl(w);
}
w.u32(<u32>this.state.length);
for(let i=0;i<this.state.length;++i){
this.state[i].writeControl(w);
}
w.u32(<u32>this.language.length);
for(let i=0;i<this.language.length;++i){
this.language[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_groot_bridgeSrvInferAction_Request{const value=new Ros2Uwvm_groot_bridgeSrvInferAction_Request();
{
const decoded=<u64>r.integer(8);
value.application_nonce=decoded;
}
{
const count=r.u32();if(!r.valid||count>3){r.valid=false;return value;}
value.video=new Array<Ros2Uwvm_groot_bridgeMsgRGBFrame>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Uwvm_groot_bridgeMsgRGBFrame.readControl(r);
value.video.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.state=new Array<Ros2Uwvm_groot_bridgeMsgFloatTensor>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Uwvm_groot_bridgeMsgFloatTensor.readControl(r);
value.state.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.language=new Array<Ros2Uwvm_groot_bridgeMsgTextSequence>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Uwvm_groot_bridgeMsgTextSequence.readControl(r);
value.language.push(decoded);
}
}
return value;}
}
export class Ros2Uwvm_groot_bridgeSrvInferAction_RequestCodec implements VendorCodec<Ros2Uwvm_groot_bridgeSrvInferAction_Request>{
static readonly codecIdentity:string="UWVMROS1_78f85929fb8646e9a92f17cafa3e8296fde42aaf49a56b271531ebad869a0fa6";
static readonly maxWireBytes:i32=10296;
encodeChecked(value:Ros2Uwvm_groot_bridgeSrvInferAction_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_groot_bridgeSrvInferAction_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_groot_bridgeSrvInferAction_Request>{const out=new VendorDecoded<Ros2Uwvm_groot_bridgeSrvInferAction_Request>();if(bytes.length>10296){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_groot_bridgeSrvInferAction_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_groot_bridgeSrvInferAction_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_groot_bridgeSrvInferAction_Request():changetype<Ros2Uwvm_groot_bridgeSrvInferAction_Request>(out.value);}
}
/** Complete original uwvm_groot_bridge/srv/InferAction_Response; spellings are published in generation proof. */
export class Ros2Uwvm_groot_bridgeSrvInferAction_Response {
static readonly maxWireBytes:i32=10929;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=10929;
/** Exact native field status, native scalar uint32. */
status:u32=0;
/** Exact native field application_nonce, native scalar uint64. */
application_nonce:u64=0;
/** Exact native field action, native scalar uwvm_groot_bridge/msg/FloatTensor. */
action:Array<Ros2Uwvm_groot_bridgeMsgFloatTensor>=new Array<Ros2Uwvm_groot_bridgeMsgFloatTensor>();
/** Exact native field info_is_empty, native scalar bool. */
info_is_empty:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>10929-n)return -90;n+=4;
if(n>10929)return -90;
}
{
if(8>10929-n)return -90;n+=8;
if(n>10929)return -90;
}
{
if(this.action.length>8)return -22;
if(4>10929-n)return -90;n+=4;
for(let i=0;i<this.action.length;++i){
const child=this.action[i].wireSize();if(child<0)return child;if(child>10929-n)return -90;n+=child;
if(n>10929)return -90;
}
}
{
if(1>10929-n)return -90;n+=1;
if(n>10929)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.status,4);
w.integer(<u64>this.application_nonce,8);
w.u32(<u32>this.action.length);
for(let i=0;i<this.action.length;++i){
this.action[i].writeControl(w);
}
w.u8(this.info_is_empty?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Uwvm_groot_bridgeSrvInferAction_Response{const value=new Ros2Uwvm_groot_bridgeSrvInferAction_Response();
{
const decoded=<u32>r.integer(4);
value.status=decoded;
}
{
const decoded=<u64>r.integer(8);
value.application_nonce=decoded;
}
{
const count=r.u32();if(!r.valid||count>8){r.valid=false;return value;}
value.action=new Array<Ros2Uwvm_groot_bridgeMsgFloatTensor>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Uwvm_groot_bridgeMsgFloatTensor.readControl(r);
value.action.push(decoded);
}
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.info_is_empty=decoded;
}
return value;}
}
export class Ros2Uwvm_groot_bridgeSrvInferAction_ResponseCodec implements VendorCodec<Ros2Uwvm_groot_bridgeSrvInferAction_Response>{
static readonly codecIdentity:string="UWVMROS1_787281a2adea187ae43ff951a414dd7563b3e08318414e5199e1950b8143a3ce";
static readonly maxWireBytes:i32=10929;
encodeChecked(value:Ros2Uwvm_groot_bridgeSrvInferAction_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Uwvm_groot_bridgeSrvInferAction_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Uwvm_groot_bridgeSrvInferAction_Response>{const out=new VendorDecoded<Ros2Uwvm_groot_bridgeSrvInferAction_Response>();if(bytes.length>10929){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Uwvm_groot_bridgeSrvInferAction_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Uwvm_groot_bridgeSrvInferAction_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Uwvm_groot_bridgeSrvInferAction_Response():changetype<Ros2Uwvm_groot_bridgeSrvInferAction_Response>(out.value);}
}
