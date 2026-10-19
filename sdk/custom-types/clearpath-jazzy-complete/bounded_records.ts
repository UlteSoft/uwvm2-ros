/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original clearpath_motor_msgs/action/LynxCalibrate_Goal; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal {
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
static readControl(r:Reader):Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal{const value=new Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal();
return value;}
}
export class Ros2Clearpath_motor_msgsActionLynxCalibrate_GoalCodec implements VendorCodec<Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal>{
static readonly codecIdentity:string="UWVMROS1_61b01a6234b41f96464de9449b3e8d1b955decf6afa12aa3ba94062c98cec681";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal():changetype<Ros2Clearpath_motor_msgsActionLynxCalibrate_Goal>(out.value);}
}
/** Complete original clearpath_motor_msgs/action/LynxCalibrate_Result; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsActionLynxCalibrate_Result {
static readonly maxWireBytes:i32=12;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=12;
/** Exact native field offset, native scalar float32. */
offset:Array<f32>=new Array<f32>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.offset.length>2)return -22;
if(4>12-n)return -90;n+=4;
for(let i=0;i<this.offset.length;++i){
if(!isFinite(this.offset[i]))return -22;
if(4>12-n)return -90;n+=4;
if(n>12)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.offset.length);
for(let i=0;i<this.offset.length;++i){
w.integer(<u64>reinterpret<u32>(this.offset[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsActionLynxCalibrate_Result{const value=new Ros2Clearpath_motor_msgsActionLynxCalibrate_Result();
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.offset=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.offset.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_motor_msgsActionLynxCalibrate_ResultCodec implements VendorCodec<Ros2Clearpath_motor_msgsActionLynxCalibrate_Result>{
static readonly codecIdentity:string="UWVMROS1_686450363b8b15eba1e9e9e9ff05c9aacfb95bb0d113eaa7c443e4c70d55d41f";
static readonly maxWireBytes:i32=12;
encodeChecked(value:Ros2Clearpath_motor_msgsActionLynxCalibrate_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsActionLynxCalibrate_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsActionLynxCalibrate_Result>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsActionLynxCalibrate_Result>();if(bytes.length>12){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsActionLynxCalibrate_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsActionLynxCalibrate_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsActionLynxCalibrate_Result():changetype<Ros2Clearpath_motor_msgsActionLynxCalibrate_Result>(out.value);}
}
/** Complete original clearpath_motor_msgs/action/LynxCalibrate_Feedback; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback {
static readonly maxWireBytes:i32=8;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=8;
/** Exact native field iteration, native scalar uint16. */
iteration:Array<u16>=new Array<u16>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.iteration.length>2)return -22;
if(4>8-n)return -90;n+=4;
for(let i=0;i<this.iteration.length;++i){
if(2>8-n)return -90;n+=2;
if(n>8)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.iteration.length);
for(let i=0;i<this.iteration.length;++i){
w.integer(<u64>this.iteration[i],2);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback{const value=new Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback();
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.iteration=new Array<u16>();for(let i:u32=0;i<count;++i){
const decoded=<u16>r.integer(2);
value.iteration.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_motor_msgsActionLynxCalibrate_FeedbackCodec implements VendorCodec<Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback>{
static readonly codecIdentity:string="UWVMROS1_ebe7badd3f72982079911d8aa72c4ddd27ff0a12d2f948e5970649317a592b53";
static readonly maxWireBytes:i32=8;
encodeChecked(value:Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback>();if(bytes.length>8){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback():changetype<Ros2Clearpath_motor_msgsActionLynxCalibrate_Feedback>(out.value);}
}
/** Complete original clearpath_motor_msgs/action/LynxUpdate_Goal; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsActionLynxUpdate_Goal {
static readonly maxWireBytes:i32=260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=260;
/** Exact native field file, native scalar string. */
file:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.file)||this.file.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.file);if(size>256)return -22;if(4+size>260-n)return -90;n+=4+size;
if(n>260)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.file);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsActionLynxUpdate_Goal{const value=new Ros2Clearpath_motor_msgsActionLynxUpdate_Goal();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.file=decoded;
}
return value;}
}
export class Ros2Clearpath_motor_msgsActionLynxUpdate_GoalCodec implements VendorCodec<Ros2Clearpath_motor_msgsActionLynxUpdate_Goal>{
static readonly codecIdentity:string="UWVMROS1_f0362f8a90391e8ffda2e8f3b798769b644091058361882c4e9457196f007565";
static readonly maxWireBytes:i32=260;
encodeChecked(value:Ros2Clearpath_motor_msgsActionLynxUpdate_Goal):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsActionLynxUpdate_Goal):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsActionLynxUpdate_Goal>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsActionLynxUpdate_Goal>();if(bytes.length>260){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsActionLynxUpdate_Goal.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsActionLynxUpdate_Goal{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsActionLynxUpdate_Goal():changetype<Ros2Clearpath_motor_msgsActionLynxUpdate_Goal>(out.value);}
}
/** Complete original clearpath_motor_msgs/action/LynxUpdate_Result; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsActionLynxUpdate_Result {
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field success, native scalar bool. */
success:Array<bool>=new Array<bool>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.success.length>2)return -22;
if(4>6-n)return -90;n+=4;
for(let i=0;i<this.success.length;++i){
if(1>6-n)return -90;n+=1;
if(n>6)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.success.length);
for(let i=0;i<this.success.length;++i){
w.u8(this.success[i]?1:0);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsActionLynxUpdate_Result{const value=new Ros2Clearpath_motor_msgsActionLynxUpdate_Result();
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.success=new Array<bool>();for(let i:u32=0;i<count;++i){
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.success.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_motor_msgsActionLynxUpdate_ResultCodec implements VendorCodec<Ros2Clearpath_motor_msgsActionLynxUpdate_Result>{
static readonly codecIdentity:string="UWVMROS1_126fe871dbcb036e485d20e8ccdc8e9fb7be01757f539bfba895aabf44d341d8";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2Clearpath_motor_msgsActionLynxUpdate_Result):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsActionLynxUpdate_Result):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsActionLynxUpdate_Result>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsActionLynxUpdate_Result>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsActionLynxUpdate_Result.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsActionLynxUpdate_Result{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsActionLynxUpdate_Result():changetype<Ros2Clearpath_motor_msgsActionLynxUpdate_Result>(out.value);}
}
/** Complete original clearpath_motor_msgs/action/LynxUpdate_Feedback; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback {
static readonly maxWireBytes:i32=12;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=12;
/** Exact native field progress, native scalar float32. */
progress:Array<f32>=new Array<f32>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.progress.length>2)return -22;
if(4>12-n)return -90;n+=4;
for(let i=0;i<this.progress.length;++i){
if(!isFinite(this.progress[i]))return -22;
if(4>12-n)return -90;n+=4;
if(n>12)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.progress.length);
for(let i=0;i<this.progress.length;++i){
w.integer(<u64>reinterpret<u32>(this.progress[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback{const value=new Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback();
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.progress=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.progress.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_motor_msgsActionLynxUpdate_FeedbackCodec implements VendorCodec<Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback>{
static readonly codecIdentity:string="UWVMROS1_2179ec9dee627234a1a31e9681ff214cc45ca837349cd8e407e019a354121b19";
static readonly maxWireBytes:i32=12;
encodeChecked(value:Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback>();if(bytes.length>12){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback():changetype<Ros2Clearpath_motor_msgsActionLynxUpdate_Feedback>(out.value);}
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
/** Complete original clearpath_control_msgs/msg/DeviceInfo; spellings are published in generation proof. */
export class Ros2Clearpath_control_msgsMsgDeviceInfo {
static readonly maxWireBytes:i32=1820;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1820;
/** Exact native field device_id, native scalar string. */
device_id:string="";
/** Exact native field device_class, native scalar string. */
device_class:string="";
/** Exact native field model, native scalar string. */
model:string="";
/** Exact native field manufacturer, native scalar string. */
manufacturer:string="";
/** Exact native field firmware_version, native scalar string. */
firmware_version:string="";
/** Exact native field hardware_version, native scalar string. */
hardware_version:string="";
/** Exact native field serial_number, native scalar string. */
serial_number:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.device_id)||this.device_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.device_id);if(size>256)return -22;if(4+size>1820-n)return -90;n+=4+size;
if(n>1820)return -90;
}
{
if(!validUnicode(this.device_class)||this.device_class.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.device_class);if(size>256)return -22;if(4+size>1820-n)return -90;n+=4+size;
if(n>1820)return -90;
}
{
if(!validUnicode(this.model)||this.model.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.model);if(size>256)return -22;if(4+size>1820-n)return -90;n+=4+size;
if(n>1820)return -90;
}
{
if(!validUnicode(this.manufacturer)||this.manufacturer.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.manufacturer);if(size>256)return -22;if(4+size>1820-n)return -90;n+=4+size;
if(n>1820)return -90;
}
{
if(!validUnicode(this.firmware_version)||this.firmware_version.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.firmware_version);if(size>256)return -22;if(4+size>1820-n)return -90;n+=4+size;
if(n>1820)return -90;
}
{
if(!validUnicode(this.hardware_version)||this.hardware_version.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.hardware_version);if(size>256)return -22;if(4+size>1820-n)return -90;n+=4+size;
if(n>1820)return -90;
}
{
if(!validUnicode(this.serial_number)||this.serial_number.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.serial_number);if(size>256)return -22;if(4+size>1820-n)return -90;n+=4+size;
if(n>1820)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.device_id);
w.text(this.device_class);
w.text(this.model);
w.text(this.manufacturer);
w.text(this.firmware_version);
w.text(this.hardware_version);
w.text(this.serial_number);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_control_msgsMsgDeviceInfo{const value=new Ros2Clearpath_control_msgsMsgDeviceInfo();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.device_id=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.device_class=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.model=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.manufacturer=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.firmware_version=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.hardware_version=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.serial_number=decoded;
}
return value;}
}
export class Ros2Clearpath_control_msgsMsgDeviceInfoCodec implements VendorCodec<Ros2Clearpath_control_msgsMsgDeviceInfo>{
static readonly codecIdentity:string="UWVMROS1_9111f5d4b5324c36eb85f01e6fdbc3c589479ff5b64a8fa7b9d467aa14675c3e";
static readonly maxWireBytes:i32=1820;
encodeChecked(value:Ros2Clearpath_control_msgsMsgDeviceInfo):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_control_msgsMsgDeviceInfo):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_control_msgsMsgDeviceInfo>{const out=new VendorDecoded<Ros2Clearpath_control_msgsMsgDeviceInfo>();if(bytes.length>1820){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_control_msgsMsgDeviceInfo.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_control_msgsMsgDeviceInfo{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_control_msgsMsgDeviceInfo():changetype<Ros2Clearpath_control_msgsMsgDeviceInfo>(out.value);}
}
/** Complete original clearpath_control_msgs/msg/StreamControl; spellings are published in generation proof. */
export class Ros2Clearpath_control_msgsMsgStreamControl {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field stream, native scalar string. */
stream:string="";
/** Exact native field enabled, native scalar bool. */
enabled:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.stream)||this.stream.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.stream);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.stream);
w.u8(this.enabled?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_control_msgsMsgStreamControl{const value=new Ros2Clearpath_control_msgsMsgStreamControl();
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.stream=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.enabled=decoded;
}
return value;}
}
export class Ros2Clearpath_control_msgsMsgStreamControlCodec implements VendorCodec<Ros2Clearpath_control_msgsMsgStreamControl>{
static readonly codecIdentity:string="UWVMROS1_26208e266c9185bfb0e715407ab0fe313f2d47e6bc9f54594a102c3a09d34734";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Clearpath_control_msgsMsgStreamControl):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_control_msgsMsgStreamControl):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_control_msgsMsgStreamControl>{const out=new VendorDecoded<Ros2Clearpath_control_msgsMsgStreamControl>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_control_msgsMsgStreamControl.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_control_msgsMsgStreamControl{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_control_msgsMsgStreamControl():changetype<Ros2Clearpath_control_msgsMsgStreamControl>(out.value);}
}
/** Complete original clearpath_motor_msgs/msg/LynxDebug; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgLynxDebug {
static readonly ACCUMULATOR_FLAG_ABOVE_RATED:u8=0;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_1_1:u8=1;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_1_2:u8=2;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_1_3:u8=3;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_1_4:u8=4;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_1_5:u8=5;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_1_6:u8=6;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_1_7:u8=7;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_1_8:u8=8;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_1_9:u8=9;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_2_0:u8=10;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_2_1:u8=11;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_2_2:u8=12;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_2_3:u8=13;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_2_4:u8=14;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_2_5:u8=15;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_2_6:u8=16;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_2_7:u8=17;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_2_8:u8=18;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_2_9:u8=19;
static readonly ACCUMULATOR_FLAG_CONTINUOUS_3_0:u8=20;
static readonly ACCUMULATOR_ABOVE_RATED:u8=0;
static readonly ACCUMULATOR_CONTINUOUS_1_1:u8=1;
static readonly ACCUMULATOR_CONTINUOUS_1_5:u8=2;
static readonly ACCUMULATOR_CONTINUOUS_2_0:u8=3;
static readonly ACCUMULATOR_CONTINUOUS_2_5:u8=4;
static readonly ACCUMULATOR_CONTINUOUS_3_0:u8=5;
static readonly CURRENT_ID_FB:u8=0;
static readonly CURRENT_ID_FF:u8=1;
static readonly CURRENT_ID_REF:u8=2;
static readonly CURRENT_IQ_FB:u8=3;
static readonly CURRENT_IQ_FF:u8=4;
static readonly CURRENT_IQ_REF:u8=5;
static readonly maxWireBytes:i32=291;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=291;
/** Exact native field can_id, native scalar uint8. */
can_id:u8=0;
/** Exact native field joint_name, native scalar string. */
joint_name:string="";
/** Exact native field fault_frequency, native scalar uint16. */
fault_frequency:u16=0;
/** Exact native field accumulator_flags, native scalar uint32. */
accumulator_flags:u32=0;
/** Exact native field accumulators, native scalar float32. */
accumulators:Array<f32>=new Array<f32>();
/** Exact native field currents, native scalar float32. */
currents:Array<f32>=new Array<f32>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>291-n)return -90;n+=1;
if(n>291)return -90;
}
{
if(!validUnicode(this.joint_name)||this.joint_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_name);if(size>256)return -22;if(4+size>291-n)return -90;n+=4+size;
if(n>291)return -90;
}
{
if(2>291-n)return -90;n+=2;
if(n>291)return -90;
}
{
if(4>291-n)return -90;n+=4;
if(n>291)return -90;
}
{
if(this.accumulators.length>2)return -22;
if(4>291-n)return -90;n+=4;
for(let i=0;i<this.accumulators.length;++i){
if(!isFinite(this.accumulators[i]))return -22;
if(4>291-n)return -90;n+=4;
if(n>291)return -90;
}
}
{
if(this.currents.length>2)return -22;
if(4>291-n)return -90;n+=4;
for(let i=0;i<this.currents.length;++i){
if(!isFinite(this.currents[i]))return -22;
if(4>291-n)return -90;n+=4;
if(n>291)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.can_id,1);
w.text(this.joint_name);
w.integer(<u64>this.fault_frequency,2);
w.integer(<u64>this.accumulator_flags,4);
w.u32(<u32>this.accumulators.length);
for(let i=0;i<this.accumulators.length;++i){
w.integer(<u64>reinterpret<u32>(this.accumulators[i]),4);
}
w.u32(<u32>this.currents.length);
for(let i=0;i<this.currents.length;++i){
w.integer(<u64>reinterpret<u32>(this.currents[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgLynxDebug{const value=new Ros2Clearpath_motor_msgsMsgLynxDebug();
{
const decoded=<u8>r.integer(1);
value.can_id=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_name=decoded;
}
{
const decoded=<u16>r.integer(2);
value.fault_frequency=decoded;
}
{
const decoded=<u32>r.integer(4);
value.accumulator_flags=decoded;
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.accumulators=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.accumulators.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.currents=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.currents.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgLynxDebugCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgLynxDebug>{
static readonly codecIdentity:string="UWVMROS1_0341ae040f1e8a1f980bde84498c4a4f8b9c55c3806839a73727c1579ab4416e";
static readonly maxWireBytes:i32=291;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgLynxDebug):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgLynxDebug):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxDebug>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxDebug>();if(bytes.length>291){out.status=90;return out;}if(bytes.length<19){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgLynxDebug.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgLynxDebug{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgLynxDebug():changetype<Ros2Clearpath_motor_msgsMsgLynxDebug>(out.value);}
}
/** Complete original clearpath_motor_msgs/msg/LynxFeedback; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgLynxFeedback {
static readonly maxWireBytes:i32=277;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=277;
/** Exact native field can_id, native scalar uint8. */
can_id:u8=0;
/** Exact native field joint_name, native scalar string. */
joint_name:string="";
/** Exact native field current, native scalar float32. */
current:f32=0;
/** Exact native field voltage, native scalar float32. */
voltage:f32=0;
/** Exact native field velocity, native scalar float32. */
velocity:f32=0;
/** Exact native field travel, native scalar float32. */
travel:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>277-n)return -90;n+=1;
if(n>277)return -90;
}
{
if(!validUnicode(this.joint_name)||this.joint_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_name);if(size>256)return -22;if(4+size>277-n)return -90;n+=4+size;
if(n>277)return -90;
}
{
if(!isFinite(this.current))return -22;
if(4>277-n)return -90;n+=4;
if(n>277)return -90;
}
{
if(!isFinite(this.voltage))return -22;
if(4>277-n)return -90;n+=4;
if(n>277)return -90;
}
{
if(!isFinite(this.velocity))return -22;
if(4>277-n)return -90;n+=4;
if(n>277)return -90;
}
{
if(!isFinite(this.travel))return -22;
if(4>277-n)return -90;n+=4;
if(n>277)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.can_id,1);
w.text(this.joint_name);
w.integer(<u64>reinterpret<u32>(this.current),4);
w.integer(<u64>reinterpret<u32>(this.voltage),4);
w.integer(<u64>reinterpret<u32>(this.velocity),4);
w.integer(<u64>reinterpret<u32>(this.travel),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgLynxFeedback{const value=new Ros2Clearpath_motor_msgsMsgLynxFeedback();
{
const decoded=<u8>r.integer(1);
value.can_id=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_name=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.current=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.voltage=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.velocity=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.travel=decoded;
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgLynxFeedbackCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgLynxFeedback>{
static readonly codecIdentity:string="UWVMROS1_5d92c7085019ebf00ba93788b5b2f4055ad40b2c6714575a66fb75cf314429eb";
static readonly maxWireBytes:i32=277;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgLynxFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgLynxFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxFeedback>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxFeedback>();if(bytes.length>277){out.status=90;return out;}if(bytes.length<21){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgLynxFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgLynxFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgLynxFeedback():changetype<Ros2Clearpath_motor_msgsMsgLynxFeedback>(out.value);}
}
/** Complete original clearpath_motor_msgs/msg/LynxMotorProtection; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgLynxMotorProtection {
static readonly NORMAL:u8=0;
static readonly THROTTLED:u8=1;
static readonly OVERHEATED:u8=2;
static readonly ERROR:u8=3;
static readonly maxWireBytes:i32=262;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=262;
/** Exact native field can_id, native scalar uint8. */
can_id:u8=0;
/** Exact native field joint_name, native scalar string. */
joint_name:string="";
/** Exact native field state, native scalar uint8. */
state:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>262-n)return -90;n+=1;
if(n>262)return -90;
}
{
if(!validUnicode(this.joint_name)||this.joint_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_name);if(size>256)return -22;if(4+size>262-n)return -90;n+=4+size;
if(n>262)return -90;
}
{
if(1>262-n)return -90;n+=1;
if(n>262)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.can_id,1);
w.text(this.joint_name);
w.integer(<u64>this.state,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgLynxMotorProtection{const value=new Ros2Clearpath_motor_msgsMsgLynxMotorProtection();
{
const decoded=<u8>r.integer(1);
value.can_id=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_name=decoded;
}
{
const decoded=<u8>r.integer(1);
value.state=decoded;
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgLynxMotorProtectionCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgLynxMotorProtection>{
static readonly codecIdentity:string="UWVMROS1_f824abfa97f6f24934e04094d66170167f19a14fb933f325cbef4ed6fca16630";
static readonly maxWireBytes:i32=262;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgLynxMotorProtection):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgLynxMotorProtection):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxMotorProtection>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxMotorProtection>();if(bytes.length>262){out.status=90;return out;}if(bytes.length<6){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgLynxMotorProtection.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgLynxMotorProtection{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgLynxMotorProtection():changetype<Ros2Clearpath_motor_msgsMsgLynxMotorProtection>(out.value);}
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
/** Complete original clearpath_motor_msgs/msg/LynxMultiDebug; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgLynxMultiDebug {
static readonly maxWireBytes:i32=1308;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1308;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field drivers, native scalar clearpath_motor_msgs/msg/LynxDebug. */
drivers:Array<Ros2Clearpath_motor_msgsMsgLynxDebug>=new Array<Ros2Clearpath_motor_msgsMsgLynxDebug>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>1308-n)return -90;n+=child;
if(n>1308)return -90;
}
{
if(this.drivers.length>4)return -22;
if(4>1308-n)return -90;n+=4;
for(let i=0;i<this.drivers.length;++i){
const child=this.drivers[i].wireSize();if(child<0)return child;if(child>1308-n)return -90;n+=child;
if(n>1308)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.drivers.length);
for(let i=0;i<this.drivers.length;++i){
this.drivers[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgLynxMultiDebug{const value=new Ros2Clearpath_motor_msgsMsgLynxMultiDebug();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.drivers=new Array<Ros2Clearpath_motor_msgsMsgLynxDebug>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgLynxDebug.readControl(r);
value.drivers.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgLynxMultiDebugCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgLynxMultiDebug>{
static readonly codecIdentity:string="UWVMROS1_056abd2153e7f1a6df223bf5daba581e6c218bb7ef365444fc5c3171dfc62e95";
static readonly maxWireBytes:i32=1308;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgLynxMultiDebug):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgLynxMultiDebug):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxMultiDebug>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxMultiDebug>();if(bytes.length>1308){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgLynxMultiDebug.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgLynxMultiDebug{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgLynxMultiDebug():changetype<Ros2Clearpath_motor_msgsMsgLynxMultiDebug>(out.value);}
}
/** Complete original clearpath_motor_msgs/msg/LynxMultiFeedback; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgLynxMultiFeedback {
static readonly maxWireBytes:i32=1252;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1252;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field drivers, native scalar clearpath_motor_msgs/msg/LynxFeedback. */
drivers:Array<Ros2Clearpath_motor_msgsMsgLynxFeedback>=new Array<Ros2Clearpath_motor_msgsMsgLynxFeedback>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>1252-n)return -90;n+=child;
if(n>1252)return -90;
}
{
if(this.drivers.length>4)return -22;
if(4>1252-n)return -90;n+=4;
for(let i=0;i<this.drivers.length;++i){
const child=this.drivers[i].wireSize();if(child<0)return child;if(child>1252-n)return -90;n+=child;
if(n>1252)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.drivers.length);
for(let i=0;i<this.drivers.length;++i){
this.drivers[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgLynxMultiFeedback{const value=new Ros2Clearpath_motor_msgsMsgLynxMultiFeedback();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.drivers=new Array<Ros2Clearpath_motor_msgsMsgLynxFeedback>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgLynxFeedback.readControl(r);
value.drivers.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgLynxMultiFeedbackCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgLynxMultiFeedback>{
static readonly codecIdentity:string="UWVMROS1_2c83cade6bec4d766fd2cff97338642b3d2065d41e9968ccf5054c6f231242c7";
static readonly maxWireBytes:i32=1252;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgLynxMultiFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgLynxMultiFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxMultiFeedback>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxMultiFeedback>();if(bytes.length>1252){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgLynxMultiFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgLynxMultiFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgLynxMultiFeedback():changetype<Ros2Clearpath_motor_msgsMsgLynxMultiFeedback>(out.value);}
}
/** Complete original clearpath_motor_msgs/msg/LynxStatus; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgLynxStatus {
static readonly STATUS_FLAG_ADC_CALIBRATED:u8=0;
static readonly STATUS_FLAG_FOC_ENABLED:u8=1;
static readonly STATUS_FLAG_CALIBRATION_REQUESTED:u8=2;
static readonly STATUS_FLAG_CALIBRATION_CANCELLED:u8=3;
static readonly STATUS_FLAG_ESTOPPED:u8=4;
static readonly WARNING_FLAG_MOTOR_THERMISTOR:u8=0;
static readonly WARNING_FLAG_PCB_THERMISTOR:u8=1;
static readonly WARNING_FLAG_PHASE:u8=2;
static readonly WARNING_FLAG_PHASE_A:u8=3;
static readonly WARNING_FLAG_PHASE_B:u8=4;
static readonly WARNING_FLAG_PHASE_C:u8=5;
static readonly WARNING_FLAG_ENCODER_INDEX:u8=6;
static readonly WARNING_FLAG_ENCODER_OUTPUT_A:u8=7;
static readonly WARNING_FLAG_ENCODER_OUTPUT_B:u8=8;
static readonly ERROR_FLAG_NOT_CALIBRATED:u8=0;
static readonly ERROR_FLAG_MOTOR_FAULT:u8=1;
static readonly ERROR_FLAG_MOTOR_STALLING:u8=2;
static readonly ERROR_FLAG_MOTOR_THERMISTOR:u8=3;
static readonly ERROR_FLAG_PCB_THERMISTOR:u8=4;
static readonly ERROR_FLAG_PHASE:u8=5;
static readonly ERROR_FLAG_PHASE_A:u8=6;
static readonly ERROR_FLAG_PHASE_B:u8=7;
static readonly ERROR_FLAG_PHASE_C:u8=8;
static readonly ERROR_FLAG_ENCODER_POWER:u8=9;
static readonly ERROR_FLAG_ENCODER_INDEX:u8=10;
static readonly ERROR_FLAG_ENCODER_OUTPUT_A:u8=11;
static readonly ERROR_FLAG_ENCODER_OUTPUT_B:u8=12;
static readonly maxWireBytes:i32=546;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=546;
/** Exact native field can_id, native scalar uint8. */
can_id:u8=0;
/** Exact native field joint_name, native scalar string. */
joint_name:string="";
/** Exact native field firmware_version, native scalar string. */
firmware_version:string="";
/** Exact native field motor_temperature, native scalar float32. */
motor_temperature:f32=0;
/** Exact native field mcu_temperature, native scalar float32. */
mcu_temperature:f32=0;
/** Exact native field pcb_temperature, native scalar float32. */
pcb_temperature:f32=0;
/** Exact native field stopped, native scalar bool. */
stopped:bool=false;
/** Exact native field status_flags, native scalar uint32. */
status_flags:u32=0;
/** Exact native field warning_flags, native scalar uint32. */
warning_flags:u32=0;
/** Exact native field error_flags, native scalar uint32. */
error_flags:u32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>546-n)return -90;n+=1;
if(n>546)return -90;
}
{
if(!validUnicode(this.joint_name)||this.joint_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.joint_name);if(size>256)return -22;if(4+size>546-n)return -90;n+=4+size;
if(n>546)return -90;
}
{
if(!validUnicode(this.firmware_version)||this.firmware_version.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.firmware_version);if(size>256)return -22;if(4+size>546-n)return -90;n+=4+size;
if(n>546)return -90;
}
{
if(!isFinite(this.motor_temperature))return -22;
if(4>546-n)return -90;n+=4;
if(n>546)return -90;
}
{
if(!isFinite(this.mcu_temperature))return -22;
if(4>546-n)return -90;n+=4;
if(n>546)return -90;
}
{
if(!isFinite(this.pcb_temperature))return -22;
if(4>546-n)return -90;n+=4;
if(n>546)return -90;
}
{
if(1>546-n)return -90;n+=1;
if(n>546)return -90;
}
{
if(4>546-n)return -90;n+=4;
if(n>546)return -90;
}
{
if(4>546-n)return -90;n+=4;
if(n>546)return -90;
}
{
if(4>546-n)return -90;n+=4;
if(n>546)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.can_id,1);
w.text(this.joint_name);
w.text(this.firmware_version);
w.integer(<u64>reinterpret<u32>(this.motor_temperature),4);
w.integer(<u64>reinterpret<u32>(this.mcu_temperature),4);
w.integer(<u64>reinterpret<u32>(this.pcb_temperature),4);
w.u8(this.stopped?1:0);
w.integer(<u64>this.status_flags,4);
w.integer(<u64>this.warning_flags,4);
w.integer(<u64>this.error_flags,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgLynxStatus{const value=new Ros2Clearpath_motor_msgsMsgLynxStatus();
{
const decoded=<u8>r.integer(1);
value.can_id=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.joint_name=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.firmware_version=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.motor_temperature=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.mcu_temperature=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.pcb_temperature=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.stopped=decoded;
}
{
const decoded=<u32>r.integer(4);
value.status_flags=decoded;
}
{
const decoded=<u32>r.integer(4);
value.warning_flags=decoded;
}
{
const decoded=<u32>r.integer(4);
value.error_flags=decoded;
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgLynxStatusCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgLynxStatus>{
static readonly codecIdentity:string="UWVMROS1_b68a7ab9bc5312c59d2ef86e49313468b631e8cacdf52468374b67f4eced5c6d";
static readonly maxWireBytes:i32=546;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgLynxStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgLynxStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxStatus>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxStatus>();if(bytes.length>546){out.status=90;return out;}if(bytes.length<34){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgLynxStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgLynxStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgLynxStatus():changetype<Ros2Clearpath_motor_msgsMsgLynxStatus>(out.value);}
}
/** Complete original clearpath_motor_msgs/msg/LynxMultiStatus; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgLynxMultiStatus {
static readonly maxWireBytes:i32=2328;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2328;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field drivers, native scalar clearpath_motor_msgs/msg/LynxStatus. */
drivers:Array<Ros2Clearpath_motor_msgsMsgLynxStatus>=new Array<Ros2Clearpath_motor_msgsMsgLynxStatus>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>2328-n)return -90;n+=child;
if(n>2328)return -90;
}
{
if(this.drivers.length>4)return -22;
if(4>2328-n)return -90;n+=4;
for(let i=0;i<this.drivers.length;++i){
const child=this.drivers[i].wireSize();if(child<0)return child;if(child>2328-n)return -90;n+=child;
if(n>2328)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.drivers.length);
for(let i=0;i<this.drivers.length;++i){
this.drivers[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgLynxMultiStatus{const value=new Ros2Clearpath_motor_msgsMsgLynxMultiStatus();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.drivers=new Array<Ros2Clearpath_motor_msgsMsgLynxStatus>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgLynxStatus.readControl(r);
value.drivers.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgLynxMultiStatusCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgLynxMultiStatus>{
static readonly codecIdentity:string="UWVMROS1_595cd0847b4332e14cc067b916166e9a6e1d1e12a7a68cdb7e1cd414a9ea9ce1";
static readonly maxWireBytes:i32=2328;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgLynxMultiStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgLynxMultiStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxMultiStatus>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxMultiStatus>();if(bytes.length>2328){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgLynxMultiStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgLynxMultiStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgLynxMultiStatus():changetype<Ros2Clearpath_motor_msgsMsgLynxMultiStatus>(out.value);}
}
/** Complete original clearpath_motor_msgs/msg/LynxSystemProtection; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgLynxSystemProtection {
static readonly maxWireBytes:i32=669;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=669;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field system_state, native scalar uint8. */
system_state:u8=0;
/** Exact native field motor_states, native scalar clearpath_motor_msgs/msg/LynxMotorProtection. */
motor_states:Array<Ros2Clearpath_motor_msgsMsgLynxMotorProtection>=new Array<Ros2Clearpath_motor_msgsMsgLynxMotorProtection>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>669-n)return -90;n+=child;
if(n>669)return -90;
}
{
if(1>669-n)return -90;n+=1;
if(n>669)return -90;
}
{
if(this.motor_states.length>2)return -22;
if(4>669-n)return -90;n+=4;
for(let i=0;i<this.motor_states.length;++i){
const child=this.motor_states[i].wireSize();if(child<0)return child;if(child>669-n)return -90;n+=child;
if(n>669)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>this.system_state,1);
w.u32(<u32>this.motor_states.length);
for(let i=0;i<this.motor_states.length;++i){
this.motor_states[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgLynxSystemProtection{const value=new Ros2Clearpath_motor_msgsMsgLynxSystemProtection();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=<u8>r.integer(1);
value.system_state=decoded;
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.motor_states=new Array<Ros2Clearpath_motor_msgsMsgLynxMotorProtection>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgLynxMotorProtection.readControl(r);
value.motor_states.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgLynxSystemProtectionCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgLynxSystemProtection>{
static readonly codecIdentity:string="UWVMROS1_4d662de03c34bb633a122399be1a1aaea6187550c83231bf6d8a06b95fa82d9f";
static readonly maxWireBytes:i32=669;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgLynxSystemProtection):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgLynxSystemProtection):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxSystemProtection>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgLynxSystemProtection>();if(bytes.length>669){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgLynxSystemProtection.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgLynxSystemProtection{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgLynxSystemProtection():changetype<Ros2Clearpath_motor_msgsMsgLynxSystemProtection>(out.value);}
}
/** Complete original clearpath_motor_msgs/msg/PumaFeedback; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgPumaFeedback {
static readonly maxWireBytes:i32=293;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=293;
/** Exact native field device_number, native scalar uint8. */
device_number:u8=0;
/** Exact native field device_name, native scalar string. */
device_name:string="";
/** Exact native field duty_cycle, native scalar float32. */
duty_cycle:f32=0;
/** Exact native field current, native scalar float32. */
current:f32=0;
/** Exact native field travel, native scalar float64. */
travel:f64=0;
/** Exact native field speed, native scalar float64. */
speed:f64=0;
/** Exact native field setpoint, native scalar float64. */
setpoint:f64=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>293-n)return -90;n+=1;
if(n>293)return -90;
}
{
if(!validUnicode(this.device_name)||this.device_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.device_name);if(size>256)return -22;if(4+size>293-n)return -90;n+=4+size;
if(n>293)return -90;
}
{
if(!isFinite(this.duty_cycle))return -22;
if(4>293-n)return -90;n+=4;
if(n>293)return -90;
}
{
if(!isFinite(this.current))return -22;
if(4>293-n)return -90;n+=4;
if(n>293)return -90;
}
{
if(!isFinite(this.travel))return -22;
if(8>293-n)return -90;n+=8;
if(n>293)return -90;
}
{
if(!isFinite(this.speed))return -22;
if(8>293-n)return -90;n+=8;
if(n>293)return -90;
}
{
if(!isFinite(this.setpoint))return -22;
if(8>293-n)return -90;n+=8;
if(n>293)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.device_number,1);
w.text(this.device_name);
w.integer(<u64>reinterpret<u32>(this.duty_cycle),4);
w.integer(<u64>reinterpret<u32>(this.current),4);
w.integer(<u64>reinterpret<u64>(this.travel),8);
w.integer(<u64>reinterpret<u64>(this.speed),8);
w.integer(<u64>reinterpret<u64>(this.setpoint),8);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgPumaFeedback{const value=new Ros2Clearpath_motor_msgsMsgPumaFeedback();
{
const decoded=<u8>r.integer(1);
value.device_number=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.device_name=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.duty_cycle=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.current=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.travel=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.speed=decoded;
}
{
const decoded=reinterpret<f64>(<u64>r.integer(8));if(!isFinite(decoded))r.valid=false;
value.setpoint=decoded;
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgPumaFeedbackCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgPumaFeedback>{
static readonly codecIdentity:string="UWVMROS1_c020162b07a55ac5f41df6021c0d3053d7302863f4e544a738b6cc5bc4dc9659";
static readonly maxWireBytes:i32=293;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgPumaFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgPumaFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgPumaFeedback>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgPumaFeedback>();if(bytes.length>293){out.status=90;return out;}if(bytes.length<37){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgPumaFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgPumaFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgPumaFeedback():changetype<Ros2Clearpath_motor_msgsMsgPumaFeedback>(out.value);}
}
/** Complete original clearpath_motor_msgs/msg/PumaMultiFeedback; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgPumaMultiFeedback {
static readonly maxWireBytes:i32=1316;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1316;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field drivers_feedback, native scalar clearpath_motor_msgs/msg/PumaFeedback. */
drivers_feedback:Array<Ros2Clearpath_motor_msgsMsgPumaFeedback>=new Array<Ros2Clearpath_motor_msgsMsgPumaFeedback>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>1316-n)return -90;n+=child;
if(n>1316)return -90;
}
{
if(this.drivers_feedback.length>4)return -22;
if(4>1316-n)return -90;n+=4;
for(let i=0;i<this.drivers_feedback.length;++i){
const child=this.drivers_feedback[i].wireSize();if(child<0)return child;if(child>1316-n)return -90;n+=child;
if(n>1316)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.drivers_feedback.length);
for(let i=0;i<this.drivers_feedback.length;++i){
this.drivers_feedback[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgPumaMultiFeedback{const value=new Ros2Clearpath_motor_msgsMsgPumaMultiFeedback();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.drivers_feedback=new Array<Ros2Clearpath_motor_msgsMsgPumaFeedback>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgPumaFeedback.readControl(r);
value.drivers_feedback.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgPumaMultiFeedbackCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgPumaMultiFeedback>{
static readonly codecIdentity:string="UWVMROS1_638309564580ad23d585799dcd46156e1e4261ebf9febfbb91754c250907c947";
static readonly maxWireBytes:i32=1316;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgPumaMultiFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgPumaMultiFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgPumaMultiFeedback>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgPumaMultiFeedback>();if(bytes.length>1316){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgPumaMultiFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgPumaMultiFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgPumaMultiFeedback():changetype<Ros2Clearpath_motor_msgsMsgPumaMultiFeedback>(out.value);}
}
/** Complete original clearpath_motor_msgs/msg/PumaStatus; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgPumaStatus {
static readonly MODE_VOLTAGE:u8=0;
static readonly MODE_CURRENT:u8=1;
static readonly MODE_SPEED:u8=2;
static readonly MODE_POSITION:u8=3;
static readonly MODE_VCOMP:u8=4;
static readonly FAULT_CURRENT:u8=1;
static readonly FAULT_TEMPERATURE:u8=2;
static readonly FAULT_BUS_VOLTAGE:u8=4;
static readonly FAULT_BRIDGE_DRIVER:u8=8;
static readonly maxWireBytes:i32=279;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=279;
/** Exact native field device_number, native scalar uint8. */
device_number:u8=0;
/** Exact native field device_name, native scalar string. */
device_name:string="";
/** Exact native field bus_voltage, native scalar float32. */
bus_voltage:f32=0;
/** Exact native field temperature, native scalar float32. */
temperature:f32=0;
/** Exact native field output_voltage, native scalar float32. */
output_voltage:f32=0;
/** Exact native field analog_input, native scalar float32. */
analog_input:f32=0;
/** Exact native field mode, native scalar uint8. */
mode:u8=0;
/** Exact native field fault, native scalar uint8. */
fault:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>279-n)return -90;n+=1;
if(n>279)return -90;
}
{
if(!validUnicode(this.device_name)||this.device_name.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.device_name);if(size>256)return -22;if(4+size>279-n)return -90;n+=4+size;
if(n>279)return -90;
}
{
if(!isFinite(this.bus_voltage))return -22;
if(4>279-n)return -90;n+=4;
if(n>279)return -90;
}
{
if(!isFinite(this.temperature))return -22;
if(4>279-n)return -90;n+=4;
if(n>279)return -90;
}
{
if(!isFinite(this.output_voltage))return -22;
if(4>279-n)return -90;n+=4;
if(n>279)return -90;
}
{
if(!isFinite(this.analog_input))return -22;
if(4>279-n)return -90;n+=4;
if(n>279)return -90;
}
{
if(1>279-n)return -90;n+=1;
if(n>279)return -90;
}
{
if(1>279-n)return -90;n+=1;
if(n>279)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.device_number,1);
w.text(this.device_name);
w.integer(<u64>reinterpret<u32>(this.bus_voltage),4);
w.integer(<u64>reinterpret<u32>(this.temperature),4);
w.integer(<u64>reinterpret<u32>(this.output_voltage),4);
w.integer(<u64>reinterpret<u32>(this.analog_input),4);
w.integer(<u64>this.mode,1);
w.integer(<u64>this.fault,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgPumaStatus{const value=new Ros2Clearpath_motor_msgsMsgPumaStatus();
{
const decoded=<u8>r.integer(1);
value.device_number=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.device_name=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.bus_voltage=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.temperature=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.output_voltage=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.analog_input=decoded;
}
{
const decoded=<u8>r.integer(1);
value.mode=decoded;
}
{
const decoded=<u8>r.integer(1);
value.fault=decoded;
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgPumaStatusCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgPumaStatus>{
static readonly codecIdentity:string="UWVMROS1_a027229db2c8500f707d97064548b27e77e24ae4840ad9722d2dc74726099da8";
static readonly maxWireBytes:i32=279;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgPumaStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgPumaStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgPumaStatus>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgPumaStatus>();if(bytes.length>279){out.status=90;return out;}if(bytes.length<23){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgPumaStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgPumaStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgPumaStatus():changetype<Ros2Clearpath_motor_msgsMsgPumaStatus>(out.value);}
}
/** Complete original clearpath_motor_msgs/msg/PumaMultiStatus; spellings are published in generation proof. */
export class Ros2Clearpath_motor_msgsMsgPumaMultiStatus {
static readonly maxWireBytes:i32=1260;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1260;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field drivers, native scalar clearpath_motor_msgs/msg/PumaStatus. */
drivers:Array<Ros2Clearpath_motor_msgsMsgPumaStatus>=new Array<Ros2Clearpath_motor_msgsMsgPumaStatus>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>1260-n)return -90;n+=child;
if(n>1260)return -90;
}
{
if(this.drivers.length>4)return -22;
if(4>1260-n)return -90;n+=4;
for(let i=0;i<this.drivers.length;++i){
const child=this.drivers[i].wireSize();if(child<0)return child;if(child>1260-n)return -90;n+=child;
if(n>1260)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.drivers.length);
for(let i=0;i<this.drivers.length;++i){
this.drivers[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_motor_msgsMsgPumaMultiStatus{const value=new Ros2Clearpath_motor_msgsMsgPumaMultiStatus();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>4){r.valid=false;return value;}
value.drivers=new Array<Ros2Clearpath_motor_msgsMsgPumaStatus>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Clearpath_motor_msgsMsgPumaStatus.readControl(r);
value.drivers.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_motor_msgsMsgPumaMultiStatusCodec implements VendorCodec<Ros2Clearpath_motor_msgsMsgPumaMultiStatus>{
static readonly codecIdentity:string="UWVMROS1_30a0210fea4fffe07371c898c365ef9696501a7541935dd80770671a7163c8f2";
static readonly maxWireBytes:i32=1260;
encodeChecked(value:Ros2Clearpath_motor_msgsMsgPumaMultiStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_motor_msgsMsgPumaMultiStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_motor_msgsMsgPumaMultiStatus>{const out=new VendorDecoded<Ros2Clearpath_motor_msgsMsgPumaMultiStatus>();if(bytes.length>1260){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_motor_msgsMsgPumaMultiStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_motor_msgsMsgPumaMultiStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_motor_msgsMsgPumaMultiStatus():changetype<Ros2Clearpath_motor_msgsMsgPumaMultiStatus>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/DisplayStatus; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgDisplayStatus {
static readonly maxWireBytes:i32=106;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=106;
/** Exact native field string1, native scalar string. */
string1:string="";
/** Exact native field string2, native scalar string. */
string2:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.string1)||this.string1.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.string1);if(size>49)return -22;if(4+size>106-n)return -90;n+=4+size;
if(n>106)return -90;
}
{
if(!validUnicode(this.string2)||this.string2.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.string2);if(size>49)return -22;if(4+size>106-n)return -90;n+=4+size;
if(n>106)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.string1);
w.text(this.string2);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgDisplayStatus{const value=new Ros2Clearpath_platform_msgsMsgDisplayStatus();
{
const size=r.u32();if(!r.valid||size>49){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.string1=decoded;
}
{
const size=r.u32();if(!r.valid||size>49){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.string2=decoded;
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgDisplayStatusCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgDisplayStatus>{
static readonly codecIdentity:string="UWVMROS1_e4585117414581491304a26d528b44c770799f622c3f1cde037078f5cb2462c2";
static readonly maxWireBytes:i32=106;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgDisplayStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgDisplayStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgDisplayStatus>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgDisplayStatus>();if(bytes.length>106){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgDisplayStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgDisplayStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgDisplayStatus():changetype<Ros2Clearpath_platform_msgsMsgDisplayStatus>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/Drive; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgDrive {
static readonly MODE_VELOCITY:i8=0;
static readonly MODE_PWM:i8=1;
static readonly MODE_NONE:i8=-1;
static readonly LEFT:i8=0;
static readonly RIGHT:i8=1;
static readonly maxWireBytes:i32=9;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=9;
/** Exact native field mode, native scalar int8. */
mode:i8=0;
/** Exact native field drivers, native scalar float32. */
drivers:Array<f32>=new Array<f32>();
constructor(){
for(let i=0;i<2;++i)this.drivers.push(0);
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>9-n)return -90;n+=1;
if(n>9)return -90;
}
{
if(this.drivers.length!=2)return -22;
for(let i=0;i<this.drivers.length;++i){
if(!isFinite(this.drivers[i]))return -22;
if(4>9-n)return -90;n+=4;
if(n>9)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.mode,1);
for(let i=0;i<this.drivers.length;++i){
w.integer(<u64>reinterpret<u32>(this.drivers[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgDrive{const value=new Ros2Clearpath_platform_msgsMsgDrive();
{
const decoded=<i8>r.integer(1);
value.mode=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.drivers=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.drivers.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgDriveCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgDrive>{
static readonly codecIdentity:string="UWVMROS1_be8664128ff89a066550f1a2496c30396f39bafd1d96483c545048a328138785";
static readonly maxWireBytes:i32=9;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgDrive):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgDrive):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgDrive>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgDrive>();if(bytes.length>9){out.status=90;return out;}if(bytes.length<9){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgDrive.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgDrive{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgDrive():changetype<Ros2Clearpath_platform_msgsMsgDrive>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/DriveFeedback; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgDriveFeedback {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field current, native scalar float32. */
current:f32=0;
/** Exact native field duty_cycle, native scalar float32. */
duty_cycle:f32=0;
/** Exact native field bridge_temperature, native scalar float32. */
bridge_temperature:f32=0;
/** Exact native field motor_temperature, native scalar float32. */
motor_temperature:f32=0;
/** Exact native field measured_velocity, native scalar float32. */
measured_velocity:f32=0;
/** Exact native field measured_travel, native scalar float32. */
measured_travel:f32=0;
/** Exact native field driver_fault, native scalar bool. */
driver_fault:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.current))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.duty_cycle))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.bridge_temperature))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.motor_temperature))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.measured_velocity))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.measured_travel))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(1>25-n)return -90;n+=1;
if(n>25)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.current),4);
w.integer(<u64>reinterpret<u32>(this.duty_cycle),4);
w.integer(<u64>reinterpret<u32>(this.bridge_temperature),4);
w.integer(<u64>reinterpret<u32>(this.motor_temperature),4);
w.integer(<u64>reinterpret<u32>(this.measured_velocity),4);
w.integer(<u64>reinterpret<u32>(this.measured_travel),4);
w.u8(this.driver_fault?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgDriveFeedback{const value=new Ros2Clearpath_platform_msgsMsgDriveFeedback();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.current=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.duty_cycle=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.bridge_temperature=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.motor_temperature=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.measured_velocity=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.measured_travel=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.driver_fault=decoded;
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgDriveFeedbackCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgDriveFeedback>{
static readonly codecIdentity:string="UWVMROS1_008f51d4c8341723ffec72651f93c00db0f9feceb037ee7b6b460ffcafd3f287";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgDriveFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgDriveFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgDriveFeedback>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgDriveFeedback>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgDriveFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgDriveFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgDriveFeedback():changetype<Ros2Clearpath_platform_msgsMsgDriveFeedback>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/Fans; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgFans {
static readonly CC01_FAN1:u8=0;
static readonly CC01_FAN2:u8=1;
static readonly CC01_FAN3:u8=2;
static readonly CC01_FAN4:u8=3;
static readonly CC01_FAN5:u8=4;
static readonly CC01_FAN6:u8=5;
static readonly CC01_FAN7:u8=6;
static readonly CC01_FAN8:u8=7;
static readonly R100_EQUIPMENT_BAY_INTAKE:u8=0;
static readonly R100_EQUIPMENT_BAY_EXHAUST:u8=1;
static readonly R100_CHARGER_BAY_INTAKE:u8=2;
static readonly R100_CHARGER_BAY_EXHAUST:u8=3;
static readonly R100_USER_BAY_INTAKE:u8=4;
static readonly R100_USER_BAY_EXHAUST:u8=5;
static readonly FAN_OFF:u8=0;
static readonly FAN_ON_HIGH:u8=1;
static readonly FAN_ON_LOW:u8=2;
static readonly maxWireBytes:i32=6;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=6;
/** Exact native field fans, native scalar uint8. */
fans:Array<u8>=new Array<u8>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.fans.length>2)return -22;
if(4>6-n)return -90;n+=4;
for(let i=0;i<this.fans.length;++i){
if(1>6-n)return -90;n+=1;
if(n>6)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.fans.length);
for(let i=0;i<this.fans.length;++i){
w.integer(<u64>this.fans[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgFans{const value=new Ros2Clearpath_platform_msgsMsgFans();
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.fans=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.fans.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgFansCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgFans>{
static readonly codecIdentity:string="UWVMROS1_a7d96f2242d992baf96f31bd8f93088547f92c72912c241c2d237f7714800cc8";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgFans):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgFans):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgFans>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgFans>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgFans.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgFans{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgFans():changetype<Ros2Clearpath_platform_msgsMsgFans>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/Feedback; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgFeedback {
static readonly maxWireBytes:i32=192;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=192;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field drivers, native scalar clearpath_platform_msgs/msg/DriveFeedback. */
drivers:Array<Ros2Clearpath_platform_msgsMsgDriveFeedback>=new Array<Ros2Clearpath_platform_msgsMsgDriveFeedback>();
/** Exact native field commanded_mode, native scalar int8. */
commanded_mode:i8=0;
/** Exact native field actual_mode, native scalar int8. */
actual_mode:i8=0;
constructor(){
for(let i=0;i<2;++i)this.drivers.push(new Ros2Clearpath_platform_msgsMsgDriveFeedback());
}
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>192-n)return -90;n+=child;
if(n>192)return -90;
}
{
if(this.drivers.length!=2)return -22;
for(let i=0;i<this.drivers.length;++i){
const child=this.drivers[i].wireSize();if(child<0)return child;if(child>192-n)return -90;n+=child;
if(n>192)return -90;
}
}
{
if(1>192-n)return -90;n+=1;
if(n>192)return -90;
}
{
if(1>192-n)return -90;n+=1;
if(n>192)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
for(let i=0;i<this.drivers.length;++i){
this.drivers[i].writeControl(w);
}
w.integer(<u64>this.commanded_mode,1);
w.integer(<u64>this.actual_mode,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgFeedback{const value=new Ros2Clearpath_platform_msgsMsgFeedback();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count:u32=2;if(!r.valid||count>2){r.valid=false;return value;}
value.drivers=new Array<Ros2Clearpath_platform_msgsMsgDriveFeedback>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Clearpath_platform_msgsMsgDriveFeedback.readControl(r);
value.drivers.push(decoded);
}
}
{
const decoded=<i8>r.integer(1);
value.commanded_mode=decoded;
}
{
const decoded=<i8>r.integer(1);
value.actual_mode=decoded;
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgFeedbackCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgFeedback>{
static readonly codecIdentity:string="UWVMROS1_15fdfd2e12df38bab09bf083beb61ce23d3c1082e794c34d51eec4c473e29442";
static readonly maxWireBytes:i32=192;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgFeedback):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgFeedback):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgFeedback>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgFeedback>();if(bytes.length>192){out.status=90;return out;}if(bytes.length<64){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgFeedback.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgFeedback{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgFeedback():changetype<Ros2Clearpath_platform_msgsMsgFeedback>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/RGB; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgRGB {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field red, native scalar uint8. */
red:u8=0;
/** Exact native field green, native scalar uint8. */
green:u8=0;
/** Exact native field blue, native scalar uint8. */
blue:u8=0;
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
w.integer(<u64>this.red,1);
w.integer(<u64>this.green,1);
w.integer(<u64>this.blue,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgRGB{const value=new Ros2Clearpath_platform_msgsMsgRGB();
{
const decoded=<u8>r.integer(1);
value.red=decoded;
}
{
const decoded=<u8>r.integer(1);
value.green=decoded;
}
{
const decoded=<u8>r.integer(1);
value.blue=decoded;
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgRGBCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgRGB>{
static readonly codecIdentity:string="UWVMROS1_42db8731293054ef44002afc54c16dac45544354932d5353cf749fb376cef1a1";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgRGB):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgRGB):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgRGB>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgRGB>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgRGB.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgRGB{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgRGB():changetype<Ros2Clearpath_platform_msgsMsgRGB>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/Lights; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgLights {
static readonly D100_LIGHTS_REAR_LEFT:u8=0;
static readonly D100_LIGHTS_FRONT_LEFT:u8=1;
static readonly D100_LIGHTS_FRONT_RIGHT:u8=2;
static readonly D100_LIGHTS_REAR_RIGHT:u8=3;
static readonly D150_LIGHTS_REAR_LEFT:u8=0;
static readonly D150_LIGHTS_FRONT_LEFT:u8=1;
static readonly D150_LIGHTS_FRONT_RIGHT:u8=2;
static readonly D150_LIGHTS_REAR_RIGHT:u8=3;
static readonly R100_LIGHTS_FRONT_PORT_UPPER:u8=0;
static readonly R100_LIGHTS_FRONT_PORT_LOWER:u8=1;
static readonly R100_LIGHTS_FRONT_STARBOARD_UPPER:u8=2;
static readonly R100_LIGHTS_FRONT_STARBOARD_LOWER:u8=3;
static readonly R100_LIGHTS_REAR_PORT_UPPER:u8=4;
static readonly R100_LIGHTS_REAR_PORT_LOWER:u8=5;
static readonly R100_LIGHTS_REAR_STARBOARD_UPPER:u8=6;
static readonly R100_LIGHTS_REAR_STARBOARD_LOWER:u8=7;
static readonly W200_LIGHTS_FRONT_LEFT:u8=0;
static readonly W200_LIGHTS_FRONT_RIGHT:u8=1;
static readonly W200_LIGHTS_REAR_LEFT:u8=2;
static readonly W200_LIGHTS_REAR_RIGHT:u8=3;
static readonly A300_LIGHTS_FRONT_RIGHT:u8=0;
static readonly A300_LIGHTS_FRONT_LEFT:u8=1;
static readonly A300_LIGHTS_REAR_LEFT:u8=2;
static readonly A300_LIGHTS_REAR_RIGHT:u8=3;
static readonly maxWireBytes:i32=10;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=10;
/** Exact native field lights, native scalar clearpath_platform_msgs/msg/RGB. */
lights:Array<Ros2Clearpath_platform_msgsMsgRGB>=new Array<Ros2Clearpath_platform_msgsMsgRGB>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.lights.length>2)return -22;
if(4>10-n)return -90;n+=4;
for(let i=0;i<this.lights.length;++i){
const child=this.lights[i].wireSize();if(child<0)return child;if(child>10-n)return -90;n+=child;
if(n>10)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.lights.length);
for(let i=0;i<this.lights.length;++i){
this.lights[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgLights{const value=new Ros2Clearpath_platform_msgsMsgLights();
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.lights=new Array<Ros2Clearpath_platform_msgsMsgRGB>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Clearpath_platform_msgsMsgRGB.readControl(r);
value.lights.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgLightsCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgLights>{
static readonly codecIdentity:string="UWVMROS1_3f975a3ec1fca4f8add9523d4f9f99963be520c0e2982e644551b71cbd4b8a27";
static readonly maxWireBytes:i32=10;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgLights):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgLights):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgLights>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgLights>();if(bytes.length>10){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgLights.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgLights{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgLights():changetype<Ros2Clearpath_platform_msgsMsgLights>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/PinoutCommand; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgPinoutCommand {
static readonly CC01_USER_PWR_CTRL:u8=0;
static readonly D100_AUX_OUT1:u8=0;
static readonly D100_AUX_OUT2:u8=1;
static readonly D100_AUX_OUT3:u8=2;
static readonly D150_AUX_OUT1:u8=0;
static readonly D150_AUX_OUT2:u8=1;
static readonly D150_AUX_OUT3:u8=2;
static readonly CC01_AUX_OUT1:u8=0;
static readonly CC01_AUX_OUT2:u8=1;
static readonly CC01_AUX_OUT3:u8=2;
static readonly CC01_GPO0:u8=3;
static readonly CC01_GPO1:u8=4;
static readonly CC01_GPO2:u8=5;
static readonly CC01_GPO3:u8=6;
static readonly maxWireBytes:i32=18;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=18;
/** Exact native field rails, native scalar bool. */
rails:Array<bool>=new Array<bool>();
/** Exact native field outputs, native scalar uint32. */
outputs:Array<u32>=new Array<u32>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.rails.length>2)return -22;
if(4>18-n)return -90;n+=4;
for(let i=0;i<this.rails.length;++i){
if(1>18-n)return -90;n+=1;
if(n>18)return -90;
}
}
{
if(this.outputs.length>2)return -22;
if(4>18-n)return -90;n+=4;
for(let i=0;i<this.outputs.length;++i){
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.rails.length);
for(let i=0;i<this.rails.length;++i){
w.u8(this.rails[i]?1:0);
}
w.u32(<u32>this.outputs.length);
for(let i=0;i<this.outputs.length;++i){
w.integer(<u64>this.outputs[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgPinoutCommand{const value=new Ros2Clearpath_platform_msgsMsgPinoutCommand();
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.rails=new Array<bool>();for(let i:u32=0;i<count;++i){
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.rails.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.outputs=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.outputs.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgPinoutCommandCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgPinoutCommand>{
static readonly codecIdentity:string="UWVMROS1_b26c2baf1e64cefa889c4ac6bd6b7f4647d9247eca0626b83cd726547a4e80ff";
static readonly maxWireBytes:i32=18;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgPinoutCommand):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgPinoutCommand):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgPinoutCommand>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgPinoutCommand>();if(bytes.length>18){out.status=90;return out;}if(bytes.length<8){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgPinoutCommand.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgPinoutCommand{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgPinoutCommand():changetype<Ros2Clearpath_platform_msgsMsgPinoutCommand>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/PinoutState; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgPinoutState {
static readonly CC01_USER_PWR_CTRL:u8=0;
static readonly D100_AUX_IN1:u8=0;
static readonly D100_AUX_IN2:u8=1;
static readonly D100_AUX_IN3:u8=2;
static readonly D150_AUX_IN1:u8=0;
static readonly D150_AUX_IN2:u8=1;
static readonly D150_AUX_IN3:u8=2;
static readonly CC01_AUX_IN1:u8=0;
static readonly CC01_AUX_IN2:u8=1;
static readonly CC01_AUX_IN3:u8=2;
static readonly CC01_GPI0:u8=3;
static readonly CC01_GPI1:u8=4;
static readonly CC01_GPI2:u8=5;
static readonly CC01_GPI3:u8=6;
static readonly D100_AUX_OUT1:u8=0;
static readonly D100_AUX_OUT2:u8=1;
static readonly D100_AUX_OUT3:u8=2;
static readonly D150_AUX_OUT1:u8=0;
static readonly D150_AUX_OUT2:u8=1;
static readonly D150_AUX_OUT3:u8=2;
static readonly CC01_AUX_OUT1:u8=0;
static readonly CC01_AUX_OUT2:u8=1;
static readonly CC01_AUX_OUT3:u8=2;
static readonly CC01_GPO0:u8=3;
static readonly CC01_GPO1:u8=4;
static readonly CC01_GPO2:u8=5;
static readonly CC01_GPO3:u8=6;
static readonly maxWireBytes:i32=170;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=170;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field rails, native scalar bool. */
rails:Array<bool>=new Array<bool>();
/** Exact native field inputs, native scalar bool. */
inputs:Array<bool>=new Array<bool>();
/** Exact native field outputs, native scalar bool. */
outputs:Array<bool>=new Array<bool>();
/** Exact native field output_periods, native scalar uint32. */
output_periods:Array<u32>=new Array<u32>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>170-n)return -90;n+=child;
if(n>170)return -90;
}
{
if(this.rails.length>2)return -22;
if(4>170-n)return -90;n+=4;
for(let i=0;i<this.rails.length;++i){
if(1>170-n)return -90;n+=1;
if(n>170)return -90;
}
}
{
if(this.inputs.length>2)return -22;
if(4>170-n)return -90;n+=4;
for(let i=0;i<this.inputs.length;++i){
if(1>170-n)return -90;n+=1;
if(n>170)return -90;
}
}
{
if(this.outputs.length>2)return -22;
if(4>170-n)return -90;n+=4;
for(let i=0;i<this.outputs.length;++i){
if(1>170-n)return -90;n+=1;
if(n>170)return -90;
}
}
{
if(this.output_periods.length>2)return -22;
if(4>170-n)return -90;n+=4;
for(let i=0;i<this.output_periods.length;++i){
if(4>170-n)return -90;n+=4;
if(n>170)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.rails.length);
for(let i=0;i<this.rails.length;++i){
w.u8(this.rails[i]?1:0);
}
w.u32(<u32>this.inputs.length);
for(let i=0;i<this.inputs.length;++i){
w.u8(this.inputs[i]?1:0);
}
w.u32(<u32>this.outputs.length);
for(let i=0;i<this.outputs.length;++i){
w.u8(this.outputs[i]?1:0);
}
w.u32(<u32>this.output_periods.length);
for(let i=0;i<this.output_periods.length;++i){
w.integer(<u64>this.output_periods[i],4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgPinoutState{const value=new Ros2Clearpath_platform_msgsMsgPinoutState();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.rails=new Array<bool>();for(let i:u32=0;i<count;++i){
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.rails.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.inputs=new Array<bool>();for(let i:u32=0;i<count;++i){
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.inputs.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.outputs=new Array<bool>();for(let i:u32=0;i<count;++i){
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.outputs.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.output_periods=new Array<u32>();for(let i:u32=0;i<count;++i){
const decoded=<u32>r.integer(4);
value.output_periods.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgPinoutStateCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgPinoutState>{
static readonly codecIdentity:string="UWVMROS1_a936c6d91440709d50fa9b0be8fb567a9ce793e84f57e5379f6a160c8cbf73e7";
static readonly maxWireBytes:i32=170;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgPinoutState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgPinoutState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgPinoutState>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgPinoutState>();if(bytes.length>170){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgPinoutState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgPinoutState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgPinoutState():changetype<Ros2Clearpath_platform_msgsMsgPinoutState>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/Power; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgPower {
static readonly NOT_APPLICABLE:i8=-1;
static readonly J100_MEASURED_BATTERY:u8=0;
static readonly J100_MEASURED_5V:u8=1;
static readonly J100_MEASURED_12V:u8=2;
static readonly D100_MEASURED_BATTERY:u8=0;
static readonly D100_MEASURED_5V:u8=1;
static readonly D100_MEASURED_12V:u8=2;
static readonly D150_MEASURED_BATTERY:u8=0;
static readonly D150_MEASURED_5V:u8=1;
static readonly D150_MEASURED_12V:u8=2;
static readonly W200_MEASURED_BATTERY:u8=0;
static readonly W200_MEASURED_12V:u8=1;
static readonly W200_MEASURED_24V:u8=2;
static readonly W200_MEASURED_48V:u8=3;
static readonly R100_MEASURED_BATTERY:u8=0;
static readonly R100_MEASURED_5V:u8=1;
static readonly R100_MEASURED_12V:u8=2;
static readonly R100_MEASURED_INVERTER:u8=3;
static readonly R100_MEASURED_FRONT_AXLE:u8=4;
static readonly R100_MEASURED_REAR_AXLE:u8=5;
static readonly R100_MEASURED_LIGHT:u8=6;
static readonly A200_BATTERY_VOLTAGE:u8=0;
static readonly A200_LEFT_DRIVER_VOLTAGE:u8=1;
static readonly A200_RIGHT_DRIVER_VOLTAGE:u8=2;
static readonly A200_VOLTAGES_SIZE:u8=3;
static readonly CC01_BATTERY_VOLTAGE:u8=0;
static readonly CC01_USER_BATTERY_VOLTAGE:u8=1;
static readonly CC01_USER_24V_VOLTAGE:u8=2;
static readonly CC01_USER_12VB_VOLTAGE:u8=3;
static readonly CC01_SYSTEM_12V_VOLTAGE:u8=4;
static readonly CC01_EXPANSION_VOLTAGE:u8=5;
static readonly CC01_BREAKOUT_24V_AUX_VOLTAGE:u8=6;
static readonly CC01_BREAKOUT_12V_AUX_VOLTAGE:u8=7;
static readonly CC01_BREAKOUT_USER_12VA_VOLTAGE:u8=8;
static readonly CC01_BREAKOUT_LYNX1_VOLTAGE:u8=9;
static readonly CC01_BREAKOUT_LYNX2_VOLTAGE:u8=10;
static readonly CC01_BREAKOUT_LYNX3_VOLTAGE:u8=11;
static readonly CC01_BREAKOUT_LYNX4_VOLTAGE:u8=12;
static readonly J100_TOTAL_CURRENT:u8=0;
static readonly J100_COMPUTER_CURRENT:u8=1;
static readonly J100_DRIVE_CURRENT:u8=2;
static readonly J100_USER_CURRENT:u8=3;
static readonly D100_TOTAL_CURRENT:u8=0;
static readonly D100_COMPUTER_CURRENT:u8=1;
static readonly D150_TOTAL_CURRENT:u8=0;
static readonly D150_COMPUTER_CURRENT:u8=1;
static readonly W200_TOTAL_CURRENT:u8=0;
static readonly W200_COMPUTER_CURRENT:u8=1;
static readonly W200_12V_CURRENT:u8=2;
static readonly W200_24V_CURRENT:u8=3;
static readonly R100_TOTAL_CURRENT:u8=0;
static readonly A200_MCU_AND_USER_PORT_CURRENT:u8=0;
static readonly A200_LEFT_DRIVER_CURRENT:u8=1;
static readonly A200_RIGHT_DRIVER_CURRENT:u8=2;
static readonly A200_CURRENTS_SIZE:u8=3;
static readonly CC01_BATTERY_PACK_CURRENT:u8=0;
static readonly CC01_USER_BATTERY_CURRENT:u8=1;
static readonly CC01_AUX_HES_CURRENT:u8=2;
static readonly CC01_SYSTEM_12V_CURRENT:u8=3;
static readonly CC01_24V_AUX_CURRENT:u8=4;
static readonly CC01_12V_AUX_CURRENT:u8=5;
static readonly CC01_BREAKOUT_USER_24V_CURRENT:u8=6;
static readonly CC01_BREAKOUT_USER_12VA_CURRENT:u8=7;
static readonly CC01_BREAKOUT_USER_12VB_CURRENT:u8=8;
static readonly CC01_BREAKOUT_USER_LYNX1_CURRENT:u8=9;
static readonly CC01_BREAKOUT_USER_LYNX2_CURRENT:u8=10;
static readonly CC01_BREAKOUT_USER_LYNX3_CURRENT:u8=11;
static readonly CC01_BREAKOUT_USER_LYNX4_CURRENT:u8=12;
static readonly maxWireBytes:i32=169;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=169;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field shore_power_connected, native scalar int8. */
shore_power_connected:i8=0;
/** Exact native field battery_connected, native scalar int8. */
battery_connected:i8=0;
/** Exact native field power_12v_user_nominal, native scalar int8. */
power_12v_user_nominal:i8=0;
/** Exact native field charger_connected, native scalar int8. */
charger_connected:i8=0;
/** Exact native field charging_complete, native scalar int8. */
charging_complete:i8=0;
/** Exact native field measured_voltages, native scalar float32. */
measured_voltages:Array<f32>=new Array<f32>();
/** Exact native field measured_currents, native scalar float32. */
measured_currents:Array<f32>=new Array<f32>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>169-n)return -90;n+=child;
if(n>169)return -90;
}
{
if(1>169-n)return -90;n+=1;
if(n>169)return -90;
}
{
if(1>169-n)return -90;n+=1;
if(n>169)return -90;
}
{
if(1>169-n)return -90;n+=1;
if(n>169)return -90;
}
{
if(1>169-n)return -90;n+=1;
if(n>169)return -90;
}
{
if(1>169-n)return -90;n+=1;
if(n>169)return -90;
}
{
if(this.measured_voltages.length>2)return -22;
if(4>169-n)return -90;n+=4;
for(let i=0;i<this.measured_voltages.length;++i){
if(!isFinite(this.measured_voltages[i]))return -22;
if(4>169-n)return -90;n+=4;
if(n>169)return -90;
}
}
{
if(this.measured_currents.length>2)return -22;
if(4>169-n)return -90;n+=4;
for(let i=0;i<this.measured_currents.length;++i){
if(!isFinite(this.measured_currents[i]))return -22;
if(4>169-n)return -90;n+=4;
if(n>169)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.integer(<u64>this.shore_power_connected,1);
w.integer(<u64>this.battery_connected,1);
w.integer(<u64>this.power_12v_user_nominal,1);
w.integer(<u64>this.charger_connected,1);
w.integer(<u64>this.charging_complete,1);
w.u32(<u32>this.measured_voltages.length);
for(let i=0;i<this.measured_voltages.length;++i){
w.integer(<u64>reinterpret<u32>(this.measured_voltages[i]),4);
}
w.u32(<u32>this.measured_currents.length);
for(let i=0;i<this.measured_currents.length;++i){
w.integer(<u64>reinterpret<u32>(this.measured_currents[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgPower{const value=new Ros2Clearpath_platform_msgsMsgPower();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const decoded=<i8>r.integer(1);
value.shore_power_connected=decoded;
}
{
const decoded=<i8>r.integer(1);
value.battery_connected=decoded;
}
{
const decoded=<i8>r.integer(1);
value.power_12v_user_nominal=decoded;
}
{
const decoded=<i8>r.integer(1);
value.charger_connected=decoded;
}
{
const decoded=<i8>r.integer(1);
value.charging_complete=decoded;
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.measured_voltages=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.measured_voltages.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.measured_currents=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.measured_currents.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgPowerCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgPower>{
static readonly codecIdentity:string="UWVMROS1_3fc241d6b81da2f80bfce22933b81786b5e582715dc66cd238fa40b239e7aecc";
static readonly maxWireBytes:i32=169;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgPower):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgPower):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgPower>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgPower>();if(bytes.length>169){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgPower.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgPower{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgPower():changetype<Ros2Clearpath_platform_msgsMsgPower>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/Status; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgStatus {
static readonly maxWireBytes:i32=676;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=676;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field hardware_id, native scalar string. */
hardware_id:string="";
/** Exact native field firmware_version, native scalar string. */
firmware_version:string="";
/** Exact native field mcu_uptime, native scalar builtin_interfaces/msg/Duration. */
mcu_uptime:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
/** Exact native field connection_uptime, native scalar builtin_interfaces/msg/Duration. */
connection_uptime:Ros2Builtin_interfacesMsgDuration=new Ros2Builtin_interfacesMsgDuration();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>676-n)return -90;n+=child;
if(n>676)return -90;
}
{
if(!validUnicode(this.hardware_id)||this.hardware_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.hardware_id);if(size>256)return -22;if(4+size>676-n)return -90;n+=4+size;
if(n>676)return -90;
}
{
if(!validUnicode(this.firmware_version)||this.firmware_version.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.firmware_version);if(size>256)return -22;if(4+size>676-n)return -90;n+=4+size;
if(n>676)return -90;
}
{
const child=this.mcu_uptime.wireSize();if(child<0)return child;if(child>676-n)return -90;n+=child;
if(n>676)return -90;
}
{
const child=this.connection_uptime.wireSize();if(child<0)return child;if(child>676-n)return -90;n+=child;
if(n>676)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.text(this.hardware_id);
w.text(this.firmware_version);
this.mcu_uptime.writeControl(w);
this.connection_uptime.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgStatus{const value=new Ros2Clearpath_platform_msgsMsgStatus();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.hardware_id=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.firmware_version=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.mcu_uptime=decoded;
}
{
const decoded=Ros2Builtin_interfacesMsgDuration.readControl(r);
value.connection_uptime=decoded;
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgStatusCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgStatus>{
static readonly codecIdentity:string="UWVMROS1_1bd99cc11ea9efcb51eeba8fc1d599960c16e958a8adf33a56d47fe27be87782";
static readonly maxWireBytes:i32=676;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgStatus>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgStatus>();if(bytes.length>676){out.status=90;return out;}if(bytes.length<36){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgStatus():changetype<Ros2Clearpath_platform_msgsMsgStatus>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/StopStatus; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgStopStatus {
static readonly maxWireBytes:i32=143;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=143;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field stop_power_status, native scalar bool. */
stop_power_status:bool=false;
/** Exact native field external_stop_present, native scalar bool. */
external_stop_present:bool=false;
/** Exact native field needs_reset, native scalar bool. */
needs_reset:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>143-n)return -90;n+=child;
if(n>143)return -90;
}
{
if(1>143-n)return -90;n+=1;
if(n>143)return -90;
}
{
if(1>143-n)return -90;n+=1;
if(n>143)return -90;
}
{
if(1>143-n)return -90;n+=1;
if(n>143)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u8(this.stop_power_status?1:0);
w.u8(this.external_stop_present?1:0);
w.u8(this.needs_reset?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgStopStatus{const value=new Ros2Clearpath_platform_msgsMsgStopStatus();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.stop_power_status=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.external_stop_present=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.needs_reset=decoded;
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgStopStatusCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgStopStatus>{
static readonly codecIdentity:string="UWVMROS1_101f96c8d48e7a945a2f94108e7b48393fc9d4bd75507dd709b95180ff4794d2";
static readonly maxWireBytes:i32=143;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgStopStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgStopStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgStopStatus>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgStopStatus>();if(bytes.length>143){out.status=90;return out;}if(bytes.length<15){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgStopStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgStopStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgStopStatus():changetype<Ros2Clearpath_platform_msgsMsgStopStatus>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/Temperature; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgTemperature {
static readonly J100_MCU:u8=0;
static readonly J100_PCB:u8=1;
static readonly D100_MCU:u8=0;
static readonly D100_PCB:u8=1;
static readonly D150_MCU:u8=0;
static readonly D150_PCB:u8=1;
static readonly W200_MCU:u8=0;
static readonly R100_MCU:u8=0;
static readonly R100_PCB:u8=1;
static readonly CC01_MCU:u8=0;
static readonly CC01_FAN1:u8=1;
static readonly CC01_FAN2:u8=2;
static readonly CC01_FAN3:u8=3;
static readonly CC01_FAN4:u8=4;
static readonly CC01_FAN5:u8=5;
static readonly CC01_FAN6:u8=6;
static readonly CC01_FAN7:u8=7;
static readonly CC01_FAN8:u8=8;
static readonly CC01_5V_INDUCTOR:u8=9;
static readonly CC01_MAIN_GND_LUG:u8=10;
static readonly CC01_24V_DCDC:u8=11;
static readonly CC01_12V_DCDC:u8=12;
static readonly CC01_EXT_1:u8=13;
static readonly CC01_EXT_2:u8=14;
static readonly CC01_EXT_3:u8=15;
static readonly CC01_EXT_4:u8=16;
static readonly CC01_BREAKOUT_1:u8=17;
static readonly CC01_BREAKOUT_2:u8=18;
static readonly maxWireBytes:i32=152;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=152;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field temperatures, native scalar float32. */
temperatures:Array<f32>=new Array<f32>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>152-n)return -90;n+=child;
if(n>152)return -90;
}
{
if(this.temperatures.length>2)return -22;
if(4>152-n)return -90;n+=4;
for(let i=0;i<this.temperatures.length;++i){
if(!isFinite(this.temperatures[i]))return -22;
if(4>152-n)return -90;n+=4;
if(n>152)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.temperatures.length);
for(let i=0;i<this.temperatures.length;++i){
w.integer(<u64>reinterpret<u32>(this.temperatures[i]),4);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsMsgTemperature{const value=new Ros2Clearpath_platform_msgsMsgTemperature();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.temperatures=new Array<f32>();for(let i:u32=0;i<count;++i){
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.temperatures.push(decoded);
}
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgTemperatureCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgTemperature>{
static readonly codecIdentity:string="UWVMROS1_b4ae4d6ace9045ce96b0931f08d3b2c7ec358ab9c6da7022afd9ea489f9b0466";
static readonly maxWireBytes:i32=152;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgTemperature):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgTemperature):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgTemperature>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgTemperature>();if(bytes.length>152){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgTemperature.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgTemperature{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgTemperature():changetype<Ros2Clearpath_platform_msgsMsgTemperature>(out.value);}
}
/** Complete original clearpath_control_msgs/srv/PowerControl_Request; spellings are published in generation proof. */
export class Ros2Clearpath_control_msgsSrvPowerControl_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field power_on, native scalar bool. */
power_on:bool=false;
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
w.u8(this.power_on?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_control_msgsSrvPowerControl_Request{const value=new Ros2Clearpath_control_msgsSrvPowerControl_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.power_on=decoded;
}
return value;}
}
export class Ros2Clearpath_control_msgsSrvPowerControl_RequestCodec implements VendorCodec<Ros2Clearpath_control_msgsSrvPowerControl_Request>{
static readonly codecIdentity:string="UWVMROS1_1ad49d42d8895b3bd219799ab9cc080cf09d23aa4bca4c1172d19bc18fa416c4";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Clearpath_control_msgsSrvPowerControl_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_control_msgsSrvPowerControl_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_control_msgsSrvPowerControl_Request>{const out=new VendorDecoded<Ros2Clearpath_control_msgsSrvPowerControl_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_control_msgsSrvPowerControl_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_control_msgsSrvPowerControl_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_control_msgsSrvPowerControl_Request():changetype<Ros2Clearpath_control_msgsSrvPowerControl_Request>(out.value);}
}
/** Complete original clearpath_control_msgs/srv/PowerControl_Response; spellings are published in generation proof. */
export class Ros2Clearpath_control_msgsSrvPowerControl_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
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
static readControl(r:Reader):Ros2Clearpath_control_msgsSrvPowerControl_Response{const value=new Ros2Clearpath_control_msgsSrvPowerControl_Response();
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
export class Ros2Clearpath_control_msgsSrvPowerControl_ResponseCodec implements VendorCodec<Ros2Clearpath_control_msgsSrvPowerControl_Response>{
static readonly codecIdentity:string="UWVMROS1_6d74d07d6a8ff1c41ed4ca13939d54e9e642f423dc2ff6598665238ebbf10a8b";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Clearpath_control_msgsSrvPowerControl_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_control_msgsSrvPowerControl_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_control_msgsSrvPowerControl_Response>{const out=new VendorDecoded<Ros2Clearpath_control_msgsSrvPowerControl_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_control_msgsSrvPowerControl_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_control_msgsSrvPowerControl_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_control_msgsSrvPowerControl_Response():changetype<Ros2Clearpath_control_msgsSrvPowerControl_Response>(out.value);}
}
/** Complete original clearpath_platform_msgs/srv/ConfigureMcu_Request; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsSrvConfigureMcu_Request {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field domain_id, native scalar uint8. */
domain_id:u8=0;
/** Exact native field robot_namespace, native scalar string. */
robot_namespace:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.robot_namespace)||this.robot_namespace.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.robot_namespace);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.domain_id,1);
w.text(this.robot_namespace);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsSrvConfigureMcu_Request{const value=new Ros2Clearpath_platform_msgsSrvConfigureMcu_Request();
{
const decoded=<u8>r.integer(1);
value.domain_id=decoded;
}
{
const size=r.u32();if(!r.valid||size>256){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.robot_namespace=decoded;
}
return value;}
}
export class Ros2Clearpath_platform_msgsSrvConfigureMcu_RequestCodec implements VendorCodec<Ros2Clearpath_platform_msgsSrvConfigureMcu_Request>{
static readonly codecIdentity:string="UWVMROS1_9c48451b208c4de7a51c5075587dfbe710e7ec6b249c7e1eef519022154023fa";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Clearpath_platform_msgsSrvConfigureMcu_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsSrvConfigureMcu_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsSrvConfigureMcu_Request>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsSrvConfigureMcu_Request>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsSrvConfigureMcu_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsSrvConfigureMcu_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsSrvConfigureMcu_Request():changetype<Ros2Clearpath_platform_msgsSrvConfigureMcu_Request>(out.value);}
}
/** Complete original clearpath_platform_msgs/srv/ConfigureMcu_Response; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsSrvConfigureMcu_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
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
static readControl(r:Reader):Ros2Clearpath_platform_msgsSrvConfigureMcu_Response{const value=new Ros2Clearpath_platform_msgsSrvConfigureMcu_Response();
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
export class Ros2Clearpath_platform_msgsSrvConfigureMcu_ResponseCodec implements VendorCodec<Ros2Clearpath_platform_msgsSrvConfigureMcu_Response>{
static readonly codecIdentity:string="UWVMROS1_552e715f290b5b9aa5dc926519cf2ab5120d6b2ae5eca9902efcc3f8c9e459ad";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Clearpath_platform_msgsSrvConfigureMcu_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsSrvConfigureMcu_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsSrvConfigureMcu_Response>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsSrvConfigureMcu_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsSrvConfigureMcu_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsSrvConfigureMcu_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsSrvConfigureMcu_Response():changetype<Ros2Clearpath_platform_msgsSrvConfigureMcu_Response>(out.value);}
}
/** Complete original clearpath_platform_msgs/srv/SetPinout_Request; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsSrvSetPinout_Request {
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field state, native scalar bool. */
state:bool=false;
/** Exact native field toggle_period, native scalar uint32. */
toggle_period:u32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
{
if(4>5-n)return -90;n+=4;
if(n>5)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u8(this.state?1:0);
w.integer(<u64>this.toggle_period,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Clearpath_platform_msgsSrvSetPinout_Request{const value=new Ros2Clearpath_platform_msgsSrvSetPinout_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.state=decoded;
}
{
const decoded=<u32>r.integer(4);
value.toggle_period=decoded;
}
return value;}
}
export class Ros2Clearpath_platform_msgsSrvSetPinout_RequestCodec implements VendorCodec<Ros2Clearpath_platform_msgsSrvSetPinout_Request>{
static readonly codecIdentity:string="UWVMROS1_3a3a2f8f16279c2e7a057353cd1f7f11e1a65e887dd3ce0a4b7f584708afb238";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Clearpath_platform_msgsSrvSetPinout_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsSrvSetPinout_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsSrvSetPinout_Request>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsSrvSetPinout_Request>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsSrvSetPinout_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsSrvSetPinout_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsSrvSetPinout_Request():changetype<Ros2Clearpath_platform_msgsSrvSetPinout_Request>(out.value);}
}
/** Complete original clearpath_platform_msgs/srv/SetPinout_Response; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsSrvSetPinout_Response {
static readonly maxWireBytes:i32=261;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=261;
/** Exact native field success, native scalar bool. */
success:bool=false;
/** Exact native field message, native scalar string. */
message:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>261-n)return -90;n+=1;
if(n>261)return -90;
}
{
if(!validUnicode(this.message)||this.message.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.message);if(size>256)return -22;if(4+size>261-n)return -90;n+=4+size;
if(n>261)return -90;
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
static readControl(r:Reader):Ros2Clearpath_platform_msgsSrvSetPinout_Response{const value=new Ros2Clearpath_platform_msgsSrvSetPinout_Response();
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
export class Ros2Clearpath_platform_msgsSrvSetPinout_ResponseCodec implements VendorCodec<Ros2Clearpath_platform_msgsSrvSetPinout_Response>{
static readonly codecIdentity:string="UWVMROS1_b643ef5de2dfb725906ef8f11d79601400953211b47220be7a6fcc05d1be8ee3";
static readonly maxWireBytes:i32=261;
encodeChecked(value:Ros2Clearpath_platform_msgsSrvSetPinout_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsSrvSetPinout_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsSrvSetPinout_Response>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsSrvSetPinout_Response>();if(bytes.length>261){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsSrvSetPinout_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsSrvSetPinout_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsSrvSetPinout_Response():changetype<Ros2Clearpath_platform_msgsSrvSetPinout_Response>(out.value);}
}
