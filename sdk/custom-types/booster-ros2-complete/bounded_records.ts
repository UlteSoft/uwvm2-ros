/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original booster_interface/msg/BoosterApiReqMsg; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgBoosterApiReqMsg {
static readonly maxWireBytes:i32=4108;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4108;
/** Exact native field api_id, native scalar int64. */
api_id:i64=0;
/** Exact native field body, native scalar string. */
body:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>4108-n)return -90;n+=8;
if(n>4108)return -90;
}
{
if(!validUnicode(this.body)||this.body.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.body);if(size>4096)return -22;if(4+size>4108-n)return -90;n+=4+size;
if(n>4108)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.api_id,8);
w.text(this.body);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgBoosterApiReqMsg{const value=new Ros2Booster_interfaceMsgBoosterApiReqMsg();
{
const decoded=<i64>r.integer(8);
value.api_id=decoded;
}
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.body=decoded;
}
return value;}
}
export class Ros2Booster_interfaceMsgBoosterApiReqMsgCodec implements VendorCodec<Ros2Booster_interfaceMsgBoosterApiReqMsg>{
static readonly codecIdentity:string="UWVMROS1_e2f9b66467bcb40a8194c744694f314e7fca56061f1cf3121eed565f724edd40";
static readonly maxWireBytes:i32=4108;
encodeChecked(value:Ros2Booster_interfaceMsgBoosterApiReqMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgBoosterApiReqMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgBoosterApiReqMsg>{const out=new VendorDecoded<Ros2Booster_interfaceMsgBoosterApiReqMsg>();if(bytes.length>4108){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgBoosterApiReqMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgBoosterApiReqMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgBoosterApiReqMsg():changetype<Ros2Booster_interfaceMsgBoosterApiReqMsg>(out.value);}
}
/** Complete original booster_interface/msg/BoosterApiRespMsg; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgBoosterApiRespMsg {
static readonly maxWireBytes:i32=4108;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4108;
/** Exact native field status, native scalar int64. */
status:i64=0;
/** Exact native field body, native scalar string. */
body:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(8>4108-n)return -90;n+=8;
if(n>4108)return -90;
}
{
if(!validUnicode(this.body)||this.body.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.body);if(size>4096)return -22;if(4+size>4108-n)return -90;n+=4+size;
if(n>4108)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.status,8);
w.text(this.body);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgBoosterApiRespMsg{const value=new Ros2Booster_interfaceMsgBoosterApiRespMsg();
{
const decoded=<i64>r.integer(8);
value.status=decoded;
}
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.body=decoded;
}
return value;}
}
export class Ros2Booster_interfaceMsgBoosterApiRespMsgCodec implements VendorCodec<Ros2Booster_interfaceMsgBoosterApiRespMsg>{
static readonly codecIdentity:string="UWVMROS1_21ce208a51f60843eecb72edbc408e926353b1a6f178424c0b691ac237fed11f";
static readonly maxWireBytes:i32=4108;
encodeChecked(value:Ros2Booster_interfaceMsgBoosterApiRespMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgBoosterApiRespMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgBoosterApiRespMsg>{const out=new VendorDecoded<Ros2Booster_interfaceMsgBoosterApiRespMsg>();if(bytes.length>4108){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgBoosterApiRespMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgBoosterApiRespMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgBoosterApiRespMsg():changetype<Ros2Booster_interfaceMsgBoosterApiRespMsg>(out.value);}
}
/** Complete original booster_interface/msg/ButtonEventMsg; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgButtonEventMsg {
static readonly PRESS_DOWN:i8=0;
static readonly PRESS_UP:i8=1;
static readonly SINGLE_CLICK:i8=0;
static readonly DOUBLE_CLICK:i8=1;
static readonly TRIPLE_CLICK:i8=0;
static readonly LONG_PRESS_START:i8=1;
static readonly LONG_PRESS_HOLD:i8=0;
static readonly LONG_PRESS_END:i8=1;
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field event, native scalar int8. */
event:i8=0;
/** Exact native field button, native scalar int32. */
button:i32=0;
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
w.integer(<u64>this.event,1);
w.integer(<u64>this.button,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgButtonEventMsg{const value=new Ros2Booster_interfaceMsgButtonEventMsg();
{
const decoded=<i8>r.integer(1);
value.event=decoded;
}
{
const decoded=<i32>r.integer(4);
value.button=decoded;
}
return value;}
}
export class Ros2Booster_interfaceMsgButtonEventMsgCodec implements VendorCodec<Ros2Booster_interfaceMsgButtonEventMsg>{
static readonly codecIdentity:string="UWVMROS1_b61682fd9c35bd54c52000224411f4373b8620e16587b31d8a683076e0d14fe4";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Booster_interfaceMsgButtonEventMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgButtonEventMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgButtonEventMsg>{const out=new VendorDecoded<Ros2Booster_interfaceMsgButtonEventMsg>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgButtonEventMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgButtonEventMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgButtonEventMsg():changetype<Ros2Booster_interfaceMsgButtonEventMsg>(out.value);}
}
/** Complete original booster_interface/msg/FallDownState; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgFallDownState {
static readonly IS_READY:u32=0;
static readonly IS_FALLING:u32=1;
static readonly HAS_FALLEN:u32=2;
static readonly IS_GETTING_UP:u32=3;
static readonly maxWireBytes:i32=5;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=5;
/** Exact native field fall_down_state, native scalar uint32. */
fall_down_state:u32=0;
/** Exact native field is_recovery_available, native scalar bool. */
is_recovery_available:bool=false;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>5-n)return -90;n+=4;
if(n>5)return -90;
}
{
if(1>5-n)return -90;n+=1;
if(n>5)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.fall_down_state,4);
w.u8(this.is_recovery_available?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgFallDownState{const value=new Ros2Booster_interfaceMsgFallDownState();
{
const decoded=<u32>r.integer(4);
value.fall_down_state=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.is_recovery_available=decoded;
}
return value;}
}
export class Ros2Booster_interfaceMsgFallDownStateCodec implements VendorCodec<Ros2Booster_interfaceMsgFallDownState>{
static readonly codecIdentity:string="UWVMROS1_8bf168ccefac72eda6070704c70e12e62776d29ad3db908095cc01220f4b533f";
static readonly maxWireBytes:i32=5;
encodeChecked(value:Ros2Booster_interfaceMsgFallDownState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgFallDownState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgFallDownState>{const out=new VendorDecoded<Ros2Booster_interfaceMsgFallDownState>();if(bytes.length>5){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgFallDownState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgFallDownState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgFallDownState():changetype<Ros2Booster_interfaceMsgFallDownState>(out.value);}
}
/** Complete original booster_interface/msg/HandParam; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgHandParam {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field angle, native scalar int32. */
angle:i32=0;
/** Exact native field force, native scalar int32. */
force:i32=0;
/** Exact native field speed, native scalar int32. */
speed:i32=0;
/** Exact native field seq, native scalar int32. */
seq:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.angle,4);
w.integer(<u64>this.force,4);
w.integer(<u64>this.speed,4);
w.integer(<u64>this.seq,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgHandParam{const value=new Ros2Booster_interfaceMsgHandParam();
{
const decoded=<i32>r.integer(4);
value.angle=decoded;
}
{
const decoded=<i32>r.integer(4);
value.force=decoded;
}
{
const decoded=<i32>r.integer(4);
value.speed=decoded;
}
{
const decoded=<i32>r.integer(4);
value.seq=decoded;
}
return value;}
}
export class Ros2Booster_interfaceMsgHandParamCodec implements VendorCodec<Ros2Booster_interfaceMsgHandParam>{
static readonly codecIdentity:string="UWVMROS1_3b91722e751e7c00c773335bb42af34850190ecc26a029e8792c49d912def158";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Booster_interfaceMsgHandParam):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgHandParam):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgHandParam>{const out=new VendorDecoded<Ros2Booster_interfaceMsgHandParam>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgHandParam.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgHandParam{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgHandParam():changetype<Ros2Booster_interfaceMsgHandParam>(out.value);}
}
/** Complete original booster_interface/msg/HandCommand; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgHandCommand {
static readonly maxWireBytes:i32=528;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=528;
/** Exact native field hand_param, native scalar booster_interface/msg/HandParam. */
hand_param:Array<Ros2Booster_interfaceMsgHandParam>=new Array<Ros2Booster_interfaceMsgHandParam>();
/** Exact native field force_mode, native scalar int32. */
force_mode:i32=0;
/** Exact native field hand_index, native scalar int32. */
hand_index:i32=0;
/** Exact native field hand_type, native scalar int32. */
hand_type:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.hand_param.length>32)return -22;
if(4>528-n)return -90;n+=4;
for(let i=0;i<this.hand_param.length;++i){
const child=this.hand_param[i].wireSize();if(child<0)return child;if(child>528-n)return -90;n+=child;
if(n>528)return -90;
}
}
{
if(4>528-n)return -90;n+=4;
if(n>528)return -90;
}
{
if(4>528-n)return -90;n+=4;
if(n>528)return -90;
}
{
if(4>528-n)return -90;n+=4;
if(n>528)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.hand_param.length);
for(let i=0;i<this.hand_param.length;++i){
this.hand_param[i].writeControl(w);
}
w.integer(<u64>this.force_mode,4);
w.integer(<u64>this.hand_index,4);
w.integer(<u64>this.hand_type,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgHandCommand{const value=new Ros2Booster_interfaceMsgHandCommand();
{
const count=r.u32();if(!r.valid||count>32){r.valid=false;return value;}
value.hand_param=new Array<Ros2Booster_interfaceMsgHandParam>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Booster_interfaceMsgHandParam.readControl(r);
value.hand_param.push(decoded);
}
}
{
const decoded=<i32>r.integer(4);
value.force_mode=decoded;
}
{
const decoded=<i32>r.integer(4);
value.hand_index=decoded;
}
{
const decoded=<i32>r.integer(4);
value.hand_type=decoded;
}
return value;}
}
export class Ros2Booster_interfaceMsgHandCommandCodec implements VendorCodec<Ros2Booster_interfaceMsgHandCommand>{
static readonly codecIdentity:string="UWVMROS1_06f3d6c0e9bef1da409330038aadbfacfe60b02e099f2ae0c4dcb92a632b20a0";
static readonly maxWireBytes:i32=528;
encodeChecked(value:Ros2Booster_interfaceMsgHandCommand):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgHandCommand):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgHandCommand>{const out=new VendorDecoded<Ros2Booster_interfaceMsgHandCommand>();if(bytes.length>528){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgHandCommand.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgHandCommand{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgHandCommand():changetype<Ros2Booster_interfaceMsgHandCommand>(out.value);}
}
/** Complete original booster_interface/msg/HandDdsMsg; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgHandDdsMsg {
static readonly maxWireBytes:i32=1060;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1060;
/** Exact native field hands_vec, native scalar booster_interface/msg/HandCommand. */
hands_vec:Array<Ros2Booster_interfaceMsgHandCommand>=new Array<Ros2Booster_interfaceMsgHandCommand>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.hands_vec.length>2)return -22;
if(4>1060-n)return -90;n+=4;
for(let i=0;i<this.hands_vec.length;++i){
const child=this.hands_vec[i].wireSize();if(child<0)return child;if(child>1060-n)return -90;n+=child;
if(n>1060)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.hands_vec.length);
for(let i=0;i<this.hands_vec.length;++i){
this.hands_vec[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgHandDdsMsg{const value=new Ros2Booster_interfaceMsgHandDdsMsg();
{
const count=r.u32();if(!r.valid||count>2){r.valid=false;return value;}
value.hands_vec=new Array<Ros2Booster_interfaceMsgHandCommand>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Booster_interfaceMsgHandCommand.readControl(r);
value.hands_vec.push(decoded);
}
}
return value;}
}
export class Ros2Booster_interfaceMsgHandDdsMsgCodec implements VendorCodec<Ros2Booster_interfaceMsgHandDdsMsg>{
static readonly codecIdentity:string="UWVMROS1_55af9d6f272a72e75e86e1a6164d235aca5f2ce99c2529749c0d5e33d6091397";
static readonly maxWireBytes:i32=1060;
encodeChecked(value:Ros2Booster_interfaceMsgHandDdsMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgHandDdsMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgHandDdsMsg>{const out=new VendorDecoded<Ros2Booster_interfaceMsgHandDdsMsg>();if(bytes.length>1060){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgHandDdsMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgHandDdsMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgHandDdsMsg():changetype<Ros2Booster_interfaceMsgHandDdsMsg>(out.value);}
}
/** Complete original booster_interface/msg/ImuState; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgImuState {
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
static readControl(r:Reader):Ros2Booster_interfaceMsgImuState{const value=new Ros2Booster_interfaceMsgImuState();
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
export class Ros2Booster_interfaceMsgImuStateCodec implements VendorCodec<Ros2Booster_interfaceMsgImuState>{
static readonly codecIdentity:string="UWVMROS1_838ee8d4b93b0c3673e0b535fd3905913e94be24d6d21a01504dedaaaaa81092";
static readonly maxWireBytes:i32=36;
encodeChecked(value:Ros2Booster_interfaceMsgImuState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgImuState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgImuState>{const out=new VendorDecoded<Ros2Booster_interfaceMsgImuState>();if(bytes.length>36){out.status=90;return out;}if(bytes.length<36){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgImuState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgImuState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgImuState():changetype<Ros2Booster_interfaceMsgImuState>(out.value);}
}
/** Complete original booster_interface/msg/MotorCmd; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgMotorCmd {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field mode, native scalar int8. */
mode:i8=0;
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
static readControl(r:Reader):Ros2Booster_interfaceMsgMotorCmd{const value=new Ros2Booster_interfaceMsgMotorCmd();
{
const decoded=<i8>r.integer(1);
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
export class Ros2Booster_interfaceMsgMotorCmdCodec implements VendorCodec<Ros2Booster_interfaceMsgMotorCmd>{
static readonly codecIdentity:string="UWVMROS1_69d814e13595d53dcf44f2a5ccb19df0f8f2be0908c310c3b60cec33bcf27392";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Booster_interfaceMsgMotorCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgMotorCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgMotorCmd>{const out=new VendorDecoded<Ros2Booster_interfaceMsgMotorCmd>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgMotorCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgMotorCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgMotorCmd():changetype<Ros2Booster_interfaceMsgMotorCmd>(out.value);}
}
/** Complete original booster_interface/msg/LowCmd; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgLowCmd {
static readonly CMD_TYPE_PARALLEL:i8=0;
static readonly CMD_TYPE_SERIAL:i8=1;
static readonly maxWireBytes:i32=730;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=730;
/** Exact native field cmd_type, native scalar int8. */
cmd_type:i8=0;
/** Exact native field motor_cmd, native scalar booster_interface/msg/MotorCmd. */
motor_cmd:Array<Ros2Booster_interfaceMsgMotorCmd>=new Array<Ros2Booster_interfaceMsgMotorCmd>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(1>730-n)return -90;n+=1;
if(n>730)return -90;
}
{
if(this.motor_cmd.length>29)return -22;
if(4>730-n)return -90;n+=4;
for(let i=0;i<this.motor_cmd.length;++i){
const child=this.motor_cmd[i].wireSize();if(child<0)return child;if(child>730-n)return -90;n+=child;
if(n>730)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.cmd_type,1);
w.u32(<u32>this.motor_cmd.length);
for(let i=0;i<this.motor_cmd.length;++i){
this.motor_cmd[i].writeControl(w);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgLowCmd{const value=new Ros2Booster_interfaceMsgLowCmd();
{
const decoded=<i8>r.integer(1);
value.cmd_type=decoded;
}
{
const count=r.u32();if(!r.valid||count>29){r.valid=false;return value;}
value.motor_cmd=new Array<Ros2Booster_interfaceMsgMotorCmd>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Booster_interfaceMsgMotorCmd.readControl(r);
value.motor_cmd.push(decoded);
}
}
return value;}
}
export class Ros2Booster_interfaceMsgLowCmdCodec implements VendorCodec<Ros2Booster_interfaceMsgLowCmd>{
static readonly codecIdentity:string="UWVMROS1_845726a14e61d211b493041f4dda56e1556b76b9d30337a2ee5f6319a3d1e870";
static readonly maxWireBytes:i32=730;
encodeChecked(value:Ros2Booster_interfaceMsgLowCmd):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgLowCmd):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgLowCmd>{const out=new VendorDecoded<Ros2Booster_interfaceMsgLowCmd>();if(bytes.length>730){out.status=90;return out;}if(bytes.length<5){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgLowCmd.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgLowCmd{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgLowCmd():changetype<Ros2Booster_interfaceMsgLowCmd>(out.value);}
}
/** Complete original booster_interface/msg/MotorState; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgMotorState {
static readonly maxWireBytes:i32=30;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=30;
/** Exact native field mode, native scalar int8. */
mode:i8=0;
/** Exact native field q, native scalar float32. */
q:f32=0;
/** Exact native field dq, native scalar float32. */
dq:f32=0;
/** Exact native field ddq, native scalar float32. */
ddq:f32=0;
/** Exact native field tau_est, native scalar float32. */
tau_est:f32=0;
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
static readControl(r:Reader):Ros2Booster_interfaceMsgMotorState{const value=new Ros2Booster_interfaceMsgMotorState();
{
const decoded=<i8>r.integer(1);
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
export class Ros2Booster_interfaceMsgMotorStateCodec implements VendorCodec<Ros2Booster_interfaceMsgMotorState>{
static readonly codecIdentity:string="UWVMROS1_6b19c0e8865e796fff5a498cff731ba268fa92ea2944e6c2c5869009b3aa066b";
static readonly maxWireBytes:i32=30;
encodeChecked(value:Ros2Booster_interfaceMsgMotorState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgMotorState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgMotorState>{const out=new VendorDecoded<Ros2Booster_interfaceMsgMotorState>();if(bytes.length>30){out.status=90;return out;}if(bytes.length<30){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgMotorState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgMotorState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgMotorState():changetype<Ros2Booster_interfaceMsgMotorState>(out.value);}
}
/** Complete original booster_interface/msg/LowState; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgLowState {
static readonly maxWireBytes:i32=1784;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1784;
/** Exact native field imu_state, native scalar booster_interface/msg/ImuState. */
imu_state:Ros2Booster_interfaceMsgImuState=new Ros2Booster_interfaceMsgImuState();
/** Exact native field motor_state_parallel, native scalar booster_interface/msg/MotorState. */
motor_state_parallel:Array<Ros2Booster_interfaceMsgMotorState>=new Array<Ros2Booster_interfaceMsgMotorState>();
/** Exact native field motor_state_serial, native scalar booster_interface/msg/MotorState. */
motor_state_serial:Array<Ros2Booster_interfaceMsgMotorState>=new Array<Ros2Booster_interfaceMsgMotorState>();
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
static readControl(r:Reader):Ros2Booster_interfaceMsgLowState{const value=new Ros2Booster_interfaceMsgLowState();
{
const decoded=Ros2Booster_interfaceMsgImuState.readControl(r);
value.imu_state=decoded;
}
{
const count=r.u32();if(!r.valid||count>29){r.valid=false;return value;}
value.motor_state_parallel=new Array<Ros2Booster_interfaceMsgMotorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Booster_interfaceMsgMotorState.readControl(r);
value.motor_state_parallel.push(decoded);
}
}
{
const count=r.u32();if(!r.valid||count>29){r.valid=false;return value;}
value.motor_state_serial=new Array<Ros2Booster_interfaceMsgMotorState>();for(let i:u32=0;i<count;++i){
const decoded=Ros2Booster_interfaceMsgMotorState.readControl(r);
value.motor_state_serial.push(decoded);
}
}
return value;}
}
export class Ros2Booster_interfaceMsgLowStateCodec implements VendorCodec<Ros2Booster_interfaceMsgLowState>{
static readonly codecIdentity:string="UWVMROS1_811db19cab7b790ed48506bca3e3ef512c1a661dc3ac99cbce6234efb6de631f";
static readonly maxWireBytes:i32=1784;
encodeChecked(value:Ros2Booster_interfaceMsgLowState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgLowState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgLowState>{const out=new VendorDecoded<Ros2Booster_interfaceMsgLowState>();if(bytes.length>1784){out.status=90;return out;}if(bytes.length<44){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgLowState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgLowState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgLowState():changetype<Ros2Booster_interfaceMsgLowState>(out.value);}
}
/** Complete original booster_interface/msg/Odometer; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgOdometer {
static readonly maxWireBytes:i32=12;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=12;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field theta, native scalar float32. */
theta:f32=0;
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
if(!isFinite(this.theta))return -22;
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
w.integer(<u64>reinterpret<u32>(this.theta),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgOdometer{const value=new Ros2Booster_interfaceMsgOdometer();
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
value.theta=decoded;
}
return value;}
}
export class Ros2Booster_interfaceMsgOdometerCodec implements VendorCodec<Ros2Booster_interfaceMsgOdometer>{
static readonly codecIdentity:string="UWVMROS1_4fea33e3e2ae186dbeaf49c6068019b702e742266541087a5d280b9ce42c671a";
static readonly maxWireBytes:i32=12;
encodeChecked(value:Ros2Booster_interfaceMsgOdometer):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgOdometer):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgOdometer>{const out=new VendorDecoded<Ros2Booster_interfaceMsgOdometer>();if(bytes.length>12){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgOdometer.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgOdometer{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgOdometer():changetype<Ros2Booster_interfaceMsgOdometer>(out.value);}
}
/** Complete original booster_interface/msg/RawBytesMsg; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgRawBytesMsg {
static readonly maxWireBytes:i32=2052;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2052;
/** Exact native field msg, native scalar char. */
msg:Array<u8>=new Array<u8>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(this.msg.length>2048)return -22;
if(4>2052-n)return -90;n+=4;
for(let i=0;i<this.msg.length;++i){
if(1>2052-n)return -90;n+=1;
if(n>2052)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.u32(<u32>this.msg.length);
for(let i=0;i<this.msg.length;++i){
w.integer(<u64>this.msg[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgRawBytesMsg{const value=new Ros2Booster_interfaceMsgRawBytesMsg();
{
const count=r.u32();if(!r.valid||count>2048){r.valid=false;return value;}
value.msg=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.msg.push(decoded);
}
}
return value;}
}
export class Ros2Booster_interfaceMsgRawBytesMsgCodec implements VendorCodec<Ros2Booster_interfaceMsgRawBytesMsg>{
static readonly codecIdentity:string="UWVMROS1_557e8e79f206c9cc8b820ed7c2ad8cc01e9b01327197b745709993d958b4578c";
static readonly maxWireBytes:i32=2052;
encodeChecked(value:Ros2Booster_interfaceMsgRawBytesMsg):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgRawBytesMsg):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgRawBytesMsg>{const out=new VendorDecoded<Ros2Booster_interfaceMsgRawBytesMsg>();if(bytes.length>2052){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgRawBytesMsg.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgRawBytesMsg{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgRawBytesMsg():changetype<Ros2Booster_interfaceMsgRawBytesMsg>(out.value);}
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
/** Complete original booster_interface/msg/RawBytesStamped; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgRawBytesStamped {
static readonly maxWireBytes:i32=2192;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=2192;
/** Exact native field header, native scalar std_msgs/msg/Header. */
header:Ros2Std_msgsMsgHeader=new Ros2Std_msgsMsgHeader();
/** Exact native field msg, native scalar char. */
msg:Array<u8>=new Array<u8>();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>2192-n)return -90;n+=child;
if(n>2192)return -90;
}
{
if(this.msg.length>2048)return -22;
if(4>2192-n)return -90;n+=4;
for(let i=0;i<this.msg.length;++i){
if(1>2192-n)return -90;n+=1;
if(n>2192)return -90;
}
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.header.writeControl(w);
w.u32(<u32>this.msg.length);
for(let i=0;i<this.msg.length;++i){
w.integer(<u64>this.msg[i],1);
}
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgRawBytesStamped{const value=new Ros2Booster_interfaceMsgRawBytesStamped();
{
const decoded=Ros2Std_msgsMsgHeader.readControl(r);
value.header=decoded;
}
{
const count=r.u32();if(!r.valid||count>2048){r.valid=false;return value;}
value.msg=new Array<u8>();for(let i:u32=0;i<count;++i){
const decoded=<u8>r.integer(1);
value.msg.push(decoded);
}
}
return value;}
}
export class Ros2Booster_interfaceMsgRawBytesStampedCodec implements VendorCodec<Ros2Booster_interfaceMsgRawBytesStamped>{
static readonly codecIdentity:string="UWVMROS1_27cddd472b2d8c41c39effb87a8a53261aff309500dbb68df6ebd69a19c7720e";
static readonly maxWireBytes:i32=2192;
encodeChecked(value:Ros2Booster_interfaceMsgRawBytesStamped):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgRawBytesStamped):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgRawBytesStamped>{const out=new VendorDecoded<Ros2Booster_interfaceMsgRawBytesStamped>();if(bytes.length>2192){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgRawBytesStamped.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgRawBytesStamped{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgRawBytesStamped():changetype<Ros2Booster_interfaceMsgRawBytesStamped>(out.value);}
}
/** Complete original booster_interface/msg/RemoteControllerState; spellings are published in generation proof. */
export class Ros2Booster_interfaceMsgRemoteControllerState {
static readonly maxWireBytes:i32=42;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=42;
/** Exact native field event, native scalar uint32. */
event:u32=0;
/** Exact native field lx, native scalar float32. */
lx:f32=0;
/** Exact native field ly, native scalar float32. */
ly:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field a, native scalar bool. */
a:bool=false;
/** Exact native field b, native scalar bool. */
b:bool=false;
/** Exact native field x, native scalar bool. */
x:bool=false;
/** Exact native field y, native scalar bool. */
y:bool=false;
/** Exact native field lb, native scalar bool. */
lb:bool=false;
/** Exact native field rb, native scalar bool. */
rb:bool=false;
/** Exact native field lt, native scalar bool. */
lt:bool=false;
/** Exact native field rt, native scalar bool. */
rt:bool=false;
/** Exact native field ls, native scalar bool. */
ls:bool=false;
/** Exact native field rs, native scalar bool. */
rs:bool=false;
/** Exact native field back, native scalar bool. */
back:bool=false;
/** Exact native field start, native scalar bool. */
start:bool=false;
/** Exact native field hat_c, native scalar bool. */
hat_c:bool=false;
/** Exact native field hat_u, native scalar bool. */
hat_u:bool=false;
/** Exact native field hat_d, native scalar bool. */
hat_d:bool=false;
/** Exact native field hat_l, native scalar bool. */
hat_l:bool=false;
/** Exact native field hat_r, native scalar bool. */
hat_r:bool=false;
/** Exact native field hat_lu, native scalar bool. */
hat_lu:bool=false;
/** Exact native field hat_ld, native scalar bool. */
hat_ld:bool=false;
/** Exact native field hat_ru, native scalar bool. */
hat_ru:bool=false;
/** Exact native field hat_rd, native scalar bool. */
hat_rd:bool=false;
/** Exact native field hat_pos, native scalar uint8. */
hat_pos:u8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(!isFinite(this.lx))return -22;
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(!isFinite(this.ly))return -22;
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>42-n)return -90;n+=4;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
{
if(1>42-n)return -90;n+=1;
if(n>42)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.event,4);
w.integer(<u64>reinterpret<u32>(this.lx),4);
w.integer(<u64>reinterpret<u32>(this.ly),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.u8(this.a?1:0);
w.u8(this.b?1:0);
w.u8(this.x?1:0);
w.u8(this.y?1:0);
w.u8(this.lb?1:0);
w.u8(this.rb?1:0);
w.u8(this.lt?1:0);
w.u8(this.rt?1:0);
w.u8(this.ls?1:0);
w.u8(this.rs?1:0);
w.u8(this.back?1:0);
w.u8(this.start?1:0);
w.u8(this.hat_c?1:0);
w.u8(this.hat_u?1:0);
w.u8(this.hat_d?1:0);
w.u8(this.hat_l?1:0);
w.u8(this.hat_r?1:0);
w.u8(this.hat_lu?1:0);
w.u8(this.hat_ld?1:0);
w.u8(this.hat_ru?1:0);
w.u8(this.hat_rd?1:0);
w.integer(<u64>this.hat_pos,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceMsgRemoteControllerState{const value=new Ros2Booster_interfaceMsgRemoteControllerState();
{
const decoded=<u32>r.integer(4);
value.event=decoded;
}
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
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.a=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.b=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.x=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.y=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.lb=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.rb=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.lt=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.rt=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.ls=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.rs=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.back=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.start=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.hat_c=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.hat_u=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.hat_d=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.hat_l=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.hat_r=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.hat_lu=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.hat_ld=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.hat_ru=decoded;
}
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.hat_rd=decoded;
}
{
const decoded=<u8>r.integer(1);
value.hat_pos=decoded;
}
return value;}
}
export class Ros2Booster_interfaceMsgRemoteControllerStateCodec implements VendorCodec<Ros2Booster_interfaceMsgRemoteControllerState>{
static readonly codecIdentity:string="UWVMROS1_f8a21705728862632c27b71d98c044a12bf1f9f9e0c1729fec27afe2c60c48a7";
static readonly maxWireBytes:i32=42;
encodeChecked(value:Ros2Booster_interfaceMsgRemoteControllerState):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceMsgRemoteControllerState):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceMsgRemoteControllerState>{const out=new VendorDecoded<Ros2Booster_interfaceMsgRemoteControllerState>();if(bytes.length>42){out.status=90;return out;}if(bytes.length<42){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceMsgRemoteControllerState.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceMsgRemoteControllerState{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceMsgRemoteControllerState():changetype<Ros2Booster_interfaceMsgRemoteControllerState>(out.value);}
}
/** Complete original booster_interface/srv/AgentService_Request; spellings are published in generation proof. */
export class Ros2Booster_interfaceSrvAgentService_Request {
static readonly maxWireBytes:i32=4100;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4100;
/** Exact native field body, native scalar string. */
body:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.body)||this.body.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.body);if(size>4096)return -22;if(4+size>4100-n)return -90;n+=4+size;
if(n>4100)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.body);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceSrvAgentService_Request{const value=new Ros2Booster_interfaceSrvAgentService_Request();
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.body=decoded;
}
return value;}
}
export class Ros2Booster_interfaceSrvAgentService_RequestCodec implements VendorCodec<Ros2Booster_interfaceSrvAgentService_Request>{
static readonly codecIdentity:string="UWVMROS1_d8b972ee6c7f82ee948c03b22354d1073f120ad2616a9e833b578348775404e8";
static readonly maxWireBytes:i32=4100;
encodeChecked(value:Ros2Booster_interfaceSrvAgentService_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceSrvAgentService_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceSrvAgentService_Request>{const out=new VendorDecoded<Ros2Booster_interfaceSrvAgentService_Request>();if(bytes.length>4100){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceSrvAgentService_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceSrvAgentService_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceSrvAgentService_Request():changetype<Ros2Booster_interfaceSrvAgentService_Request>(out.value);}
}
/** Complete original booster_interface/srv/AgentService_Response; spellings are published in generation proof. */
export class Ros2Booster_interfaceSrvAgentService_Response {
static readonly maxWireBytes:i32=4100;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4100;
/** Exact native field body, native scalar string. */
body:string="";
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!validUnicode(this.body)||this.body.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.body);if(size>4096)return -22;if(4+size>4100-n)return -90;n+=4+size;
if(n>4100)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.text(this.body);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceSrvAgentService_Response{const value=new Ros2Booster_interfaceSrvAgentService_Response();
{
const size=r.u32();if(!r.valid||size>4096){r.valid=false;return value;}const raw=r.take(size);
if(!r.valid||!validUtf8(raw,false)){r.valid=false;return value;}const decoded=String.UTF8.decode(raw.buffer);
value.body=decoded;
}
return value;}
}
export class Ros2Booster_interfaceSrvAgentService_ResponseCodec implements VendorCodec<Ros2Booster_interfaceSrvAgentService_Response>{
static readonly codecIdentity:string="UWVMROS1_86aecf9ee8652016b5c74e30d2dd4100e8b547542a81a98e7795942b47b87520";
static readonly maxWireBytes:i32=4100;
encodeChecked(value:Ros2Booster_interfaceSrvAgentService_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceSrvAgentService_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceSrvAgentService_Response>{const out=new VendorDecoded<Ros2Booster_interfaceSrvAgentService_Response>();if(bytes.length>4100){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceSrvAgentService_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceSrvAgentService_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceSrvAgentService_Response():changetype<Ros2Booster_interfaceSrvAgentService_Response>(out.value);}
}
/** Complete original booster_interface/srv/RpcService_Request; spellings are published in generation proof. */
export class Ros2Booster_interfaceSrvRpcService_Request {
static readonly maxWireBytes:i32=4108;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4108;
/** Exact native field msg, native scalar booster_interface/msg/BoosterApiReqMsg. */
msg:Ros2Booster_interfaceMsgBoosterApiReqMsg=new Ros2Booster_interfaceMsgBoosterApiReqMsg();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.msg.wireSize();if(child<0)return child;if(child>4108-n)return -90;n+=child;
if(n>4108)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.msg.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceSrvRpcService_Request{const value=new Ros2Booster_interfaceSrvRpcService_Request();
{
const decoded=Ros2Booster_interfaceMsgBoosterApiReqMsg.readControl(r);
value.msg=decoded;
}
return value;}
}
export class Ros2Booster_interfaceSrvRpcService_RequestCodec implements VendorCodec<Ros2Booster_interfaceSrvRpcService_Request>{
static readonly codecIdentity:string="UWVMROS1_cc8eb67d80bdf131f1878a1be998d68560e235ffef6a539876410dfcd6c00150";
static readonly maxWireBytes:i32=4108;
encodeChecked(value:Ros2Booster_interfaceSrvRpcService_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceSrvRpcService_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceSrvRpcService_Request>{const out=new VendorDecoded<Ros2Booster_interfaceSrvRpcService_Request>();if(bytes.length>4108){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceSrvRpcService_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceSrvRpcService_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceSrvRpcService_Request():changetype<Ros2Booster_interfaceSrvRpcService_Request>(out.value);}
}
/** Complete original booster_interface/srv/RpcService_Response; spellings are published in generation proof. */
export class Ros2Booster_interfaceSrvRpcService_Response {
static readonly maxWireBytes:i32=4108;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4108;
/** Exact native field msg, native scalar booster_interface/msg/BoosterApiRespMsg. */
msg:Ros2Booster_interfaceMsgBoosterApiRespMsg=new Ros2Booster_interfaceMsgBoosterApiRespMsg();
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.msg.wireSize();if(child<0)return child;if(child>4108-n)return -90;n+=child;
if(n>4108)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
this.msg.writeControl(w);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Booster_interfaceSrvRpcService_Response{const value=new Ros2Booster_interfaceSrvRpcService_Response();
{
const decoded=Ros2Booster_interfaceMsgBoosterApiRespMsg.readControl(r);
value.msg=decoded;
}
return value;}
}
export class Ros2Booster_interfaceSrvRpcService_ResponseCodec implements VendorCodec<Ros2Booster_interfaceSrvRpcService_Response>{
static readonly codecIdentity:string="UWVMROS1_7aeb7b45542f9f594e1f61be2f715abcde54b1e55efdde15100b0c553ae96322";
static readonly maxWireBytes:i32=4108;
encodeChecked(value:Ros2Booster_interfaceSrvRpcService_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Booster_interfaceSrvRpcService_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Booster_interfaceSrvRpcService_Response>{const out=new VendorDecoded<Ros2Booster_interfaceSrvRpcService_Response>();if(bytes.length>4108){out.status=90;return out;}if(bytes.length<12){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Booster_interfaceSrvRpcService_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Booster_interfaceSrvRpcService_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Booster_interfaceSrvRpcService_Response():changetype<Ros2Booster_interfaceSrvRpcService_Response>(out.value);}
}
