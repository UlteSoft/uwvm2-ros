/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
/** Complete original mecharm_interfaces/msg/MecharmAngles; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesMsgMecharmAngles {
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
static readControl(r:Reader):Ros2Mecharm_interfacesMsgMecharmAngles{const value=new Ros2Mecharm_interfacesMsgMecharmAngles();
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
export class Ros2Mecharm_interfacesMsgMecharmAnglesCodec implements VendorCodec<Ros2Mecharm_interfacesMsgMecharmAngles>{
static readonly codecIdentity:string="UWVMROS1_37d12e0fc3b7bbb50bdcee0ca624a5c637f9554f3c64d640f538aeda57417331";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mecharm_interfacesMsgMecharmAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesMsgMecharmAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesMsgMecharmAngles>{const out=new VendorDecoded<Ros2Mecharm_interfacesMsgMecharmAngles>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesMsgMecharmAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesMsgMecharmAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesMsgMecharmAngles():changetype<Ros2Mecharm_interfacesMsgMecharmAngles>(out.value);}
}
/** Complete original mecharm_interfaces/msg/MecharmCoords; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesMsgMecharmCoords {
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
static readControl(r:Reader):Ros2Mecharm_interfacesMsgMecharmCoords{const value=new Ros2Mecharm_interfacesMsgMecharmCoords();
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
export class Ros2Mecharm_interfacesMsgMecharmCoordsCodec implements VendorCodec<Ros2Mecharm_interfacesMsgMecharmCoords>{
static readonly codecIdentity:string="UWVMROS1_e1bb96b1908c796e2a89eb4dd2402a6cc02bcc5077c52268bb378a52dd3029ed";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mecharm_interfacesMsgMecharmCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesMsgMecharmCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesMsgMecharmCoords>{const out=new VendorDecoded<Ros2Mecharm_interfacesMsgMecharmCoords>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesMsgMecharmCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesMsgMecharmCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesMsgMecharmCoords():changetype<Ros2Mecharm_interfacesMsgMecharmCoords>(out.value);}
}
/** Complete original mecharm_interfaces/msg/MecharmGripperStatus; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesMsgMecharmGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar bool. */
status:bool=false;
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
w.u8(this.status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mecharm_interfacesMsgMecharmGripperStatus{const value=new Ros2Mecharm_interfacesMsgMecharmGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
return value;}
}
export class Ros2Mecharm_interfacesMsgMecharmGripperStatusCodec implements VendorCodec<Ros2Mecharm_interfacesMsgMecharmGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_2123b61d7b9ec218ee074473c26e5ec82fde818f40796047fc369cb84516e046";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mecharm_interfacesMsgMecharmGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesMsgMecharmGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesMsgMecharmGripperStatus>{const out=new VendorDecoded<Ros2Mecharm_interfacesMsgMecharmGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesMsgMecharmGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesMsgMecharmGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesMsgMecharmGripperStatus():changetype<Ros2Mecharm_interfacesMsgMecharmGripperStatus>(out.value);}
}
/** Complete original mecharm_interfaces/msg/MecharmPumpStatus; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesMsgMecharmPumpStatus {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field status, native scalar bool. */
status:bool=false;
/** Exact native field pin1, native scalar int8. */
pin1:i8=0;
/** Exact native field pin2, native scalar int8. */
pin2:i8=0;
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
w.u8(this.status?1:0);
w.integer(<u64>this.pin1,1);
w.integer(<u64>this.pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mecharm_interfacesMsgMecharmPumpStatus{const value=new Ros2Mecharm_interfacesMsgMecharmPumpStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin2=decoded;
}
return value;}
}
export class Ros2Mecharm_interfacesMsgMecharmPumpStatusCodec implements VendorCodec<Ros2Mecharm_interfacesMsgMecharmPumpStatus>{
static readonly codecIdentity:string="UWVMROS1_4d44c84c1fdd3f4db92d9975545348bfd727320e63c9fbfde5adf0b81ca04add";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros2Mecharm_interfacesMsgMecharmPumpStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesMsgMecharmPumpStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesMsgMecharmPumpStatus>{const out=new VendorDecoded<Ros2Mecharm_interfacesMsgMecharmPumpStatus>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesMsgMecharmPumpStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesMsgMecharmPumpStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesMsgMecharmPumpStatus():changetype<Ros2Mecharm_interfacesMsgMecharmPumpStatus>(out.value);}
}
/** Complete original mecharm_interfaces/msg/MecharmSetAngles; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesMsgMecharmSetAngles {
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
static readControl(r:Reader):Ros2Mecharm_interfacesMsgMecharmSetAngles{const value=new Ros2Mecharm_interfacesMsgMecharmSetAngles();
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
export class Ros2Mecharm_interfacesMsgMecharmSetAnglesCodec implements VendorCodec<Ros2Mecharm_interfacesMsgMecharmSetAngles>{
static readonly codecIdentity:string="UWVMROS1_54b634e4c5d0bc9fdf04a98d31816e24d3a9c5eb497bb38fb16e2de593cf81f5";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Mecharm_interfacesMsgMecharmSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesMsgMecharmSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesMsgMecharmSetAngles>{const out=new VendorDecoded<Ros2Mecharm_interfacesMsgMecharmSetAngles>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesMsgMecharmSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesMsgMecharmSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesMsgMecharmSetAngles():changetype<Ros2Mecharm_interfacesMsgMecharmSetAngles>(out.value);}
}
/** Complete original mecharm_interfaces/msg/MecharmSetCoords; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesMsgMecharmSetCoords {
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
static readControl(r:Reader):Ros2Mecharm_interfacesMsgMecharmSetCoords{const value=new Ros2Mecharm_interfacesMsgMecharmSetCoords();
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
export class Ros2Mecharm_interfacesMsgMecharmSetCoordsCodec implements VendorCodec<Ros2Mecharm_interfacesMsgMecharmSetCoords>{
static readonly codecIdentity:string="UWVMROS1_44944ee22cd4254167083cfdcd7377ee2f59dbe27414932c980670a471a6e7c2";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros2Mecharm_interfacesMsgMecharmSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesMsgMecharmSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesMsgMecharmSetCoords>{const out=new VendorDecoded<Ros2Mecharm_interfacesMsgMecharmSetCoords>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesMsgMecharmSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesMsgMecharmSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesMsgMecharmSetCoords():changetype<Ros2Mecharm_interfacesMsgMecharmSetCoords>(out.value);}
}
/** Complete original mecharm_interfaces/srv/GetAngles_Request; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvGetAngles_Request {
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
static readControl(r:Reader):Ros2Mecharm_interfacesSrvGetAngles_Request{const value=new Ros2Mecharm_interfacesSrvGetAngles_Request();
return value;}
}
export class Ros2Mecharm_interfacesSrvGetAngles_RequestCodec implements VendorCodec<Ros2Mecharm_interfacesSrvGetAngles_Request>{
static readonly codecIdentity:string="UWVMROS1_95b1d7786c03c6fec98fec0706088481fe9c7aa48c9f33f4325031d0437d97df";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Mecharm_interfacesSrvGetAngles_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvGetAngles_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvGetAngles_Request>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvGetAngles_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvGetAngles_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvGetAngles_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvGetAngles_Request():changetype<Ros2Mecharm_interfacesSrvGetAngles_Request>(out.value);}
}
/** Complete original mecharm_interfaces/srv/GetAngles_Response; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvGetAngles_Response {
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
static readControl(r:Reader):Ros2Mecharm_interfacesSrvGetAngles_Response{const value=new Ros2Mecharm_interfacesSrvGetAngles_Response();
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
export class Ros2Mecharm_interfacesSrvGetAngles_ResponseCodec implements VendorCodec<Ros2Mecharm_interfacesSrvGetAngles_Response>{
static readonly codecIdentity:string="UWVMROS1_b1a035847ac7a888053ba8de7494abb6d2cdc01f61e4d0a51cb3ef6f479adf61";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mecharm_interfacesSrvGetAngles_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvGetAngles_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvGetAngles_Response>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvGetAngles_Response>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvGetAngles_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvGetAngles_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvGetAngles_Response():changetype<Ros2Mecharm_interfacesSrvGetAngles_Response>(out.value);}
}
/** Complete original mecharm_interfaces/srv/GetCoords_Request; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvGetCoords_Request {
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
static readControl(r:Reader):Ros2Mecharm_interfacesSrvGetCoords_Request{const value=new Ros2Mecharm_interfacesSrvGetCoords_Request();
return value;}
}
export class Ros2Mecharm_interfacesSrvGetCoords_RequestCodec implements VendorCodec<Ros2Mecharm_interfacesSrvGetCoords_Request>{
static readonly codecIdentity:string="UWVMROS1_353827e7125cfa6c829f1138cc33901099a26cf73a8d045cf806b427994ccef1";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Mecharm_interfacesSrvGetCoords_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvGetCoords_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvGetCoords_Request>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvGetCoords_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvGetCoords_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvGetCoords_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvGetCoords_Request():changetype<Ros2Mecharm_interfacesSrvGetCoords_Request>(out.value);}
}
/** Complete original mecharm_interfaces/srv/GetCoords_Response; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvGetCoords_Response {
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
static readControl(r:Reader):Ros2Mecharm_interfacesSrvGetCoords_Response{const value=new Ros2Mecharm_interfacesSrvGetCoords_Response();
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
export class Ros2Mecharm_interfacesSrvGetCoords_ResponseCodec implements VendorCodec<Ros2Mecharm_interfacesSrvGetCoords_Response>{
static readonly codecIdentity:string="UWVMROS1_f7fcb8c4fdb47c06e93a8f67613490f7ab0ef2af7efb02092874722670b36c75";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mecharm_interfacesSrvGetCoords_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvGetCoords_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvGetCoords_Response>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvGetCoords_Response>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvGetCoords_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvGetCoords_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvGetCoords_Response():changetype<Ros2Mecharm_interfacesSrvGetCoords_Response>(out.value);}
}
/** Complete original mecharm_interfaces/srv/GripperStatus_Request; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvGripperStatus_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar bool. */
status:bool=false;
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
w.u8(this.status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mecharm_interfacesSrvGripperStatus_Request{const value=new Ros2Mecharm_interfacesSrvGripperStatus_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
return value;}
}
export class Ros2Mecharm_interfacesSrvGripperStatus_RequestCodec implements VendorCodec<Ros2Mecharm_interfacesSrvGripperStatus_Request>{
static readonly codecIdentity:string="UWVMROS1_044bf47303f5da0d0e17e795b8567b714d55680e8144c8e2e71ff7032859dfbb";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mecharm_interfacesSrvGripperStatus_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvGripperStatus_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvGripperStatus_Request>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvGripperStatus_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvGripperStatus_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvGripperStatus_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvGripperStatus_Request():changetype<Ros2Mecharm_interfacesSrvGripperStatus_Request>(out.value);}
}
/** Complete original mecharm_interfaces/srv/GripperStatus_Response; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvGripperStatus_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mecharm_interfacesSrvGripperStatus_Response{const value=new Ros2Mecharm_interfacesSrvGripperStatus_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mecharm_interfacesSrvGripperStatus_ResponseCodec implements VendorCodec<Ros2Mecharm_interfacesSrvGripperStatus_Response>{
static readonly codecIdentity:string="UWVMROS1_5aeee3c79bb6ebe4a4cb77f54552e97dcc155c65151a5965761a360d41acbea3";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mecharm_interfacesSrvGripperStatus_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvGripperStatus_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvGripperStatus_Response>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvGripperStatus_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvGripperStatus_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvGripperStatus_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvGripperStatus_Response():changetype<Ros2Mecharm_interfacesSrvGripperStatus_Response>(out.value);}
}
/** Complete original mecharm_interfaces/srv/PumpStatus_Request; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvPumpStatus_Request {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field status, native scalar bool. */
status:bool=false;
/** Exact native field pin1, native scalar int8. */
pin1:i8=0;
/** Exact native field pin2, native scalar int8. */
pin2:i8=0;
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
w.u8(this.status?1:0);
w.integer(<u64>this.pin1,1);
w.integer(<u64>this.pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mecharm_interfacesSrvPumpStatus_Request{const value=new Ros2Mecharm_interfacesSrvPumpStatus_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin2=decoded;
}
return value;}
}
export class Ros2Mecharm_interfacesSrvPumpStatus_RequestCodec implements VendorCodec<Ros2Mecharm_interfacesSrvPumpStatus_Request>{
static readonly codecIdentity:string="UWVMROS1_f80774682b7b1d447ff2f8dd7f6ac2f917dd4f71ccd3b1685a57b579fa41d6b9";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros2Mecharm_interfacesSrvPumpStatus_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvPumpStatus_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvPumpStatus_Request>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvPumpStatus_Request>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvPumpStatus_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvPumpStatus_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvPumpStatus_Request():changetype<Ros2Mecharm_interfacesSrvPumpStatus_Request>(out.value);}
}
/** Complete original mecharm_interfaces/srv/PumpStatus_Response; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvPumpStatus_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mecharm_interfacesSrvPumpStatus_Response{const value=new Ros2Mecharm_interfacesSrvPumpStatus_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mecharm_interfacesSrvPumpStatus_ResponseCodec implements VendorCodec<Ros2Mecharm_interfacesSrvPumpStatus_Response>{
static readonly codecIdentity:string="UWVMROS1_935c0e2aaca3f1e8aad8e4487484292b3d30ccdff180a9aaa50f454f75ecff1b";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mecharm_interfacesSrvPumpStatus_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvPumpStatus_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvPumpStatus_Response>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvPumpStatus_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvPumpStatus_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvPumpStatus_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvPumpStatus_Response():changetype<Ros2Mecharm_interfacesSrvPumpStatus_Response>(out.value);}
}
/** Complete original mecharm_interfaces/srv/SetAngles_Request; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvSetAngles_Request {
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
static readControl(r:Reader):Ros2Mecharm_interfacesSrvSetAngles_Request{const value=new Ros2Mecharm_interfacesSrvSetAngles_Request();
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
export class Ros2Mecharm_interfacesSrvSetAngles_RequestCodec implements VendorCodec<Ros2Mecharm_interfacesSrvSetAngles_Request>{
static readonly codecIdentity:string="UWVMROS1_86897875ef9a609098bc1eabba63aec7d960de380db98370e7a67626b2253eeb";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Mecharm_interfacesSrvSetAngles_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvSetAngles_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvSetAngles_Request>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvSetAngles_Request>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvSetAngles_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvSetAngles_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvSetAngles_Request():changetype<Ros2Mecharm_interfacesSrvSetAngles_Request>(out.value);}
}
/** Complete original mecharm_interfaces/srv/SetAngles_Response; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvSetAngles_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mecharm_interfacesSrvSetAngles_Response{const value=new Ros2Mecharm_interfacesSrvSetAngles_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mecharm_interfacesSrvSetAngles_ResponseCodec implements VendorCodec<Ros2Mecharm_interfacesSrvSetAngles_Response>{
static readonly codecIdentity:string="UWVMROS1_4782fe6a137dee35bf39d863be0aea118f61dcef092c11b013c44c6f4c129620";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mecharm_interfacesSrvSetAngles_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvSetAngles_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvSetAngles_Response>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvSetAngles_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvSetAngles_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvSetAngles_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvSetAngles_Response():changetype<Ros2Mecharm_interfacesSrvSetAngles_Response>(out.value);}
}
/** Complete original mecharm_interfaces/srv/SetCoords_Request; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvSetCoords_Request {
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
static readControl(r:Reader):Ros2Mecharm_interfacesSrvSetCoords_Request{const value=new Ros2Mecharm_interfacesSrvSetCoords_Request();
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
export class Ros2Mecharm_interfacesSrvSetCoords_RequestCodec implements VendorCodec<Ros2Mecharm_interfacesSrvSetCoords_Request>{
static readonly codecIdentity:string="UWVMROS1_a0daefe14076b73f1a64924ffc15a6b9647abe1b1887377e642f41b1a7920916";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros2Mecharm_interfacesSrvSetCoords_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvSetCoords_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvSetCoords_Request>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvSetCoords_Request>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvSetCoords_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvSetCoords_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvSetCoords_Request():changetype<Ros2Mecharm_interfacesSrvSetCoords_Request>(out.value);}
}
/** Complete original mecharm_interfaces/srv/SetCoords_Response; spellings are published in generation proof. */
export class Ros2Mecharm_interfacesSrvSetCoords_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mecharm_interfacesSrvSetCoords_Response{const value=new Ros2Mecharm_interfacesSrvSetCoords_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mecharm_interfacesSrvSetCoords_ResponseCodec implements VendorCodec<Ros2Mecharm_interfacesSrvSetCoords_Response>{
static readonly codecIdentity:string="UWVMROS1_43d02cfe61b6fb08f035e1f8ea78496cdcb488ed4c916f528415dce4ab810b7f";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mecharm_interfacesSrvSetCoords_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mecharm_interfacesSrvSetCoords_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mecharm_interfacesSrvSetCoords_Response>{const out=new VendorDecoded<Ros2Mecharm_interfacesSrvSetCoords_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mecharm_interfacesSrvSetCoords_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mecharm_interfacesSrvSetCoords_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mecharm_interfacesSrvSetCoords_Response():changetype<Ros2Mecharm_interfacesSrvSetCoords_Response>(out.value);}
}
/** Complete original mycobot_interfaces/msg/MycobotAngles; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesMsgMycobotAngles {
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
static readControl(r:Reader):Ros2Mycobot_interfacesMsgMycobotAngles{const value=new Ros2Mycobot_interfacesMsgMycobotAngles();
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
export class Ros2Mycobot_interfacesMsgMycobotAnglesCodec implements VendorCodec<Ros2Mycobot_interfacesMsgMycobotAngles>{
static readonly codecIdentity:string="UWVMROS1_38f099339ed5ee9fe56a5b44c3f0849e2ff7ff721b2a95b5cf49e3b92f6a9d82";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mycobot_interfacesMsgMycobotAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesMsgMycobotAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesMsgMycobotAngles>{const out=new VendorDecoded<Ros2Mycobot_interfacesMsgMycobotAngles>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesMsgMycobotAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesMsgMycobotAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesMsgMycobotAngles():changetype<Ros2Mycobot_interfacesMsgMycobotAngles>(out.value);}
}
/** Complete original mycobot_interfaces/msg/MycobotCoords; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesMsgMycobotCoords {
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
static readControl(r:Reader):Ros2Mycobot_interfacesMsgMycobotCoords{const value=new Ros2Mycobot_interfacesMsgMycobotCoords();
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
export class Ros2Mycobot_interfacesMsgMycobotCoordsCodec implements VendorCodec<Ros2Mycobot_interfacesMsgMycobotCoords>{
static readonly codecIdentity:string="UWVMROS1_8d9606c561db7680e816f5a19fd148c374f271acee8b261f7d060f370e0399d7";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mycobot_interfacesMsgMycobotCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesMsgMycobotCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesMsgMycobotCoords>{const out=new VendorDecoded<Ros2Mycobot_interfacesMsgMycobotCoords>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesMsgMycobotCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesMsgMycobotCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesMsgMycobotCoords():changetype<Ros2Mycobot_interfacesMsgMycobotCoords>(out.value);}
}
/** Complete original mycobot_interfaces/msg/MycobotGripperStatus; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesMsgMycobotGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar bool. */
status:bool=false;
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
w.u8(this.status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_interfacesMsgMycobotGripperStatus{const value=new Ros2Mycobot_interfacesMsgMycobotGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
return value;}
}
export class Ros2Mycobot_interfacesMsgMycobotGripperStatusCodec implements VendorCodec<Ros2Mycobot_interfacesMsgMycobotGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_4dc3ab413af3da01e26a3289ef9e5ef39488d00483631d701b4d617d9c9ba33d";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_interfacesMsgMycobotGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesMsgMycobotGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesMsgMycobotGripperStatus>{const out=new VendorDecoded<Ros2Mycobot_interfacesMsgMycobotGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesMsgMycobotGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesMsgMycobotGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesMsgMycobotGripperStatus():changetype<Ros2Mycobot_interfacesMsgMycobotGripperStatus>(out.value);}
}
/** Complete original mycobot_interfaces/msg/MycobotPumpStatus; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesMsgMycobotPumpStatus {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field status, native scalar bool. */
status:bool=false;
/** Exact native field pin1, native scalar int8. */
pin1:i8=0;
/** Exact native field pin2, native scalar int8. */
pin2:i8=0;
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
w.u8(this.status?1:0);
w.integer(<u64>this.pin1,1);
w.integer(<u64>this.pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_interfacesMsgMycobotPumpStatus{const value=new Ros2Mycobot_interfacesMsgMycobotPumpStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin2=decoded;
}
return value;}
}
export class Ros2Mycobot_interfacesMsgMycobotPumpStatusCodec implements VendorCodec<Ros2Mycobot_interfacesMsgMycobotPumpStatus>{
static readonly codecIdentity:string="UWVMROS1_860004dc3975ab44b5a1b0a77a9ff01ab58bfc71d7142d842d3bf9c636365e58";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros2Mycobot_interfacesMsgMycobotPumpStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesMsgMycobotPumpStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesMsgMycobotPumpStatus>{const out=new VendorDecoded<Ros2Mycobot_interfacesMsgMycobotPumpStatus>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesMsgMycobotPumpStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesMsgMycobotPumpStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesMsgMycobotPumpStatus():changetype<Ros2Mycobot_interfacesMsgMycobotPumpStatus>(out.value);}
}
/** Complete original mycobot_interfaces/msg/MycobotSetAngles; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesMsgMycobotSetAngles {
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
static readControl(r:Reader):Ros2Mycobot_interfacesMsgMycobotSetAngles{const value=new Ros2Mycobot_interfacesMsgMycobotSetAngles();
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
export class Ros2Mycobot_interfacesMsgMycobotSetAnglesCodec implements VendorCodec<Ros2Mycobot_interfacesMsgMycobotSetAngles>{
static readonly codecIdentity:string="UWVMROS1_798576c5f0b03e46e5cc5d2edf81bc7042042482ef574643ca136621346d731e";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Mycobot_interfacesMsgMycobotSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesMsgMycobotSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesMsgMycobotSetAngles>{const out=new VendorDecoded<Ros2Mycobot_interfacesMsgMycobotSetAngles>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesMsgMycobotSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesMsgMycobotSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesMsgMycobotSetAngles():changetype<Ros2Mycobot_interfacesMsgMycobotSetAngles>(out.value);}
}
/** Complete original mycobot_interfaces/msg/MycobotSetCoords; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesMsgMycobotSetCoords {
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
static readControl(r:Reader):Ros2Mycobot_interfacesMsgMycobotSetCoords{const value=new Ros2Mycobot_interfacesMsgMycobotSetCoords();
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
export class Ros2Mycobot_interfacesMsgMycobotSetCoordsCodec implements VendorCodec<Ros2Mycobot_interfacesMsgMycobotSetCoords>{
static readonly codecIdentity:string="UWVMROS1_462b7629172037a6959d08984896980c8c07ae5b6364c6d848b172dea23cab49";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros2Mycobot_interfacesMsgMycobotSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesMsgMycobotSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesMsgMycobotSetCoords>{const out=new VendorDecoded<Ros2Mycobot_interfacesMsgMycobotSetCoords>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesMsgMycobotSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesMsgMycobotSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesMsgMycobotSetCoords():changetype<Ros2Mycobot_interfacesMsgMycobotSetCoords>(out.value);}
}
/** Complete original mycobot_interfaces/srv/GetAngles_Request; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvGetAngles_Request {
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
static readControl(r:Reader):Ros2Mycobot_interfacesSrvGetAngles_Request{const value=new Ros2Mycobot_interfacesSrvGetAngles_Request();
return value;}
}
export class Ros2Mycobot_interfacesSrvGetAngles_RequestCodec implements VendorCodec<Ros2Mycobot_interfacesSrvGetAngles_Request>{
static readonly codecIdentity:string="UWVMROS1_ea433bdb6e3cb77923720a56bec24aaec62c88480248a4ed425370092474d38e";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Mycobot_interfacesSrvGetAngles_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvGetAngles_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvGetAngles_Request>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvGetAngles_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvGetAngles_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvGetAngles_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvGetAngles_Request():changetype<Ros2Mycobot_interfacesSrvGetAngles_Request>(out.value);}
}
/** Complete original mycobot_interfaces/srv/GetAngles_Response; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvGetAngles_Response {
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
static readControl(r:Reader):Ros2Mycobot_interfacesSrvGetAngles_Response{const value=new Ros2Mycobot_interfacesSrvGetAngles_Response();
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
export class Ros2Mycobot_interfacesSrvGetAngles_ResponseCodec implements VendorCodec<Ros2Mycobot_interfacesSrvGetAngles_Response>{
static readonly codecIdentity:string="UWVMROS1_14d6e2d8a6f7234719fae06d8807c651965387e252160df555ede0a070123b8a";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mycobot_interfacesSrvGetAngles_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvGetAngles_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvGetAngles_Response>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvGetAngles_Response>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvGetAngles_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvGetAngles_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvGetAngles_Response():changetype<Ros2Mycobot_interfacesSrvGetAngles_Response>(out.value);}
}
/** Complete original mycobot_interfaces/srv/GetCoords_Request; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvGetCoords_Request {
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
static readControl(r:Reader):Ros2Mycobot_interfacesSrvGetCoords_Request{const value=new Ros2Mycobot_interfacesSrvGetCoords_Request();
return value;}
}
export class Ros2Mycobot_interfacesSrvGetCoords_RequestCodec implements VendorCodec<Ros2Mycobot_interfacesSrvGetCoords_Request>{
static readonly codecIdentity:string="UWVMROS1_31ef03eefc143e9d040572e378a5803f44f68bac6d03fbb44ca27f87fb5fb076";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Mycobot_interfacesSrvGetCoords_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvGetCoords_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvGetCoords_Request>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvGetCoords_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvGetCoords_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvGetCoords_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvGetCoords_Request():changetype<Ros2Mycobot_interfacesSrvGetCoords_Request>(out.value);}
}
/** Complete original mycobot_interfaces/srv/GetCoords_Response; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvGetCoords_Response {
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
static readControl(r:Reader):Ros2Mycobot_interfacesSrvGetCoords_Response{const value=new Ros2Mycobot_interfacesSrvGetCoords_Response();
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
export class Ros2Mycobot_interfacesSrvGetCoords_ResponseCodec implements VendorCodec<Ros2Mycobot_interfacesSrvGetCoords_Response>{
static readonly codecIdentity:string="UWVMROS1_c9886b5cc5f3c48116a1c884f75ab4af49be46e2c0476ffc3aa8167ff96f7787";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mycobot_interfacesSrvGetCoords_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvGetCoords_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvGetCoords_Response>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvGetCoords_Response>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvGetCoords_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvGetCoords_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvGetCoords_Response():changetype<Ros2Mycobot_interfacesSrvGetCoords_Response>(out.value);}
}
/** Complete original mycobot_interfaces/srv/GripperStatus_Request; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvGripperStatus_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar bool. */
status:bool=false;
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
w.u8(this.status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_interfacesSrvGripperStatus_Request{const value=new Ros2Mycobot_interfacesSrvGripperStatus_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
return value;}
}
export class Ros2Mycobot_interfacesSrvGripperStatus_RequestCodec implements VendorCodec<Ros2Mycobot_interfacesSrvGripperStatus_Request>{
static readonly codecIdentity:string="UWVMROS1_6c5ae66e66f12834b7d623e07a5b3ef2b8127cd9cd83cb36d6793a68d921fcd5";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_interfacesSrvGripperStatus_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvGripperStatus_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvGripperStatus_Request>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvGripperStatus_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvGripperStatus_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvGripperStatus_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvGripperStatus_Request():changetype<Ros2Mycobot_interfacesSrvGripperStatus_Request>(out.value);}
}
/** Complete original mycobot_interfaces/srv/GripperStatus_Response; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvGripperStatus_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_interfacesSrvGripperStatus_Response{const value=new Ros2Mycobot_interfacesSrvGripperStatus_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mycobot_interfacesSrvGripperStatus_ResponseCodec implements VendorCodec<Ros2Mycobot_interfacesSrvGripperStatus_Response>{
static readonly codecIdentity:string="UWVMROS1_530f1e1acaa8cda6b43f8a8c0a38f5376eaebce3e99f7fb194a28143bed2d1ab";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_interfacesSrvGripperStatus_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvGripperStatus_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvGripperStatus_Response>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvGripperStatus_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvGripperStatus_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvGripperStatus_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvGripperStatus_Response():changetype<Ros2Mycobot_interfacesSrvGripperStatus_Response>(out.value);}
}
/** Complete original mycobot_interfaces/srv/PumpStatus_Request; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvPumpStatus_Request {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field status, native scalar bool. */
status:bool=false;
/** Exact native field pin1, native scalar int8. */
pin1:i8=0;
/** Exact native field pin2, native scalar int8. */
pin2:i8=0;
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
w.u8(this.status?1:0);
w.integer(<u64>this.pin1,1);
w.integer(<u64>this.pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_interfacesSrvPumpStatus_Request{const value=new Ros2Mycobot_interfacesSrvPumpStatus_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin2=decoded;
}
return value;}
}
export class Ros2Mycobot_interfacesSrvPumpStatus_RequestCodec implements VendorCodec<Ros2Mycobot_interfacesSrvPumpStatus_Request>{
static readonly codecIdentity:string="UWVMROS1_50b7fa285b93dd9e6b3b0eafda0afeb5911ab6e3b79d58f03f1cf1f76e732cd0";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros2Mycobot_interfacesSrvPumpStatus_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvPumpStatus_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvPumpStatus_Request>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvPumpStatus_Request>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvPumpStatus_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvPumpStatus_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvPumpStatus_Request():changetype<Ros2Mycobot_interfacesSrvPumpStatus_Request>(out.value);}
}
/** Complete original mycobot_interfaces/srv/PumpStatus_Response; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvPumpStatus_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_interfacesSrvPumpStatus_Response{const value=new Ros2Mycobot_interfacesSrvPumpStatus_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mycobot_interfacesSrvPumpStatus_ResponseCodec implements VendorCodec<Ros2Mycobot_interfacesSrvPumpStatus_Response>{
static readonly codecIdentity:string="UWVMROS1_b347313e1d4430f4e78a72fb8c35d53a39d1d391290373f84f1f8b2b44375eb0";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_interfacesSrvPumpStatus_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvPumpStatus_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvPumpStatus_Response>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvPumpStatus_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvPumpStatus_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvPumpStatus_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvPumpStatus_Response():changetype<Ros2Mycobot_interfacesSrvPumpStatus_Response>(out.value);}
}
/** Complete original mycobot_interfaces/srv/SetAngles_Request; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvSetAngles_Request {
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
static readControl(r:Reader):Ros2Mycobot_interfacesSrvSetAngles_Request{const value=new Ros2Mycobot_interfacesSrvSetAngles_Request();
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
export class Ros2Mycobot_interfacesSrvSetAngles_RequestCodec implements VendorCodec<Ros2Mycobot_interfacesSrvSetAngles_Request>{
static readonly codecIdentity:string="UWVMROS1_a06efd8b6af878d55c26fda8a7177a0b6c8b687f7e1b9f2e17e2ecd19c440310";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Mycobot_interfacesSrvSetAngles_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvSetAngles_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvSetAngles_Request>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvSetAngles_Request>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvSetAngles_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvSetAngles_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvSetAngles_Request():changetype<Ros2Mycobot_interfacesSrvSetAngles_Request>(out.value);}
}
/** Complete original mycobot_interfaces/srv/SetAngles_Response; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvSetAngles_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_interfacesSrvSetAngles_Response{const value=new Ros2Mycobot_interfacesSrvSetAngles_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mycobot_interfacesSrvSetAngles_ResponseCodec implements VendorCodec<Ros2Mycobot_interfacesSrvSetAngles_Response>{
static readonly codecIdentity:string="UWVMROS1_af4daae72e9ec28c8848b76b556c9387e65ba9698d8f0f6e5ba17522472ab665";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_interfacesSrvSetAngles_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvSetAngles_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvSetAngles_Response>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvSetAngles_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvSetAngles_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvSetAngles_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvSetAngles_Response():changetype<Ros2Mycobot_interfacesSrvSetAngles_Response>(out.value);}
}
/** Complete original mycobot_interfaces/srv/SetCoords_Request; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvSetCoords_Request {
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
static readControl(r:Reader):Ros2Mycobot_interfacesSrvSetCoords_Request{const value=new Ros2Mycobot_interfacesSrvSetCoords_Request();
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
export class Ros2Mycobot_interfacesSrvSetCoords_RequestCodec implements VendorCodec<Ros2Mycobot_interfacesSrvSetCoords_Request>{
static readonly codecIdentity:string="UWVMROS1_76a98d5db6f98255046f13855dc7468049894805d83e3d3a7fbdaace40292213";
static readonly maxWireBytes:i32=26;
encodeChecked(value:Ros2Mycobot_interfacesSrvSetCoords_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvSetCoords_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvSetCoords_Request>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvSetCoords_Request>();if(bytes.length>26){out.status=90;return out;}if(bytes.length<26){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvSetCoords_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvSetCoords_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvSetCoords_Request():changetype<Ros2Mycobot_interfacesSrvSetCoords_Request>(out.value);}
}
/** Complete original mycobot_interfaces/srv/SetCoords_Response; spellings are published in generation proof. */
export class Ros2Mycobot_interfacesSrvSetCoords_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_interfacesSrvSetCoords_Response{const value=new Ros2Mycobot_interfacesSrvSetCoords_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mycobot_interfacesSrvSetCoords_ResponseCodec implements VendorCodec<Ros2Mycobot_interfacesSrvSetCoords_Response>{
static readonly codecIdentity:string="UWVMROS1_75e3ae0e6db60a275b08e450c420969aa1ee05d0f34864e8fb6535d2d13613b9";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_interfacesSrvSetCoords_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_interfacesSrvSetCoords_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_interfacesSrvSetCoords_Response>{const out=new VendorDecoded<Ros2Mycobot_interfacesSrvSetCoords_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_interfacesSrvSetCoords_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_interfacesSrvSetCoords_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_interfacesSrvSetCoords_Response():changetype<Ros2Mycobot_interfacesSrvSetCoords_Response>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/msg/MycobotAngles; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesMsgMycobotAngles {
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
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesMsgMycobotAngles{const value=new Ros2Mycobot_pro450_interfacesMsgMycobotAngles();
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
export class Ros2Mycobot_pro450_interfacesMsgMycobotAnglesCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesMsgMycobotAngles>{
static readonly codecIdentity:string="UWVMROS1_2351398be27155ac63b8a3d15ffd0d3088dd7cf284182a9b494791b7780aa6da";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mycobot_pro450_interfacesMsgMycobotAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesMsgMycobotAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesMsgMycobotAngles>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesMsgMycobotAngles>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesMsgMycobotAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesMsgMycobotAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesMsgMycobotAngles():changetype<Ros2Mycobot_pro450_interfacesMsgMycobotAngles>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/msg/MycobotCoords; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesMsgMycobotCoords {
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
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesMsgMycobotCoords{const value=new Ros2Mycobot_pro450_interfacesMsgMycobotCoords();
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
export class Ros2Mycobot_pro450_interfacesMsgMycobotCoordsCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesMsgMycobotCoords>{
static readonly codecIdentity:string="UWVMROS1_b8513545b0975757c6b7f756a5cedfe8aa0c5d28d0e573adfde001b452e923de";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mycobot_pro450_interfacesMsgMycobotCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesMsgMycobotCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesMsgMycobotCoords>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesMsgMycobotCoords>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesMsgMycobotCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesMsgMycobotCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesMsgMycobotCoords():changetype<Ros2Mycobot_pro450_interfacesMsgMycobotCoords>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/msg/MycobotGripperStatus; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar bool. */
status:bool=false;
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
w.u8(this.status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus{const value=new Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
return value;}
}
export class Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatusCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_1d097932f2e6b9b4e73ff838b5223b585b230e4edc5c229b310eb00404f6e784";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus():changetype<Ros2Mycobot_pro450_interfacesMsgMycobotGripperStatus>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/msg/MycobotSetAngles; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles {
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
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles{const value=new Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles();
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
export class Ros2Mycobot_pro450_interfacesMsgMycobotSetAnglesCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles>{
static readonly codecIdentity:string="UWVMROS1_23d208d7fa8f7b7488be7c7db487698db4b227ef65e99c55d996901d27c3e591";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles():changetype<Ros2Mycobot_pro450_interfacesMsgMycobotSetAngles>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/msg/MycobotSetCoords; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords {
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
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords{const value=new Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords();
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
export class Ros2Mycobot_pro450_interfacesMsgMycobotSetCoordsCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords>{
static readonly codecIdentity:string="UWVMROS1_60f40a2f6a577e5785fd69e0cbe9128b301aeaec6b70fd678c71e7f9a5426cea";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords():changetype<Ros2Mycobot_pro450_interfacesMsgMycobotSetCoords>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/GetAngles_Request; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvGetAngles_Request {
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
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvGetAngles_Request{const value=new Ros2Mycobot_pro450_interfacesSrvGetAngles_Request();
return value;}
}
export class Ros2Mycobot_pro450_interfacesSrvGetAngles_RequestCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvGetAngles_Request>{
static readonly codecIdentity:string="UWVMROS1_d2d8197675b9dadd50ae1a22b165a84e2e76b293b28cc70bec211225c60626c4";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvGetAngles_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvGetAngles_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetAngles_Request>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetAngles_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvGetAngles_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvGetAngles_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvGetAngles_Request():changetype<Ros2Mycobot_pro450_interfacesSrvGetAngles_Request>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/GetAngles_Response; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvGetAngles_Response {
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
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvGetAngles_Response{const value=new Ros2Mycobot_pro450_interfacesSrvGetAngles_Response();
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
export class Ros2Mycobot_pro450_interfacesSrvGetAngles_ResponseCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvGetAngles_Response>{
static readonly codecIdentity:string="UWVMROS1_13056bbe3bdef4feb8488783e1c5cf725d1fb94006f36f2221f61081084227f8";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvGetAngles_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvGetAngles_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetAngles_Response>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetAngles_Response>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvGetAngles_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvGetAngles_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvGetAngles_Response():changetype<Ros2Mycobot_pro450_interfacesSrvGetAngles_Response>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/GetCoords_Request; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvGetCoords_Request {
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
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvGetCoords_Request{const value=new Ros2Mycobot_pro450_interfacesSrvGetCoords_Request();
return value;}
}
export class Ros2Mycobot_pro450_interfacesSrvGetCoords_RequestCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvGetCoords_Request>{
static readonly codecIdentity:string="UWVMROS1_bda408ece90e671ce58ae428f79f01c1429c3b3aa3c1ef2efe9dc045faa9e3c2";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvGetCoords_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvGetCoords_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetCoords_Request>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetCoords_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvGetCoords_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvGetCoords_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvGetCoords_Request():changetype<Ros2Mycobot_pro450_interfacesSrvGetCoords_Request>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/GetCoords_Response; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvGetCoords_Response {
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
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvGetCoords_Response{const value=new Ros2Mycobot_pro450_interfacesSrvGetCoords_Response();
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
export class Ros2Mycobot_pro450_interfacesSrvGetCoords_ResponseCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvGetCoords_Response>{
static readonly codecIdentity:string="UWVMROS1_01e8a3ef4c048100af32d92cd027769e32eacd82dba36dc3bde30c70cb44afdf";
static readonly maxWireBytes:i32=24;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvGetCoords_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvGetCoords_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetCoords_Response>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetCoords_Response>();if(bytes.length>24){out.status=90;return out;}if(bytes.length<24){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvGetCoords_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvGetCoords_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvGetCoords_Response():changetype<Ros2Mycobot_pro450_interfacesSrvGetCoords_Response>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/GetGripperValue_Request; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request {
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
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request{const value=new Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request();
return value;}
}
export class Ros2Mycobot_pro450_interfacesSrvGetGripperValue_RequestCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request>{
static readonly codecIdentity:string="UWVMROS1_17cb5e3826409b661c2ad4bad8fde2b4c826dde95d8600e1a17bcf4908eca125";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request():changetype<Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Request>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/GetGripperValue_Response; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field gripper_angle, native scalar int8. */
gripper_angle:i8=0;
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
w.integer(<u64>this.gripper_angle,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response{const value=new Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response();
{
const decoded=<i8>r.integer(1);
value.gripper_angle=decoded;
}
return value;}
}
export class Ros2Mycobot_pro450_interfacesSrvGetGripperValue_ResponseCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response>{
static readonly codecIdentity:string="UWVMROS1_6b210e18d0696735196fb2e166b7168165dba6f03b97759a437fc8dc08ab2676";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response():changetype<Ros2Mycobot_pro450_interfacesSrvGetGripperValue_Response>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/GripperStatus_Request; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar bool. */
status:bool=false;
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
w.u8(this.status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request{const value=new Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
return value;}
}
export class Ros2Mycobot_pro450_interfacesSrvGripperStatus_RequestCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request>{
static readonly codecIdentity:string="UWVMROS1_edc3352d37effe3923c55a024d6acac85a3cb49350974ea35934efa41555ccac";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request():changetype<Ros2Mycobot_pro450_interfacesSrvGripperStatus_Request>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/GripperStatus_Response; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response{const value=new Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mycobot_pro450_interfacesSrvGripperStatus_ResponseCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response>{
static readonly codecIdentity:string="UWVMROS1_65166c2943fd319555cc1a2dcdaf4475b1e7779960907fe5ab6d1b5dc5536fc2";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response():changetype<Ros2Mycobot_pro450_interfacesSrvGripperStatus_Response>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/SetAngles_Request; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvSetAngles_Request {
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
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvSetAngles_Request{const value=new Ros2Mycobot_pro450_interfacesSrvSetAngles_Request();
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
export class Ros2Mycobot_pro450_interfacesSrvSetAngles_RequestCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvSetAngles_Request>{
static readonly codecIdentity:string="UWVMROS1_ec2659577304938ca4f78c1b4add50190766d8d1b551397668ef7ad03e2ccb09";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvSetAngles_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvSetAngles_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvSetAngles_Request>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvSetAngles_Request>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvSetAngles_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvSetAngles_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvSetAngles_Request():changetype<Ros2Mycobot_pro450_interfacesSrvSetAngles_Request>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/SetAngles_Response; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvSetAngles_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvSetAngles_Response{const value=new Ros2Mycobot_pro450_interfacesSrvSetAngles_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mycobot_pro450_interfacesSrvSetAngles_ResponseCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvSetAngles_Response>{
static readonly codecIdentity:string="UWVMROS1_28dc59c06324913c1ccbe286ca05b9f47585653a8f086fc5d99a01f6e9ee08b4";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvSetAngles_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvSetAngles_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvSetAngles_Response>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvSetAngles_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvSetAngles_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvSetAngles_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvSetAngles_Response():changetype<Ros2Mycobot_pro450_interfacesSrvSetAngles_Response>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/SetCoords_Request; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvSetCoords_Request {
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
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvSetCoords_Request{const value=new Ros2Mycobot_pro450_interfacesSrvSetCoords_Request();
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
export class Ros2Mycobot_pro450_interfacesSrvSetCoords_RequestCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvSetCoords_Request>{
static readonly codecIdentity:string="UWVMROS1_e0c550200472f77aa582a46a5149c05c95648615a306f254599f820992890c81";
static readonly maxWireBytes:i32=25;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvSetCoords_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvSetCoords_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvSetCoords_Request>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvSetCoords_Request>();if(bytes.length>25){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvSetCoords_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvSetCoords_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvSetCoords_Request():changetype<Ros2Mycobot_pro450_interfacesSrvSetCoords_Request>(out.value);}
}
/** Complete original mycobot_pro450_interfaces/srv/SetCoords_Response; spellings are published in generation proof. */
export class Ros2Mycobot_pro450_interfacesSrvSetCoords_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mycobot_pro450_interfacesSrvSetCoords_Response{const value=new Ros2Mycobot_pro450_interfacesSrvSetCoords_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mycobot_pro450_interfacesSrvSetCoords_ResponseCodec implements VendorCodec<Ros2Mycobot_pro450_interfacesSrvSetCoords_Response>{
static readonly codecIdentity:string="UWVMROS1_9a5ee934233519095e1b28568490f8d958c319e2544976fae0b7de53a54a3f51";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mycobot_pro450_interfacesSrvSetCoords_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mycobot_pro450_interfacesSrvSetCoords_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mycobot_pro450_interfacesSrvSetCoords_Response>{const out=new VendorDecoded<Ros2Mycobot_pro450_interfacesSrvSetCoords_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mycobot_pro450_interfacesSrvSetCoords_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mycobot_pro450_interfacesSrvSetCoords_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mycobot_pro450_interfacesSrvSetCoords_Response():changetype<Ros2Mycobot_pro450_interfacesSrvSetCoords_Response>(out.value);}
}
/** Complete original mypalletizer_interfaces/msg/MypalAngles; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesMsgMypalAngles {
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
static readControl(r:Reader):Ros2Mypalletizer_interfacesMsgMypalAngles{const value=new Ros2Mypalletizer_interfacesMsgMypalAngles();
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
export class Ros2Mypalletizer_interfacesMsgMypalAnglesCodec implements VendorCodec<Ros2Mypalletizer_interfacesMsgMypalAngles>{
static readonly codecIdentity:string="UWVMROS1_0a2dc324515448310b0f026fb26c6c5c6da452e5e7067096e315abe3ab86a35d";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Mypalletizer_interfacesMsgMypalAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesMsgMypalAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalAngles>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalAngles>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesMsgMypalAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesMsgMypalAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesMsgMypalAngles():changetype<Ros2Mypalletizer_interfacesMsgMypalAngles>(out.value);}
}
/** Complete original mypalletizer_interfaces/msg/MypalCoords; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesMsgMypalCoords {
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
static readControl(r:Reader):Ros2Mypalletizer_interfacesMsgMypalCoords{const value=new Ros2Mypalletizer_interfacesMsgMypalCoords();
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
export class Ros2Mypalletizer_interfacesMsgMypalCoordsCodec implements VendorCodec<Ros2Mypalletizer_interfacesMsgMypalCoords>{
static readonly codecIdentity:string="UWVMROS1_839491548a07528380403fa31c89889ce82f28b2f82e6fd9474835659714b4f7";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Mypalletizer_interfacesMsgMypalCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesMsgMypalCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalCoords>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalCoords>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesMsgMypalCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesMsgMypalCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesMsgMypalCoords():changetype<Ros2Mypalletizer_interfacesMsgMypalCoords>(out.value);}
}
/** Complete original mypalletizer_interfaces/msg/MypalGripperStatus; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesMsgMypalGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar bool. */
status:bool=false;
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
w.u8(this.status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mypalletizer_interfacesMsgMypalGripperStatus{const value=new Ros2Mypalletizer_interfacesMsgMypalGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
return value;}
}
export class Ros2Mypalletizer_interfacesMsgMypalGripperStatusCodec implements VendorCodec<Ros2Mypalletizer_interfacesMsgMypalGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_f6e4ad6193272d6401bbbfcc20c0f38b6a147a81d9de1ad956d7c154f29daeb0";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mypalletizer_interfacesMsgMypalGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesMsgMypalGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalGripperStatus>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesMsgMypalGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesMsgMypalGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesMsgMypalGripperStatus():changetype<Ros2Mypalletizer_interfacesMsgMypalGripperStatus>(out.value);}
}
/** Complete original mypalletizer_interfaces/msg/MypalPumpStatus; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesMsgMypalPumpStatus {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field status, native scalar bool. */
status:bool=false;
/** Exact native field pin1, native scalar int8. */
pin1:i8=0;
/** Exact native field pin2, native scalar int8. */
pin2:i8=0;
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
w.u8(this.status?1:0);
w.integer(<u64>this.pin1,1);
w.integer(<u64>this.pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mypalletizer_interfacesMsgMypalPumpStatus{const value=new Ros2Mypalletizer_interfacesMsgMypalPumpStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin2=decoded;
}
return value;}
}
export class Ros2Mypalletizer_interfacesMsgMypalPumpStatusCodec implements VendorCodec<Ros2Mypalletizer_interfacesMsgMypalPumpStatus>{
static readonly codecIdentity:string="UWVMROS1_008dfd13cad42029330909fb7dc68392ba7f6678ad1baa14a41aad1e759f523c";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros2Mypalletizer_interfacesMsgMypalPumpStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesMsgMypalPumpStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalPumpStatus>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalPumpStatus>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesMsgMypalPumpStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesMsgMypalPumpStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesMsgMypalPumpStatus():changetype<Ros2Mypalletizer_interfacesMsgMypalPumpStatus>(out.value);}
}
/** Complete original mypalletizer_interfaces/msg/MypalSetAngles; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesMsgMypalSetAngles {
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
static readControl(r:Reader):Ros2Mypalletizer_interfacesMsgMypalSetAngles{const value=new Ros2Mypalletizer_interfacesMsgMypalSetAngles();
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
export class Ros2Mypalletizer_interfacesMsgMypalSetAnglesCodec implements VendorCodec<Ros2Mypalletizer_interfacesMsgMypalSetAngles>{
static readonly codecIdentity:string="UWVMROS1_b9fdd6ac956fdeb4934ff41982e616f7c0c45b11169b6d60ba74fbcc61458bb5";
static readonly maxWireBytes:i32=17;
encodeChecked(value:Ros2Mypalletizer_interfacesMsgMypalSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesMsgMypalSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalSetAngles>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalSetAngles>();if(bytes.length>17){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesMsgMypalSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesMsgMypalSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesMsgMypalSetAngles():changetype<Ros2Mypalletizer_interfacesMsgMypalSetAngles>(out.value);}
}
/** Complete original mypalletizer_interfaces/msg/MypalSetCoords; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesMsgMypalSetCoords {
static readonly maxWireBytes:i32=17;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=17;
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
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.rx))return -22;
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
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mypalletizer_interfacesMsgMypalSetCoords{const value=new Ros2Mypalletizer_interfacesMsgMypalSetCoords();
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
return value;}
}
export class Ros2Mypalletizer_interfacesMsgMypalSetCoordsCodec implements VendorCodec<Ros2Mypalletizer_interfacesMsgMypalSetCoords>{
static readonly codecIdentity:string="UWVMROS1_0c1b65f543cdd239a5a56a7839ac607c5310cd4145ef6a6da5639a9af8ba4c79";
static readonly maxWireBytes:i32=17;
encodeChecked(value:Ros2Mypalletizer_interfacesMsgMypalSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesMsgMypalSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalSetCoords>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesMsgMypalSetCoords>();if(bytes.length>17){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesMsgMypalSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesMsgMypalSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesMsgMypalSetCoords():changetype<Ros2Mypalletizer_interfacesMsgMypalSetCoords>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/GetAngles_Request; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvGetAngles_Request {
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
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvGetAngles_Request{const value=new Ros2Mypalletizer_interfacesSrvGetAngles_Request();
return value;}
}
export class Ros2Mypalletizer_interfacesSrvGetAngles_RequestCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvGetAngles_Request>{
static readonly codecIdentity:string="UWVMROS1_c8124a1063d47b24dae2e279f51d4e48ebf9936a5e305b81b53387a4be344f78";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvGetAngles_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvGetAngles_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvGetAngles_Request>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvGetAngles_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvGetAngles_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvGetAngles_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvGetAngles_Request():changetype<Ros2Mypalletizer_interfacesSrvGetAngles_Request>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/GetAngles_Response; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvGetAngles_Response {
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
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvGetAngles_Response{const value=new Ros2Mypalletizer_interfacesSrvGetAngles_Response();
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
export class Ros2Mypalletizer_interfacesSrvGetAngles_ResponseCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvGetAngles_Response>{
static readonly codecIdentity:string="UWVMROS1_33812a320c14891a5bc76a86ccbb0a5507cfbcc3706e01bc304263edabc6f75d";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvGetAngles_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvGetAngles_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvGetAngles_Response>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvGetAngles_Response>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvGetAngles_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvGetAngles_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvGetAngles_Response():changetype<Ros2Mypalletizer_interfacesSrvGetAngles_Response>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/GetCoords_Request; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvGetCoords_Request {
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
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvGetCoords_Request{const value=new Ros2Mypalletizer_interfacesSrvGetCoords_Request();
return value;}
}
export class Ros2Mypalletizer_interfacesSrvGetCoords_RequestCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvGetCoords_Request>{
static readonly codecIdentity:string="UWVMROS1_b3db3a0e7d2aed097027798a897caf833f9cc1b8bb027f98c0db016968fa4440";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvGetCoords_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvGetCoords_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvGetCoords_Request>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvGetCoords_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvGetCoords_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvGetCoords_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvGetCoords_Request():changetype<Ros2Mypalletizer_interfacesSrvGetCoords_Request>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/GetCoords_Response; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvGetCoords_Response {
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
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvGetCoords_Response{const value=new Ros2Mypalletizer_interfacesSrvGetCoords_Response();
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
export class Ros2Mypalletizer_interfacesSrvGetCoords_ResponseCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvGetCoords_Response>{
static readonly codecIdentity:string="UWVMROS1_9d237cfa03c4b3c17d99fe0a9c08d1cafa3e8d0e404191e2169eea8683c54c84";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvGetCoords_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvGetCoords_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvGetCoords_Response>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvGetCoords_Response>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvGetCoords_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvGetCoords_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvGetCoords_Response():changetype<Ros2Mypalletizer_interfacesSrvGetCoords_Response>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/GripperStatus_Request; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvGripperStatus_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar bool. */
status:bool=false;
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
w.u8(this.status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvGripperStatus_Request{const value=new Ros2Mypalletizer_interfacesSrvGripperStatus_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
return value;}
}
export class Ros2Mypalletizer_interfacesSrvGripperStatus_RequestCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvGripperStatus_Request>{
static readonly codecIdentity:string="UWVMROS1_b6d4c5991599ec4ac56e5f65dacd5a1ab89f073e9f07dc2b03692f4289fdcc9d";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvGripperStatus_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvGripperStatus_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvGripperStatus_Request>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvGripperStatus_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvGripperStatus_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvGripperStatus_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvGripperStatus_Request():changetype<Ros2Mypalletizer_interfacesSrvGripperStatus_Request>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/GripperStatus_Response; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvGripperStatus_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvGripperStatus_Response{const value=new Ros2Mypalletizer_interfacesSrvGripperStatus_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mypalletizer_interfacesSrvGripperStatus_ResponseCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvGripperStatus_Response>{
static readonly codecIdentity:string="UWVMROS1_bd0c427dc564ff7201019c6d33c7d0c743a65d263c67eb420b5e653d48e4daf4";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvGripperStatus_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvGripperStatus_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvGripperStatus_Response>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvGripperStatus_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvGripperStatus_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvGripperStatus_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvGripperStatus_Response():changetype<Ros2Mypalletizer_interfacesSrvGripperStatus_Response>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/PumpStatus_Request; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvPumpStatus_Request {
static readonly maxWireBytes:i32=3;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=3;
/** Exact native field status, native scalar bool. */
status:bool=false;
/** Exact native field pin1, native scalar int8. */
pin1:i8=0;
/** Exact native field pin2, native scalar int8. */
pin2:i8=0;
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
w.u8(this.status?1:0);
w.integer(<u64>this.pin1,1);
w.integer(<u64>this.pin2,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvPumpStatus_Request{const value=new Ros2Mypalletizer_interfacesSrvPumpStatus_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin1=decoded;
}
{
const decoded=<i8>r.integer(1);
value.pin2=decoded;
}
return value;}
}
export class Ros2Mypalletizer_interfacesSrvPumpStatus_RequestCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvPumpStatus_Request>{
static readonly codecIdentity:string="UWVMROS1_6b14af554cb5c65c9411b2c6a76d49cb7cbe06dedcd26d47804d00d12ccf2e2f";
static readonly maxWireBytes:i32=3;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvPumpStatus_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvPumpStatus_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvPumpStatus_Request>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvPumpStatus_Request>();if(bytes.length>3){out.status=90;return out;}if(bytes.length<3){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvPumpStatus_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvPumpStatus_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvPumpStatus_Request():changetype<Ros2Mypalletizer_interfacesSrvPumpStatus_Request>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/PumpStatus_Response; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvPumpStatus_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvPumpStatus_Response{const value=new Ros2Mypalletizer_interfacesSrvPumpStatus_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mypalletizer_interfacesSrvPumpStatus_ResponseCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvPumpStatus_Response>{
static readonly codecIdentity:string="UWVMROS1_187a703ce648bc82a276961d82ed8abd5365d8db7af3bb211e590620e1bb2b45";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvPumpStatus_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvPumpStatus_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvPumpStatus_Response>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvPumpStatus_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvPumpStatus_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvPumpStatus_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvPumpStatus_Response():changetype<Ros2Mypalletizer_interfacesSrvPumpStatus_Response>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/SetAngles_Request; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvSetAngles_Request {
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
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvSetAngles_Request{const value=new Ros2Mypalletizer_interfacesSrvSetAngles_Request();
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
export class Ros2Mypalletizer_interfacesSrvSetAngles_RequestCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvSetAngles_Request>{
static readonly codecIdentity:string="UWVMROS1_3997bb19c183b59fbca1675695ea5ee548a247a34f9682dae3fb68fdf1a155ec";
static readonly maxWireBytes:i32=17;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvSetAngles_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvSetAngles_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvSetAngles_Request>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvSetAngles_Request>();if(bytes.length>17){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvSetAngles_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvSetAngles_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvSetAngles_Request():changetype<Ros2Mypalletizer_interfacesSrvSetAngles_Request>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/SetAngles_Response; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvSetAngles_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvSetAngles_Response{const value=new Ros2Mypalletizer_interfacesSrvSetAngles_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mypalletizer_interfacesSrvSetAngles_ResponseCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvSetAngles_Response>{
static readonly codecIdentity:string="UWVMROS1_a7952f549b35aa140adac4f18bbe8538c0f6230aa884459091cd74720ab757c9";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvSetAngles_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvSetAngles_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvSetAngles_Response>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvSetAngles_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvSetAngles_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvSetAngles_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvSetAngles_Response():changetype<Ros2Mypalletizer_interfacesSrvSetAngles_Response>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/SetCoords_Request; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvSetCoords_Request {
static readonly maxWireBytes:i32=17;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=17;
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
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
if(!isFinite(this.x))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.y))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.z))return -22;
if(4>17-n)return -90;n+=4;
if(n>17)return -90;
}
{
if(!isFinite(this.rx))return -22;
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
w.integer(<u64>reinterpret<u32>(this.x),4);
w.integer(<u64>reinterpret<u32>(this.y),4);
w.integer(<u64>reinterpret<u32>(this.z),4);
w.integer(<u64>reinterpret<u32>(this.rx),4);
w.integer(<u64>this.speed,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvSetCoords_Request{const value=new Ros2Mypalletizer_interfacesSrvSetCoords_Request();
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
return value;}
}
export class Ros2Mypalletizer_interfacesSrvSetCoords_RequestCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvSetCoords_Request>{
static readonly codecIdentity:string="UWVMROS1_c72f4f1cb71f1ad1076bcc89499abbcfc5eab173fadd19c9a266baf1c32d1541";
static readonly maxWireBytes:i32=17;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvSetCoords_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvSetCoords_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvSetCoords_Request>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvSetCoords_Request>();if(bytes.length>17){out.status=90;return out;}if(bytes.length<17){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvSetCoords_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvSetCoords_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvSetCoords_Request():changetype<Ros2Mypalletizer_interfacesSrvSetCoords_Request>(out.value);}
}
/** Complete original mypalletizer_interfaces/srv/SetCoords_Response; spellings are published in generation proof. */
export class Ros2Mypalletizer_interfacesSrvSetCoords_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Mypalletizer_interfacesSrvSetCoords_Response{const value=new Ros2Mypalletizer_interfacesSrvSetCoords_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Mypalletizer_interfacesSrvSetCoords_ResponseCodec implements VendorCodec<Ros2Mypalletizer_interfacesSrvSetCoords_Response>{
static readonly codecIdentity:string="UWVMROS1_0cedaf9087ba0ae3726b9dd0d304665a226fc6df2ee068f6ac2b1c12ce88cb5b";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Mypalletizer_interfacesSrvSetCoords_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Mypalletizer_interfacesSrvSetCoords_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Mypalletizer_interfacesSrvSetCoords_Response>{const out=new VendorDecoded<Ros2Mypalletizer_interfacesSrvSetCoords_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Mypalletizer_interfacesSrvSetCoords_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Mypalletizer_interfacesSrvSetCoords_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Mypalletizer_interfacesSrvSetCoords_Response():changetype<Ros2Mypalletizer_interfacesSrvSetCoords_Response>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/msg/MycobotAngles; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesMsgMycobotAngles {
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
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesMsgMycobotAngles{const value=new Ros2Ultraarm_p1_interfacesMsgMycobotAngles();
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
export class Ros2Ultraarm_p1_interfacesMsgMycobotAnglesCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesMsgMycobotAngles>{
static readonly codecIdentity:string="UWVMROS1_75965ba712c12f465c2424b540359dbc4c88e2e85aa772698ee040db7c32a945";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Ultraarm_p1_interfacesMsgMycobotAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesMsgMycobotAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesMsgMycobotAngles>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesMsgMycobotAngles>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesMsgMycobotAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesMsgMycobotAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesMsgMycobotAngles():changetype<Ros2Ultraarm_p1_interfacesMsgMycobotAngles>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/msg/MycobotCoords; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesMsgMycobotCoords {
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
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesMsgMycobotCoords{const value=new Ros2Ultraarm_p1_interfacesMsgMycobotCoords();
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
export class Ros2Ultraarm_p1_interfacesMsgMycobotCoordsCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesMsgMycobotCoords>{
static readonly codecIdentity:string="UWVMROS1_c79774c9af7fb4c150cb18c2510df52bbc7bad3c05b604abf90e19087aa05874";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Ultraarm_p1_interfacesMsgMycobotCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesMsgMycobotCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesMsgMycobotCoords>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesMsgMycobotCoords>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesMsgMycobotCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesMsgMycobotCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesMsgMycobotCoords():changetype<Ros2Ultraarm_p1_interfacesMsgMycobotCoords>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/msg/MycobotGripperStatus; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar bool. */
status:bool=false;
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
w.u8(this.status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus{const value=new Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
return value;}
}
export class Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatusCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus>{
static readonly codecIdentity:string="UWVMROS1_1efb83ff22c5eb211fe92355cb5358cb991a373693d33180362e43a2ef56d22f";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus():changetype<Ros2Ultraarm_p1_interfacesMsgMycobotGripperStatus>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/msg/MycobotSetAngles; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles {
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
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles{const value=new Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles();
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
export class Ros2Ultraarm_p1_interfacesMsgMycobotSetAnglesCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles>{
static readonly codecIdentity:string="UWVMROS1_cb53aab11e200533386b764a423f86bd7a24c65105236945c6b27ba7a9c3221c";
static readonly maxWireBytes:i32=20;
encodeChecked(value:Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles>();if(bytes.length>20){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles():changetype<Ros2Ultraarm_p1_interfacesMsgMycobotSetAngles>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/msg/MycobotSetCoords; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords {
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
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords{const value=new Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords();
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
export class Ros2Ultraarm_p1_interfacesMsgMycobotSetCoordsCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords>{
static readonly codecIdentity:string="UWVMROS1_fca21ab6eed31f03f0f7c9c6010d893f709f1eebb1bf07c1911452da1c50bbee";
static readonly maxWireBytes:i32=20;
encodeChecked(value:Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords>();if(bytes.length>20){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords():changetype<Ros2Ultraarm_p1_interfacesMsgMycobotSetCoords>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/GetAngles_Request; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvGetAngles_Request {
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
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvGetAngles_Request{const value=new Ros2Ultraarm_p1_interfacesSrvGetAngles_Request();
return value;}
}
export class Ros2Ultraarm_p1_interfacesSrvGetAngles_RequestCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvGetAngles_Request>{
static readonly codecIdentity:string="UWVMROS1_b2ff68f6919df813ddf49ad9337c2345e3c4e70242ad81f6440d5cd1cd612ff9";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvGetAngles_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvGetAngles_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetAngles_Request>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetAngles_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvGetAngles_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvGetAngles_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvGetAngles_Request():changetype<Ros2Ultraarm_p1_interfacesSrvGetAngles_Request>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/GetAngles_Response; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvGetAngles_Response {
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
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvGetAngles_Response{const value=new Ros2Ultraarm_p1_interfacesSrvGetAngles_Response();
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
export class Ros2Ultraarm_p1_interfacesSrvGetAngles_ResponseCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvGetAngles_Response>{
static readonly codecIdentity:string="UWVMROS1_cc8b4259c50ed82d6f3b3bbb9470ac089b127b70ace12457cb41fccb7ae67cbc";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvGetAngles_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvGetAngles_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetAngles_Response>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetAngles_Response>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvGetAngles_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvGetAngles_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvGetAngles_Response():changetype<Ros2Ultraarm_p1_interfacesSrvGetAngles_Response>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/GetCoords_Request; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvGetCoords_Request {
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
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvGetCoords_Request{const value=new Ros2Ultraarm_p1_interfacesSrvGetCoords_Request();
return value;}
}
export class Ros2Ultraarm_p1_interfacesSrvGetCoords_RequestCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvGetCoords_Request>{
static readonly codecIdentity:string="UWVMROS1_0ed9a0f7124cb00e62b71aecaedb60ea8832a94aaf4dc8f1de943d91dbf74e01";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvGetCoords_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvGetCoords_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetCoords_Request>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetCoords_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvGetCoords_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvGetCoords_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvGetCoords_Request():changetype<Ros2Ultraarm_p1_interfacesSrvGetCoords_Request>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/GetCoords_Response; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvGetCoords_Response {
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
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvGetCoords_Response{const value=new Ros2Ultraarm_p1_interfacesSrvGetCoords_Response();
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
export class Ros2Ultraarm_p1_interfacesSrvGetCoords_ResponseCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvGetCoords_Response>{
static readonly codecIdentity:string="UWVMROS1_47ef794ffd2a3a4125ce3c9cbbd6b0d85fbb42cbfc888dd8fec39020cd454565";
static readonly maxWireBytes:i32=16;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvGetCoords_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvGetCoords_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetCoords_Response>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetCoords_Response>();if(bytes.length>16){out.status=90;return out;}if(bytes.length<16){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvGetCoords_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvGetCoords_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvGetCoords_Response():changetype<Ros2Ultraarm_p1_interfacesSrvGetCoords_Response>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/GetGripperValue_Request; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request {
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
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request{const value=new Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request();
return value;}
}
export class Ros2Ultraarm_p1_interfacesSrvGetGripperValue_RequestCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request>{
static readonly codecIdentity:string="UWVMROS1_dad3d4af78250b2b8e44424ee1bb532b1e24bd9b867417fa428dec761fcf4be9";
static readonly maxWireBytes:i32=0;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request>();if(bytes.length>0){out.status=90;return out;}if(bytes.length<0){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request():changetype<Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Request>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/GetGripperValue_Response; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field gripper_angle, native scalar int8. */
gripper_angle:i8=0;
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
w.integer(<u64>this.gripper_angle,1);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response{const value=new Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response();
{
const decoded=<i8>r.integer(1);
value.gripper_angle=decoded;
}
return value;}
}
export class Ros2Ultraarm_p1_interfacesSrvGetGripperValue_ResponseCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response>{
static readonly codecIdentity:string="UWVMROS1_3b3830d2ae5b4327771923acab70739d05dd3f9c8e2b3b060f6a3136a9e6c647";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response():changetype<Ros2Ultraarm_p1_interfacesSrvGetGripperValue_Response>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/GripperStatus_Request; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field status, native scalar bool. */
status:bool=false;
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
w.u8(this.status?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request{const value=new Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.status=decoded;
}
return value;}
}
export class Ros2Ultraarm_p1_interfacesSrvGripperStatus_RequestCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request>{
static readonly codecIdentity:string="UWVMROS1_cdaff7754e24b6910102060d2af379d4bcc9c3a647604ccdf2dc8b4a50d854b8";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request():changetype<Ros2Ultraarm_p1_interfacesSrvGripperStatus_Request>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/GripperStatus_Response; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response{const value=new Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Ultraarm_p1_interfacesSrvGripperStatus_ResponseCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response>{
static readonly codecIdentity:string="UWVMROS1_55826c68885c7382e5a9e8a0ec35b312af3b2e974236d80518564c3fb8b9fa48";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response():changetype<Ros2Ultraarm_p1_interfacesSrvGripperStatus_Response>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/SetAngles_Request; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvSetAngles_Request {
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
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvSetAngles_Request{const value=new Ros2Ultraarm_p1_interfacesSrvSetAngles_Request();
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
export class Ros2Ultraarm_p1_interfacesSrvSetAngles_RequestCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvSetAngles_Request>{
static readonly codecIdentity:string="UWVMROS1_4184d40dd5cef87f13e913d349e37b0661905c5c9f0d8c5f9cc965ea272be025";
static readonly maxWireBytes:i32=20;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvSetAngles_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvSetAngles_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvSetAngles_Request>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvSetAngles_Request>();if(bytes.length>20){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvSetAngles_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvSetAngles_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvSetAngles_Request():changetype<Ros2Ultraarm_p1_interfacesSrvSetAngles_Request>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/SetAngles_Response; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvSetAngles_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvSetAngles_Response{const value=new Ros2Ultraarm_p1_interfacesSrvSetAngles_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Ultraarm_p1_interfacesSrvSetAngles_ResponseCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvSetAngles_Response>{
static readonly codecIdentity:string="UWVMROS1_de517cd68c164d4214aed8ad92f7bb9ee49f24c7e43d96c18e193095b0b23e1a";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvSetAngles_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvSetAngles_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvSetAngles_Response>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvSetAngles_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvSetAngles_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvSetAngles_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvSetAngles_Response():changetype<Ros2Ultraarm_p1_interfacesSrvSetAngles_Response>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/SetCoords_Request; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvSetCoords_Request {
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
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvSetCoords_Request{const value=new Ros2Ultraarm_p1_interfacesSrvSetCoords_Request();
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
export class Ros2Ultraarm_p1_interfacesSrvSetCoords_RequestCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvSetCoords_Request>{
static readonly codecIdentity:string="UWVMROS1_df9914ced1ef5f92f6102595de772ed076be380360b8fd5732cf7f0d4c0ec3e3";
static readonly maxWireBytes:i32=20;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvSetCoords_Request):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvSetCoords_Request):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvSetCoords_Request>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvSetCoords_Request>();if(bytes.length>20){out.status=90;return out;}if(bytes.length<20){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvSetCoords_Request.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvSetCoords_Request{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvSetCoords_Request():changetype<Ros2Ultraarm_p1_interfacesSrvSetCoords_Request>(out.value);}
}
/** Complete original ultraarm_p1_interfaces/srv/SetCoords_Response; spellings are published in generation proof. */
export class Ros2Ultraarm_p1_interfacesSrvSetCoords_Response {
static readonly maxWireBytes:i32=1;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=1;
/** Exact native field flag, native scalar bool. */
flag:bool=false;
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
w.u8(this.flag?1:0);
}
/** Internal shared reader. Only decodeChecked publishes accepted values. */
static readControl(r:Reader):Ros2Ultraarm_p1_interfacesSrvSetCoords_Response{const value=new Ros2Ultraarm_p1_interfacesSrvSetCoords_Response();
{
const bit=r.u8();if(bit>1)r.valid=false;const decoded=bit!=0;
value.flag=decoded;
}
return value;}
}
export class Ros2Ultraarm_p1_interfacesSrvSetCoords_ResponseCodec implements VendorCodec<Ros2Ultraarm_p1_interfacesSrvSetCoords_Response>{
static readonly codecIdentity:string="UWVMROS1_c66cfcc79da5bbc9a19b077761cfd3e89b3dae56ae604205de6316aa37935d7f";
static readonly maxWireBytes:i32=1;
encodeChecked(value:Ros2Ultraarm_p1_interfacesSrvSetCoords_Response):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Ultraarm_p1_interfacesSrvSetCoords_Response):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Ultraarm_p1_interfacesSrvSetCoords_Response>{const out=new VendorDecoded<Ros2Ultraarm_p1_interfacesSrvSetCoords_Response>();if(bytes.length>1){out.status=90;return out;}if(bytes.length<1){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Ultraarm_p1_interfacesSrvSetCoords_Response.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Ultraarm_p1_interfacesSrvSetCoords_Response{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Ultraarm_p1_interfacesSrvSetCoords_Response():changetype<Ros2Ultraarm_p1_interfacesSrvSetCoords_Response>(out.value);}
}
