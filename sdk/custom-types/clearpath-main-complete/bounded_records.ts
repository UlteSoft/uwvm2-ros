/** Complete industrial canonical records. No I/O or authority. */
import {Codec,Reader,validUnicode,validUtf8} from "../../assemblyscript/codecs";
export class VendorEncoded {constructor(public status:u32=0,public bytes:Uint8Array=new Uint8Array(0)){} }
export class VendorDecoded<T> {value:T|null=null;constructor(public status:u32=0){} }
export interface VendorCodec<T> {encode(value:T):Uint8Array;valid(bytes:Uint8Array):bool;decode(bytes:Uint8Array):T;encodeChecked(value:T):VendorEncoded;decodeChecked(bytes:Uint8Array):VendorDecoded<T>;}
/** One exact-size private linear buffer; finish refuses any unexpected extent. */
class VendorWriter {private b:Uint8Array;private at:i32=0;private good:bool=true;constructor(n:i32){if(n<0||n>32768){this.good=false;n=0;}this.b=new Uint8Array(n);}u8(v:u8):void{if(!this.good||this.at>=this.b.length){this.good=false;return;}this.b[this.at++]=v;}integer(v:u64,n:i32):void{if(!this.good||n<0||n>8||n>this.b.length-this.at){this.good=false;return;}for(let i=0;i<n;++i)this.b[this.at++]=<u8>(v>>(i*8));}u32(v:u32):void{this.integer(v,4);}blob(v:Uint8Array):void{this.u32(<u32>v.length);if(!this.good||v.length>this.b.length-this.at){this.good=false;return;}this.b.set(v,this.at);this.at+=v.length;}text(v:string):void{this.blob(Uint8Array.wrap(String.UTF8.encode(v)));}finish():VendorEncoded{return this.good&&this.at==this.b.length?new VendorEncoded(0,this.b):new VendorEncoded(22);}}
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
static readonly codecIdentity:string="UWVMROS1_addc15a9b6db4f7a0eba3545f02ece0800e48796f71e387b4d249af861f95317";
static readonly maxWireBytes:i32=6;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgFans):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgFans):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgFans>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgFans>();if(bytes.length>6){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgFans.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgFans{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgFans():changetype<Ros2Clearpath_platform_msgsMsgFans>(out.value);}
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
static readonly codecIdentity:string="UWVMROS1_14da254234c9c897e39bf4ee0db8faad0798d23fe7017195f35f88fb6e0b3332";
static readonly maxWireBytes:i32=10;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgLights):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgLights):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgLights>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgLights>();if(bytes.length>10){out.status=90;return out;}if(bytes.length<4){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgLights.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgLights{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgLights():changetype<Ros2Clearpath_platform_msgsMsgLights>(out.value);}
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
static readonly codecIdentity:string="UWVMROS1_e97ba89dea149ff43d0134038985ea45d2f072af592ea128aa5502a053478d22";
static readonly maxWireBytes:i32=169;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgPower):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgPower):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgPower>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgPower>();if(bytes.length>169){out.status=90;return out;}if(bytes.length<25){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgPower.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
valid(bytes:Uint8Array):bool{return this.decodeChecked(bytes).status==0;}
decode(bytes:Uint8Array):Ros2Clearpath_platform_msgsMsgPower{const out=this.decodeChecked(bytes);return out.value===null?new Ros2Clearpath_platform_msgsMsgPower():changetype<Ros2Clearpath_platform_msgsMsgPower>(out.value);}
}
/** Complete original clearpath_platform_msgs/msg/Status; spellings are published in generation proof. */
export class Ros2Clearpath_platform_msgsMsgStatus {
static readonly maxWireBytes:i32=684;
static readonly aggregateWireBytes:i32=0;
static readonly cartesianWireBytes:u64=684;
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
/** Exact native field pcb_temperature, native scalar float32. */
pcb_temperature:f32=0;
/** Exact native field mcu_temperature, native scalar float32. */
mcu_temperature:f32=0;
/** Exact active logical bytes or negative errno; no cartesian allocation. */
wireSize():i32 {let n:i32=0;
{
const child=this.header.wireSize();if(child<0)return child;if(child>684-n)return -90;n+=child;
if(n>684)return -90;
}
{
if(!validUnicode(this.hardware_id)||this.hardware_id.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.hardware_id);if(size>256)return -22;if(4+size>684-n)return -90;n+=4+size;
if(n>684)return -90;
}
{
if(!validUnicode(this.firmware_version)||this.firmware_version.indexOf("\0")>=0)return -22;const size=String.UTF8.byteLength(this.firmware_version);if(size>256)return -22;if(4+size>684-n)return -90;n+=4+size;
if(n>684)return -90;
}
{
const child=this.mcu_uptime.wireSize();if(child<0)return child;if(child>684-n)return -90;n+=child;
if(n>684)return -90;
}
{
const child=this.connection_uptime.wireSize();if(child<0)return child;if(child>684-n)return -90;n+=child;
if(n>684)return -90;
}
{
if(!isFinite(this.pcb_temperature))return -22;
if(4>684-n)return -90;n+=4;
if(n>684)return -90;
}
{
if(!isFinite(this.mcu_temperature))return -22;
if(4>684-n)return -90;n+=4;
if(n>684)return -90;
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
w.integer(<u64>reinterpret<u32>(this.pcb_temperature),4);
w.integer(<u64>reinterpret<u32>(this.mcu_temperature),4);
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
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.pcb_temperature=decoded;
}
{
const decoded=reinterpret<f32>(<u32>r.integer(4));if(!isFinite(decoded))r.valid=false;
value.mcu_temperature=decoded;
}
return value;}
}
export class Ros2Clearpath_platform_msgsMsgStatusCodec implements VendorCodec<Ros2Clearpath_platform_msgsMsgStatus>{
static readonly codecIdentity:string="UWVMROS1_331afb70728e70e0391da1b2a0f2b97fdc8742a363a2b007606121ad778dd5c1";
static readonly maxWireBytes:i32=684;
encodeChecked(value:Ros2Clearpath_platform_msgsMsgStatus):VendorEncoded{const n=value.wireSize();if(n<0)return new VendorEncoded(<u32>(-n));const w=new VendorWriter(n);value.writeControl(w);return w.finish();}
encode(value:Ros2Clearpath_platform_msgsMsgStatus):Uint8Array{return this.encodeChecked(value).bytes;}
decodeChecked(bytes:Uint8Array):VendorDecoded<Ros2Clearpath_platform_msgsMsgStatus>{const out=new VendorDecoded<Ros2Clearpath_platform_msgsMsgStatus>();if(bytes.length>684){out.status=90;return out;}if(bytes.length<44){out.status=22;return out;}const reader=new Reader(bytes),value=Ros2Clearpath_platform_msgsMsgStatus.readControl(reader);if(!reader.done()){out.status=22;return out;}const status=value.statusValue();if(status){out.status=status;return out;}out.value=value;return out;}
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
