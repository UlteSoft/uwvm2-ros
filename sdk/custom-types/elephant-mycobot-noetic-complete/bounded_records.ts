/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original mybuddy_communication/MybuddyAngles; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationMybuddyAngles {
static readonly maxWireBytes:i32=52;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=52;
/** Exact native field joint_0, native scalar float32. */
joint_0:f32=0;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field joint_7, native scalar float32. */
joint_7:f32=0;
/** Exact native field joint_8, native scalar float32. */
joint_8:f32=0;
/** Exact native field joint_9, native scalar float32. */
joint_9:f32=0;
/** Exact native field joint_10, native scalar float32. */
joint_10:f32=0;
/** Exact native field joint_11, native scalar float32. */
joint_11:f32=0;
/** Exact native field joint_12, native scalar float32. */
joint_12:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_0))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_1))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_7))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_8))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_9))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_10))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_11))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_12))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_0),4);
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>reinterpret<u32>(this.joint_7),4);
w.integer(<u64>reinterpret<u32>(this.joint_8),4);
w.integer(<u64>reinterpret<u32>(this.joint_9),4);
w.integer(<u64>reinterpret<u32>(this.joint_10),4);
w.integer(<u64>reinterpret<u32>(this.joint_11),4);
w.integer(<u64>reinterpret<u32>(this.joint_12),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationMybuddyAngles{const value=new Ros1Mybuddy_communicationMybuddyAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_0=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_7=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_8=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_9=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_10=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_11=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_12=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationMybuddyAnglesCodec implements VendorCodec<Ros1Mybuddy_communicationMybuddyAngles>{
static readonly codecIdentity:string="UWVMROS1_f3c510869207e6d72e868ae35ef95d49a1eea5164c7ef1056cb3d335029563e9";
static readonly maxWireBytes:i32=52;
encodeChecked(value:Ros1Mybuddy_communicationMybuddyAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationMybuddyAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationMybuddyAngles>{const out=new VendorDecoded<Ros1Mybuddy_communicationMybuddyAngles>();if(bytes.length>52){out.status=90;return out;}if(bytes.length<52){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationMybuddyAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationMybuddyAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationMybuddyAngles():changetype<Ros1Mybuddy_communicationMybuddyAngles>(out.value);}
}
/** Complete original mybuddy_communication/MybuddyCoords; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationMybuddyCoords {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationMybuddyCoords{const value=new Ros1Mybuddy_communicationMybuddyCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationMybuddyCoordsCodec implements VendorCodec<Ros1Mybuddy_communicationMybuddyCoords>{
static readonly codecIdentity:string="UWVMROS1_a1df376733f2673e25fed814a038e00f556b82c2b8bce3755644904e50c95384";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mybuddy_communicationMybuddyCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationMybuddyCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationMybuddyCoords>{const out=new VendorDecoded<Ros1Mybuddy_communicationMybuddyCoords>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationMybuddyCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationMybuddyCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationMybuddyCoords():changetype<Ros1Mybuddy_communicationMybuddyCoords>(out.value);}
}
/** Complete original mybuddy_communication/MybuddyGripperStatus; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationMybuddyGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationMybuddyGripperStatus{const value=new Ros1Mybuddy_communicationMybuddyGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationMybuddyGripperStatusCodec implements VendorCodec<Ros1Mybuddy_communicationMybuddyGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_86fdaf7d3397b26155c891e15d2ebdd903d5899a0895128e720276eb35a63854";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mybuddy_communicationMybuddyGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationMybuddyGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationMybuddyGripperStatus>{const out=new VendorDecoded<Ros1Mybuddy_communicationMybuddyGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationMybuddyGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationMybuddyGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationMybuddyGripperStatus():changetype<Ros1Mybuddy_communicationMybuddyGripperStatus>(out.value);}
}
/** Complete original mybuddy_communication/MybuddyPumpStatus; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationMybuddyPumpStatus {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationMybuddyPumpStatus{const value=new Ros1Mybuddy_communicationMybuddyPumpStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationMybuddyPumpStatusCodec implements VendorCodec<Ros1Mybuddy_communicationMybuddyPumpStatus>{
static readonly codecIdentity:string="UWVMROS1_33b6419700f9baf41b584bb464d0234426a04e1bd86d198fe3d66c06845cd80a";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Mybuddy_communicationMybuddyPumpStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationMybuddyPumpStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationMybuddyPumpStatus>{const out=new VendorDecoded<Ros1Mybuddy_communicationMybuddyPumpStatus>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationMybuddyPumpStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationMybuddyPumpStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationMybuddyPumpStatus():changetype<Ros1Mybuddy_communicationMybuddyPumpStatus>(out.value);}
}
/** Complete original mybuddy_communication/MybuddySetAngles; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationMybuddySetAngles {
static readonly maxWireBytes:i32=53;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=53;
/** Exact native field joint_0, native scalar float32. */
joint_0:f32=0;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field joint_7, native scalar float32. */
joint_7:f32=0;
/** Exact native field joint_8, native scalar float32. */
joint_8:f32=0;
/** Exact native field joint_9, native scalar float32. */
joint_9:f32=0;
/** Exact native field joint_10, native scalar float32. */
joint_10:f32=0;
/** Exact native field joint_11, native scalar float32. */
joint_11:f32=0;
/** Exact native field joint_12, native scalar float32. */
joint_12:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_0))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_1))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_7))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_8))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_9))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_10))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_11))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_12))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
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
w.integer(<u64>reinterpret<u32>(this.joint_0),4);
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>reinterpret<u32>(this.joint_7),4);
w.integer(<u64>reinterpret<u32>(this.joint_8),4);
w.integer(<u64>reinterpret<u32>(this.joint_9),4);
w.integer(<u64>reinterpret<u32>(this.joint_10),4);
w.integer(<u64>reinterpret<u32>(this.joint_11),4);
w.integer(<u64>reinterpret<u32>(this.joint_12),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationMybuddySetAngles{const value=new Ros1Mybuddy_communicationMybuddySetAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_0=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_7=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_8=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_9=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_10=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_11=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_12=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationMybuddySetAnglesCodec implements VendorCodec<Ros1Mybuddy_communicationMybuddySetAngles>{
static readonly codecIdentity:string="UWVMROS1_8f41fddfb715bfaa95c90974cf257181a9ae3e2f36969f6fdfdf3ab03b7d4486";
static readonly maxWireBytes:i32=53;
encodeChecked(value:Ros1Mybuddy_communicationMybuddySetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationMybuddySetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationMybuddySetAngles>{const out=new VendorDecoded<Ros1Mybuddy_communicationMybuddySetAngles>();if(bytes.length>53){out.status=90;return out;}if(bytes.length<53){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationMybuddySetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationMybuddySetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationMybuddySetAngles():changetype<Ros1Mybuddy_communicationMybuddySetAngles>(out.value);}
}
/** Complete original mybuddy_communication/MybuddySetCoords; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationMybuddySetCoords {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationMybuddySetCoords{const value=new Ros1Mybuddy_communicationMybuddySetCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationMybuddySetCoordsCodec implements VendorCodec<Ros1Mybuddy_communicationMybuddySetCoords>{
static readonly codecIdentity:string="UWVMROS1_bf9f2ee073928aafabbd532644eb57eec70d4b864a3097e2ab1a37840abd3669";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros1Mybuddy_communicationMybuddySetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationMybuddySetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationMybuddySetCoords>{const out=new VendorDecoded<Ros1Mybuddy_communicationMybuddySetCoords>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationMybuddySetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationMybuddySetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationMybuddySetCoords():changetype<Ros1Mybuddy_communicationMybuddySetCoords>(out.value);}
}
/** Complete original mybuddy_communication/GetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationGetAnglesRequest {
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
static readControl(r:Reader):Ros1Mybuddy_communicationGetAnglesRequest{const value=new Ros1Mybuddy_communicationGetAnglesRequest();
return value;}
}
export class Ros1Mybuddy_communicationGetAnglesRequestCodec implements VendorCodec<Ros1Mybuddy_communicationGetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_7ba90c12eb91dbcc296c0d61069d0dc12c3ac576c970cd2fe9a0fed29f058e38";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mybuddy_communicationGetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationGetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationGetAnglesRequest>{const out=new VendorDecoded<Ros1Mybuddy_communicationGetAnglesRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationGetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationGetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationGetAnglesRequest():changetype<Ros1Mybuddy_communicationGetAnglesRequest>(out.value);}
}
/** Complete original mybuddy_communication/GetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationGetAnglesResponse {
static readonly maxWireBytes:i32=52;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=52;
/** Exact native field joint_0, native scalar float32. */
joint_0:f32=0;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field joint_7, native scalar float32. */
joint_7:f32=0;
/** Exact native field joint_8, native scalar float32. */
joint_8:f32=0;
/** Exact native field joint_9, native scalar float32. */
joint_9:f32=0;
/** Exact native field joint_10, native scalar float32. */
joint_10:f32=0;
/** Exact native field joint_11, native scalar float32. */
joint_11:f32=0;
/** Exact native field joint_12, native scalar float32. */
joint_12:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_0))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_1))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_7))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_8))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_9))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_10))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_11))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
{
if(!isFinite(this.joint_12))return -22;
if(4>52-n)return -90;n+=4;
if(n>52)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_0),4);
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>reinterpret<u32>(this.joint_7),4);
w.integer(<u64>reinterpret<u32>(this.joint_8),4);
w.integer(<u64>reinterpret<u32>(this.joint_9),4);
w.integer(<u64>reinterpret<u32>(this.joint_10),4);
w.integer(<u64>reinterpret<u32>(this.joint_11),4);
w.integer(<u64>reinterpret<u32>(this.joint_12),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationGetAnglesResponse{const value=new Ros1Mybuddy_communicationGetAnglesResponse();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_0=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_7=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_8=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_9=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_10=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_11=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_12=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationGetAnglesResponseCodec implements VendorCodec<Ros1Mybuddy_communicationGetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_98085b01a21ae26a74a286d790aab310530fff2a88d7a3c35e071df09abd709d";
static readonly maxWireBytes:i32=52;
encodeChecked(value:Ros1Mybuddy_communicationGetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationGetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationGetAnglesResponse>{const out=new VendorDecoded<Ros1Mybuddy_communicationGetAnglesResponse>();if(bytes.length>52){out.status=90;return out;}if(bytes.length<52){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationGetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationGetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationGetAnglesResponse():changetype<Ros1Mybuddy_communicationGetAnglesResponse>(out.value);}
}
/** Complete original mybuddy_communication/GetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationGetCoordsRequest {
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
static readControl(r:Reader):Ros1Mybuddy_communicationGetCoordsRequest{const value=new Ros1Mybuddy_communicationGetCoordsRequest();
return value;}
}
export class Ros1Mybuddy_communicationGetCoordsRequestCodec implements VendorCodec<Ros1Mybuddy_communicationGetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_e5b4bd6f4d17f79bf3477c2577e73355b871523291aace267fea201f5a2b43ef";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mybuddy_communicationGetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationGetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationGetCoordsRequest>{const out=new VendorDecoded<Ros1Mybuddy_communicationGetCoordsRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationGetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationGetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationGetCoordsRequest():changetype<Ros1Mybuddy_communicationGetCoordsRequest>(out.value);}
}
/** Complete original mybuddy_communication/GetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationGetCoordsResponse {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationGetCoordsResponse{const value=new Ros1Mybuddy_communicationGetCoordsResponse();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationGetCoordsResponseCodec implements VendorCodec<Ros1Mybuddy_communicationGetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_604a9a58b11d6e51faee74226c4820db7668b2054b2e48949cf9deb5958f5873";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mybuddy_communicationGetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationGetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationGetCoordsResponse>{const out=new VendorDecoded<Ros1Mybuddy_communicationGetCoordsResponse>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationGetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationGetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationGetCoordsResponse():changetype<Ros1Mybuddy_communicationGetCoordsResponse>(out.value);}
}
/** Complete original mybuddy_communication/GripperStatusRequest; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationGripperStatusRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationGripperStatusRequest{const value=new Ros1Mybuddy_communicationGripperStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationGripperStatusRequestCodec implements VendorCodec<Ros1Mybuddy_communicationGripperStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_7816f3fa4c8b7068eeb16e74940d2e26401df47b7bc6881d123f94b554523173";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mybuddy_communicationGripperStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationGripperStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationGripperStatusRequest>{const out=new VendorDecoded<Ros1Mybuddy_communicationGripperStatusRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationGripperStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationGripperStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationGripperStatusRequest():changetype<Ros1Mybuddy_communicationGripperStatusRequest>(out.value);}
}
/** Complete original mybuddy_communication/GripperStatusResponse; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationGripperStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationGripperStatusResponse{const value=new Ros1Mybuddy_communicationGripperStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationGripperStatusResponseCodec implements VendorCodec<Ros1Mybuddy_communicationGripperStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_b329fd687ef106ed366d80c5651fc536ad266d2b16dee8f985f1bad32738330b";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mybuddy_communicationGripperStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationGripperStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationGripperStatusResponse>{const out=new VendorDecoded<Ros1Mybuddy_communicationGripperStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationGripperStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationGripperStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationGripperStatusResponse():changetype<Ros1Mybuddy_communicationGripperStatusResponse>(out.value);}
}
/** Complete original mybuddy_communication/PumpStatusRequest; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationPumpStatusRequest {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationPumpStatusRequest{const value=new Ros1Mybuddy_communicationPumpStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationPumpStatusRequestCodec implements VendorCodec<Ros1Mybuddy_communicationPumpStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_4261e7e11f0c6065de868444a0be45d363465b0913246b06b4a2c413714d77e9";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Mybuddy_communicationPumpStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationPumpStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationPumpStatusRequest>{const out=new VendorDecoded<Ros1Mybuddy_communicationPumpStatusRequest>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationPumpStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationPumpStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationPumpStatusRequest():changetype<Ros1Mybuddy_communicationPumpStatusRequest>(out.value);}
}
/** Complete original mybuddy_communication/PumpStatusResponse; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationPumpStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationPumpStatusResponse{const value=new Ros1Mybuddy_communicationPumpStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationPumpStatusResponseCodec implements VendorCodec<Ros1Mybuddy_communicationPumpStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_975f537d88c880f8daa3459f6762dc6ec7d7eee1f580c0e79f3cb581b77cabd4";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mybuddy_communicationPumpStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationPumpStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationPumpStatusResponse>{const out=new VendorDecoded<Ros1Mybuddy_communicationPumpStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationPumpStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationPumpStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationPumpStatusResponse():changetype<Ros1Mybuddy_communicationPumpStatusResponse>(out.value);}
}
/** Complete original mybuddy_communication/SetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationSetAnglesRequest {
static readonly maxWireBytes:i32=53;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=53;
/** Exact native field joint_0, native scalar float32. */
joint_0:f32=0;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field joint_7, native scalar float32. */
joint_7:f32=0;
/** Exact native field joint_8, native scalar float32. */
joint_8:f32=0;
/** Exact native field joint_9, native scalar float32. */
joint_9:f32=0;
/** Exact native field joint_10, native scalar float32. */
joint_10:f32=0;
/** Exact native field joint_11, native scalar float32. */
joint_11:f32=0;
/** Exact native field joint_12, native scalar float32. */
joint_12:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_0))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_1))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_7))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_8))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_9))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_10))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_11))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
}
{
if(!isFinite(this.joint_12))return -22;
if(4>53-n)return -90;n+=4;
if(n>53)return -90;
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
w.integer(<u64>reinterpret<u32>(this.joint_0),4);
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>reinterpret<u32>(this.joint_7),4);
w.integer(<u64>reinterpret<u32>(this.joint_8),4);
w.integer(<u64>reinterpret<u32>(this.joint_9),4);
w.integer(<u64>reinterpret<u32>(this.joint_10),4);
w.integer(<u64>reinterpret<u32>(this.joint_11),4);
w.integer(<u64>reinterpret<u32>(this.joint_12),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationSetAnglesRequest{const value=new Ros1Mybuddy_communicationSetAnglesRequest();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_0=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_7=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_8=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_9=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_10=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_11=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_12=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationSetAnglesRequestCodec implements VendorCodec<Ros1Mybuddy_communicationSetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_47d19578189892ca10e073d8d250839f1e2b39ebbb3525ed9b03a15af18eb240";
static readonly maxWireBytes:i32=53;
encodeChecked(value:Ros1Mybuddy_communicationSetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationSetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationSetAnglesRequest>{const out=new VendorDecoded<Ros1Mybuddy_communicationSetAnglesRequest>();if(bytes.length>53){out.status=90;return out;}if(bytes.length<53){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationSetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationSetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationSetAnglesRequest():changetype<Ros1Mybuddy_communicationSetAnglesRequest>(out.value);}
}
/** Complete original mybuddy_communication/SetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationSetAnglesResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationSetAnglesResponse{const value=new Ros1Mybuddy_communicationSetAnglesResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationSetAnglesResponseCodec implements VendorCodec<Ros1Mybuddy_communicationSetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_7371fa369708af552fd68522d7effdd00bf82bf7e2fe21469ceb210f51582f81";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mybuddy_communicationSetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationSetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationSetAnglesResponse>{const out=new VendorDecoded<Ros1Mybuddy_communicationSetAnglesResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationSetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationSetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationSetAnglesResponse():changetype<Ros1Mybuddy_communicationSetAnglesResponse>(out.value);}
}
/** Complete original mybuddy_communication/SetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationSetCoordsRequest {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationSetCoordsRequest{const value=new Ros1Mybuddy_communicationSetCoordsRequest();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationSetCoordsRequestCodec implements VendorCodec<Ros1Mybuddy_communicationSetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_81f7315bebd612a9406a2805b393415b8a1956e49f3d1b8efb21f9e703b0bfe0";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros1Mybuddy_communicationSetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationSetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationSetCoordsRequest>{const out=new VendorDecoded<Ros1Mybuddy_communicationSetCoordsRequest>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationSetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationSetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationSetCoordsRequest():changetype<Ros1Mybuddy_communicationSetCoordsRequest>(out.value);}
}
/** Complete original mybuddy_communication/SetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mybuddy_communicationSetCoordsResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mybuddy_communicationSetCoordsResponse{const value=new Ros1Mybuddy_communicationSetCoordsResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mybuddy_communicationSetCoordsResponseCodec implements VendorCodec<Ros1Mybuddy_communicationSetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_e5c41ea9420e43a5406ddee23cee961347bfad4f83baf9ebac93d16cdb5d0b21";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mybuddy_communicationSetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mybuddy_communicationSetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mybuddy_communicationSetCoordsResponse>{const out=new VendorDecoded<Ros1Mybuddy_communicationSetCoordsResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mybuddy_communicationSetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mybuddy_communicationSetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mybuddy_communicationSetCoordsResponse():changetype<Ros1Mybuddy_communicationSetCoordsResponse>(out.value);}
}
/** Complete original mecharm_communication/MycobotAngles; spellings are published in generation proof. */
export class Ros1Mecharm_communicationMycobotAngles {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationMycobotAngles{const value=new Ros1Mecharm_communicationMycobotAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationMycobotAnglesCodec implements VendorCodec<Ros1Mecharm_communicationMycobotAngles>{
static readonly codecIdentity:string="UWVMROS1_4afa3dc66ce6ffe02170c76da688514a04aa7b47008aeb8f79c44f9ff867d189";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mecharm_communicationMycobotAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationMycobotAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationMycobotAngles>{const out=new VendorDecoded<Ros1Mecharm_communicationMycobotAngles>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationMycobotAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationMycobotAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationMycobotAngles():changetype<Ros1Mecharm_communicationMycobotAngles>(out.value);}
}
/** Complete original mecharm_communication/MycobotCoords; spellings are published in generation proof. */
export class Ros1Mecharm_communicationMycobotCoords {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationMycobotCoords{const value=new Ros1Mecharm_communicationMycobotCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationMycobotCoordsCodec implements VendorCodec<Ros1Mecharm_communicationMycobotCoords>{
static readonly codecIdentity:string="UWVMROS1_a6a82e22cd0a2f24657c0a253ed00bf31c0a266e6ebaab071cfd369780818d35";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mecharm_communicationMycobotCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationMycobotCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationMycobotCoords>{const out=new VendorDecoded<Ros1Mecharm_communicationMycobotCoords>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationMycobotCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationMycobotCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationMycobotCoords():changetype<Ros1Mecharm_communicationMycobotCoords>(out.value);}
}
/** Complete original mecharm_communication/MycobotGripperStatus; spellings are published in generation proof. */
export class Ros1Mecharm_communicationMycobotGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationMycobotGripperStatus{const value=new Ros1Mecharm_communicationMycobotGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationMycobotGripperStatusCodec implements VendorCodec<Ros1Mecharm_communicationMycobotGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_07e04dad52b423c371dc5432b68a2f4272cf55328d6f0a10f091dc389d57a4bf";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mecharm_communicationMycobotGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationMycobotGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationMycobotGripperStatus>{const out=new VendorDecoded<Ros1Mecharm_communicationMycobotGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationMycobotGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationMycobotGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationMycobotGripperStatus():changetype<Ros1Mecharm_communicationMycobotGripperStatus>(out.value);}
}
/** Complete original mecharm_communication/MycobotPumpStatus; spellings are published in generation proof. */
export class Ros1Mecharm_communicationMycobotPumpStatus {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationMycobotPumpStatus{const value=new Ros1Mecharm_communicationMycobotPumpStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationMycobotPumpStatusCodec implements VendorCodec<Ros1Mecharm_communicationMycobotPumpStatus>{
static readonly codecIdentity:string="UWVMROS1_f559fbf7d5d2c872b3660132eb8c17c79ca3c2d27e975f8fb8fde1d86072a6fa";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Mecharm_communicationMycobotPumpStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationMycobotPumpStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationMycobotPumpStatus>{const out=new VendorDecoded<Ros1Mecharm_communicationMycobotPumpStatus>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationMycobotPumpStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationMycobotPumpStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationMycobotPumpStatus():changetype<Ros1Mecharm_communicationMycobotPumpStatus>(out.value);}
}
/** Complete original mecharm_communication/MycobotSetAngles; spellings are published in generation proof. */
export class Ros1Mecharm_communicationMycobotSetAngles {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
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
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationMycobotSetAngles{const value=new Ros1Mecharm_communicationMycobotSetAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationMycobotSetAnglesCodec implements VendorCodec<Ros1Mecharm_communicationMycobotSetAngles>{
static readonly codecIdentity:string="UWVMROS1_bce09b709e806417595c05e0e795d689b8c53008f9916633d934f935f9389de9";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros1Mecharm_communicationMycobotSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationMycobotSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationMycobotSetAngles>{const out=new VendorDecoded<Ros1Mecharm_communicationMycobotSetAngles>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationMycobotSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationMycobotSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationMycobotSetAngles():changetype<Ros1Mecharm_communicationMycobotSetAngles>(out.value);}
}
/** Complete original mecharm_communication/MycobotSetCoords; spellings are published in generation proof. */
export class Ros1Mecharm_communicationMycobotSetCoords {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationMycobotSetCoords{const value=new Ros1Mecharm_communicationMycobotSetCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationMycobotSetCoordsCodec implements VendorCodec<Ros1Mecharm_communicationMycobotSetCoords>{
static readonly codecIdentity:string="UWVMROS1_68ffa6950e0934b845b2b45140612997c8276a3ac7ed4303e43cd70417491913";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros1Mecharm_communicationMycobotSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationMycobotSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationMycobotSetCoords>{const out=new VendorDecoded<Ros1Mecharm_communicationMycobotSetCoords>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationMycobotSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationMycobotSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationMycobotSetCoords():changetype<Ros1Mecharm_communicationMycobotSetCoords>(out.value);}
}
/** Complete original mecharm_communication/GetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mecharm_communicationGetAnglesRequest {
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
static readControl(r:Reader):Ros1Mecharm_communicationGetAnglesRequest{const value=new Ros1Mecharm_communicationGetAnglesRequest();
return value;}
}
export class Ros1Mecharm_communicationGetAnglesRequestCodec implements VendorCodec<Ros1Mecharm_communicationGetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_6a8f60e71495c9944e1e97c308d5c992674bd46060b6f6abf2df3f9748c13123";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mecharm_communicationGetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationGetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationGetAnglesRequest>{const out=new VendorDecoded<Ros1Mecharm_communicationGetAnglesRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationGetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationGetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationGetAnglesRequest():changetype<Ros1Mecharm_communicationGetAnglesRequest>(out.value);}
}
/** Complete original mecharm_communication/GetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mecharm_communicationGetAnglesResponse {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationGetAnglesResponse{const value=new Ros1Mecharm_communicationGetAnglesResponse();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationGetAnglesResponseCodec implements VendorCodec<Ros1Mecharm_communicationGetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_e792a206903dc49fab725e3b3507af3cf4712f10740e9dcceeb90dedfe6ea6e8";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mecharm_communicationGetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationGetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationGetAnglesResponse>{const out=new VendorDecoded<Ros1Mecharm_communicationGetAnglesResponse>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationGetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationGetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationGetAnglesResponse():changetype<Ros1Mecharm_communicationGetAnglesResponse>(out.value);}
}
/** Complete original mecharm_communication/GetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mecharm_communicationGetCoordsRequest {
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
static readControl(r:Reader):Ros1Mecharm_communicationGetCoordsRequest{const value=new Ros1Mecharm_communicationGetCoordsRequest();
return value;}
}
export class Ros1Mecharm_communicationGetCoordsRequestCodec implements VendorCodec<Ros1Mecharm_communicationGetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_55d4d1b8965ea12f9b15d4bd5a22c43793d04af452fefb84bd98bc3d7d8da8bf";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mecharm_communicationGetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationGetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationGetCoordsRequest>{const out=new VendorDecoded<Ros1Mecharm_communicationGetCoordsRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationGetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationGetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationGetCoordsRequest():changetype<Ros1Mecharm_communicationGetCoordsRequest>(out.value);}
}
/** Complete original mecharm_communication/GetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mecharm_communicationGetCoordsResponse {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationGetCoordsResponse{const value=new Ros1Mecharm_communicationGetCoordsResponse();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationGetCoordsResponseCodec implements VendorCodec<Ros1Mecharm_communicationGetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_2230e517685ea625507334067ccceb8424ecf782fcd4163c551d09048e900ca8";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mecharm_communicationGetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationGetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationGetCoordsResponse>{const out=new VendorDecoded<Ros1Mecharm_communicationGetCoordsResponse>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationGetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationGetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationGetCoordsResponse():changetype<Ros1Mecharm_communicationGetCoordsResponse>(out.value);}
}
/** Complete original mecharm_communication/GripperStatusRequest; spellings are published in generation proof. */
export class Ros1Mecharm_communicationGripperStatusRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationGripperStatusRequest{const value=new Ros1Mecharm_communicationGripperStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationGripperStatusRequestCodec implements VendorCodec<Ros1Mecharm_communicationGripperStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_98284fbac3bdb61857b9175b1f82adc7265d0ed418088123d79bf75cb04c9a2c";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mecharm_communicationGripperStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationGripperStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationGripperStatusRequest>{const out=new VendorDecoded<Ros1Mecharm_communicationGripperStatusRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationGripperStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationGripperStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationGripperStatusRequest():changetype<Ros1Mecharm_communicationGripperStatusRequest>(out.value);}
}
/** Complete original mecharm_communication/GripperStatusResponse; spellings are published in generation proof. */
export class Ros1Mecharm_communicationGripperStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationGripperStatusResponse{const value=new Ros1Mecharm_communicationGripperStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationGripperStatusResponseCodec implements VendorCodec<Ros1Mecharm_communicationGripperStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_29cfb0267fbe68d5dafcd3c9d16a8d4dab6b21477921a48ba35135bc0c22bb32";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mecharm_communicationGripperStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationGripperStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationGripperStatusResponse>{const out=new VendorDecoded<Ros1Mecharm_communicationGripperStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationGripperStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationGripperStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationGripperStatusResponse():changetype<Ros1Mecharm_communicationGripperStatusResponse>(out.value);}
}
/** Complete original mecharm_communication/PumpStatusRequest; spellings are published in generation proof. */
export class Ros1Mecharm_communicationPumpStatusRequest {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationPumpStatusRequest{const value=new Ros1Mecharm_communicationPumpStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationPumpStatusRequestCodec implements VendorCodec<Ros1Mecharm_communicationPumpStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_aa138db91e46b41659dfc5901225aaaae1c1ae0461c35d3f7a90d0cf86c9920d";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Mecharm_communicationPumpStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationPumpStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationPumpStatusRequest>{const out=new VendorDecoded<Ros1Mecharm_communicationPumpStatusRequest>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationPumpStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationPumpStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationPumpStatusRequest():changetype<Ros1Mecharm_communicationPumpStatusRequest>(out.value);}
}
/** Complete original mecharm_communication/PumpStatusResponse; spellings are published in generation proof. */
export class Ros1Mecharm_communicationPumpStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationPumpStatusResponse{const value=new Ros1Mecharm_communicationPumpStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationPumpStatusResponseCodec implements VendorCodec<Ros1Mecharm_communicationPumpStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_876caa88c833bfa180b8747110683c789aa238c0cda162a8e262c66fddefdd8f";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mecharm_communicationPumpStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationPumpStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationPumpStatusResponse>{const out=new VendorDecoded<Ros1Mecharm_communicationPumpStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationPumpStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationPumpStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationPumpStatusResponse():changetype<Ros1Mecharm_communicationPumpStatusResponse>(out.value);}
}
/** Complete original mecharm_communication/SetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mecharm_communicationSetAnglesRequest {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
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
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationSetAnglesRequest{const value=new Ros1Mecharm_communicationSetAnglesRequest();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationSetAnglesRequestCodec implements VendorCodec<Ros1Mecharm_communicationSetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_7ad54edf4730457f2b2124c925b87a99bdbeddd8bfcc1e0d806f7b9dad31507b";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros1Mecharm_communicationSetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationSetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationSetAnglesRequest>{const out=new VendorDecoded<Ros1Mecharm_communicationSetAnglesRequest>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationSetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationSetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationSetAnglesRequest():changetype<Ros1Mecharm_communicationSetAnglesRequest>(out.value);}
}
/** Complete original mecharm_communication/SetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mecharm_communicationSetAnglesResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationSetAnglesResponse{const value=new Ros1Mecharm_communicationSetAnglesResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationSetAnglesResponseCodec implements VendorCodec<Ros1Mecharm_communicationSetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_940a294ef91d04afeb65b88bbdd91df3a7db0a07435ced3993db3a9d0f574c6e";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mecharm_communicationSetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationSetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationSetAnglesResponse>{const out=new VendorDecoded<Ros1Mecharm_communicationSetAnglesResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationSetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationSetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationSetAnglesResponse():changetype<Ros1Mecharm_communicationSetAnglesResponse>(out.value);}
}
/** Complete original mecharm_communication/SetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mecharm_communicationSetCoordsRequest {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationSetCoordsRequest{const value=new Ros1Mecharm_communicationSetCoordsRequest();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationSetCoordsRequestCodec implements VendorCodec<Ros1Mecharm_communicationSetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_68deaf663a1fe2d3d7434935f10161eb444e7ea2f79e4b746fee9bd0383fc929";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros1Mecharm_communicationSetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationSetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationSetCoordsRequest>{const out=new VendorDecoded<Ros1Mecharm_communicationSetCoordsRequest>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationSetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationSetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationSetCoordsRequest():changetype<Ros1Mecharm_communicationSetCoordsRequest>(out.value);}
}
/** Complete original mecharm_communication/SetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mecharm_communicationSetCoordsResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mecharm_communicationSetCoordsResponse{const value=new Ros1Mecharm_communicationSetCoordsResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mecharm_communicationSetCoordsResponseCodec implements VendorCodec<Ros1Mecharm_communicationSetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_c66a6e14c4c7eba1b52d7ef57802cb6122a120f692bcc1552c51078301b1b409";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mecharm_communicationSetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mecharm_communicationSetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mecharm_communicationSetCoordsResponse>{const out=new VendorDecoded<Ros1Mecharm_communicationSetCoordsResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mecharm_communicationSetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mecharm_communicationSetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mecharm_communicationSetCoordsResponse():changetype<Ros1Mecharm_communicationSetCoordsResponse>(out.value);}
}
/** Complete original myarm_communication/MyArmAngles; spellings are published in generation proof. */
export class Ros1Myarm_communicationMyArmAngles {
static readonly maxWireBytes:i32=28;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=28;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field joint_7, native scalar float32. */
joint_7:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_7))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>reinterpret<u32>(this.joint_7),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationMyArmAngles{const value=new Ros1Myarm_communicationMyArmAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_7=decoded;
}
return value;}
}
export class Ros1Myarm_communicationMyArmAnglesCodec implements VendorCodec<Ros1Myarm_communicationMyArmAngles>{
static readonly codecIdentity:string="UWVMROS1_eff11a03e974b4af030c2dc92d0111cf95e0fb3ce004235bada3fe358b63fd4c";
static readonly maxWireBytes:i32=28;
encodeChecked(value:Ros1Myarm_communicationMyArmAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationMyArmAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationMyArmAngles>{const out=new VendorDecoded<Ros1Myarm_communicationMyArmAngles>();if(bytes.length>28){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationMyArmAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationMyArmAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationMyArmAngles():changetype<Ros1Myarm_communicationMyArmAngles>(out.value);}
}
/** Complete original myarm_communication/MyArmCoords; spellings are published in generation proof. */
export class Ros1Myarm_communicationMyArmCoords {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationMyArmCoords{const value=new Ros1Myarm_communicationMyArmCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Myarm_communicationMyArmCoordsCodec implements VendorCodec<Ros1Myarm_communicationMyArmCoords>{
static readonly codecIdentity:string="UWVMROS1_f0df1605988924f0263805fb2b27ab7ef9e2296691ed15e03a337606362c983c";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Myarm_communicationMyArmCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationMyArmCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationMyArmCoords>{const out=new VendorDecoded<Ros1Myarm_communicationMyArmCoords>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationMyArmCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationMyArmCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationMyArmCoords():changetype<Ros1Myarm_communicationMyArmCoords>(out.value);}
}
/** Complete original myarm_communication/MyArmGripperStatus; spellings are published in generation proof. */
export class Ros1Myarm_communicationMyArmGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationMyArmGripperStatus{const value=new Ros1Myarm_communicationMyArmGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Myarm_communicationMyArmGripperStatusCodec implements VendorCodec<Ros1Myarm_communicationMyArmGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_7875df5b50417da1cef60d79c872aff0b40a08f9bbfe80c103729d3c4c892c6f";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Myarm_communicationMyArmGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationMyArmGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationMyArmGripperStatus>{const out=new VendorDecoded<Ros1Myarm_communicationMyArmGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationMyArmGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationMyArmGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationMyArmGripperStatus():changetype<Ros1Myarm_communicationMyArmGripperStatus>(out.value);}
}
/** Complete original myarm_communication/MyArmPumpStatus; spellings are published in generation proof. */
export class Ros1Myarm_communicationMyArmPumpStatus {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationMyArmPumpStatus{const value=new Ros1Myarm_communicationMyArmPumpStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Myarm_communicationMyArmPumpStatusCodec implements VendorCodec<Ros1Myarm_communicationMyArmPumpStatus>{
static readonly codecIdentity:string="UWVMROS1_58765b94f1ecb3bb131f6480ea6830631cde9c96eeef94fd9b22598ccc9ea928";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Myarm_communicationMyArmPumpStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationMyArmPumpStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationMyArmPumpStatus>{const out=new VendorDecoded<Ros1Myarm_communicationMyArmPumpStatus>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationMyArmPumpStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationMyArmPumpStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationMyArmPumpStatus():changetype<Ros1Myarm_communicationMyArmPumpStatus>(out.value);}
}
/** Complete original myarm_communication/MyArmSetAngles; spellings are published in generation proof. */
export class Ros1Myarm_communicationMyArmSetAngles {
static readonly maxWireBytes:i32=29;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=29;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field joint_7, native scalar float32. */
joint_7:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_7))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(1>29-n)return -90;n+=1;
if(n>29)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>reinterpret<u32>(this.joint_7),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationMyArmSetAngles{const value=new Ros1Myarm_communicationMyArmSetAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_7=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Myarm_communicationMyArmSetAnglesCodec implements VendorCodec<Ros1Myarm_communicationMyArmSetAngles>{
static readonly codecIdentity:string="UWVMROS1_6acbfd19e3864b66e47b9da5c7c0e8890e7336e8d6c9a5657c4f5eaa5285fe42";
static readonly maxWireBytes:i32=29;
encodeChecked(value:Ros1Myarm_communicationMyArmSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationMyArmSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationMyArmSetAngles>{const out=new VendorDecoded<Ros1Myarm_communicationMyArmSetAngles>();if(bytes.length>29){out.status=90;return out;}if(bytes.length<29){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationMyArmSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationMyArmSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationMyArmSetAngles():changetype<Ros1Myarm_communicationMyArmSetAngles>(out.value);}
}
/** Complete original myarm_communication/MyArmSetCoords; spellings are published in generation proof. */
export class Ros1Myarm_communicationMyArmSetCoords {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationMyArmSetCoords{const value=new Ros1Myarm_communicationMyArmSetCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Myarm_communicationMyArmSetCoordsCodec implements VendorCodec<Ros1Myarm_communicationMyArmSetCoords>{
static readonly codecIdentity:string="UWVMROS1_ce10c29be4cfd832d5076fead55817a0ac7ccde002d5bbbd7a2befd95e9752fd";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros1Myarm_communicationMyArmSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationMyArmSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationMyArmSetCoords>{const out=new VendorDecoded<Ros1Myarm_communicationMyArmSetCoords>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationMyArmSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationMyArmSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationMyArmSetCoords():changetype<Ros1Myarm_communicationMyArmSetCoords>(out.value);}
}
/** Complete original myarm_communication/GetAnglesRequest; spellings are published in generation proof. */
export class Ros1Myarm_communicationGetAnglesRequest {
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
static readControl(r:Reader):Ros1Myarm_communicationGetAnglesRequest{const value=new Ros1Myarm_communicationGetAnglesRequest();
return value;}
}
export class Ros1Myarm_communicationGetAnglesRequestCodec implements VendorCodec<Ros1Myarm_communicationGetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_18ea682d1c475321b67435e9f207a14627113ad237a38c759cfe4e1c92fded37";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Myarm_communicationGetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationGetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationGetAnglesRequest>{const out=new VendorDecoded<Ros1Myarm_communicationGetAnglesRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationGetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationGetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationGetAnglesRequest():changetype<Ros1Myarm_communicationGetAnglesRequest>(out.value);}
}
/** Complete original myarm_communication/GetAnglesResponse; spellings are published in generation proof. */
export class Ros1Myarm_communicationGetAnglesResponse {
static readonly maxWireBytes:i32=28;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=28;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field joint_7, native scalar float32. */
joint_7:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
{
if(!isFinite(this.joint_7))return -22;
if(4>28-n)return -90;n+=4;
if(n>28)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>reinterpret<u32>(this.joint_7),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationGetAnglesResponse{const value=new Ros1Myarm_communicationGetAnglesResponse();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_7=decoded;
}
return value;}
}
export class Ros1Myarm_communicationGetAnglesResponseCodec implements VendorCodec<Ros1Myarm_communicationGetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_0c68b293384d21d06e23d4aba10b4a4c2eff4e1a0eaa81903e026fe28da410c7";
static readonly maxWireBytes:i32=28;
encodeChecked(value:Ros1Myarm_communicationGetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationGetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationGetAnglesResponse>{const out=new VendorDecoded<Ros1Myarm_communicationGetAnglesResponse>();if(bytes.length>28){out.status=90;return out;}if(bytes.length<28){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationGetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationGetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationGetAnglesResponse():changetype<Ros1Myarm_communicationGetAnglesResponse>(out.value);}
}
/** Complete original myarm_communication/GetCoordsRequest; spellings are published in generation proof. */
export class Ros1Myarm_communicationGetCoordsRequest {
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
static readControl(r:Reader):Ros1Myarm_communicationGetCoordsRequest{const value=new Ros1Myarm_communicationGetCoordsRequest();
return value;}
}
export class Ros1Myarm_communicationGetCoordsRequestCodec implements VendorCodec<Ros1Myarm_communicationGetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_04d9d79ad983cf4631cf65a48cae60d982cd0f234f0f4ed1afa0cc630bd63d95";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Myarm_communicationGetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationGetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationGetCoordsRequest>{const out=new VendorDecoded<Ros1Myarm_communicationGetCoordsRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationGetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationGetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationGetCoordsRequest():changetype<Ros1Myarm_communicationGetCoordsRequest>(out.value);}
}
/** Complete original myarm_communication/GetCoordsResponse; spellings are published in generation proof. */
export class Ros1Myarm_communicationGetCoordsResponse {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationGetCoordsResponse{const value=new Ros1Myarm_communicationGetCoordsResponse();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Myarm_communicationGetCoordsResponseCodec implements VendorCodec<Ros1Myarm_communicationGetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_752564704c08fb2076df8eea4f6f5b89621041372472f0f99b6627fdfda831b7";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Myarm_communicationGetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationGetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationGetCoordsResponse>{const out=new VendorDecoded<Ros1Myarm_communicationGetCoordsResponse>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationGetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationGetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationGetCoordsResponse():changetype<Ros1Myarm_communicationGetCoordsResponse>(out.value);}
}
/** Complete original myarm_communication/GripperStatusRequest; spellings are published in generation proof. */
export class Ros1Myarm_communicationGripperStatusRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationGripperStatusRequest{const value=new Ros1Myarm_communicationGripperStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Myarm_communicationGripperStatusRequestCodec implements VendorCodec<Ros1Myarm_communicationGripperStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_adec2e0fbf48862c2245aea4c31c59ea12abdec26b4727b14005b65006c58976";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Myarm_communicationGripperStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationGripperStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationGripperStatusRequest>{const out=new VendorDecoded<Ros1Myarm_communicationGripperStatusRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationGripperStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationGripperStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationGripperStatusRequest():changetype<Ros1Myarm_communicationGripperStatusRequest>(out.value);}
}
/** Complete original myarm_communication/GripperStatusResponse; spellings are published in generation proof. */
export class Ros1Myarm_communicationGripperStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationGripperStatusResponse{const value=new Ros1Myarm_communicationGripperStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Myarm_communicationGripperStatusResponseCodec implements VendorCodec<Ros1Myarm_communicationGripperStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_d703a0ec6f562f1deb444313e36e6a728b4e521d1196d90a63725753d5fe9659";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Myarm_communicationGripperStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationGripperStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationGripperStatusResponse>{const out=new VendorDecoded<Ros1Myarm_communicationGripperStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationGripperStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationGripperStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationGripperStatusResponse():changetype<Ros1Myarm_communicationGripperStatusResponse>(out.value);}
}
/** Complete original myarm_communication/PumpStatusRequest; spellings are published in generation proof. */
export class Ros1Myarm_communicationPumpStatusRequest {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationPumpStatusRequest{const value=new Ros1Myarm_communicationPumpStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Myarm_communicationPumpStatusRequestCodec implements VendorCodec<Ros1Myarm_communicationPumpStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_eac2f5c006a0fa86df638c52e9cfb7d2c17818ec10e32ad5277af216c6c944f4";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Myarm_communicationPumpStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationPumpStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationPumpStatusRequest>{const out=new VendorDecoded<Ros1Myarm_communicationPumpStatusRequest>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationPumpStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationPumpStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationPumpStatusRequest():changetype<Ros1Myarm_communicationPumpStatusRequest>(out.value);}
}
/** Complete original myarm_communication/PumpStatusResponse; spellings are published in generation proof. */
export class Ros1Myarm_communicationPumpStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationPumpStatusResponse{const value=new Ros1Myarm_communicationPumpStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Myarm_communicationPumpStatusResponseCodec implements VendorCodec<Ros1Myarm_communicationPumpStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_28533ec27c01d85c9a4644959ea8e5c3f2480f5203a9545e96e4242158f08044";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Myarm_communicationPumpStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationPumpStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationPumpStatusResponse>{const out=new VendorDecoded<Ros1Myarm_communicationPumpStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationPumpStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationPumpStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationPumpStatusResponse():changetype<Ros1Myarm_communicationPumpStatusResponse>(out.value);}
}
/** Complete original myarm_communication/SetAnglesRequest; spellings are published in generation proof. */
export class Ros1Myarm_communicationSetAnglesRequest {
static readonly maxWireBytes:i32=29;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=29;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field joint_7, native scalar float32. */
joint_7:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(!isFinite(this.joint_7))return -22;
if(4>29-n)return -90;n+=4;
if(n>29)return -90;
}
{
if(1>29-n)return -90;n+=1;
if(n>29)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>reinterpret<u32>(this.joint_7),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationSetAnglesRequest{const value=new Ros1Myarm_communicationSetAnglesRequest();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_7=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Myarm_communicationSetAnglesRequestCodec implements VendorCodec<Ros1Myarm_communicationSetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_c15c81907fbace209686a98578019d893d86cecf4782427ac2c3858da8cd3e0a";
static readonly maxWireBytes:i32=29;
encodeChecked(value:Ros1Myarm_communicationSetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationSetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationSetAnglesRequest>{const out=new VendorDecoded<Ros1Myarm_communicationSetAnglesRequest>();if(bytes.length>29){out.status=90;return out;}if(bytes.length<29){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationSetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationSetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationSetAnglesRequest():changetype<Ros1Myarm_communicationSetAnglesRequest>(out.value);}
}
/** Complete original myarm_communication/SetAnglesResponse; spellings are published in generation proof. */
export class Ros1Myarm_communicationSetAnglesResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationSetAnglesResponse{const value=new Ros1Myarm_communicationSetAnglesResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Myarm_communicationSetAnglesResponseCodec implements VendorCodec<Ros1Myarm_communicationSetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_dbdbcb001f0478a4bba5ae9d7320d8dbfa4e36a5c1c130215ff5ab864c73af0f";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Myarm_communicationSetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationSetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationSetAnglesResponse>{const out=new VendorDecoded<Ros1Myarm_communicationSetAnglesResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationSetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationSetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationSetAnglesResponse():changetype<Ros1Myarm_communicationSetAnglesResponse>(out.value);}
}
/** Complete original myarm_communication/SetCoordsRequest; spellings are published in generation proof. */
export class Ros1Myarm_communicationSetCoordsRequest {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationSetCoordsRequest{const value=new Ros1Myarm_communicationSetCoordsRequest();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Myarm_communicationSetCoordsRequestCodec implements VendorCodec<Ros1Myarm_communicationSetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_86cfc6edb6b86f90a21818e824aea50b697e314687472e1e96e9ce5399ef66c6";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros1Myarm_communicationSetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationSetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationSetCoordsRequest>{const out=new VendorDecoded<Ros1Myarm_communicationSetCoordsRequest>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationSetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationSetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationSetCoordsRequest():changetype<Ros1Myarm_communicationSetCoordsRequest>(out.value);}
}
/** Complete original myarm_communication/SetCoordsResponse; spellings are published in generation proof. */
export class Ros1Myarm_communicationSetCoordsResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Myarm_communicationSetCoordsResponse{const value=new Ros1Myarm_communicationSetCoordsResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Myarm_communicationSetCoordsResponseCodec implements VendorCodec<Ros1Myarm_communicationSetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_52a54371663141d4384dae8969b729baf097a0aa6105f263e78593743e44272d";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Myarm_communicationSetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Myarm_communicationSetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Myarm_communicationSetCoordsResponse>{const out=new VendorDecoded<Ros1Myarm_communicationSetCoordsResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Myarm_communicationSetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Myarm_communicationSetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Myarm_communicationSetCoordsResponse():changetype<Ros1Myarm_communicationSetCoordsResponse>(out.value);}
}
/** Complete original mycobot_320_communication/MycobotAngles; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationMycobotAngles {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationMycobotAngles{const value=new Ros1Mycobot_320_communicationMycobotAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationMycobotAnglesCodec implements VendorCodec<Ros1Mycobot_320_communicationMycobotAngles>{
static readonly codecIdentity:string="UWVMROS1_67ec61da0c2bf7ee461c21abbfcab14d74b84cb6c9a12430e2a306abdc65690a";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_320_communicationMycobotAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationMycobotAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationMycobotAngles>{const out=new VendorDecoded<Ros1Mycobot_320_communicationMycobotAngles>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationMycobotAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationMycobotAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationMycobotAngles():changetype<Ros1Mycobot_320_communicationMycobotAngles>(out.value);}
}
/** Complete original mycobot_320_communication/MycobotCoords; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationMycobotCoords {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationMycobotCoords{const value=new Ros1Mycobot_320_communicationMycobotCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationMycobotCoordsCodec implements VendorCodec<Ros1Mycobot_320_communicationMycobotCoords>{
static readonly codecIdentity:string="UWVMROS1_1f2ada8331783764a52ed35c81c3ce9a2fdd260e2dffa2990751ce5156c3ad03";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_320_communicationMycobotCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationMycobotCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationMycobotCoords>{const out=new VendorDecoded<Ros1Mycobot_320_communicationMycobotCoords>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationMycobotCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationMycobotCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationMycobotCoords():changetype<Ros1Mycobot_320_communicationMycobotCoords>(out.value);}
}
/** Complete original mycobot_320_communication/MycobotGripperStatus; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationMycobotGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationMycobotGripperStatus{const value=new Ros1Mycobot_320_communicationMycobotGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationMycobotGripperStatusCodec implements VendorCodec<Ros1Mycobot_320_communicationMycobotGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_efb4ccdb3ce35d67174700f3d3dd7332d0f7a834c5d76b6153e389e031907984";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_320_communicationMycobotGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationMycobotGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationMycobotGripperStatus>{const out=new VendorDecoded<Ros1Mycobot_320_communicationMycobotGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationMycobotGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationMycobotGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationMycobotGripperStatus():changetype<Ros1Mycobot_320_communicationMycobotGripperStatus>(out.value);}
}
/** Complete original mycobot_320_communication/MycobotPumpStatus; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationMycobotPumpStatus {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationMycobotPumpStatus{const value=new Ros1Mycobot_320_communicationMycobotPumpStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationMycobotPumpStatusCodec implements VendorCodec<Ros1Mycobot_320_communicationMycobotPumpStatus>{
static readonly codecIdentity:string="UWVMROS1_bb1d48067f44b7022820a6175afe05d5ba8caed9f750874a4e703389cb0d83f4";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Mycobot_320_communicationMycobotPumpStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationMycobotPumpStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationMycobotPumpStatus>{const out=new VendorDecoded<Ros1Mycobot_320_communicationMycobotPumpStatus>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationMycobotPumpStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationMycobotPumpStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationMycobotPumpStatus():changetype<Ros1Mycobot_320_communicationMycobotPumpStatus>(out.value);}
}
/** Complete original mycobot_320_communication/MycobotSetAngles; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationMycobotSetAngles {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
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
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationMycobotSetAngles{const value=new Ros1Mycobot_320_communicationMycobotSetAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationMycobotSetAnglesCodec implements VendorCodec<Ros1Mycobot_320_communicationMycobotSetAngles>{
static readonly codecIdentity:string="UWVMROS1_bf04b1bd24ab2f56c1835d1abe44b73fe023407c3ebdd3d35341242116a5d259";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros1Mycobot_320_communicationMycobotSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationMycobotSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationMycobotSetAngles>{const out=new VendorDecoded<Ros1Mycobot_320_communicationMycobotSetAngles>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationMycobotSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationMycobotSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationMycobotSetAngles():changetype<Ros1Mycobot_320_communicationMycobotSetAngles>(out.value);}
}
/** Complete original mycobot_320_communication/MycobotSetCoords; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationMycobotSetCoords {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationMycobotSetCoords{const value=new Ros1Mycobot_320_communicationMycobotSetCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationMycobotSetCoordsCodec implements VendorCodec<Ros1Mycobot_320_communicationMycobotSetCoords>{
static readonly codecIdentity:string="UWVMROS1_761235ef96bcc1c687ad8dd3f6d39982d1b0a7a24eaab3bb54feb9780b9e0092";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros1Mycobot_320_communicationMycobotSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationMycobotSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationMycobotSetCoords>{const out=new VendorDecoded<Ros1Mycobot_320_communicationMycobotSetCoords>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationMycobotSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationMycobotSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationMycobotSetCoords():changetype<Ros1Mycobot_320_communicationMycobotSetCoords>(out.value);}
}
/** Complete original mycobot_320_communication/GetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationGetAnglesRequest {
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
static readControl(r:Reader):Ros1Mycobot_320_communicationGetAnglesRequest{const value=new Ros1Mycobot_320_communicationGetAnglesRequest();
return value;}
}
export class Ros1Mycobot_320_communicationGetAnglesRequestCodec implements VendorCodec<Ros1Mycobot_320_communicationGetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_fe13e75d9def5e6f7f831297c771cd4d319e1e9862b5026382fed23747b721b8";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mycobot_320_communicationGetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationGetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationGetAnglesRequest>{const out=new VendorDecoded<Ros1Mycobot_320_communicationGetAnglesRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationGetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationGetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationGetAnglesRequest():changetype<Ros1Mycobot_320_communicationGetAnglesRequest>(out.value);}
}
/** Complete original mycobot_320_communication/GetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationGetAnglesResponse {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationGetAnglesResponse{const value=new Ros1Mycobot_320_communicationGetAnglesResponse();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationGetAnglesResponseCodec implements VendorCodec<Ros1Mycobot_320_communicationGetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_c0cb03f21add6e566a30129ed7ce3174fa86786859c472a848c6768678adc80a";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_320_communicationGetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationGetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationGetAnglesResponse>{const out=new VendorDecoded<Ros1Mycobot_320_communicationGetAnglesResponse>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationGetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationGetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationGetAnglesResponse():changetype<Ros1Mycobot_320_communicationGetAnglesResponse>(out.value);}
}
/** Complete original mycobot_320_communication/GetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationGetCoordsRequest {
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
static readControl(r:Reader):Ros1Mycobot_320_communicationGetCoordsRequest{const value=new Ros1Mycobot_320_communicationGetCoordsRequest();
return value;}
}
export class Ros1Mycobot_320_communicationGetCoordsRequestCodec implements VendorCodec<Ros1Mycobot_320_communicationGetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_82b2ae7ba4bf2aff8cd6c88ae344b376604624612abdd633c930695c1d1650d6";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mycobot_320_communicationGetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationGetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationGetCoordsRequest>{const out=new VendorDecoded<Ros1Mycobot_320_communicationGetCoordsRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationGetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationGetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationGetCoordsRequest():changetype<Ros1Mycobot_320_communicationGetCoordsRequest>(out.value);}
}
/** Complete original mycobot_320_communication/GetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationGetCoordsResponse {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationGetCoordsResponse{const value=new Ros1Mycobot_320_communicationGetCoordsResponse();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationGetCoordsResponseCodec implements VendorCodec<Ros1Mycobot_320_communicationGetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_821bac75a42a37fa06f51e57c09839f3557487dc8e883310601cb2e00bb6b05d";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_320_communicationGetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationGetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationGetCoordsResponse>{const out=new VendorDecoded<Ros1Mycobot_320_communicationGetCoordsResponse>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationGetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationGetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationGetCoordsResponse():changetype<Ros1Mycobot_320_communicationGetCoordsResponse>(out.value);}
}
/** Complete original mycobot_320_communication/GripperStatusRequest; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationGripperStatusRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationGripperStatusRequest{const value=new Ros1Mycobot_320_communicationGripperStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationGripperStatusRequestCodec implements VendorCodec<Ros1Mycobot_320_communicationGripperStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_68d072c047f4f026459309282a40c78c5db51571126efb85b7ca9e9f121b5eb5";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_320_communicationGripperStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationGripperStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationGripperStatusRequest>{const out=new VendorDecoded<Ros1Mycobot_320_communicationGripperStatusRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationGripperStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationGripperStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationGripperStatusRequest():changetype<Ros1Mycobot_320_communicationGripperStatusRequest>(out.value);}
}
/** Complete original mycobot_320_communication/GripperStatusResponse; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationGripperStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationGripperStatusResponse{const value=new Ros1Mycobot_320_communicationGripperStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationGripperStatusResponseCodec implements VendorCodec<Ros1Mycobot_320_communicationGripperStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_9c1387ec1a93d6674ac6dd735b2811957db57359e9e461dcedc70ae6fccc5544";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_320_communicationGripperStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationGripperStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationGripperStatusResponse>{const out=new VendorDecoded<Ros1Mycobot_320_communicationGripperStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationGripperStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationGripperStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationGripperStatusResponse():changetype<Ros1Mycobot_320_communicationGripperStatusResponse>(out.value);}
}
/** Complete original mycobot_320_communication/PumpStatusRequest; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationPumpStatusRequest {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationPumpStatusRequest{const value=new Ros1Mycobot_320_communicationPumpStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationPumpStatusRequestCodec implements VendorCodec<Ros1Mycobot_320_communicationPumpStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_23a6e68b36341b30ab13026cbddfd01b16d352105bb5faecd7c1ec6ad654ab60";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Mycobot_320_communicationPumpStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationPumpStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationPumpStatusRequest>{const out=new VendorDecoded<Ros1Mycobot_320_communicationPumpStatusRequest>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationPumpStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationPumpStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationPumpStatusRequest():changetype<Ros1Mycobot_320_communicationPumpStatusRequest>(out.value);}
}
/** Complete original mycobot_320_communication/PumpStatusResponse; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationPumpStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationPumpStatusResponse{const value=new Ros1Mycobot_320_communicationPumpStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationPumpStatusResponseCodec implements VendorCodec<Ros1Mycobot_320_communicationPumpStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_037f53ebd0750c76a798ff17507759fcf71aa8a55496e5b151b8d977dff2c1c8";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_320_communicationPumpStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationPumpStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationPumpStatusResponse>{const out=new VendorDecoded<Ros1Mycobot_320_communicationPumpStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationPumpStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationPumpStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationPumpStatusResponse():changetype<Ros1Mycobot_320_communicationPumpStatusResponse>(out.value);}
}
/** Complete original mycobot_320_communication/SetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationSetAnglesRequest {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
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
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationSetAnglesRequest{const value=new Ros1Mycobot_320_communicationSetAnglesRequest();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationSetAnglesRequestCodec implements VendorCodec<Ros1Mycobot_320_communicationSetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_644a7c29bae881d5f29708b552dcd8fd4102289aa396614590bca33dbda9f7da";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros1Mycobot_320_communicationSetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationSetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationSetAnglesRequest>{const out=new VendorDecoded<Ros1Mycobot_320_communicationSetAnglesRequest>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationSetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationSetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationSetAnglesRequest():changetype<Ros1Mycobot_320_communicationSetAnglesRequest>(out.value);}
}
/** Complete original mycobot_320_communication/SetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationSetAnglesResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationSetAnglesResponse{const value=new Ros1Mycobot_320_communicationSetAnglesResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationSetAnglesResponseCodec implements VendorCodec<Ros1Mycobot_320_communicationSetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_57578e8427fc6a3a89aa595259e03c3498153482afd716dd44c0d09ee9f59f20";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_320_communicationSetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationSetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationSetAnglesResponse>{const out=new VendorDecoded<Ros1Mycobot_320_communicationSetAnglesResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationSetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationSetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationSetAnglesResponse():changetype<Ros1Mycobot_320_communicationSetAnglesResponse>(out.value);}
}
/** Complete original mycobot_320_communication/SetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationSetCoordsRequest {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationSetCoordsRequest{const value=new Ros1Mycobot_320_communicationSetCoordsRequest();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationSetCoordsRequestCodec implements VendorCodec<Ros1Mycobot_320_communicationSetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_f398444b45a64155c410a44da67043cee13993d75beabd6bc104b23ccd1d6656";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros1Mycobot_320_communicationSetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationSetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationSetCoordsRequest>{const out=new VendorDecoded<Ros1Mycobot_320_communicationSetCoordsRequest>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationSetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationSetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationSetCoordsRequest():changetype<Ros1Mycobot_320_communicationSetCoordsRequest>(out.value);}
}
/** Complete original mycobot_320_communication/SetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mycobot_320_communicationSetCoordsResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_320_communicationSetCoordsResponse{const value=new Ros1Mycobot_320_communicationSetCoordsResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_320_communicationSetCoordsResponseCodec implements VendorCodec<Ros1Mycobot_320_communicationSetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_3fef1bfe9036381f8ad45bc1490e96b4917df5689ccf3a84d66e5ba5145e56c4";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_320_communicationSetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_320_communicationSetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_320_communicationSetCoordsResponse>{const out=new VendorDecoded<Ros1Mycobot_320_communicationSetCoordsResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_320_communicationSetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_320_communicationSetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_320_communicationSetCoordsResponse():changetype<Ros1Mycobot_320_communicationSetCoordsResponse>(out.value);}
}
/** Complete original mycobot_communication/MycobotAngles; spellings are published in generation proof. */
export class Ros1Mycobot_communicationMycobotAngles {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationMycobotAngles{const value=new Ros1Mycobot_communicationMycobotAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationMycobotAnglesCodec implements VendorCodec<Ros1Mycobot_communicationMycobotAngles>{
static readonly codecIdentity:string="UWVMROS1_4cbda93a13d78e9c592327caa737dfecf33adf6db898015afd56ecf7732f9aa6";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_communicationMycobotAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationMycobotAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationMycobotAngles>{const out=new VendorDecoded<Ros1Mycobot_communicationMycobotAngles>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationMycobotAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationMycobotAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationMycobotAngles():changetype<Ros1Mycobot_communicationMycobotAngles>(out.value);}
}
/** Complete original mycobot_communication/MycobotCoords; spellings are published in generation proof. */
export class Ros1Mycobot_communicationMycobotCoords {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationMycobotCoords{const value=new Ros1Mycobot_communicationMycobotCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationMycobotCoordsCodec implements VendorCodec<Ros1Mycobot_communicationMycobotCoords>{
static readonly codecIdentity:string="UWVMROS1_1da0d6342a1f4079244a65ad413d4559047d33b049ce9da04f8ab0bd226410a2";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_communicationMycobotCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationMycobotCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationMycobotCoords>{const out=new VendorDecoded<Ros1Mycobot_communicationMycobotCoords>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationMycobotCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationMycobotCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationMycobotCoords():changetype<Ros1Mycobot_communicationMycobotCoords>(out.value);}
}
/** Complete original mycobot_communication/MycobotGetGripperValue; spellings are published in generation proof. */
export class Ros1Mycobot_communicationMycobotGetGripperValue {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field gripper_angle, native scalar int32. */
gripper_angle:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>4-n)return -90;n+=4;
if(n>4)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.gripper_angle,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationMycobotGetGripperValue{const value=new Ros1Mycobot_communicationMycobotGetGripperValue();
{
const decoded=<i32>r.integer(4);
value.gripper_angle=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationMycobotGetGripperValueCodec implements VendorCodec<Ros1Mycobot_communicationMycobotGetGripperValue>{
static readonly codecIdentity:string="UWVMROS1_8080d3e4a1844f789af2ef6752e8b46654a9c595c7dcbb3495f49e6c62419ae9";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros1Mycobot_communicationMycobotGetGripperValue):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationMycobotGetGripperValue):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationMycobotGetGripperValue>{const out=new VendorDecoded<Ros1Mycobot_communicationMycobotGetGripperValue>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationMycobotGetGripperValue.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationMycobotGetGripperValue{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationMycobotGetGripperValue():changetype<Ros1Mycobot_communicationMycobotGetGripperValue>(out.value);}
}
/** Complete original mycobot_communication/MycobotGripperStatus; spellings are published in generation proof. */
export class Ros1Mycobot_communicationMycobotGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationMycobotGripperStatus{const value=new Ros1Mycobot_communicationMycobotGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationMycobotGripperStatusCodec implements VendorCodec<Ros1Mycobot_communicationMycobotGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_65edcc278c4e21355c62408ff310598b3dbdf03ed426e3da0f2ee0bc6910e649";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationMycobotGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationMycobotGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationMycobotGripperStatus>{const out=new VendorDecoded<Ros1Mycobot_communicationMycobotGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationMycobotGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationMycobotGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationMycobotGripperStatus():changetype<Ros1Mycobot_communicationMycobotGripperStatus>(out.value);}
}
/** Complete original mycobot_communication/MycobotPumpStatus; spellings are published in generation proof. */
export class Ros1Mycobot_communicationMycobotPumpStatus {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationMycobotPumpStatus{const value=new Ros1Mycobot_communicationMycobotPumpStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationMycobotPumpStatusCodec implements VendorCodec<Ros1Mycobot_communicationMycobotPumpStatus>{
static readonly codecIdentity:string="UWVMROS1_0c0cb3c0fe97b9f9c65769a09f505ff6c27a60723598d656cd5f16255f36bfbd";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Mycobot_communicationMycobotPumpStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationMycobotPumpStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationMycobotPumpStatus>{const out=new VendorDecoded<Ros1Mycobot_communicationMycobotPumpStatus>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationMycobotPumpStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationMycobotPumpStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationMycobotPumpStatus():changetype<Ros1Mycobot_communicationMycobotPumpStatus>(out.value);}
}
/** Complete original mycobot_communication/MycobotSetAngles; spellings are published in generation proof. */
export class Ros1Mycobot_communicationMycobotSetAngles {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
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
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationMycobotSetAngles{const value=new Ros1Mycobot_communicationMycobotSetAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationMycobotSetAnglesCodec implements VendorCodec<Ros1Mycobot_communicationMycobotSetAngles>{
static readonly codecIdentity:string="UWVMROS1_0844a6cf5033b27ad5062741c939a7cfff63edeba34f7e56c8178e1fab85b334";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros1Mycobot_communicationMycobotSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationMycobotSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationMycobotSetAngles>{const out=new VendorDecoded<Ros1Mycobot_communicationMycobotSetAngles>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationMycobotSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationMycobotSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationMycobotSetAngles():changetype<Ros1Mycobot_communicationMycobotSetAngles>(out.value);}
}
/** Complete original mycobot_communication/MycobotSetCoords; spellings are published in generation proof. */
export class Ros1Mycobot_communicationMycobotSetCoords {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationMycobotSetCoords{const value=new Ros1Mycobot_communicationMycobotSetCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationMycobotSetCoordsCodec implements VendorCodec<Ros1Mycobot_communicationMycobotSetCoords>{
static readonly codecIdentity:string="UWVMROS1_39376ad94c0c9b213703a373b6a3b12fd0dad0ec20ff312143d30099842747a9";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros1Mycobot_communicationMycobotSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationMycobotSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationMycobotSetCoords>{const out=new VendorDecoded<Ros1Mycobot_communicationMycobotSetCoords>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationMycobotSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationMycobotSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationMycobotSetCoords():changetype<Ros1Mycobot_communicationMycobotSetCoords>(out.value);}
}
/** Complete original mycobot_communication/MycobotSetEndType; spellings are published in generation proof. */
export class Ros1Mycobot_communicationMycobotSetEndType {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar uint8. */
Status:u8=0;
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
w.integer(<u64>this.Status,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationMycobotSetEndType{const value=new Ros1Mycobot_communicationMycobotSetEndType();
{
const decoded=<u8>r.integer(1);
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationMycobotSetEndTypeCodec implements VendorCodec<Ros1Mycobot_communicationMycobotSetEndType>{
static readonly codecIdentity:string="UWVMROS1_bb540db63ace33482ad35b74951f0423705297f9a70a4c0114cd604f6018f316";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationMycobotSetEndType):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationMycobotSetEndType):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationMycobotSetEndType>{const out=new VendorDecoded<Ros1Mycobot_communicationMycobotSetEndType>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationMycobotSetEndType.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationMycobotSetEndType{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationMycobotSetEndType():changetype<Ros1Mycobot_communicationMycobotSetEndType>(out.value);}
}
/** Complete original mycobot_communication/MycobotSetFreshMode; spellings are published in generation proof. */
export class Ros1Mycobot_communicationMycobotSetFreshMode {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar uint8. */
Status:u8=0;
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
w.integer(<u64>this.Status,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationMycobotSetFreshMode{const value=new Ros1Mycobot_communicationMycobotSetFreshMode();
{
const decoded=<u8>r.integer(1);
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationMycobotSetFreshModeCodec implements VendorCodec<Ros1Mycobot_communicationMycobotSetFreshMode>{
static readonly codecIdentity:string="UWVMROS1_8e09301673b7d625945b4cd01e6bdd2ed1b7e6cfa1bd88beda7f409322a62c8f";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationMycobotSetFreshMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationMycobotSetFreshMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationMycobotSetFreshMode>{const out=new VendorDecoded<Ros1Mycobot_communicationMycobotSetFreshMode>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationMycobotSetFreshMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationMycobotSetFreshMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationMycobotSetFreshMode():changetype<Ros1Mycobot_communicationMycobotSetFreshMode>(out.value);}
}
/** Complete original mycobot_communication/MycobotSetToolReference; spellings are published in generation proof. */
export class Ros1Mycobot_communicationMycobotSetToolReference {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationMycobotSetToolReference{const value=new Ros1Mycobot_communicationMycobotSetToolReference();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationMycobotSetToolReferenceCodec implements VendorCodec<Ros1Mycobot_communicationMycobotSetToolReference>{
static readonly codecIdentity:string="UWVMROS1_1fb3b9a6679217b95db0a430f5a1f0bece575615dfc80599277edf90190ea11c";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_communicationMycobotSetToolReference):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationMycobotSetToolReference):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationMycobotSetToolReference>{const out=new VendorDecoded<Ros1Mycobot_communicationMycobotSetToolReference>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationMycobotSetToolReference.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationMycobotSetToolReference{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationMycobotSetToolReference():changetype<Ros1Mycobot_communicationMycobotSetToolReference>(out.value);}
}
/** Complete original mycobot_communication/MycobotSetVisionMode; spellings are published in generation proof. */
export class Ros1Mycobot_communicationMycobotSetVisionMode {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar uint8. */
Status:u8=0;
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
w.integer(<u64>this.Status,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationMycobotSetVisionMode{const value=new Ros1Mycobot_communicationMycobotSetVisionMode();
{
const decoded=<u8>r.integer(1);
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationMycobotSetVisionModeCodec implements VendorCodec<Ros1Mycobot_communicationMycobotSetVisionMode>{
static readonly codecIdentity:string="UWVMROS1_c930276fdaaef4870fc6058b156ccc574cc04b4ac96f0bc2f6031aba8016efe9";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationMycobotSetVisionMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationMycobotSetVisionMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationMycobotSetVisionMode>{const out=new VendorDecoded<Ros1Mycobot_communicationMycobotSetVisionMode>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationMycobotSetVisionMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationMycobotSetVisionMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationMycobotSetVisionMode():changetype<Ros1Mycobot_communicationMycobotSetVisionMode>(out.value);}
}
/** Complete original mycobot_communication/GetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mycobot_communicationGetAnglesRequest {
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
static readControl(r:Reader):Ros1Mycobot_communicationGetAnglesRequest{const value=new Ros1Mycobot_communicationGetAnglesRequest();
return value;}
}
export class Ros1Mycobot_communicationGetAnglesRequestCodec implements VendorCodec<Ros1Mycobot_communicationGetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_8915949f81b788e5797f3bc4935e33e9e9eae82c5db3a1211129e7ef03cd2c54";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mycobot_communicationGetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationGetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationGetAnglesRequest>{const out=new VendorDecoded<Ros1Mycobot_communicationGetAnglesRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationGetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationGetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationGetAnglesRequest():changetype<Ros1Mycobot_communicationGetAnglesRequest>(out.value);}
}
/** Complete original mycobot_communication/GetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mycobot_communicationGetAnglesResponse {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationGetAnglesResponse{const value=new Ros1Mycobot_communicationGetAnglesResponse();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationGetAnglesResponseCodec implements VendorCodec<Ros1Mycobot_communicationGetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_482e6b845d6913ab7ea12580a395caf0a9815b56f985eb3a0553263fb7fd4ff3";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_communicationGetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationGetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationGetAnglesResponse>{const out=new VendorDecoded<Ros1Mycobot_communicationGetAnglesResponse>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationGetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationGetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationGetAnglesResponse():changetype<Ros1Mycobot_communicationGetAnglesResponse>(out.value);}
}
/** Complete original mycobot_communication/GetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mycobot_communicationGetCoordsRequest {
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
static readControl(r:Reader):Ros1Mycobot_communicationGetCoordsRequest{const value=new Ros1Mycobot_communicationGetCoordsRequest();
return value;}
}
export class Ros1Mycobot_communicationGetCoordsRequestCodec implements VendorCodec<Ros1Mycobot_communicationGetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_54acc83a24f2f62d8ba9c14b1cea1703cb6097360720550cc438671282d45876";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mycobot_communicationGetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationGetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationGetCoordsRequest>{const out=new VendorDecoded<Ros1Mycobot_communicationGetCoordsRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationGetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationGetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationGetCoordsRequest():changetype<Ros1Mycobot_communicationGetCoordsRequest>(out.value);}
}
/** Complete original mycobot_communication/GetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mycobot_communicationGetCoordsResponse {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationGetCoordsResponse{const value=new Ros1Mycobot_communicationGetCoordsResponse();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationGetCoordsResponseCodec implements VendorCodec<Ros1Mycobot_communicationGetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_275b9bec563bb8578112b3680400523000a08a8e194044164e5334336b73d107";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_communicationGetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationGetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationGetCoordsResponse>{const out=new VendorDecoded<Ros1Mycobot_communicationGetCoordsResponse>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationGetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationGetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationGetCoordsResponse():changetype<Ros1Mycobot_communicationGetCoordsResponse>(out.value);}
}
/** Complete original mycobot_communication/GripperStatusRequest; spellings are published in generation proof. */
export class Ros1Mycobot_communicationGripperStatusRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationGripperStatusRequest{const value=new Ros1Mycobot_communicationGripperStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationGripperStatusRequestCodec implements VendorCodec<Ros1Mycobot_communicationGripperStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_d2c94d1906d311981c72642d82bbea634cfed4eac8c0aef41219bf4f382d4585";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationGripperStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationGripperStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationGripperStatusRequest>{const out=new VendorDecoded<Ros1Mycobot_communicationGripperStatusRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationGripperStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationGripperStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationGripperStatusRequest():changetype<Ros1Mycobot_communicationGripperStatusRequest>(out.value);}
}
/** Complete original mycobot_communication/GripperStatusResponse; spellings are published in generation proof. */
export class Ros1Mycobot_communicationGripperStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationGripperStatusResponse{const value=new Ros1Mycobot_communicationGripperStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationGripperStatusResponseCodec implements VendorCodec<Ros1Mycobot_communicationGripperStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_3a75c0d75882836971ed4c51e50c7b88778fcfeadf9b30df211d5055eb767664";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationGripperStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationGripperStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationGripperStatusResponse>{const out=new VendorDecoded<Ros1Mycobot_communicationGripperStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationGripperStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationGripperStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationGripperStatusResponse():changetype<Ros1Mycobot_communicationGripperStatusResponse>(out.value);}
}
/** Complete original mycobot_communication/PumpStatusRequest; spellings are published in generation proof. */
export class Ros1Mycobot_communicationPumpStatusRequest {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationPumpStatusRequest{const value=new Ros1Mycobot_communicationPumpStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationPumpStatusRequestCodec implements VendorCodec<Ros1Mycobot_communicationPumpStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_561f4bb1f74832111672bbfed1ef1266a91914fb1034437199f730f55324c65c";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Mycobot_communicationPumpStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationPumpStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationPumpStatusRequest>{const out=new VendorDecoded<Ros1Mycobot_communicationPumpStatusRequest>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationPumpStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationPumpStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationPumpStatusRequest():changetype<Ros1Mycobot_communicationPumpStatusRequest>(out.value);}
}
/** Complete original mycobot_communication/PumpStatusResponse; spellings are published in generation proof. */
export class Ros1Mycobot_communicationPumpStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationPumpStatusResponse{const value=new Ros1Mycobot_communicationPumpStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationPumpStatusResponseCodec implements VendorCodec<Ros1Mycobot_communicationPumpStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_b5a1672fe12d2e2131034bb6da8751130b64ef6d48ea48ac0a046c506da452dd";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationPumpStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationPumpStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationPumpStatusResponse>{const out=new VendorDecoded<Ros1Mycobot_communicationPumpStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationPumpStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationPumpStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationPumpStatusResponse():changetype<Ros1Mycobot_communicationPumpStatusResponse>(out.value);}
}
/** Complete original mycobot_communication/SetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetAnglesRequest {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
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
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetAnglesRequest{const value=new Ros1Mycobot_communicationSetAnglesRequest();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetAnglesRequestCodec implements VendorCodec<Ros1Mycobot_communicationSetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_10bcf04e47b743a985ffefc61dff4522f3718d2ac7170960c8ae6857aaa11cbd";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros1Mycobot_communicationSetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetAnglesRequest>{const out=new VendorDecoded<Ros1Mycobot_communicationSetAnglesRequest>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetAnglesRequest():changetype<Ros1Mycobot_communicationSetAnglesRequest>(out.value);}
}
/** Complete original mycobot_communication/SetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetAnglesResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetAnglesResponse{const value=new Ros1Mycobot_communicationSetAnglesResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetAnglesResponseCodec implements VendorCodec<Ros1Mycobot_communicationSetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_d407051eb285975d89ad71f39aede021fb564aecca0e0744126c9a599fe396b2";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationSetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetAnglesResponse>{const out=new VendorDecoded<Ros1Mycobot_communicationSetAnglesResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetAnglesResponse():changetype<Ros1Mycobot_communicationSetAnglesResponse>(out.value);}
}
/** Complete original mycobot_communication/SetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetCoordsRequest {
static readonly maxWireBytes:i32=26;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=26;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>26-n)return -90;n+=4;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
{
if(1>26-n)return -90;n+=1;
if(n>26)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetCoordsRequest{const value=new Ros1Mycobot_communicationSetCoordsRequest();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetCoordsRequestCodec implements VendorCodec<Ros1Mycobot_communicationSetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_7a59a4e31eca60caea9f82142225c890a136c60a9e4c821e9463e89139945f7a";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros1Mycobot_communicationSetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetCoordsRequest>{const out=new VendorDecoded<Ros1Mycobot_communicationSetCoordsRequest>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetCoordsRequest():changetype<Ros1Mycobot_communicationSetCoordsRequest>(out.value);}
}
/** Complete original mycobot_communication/SetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetCoordsResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetCoordsResponse{const value=new Ros1Mycobot_communicationSetCoordsResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetCoordsResponseCodec implements VendorCodec<Ros1Mycobot_communicationSetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_1a936a95cb1bfd0a9158342618c40c587af58388abf3b679eb4bad2518aec6af";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationSetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetCoordsResponse>{const out=new VendorDecoded<Ros1Mycobot_communicationSetCoordsResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetCoordsResponse():changetype<Ros1Mycobot_communicationSetCoordsResponse>(out.value);}
}
/** Complete original mycobot_communication/SetEndTypeRequest; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetEndTypeRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar uint8. */
Status:u8=0;
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
w.integer(<u64>this.Status,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetEndTypeRequest{const value=new Ros1Mycobot_communicationSetEndTypeRequest();
{
const decoded=<u8>r.integer(1);
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetEndTypeRequestCodec implements VendorCodec<Ros1Mycobot_communicationSetEndTypeRequest>{
static readonly codecIdentity:string="UWVMROS1_59248e0ca199fb3bab9b866351648a6d1a58ed2240556122cfecb99331635472";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationSetEndTypeRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetEndTypeRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetEndTypeRequest>{const out=new VendorDecoded<Ros1Mycobot_communicationSetEndTypeRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetEndTypeRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetEndTypeRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetEndTypeRequest():changetype<Ros1Mycobot_communicationSetEndTypeRequest>(out.value);}
}
/** Complete original mycobot_communication/SetEndTypeResponse; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetEndTypeResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetEndTypeResponse{const value=new Ros1Mycobot_communicationSetEndTypeResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetEndTypeResponseCodec implements VendorCodec<Ros1Mycobot_communicationSetEndTypeResponse>{
static readonly codecIdentity:string="UWVMROS1_bf91211b167627be2325f38c0b2981bffa3984848681d52ed3c31d87586f9768";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationSetEndTypeResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetEndTypeResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetEndTypeResponse>{const out=new VendorDecoded<Ros1Mycobot_communicationSetEndTypeResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetEndTypeResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetEndTypeResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetEndTypeResponse():changetype<Ros1Mycobot_communicationSetEndTypeResponse>(out.value);}
}
/** Complete original mycobot_communication/SetFreshModeRequest; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetFreshModeRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar uint8. */
Status:u8=0;
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
w.integer(<u64>this.Status,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetFreshModeRequest{const value=new Ros1Mycobot_communicationSetFreshModeRequest();
{
const decoded=<u8>r.integer(1);
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetFreshModeRequestCodec implements VendorCodec<Ros1Mycobot_communicationSetFreshModeRequest>{
static readonly codecIdentity:string="UWVMROS1_b2ed41361e0adfc856bea480c2703f183b397b4f8b53bc561c95be0ec94d9350";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationSetFreshModeRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetFreshModeRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetFreshModeRequest>{const out=new VendorDecoded<Ros1Mycobot_communicationSetFreshModeRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetFreshModeRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetFreshModeRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetFreshModeRequest():changetype<Ros1Mycobot_communicationSetFreshModeRequest>(out.value);}
}
/** Complete original mycobot_communication/SetFreshModeResponse; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetFreshModeResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetFreshModeResponse{const value=new Ros1Mycobot_communicationSetFreshModeResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetFreshModeResponseCodec implements VendorCodec<Ros1Mycobot_communicationSetFreshModeResponse>{
static readonly codecIdentity:string="UWVMROS1_e075cb6f6112e26ec799de1a72299af83a6bb2361cb90b6aae88434c33023e6f";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationSetFreshModeResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetFreshModeResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetFreshModeResponse>{const out=new VendorDecoded<Ros1Mycobot_communicationSetFreshModeResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetFreshModeResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetFreshModeResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetFreshModeResponse():changetype<Ros1Mycobot_communicationSetFreshModeResponse>(out.value);}
}
/** Complete original mycobot_communication/SetToolReferenceRequest; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetToolReferenceRequest {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetToolReferenceRequest{const value=new Ros1Mycobot_communicationSetToolReferenceRequest();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetToolReferenceRequestCodec implements VendorCodec<Ros1Mycobot_communicationSetToolReferenceRequest>{
static readonly codecIdentity:string="UWVMROS1_ef6dc2cc6e16fae40d20fa7d55d0d988204f4ca62fb99b755a2693aa628f7a79";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_communicationSetToolReferenceRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetToolReferenceRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetToolReferenceRequest>{const out=new VendorDecoded<Ros1Mycobot_communicationSetToolReferenceRequest>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetToolReferenceRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetToolReferenceRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetToolReferenceRequest():changetype<Ros1Mycobot_communicationSetToolReferenceRequest>(out.value);}
}
/** Complete original mycobot_communication/SetToolReferenceResponse; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetToolReferenceResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetToolReferenceResponse{const value=new Ros1Mycobot_communicationSetToolReferenceResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetToolReferenceResponseCodec implements VendorCodec<Ros1Mycobot_communicationSetToolReferenceResponse>{
static readonly codecIdentity:string="UWVMROS1_a14c04cc61a7b5233212072bd203a643b90c551dec9235e57d7eddf1c5e0b146";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationSetToolReferenceResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetToolReferenceResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetToolReferenceResponse>{const out=new VendorDecoded<Ros1Mycobot_communicationSetToolReferenceResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetToolReferenceResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetToolReferenceResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetToolReferenceResponse():changetype<Ros1Mycobot_communicationSetToolReferenceResponse>(out.value);}
}
/** Complete original mycobot_communication/SetVisionModeRequest; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetVisionModeRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar uint8. */
Status:u8=0;
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
w.integer(<u64>this.Status,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetVisionModeRequest{const value=new Ros1Mycobot_communicationSetVisionModeRequest();
{
const decoded=<u8>r.integer(1);
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetVisionModeRequestCodec implements VendorCodec<Ros1Mycobot_communicationSetVisionModeRequest>{
static readonly codecIdentity:string="UWVMROS1_2715ecd55f7669963c076c781ceeef4ae54dac5ab71c378737ade7279c115c8b";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationSetVisionModeRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetVisionModeRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetVisionModeRequest>{const out=new VendorDecoded<Ros1Mycobot_communicationSetVisionModeRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetVisionModeRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetVisionModeRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetVisionModeRequest():changetype<Ros1Mycobot_communicationSetVisionModeRequest>(out.value);}
}
/** Complete original mycobot_communication/SetVisionModeResponse; spellings are published in generation proof. */
export class Ros1Mycobot_communicationSetVisionModeResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_communicationSetVisionModeResponse{const value=new Ros1Mycobot_communicationSetVisionModeResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_communicationSetVisionModeResponseCodec implements VendorCodec<Ros1Mycobot_communicationSetVisionModeResponse>{
static readonly codecIdentity:string="UWVMROS1_2caae9357e25a170c1d26630e666c2818a9a0f305e4608ad07031819f7d89cdb";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_communicationSetVisionModeResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_communicationSetVisionModeResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_communicationSetVisionModeResponse>{const out=new VendorDecoded<Ros1Mycobot_communicationSetVisionModeResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_communicationSetVisionModeResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_communicationSetVisionModeResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_communicationSetVisionModeResponse():changetype<Ros1Mycobot_communicationSetVisionModeResponse>(out.value);}
}
/** Complete original mycobot_pro450_communication/MycobotAngles; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationMycobotAngles {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationMycobotAngles{const value=new Ros1Mycobot_pro450_communicationMycobotAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationMycobotAnglesCodec implements VendorCodec<Ros1Mycobot_pro450_communicationMycobotAngles>{
static readonly codecIdentity:string="UWVMROS1_cebcee90c89b4e73d096135b0c70c8fb644f6aa32c5db066f0bf90982e25c72d";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_pro450_communicationMycobotAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationMycobotAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationMycobotAngles>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationMycobotAngles>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationMycobotAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationMycobotAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationMycobotAngles():changetype<Ros1Mycobot_pro450_communicationMycobotAngles>(out.value);}
}
/** Complete original mycobot_pro450_communication/MycobotCoords; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationMycobotCoords {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationMycobotCoords{const value=new Ros1Mycobot_pro450_communicationMycobotCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationMycobotCoordsCodec implements VendorCodec<Ros1Mycobot_pro450_communicationMycobotCoords>{
static readonly codecIdentity:string="UWVMROS1_4a604335875e926bd7593712c93d8802048266e4ea926ba5447fa9b396ea32e6";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_pro450_communicationMycobotCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationMycobotCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationMycobotCoords>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationMycobotCoords>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationMycobotCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationMycobotCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationMycobotCoords():changetype<Ros1Mycobot_pro450_communicationMycobotCoords>(out.value);}
}
/** Complete original mycobot_pro450_communication/MycobotGetGripperValue; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationMycobotGetGripperValue {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field gripper_angle, native scalar int32. */
gripper_angle:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>4-n)return -90;n+=4;
if(n>4)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.gripper_angle,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationMycobotGetGripperValue{const value=new Ros1Mycobot_pro450_communicationMycobotGetGripperValue();
{
const decoded=<i32>r.integer(4);
value.gripper_angle=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationMycobotGetGripperValueCodec implements VendorCodec<Ros1Mycobot_pro450_communicationMycobotGetGripperValue>{
static readonly codecIdentity:string="UWVMROS1_88ad1a27a0982359460b5043bdd91a2c661ca0dc19009473ae7e0220c5215772";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros1Mycobot_pro450_communicationMycobotGetGripperValue):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationMycobotGetGripperValue):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationMycobotGetGripperValue>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationMycobotGetGripperValue>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationMycobotGetGripperValue.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationMycobotGetGripperValue{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationMycobotGetGripperValue():changetype<Ros1Mycobot_pro450_communicationMycobotGetGripperValue>(out.value);}
}
/** Complete original mycobot_pro450_communication/MycobotGripperStatus; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationMycobotGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationMycobotGripperStatus{const value=new Ros1Mycobot_pro450_communicationMycobotGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationMycobotGripperStatusCodec implements VendorCodec<Ros1Mycobot_pro450_communicationMycobotGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_7c9ed441c8d5bac4dbf566f7a5052d99f0a01a8dcd281059fd7c329f4b232843";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_pro450_communicationMycobotGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationMycobotGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationMycobotGripperStatus>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationMycobotGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationMycobotGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationMycobotGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationMycobotGripperStatus():changetype<Ros1Mycobot_pro450_communicationMycobotGripperStatus>(out.value);}
}
/** Complete original mycobot_pro450_communication/MycobotSetAngles; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationMycobotSetAngles {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
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
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationMycobotSetAngles{const value=new Ros1Mycobot_pro450_communicationMycobotSetAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationMycobotSetAnglesCodec implements VendorCodec<Ros1Mycobot_pro450_communicationMycobotSetAngles>{
static readonly codecIdentity:string="UWVMROS1_896b2be337b734707291fc763629a14362649079634aaa782630607986be6154";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros1Mycobot_pro450_communicationMycobotSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationMycobotSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationMycobotSetAngles>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationMycobotSetAngles>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationMycobotSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationMycobotSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationMycobotSetAngles():changetype<Ros1Mycobot_pro450_communicationMycobotSetAngles>(out.value);}
}
/** Complete original mycobot_pro450_communication/MycobotSetCoords; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationMycobotSetCoords {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.rz))return -22;
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
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationMycobotSetCoords{const value=new Ros1Mycobot_pro450_communicationMycobotSetCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationMycobotSetCoordsCodec implements VendorCodec<Ros1Mycobot_pro450_communicationMycobotSetCoords>{
static readonly codecIdentity:string="UWVMROS1_27409949ade9bb2415cd1af1a278139049bc58098e7de7a5c8adc83d1ad851f6";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros1Mycobot_pro450_communicationMycobotSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationMycobotSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationMycobotSetCoords>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationMycobotSetCoords>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationMycobotSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationMycobotSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationMycobotSetCoords():changetype<Ros1Mycobot_pro450_communicationMycobotSetCoords>(out.value);}
}
/** Complete original mycobot_pro450_communication/MycobotSetFreshMode; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationMycobotSetFreshMode {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar uint8. */
Status:u8=0;
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
w.integer(<u64>this.Status,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationMycobotSetFreshMode{const value=new Ros1Mycobot_pro450_communicationMycobotSetFreshMode();
{
const decoded=<u8>r.integer(1);
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationMycobotSetFreshModeCodec implements VendorCodec<Ros1Mycobot_pro450_communicationMycobotSetFreshMode>{
static readonly codecIdentity:string="UWVMROS1_9e06b1944e7fbab814bbaaa85f167d3804b5d11a1968f18b10e973222d0880ec";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_pro450_communicationMycobotSetFreshMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationMycobotSetFreshMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationMycobotSetFreshMode>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationMycobotSetFreshMode>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationMycobotSetFreshMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationMycobotSetFreshMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationMycobotSetFreshMode():changetype<Ros1Mycobot_pro450_communicationMycobotSetFreshMode>(out.value);}
}
/** Complete original mycobot_pro450_communication/GetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationGetAnglesRequest {
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
static readControl(r:Reader):Ros1Mycobot_pro450_communicationGetAnglesRequest{const value=new Ros1Mycobot_pro450_communicationGetAnglesRequest();
return value;}
}
export class Ros1Mycobot_pro450_communicationGetAnglesRequestCodec implements VendorCodec<Ros1Mycobot_pro450_communicationGetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_07223209461cde1ca99ba125067ca257ceaa9bd1aeceed1ffeb6a3769c152278";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mycobot_pro450_communicationGetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationGetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationGetAnglesRequest>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationGetAnglesRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationGetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationGetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationGetAnglesRequest():changetype<Ros1Mycobot_pro450_communicationGetAnglesRequest>(out.value);}
}
/** Complete original mycobot_pro450_communication/GetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationGetAnglesResponse {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationGetAnglesResponse{const value=new Ros1Mycobot_pro450_communicationGetAnglesResponse();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationGetAnglesResponseCodec implements VendorCodec<Ros1Mycobot_pro450_communicationGetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_000239e0594bda363723370cdde86974aa367ebf3bc93582779edbb2bf578ffb";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_pro450_communicationGetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationGetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationGetAnglesResponse>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationGetAnglesResponse>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationGetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationGetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationGetAnglesResponse():changetype<Ros1Mycobot_pro450_communicationGetAnglesResponse>(out.value);}
}
/** Complete original mycobot_pro450_communication/GetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationGetCoordsRequest {
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
static readControl(r:Reader):Ros1Mycobot_pro450_communicationGetCoordsRequest{const value=new Ros1Mycobot_pro450_communicationGetCoordsRequest();
return value;}
}
export class Ros1Mycobot_pro450_communicationGetCoordsRequestCodec implements VendorCodec<Ros1Mycobot_pro450_communicationGetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_db7906239cfb05812965b398a427fe4e23bf4f0f43fd927c82b0b2370132a649";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mycobot_pro450_communicationGetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationGetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationGetCoordsRequest>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationGetCoordsRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationGetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationGetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationGetCoordsRequest():changetype<Ros1Mycobot_pro450_communicationGetCoordsRequest>(out.value);}
}
/** Complete original mycobot_pro450_communication/GetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationGetCoordsResponse {
static readonly maxWireBytes:i32=24;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=24;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
{
if(!isFinite(this.rz))return -22;
if(4>24-n)return -90;n+=4;
if(n>24)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationGetCoordsResponse{const value=new Ros1Mycobot_pro450_communicationGetCoordsResponse();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationGetCoordsResponseCodec implements VendorCodec<Ros1Mycobot_pro450_communicationGetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_46aee6d68fc728529b5662b86d4f6aa283ff4709a0fdeafa7604347a0ed50d72";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros1Mycobot_pro450_communicationGetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationGetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationGetCoordsResponse>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationGetCoordsResponse>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationGetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationGetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationGetCoordsResponse():changetype<Ros1Mycobot_pro450_communicationGetCoordsResponse>(out.value);}
}
/** Complete original mycobot_pro450_communication/GetGripperValueRequest; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationGetGripperValueRequest {
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
static readControl(r:Reader):Ros1Mycobot_pro450_communicationGetGripperValueRequest{const value=new Ros1Mycobot_pro450_communicationGetGripperValueRequest();
return value;}
}
export class Ros1Mycobot_pro450_communicationGetGripperValueRequestCodec implements VendorCodec<Ros1Mycobot_pro450_communicationGetGripperValueRequest>{
static readonly codecIdentity:string="UWVMROS1_63c7779d43e5fef7c0ec249a46840bfc1844f55efcd29933f6b7a6a9dbdadec6";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mycobot_pro450_communicationGetGripperValueRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationGetGripperValueRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationGetGripperValueRequest>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationGetGripperValueRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationGetGripperValueRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationGetGripperValueRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationGetGripperValueRequest():changetype<Ros1Mycobot_pro450_communicationGetGripperValueRequest>(out.value);}
}
/** Complete original mycobot_pro450_communication/GetGripperValueResponse; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationGetGripperValueResponse {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field gripper_angle, native scalar int32. */
gripper_angle:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>4-n)return -90;n+=4;
if(n>4)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.gripper_angle,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationGetGripperValueResponse{const value=new Ros1Mycobot_pro450_communicationGetGripperValueResponse();
{
const decoded=<i32>r.integer(4);
value.gripper_angle=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationGetGripperValueResponseCodec implements VendorCodec<Ros1Mycobot_pro450_communicationGetGripperValueResponse>{
static readonly codecIdentity:string="UWVMROS1_7c15e7468746ac577b70084cee685a3d71265a9f296caf5ae0b21755a2523b9e";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros1Mycobot_pro450_communicationGetGripperValueResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationGetGripperValueResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationGetGripperValueResponse>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationGetGripperValueResponse>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationGetGripperValueResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationGetGripperValueResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationGetGripperValueResponse():changetype<Ros1Mycobot_pro450_communicationGetGripperValueResponse>(out.value);}
}
/** Complete original mycobot_pro450_communication/GripperStatusRequest; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationGripperStatusRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationGripperStatusRequest{const value=new Ros1Mycobot_pro450_communicationGripperStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationGripperStatusRequestCodec implements VendorCodec<Ros1Mycobot_pro450_communicationGripperStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_10b38d264413567fdf84efc6b7f8effe15e69956db282f9b66dae8b32eddb363";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_pro450_communicationGripperStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationGripperStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationGripperStatusRequest>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationGripperStatusRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationGripperStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationGripperStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationGripperStatusRequest():changetype<Ros1Mycobot_pro450_communicationGripperStatusRequest>(out.value);}
}
/** Complete original mycobot_pro450_communication/GripperStatusResponse; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationGripperStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationGripperStatusResponse{const value=new Ros1Mycobot_pro450_communicationGripperStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationGripperStatusResponseCodec implements VendorCodec<Ros1Mycobot_pro450_communicationGripperStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_0d8dc6af5bd1643e43105c6a91f9dfef97b23ea5b7ac2d55e3511959f3d75f56";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_pro450_communicationGripperStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationGripperStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationGripperStatusResponse>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationGripperStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationGripperStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationGripperStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationGripperStatusResponse():changetype<Ros1Mycobot_pro450_communicationGripperStatusResponse>(out.value);}
}
/** Complete original mycobot_pro450_communication/SetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationSetAnglesRequest {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field joint_5, native scalar float32. */
joint_5:f32=0;
/** Exact native field joint_6, native scalar float32. */
joint_6:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_5))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.joint_6))return -22;
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
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>reinterpret<u32>(this.joint_5),4);
w.integer(<u64>reinterpret<u32>(this.joint_6),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationSetAnglesRequest{const value=new Ros1Mycobot_pro450_communicationSetAnglesRequest();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_5=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_6=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationSetAnglesRequestCodec implements VendorCodec<Ros1Mycobot_pro450_communicationSetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_8868bfafb22b925be720901f303faeec6396f2aab40d9b3e4d94518abd5e7afd";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros1Mycobot_pro450_communicationSetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationSetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationSetAnglesRequest>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationSetAnglesRequest>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationSetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationSetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationSetAnglesRequest():changetype<Ros1Mycobot_pro450_communicationSetAnglesRequest>(out.value);}
}
/** Complete original mycobot_pro450_communication/SetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationSetAnglesResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationSetAnglesResponse{const value=new Ros1Mycobot_pro450_communicationSetAnglesResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationSetAnglesResponseCodec implements VendorCodec<Ros1Mycobot_pro450_communicationSetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_2228dd37fed5398cd2a53dd7653d6749fd5b70960c8eaa60fcfb09de4220474c";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_pro450_communicationSetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationSetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationSetAnglesResponse>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationSetAnglesResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationSetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationSetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationSetAnglesResponse():changetype<Ros1Mycobot_pro450_communicationSetAnglesResponse>(out.value);}
}
/** Complete original mycobot_pro450_communication/SetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationSetCoordsRequest {
static readonly maxWireBytes:i32=25;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=25;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field ry, native scalar float32. */
ry:f32=0;
/** Exact native field rz, native scalar float32. */
rz:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.ry))return -22;
if(4>25-n)return -90;n+=4;
if(n>25)return -90;
}
{
if(!isFinite(this.rz))return -22;
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
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>reinterpret<u32>(this.ry),4);
w.integer(<u64>reinterpret<u32>(this.rz),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationSetCoordsRequest{const value=new Ros1Mycobot_pro450_communicationSetCoordsRequest();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.ry=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rz=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationSetCoordsRequestCodec implements VendorCodec<Ros1Mycobot_pro450_communicationSetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_972fe4365e24ef46a54d7f655873ba4351aa6bdfd9caaec83f158655905d536d";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros1Mycobot_pro450_communicationSetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationSetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationSetCoordsRequest>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationSetCoordsRequest>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationSetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationSetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationSetCoordsRequest():changetype<Ros1Mycobot_pro450_communicationSetCoordsRequest>(out.value);}
}
/** Complete original mycobot_pro450_communication/SetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationSetCoordsResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationSetCoordsResponse{const value=new Ros1Mycobot_pro450_communicationSetCoordsResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationSetCoordsResponseCodec implements VendorCodec<Ros1Mycobot_pro450_communicationSetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_9c62a41f3a4198ab13a993036e78164f2cf09bb61b43a23d54f0674dec58435d";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_pro450_communicationSetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationSetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationSetCoordsResponse>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationSetCoordsResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationSetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationSetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationSetCoordsResponse():changetype<Ros1Mycobot_pro450_communicationSetCoordsResponse>(out.value);}
}
/** Complete original mycobot_pro450_communication/SetFreshModeRequest; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationSetFreshModeRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar uint8. */
Status:u8=0;
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
w.integer(<u64>this.Status,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationSetFreshModeRequest{const value=new Ros1Mycobot_pro450_communicationSetFreshModeRequest();
{
const decoded=<u8>r.integer(1);
value.Status=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationSetFreshModeRequestCodec implements VendorCodec<Ros1Mycobot_pro450_communicationSetFreshModeRequest>{
static readonly codecIdentity:string="UWVMROS1_d744f6c3f5e9ddf6db911e423d5aa4904bbf32ff156121aa4ad7fe5ee8b28628";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_pro450_communicationSetFreshModeRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationSetFreshModeRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationSetFreshModeRequest>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationSetFreshModeRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationSetFreshModeRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationSetFreshModeRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationSetFreshModeRequest():changetype<Ros1Mycobot_pro450_communicationSetFreshModeRequest>(out.value);}
}
/** Complete original mycobot_pro450_communication/SetFreshModeResponse; spellings are published in generation proof. */
export class Ros1Mycobot_pro450_communicationSetFreshModeResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mycobot_pro450_communicationSetFreshModeResponse{const value=new Ros1Mycobot_pro450_communicationSetFreshModeResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mycobot_pro450_communicationSetFreshModeResponseCodec implements VendorCodec<Ros1Mycobot_pro450_communicationSetFreshModeResponse>{
static readonly codecIdentity:string="UWVMROS1_97d367a4d675b8762303e66c1d1aaab32277ac2c5fd92103eb60747214d9d431";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mycobot_pro450_communicationSetFreshModeResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mycobot_pro450_communicationSetFreshModeResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mycobot_pro450_communicationSetFreshModeResponse>{const out=new VendorDecoded<Ros1Mycobot_pro450_communicationSetFreshModeResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mycobot_pro450_communicationSetFreshModeResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mycobot_pro450_communicationSetFreshModeResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mycobot_pro450_communicationSetFreshModeResponse():changetype<Ros1Mycobot_pro450_communicationSetFreshModeResponse>(out.value);}
}
/** Complete original mypalletizer_communication/MypalAngles; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationMypalAngles {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationMypalAngles{const value=new Ros1Mypalletizer_communicationMypalAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationMypalAnglesCodec implements VendorCodec<Ros1Mypalletizer_communicationMypalAngles>{
static readonly codecIdentity:string="UWVMROS1_1d45a33e05e948c1b58988c84afe359138c896119f82da0d737377f0272892d9";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Mypalletizer_communicationMypalAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationMypalAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationMypalAngles>{const out=new VendorDecoded<Ros1Mypalletizer_communicationMypalAngles>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationMypalAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationMypalAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationMypalAngles():changetype<Ros1Mypalletizer_communicationMypalAngles>(out.value);}
}
/** Complete original mypalletizer_communication/MypalCoords; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationMypalCoords {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationMypalCoords{const value=new Ros1Mypalletizer_communicationMypalCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationMypalCoordsCodec implements VendorCodec<Ros1Mypalletizer_communicationMypalCoords>{
static readonly codecIdentity:string="UWVMROS1_c542d1a1a71224901044bc38ca2934021d811417c7c6937ca194995f67e59b80";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Mypalletizer_communicationMypalCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationMypalCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationMypalCoords>{const out=new VendorDecoded<Ros1Mypalletizer_communicationMypalCoords>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationMypalCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationMypalCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationMypalCoords():changetype<Ros1Mypalletizer_communicationMypalCoords>(out.value);}
}
/** Complete original mypalletizer_communication/MypalGripperStatus; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationMypalGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationMypalGripperStatus{const value=new Ros1Mypalletizer_communicationMypalGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationMypalGripperStatusCodec implements VendorCodec<Ros1Mypalletizer_communicationMypalGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_e37f608d4499ab2ed49618fc244831d274fadb23bd408d42619e3f8409ad2ee6";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mypalletizer_communicationMypalGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationMypalGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationMypalGripperStatus>{const out=new VendorDecoded<Ros1Mypalletizer_communicationMypalGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationMypalGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationMypalGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationMypalGripperStatus():changetype<Ros1Mypalletizer_communicationMypalGripperStatus>(out.value);}
}
/** Complete original mypalletizer_communication/MypalPumpStatus; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationMypalPumpStatus {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationMypalPumpStatus{const value=new Ros1Mypalletizer_communicationMypalPumpStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationMypalPumpStatusCodec implements VendorCodec<Ros1Mypalletizer_communicationMypalPumpStatus>{
static readonly codecIdentity:string="UWVMROS1_cdf79e610b5d49b7f73857e02c64688a71517f2fe8e3a96739b16a53ad4d0ebc";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Mypalletizer_communicationMypalPumpStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationMypalPumpStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationMypalPumpStatus>{const out=new VendorDecoded<Ros1Mypalletizer_communicationMypalPumpStatus>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationMypalPumpStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationMypalPumpStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationMypalPumpStatus():changetype<Ros1Mypalletizer_communicationMypalPumpStatus>(out.value);}
}
/** Complete original mypalletizer_communication/MypalSetAngles; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationMypalSetAngles {
static readonly maxWireBytes:i32=17;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=17;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(1>17-n)return -90;n+=1;
if(n>17)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationMypalSetAngles{const value=new Ros1Mypalletizer_communicationMypalSetAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationMypalSetAnglesCodec implements VendorCodec<Ros1Mypalletizer_communicationMypalSetAngles>{
static readonly codecIdentity:string="UWVMROS1_8d11ba2c705bd577a329a01d039b831162742efe790c93b6c2c2631699d04f98";
static readonly maxWireBytes:i32=17;
encodeChecked(value:Ros1Mypalletizer_communicationMypalSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationMypalSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationMypalSetAngles>{const out=new VendorDecoded<Ros1Mypalletizer_communicationMypalSetAngles>();if(bytes.length>17){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationMypalSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationMypalSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationMypalSetAngles():changetype<Ros1Mypalletizer_communicationMypalSetAngles>(out.value);}
}
/** Complete original mypalletizer_communication/MypalSetCoords; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationMypalSetCoords {
static readonly maxWireBytes:i32=18;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=18;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(1>18-n)return -90;n+=1;
if(n>18)return -90;
}
{
if(1>18-n)return -90;n+=1;
if(n>18)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationMypalSetCoords{const value=new Ros1Mypalletizer_communicationMypalSetCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationMypalSetCoordsCodec implements VendorCodec<Ros1Mypalletizer_communicationMypalSetCoords>{
static readonly codecIdentity:string="UWVMROS1_51a2d485c8d2252f184eb27724e88c1ae06002d54454bd8ebf31962a79f7c448";
static readonly maxWireBytes:i32=18;
encodeChecked(value:Ros1Mypalletizer_communicationMypalSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationMypalSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationMypalSetCoords>{const out=new VendorDecoded<Ros1Mypalletizer_communicationMypalSetCoords>();if(bytes.length>18){out.status=90;return out;}if(bytes.length<18){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationMypalSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationMypalSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationMypalSetCoords():changetype<Ros1Mypalletizer_communicationMypalSetCoords>(out.value);}
}
/** Complete original mypalletizer_communication/GetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationGetAnglesRequest {
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
static readControl(r:Reader):Ros1Mypalletizer_communicationGetAnglesRequest{const value=new Ros1Mypalletizer_communicationGetAnglesRequest();
return value;}
}
export class Ros1Mypalletizer_communicationGetAnglesRequestCodec implements VendorCodec<Ros1Mypalletizer_communicationGetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_d0412d8ccc8f724f2a0660d20e24a39f6445f0cb433e775058b6b092fde65de8";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mypalletizer_communicationGetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationGetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationGetAnglesRequest>{const out=new VendorDecoded<Ros1Mypalletizer_communicationGetAnglesRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationGetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationGetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationGetAnglesRequest():changetype<Ros1Mypalletizer_communicationGetAnglesRequest>(out.value);}
}
/** Complete original mypalletizer_communication/GetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationGetAnglesResponse {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationGetAnglesResponse{const value=new Ros1Mypalletizer_communicationGetAnglesResponse();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationGetAnglesResponseCodec implements VendorCodec<Ros1Mypalletizer_communicationGetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_cb5a18aec2420c030d8193eb40d25691a2491b98606a7722156984e82f77f745";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Mypalletizer_communicationGetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationGetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationGetAnglesResponse>{const out=new VendorDecoded<Ros1Mypalletizer_communicationGetAnglesResponse>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationGetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationGetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationGetAnglesResponse():changetype<Ros1Mypalletizer_communicationGetAnglesResponse>(out.value);}
}
/** Complete original mypalletizer_communication/GetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationGetCoordsRequest {
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
static readControl(r:Reader):Ros1Mypalletizer_communicationGetCoordsRequest{const value=new Ros1Mypalletizer_communicationGetCoordsRequest();
return value;}
}
export class Ros1Mypalletizer_communicationGetCoordsRequestCodec implements VendorCodec<Ros1Mypalletizer_communicationGetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_47d45419f14ce576d9e8c2bb288c7b67bb161115e7b99a2c4752d09cbb8087b8";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Mypalletizer_communicationGetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationGetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationGetCoordsRequest>{const out=new VendorDecoded<Ros1Mypalletizer_communicationGetCoordsRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationGetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationGetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationGetCoordsRequest():changetype<Ros1Mypalletizer_communicationGetCoordsRequest>(out.value);}
}
/** Complete original mypalletizer_communication/GetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationGetCoordsResponse {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationGetCoordsResponse{const value=new Ros1Mypalletizer_communicationGetCoordsResponse();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationGetCoordsResponseCodec implements VendorCodec<Ros1Mypalletizer_communicationGetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_7fdad367484b5f89c812c2513e99373844dcd160a115e326fb1c597f221b53fb";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Mypalletizer_communicationGetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationGetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationGetCoordsResponse>{const out=new VendorDecoded<Ros1Mypalletizer_communicationGetCoordsResponse>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationGetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationGetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationGetCoordsResponse():changetype<Ros1Mypalletizer_communicationGetCoordsResponse>(out.value);}
}
/** Complete original mypalletizer_communication/GripperStatusRequest; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationGripperStatusRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationGripperStatusRequest{const value=new Ros1Mypalletizer_communicationGripperStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationGripperStatusRequestCodec implements VendorCodec<Ros1Mypalletizer_communicationGripperStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_48d661f8fd6adfcf1703123464fcfb9351f8f831c0779ffad103a63bcf7ea709";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mypalletizer_communicationGripperStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationGripperStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationGripperStatusRequest>{const out=new VendorDecoded<Ros1Mypalletizer_communicationGripperStatusRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationGripperStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationGripperStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationGripperStatusRequest():changetype<Ros1Mypalletizer_communicationGripperStatusRequest>(out.value);}
}
/** Complete original mypalletizer_communication/GripperStatusResponse; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationGripperStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationGripperStatusResponse{const value=new Ros1Mypalletizer_communicationGripperStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationGripperStatusResponseCodec implements VendorCodec<Ros1Mypalletizer_communicationGripperStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_00ab8ceb190b588a9420c2e044da64857cfdb76e709e40e57835ab5732074e2b";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mypalletizer_communicationGripperStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationGripperStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationGripperStatusResponse>{const out=new VendorDecoded<Ros1Mypalletizer_communicationGripperStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationGripperStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationGripperStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationGripperStatusResponse():changetype<Ros1Mypalletizer_communicationGripperStatusResponse>(out.value);}
}
/** Complete original mypalletizer_communication/PumpStatusRequest; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationPumpStatusRequest {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
/** Exact native field Pin1, native scalar int8. */
Pin1:i8=0;
/** Exact native field Pin2, native scalar int8. */
Pin2:i8=0;
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
w.u8(this.Status?1:0);
w.integer(<u64>this.Pin1,1);
w.integer(<u64>this.Pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationPumpStatusRequest{const value=new Ros1Mypalletizer_communicationPumpStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.Pin2=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationPumpStatusRequestCodec implements VendorCodec<Ros1Mypalletizer_communicationPumpStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_6dd70636864c4450a066c1d75e67643331fac4c3e1625bce1b94c85ce7a35636";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros1Mypalletizer_communicationPumpStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationPumpStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationPumpStatusRequest>{const out=new VendorDecoded<Ros1Mypalletizer_communicationPumpStatusRequest>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationPumpStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationPumpStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationPumpStatusRequest():changetype<Ros1Mypalletizer_communicationPumpStatusRequest>(out.value);}
}
/** Complete original mypalletizer_communication/PumpStatusResponse; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationPumpStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationPumpStatusResponse{const value=new Ros1Mypalletizer_communicationPumpStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationPumpStatusResponseCodec implements VendorCodec<Ros1Mypalletizer_communicationPumpStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_c2503f89c84c49db8dab2ac9f847ba9d09434d366fd8916a7d5f30ea161bb4c5";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mypalletizer_communicationPumpStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationPumpStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationPumpStatusResponse>{const out=new VendorDecoded<Ros1Mypalletizer_communicationPumpStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationPumpStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationPumpStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationPumpStatusResponse():changetype<Ros1Mypalletizer_communicationPumpStatusResponse>(out.value);}
}
/** Complete original mypalletizer_communication/SetAnglesRequest; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationSetAnglesRequest {
static readonly maxWireBytes:i32=17;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=17;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(1>17-n)return -90;n+=1;
if(n>17)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationSetAnglesRequest{const value=new Ros1Mypalletizer_communicationSetAnglesRequest();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationSetAnglesRequestCodec implements VendorCodec<Ros1Mypalletizer_communicationSetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_32072510c6deddb3f372b5f300068ea457916a4e0397b30951e421a1da641d5d";
static readonly maxWireBytes:i32=17;
encodeChecked(value:Ros1Mypalletizer_communicationSetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationSetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationSetAnglesRequest>{const out=new VendorDecoded<Ros1Mypalletizer_communicationSetAnglesRequest>();if(bytes.length>17){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationSetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationSetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationSetAnglesRequest():changetype<Ros1Mypalletizer_communicationSetAnglesRequest>(out.value);}
}
/** Complete original mypalletizer_communication/SetAnglesResponse; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationSetAnglesResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationSetAnglesResponse{const value=new Ros1Mypalletizer_communicationSetAnglesResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationSetAnglesResponseCodec implements VendorCodec<Ros1Mypalletizer_communicationSetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_1472b47adc3171a9f1938e52bc937af6dd6bb720d33e163b70ae066a89a3211c";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mypalletizer_communicationSetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationSetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationSetAnglesResponse>{const out=new VendorDecoded<Ros1Mypalletizer_communicationSetAnglesResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationSetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationSetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationSetAnglesResponse():changetype<Ros1Mypalletizer_communicationSetAnglesResponse>(out.value);}
}
/** Complete original mypalletizer_communication/SetCoordsRequest; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationSetCoordsRequest {
static readonly maxWireBytes:i32=18;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=18;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field speed, native scalar int8. */
speed:i8=0;
/** Exact native field model, native scalar int8. */
model:i8=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>18-n)return -90;n+=4;
if(n>18)return -90;
}
{
if(1>18-n)return -90;n+=1;
if(n>18)return -90;
}
{
if(1>18-n)return -90;n+=1;
if(n>18)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>this.speed,1);
w.integer(<u64>this.model,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationSetCoordsRequest{const value=new Ros1Mypalletizer_communicationSetCoordsRequest();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=<i8>r.integer(1);
value.speed=decoded;
}
{
const decoded=<i8>r.integer(1);
value.model=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationSetCoordsRequestCodec implements VendorCodec<Ros1Mypalletizer_communicationSetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_2d608cac1a7129ca5cd68ff52170ed40258cb6d199f4842a268a1d7570f9bda1";
static readonly maxWireBytes:i32=18;
encodeChecked(value:Ros1Mypalletizer_communicationSetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationSetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationSetCoordsRequest>{const out=new VendorDecoded<Ros1Mypalletizer_communicationSetCoordsRequest>();if(bytes.length>18){out.status=90;return out;}if(bytes.length<18){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationSetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationSetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationSetCoordsRequest():changetype<Ros1Mypalletizer_communicationSetCoordsRequest>(out.value);}
}
/** Complete original mypalletizer_communication/SetCoordsResponse; spellings are published in generation proof. */
export class Ros1Mypalletizer_communicationSetCoordsResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Mypalletizer_communicationSetCoordsResponse{const value=new Ros1Mypalletizer_communicationSetCoordsResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Mypalletizer_communicationSetCoordsResponseCodec implements VendorCodec<Ros1Mypalletizer_communicationSetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_86900f8772e9609e1d2b882b6ab613ce9609be6c589e5b9574a57b8749231308";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Mypalletizer_communicationSetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Mypalletizer_communicationSetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Mypalletizer_communicationSetCoordsResponse>{const out=new VendorDecoded<Ros1Mypalletizer_communicationSetCoordsResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Mypalletizer_communicationSetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Mypalletizer_communicationSetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Mypalletizer_communicationSetCoordsResponse():changetype<Ros1Mypalletizer_communicationSetCoordsResponse>(out.value);}
}
/** Complete original ultraarm_communication/MycobotAngles; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationMycobotAngles {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationMycobotAngles{const value=new Ros1Ultraarm_communicationMycobotAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationMycobotAnglesCodec implements VendorCodec<Ros1Ultraarm_communicationMycobotAngles>{
static readonly codecIdentity:string="UWVMROS1_6bd94559b201105277e0638cff9820be0cf03efb492bc958a61a78222e1afe67";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Ultraarm_communicationMycobotAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationMycobotAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationMycobotAngles>{const out=new VendorDecoded<Ros1Ultraarm_communicationMycobotAngles>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationMycobotAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationMycobotAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationMycobotAngles():changetype<Ros1Ultraarm_communicationMycobotAngles>(out.value);}
}
/** Complete original ultraarm_communication/MycobotCoords; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationMycobotCoords {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationMycobotCoords{const value=new Ros1Ultraarm_communicationMycobotCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationMycobotCoordsCodec implements VendorCodec<Ros1Ultraarm_communicationMycobotCoords>{
static readonly codecIdentity:string="UWVMROS1_f4228da3bdb19f7a63e4fbdddb7bc271debed1178663a855476c2b2425e36134";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Ultraarm_communicationMycobotCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationMycobotCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationMycobotCoords>{const out=new VendorDecoded<Ros1Ultraarm_communicationMycobotCoords>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationMycobotCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationMycobotCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationMycobotCoords():changetype<Ros1Ultraarm_communicationMycobotCoords>(out.value);}
}
/** Complete original ultraarm_communication/MycobotGetGripperValue; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationMycobotGetGripperValue {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field gripper_angle, native scalar int32. */
gripper_angle:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>4-n)return -90;n+=4;
if(n>4)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.gripper_angle,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationMycobotGetGripperValue{const value=new Ros1Ultraarm_communicationMycobotGetGripperValue();
{
const decoded=<i32>r.integer(4);
value.gripper_angle=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationMycobotGetGripperValueCodec implements VendorCodec<Ros1Ultraarm_communicationMycobotGetGripperValue>{
static readonly codecIdentity:string="UWVMROS1_c5c4c82dfab7600d1ee48fc899c1e0906a80b59e03fee89ef75de5e8bbd1a672";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros1Ultraarm_communicationMycobotGetGripperValue):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationMycobotGetGripperValue):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationMycobotGetGripperValue>{const out=new VendorDecoded<Ros1Ultraarm_communicationMycobotGetGripperValue>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationMycobotGetGripperValue.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationMycobotGetGripperValue{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationMycobotGetGripperValue():changetype<Ros1Ultraarm_communicationMycobotGetGripperValue>(out.value);}
}
/** Complete original ultraarm_communication/MycobotGripperStatus; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationMycobotGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationMycobotGripperStatus{const value=new Ros1Ultraarm_communicationMycobotGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationMycobotGripperStatusCodec implements VendorCodec<Ros1Ultraarm_communicationMycobotGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_c3ca37a8201c045099baec080410ebfc399793a7953d36c0bce7d50e786611ce";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ultraarm_communicationMycobotGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationMycobotGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationMycobotGripperStatus>{const out=new VendorDecoded<Ros1Ultraarm_communicationMycobotGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationMycobotGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationMycobotGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationMycobotGripperStatus():changetype<Ros1Ultraarm_communicationMycobotGripperStatus>(out.value);}
}
/** Complete original ultraarm_communication/MycobotSetAngles; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationMycobotSetAngles {
static readonly maxWireBytes:i32=20;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=20;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field speed, native scalar int32. */
speed:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>this.speed,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationMycobotSetAngles{const value=new Ros1Ultraarm_communicationMycobotSetAngles();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=<i32>r.integer(4);
value.speed=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationMycobotSetAnglesCodec implements VendorCodec<Ros1Ultraarm_communicationMycobotSetAngles>{
static readonly codecIdentity:string="UWVMROS1_cb0fbf2cc911d7b907fbe1c09f7dbd4643439430b05976ce508bd7dbf71f9baa";
static readonly maxWireBytes:i32=20;
encodeChecked(value:Ros1Ultraarm_communicationMycobotSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationMycobotSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationMycobotSetAngles>{const out=new VendorDecoded<Ros1Ultraarm_communicationMycobotSetAngles>();if(bytes.length>20){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationMycobotSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationMycobotSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationMycobotSetAngles():changetype<Ros1Ultraarm_communicationMycobotSetAngles>(out.value);}
}
/** Complete original ultraarm_communication/MycobotSetCoords; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationMycobotSetCoords {
static readonly maxWireBytes:i32=20;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=20;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field speed, native scalar int32. */
speed:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>this.speed,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationMycobotSetCoords{const value=new Ros1Ultraarm_communicationMycobotSetCoords();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=<i32>r.integer(4);
value.speed=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationMycobotSetCoordsCodec implements VendorCodec<Ros1Ultraarm_communicationMycobotSetCoords>{
static readonly codecIdentity:string="UWVMROS1_7eb6643462e1eec9cf943d9251c8b5fb47a6c1c63a4381bacd540e22c3584a68";
static readonly maxWireBytes:i32=20;
encodeChecked(value:Ros1Ultraarm_communicationMycobotSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationMycobotSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationMycobotSetCoords>{const out=new VendorDecoded<Ros1Ultraarm_communicationMycobotSetCoords>();if(bytes.length>20){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationMycobotSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationMycobotSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationMycobotSetCoords():changetype<Ros1Ultraarm_communicationMycobotSetCoords>(out.value);}
}
/** Complete original ultraarm_communication/MycobotSetFreshMode; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationMycobotSetFreshMode {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar uint8. */
Status:u8=0;
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
w.integer(<u64>this.Status,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationMycobotSetFreshMode{const value=new Ros1Ultraarm_communicationMycobotSetFreshMode();
{
const decoded=<u8>r.integer(1);
value.Status=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationMycobotSetFreshModeCodec implements VendorCodec<Ros1Ultraarm_communicationMycobotSetFreshMode>{
static readonly codecIdentity:string="UWVMROS1_0934edaedfc4606d4301b38218c72e6fc323fcea3bb39143bd507ec3b410e638";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ultraarm_communicationMycobotSetFreshMode):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationMycobotSetFreshMode):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationMycobotSetFreshMode>{const out=new VendorDecoded<Ros1Ultraarm_communicationMycobotSetFreshMode>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationMycobotSetFreshMode.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationMycobotSetFreshMode{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationMycobotSetFreshMode():changetype<Ros1Ultraarm_communicationMycobotSetFreshMode>(out.value);}
}
/** Complete original ultraarm_communication/GetAnglesRequest; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationGetAnglesRequest {
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
static readControl(r:Reader):Ros1Ultraarm_communicationGetAnglesRequest{const value=new Ros1Ultraarm_communicationGetAnglesRequest();
return value;}
}
export class Ros1Ultraarm_communicationGetAnglesRequestCodec implements VendorCodec<Ros1Ultraarm_communicationGetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_fd6922aabe6ec7f82731d19efb0963753d86a8f412de96f8bc67ea2615a7c860";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Ultraarm_communicationGetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationGetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationGetAnglesRequest>{const out=new VendorDecoded<Ros1Ultraarm_communicationGetAnglesRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationGetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationGetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationGetAnglesRequest():changetype<Ros1Ultraarm_communicationGetAnglesRequest>(out.value);}
}
/** Complete original ultraarm_communication/GetAnglesResponse; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationGetAnglesResponse {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationGetAnglesResponse{const value=new Ros1Ultraarm_communicationGetAnglesResponse();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationGetAnglesResponseCodec implements VendorCodec<Ros1Ultraarm_communicationGetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_d6b80e48675b3a8f262cc463bca84353d52291cd450a656b14f5d81be851a8cf";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Ultraarm_communicationGetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationGetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationGetAnglesResponse>{const out=new VendorDecoded<Ros1Ultraarm_communicationGetAnglesResponse>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationGetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationGetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationGetAnglesResponse():changetype<Ros1Ultraarm_communicationGetAnglesResponse>(out.value);}
}
/** Complete original ultraarm_communication/GetCoordsRequest; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationGetCoordsRequest {
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
static readControl(r:Reader):Ros1Ultraarm_communicationGetCoordsRequest{const value=new Ros1Ultraarm_communicationGetCoordsRequest();
return value;}
}
export class Ros1Ultraarm_communicationGetCoordsRequestCodec implements VendorCodec<Ros1Ultraarm_communicationGetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_40a6535b429baf0bbe0bc642ade7a8048e693633388ccd061bf12b6d795bbbd1";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Ultraarm_communicationGetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationGetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationGetCoordsRequest>{const out=new VendorDecoded<Ros1Ultraarm_communicationGetCoordsRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationGetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationGetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationGetCoordsRequest():changetype<Ros1Ultraarm_communicationGetCoordsRequest>(out.value);}
}
/** Complete original ultraarm_communication/GetCoordsResponse; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationGetCoordsResponse {
static readonly maxWireBytes:i32=16;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=16;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>16-n)return -90;n+=4;
if(n>16)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationGetCoordsResponse{const value=new Ros1Ultraarm_communicationGetCoordsResponse();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationGetCoordsResponseCodec implements VendorCodec<Ros1Ultraarm_communicationGetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_4ce97f0eb2846366887ab0ae90d2b2ca8f4571cddb5e0c7095ca7098b2e127a1";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros1Ultraarm_communicationGetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationGetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationGetCoordsResponse>{const out=new VendorDecoded<Ros1Ultraarm_communicationGetCoordsResponse>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationGetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationGetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationGetCoordsResponse():changetype<Ros1Ultraarm_communicationGetCoordsResponse>(out.value);}
}
/** Complete original ultraarm_communication/GetGripperValueRequest; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationGetGripperValueRequest {
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
static readControl(r:Reader):Ros1Ultraarm_communicationGetGripperValueRequest{const value=new Ros1Ultraarm_communicationGetGripperValueRequest();
return value;}
}
export class Ros1Ultraarm_communicationGetGripperValueRequestCodec implements VendorCodec<Ros1Ultraarm_communicationGetGripperValueRequest>{
static readonly codecIdentity:string="UWVMROS1_f4daff0f9c6ba692e9b20e29cc67066afb3931d146ba995848dcdc83db2b52b1";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros1Ultraarm_communicationGetGripperValueRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationGetGripperValueRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationGetGripperValueRequest>{const out=new VendorDecoded<Ros1Ultraarm_communicationGetGripperValueRequest>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationGetGripperValueRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationGetGripperValueRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationGetGripperValueRequest():changetype<Ros1Ultraarm_communicationGetGripperValueRequest>(out.value);}
}
/** Complete original ultraarm_communication/GetGripperValueResponse; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationGetGripperValueResponse {
static readonly maxWireBytes:i32=4;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=4;
/** Exact native field gripper_angle, native scalar int32. */
gripper_angle:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(4>4-n)return -90;n+=4;
if(n>4)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>this.gripper_angle,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationGetGripperValueResponse{const value=new Ros1Ultraarm_communicationGetGripperValueResponse();
{
const decoded=<i32>r.integer(4);
value.gripper_angle=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationGetGripperValueResponseCodec implements VendorCodec<Ros1Ultraarm_communicationGetGripperValueResponse>{
static readonly codecIdentity:string="UWVMROS1_ff7452fb5fb1f5a55c8db0a2aa6d18df1a62a85d32a659506dd31a1ce1721c90";
static readonly maxWireBytes:i32=4;
encodeChecked(value:Ros1Ultraarm_communicationGetGripperValueResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationGetGripperValueResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationGetGripperValueResponse>{const out=new VendorDecoded<Ros1Ultraarm_communicationGetGripperValueResponse>();if(bytes.length>4){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationGetGripperValueResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationGetGripperValueResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationGetGripperValueResponse():changetype<Ros1Ultraarm_communicationGetGripperValueResponse>(out.value);}
}
/** Complete original ultraarm_communication/GripperStatusRequest; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationGripperStatusRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar bool. */
Status:bool=false;
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
w.u8(this.Status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationGripperStatusRequest{const value=new Ros1Ultraarm_communicationGripperStatusRequest();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Status=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationGripperStatusRequestCodec implements VendorCodec<Ros1Ultraarm_communicationGripperStatusRequest>{
static readonly codecIdentity:string="UWVMROS1_c435d39bef04bd7022e048c0cf364d100bf7a92855f04c816d2a22f0e672d586";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ultraarm_communicationGripperStatusRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationGripperStatusRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationGripperStatusRequest>{const out=new VendorDecoded<Ros1Ultraarm_communicationGripperStatusRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationGripperStatusRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationGripperStatusRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationGripperStatusRequest():changetype<Ros1Ultraarm_communicationGripperStatusRequest>(out.value);}
}
/** Complete original ultraarm_communication/GripperStatusResponse; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationGripperStatusResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationGripperStatusResponse{const value=new Ros1Ultraarm_communicationGripperStatusResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationGripperStatusResponseCodec implements VendorCodec<Ros1Ultraarm_communicationGripperStatusResponse>{
static readonly codecIdentity:string="UWVMROS1_fa0c99ff37acea63c5d507174f9725a9341ffaa007e722e21a8e0b93097e56d4";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ultraarm_communicationGripperStatusResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationGripperStatusResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationGripperStatusResponse>{const out=new VendorDecoded<Ros1Ultraarm_communicationGripperStatusResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationGripperStatusResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationGripperStatusResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationGripperStatusResponse():changetype<Ros1Ultraarm_communicationGripperStatusResponse>(out.value);}
}
/** Complete original ultraarm_communication/SetAnglesRequest; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationSetAnglesRequest {
static readonly maxWireBytes:i32=20;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=20;
/** Exact native field joint_1, native scalar float32. */
joint_1:f32=0;
/** Exact native field joint_2, native scalar float32. */
joint_2:f32=0;
/** Exact native field joint_3, native scalar float32. */
joint_3:f32=0;
/** Exact native field joint_4, native scalar float32. */
joint_4:f32=0;
/** Exact native field speed, native scalar int32. */
speed:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.joint_1))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.joint_2))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.joint_3))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.joint_4))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.joint_1),4);
w.integer(<u64>reinterpret<u32>(this.joint_2),4);
w.integer(<u64>reinterpret<u32>(this.joint_3),4);
w.integer(<u64>reinterpret<u32>(this.joint_4),4);
w.integer(<u64>this.speed,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationSetAnglesRequest{const value=new Ros1Ultraarm_communicationSetAnglesRequest();
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_1=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_2=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_3=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.joint_4=decoded;
}
{
const decoded=<i32>r.integer(4);
value.speed=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationSetAnglesRequestCodec implements VendorCodec<Ros1Ultraarm_communicationSetAnglesRequest>{
static readonly codecIdentity:string="UWVMROS1_5a1aab8379585f404a15dbfd8832dd6434d969f41681127befca7458181fc65a";
static readonly maxWireBytes:i32=20;
encodeChecked(value:Ros1Ultraarm_communicationSetAnglesRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationSetAnglesRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationSetAnglesRequest>{const out=new VendorDecoded<Ros1Ultraarm_communicationSetAnglesRequest>();if(bytes.length>20){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationSetAnglesRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationSetAnglesRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationSetAnglesRequest():changetype<Ros1Ultraarm_communicationSetAnglesRequest>(out.value);}
}
/** Complete original ultraarm_communication/SetAnglesResponse; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationSetAnglesResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationSetAnglesResponse{const value=new Ros1Ultraarm_communicationSetAnglesResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationSetAnglesResponseCodec implements VendorCodec<Ros1Ultraarm_communicationSetAnglesResponse>{
static readonly codecIdentity:string="UWVMROS1_f1ecaff9a60a47ef978b0faf2b9b2b9166ae308b3df718b0536536004dc8a3b4";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ultraarm_communicationSetAnglesResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationSetAnglesResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationSetAnglesResponse>{const out=new VendorDecoded<Ros1Ultraarm_communicationSetAnglesResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationSetAnglesResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationSetAnglesResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationSetAnglesResponse():changetype<Ros1Ultraarm_communicationSetAnglesResponse>(out.value);}
}
/** Complete original ultraarm_communication/SetCoordsRequest; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationSetCoordsRequest {
static readonly maxWireBytes:i32=20;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=20;
/** Exact native field x, native scalar float32. */
x:f32=0;
/** Exact native field y, native scalar float32. */
y:f32=0;
/** Exact native field z, native scalar float32. */
z:f32=0;
/** Exact native field rx, native scalar float32. */
rx:f32=0;
/** Exact native field speed, native scalar int32. */
speed:i32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(!isFinite(this.rx))return -22;
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
{
if(4>20-n)return -90;n+=4;
if(n>20)return -90;
}
return n;}
statusValue():u32{const n=this.wireSize();return n<0?<u32>(-n):0;}
validValue():bool{return this.statusValue()==0;}
/** Internal trusted writer; public encode first measures the complete value. */
writeControl(w:VendorWriter):void{
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>this.speed,4);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationSetCoordsRequest{const value=new Ros1Ultraarm_communicationSetCoordsRequest();
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.rx=decoded;
}
{
const decoded=<i32>r.integer(4);
value.speed=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationSetCoordsRequestCodec implements VendorCodec<Ros1Ultraarm_communicationSetCoordsRequest>{
static readonly codecIdentity:string="UWVMROS1_54d83be688da3c885a7166e4d2c1c636a1144ba863a0a388907511896d5626e5";
static readonly maxWireBytes:i32=20;
encodeChecked(value:Ros1Ultraarm_communicationSetCoordsRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationSetCoordsRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationSetCoordsRequest>{const out=new VendorDecoded<Ros1Ultraarm_communicationSetCoordsRequest>();if(bytes.length>20){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationSetCoordsRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationSetCoordsRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationSetCoordsRequest():changetype<Ros1Ultraarm_communicationSetCoordsRequest>(out.value);}
}
/** Complete original ultraarm_communication/SetCoordsResponse; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationSetCoordsResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationSetCoordsResponse{const value=new Ros1Ultraarm_communicationSetCoordsResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationSetCoordsResponseCodec implements VendorCodec<Ros1Ultraarm_communicationSetCoordsResponse>{
static readonly codecIdentity:string="UWVMROS1_02aa6c82612c5ffd43e890bb173a59245e00cd91dbc38303dcbdc0fb30683431";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ultraarm_communicationSetCoordsResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationSetCoordsResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationSetCoordsResponse>{const out=new VendorDecoded<Ros1Ultraarm_communicationSetCoordsResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationSetCoordsResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationSetCoordsResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationSetCoordsResponse():changetype<Ros1Ultraarm_communicationSetCoordsResponse>(out.value);}
}
/** Complete original ultraarm_communication/SetFreshModeRequest; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationSetFreshModeRequest {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Status, native scalar uint8. */
Status:u8=0;
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
w.integer(<u64>this.Status,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationSetFreshModeRequest{const value=new Ros1Ultraarm_communicationSetFreshModeRequest();
{
const decoded=<u8>r.integer(1);
value.Status=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationSetFreshModeRequestCodec implements VendorCodec<Ros1Ultraarm_communicationSetFreshModeRequest>{
static readonly codecIdentity:string="UWVMROS1_142c43e6dcd72843cdd002e5b6815524b2b6a5a02372e5b966dc630947a90649";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ultraarm_communicationSetFreshModeRequest):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationSetFreshModeRequest):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationSetFreshModeRequest>{const out=new VendorDecoded<Ros1Ultraarm_communicationSetFreshModeRequest>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationSetFreshModeRequest.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationSetFreshModeRequest{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationSetFreshModeRequest():changetype<Ros1Ultraarm_communicationSetFreshModeRequest>(out.value);}
}
/** Complete original ultraarm_communication/SetFreshModeResponse; spellings are published in generation proof. */
export class Ros1Ultraarm_communicationSetFreshModeResponse {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field Flag, native scalar bool. */
Flag:bool=false;
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
w.u8(this.Flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros1Ultraarm_communicationSetFreshModeResponse{const value=new Ros1Ultraarm_communicationSetFreshModeResponse();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.Flag=decoded;
}
return value;}
}
export class Ros1Ultraarm_communicationSetFreshModeResponseCodec implements VendorCodec<Ros1Ultraarm_communicationSetFreshModeResponse>{
static readonly codecIdentity:string="UWVMROS1_764d9c9dfff1f18a5501c5f040a6bc66ae6f93bfb7b932a33c2ece2a70d38104";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros1Ultraarm_communicationSetFreshModeResponse):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros1Ultraarm_communicationSetFreshModeResponse):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros1Ultraarm_communicationSetFreshModeResponse>{const out=new VendorDecoded<Ros1Ultraarm_communicationSetFreshModeResponse>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros1Ultraarm_communicationSetFreshModeResponse.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros1Ultraarm_communicationSetFreshModeResponse{const out=this.decodeChecked(bytes);return out.value===null?new Ros1Ultraarm_communicationSetFreshModeResponse():changetype<Ros1Ultraarm_communicationSetFreshModeResponse>(out.value);}
}
