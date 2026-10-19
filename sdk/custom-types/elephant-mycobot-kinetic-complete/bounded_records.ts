/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
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
